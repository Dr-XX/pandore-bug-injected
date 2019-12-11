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
 * @file pcontrastthresholding.cpp
 * This thresholding uses the maximum contrast in the boundaries of objets.
 * Thresholds are choosen as the maximum contrast in
 * the boundaries between two regions.
 */

#define ALPHA 0.5F	// Smoothing degree.

/*
 * 1- Computes histogram of boundaries amplitudes.
 * 2- Smoothes the result histogram.
 * 3- Sort maxima.
 * 4- Build nb_class thresholding from the maxima
 */
Errc PContrastThresholding( const Img2duc &ims, Img2duc &ima, Img2duc &imd, int nb_class ) {
   Point2d p;
   int k, v;
   int i, j;
   Uchar min, max, centre, val;
   float hg[MAXUCHAR + 1];
   Uchar *maxima;
   Long nbseuils;
   
   if ((nb_class <= 1) || (nb_class > MAXUCHAR/2)) {
      std::cerr<<"Error pcontrastthresholding: Bad number of class: "<<nb_class<<std::endl;
      return FAILURE;
   }
   nb_class--;

   // 1: Boundaries histogram computing.
   for (i = 0; i <MAXUCHAR + 1; hg[i++] = 0.0F) ;
   
   for (p.y = 1; p.y < ims.Height() - 1; p.y++) {
      for (p.x = 1; p.x < ims.Width()-1; p.x++) {
	 if ((val = ima[p]) > 0) {
	    centre = ims[p];
	    for (v = 0; v < 4; v++) {
	       if ((min = ims[p + v8[v]]) > centre) {
		  max = min;
		  min = centre;
	       } else {
		  max = centre;
	       }
	       for (k = min; k <= max; k++) {
		  hg[k] += val;
	       }
	    }
	 }
      }
   }

   // Smoothing with a symetric exponential.
   for (k = 1; k < MAXUCHAR + 1; k++) {
      hg[k] = (float)(ALPHA * hg[k] + (1.0 - ALPHA) * hg[k - 1]);
   }
   for (k = MAXUCHAR - 1; k >= 0; k--) {
       hg[k] = (float)(ALPHA * hg[k] + (1.0 - ALPHA) * hg[k + 1]);
   }

   // Maxima extraction and sorting by values.
   // Keeps only the nb_class-1 values.
   maxima = (Uchar*)calloc(nb_class, sizeof(Uchar));
   hg[0] = 0;
   nbseuils = 0;
   for (i = 1; i < MAXUCHAR; i++) {
      if ( hg[i - 1] < hg[i] && hg[i] > hg[i + 1]) {
	 for (j = 0; j < nb_class; j++) {
	    if (hg[i] > hg[maxima[j]]) {// Must be > and could be (float)
	       for (int k = nb_class- 2 ; k >= j ; k--) {
		  maxima[j + 1] = maxima[j];
	       }
	       maxima[j] = i;
	       nbseuils++;
	       break;
	    }
	 }
      }
   }
   // Sort by index.
   int temp;
   for (i  =0; i < nb_class - 1; i++) {
      for (j = 0; j < nb_class - 1;j++) {
	 if (maxima[j] > maxima[j + 1]) {
	    temp = maxima[j];
	    maxima[j] = maxima[j + 1];
	    maxima[j + 1] = temp;
	 }
      }
   }
   maxima[nb_class] = 255;

   // Use thresholds to build the output image.
   // imd[y][x]=seuil[k] si imd[y][x]<=seuil[k].
   for (p.y = 0; p.y < ims.Height(); p.y++) {
      for (p.x = 0; p.x < ims.Width(); p.x++) {
	 for (k = 0; k <= nb_class; k++) {
	    if (ims[p] <= maxima[k]) {
	       imd[p] = maxima[k];
	       break;
	    }
	 }
      }
   }
   free(maxima);
   nb_class++;
   if (nbseuils > nb_class) {
      return (Long)nb_class;
   } else {
      return nbseuils;
   }
}

