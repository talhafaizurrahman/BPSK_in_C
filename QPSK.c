#include "liquid/liquid.h"
#include <stdio.h>
#include <stdlib.h>
#include <complex.h>
#include <math.h>

int main()
{

    float SNRdB_min = -0.0f;       // starting SNR value
    float SNRdB_max = 30.0f;       // ending SNR value
    float SNRdB_step = 1.0f;       // step size
    unsigned int num_trials = 1e6; // number of symbols

    modulation_scheme ms = LIQUID_MODEM_BPSK;
    modem mod = modem_create(ms);
    modem demod = modem_create(ms); 

    unsigned int m = modem_get_bps(mod);
    unsigned int M = 1 << m;
    int N = 100000;

    unsigned int sym_in;
    unsigned int sym_out; 

    float complex sample;

    float SNRdB = SNRdB_min; // set SNR value
    float nstd = powf(10.0f, -SNRdB / 20.0f);

    unsigned int error_count = 0;

    for (unsigned int i = 0; i < N; i++)
    {
        sym_in = rand() % M;

        modem_modulate(mod, sym_in, &sample);

        sample += nstd * (randnf() + _Complex_I * randnf()) / sqrtf(2.0f);

        modem_demodulate(demod, sample, &sym_out); 

        error_count += count_bit_errors(sym_in, sym_out); 
    }

    printf("%12.8f", (float)error_count/N); 
    modem_destroy(mod);
    modem_destroy(demod);
    return 0;
}