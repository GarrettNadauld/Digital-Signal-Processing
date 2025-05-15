#include "ece5210.h"
#include <stdio.h>

float b_linear[] = {
    0.1f, 0.27931608f, 0.14200746f, -0.50340553f,
    0.80721034f, 0.80721034f, -0.50340553f,
    0.14200746f, 0.27931608f, 0.1f
};

float a_ap[] = {
    1.000000000000000f, -0.484157254398518f, 0.067761404393364f,
	0.233197365136217f, 0.1024f
};

float b_ap[] = {
		0.1024f, 0.233197365136217f, 0.067761404393364f,
		-0.484157254398518f, 1.0f
//    1.000000000000000e+00f, 2.277318018908367e+00f, 6.617324647789470e-01f,
//    -4.728098187485529e+00f, 9.765624999999998e+00f
};

float b_min[] = {
    9.765625000000000e-01f, 3.094286250289392e-02f, -5.843588408433565e-01f,
    7.526478857766759e-01f, 3.753550733521464e-01f, -8.200061320817401e-02f,
    6.901395999441788e-04f, 1.132982100645823e-01f, 5.687947302724340e-02f,
    1.024000000000001e-02f
};

#define NUM_TAPS 35

float linear_buffer[NUM_TAPS] = {0.0f};
float min_buffer[NUM_TAPS] = {0.0f};
float ap_buffer[NUM_TAPS] = {0.0f};
float d[NUM_TAPS] = {0.0f};
float y[NUM_TAPS] = {0.0f}; // Output array

uint16_t linear_buffer_index = 0;
uint16_t min_buffer_index = 0;
uint16_t ap_buffer_index = 0;

void write_buff_val(buffer *buff, float value) {
    buff->data[(++buff->write_buff_index) & BUFF_SIZE_MASK] = value;
}

float read_buff_val(buffer *buff, uint16_t idx_in) {
    return buff->data[(buff->write_buff_index - idx_in) & BUFF_SIZE_MASK];
}

float H_linear(float x, float *b, uint16_t len_b) {
    float sample_out_f = 0.0f;

    linear_buffer[linear_buffer_index] = x;

    for (int n = 0; n < len_b; n++) {
        int k = (linear_buffer_index + len_b - n) % len_b;
        sample_out_f += b[n] * linear_buffer[k];
    }

    linear_buffer_index = (linear_buffer_index + 1) % len_b;

    return sample_out_f;
}

float H_min(float x, float *b, uint16_t len_b) {
    float sample_out_f = 0.0f;

    min_buffer[min_buffer_index] = x;

    for (int n = 0; n < len_b; n++) {
        int k = (min_buffer_index + len_b - n) % len_b;
        sample_out_f += b[n] * min_buffer[k];
    }

    min_buffer_index = (min_buffer_index + 1) % len_b;

    return sample_out_f;
}

float H_ap(float x, float *b, float *a, uint16_t len_b) {
    float sample_out_f = 0.0f;

    d[ap_buffer_index] = x;

    for (int i = 0; i < len_b; i++) {
        sample_out_f += b[i] * d[(ap_buffer_index - i + len_b) % len_b];
    }
    for (int i = 1; i < len_b; i++) {
        sample_out_f -= a[i] * y[(ap_buffer_index - i + len_b) % len_b];
    }

    y[ap_buffer_index] = sample_out_f;
    ap_buffer_index = (ap_buffer_index + 1) % len_b;

    return sample_out_f;
}

int16_t process_sample(int16_t sample_in) {
    int16_t sample_out = 0;
    float sample_in_f = (float)sample_in;

    float sample_out_f = H_ap(sample_in_f, b_ap, a_ap, 5);

    sample_out = (int16_t)sample_out_f;
    return sample_out;
}

