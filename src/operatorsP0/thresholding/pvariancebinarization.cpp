/* -*- c-basic-offset: 3; mode: c++ -*-
 *
 * Copyright (c), GREYC.
 * All rights reserved
 *
 * You may use this file under the terms of the BSD license as follows:
 *
 * "Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 *   * Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer.
 *   * Redistributions in binary form must reproduce the above copyright
 *     notice, this list of conditions and the following disclaimer in
 *     the documentation and/or other materials provided with the
 *     distribution.
 *   * Neither the name of the GREYC, nor the name of its
 *     contributors may be used to endorse or promote products
 *     derived from this software without specific prior written
 *     permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
 *
 * 
 * For more information, refer to:
 * https://clouard.users.greyc.fr/Pandore
 */

#include <pandore.h>
using namespace pandore;

/**
 * @file pvariancethresholding.cpp
 * @brief binarization based on Otsu algorithm: use the threshold
 * that maximizes the interclass variance (2 classes 0 - 255).
 *
 * Reference: N. Otsu, "A threshold selection method from grey scale histogram",
 * IEEE Trans. on Syst. Man and Cyber., vol 1, pp 62-66, 1979 
 */

#define ALPHA 0.5F	// Smoothing degree.


Errc PVarianceBinarization( const Img1duc &ims, Img1duc &imd ) {
   std::map<Img1duc::ValueType, float> h;
   std::map<Img1duc::ValueType, float> ch; // Cumulated histogram
   std::map<Img1duc::ValueType, float> wh; // Weighted histogram
   std::map<Img1duc::ValueType, float>::iterator i;

   int x;
   float m0, m1;
   
   // Compute histogram
   for (x = 0; x < ims.Width(); x++)
   {
      h[ims[x]]++;
   }

   // Compute the cumulated histogram and the weighted histogram.
   i = h.begin();
   float chv = ch[i->first] = i->second;
   float whv = wh[i->first] = 0.0;
   i++;
   for (; i != h.end(); ++i) {
      chv = ch[i->first] = chv + i->second;
      whv = wh[i->first] = whv + (i->first * i->second);
   }

   // Compute the variance for each grayscale.
   for (i = h.begin(); i != h.end(); ++i) {
      m0 = wh[i->first] / ch[i->first];
      if ((int)ch[i->first] == (int)chv) { // hcp = total pixels
	 m1 = 0;
      } else {
	 m1 = (whv - wh[i->first]) / (chv - ch[i->first]);
      }
      h[i->first] = -ch[i->first] * (chv - ch[i->first]) * SQR(m0 - m1);
   }

   // Smoothing with a symetric exponential.
   float hprev;
   i = h.begin();
   hprev = (i++)->second;
   for (; i != h.end(); ++i) {
      hprev = h[i->first] = ALPHA * (h[i->first] - hprev) + hprev;
   }
   i = h.end();
   i--;
   hprev = (i--)->second;
   for (; i != h.begin(); --i) {
      hprev = h[i->first] = ALPHA * (h[i->first] - hprev) + hprev;
   }
   i = h.begin();
   hprev = h[i->first] = ALPHA * (h[i->first] - hprev) + hprev;
   
   // Get the minima to determine the threshold.
   i = h.begin();
   Img1duc::ValueType minimum = (i++)->first;
   for (; i != h.end(); ++i) {
      if (h[i->first] < h[minimum]) {
	 minimum = i->first;
      }
   }

   minimum++; // Because thresholding use disequality (> threshold).
   // For example, if we have only two grayscale,
   // the minimum is the first grayscale, so the
   // threshood will be the second grayscale.
   // Use thresholds to build the output image.
   for (x = 0; x < ims.Width(); x++)
   {
      if (ims[x] < minimum) {
	 imd[x] = 0;
      } else {
	 imd[x] = MAXUCHAR;
      }
   }
   return minimum;
}

