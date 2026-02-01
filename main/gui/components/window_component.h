//
// Created by Sam Weffling on 01/01/2026.
//

#ifndef LVGL_WINDOW_COMPONENT_H
#define LVGL_WINDOW_COMPONENT_H
#include "gui/gui.h"

lv_obj_t* create_window_component(lv_obj_t* parent , const char* title , close_window_cb_t close_cb);

#endif //LVGL_WINDOW_COMPONENT_H