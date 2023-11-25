#include <stdio.h>
#include <math.h>

// #include "codec/codec_v1_0.c"
#include "fft/DSPF_sp_cfftr2_dit.h"
#include "fft/utility.h"

#include "tinywav/tinywav.h"

#define N               512
#define N_mask          511
#define N2              (2*(N))
#define N2_mask         ((N2)-1)

#define V               1
#define FREQ_SINUS      1000
#define SAMPLE_FREQ     48000
#define T               (1/(FREQ_SINUS))

// Function for signal processing
static void przerwanie_rcv();
static void pierwszy_task();
static void generate_sin(float *x, int n);
static void generate_hanning(float *x, int n);
static void generate_bartlett(float *x, int n);
static int  open_wav_files();
static void open_files();
static void close_files();

float probka;  //In this case probka needs to be float, not int
short n;
float x[N2*2];
float w[N];
float m[N];
float h[N];
float b[N];
int tmp;

float Read_mcasp1_rcv[N];

FILE *signal_file;
FILE *windowed_signal_file;
FILE *windowed_whole_signal_file;
FILE *magnitude_file;
FILE *hanning_file;
FILE *bartlett_file;

TinyWav input_wav;

int main(void)
{
    int err = open_wav_files();
    if(err) return 1;

    open_files();
    pierwszy_task();
    for (int i = 0; i < N; i++)
    {
        przerwanie_rcv();
    }
    close_files();
    return(0);
}

static void przerwanie_rcv()
{
    probka = Read_mcasp1_rcv[n>>1];
    x[n] =  probka; // We also do not need the casting to short
    x[n] *= b[n>>1];
    x[n+1] = 0.0f;

    // Write signal sample to csv file
    fprintf(signal_file, "%.2f,", probka);
    fprintf(windowed_signal_file, "%.2f,", x[n]);

    // Add zeros at the end  of signal
    x[n+N2] = 0.0f;
    x[n+1+N2] = 0.0f;
    n += 2;

    // if condition takes too long. It is better to do logical multiply
    n &= N2_mask;    // n = n & N_mask;

    if(n == 0)
    {
       ++tmp;
       for (int i = 0; i < N2*2; i++)
       {
            fprintf(windowed_whole_signal_file, "%.2f,", x[i]);
       }

        DSPF_sp_cfftr2_dit(x, w, N2);
        bit_rev(x, N2);
        for (int i = 0; i < N2; i++)
        {
            m[i] = sqrt(x[2*i]*x[2*i] + x[2*i+1]*x[2*i+1]);
            fprintf(magnitude_file, "%.2f,", m[i]);
        }

    }
}

static void pierwszy_task()
{
    // generate_sin(Read_mcasp1_rcv, N);
    printf("NumChannels=%d, SampleRate=%d", input_wav.h.NumChannels, input_wav.h.SampleRate);
    generate_hanning(h, N);
    generate_bartlett(b, N);
    n = 0;
    tw_genr2fft(w, N2);
    bit_rev(w, N2>>1);
}

static void generate_sin(float *x, int n)
{
    float t = 0.0;
    float time_interval = 2*PI*(((float)FREQ_SINUS/(float)SAMPLE_FREQ));
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
        fprintf(hanning_file, "%.2f,", h[i]);
    }
}

static void generate_bartlett(float *b, int n)
{
    float L = (float)n;
    for (int i = 0; i < n; i++)
    {
        b[i] = 1 - fabs(((float)i - L/2)/(L/2));
        fprintf(bartlett_file, "%.2f,", b[i]);
    }
}

static int open_wav_files()
{
    int err = tinywav_open_read(&input_wav
                    ,"/home/dawid/Projects/studia/mgr/II_sem/AplikacjeProcesorowSygnalowych/dsp_project_linux/resources/dialdtmf_wav_short/0.wav"
                    ,TW_INLINE);
    if(err)
    {
        fprintf(stderr, "Error at reading audio file");
        return 1;
    }
}

static void open_files()
{
    signal_file                 = fopen("signal_file.csv", "w");
    windowed_signal_file        = fopen("windowed_signal_file.csv", "w");
    windowed_whole_signal_file  = fopen("windowed_whole_signal_file.csv", "w");
    magnitude_file              = fopen("magnitude_file.csv", "w");
    hanning_file                = fopen("hanning_file.csv", "w");
    bartlett_file               = fopen("bartlett_file.csv", "w");
}

static void close_files()
{
    fclose(signal_file);
    fclose(windowed_signal_file);
    fclose(windowed_whole_signal_file);
    fclose(magnitude_file);
    fclose(hanning_file);
    fclose(bartlett_file);
    tinywav_close_read(&input_wav);
}