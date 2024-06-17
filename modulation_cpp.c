// #include <itpp/itcomm.h>
#include <liquid/liquid.h>
#include <math.h>
#include <complex.h>

int main()
{
  float EbNo_min = 0;
  float EbNo_max = 20;

  float EbNo_db = EbNo_min;

  unsigned int N = 64;
  modulation_scheme ms = LIQUID_MODEM_QPSK;

  modem mod = modem_create(ms);
  modem demod = modem_create(ms);

  unsigned int m = modem_get_bps(mod);
  unsigned int M = 1 << m;

  float SNR_db = log2f(M) + EbNo_min;

  unsigned int i, t, sym_in[N], sym_out[N];
  float complex symbols[N];

  float complex noisySamples[N];

  modem_reset(mod);
  modem_reset(demod);

  unsigned int num_of_errors, total_errors;
  float ber;
  unsigned int num_trails = 10000;
  
  printf("EbNo (dB), BER \n");
  while (EbNo_db <= EbNo_max)
  {
    float nstd = powf(10.0f, -SNR_db / 20.0f);
    total_errors = 0; 
    t = 0;

    while (t < num_trails)
    {
      num_of_errors = 0;

      for (i = 0; i < N; i++)
      {
        sym_in[i] = rand() % M;

        modem_modulate(mod, sym_in[i], &symbols[i]);

        // Add AWGN
        noisySamples[i] = symbols[i] + nstd * (randnf() + _Complex_I * randnf()) / sqrtf(2.0f);

        modem_demodulate(demod, noisySamples[i], &sym_out[i]);

        num_of_errors += count_bit_errors(sym_in[i], sym_out[i]);

        // printf("%12.8f + j*%12.8f \n", crealf(noisySamples[i]), cimagf(noisySamples[i]));
      }

      total_errors += num_of_errors;
      t++;

    } // trail loop
    unsigned int bit_trails = m * N * num_trails;
    ber = (float)total_errors / (float)(bit_trails);
    printf("%12.8f, %12.8f \n", EbNo_db, ber);

    SNR_db = SNR_db + 1.0f; 
    EbNo_db = EbNo_db + 1.0f;
  }

  return 0;
}