Errc PVarianceBinarization( const Img1dsl &ims, Img1duc &imd ) {
   std::map<Img1dsl::ValueType, float> h;
   std::map<Img1dsl::ValueType, float> ch; // Cumulated histogram
   std::map<Img1dsl::ValueType, float> wh; // Weighted histogram
   std::map<Img1dsl::ValueType, float>::iterator i;

   int x;
   float m0, m1;
   
   // Compute histogram
   for (x = 0; x < ims.Width(); x++)
   {
      h[ims[x]]++;
   }

   // Compute the cumulated histogram and the weighted histogram.
   i = h.begin();
   float chv = ch[i->first] = i->second;
   float whv = wh[i->first] = 0.0;
   i++;
   for (; i != h.end(); ++i) {
      chv = ch[i->first] = chv + i->second;
      whv = wh[i->first] = whv + (i->first * i->second);
   }

   // Compute the variance for each grayscale.
   for (i = h.begin(); i != h.end(); ++i) {
      m0 = wh[i->first] / ch[i->first];
      if ((int)ch[i->first] == (int)chv) { // hcp = total pixels
	 m1 = 0;
      } else {
	 m1 = (whv - wh[i->first]) / (chv - ch[i->first]);
      }
      h[i->first] = -ch[i->first] * (chv - ch[i->first]) * SQR(m0 - m1);
   }

   // Smoothing with a symetric exponential.
   float hprev;
   i = h.begin();
   hprev = (i++)->second;
   for (; i != h.end(); ++i) {
      hprev = h[i->first] = ALPHA * (h[i->first] - hprev) + hprev;
   }
   i = h.end();
   i--;
   hprev = (i--)->second;
   for (; i != h.begin(); --i) {
      hprev = h[i->first] = ALPHA * (h[i->first] - hprev) + hprev;
   }
   i = h.begin();
   hprev = h[i->first] = ALPHA * (h[i->first] - hprev) + hprev;
   
   // Get the minima to determine the threshold.
   i = h.begin();
   Img1dsl::ValueType minimum = (i++)->first;
   for (; i != h.end(); ++i) {
      if (h[i->first] < h[minimum]) {
	 minimum = i->first;
      }
   }

   minimum++; // Because thresholding use disequality (> threshold).
   // For example, if we have only two grayscale,
   // the minimum is the first grayscale, so the
   // threshood will be the second grayscale.
   // Use thresholds to build the output image.
   for (x = 0; x < ims.Width(); x++)
   {
      if (ims[x] < minimum) {
	 imd[x] = 0;
      } else {
	 imd[x] = MAXUCHAR;
      }
   }
   return minimum;
}

Errc PVarianceBinarization( const Img1dsf &ims, Img1duc &imd ) {
   std::map<Img1dsf::ValueType, float> h;
   std::map<Img1dsf::ValueType, float> ch; // Cumulated histogram
   std::map<Img1dsf::ValueType, float> wh; // Weighted histogram
   std::map<Img1dsf::ValueType, float>::iterator i;

   int x;
   float m0, m1;
   
   // Compute histogram
   for (x = 0; x < ims.Width(); x++)
   {
      h[ims[x]]++;
   }

   // Compute the cumulated histogram and the weighted histogram.
   i = h.begin();
   float chv = ch[i->first] = i->second;
   float whv = wh[i->first] = 0.0;
   i++;
   for (; i != h.end(); ++i) {
      chv = ch[i->first] = chv + i->second;
      whv = wh[i->first] = whv + (i->first * i->second);
   }

   // Compute the variance for each grayscale.
   for (i = h.begin(); i != h.end(); ++i) {
      m0 = wh[i->first] / ch[i->first];
      if ((int)ch[i->first] == (int)chv) { // hcp = total pixels
	 m1 = 0;
      } else {
	 m1 = (whv - wh[i->first]) / (chv - ch[i->first]);
      }
      h[i->first] = -ch[i->first] * (chv - ch[i->first]) * SQR(m0 - m1);
   }

   // Smoothing with a symetric exponential.
   float hprev;
   i = h.begin();
   hprev = (i++)->second;
   for (; i != h.end(); ++i) {
      hprev = h[i->first] = ALPHA * (h[i->first] - hprev) + hprev;
   }
   i = h.end();
   i--;
   hprev = (i--)->second;
   for (; i != h.begin(); --i) {
      hprev = h[i->first] = ALPHA * (h[i->first] - hprev) + hprev;
   }
   i = h.begin();
   hprev = h[i->first] = ALPHA * (h[i->first] - hprev) + hprev;
   
   // Get the minima to determine the threshold.
   i = h.begin();
   Img1dsf::ValueType minimum = (i++)->first;
   for (; i != h.end(); ++i) {
      if (h[i->first] < h[minimum]) {
	 minimum = i->first;
      }
   }

   minimum++; // Because thresholding use disequality (> threshold).
   // For example, if we have only two grayscale,
   // the minimum is the first grayscale, so the
   // threshood will be the second grayscale.
   // Use thresholds to build the output image.
   for (x = 0; x < ims.Width(); x++)
   {
      if (ims[x] < minimum) {
	 imd[x] = 0;
      } else {
	 imd[x] = MAXUCHAR;
      }
   }
   return minimum;
}

