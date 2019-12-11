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

#include <pandore.h>
using namespace pandore;

/**
 * @file pcontrastbinarization.cpp
 *
 * This binarization uses the maximum contrast in the boundaries of objets.
 * Thresholds is choosen as the maximum contrast in
 * the boundaries between two regions.
 */

#define ALPHA 0.5F	// Smoothing degree.

/*
 * 1- Computes histogram of boundaries amplitudes.
 * 2- Smoothes the result histogram.
 * 4- Build the output image from the maximum
 */
Errc PContrastBinarization( const Img2duc &ims, Img2duc &ima, Img2duc &imd ) {
   // 1: Boundaries histogram computing.
   const int histogramSize = MAXUCHAR + 1;
   float hg[histogramSize] = {0};

   Point2d p;

   // top 
   p.y = 0;
   for (p.x = 1; p.x < ims.Width() - 1; p.x++) {
      Uchar val = ima[p];
      if (val > 0) {
	 Uchar centre = ims[p];
	 Uchar min, max;
	 for (int v = 0; v < 4; v++) {
	    if (ims.Hold(p + v8[v])) {
	       if ((min = ims[p + v8[v]]) > centre) {
		  max = min;
		  min = centre;
	       } else {
		  max = centre;
	       }
	       for (int k = min; k <= max; k++) {
		  hg[k] += (float)val;
	       }
	    }
	 }
      }
   }

   // Left
   for (p.y = 0; p.y < ims.Height(); p.y++) {
      p.x = 0;
      Uchar val = ima[p];
      if (val > 0) {
	 Uchar centre = ims[p];
	 Uchar min, max;
	 for (int v = 0; v < 4; v++) {
	    if (ims.Hold(p + v8[v])) {
	       if ((min = ims[p + v8[v]]) > centre) {
		  max = min;
		  min = centre;
	       } else {
		  max = centre;
	       }
	       for (int k = min; k <= max; k++) {
		  hg[k] += (float)val;
	       }
	    }
	 }
      }
   }
   
   // right
   for (p.y = 0; p.y < ims.Height(); p.y++) {
      p.x = ims.Width() - 1;
      Uchar val = ima[p];
      if (val > 0) {
	 Uchar centre = ims[p];
	 Uchar min, max;
	 for (int v = 0; v < 4; v++) {
	    if (ims.Hold(p + v8[v])) {
	       if ((min = ims[p + v8[v]]) > centre) {
		  max = min;
		  min = centre;
	       } else {
		  max = centre;
	       }
	       for (int k = min; k <= max; k++) {
		  hg[k] += (float)val;
	       }
	    }
	 }
      }
   }

   // General
   for (p.y = 1; p.y < ims.Height(); p.y++) {
      for (p.x = 1; p.x<ims.Width() - 1; p.x++) {
	 Uchar val = ima[p];
	 if (val > 0) {
	    Uchar centre = ims[p];
	    Uchar min, max;
	    for (int v = 0; v < 4; v++) {
	       if ((min = ims[p + v8[v]]) > centre) {
		  max = min;
		  min = centre;
	       } else {
		  max = centre;
	       }
	       for (int k = min; k <= max; k++) {
		  hg[k] += (float)val;
	       }
	    }
	 }
      }
   }
   
   // Smoothing with a symetric exponential to 
   // ease the detection of the mximazum.
   for (int k = 1; k < histogramSize; k++) {
      hg[k] = (float)(ALPHA * hg[k] + (1.0F - ALPHA) * hg[k - 1]);
   }
   for (int k = histogramSize-  2; k >= 0; k--) {
      hg[k] = (float)(ALPHA * hg[k] + (1.0F - ALPHA) * hg[k + 1]);
   }

   // Maximum detection.
   int maximum = 0;
   hg[0]=0; // remove the background.
   for (int i = 1; i < histogramSize - 1; i++) {
      if ( hg[i - 1] < hg[i] && hg[i] > hg[i + 1]) {
	 if (hg[i] > hg[maximum]) {// Must be > and could be (float)
	    maximum = i;
	 }
      }
   }
   
   for (p.y = 0; p.y < ims.Height(); p.y++) {
      for (p.x = 0; p.x < ims.Width(); p.x++) {
	 if (ims[p] <= maximum) {
	    imd[p] = 0;
	 } else {
	    imd[p] = 255;
	 }
      }
   }

   return maximum;
}

