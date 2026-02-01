//
// Created by Sam Weffling on 01/01/2026.
//

#include "label_switch.h"



lv_obj_t* create_label_switch(lv_obj_t* parent , const char* label_text) {


     lv_obj_t* container = lv_obj_create(parent);
     lv_obj_set_size(container , lv_pct(100) , lv_pct(100));
     lv_obj_set_style_pad_all(container , 0 , LV_PART_MAIN);
     lv_obj_set_style_border_side(container , LV_BORDER_SIDE_NONE , LV_PART_MAIN);
     lv_obj_set_style_bg_opa(container , LV_OPA_0 , LV_PART_MAIN);

     lv_obj_t* label = lv_label_create(container);
     lv_obj_set_size(label , lv_pct(50) , LV_SIZE_CONTENT);
     lv_label_set_text(label , label_text);
     lv_obj_set_style_text_align(label , LV_TEXT_ALIGN_LEFT , LV_PART_MAIN);
     lv_obj_set_align(label , LV_ALIGN_LEFT_MID);

     lv_obj_t* switch_obj = lv_switch_create(container);
     lv_obj_set_size(switch_obj , lv_pct(30) , lv_pct(70));
     lv_obj_set_align(switch_obj , LV_ALIGN_RIGHT_MID);

     return switch_obj;

}