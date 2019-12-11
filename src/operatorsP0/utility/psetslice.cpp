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

/**
 * @author Régis Clouard - 2006-05-30
 */

#include <pandore.h>
using namespace pandore;

/**
 * @file psetslice.cpp
 * Sets one slice to a 3D image.
 */

template <typename T>
Errc PSetSlice( const Imx3d<T> &ims1, const Imx2d<T> &ims2, Imx3d<T> &imd, long slice ) {
   Point2d p;
   
   if (slice < 0 || slice >= ims1.Depth()) {
      return FAILURE;
   }

   imd = ims1;
   for (p.y = 0;p.y < ims2.Height(); p.y++) {
      for (p.x = 0; p.x < ims2.Width(); p.x++) {
	 for (int b = 0; b < ims2.Bands(); b++) {
	    imd[b][slice][p.y][p.x] = ims2[b][p];
	 }
      } 
   }
   return SUCCESS;
}






Errc PSetSlice( const Reg3d &rgs1, const Reg2d &rgs2, Reg3d &rgd, long slice ) {
   Point2d p;
   Ulong nlabels = 0;

   if (slice < 0 || slice >= rgs1.Depth()) {
      return FAILURE;
   }

   rgd = rgs1;
   for (p.y = 0;p.y < rgd.Height();p.y++) {
      for (p.x = 0;p.x < rgd.Width();p.x++) {
	 if ((rgd[slice][p.y][p.x] = rgs2[p])>nlabels) {
	    nlabels = rgs2[p];
	 }
      }
   }
   rgd.Labels(nlabels);
   return SUCCESS;
}


#ifdef MAIN
#define	USAGE	"usage: %s slice [im_in1|-] [im_in2|-] [im_out|-]"
#define	PARC	1
#define	FINC	2
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
   if (objs[0]->Type() == Po_Img3duc && objs[1]->Type() == Po_Img2duc) {
      Img3duc* const ims1 = (Img3duc*)objs[0];
      Img2duc* const ims2 = (Img2duc*)objs[1];

      objd[0] = new Img3duc(ims1->Props());      
      Img3duc* const imd = (Img3duc*)objd[0];

      result = PSetSlice(*ims1, *ims2, *imd, atol(parv[0]));
      goto end;
   }
   if (objs[0]->Type() == Po_Img3dsl && objs[1]->Type() == Po_Img2dsl) {
      Img3dsl* const ims1 = (Img3dsl*)objs[0];
      Img2dsl* const ims2 = (Img2dsl*)objs[1];

      objd[0] = new Img3dsl(ims1->Props());      
      Img3dsl* const imd = (Img3dsl*)objd[0];

      result = PSetSlice(*ims1, *ims2, *imd, atol(parv[0]));
      goto end;
   }
   if (objs[0]->Type() == Po_Img3dsf && objs[1]->Type() == Po_Img2dsf) {
      Img3dsf* const ims1 = (Img3dsf*)objs[0];
      Img2dsf* const ims2 = (Img2dsf*)objs[1];

      objd[0] = new Img3dsf(ims1->Props());      
      Img3dsf* const imd = (Img3dsf*)objd[0];

      result = PSetSlice(*ims1, *ims2, *imd, atol(parv[0]));
      goto end;
   }
   if (objs[0]->Type() == Po_Imc3duc && objs[1]->Type() == Po_Imc2duc) {
      Imc3duc* const ims1 = (Imc3duc*)objs[0];
      Imc2duc* const ims2 = (Imc2duc*)objs[1];

      objd[0] = new Imc3duc(ims1->Props());      
      Imc3duc* const imd = (Imc3duc*)objd[0];

      result = PSetSlice(*ims1, *ims2, *imd, atol(parv[0]));
      goto end;
   }
   if (objs[0]->Type() == Po_Imc3dsl && objs[1]->Type() == Po_Imc2dsl) {
      Imc3dsl* const ims1 = (Imc3dsl*)objs[0];
      Imc2dsl* const ims2 = (Imc2dsl*)objs[1];

      objd[0] = new Imc3dsl(ims1->Props());      
      Imc3dsl* const imd = (Imc3dsl*)objd[0];

      result = PSetSlice(*ims1, *ims2, *imd, atol(parv[0]));
      goto end;
   }
   if (objs[0]->Type() == Po_Imc3dsf && objs[1]->Type() == Po_Imc2dsf) {
      Imc3dsf* const ims1 = (Imc3dsf*)objs[0];
      Imc2dsf* const ims2 = (Imc2dsf*)objs[1];

      objd[0] = new Imc3dsf(ims1->Props());      
      Imc3dsf* const imd = (Imc3dsf*)objd[0];

      result = PSetSlice(*ims1, *ims2, *imd, atol(parv[0]));
      goto end;
   }
   if (objs[0]->Type() == Po_Imx3duc && objs[1]->Type() == Po_Imx2duc) {
      Imx3duc* const ims1 = (Imx3duc*)objs[0];
      Imx2duc* const ims2 = (Imx2duc*)objs[1];

      objd[0] = new Imx3duc(ims1->Props());      
      Imx3duc* const imd = (Imx3duc*)objd[0];

      result = PSetSlice(*ims1, *ims2, *imd, atol(parv[0]));
      goto end;
   }
   if (objs[0]->Type() == Po_Imx3dsl && objs[1]->Type() == Po_Imx2dsl) {
      Imx3dsl* const ims1 = (Imx3dsl*)objs[0];
      Imx2dsl* const ims2 = (Imx2dsl*)objs[1];

      objd[0] = new Imx3dsl(ims1->Props());      
      Imx3dsl* const imd = (Imx3dsl*)objd[0];

      result = PSetSlice(*ims1, *ims2, *imd, atol(parv[0]));
      goto end;
   }
   if (objs[0]->Type() == Po_Imx3dsf && objs[1]->Type() == Po_Imx2dsf) {
      Imx3dsf* const ims1 = (Imx3dsf*)objs[0];
      Imx2dsf* const ims2 = (Imx2dsf*)objs[1];

      objd[0] = new Imx3dsf(ims1->Props());      
      Imx3dsf* const imd = (Imx3dsf*)objd[0];

      result = PSetSlice(*ims1, *ims2, *imd, atol(parv[0]));
      goto end;
   }
   if (objs[0]->Type() == Po_Reg3d && objs[1]->Type() == Po_Reg2d) {
      Reg3d* const rgs1 = (Reg3d*)objs[0];
      Reg2d* const rgs2 = (Reg2d*)objs[1];

      objd[0] = new Reg3d(rgs1->Props());
      Reg3d* const imd = (Reg3d*)objd[0];
      
      result = PSetSlice(*rgs1, *rgs2, *imd, atol(parv[0]));
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
