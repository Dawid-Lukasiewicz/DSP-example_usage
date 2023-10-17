/*
 * main.c
 *
 *  Created on: 10 pa� 2023
 *      Author: Stud9
 */

#include <stdio.h>
#include <math.h>
// #include <ti/sysbios/BIOS.h>
// #include <xdc/cfg/global.h> // for Hwi_enableInterrupt(13)
// #include "codec/codec_v1_0.c"
//#include "fft/DSPF_sp_cfftr2_dit.h"
//#include "fft/utility.h"
#include "fft/DSPF_sp_cfftr2_dit.c"
#include "fft/utility.c"

#define N 512
#define N_mask 511

static void przerwanie_rcv();
static void pierwszy_task();

int probka;
short n;
float x[N*2];
float w[N];
//int tmp = 0;

int main(void)
{
    pierwszy_task();
    for (int i = 0; i < N*8; i++)
    {
        przerwanie_rcv();
    }

    return(0);
}

static void przerwanie_rcv()
{
    // probka = Read_mcasp1_rcv();
    x[n] = (float)((short)probka);
    ++n;
    x[n] = 0.0f;
    ++n;


    // if condition takes too long. It is better to do logical multiply
    n &= N_mask;    // n = n & N_mask;

    if(n == 0)
    {
//        ++tmp;
        DSPF_sp_cfftr2_dit(x, w, N);
    }
    // Write_mcasp1_xmt(probka);
    // Restart_mcasp1_if_error();  //Must be the last line of INT
}

static void pierwszy_task()
{
    // Config_i2c_and_codec();
    n = 0;
    tw_genr2fft(w, N);
    bit_rev(w, N>>1);

    // Config_and_start_mcasp1(); // Do not add anything after that lines
    // Hwi_enableInterrupt(13);
    // while(1)
    // {

    // }
}
