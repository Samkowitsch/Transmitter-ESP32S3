//
// Created by Sam Weffling on 01/01/2026.
//

#ifndef LVGL_FLEX_COMPONENTS_H
#define LVGL_FLEX_COMPONENTS_H

#include "gui/gui.h"


lv_obj_t* create_flex_row(lv_obj_t* parent , int height);

lv_obj_t* create_flex_spacer(lv_obj_t* parent , bool vertical);
#endif //LVGL_FLEX_COMPONENTS_H