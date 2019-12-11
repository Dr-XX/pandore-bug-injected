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
 * @file pderavi.cpp
 * Multi-seuillage a partir d'une matrice de co-occurence
 * selon deravi et Pal.
 * D'apres C. FERNANDEZ-MALOIGNE.
 */

#define ALPHA 0.5F	// Smoothing degree.

/*
 * Imd est contruite avec les valeurs de seuils detectes.
 */
Errc PDeravi( const Img2duc &ims, Img2duc &imd, int length ) {
   Long hg[MAXUCHAR + 1][MAXUCHAR + 1] = {{0}};
   Float E[MAXUCHAR + 1];

   // Compute co-occurrence matrix 
   // Byx = {(y+1, x), (y, x+1)}
   for (int y = 1; y < ims.Height() - 1; y++) {
      for (int x = 1; x < ims.Width() - 1; x++) {
	 if (ims(y, x + 1) == ims(y + 1, x)) {
	    hg[ims(y, x)][ims(y, x + 1)]++;
	 }
      }
   }
   
   // Compute the transition probabilities.
   for (int k = 0; k < MAXUCHAR + 1; k++) {
      Long numerator = 0;
      Long denominator;
      for (int i = 0; i <= k; i++) {
	 for (int j = k + 1; j < MAXUCHAR + 1; j++) {
	    numerator += hg[i][j];
	 }
	 denominator = numerator;
	 for (int j = 0; j <= k; j++) {
	    denominator += hg[i][j];
	 }
      }
      Float p1;
      if (denominator) {
	 p1 = Float(numerator) / Float(denominator);
      } else {
	 p1 = 0.0;
      }
      numerator = 0;
      for (int i = k + 1; i < MAXUCHAR + 1; i++) {
	 for (int j = 0; j <= k; j++) {
	    numerator += hg[i][j];
	 }
	 denominator = numerator;
	 for (int j = k + 1; j < MAXUCHAR + 1; j++) {
	    denominator += hg[i][j];
	 }
      }
      Float p2;
      if (denominator) {
	 p2 = Float(numerator) / Float(denominator);
      } else {
	 p2 = 0.0;
      }
      E[k] = (p1 + p2) / 2.0F;
   }

   // Smoothing with a symetric exponential.
   for (int k = 1 ; k < MAXUCHAR + 1; k++) {
      E[k] = ALPHA * (E[k] - E[k - 1]) + E[k - 1];
   }
   for (int k = MAXUCHAR - 1; k >= 0; k--) {
      E[k] = ALPHA * (E[k] - E[k + 1]) + E[k + 1];
   }
   
   if (length < 0) {
      length = 1;
   }
   if (length > MAXUCHAR / 2) {
      length = MAXUCHAR / 2;
   }

   // Look for regional minima to determine thresholds.
   Uchar seuil[MAXUCHAR + 1];
   bool minimum;
   Long labels = 0;
   for (int i = 1; i < MAXUCHAR + 1; i++) {
      minimum = true;
      for (int j = 1; j <= length; j++) {
	 if ((((i-j) >= 0)  && (E[i - j] <= E[i])) || (((i + j) <= MAXUCHAR) && (E[i + j] <= E[i]))) {
	    minimum = false;
	    break;
	 }
      }
      if (minimum) {
	 seuil[labels++] = (Uchar)i;
      }
   }
   seuil[labels] = MAXUCHAR;

   // Use thresholds to build the output image.
   // imd[y][x]=seuil[k] si imd[y][x]<=seuil[k].
   for (int y = 0; y < ims.Height(); y++) {
      for (int x = 0; x < ims.Width(); x++) {
	 for (int k = 0; k <= labels; k++) {
	    if (ims(y, x) <= seuil[k]) {
	       imd(y, x) = seuil[k];
	       break;
	    }
	 }
      }
   }
   
   return labels;
}

#ifdef MAIN

/*
 * Modify only the following constants, and the function call.
 */
#define	USAGE	"usage: %s length [-m mask] [im_in|-] [im_out|-]"
#define	PARC	1
#define	FINC	1
#define	FOUTC	1
#define	MASK	1

int main( int argc, char *argv[] ) {
   Errc result;	               // The result code of the execution.
   Pobject* mask;	       // The region mask.
   Pobject* objin[FINC + 1];   // The input objects.
   Pobject* objs[FINC + 1];    // The source objects masked by the mask.
   Pobject* objout[FOUTC + 1]; // The output object.
   Pobject* objd[FOUTC + 1];   // The result object of the execution.
   char* parv[PARC + 1];       // The input parameters.

   ReadArgs(argc, argv, PARC, FINC, FOUTC, &mask, objin, objs, objout, objd, parv, USAGE, MASK);

   switch (objs[0]->Type()) {
   case Po_Img2duc : {
      Img2duc* const ims = (Img2duc*)objs[0];
      objd[0] = new Img2duc(ims->Props());
      Img2duc* const imd = (Img2duc*)objd[0];

      result = PDeravi(*ims, *imd, atoi(parv[0]));
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
