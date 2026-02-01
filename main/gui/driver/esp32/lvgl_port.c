#include "lvgl_port.h"
#include "rgb_interface.h"
#include "waveshare_bsp.h"
#include "lvgl_private.h"

#define LVGL_PORT_TASK_STACK_SIZE   10 * 1024
#define LVGL_PORT_TASK_PRIORITY     5
static TaskHandle_t lvgl_task_handle;

static lv_port_dirty_area_t dirty_area;             // Instance of dirty area structure

static const char TAG[] = "PORT";


bool notify_vsync_event(){
    BaseType_t need_yield = pdTRUE; // Flag to check if a yield is needed
    // Notify that the current RGB frame buffer has been transmitted
    if(lvgl_task_handle){
        xTaskNotifyFromISR(lvgl_task_handle, ULONG_MAX, eNoAction, &need_yield); // Notify the LVGL task
    }
    return (need_yield == pdTRUE); // Return whether a yield is needed
}

bool notify_frame_buf_complete_event(){
    BaseType_t need_yield = pdTRUE; // Flag to check if a yield is needed

    if(lvgl_task_handle){
            //ESP_LOGI("ITR" , "FB");

        xTaskNotifyFromISR(lvgl_task_handle, ULONG_MAX, eNoAction, &need_yield); // Notify the LVGL task
    }
    return (need_yield == pdTRUE); // Return whether a yield is needed
}

static void tick_increment(void *arg)
{
    /* Tell LVGL how many milliseconds have elapsed */
    lv_tick_inc(LVGL_PORT_TICK_PERIOD_MS); // Increment the LVGL tick count
}


esp_err_t lvgl_tick_init(){
        // Tick interface for LVGL (using esp_timer to generate 2ms periodic event)
    const esp_timer_create_args_t lvgl_tick_timer_args = {
        .callback = &tick_increment, // Set the callback function for the timer
        .name = "LVGL tick" // Name of the timer
    };
    esp_timer_handle_t lvgl_tick_timer = NULL; // Timer handle
    ESP_ERROR_CHECK(esp_timer_create(&lvgl_tick_timer_args, &lvgl_tick_timer)); // Create the timer
    return esp_timer_start_periodic(lvgl_tick_timer, LVGL_PORT_TICK_PERIOD_MS * 1000); // Start the timer
}






static void *get_next_frame_buffer(esp_lcd_panel_handle_t panel_handle)
{
    static void *next_fb = NULL;                          // Pointer to the next frame buffer
    static void *fb[2] = { NULL };                        // Array to hold two frame buffers
    if (next_fb == NULL) {
        ESP_ERROR_CHECK(esp_lcd_rgb_panel_get_frame_buffer(panel_handle, 2, &fb[0], &fb[1])); // Get the frame buffers
        next_fb = fb[1];                                  // Initialize to the second buffer
    } else {
        // Toggle between the two frame buffers
        next_fb = (next_fb == fb[0]) ? fb[1] : fb[0];
    }
    return next_fb;                                       // Return the next frame buffer
}

