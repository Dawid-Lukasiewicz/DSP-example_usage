#ifndef DTMF
#define DTMF

/*

        1209Hz	1336Hz	1477Hz	1633Hz

697Hz     1       2       3       A

770Hz     4       5       6       B

852Hz     7       8       9       C

941Hz     *       0       #       D

*/

#define DTMF_R1 697
#define DTMF_R2 770
#define DTMF_R3 852
#define DTMF_R4 941
#define DTMF_C1 1209
#define DTMF_C2 1336
#define DTMF_C3 1477
#define DTMF_C4 1633

/*
Fk = k*Fs/N
*/

float dtmf_find_peak(float *magnitude_spectrum, int N);
void dtmf_find_samples(float *magnitude_spectrum, int *k1, int *k2, int N);
char dtmf_decode(float Fs, int k1, int k2, int N);


#endif //DTMF