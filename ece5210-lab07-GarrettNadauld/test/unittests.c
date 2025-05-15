#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#include "test_utils/utest.h"

#include "ece5210.h"

#define NUM_TAPS 30
#define NUM_SAMPS 100000

extern float h_float[NUM_TAPS];
static float state_array_f32[NUM_TAPS] = {0.f};

float fir_float(int16_t sample_in, uint16_t len_h);

UTEST(ece5210_fixed, random_input)
{

    // initialize the filter
    init_firwin();

    srand((unsigned int)time(NULL));

    uint16_t a = 1 << 15;
    float offset = a >> 1;

    double error = 0.;

    for (uint32_t i = 0; i < NUM_SAMPS; i++)
    {
        float temp = (float)rand()/(float)(RAND_MAX/a) - offset;
        int16_t in = (int16_t)temp;

        double y_float = (double)fir_float(in, NUM_TAPS);
        double y_fixed = (double)fir_fixed(in, NUM_TAPS);

        error += fabs((y_float - y_fixed)/y_float);

    }

    error /= NUM_SAMPS;

    ASSERT_LT(error, 0.01);

}

//UTEST(ece5210_fixed, random_input)
//{
//    // Initialize the filter
//    init_firwin();
//
//    srand((unsigned int)time(NULL));
//
//    uint16_t a = 1 << 15;
//    float offset = a >> 1;
//
//    double error = 0.;
//    double y_float_arr[NUM_SAMPS];
//    double y_fixed_arr[NUM_SAMPS];
//
//    for (uint32_t i = 0; i < NUM_SAMPS; i++)
//    {
//        float temp = (float)rand() / (float)(RAND_MAX / a) - offset;
//        uint16_t in = (uint16_t)temp; // Change type of in to uint16_t
//
//        // Compute fixed and floating-point values and store them in arrays
//        y_float_arr[i] = (double)fir_float((int16_t)in, NUM_TAPS); // Convert in to int16_t for fir_float
//        y_fixed_arr[i] = (double)fir_fixed(in, NUM_TAPS);
//
//        error += fabs((y_float_arr[i] - y_fixed_arr[i]) / y_float_arr[i]);
//    }
//
//    error /= NUM_SAMPS;
//
//    // Print fixed and floating-point values from the arrays
//    for (uint32_t i = 0; i < NUM_SAMPS/1000; i++)
//    {
//        printf("fixed: %f || floating: %f\n", y_fixed_arr[i], y_float_arr[i]);
//    }
//
//    ASSERT_LT(error, 0.01);
//}



UTEST(ece5210_fixed, dc_input)
{

    // initialize the filter
    init_firwin();

    double error = 0.;
    
    for (uint32_t i = 0; i < NUM_SAMPS; i++)
    {
        int16_t in = 32000;

        double y_float = (double)fir_float(in, NUM_TAPS);
        double y_fixed = (double)fir_fixed(in, NUM_TAPS);

        error += fabs((y_float - y_fixed)/y_float);

    }

    error /= NUM_SAMPS;
    
    ASSERT_LT(error, 0.01);

}




UTEST_MAIN();

/************************************* 
         SUPPORT FUNCTIONS 
*************************************/

float fir_float(int16_t sample_in, uint16_t len_h)
{

    float sample_in_f32 = (float)sample_in;
    for (uint16_t idx = len_h - 1; idx > 0; idx--)
    {
        state_array_f32[idx] = state_array_f32[idx-1];
    }
    state_array_f32[0] = sample_in_f32;

    float sample_out = 0;
    for (uint16_t idx = 0; idx < len_h; idx++)
    {
        sample_out += h_float[idx]*state_array_f32[idx];
    }
    
    return sample_out;
}
