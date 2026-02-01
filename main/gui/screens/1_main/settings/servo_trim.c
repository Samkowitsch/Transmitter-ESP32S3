//
// Created by Sam Weffling on 02/01/2026.
//

#include "servo_trim.h"

#include "gui/components/simple_button.h"
#include "gui/components/window_component.h"


typedef struct {
    int16_t* value_ptr;
    lv_obj_t* indicator;
}t_indicator_value;

static t_indicator_value indicator_value [CHANNEL_COUNT];


static void close_window_cb(void) {
    write_model_setup();
}


static void set_indicator(t_indicator_value* indicator) {

    //LV_LOG_USER("Indicator Value : %d" , *indicator->value_ptr);
    lv_slider_set_value(indicator->indicator , *indicator->value_ptr  , false);
    //lv_bar_set_start_value(indicator->indicator , *indicator->value_ptr - 2 , false);

}

static void increase_cb(lv_event_t* event) {

    lv_event_code_t code = lv_event_get_code(event);
    t_indicator_value* indicator = lv_event_get_user_data(event);

    if (code == LV_EVENT_CLICKED || code == LV_EVENT_LONG_PRESSED_REPEAT) {

        if (*indicator->value_ptr < 150) {
            *indicator->value_ptr += 1;
        }
        set_indicator(indicator);
    }
}

static void decrease_cb(lv_event_t* event) {

    lv_event_code_t code = lv_event_get_code(event);
    t_indicator_value* indicator = lv_event_get_user_data(event);

    if (code == LV_EVENT_CLICKED || code == LV_EVENT_LONG_PRESSED_REPEAT) {

        if (*indicator->value_ptr > -150) {
            *indicator->value_ptr -= 1;
        }

        set_indicator(indicator);

    }
}

static void slider_cb(lv_event_t* event) {
    lv_event_code_t code = lv_event_get_code(event);
    t_indicator_value* indicator = lv_event_get_user_data(event);
    lv_obj_t* target = lv_event_get_target(event);

    if (code == LV_EVENT_VALUE_CHANGED) {
        int16_t value = lv_slider_get_value(target);
        *indicator->value_ptr = value;
    }
}

static lv_obj_t* create_indicator(lv_obj_t * parent , const int width , const int height , const int index) {

    const int bar_width = 10;
    const int label_height = 30;
    const int button_height = 30;

    lv_obj_t* container = lv_obj_create(parent);
    lv_obj_set_size(container , width , height);
    lv_obj_set_style_pad_hor(container , 0 , LV_PART_MAIN);
    lv_obj_set_style_pad_ver(container , 5 , LV_PART_MAIN);
    lv_obj_set_style_bg_opa(container , LV_OPA_0 , LV_PART_MAIN);
    lv_obj_set_style_border_side(container , LV_BORDER_SIDE_NONE , LV_PART_MAIN);

    lv_obj_t* increase_btn = create_simple_button(container , LV_SYMBOL_UP , lv_pct(100) , button_height);
    lv_obj_set_align(increase_btn , LV_ALIGN_TOP_MID);

    lv_obj_t* decrease_btn = create_simple_button(container , LV_SYMBOL_DOWN , lv_pct(100) , button_height);
    lv_obj_set_align(decrease_btn , LV_ALIGN_BOTTOM_MID);


    lv_obj_t* scale_1 = lv_scale_create(container);
    lv_obj_set_height(scale_1 , lv_pct(100) - (label_height));
    lv_obj_set_width(scale_1 , lv_pct(40));
    lv_obj_set_align(scale_1  , LV_ALIGN_LEFT_MID);
    lv_scale_set_mode(scale_1 , LV_SCALE_MODE_VERTICAL_LEFT);
    lv_scale_set_range(scale_1 , -100 , 100);
    lv_scale_set_total_tick_count(scale_1 , 7);
    lv_scale_set_major_tick_every(scale_1 , 3);
    lv_scale_set_label_show(scale_1 , false );
    lv_obj_set_style_line_color(scale_1 , lv_color_hex(0x686765) , LV_PART_MAIN);

    lv_obj_t* slider = lv_slider_create(container);
    lv_obj_align(slider , LV_ALIGN_LEFT_MID ,  lv_pct(40) + 10 , 0);
    lv_obj_set_size(slider , bar_width , lv_pct(100)  - label_height);
    lv_slider_set_range(slider , -150 , 150);
    lv_slider_set_mode(slider , LV_SLIDER_MODE_SYMMETRICAL);
    lv_obj_add_flag(slider , LV_OBJ_FLAG_ADV_HITTEST);

    lv_obj_set_style_radius(slider , 0 , LV_PART_MAIN );
    lv_obj_set_style_bg_opa(slider , LV_OPA_10 , LV_PART_MAIN);
    lv_obj_set_style_margin_ver(slider , 5 , LV_PART_MAIN);
    lv_obj_set_style_outline_width(slider,  5 , LV_PART_MAIN);
    lv_obj_set_style_outline_color(slider , lv_palette_main(LV_PALETTE_BLUE) , LV_PART_MAIN);
    lv_obj_set_style_outline_opa(slider , LV_OPA_10 , LV_PART_MAIN);

    lv_obj_set_style_radius(slider , 0 , LV_PART_INDICATOR );
    lv_obj_set_style_bg_opa(slider , LV_OPA_0 , LV_PART_INDICATOR);

    lv_obj_set_style_radius(slider , 5 , LV_PART_KNOB );
    lv_obj_set_style_bg_color(slider , lv_color_hex(0x086788) , LV_PART_KNOB);
    lv_obj_set_style_pad_ver(slider , 0 , LV_PART_KNOB);

    lv_obj_t* label = lv_label_create(container);
    lv_obj_set_align(label , LV_ALIGN_BOTTOM_MID);
    lv_obj_set_style_text_align(label , LV_TEXT_ALIGN_CENTER , LV_PART_MAIN);
    lv_label_set_text_fmt(label , "Ch.%d" , index);

    indicator_value[index].indicator = slider;
    indicator_value[index].value_ptr = &get_channel_setup(index)->trim_value;
    lv_obj_add_event_cb(increase_btn , increase_cb , LV_EVENT_ALL , &indicator_value[index]);
    lv_obj_add_event_cb(decrease_btn , decrease_cb , LV_EVENT_ALL , &indicator_value[index]);
    lv_obj_add_event_cb(slider , slider_cb , LV_EVENT_VALUE_CHANGED , &indicator_value[index]);

    set_indicator(&indicator_value[index]);

    return slider;
}

void draw_servo_trim_window(lv_obj_t* parent) {

    lv_obj_t* window_content = create_window_component(parent , "Servo Trim" , close_window_cb);


    for (int i = 0 ; i < CHANNEL_COUNT ; i++) {
        create_indicator(window_content , 50 , lv_pct(90) , i);
    }
}
