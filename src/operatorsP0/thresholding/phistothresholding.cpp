/* -*- c-basic-offset: 3 -*-
 *
 * Copyright (c),  GREYC.
 * All rights reserved
 *
 * You may use this file under the terms of the BSD license as follows:
 *
 * "Redistribution and use in source and binary forms,  with or without
 * modification,  are permitted provided that the following conditions are
 * met:
 *   * Redistributions of source code must retain the above copyright
 *     notice,  this list of conditions and the following disclaimer.
 *   * Redistributions in binary form must reproduce the above copyright
 *     notice,  this list of conditions and the following disclaimer in
 *     the documentation and/or other materials provided with the
 *     distribution.
 *   * Neither the name of the GREYC,  nor the name of its
 *     contributors may be used to endorse or promote products
 *     derived from this software without specific prior written
 *     permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,  INCLUDING,  BUT NOT
 * LIMITED TO,  THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT,  INDIRECT,  INCIDENTAL, 
 * SPECIAL,  EXEMPLARY,  OR CONSEQUENTIAL DAMAGES (INCLUDING,  BUT NOT
 * LIMITED TO,  PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, 
 * DATA,  OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY,  WHETHER IN CONTRACT,  STRICT LIABILITY,  OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE,  EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
 *
 * 
 * For more information,  refer to:
 * https://clouard.users.greyc.fr/Pandore
 */

/**
 * @author Olivier Lezoray - 1997-03-03
 */

#include <math.h>
#include <pandore.h>
using namespace pandore;

/**
 * @file phistothresholding.cpp
 * Classification basee sur l'analyse des maxima de l'histogramme.
 * La determination des seuils est faite par LPE.
 */

#define ALPHA 0.5F	// Smoothing degree.

Long PHistoThresholding( const Img2duc &ims,   Img2duc &imd,   int length ) {
   float H[MAXUCHAR + 1];
   int x,  y,  k,  j;
   Uchar seuil[MAXUCHAR + 1];
   Uchar couleur[MAXUCHAR + 1];
   float max,  val;
   bool minimum;
   int v,   min;
   Fifo<Point1d> f[MAXUSHORT + 1];
   Point1d p,  p1;
   
   for (k = 0; k < MAXUCHAR + 1; k++) {
      H[k] = 0.0F;
      couleur[k] = 0;
   }

   for (y = 0; y < ims.Height(); y++) {
      for (x = 0; x < ims.Width(); x++) {
	 H[int(ims[y][x])]++;
      }
   }

   // Smoothing with a symetric exponential.
   for (k = 1; k < MAXUCHAR + 1; k++) {
      H[k] = ALPHA * (H[k] - H[k - 1])+ H[k - 1];
   }
   max = 0.0F;
   for (k = MAXUCHAR - 1; k >= 0; k--) {
      H[k] = ALPHA * (H[k] - H[k + 1]) + H[k + 1];
      if (H[k] > max) {
	 max = H[k];
      }
   }

   // On inverse l'histogramme pour que
   // les maxima deviennent les minima.
   if (max > MAXUSHORT) {
      max = MAXUSHORT;
   }
   for (k = 0; k < MAXUCHAR + 1; k++) {
      H[k] = max - H[k];
      if (H[k] < 0) {
	 H[k]  =0;
      }
   }

   if (length < 0) {
      length = 1;
   }
   if (length > MAXUCHAR / 2) {
      length = MAXUCHAR / 2;
   }

   // Look for regional minima
   // to determine seeds.
   long labels = 0;
   for (k = 1; k < MAXUCHAR + 1; k++) {
      minimum = true;
      for (j = 1; j <= length; j++) {
	 if ((((k - j) >= 0)  && (H[k - j] <= H[k])) || (((k + j) <= MAXUCHAR) && (H[k + j] <= H[k]))) {
	    minimum = false;
	    break;
	 }
      }
      if (minimum) {
	 val = H[k];
	 p.x = k;
	 couleur[k] = (Uchar)++labels;
	 f[(int)val].Push(p);
      }
   }

   // Markers growing.
   min = 0;
   while (min <= (int)max) {
      k = min++;
      while (!f[k].Empty()) {
 	 p = f[k].Pop();
 	 for (v = 0; v < 2; v++) {
	    p1.x = p.x + v2[v].x;
 	    if ((p1.x >= 0) && (p1.x <= MAXUCHAR) && (couleur[p1.x] == 0)) {
	       val = H[p1.x];
	       if ((int)val < min) {
		  min = (int)val;
	       }
 	       f[(int)val].Push(p1);
 	       couleur[p1.x] = couleur[p.x];
 	    }
	 }
      }
   }

   // Trouver les seuils
   labels = 0;
   for (k = 0; k < MAXUCHAR + 1; k++) {
      if (couleur[k] != couleur[k + 1]) {
	 seuil[labels++] = k;
      }
   }
   seuil[labels] = MAXUCHAR;

   // Seuiller l'image en l'etiquettant
   // Calcul de l'image de sortie
   // en attribuant des etiquettes croissantes: 1,   2,   ...
   for (y = 0; y < ims.Height(); y++) {
      for (x = 0; x < ims.Width(); x++) {
	 for (k = 0; k <= labels; k++) {
	    if (ims[y][x] <= seuil[k]) {
	       imd[y][x] = seuil[k];
	       break;
	    }
	 }
      }
   }
   
   return labels;
}

