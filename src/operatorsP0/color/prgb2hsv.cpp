/* -*- c-basic-offset: 3; mode:c++ -*-
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
 * @author R�gis Clouard - 28 Dec 2014
 */

#include <math.h>
#include <pandore.h>
using namespace pandore;

/**
 * @file prgb2hsv.cpp
 
  Soit max = MAX(R,G,B) et min = MIN(R,G,B)

   |	   (G-B) 
H= |- (60 * ----- + 360) mod 360 si max = R
   |	  (max-min)
   |	   (B-R) 
   |- (60 * ----- + 120) + 210	si max = V
   |	  (max-min)
   |	   (R-G)
   |- (60 * ----- + 240)		si max = B
   | (max-min)

V  = max(R, G, B)

    |- 0                 si max = 0
S = |
    |         max-min
    |- 100 * ----------- si max > 0
    |        max 
 */
template<typename T>
Errc PRGB2HSV( const Imx3d<T> &ims, Imx3dsf &imd ) {
   const T *r, *g, *b;
   if (ims.Bands() == 3 ) {
      r = ims.Vector(0);
      g = ims.Vector(1);
      b = ims.Vector(2);
   } else if (ims.Bands() == 1) {
      r = ims.Vector(0);
      g = ims.Vector(0);
      b = ims.Vector(0);
   } else {
      std::cerr << "Error prgb2hsv: Bad image type, RGB color image expected!" << std::endl;
      return FAILURE;
   }
   
   Float *h = imd.Vector(0);
   Float *s = imd.Vector(1);
   Float *v = imd.Vector(2);
   for (Ulong i = 0; i < ims.VectorSize(); i++, r++, g++, b++, h++, s++, v++) {
      Float min = (Float)MIN(*r, MIN(*g, *b));
      Float max = (Float)MAX(*r, MAX(*g, *b));
      Float gap = max - min;
      
      if (*r == max) {
	 *h = (*g - *b) / gap;		// between yellow & magenta
      } else if (*g == max) {
	 *h = 2 + (*b - *r) / gap;	// between cyan & yellow
      } else {
	 *h = 4 + (*r - *g) / gap;	// between magenta & cyan
      }
      *h *= 60;				// convert in degrees
      if (*h < 0) {
	 *h += 360;
      }
      
      *v  = max;
      
      if (max != 0) {
	 *s = 100.0f * gap / max;
      } else {
	 // r = g = b = 0		// s = 0, v is undefined
	 *s = 0;
	 *h = 0;
      }
   }
   return SUCCESS;
}



#ifdef MAIN
#define	USAGE	"usage: %s [-m mask] [im_in|-] [im_out|-]"
#define	PARC	0
#define	FINC	1
#define	FOUTC	1
#define	MASK	0
int main( int argc, char *argv[] ) {
   Errc result;                // The result code of the execution.
   Pobject* mask;              // The region map.
   Pobject* objin[FINC + 1];   // The input objects.
   Pobject* objs[FINC + 1];    // The source objects masked.
   Pobject* objout[FOUTC + 1]; // The output object.
   Pobject* objd[FOUTC + 1];   // The result object of the execution.
   char* parv[PARC + 1];       // The input parameters.

   ReadArgs(argc, argv, PARC, FINC, FOUTC, &mask, objin, objs, objout, objd, parv, USAGE, MASK); 
   if (objs[0]->Type() == Po_Imc2duc) {
     Imc2duc* const ims = (Imc2duc*)objs[0];
     objd[0] = new Imc2dsf(ims->Size());
     Imc2dsf* const imd = (Imc2dsf*)objd[0];
     
     result = PRGB2HSV(*ims, *imd);
     goto end;
   }
   if (objs[0]->Type() == Po_Imc3duc) {
     Imc3duc* const ims = (Imc3duc*)objs[0];
     objd[0] = new Imc3dsf(ims->Size());
     Imc3dsf* const imd = (Imc3dsf*)objd[0];
     
     result = PRGB2HSV(*ims, *imd);
     goto end;
   }
   if (objs[0]->Type() == Po_Img2duc) {
     Img2duc* const ims = (Img2duc*)objs[0];
     objd[0] = new Imc2dsf(ims->Size());
     Imc2dsf* const imd = (Imc2dsf*)objd[0];
     
     result = PRGB2HSV(*ims, *imd);
     goto end;
   }
   if (objs[0]->Type() == Po_Img3duc) {
     Img3duc* const ims = (Img3duc*)objs[0];
     objd[0] = new Imc3dsf(ims->Size());
     Imc3dsf* const imd = (Imc3dsf*)objd[0];
     
     result = PRGB2HSV(*ims, *imd);
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
