/*
Copyright (C) 2006 Pedro Felzenszwalb

This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307 USA
*/

/* simple filters */

#ifndef FILTER_H
#define FILTER_H

#include <vector>
#include <cmath>
#include "image.h"
#include "misc.h"
#include "convolve.h"
#include "imconv.h"

#define WIDTH 4.0

/* normalize mask so it integrates to one */
static void normalize(std::vector<float> &mask) {
    int len = mask.size();
    float sum = 0;
    for (int i = 1; i < len; i++) {
	sum += fabs(mask[i]);
    }
    sum = 2*sum + fabs(mask[0]);
    for (int i = 0; i < len; i++) {
	mask[i] /= sum;
    }
}

/* make filters */
#define MAKE_FILTER(name, fun)                                \
static std::vector<float> make_ ## name (float sigma) {       \
  sigma = std::max(sigma, 0.01F);			      \
  int len = (int)ceil(sigma * WIDTH) + 1;                     \
  std::vector<float> mask(len);                               \
  for (int i = 0; i < len; i++) {                             \
    mask[i] = (float)fun;                                            \
  }                                                           \
  return mask;                                                \
}

MAKE_FILTER(fgauss, exp(-0.5*square(i/sigma)));

/* convolve image with gaussian filter */
static image<float> *smooth(image<float> *src, float sigma) {
  std::vector<float> mask = make_fgauss(sigma);
  normalize(mask);

  image<float> *tmp = new image<float>(src->height(), src->width(), false);
  image<float> *dst = new image<float>(src->width(), src->height(), false);
  convolve_even(src, tmp, mask);
  convolve_even(tmp, dst, mask);

  delete tmp;
  return dst;
}

/* convolve image with gaussian filter */
image<float> *smooth(image<uchar> *src, float sigma) {
  image<float> *tmp = imageUCHARtoFLOAT(src);
  image<float> *dst = smooth(tmp, sigma);
  delete tmp;
  return dst;
}
#endif
