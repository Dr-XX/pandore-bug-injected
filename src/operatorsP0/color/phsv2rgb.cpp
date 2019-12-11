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

/*
 * @author Olivier Lezoray  - 2005-11-15
 * @author Régis Clouard  - Jan 17, 2011
 */

#include <cmath>
#include <pandore.h>
using namespace pandore;

/**
 * @file phsl2rgb.cpp
 * Convert HSL image to RGB image.
 */


Errc PHSV2RGB( const Imc2dsf &ims, Imc2duc &imd ) {
   Float *h = ims.Vector(0);
   Float *s = ims.Vector(1);
   Float *v = ims.Vector(2);
   Imc2duc::ValueType *r = imd.Vector(0);
   Imc2duc::ValueType *g = imd.Vector(1);
   Imc2duc::ValueType *b = imd.Vector(2);

   for (Ulong i = 0; i < ims.VectorSize(); i++, r++, g++, b++, h++, s++, v++) {
      if (*s == 0) {
	 // achromatic (grey)
	 *r = *g = *b = (Imc2duc::ValueType)*v;
      } else {
	 *h /= 60;			// sector 0 to 5
	 *s /= 100.0f;
	 int w = (int)floor(*h);
	 float f = *h - w;			// factorial part of h
	 float p = *v * (1.0f - *s);
	 float q = *v * (1.0f - *s * f);
	 float t = *v * (1.0f - *s * (1.0f - f));
	 
	 switch (w) {
	     case 0:
		*r = (Imc2duc::ValueType)*v;
		*g = (Imc2duc::ValueType)t;
		*b = (Imc2duc::ValueType)p;
		break;
	     case 1:
		*r = (Imc2duc::ValueType)q;
		*g = (Imc2duc::ValueType)*v;
		*b = (Imc2duc::ValueType)p;
		break;
	     case 2:
		*r = (Imc2duc::ValueType)p;
		*g = (Imc2duc::ValueType)*v;
		*b = (Imc2duc::ValueType)t;
		break;
	     case 3:
		*r = (Imc2duc::ValueType)p;
		*g = (Imc2duc::ValueType)q;
		*b = (Imc2duc::ValueType)*v;
		break;
	     case 4:
		*r = (Imc2duc::ValueType)t;
		*g = (Imc2duc::ValueType)p;
		*b = (Imc2duc::ValueType)*v;
		break;
	     default:		// case 5:
		*r = (Imc2duc::ValueType)*v;
		*g = (Imc2duc::ValueType)p;
		*b = (Imc2duc::ValueType)q;
		break;
	 }
      }
   }
   return SUCCESS;
}

Errc PHSV2RGB( const Imc3dsf &ims, Imc3duc &imd ) {
   Float *h = ims.Vector(0);
   Float *s = ims.Vector(1);
   Float *v = ims.Vector(2);
   Imc3duc::ValueType *r = imd.Vector(0);
   Imc3duc::ValueType *g = imd.Vector(1);
   Imc3duc::ValueType *b = imd.Vector(2);

   for (Ulong i = 0; i < ims.VectorSize(); i++, r++, g++, b++, h++, s++, v++) {
      if (*s == 0) {
	 // achromatic (grey)
	 *r = *g = *b = (Imc3duc::ValueType)*v;
      } else {
	 *h /= 60;			// sector 0 to 5
	 *s /= 100.0f;
	 int w = (int)floor(*h);
	 float f = *h - w;			// factorial part of h
	 float p = *v * (1.0f - *s);
	 float q = *v * (1.0f - *s * f);
	 float t = *v * (1.0f - *s * (1.0f - f));
	 
	 switch (w) {
	     case 0:
		*r = (Imc3duc::ValueType)*v;
		*g = (Imc3duc::ValueType)t;
		*b = (Imc3duc::ValueType)p;
		break;
	     case 1:
		*r = (Imc3duc::ValueType)q;
		*g = (Imc3duc::ValueType)*v;
		*b = (Imc3duc::ValueType)p;
		break;
	     case 2:
		*r = (Imc3duc::ValueType)p;
		*g = (Imc3duc::ValueType)*v;
		*b = (Imc3duc::ValueType)t;
		break;
	     case 3:
		*r = (Imc3duc::ValueType)p;
		*g = (Imc3duc::ValueType)q;
		*b = (Imc3duc::ValueType)*v;
		break;
	     case 4:
		*r = (Imc3duc::ValueType)t;
		*g = (Imc3duc::ValueType)p;
		*b = (Imc3duc::ValueType)*v;
		break;
	     default:		// case 5:
		*r = (Imc3duc::ValueType)*v;
		*g = (Imc3duc::ValueType)p;
		*b = (Imc3duc::ValueType)q;
		break;
	 }
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
   if (objs[0]->Type() == Po_Imc2dsf) {
     Imc2dsf* const ims = (Imc2dsf*)objs[0];
     objd[0] = new Imc2duc(ims->Size());
     Imc2duc* const imd = (Imc2duc*)objd[0];
     
     result = PHSV2RGB(*ims, *imd);
     goto end;
   }
   if (objs[0]->Type() == Po_Imc3dsf) {
     Imc3dsf* const ims = (Imc3dsf*)objs[0];
     objd[0] = new Imc3duc(ims->Size());
     Imc3duc* const imd = (Imc3duc*)objd[0];
     
     result = PHSV2RGB(*ims, *imd);
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