#if 0
// Function to rotate and copy pixels from one buffer to another
IRAM_ATTR static void rotate_copy_pixel(const uint16_t *from, uint16_t *to, uint16_t x_start, uint16_t y_start, uint16_t x_end, uint16_t y_end, uint16_t w, uint16_t h, uint16_t rotation)
{
    int from_index = 0;                                   // Index for source buffer
    int to_index = 0;                                     // Index for destination buffer
    int to_index_const = 0;                               // Constant index for destination buffer

    switch (rotation) {
    case 90:
        to_index_const = (w - x_start - 1) * h;          // Calculate constant index for 90-degree rotation
        for (int from_y = y_start; from_y < y_end + 1; from_y++) {
            from_index = from_y * w + x_start;           // Calculate index in the source buffer
            to_index = to_index_const + from_y;          // Calculate index in the destination buffer
            for (int from_x = x_start; from_x < x_end + 1; from_x++) {
                *(to + to_index) = *(from + from_index);  // Copy pixel
                from_index += 1;                          // Move to the next pixel in the source
                to_index -= h;                            // Move to the next pixel in the destination
            }
        }
        break;
    case 180:
        to_index_const = h * w - x_start - 1;            // Calculate constant index for 180-degree rotation
        for (int from_y = y_start; from_y < y_end + 1; from_y++) {
            from_index = from_y * w + x_start;           // Calculate index in the source buffer
            to_index = to_index_const - from_y * w;      // Calculate index in the destination buffer
            for (int from_x = x_start; from_x < x_end + 1; from_x++) {
                *(to + to_index) = *(from + from_index);  // Copy pixel
                from_index += 1;                          // Move to the next pixel in the source
                to_index -= 1;                            // Move to the next pixel in the destination
            }
        }
        break;
    case 270:
        to_index_const = (x_start + 1) * h - 1;          // Calculate constant index for 270-degree rotation
        for (int from_y = y_start; from_y < y_end + 1; from_y++) {
            from_index = from_y * w + x_start;           // Calculate index in the source buffer
            to_index = to_index_const - from_y;          // Calculate index in the destination buffer
            for (int from_x = x_start; from_x < x_end + 1; from_x++) {
                *(to + to_index) = *(from + from_index);  // Copy pixel
                from_index += 1;                          // Move to the next pixel in the source
                to_index += h;                            // Move to the next pixel in the destination
            }
        }
        break;
    default:
        break;                                             // Do nothing for unsupported rotation angles
    }
}

// Function to save the current dirty area information
static void flush_dirty_save(lv_port_dirty_area_t *dirty_area)
{
    
    lv_display_t *disp = lv_refr_get_disp_refreshing(); // Get the currently refreshing display
    dirty_area->inv_p = disp->inv_p;                  // Save the number of invalid areas
    for (int i = 0; i < disp->inv_p; i++) {
        dirty_area->inv_area_joined[i] = disp->inv_area_joined[i]; // Save joined areas status
        dirty_area->inv_areas[i] = disp->inv_areas[i]; // Save invalid areas
    }
}

/**
 * @brief Probe dirty area to copy
 *
 * @note This function is used to avoid tearing effect, and only works with LVGL direct mode.
 *
 */
static lv_port_flush_probe_t flush_copy_probe(lv_display_t *drv)
{   
    static lv_port_flush_status_t prev_status = FLUSH_STATUS_PART; // Previous flush status
    lv_port_flush_status_t cur_status;                            // Current flush status
    lv_port_flush_probe_t probe_result;                           // Result of the probe
    lv_display_t *disp_refr = lv_refr_get_disp_refreshing();       // Get the currently refreshing display

    uint32_t flush_ver = 0;                                       // Vertical size to flush
    uint32_t flush_hor = 0;                                       // Horizontal size to flush
    for (int i = 0; i < disp_refr->inv_p; i++) {
        if (disp_refr->inv_area_joined[i] == 0) {
            flush_ver = (disp_refr->inv_areas[i].y2 + 1 - disp_refr->inv_areas[i].y1); // Calculate vertical flush size
            flush_hor = (disp_refr->inv_areas[i].x2 + 1 - disp_refr->inv_areas[i].x1); // Calculate horizontal flush size
            break; // Exit the loop after finding the first unjoined area
        }
    }
    /* Check if the current full screen refreshes */
    cur_status = ((flush_ver == drv->ver_res) && (flush_hor == drv->hor_res)) ? (FLUSH_STATUS_FULL) : (FLUSH_STATUS_PART);

    // Determine the probe result based on previous and current status
    if (prev_status == FLUSH_STATUS_FULL) {
        if ((cur_status == FLUSH_STATUS_PART)) {
            probe_result = FLUSH_PROBE_FULL_COPY; // Full copy needed
        } else {
            probe_result = FLUSH_PROBE_SKIP_COPY; // Skip copy
        }
    } else {
        probe_result = FLUSH_PROBE_PART_COPY; // Partial copy needed
    }
    prev_status = cur_status; // Update previous status

    return probe_result; // Return the result of the probe
}

// Inline function to get the next buffer for flushing
static inline void *flush_get_next_buf(void *panel_handle)
{
    return get_next_frame_buffer(panel_handle); // Return the next frame buffer
}

/**
 * @brief Copy dirty area
 *
 * @note This function is used to avoid tearing effect, and only works with LVGL direct mode.
 *
 */
