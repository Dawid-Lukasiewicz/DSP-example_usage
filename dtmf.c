#include "dmtx.h"


float dtmf_find_peak(float *magnitude_spectrum, int N)
{
    float peak = 0.0f;
    for (int i = 0; i < N/2; i++)
    {
        if (peak < magnitude_spectrum[i])
        {
            peak = magnitude_spectrum[i];
        }
    }
    return peak;
}

void dtmf_find_samples(float *magnitude_spectrum, int sample1, int sample2, int N)
{
    float peak = dtmf_find_peak(magnitude_spectrum, N);
    float zone = peak - 0.15*peak;
    int i = 0;

    // float found = 0.0f;
    // for (i; i < N/2; i++)
    // {
    //     if(zone < magnitude_spectrum[i]);
    // }

}

char dtmf_decode(int sample1, int sample2)
{

}