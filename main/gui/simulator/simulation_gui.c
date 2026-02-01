//
// Created by Sam Weffling on 03/01/2026.
//

#include "simulation_gui.h"
#include "data/input_data.h"

static lv_display_t *display;
static lv_indev_t *mouse;
static lv_indev_t *mouse_wheel;
static lv_indev_t *keyboard;

#define ANALOG_INPUTS  4


lv_subject_t slider_values[ANALOG_INPUTS];
static int channels[CHANNEL_COUNT] = {0,1,2,3,4,5,6,7,8};

static void slider_value_cb(lv_observer_t * observer, lv_subject_t * subject)
{
    int32_t v = lv_subject_get_int(subject);
    int* channel = lv_observer_get_user_data(observer);
    set_servo_output(*channel , calc_servo_output(v , *channel));
}

void init_test_input(int heigth) {



    lv_obj_t* test_input = lv_obj_create(lv_scr_act());
    lv_obj_set_size(test_input, lv_pct(100) , heigth);
    lv_obj_set_align(test_input, LV_ALIGN_BOTTOM_MID);

    lv_obj_set_style_radius(test_input , 0 , LV_PART_MAIN);
    lv_obj_add_style(test_input , &window_style , LV_PART_MAIN);

    for (int i = 0; i < ANALOG_INPUTS; i++) {
        lv_subject_init_int(&slider_values[i] , 0);
        lv_subject_add_observer(&slider_values[i] , slider_value_cb , &channels[i]);

        lv_obj_t* slider = lv_slider_create(test_input);
        lv_obj_set_size(slider , 20 , lv_pct(100));
        lv_obj_set_style_margin_ver(slider , 20 , LV_PART_MAIN);
        lv_obj_set_style_bg_opa(slider , LV_OPA_50 , LV_PART_INDICATOR);
        lv_obj_add_flag(slider , LV_OBJ_FLAG_OVERFLOW_VISIBLE);
        lv_slider_set_mode(slider , LV_SLIDER_MODE_SYMMETRICAL);
        lv_slider_set_range(slider , -10000 , +10000);

        lv_obj_t* label = lv_label_create(slider);
        lv_obj_align(label , LV_ALIGN_BOTTOM_MID , 0 , 20);
        lv_label_bind_text(label , &slider_values[i] , "%d");

        lv_slider_bind_value(slider , &slider_values[i]);
    }



}