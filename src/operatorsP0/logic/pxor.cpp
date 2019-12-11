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

#include <pandore.h>
using namespace pandore;

/**
 * @file pxor.cpp
 * Images and graphs: Binary exclusive or operator. Operation: (pix1 ^ pix2)
 * Regions = UNION.
 */

Errc PXor( const Imx3d<Float> &ims1, const Imx3d<Float> &ims2, Imx3d<Float> &imd ) {
   for (int b = 0; b < ims1.Bands(); b++) {
      Float *ps1 = ims1.Vector(b);
      Float *ps2 = ims2.Vector(b);
      Float *pd = imd.Vector(b);
      for (; ps1 < ims1.Vector(b) + ims1.VectorSize(); ps1++, ps2++, pd++) {
	 if (*ps1 == *ps2) {
	    *pd = 0;
	 } else {
	    *pd = *ps1 + *ps2;
	 }
      }
   }
   return SUCCESS;
}

template<typename T>
Errc PXor( const Imx3d<T>& ims1, const Imx3d<T>& ims2, Imx3d<T>& imd ) {
   for (int b = 0; b < ims1.Bands(); b++) {
      T *ps1 = ims1.Vector(b);
      T *ps2 = ims2.Vector(b);
      T *pd = imd.Vector(b);
      for (; ps1 < ims1.Vector(b) + ims1.VectorSize(); ps1++, ps2++, pd++) {
	 *pd = (T)(*ps1 ^ *ps2);
      }
   }
   return SUCCESS;
}




#ifdef MAIN
#undef MAIN
#define EXMAIN
#endif

namespace labeling{
#include "segmentation/plabeling.cpp"
}

#ifdef EXMAIN
#define MAIN
#endif


Errc PXor( const Graph2d &grs1, const Graph2d &grs2, Graph2d &grd ) {
   grd = grs1;
   for (int i = 0; i < grd.Size(); i++) {
      if (grs1[i] && grs2[i]) {
	 if (grs1[i]->value == grs2[i]->value) {
	    grd[i]->value = 0;
	 } else {
	    grd[i]->value = grs1[i]->value + grs2[i]->value;
	 }
      }
   }
   return SUCCESS;
}
Errc PXor( const Graph3d &grs1, const Graph3d &grs2, Graph3d &grd ) {
   grd = grs1;
   for (int i = 0; i < grd.Size(); i++) {
      if (grs1[i] && grs2[i]) {
	 if (grs1[i]->value == grs2[i]->value) {
	    grd[i]->value = 0;
	 } else {
	    grd[i]->value = grs1[i]->value + grs2[i]->value;
	 }
      }
   }
   return SUCCESS;
}
/**
 * Symmetric difference between 2 regions.
 * Algorithm : 1) rgs= Union(rgs1, rgs2) - intersection(rgs1, rgs2)
 *             2) Minimization of the labels.
 */
Errc PXor( const Reg2d &rgs1, const Reg2d &rgs2, Reg2d &rgd ) {
   Reg2d rgi(rgs1.Props());
   Reg2d::ValueType* couleur = (Reg2d::ValueType*)calloc(2 * rgs1.Labels() + rgs2.Labels() + 1, sizeof(Reg2d::ValueType));
   Reg2d::ValueType* prgs1 = rgs1.Vector();
   Reg2d::ValueType* prgs2 = rgs2.Vector();
   Reg2d::ValueType* prgi = rgi.Vector();

   Reg2d::ValueType numberOfRegions = 0;
   Reg2d::ValueType val;

   for (; prgs1 < rgs1.Vector() + rgs1.VectorSize(); prgs1++, prgs2++) {
      val = 0;
      if (*prgs1 == 0) {
	 val = *prgs2 + rgs1.Labels();
      }
      if (*prgs2 == 0) {
	 val = *prgs1;
      }
      if (val > 0 && couleur[val] == 0) {
	 couleur[val] = ++numberOfRegions;
      }
      *(prgi++) = couleur[val];
   }
   free(couleur);
   
   rgi.Labels(numberOfRegions);
   labeling::PLabeling(rgi, rgd, 8);
   return rgd.Labels();
}

/**
 * Symmetric difference between 2 regions.
 * Algorithm : 1) rgs= Union(rgs1, rgs2) - intersection(rgs1, rgs2)
 *             2) Minimization of the labels.
 */
