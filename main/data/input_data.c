//
// Created by Sam Weffling on 03/01/2026.
//

#include "input_data.h"
#include "math.h"

int16_t servo_output[CHANNEL_COUNT];



static int32_t map(const int32_t x, const int32_t min_in, const int32_t max_in, const int32_t min_out, const int32_t max_out)
{
    if(max_in == min_in) return min_out; /*Avoid division by zero later*/

    if(max_in >= min_in && x >= max_in) return max_out;
    if(max_in >= min_in && x <= min_in) return min_out;

    if(max_in <= min_in && x <= max_in) return max_out;
    if(max_in <= min_in && x >= min_in) return min_out;

    /**
     * The equation should be:
     *   ((x - min_in) * delta_out) / delta in) + min_out
     * To avoid rounding error reorder the operations:
     *   (x - min_in) * (delta_out / delta_min) + min_out
     */

    const int32_t delta_in = max_in - min_in;
    const int32_t delta_out = max_out - min_out;

    return ((x - min_in) * delta_out) / delta_in + min_out;
}


int16_t get_servo_output(const int channel) {
    return servo_output[channel];
}

void set_servo_output(const int channel, const int16_t value) {
    servo_output[channel] = value;
}



int16_t calc_servo_output_without_trim(int16_t input , const int channel) {
    int16_t output = 0;


    t_channel_setup *setup = get_channel_setup(channel);


    if (input >= 0) {
        output = map(input , ANALOG_POS_MIN_IN , ANALOG_POS_MAX_IN , setup->zero_value * PERCENT_FACTOR , setup->max_value * PERCENT_FACTOR);
    }else {
        output = map(input , ANALOG_NEG_MAX_IN , ANALOG_NEG_MIN_IN , setup->zero_value * PERCENT_FACTOR , -setup->min_value * PERCENT_FACTOR);
    }

    if (setup->reversed) {
        output = -output;
    }

    return output;
}


int16_t calc_servo_output(int16_t input , const int channel) {

    t_channel_setup *setup = get_channel_setup(channel);

    int16_t output =  calc_servo_output_without_trim( input ,channel);

    output += setup->trim_value * PERCENT_FACTOR;


    return output;
}