static void flush_dirty_copy(void *dst, void *src, lv_port_dirty_area_t *dirty_area)
{
    lv_coord_t x_start, x_end, y_start, y_end; // Coordinates for the area to be copied
    for (int i = 0; i < dirty_area->inv_p; i++) {
        /* Refresh the unjoined areas */
        if (dirty_area->inv_area_joined[i] == 0) {
            x_start = dirty_area->inv_areas[i].x1; // Start X coordinate
            x_end = dirty_area->inv_areas[i].x2;   // End X coordinate
            y_start = dirty_area->inv_areas[i].y1; // Start Y coordinate
            y_end = dirty_area->inv_areas[i].y2;   // End Y coordinate

            // Rotate and copy pixel data from source to destination buffer
            rotate_copy_pixel(src, dst, x_start, y_start, x_end, y_end, LV_HOR_RES, LV_VER_RES, EXAMPLE_LVGL_PORT_ROTATION_DEGREE);
        }
    }
}


static void flush_callback(lv_display_t *drv, const lv_area_t *area, lv_color_t *color_map)
{

    esp_lcd_panel_handle_t panel_handle = (esp_lcd_panel_handle_t) drv->user_data; // Get the panel handle from driver user data
    const int offsetx1 = area->x1; // Start X coordinate of the area to flush
    const int offsetx2 = area->x2; // End X coordinate of the area to flush
    const int offsety1 = area->y1; // Start Y coordinate of the area to flush
    const int offsety2 = area->y2; // End Y coordinate of the area to flush
    void *next_fb = NULL; // Pointer for the next frame buffer
    lv_port_flush_probe_t probe_result = FLUSH_PROBE_PART_COPY; // Default probe result
    lv_disp_t *disp = lv_disp_get_default(); // Get the default display

    /* Action after last area refresh */
    if (lv_disp_flush_is_last(drv)) {
        /* Check if the `full_refresh` flag has been triggered */
        if (drv->full_refresh) {
            /* Reset flag */
            drv->full_refresh = 0;

            // Rotate and copy data from the whole screen LVGL's buffer to the next frame buffer
            next_fb = flush_get_next_buf(panel_handle);
            rotate_copy_pixel((uint16_t *)color_map, next_fb, offsetx1, offsety1, offsetx2, offsety2, LV_HOR_RES, LV_VER_RES, EXAMPLE_LVGL_PORT_ROTATION_DEGREE);

            /* Switch the current RGB frame buffer to `next_fb` */
            esp_lcd_panel_draw_bitmap(panel_handle, offsetx1, offsety1, offsetx2 + 1, offsety2 + 1, next_fb);

            /* Wait for the current frame buffer to complete transmission */
            ulTaskNotifyValueClear(NULL, ULONG_MAX);
            ulTaskNotifyTake(pdTRUE, portMAX_DELAY);

            /* Synchronously update the dirty area for another frame buffer */
            flush_dirty_copy(flush_get_next_buf(panel_handle), color_map, &dirty_area);
            flush_get_next_buf(panel_handle);
        } else {
            /* Probe the copy method for the current dirty area */
            probe_result = flush_copy_probe(drv);

            if (probe_result == FLUSH_PROBE_FULL_COPY) {
                /* Save current dirty area for the next frame buffer */
                flush_dirty_save(&dirty_area);

                /* Set LVGL full-refresh flag and set flush ready in advance */
                drv->full_refresh = 1; // Indicate that a full refresh is required
                disp->rendering_in_progress = false; // Mark rendering as not in progress
                lv_disp_flush_ready(drv); // Mark flush as ready

                /* Force to refresh the whole screen, will invoke `flush_callback` recursively */
                lv_refr_now(_lv_refr_get_disp_refreshing());
            } else {
                /* Update current dirty area for the next frame buffer */
                next_fb = flush_get_next_buf(panel_handle);
                flush_dirty_save(&dirty_area);
                flush_dirty_copy(next_fb, color_map, &dirty_area);

                /* Switch the current RGB frame buffer to `next_fb` */
                esp_lcd_panel_draw_bitmap(panel_handle, offsetx1, offsety1, offsetx2 + 1, offsety2 + 1, next_fb);

                /* Wait for the current frame buffer to complete transmission */
                ulTaskNotifyValueClear(NULL, ULONG_MAX);
                ulTaskNotifyTake(pdTRUE, portMAX_DELAY);

                if (probe_result == FLUSH_PROBE_PART_COPY) {
                    /* Synchronously update the dirty area for another frame buffer */
                    flush_dirty_save(&dirty_area);
                    flush_dirty_copy(flush_get_next_buf(panel_handle), color_map, &dirty_area);
                    flush_get_next_buf(panel_handle);
                }
            }
        }
    }

    lv_disp_flush_ready(drv); // Mark the display flush as complete
}

