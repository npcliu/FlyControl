/*
 * File: kalamFilter.h
 *
 * MATLAB Coder version            : 4.1
 * C/C++ source code generated on  : 16-Apr-2021 13:22:46
 */

#ifndef KALAMFILTER_H
#define KALAMFILTER_H

/* Include Files */
#include <stddef.h>
#include <stdlib.h>
#include "rtwtypes.h"


/* Function Declarations */
extern void kalamFilterFusion(const float x[3], float altitude, float acc, float P[9],
  float filterx[3], float TEMPP[9]);

#endif

/*
 * File trailer for kalamFilter.h
 *
 * [EOF]
 */