Errc PContrastBinarization( const Img3duc &ims, Img3duc &ima, Img3duc &imd ) {
   const int histogramSize = MAXUCHAR + 1;
   float hg[histogramSize] = {0};

   Point3d p;
   for (p.z = 1; p.z < ims.Depth(); p.z++) {
      for (p.y = 1; p.y < ims.Height(); p.y++) {
	 for (p.x = 1; p.x < ims.Width() - 1; p.x++) {
	    Uchar val = ima[p];
	    if (val > 0) {
	       Uchar min, max;
	       Uchar centre = ims[p];
	       for (int v = 0; v < 13; v++) {
		  if ((min = ims[p + v26[v]]) > centre) {
		     max = min;
		     min = centre;
		  } else {
		     max = centre;
		  }
		  for (int k = min; k <= max; k++) {
		     hg[k] += (float)val; // Use -hg with minima.
		  }
	       }
	    }
	 }
      }
   }
   
   // Smoothing with a symetric exponential.
   for (int k = 1; k < histogramSize; k++) {
      hg[k] = (float)(ALPHA * hg[k] + (1.0F - ALPHA) * hg[k - 1]);
   }
   for (int k = histogramSize - 2; k >= 0; k--) {
      hg[k] = (float)(ALPHA * hg[k] + (1.0F - ALPHA) * hg[k + 1]);
   }

   // Maximum detection.
   int maximum = 0;
   hg[0] = 0;
   for (int i = 1; i < histogramSize - 1; i++) {
      if ( hg[i - 1] < hg[i] && hg[i] > hg[i + 1]) {
	 if (hg[i] > hg[maximum]) { // Must be > and could be (float)
	    maximum = i;
	 }
      }
   }

   // Use thresholds to build the output image.
   for (p.z = 0; p.z < ims.Depth(); p.z++) {
      for (p.y = 0; p.y < ims.Height(); p.y++) {
	 for (p.x = 0; p.x < ims.Width(); p.x++) {
	    if (ims[p] <= maximum) {
	       imd[p] = 0;
	    } else {
	       imd[p] = 255;
	    }
	 }
      }
   }
   
   return maximum;
}

#ifdef MAIN

/*
 * Modify only the following constants, and the function call.
 */
#define USAGE	"usage: %s [-m mask] [im_in|-] [im_amp|-] [im_out|-]"
#define PARC	0
#define FINC	2
#define FOUTC	1
#define MASK	1

int main( int argc, char *argv[] ) {
   Errc result;                // The result code of the execution.
   Pobject* mask;              // The region mask.
   Pobject* objin[FINC + 1];   // The input objects.
   Pobject* objs[FINC + 1];    // The source objects masked by the mask.
   Pobject* objout[FOUTC + 1]; // The output object.
   Pobject* objd[FOUTC + 1];   // The result object of the execution.
   char* parv[PARC + 1];       // The input parameters.
   
   ReadArgs(argc, argv, PARC, FINC, FOUTC, &mask, objin, objs, objout, objd, parv, USAGE);
   
   switch (objs[0]->Type()) {
   case Po_Img2duc : {
      Img2duc* const ims = (Img2duc*)objs[0];
      Img2duc* const ima = (Img2duc*)objs[1];
      objd[0] = new Img2duc(ims->Size());
      Img2duc* const imd = (Img2duc*)objd[0];
      
      result = PContrastBinarization(*ims, *ima, *imd);
   } break;
   case Po_Img3duc :{
      Img3duc* const ims = (Img3duc*)objs[0];
      Img3duc* const ima = (Img3duc*)objs[1];
      objd[0] = new Img3duc(ims->Size());
      Img3duc* const imd = (Img3duc*)objd[0];
      
      result = PContrastBinarization(*ims, *ima, *imd);
   } break;
   default:
      PrintErrorFormat(objin,FINC);
      result = FAILURE;
   }
   
   if (result) {
      WriteArgs(argc, argv, PARC, FINC, FOUTC, &mask, objin, objs, objout, objd, MASK);
   }
   
   Exit(result);
   return 0;
}

#endif
