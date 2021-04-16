/*
 * File: kalamFilter.c
 *
 * MATLAB Coder version            : 4.1
 * C/C++ source code generated on  : 16-Apr-2021 13:22:46
 */

/* Include Files */
#include <math.h>
#include "kalamFilter.h"

/* Function Definitions */

/*
 * KALAMFILTER 此处显示有关此函数的摘要
 *    此处显示详细说明
 * Arguments    : const float x[3]
 *                float altitude
 *                float acc
 *                float P[9]
 *                float filterx[3]
 *                float TEMPP[9]
 * Return Type  : void
 */
void kalamFilterFusion(const float x[3], float altitude, float acc, float P[9], float
                 filterx[3], float TEMPP[9])
{
  int coffset;
  int boffset;
  int a_tmp;
  float c[4];
  float a[9];
  float r;
  static const float b[9] = { 1.0F, 0.02F, 0.0002F, 0.0F, 1.0F, 0.02F, 0.0F,
    0.0F, 1.0F };

  static const float fv0[9] = { 1.0E-5F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F,
    0.0F, 0.001F };

  static const float b_a[9] = { 1.0F, 0.0F, 0.0F, 0.02F, 1.0F, 0.0F, 0.0002F,
    0.02F, 1.0F };

  float t;
  float y[4];
  float c_a[6];
  static const signed char d_a[6] = { 1, 0, 0, 0, 0, 1 };

  static const signed char b_b[6] = { 1, 0, 0, 0, 0, 1 };

  static const float fv1[4] = { 0.0112F, 0.0F, 0.0F, 0.1F };

  float b_c[6];
  static const signed char iv0[9] = { 1, 0, 0, 0, 1, 0, 0, 0, 1 };

  float c_data[9];
  float b_altitude[2];
  int k;
  int aoffset;
  int temp;
  static const signed char iv1[6] = { 1, 0, 0, 0, 0, 1 };

  float c_data_idx_2;
  for (coffset = 0; coffset < 3; coffset++) {
    filterx[coffset] = 0.0F;
    for (boffset = 0; boffset < 3; boffset++) {
      a_tmp = coffset + 3 * boffset;
      a[a_tmp] = 0.0F;
      a[a_tmp] = (b_a[coffset] * P[3 * boffset] + b_a[coffset + 3] * P[1 + 3 *
                  boffset]) + b_a[coffset + 6] * P[2 + 3 * boffset];
      filterx[coffset] += b_a[a_tmp] * x[boffset];
    }
  }

  for (coffset = 0; coffset < 3; coffset++) {
    for (boffset = 0; boffset < 3; boffset++) {
      a_tmp = coffset + 3 * boffset;
      P[a_tmp] = ((a[coffset] * b[3 * boffset] + a[coffset + 3] * b[1 + 3 *
                   boffset]) + a[coffset + 6] * b[2 + 3 * boffset]) + fv0[a_tmp];
    }
  }

  for (coffset = 0; coffset < 2; coffset++) {
    for (boffset = 0; boffset < 3; boffset++) {
      a_tmp = coffset + (boffset << 1);
      c_a[a_tmp] = 0.0F;
      c_a[a_tmp] = ((float)d_a[coffset] * P[3 * boffset] + 0.0F * P[1 + 3 *
                    boffset]) + (float)d_a[coffset + 4] * P[2 + 3 * boffset];
    }

    for (boffset = 0; boffset < 2; boffset++) {
      a_tmp = coffset + (boffset << 1);
      c[a_tmp] = ((c_a[coffset] * (float)b_b[3 * boffset] + c_a[coffset + 2] *
                   0.0F) + c_a[coffset + 4] * (float)b_b[2 + 3 * boffset]) +
        fv1[a_tmp];
    }
  }

  if (fabsf(c[1]) > fabsf(c[0])) {
    r = c[0] / c[1];
    t = 1.0F / (r * c[3] - c[2]);
    y[0] = c[3] / c[1] * t;
    y[1] = -t;
    y[2] = -c[2] / c[1] * t;
    y[3] = r * t;
  } else {
    r = c[1] / c[0];
    t = 1.0F / (c[3] - r * c[2]);
    y[0] = c[3] / c[0] * t;
    y[1] = -r * t;
    y[2] = -c[2] / c[0] * t;
    y[3] = t;
  }

  for (coffset = 0; coffset < 3; coffset++) {
    for (boffset = 0; boffset < 2; boffset++) {
      a_tmp = coffset + 3 * boffset;
      c_a[a_tmp] = 0.0F;
      c_a[a_tmp] = (P[coffset] * (float)b_b[3 * boffset] + P[coffset + 3] * 0.0F)
        + P[coffset + 6] * (float)b_b[2 + 3 * boffset];
    }

    b_c[coffset] = 0.0F;
    r = c_a[coffset + 3];
    b_c[coffset] = c_a[coffset] * y[0] + r * y[1];
    b_c[coffset + 3] = 0.0F;
    b_c[coffset + 3] = c_a[coffset] * y[2] + r * y[3];
  }

  for (a_tmp = 0; a_tmp < 3; a_tmp++) {
    coffset = a_tmp * 3;
    boffset = a_tmp << 1;
    c_data[coffset] = 0.0F;
    c_data[coffset + 1] = 0.0F;
    c_data[coffset + 2] = 0.0F;
    for (k = 0; k < 2; k++) {
      aoffset = k * 3;
      temp = iv1[boffset + k];
      c_data[coffset] += (float)temp * b_c[aoffset];
      c_data[coffset + 1] += (float)temp * b_c[aoffset + 1];
      c_data[coffset + 2] += (float)temp * b_c[aoffset + 2];
    }
  }

  for (coffset = 0; coffset < 9; coffset++) {
    a[coffset] = (float)iv0[coffset] - c_data[coffset];
  }

  for (coffset = 0; coffset < 3; coffset++) {
    for (boffset = 0; boffset < 3; boffset++) {
      a_tmp = coffset + 3 * boffset;
      TEMPP[a_tmp] = 0.0F;
      TEMPP[a_tmp] = (a[coffset] * P[3 * boffset] + a[coffset + 3] * P[1 + 3 *
                      boffset]) + a[coffset + 6] * P[2 + 3 * boffset];
    }
  }

  b_altitude[0] = altitude;
  b_altitude[1] = acc;
  for (coffset = 0; coffset < 2; coffset++) {
    b_altitude[coffset] -= ((float)d_a[coffset] * filterx[0] + 0.0F * filterx[1])
      + (float)d_a[coffset + 4] * filterx[2];
  }

  r = 0.0F;
  t = 0.0F;
  c_data_idx_2 = 0.0F;
  for (k = 0; k < 2; k++) {
    aoffset = k * 3;
    r += b_altitude[k] * b_c[aoffset];
    t += b_altitude[k] * b_c[aoffset + 1];
    c_data_idx_2 += b_altitude[k] * b_c[aoffset + 2];
  }

  filterx[0] += r;
  filterx[1] += t;
  filterx[2] += c_data_idx_2;
}

/*
 * File trailer for kalamFilter.c
 *
 * [EOF]
 */
