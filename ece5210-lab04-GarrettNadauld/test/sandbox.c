#include <stdio.h>
#include <math.h>

#include "ece5210.h"

#define M 100
#define LEN_X 5000
#define FREQ 280
#define FS 48000

int main(int argc, char *argv[])
{

    int16_t x[LEN_X] = {0};
    int16_t y[LEN_X] = {0};
    
    for (int n = 0; n < LEN_X; n++)
    {
        if ((n % M) == 0)
        {
            x[n] = (int16_t)(30000*cos(2*M_PI*FREQ/FS * n));
        }
    }

    
    for (int16_t i = 0; i < LEN_X; i++)
    {
        y[i] = process_sample(x[i]);
        printf("%i %i\n", x[i], y[i]);
    }

    
    return 0;
}
