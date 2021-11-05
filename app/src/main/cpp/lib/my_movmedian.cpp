/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 * File: my_movmedian.c
 *
 * MATLAB Coder version            : 5.2
 * C/C++ source code generated on  : 05-Nov-2021 16:55:26
 */

/* Include Files */
#include "my_movmedian.h"
#include "my_movmedian_data.h"
#include "my_movmedian_emxutil.h"
#include "my_movmedian_initialize.h"
#include "my_movmedian_types.h"
#include "quickselect.h"
#include <math.h>

/* Variable Definitions */
static emxArray_real_T *buffer;

static boolean_T buffer_not_empty;

/* Function Definitions */
/*
 * function y = my_movmedian(x, order)
 *
 * Arguments    : double x
 *                double order
 * Return Type  : double
 */
double my_movmedian(double x, double order)
{
  static double id;
  emxArray_real_T *a__4;
  double q;
  double r;
  double y;
  int a__6;
  int ilast;
  int loop_ub_tmp;
  int midm1;
  boolean_T rEQ0;
  if (!isInitialized_my_movmedian) {
    my_movmedian_initialize();
  }
  /* 'my_movmedian:5' if(isempty(buffer)) */
  if (!buffer_not_empty) {
    /* 'my_movmedian:6' buffer = zeros(1, order); */
    a__6 = buffer->size[0] * buffer->size[1];
    buffer->size[0] = 1;
    loop_ub_tmp = (int)order;
    buffer->size[1] = (int)order;
    emxEnsureCapacity_real_T(buffer, a__6);
    for (a__6 = 0; a__6 < loop_ub_tmp; a__6++) {
      buffer->data[a__6] = 0.0;
    }
    buffer_not_empty = (buffer->size[1] != 0);
    /* 'my_movmedian:7' id = 0; */
    id = 0.0;
  }
  /* 'my_movmedian:10' buffer( mod(id, order) + 1 ) = x; */
  r = id;
  if (order == 0.0) {
    if (id == 0.0) {
      r = order;
    }
  } else if (id == 0.0) {
    r = 0.0 / order;
  } else {
    r = fmod(id, order);
    rEQ0 = (r == 0.0);
    if ((!rEQ0) && (order > floor(order))) {
      q = fabs(id / order);
      rEQ0 = (fabs(q - floor(q + 0.5)) <= 2.2204460492503131E-16 * q);
    }
    if (rEQ0) {
      r = 0.0;
    } else if (order < 0.0) {
      r += order;
    }
  }
  buffer->data[(int)(r + 1.0) - 1] = x;
  /* 'my_movmedian:11' id = id + 1; */
  id++;
  /* 'my_movmedian:12' y = median(buffer); */
  if (buffer->size[1] == 0) {
    y = 0.0;
  } else if (buffer->size[1] <= 4) {
    if (buffer->size[1] == 0) {
      y = 0.0;
    } else if (buffer->size[1] == 1) {
      y = buffer->data[0];
    } else if (buffer->size[1] == 2) {
      if ((buffer->data[0] < 0.0) != (buffer->data[1] < 0.0)) {
        y = (buffer->data[0] + buffer->data[1]) / 2.0;
      } else {
        y = buffer->data[0] + (buffer->data[1] - buffer->data[0]) / 2.0;
      }
    } else if (buffer->size[1] == 3) {
      if (buffer->data[0] < buffer->data[1]) {
        if (buffer->data[1] < buffer->data[2]) {
          a__6 = 1;
        } else if (buffer->data[0] < buffer->data[2]) {
          a__6 = 2;
        } else {
          a__6 = 0;
        }
      } else if (buffer->data[0] < buffer->data[2]) {
        a__6 = 0;
      } else if (buffer->data[1] < buffer->data[2]) {
        a__6 = 2;
      } else {
        a__6 = 1;
      }
      y = buffer->data[a__6];
    } else {
      if (buffer->data[0] < buffer->data[1]) {
        if (buffer->data[1] < buffer->data[2]) {
          loop_ub_tmp = 0;
          a__6 = 1;
          ilast = 2;
        } else if (buffer->data[0] < buffer->data[2]) {
          loop_ub_tmp = 0;
          a__6 = 2;
          ilast = 1;
        } else {
          loop_ub_tmp = 2;
          a__6 = 0;
          ilast = 1;
        }
      } else if (buffer->data[0] < buffer->data[2]) {
        loop_ub_tmp = 1;
        a__6 = 0;
        ilast = 2;
      } else if (buffer->data[1] < buffer->data[2]) {
        loop_ub_tmp = 1;
        a__6 = 2;
        ilast = 0;
      } else {
        loop_ub_tmp = 2;
        a__6 = 1;
        ilast = 0;
      }
      if (buffer->data[loop_ub_tmp] < buffer->data[3]) {
        if (buffer->data[3] < buffer->data[ilast]) {
          if ((buffer->data[a__6] < 0.0) != (buffer->data[3] < 0.0)) {
            y = (buffer->data[a__6] + buffer->data[3]) / 2.0;
          } else {
            y = buffer->data[a__6] +
                (buffer->data[3] - buffer->data[a__6]) / 2.0;
          }
        } else if ((buffer->data[a__6] < 0.0) != (buffer->data[ilast] < 0.0)) {
          y = (buffer->data[a__6] + buffer->data[ilast]) / 2.0;
        } else {
          y = buffer->data[a__6] +
              (buffer->data[ilast] - buffer->data[a__6]) / 2.0;
        }
      } else if ((buffer->data[loop_ub_tmp] < 0.0) !=
                 (buffer->data[a__6] < 0.0)) {
        y = (buffer->data[loop_ub_tmp] + buffer->data[a__6]) / 2.0;
      } else {
        y = buffer->data[loop_ub_tmp] +
            (buffer->data[a__6] - buffer->data[loop_ub_tmp]) / 2.0;
      }
    }
  } else {
    midm1 = buffer->size[1] >> 1;
    emxInit_real_T(&a__4, 2);
    if ((buffer->size[1] & 1) == 0) {
      a__6 = a__4->size[0] * a__4->size[1];
      a__4->size[0] = 1;
      a__4->size[1] = buffer->size[1];
      emxEnsureCapacity_real_T(a__4, a__6);
      loop_ub_tmp = buffer->size[1];
      for (a__6 = 0; a__6 < loop_ub_tmp; a__6++) {
        a__4->data[a__6] = buffer->data[a__6];
      }
      quickselect(a__4, midm1 + 1, buffer->size[1], &y, &loop_ub_tmp, &ilast);
      if (midm1 < loop_ub_tmp) {
        quickselect(a__4, midm1, ilast - 1, &r, &loop_ub_tmp, &a__6);
        if ((y < 0.0) != (r < 0.0)) {
          y = (y + r) / 2.0;
        } else {
          y += (r - y) / 2.0;
        }
      }
    } else {
      a__6 = a__4->size[0] * a__4->size[1];
      a__4->size[0] = 1;
      a__4->size[1] = buffer->size[1];
      emxEnsureCapacity_real_T(a__4, a__6);
      loop_ub_tmp = buffer->size[1];
      for (a__6 = 0; a__6 < loop_ub_tmp; a__6++) {
        a__4->data[a__6] = buffer->data[a__6];
      }
      quickselect(a__4, midm1 + 1, buffer->size[1], &y, &loop_ub_tmp, &a__6);
    }
    emxFree_real_T(&a__4);
  }
  return y;
}

/*
 * function y = my_movmedian(x, order)
 *
 * Arguments    : void
 * Return Type  : void
 */
void my_movmedian_free(void)
{
  emxFree_real_T(&buffer);
}

/*
 * function y = my_movmedian(x, order)
 *
 * Arguments    : void
 * Return Type  : void
 */
void my_movmedian_init(void)
{
  emxInit_real_T(&buffer, 2);
  buffer_not_empty = false;
}

/*
 * File trailer for my_movmedian.c
 *
 * [EOF]
 */
