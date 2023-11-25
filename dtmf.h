#ifndef DTMF
#define DTMF

/*

        1209Hz	1336Hz	1477Hz	1633Hz

697Hz     1       2       3       A

770Hz     4       5       6       B

852Hz     7       8       9       C

941Hz     *       0       #       D

*/

float dtmf_find_peak(float *magnitude_spectrum, int N);
void dtmf_find_samples(float *magnitude_spectrum, int *sample1, int *sample2, int N);
char dtmf_decode(int sample1, int sample2);


#endif //DTMF