#include "waveshare_bsp.h"

#include "esp_log.h"

static i2c_master_dev_handle_t ch422g_wr_set = NULL;
static i2c_master_dev_handle_t ch422g_wr_io = NULL;
static t_extended_pins wr_io_pins = {0};

/**
 * @brief Initialise GPIOs
 */
static void init_gpio(void)
{
    ESP_LOGI("BSP", "Initialize GPIOs"); 
    // Zero-initialize the config structure
    gpio_config_t io_conf = {};
    // Disable interrupt
    io_conf.intr_type = GPIO_INTR_DISABLE;
    // Bit mask of the pins, use GPIO4 here
    io_conf.pin_bit_mask = GPIO_TOUCH_INTR_MASK;
    // Set as input mode
    io_conf.mode = GPIO_MODE_OUTPUT;
    gpio_config(&io_conf);
}


/**
 * @brief I2C master initialization
 */
i2c_master_bus_handle_t init_i2c(void){
    ESP_LOGI("BSP", "Initialize I2C bus");

    i2c_master_bus_config_t i2c_conf = {
        .clk_source = I2C_CLK_SRC_DEFAULT,
        .i2c_port = I2C_MASTER_NUM,
        .sda_io_num = I2C_MASTER_SDA_IO,
        .scl_io_num = I2C_MASTER_SCL_IO,
        .flags.enable_internal_pullup = true,
    };

    i2c_master_bus_handle_t i2c_handle;
    ESP_ERROR_CHECK(i2c_new_master_bus(&i2c_conf , &i2c_handle));
    return i2c_handle;
}


/**
 * @brief Initialise BSP for Waveshare board
 */
void init_bsp(i2c_master_bus_handle_t i2c_handle){

    init_gpio();

    i2c_device_config_t ch422g_wr_set_cfg = {
        .dev_addr_length = I2C_ADDR_BIT_LEN_7,
        .device_address = CH422G_WR_SET_ADDR,
        .scl_speed_hz = CH422G_I2C_SPEED
    };


    i2c_device_config_t ch422g_wr_io_cfg = {
        .dev_addr_length = I2C_ADDR_BIT_LEN_7,
        .device_address = CH422G_WR_IO_ADDR,
        .scl_speed_hz = CH422G_I2C_SPEED
    };

    ESP_ERROR_CHECK(i2c_master_bus_add_device(i2c_handle , &ch422g_wr_set_cfg ,  &ch422g_wr_set));
    ESP_ERROR_CHECK(i2c_master_bus_add_device(i2c_handle , &ch422g_wr_io_cfg ,  &ch422g_wr_io));

    // Enable Output of I/O Pins
    uint8_t write_buf = CH422G_IO_OE;
    ESP_ERROR_CHECK(i2c_master_transmit(ch422g_wr_set , &write_buf , 1 , CH422G_I2C_TIMEOUT / portTICK_PERIOD_MS));

    // Pull all ios down 
    ESP_ERROR_CHECK(i2c_master_transmit(ch422g_wr_io , (uint8_t*)&wr_io_pins , 1 , CH422G_I2C_TIMEOUT / portTICK_PERIOD_MS ));


}



/**
 * @brief Set lcd reset 
 */
void set_lcd(bool state){
    wr_io_pins.lcd_reset = state;
    ESP_ERROR_CHECK(i2c_master_transmit(ch422g_wr_io , (uint8_t*)&wr_io_pins , 1 , CH422G_I2C_TIMEOUT / portTICK_PERIOD_MS));
}

/**
 * @brief Set touch reset 
 */
void set_touch(bool state){

    // Reset the touch screen. It is recommended to reset the touch screen before using it.
    wr_io_pins.touch_reset = state;
    ESP_ERROR_CHECK(i2c_master_transmit(ch422g_wr_io , (uint8_t*)&wr_io_pins , 1 , CH422G_I2C_TIMEOUT / portTICK_PERIOD_MS));
}


/**
 * @brief Set Backlight state
 */
void set_backlight(bool state){

    wr_io_pins.backlight = state;
    //uint8_t write_buf = CH422G_IO_OE;
    //ESP_ERROR_CHECK(i2c_master_transmit(ch422g_wr_set , &write_buf , 1 , CH422G_I2C_TIMEOUT / portTICK_PERIOD_MS));
    ESP_ERROR_CHECK(i2c_master_transmit(ch422g_wr_io , (uint8_t*)&wr_io_pins , 1 , CH422G_I2C_TIMEOUT / portTICK_PERIOD_MS ));

}


void set_sdcard_cs(bool state){
    wr_io_pins.sd_select = state;
    ESP_ERROR_CHECK(i2c_master_transmit(ch422g_wr_io , (uint8_t*)&wr_io_pins , 1 , CH422G_I2C_TIMEOUT / portTICK_PERIOD_MS )); 
}