Errc PVarianceBinarization( const Img2duc &ims, Img2duc &imd ) {
   std::map<Img2duc::ValueType, float> h;
   std::map<Img2duc::ValueType, float> ch; // Cumulated histogram
   std::map<Img2duc::ValueType, float> wh; // Weighted histogram
   std::map<Img2duc::ValueType, float>::iterator i;

   int y, x;
   float m0, m1;
   
   // Compute histogram
   for (y = 0; y < ims.Height(); y++)
   for (x = 0; x < ims.Width(); x++)
   {
      h[ims[y][x]]++;
   }

   // Compute the cumulated histogram and the weighted histogram.
   i = h.begin();
   float chv = ch[i->first] = i->second;
   float whv = wh[i->first] = 0.0;
   i++;
   for (; i != h.end(); ++i) {
      chv = ch[i->first] = chv + i->second;
      whv = wh[i->first] = whv + (i->first * i->second);
   }

   // Compute the variance for each grayscale.
   for (i = h.begin(); i != h.end(); ++i) {
      m0 = wh[i->first] / ch[i->first];
      if ((int)ch[i->first] == (int)chv) { // hcp = total pixels
	 m1 = 0;
      } else {
	 m1 = (whv - wh[i->first]) / (chv - ch[i->first]);
      }
      h[i->first] = -ch[i->first] * (chv - ch[i->first]) * SQR(m0 - m1);
   }

   // Smoothing with a symetric exponential.
   float hprev;
   i = h.begin();
   hprev = (i++)->second;
   for (; i != h.end(); ++i) {
      hprev = h[i->first] = ALPHA * (h[i->first] - hprev) + hprev;
   }
   i = h.end();
   i--;
   hprev = (i--)->second;
   for (; i != h.begin(); --i) {
      hprev = h[i->first] = ALPHA * (h[i->first] - hprev) + hprev;
   }
   i = h.begin();
   hprev = h[i->first] = ALPHA * (h[i->first] - hprev) + hprev;
   
   // Get the minima to determine the threshold.
   i = h.begin();
   Img2duc::ValueType minimum = (i++)->first;
   for (; i != h.end(); ++i) {
      if (h[i->first] < h[minimum]) {
	 minimum = i->first;
      }
   }

   minimum++; // Because thresholding use disequality (> threshold).
   // For example, if we have only two grayscale,
   // the minimum is the first grayscale, so the
   // threshood will be the second grayscale.
   // Use thresholds to build the output image.
   for (y = 0; y < ims.Height(); y++)
   for (x = 0; x < ims.Width(); x++)
   {
      if (ims[y][x] < minimum) {
	 imd[y][x] = 0;
      } else {
	 imd[y][x] = MAXUCHAR;
      }
   }
   return minimum;
}

