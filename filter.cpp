#include <hls_stream.h>
#include <ap_axi_sdata.h>
#include <math.h>

typedef ap_axis<32,1,1,1> AXI_VAL;
typedef int data_t;
typedef int coef_t;
typedef int acc_t;

#define N 4
#define PI 3.14159265358979323846

void filt (hls::stream<AXI_VAL>& y, coef_t c[N], hls::stream<AXI_VAL>& x, int sampling_rate, int bandwidth, int max_freq) {
#pragma HLS INTERFACE m_axi depth=11 port=c
#pragma HLS INTERFACE axis register both port=x
#pragma HLS INTERFACE axis register both port=y
#pragma HLS INTERFACE s_axilite port=sampling_rate
#pragma HLS INTERFACE s_axilite port=bandwidth
#pragma HLS INTERFACE s_axilite port=max_freq
#pragma HLS INTERFACE ap_ctrl_none port=return

  // Calculate filter coefficients based on the given parameters
  // You need to implement this part based on your bandpass filter design

  // Example: Generate coefficients
  // coef_t taps[N] = {0,-10,-9,23,56,63,56,23,-9,-10,0};
  // For demonstration purpose, you need to replace this with your bandpass filter design
	float lowcut = 300; //Low cutoff frequency
	float highcut = 500;  // High cutoff frequency
	float nyquist = 0.5 * sampling_rate;
	float low = lowcut / nyquist;
	float high = highcut / nyquist;

	for (int i = 0; i < N; i++) {

	    if (i - N / 2 == 0) {

	        c[i] = 2 * (high - low);
	    }

	    else {
	        c[i] = (sinf(2 * PI * high * (i - N / 2)) - sinf(2 * PI * low * (i - N / 2))) / (PI * (i - N / 2));
	        }
	    }

  while(1) {
    static data_t shift_reg[N];
    acc_t acc;
    data_t data;
    acc=0;
    AXI_VAL tmp1;
    x.read(tmp1);
    int i;
    Shift_Accum_Loop:
    for (i = N - 1; i > 0; i--) {
      #pragma HLS UNROLL
      shift_reg[i] = shift_reg[i - 1];
      acc += shift_reg[i] * c[i];
    }
    acc += tmp1.data.to_int() * c[0];
    shift_reg[0] = tmp1.data.to_int();
    AXI_VAL output;
    output.data = acc;
    output.keep = tmp1.keep;
    output.strb = tmp1.strb;
    output.last = tmp1.last;
    output.dest = tmp1.dest;
    output.id = tmp1.id;
    output.user = tmp1.user;
    y.write(output);
    if (tmp1.last) {
      break;
    }
  }
}