#endif 

#if 1
static void display_flush_callback(lv_display_t *drv, const lv_area_t *area, uint8_t *px_map)
{
    esp_lcd_panel_handle_t panel_handle = (esp_lcd_panel_handle_t) drv->user_data; // Get the panel handle from driver user data
    const int offsetx1 = area->x1; // Start X coordinate of the area to flush
    const int offsetx2 = area->x2; // End X coordinate of the area to flush
    const int offsety1 = area->y1; // Start Y coordinate of the area to flush
    const int offsety2 = area->y2; // End Y coordinate of the area to flush

    //ESP_LOGI("FLUSH" , "Addr 0x%x x:%ld y:%ld" , px_map , offsetx1 , offsety1);

    /* Action after last area refresh */
    if (lv_display_flush_is_last(drv)) {
        /* Switch the current RGB frame buffer to `color_map` */
        ulTaskNotifyValueClear(NULL, ULONG_MAX);
        esp_lcd_panel_draw_bitmap(panel_handle, offsetx1, offsety1, offsetx2 + 1, offsety2 + 1, px_map);

        /* Wait for the last frame buffer to complete transmission */
        ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
        //esp_lcd_rgb_panel_refresh(panel_handle);
    }

    lv_display_flush_ready(drv); // Mark the display flush as complete
}

#else

void display_flush_callback(lv_display_t * display, const lv_area_t * area, uint8_t * px_map){

    esp_lcd_panel_handle_t panel_handle = (esp_lcd_panel_handle_t) lv_display_get_user_data(display); // Get the panel handle from driver user data
    
    const int offsetx1 = area->x1; // Start X coordinate of the area to flush
    const int offsetx2 = area->x2; // End X coordinate of the area to flush
    const int offsety1 = area->y1; // Start Y coordinate of the area to flush
    const int offsety2 = area->y2; // End Y coordinate of the area to flush

    /* Just copy data from the color map to the RGB frame buffer */
    
    ulTaskNotifyValueClear(NULL, ULONG_MAX);
    esp_lcd_panel_draw_bitmap(panel_handle, offsetx1 , offsety1, offsetx2 + 1 , offsety2 + 1, px_map);
    //ESP_LOGI("FLUSH" , "Wait");
    ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
    lv_display_flush_ready(display);

}

#endif

static void touchpad_read(lv_indev_t *indev, lv_indev_data_t *data)
{
    esp_lcd_touch_handle_t tp = lv_indev_get_user_data(indev); // Get touchpad handle from user data
    assert(tp); // Ensure touchpad handle is valid

    uint16_t touchpad_x; // Variable for X coordinate
    uint16_t touchpad_y; // Variable for Y coordinate
    uint8_t touchpad_cnt = 0; // Variable for touch count

    /* Read data from touch controller into memory */
    esp_lcd_touch_read_data(tp); // Read data from touch controller

    /* Read data from touch controller */
    bool touchpad_pressed = esp_lcd_touch_get_coordinates(tp, &touchpad_x, &touchpad_y, NULL, &touchpad_cnt, 1); // Get touch coordinates
    if (touchpad_pressed && touchpad_cnt > 0) {
        data->point.x = touchpad_x; // Set the X coordinate
        data->point.y = touchpad_y; // Set the Y coordinate
        data->state = LV_INDEV_STATE_PRESSED; // Set state to pressed
        ESP_LOGD(TAG, "Touch position: %d,%d", touchpad_x, touchpad_y); // Log touch position
    } else {
        data->state = LV_INDEV_STATE_RELEASED; // Set state to released
    }
}


