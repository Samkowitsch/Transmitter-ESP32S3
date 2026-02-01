#ifndef _RGB_LCD_H_
#define _RGB_LCD_H_

#include "esp_log.h"
#include "esp_heap_caps.h"
#include "driver/gpio.h"
#include "driver/i2c_master.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_lcd_panel_ops.h"
#include "esp_lcd_panel_rgb.h"
#include "esp_lcd_touch_gt911.h"
#include "lv_demos.h"
//#include "lvgl_port.h"

#define LCD_NUM_FRAMEBUFFER   2

#define LCD_H_RES               (800u)
#define LCD_V_RES               (480u)
#define LCD_PIXEL_CLOCK_HZ      (16 * 1000 * 1000)
#define LCD_BIT_PER_PIXEL       (16)
#define LCD_RGB_BIT_PER_PIXEL       (16)
#define LCD_RGB_DATA_WIDTH          (16)
#define LCD_BOUNCE_BUFFER_SIZE  0 // LCD_H_RES * 20 //(LCD_H_RES * LCD_V_RES) / 40
#define LCD_IO_RGB_DISP         (-1)             // -1 if not used
#define LCD_IO_RGB_VSYNC        (GPIO_NUM_3)
#define LCD_IO_RGB_HSYNC        (GPIO_NUM_46)
#define LCD_IO_RGB_DE           (GPIO_NUM_5)
#define LCD_IO_RGB_PCLK         (GPIO_NUM_7)
#define LCD_IO_RGB_DATA0        (GPIO_NUM_14)
#define LCD_IO_RGB_DATA1        (GPIO_NUM_38)
#define LCD_IO_RGB_DATA2        (GPIO_NUM_18)
#define LCD_IO_RGB_DATA3        (GPIO_NUM_17)
#define LCD_IO_RGB_DATA4        (GPIO_NUM_10)
#define LCD_IO_RGB_DATA5        (GPIO_NUM_39)
#define LCD_IO_RGB_DATA6        (GPIO_NUM_0)
#define LCD_IO_RGB_DATA7        (GPIO_NUM_45)
#define LCD_IO_RGB_DATA8        (GPIO_NUM_48)
#define LCD_IO_RGB_DATA9        (GPIO_NUM_47)
#define LCD_IO_RGB_DATA10       (GPIO_NUM_21)
#define LCD_IO_RGB_DATA11       (GPIO_NUM_1)
#define LCD_IO_RGB_DATA12       (GPIO_NUM_2)
#define LCD_IO_RGB_DATA13       (GPIO_NUM_42)
#define LCD_IO_RGB_DATA14       (GPIO_NUM_41)
#define LCD_IO_RGB_DATA15       (GPIO_NUM_40)

#define LCD_IO_RST              (-1)             // -1 if not used
#define PIN_NUM_BK_LIGHT        (-1)    // -1 if not used
#define LCD_BK_LIGHT_ON_LEVEL   (1)
#define LCD_BK_LIGHT_OFF_LEVEL  !LCD_BK_LIGHT_ON_LEVEL

#define PIN_NUM_TOUCH_RST       (-1)            // -1 if not used
#define PIN_NUM_TOUCH_INT       (-1)            // -1 if not used


esp_lcd_panel_handle_t init_lcd_panel(int32_t width , int32_t heigth);
esp_lcd_touch_handle_t init_touch(i2c_master_bus_handle_t i2c_handle , int32_t width , int32_t heigth);


#endif