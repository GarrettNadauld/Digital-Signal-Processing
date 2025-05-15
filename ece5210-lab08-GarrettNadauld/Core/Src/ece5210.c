#include "ece5210.h"

#include <stdio.h>

#define NS_TOT 4
#define NUM_BQ_TAPS 3

float b_len_test[NS_TOT*NUM_BQ_TAPS] = {0.9f,0.8f,0.7f,
    0.6f,0.5f,0.4f,
    0.6f,0.5f,0.4f,
    0.6f,0.5f,0.4f,
    0.6f,0.5f,0.4f,
    0.6f,0.5f,0.4f,
    0.6f,0.5f,0.4f,
    0.6f,0.5f,0.4f,
    0.6f,0.5f,0.4f,
    0.6f,0.5f,0.4f,
    0.6f,0.5f,0.4f,
    0.6f,0.5f,0.4f,
    0.3f,0.2f,0.1f
};

float a_len_test[NS_TOT*NUM_BQ_TAPS] = {1.f,0.6f,0.5f,
    1.f,0.4f,0.3f,
    1.f,0.4f,0.3f,
    1.f,0.4f,0.3f,
    1.f,0.4f,0.3f,
    1.f,0.4f,0.3f,
    1.f,0.4f,0.3f,
    1.f,0.4f,0.3f,
    1.f,0.4f,0.3f,
    1.f,0.4f,0.3f,
    1.f,0.4f,0.3f,
    1.f,0.4f,0.3f,
    1.f,0.2f,0.1f
};

float b[NS_TOT*NUM_BQ_TAPS] = {9.97562846e-04,  9.97562846e-04,  0.00000000e+00,
		1.00000000e+00, -1.56290161e+00,  1.00000000e+00,
		1.00000000e+00, -1.81628975e+00,  1.00000000e+00,
		1.00000000e+00, -1.85808382e+00,  1.00000000e+00
};

float a[NS_TOT*NUM_BQ_TAPS] = {1.00000000e+00, -9.10906181e-01,  0.00000000e+00,
		1.00000000e+00, -1.83780839e+00,  8.72423683e-01,
		1.00000000e+00, -1.86469357e+00,  9.40538624e-01,
		1.00000000e+00, -1.88711126e+00,  9.84312541e-01
};

//static buffer int_buf[NUM_BQ_TAPS];

float d[(NS_TOT)*NUM_BQ_TAPS] = {0.0f};
float y[NS_TOT*NUM_BQ_TAPS] = {0.0f};
float w[NS_TOT][NUM_BQ_TAPS] = {0};

float H_ap(float x, float *b, float *a, uint16_t len_b, float *z) {
    float sample_out_f = 0.0f;

    z[0] = x;

    for (int i = 1; i < len_b; i++) {
        z[0] -= a[i] * z[i];
    }

    for (int i = 0; i < len_b; i++) {
        sample_out_f += b[i] * z[i];
    }

    for(int i = 2; i>0; i--) {
    	z[i] = z[i-1];
    }

    return sample_out_f;
}

float iir_sos(float x, float *b , float *a, int num_stages) {
	float y = x;
	for(int i =0; i<num_stages; i++) {
		y = H_ap(y, &b[3*i], &a[3*i], NUM_BQ_TAPS, w[i]);
	}
	return y;
}


int16_t process_sample(int16_t sample_in)
{
    int16_t sample_out = 0;
    float sample_in_f = (float)sample_in;
    float sample_out_f = iir_sos(sample_in_f, b, a, NS_TOT);
    
    sample_out = (int16_t)sample_out_f;
    return sample_out;
}