Errc PVarianceBinarization( const Img2dsl &ims, Img2duc &imd ) {
   std::map<Img2dsl::ValueType, float> h;
   std::map<Img2dsl::ValueType, float> ch; // Cumulated histogram
   std::map<Img2dsl::ValueType, float> wh; // Weighted histogram
   std::map<Img2dsl::ValueType, float>::iterator i;

   int y, x;
   float m0, m1;
   
   // Compute histogram
   for (y = 0; y < ims.Height(); y++)
   for (x = 0; x < ims.Width(); x++)
   {
      h[ims[y][x]]++;
   }

   // Compute the cumulated histogram and the weighted histogram.
   i = h.begin();
   float chv = ch[i->first] = i->second;
   float whv = wh[i->first] = 0.0;
   i++;
   for (; i != h.end(); ++i) {
      chv = ch[i->first] = chv + i->second;
      whv = wh[i->first] = whv + (i->first * i->second);
   }

   // Compute the variance for each grayscale.
   for (i = h.begin(); i != h.end(); ++i) {
      m0 = wh[i->first] / ch[i->first];
      if ((int)ch[i->first] == (int)chv) { // hcp = total pixels
	 m1 = 0;
      } else {
	 m1 = (whv - wh[i->first]) / (chv - ch[i->first]);
      }
      h[i->first] = -ch[i->first] * (chv - ch[i->first]) * SQR(m0 - m1);
   }

   // Smoothing with a symetric exponential.
   float hprev;
   i = h.begin();
   hprev = (i++)->second;
   for (; i != h.end(); ++i) {
      hprev = h[i->first] = ALPHA * (h[i->first] - hprev) + hprev;
   }
   i = h.end();
   i--;
   hprev = (i--)->second;
   for (; i != h.begin(); --i) {
      hprev = h[i->first] = ALPHA * (h[i->first] - hprev) + hprev;
   }
   i = h.begin();
   hprev = h[i->first] = ALPHA * (h[i->first] - hprev) + hprev;
   
   // Get the minima to determine the threshold.
   i = h.begin();
   Img2dsl::ValueType minimum = (i++)->first;
   for (; i != h.end(); ++i) {
      if (h[i->first] < h[minimum]) {
	 minimum = i->first;
      }
   }

   minimum++; // Because thresholding use disequality (> threshold).
   // For example, if we have only two grayscale,
   // the minimum is the first grayscale, so the
   // threshood will be the second grayscale.
   // Use thresholds to build the output image.
   for (y = 0; y < ims.Height(); y++)
   for (x = 0; x < ims.Width(); x++)
   {
      if (ims[y][x] < minimum) {
	 imd[y][x] = 0;
      } else {
	 imd[y][x] = MAXUCHAR;
      }
   }
   return minimum;
}

Errc PVarianceBinarization( const Img2dsf &ims, Img2duc &imd ) {
   std::map<Img2dsf::ValueType, float> h;
   std::map<Img2dsf::ValueType, float> ch; // Cumulated histogram
   std::map<Img2dsf::ValueType, float> wh; // Weighted histogram
   std::map<Img2dsf::ValueType, float>::iterator i;

   int y, x;
   float m0, m1;
   
   // Compute histogram
   for (y = 0; y < ims.Height(); y++)
   for (x = 0; x < ims.Width(); x++)
   {
      h[ims[y][x]]++;
   }

   // Compute the cumulated histogram and the weighted histogram.
   i = h.begin();
   float chv = ch[i->first] = i->second;
   float whv = wh[i->first] = 0.0;
   i++;
   for (; i != h.end(); ++i) {
      chv = ch[i->first] = chv + i->second;
      whv = wh[i->first] = whv + (i->first * i->second);
   }

   // Compute the variance for each grayscale.
   for (i = h.begin(); i != h.end(); ++i) {
      m0 = wh[i->first] / ch[i->first];
      if ((int)ch[i->first] == (int)chv) { // hcp = total pixels
	 m1 = 0;
      } else {
	 m1 = (whv - wh[i->first]) / (chv - ch[i->first]);
      }
      h[i->first] = -ch[i->first] * (chv - ch[i->first]) * SQR(m0 - m1);
   }

   // Smoothing with a symetric exponential.
   float hprev;
   i = h.begin();
   hprev = (i++)->second;
   for (; i != h.end(); ++i) {
      hprev = h[i->first] = ALPHA * (h[i->first] - hprev) + hprev;
   }
   i = h.end();
   i--;
   hprev = (i--)->second;
   for (; i != h.begin(); --i) {
      hprev = h[i->first] = ALPHA * (h[i->first] - hprev) + hprev;
   }
   i = h.begin();
   hprev = h[i->first] = ALPHA * (h[i->first] - hprev) + hprev;
   
   // Get the minima to determine the threshold.
   i = h.begin();
   Img2dsf::ValueType minimum = (i++)->first;
   for (; i != h.end(); ++i) {
      if (h[i->first] < h[minimum]) {
	 minimum = i->first;
      }
   }

   minimum++; // Because thresholding use disequality (> threshold).
   // For example, if we have only two grayscale,
   // the minimum is the first grayscale, so the
   // threshood will be the second grayscale.
   // Use thresholds to build the output image.
   for (y = 0; y < ims.Height(); y++)
   for (x = 0; x < ims.Width(); x++)
   {
      if (ims[y][x] < minimum) {
	 imd[y][x] = 0;
      } else {
	 imd[y][x] = MAXUCHAR;
      }
   }
   return minimum;
}

