//
// Created by Sam Weffling on 01/01/2026.
//
#include "stdio.h"
#include "servo_travel.h"

#include "data/input_data.h"
#include "gui/components/centered_label.h"
#include "gui/components/channel_selector.h"
#include "gui/components/flex_components.h"
#include "gui/components/label_switch.h"
#include "gui/components/simple_button.h"
#include "gui/components/window_component.h"

const char SERVO_TRAVEL_TITLE[] = "Servo Reverse + Travel";

static uint32_t selected_channel = 0;
static lv_obj_t* max_label;
static lv_obj_t* min_label;
static lv_obj_t* zero_label;
static lv_obj_t* chart;
static lv_chart_series_t* series;
static lv_chart_cursor_t* cursor;
static int32_t* series_y_points;

static const int CHART_POINTS = 100;



static void close_window_cb(void) {
    write_model_setup();
}


void limit_travel(int16_t *value) {
    if (*value < -150) {
        *value = -150;
    }else if (*value > 150) {
        *value = 150;
    }
}


static void draw_chart_line() {
    uint32_t point = 0;
    for (int16_t input = ANALOG_NEG_MIN_IN; input < ANALOG_POS_MAX_IN; input+=ANALOG_RANGE_IN / CHART_POINTS) {

        const int16_t value = calc_servo_output_without_trim(input , selected_channel) / PERCENT_FACTOR;
        series_y_points[point] = value;
        point++;
    }

    lv_chart_refresh(chart);


}



void update_max_value(int16_t new_value) {

    limit_travel(&new_value);

    get_channel_setup(selected_channel)->max_value = new_value;
    lv_chart_set_cursor_point(chart , cursor , series , CHART_POINTS -1);


    if (lv_obj_is_valid(max_label)) {
        lv_label_set_text_fmt(max_label , "%d %%" , get_channel_setup(selected_channel)->max_value);

        draw_chart_line();
    }
}

void update_zero_value(int16_t new_value) {

    limit_travel(&new_value);

    get_channel_setup(selected_channel)->zero_value = new_value;
    lv_chart_set_cursor_point(chart , cursor , series , CHART_POINTS / 2);


    if (lv_obj_is_valid(zero_label)) {
        lv_label_set_text_fmt(zero_label , "%d %%" , get_channel_setup(selected_channel)->zero_value);

        draw_chart_line();

    }
}


void update_min_value(int16_t new_value) {

    limit_travel(&new_value);


    get_channel_setup(selected_channel)->min_value = new_value;
    lv_chart_set_cursor_point(chart , cursor , series , 0);


    if (lv_obj_is_valid(min_label)) {
        lv_label_set_text_fmt(min_label , "%d %%" , get_channel_setup(selected_channel)->min_value);

        draw_chart_line();


    }
}

void update_rev_value(const bool new_value) {
    get_channel_setup(selected_channel)->reversed = new_value;
    update_max_value(get_channel_setup(selected_channel)->max_value);
    update_min_value(get_channel_setup(selected_channel)->min_value);
    update_zero_value(get_channel_setup(selected_channel)->zero_value);
}


void channel_select_cb(lv_event_t* event) {
    const lv_event_code_t code = lv_event_get_code(event);
    const lv_obj_t* target =  lv_event_get_target(event);

    if (code == LV_EVENT_VALUE_CHANGED) {
        selected_channel = lv_dropdown_get_selected(target);
        update_rev_value(get_channel_setup(selected_channel)->reversed);
    }
}


static void rev_switch_cb(lv_event_t *event) {
    const lv_event_code_t code = lv_event_get_code(event);
    lv_obj_t* target = lv_event_get_target(event);

    if (code == LV_EVENT_VALUE_CHANGED) {
        if (lv_obj_has_state(target, LV_STATE_CHECKED)) {
            update_rev_value(true);
        }else {
            update_rev_value(false);
        }
    }
}


static void max_sub_btn_cb(lv_event_t* event) {
    const lv_event_code_t code = lv_event_get_code(event);

    if (code == LV_EVENT_PRESSED  || code == LV_EVENT_LONG_PRESSED_REPEAT) {
        update_max_value(--get_channel_setup(selected_channel)->max_value);
    }
}


static void max_add_btn_cb(lv_event_t* event) {
    const lv_event_code_t code = lv_event_get_code(event);

    if (code == LV_EVENT_PRESSED || code == LV_EVENT_LONG_PRESSED_REPEAT) {
        update_max_value(++get_channel_setup(selected_channel)->max_value);
    }
}

static void zero_sub_btn_cb(lv_event_t* event) {
    const lv_event_code_t code = lv_event_get_code(event);

    if (code == LV_EVENT_PRESSED  || code == LV_EVENT_LONG_PRESSED_REPEAT) {
        update_zero_value(--get_channel_setup(selected_channel)->zero_value);
    }
}


static void zero_add_btn_cb(lv_event_t* event) {
    const lv_event_code_t code = lv_event_get_code(event);

    if (code == LV_EVENT_PRESSED  || code == LV_EVENT_LONG_PRESSED_REPEAT) {
        update_zero_value(++get_channel_setup(selected_channel)->zero_value);
    }
}


