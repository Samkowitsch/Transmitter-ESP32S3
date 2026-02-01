//
// Created by Sam Weffling on 02/01/2026.
//

#include "channel_monitor.h"
#include "gui/components/window_component.h"
#include "data/input_data.h"

static lv_timer_t* refresh_timer;

static void close_window_cb(void) {

    LV_LOG_USER("Close Window Callback");
    lv_timer_delete(refresh_timer);
}


static void timer_cb(lv_timer_t* timer) {
    LV_LOG_USER("Timer callback");


    lv_obj_t** indicators = lv_timer_get_user_data(timer);

    for (int i = 0 ; i < CHANNEL_COUNT ; i++) {
        if (lv_obj_is_valid(indicators[i])) {

            lv_bar_set_value(indicators[i] , get_servo_output(i) / 100 , false);
        }
    }

}

static lv_obj_t* create_indicator(lv_obj_t * parent , const int width , const int height , const int index) {

    const int bar_width = 10;
    const int label_height = 30;

    lv_obj_t* container = lv_obj_create(parent);
    lv_obj_set_size(container , width , height);
    lv_obj_set_style_pad_hor(container , 0 , LV_PART_MAIN);
    lv_obj_set_style_pad_ver(container , 5 , LV_PART_MAIN);
    lv_obj_set_style_bg_opa(container , LV_OPA_0 , LV_PART_MAIN);
    lv_obj_set_style_border_side(container , LV_BORDER_SIDE_NONE , LV_PART_MAIN);

    lv_obj_t* scale_1 = lv_scale_create(container);
    lv_obj_set_height(scale_1 , lv_pct(100) - label_height);
    lv_obj_align(scale_1 , LV_ALIGN_RIGHT_MID , -bar_width , 0);
    lv_scale_set_mode(scale_1 , LV_SCALE_MODE_VERTICAL_LEFT);
    lv_scale_set_range(scale_1 , -150 , 150);
    lv_scale_set_total_tick_count(scale_1 , 7);
    lv_scale_set_major_tick_every(scale_1 , 3);


    lv_obj_t* bar = lv_bar_create(container);
    lv_obj_align(bar , LV_ALIGN_RIGHT_MID , 0 , 0);
    lv_bar_set_range(bar , -150 , 150);
    lv_bar_set_mode(bar , LV_BAR_MODE_SYMMETRICAL);
    lv_bar_set_orientation(bar , LV_BAR_ORIENTATION_VERTICAL);
    lv_obj_set_size(bar , bar_width , lv_pct(100)  - label_height);
    lv_bar_set_value(bar , 0 , false);

    lv_obj_set_style_radius(bar , 0 , LV_PART_MAIN );
    lv_obj_set_style_radius(bar , 0 , LV_PART_INDICATOR );

    lv_obj_t* label = lv_label_create(container);
    lv_obj_set_align(label , LV_ALIGN_BOTTOM_MID);
    lv_obj_set_style_text_align(label , LV_TEXT_ALIGN_CENTER , LV_PART_MAIN);
    lv_label_set_text_fmt(label , "Ch.%d" , index);

    return bar;
}

void draw_channel_monitor(lv_obj_t* parent) {


    lv_obj_t* window_content = create_window_component(parent , "Monitor" , close_window_cb);

    static lv_obj_t* indicators[CHANNEL_COUNT];

    for (int i = 0 ; i < CHANNEL_COUNT ; i++) {
        indicators[i] = create_indicator(window_content , 50 , 250 , i + 1);
    }

    refresh_timer = lv_timer_create(timer_cb , 100 , indicators);



}
