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

#include <math.h>
#include <pandore.h>
using namespace pandore;

/**
 * @file pround.cpp
 * Computes the round-to-nearest of real values.
 * mode: the round-to-nearest mode.
 *       0: nearest integer (1.1=1; 1.9=2; -8.8=-9; -8.1=-8).
 *       1: largest integral value not greater than x (1.1=1, 1.9=1; -8.8=-9; -8.1=-9);
 *       2: smallest integral value not less than x (1.1=2; 1.9=2; -8.8=-8; -8.1=-8);
 */

#define PRECISION 0.49999999

template <typename T>
Errc PRound( const Imx3d<T> &ims, Imx3d<T> &imd, int mode ) {
   if (mode == 0) {
    
      for (int b = 0; b < ims.Bands(); b++) {
	 T *ps = ims.Vector(b);
	 T *pd = imd.Vector(b);
	 for (; ps < ims.Vector(b) + ims.VectorSize(); ps++, pd++) {
	    if (*ps >= 0) {
	       *pd = (T)floor((double)*ps + PRECISION);
	    } else {
	       *pd = (T)ceil((double)*ps - PRECISION);
	    }
	 }
      }
   } else if (mode == 1) {
      for (int b = 0; b < ims.Bands(); b++) {
	 T *ps = ims.Vector(b);
	 T *pd  =imd.Vector(b);
	 
	 for (; ps < ims.Vector(b) + ims.VectorSize(); ps++, pd++) {
	    *pd = (T)floor((double)*ps);
	 }
      }
   } else {
      for (int b = 0; b < ims.Bands(); b++) {
	 T *ps = ims.Vector(b);
	 T *pd = imd.Vector(b);
	 for (; ps < ims.Vector(b) + ims.VectorSize(); ps++, pd++) {
	    *pd = (T)ceil((double)*ps);
	 }
      }
   }
   return SUCCESS;
}



/**
 * Graph (nodes).
 */
Errc PRound( const Graph2d &grs, Graph2d &grd, int mode ) {
   grd = grs;

   if (mode == 0) {
      for (int i = 0; i < grs.Size(); i++) {
	 if ((grd[i])) {
	    if (grd[i]->value >= 0) {
	       grd[i]->value = (Graph2d::ValueType)floor((double)grd[i]->value+PRECISION);
	    } else {
	       grd[i]->value = (Graph2d::ValueType)ceil((double)grd[i]->value-PRECISION);
	    }
	 }
      }
   } else if (mode == 1) {
      for (int i = 0; i < grs.Size(); i++) {
	 if ((grd[i])) {
	    grd[i]->value = (Graph2d::ValueType)floor((double)grd[i]->value);
	 }
      }
   } else {
      for (int i = 0; i < grs.Size(); i++) {
	 if ((grd[i])) {
	    grd[i]->value = (Graph2d::ValueType)ceil((double)grd[i]->value);
	 }
      }
   }
   return  SUCCESS;
}

/**
 * Graph (nodes).
 */
Errc PRound( const Graph3d &grs, Graph3d &grd, int mode ) {
   grd = grs;

   if (mode == 0) {
      for (int i = 0; i < grs.Size(); i++) {
	 if ((grd[i])) {
	    if (grd[i]->value >= 0) {
	       grd[i]->value = (Graph3d::ValueType)floor((double)grd[i]->value+PRECISION);
	    } else {
	       grd[i]->value = (Graph3d::ValueType)ceil((double)grd[i]->value-PRECISION);
	    }
	 }
      }
   } else if (mode == 1) {
      for (int i = 0; i < grs.Size(); i++) {
	 if ((grd[i])) {
	    grd[i]->value = (Graph3d::ValueType)floor((double)grd[i]->value);
	 }
      }
   } else {
      for (int i = 0; i < grs.Size(); i++) {
	 if ((grd[i])) {
	    grd[i]->value = (Graph3d::ValueType)ceil((double)grd[i]->value);
	 }
      }
   }
   return  SUCCESS;
}


#ifdef MAIN
#define	USAGE	"usage: %s mode [-m mask] [im_in|-] [im_out|-]"
#define	PARC	1
#define	FINC	1
#define	FOUTC	1
#define	MASK	1

