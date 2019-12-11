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
 * @file paddslice.cpp
 * Adds one slice at the beginning (if direction <=0)
 * or at the end (if direction >0) of a 3D image.
 */
template <typename T>
Errc PAddSlice( const Imx3d<T> &ims1, const Imx2d<T> &ims2, Imx3d<T> &imd, int direction ) {
   PobjectProps props=ims1.Props();
   props.ndep++;
   imd.New(props);
   
   Long shift=(direction <= 0)? 1: 0;

   Point3d p1;
   for (p1.z=0; p1.z<ims1.Depth(); p1.z++) {
      for (p1.y=0; p1.y<ims1.Height(); p1.y++) {
	 for (p1.x=0; p1.x<ims1.Width(); p1.x++) {
	    for (int b=0; b<ims1.Bands(); b++) {
	       imd[b][p1.z+shift][p1.y][p1.x]=ims1[b][p1];
	    }
	 }
      }
   }
   Long plane = (direction<=0)? 0: imd.Depth()-1;

   Point2d p2;
   for (p2.y=0; p2.y<ims2.Height(); p2.y++) {
      for (p2.x=0; p2.x<ims2.Width(); p2.x++) {
	 for (int b=0; b<ims2.Bands(); b++) {
	    imd[b][plane][p2.y][p2.x]=ims2[b][p2];
	 }
      }
   }
   
   return SUCCESS;
}





#ifdef MAIN
#define	USAGE	"usage: %s direction [im_in1|-] [im_in2|-] [im_out|-]"
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
   if (objs[0]->Type()==Po_Img3duc && objs[1]->Type()==Po_Img2duc) {
      Img3duc* const ims1=(Img3duc*)objs[0];
      Img2duc* const ims2=(Img2duc*)objs[1];

      objd[0]=new Img3duc;
      Img3duc* const imd=(Img3duc*)objd[0];

      result=PAddSlice(*ims1, *ims2, *imd, atol(parv[0]));
      goto end;
   }
   if (objs[0]->Type()==Po_Img3dsl && objs[1]->Type()==Po_Img2dsl) {
      Img3dsl* const ims1=(Img3dsl*)objs[0];
      Img2dsl* const ims2=(Img2dsl*)objs[1];

      objd[0]=new Img3dsl;
      Img3dsl* const imd=(Img3dsl*)objd[0];

      result=PAddSlice(*ims1, *ims2, *imd, atol(parv[0]));
      goto end;
   }
   if (objs[0]->Type()==Po_Img3dsf && objs[1]->Type()==Po_Img2dsf) {
      Img3dsf* const ims1=(Img3dsf*)objs[0];
      Img2dsf* const ims2=(Img2dsf*)objs[1];

      objd[0]=new Img3dsf;
      Img3dsf* const imd=(Img3dsf*)objd[0];

      result=PAddSlice(*ims1, *ims2, *imd, atol(parv[0]));
      goto end;
   }
   if (objs[0]->Type()==Po_Imc3duc && objs[1]->Type()==Po_Imc2duc) {
      Imc3duc* const ims1=(Imc3duc*)objs[0];
      Imc2duc* const ims2=(Imc2duc*)objs[1];

      objd[0]=new Imc3duc;
      Imc3duc* const imd=(Imc3duc*)objd[0];

      result=PAddSlice(*ims1, *ims2, *imd, atol(parv[0]));
      goto end;
   }
   if (objs[0]->Type()==Po_Imc3dsl && objs[1]->Type()==Po_Imc2dsl) {
      Imc3dsl* const ims1=(Imc3dsl*)objs[0];
      Imc2dsl* const ims2=(Imc2dsl*)objs[1];

      objd[0]=new Imc3dsl;
      Imc3dsl* const imd=(Imc3dsl*)objd[0];

      result=PAddSlice(*ims1, *ims2, *imd, atol(parv[0]));
      goto end;
   }
   if (objs[0]->Type()==Po_Imc3dsf && objs[1]->Type()==Po_Imc2dsf) {
      Imc3dsf* const ims1=(Imc3dsf*)objs[0];
      Imc2dsf* const ims2=(Imc2dsf*)objs[1];

      objd[0]=new Imc3dsf;
      Imc3dsf* const imd=(Imc3dsf*)objd[0];

      result=PAddSlice(*ims1, *ims2, *imd, atol(parv[0]));
      goto end;
   }
   if (objs[0]->Type()==Po_Imx3duc && objs[1]->Type()==Po_Imx2duc) {
      Imx3duc* const ims1=(Imx3duc*)objs[0];
      Imx2duc* const ims2=(Imx2duc*)objs[1];

      objd[0]=new Imx3duc;
      Imx3duc* const imd=(Imx3duc*)objd[0];

      result=PAddSlice(*ims1, *ims2, *imd, atol(parv[0]));
      goto end;
   }
   if (objs[0]->Type()==Po_Imx3dsl && objs[1]->Type()==Po_Imx2dsl) {
      Imx3dsl* const ims1=(Imx3dsl*)objs[0];
      Imx2dsl* const ims2=(Imx2dsl*)objs[1];

      objd[0]=new Imx3dsl;
      Imx3dsl* const imd=(Imx3dsl*)objd[0];

      result=PAddSlice(*ims1, *ims2, *imd, atol(parv[0]));
      goto end;
   }
   if (objs[0]->Type()==Po_Imx3dsf && objs[1]->Type()==Po_Imx2dsf) {
      Imx3dsf* const ims1=(Imx3dsf*)objs[0];
      Imx2dsf* const ims2=(Imx2dsf*)objs[1];

      objd[0]=new Imx3dsf;
      Imx3dsf* const imd=(Imx3dsf*)objd[0];

      result=PAddSlice(*ims1, *ims2, *imd, atol(parv[0]));
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
