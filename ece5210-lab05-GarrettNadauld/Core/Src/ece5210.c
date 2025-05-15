#include "ece5210.h"

#include <stdio.h>
#include <string.h>
#include <math.h>

#define PI 3.14159265358979323846264338327950288f

#define NUM_TAPS 1000
#define DECIMATE 100
#define DEC_TAPS (NUM_TAPS / DECIMATE)
#define INTERPOLATE (DECIMATE)
#define INT_TAPS (NUM_TAPS / INTERPOLATE)

#define W_C (PI / DECIMATE)


float h_aa[NUM_TAPS] = {0.f};
float h_poly[NUM_TAPS] = {0.f};
int cnt_dec = 0;
int buffer_index = 0;

void write_buff_val(buffer *buff, float value){
	buff->data[(++buff->write_buff_index) & BUFF_SIZE_MASK] = value;
}

float read_buff_val(buffer *buff, uint16_t idx_in){
	return buff->data[(buff->write_buff_index - idx_in) & BUFF_SIZE_MASK];
}

/**
Generates a windowed sinc lowpass filter with cutoff
frequency defined as a constant W_C with the number of
taps also defined as a constant (NUM_TAPS).  This filter
uses a Hamming window.

   * @param void
   * @return void
*/
float h[NUM_TAPS] = {1.0f, 1.0f, 1.0f};
int cnt = 0;
float buf[NUM_TAPS] = {0.0f};
uint16_t og_buffer_index = 0;

float fir(float sample_in, float *b, uint16_t len_b){

    float sample_out_f = 0.0f; // Output sample

    buf[og_buffer_index] = sample_in;

    for(uint16_t n = 0; n < len_b; n++) {
    	int k = (og_buffer_index + len_b - n) % len_b;
        sample_out_f += b[n] * buf[k];
    }

    og_buffer_index = (og_buffer_index + 1) % len_b;

    return sample_out_f;
}

static buffer dec_buf[DECIMATE];
static buffer fir_buf[DECIMATE];

float fir_dec(float sample_in, float *b, uint16_t len_b, uint16_t z){

    float sample_out_f = 0.0f;

    write_buff_val(&fir_buf[z], sample_in);

    for(uint16_t n = 0; n < len_b; n++) {
        sample_out_f += b[z*DEC_TAPS+n] * read_buff_val(&fir_buf[z], n);
    }

    return sample_out_f;
}

static buffer int_buf[DECIMATE];

float fir_int(float sample_in, float *b, uint16_t len_b, uint16_t z){

    float sample_out_f = 0.0f;

    write_buff_val(&int_buf[z], sample_in);

    for(uint16_t n = 0; n < len_b; n++) {
        sample_out_f += b[z*INT_TAPS+n] * read_buff_val(&int_buf[z], n);
    }

    return sample_out_f;
}

float polyphase_decimation(float x, float *h, uint16_t M){
	float y = 0.0;

	write_buff_val(dec_buf, x);

	if(cnt_dec%DECIMATE == 0){
		cnt_dec ++;
		for(uint16_t i=0; i<DECIMATE; i++){
			float q = read_buff_val(dec_buf,i);
			y += fir_dec(q, h, (uint16_t)DEC_TAPS, i);
		}
		return y;
	}
		cnt_dec ++;
		return 0.0;
}

uint16_t int_cnt = 0;
float p = 0.0;

float polyphase_interpolation(float x , float *h , uint16_t L){
	if(x==0.0){
		int_cnt ++;
		return fir_int(p, h, (uint16_t)INT_TAPS, int_cnt)*L;
	}
	p = x;
	int_cnt = 0;
	return fir_int(p, h, (uint16_t)INT_TAPS, int_cnt)*L;
}

void init_firwin(void)
{
    uint16_t M = NUM_TAPS - 1;
    float sum = 0.f;
    int cnt = 0;

    for (uint16_t n = 0; n < NUM_TAPS; n++)
    {
        if ((n == M/2) && (NUM_TAPS % 2))
        {
            h_aa[n] = W_C/PI;
        }
        else
        {
            h_aa[n] = sinf(W_C*(n - (float)M/2)) /
                (PI*(n - (float)M/2));
        }

        h_aa[n] *= 0.54f - 0.46f*cosf(2*PI*n/M);
        
        sum += h_aa[n];
    }

    for (int n = 0; n < NUM_TAPS; n++)
    {
        h_aa[n] /= sum;
    }

    for(int i=0; i<DECIMATE; i++) {
    	for(int j=0; j<DEC_TAPS; j++) {
    		h_poly[cnt] = h_aa[i+j*DECIMATE];
		cnt += 1;
    	}
    }
    
}

int16_t process_sample(int16_t sample_in)
{
	h[1] = (float)(-2.0*(float)cos((float)(120.0/480.0 * M_PI)));
    int16_t sample_out = 0;
    float sample_out_f = 0.0;
    float sample_in_f = (float)sample_in;

    sample_out_f = polyphase_decimation(sample_in_f, h_poly, DECIMATE);

    if(sample_out_f!=0.0){
        	sample_out_f = fir(sample_out_f, h, 3);
    }

    sample_out_f = polyphase_interpolation(sample_out_f, h_poly, INTERPOLATE);

    // Convert back to int16_t
    sample_out = (int16_t)sample_out_f;
    return sample_out;
}


