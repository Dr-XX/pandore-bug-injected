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

#define _USE_MATH_DEFINES
#include <cmath>
#include <pandore.h>
using namespace pandore;

/**
 * @file phsi2rgb.cpp
 * Passe du HSI au RGB .
 */


Errc PHSI2RGB( const Imc2dsf &ims, Imc2duc &imd ) {
   Point2d pt;
  
   imd.ColorSpace(RGB);

   for (pt.y = 0; pt.y < ims.Height(); pt.y++)
   for (pt.x = 0; pt.x < ims.Width(); pt.x++)
   {
      Float h = (Float)(ims(0,pt)*M_PI/180.0f);
      Float s = ims(1,pt)/100.0f;
      Float i = ims(2,pt)/255.0f;
      Float r, g, b;

      if (s == 0.0f) {
	 r = g = b = i;
      } else {
	 if ( (h>=0) && (h< Float(2*M_PI/3)) ) {
	    b=(Float)((1-s)/3.0f);
	    r=(Float)((1+s*cos(h)/cos(M_PI/3-h))/3.0f);
	    g=(Float)(1.0f-r-b);
	 } else if ( (h>= Float(2*M_PI/3)) && (h<Float(4*M_PI/3)) ) {
	    h=Float(h-2*(M_PI/3.0f));
	    r=(Float)((1-s)/3.0f);
	    g=(Float)((1+s*cos(h)/cos(M_PI/3-h))/3.0f);
	    b=(Float)(1.0f-r-g);
	 } else if ( (h>=Float(4*M_PI/3)) && (h<Float(2*M_PI)) ) {
	    h=(Float)(h-4*M_PI/3.0f);
	    g=(Float)((1-s)/3.0f);
	    b=(Float)((1+s*cos(h)/cos(M_PI/3-h))/3.0f);
	    r=(Float)(1.0f-b-g);
	 } else {
	    std::cerr<<"Error phsi2rgb: out of range"<<std::endl;
	    return FAILURE;
	 }
	 if (r<0) {
	    r=0;
	 }
	 if (g<0) {
	    g=0;
	 }
	 if (b<0) {
	    b=0;
	 }
	 r=3*i*r;
	 g=3*i*g;
	 b=3*i*b;
	 if (r>1) {
	    r=1;
	 }
	 if (g>1) {
	    g=1;
	 }
	 if (b>1) {
	    b=1;
	 }
      }
      imd(0, pt) = (Uchar)(r * MAXUCHAR);
      imd(1, pt) = (Uchar)(g * MAXUCHAR);
      imd(2, pt) = (Uchar)(b * MAXUCHAR);
   }
   return SUCCESS;
}

Errc PHSI2RGB( const Imc3dsf &ims, Imc3duc &imd ) {
   Point3d pt;
  
   imd.ColorSpace(RGB);

   for (pt.z = 0; pt.z < ims.Depth(); pt.z++)
   for (pt.y = 0; pt.y < ims.Height(); pt.y++)
   for (pt.x = 0; pt.x < ims.Width(); pt.x++)
   {
      Float h = (Float)(ims(0,pt)*M_PI/180.0f);
      Float s = ims(1,pt)/100.0f;
      Float i = ims(2,pt)/255.0f;
      Float r, g, b;

      if (s == 0.0f) {
	 r = g = b = i;
      } else {
	 if ( (h>=0) && (h< Float(2*M_PI/3)) ) {
	    b=(Float)((1-s)/3.0f);
	    r=(Float)((1+s*cos(h)/cos(M_PI/3-h))/3.0f);
	    g=(Float)(1.0f-r-b);
	 } else if ( (h>= Float(2*M_PI/3)) && (h<Float(4*M_PI/3)) ) {
	    h=Float(h-2*(M_PI/3.0f));
	    r=(Float)((1-s)/3.0f);
	    g=(Float)((1+s*cos(h)/cos(M_PI/3-h))/3.0f);
	    b=(Float)(1.0f-r-g);
	 } else if ( (h>=Float(4*M_PI/3)) && (h<Float(2*M_PI)) ) {
	    h=(Float)(h-4*M_PI/3.0f);
	    g=(Float)((1-s)/3.0f);
	    b=(Float)((1+s*cos(h)/cos(M_PI/3-h))/3.0f);
	    r=(Float)(1.0f-b-g);
	 } else {
	    std::cerr<<"Error phsi2rgb: out of range"<<std::endl;
	    return FAILURE;
	 }
	 if (r<0) {
	    r=0;
	 }
	 if (g<0) {
	    g=0;
	 }
	 if (b<0) {
	    b=0;
	 }
	 r=3*i*r;
	 g=3*i*g;
	 b=3*i*b;
	 if (r>1) {
	    r=1;
	 }
	 if (g>1) {
	    g=1;
	 }
	 if (b>1) {
	    b=1;
	 }
      }
      imd(0, pt) = (Uchar)(r * MAXUCHAR);
      imd(1, pt) = (Uchar)(g * MAXUCHAR);
      imd(2, pt) = (Uchar)(b * MAXUCHAR);
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
     
     result = PHSI2RGB(*ims, *imd);
     goto end;
   }
   if (objs[0]->Type() == Po_Imc3dsf) {
     Imc3dsf* const ims = (Imc3dsf*)objs[0];
     objd[0] = new Imc3duc(ims->Size());
     Imc3duc* const imd = (Imc3duc*)objd[0];
     
     result = PHSI2RGB(*ims, *imd);
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
