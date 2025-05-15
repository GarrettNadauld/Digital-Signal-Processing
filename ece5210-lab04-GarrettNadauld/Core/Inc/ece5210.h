#ifndef ECE5210_H
#define ECE5210_H

#include <stdint.h>

#define PASSTHROUGH_RIGHT

float fir(float sample_in, float *b, uint16_t len_b);
int16_t process_sample(int16_t sample_in);

#endif
