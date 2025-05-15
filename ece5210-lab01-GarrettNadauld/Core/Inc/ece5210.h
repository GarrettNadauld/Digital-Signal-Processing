#ifndef ECE5210_H
#define ECE5210_H

#include <stdint.h>

#define PASSTHROUGH_RIGHT

float distortion(float insample, float limit);

int16_t process_sample(int16_t sample_in);

#endif
