#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#include "test_utils/utest.h"

#include "ece5210.h"

#define NS 3
#define NUM_BQ_TAPS 3

float b_sos_test[NS*NUM_BQ_TAPS] = {0.9f,0.8f,0.7f,
    0.6f,0.5f,0.4f,
    0.3f,0.2f,0.1f};
float a_sos_test[NS*NUM_BQ_TAPS] = {1.f,0.6f,0.5f,
    1.f,0.4f,0.3f,
    1.f,0.2f,0.1f};


typedef struct array
{
    uint16_t size;
    int16_t data[];
} array;

typedef struct array_f32
{
    uint32_t size;
    float data[];
} array_f32;


double mean_absolute_error(array *y, array *x);
double mean_absolute_error_f32(array_f32 *y, array_f32 *x);
array * initialize_array(uint16_t size);
array_f32 * initialize_array_f32(uint32_t size);
void skip_lines(FILE *fp, int n_lines);
array * read_1d_mtx(char filename[]);
array_f32 * read_1d_f32_mtx(char filename[]);
array_f32 * rand_uniform_f32(float start, float end, uint32_t size);

UTEST(ece5210_iir, filt_sos)
{
    int ret = system("python test_utils/ece5210_iir_sos.py");
    char filename_x[] = "x_out.mtx";
    char filename_y[] = "y_out.mtx";

    array_f32 *x = read_1d_f32_mtx(filename_x);
    array_f32 *y_sol = read_1d_f32_mtx(filename_y);

    array_f32 *y_my = initialize_array_f32(x->size);

    for (uint16_t i = 0; i < x->size; i++)
    {
        y_my->data[i] = iir_sos(x->data[i],
                                b_sos_test,
                                a_sos_test,
                                NS);
    }

    double error = mean_absolute_error_f32(y_sol, y_my);

    ASSERT_LT(error, 0.0001);

    ret = system("rm x_out.mtx y_out.mtx");
    (void)ret;

    free(x);
    free(y_sol);
    free(y_my);

    x = NULL;
    y_sol = NULL;
    y_my = NULL;

}




UTEST_MAIN();

/************************************* 
         SUPPORT FUNCTIONS 
*************************************/

double mean_absolute_error(array *y, array *x)
{
    double diff = 0;
    for (int n = 0; n < y->size; n++)
    {
        diff += fabs((double)y->data[n] - (double)x->data[n]);
    }

    return diff/y->size;
}

double mean_absolute_error_f32(array_f32 *y, array_f32 *x)
{
    double diff = 0;
    for (int n = 0; n < y->size; n++)
    {
                    diff += fabs((double)y->data[n] - (double)x->data[n]);

        if (n == 10)
        {
            break;
        }   
    }

    return diff/y->size;
}


void skip_lines(FILE *fp, int n_lines)
{
    // Skip the first n lines
    for (int i = 0; i < n_lines; ++i)
    {
        if (fscanf(fp, "%*[^\n]\n") == -1)
        {
            printf("ERROR: fscanf() failed in %s on line %i\n",
                   __FILE__,__LINE__);
        }
    }
}

array * initialize_array(uint16_t size)
{
    array *out = malloc(sizeof(*out) +
                        sizeof(int16_t)*(size_t)size);

    out->size = size;
    return out;
}

array_f32 * initialize_array_f32(uint32_t size)
{
    array_f32 *out = malloc(sizeof(*out) +
                            sizeof(float)*(size_t)size);

    out->size = size;
    return out;
}



array * read_1d_mtx(char filename[])
{
    int temp;
    
    FILE *fp;
    fp = fopen(filename,"r");

    int n_lines = 2;
    skip_lines(fp, n_lines);


    // read in data and find max value
    if (fscanf(fp,"%i %*i", &temp) == -1)
    {
        printf("ERROR: fscanf in %s in line %i failed\n",
               __FILE__,__LINE__);
        exit(1);
    }

    array *out = initialize_array((uint16_t)temp);

    for (uint32_t m = 0; m < out->size; m++)
    {
        if ((fscanf(fp,"%i", &temp) == 1) &&
            !feof(fp))
        {
            out->data[m] = (int16_t)temp;		
        }
        else
        {
            printf("ERROR:  fscanf() failed\n");
        }
    }

    fclose(fp);
    
    return out;
}


array_f32 * read_1d_f32_mtx(char filename[])
{

    
    int size;
    float temp;
    
    FILE *fp;
    fp = fopen(filename,"r");

    int n_lines = 2;
    skip_lines(fp, n_lines);

    // read in the size
    if (fscanf(fp,"%i %*f", &size) == -1)
    {
        printf("ERROR: fscanf in %s in line %i failed\n",
               __FILE__,__LINE__);
        exit(1);
    }

    array_f32 *out = initialize_array_f32((uint32_t)size);

    for (uint32_t m = 0; m < out->size; m++)
    {
        if ((fscanf(fp,"%f", &temp) == 1) &&
            !feof(fp))
        {
            out->data[m] = temp;		
        }
        else
        {
            printf("ERROR:  fscanf() failed\n");
        }
    }

    fclose(fp);
    
    return out;
}

array_f32 * rand_uniform_f32(float start, float end, uint32_t size)
{
    
    array_f32 *out = initialize_array_f32((uint32_t)size);

    srand((unsigned int)time(NULL));
    
    float a = end - start;
    float offset = a/2;
    for (uint32_t i = 0; i < out->size; i++)
    {
        out->data[i] = (float)rand()/(float)(RAND_MAX/a) - offset;
    }

    return out;
}

