#ifndef _WAVESHARE_BSP_H_
#define _WAVESHARE_BSP_H_

#include "stdint.h"
#include "freertos/FreeRTOS.h"
#include "driver/i2c_master.h"
#include "driver/gpio.h"

#define I2C_MASTER_NUM              0       /*!< I2C master i2c port number, the number of i2c peripheral interfaces available will depend on the chip */
#define I2C_MASTER_SCL_IO           9       /*!< GPIO number used for I2C master clock */
#define I2C_MASTER_SDA_IO           8       /*!< GPIO number used for I2C master data  */

#define GPIO_TOUCH_INTR_PIN    4
#define GPIO_TOUCH_INTR_MASK  1ULL<<GPIO_TOUCH_INTR_PIN

#define CH422G_WR_SET_ADDR  0x48 >> 1
#define CH422G_WR_OC_ADDR   0x46 >> 1
#define CH422G_WR_IO_ADDR   0x70 >> 1
#define CH422G_RD_IO_ADDR   0x4D >> 1
#define CH422G_I2C_SPEED    400000
#define CH422G_I2C_TIMEOUT  1000

#define CH422G_IO_OE        0x01

typedef struct {
    uint8_t :1; //IO0
    uint8_t touch_reset : 1; // IO1
    uint8_t backlight : 1; // IO2
    uint8_t lcd_reset : 1; // IO3
    uint8_t sd_select : 1 ; // IO4
    uint8_t can_select : 1; // IO5
    uint8_t :1; //IO6
    uint8_t :1; // IO7
}t_extended_pins;


i2c_master_bus_handle_t init_i2c(void);
void init_bsp(i2c_master_bus_handle_t i2c_handle);


void set_lcd(bool state);
void set_touch(bool state);
void set_can_enable(bool state);
void set_backlight(bool state);

#endif