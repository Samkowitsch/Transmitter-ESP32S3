//
// Created by Sam Weffling on 30/12/2025.
//

#include "main_screen.h"
#include "gui/gui.h"
#include "tabs/home_tab.h"
#include "tabs/connection_tab.h"
#include "tabs/settings_tab.h"


static lv_obj_t* main_screen;

static lv_obj_t* time_indicator = NULL;
static lv_obj_t* battery_indicator = NULL;
static lv_obj_t* sdcard_indicator = NULL;
static lv_obj_t* model_name = NULL;


void set_battery_level(const float voltage ,const int indicator) {

    if (!lv_obj_is_valid(battery_indicator)) {
        return;
    }

    char symbol[16] = "test";

    switch (indicator) {
        case 4:
            lv_strcpy(symbol , "#00ff00 " LV_SYMBOL_BATTERY_FULL " #");
            break;
        case 3:
            lv_strcpy(symbol , "#00ff00 " LV_SYMBOL_BATTERY_3 " #");
            break;
        case 2:
            lv_strcpy(symbol , "#00ff00 " LV_SYMBOL_BATTERY_2 " #");
            break;
        case 1:
            lv_strcpy(symbol , "#ffff00 " LV_SYMBOL_BATTERY_1 " #");
            break;
        default:
            lv_strcpy(symbol , "#ff0000 " LV_SYMBOL_BATTERY_EMPTY " #");
            break;
    }

    lv_label_set_text_fmt(battery_indicator , "%.1fV %s" , voltage , symbol );

}

void set_sdcard_state(const bool state) {
    if (!lv_obj_is_valid(sdcard_indicator)) {
        return;
    }


    if (state) {
        lv_label_set_text(sdcard_indicator , LV_SYMBOL_SD_CARD );
    }else {
        lv_label_set_text(sdcard_indicator , "#ff0000 " LV_SYMBOL_SD_CARD " #");
    }


}


void set_model_name(const char* name) {
    if (!lv_obj_is_valid(model_name)) {
        return;
    }

    lv_label_set_text(model_name , name);
}

