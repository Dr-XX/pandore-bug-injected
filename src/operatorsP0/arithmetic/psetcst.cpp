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
 * @file psetcst.cpp
 * @brief  Set all image pixels to the given constant.
 */

/**
 * Gray image (pixels).
 */
template <typename T>
Errc PSetCst( const Imx3d<T> &, Imx3d<T> &imd, T value ) {
   imd = value;
   return SUCCESS;
}




/**
 * Region maps.
 */
Errc PSetCst( const Reg2d &, Reg2d &rgd, Reg2d::ValueType value ) {
   rgd = value;
   if (value > 0) {
      rgd.Labels(1);
   } else {
      rgd.Labels(0);
   }
   
   return SUCCESS;
}

/**
 * Region maps.
 */
Errc PSetCst( const Reg3d &, Reg3d &rgd, Reg3d::ValueType value ) {
   rgd = value;
   if (value > 0) {
      rgd.Labels(1);
   } else {
      rgd.Labels(0);
   }
   
   return SUCCESS;
}

/**
 * Graph (nodes).
 */
Errc PSetCst( const Graph2d &grs, Graph2d &grd, Graph2d::ValueType value ) {   
   grd = grs;
   int deb = (grs[0]->value) ? 0 : 1;
   for (int i = deb; i < grs.Size(); i++) {
      if ((grd[i])) {
	 grd[i]->value = value;
      }
   }
   return SUCCESS;
}

/**
 * Graph (nodes).
 */
Errc PSetCst( const Graph3d &grs, Graph3d &grd, Graph3d::ValueType value ) {   
   grd = grs;
   int deb = (grs[0]->value) ? 0 : 1;
   for (int i = deb; i < grs.Size(); i++) {
      if ((grd[i])) {
	 grd[i]->value = value;
      }
   }
   return SUCCESS;
}


