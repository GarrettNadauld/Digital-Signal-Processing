#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#include "test_utils/utest.h"

#include "ece5210.h"

#define NUM_TAPS 35

static float h_sol[] = {
   0.200081f,0.160065f,0.128052f,0.102442f,0.081953f,0.065563f,
   0.052450f,0.041960f,0.033568f,0.026854f,0.021484f,0.017187f,
   0.013749f,0.011000f,0.008800f,0.007040f,0.005632f,0.004505f,
   0.003604f,0.002883f,0.002307f,0.001845f,0.001476f,0.001181f,
   0.000945f,0.000756f,0.000605f,0.000484f,0.000387f,0.000310f,
   0.000248f,0.000198f,0.000159f,0.000127f,0.000101f
};

typedef struct array
{
    uint16_t size;
    int16_t data[];
} array;

extern float h[NUM_TAPS];

double mean_absolute_error(array *y, array *x);
array * initialize_array(uint16_t size);
void skip_lines(FILE *fp, int n_lines);
array * read_1d_mtx(char filename[]);

UTEST(ece5210_lab02, fir_accuracy)
{
    int ret = system("python test_utils/python_lab02.py");
    char filename_x[] = "x_out.mtx";
    char filename_y[] = "y_out.mtx";

    array *x = read_1d_mtx(filename_x);
    array *y_sol = read_1d_mtx(filename_y);

    array *y_my = initialize_array(x->size);

    // initialize the filter
    init_filter();

    // perform the filtering
    for (uint16_t i = 0; i < x->size; i++)
    {
        y_my->data[i] = process_sample(x->data[i]);    
    }
    
    double error = mean_absolute_error(y_sol, y_my);

    ASSERT_LT(error, 0.01);

    ret = system("rm x_out.mtx y_out.mtx");
    (void)ret;

    free(x);
    free(y_sol);
    free(y_my);

    x = NULL;
    y_sol = NULL;
    y_my = NULL;

}

UTEST(ece5210_lab02, init_filter)
{
    init_filter();

    for (int i = 0; i < NUM_TAPS; i++)
    {
        float abs_diff = fabsf(h[i] - h_sol[i]);
        ASSERT_LT(abs_diff,1e-4);
    }
        
}



/* UTEST(ece5210_lab01, python) */
/* { */
/*     printf("Testing Python code\n"); */
/*     int ret = system("python test_utils/test_lab01.py"); */
/*     ASSERT_EQ(ret, 0); */
/* } */



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

array * initialize_array(uint16_t size)
{
    array *out = malloc(sizeof(*out) +
                        sizeof(int16_t)*(size_t)size);

    out->size = size;
    return out;
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

    for (uint16_t m = 0; m < out->size; m++)
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
