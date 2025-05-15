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
float fir_int(float sample_in, float *b, uint16_t len_b, uint16_t z);
float fir_dec(float sample_in, float *b, uint16_t len_b, uint16_t z);
float fir(float sample_in, float *b, uint16_t len_b);
void init_firwin(void);
float polyphase_decimation(float x, float *h, uint16_t M);
float polyphase_interpolation(float x, float *h, uint16_t L);
int16_t process_sample(int16_t sample_in);


#endif
