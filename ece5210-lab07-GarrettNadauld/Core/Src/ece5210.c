#include "ece5210.h"

#include <stdio.h>
#include <math.h>

#define PI 3.14159265358979323846264338327950288f

#define W_C (PI/3)
#define NUM_TAPS 30
int q = 15;

//#define TOFIX(d, q) ((int)((d)*(double)(1<<(q))))
//#define TOFLT(a, q) ((double)(a)/(double)(1<<(q)))
//#define FMUL(a,b,q) (((a)*(b))>>(q));

#define TOFIX(d) ((int)((d)*(double)(1<<(q))))
#define TOFLT(a) ((double)(a)/(double)(1<<(q)))
#define FMUL(a,b) (((a)*(b))>>(q));

float h_float[NUM_TAPS] = {0.f};
int32_t h_fixed[NUM_TAPS];
int32_t fir_buff[NUM_TAPS] ={0};

int buff_index = 0;

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
            h_float[n] = W_C / PI;
        }
        else
        {
            h_float[n] = sinf(W_C*(n - (float)M/2)) /
                (PI*(n - (float)M/2));
        }

        h_float[n] *= 0.54f - 0.46f*cosf(2*PI*n/M);
        
        sum += h_float[n];
    }

    for (int n = 0; n < NUM_TAPS; n++)
    {
        h_float[n] /= sum;
    }
    for (int n = 0; n < NUM_TAPS; n++)
    {
    	h_fixed[n] = TOFIX(h_float[n]);
    }


//    FILE *file_fixed = fopen("fixed_coefficients121.txt", "w");
//    FILE *file_float = fopen("floating_coefficients121.txt", "w");
//
//    // Print the fixed-point coefficients to file
//    fprintf(file_fixed, "Fixed-point coefficients:\n");
//    for (int i = 0; i < NUM_TAPS; i++) {
//        fprintf(file_fixed, "%d\n", h_fixed[i]);
//    }
//
//    // Print the floating-point coefficients to file
//    fprintf(file_float, "Floating-point coefficients:\n");
//    for (int i = 0; i < NUM_TAPS; i++) {
//        fprintf(file_float, "%f\n", h_float[i]);
//    }
//
//    fclose(file_fixed);
//    fclose(file_float);

}

int16_t fir_fixed(int16_t sample_in, uint16_t len_h) {
    int32_t sample_out_f = 0;


    fir_buff[buff_index] = (int32_t)sample_in;//((int32_t)(sample_in * (1<<NUM_TAPS)));

    for (int n = 0; n < len_h; n++) {
        int k = (buff_index + len_h - n) % len_h;
        sample_out_f += h_fixed[n] * fir_buff[k];
    }


    buff_index = (buff_index + 1) % len_h;
    sample_out_f = (int32_t)TOFLT(sample_out_f);
    return (int16_t)sample_out_f;
}


/**
Function that processes a single sample

   * @param sample_in Input sample in int16_t format
   * @return Output sample in int16_t format
*/
int16_t process_sample(int16_t sample_in)
{

    int16_t sample_out = fir_fixed(sample_in, NUM_TAPS);

    return sample_out;
}

