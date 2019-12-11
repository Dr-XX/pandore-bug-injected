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
 * @author Nicolas Convers - 2011-04-07
 */

/**
 * Estimate skew angle and correct the image orientation.
 * Based on:
 * E. Kavallieratou, N. Fatotakis, G. Kokkinakis (2002)
 * "Skew angle estimation for printed and handwritten
 * documents using the Wigner-Ville distribution", 
 * Image and Vision Computing, 20: 813-824.
 * Usage:
 * ./pkewanglecorrection w h max_angle image_in image_out
 * where w and h are the size of the mask that is used
 * to estimate the skew angle. This mask is extracted from the
 * input image and represents the best trade-off between density
 * and number of black and white transitions in row.
 * Then, the skew angle is determined by the angle which gives
 * the higher maximum in the Wigner-Ville distribution of
 * the horizontal distribution of the mask image.
 */

#include <pandore.h>
#include <math.h>
using namespace pandore;

#ifdef MAIN
#undef MAIN
#define __PSKEWANGLECORRECTION__
#endif
namespace rotation {
#include "transformation/protation.cpp"
}
namespace fft {
#include "frequency/pfft.cpp"
}
#ifdef __PSKEWANGLECORRECTION__
#define MAIN
#endif

/**
 * Selects the image part with the best trade-off
 * between higher density and higher white-black transitions.
 * @param ims an image.
 * @param mask the result mask extracted from the input image.  
 */
Errc SelectMask( const Img2duc &ims, Img2duc &mask ) {
   const Long height = mask.Height();
   const Long width = mask.Width();
   const int maxPosHeight = 2 * ims.Height() / height;
   const int maxPosWidth = 2 * ims.Width() / width;
   
   Img2dsf density(maxPosHeight, maxPosWidth);
   Img2dsf altern(maxPosHeight, maxPosWidth);
   float maxAltern = -1;

   density = 0;
   altern = 0;

   // Calculate values for each window position.
   int stepY = height / 2;
   int stepX = width / 2;
   int maxY = ims.Height() - height / 2;
   int maxX = ims.Width() - width / 2;

   for (int y = stepY; y <= maxY; y += stepY) {
      for (int x = stepX; x <= maxX; x += stepX) {

	 for (int l = y - stepY; l < y + stepY; l++) {
	    for (int k = x - stepX; k < x + stepX; k++) {
	       // compute density
	       if (ims[l][k] != 0) {
		  density[y / stepY - 1][x / stepX - 1]++;
	       }
	       // compute X transitions.
	       if (k > 0 && ims[l][k-1] != ims[l][k]) {
		  altern[y / stepY - 1][x / stepX - 1]++;
	       }
	    }
	 }
 	 if (altern[y / stepY - 1][x / stepX- 1] > maxAltern) {
 	    maxAltern = altern[y / stepY - 1][ x /stepX - 1];
	 }
      } 
   }

   // Normalize values and search for maximum product.
   float maxProduct = -1;
   for (int Y = 0; Y < maxPosHeight; Y++) {
      for (int X = 0; X < maxPosWidth; X++) {
	 if ((density[Y][X]/(height * width)) * (altern[Y][X] / maxAltern) > maxProduct) {
	    maxProduct = (density[Y][X] / (height * width)) * (altern[Y][X] / maxAltern);
	    maxY = Y;
	    maxX = X;
	 }
      }
   }
#ifndef  NDEBUG
   //std::cout<<"Maximum : ("<<maxY*stepY<<","<<maxX*stepX<<") "<<maxProduct<<std::endl;
#endif

   // Initialize mask.
   int startY = maxY * stepY;
   int startX = maxX * stepX;
   for (int y = startY; y < startY + height && y < ims.Height(); y++) {
      for (int x = startX; x < startX + width && x < ims.Width(); x++) {
	 mask[y - startY][x - startX] = ims[y][x];
      }
   }

   return SUCCESS;
}

/**
 * Calculates the Wigner-Ville distribution.
 * From Emmanuel Roy, Manuel Davy (1999-02-10).
 *
 * @param signal the signal to be analyzed.
 * @param tfr    the resulted Wigner-Ville distribution (y=frequency, and x = time).
 * @return error code.
 */
