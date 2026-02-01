//
// Created by Sam Weffling on 02/01/2026.
//

#ifndef LVGL_SETUP_DATA_H
#define LVGL_SETUP_DATA_H

#include "stdint.h"
#include "stdbool.h"

#define CHANNEL_COUNT  9

typedef enum {
    SOURCE_NONE,

    SOURCE_ANALOG_A,
    SOURCE_ANALOG_B,
    SOURCE_ANALOG_C,
    SOURCE_ANALOG_D,
    SOURCE_ANALOG_E,
    SOURCE_ANALOG_F,
    SOURCE_ANALOG_G,
    SOURCE_ANALOG_H,

    SOURCE_DIGITAL_A,
    SOURCE_DIGITAL_B,
    SOURCE_DIGITAL_C,
    SOURCE_DIGITAL_D,
    SOURCE_DIGITAL_E,
    SOURCE_DIGITAL_F,
    SOURCE_DIGITAL_G,
    SOURCE_DIGITAL_H,
}e_channel_source;

typedef struct {
    e_channel_source source;
    bool reversed;
    int16_t max_value;
    int16_t min_value;
    int16_t zero_value;
    int16_t trim_value;
    int16_t exp_value;
}t_channel_setup;


typedef struct {
    char model_name[32];

    t_channel_setup channel_setups[CHANNEL_COUNT];
}t_model_setup;


t_channel_setup* get_channel_setup(int channel);
void init_model_setup();
void read_model_setup();
void write_model_setup();

#endif //LVGL_SETUP_DATA_H