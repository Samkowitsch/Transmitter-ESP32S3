//
// Created by Sam Weffling on 30/12/2025.
//

#include "gui.h"
#include "lvgl.h"
#include "gui/driver/esp32/lvgl_port.h"
#include "screens/1_main/main_screen.h"

void init_gui() {


    ESP_ERROR_CHECK(lvgl_port_init(800 , 480));
    init_styles();

    lv_lock();
    init_main_screen(true);

    lv_unlock();

}