Errc WignerVille( const Img1dsf &signal, Img2dsf &tfr ) {
   int N_freq = signal.Width();
   int N_time = N_freq;
   int *time_instants = new int[N_time];

   for (int i = 0; i < N_time; i++) {
      time_instants[i] = i + 1;
   }
   
   // Vector of frequency bins (freqs where the tfr matrix is computed).
   Img1dsf freq_bins(N_freq);
   for (int x = 0; x < N_freq; x++) {
      freq_bins[x] = (float) x / (2 * N_freq); 
   }
   
   // Memory allocation for the local autocorrelation function.
   int size = fft::nextpow2(N_freq);
   Img1dsf lacf_real(size);
   Img1dsf lacf_imag(size);
   Img1dsf laco_real(size);
   Img1dsf laco_imag(size);
   lacf_real = 0;
   lacf_imag = 0;

   // Computation of the FFT for the current windowed signal.
   for (int column = 0; column < N_time; column++) {
      /* time instants of interest to compute the Wigner distrib. */
      int time = time_instants[column] - 1;

      /* taumax enables the computation near the edges */
      int taumax = MIN (time, (signal.Width() - time - 1));
      taumax = MIN (taumax, (N_freq / 2 - 1));
      
      /* for each delay value, the laf is computed and ffted */
      /* in order to pass from the (t,tau) domain to the (t,f) domain. */
      int tau;
      for (tau = -taumax; tau <= taumax; tau++) {
	 int row = (N_freq + tau) % N_freq;
	 lacf_real[row] = signal[time + tau] * signal[time - tau];
      }
      tau = (int)floor(N_freq / 2.0f);
      if ((time<=signal.Width() - tau - 1) && (time >= tau)) {
	 lacf_real[tau] =  signal[time+tau] * signal[time-tau];
      }

      /* fft of the local autocorrelation function lacf */
      if (!fft::PFFT(lacf_real, lacf_imag, laco_real, laco_imag))
	 return FAILURE;
      
      /* the fft is put in the wv matrix and reinitialized */
      for (int row = 0; row < N_freq; row++) {
	 tfr[row][column] = laco_real[row];
	 lacf_imag[row] = 0.0;
      }
   }

   delete[] time_instants;
   return SUCCESS;
}

/**
 * Estimates the skew angle for the given mask.
 *
 * @param maxskew the maximum search angle (in degrees)
 * @param step step between two angles.
 */
Errc CalculateSkewAngle( const Img2duc &mask, float maxskew, double step ) {
   Point2d p;
   float skewAngle = 0;
   float maximumIntensity = 0;

   int size = mask.Height();
   Img2duc imi(mask.Props());

   Img1dsf profile(size);
   Img2dsf distrib(size,size);

   for (float angle = -maxskew; angle <= maxskew; angle += (float)step) {
      profile = 0;
      // 1. Rotate
      if (angle != 0 && !rotation::PRotation(mask, imi, 0, angle)) {
	 return FAILURE;
      }
      
      // 2. Calculate horizontal histogram.
      for (p.y = 0; p.y < imi.Height(); p.y++) {
	 for (p.x = 0; p.x < imi.Width(); p.x++) {
	    if (imi[p] != 0) {
	       profile[p.y]++;
	    }
	 }
      }

      // 3. Calculate Wigner-Ville distribution 
      if (!WignerVille(profile, distrib)) {
	 return FAILURE;
      }
      
      // Extract maximum intensity curve.
      float max = -1;
      for (p.y = 0; p.y < distrib.Height(); p.y++) {
	 for (p.x = 0; p.x < distrib.Width(); p.x++) {
	    if (distrib[p] > max) {
	       max = distrib[p];
	    }
	 }
      }

      if ( max > maximumIntensity ) {
	 skewAngle = angle;
	 maximumIntensity = max;
      } else if ( max == maximumIntensity && ABS(angle) < ABS(skewAngle) ) {
	 skewAngle = angle;
      }
   }
   
   return skewAngle;
}

/**
 * @param width the width the search window.
 * @param height the height the search window.
 */
float PSkewAngleCorrection( const Img2duc &ims, int height, int width, float maxskew ) {
   if (width > ims.Width() || height > ims.Height() || width <= 0 || height <= 0) {
      std::cerr << "PSkewAngleCorrection: Bad parameters values: " << height << ", " << width << std::endl;
      return FAILURE;
   }
   
   // Select the mask
   Img2duc mask(height, width);
   mask = 0;
   if (!SelectMask(ims, mask)) {
      return FAILURE;
   }
   
   // Determine the skew angle
   const float step = maxskew / 10.f;
   float angle = CalculateSkewAngle(mask, maxskew, step);

#ifndef NDEBUG
   printf("  skew angle = %f\n", -angle);
#endif
   
   return angle;
}


#ifdef MAIN
#define	USAGE	"usage: %s height width max_skew [-m mask] [im_in|-]"
#define	PARC	3
#define	FINC	1
#define	FOUTC	0
#define	MASK	1

int main( int argc, char *argv[] ) {
   Errc result;                // The result code of the execution.
   Pobject* mask;              // The region map.
   Pobject* objin[FINC + 1];   // The input objects.
   Pobject* objs[FINC + 1];    // The source objects masked.
   Pobject* objout[FOUTC + 1]; // The output object.
   Pobject* objd[FOUTC + 1];   // The result object of the execution.
   char* parv[PARC + 1];       // The input parameters.

   ReadArgs(argc, argv, PARC, FINC, FOUTC, &mask, objin, objs, objout, objd, parv, USAGE, MASK);
   if (objs[0]->Type() == Po_Img2duc) {
      Img2duc* const ims = (Img2duc*)objs[0];
      
      result = PSkewAngleCorrection(*ims, atoi(parv[0]), atoi(parv[1]), (float)atof(parv[2]));
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
