#ifndef ECE5210_H
#define ECE5210_H

#include <stdint.h>

#define PASSTHROUGH_RIGHT

//typedef struct {
//    float data[BUFF_SIZE];
//    uint16_t write_buff_index;
//} buffer;

float iir_sos(float x, float *b , float *a, int num_stages);
int16_t process_sample(int16_t sample_in);

#endif
