#include <stdio.h>
#include "esp_log.h"
#include "espnow/espnow.h"
#include "nvs_flash.h"

#include "gui/gui.h"
#include "freertos/FreeRTOS.h"
#include "sdcard/sdcard_interface.h"

static const char TAG[] = "MAIN";

void app_main(void)
{

ESP_LOGI("Test " , "Henlo");

esp_err_t ret = nvs_flash_init();
if(ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND){
    ESP_ERROR_CHECK(nvs_flash_erase());
    ESP_ERROR_CHECK(nvs_flash_init());
}


vTaskDelay(1000 / portTICK_PERIOD_MS);
init_espnow_task();

//waveshare_esp32_s3_rgb_lcd_init(800 , 480);
//lcd_backlight_on();
ESP_LOGI(TAG , "Init gui");
init_gui();
ESP_LOGI(TAG , "Init sdcard");
init_sdcard();
read_model_setup();

while (1)
{
    vTaskDelay(1000);
}


}
