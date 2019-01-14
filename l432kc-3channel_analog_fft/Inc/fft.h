/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __FFT_H__
#define __FFT_H__

float window(uint16_t *x_t, uint32_t m0, float *f, uint32_t len);
void realft(float data[], uint32_t n, int32_t isign);
uint32_t convert_fft_to_modamps(float *f, uint32_t len_f, uint32_t max_amp);

#endif
