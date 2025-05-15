#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "test_utils/utest.h"

#include "ece5210.h"

typedef struct array
{
    uint16_t size;
    int16_t data[];
} array;

double mean_absolute_error(array *y, array *x);
array * initialize_array(uint16_t size);
void skip_lines(FILE *fp, int n_lines);
array * read_1d_mtx(char filename[]);
    

UTEST(ece5210_lab01, moving_average)
{
    int ret = system("python test_utils/python_lab01.py");
    char filename_x[] = "x_out.mtx";
    char filename_y[] = "y_out.mtx";

    array *x = read_1d_mtx(filename_x);
    array *y_sol = read_1d_mtx(filename_y);

    array *y_my = initialize_array(x->size);

    for (uint16_t i = 0; i < x->size; i++)
    {
        y_my->data[i] = process_sample(x->data[i]);
    }
  
    double error = mean_absolute_error(y_sol, y_my);

    ASSERT_LT(error, 0.03);

    ret = system("rm x_out.mtx y_out.mtx");
    (void)ret;

    free(x);
    free(y_sol);
    free(y_my);

    x = NULL;
    y_sol = NULL;
    y_my = NULL;

}

UTEST(ece5210_lab01, dtft)
{
    int ret = system("python test_utils/test_lab01.py TestDTFT.test_dtft");
    ASSERT_EQ(ret, 0);
}

UTEST(ece5210_lab01, phase_correct) {
    int ret = system("python test_utils/test_lab01.py TestDTFT.test_phase_correct");
    ASSERT_EQ(ret, 0);
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
