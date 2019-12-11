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
 * @file pdwt.cpp
 * operator source: dwt carré
 */

typedef struct filtre {
   unsigned long size; // Taille du filtre
   unsigned int shift; // Origine du filtre
   float *H;           // Vecteur des coefficients du filtre P
   float *G;           // Vecteur des coefficients du filtre PH
} FILTRE;

static int nextpow2( double num ) {
   double f;
   int exp;
   
   f = frexp(fabs(num), &exp);
   
   if (f && f == 0.5) {
      exp = exp - 1;
   }
   
   return exp;
}

/*
 * Convolution-decimation d'un signal par un filtre Filt.
 *
 * Cette fonction applique une étape de décomposition
 * de l'algorithme pyramidal.
 * Elle convolue le signal d'entrée input par le
 * filtre passe-bas ~h (resp. passe-haut ~g) pour		 
 * l'approximation et le détail, et ensuite sous-échantillonne
 * par un facteur 2. Les coefficients de l'approximation sont
 * stockés dans la moitié basse de la sortie output
 * et les détails dans la moitié haute.	 
 * Le traitement des effets de bord se fait par
 * périodisation du motif du signal convolué.
 */
void ConvDecim( float *input, float *output, FILTRE Filt, int length ) {
   unsigned long i, ii, jf, jr, k, n, n1, ni, nj, nh, nmod;

   n = length ;
   if (n < 4 || !output || !input) {
      return;
   }
		
   nmod = Filt.size * n;
   n1 = n - 1;
   nh = n >> 1;

   for (i = 0; i <= n; i++) {
      output[i] = 0;
   }

   for (ii = 1, i = 1; i <= n; i += 2, ii++) {
      ni = i + nmod + Filt.shift;
      nj = i + nmod + Filt.shift;
      
      for (k = 1; k <= Filt.size; k++) {
	 jf = (n1 & (ni + k));
	 jr = (n1 & (nj + k));
	 
	 output[ii]      += (float)(Filt.H[k] * (float)input[jf + 1]);
	 output[ii + nh] += (float)(Filt.G[k] * (float)input[jr + 1]);
      }
   }
}

/**
 * Direct Wavelet Transform
 */
Errc PDwt( const Img2duc &ims, const Collection &c, Img2dsf &imd, int scale=1 ) {
   int width = imd.Width();
   int height = imd.Height();
   int x, y, m;
   long k;

   FILTRE filter;
   
   int max = (width > height) ? width : height;
   int min = (width < height) ? width : height;

   if (scale>nextpow2(min)) {
      std::cerr << "Error pdwt: Incorrect scale value : scale > nexpow2(N)" << std::endl;
      return FAILURE;
   }
      
   float *data = new float[max + 1]; 
   float *work = new float[max + 1];
   
   char * nom = c.GETARRAY("filtre", Char);
   
   filter.H = c.GETARRAY(nom, Float);
   filter.size = c.GETARRAYSIZE(nom, Float) - 1;
   
   // Calcul de G	
   int s = -1;

   if (!filter.H) {
      std::cerr << "Error pdwt: Wrong filter." << std::endl;
      return FAILURE;
   }
   
   filter.G=new float[filter.size + 1];
   
   for (k = 1; k <= (long)filter.size; k++) {
      filter.G[filter.size + 1 - k] = s * filter.H[k];
      s = -s;
   }
   
   filter.shift = (unsigned int)(-(int)(filter.size >> 1));
   // Fin Calcul de G

   // Copie de l'image d'entrée vers l'image de sortie
   for (y = 0; y < ims.Height(); y++) {
      for (x = 0; x < ims.Width(); x++) {
	 imd[y][x] = (float)ims[y][x];
      }
   }
   // Décomposition			
   for (k = 1; k <= scale; k++) {
      // Décomposition horizontale
      for (y = 0; y < height; y++) {
	 for (x = 0; x < width; x++) {
	    data[x+1] = imd[y][x];
	 }
	 
	 ConvDecim(data, work, filter, width);
	 
	 for (m = 0; m < width; m++) {
	    imd[y][m] = work[m + 1];
	 }
	 
      }
      
      // Décomposition verticale
      for (x = 0; x < width; x++) {
	 for (y = 0; y < height; y++) {
	    data[y + 1] = imd[y][x];
	 }
	 
	 ConvDecim(data, work, filter, height);
	 
	 for (m = 0; m < height; m++) {
	    imd[m][x] = work[m + 1];
	 }
      }
      
      height = height >> 1;
      width  = width  >> 1;
   }
   
   delete[] data;
   delete[] work;
   delete[] filter.G;

   return SUCCESS;
}

