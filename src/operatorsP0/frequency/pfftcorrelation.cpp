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

#define _USE_MATH_DEFINES
#include <cmath>
#include <stdio.h>
#include <stdlib.h>
#include <pandore.h>
using namespace pandore;

/**
 * @file pfftcorrelation.cpp
 * Calcul de la correlation entre 2 images.
 * On prend 2 images réelles (on ne s'intéresse pas ici à des images complexes)
 * et on retourne une image réelle qui est la transformée de Fourier inverse
 * du produit des transformées de Fourier des 2 images (on prend le conjugue de
 * l'une des 2 transformees).
 * Le calcul de l'autocorrélation n'est qu'un cas particulier
 * où image1 = image2.
 */

// Inclusion des deux operateurs fft et ifft.
#ifdef MAIN
#undef MAIN
namespace pfft{
#include "pfft.cpp"
}
namespace pifft{
#include "pifft.cpp"
}
#define MAIN

#else
namespace pfft{
#include "pfft.cpp"
}
namespace pifft{
#include "pifft.cpp"
}
#endif



/**
 * Integer 2d gray image.
 * Images MUST have their dimensions all powers of 2.
 */
Errc PFFTCorrelation( const Img2duc &ims1, const  Img2duc &ims2, Img2dsf &imd ) {
   Point2d pt;
   
   if (ims2.Height() > ims1.Height() || ims2.Width() > ims1.Width()) {
      return FAILURE;
   }
   int nrow = ims1.Height();
   int ncol = ims1.Width();
   Img2duc imi(nrow, ncol);
   Img2duc imsr2(nrow, ncol);

   // destination
   nrow = pfft::nextpow2(nrow);
   ncol = pfft::nextpow2(ncol);
   Img2dsf imdr1(nrow, ncol);
   Img2dsf imdi1(nrow, ncol);
   Img2dsf imdr2(nrow, ncol);
   Img2dsf imdi2(nrow, ncol);

   // Build the mask image data.
   // Mask is centered in the result image.
   imsr2 = 0;
   for (pt.y = 0; pt.y < ims2.Height(); pt.y++)
   for (pt.x = 0; pt.x < ims2.Width(); pt.x++)
   {
      imsr2[pt] = ims2[pt];
   }

   imi = 0;
   pfft::PFFT(ims1, imi, imdr1, imdi1);
   pfft::PFFT(imsr2, imi, imdr2, imdi2);

   for (pt.y = 0; pt.y < imdr1.Height(); pt.y++)
   for (pt.x = 0; pt.x < imdr1.Width(); pt.x++)
   {
      Float a, b, c, d;
      
      a = imdr1[pt];
      c = imdr2[pt];
      b = imdi1[pt];
      d = imdi2[pt];
      imdr1[pt] =  a * c + b * d;
      imdi1[pt] = -a * d + b * c;
   }

   pifft::PIFFT(imdr1, imdi1, imdr2, imdi2);

   for (pt.y = 0; pt.y < imd.Height(); pt.y++)
   for (pt.x = 0; pt.x < imd.Width(); pt.x++)
   {
      imd[pt] = imdr2[pt];
   }

   return SUCCESS;
}


/**
 * Integer 2d gray image.
 * Images MUST have their dimensions all powers of 2.
 */
Errc PFFTCorrelation( const Img2dsl &ims1, const  Img2dsl &ims2, Img2dsf &imd ) {
   Point2d pt;
   
   if (ims2.Height() > ims1.Height() || ims2.Width() > ims1.Width()) {
      return FAILURE;
   }
   int nrow = ims1.Height();
   int ncol = ims1.Width();
   Img2dsl imi(nrow, ncol);
   Img2dsl imsr2(nrow, ncol);

   // destination
   nrow = pfft::nextpow2(nrow);
   ncol = pfft::nextpow2(ncol);
   Img2dsf imdr1(nrow, ncol);
   Img2dsf imdi1(nrow, ncol);
   Img2dsf imdr2(nrow, ncol);
   Img2dsf imdi2(nrow, ncol);

   // Build the mask image data.
   // Mask is centered in the result image.
   imsr2 = 0;
   for (pt.y = 0; pt.y < ims2.Height(); pt.y++)
   for (pt.x = 0; pt.x < ims2.Width(); pt.x++)
   {
      imsr2[pt] = ims2[pt];
   }

   imi = 0;
   pfft::PFFT(ims1, imi, imdr1, imdi1);
   pfft::PFFT(imsr2, imi, imdr2, imdi2);

   for (pt.y = 0; pt.y < imdr1.Height(); pt.y++)
   for (pt.x = 0; pt.x < imdr1.Width(); pt.x++)
   {
      Float a, b, c, d;
      
      a = imdr1[pt];
      c = imdr2[pt];
      b = imdi1[pt];
      d = imdi2[pt];
      imdr1[pt] =  a * c + b * d;
      imdi1[pt] = -a * d + b * c;
   }

   pifft::PIFFT(imdr1, imdi1, imdr2, imdi2);

   for (pt.y = 0; pt.y < imd.Height(); pt.y++)
   for (pt.x = 0; pt.x < imd.Width(); pt.x++)
   {
      imd[pt] = imdr2[pt];
   }

   return SUCCESS;
}


