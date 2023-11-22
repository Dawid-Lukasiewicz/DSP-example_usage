/*
 * main.c
 *
 *  Created on: 10 paü 2023
 *      Author: Stud9
 */

#include <stdio.h>
#include <ti/sysbios/BIOS.h>
#include <xdc/cfg/global.h> // for Hwi_enableInterrupt(13)
#include "codec/codec_v1_0.c"
//#include "fft/DSPF_sp_cfftr2_dit.h"
//#include "fft/utility.h"
#include "fft/DSPF_sp_cfftr2_dit.c"
#include "fft/utility.c"

#define N 512
#define N_mask 1023
#define N2 1024

int probka;
short n;
short n_h;

float x[N2];
float x2[N2];
float x3[N2];
float x4[N2*2];
float s[N];
float sw2[N];
float sw3[N];

float w[N];
float w2[N2];
float m[N];
float m2[N];
float m3[N];
float m4[N2];
float h[N];
float b[N];
int tmp = 0;
int flag = 0;

int main(void)
{

    Pins_config();
    BIOS_start();
    return(0);
}

void przerwanie_rcv()
{

    if ((n>>2) >= 3 && !flag)
    {
        n = 0;
        flag = 1;
    }

    probka = Read_mcasp1_rcv();
    x[n] = (float)((short)probka);
    s[n>>1] = x[n];
    x2[n] = x[n] * b[n>>1];
    x3[n] = x[n] * h[n>>1];
    x4[n] = x[n];
    // remember signal*window in array
    sw2[n>>1] = x2[n];
    sw3[n>>1] = x3[n];
    // Add zeros in range of 1024 to 1535
    x4[n+N*2] = 0.0f;
    ++n;
    // Add zeros as interleaved imaginary numbers
    x[n] = 0.0f;
    x2[n] = 0.0f;
    x3[n] = 0.0f;
    x4[n] = 0.0f;
    // Add zeros in range of 1536 to 2047
    x4[n+N*2] = 0.0f;
    ++n;

    if(n >= N_mask)
    {
        DSPF_sp_cfftr2_dit(x, w, N);
        bit_rev(x, N);
        DSPF_sp_cfftr2_dit(x2, w, N);
        bit_rev(x2, N);
        DSPF_sp_cfftr2_dit(x3, w, N);
        bit_rev(x3, N);
        DSPF_sp_cfftr2_dit(x4, w2, N2);
        bit_rev(x4, N2);
        int i = 0;
        for(i; i < N; i++)
        {
            m[i] = sqrt(x[2*i]*x[2*i] + x[2*i+1]*x[2*i+1]);
            m2[i] = sqrt(x2[2*i]*x2[2*i] + x2[2*i+1]*x2[2*i+1]);
            m3[i] = sqrt(x3[2*i]*x3[2*i] + x3[2*i+1]*x3[2*i+1]);
        }
        i = 0;
        for(i; i < N2; i++)
        {
            m4[i] = sqrt(x4[2*i]*x4[2*i] + x4[2*i+1]*x4[2*i+1]);
        }

        flag = 0;
    }

    // if condition takes too long. It is better to do logical multiply
    n &= N_mask;    // n = n & N_mask;

    Write_mcasp1_xmt(probka);
    Restart_mcasp1_if_error();  //Must be the last line of INT
}

void pierwszy_task(UArg arg0, UArg arg1)
{
//    printf("Start pierwszy_task\n\r");
    Config_i2c_and_codec();
    n = 0;
    n_h = 0;
    tw_genr2fft(w, N);
    bit_rev(w, N>>1);
    tw_genr2fft(w2, N2);
    bit_rev(w2, N);
    int i = 0;
    // Generate Hanning window
    for(i; i < N; i++)
    {
        h[i] = 0.5*(1 - cos((2*PI*i)/(N)));
    }
    // Generate Bartlett window
    i = 0;
    for (i; i < N; i++)
    {
        b[i] = 1 - fabs(((float)i - N/2)/(N/2));
    }

    Config_and_start_mcasp1(); // Do not add anything after that lines
    Hwi_enableInterrupt(13);

    while(1)
    {

    }
}
