/* -*- mode: c++; c-basic-offset: 3 -*-
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
 * @file pbinarization.cpp
 * Binary threshold of image.
 * If low <= high -> [low..high]
 * If low > high  -> [min..low[ U ]high..max]
 */
template <typename T1, typename T2>
Errc PBinarization( const Imx3d<T1> &ims, Imx3d<T2> &imd, float low, float high ) {
   if (low < (float)Limits<T1>::min()) {
      low = (float)Limits<T1>::min();
   } else if (low > (float)Limits<T1>::max()) {
      low = (float)Limits<T1>::max();
   }
   if (high > (float)Limits<T1>::max()) {
      high  =(float)Limits<T1>::max();
   } else if (high < (float)Limits<T1>::min()) {
      high = (float)Limits<T1>::min();
   }
   
   if (high < low) {
      T1 *ps = ims.Vector(0);
      T2 *pd = imd.Vector(0);
      const T1 *pe = ps + ims.VectorSize();
      for ( ; ps < pe; ps++, pd++) {
	 if (((float)*ps > low) || ((float)*ps < high)) {
	    *pd = 255;
	 } else {
	    *pd = 0;
	 }
      }
      for ( int b=1 ; b < ims.Bands(); b++ ) {
	 T1 *ps = ims.Vector(b);
	 T2 *pd = imd.Vector(0);
	 const T1 *pe = ps + ims.VectorSize();
	 for ( ; ps < pe; ps++, pd++) {
	    if (((float)*ps > low) || ((float)*ps < high)) {
	       *pd = 255;
	    }
	 }
      }
   } else {
      T1 *ps = ims.Vector(0);
      T2 *pd = imd.Vector(0);
      const T1 *pe = ps + ims.VectorSize();
      for ( ; ps < pe; ps++, pd++) {
	 if (((float)*ps >= low) && ((float)*ps <= high)) {
	    *pd = 255;
	 } else {
	    *pd = 0;
	 }
      }
      for (int b = 1; b < ims.Bands(); b++) {
	 T1 *ps = ims.Vector(b);
	 T2 *pd = imd.Vector(0);
	 const T1 *pe = ps + ims.VectorSize();
	 for ( ; ps < pe; ps++,pd++)  {
	    if (((float)*ps >= low) && ((float)*ps <= high)) {
	       *pd = 255;
	    }
	 }
      }
   }
   return SUCCESS;
}



Errc PBinarization( const Graph2d &grs, Graph2d &grd, Graph2d::ValueType low, Graph2d::ValueType high ) {
   if (low < Limits<Graph2d::ValueType>::min()) {
      low = Limits<Graph2d::ValueType>::min();
   } else if (low > Limits<Graph2d::ValueType>::max()) {
      low = Limits<Graph2d::ValueType>::max();
   }
   if (high>Limits<Graph2d::ValueType>::max()) {
      high = Limits<Graph2d::ValueType>::max();
   } else if (high < Limits<Graph2d::ValueType>::min()) {
      high = Limits<Graph2d::ValueType>::min();
   }

   grd = grs;
   if (high < low) {
      for (int i = 1; i < grs.Size(); i++) {
	 if ((grs[i]->value > low) || (grs[i]->value > high)) {
	    grd[i]->value = 255;
	 } else {
	    grd[i]->value = 0;
	 }
      }
   } else {
      for (int i = 1; i < grs.Size(); i++) {
	 if ((grs[i]->value >= low) && (grs[i]->value <= high)) {
	    grd[i]->value = 255;
	 } else {
	    grd[i]->value = 0;
	 }
      }
   }
   return SUCCESS;
}

