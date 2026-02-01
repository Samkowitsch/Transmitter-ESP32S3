//
// Created by Sam Weffling on 01/01/2026.
//

#include "flex_components.h"



lv_obj_t* create_flex_row(lv_obj_t* parent , const int height) {

    lv_obj_t* row = lv_obj_create(parent);
    lv_obj_add_style(row , &flex_row_style , LV_PART_MAIN);
    if (height) {
        lv_obj_set_height(row , height);
    }

    return row;
}



lv_obj_t* create_flex_spacer(lv_obj_t* parent , bool vertical) {


    lv_obj_t* spacer = lv_obj_create(parent);
    lv_obj_set_style_border_width(spacer , 0 , LV_PART_MAIN);
    lv_obj_set_style_bg_opa(spacer , LV_OPA_0 , LV_PART_MAIN);
    lv_obj_set_style_flex_grow(spacer , 1 , LV_PART_MAIN);

    return spacer;
}