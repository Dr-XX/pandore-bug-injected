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
 * @file pim2d23d.cpp
 * Build 3D image from 2D image.
 */
template <typename T>
Errc PIm2d23d( const Imx3d<T> &ims, Imx3d<T> &imd ) {
   Point3d p;
   
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





Errc PIm2d23d( const Reg2d &rgs, Reg3d &rgd ) {
   Point3d p;
   
   p.z = 0;
   for (p.y =0 ; p.y < rgs.Height(); p.y++) {
      for (p.x = 0; p.x < rgs.Width(); p.x++) {
	 rgd[p] = rgs[p.y][p.x];
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
   if (objs[0]->Type() == Po_Img2duc) {

      Img2duc* const ims = (Img2duc*)objs[0];
      PobjectProps props = ims->Props();
      props.ndep = 1;
      objd[0] = new Img3duc(props);
      Img3duc* const imd = (Img3duc*)objd[0];
      
      result = PIm2d23d(*ims, *imd);
      goto end;
   }
   if (objs[0]->Type() == Po_Img2dsl) {

      Img2dsl* const ims = (Img2dsl*)objs[0];
      PobjectProps props = ims->Props();
      props.ndep = 1;
      objd[0] = new Img3dsl(props);
      Img3dsl* const imd = (Img3dsl*)objd[0];
      
      result = PIm2d23d(*ims, *imd);
      goto end;
   }
   if (objs[0]->Type() == Po_Img2dsf) {

      Img2dsf* const ims = (Img2dsf*)objs[0];
      PobjectProps props = ims->Props();
      props.ndep = 1;
      objd[0] = new Img3dsf(props);
      Img3dsf* const imd = (Img3dsf*)objd[0];
      
      result = PIm2d23d(*ims, *imd);
      goto end;
   }
   if (objs[0]->Type() == Po_Imc2duc) {

      Imc2duc* const ims = (Imc2duc*)objs[0];
      PobjectProps props = ims->Props();
      props.ndep = 1;
      objd[0] = new Imc3duc(props);
      Imc3duc* const imd = (Imc3duc*)objd[0];
      
      result = PIm2d23d(*ims, *imd);
      goto end;
   }
   if (objs[0]->Type() == Po_Imc2dsl) {

      Imc2dsl* const ims = (Imc2dsl*)objs[0];
      PobjectProps props = ims->Props();
      props.ndep = 1;
      objd[0] = new Imc3dsl(props);
      Imc3dsl* const imd = (Imc3dsl*)objd[0];
      
      result = PIm2d23d(*ims, *imd);
      goto end;
   }
   if (objs[0]->Type() == Po_Imc2dsf) {

      Imc2dsf* const ims = (Imc2dsf*)objs[0];
      PobjectProps props = ims->Props();
      props.ndep = 1;
      objd[0] = new Imc3dsf(props);
      Imc3dsf* const imd = (Imc3dsf*)objd[0];
      
      result = PIm2d23d(*ims, *imd);
      goto end;
   }
   if (objs[0]->Type() == Po_Imx2duc) {

      Imx2duc* const ims = (Imx2duc*)objs[0];
      PobjectProps props = ims->Props();
      props.ndep = 1;
      objd[0] = new Imx3duc(props);
      Imx3duc* const imd = (Imx3duc*)objd[0];
      
      result = PIm2d23d(*ims, *imd);
      goto end;
   }
   if (objs[0]->Type() == Po_Imx2dsl) {

      Imx2dsl* const ims = (Imx2dsl*)objs[0];
      PobjectProps props = ims->Props();
      props.ndep = 1;
      objd[0] = new Imx3dsl(props);
      Imx3dsl* const imd = (Imx3dsl*)objd[0];
      
      result = PIm2d23d(*ims, *imd);
      goto end;
   }
   if (objs[0]->Type() == Po_Imx2dsf) {

      Imx2dsf* const ims = (Imx2dsf*)objs[0];
      PobjectProps props = ims->Props();
      props.ndep = 1;
      objd[0] = new Imx3dsf(props);
      Imx3dsf* const imd = (Imx3dsf*)objd[0];
      
      result = PIm2d23d(*ims, *imd);
      goto end;
   }
   if (objs[0]->Type() == Po_Reg2d) {

      Reg2d* const rgs = (Reg2d*)objs[0];
      PobjectProps props = rgs->Props();
      props.ndep = 1;
      objd[0] = new Reg3d(props);
      Reg3d* const rgd = (Reg3d*)objd[0];

      result = PIm2d23d(*rgs, *rgd);
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
