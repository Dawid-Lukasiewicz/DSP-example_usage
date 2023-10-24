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

int probka;
short n;
short n_h;
float x[N*2];
float w[N];
float m[N];
float h[N];
int tmp = 0;
int flag = 0;

int main(void)
{

    Pins_config();
//    printf("Program lab2 started\n\r");
    BIOS_start();
    return(0);
}

void przerwanie_rcv()
{

    if ((n/2) >= 2 && !flag)
    {
        n = 0;
        flag = 1;
    }

    probka = Read_mcasp1_rcv();
    x[n] = (float)((short)probka);
    x[n] = x[n] * h[n_h];
    ++n_h;
    ++n;
    x[n] = 0.0f;
    ++n;

    if(n >= N_mask)
    {
        tmp++;
        DSPF_sp_cfftr2_dit(x, w, N);
        bit_rev(x, N);
        int i = 0;
        for(i; i < N; i++)
        {
            m[i] = sqrt(x[2*i]*x[2*i] + x[2*i+1]*x[2*i+1]);
        }
        flag = 0;
        ++tmp;
    }

    // if condition takes too long. It is better to do logical multiply
    n &= N_mask;    // n = n & N_mask;
    n_h &= 511;
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
    int i = 0;
    for(i; i < N; i++)
    {
        h[i] = 0.5*(1 - cos((2*PI*i)/(N-1)));
    }

    Config_and_start_mcasp1(); // Do not add anything after that lines
    Hwi_enableInterrupt(13);
    while(1)
    {

    }
}
