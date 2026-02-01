//
// Created by Sam Weffling on 02/01/2026.
//

#include "model_setup.h"

#include "gui/components/flex_components.h"
#include "gui/components/window_component.h"


void draw_model_setup_window(lv_obj_t* parent) {


    lv_obj_t* window_content = create_window_component(parent , "Model setup" , NULL);
 
    /*
    lv_obj_t* row = create_flex_row(window_content , 50);

    lv_obj_t* name_field = lv_textarea_create(row);
    lv_obj_set_height(name_field, lv_pct(90));

    lv_obj_t* keyboard = lv_keyboard_create(window_content);
    lv_obj_set_height(keyboard, lv_pct(50));

    lv_keyboard_set_textarea(keyboard , name_field);

    */


}