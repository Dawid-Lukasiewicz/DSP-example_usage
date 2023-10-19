#include <stdio.h>
#include <math.h>

// #include "codec/codec_v1_0.c"
#include "fft/DSPF_sp_cfftr2_dit.h"
#include "fft/utility.h"

#ifdef LINUX_IMPL
#include "fftw3.h"
#endif

#define N               512
#define N_mask          511
#define N2              (2*(N))
#define N2_mask         ((N2)-1)

#define V               1
#define FREQ            1000
#define SAMPLE_FREQ     48000
#define T               (1/(FREQ))


static void przerwanie_rcv();
static void pierwszy_task();
static void generate_sin(float *x, int n);

#ifdef LINUX_IMPL
static void linux_fft();
static void generate_sin_linux(fftw_complex *x, int n);
#endif

float probka;  //In this case probka needs to be float, not int
short n;
float x[N2];
float w[N];
int tmp;

short n_read = 0;
float Read_mcasp1_rcv[N];

int main(void)
{
    pierwszy_task();
    for (int i = 0; i < N*8; i++)
    {
#ifndef LINUX_IMPL
        przerwanie_rcv();
#else
        linux_fft();
#endif
    }

    return(0);
}

static void przerwanie_rcv()
{
    // probka = Read_mcasp1_rcv();
    probka = Read_mcasp1_rcv[n_read];
    ++n_read;

    n_read &= N_mask;

    // x[n] = (float)((short)probka);
    x[n] = probka; // We also do not need the casting to short
    ++n;
    x[n] = 0.0f;
    ++n;


    // if condition takes too long. It is better to do logical multiply
    n &= N2_mask;    // n = n & N_mask;

    if(n == 0)
    {
       ++tmp;
        DSPF_sp_cfftr2_dit(x, w, N);
    }
    // Write_mcasp1_xmt(probka);
    // Restart_mcasp1_if_error();  //Must be the last line of INT
}

static void pierwszy_task()
{
    // Config_i2c_and_codec();
    generate_sin(Read_mcasp1_rcv, N);
    n_read = 0;
    n = 0;
    tw_genr2fft(w, N);
    // bit_rev(w, N>>1);

    // Config_and_start_mcasp1(); // Do not add anything after that lines
    // Hwi_enableInterrupt(13);
    // while(1)
    // {

    // }
}

#ifdef LINUX_IMPL
static void linux_fft()
{
    fftw_complex *in, *out;
    fftw_plan p;
    in = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * N);
    out = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * N);
    generate_sin_linux(in, N);
    p = fftw_plan_dft_1d(N, in, out, FFTW_FORWARD, FFTW_ESTIMATE);

    fftw_execute(p);

    fftw_destroy_plan(p);
    fftw_free(in); fftw_free(out);
}

static void generate_sin_linux(fftw_complex *x, int n)
{
    double t = 0.0;
    double time_interval = 2*PI*(((double)FREQ/(double)SAMPLE_FREQ));
    for (int i = 0; i < n; i++)
    {
        x[i][0] = V * sin(t);
        x[i][1] = 0.0;
        t += time_interval;
    }
}
#endif

static void generate_sin(float *x, int n)
{
    float t = 0.0;
    float time_interval = 2*PI*(((float)FREQ/(float)SAMPLE_FREQ));
    for (int i = 0; i < n; i++)
    {
        x[i] = V * sin(t);
        t += time_interval;
    }
}
