#include "ece5210.h"

#include <stdio.h>

static float x_n[] = {0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f};
static int cnt = 0;

int16_t process_sample(int16_t sample_in)
{
    int16_t sample_out = 0;
    float sample_in_f = (float)sample_in;
    float y_n = 0.0;
    // This is just simple pass through, if you want to
    // do any signal processing, start editing here...
//    static double x_n[] = {


    if (cnt < 6) {
    	x_n[cnt] = sample_in_f;
    	cnt += 1;
    }
    else {
    	x_n[cnt] = sample_in_f;
    	cnt = 0;
    }

    for(int i=0; i<7; i++) {
    	y_n += x_n[i];
    }

    y_n = y_n/((float)7.0);

    float sample_out_f = y_n;

    // Convert back to int16_t
    sample_out = (int16_t)sample_out_f;
    return sample_out;
}