Errc PXor( const Reg3d &rgs1, const Reg3d &rgs2, Reg3d &rgd ) {
   Reg3d rgi(rgs1.Props());
   Reg3d::ValueType* couleur = (Reg3d::ValueType*)calloc(2 * rgs1.Labels() + rgs2.Labels() + 1, sizeof(Reg3d::ValueType));
   Reg3d::ValueType* prgs1 = rgs1.Vector();
   Reg3d::ValueType* prgs2 = rgs2.Vector();
   Reg3d::ValueType* prgi = rgi.Vector();

   Reg3d::ValueType numberOfRegions = 0;
   Reg3d::ValueType val;

   for (; prgs1 < rgs1.Vector() + rgs1.VectorSize(); prgs1++, prgs2++) {
      val = 0;
      if (*prgs1 == 0) {
	 val = *prgs2 + rgs1.Labels();
      }
      if (*prgs2 == 0) {
	 val = *prgs1;
      }
      if (val > 0 && couleur[val] == 0) {
	 couleur[val] = ++numberOfRegions;
      }
      *(prgi++) = couleur[val];
   }
   free(couleur);
   
   rgi.Labels(numberOfRegions);
   labeling::PLabeling(rgi, rgd, 26);
   return rgd.Labels();
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
  if (objs[0]->Type() == Po_Img2duc && objs[1]->Type() == Po_Img2duc) {
    Img2duc* const ims1 = (Img2duc*)objs[0];
    Img2duc* const ims2 = (Img2duc*)objs[1];
    objd[0] = new Img2duc(ims1->Props());

    Img2duc* const imd = (Img2duc*)objd[0];

    result = PXor(*ims1, *ims2, *imd);
    goto end;
}
  if (objs[0]->Type() == Po_Img2dsl && objs[1]->Type() == Po_Img2dsl) {
    Img2dsl* const ims1 = (Img2dsl*)objs[0];
    Img2dsl* const ims2 = (Img2dsl*)objs[1];
    objd[0] = new Img2dsl(ims1->Props());

    Img2dsl* const imd = (Img2dsl*)objd[0];

    result = PXor(*ims1, *ims2, *imd);
    goto end;
}
  if (objs[0]->Type() == Po_Img2dsf && objs[1]->Type() == Po_Img2dsf) {
    Img2dsf* const ims1 = (Img2dsf*)objs[0];
    Img2dsf* const ims2 = (Img2dsf*)objs[1];
    objd[0] = new Img2dsf(ims1->Props());

    Img2dsf* const imd = (Img2dsf*)objd[0];

    result = PXor(*ims1, *ims2, *imd);
    goto end;
}
  if (objs[0]->Type() == Po_Img3duc && objs[1]->Type() == Po_Img3duc) {
    Img3duc* const ims1 = (Img3duc*)objs[0];
    Img3duc* const ims2 = (Img3duc*)objs[1];
    objd[0] = new Img3duc(ims1->Props());

    Img3duc* const imd = (Img3duc*)objd[0];

    result = PXor(*ims1, *ims2, *imd);
    goto end;
}
  if (objs[0]->Type() == Po_Img3dsl && objs[1]->Type() == Po_Img3dsl) {
    Img3dsl* const ims1 = (Img3dsl*)objs[0];
    Img3dsl* const ims2 = (Img3dsl*)objs[1];
    objd[0] = new Img3dsl(ims1->Props());

    Img3dsl* const imd = (Img3dsl*)objd[0];

    result = PXor(*ims1, *ims2, *imd);
    goto end;
}
  if (objs[0]->Type() == Po_Img3dsf && objs[1]->Type() == Po_Img3dsf) {
    Img3dsf* const ims1 = (Img3dsf*)objs[0];
    Img3dsf* const ims2 = (Img3dsf*)objs[1];
    objd[0] = new Img3dsf(ims1->Props());

    Img3dsf* const imd = (Img3dsf*)objd[0];

    result = PXor(*ims1, *ims2, *imd);
    goto end;
}
   if (objs[0]->Type() == Po_Graph2d && objs[1]->Type() == Po_Graph2d) {
      Graph2d* const gs1 = (Graph2d*)objs[0];
      Graph2d* const gs2 = (Graph2d*)objs[1];
      objd[0] = new Graph2d(gs1->Size());
      Graph2d* const gd = (Graph2d*)objd[0];
      
      result = PXor(*gs1, *gs2, *gd);
      goto end;
   }
   if (objs[0]->Type() == Po_Graph3d && objs[1]->Type() == Po_Graph3d) {
      Graph3d* const gs1 = (Graph3d*)objs[0];
      Graph3d* const gs2 = (Graph3d*)objs[1];
      objd[0] = new Graph3d(gs1->Size());
      Graph3d* const gd = (Graph3d*)objd[0];
      
      result = PXor(*gs1, *gs2, *gd);
      goto end;
   }
   if ((objs[0]->Type() == Po_Reg2d) && (objs[1]->Type() == Po_Reg2d)) {
      Reg2d* const ims1 = (Reg2d*)objs[0];
      Reg2d* const ims2 = (Reg2d*)objs[1];
      objd[0] = new Reg2d(ims1->Size());
      
      Reg2d* const imd = (Reg2d*)objd[0];
      
      result = PXor(*ims1, *ims2, *imd);
      goto end;
   }
   if ((objs[0]->Type() == Po_Reg3d) && (objs[1]->Type() == Po_Reg3d)) {
      Reg3d* const ims1 = (Reg3d*)objs[0];
      Reg3d* const ims2 = (Reg3d*)objs[1];
      objd[0] = new Reg3d(ims1->Size());
      
      Reg3d* const imd = (Reg3d*)objd[0];
      
      result = PXor(*ims1, *ims2, *imd);
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