int main( int argc, char *argv[] ) {
   Errc result;                // The result code of the execution.
   Pobject* mask;              // The region map.
   Pobject* objin[FINC + 1];   // The input objects.
   Pobject* objs[FINC + 1];    // The source objects masked.
   Pobject* objout[FOUTC + 1]; // The output object.
   Pobject* objd[FOUTC + 1];   // The result object of the execution.
   char* parv[PARC + 1];       // The input parameters.

   ReadArgs(argc, argv, PARC, FINC, FOUTC, &mask, objin, objs, objout, objd, parv, USAGE, MASK); 
if (objs[0]->Type() == Po_Img1dsf) {
   Img1dsf* const ims = (Img1dsf*)objs[0];
   objd[0] = new Img1dsf(ims->Props());
   Img1dsf* imd = (Img1dsf*)objd[0];
      
   result = PRound(*ims, *imd, atoi(parv[0]));
   goto end;
} 
if (objs[0]->Type() == Po_Img2dsf) {
   Img2dsf* const ims = (Img2dsf*)objs[0];
   objd[0] = new Img2dsf(ims->Props());
   Img2dsf* imd = (Img2dsf*)objd[0];
      
   result = PRound(*ims, *imd, atoi(parv[0]));
   goto end;
} 
if (objs[0]->Type() == Po_Img3dsf) {
   Img3dsf* const ims = (Img3dsf*)objs[0];
   objd[0] = new Img3dsf(ims->Props());
   Img3dsf* imd = (Img3dsf*)objd[0];
      
   result = PRound(*ims, *imd, atoi(parv[0]));
   goto end;
} 
if (objs[0]->Type() == Po_Imc2dsf) {
   Imc2dsf* const ims = (Imc2dsf*)objs[0];
   objd[0] = new Imc2dsf(ims->Props());
   Imc2dsf* imd = (Imc2dsf*)objd[0];
      
   result = PRound(*ims, *imd, atoi(parv[0]));
   goto end;
} 
if (objs[0]->Type() == Po_Imc3dsf) {
   Imc3dsf* const ims = (Imc3dsf*)objs[0];
   objd[0] = new Imc3dsf(ims->Props());
   Imc3dsf* imd = (Imc3dsf*)objd[0];
      
   result = PRound(*ims, *imd, atoi(parv[0]));
   goto end;
} 
if (objs[0]->Type() == Po_Imx1dsf) {
   Imx1dsf* const ims = (Imx1dsf*)objs[0];
   objd[0] = new Imx1dsf(ims->Props());
   Imx1dsf* imd = (Imx1dsf*)objd[0];
      
   result = PRound(*ims, *imd, atoi(parv[0]));
   goto end;
} 
if (objs[0]->Type() == Po_Imx2dsf) {
   Imx2dsf* const ims = (Imx2dsf*)objs[0];
   objd[0] = new Imx2dsf(ims->Props());
   Imx2dsf* imd = (Imx2dsf*)objd[0];
      
   result = PRound(*ims, *imd, atoi(parv[0]));
   goto end;
} 
if (objs[0]->Type() == Po_Imx3dsf) {
   Imx3dsf* const ims = (Imx3dsf*)objs[0];
   objd[0] = new Imx3dsf(ims->Props());
   Imx3dsf* imd = (Imx3dsf*)objd[0];
      
   result = PRound(*ims, *imd, atoi(parv[0]));
   goto end;
} 
if (objs[0]->Type() == Po_Graph2d) {
   Graph2d* const grs = (Graph2d*)objs[0];
   objd[0] = new Graph2d(grs->Size());
   Graph2d* const grd = (Graph2d*)objd[0];
      
   result = PRound(*grs, *grd, atoi(parv[0]));
   goto end;
}
if (objs[0]->Type() == Po_Graph3d) {
   Graph3d* const grs = (Graph3d*)objs[0];
   objd[0] = new Graph3d(grs->Size());
   Graph3d* const grd = (Graph3d*)objd[0];
      
   result = PRound(*grs, *grd, atoi(parv[0]));
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