static lv_disp_t *display_init(esp_lcd_panel_handle_t panel_handle , int32_t w , int32_t h){

    ESP_LOGI(TAG , "Create display");
    lv_display_t * display = lv_display_create(w , h);

    void *fb_1 = NULL;
    void *fb_2 = NULL;

    uint32_t buffer_size = w * h * sizeof(uint16_t);

    ESP_LOGI(TAG , "Get fb");
    ESP_ERROR_CHECK(esp_lcd_rgb_panel_get_frame_buffer(panel_handle , LCD_NUM_FRAMEBUFFER , &fb_1 , &fb_2));


    ESP_LOGI(TAG , "fb_1 0x%x , fb_2 : 0x%x" , fb_1, fb_2);

    ESP_LOGI(TAG , "Set user data"); 
    lv_display_set_user_data(display , panel_handle);
    ESP_LOGI(TAG , "Set display buffers");
    lv_display_set_buffers(display , fb_1 , fb_2 , buffer_size , LV_DISPLAY_RENDER_MODE_DIRECT);
    ESP_LOGI(TAG , "Set display flush callback");
    lv_display_set_flush_cb(display , display_flush_callback);

    return display;
}



static lv_indev_t *indev_init(esp_lcd_touch_handle_t tp_handle)
{
    assert(tp_handle); // Ensure the touch panel handle is valid

    lv_indev_t* indev = lv_indev_create(); 
    lv_indev_set_type(indev , LV_INDEV_TYPE_POINTER);
    lv_indev_set_read_cb(indev , touchpad_read);
    lv_indev_set_user_data(indev , tp_handle);

    return indev;
}





static void lvgl_port_task(void *arg)
{
    ESP_LOGD(TAG, "Starting LVGL task"); // Log the task start
    uint32_t task_delay_ms = LVGL_PORT_TASK_MAX_DELAY_MS;
    while (1) {
        task_delay_ms = lv_timer_handler();

                // Ensure the delay time is within limits
        if (task_delay_ms > LVGL_PORT_TASK_MAX_DELAY_MS) {
            task_delay_ms = LVGL_PORT_TASK_MAX_DELAY_MS;
        } else if (task_delay_ms < LVGL_PORT_TASK_MIN_DELAY_MS) {
            task_delay_ms = LVGL_PORT_TASK_MIN_DELAY_MS;
        }

        vTaskDelay(pdMS_TO_TICKS(task_delay_ms)); // Delay the task for the calculated time
    }
}



esp_err_t lvgl_port_init(int32_t width , int32_t heigth) { //, esp_lcd_touch_handle_t tp_handle){
    //ESP_ERROR_CHECK(lvgl_port_init(panel_handle , tp_handle , width ,heigth)); // Initialize LVGL with the panel and touch handles


    i2c_master_bus_handle_t i2c_handle = init_i2c(); // Initialize the I2C master
    init_bsp(i2c_handle);

    set_touch(false);
    set_lcd(false);
    vTaskDelay(pdMS_TO_TICKS(100));
    set_touch(true);
    set_lcd(true);
    set_backlight(true);

    esp_lcd_panel_handle_t lcd_handle = init_lcd_panel(width , heigth);
    esp_lcd_touch_handle_t tp_handle = init_touch(i2c_handle , width , heigth);
    
    lv_init();

    ESP_ERROR_CHECK(lvgl_tick_init()); // Initialize the tick timer

    lv_disp_t* display = display_init(lcd_handle , width ,heigth);
    assert(display);
    lv_indev_t* indev = indev_init(tp_handle);

    ESP_LOGI(TAG, "Create LVGL task"); // Log task creation
    BaseType_t ret = xTaskCreate(lvgl_port_task, "lvgl", LVGL_PORT_TASK_STACK_SIZE, NULL,
                                             LVGL_PORT_TASK_PRIORITY, &lvgl_task_handle); // Create the LVGL task
    if (ret != pdPASS) {
        ESP_LOGE(TAG, "Failed to create LVGL task"); // Log error if task creation fails
        return ESP_FAIL; // Return failure
    }

    return ESP_OK; // Return success
}
