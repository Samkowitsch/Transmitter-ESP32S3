//
// Created by Sam Weffling on 01/01/2026.
//

#include "simple_button.h"


lv_obj_t* create_simple_button(lv_obj_t* parent, const char* label_text , const int width , const int height) {


    lv_obj_t* button = lv_button_create(parent);
    lv_obj_add_style(button , &simple_button_style , LV_PART_MAIN);
    lv_obj_set_size(button , width , height);

    lv_obj_t* label = lv_label_create(button);
    lv_obj_set_align(label  , LV_ALIGN_CENTER);
    lv_label_set_text(label , label_text);



    return button;
}