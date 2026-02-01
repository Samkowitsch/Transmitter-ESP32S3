/*
 * SPDX-FileCopyrightText: 2022 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: CC0-1.0
 */

#include "rgb_interface.h"
#include "esp_lcd_touch_gt911.h"
#include "lvgl_port.h"

static const char TAG[] = "RGB";

// VSYNC event callback function
IRAM_ATTR static bool rgb_lcd_on_vsync_event(esp_lcd_panel_handle_t panel, const esp_lcd_rgb_panel_event_data_t *edata, void *user_ctx)
{
    return notify_vsync_event();
}

IRAM_ATTR static bool rgb_lcd_on_bounce_event(esp_lcd_panel_handle_t panel, void *bounce_buf, int pos_px, int len_bytes, void *user_ctx)
{
    return notify_vsync_event();
}

IRAM_ATTR static bool on_frame_buf_complete_cb(esp_lcd_panel_handle_t panel, const esp_lcd_rgb_panel_event_data_t *edata, void *user_ctx){
    return notify_frame_buf_complete_event();
}



// Initialize RGB LCD
esp_lcd_panel_handle_t init_lcd_panel(int32_t width , int32_t heigth)
{
    ESP_LOGI(TAG, "Install RGB LCD panel driver"); // Log the start of the RGB LCD panel driver installation
    esp_lcd_panel_handle_t panel_handle = NULL; // Declare a handle for the LCD panel
    esp_lcd_rgb_panel_config_t panel_config = {
        .clk_src = LCD_CLK_SRC_DEFAULT, // Set the clock source for the panel
        .timings =  {
            .pclk_hz = LCD_PIXEL_CLOCK_HZ, // Pixel clock frequency
            .h_res = width, // Horizontal resolution
            .v_res = heigth, // Vertical resolution
            .hsync_pulse_width = 4, // Horizontal sync pulse width
            .hsync_back_porch = 8, // Horizontal back porch
            .hsync_front_porch = 8, // Horizontal front porch
            .vsync_pulse_width = 4, // Vertical sync pulse width
            .vsync_back_porch = 8, // Vertical back porch
            .vsync_front_porch = 8, // Vertical front porch
            .flags = {
                .pclk_active_neg = 1, // Active low pixel clock
            },
        },
        .data_width = LCD_BIT_PER_PIXEL, // Data width for RGB
        .num_fbs = LCD_NUM_FRAMEBUFFER, // Number of frame buffers
        .bounce_buffer_size_px = LCD_BOUNCE_BUFFER_SIZE, // Bounce buffer size in pixels
        //.sram_trans_align = 4, // SRAM transaction alignment
        //.psram_trans_align = 64, // PSRAM transaction alignment
        .hsync_gpio_num = LCD_IO_RGB_HSYNC, // GPIO number for horizontal sync
        .vsync_gpio_num = LCD_IO_RGB_VSYNC, // GPIO number for vertical sync
        .de_gpio_num = LCD_IO_RGB_DE, // GPIO number for data enable
        .pclk_gpio_num = LCD_IO_RGB_PCLK, // GPIO number for pixel clock
        .disp_gpio_num = LCD_IO_RGB_DISP, // GPIO number for display
        .data_gpio_nums = {
            LCD_IO_RGB_DATA0,
            LCD_IO_RGB_DATA1,
            LCD_IO_RGB_DATA2,
            LCD_IO_RGB_DATA3,
            LCD_IO_RGB_DATA4,
            LCD_IO_RGB_DATA5,
            LCD_IO_RGB_DATA6,
            LCD_IO_RGB_DATA7,
            LCD_IO_RGB_DATA8,
            LCD_IO_RGB_DATA9,
            LCD_IO_RGB_DATA10,
            LCD_IO_RGB_DATA11,
            LCD_IO_RGB_DATA12,
            LCD_IO_RGB_DATA13,
            LCD_IO_RGB_DATA14,
            LCD_IO_RGB_DATA15,
        },
        .flags = {
            .fb_in_psram = 1, // Use PSRAM for framebuffer
            //.refresh_on_demand = 1,
        },
    };

    // Create a new RGB panel with the specified configuration
    ESP_ERROR_CHECK(esp_lcd_new_rgb_panel(&panel_config, &panel_handle));

    ESP_LOGI(TAG, "Initialize RGB LCD panel"); // Log the initialization of the RGB LCD panel
    ESP_ERROR_CHECK(esp_lcd_panel_init(panel_handle)); // Initialize the LCD panel

    // Register callbacks for RGB panel events
    esp_lcd_rgb_panel_event_callbacks_t cbs = {
        .on_vsync = rgb_lcd_on_vsync_event, // Callback for vertical sync
        //.on_frame_buf_complete = on_frame_buf_complete_cb, // Callback for bounce frame finish
        //.on_color_trans_done = on_frame_buf_complete_cb

    };
    ESP_ERROR_CHECK(esp_lcd_rgb_panel_register_event_callbacks(panel_handle, &cbs, NULL)); // Register event callbacks

    return panel_handle; // Return success 
}



esp_lcd_touch_handle_t init_touch(i2c_master_bus_handle_t i2c_handle , int32_t width , int32_t heigth){

    esp_lcd_touch_handle_t tp_handle = NULL; // Declare a handle for the touch panel
    esp_lcd_panel_io_handle_t tp_io_handle = NULL; // Declare a handle for touch panel I/O
    esp_lcd_panel_io_i2c_config_t tp_io_config = ESP_LCD_TOUCH_IO_I2C_GT911_CONFIG(); // Configure I2C for GT911 touch controller
    tp_io_config.scl_speed_hz = 400000;

    ESP_LOGI(TAG, "Initialize I2C panel IO"); // Log I2C panel I/O initialization

    ESP_ERROR_CHECK(esp_lcd_new_panel_io_i2c(i2c_handle , &tp_io_config , &tp_io_handle));

    ESP_LOGI(TAG, "Initialize touch controller GT911"); // Log touch controller initialization
    const esp_lcd_touch_config_t tp_cfg = {
        .x_max = width, // Set maximum X coordinate
        .y_max = heigth, // Set maximum Y coordinate
        .rst_gpio_num = PIN_NUM_TOUCH_RST, // GPIO number for reset
        .int_gpio_num = PIN_NUM_TOUCH_INT, // GPIO number for interrupt
        .levels = {
            .reset = 0, // Reset level
            .interrupt = 0, // Interrupt level
        },
        .flags = {
            .swap_xy = 0, // No swap of X and Y
            .mirror_x = 0, // No mirroring of X
            .mirror_y = 0, // No mirroring of Y
        },
    };
    ESP_ERROR_CHECK(esp_lcd_touch_new_i2c_gt911(tp_io_handle, &tp_cfg, &tp_handle)); // Create new I2C GT911 touch controller

    return tp_handle;
}