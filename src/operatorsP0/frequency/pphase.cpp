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


#include <math.h>
#include <pandore.h>
using namespace pandore;

/**
 * @file pphase.cpp
 * Computes the phase of a complex image: atan(I/R)
 * with R real part and I imaginary part of the complex image.
 */
template <typename T>
Errc PPhase( const Imx3d<T> &ims1, Imx3d<T> &ims2, Imx3dsf &imd ) {
   for (int b = 0; b < ims1.Bands(); b++) {
      T *ps1 = ims1.Vector(b);
      const T *end = ps1 + ims1.VectorSize();
      T *ps2 = ims2.Vector(b);
      Float *pd = imd.Vector(b);
      for (; ps1 < end; ps1++, ps2++, pd++) {
	 *pd = (Float)atan2((Double)*ps2, (Double)*ps1);
      }
   }
   return SUCCESS;
}



/**
 * Between 2 graphs (nodes).
 */
Errc PPhase( const Graph2d &gs1, Graph2d &gs2, Graph2d &gd ) {
   gd = gs1;
   for (int i = 0; i < gd.Size(); i++) {
      if (gs1[i] == 0) {
	 gd[i]->value = 0;
      } else {
	 gd[i]->value = (Graph2d::ValueType)atan2(gs2[i]->value, gs1[i]->value);
      }
   }
   
   return SUCCESS;
}
/**
 * Between 2 graphs (nodes).
 */
Errc PPhase( const Graph3d &gs1, Graph3d &gs2, Graph3d &gd ) {
   gd = gs1;
   for (int i = 0; i < gd.Size(); i++) {
      if (gs1[i] == 0) {
	 gd[i]->value = 0;
      } else {
	 gd[i]->value = (Graph3d::ValueType)atan2(gs2[i]->value, gs1[i]->value);
      }
   }
   
   return SUCCESS;
}

