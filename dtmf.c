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

void dtmf_find_samples(float *magnitude_spectrum, int *sample1, int *sample2, int N)
{
    float peak = dtmf_find_peak(magnitude_spectrum, N);
    float zone = peak - 0.15*peak;
    float found_samples = 0;
    float sample1_prev = 0.0f;
    float sample2_prev = 0.0f;
    int sample1_index = 0;
    int sample2_index = 0;
    int i = 0;
    int flag_zone = 0;
    int flag_entered = 0;

    for (i; i < N/2; i++)
    {
        if(zone < magnitude_spectrum[i])
        {
            flag_zone = 1;
            flag_entered = 1;
        }
        else
            flag_zone = 0;

        if(!flag_zone && flag_entered)
        {
            flag_entered = 0;
            found_samples += 1;
        }

        if(found_samples == 0 && flag_zone)
        {
            if(sample1_prev < magnitude_spectrum[i])
            {
                sample1_prev = magnitude_spectrum[i];
                sample1_index = i;
            }
        }
        else if(found_samples == 1 && flag_zone)
        {
            if(sample2_prev < magnitude_spectrum[i])
            {
                sample2_prev = magnitude_spectrum[i];
                sample2_index = i;
            }
        }
        else if(found_samples == 2 && !flag_zone)
        {
            *sample1 = sample1_index;
            *sample2 = sample2_index;
            break;
        }
    }

}

char dtmf_decode(int sample1, int sample2)
{

}