/* -*- c-basic-offset: 3 -*-
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

/*
 * @file pniblackbinarization.cpp
 *
 * Sauvola's version of the Niblack's binarization algorithm.
 * References:
 * W. Niblack, "An introduction to digital image processing", 
 * Prentice hall, pp. 115-116, 1986.
 * J. Sauvola, M. Pietikainen, "Adaptative document ipage binarization", 
 * Pattern Recognition</i>, vol 33, pp 255-236, 2000.
 */

#include <stdio.h>
#include <math.h>
#include <pandore.h>
using namespace pandore;

const static int R = 128;

inline float _PNiblackBinarization( const Img2duc &ims, int width, int height, float k, Point2d p ) {
   Point2d w;
   long sum = 0;
   long sum2 = 0;
   long pixelCount = 0;
   
   for (w.y = p.y - height; w.y <= p.y + height; w.y++) {
      for (w.x = p.x - width; w.x <= p.x + width; w.x++) {
	 if (ims.Hold(w)) {
	    pixelCount++;
	    sum += (long)ims[w];
	    sum2 += (long)ims[w] * ims[w];
	 }
      }
   }
   float mean = (float)sum / pixelCount;
   float mean2 = (float)sum2 / pixelCount;
   float s = sqrt(mean2 - mean * mean);
   return mean * (1 - k * (1.0F - (s / R)));
}

Errc PNiblackBinarization( const Img2duc &ims, Img2duc &imd, int width, int height, float k) {
   Point2d p, w;
   width = (width - 1) / 2; // Use half windows size to get a center.
   height = (height - 1) / 2;

   float threshold;

   // borders
   for (p.y = 0; p.y < ims.Height(); p.y++) {
      for (p.x = 0; p.x < width; p.x++) {
	 threshold = _PNiblackBinarization(ims, width, height, k, p) ;
	 imd[p] = (ims[p] <= threshold)? 255: 0; 
	 Point2d q(p.y, ims.Width() - 1 - p.x);
	 threshold = _PNiblackBinarization(ims, width, height, k, q) ;
	 imd[q] = (ims[q] <= threshold)? 255: 0; 
      }
   }
   for (p.x = width; p.x < ims.Width() - width; p.x++) {
      for (p.y = 0; p.y < height; p.y++) {
	 threshold=_PNiblackBinarization(ims, width, height, k, p) ;
	 imd[p] = (ims[p] <= threshold)? 255: 0; 
	 Point2d q(ims.Height() - 1 - p.y, ims.Width() - 1 - p.x);
	 threshold = _PNiblackBinarization(ims, width, height, k, q);
	 imd[q] = (ims[q] <= threshold)? 255: 0; 
      }
   }

   // General case
   const long pixelCount = (width + width + 1) * (height + height + 1);
   long _sum = 0, sum;
   long _sum2 = 0, sum2;
   for (p.y = height; p.y < ims.Height() - height; p.y++) {
      p.x = width;
      if (p.y == height) {
	 // Compute the first mean and stdev
	 for (w.y = 0; w.y <= height + height; w.y++) {
	    for (w.x = 0; w.x <= width + width; w.x++) {
	       _sum += (long)ims[w];
	       _sum2 += (long)ims[w] * ims[w];
	    }
	 }
      } else {
	 // Compute the first mean and stdev
	 for (int h = -width; h <= width; h++) {
	    long tmp = (long)ims(p.y - height - 1, p.x + h);
	    _sum -= tmp;
	    _sum2 -= tmp * tmp;
	    tmp = (long)ims(p.y + height, p.x + h); 
	    _sum += tmp;
	    _sum2 += tmp * tmp;
	 }
      }
      sum = _sum;
      sum2 = _sum2;
      float mean = (float)sum / pixelCount;
      float mean2 = (float)sum2 / pixelCount;
      float s = sqrt(mean2 - mean * mean);
      float threshold = mean * (1 - k * (1.0F - (s / R)));
      imd[p] = (ims[p] <= threshold)? 255: 0; 

      for (p.x = width + 1; p.x < ims.Width() - width; p.x++) {
	 for (int h = -height; h <= height; h++) {
	    long tmp = (long)ims(p.y + h, p.x - width - 1);
	    sum -= tmp;
	    sum2 -=tmp * tmp;
	    tmp = (long)ims(p.y + h, p.x + width); 
	    sum += tmp;
	    sum2 += tmp * tmp;
	 }
	 float mean = (float)sum / pixelCount;
	 float mean2 = (float)sum2 / pixelCount;
	 float s = sqrt(mean2 - mean * mean);
	 float threshold = mean * (1 - k * (1.0F - (s / R)));
	 imd[p] = (ims[p] <= threshold)? 255: 0; 
      }
   }
   return SUCCESS;
}

#ifdef MAIN
#define USAGE   "usage: %s width height depth k [-m mask] [im_in|-] [im_out|-]"
#define PARC    4
#define FINC    1
#define FOUTC   1
#define MASK    1

int main(int argc, char* argv[]) {
   Errc result;			// The result code of the execution.
   Pobject *mask;		// The mask.
   Pobject *objin[FINC + 1];	// The input objects.
   Pobject *objs[FINC + 1];	// The source objects masked by mask.
   Pobject *objout[FOUTC + 1];	// The output object unmasked by mask.
   Pobject *objd[FOUTC + 1];	// The result object.
   char *parv[PARC + 1];	// The input parameters.
   
   ReadArgs(argc, argv, PARC, FINC, FOUTC, &mask, objin, objs, objout, objd, parv, USAGE, MASK);
   switch (objs[0]->Type()) {
      case Po_Img2duc : {
	 Img2duc* const ims = (Img2duc*)objs[0];
	 objd[0] = new Img2duc(ims->Props());
	 Img2duc* const imd = (Img2duc*)objd[0];
	 
	 result = PNiblackBinarization(*ims, *imd, (int)atoi(parv[0]), (int)atoi(parv[1]), (float)atof(parv[3]));
      } break;
      default:
	 PrintErrorFormat(objin, FINC);
	 result = FAILURE;
   }
   
   if (result) {
      WriteArgs(argc, argv, PARC, FINC, FOUTC, &mask, objin, objs, objout, objd, MASK);
   }
   Exit(result);
   return 0;
}

#endif

