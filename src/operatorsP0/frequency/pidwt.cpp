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

/**
 * @author Ludovic SOLTYS - 02-02-2003
 * @author Régis Clouard - 2008-10-22 (added free allocated memory)
 */

#include <math.h>
#include <pandore.h>
using namespace pandore;

/**
 * @file pidwt.cpp
 * operator source: idwt carré
 */
typedef struct filtre {
   unsigned long size;  /* Taille du filtre                     */
   unsigned int shift;  /* Origine du filtre                     */
   float *H;            /* Vecteur des coefficients du filtre PB */
   float *G;            /* Vecteur des coefficients du filtre PH */
} FILTRE;

static int nextpow2( double num ) {
   int exp;   
   double f = frexp(fabs(num), &exp);
   
   if (f && f == 0.5) {
      exp = exp - 1;
   }
   
   return exp;
}

/*
 * Expansion-Convolution
 *
 * Expansion-Convolution d'un signal par un filtre Filt.
 * Cette fonction applique une étape de reconstruction
 * de l'algorithme pyramidal.
 * Elle convolue la TOD d'entrée input par le
 * filtre passe-bas h (resp. passe-haut g) pour
 * l'approximation et le détail, en ayant placé un zéro
 * un échantillon sur deux. Les coefficients de l'approximation sont
 * supposés stockés dans la moitié basse du champ Y de l'entrée
 * input et les détails dans la moitié haute. A la sortie, le
 * champ Y de output contient l'approximation à l'échelle
 * juste au dessus.
 * Le traitement des effets de bord se fait par périodisation 
 * du motif du signal convolué.
 */
void ExpandConv( float *input, float *output, FILTRE Filt, int length ) {
   float ai, ai1;
   unsigned long i, ii, j, jf, jr, k, n, n1, ni, nj, nh, nmod;
   
   n = length ;
   if (n < 4 || !input || !output) {
      return;
   }
   
   nmod = Filt.size * n;
   n1 = n - 1;
   nh = n >> 1;
   
   for (j = 1; j <= n; j++) {
      output[j] = 0;
   }
   
   for (ii = 1, i = 1; i <= n; i += 2, ii++) {
      ai = input[ii];
      ai1 = input[ii + nh];
      ni = i + nmod + Filt.shift;
      nj = i + nmod + Filt.shift;
	 
      for (k = 1; k <= Filt.size; k++) {
	 jf = (n1 & (ni + k)) + 1;
	 jr = (n1 & (nj + k)) + 1;
	 output[jf] += Filt.H[k] * ai;
	 output[jr] += Filt.G[k] * ai1;
      }
   }
}

/*
 * Inverse Wavelet Transform
 */
Errc PIdwt( const Img2dsf &ims, const Collection &c, Img2dsf &imd, int scale = 1 ) {	
   int y, x, m;
   int width = ims.Width();
   int height = ims.Height();
   long k;

   FILTRE filter;

   int J = nextpow2(ims.Width());
   if (scale > J) {
      std::cerr << "Error pidwt: Incorrect scale value : scale > nexpow2(N)" << std::endl;
      return FAILURE;
   }

   float *data = new float[width + 1]; 
   float *work = new float[width + 1];
	
   char * nom = c.GETARRAY("filtre", Char);

   filter.H = c.GETARRAY(nom, Float);
   filter.size = c.GETARRAYSIZE(nom, Float) - 1;

   // Calcul de G	
   int s = -1;
   if (!filter.H) {
      std::cerr << "Error pidwt: Wrong filter." << std::endl;
      return FAILURE;
   }

   filter.G = new float[filter.size + 1];

   for (k = 1; k <= (long)filter.size; k++) {
      filter.G[filter.size + 1 - k] = s * filter.H[k];
      s = -s;
   }

   filter.shift = (unsigned int)(-(int)(filter.size >> 1));

   // Copie de l'image d'entrée vers l'image de sortie
   for (y = 0; y < ims.Height(); y++) {
      for (x = 0; x < ims.Width(); x++) {
	 imd[y][x] = ims[y][x];
      }
   }

   for (k = scale - 1; k >= 0; k--) {

      width  = ims.Width()  >> k;
      height = ims.Height() >> k;	
	
      // Reconstruction verticale
      for (x = 0; x < width; x++) {
	 for (y = 0; y < height; y++) {
	    data[y + 1] = imd[y][x];
	 }
	 
	 ExpandConv(data, work, filter, height);
			
	 for (m = 0; m < height; m++) {
	    imd[m][x] = work[m + 1];
	 }
      }

      // Reconstruction horizontale
      for (y = 0; y < height; y++) {
	 for (x = 0; x < width; x++) {
	    data[x + 1] = imd[y][x];
	 }

	 ExpandConv(data, work, filter, width);

	 for (m = 0; m < width; m++) {
	    imd[y][m] = work[m + 1];
	 }
      }
   }
   
   delete[] data;
   delete[] work;
   delete[] filter.G;
   return SUCCESS;
}

#ifdef MAIN

#define USAGE   "usage: %s scale [im_in|-] [col_in|-] [im_out|-]"
#define PARC    1
#define FINC    2
#define FOUTC   1
#define MASK    0

int main( int argc, char *argv[] ) {
   Errc result;                // The result code of the execution.
   Pobject *mask;              // The mask.
   Pobject *objin[FINC + 1];   // The input objects.
   Pobject *objs[FINC + 1];    // The source objects masked by mask.
   Pobject *objout[FOUTC + 1]; // The output object unmasked by mask.
   Pobject *objd[FOUTC + 1];   // The result object.
   char *parv[PARC + 1];       // The input parameters.

   ReadArgs(argc, argv, PARC, FINC, FOUTC, &mask, objin, objs, objout, objd, parv, USAGE, MASK);

   if (objs[0]->Type() == Po_Img2dsf && objs[1]->Type() == Po_Collection) {
      Img2dsf* const ims = (Img2dsf*)objs[0];
      Collection* const col_in = (Collection *) objs[1];      
      objd[0] = new Img2dsf(ims->Size());
      Img2dsf* const imd = (Img2dsf*)objd[0];
      
      result = PIdwt(*ims, *col_in, *imd, atoi(parv[0]));
   } else {
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