#ifdef MAIN
#define	USAGE	"usage: %s value [-m mask] [im_in|-] [im_out|-]"
#define	PARC	1
#define	FINC	1
#define	FOUTC	1
#define	MASK	3

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
      Img1duc* imd = (Img1duc*)objd[0];
      
      result = PSetCst(*ims, *imd, (Img1duc::ValueType)atof(parv[0]));
      goto end;
   } 
   if (objs[0]->Type() == Po_Img1dsl) {
      Img1dsl* const ims = (Img1dsl*)objs[0];
      objd[0] = new Img1dsl(ims->Props());
      Img1dsl* imd = (Img1dsl*)objd[0];
      
      result = PSetCst(*ims, *imd, (Img1dsl::ValueType)atof(parv[0]));
      goto end;
   } 
   if (objs[0]->Type() == Po_Img1dsf) {
      Img1dsf* const ims = (Img1dsf*)objs[0];
      objd[0] = new Img1dsf(ims->Props());
      Img1dsf* imd = (Img1dsf*)objd[0];
      
      result = PSetCst(*ims, *imd, (Img1dsf::ValueType)atof(parv[0]));
      goto end;
   } 
   if (objs[0]->Type() == Po_Img2duc) {
      Img2duc* const ims = (Img2duc*)objs[0];
      objd[0] = new Img2duc(ims->Props());
      Img2duc* imd = (Img2duc*)objd[0];
      
      result = PSetCst(*ims, *imd, (Img2duc::ValueType)atof(parv[0]));
      goto end;
   } 
   if (objs[0]->Type() == Po_Img2dsl) {
      Img2dsl* const ims = (Img2dsl*)objs[0];
      objd[0] = new Img2dsl(ims->Props());
      Img2dsl* imd = (Img2dsl*)objd[0];
      
      result = PSetCst(*ims, *imd, (Img2dsl::ValueType)atof(parv[0]));
      goto end;
   } 
   if (objs[0]->Type() == Po_Img2dsf) {
      Img2dsf* const ims = (Img2dsf*)objs[0];
      objd[0] = new Img2dsf(ims->Props());
      Img2dsf* imd = (Img2dsf*)objd[0];
      
      result = PSetCst(*ims, *imd, (Img2dsf::ValueType)atof(parv[0]));
      goto end;
   } 
   if (objs[0]->Type() == Po_Img3duc) {
      Img3duc* const ims = (Img3duc*)objs[0];
      objd[0] = new Img3duc(ims->Props());
      Img3duc* imd = (Img3duc*)objd[0];
      
      result = PSetCst(*ims, *imd, (Img3duc::ValueType)atof(parv[0]));
      goto end;
   } 
   if (objs[0]->Type() == Po_Img3dsl) {
      Img3dsl* const ims = (Img3dsl*)objs[0];
      objd[0] = new Img3dsl(ims->Props());
      Img3dsl* imd = (Img3dsl*)objd[0];
      
      result = PSetCst(*ims, *imd, (Img3dsl::ValueType)atof(parv[0]));
      goto end;
   } 
   if (objs[0]->Type() == Po_Img3dsf) {
      Img3dsf* const ims = (Img3dsf*)objs[0];
      objd[0] = new Img3dsf(ims->Props());
      Img3dsf* imd = (Img3dsf*)objd[0];
      
      result = PSetCst(*ims, *imd, (Img3dsf::ValueType)atof(parv[0]));
      goto end;
   } 
   if (objs[0]->Type() == Po_Imc2duc) {
      Imc2duc* const ims = (Imc2duc*)objs[0];
      objd[0] = new Imc2duc(ims->Props());
      Imc2duc* imd = (Imc2duc*)objd[0];
      
      result = PSetCst(*ims, *imd, (Imc2duc::ValueType)atof(parv[0]));
      goto end;
   } 
   if (objs[0]->Type() == Po_Imc2dsl) {
      Imc2dsl* const ims = (Imc2dsl*)objs[0];
      objd[0] = new Imc2dsl(ims->Props());
      Imc2dsl* imd = (Imc2dsl*)objd[0];
      
      result = PSetCst(*ims, *imd, (Imc2dsl::ValueType)atof(parv[0]));
      goto end;
   } 
   if (objs[0]->Type() == Po_Imc2dsf) {
      Imc2dsf* const ims = (Imc2dsf*)objs[0];
      objd[0] = new Imc2dsf(ims->Props());
      Imc2dsf* imd = (Imc2dsf*)objd[0];
      
      result = PSetCst(*ims, *imd, (Imc2dsf::ValueType)atof(parv[0]));
      goto end;
   } 
   if (objs[0]->Type() == Po_Imc3duc) {
      Imc3duc* const ims = (Imc3duc*)objs[0];
      objd[0] = new Imc3duc(ims->Props());
      Imc3duc* imd = (Imc3duc*)objd[0];
      
      result = PSetCst(*ims, *imd, (Imc3duc::ValueType)atof(parv[0]));
      goto end;
   } 
   if (objs[0]->Type() == Po_Imc3dsl) {
      Imc3dsl* const ims = (Imc3dsl*)objs[0];
      objd[0] = new Imc3dsl(ims->Props());
      Imc3dsl* imd = (Imc3dsl*)objd[0];
      
      result = PSetCst(*ims, *imd, (Imc3dsl::ValueType)atof(parv[0]));
      goto end;
   } 
   if (objs[0]->Type() == Po_Imc3dsf) {
      Imc3dsf* const ims = (Imc3dsf*)objs[0];
      objd[0] = new Imc3dsf(ims->Props());
      Imc3dsf* imd = (Imc3dsf*)objd[0];
      
      result = PSetCst(*ims, *imd, (Imc3dsf::ValueType)atof(parv[0]));
      goto end;
   } 
   if (objs[0]->Type() == Po_Imx1duc) {
      Imx1duc* const ims = (Imx1duc*)objs[0];
      objd[0] = new Imx1duc(ims->Props());
      Imx1duc* imd = (Imx1duc*)objd[0];
      
      result = PSetCst(*ims, *imd, (Imx1duc::ValueType)atof(parv[0]));
      goto end;
   } 
   if (objs[0]->Type() == Po_Imx1dsl) {
      Imx1dsl* const ims = (Imx1dsl*)objs[0];
      objd[0] = new Imx1dsl(ims->Props());
      Imx1dsl* imd = (Imx1dsl*)objd[0];
      
      result = PSetCst(*ims, *imd, (Imx1dsl::ValueType)atof(parv[0]));
      goto end;
   } 
   if (objs[0]->Type() == Po_Imx1dsf) {
      Imx1dsf* const ims = (Imx1dsf*)objs[0];
      objd[0] = new Imx1dsf(ims->Props());
      Imx1dsf* imd = (Imx1dsf*)objd[0];
      
      result = PSetCst(*ims, *imd, (Imx1dsf::ValueType)atof(parv[0]));
      goto end;
   } 
   if (objs[0]->Type() == Po_Imx2duc) {
      Imx2duc* const ims = (Imx2duc*)objs[0];
      objd[0] = new Imx2duc(ims->Props());
      Imx2duc* imd = (Imx2duc*)objd[0];
      
      result = PSetCst(*ims, *imd, (Imx2duc::ValueType)atof(parv[0]));
      goto end;
   } 
   if (objs[0]->Type() == Po_Imx2dsl) {
      Imx2dsl* const ims = (Imx2dsl*)objs[0];
      objd[0] = new Imx2dsl(ims->Props());
      Imx2dsl* imd = (Imx2dsl*)objd[0];
      
      result = PSetCst(*ims, *imd, (Imx2dsl::ValueType)atof(parv[0]));
      goto end;
   } 
   if (objs[0]->Type() == Po_Imx2dsf) {
      Imx2dsf* const ims = (Imx2dsf*)objs[0];
      objd[0] = new Imx2dsf(ims->Props());
      Imx2dsf* imd = (Imx2dsf*)objd[0];
      
      result = PSetCst(*ims, *imd, (Imx2dsf::ValueType)atof(parv[0]));
      goto end;
   } 
   if (objs[0]->Type() == Po_Imx3duc) {
      Imx3duc* const ims = (Imx3duc*)objs[0];
      objd[0] = new Imx3duc(ims->Props());
      Imx3duc* imd = (Imx3duc*)objd[0];
      
      result = PSetCst(*ims, *imd, (Imx3duc::ValueType)atof(parv[0]));
      goto end;
   } 
   if (objs[0]->Type() == Po_Imx3dsl) {
      Imx3dsl* const ims = (Imx3dsl*)objs[0];
      objd[0] = new Imx3dsl(ims->Props());
      Imx3dsl* imd = (Imx3dsl*)objd[0];
      
      result = PSetCst(*ims, *imd, (Imx3dsl::ValueType)atof(parv[0]));
      goto end;
   } 
   if (objs[0]->Type() == Po_Imx3dsf) {
      Imx3dsf* const ims = (Imx3dsf*)objs[0];
      objd[0] = new Imx3dsf(ims->Props());
      Imx3dsf* imd = (Imx3dsf*)objd[0];
      
      result = PSetCst(*ims, *imd, (Imx3dsf::ValueType)atof(parv[0]));
      goto end;
   } 
   if (objs[0]->Type() == Po_Reg2d) {
      Reg2d* const rgs = (Reg2d*)objs[0];
      objd[0] = new Reg2d(rgs->Size());
      Reg2d* rgd = (Reg2d*)objd[0];
      
      result = PSetCst(*rgs, *rgd, (Reg2d::ValueType)atof(parv[0]));
      goto end;
   }
   if (objs[0]->Type() == Po_Reg3d) {
      Reg3d* const rgs = (Reg3d*)objs[0];
      objd[0] = new Reg3d(rgs->Size());
      Reg3d* rgd = (Reg3d*)objd[0];
      
      result = PSetCst(*rgs, *rgd, (Reg3d::ValueType)atof(parv[0]));
      goto end;
   }
   if (objs[0]->Type() == Po_Graph2d) {
      Graph2d* const grs = (Graph2d*)objs[0];
      objd[0] = new Graph2d(grs->Size());
      Graph2d* const grd = (Graph2d*)objd[0];
      
      result = PSetCst(*grs, *grd, (float)atof(parv[0]));
      goto end;
   }
   if (objs[0]->Type() == Po_Graph3d) {
      Graph3d* const grs = (Graph3d*)objs[0];
      objd[0] = new Graph3d(grs->Size());
      Graph3d* const grd = (Graph3d*)objd[0];
      
      result = PSetCst(*grs, *grd, (float)atof(parv[0]));
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
