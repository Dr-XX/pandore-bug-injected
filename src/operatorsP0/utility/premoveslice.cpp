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
 * @file premoveslice.cpp
 * Removes one slice at the beginning (if direction <=0)
 * or at the end (if direction >0) of a 3D image.
 */
template <typename T>
Errc PRemoveSlice( const Imx3d<T> &ims, Imx3d<T> &imd1, Imx2d<T> &imd2, int direction ) {
   PobjectProps props = ims.Props();
   if (--props.ndep <= 0) {
      return FAILURE;
   }
   
   imd1.New(props);
   props.ndep = 0;
   imd2.New(props);
   
   Long shift = (direction <= 0)? 1: 0;

   Point3d p;
   for (p.z = 0; p.z < imd1.Depth(); p.z++) {
      for (p.y = 0; p.y < imd1.Height(); p.y++) {
	 for (p.x = 0; p.x < imd1.Width(); p.x++) {
	    for (int b = 0; b < imd1.Bands(); b++) {
	       imd1[b][p] = ims[b][p.z + shift][p.y][p.x];
	    }
	 }
      }
   }
   
   p.z = (direction <= 0) ? 0 : ims.Depth() - 1;

   for (p.y = 0; p.y < ims.Height(); p.y++) {
      for (p.x = 0; p.x < ims.Width(); p.x++) {
	 for (int b = 0; b < ims.Bands(); b++) { 
	    imd2[b][p.y][p.x] = ims[b][p];
	 }
      }
   }
   return SUCCESS;
}





#ifdef MAIN
#define	USAGE	"usage: %s direction [im_in|-] [im_ou1|-] [im_out2|-]"
#define	PARC	1
#define	FINC	1
#define	FOUTC	2
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
   if (objs[0]->Type() == Po_Img3duc) {
      Img3duc* const ims = (Img3duc*)objs[0];

      objd[0] = new Img3duc;
      Img3duc* const imd1 = (Img3duc*)objd[0];
      objd[1] = new Img2duc;
      Img2duc* const imd2 = (Img2duc*)objd[1];

      result = PRemoveSlice(*ims, *imd1, *imd2, atol(parv[0]));
      goto end;
   }
   if (objs[0]->Type() == Po_Img3dsl) {
      Img3dsl* const ims = (Img3dsl*)objs[0];

      objd[0] = new Img3dsl;
      Img3dsl* const imd1 = (Img3dsl*)objd[0];
      objd[1] = new Img2dsl;
      Img2dsl* const imd2 = (Img2dsl*)objd[1];

      result = PRemoveSlice(*ims, *imd1, *imd2, atol(parv[0]));
      goto end;
   }
   if (objs[0]->Type() == Po_Img3dsf) {
      Img3dsf* const ims = (Img3dsf*)objs[0];

      objd[0] = new Img3dsf;
      Img3dsf* const imd1 = (Img3dsf*)objd[0];
      objd[1] = new Img2dsf;
      Img2dsf* const imd2 = (Img2dsf*)objd[1];

      result = PRemoveSlice(*ims, *imd1, *imd2, atol(parv[0]));
      goto end;
   }
   if (objs[0]->Type() == Po_Imc3duc) {
      Imc3duc* const ims = (Imc3duc*)objs[0];

      objd[0] = new Imc3duc;
      Imc3duc* const imd1 = (Imc3duc*)objd[0];
      objd[1] = new Imc2duc;
      Imc2duc* const imd2 = (Imc2duc*)objd[1];

      result = PRemoveSlice(*ims, *imd1, *imd2, atol(parv[0]));
      goto end;
   }
   if (objs[0]->Type() == Po_Imc3dsl) {
      Imc3dsl* const ims = (Imc3dsl*)objs[0];

      objd[0] = new Imc3dsl;
      Imc3dsl* const imd1 = (Imc3dsl*)objd[0];
      objd[1] = new Imc2dsl;
      Imc2dsl* const imd2 = (Imc2dsl*)objd[1];

      result = PRemoveSlice(*ims, *imd1, *imd2, atol(parv[0]));
      goto end;
   }
   if (objs[0]->Type() == Po_Imc3dsf) {
      Imc3dsf* const ims = (Imc3dsf*)objs[0];

      objd[0] = new Imc3dsf;
      Imc3dsf* const imd1 = (Imc3dsf*)objd[0];
      objd[1] = new Imc2dsf;
      Imc2dsf* const imd2 = (Imc2dsf*)objd[1];

      result = PRemoveSlice(*ims, *imd1, *imd2, atol(parv[0]));
      goto end;
   }
   if (objs[0]->Type() == Po_Imx3duc) {
      Imx3duc* const ims = (Imx3duc*)objs[0];

      objd[0] = new Imx3duc;
      Imx3duc* const imd1 = (Imx3duc*)objd[0];
      objd[1] = new Imx2duc;
      Imx2duc* const imd2 = (Imx2duc*)objd[1];

      result = PRemoveSlice(*ims, *imd1, *imd2, atol(parv[0]));
      goto end;
   }
   if (objs[0]->Type() == Po_Imx3dsl) {
      Imx3dsl* const ims = (Imx3dsl*)objs[0];

      objd[0] = new Imx3dsl;
      Imx3dsl* const imd1 = (Imx3dsl*)objd[0];
      objd[1] = new Imx2dsl;
      Imx2dsl* const imd2 = (Imx2dsl*)objd[1];

      result = PRemoveSlice(*ims, *imd1, *imd2, atol(parv[0]));
      goto end;
   }
   if (objs[0]->Type() == Po_Imx3dsf) {
      Imx3dsf* const ims = (Imx3dsf*)objs[0];

      objd[0] = new Imx3dsf;
      Imx3dsf* const imd1 = (Imx3dsf*)objd[0];
      objd[1] = new Imx2dsf;
      Imx2dsf* const imd2 = (Imx2dsf*)objd[1];

      result = PRemoveSlice(*ims, *imd1, *imd2, atol(parv[0]));
      goto end;
   }
   if (objs[0]->Type() == Po_Reg3d) {
      Reg3d* const ims = (Reg3d*)objs[0];

      objd[0] = new Reg3d;
      Reg3d* const imd1 = (Reg3d*)objd[0];
      objd[1] = new Reg2d;
      Reg2d* const imd2 = (Reg2d*)objd[1];

      result = PRemoveSlice(*ims, *imd1, *imd2, atol(parv[0]));
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