static lv_obj_t* draw_toolbar(lv_obj_t * parent) {


    lv_obj_t* toolbar = lv_obj_create(parent);
    lv_obj_set_size(toolbar , SCREEN_WIDTH , TOOLBAR_HEIGHT);
    lv_obj_add_style(toolbar , &toolbar_style , LV_PART_MAIN);


    lv_obj_t* right_row = lv_obj_create(toolbar);
    lv_obj_set_size(right_row , SCREEN_WIDTH / 2 , TOOLBAR_HEIGHT);
    lv_obj_set_align(right_row , LV_ALIGN_TOP_RIGHT);
    lv_obj_set_flex_flow(right_row , LV_FLEX_FLOW_ROW_REVERSE);
    lv_obj_set_flex_align(right_row, LV_FLEX_ALIGN_END , LV_FLEX_ALIGN_CENTER , LV_FLEX_ALIGN_CENTER);

    lv_obj_set_style_margin_all(right_row , 0 , LV_PART_MAIN);
    lv_obj_set_style_pad_all(right_row , 0 , LV_PART_MAIN);
    lv_obj_set_style_border_side(right_row , LV_BORDER_SIDE_NONE , LV_PART_MAIN);
    lv_obj_set_style_bg_opa(right_row , LV_OPA_0 , LV_PART_MAIN);

    lv_obj_t* left_row = lv_obj_create(toolbar);
    lv_obj_set_size(left_row , SCREEN_WIDTH / 2 , TOOLBAR_HEIGHT);
    lv_obj_set_align(left_row , LV_ALIGN_TOP_LEFT);
    lv_obj_set_flex_flow(left_row , LV_FLEX_FLOW_ROW);
    lv_obj_set_flex_align(left_row, LV_FLEX_ALIGN_START , LV_FLEX_ALIGN_CENTER , LV_FLEX_ALIGN_CENTER);

    lv_obj_set_style_margin_all(left_row , 0 , LV_PART_MAIN);
    lv_obj_set_style_pad_all(left_row , 0 , LV_PART_MAIN);
    lv_obj_set_style_border_side(left_row , LV_BORDER_SIDE_NONE , LV_PART_MAIN);
    lv_obj_set_style_bg_opa(left_row , LV_OPA_0 , LV_PART_MAIN);


    time_indicator = lv_label_create(right_row);
    lv_obj_set_size(time_indicator , LV_SIZE_CONTENT , LV_SIZE_CONTENT);
    lv_obj_add_style(time_indicator , &toolbar_item_style , LV_PART_MAIN);
    lv_label_set_text(time_indicator , "00:00");

    battery_indicator = lv_label_create(right_row);
    lv_obj_set_size(battery_indicator , LV_SIZE_CONTENT , LV_SIZE_CONTENT);
    lv_obj_add_style(battery_indicator , &toolbar_item_style , LV_PART_MAIN);
    lv_label_set_recolor(battery_indicator , true);
    set_battery_level(0.0 , 4);

    sdcard_indicator = lv_label_create(right_row);
    lv_obj_set_size(sdcard_indicator , LV_SIZE_CONTENT , LV_SIZE_CONTENT);
    lv_obj_add_style(sdcard_indicator , &toolbar_item_style , LV_PART_MAIN);
    lv_label_set_recolor(sdcard_indicator , true);
    set_sdcard_state(false);



    model_name = lv_label_create(left_row);
    lv_obj_set_size(model_name , LV_SIZE_CONTENT , LV_SIZE_CONTENT);
    lv_obj_add_style(model_name , &toolbar_item_style , LV_PART_MAIN);
    set_model_name("Test Name");


    return toolbar;

}

void init_main_screen(const bool load_screen) {
    lv_lock();
    if (main_screen != NULL) {
        //lv_obj_del(main_screen);
    }

    main_screen = lv_obj_create(NULL);
    lv_obj_set_size(main_screen , SCREEN_WIDTH , SCREEN_HEIGHT );
    lv_obj_set_align(main_screen , LV_ALIGN_TOP_MID);

    lv_obj_t* toolbar = draw_toolbar(main_screen);

    lv_obj_t* tabview = lv_tabview_create(main_screen);
    lv_obj_align(tabview , LV_ALIGN_TOP_RIGHT , 0 , TOOLBAR_HEIGHT);
    lv_obj_set_size(tabview , SCREEN_WIDTH , MAIN_WINDOW_HEIGHT );
    lv_tabview_set_tab_bar_position(tabview , LV_DIR_LEFT);
    lv_tabview_set_tab_bar_size(tabview , MAIN_TAB_WIDTH);
    lv_obj_remove_flag(lv_tabview_get_content(tabview) , LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_add_style(tabview , &main_tab_style , LV_PART_MAIN);

    lv_obj_t* tabview_btns = lv_tabview_get_tab_bar(tabview);
    lv_obj_add_style(tabview_btns , &main_tab_item_style , LV_PART_MAIN);




    draw_home_tab(lv_tabview_add_tab(tabview , LV_SYMBOL_HOME));
    draw_connection_tab(lv_tabview_add_tab(tabview , LV_SYMBOL_WIFI));
    draw_settings_tab(lv_tabview_add_tab(tabview , LV_SYMBOL_SETTINGS));


    for (int i = 0 ; i < lv_tabview_get_tab_count(tabview) ; i++) {
        lv_obj_t* btn = lv_obj_get_child(tabview_btns , i );
        lv_obj_add_style(btn , &main_tab_item_active_style , LV_PART_MAIN | LV_STATE_CHECKED);
    }


    if (load_screen) {
        lv_screen_load(main_screen);
    }

    lv_unlock();


}