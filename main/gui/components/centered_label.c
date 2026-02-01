//
// Created by Sam Weffling on 01/01/2026.
//

#include "centered_label.h"



lv_obj_t* create_centered_label(lv_obj_t* parent , const int width , const int height) {


    lv_obj_t* label_div = lv_obj_create(parent);
    lv_obj_set_size(label_div , width , height);
    lv_obj_set_style_pad_all(label_div , 0 , LV_PART_MAIN);

    lv_obj_t* label = lv_label_create(label_div);
    lv_obj_set_align(label , LV_ALIGN_CENTER);

    return label;
}