Long PHistoThresholding( const Img3duc &ims,   Img3duc &imd,  int length ) {
   float H[MAXUCHAR + 1];
   int x, y, z, k, j;
   Uchar seuil[MAXUCHAR + 1];
   Uchar couleur[MAXUCHAR + 1];
   float max, val;
   bool minimum;
   int v,  min;
   Fifo<Point1d> f[MAXUSHORT + 1];
   Point1d p, p1;
   
   for (k = 0; k < MAXUCHAR + 1; k++) {
      H[k] = 0.0F;
      couleur[k] = 0;
   }

   for (z = 0; z < ims.Depth(); z++) {
      for (y = 0; y < ims.Height(); y++) {
	 for (x = 0; x < ims.Width(); x++) {
	    H[int(ims[z][y][x])]++;
	 }
      }
   }
   // Smoothing with a symetric exponential.
   for (k = 1; k < MAXUCHAR + 1; k++) {
      H[k] = ALPHA * (H[k] - H[k-1]) + H[k - 1];
   }
   max = 0.0F;
   for (k = MAXUCHAR - 1; k >= 0; k--) {
      H[k] = ALPHA * (H[k] - H[k + 1]) + H[k + 1];
      if (H[k] > max) {
	 max = H[k];
      }
   }

   // On inverse l'histogramme pour que
   // les maxima deviennent les minima.
   if (max > MAXUSHORT) {
      max = MAXUSHORT;
   }
   for (k = 0; k < MAXUCHAR + 1; k++) {
      H[k] = max - H[k];
      if (H[k] < 0) {
	 H[k] = 0;
      }
   }

   if (length < 0) {
      length = 1;
   }
   if (length > MAXUCHAR / 2) {
      length = MAXUCHAR / 2;
   }
   
   // Look for regional minima
   // to determine seeds.
   long labels = 0;
   for (k = 1; k < MAXUCHAR + 1; k++) {
      minimum = true;
      for (j = 1; j <= length; j++) {
	 if ((((k - j) >= 0)  && (H[k - j] <= H[k])) || (((k + j) <= MAXUCHAR) && (H[k + j] <= H[k]))) {
	    minimum=false;
	    break;
	 }
      }
      if (minimum) {
	 val = H[k];
	 p.x = k;
	 couleur[k] = (Uchar)++labels;
	 f[(int)val].Push(p);
      }
   }

   // Markers growing.
   min = 0;
   while (min <= (int)max) {
      k = min++;
      while (!f[k].Empty()) {
 	 p = f[k].Pop();
 	 for (v = 0; v < 2; v++) {
	    p1.x = p.x + v2[v].x;
 	    if ((p1.x >= 0) && (p1.x <= MAXUCHAR) && (couleur[p1.x] == 0)) {
	       val = H[p1.x];
	       if ((int)val < min) {
		  min = (int)val;
	       }
 	       f[(int)val].Push(p1);
 	       couleur[p1.x] = couleur[p.x];
 	    }
	 }
      }
   }

   // Trouver les seuils
   labels = 0;
   for (k = 0; k < MAXUCHAR + 1; k++) {
      if (couleur[k] != couleur[k + 1]) {
	 seuil[labels++] = k;
      }
   }
   seuil[labels] = MAXUCHAR;

   // Seuiller l'image en l'etiquettant
   // Calcul de l'image de sortie
   // en attribuant des etiquettes croissantes: 1,  2,  ...
   for (z = 0; z < ims.Depth(); z++) {
      for (y = 0; y < ims.Height(); y++) {
	 for (x = 0; x < ims.Width();x++) {
	    for (k = 0; k <= labels; k++) {
	       if (ims[z][y][x] <= seuil[k]) {
		  imd[z][y][x] = seuil[k];
		  break;
	       }
	    }
	 }
      }
   }   
   return labels;
}

#ifdef MAIN

/*
 * Modify only the following constants,  and the operator switches.
 */
#define	USAGE	"usage: %s length [-m mask] [im_in|-] [im_out|-]"
#define	PARC	1
#define	FINC	1
#define	FOUTC	1
#define MASK	1

int main( int argc,  char *argv[] ) {
   Errc result;                // The result code of the execution.
   Pobject* mask;              // The region mask.
   Pobject* objin[FINC + 1];   // The input objects.
   Pobject* objs[FINC + 1];    // The source objects masked by the mask.
   Pobject* objout[FOUTC + 1]; // The output object.
   Pobject* objd[FOUTC + 1];   // The result object of the execution.
   char* parv[PARC + 1];       // The input parameters.

   ReadArgs(argc, argv, PARC, FINC, FOUTC, &mask, objin, objs, objout, objd, parv, USAGE, MASK);

   switch (objs[0]->Type()) {
   case Po_Img2duc : {
      Img2duc* const ims = (Img2duc*)objs[0];
      objd[0] = new Img2duc(ims->Size());
      Img2duc* const imd = (Img2duc*)objd[0];
	      
      result = PHistoThresholding(*ims, *imd, atoi(parv[0]));
   } break;
   case Po_Img3duc : {
      Img3duc* const ims = (Img3duc*)objs[0];
      objd[0] = new Img3duc(ims->Size());
      Img3duc* const imd = (Img3duc*)objd[0];
	      
      result = PHistoThresholding(*ims, *imd, atoi(parv[0]));
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