Errc PDwt( const Img2dsl &ims, const Collection &c, Img2dsf &imd, int scale = 1 ) {
   int width = imd.Width();
   int height = imd.Height();
   int x, y, m;
   long k;

   FILTRE filter;
   
   int max=(width > height) ? width : height;
   int min=(width < height) ? width : height;
   
   if (scale > nextpow2(min)) {
      std::cerr << "Error pdwt: Incorrect scale value : scale > nexpow2(N)" << std::endl;
      return FAILURE;
   }

   float *data = new float[max + 1]; 
   float *work = new float[max + 1];
   
   char * nom  =c.GETARRAY("filtre", Char);
   
   filter.H = c.GETARRAY(nom, Float);
   filter.size = c.GETARRAYSIZE(nom, Float) - 1;
   
   // Calcul de G	
   int s = -1;

   if (!filter.H) {
      std::cerr << "Error pdwt: Wrong filter." << std::endl;
      return FAILURE;
   }
   
   filter.G = new float[filter.size + 1];
   
   for (k = 1; k <= (long)filter.size; k++) {
      filter.G[filter.size + 1 - k] = s * filter.H[k];
      s = -s;
   }
   
   filter.shift = (unsigned int)(-(int)(filter.size >> 1));
   // Fin Calcul de G

   // Copie de l'image d'entrée vers l'image de sortie
   for (y = 0; y < ims.Height(); y++) {
      for (x = 0; x < ims.Width(); x++) {
	 imd[y][x] = (float)ims[y][x];
      }
   }
   
   // Décomposition			
   for (k = 1; k <= scale; k++) {
      // Décomposition horizontale
      for (y = 0; y < height; y++) {
	 for (x = 0; x < width; x++) {
	    data[x + 1] = imd[y][x];
	 }
	 
	 ConvDecim(data, work, filter, width);
	 
	 for (m = 0; m < width; m++) {
	    imd[y][m] = work[m + 1];
	 }
	 
      }
      
      // Décomposition verticale
      for (x = 0; x < width; x++) {
	 for (y=  0; y < height; y++) {
	    data[y + 1] = imd[y][x];
	 }
	 
	 ConvDecim(data, work, filter, height);
	 
	 for (m = 0; m < height; m++) {
	    imd[m][x] = work[m + 1];
	 }
      }
      
      height = height >> 1;
      width  = width  >> 1;
   }
   
   delete[] data;
   delete[] work;
   delete[] filter.G;

   return SUCCESS;
}


