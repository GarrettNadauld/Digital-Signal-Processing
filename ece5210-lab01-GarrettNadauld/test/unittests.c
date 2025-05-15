#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "test_utils/utest.h"

#include "ece5210.h"

UTEST(ece5210_lab01, distort)
{
    int ret;

    time_t t;
    srand((unsigned int)time(&t));

    char command[100];

    for (int i = 0; i < 100; ++i)
    {

        /* generate random numbers from -1000 to 1000 */
        float x = (float)((double)rand() / (double)(RAND_MAX) * 2000 - 1000);

        /* generate random numbers from 300 to 600 */
        float limit = (float)((double)rand() / (double)(RAND_MAX) * 300 + 300);

        sprintf(command,
                "python test_utils/python_distort.pyc %lf %lf > tmp.txt", x,
                limit);

        ret = system(command);

        float sol;
        FILE *f = fopen("tmp.txt", "r");
        fscanf(f, "%f", &sol);

        double ans = distortion(x, limit);

        ASSERT_LT(fabs(sol - ans), 10e-5);
    }

    ret = system("rm tmp.txt");
    (void)ret;
}

UTEST_MAIN();