Errc PVarianceBinarization( const Img3duc &ims, Img3duc &imd ) {
   std::map<Img3duc::ValueType, float> h;
   std::map<Img3duc::ValueType, float> ch; // Cumulated histogram
   std::map<Img3duc::ValueType, float> wh; // Weighted histogram
   std::map<Img3duc::ValueType, float>::iterator i;

   int z, y, x;
   float m0, m1;
   
   // Compute histogram
   for (z = 0; z < ims.Depth(); z++)
   for (y = 0; y < ims.Height(); y++)
   for (x = 0; x < ims.Width(); x++)
   {
      h[ims[z][y][x]]++;
   }

   // Compute the cumulated histogram and the weighted histogram.
   i = h.begin();
   float chv = ch[i->first] = i->second;
   float whv = wh[i->first] = 0.0;
   i++;
   for (; i != h.end(); ++i) {
      chv = ch[i->first] = chv + i->second;
      whv = wh[i->first] = whv + (i->first * i->second);
   }

   // Compute the variance for each grayscale.
   for (i = h.begin(); i != h.end(); ++i) {
      m0 = wh[i->first] / ch[i->first];
      if ((int)ch[i->first] == (int)chv) { // hcp = total pixels
	 m1 = 0;
      } else {
	 m1 = (whv - wh[i->first]) / (chv - ch[i->first]);
      }
      h[i->first] = -ch[i->first] * (chv - ch[i->first]) * SQR(m0 - m1);
   }

   // Smoothing with a symetric exponential.
   float hprev;
   i = h.begin();
   hprev = (i++)->second;
   for (; i != h.end(); ++i) {
      hprev = h[i->first] = ALPHA * (h[i->first] - hprev) + hprev;
   }
   i = h.end();
   i--;
   hprev = (i--)->second;
   for (; i != h.begin(); --i) {
      hprev = h[i->first] = ALPHA * (h[i->first] - hprev) + hprev;
   }
   i = h.begin();
   hprev = h[i->first] = ALPHA * (h[i->first] - hprev) + hprev;
   
   // Get the minima to determine the threshold.
   i = h.begin();
   Img3duc::ValueType minimum = (i++)->first;
   for (; i != h.end(); ++i) {
      if (h[i->first] < h[minimum]) {
	 minimum = i->first;
      }
   }

   minimum++; // Because thresholding use disequality (> threshold).
   // For example, if we have only two grayscale,
   // the minimum is the first grayscale, so the
   // threshood will be the second grayscale.
   // Use thresholds to build the output image.
   for (z = 0; z < ims.Depth(); z++)
   for (y = 0; y < ims.Height(); y++)
   for (x = 0; x < ims.Width(); x++)
   {
      if (ims[z][y][x] < minimum) {
	 imd[z][y][x] = 0;
      } else {
	 imd[z][y][x] = MAXUCHAR;
      }
   }
   return minimum;
}

