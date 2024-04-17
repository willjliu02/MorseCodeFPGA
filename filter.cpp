#include "fir.h"
#include <math.h>

void band_pass_filter (
    data_t *y,
    coef_t c[N],
    data_t x,
    double max_freq,
    double sampling_rate,
    double bandwidth
) {
    static data_t shift_reg[N];
    acc_t acc;
    data_t data;
    int i;

    acc = 0;

    // Calculate lower and upper cutoff frequencies
    double lower_cutoff = max_freq - bandwidth / 2.0;
    double upper_cutoff = max_freq + bandwidth / 2.0;

    // Generate band-pass filter coefficients
    for (i = 0; i < N; i++) {
        double t = (double)(i - (N-1)/2) / sampling_rate;
        if (i == (N-1)/2) {
            c[i] = 2 * M_PI * (upper_cutoff - lower_cutoff);
        } else {
            c[i] = (sin(2 * M_PI * upper_cutoff * t) - sin(2 * M_PI * lower_cutoff * t)) / (M_PI * t);
        }
    }

    // Apply filter
    Shift_Accum_Loop: for (i = N - 1; i >= 0; i--) {
        if (i == 0) {
            shift_reg[0] = x;
            data = x;
        } else {
            shift_reg[i] = shift_reg[i - 1];
            data = shift_reg[i];
        }
        acc += data * c[i];
    }

    *y = acc;
}