static void min_sub_btn_cb(lv_event_t* event) {
    const lv_event_code_t code = lv_event_get_code(event);

    if (code == LV_EVENT_PRESSED  || code == LV_EVENT_LONG_PRESSED_REPEAT) {
        update_min_value(--get_channel_setup(selected_channel)->min_value);
    }
}

static void min_add_btn_cb(lv_event_t* event) {
    const lv_event_code_t code = lv_event_get_code(event);

    if (code == LV_EVENT_PRESSED  || code == LV_EVENT_LONG_PRESSED_REPEAT) {
        update_min_value(++get_channel_setup(selected_channel)->min_value);
    }
}


void draw_servo_travel_window(lv_obj_t* parent) {

    lv_obj_t* window_content = create_window_component(parent , SERVO_TRAVEL_TITLE , close_window_cb);


    lv_obj_t* left_container = lv_obj_create(window_content);
    lv_obj_set_size(left_container , lv_pct(50) - 5, lv_pct(100));
    lv_obj_add_style(left_container , &flex_container_style , LV_PART_MAIN);


    lv_obj_t* channel_dropdown = create_channel_selector(left_container , lv_pct(100));
    lv_obj_add_event_cb(channel_dropdown , channel_select_cb , LV_EVENT_VALUE_CHANGED , NULL);




    //create_flex_spacer(left_container , true);

    lv_obj_t* rev_row = create_flex_row(left_container , 75);
    lv_obj_t* rev_switch = create_label_switch(rev_row , "Reverse");
    lv_obj_add_event_cb(rev_switch , rev_switch_cb , LV_EVENT_VALUE_CHANGED , NULL);



    lv_obj_t* max_row = create_flex_row(left_container , 50);

    lv_obj_t* max_sub_btn = create_simple_button(max_row , LV_SYMBOL_MINUS , lv_pct(20) , lv_pct(100));
    lv_obj_add_event_cb(max_sub_btn , max_sub_btn_cb , LV_EVENT_ALL , NULL);

    max_label = create_centered_label(max_row , lv_pct(50) , lv_pct(100));
    lv_obj_t* max_add_btn = create_simple_button(max_row , LV_SYMBOL_PLUS , lv_pct(20) , lv_pct(100));
    lv_obj_add_event_cb(max_add_btn , max_add_btn_cb , LV_EVENT_ALL  ,  NULL);


    lv_obj_t* zero_row = create_flex_row(left_container , 50);
    lv_obj_t* zero_sub_btn = create_simple_button(zero_row , LV_SYMBOL_MINUS , lv_pct(20) , lv_pct(100));
    lv_obj_add_event_cb(zero_sub_btn , zero_sub_btn_cb , LV_EVENT_ALL , NULL);

    zero_label = create_centered_label(zero_row , lv_pct(50) , lv_pct(100));
    lv_obj_t* zero_add_btn = create_simple_button(zero_row , LV_SYMBOL_PLUS , lv_pct(20) , lv_pct(100));
    lv_obj_add_event_cb(zero_add_btn , zero_add_btn_cb , LV_EVENT_ALL  , NULL);


    lv_obj_t* min_row = create_flex_row(left_container , 50);
    lv_obj_t* min_sub_btn = create_simple_button(min_row , LV_SYMBOL_MINUS , lv_pct(20) , lv_pct(100));
    lv_obj_add_event_cb(min_sub_btn , min_sub_btn_cb , LV_EVENT_ALL  , NULL);

    min_label = create_centered_label(min_row , lv_pct(50) , lv_pct(100));
    lv_obj_t* min_add_btn = create_simple_button(min_row , LV_SYMBOL_PLUS , lv_pct(20) , lv_pct(100));
    lv_obj_add_event_cb(min_add_btn , min_add_btn_cb , LV_EVENT_ALL   , NULL);





    lv_obj_t* right_container = lv_obj_create(window_content);
    lv_obj_set_size(right_container , lv_pct(50) - 5 , lv_pct(100));
    lv_obj_add_style(right_container , &flex_container_style , LV_PART_MAIN);


    chart = lv_chart_create(right_container);
    lv_obj_set_size(chart , lv_pct(100) , lv_pct(100));
    lv_chart_set_type(chart , LV_CHART_TYPE_LINE);
    lv_chart_set_point_count(chart , CHART_POINTS);
    lv_chart_set_div_line_count(chart , 7 , 3);
    cursor = lv_chart_add_cursor(chart , lv_palette_darken(LV_PALETTE_GREY , 1) , LV_DIR_ALL);
    lv_obj_set_style_size(chart , 0 , 0 , LV_PART_INDICATOR);

    series = lv_chart_add_series(chart , lv_palette_darken(LV_PALETTE_BLUE , 2) , LV_CHART_AXIS_PRIMARY_Y);
    lv_chart_set_axis_min_value(chart , LV_CHART_AXIS_PRIMARY_Y , -150);
    lv_chart_set_axis_max_value(chart , LV_CHART_AXIS_PRIMARY_Y , +150);
    series_y_points = lv_chart_get_series_y_array(chart , series);

    update_rev_value(get_channel_setup(selected_channel)->reversed);

    lv_chart_refresh(chart); /*Required after direct set*/

}