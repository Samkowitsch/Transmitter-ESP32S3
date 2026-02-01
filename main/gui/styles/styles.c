//
// Created by Sam Weffling on 30/12/2025.
//
#include "styles.h"

const int32_t TOOLBAR_COLOR = 0x2f4550;
const int32_t TOOLBAR_BORDER = 0x1b263b;

lv_style_t toolbar_style;
lv_style_t toolbar_item_style;

lv_style_t main_tab_style;
lv_style_t main_tab_item_style;
lv_style_t main_tab_item_active_style;
lv_style_t main_tab_content_style;

lv_style_t window_style;
lv_style_t window_title_style;
lv_style_t window_close_btn_style;


lv_style_t content_container_style;

lv_style_t flex_container_style;
lv_style_t flex_row_style;

lv_style_t simple_button_style;

lv_style_t dropdown_style;

void init_styles() {

// Toolbar
    lv_style_init(&toolbar_style);
    lv_style_set_margin_all(&toolbar_style , 0);
    lv_style_set_pad_all(&toolbar_style , 0);
    lv_style_set_border_side(&toolbar_style , LV_BORDER_SIDE_BOTTOM);
    lv_style_set_border_color(&toolbar_style , lv_color_hex(TOOLBAR_BORDER));
    lv_style_set_align(&toolbar_style , LV_ALIGN_TOP_MID);
    lv_style_set_bg_color(&toolbar_style , lv_color_hex(TOOLBAR_COLOR));
    lv_style_set_radius(&toolbar_style , 0);

// Toolbar items
    lv_style_init(&toolbar_item_style);
    lv_style_set_align(&toolbar_item_style , LV_ALIGN_CENTER);
    lv_style_set_text_font(&toolbar_item_style , &lv_font_montserrat_30);
    lv_style_set_text_align(&toolbar_item_style , LV_TEXT_ALIGN_CENTER);
    lv_style_set_margin_hor(&toolbar_item_style , 5);
    lv_style_set_text_color(&toolbar_item_style , lv_color_white());

// Main tabs
    lv_style_init(&main_tab_style);
    lv_style_set_text_font(&main_tab_style , &lv_font_montserrat_30);
    lv_style_set_align(&main_tab_style , LV_ALIGN_BOTTOM_LEFT);
    lv_style_set_pad_all(&main_tab_style , 0);
    lv_style_set_margin_all(&main_tab_style , 0);

// Main tabs buttons
    lv_style_init(&main_tab_item_style);
    lv_style_set_bg_color(&main_tab_item_style , lv_color_hex(TOOLBAR_COLOR));
    lv_style_set_text_color(&main_tab_item_style , lv_color_white());

// Main tabs buttons active
    lv_style_init(&main_tab_item_active_style);
    lv_style_set_border_side(&main_tab_item_active_style , LV_BORDER_SIDE_RIGHT);

// Main tabs content
    lv_style_init(&main_tab_content_style);
    lv_style_set_border_side(&main_tab_content_style , LV_BORDER_SIDE_NONE);
    lv_style_set_bg_opa(&main_tab_content_style , LV_OPA_0);
    lv_style_set_outline_width(&main_tab_content_style , 0);
    lv_style_set_margin_all(&main_tab_content_style , 0);
    lv_style_set_pad_all(&main_tab_content_style , 10);


    lv_style_init(&window_style);
    lv_style_set_flex_flow(&window_style , LV_FLEX_FLOW_ROW_WRAP);
    lv_style_set_flex_main_place(&window_style , LV_FLEX_ALIGN_SPACE_EVENLY);
    lv_style_set_layout(&window_style , LV_LAYOUT_FLEX);
    lv_style_set_pad_hor(&window_style , 0);
    lv_style_set_pad_ver(&window_style , 5);

    lv_style_init(&window_title_style);
    lv_style_set_pad_all(&window_title_style , 0);

    lv_style_init(&window_close_btn_style);
    lv_style_set_text_font(&window_close_btn_style , &lv_font_montserrat_30);
    lv_style_set_bg_color(&window_close_btn_style , lv_color_hex(0xA51E1C));
    lv_style_set_radius(&window_close_btn_style , 0);
    lv_style_set_border_color(&window_close_btn_style , lv_color_hex(0x686765));
    lv_style_set_border_side(&window_close_btn_style , LV_BORDER_SIDE_LEFT);
    lv_style_set_border_width(&window_close_btn_style , 2);
    lv_style_set_margin_all(&window_close_btn_style , 0);
    lv_style_set_pad_all(&window_close_btn_style , 0);


    lv_style_init(&content_container_style);
    lv_style_set_pad_all(&content_container_style , 4 );
    lv_style_set_text_font(&content_container_style , &lv_font_montserrat_22);
    lv_style_set_margin_all(&content_container_style , 0);
    lv_style_set_outline_width(&content_container_style , 0);
    lv_style_set_radius(&content_container_style , 0);
    lv_style_set_border_width(&content_container_style , 0);


    lv_style_init(&flex_container_style);
    lv_style_set_text_font(&flex_container_style , &lv_font_montserrat_22);
    lv_style_set_flex_flow(&flex_container_style , LV_FLEX_FLOW_COLUMN);
    lv_style_set_flex_main_place(&flex_container_style , LV_FLEX_ALIGN_START);
    lv_style_set_layout(&flex_container_style , LV_LAYOUT_FLEX);
    lv_style_set_pad_all(&flex_container_style , 0);
    lv_style_set_border_side(&flex_container_style , LV_BORDER_SIDE_NONE);
    lv_style_set_bg_opa(&flex_container_style , LV_OPA_0);

    lv_style_init(&flex_row_style);
    lv_style_set_border_side(&flex_row_style , LV_BORDER_SIDE_NONE);
    lv_style_set_bg_opa(&flex_row_style , LV_OPA_0);
    lv_style_set_radius(&flex_row_style , 0);
    lv_style_set_flex_flow(&flex_row_style , LV_FLEX_FLOW_ROW);
    lv_style_set_flex_main_place(&flex_row_style , LV_FLEX_ALIGN_SPACE_EVENLY);
    lv_style_set_layout(&flex_row_style , LV_LAYOUT_FLEX);
    lv_style_set_pad_all(&flex_row_style , 0);
    lv_style_set_pad_row(&flex_row_style , 0);
    lv_style_set_text_align(&flex_row_style , LV_TEXT_ALIGN_CENTER);
    lv_style_set_align(&flex_row_style , LV_ALIGN_CENTER);
    lv_style_set_size(&flex_row_style , lv_pct(100) , 50);


    lv_style_init(&simple_button_style);

    lv_style_init(&dropdown_style);
    lv_style_set_text_font(&dropdown_style , &lv_font_montserrat_22);


}