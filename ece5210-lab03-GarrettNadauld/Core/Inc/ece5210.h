#ifndef ECE5210_H
#define ECE5210_H

#include <stdint.h>

#define PASSTHROUGH_RIGHT

//void fir2lat(float *h, uint16_t len_b, float *k);
//
//float azlatfilt(float x_in, float *k, float G);
//
//float lattice(float sample_in, float *k, uint16_t len_b);
////float lattice(float f_x, float g_x, float k, uint16_t len_b, float *f, float *g);
//
float fir(float sample_in, float *b, uint16_t len_b);
//
void init_filter(void);

float a_filt(float x);
int16_t process_sample(int16_t sample_in);

#endif
