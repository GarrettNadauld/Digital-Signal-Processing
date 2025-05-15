#include "ece5210.h"
#include <math.h>
#include <stdio.h>
#define NUM_TAPS 34

float h[NUM_TAPS] = {0.0f};
//float k_m[NUM_TAPS-1] = {0.0f};

static uint16_t buffer_index = 0;
static float buffer[NUM_TAPS] = {0};
//
////float f[NUM_TAPS][2];
////float g[NUM_TAPS][2];
////float y[NUM_TAPS];
////float a[NUM_TAPS][NUM_TAPS];
////float x[NUM_TAPS] = {0.0f};
//static float f[NUM_TAPS], g[NUM_TAPS];
//static float oldg[NUM_TAPS] = {0}, oldx = 0;
//static float x[NUM_TAPS] = {0.0f};
//static float y[NUM_TAPS] = {0.0f};
////float G = 0.0;
//int buffer_index = 0;
//int flag = 0;
float alpha = 0.21; 	//low alpha more filtering, high alpha low filtering a~[0,1]
float beta = 0.99;		//low beta less filtering, high beta high filtering B~[0,1]
float y[2] = {0.f, 0.f};
float x_prev = 0.0f;

float a_filt(float x) {
	y[0] = alpha*x + (1.0-alpha)*y[1];
	y[1] = y[0];
	return y[0];
}

float b_filt(float x) {
	y[0] = 0.5 * (2.0 - beta) * (x - x_prev) + (1 - beta) * y[1];
	x_prev = x;
	y[1] = y[0];
	return y[0];
}

float fir(float sample_in, float *b, uint16_t len_b) {

    float sample_out_f = 0.0f; // Output sample

    	buffer[buffer_index] = sample_in;

    for(uint16_t n = 0; n < len_b; n++) {
    	int k = (buffer_index + len_b - n) % len_b;
        sample_out_f += b[n] * buffer[k];
    }

    buffer_index = (buffer_index + 1) % len_b;
//	printf("%f\n",sample_out_f);
    return sample_out_f;
}

void init_filter(void) {
	float sum = 0.0f;

	for(int n = 0; n < NUM_TAPS; n++) {
		h[n] = (float)powf(0.8f, n);
		sum += h[n];
	}

	for(int n = 0; n < NUM_TAPS; n++) {
		h[n] /= sum;
	}

}

int16_t process_sample(int16_t sample_in)
{
    float sample_in_f = (float)sample_in;

//    float sample_out_f = a_filt(sample_in_f);
//    float sample_out_f = b_filt(sample_in_f);
    float sample_out_f = fir(sample_in_f, h, NUM_TAPS);

    buffer_index = 0;
    return (int16_t)sample_out_f;
}