Errc PContrastThresholding( const Img3duc &ims, Img3duc &ima, Img3duc &imd, int nb_class ) {
   Point3d p;
   int k, v;
   int i, j;
   Uchar min, max, centre, val;
   float hg[MAXUCHAR + 1];
   Uchar *maxima;
   Long nbseuils;
   
   if ((nb_class <= 1) || (nb_class > MAXUCHAR  /2)) {
      std::cerr<<"Error pcontrastthresholding: Bad number of class: " << nb_class << std::endl;
      return FAILURE;
   }
   nb_class--;

   // 1: Boundaries histogram computing.
   for (i = 0; i < MAXUCHAR + 1; hg[i++] = 0.0F) ;
   
   for (p.z = 1; p.z < ims.Depth() - 1; p.z++) {
      for (p.y = 1; p.y < ims.Height() - 1; p.y++) {
	 for (p.x = 1; p.x < ims.Width() - 1; p.x++) {
	    if ((val = ima[p]) > 0) {
	       centre = ims[p];
	       for (v  =0; v < 13; v++) {
		  if ((min  =ims[p + v26[v]]) > centre) {
		     max = min;
		     min = centre;
		  } else {
		     max = centre;
		  }
		  for (k = min; k <= max; k++) {
		     hg[k] += (float)val; // Use -hg with minima.
		  }
	       }
	    }
	 }
      }
   }
   
   // Smoothing with a symetric exponential.
   for (k = 1; k < MAXUCHAR + 1; k++) {
      hg[k] = (float)(ALPHA * hg[k] + (1.0 - ALPHA) * hg[k - 1]);
   }
   for (k = MAXUCHAR - 1; k >= 0; k--) {
      hg[k] = (float)(ALPHA * hg[k] + (1.0 - ALPHA) * hg[k + 1]);
   }

   // Maxima extraction and sorting by values.
   // Keeps only the nb_class-1 values.
   maxima = (Uchar*)calloc(nb_class, sizeof(Uchar));
   hg[0] = 0;
   nbseuils = 0;
   for (i = 1; i < MAXUCHAR; i++) {
      if (hg[i - 1] < hg[i] && hg[i] > hg[i + 1]) {
	 for (j = 0; j < nb_class;j ++) {
	    if (hg[i] > hg[maxima[j]]) {// Must be > and could be (float)
	       for (int k = nb_class - 2; k >= j; k--) {
		  maxima[j + 1] = maxima[j];
	       }
	       maxima[j] = i;
	       nbseuils++;
	       break;
	    }
	 }
      }
   }

   // Sort by index.
   int temp;
   for (i = 0; i < nb_class - 1; i++) {
      for (j = 0; j < nb_class - 1; j++) {
	 if (maxima[j] > maxima[j + 1]) {
	    temp = maxima[j];
	    maxima[j] = maxima[j + 1];
	    maxima[j + 1] = temp;
	 }
      }
   }
   maxima[nb_class] = 255;

   // Use thresholds to build the output image.
   // imd[y][x]=seuil[k] si imd[y][x]<=seuil[k].
   for (p.z = 0; p.z < ims.Depth(); p.z++) {
      for (p.y = 0; p.y < ims.Height(); p.y++) {
	 for (p.x=0; p.x < ims.Width(); p.x++) {
	    for (k = 0; k <= nb_class; k++) {
	       if (ims[p] <= maxima[k]) {
		  imd[p] = maxima[k];
		  break;
	       }
	    }
	 }
      }
   }
   
   free(maxima);
   nb_class++;
   if (nbseuils > nb_class) {
      return (Long)nb_class;
   } else {
      return nbseuils;
   }
}

#ifdef MAIN

/*
 * Modify only the following constants, and the function call.
 */
#define USAGE	"usage: %s nb_class [-m mask] [im_in|-] [im_amp|-] [im_out|-]"
#define PARC	1
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
      
      result = PContrastThresholding(*ims, *ima, *imd, atoi(parv[0]));
   } break;
   case Po_Img3duc : {
      Img3duc* const ims = (Img3duc*)objs[0];
      Img3duc* const ima = (Img3duc*)objs[1];
      objd[0] = new Img3duc(ims->Size());
      Img3duc* const imd = (Img3duc*)objd[0];
      
      result = PContrastThresholding(*ims, *ima, *imd, atoi(parv[0]));
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
