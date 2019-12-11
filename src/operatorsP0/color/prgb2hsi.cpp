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
 * @author Olivier Lezoray  - 1997-12-12
 * @author Olivier Lezoray  - 2002-05-21
 * @author Régis Clouard - 2003-06-25 (add 3D)
 * @author Régis Clouard - Jun 22, 2012 (saturation in [0..100])
 */

#define _USE_MATH_DEFINES
#include <cmath>
#include <pandore.h>
using namespace pandore;

/**
 * @file prgb2hsi.cpp
 * Passe du * GB uc au HSI uc.
 *          [         ((R-V)+(R-B))            ]
 * H = acos [ -------------------------------  ]
 *          [ 2*sqrt((R-V)*(R-V) + (R-B)(V-B)) ]
 * S = 1 - 3*MIN(R,V,B)
 *         ------------
 *             L
 * L = (R + V + B)/3
 */


Errc PRGB2HSI( const Imc2duc &ims, Imc2dsf &imd ) {
   Point2d p;
   Float h, s, i, t;
   
   imd.ColorSpace(HSI);
   
   for (p.y = 0; p.y < ims.Height(); p.y++)
   for (p.x = 0; p.x < ims.Width(); p.x++)
   {
      i = (Float)(ims.X[p] + ims.Y[p] + ims.Z[p]) / 3.0F;
      if (i == 0) {
	 s = 1.0F;
      }else {
	 s = (Float)(1. - ((Float)MIN(ims.X[p], MIN(ims.Y[p], ims.Z[p]))) / i);
      }
      
      t = (Float)sqrt((double)(ims.X[p] - ims.Y[p]) * (ims.X[p] - ims.Y[p])
		     +(double)(ims.X[p] - ims.Z[p]) * (ims.Y[p] - ims.Z[p]));
      // acos(x) returns the arc cosine of x,  in the range [0,pi] radians.
      if (t == 0.F) {
	 h = (Float) acos(0.F);
      } else {
	 h = (Float) acos(0.5 * (2.0 * ims.X[p] - ims.Y[p] - ims.Z[p]) / t);
	 if (ims.Y[p] < ims.Z[p]) {
	    h = (Float)(2. * M_PI - h);
	 }
      }
      imd.X[p] = (float)(h * 360 / (2.0 * M_PI)); // In degrees
      imd.Y[p] = (float)s * 100.0f; // % [0..100]
      imd.Z[p] = (float)i; // intensity level [0..255]
   }
   return SUCCESS;
}

Errc PRGB2HSI( const Imc3duc &ims, Imc3dsf &imd ) {
   Point3d p;
   Float h, s, i, t;
   
   imd.ColorSpace(HSI);
   
   for (p.z = 0; p.z < ims.Depth(); p.z++)
   for (p.y = 0; p.y < ims.Height(); p.y++)
   for (p.x = 0; p.x < ims.Width(); p.x++)
   {
      i = (Float)(ims.X[p] + ims.Y[p] + ims.Z[p]) / 3.0F;
      if (i == 0) {
	 s = 1.0F;
      }else {
	 s = (Float)(1. - ((Float)MIN(ims.X[p], MIN(ims.Y[p], ims.Z[p]))) / i);
      }
      
      t = (Float)sqrt((double)(ims.X[p] - ims.Y[p]) * (ims.X[p] - ims.Y[p])
		     +(double)(ims.X[p] - ims.Z[p]) * (ims.Y[p] - ims.Z[p]));
      // acos(x) returns the arc cosine of x,  in the range [0,pi] radians.
      if (t == 0.F) {
	 h = (Float) acos(0.F);
      } else {
	 h = (Float) acos(0.5 * (2.0 * ims.X[p] - ims.Y[p] - ims.Z[p]) / t);
	 if (ims.Y[p] < ims.Z[p]) {
	    h = (Float)(2. * M_PI - h);
	 }
      }
      imd.X[p] = (float)(h * 360 / (2.0 * M_PI)); // In degrees
      imd.Y[p] = (float)s * 100.0f; // % [0..100]
      imd.Z[p] = (float)i; // intensity level [0..255]
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
     
     result = PRGB2HSI(*ims, *imd);
     goto end;
   }
   if (objs[0]->Type() == Po_Imc3duc) {
     Imc3duc* const ims = (Imc3duc*)objs[0];
     objd[0] = new Imc3dsf(ims->Size());
     Imc3dsf* const imd = (Imc3dsf*)objd[0];
     
     result = PRGB2HSI(*ims, *imd);
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