#ifdef MAIN
#define	USAGE	"usage: %s [-m mask] [im_in1|-] [im_in2|-] [im_out|-]"
#define	PARC	0
#define	FINC	2
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
   if ((objs[0]->Type() == Po_Img1duc) && (objs[1]->Type() == Po_Img1duc)) {
      Img1duc* const ims1 = (Img1duc*)objs[0];
      Img1duc* const ims2 = (Img1duc*)objs[1];
      objd[0] = new Img1dsf(ims1->Size());
      
      Img1dsf* const imd = (Img1dsf*)objd[0];
      
      result = PPhase(*ims1, *ims2, *imd);
      goto end;
   }
   if ((objs[0]->Type() == Po_Img1dsl) && (objs[1]->Type() == Po_Img1dsl)) {
      Img1dsl* const ims1 = (Img1dsl*)objs[0];
      Img1dsl* const ims2 = (Img1dsl*)objs[1];
      objd[0] = new Img1dsf(ims1->Size());
      
      Img1dsf* const imd = (Img1dsf*)objd[0];
      
      result = PPhase(*ims1, *ims2, *imd);
      goto end;
   }
   if ((objs[0]->Type() == Po_Img1dsf) && (objs[1]->Type() == Po_Img1dsf)) {
      Img1dsf* const ims1 = (Img1dsf*)objs[0];
      Img1dsf* const ims2 = (Img1dsf*)objs[1];
      objd[0] = new Img1dsf(ims1->Size());
      
      Img1dsf* const imd = (Img1dsf*)objd[0];
      
      result = PPhase(*ims1, *ims2, *imd);
      goto end;
   }
   if ((objs[0]->Type() == Po_Img2duc) && (objs[1]->Type() == Po_Img2duc)) {
      Img2duc* const ims1 = (Img2duc*)objs[0];
      Img2duc* const ims2 = (Img2duc*)objs[1];
      objd[0] = new Img2dsf(ims1->Size());
      
      Img2dsf* const imd = (Img2dsf*)objd[0];
      
      result = PPhase(*ims1, *ims2, *imd);
      goto end;
   }
   if ((objs[0]->Type() == Po_Img2dsl) && (objs[1]->Type() == Po_Img2dsl)) {
      Img2dsl* const ims1 = (Img2dsl*)objs[0];
      Img2dsl* const ims2 = (Img2dsl*)objs[1];
      objd[0] = new Img2dsf(ims1->Size());
      
      Img2dsf* const imd = (Img2dsf*)objd[0];
      
      result = PPhase(*ims1, *ims2, *imd);
      goto end;
   }
   if ((objs[0]->Type() == Po_Img2dsf) && (objs[1]->Type() == Po_Img2dsf)) {
      Img2dsf* const ims1 = (Img2dsf*)objs[0];
      Img2dsf* const ims2 = (Img2dsf*)objs[1];
      objd[0] = new Img2dsf(ims1->Size());
      
      Img2dsf* const imd = (Img2dsf*)objd[0];
      
      result = PPhase(*ims1, *ims2, *imd);
      goto end;
   }
   if ((objs[0]->Type() == Po_Img3duc) && (objs[1]->Type() == Po_Img3duc)) {
      Img3duc* const ims1 = (Img3duc*)objs[0];
      Img3duc* const ims2 = (Img3duc*)objs[1];
      objd[0] = new Img3dsf(ims1->Size());
      
      Img3dsf* const imd = (Img3dsf*)objd[0];
      
      result = PPhase(*ims1, *ims2, *imd);
      goto end;
   }
   if ((objs[0]->Type() == Po_Img3dsl) && (objs[1]->Type() == Po_Img3dsl)) {
      Img3dsl* const ims1 = (Img3dsl*)objs[0];
      Img3dsl* const ims2 = (Img3dsl*)objs[1];
      objd[0] = new Img3dsf(ims1->Size());
      
      Img3dsf* const imd = (Img3dsf*)objd[0];
      
      result = PPhase(*ims1, *ims2, *imd);
      goto end;
   }
   if ((objs[0]->Type() == Po_Img3dsf) && (objs[1]->Type() == Po_Img3dsf)) {
      Img3dsf* const ims1 = (Img3dsf*)objs[0];
      Img3dsf* const ims2 = (Img3dsf*)objs[1];
      objd[0] = new Img3dsf(ims1->Size());
      
      Img3dsf* const imd = (Img3dsf*)objd[0];
      
      result = PPhase(*ims1, *ims2, *imd);
      goto end;
   }
   if ((objs[0]->Type() == Po_Imc2duc) && (objs[1]->Type() == Po_Imc2duc)) {
      Imc2duc* const ims1 = (Imc2duc*)objs[0];
      Imc2duc* const ims2 = (Imc2duc*)objs[1];
      objd[0] = new Imc2dsf(ims1->Size());
      
      Imc2dsf* const imd = (Imc2dsf*)objd[0];
      
      result = PPhase(*ims1, *ims2, *imd);
      goto end;
   }
   if ((objs[0]->Type() == Po_Imc2dsl) && (objs[1]->Type() == Po_Imc2dsl)) {
      Imc2dsl* const ims1 = (Imc2dsl*)objs[0];
      Imc2dsl* const ims2 = (Imc2dsl*)objs[1];
      objd[0] = new Imc2dsf(ims1->Size());
      
      Imc2dsf* const imd = (Imc2dsf*)objd[0];
      
      result = PPhase(*ims1, *ims2, *imd);
      goto end;
   }
   if ((objs[0]->Type() == Po_Imc2dsf) && (objs[1]->Type() == Po_Imc2dsf)) {
      Imc2dsf* const ims1 = (Imc2dsf*)objs[0];
      Imc2dsf* const ims2 = (Imc2dsf*)objs[1];
      objd[0] = new Imc2dsf(ims1->Size());
      
      Imc2dsf* const imd = (Imc2dsf*)objd[0];
      
      result = PPhase(*ims1, *ims2, *imd);
      goto end;
   }
   if ((objs[0]->Type() == Po_Imc3duc) && (objs[1]->Type() == Po_Imc3duc)) {
      Imc3duc* const ims1 = (Imc3duc*)objs[0];
      Imc3duc* const ims2 = (Imc3duc*)objs[1];
      objd[0] = new Imc3dsf(ims1->Size());
      
      Imc3dsf* const imd = (Imc3dsf*)objd[0];
      
      result = PPhase(*ims1, *ims2, *imd);
      goto end;
   }
   if ((objs[0]->Type() == Po_Imc3dsl) && (objs[1]->Type() == Po_Imc3dsl)) {
      Imc3dsl* const ims1 = (Imc3dsl*)objs[0];
      Imc3dsl* const ims2 = (Imc3dsl*)objs[1];
      objd[0] = new Imc3dsf(ims1->Size());
      
      Imc3dsf* const imd = (Imc3dsf*)objd[0];
      
      result = PPhase(*ims1, *ims2, *imd);
      goto end;
   }
   if ((objs[0]->Type() == Po_Imc3dsf) && (objs[1]->Type() == Po_Imc3dsf)) {
      Imc3dsf* const ims1 = (Imc3dsf*)objs[0];
      Imc3dsf* const ims2 = (Imc3dsf*)objs[1];
      objd[0] = new Imc3dsf(ims1->Size());
      
      Imc3dsf* const imd = (Imc3dsf*)objd[0];
      
      result = PPhase(*ims1, *ims2, *imd);
      goto end;
   }
   if ((objs[0]->Type() == Po_Graph2d) && (objs[1]->Type() == Po_Graph2d)) {
      Graph2d* const gs1 = (Graph2d*)objs[0];
      Graph2d* const gs2 = (Graph2d*)objs[1];
      objd[0] = new Graph2d(gs1->Size());
      Graph2d* const gd = (Graph2d*)objd[0];
      
      result = PPhase(*gs1, *gs2, *gd);
      goto end;
   }
   if ((objs[0]->Type() == Po_Graph3d) && (objs[1]->Type() == Po_Graph3d)) {
      Graph3d* const gs1 = (Graph3d*)objs[0];
      Graph3d* const gs2 = (Graph3d*)objs[1];
      objd[0] = new Graph3d(gs1->Size());
      Graph3d* const gd = (Graph3d*)objd[0];
      
      result = PPhase(*gs1, *gs2, *gd);
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
