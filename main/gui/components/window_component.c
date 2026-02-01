//
// Created by Sam Weffling on 01/01/2026.
//

#include "window_component.h"


static void close_window_cb(lv_event_t* event) {
    const lv_event_code_t code = lv_event_get_code(event);
    lv_obj_t* target = lv_event_get_target(event);

    if (code == LV_EVENT_CLICKED) {
        const close_window_cb_t close_cb = (close_window_cb_t)lv_event_get_user_data(event);

        if (close_cb) {
            close_cb();
        }

        // Button -> Header -> Window
        lv_obj_delete(lv_obj_get_parent(lv_obj_get_parent(target)));


    }
}

lv_obj_t* create_window_component(lv_obj_t* parent , const char* title , close_window_cb_t close_cb) {

    lv_obj_t* window = lv_win_create(parent);
    lv_obj_set_size(window , lv_pct(100) , lv_pct(100));
    lv_obj_set_align(window , LV_ALIGN_CENTER);
    lv_obj_set_style_margin_all(window , 0 , LV_PART_MAIN);

    lv_obj_add_style(lv_win_get_header(window) , &window_title_style , LV_PART_MAIN);
    lv_obj_t* title_obj = lv_win_add_title(window , title);
    lv_obj_set_style_margin_left(title_obj , 10 , LV_PART_MAIN);

    lv_obj_t* close_btn = lv_win_add_button(window , LV_SYMBOL_CLOSE , 75);
    lv_obj_add_style(close_btn , &window_close_btn_style , LV_PART_MAIN);
    lv_obj_add_event_cb(close_btn , close_window_cb , LV_EVENT_CLICKED , close_cb);


    lv_obj_t* window_content = lv_win_get_content(window);
    lv_obj_add_style(window_content , &window_style , LV_PART_MAIN);

    return window_content;
}