Errc PDwt( const Img2dsf& ims, const Collection &c, Img2dsf& imd, int scale=1 ) {
   int width = imd.Width();
   int height = imd.Height();
   int x, y, m;
   long k;

   FILTRE filter;
   
   int max = (width > height) ? width : height;
   int min = (width < height) ? width : height;
   
    if (scale > nextpow2(min)) {
      std::cerr << "Error pdwt: Incorrect scale value : scale > nexpow2(N)" << std::endl;
      return FAILURE;
   }
   
   float *data=new float[max+1]; 
   float *work=new float[max+1];
     
   char * nom=c.GETARRAY("filtre", Char);
   
   filter.H= c.GETARRAY(nom, Float);
   filter.size=c.GETARRAYSIZE(nom, Float)-1;
   
   // Calcul de G	
   int s = -1;

   if (!filter.H) {
      std::cerr << "Error pdwt: Wrong filter." << std::endl;
      return FAILURE;
   }
   
   filter.G = new float[filter.size + 1];
   
   for (k = 1; k <= (long)filter.size; k++) {
      filter.G[filter.size + 1 - k] = s * filter.H[k];
      s = -s;
   }
   
   filter.shift = (unsigned int)(-(int)(filter.size >> 1));
   // Fin Calcul de G

   // Copie de l'image d'entrée vers l'image de sortie
   for (y = 0; y < ims.Height(); y++) {
      for (x = 0; x < ims.Width(); x++) {
	 imd[y][x] = ims[y][x];
      }
   }
   // Décomposition			
   for (k = 1;k <= scale; k++) {
      // Décomposition horizontale
      for (y = 0; y < height; y++) {
	 for (x = 0; x < width; x++) {
	    data[x + 1] = imd[y][x];
	 }
	 
	 ConvDecim(data, work, filter, width);
	 
	 for (m = 0; m < width; m++) {
	    imd[y][m] = work[m + 1];
	 }
      }
      
      // Décomposition verticale
      for (x = 0; x < width; x++) {
	 for (y = 0; y < height; y++) {
	    data[y + 1] = imd[y][x];
	 }
	 
	 ConvDecim(data, work, filter, height);
	 
	 for (m = 0; m < height; m++) {
	    imd[m][x] = work[m + 1];
	 }
      }
      
      height = height >> 1;
      width  = width  >> 1;
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
   char* parv[PARC + 1];       // The input parameters.
   
   ReadArgs(argc, argv, PARC, FINC, FOUTC, &mask, objin, objs, objout, objd, parv, USAGE, MASK);
   
   if (objs[0]->Type() == Po_Img2duc && objs[1]->Type() == Po_Collection) {
      Img2duc* const ims = (Img2duc*)objs[0];	
      Collection* const colin = (Collection *) objs[1];
      
      int I = nextpow2(ims->Height());
      int J = nextpow2(ims->Width());
      
      objd[0] = new Img2dsf((long)pow(2.0, I), (long)pow(2.0, J));
      
      Img2dsf* const imd = (Img2dsf*)objd[0];
      
      result = PDwt(*ims, *colin, *imd, atoi(parv[0]));
      
   } else if (objs[0]->Type() == Po_Img2dsf && objs[1]->Type() == Po_Collection) {
      Img2dsf* const ims = (Img2dsf*)objs[0];	
      Collection* const colin = (Collection *) objs[1];
      
      int I = nextpow2(ims->Height());
      int J = nextpow2(ims->Width());
      
      objd[0] = new Img2dsf((long)pow(2.0, I), (long)pow(2.0, J));
      
      Img2dsf* const imd = (Img2dsf*)objd[0];
      
      result = PDwt(*ims, *colin, *imd, atoi(parv[0]));
      
   } else if (objs[0]->Type() == Po_Img2dsl && objs[1]->Type() == Po_Collection) {
      Img2dsl* const ims = (Img2dsl*)objs[0];
      Collection* const colin = (Collection *) objs[1];
      
      int I = nextpow2(ims->Height());
      int J = nextpow2(ims->Width());
      
      objd[0] = new Img2dsf((long)pow(2.0, I), (long)pow(2.0, J));
      
      Img2dsf* const imd = (Img2dsf*)objd[0];
      
      result = PDwt(*ims, *colin, *imd, atoi(parv[0]));
      
   } else if (objs[0]->Type() == Po_Img2dsf && objs[1]->Type() == Po_Collection) {
      Img2dsf* const ims = (Img2dsf*)objs[0];	
      Collection* const colin = (Collection *) objs[1];
      
      int I = nextpow2(ims->Height());
      int J = nextpow2(ims->Width());
      
      objd[0] = new Img2dsf((long)pow(2.0, I), (long)pow(2.0, J));
      
      Img2dsf* const imd = (Img2dsf*)objd[0];
      
      result  =PDwt(*ims, *colin, *imd, atoi(parv[0]));
      
   } else{
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