Errc PVarianceBinarization( const Img3dsl &ims, Img3duc &imd ) {
   std::map<Img3dsl::ValueType, float> h;
   std::map<Img3dsl::ValueType, float> ch; // Cumulated histogram
   std::map<Img3dsl::ValueType, float> wh; // Weighted histogram
   std::map<Img3dsl::ValueType, float>::iterator i;

   int z, y, x;
   float m0, m1;
   
   // Compute histogram
   for (z = 0; z < ims.Depth(); z++)
   for (y = 0; y < ims.Height(); y++)
   for (x = 0; x < ims.Width(); x++)
   {
      h[ims[z][y][x]]++;
   }

   // Compute the cumulated histogram and the weighted histogram.
   i = h.begin();
   float chv = ch[i->first] = i->second;
   float whv = wh[i->first] = 0.0;
   i++;
   for (; i != h.end(); ++i) {
      chv = ch[i->first] = chv + i->second;
      whv = wh[i->first] = whv + (i->first * i->second);
   }

   // Compute the variance for each grayscale.
   for (i = h.begin(); i != h.end(); ++i) {
      m0 = wh[i->first] / ch[i->first];
      if ((int)ch[i->first] == (int)chv) { // hcp = total pixels
	 m1 = 0;
      } else {
	 m1 = (whv - wh[i->first]) / (chv - ch[i->first]);
      }
      h[i->first] = -ch[i->first] * (chv - ch[i->first]) * SQR(m0 - m1);
   }

   // Smoothing with a symetric exponential.
   float hprev;
   i = h.begin();
   hprev = (i++)->second;
   for (; i != h.end(); ++i) {
      hprev = h[i->first] = ALPHA * (h[i->first] - hprev) + hprev;
   }
   i = h.end();
   i--;
   hprev = (i--)->second;
   for (; i != h.begin(); --i) {
      hprev = h[i->first] = ALPHA * (h[i->first] - hprev) + hprev;
   }
   i = h.begin();
   hprev = h[i->first] = ALPHA * (h[i->first] - hprev) + hprev;
   
   // Get the minima to determine the threshold.
   i = h.begin();
   Img3dsl::ValueType minimum = (i++)->first;
   for (; i != h.end(); ++i) {
      if (h[i->first] < h[minimum]) {
	 minimum = i->first;
      }
   }

   minimum++; // Because thresholding use disequality (> threshold).
   // For example, if we have only two grayscale,
   // the minimum is the first grayscale, so the
   // threshood will be the second grayscale.
   // Use thresholds to build the output image.
   for (z = 0; z < ims.Depth(); z++)
   for (y = 0; y < ims.Height(); y++)
   for (x = 0; x < ims.Width(); x++)
   {
      if (ims[z][y][x] < minimum) {
	 imd[z][y][x] = 0;
      } else {
	 imd[z][y][x] = MAXUCHAR;
      }
   }
   return minimum;
}

Errc PVarianceBinarization( const Img3dsf &ims, Img3duc &imd ) {
   std::map<Img3dsf::ValueType, float> h;
   std::map<Img3dsf::ValueType, float> ch; // Cumulated histogram
   std::map<Img3dsf::ValueType, float> wh; // Weighted histogram
   std::map<Img3dsf::ValueType, float>::iterator i;

   int z, y, x;
   float m0, m1;
   
   // Compute histogram
   for (z = 0; z < ims.Depth(); z++)
   for (y = 0; y < ims.Height(); y++)
   for (x = 0; x < ims.Width(); x++)
   {
      h[ims[z][y][x]]++;
   }

   // Compute the cumulated histogram and the weighted histogram.
   i = h.begin();
   float chv = ch[i->first] = i->second;
   float whv = wh[i->first] = 0.0;
   i++;
   for (; i != h.end(); ++i) {
      chv = ch[i->first] = chv + i->second;
      whv = wh[i->first] = whv + (i->first * i->second);
   }

   // Compute the variance for each grayscale.
   for (i = h.begin(); i != h.end(); ++i) {
      m0 = wh[i->first] / ch[i->first];
      if ((int)ch[i->first] == (int)chv) { // hcp = total pixels
	 m1 = 0;
      } else {
	 m1 = (whv - wh[i->first]) / (chv - ch[i->first]);
      }
      h[i->first] = -ch[i->first] * (chv - ch[i->first]) * SQR(m0 - m1);
   }

   // Smoothing with a symetric exponential.
   float hprev;
   i = h.begin();
   hprev = (i++)->second;
   for (; i != h.end(); ++i) {
      hprev = h[i->first] = ALPHA * (h[i->first] - hprev) + hprev;
   }
   i = h.end();
   i--;
   hprev = (i--)->second;
   for (; i != h.begin(); --i) {
      hprev = h[i->first] = ALPHA * (h[i->first] - hprev) + hprev;
   }
   i = h.begin();
   hprev = h[i->first] = ALPHA * (h[i->first] - hprev) + hprev;
   
   // Get the minima to determine the threshold.
   i = h.begin();
   Img3dsf::ValueType minimum = (i++)->first;
   for (; i != h.end(); ++i) {
      if (h[i->first] < h[minimum]) {
	 minimum = i->first;
      }
   }

   minimum++; // Because thresholding use disequality (> threshold).
   // For example, if we have only two grayscale,
   // the minimum is the first grayscale, so the
   // threshood will be the second grayscale.
   // Use thresholds to build the output image.
   for (z = 0; z < ims.Depth(); z++)
   for (y = 0; y < ims.Height(); y++)
   for (x = 0; x < ims.Width(); x++)
   {
      if (ims[z][y][x] < minimum) {
	 imd[z][y][x] = 0;
      } else {
	 imd[z][y][x] = MAXUCHAR;
      }
   }
   return minimum;
}