Errc PBinarization( const Graph3d &grs, Graph3d &grd, Graph3d::ValueType low, Graph3d::ValueType high ) {
   if (low < Limits<Graph3d::ValueType>::min()) {
      low = Limits<Graph3d::ValueType>::min();
   } else if (low > Limits<Graph3d::ValueType>::max()) {
      low = Limits<Graph3d::ValueType>::max();
   }
   if (high>Limits<Graph3d::ValueType>::max()) {
      high = Limits<Graph3d::ValueType>::max();
   } else if (high < Limits<Graph3d::ValueType>::min()) {
      high = Limits<Graph3d::ValueType>::min();
   }

   grd = grs;
   if (high < low) {
      for (int i = 1; i < grs.Size(); i++) {
	 if ((grs[i]->value > low) || (grs[i]->value > high)) {
	    grd[i]->value = 255;
	 } else {
	    grd[i]->value = 0;
	 }
      }
   } else {
      for (int i = 1; i < grs.Size(); i++) {
	 if ((grs[i]->value >= low) && (grs[i]->value <= high)) {
	    grd[i]->value = 255;
	 } else {
	    grd[i]->value = 0;
	 }
      }
   }
   return SUCCESS;
}


#ifdef MAIN
#define	USAGE	"usage: %s low high [-m mask] [im_in|-][im_out|-]"
#define	PARC	2
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
   if (objs[0]->Type() == Po_Img1duc) {
      Img1duc* const ims = (Img1duc*)objs[0];
      objd[0] = new Img1duc(ims->Props());
      
      Img1duc* const imd = (Img1duc*)objd[0];
      
      result = PBinarization(*ims,*imd,(float)atof(parv[0]), (float)atof(parv[1]));
      goto end;
   }
   if (objs[0]->Type() == Po_Img1dsl) {
      Img1dsl* const ims = (Img1dsl*)objs[0];
      objd[0] = new Img1duc(ims->Props());
      
      Img1duc* const imd = (Img1duc*)objd[0];
      
      result = PBinarization(*ims,*imd,(float)atof(parv[0]), (float)atof(parv[1]));
      goto end;
   }
   if (objs[0]->Type() == Po_Img1dsf) {
      Img1dsf* const ims = (Img1dsf*)objs[0];
      objd[0] = new Img1duc(ims->Props());
      
      Img1duc* const imd = (Img1duc*)objd[0];
      
      result = PBinarization(*ims,*imd,(float)atof(parv[0]), (float)atof(parv[1]));
      goto end;
   }
   if (objs[0]->Type() == Po_Imx1duc) {
      Imx1duc* const ims = (Imx1duc*)objs[0];
      objd[0] = new Img1duc(ims->Props());
      
      Img1duc* const imd = (Img1duc*)objd[0];
      
      result = PBinarization(*ims,*imd,(float)atof(parv[0]), (float)atof(parv[1]));
      goto end;
   }
   if (objs[0]->Type() == Po_Imx1dsl) {
      Imx1dsl* const ims = (Imx1dsl*)objs[0];
      objd[0] = new Img1duc(ims->Props());
      
      Img1duc* const imd = (Img1duc*)objd[0];
      
      result = PBinarization(*ims,*imd,(float)atof(parv[0]), (float)atof(parv[1]));
      goto end;
   }
   if (objs[0]->Type() == Po_Imx1dsf) {
      Imx1dsf* const ims = (Imx1dsf*)objs[0];
      objd[0] = new Img1duc(ims->Props());
      
      Img1duc* const imd = (Img1duc*)objd[0];
      
      result = PBinarization(*ims,*imd,(float)atof(parv[0]), (float)atof(parv[1]));
      goto end;
   }
   if (objs[0]->Type() == Po_Img2duc) {
      Img2duc* const ims = (Img2duc*)objs[0];
      objd[0] = new Img2duc(ims->Props());
      
      Img2duc* const imd = (Img2duc*)objd[0];
      
      result = PBinarization(*ims,*imd,(float)atof(parv[0]), (float)atof(parv[1]));
      goto end;
   }
   if (objs[0]->Type() == Po_Img2dsl) {
      Img2dsl* const ims = (Img2dsl*)objs[0];
      objd[0] = new Img2duc(ims->Props());
      
      Img2duc* const imd = (Img2duc*)objd[0];
      
      result = PBinarization(*ims,*imd,(float)atof(parv[0]), (float)atof(parv[1]));
      goto end;
   }
   if (objs[0]->Type() == Po_Img2dsf) {
      Img2dsf* const ims = (Img2dsf*)objs[0];
      objd[0] = new Img2duc(ims->Props());
      
      Img2duc* const imd = (Img2duc*)objd[0];
      
      result = PBinarization(*ims,*imd,(float)atof(parv[0]), (float)atof(parv[1]));
      goto end;
   }
   if (objs[0]->Type() == Po_Imc2duc) {
      Imc2duc* const ims = (Imc2duc*)objs[0];
      objd[0] = new Img2duc(ims->Props());
      
      Img2duc* const imd = (Img2duc*)objd[0];
      
      result = PBinarization(*ims,*imd,(float)atof(parv[0]), (float)atof(parv[1]));
      goto end;
   }
   if (objs[0]->Type() == Po_Imc2dsl) {
      Imc2dsl* const ims = (Imc2dsl*)objs[0];
      objd[0] = new Img2duc(ims->Props());
      
      Img2duc* const imd = (Img2duc*)objd[0];
      
      result = PBinarization(*ims,*imd,(float)atof(parv[0]), (float)atof(parv[1]));
      goto end;
   }
   if (objs[0]->Type() == Po_Imc2dsf) {
      Imc2dsf* const ims = (Imc2dsf*)objs[0];
      objd[0] = new Img2duc(ims->Props());
      
      Img2duc* const imd = (Img2duc*)objd[0];
      
      result = PBinarization(*ims,*imd,(float)atof(parv[0]), (float)atof(parv[1]));
      goto end;
   }
   if (objs[0]->Type() == Po_Imx2duc) {
      Imx2duc* const ims = (Imx2duc*)objs[0];
      objd[0] = new Img2duc(ims->Props());
      
      Img2duc* const imd = (Img2duc*)objd[0];
      
      result = PBinarization(*ims,*imd,(float)atof(parv[0]), (float)atof(parv[1]));
      goto end;
   }
   if (objs[0]->Type() == Po_Imx2dsl) {
      Imx2dsl* const ims = (Imx2dsl*)objs[0];
      objd[0] = new Img2duc(ims->Props());
      
      Img2duc* const imd = (Img2duc*)objd[0];
      
      result = PBinarization(*ims,*imd,(float)atof(parv[0]), (float)atof(parv[1]));
      goto end;
   }
   if (objs[0]->Type() == Po_Imx2dsf) {
      Imx2dsf* const ims = (Imx2dsf*)objs[0];
      objd[0] = new Img2duc(ims->Props());
      
      Img2duc* const imd = (Img2duc*)objd[0];
      
      result = PBinarization(*ims,*imd,(float)atof(parv[0]), (float)atof(parv[1]));
      goto end;
   }
   if (objs[0]->Type() == Po_Img3duc) {
      Img3duc* const ims = (Img3duc*)objs[0];
      objd[0] = new Img3duc(ims->Props());
      
      Img3duc* const imd = (Img3duc*)objd[0];
      
      result = PBinarization(*ims,*imd,(float)atof(parv[0]), (float)atof(parv[1]));
      goto end;
   }
   if (objs[0]->Type() == Po_Img3dsl) {
      Img3dsl* const ims = (Img3dsl*)objs[0];
      objd[0] = new Img3duc(ims->Props());
      
      Img3duc* const imd = (Img3duc*)objd[0];
      
      result = PBinarization(*ims,*imd,(float)atof(parv[0]), (float)atof(parv[1]));
      goto end;
   }
   if (objs[0]->Type() == Po_Img3dsf) {
      Img3dsf* const ims = (Img3dsf*)objs[0];
      objd[0] = new Img3duc(ims->Props());
      
      Img3duc* const imd = (Img3duc*)objd[0];
      
      result = PBinarization(*ims,*imd,(float)atof(parv[0]), (float)atof(parv[1]));
      goto end;
   }
   if (objs[0]->Type() == Po_Imc3duc) {
      Imc3duc* const ims = (Imc3duc*)objs[0];
      objd[0] = new Img3duc(ims->Props());
      
      Img3duc* const imd = (Img3duc*)objd[0];
      
      result = PBinarization(*ims,*imd,(float)atof(parv[0]), (float)atof(parv[1]));
      goto end;
   }
   if (objs[0]->Type() == Po_Imc3dsl) {
      Imc3dsl* const ims = (Imc3dsl*)objs[0];
      objd[0] = new Img3duc(ims->Props());
      
      Img3duc* const imd = (Img3duc*)objd[0];
      
      result = PBinarization(*ims,*imd,(float)atof(parv[0]), (float)atof(parv[1]));
      goto end;
   }
   if (objs[0]->Type() == Po_Imc3dsf) {
      Imc3dsf* const ims = (Imc3dsf*)objs[0];
      objd[0] = new Img3duc(ims->Props());
      
      Img3duc* const imd = (Img3duc*)objd[0];
      
      result = PBinarization(*ims,*imd,(float)atof(parv[0]), (float)atof(parv[1]));
      goto end;
   }
   if (objs[0]->Type() == Po_Imx3duc) {
      Imx3duc* const ims = (Imx3duc*)objs[0];
      objd[0] = new Img3duc(ims->Props());
      
      Img3duc* const imd = (Img3duc*)objd[0];
      
      result = PBinarization(*ims,*imd,(float)atof(parv[0]), (float)atof(parv[1]));
      goto end;
   }
   if (objs[0]->Type() == Po_Imx3dsl) {
      Imx3dsl* const ims = (Imx3dsl*)objs[0];
      objd[0] = new Img3duc(ims->Props());
      
      Img3duc* const imd = (Img3duc*)objd[0];
      
      result = PBinarization(*ims,*imd,(float)atof(parv[0]), (float)atof(parv[1]));
      goto end;
   }
   if (objs[0]->Type() == Po_Imx3dsf) {
      Imx3dsf* const ims = (Imx3dsf*)objs[0];
      objd[0] = new Img3duc(ims->Props());
      
      Img3duc* const imd = (Img3duc*)objd[0];
      
      result = PBinarization(*ims,*imd,(float)atof(parv[0]), (float)atof(parv[1]));
      goto end;
   }
   if (objs[0]->Type() == Po_Reg2d) {
      Reg2d* const ims = (Reg2d*)objs[0];
      objd[0] = new Img2duc(ims->Props());
      
      Img2duc* const imd = (Img2duc*)objd[0];
      
      result = PBinarization(*ims,*imd,(float)atof(parv[0]), (float)atof(parv[1]));
      goto end;
   }
   if (objs[0]->Type() == Po_Reg3d) {
      Reg3d* const ims = (Reg3d*)objs[0];
      objd[0] = new Img3duc(ims->Props());
      
      Img3duc* const imd = (Img3duc*)objd[0];
      
      result = PBinarization(*ims,*imd,(float)atof(parv[0]), (float)atof(parv[1]));
      goto end;
   }
   if (objs[0]->Type() == Po_Graph2d) {
      Graph2d* const grs = (Graph2d*)objs[0];
      objd[0] = new Graph2d(grs->Size());
      
      Graph2d* const grd = (Graph2d*)objd[0];
      
      result = PBinarization(*grs, *grd, (Graph2d::ValueType)atof(parv[0]), (Graph2d::ValueType)atof(parv[1]));
      goto end;
   }
   if (objs[0]->Type() == Po_Graph3d) {
      Graph3d* const grs = (Graph3d*)objs[0];
      objd[0] = new Graph3d(grs->Size());
      
      Graph3d* const grd = (Graph3d*)objd[0];
      
      result = PBinarization(*grs, *grd, (Graph3d::ValueType)atof(parv[0]), (Graph3d::ValueType)atof(parv[1]));
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
