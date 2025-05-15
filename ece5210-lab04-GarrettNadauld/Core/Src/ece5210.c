#include "ece5210.h"
#include <math.h>
#include <stdio.h>
#define NUM_TAPS 3

float h[NUM_TAPS] = {1.0f, 1.0f, 1.0f};
float x[2] = {0.0f};
int cnt = 0;
float buffer[NUM_TAPS] = {0.0f};
uint16_t buffer_index = 0;

float fir(float sample_in, float *b, uint16_t len_b){

    float sample_out_f = 0.0f; // Output sample

    buffer[buffer_index] = sample_in;

    for(uint16_t n = 0; n < len_b; n++) {
    	int k = (buffer_index + len_b - n) % len_b;
        sample_out_f += b[n] * buffer[k];
    }

    buffer_index = (buffer_index + 1) % len_b;

    return sample_out_f;
}

int16_t process_sample(int16_t sample_in){
    h[1] = (float)(-2.0*(float)cos((float)(120.0/480.0 * M_PI)));
    float sample_in_f = (float)sample_in;
    float sample_out_f = 0;

    if(cnt==0){
    	x[0] = x[1];
    	x[1] = fir(sample_in_f, h, NUM_TAPS);
    	sample_out_f = (x[0] + (float)((float)(cnt+1.0) * ((x[1]-x[0])/100.0)));
    	cnt += 1;
    }
    else{
   	   	sample_out_f = (x[0] + (float)((float)(cnt+1.0) * ((x[1]-x[0])/100.0)));
       	cnt += 1;
    	cnt = cnt % 100;
    }

    int16_t sample_out = (int16_t)sample_out_f;
    return sample_out;
}
