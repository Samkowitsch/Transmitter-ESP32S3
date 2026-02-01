//
// Created by Sam Weffling on 01/01/2026.
//

#include "channel_selector.h"
#include "stdio.h"

lv_obj_t * create_channel_selector(lv_obj_t *parent , const int32_t width) {

    lv_obj_t* channel_selector = lv_dropdown_create(parent);
    lv_obj_add_style(channel_selector , &dropdown_style, LV_PART_MAIN);
    lv_obj_add_style(lv_dropdown_get_list(channel_selector) , &dropdown_style , LV_PART_MAIN);
    lv_obj_set_size(channel_selector , width , LV_SIZE_CONTENT);
    lv_dropdown_clear_options(channel_selector);
    for (int i = 1 ; i <= CHANNEL_COUNT; i++) {
        char name[8];
        sprintf(name, "Ch.%d", i);
        lv_dropdown_add_option(channel_selector , name , LV_DROPDOWN_POS_LAST);
    }





    return channel_selector;
}