#ifdef MAIN
#define USAGE	"usage: %s [-m mask] [im_in|-] [im_out|-]"
#define PARC	0
#define FINC	1
#define FOUTC	1
#define MASK	1

int main( int argc, char *argv[] ) {
   Errc result;                // The result code of the execution.
   Pobject* mask;              // The region map.
   Pobject* objin[FINC + 1];   // The input objects.
   Pobject* objs[FINC + 1];    // The source objects masked.
   Pobject* objout[FOUTC + 1]; // The output object.
   Pobject* objd[FOUTC + 1];   // The result object of the execution.
   char* parv[PARC + 1];       // The input parameters.

   ReadArgs(argc, argv, PARC, FINC, FOUTC, &mask, objin, objs, objout, objd, parv, USAGE, MASK); 
   if (objs[0]->Type() == Po_Img1duc) {
      Img1duc* const ims = (Img1duc*)objs[0];
      objd[0] = new Img1duc(ims->Props());
      Img1duc* const imd = (Img1duc*)objd[0];
      
      result = PVarianceBinarization(*ims, *imd);
      goto end;
   }
   if (objs[0]->Type() == Po_Img1dsl) {
      Img1dsl* const ims = (Img1dsl*)objs[0];
      objd[0] = new Img1duc(ims->Props());
      Img1duc* const imd = (Img1duc*)objd[0];
      
      result = PVarianceBinarization(*ims, *imd);
      goto end;
   }
   if (objs[0]->Type() == Po_Img1dsf) {
      Img1dsf* const ims = (Img1dsf*)objs[0];
      objd[0] = new Img1duc(ims->Props());
      Img1duc* const imd = (Img1duc*)objd[0];
      
      result = PVarianceBinarization(*ims, *imd);
      goto end;
   }
   if (objs[0]->Type() == Po_Img2duc) {
      Img2duc* const ims = (Img2duc*)objs[0];
      objd[0] = new Img2duc(ims->Props());
      Img2duc* const imd = (Img2duc*)objd[0];
      
      result = PVarianceBinarization(*ims, *imd);
      goto end;
   }
   if (objs[0]->Type() == Po_Img2dsl) {
      Img2dsl* const ims = (Img2dsl*)objs[0];
      objd[0] = new Img2duc(ims->Props());
      Img2duc* const imd = (Img2duc*)objd[0];
      
      result = PVarianceBinarization(*ims, *imd);
      goto end;
   }
   if (objs[0]->Type() == Po_Img2dsf) {
      Img2dsf* const ims = (Img2dsf*)objs[0];
      objd[0] = new Img2duc(ims->Props());
      Img2duc* const imd = (Img2duc*)objd[0];
      
      result = PVarianceBinarization(*ims, *imd);
      goto end;
   }
   if (objs[0]->Type() == Po_Img3duc) {
      Img3duc* const ims = (Img3duc*)objs[0];
      objd[0] = new Img3duc(ims->Props());
      Img3duc* const imd = (Img3duc*)objd[0];
      
      result = PVarianceBinarization(*ims, *imd);
      goto end;
   }
   if (objs[0]->Type() == Po_Img3dsl) {
      Img3dsl* const ims = (Img3dsl*)objs[0];
      objd[0] = new Img3duc(ims->Props());
      Img3duc* const imd = (Img3duc*)objd[0];
      
      result = PVarianceBinarization(*ims, *imd);
      goto end;
   }
   if (objs[0]->Type() == Po_Img3dsf) {
      Img3dsf* const ims = (Img3dsf*)objs[0];
      objd[0] = new Img3duc(ims->Props());
      Img3duc* const imd = (Img3duc*)objd[0];
      
      result = PVarianceBinarization(*ims, *imd);
      goto end;
   }
  {
     PrintErrorFormat(objin, FINC, argv); 
     result = FAILURE; 
  }	

end:
  if (result) {
	WriteArgs(argc, argv, PARC, FINC, FOUTC, &mask, objin, objs, objout, objd, MASK); 
  }
  Exit(result); 
  return 0; 
}

#endif
