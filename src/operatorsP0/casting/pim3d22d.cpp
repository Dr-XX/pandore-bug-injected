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
 * @author Regis Clouard - 2006-05-30
 */

#include <math.h>
#include <pandore.h>
using namespace pandore;

/**
 * @file pim3d22d.cpp
 * Build 3D image from 2D image.
 */

template <typename T>
Errc PIm3d22d( const Imx3d<T> &ims, Imx3d<T> &imd ) {
   Point3d p;

   if (ims.Depth() > 1) {
      fprintf(stderr, "Error pim3d22d: input 3D image is too deep to be converted.\n");
      return FAILURE;
   }
   p.z = 0;
   for (int b = 0; b < ims.Bands(); b++) {
      for (p.y = 0; p.y < ims.Height(); p.y++) {
	 for (p.x = 0; p.x < ims.Width(); p.x++) {
	    imd[b][p] = ims[b][p];
	 }
      }
   }
   return SUCCESS;
}






Errc PIm3d22d( const Reg3d &rgs, Reg2d &rgd ) {
   Point3d p;

   if (rgs.Depth() > 1) {
      fprintf(stderr, "Error pim3d22d: input 3D image is too deep to be converted.\n");
      return FAILURE;
   }
   p.z = 0;
   for (p.y = 0; p.y < rgs.Height(); p.y++) {
      for (p.x = 0; p.x < rgs.Width(); p.x++) {
	 rgd[p.y][p.x] = rgs[p];
      }
   }

   return SUCCESS;
}

#ifdef MAIN
#define	USAGE	"usage: %s [im_in|-] [im_out|-]"
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
   if (objs[0]->Type() == Po_Img3duc) {

      Img3duc* const ims = (Img3duc*)objs[0];
      objd[0] = new Img2duc (ims->Props());
      Img2duc* const imd = (Img2duc*)objd[0];

      result = PIm3d22d(*ims, *imd);
      goto end;
   }
   if (objs[0]->Type() == Po_Img3dsl) {

      Img3dsl* const ims = (Img3dsl*)objs[0];
      objd[0] = new Img2dsl (ims->Props());
      Img2dsl* const imd = (Img2dsl*)objd[0];

      result = PIm3d22d(*ims, *imd);
      goto end;
   }
   if (objs[0]->Type() == Po_Img3dsf) {

      Img3dsf* const ims = (Img3dsf*)objs[0];
      objd[0] = new Img2dsf (ims->Props());
      Img2dsf* const imd = (Img2dsf*)objd[0];

      result = PIm3d22d(*ims, *imd);
      goto end;
   }
   if (objs[0]->Type() == Po_Imc3duc) {

      Imc3duc* const ims = (Imc3duc*)objs[0];
      objd[0] = new Imc2duc (ims->Props());
      Imc2duc* const imd = (Imc2duc*)objd[0];

      result = PIm3d22d(*ims, *imd);
      goto end;
   }
   if (objs[0]->Type() == Po_Imc3dsl) {

      Imc3dsl* const ims = (Imc3dsl*)objs[0];
      objd[0] = new Imc2dsl (ims->Props());
      Imc2dsl* const imd = (Imc2dsl*)objd[0];

      result = PIm3d22d(*ims, *imd);
      goto end;
   }
   if (objs[0]->Type() == Po_Imc3dsf) {

      Imc3dsf* const ims = (Imc3dsf*)objs[0];
      objd[0] = new Imc2dsf (ims->Props());
      Imc2dsf* const imd = (Imc2dsf*)objd[0];

      result = PIm3d22d(*ims, *imd);
      goto end;
   }
   if (objs[0]->Type() == Po_Imx3duc) {

      Imx3duc* const ims = (Imx3duc*)objs[0];
      objd[0] = new Imx2duc (ims->Props());
      Imx2duc* const imd = (Imx2duc*)objd[0];

      result = PIm3d22d(*ims, *imd);
      goto end;
   }
   if (objs[0]->Type() == Po_Imx3dsl) {

      Imx3dsl* const ims = (Imx3dsl*)objs[0];
      objd[0] = new Imx2dsl (ims->Props());
      Imx2dsl* const imd = (Imx2dsl*)objd[0];

      result = PIm3d22d(*ims, *imd);
      goto end;
   }
   if (objs[0]->Type() == Po_Imx3dsf) {

      Imx3dsf* const ims = (Imx3dsf*)objs[0];
      objd[0] = new Imx2dsf (ims->Props());
      Imx2dsf* const imd = (Imx2dsf*)objd[0];

      result = PIm3d22d(*ims, *imd);
      goto end;
   }
   if (objs[0]->Type() == Po_Reg3d) {

      Reg3d* const rgs = (Reg3d*)objs[0];
      objd[0] = new Reg2d(rgs->Props());
      Reg2d* const rgd = (Reg2d*)objd[0];

      result = PIm3d22d(*rgs, *rgd);
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
