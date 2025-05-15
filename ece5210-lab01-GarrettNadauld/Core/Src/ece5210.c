#include "ece5210.h"

#include <math.h>
#include <stddef.h>
#include <stdio.h>

#define LIMIT 4500

float distortion(float insample, float limit){
	float y_n = 0.0;
	float temp = insample/limit;

	if (temp <= -1){
		y_n = (float)(-2.0/3.0);//*limit;
	}

	else if (temp > 1){
		y_n = (float)(2.0/3.0 );//*limit;
	}
	else {
		y_n = temp - ((float)((pow(temp,3))/3));
	}

	y_n *= limit;

	return y_n;
}

int16_t process_sample(int16_t sample_in)
{
    int16_t sample_out = 0;
    float sample_in_f = (float)sample_in;

    // This is just simple pass through, if you want to
    // do any signal processing, start editing here...
    float sample_out_f = distortion(sample_in_f, LIMIT);

    // Convert back to int16_t
    sample_out = (int16_t)sample_out_f;
    return sample_out;
}