/**
 * Integer 2d gray image.
 * Images MUST have their dimensions all powers of 2.
 */
Errc PFFTCorrelation( const Img2dsf &ims1, const  Img2dsf &ims2, Img2dsf &imd ) {
   Point2d pt;
   
   if (ims2.Height() > ims1.Height() || ims2.Width() > ims1.Width()) {
      return FAILURE;
   }
   int nrow = ims1.Height();
   int ncol = ims1.Width();
   Img2dsf imi(nrow, ncol);
   Img2dsf imsr2(nrow, ncol);

   // destination
   nrow = pfft::nextpow2(nrow);
   ncol = pfft::nextpow2(ncol);
   Img2dsf imdr1(nrow, ncol);
   Img2dsf imdi1(nrow, ncol);
   Img2dsf imdr2(nrow, ncol);
   Img2dsf imdi2(nrow, ncol);

   // Build the mask image data.
   // Mask is centered in the result image.
   imsr2 = 0;
   for (pt.y = 0; pt.y < ims2.Height(); pt.y++)
   for (pt.x = 0; pt.x < ims2.Width(); pt.x++)
   {
      imsr2[pt] = ims2[pt];
   }

   imi = 0;
   pfft::PFFT(ims1, imi, imdr1, imdi1);
   pfft::PFFT(imsr2, imi, imdr2, imdi2);

   for (pt.y = 0; pt.y < imdr1.Height(); pt.y++)
   for (pt.x = 0; pt.x < imdr1.Width(); pt.x++)
   {
      Float a, b, c, d;
      
      a = imdr1[pt];
      c = imdr2[pt];
      b = imdi1[pt];
      d = imdi2[pt];
      imdr1[pt] =  a * c + b * d;
      imdi1[pt] = -a * d + b * c;
   }

   pifft::PIFFT(imdr1, imdi1, imdr2, imdi2);

   for (pt.y = 0; pt.y < imd.Height(); pt.y++)
   for (pt.x = 0; pt.x < imd.Width(); pt.x++)
   {
      imd[pt] = imdr2[pt];
   }

   return SUCCESS;
}


#ifdef MAIN
#define USAGE "usage: %s [im_in1|-] [im_in2|-] [im_out|-]"
#define PARC  0 // Number of parameters.
#define FINC  2 // Number of input images.
#define FOUTC 1 // Number of output images.
#define MASK  0 // No masking allowed.

int main( int argc, char *argv[] ) {
   Errc result;                // The result code of the execution.
   Pobject* mask;              // The region map.
   Pobject* objin[FINC + 1];   // The input objects.
   Pobject* objs[FINC + 1];    // The source objects masked.
   Pobject* objout[FOUTC + 1]; // The output object.
   Pobject* objd[FOUTC + 1];   // The result object of the execution.
   char* parv[PARC + 1];       // The input parameters.

   ReadArgs(argc, argv, PARC, FINC, FOUTC, &mask, objin, objs, objout, objd, parv, USAGE, MASK); 
   if (objs[0]->Type() == Po_Img2duc && objs[1]->Type() == Po_Img2duc) {
      Img2duc* const ims1 = (Img2duc*)objs[0];
      Img2duc* const ims2 = (Img2duc*)objs[1];
      objd[0] = new Img2dsf(ims1->Size());
      Img2dsf* const imd = (Img2dsf*)objd[0];

      result = PFFTCorrelation(*ims1, *ims2, *imd);

      goto end;
   }
   if (objs[0]->Type() == Po_Img2dsl && objs[1]->Type() == Po_Img2dsl) {
      Img2dsl* const ims1 = (Img2dsl*)objs[0];
      Img2dsl* const ims2 = (Img2dsl*)objs[1];
      objd[0] = new Img2dsf(ims1->Size());
      Img2dsf* const imd = (Img2dsf*)objd[0];

      result = PFFTCorrelation(*ims1, *ims2, *imd);

      goto end;
   }
   if (objs[0]->Type() == Po_Img2dsf && objs[1]->Type() == Po_Img2dsf) {
      Img2dsf* const ims1 = (Img2dsf*)objs[0];
      Img2dsf* const ims2 = (Img2dsf*)objs[1];
      objd[0] = new Img2dsf(ims1->Size());
      Img2dsf* const imd = (Img2dsf*)objd[0];

      result = PFFTCorrelation(*ims1, *ims2, *imd);

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
