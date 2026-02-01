//
// Created by Sam Weffling on 30/12/2025.
//

#ifndef LVGL_STYLES_H
#define LVGL_STYLES_H


#include "gui/gui.h"

extern lv_style_t toolbar_style;
extern lv_style_t toolbar_item_style;


extern lv_style_t main_tab_style;
extern lv_style_t main_tab_item_style;
extern lv_style_t main_tab_item_active_style;
extern lv_style_t main_tab_content_style;

extern lv_style_t window_style;
extern lv_style_t window_title_style;
extern lv_style_t window_close_btn_style;


extern lv_style_t content_container_style;

extern lv_style_t flex_container_style;
extern lv_style_t flex_row_style;


extern lv_style_t simple_button_style;

extern lv_style_t dropdown_style;

void init_styles();


#endif //LVGL_STYLES_H