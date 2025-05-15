#include "ece5210.h"

#include <stdio.h>
#include <string.h>
#include <math.h>

#define PI 3.14159265358979323846264338327950288f

#define NUM_TAPS 19 // need to adjust
#define W_C (PI / 2)

float h_hp[] = {
    0.02055109, 0.03641191, 0.03820462, 0.022702, -0.00959248, -0.05405346,
    -0.103017, -0.14716693, -0.17784654, 0.81120165, -0.17784654, -0.14716693,
    -0.103017, -0.05405346, -0.00959248, 0.022702, 0.03820462, 0.03641191,
    0.02055109
};

float h_lp[] = {
    0.001456, -0.02666342, -0.0431693, -0.03926709, -0.01154633, 0.03689976,
    0.09646407, 0.15380424, 0.19505105, 0.21009135, 0.19505105, 0.15380424,
    0.09646407, 0.03689976, -0.01154633, -0.03926709, -0.0431693, -0.02666342,
    0.001456
};

float h_high[] = {
	    0.02055109, 0.03641191, 0.03820462, 0.022702, -0.00959248, -0.05405346,
	    -0.103017, -0.14716693, -0.17784654, 0.81120165, -0.17784654, -0.14716693,
	    -0.103017, -0.05405346, -0.00959248, 0.022702, 0.03820462, 0.03641191,
	    0.02055109
};

float h_low[] = {
    0.001456, -0.02666342, -0.0431693, -0.03926709, -0.01154633, 0.03689976,
    0.09646407, 0.15380424, 0.19505105, 0.21009135, 0.19505105, 0.15380424,
    0.09646407, 0.03689976, -0.01154633, -0.03926709, -0.0431693, -0.02666342,
    0.001456
};

//float h_lp[NUM_TAPS] = {0.f};
//float h_hp[NUM_TAPS] = {0.f};
//int M = NUM_TAPS -1;


/**
   Generates a windowed sinc lowpass filter with cutoff
   frequency defined as a constant W_C with the number of
   taps also defined as a constant (NUM_TAPS).  This filter
   uses a Hamming window.

   * @param void
   * @return void
   */
void init_firwin(void)
{
    uint16_t M = NUM_TAPS - 1;
    float sum = 0.f;

    for (uint16_t n = 0; n < NUM_TAPS; n++)
    {
        if ((n == M/2) && (NUM_TAPS % 2))
        {
            h_lp[n] = W_C/PI;
        }
        else
        {
            h_lp[n] = sinf(W_C*(n - (float)M/2)) /
                (PI*(n - (float)M/2));
        }

        h_lp[n] *= 0.54f - 0.46f*cosf(2*PI*n/M);
        
        sum += h_lp[n];
    }

    for (int n = 0; n < NUM_TAPS; n++)
    {
        h_lp[n] /= sum;
        if (n % 2 == 1)
        {
            h_hp[n] = -h_lp[n];
        }
        else
        {
            h_hp[n] = h_lp[n];
        }
    }
}

float x_lp[NUM_TAPS] = {0.f};

float lpf(float x, float *h, uint8_t M) {
    float y = 0.0f;

    x_lp[0] = x;

	for (int i = 0; i < (M / 2); i++) {

		y += h[i] * (x_lp[M-i] + x_lp[i]);
	}

	y += h[M/2] * x_lp[(M/2)];

	for(int k=M; k>0; k--) {
		x_lp[k] = x_lp[k-1];
	}

	return y;
}

float x_hp[NUM_TAPS] = {0.f};

float hpf(float x, float *h, uint8_t M) {
    float y = 0.0f;

    x_hp[0] = x;

	for (int i = 0; i < (M / 2); i++) {

		y += h[i] * (x_hp[M-i] + x_hp[i]);
	}

	y += h[M/2] * x_hp[(M/2)];

	for(int k=M; k>0; k--) {
		x_hp[k] = x_hp[k-1];
	}

	return y;
}



int16_t process_sample_left(int16_t sample_in)
{
    int16_t sample_out = 0;
    float sample_in_f = (float)sample_in;


    float sample_out_f = lpf(sample_in_f, h_low, NUM_TAPS-1);
//    float sample_out_f = hpf(sample_in_f, h_high, NUM_TAPS-1);

    // Convert back to int16_t
    sample_out = (int16_t)sample_out_f;
    return sample_out;
}


int16_t process_sample_right(int16_t sample_in)
{
    int16_t sample_out = 0;
    float sample_in_f = (float)sample_in;

    float sample_out_f = hpf(sample_in_f, h_high, NUM_TAPS-1);

    // Convert back to int16_t
    sample_out = (int16_t)sample_out_f;
    return sample_out;
}
