#include <stdio.h>
#include <math.h>

// #include "codec/codec_v1_0.c"
#include "fft/DSPF_sp_cfftr2_dit.h"
#include "fft/utility.h"

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
static void generate_hanning(float *x, int n);
static void open_files();
static void close_files();

float probka;  //In this case probka needs to be float, not int
short n;
float x[N2];
float w[N];
float m[N];
float h[N];
int tmp;

short n_read = 0;
float Read_mcasp1_rcv[N];

FILE *signal_file;
FILE *magnitude_file;
FILE *hanning_file;

int main(void)
{
    pierwszy_task();
    open_files();
    for (int i = 0; i < N; i++)
    {
        przerwanie_rcv();
    }
    close_files();
    return(0);
}

static void przerwanie_rcv()
{
    probka = Read_mcasp1_rcv[n_read];
    ++n_read;

    n_read &= N_mask;

    x[n] = probka; // We also do not need the casting to short
    x[n] *= h[n>>1];
    x[n+1] = 0.0f;

    // Write signal sample to csv file
    fprintf(signal_file, "%.2f,", probka);
    fprintf(hanning_file, "%.2f,", x[n]);

    n += 2;

    // if condition takes too long. It is better to do logical multiply
    n &= N2_mask;    // n = n & N_mask;

    if(n == 0)
    {
       ++tmp;
        DSPF_sp_cfftr2_dit(x, w, N);
        bit_rev(x, N);
        for (int i = 0; i < N; i++)
        {
            m[i] = sqrt(x[2*i]*x[2*i] + x[2*i+1]*x[2*i+1]);
            fprintf(magnitude_file, "%.2f,", m[i]);
        }

    }
}

static void pierwszy_task()
{
    generate_sin(Read_mcasp1_rcv, N);
    generate_hanning(h, N);
    n_read = 0;
    n = 0;
    tw_genr2fft(w, N);
    bit_rev(w, N>>1);
}

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

static void generate_hanning(float *h, int n)
{
    for (int i = 0; i < n; i++)
    {
        h[i] = 0.5f*(1-cosf((2*PI*i)/(n-1)));
    }
}

static void open_files()
{
    signal_file     = fopen("signal_file.csv", "w");
    magnitude_file  = fopen("magnitude_file.csv", "w");
    hanning_file    = fopen("hanning_file.csv", "w");
}

static void close_files()
{
    fclose(signal_file);
    fclose(magnitude_file);
    fclose(hanning_file);
}