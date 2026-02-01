#ifndef _LVGL_PORT_H_
#define _LVGL_PORT_H_

#include "lvgl.h"
#include "lvgl_private.h"
#include "esp_timer.h"
#include "freertos/FreeRTOS.h"

#define LVGL_PORT_TICK_PERIOD_MS 50

#define LVGL_PORT_TASK_MAX_DELAY_MS 500    // The maximum delay of the LVGL timer task, in milliseconds
#define LVGL_PORT_TASK_MIN_DELAY_MS 10
#define EXAMPLE_LVGL_PORT_ROTATION_DEGREE 0


// Structure to store information about dirty areas that need refreshing
typedef struct {
    uint16_t inv_p;                                   // Number of invalid areas
    uint8_t inv_area_joined[LV_INV_BUF_SIZE];       // Array to track joined invalid areas
    lv_area_t inv_areas[LV_INV_BUF_SIZE];            // Array of invalid areas
} lv_port_dirty_area_t;

// Enumeration for flush status
typedef enum {
    FLUSH_STATUS_PART,                                // Partial flush
    FLUSH_STATUS_FULL                                 // Full flush
} lv_port_flush_status_t;

// Enumeration for flush probe results
typedef enum {
    FLUSH_PROBE_PART_COPY,                           // Probe result for partial copy
    FLUSH_PROBE_SKIP_COPY,                           // Probe result to skip copy
    FLUSH_PROBE_FULL_COPY,                           // Probe result for full copy
} lv_port_flush_probe_t;

bool notify_vsync_event();
bool notify_frame_buf_complete_event();
esp_err_t lvgl_port_init(int32_t width , int32_t heigth);

#endif