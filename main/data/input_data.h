//
// Created by Sam Weffling on 03/01/2026.
//

#ifndef LVGL_INPUT_DATA_H
#define LVGL_INPUT_DATA_H

#include "stdint.h"
#include "setup_data.h"

static const int32_t ANALOG_POS_MIN_IN = 0;
static const int32_t ANALOG_POS_MAX_IN = 10000;
static const int32_t ANALOG_NEG_MAX_IN = 0;
static const int32_t ANALOG_NEG_MIN_IN = -10000;
static const int32_t PERCENT_FACTOR = ANALOG_POS_MAX_IN / 100;
static const int32_t ANALOG_RANGE_IN = ANALOG_POS_MAX_IN - ANALOG_NEG_MIN_IN;

int16_t get_servo_output(int channel);
void set_servo_output(int channel, int16_t value);
int16_t calc_servo_output_without_trim(int16_t input , int channel);
int16_t calc_servo_output(int16_t input , int channel);

#endif //LVGL_INPUT_DATA_H