#ifndef ECE5210_H
#define ECE5210_H

#include <stdint.h>

#define PASSTHROUGH_RIGHT

#define BUFF_SIZE 128
#define BUFF_SIZE_MASK (BUFF_SIZE - 1)

typedef struct {
    float data[BUFF_SIZE];
    uint16_t write_buff_index;
} buffer;

void write_buff_val(buffer *buff, float value);
float read_buff_val(buffer *buff, uint16_t idx_in);

float H_linear(float x, float *b, uint16_t len_b);
float H_min(float x, float *b, uint16_t len_b);
float H_ap(float x, float *b, float *a, uint16_t len_b);
int16_t process_sample(int16_t sample_in);

#endif
