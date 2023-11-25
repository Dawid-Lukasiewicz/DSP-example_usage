#include "dtmf.h"


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

void dtmf_find_samples(float *magnitude_spectrum, int *k1, int *k2, int N)
{
    float peak = dtmf_find_peak(magnitude_spectrum, N);
    float zone = peak - 0.15*peak;
    float found_samples = 0;
    float k1_prev = 0.0f;
    float k2_prev = 0.0f;
    int k1_index = 0;
    int k2_index = 0;
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
            if(k1_prev < magnitude_spectrum[i])
            {
                k1_prev = magnitude_spectrum[i];
                k1_index = i;
            }
        }
        else if(found_samples == 1 && flag_zone)
        {
            if(k2_prev < magnitude_spectrum[i])
            {
                k2_prev = magnitude_spectrum[i];
                k2_index = i;
            }
        }
        else if(found_samples == 2 && !flag_zone)
        {
            *k1 = k1_index;
            *k2 = k2_index;
            break;
        }
    }
    *k1 = k1_index;
    *k2 = k2_index;
}

char dtmf_decode(float Fs, int k1, int k2, int N)
{
    char dtmf_table[4][4] =
                {
                    {'1', '2', '3', 'A'},
                    {'4', '5', '6', 'B'},
                    {'7', '8', '9', 'C'},
                    {'*', '0', '#', 'D'}
                };
    float Fk1 = k1*Fs/N;
    float Fk2 = k2*Fs/N;
    float err = 2 * Fs/N;
    int col = -1;
    int row = -1;

    if(Fk1 >= DTMF_R1 - err && Fk1 <= DTMF_R1 + err)
        row = 0;
    else if(Fk1 >= DTMF_R2 - err && Fk1 <= DTMF_R2 + err)
        row = 1;
    else if(Fk1 >= DTMF_R3 - err && Fk1 <= DTMF_R3 + err)
        row = 2;
    else if(Fk1 >= DTMF_R4 - err && Fk1 <= DTMF_R4 + err)
        row = 3;

    if(Fk2 >= DTMF_C1 - err && Fk2 <= DTMF_C1 + err)
        col = 0;
    else if(Fk2 >= DTMF_C2 - err && Fk2 <= DTMF_C2 + err)
        col = 1;
    else if(Fk2 >= DTMF_C3 - err && Fk2 <= DTMF_C3 + err)
        col = 2;
    else if(Fk2 >= DTMF_C4 - err && Fk2 <= DTMF_C4 + err)
        col = 3;

    if(col == -1 || row == -1)
        return 'Q';

    return dtmf_table[row][col];
}