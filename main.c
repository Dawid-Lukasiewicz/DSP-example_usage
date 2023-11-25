#include <stdio.h>
#include <math.h>

// #include "codec/codec_v1_0.c"
#include "fft/DSPF_sp_cfftr2_dit.h"
#include "fft/utility.h"

#include "tinywav/tinywav.h"
#include "dtmf.h"

#define N               512
#define N_mask          511
#define N2              (2*(N))
#define N2_mask         ((N2)-1)

#define T               (1/(FREQ_SINUS))

// Function for signal processing
static void przerwanie_rcv();
static void pierwszy_task(bool flag);
static void generate_sin(float *x, int n);
static void generate_hanning(float *x, int n);
static void generate_bartlett(float *x, int n);
static int  open_wav_files();
static int open_files(bool flag, char *input_path);
static void close_files(bool flag);

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

int main(int argc, char *argv[])
{
    int8_t flag_generate_singal = (argc > 1 ? true : false);
    int8_t err;
    char* input_path;

    if(flag_generate_singal)
    {
        input_path = argv[1];
    }
    err = open_files(flag_generate_singal, input_path);
    if(err)
    {
        close_files(flag_generate_singal);
        return 1;
    }

    pierwszy_task(flag_generate_singal);
    for (int i = 0; i < N; i++)
    {
        przerwanie_rcv();
    }
    close_files(flag_generate_singal);
    return 0;
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
        int dtmf1, dtmf2;
        dtmf_find_samples(x, &dtmf1, &dtmf2, N2);
        printf("sample_dtmf1=%d, sample_dtmf2=%d\n\r", dtmf1, dtmf2);
    }
}

static void pierwszy_task(bool flag)
{
    if(flag)
    {
        tinywav_read_f(&input_wav, Read_mcasp1_rcv, N);
    }
    else
    {
        generate_sin(Read_mcasp1_rcv, N);
    }
    generate_hanning(h, N);
    generate_bartlett(b, N);
    n = 0;
    tw_genr2fft(w, N2);
    bit_rev(w, N2>>1);
}

static void generate_sin(float *x, int n)
{
    float v = 1.0f;
    float freq_sinus = 1000.0f;
    float sample_freq = 48000.0f;
    float t = 0.0f;
    float time_interval = 2*PI*((freq_sinus/sample_freq));
    for (int i = 0; i < n; i++)
    {
        x[i] = v * sin(t);
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
                    ,"/home/dawid/Projects/studia/mgr/II_sem/AplikacjeProcesorowSygnalowych/dsp_project_linux/resources/dialdtmf_wav_short/6.wav"
                    ,TW_INLINE);
    if(err)
    {
        fprintf(stderr, "Error at reading audio file");
        return 1;
    }
}

static int open_files(bool flag, char *input_path)
{
    int err = 0;
    if(flag)
    {
        err = tinywav_open_read(&input_wav
                    ,input_path
                    ,TW_INLINE);

    }
    if(err)
    {
        fprintf(stderr, "Error at reading audio file");
        return 1;
    }
    signal_file                 = fopen("signal_file.csv", "w");
    if(!signal_file) return 1;

    windowed_signal_file        = fopen("windowed_signal_file.csv", "w");
    if(!windowed_signal_file) return 1;

    windowed_whole_signal_file  = fopen("windowed_whole_signal_file.csv", "w");
    if(!windowed_whole_signal_file) return 1;

    magnitude_file              = fopen("magnitude_file.csv", "w");
    if(!magnitude_file) return 1;

    hanning_file                = fopen("hanning_file.csv", "w");
    if(!hanning_file) return 1;

    bartlett_file               = fopen("bartlett_file.csv", "w");
    if(!bartlett_file) return 1;

    return 0;
}

static void close_files(bool flag)
{
    if(flag && input_wav.f)         tinywav_close_read(&input_wav);

    if(signal_file)                 fclose(signal_file);
    if(windowed_signal_file)        fclose(windowed_signal_file);
    if(windowed_whole_signal_file)  fclose(windowed_whole_signal_file);
    if(magnitude_file)              fclose(magnitude_file);
    if(hanning_file)                fclose(hanning_file);
    if(bartlett_file)               fclose(bartlett_file);
}