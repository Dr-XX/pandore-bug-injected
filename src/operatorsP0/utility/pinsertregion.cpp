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
 * @author Régis Clouard - 1995-21-07
 */

#include <pandore.h>
using namespace pandore;

template <typename T, typename U>
Errc PInsertRegion( const Imx3d<T> &msk, const Imx3d<U> &ims1,  const Imx3d<U> &ims2, Imx3d<U> &imd ) {
   if (msk.Width() != ims2.Width()
       || msk.Height() != ims2.Height()
       || msk.Depth() != ims2.Depth()) {
      std::cerr << "Error pinsertregion: incompatibles input images sizes" << std::endl;
      return FAILURE;
   }

   // The bounding box coordinates.
   int x1 = msk.Width();
   int y1 = msk.Height();
   int z1 = msk.Depth();
   int x2 = 0;
   int y2 = 0;
   int z2 = 0;
   Point3d p;

   // Crop the image with respect to the bounding box.
   for (p.z = 0; p.z < msk.Depth(); p.z++) {
      for (p.y = 0; p.y < msk.Height(); p.y++) {
	 for (p.x = 0; p.x < msk.Width(); p.x++) {
	    if (msk[0][p] > 0) {
	       x1 = (x1 < p.x) ? x1 : p.x; 
	       y1 = (y1 < p.y) ? y1 : p.y; 
	       z1 = (z1 < p.z) ? z1 : p.z; 
	       x2 = (x2 > p.x) ? x2 : p.x; 
	       y2 = (y2 > p.y) ? y2 : p.y; 
	       z2 = (z2 > p.z) ? z2 : p.z; 
	    }
	 }
      }
   }
   for (int b = 0; b < ims2.Bands(); b++) {
      for (p.z = 0; p.z < ims2.Depth(); p.z++) {
	 for (p.y = 0; p.y < ims2.Height(); p.y++) {
	    for (p.x = 0; p.x < ims2.Width(); p.x++) {
	       if (p.x >= x1 && p.x <= x2
		   && p.y >= y1 && p.y <= y2
		   && p.z >= z1 && p.z <= z2) {
		  imd[b][p] = ims1(b, p.z - z1, p.y - y1, p.x - x1);
	       } else {
		  imd[b][p] = ims2(b, p.z, p.y, p.x);
	       }
	    }
	 }
      }
   }

   return SUCCESS;
}




#ifdef MAIN
#define	USAGE	"usage: [mask_in|-] [im_in1|-] [im_in2|-] [im_out|-]"
#define	PARC	0
#define	FINC	3
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
   if (objs[0]->Type() == Po_Reg2d
       && objs[1]->Type() == Po_Img2duc
       && objs[2]->Type() == Po_Img2duc) {
      Reg2d* const msk = (Reg2d*)objs[0];
      Img2duc* const ims1 = (Img2duc*)objs[1];
      Img2duc* const ims2 = (Img2duc*)objs[2];
      objd[0] = new Img2duc(ims2->Props());
      Img2duc* const imd = (Img2duc*)objd[0];
      result = PInsertRegion(*msk, *ims1, *ims2, *imd);
      goto end;
   }
   if (objs[0]->Type() == Po_Reg2d
       && objs[1]->Type() == Po_Img2dsl
       && objs[2]->Type() == Po_Img2dsl) {
      Reg2d* const msk = (Reg2d*)objs[0];
      Img2dsl* const ims1 = (Img2dsl*)objs[1];
      Img2dsl* const ims2 = (Img2dsl*)objs[2];
      objd[0] = new Img2dsl(ims2->Props());
      Img2dsl* const imd = (Img2dsl*)objd[0];
      result = PInsertRegion(*msk, *ims1, *ims2, *imd);
      goto end;
   }
   if (objs[0]->Type() == Po_Reg2d
       && objs[1]->Type() == Po_Img2dsf
       && objs[2]->Type() == Po_Img2dsf) {
      Reg2d* const msk = (Reg2d*)objs[0];
      Img2dsf* const ims1 = (Img2dsf*)objs[1];
      Img2dsf* const ims2 = (Img2dsf*)objs[2];
      objd[0] = new Img2dsf(ims2->Props());
      Img2dsf* const imd = (Img2dsf*)objd[0];
      result = PInsertRegion(*msk, *ims1, *ims2, *imd);
      goto end;
   }
   if (objs[0]->Type() == Po_Reg2d
       && objs[1]->Type() == Po_Imc2duc
       && objs[2]->Type() == Po_Imc2duc) {
      Reg2d* const msk = (Reg2d*)objs[0];
      Imc2duc* const ims1 = (Imc2duc*)objs[1];
      Imc2duc* const ims2 = (Imc2duc*)objs[2];
      objd[0] = new Imc2duc(ims2->Props());
      Imc2duc* const imd = (Imc2duc*)objd[0];
      result = PInsertRegion(*msk, *ims1, *ims2, *imd);
      goto end;
   }
   if (objs[0]->Type() == Po_Reg2d
       && objs[1]->Type() == Po_Imc2dsl
       && objs[2]->Type() == Po_Imc2dsl) {
      Reg2d* const msk = (Reg2d*)objs[0];
      Imc2dsl* const ims1 = (Imc2dsl*)objs[1];
      Imc2dsl* const ims2 = (Imc2dsl*)objs[2];
      objd[0] = new Imc2dsl(ims2->Props());
      Imc2dsl* const imd = (Imc2dsl*)objd[0];
      result = PInsertRegion(*msk, *ims1, *ims2, *imd);
      goto end;
   }
   if (objs[0]->Type() == Po_Reg2d
       && objs[1]->Type() == Po_Imc2dsf
       && objs[2]->Type() == Po_Imc2dsf) {
      Reg2d* const msk = (Reg2d*)objs[0];
      Imc2dsf* const ims1 = (Imc2dsf*)objs[1];
      Imc2dsf* const ims2 = (Imc2dsf*)objs[2];
      objd[0] = new Imc2dsf(ims2->Props());
      Imc2dsf* const imd = (Imc2dsf*)objd[0];
      result = PInsertRegion(*msk, *ims1, *ims2, *imd);
      goto end;
   }
   if (objs[0]->Type() == Po_Reg2d
       && objs[1]->Type() == Po_Imx2duc
       && objs[2]->Type() == Po_Imx2duc) {
      Reg2d* const msk = (Reg2d*)objs[0];
      Imx2duc* const ims1 = (Imx2duc*)objs[1];
      Imx2duc* const ims2 = (Imx2duc*)objs[2];
      objd[0] = new Imx2duc(ims2->Props());
      Imx2duc* const imd = (Imx2duc*)objd[0];
      result = PInsertRegion(*msk, *ims1, *ims2, *imd);
      goto end;
   }
   if (objs[0]->Type() == Po_Reg2d
       && objs[1]->Type() == Po_Imx2dsl
       && objs[2]->Type() == Po_Imx2dsl) {
      Reg2d* const msk = (Reg2d*)objs[0];
      Imx2dsl* const ims1 = (Imx2dsl*)objs[1];
      Imx2dsl* const ims2 = (Imx2dsl*)objs[2];
      objd[0] = new Imx2dsl(ims2->Props());
      Imx2dsl* const imd = (Imx2dsl*)objd[0];
      result = PInsertRegion(*msk, *ims1, *ims2, *imd);
      goto end;
   }
   if (objs[0]->Type() == Po_Reg2d
       && objs[1]->Type() == Po_Imx2dsf
       && objs[2]->Type() == Po_Imx2dsf) {
      Reg2d* const msk = (Reg2d*)objs[0];
      Imx2dsf* const ims1 = (Imx2dsf*)objs[1];
      Imx2dsf* const ims2 = (Imx2dsf*)objs[2];
      objd[0] = new Imx2dsf(ims2->Props());
      Imx2dsf* const imd = (Imx2dsf*)objd[0];
      result = PInsertRegion(*msk, *ims1, *ims2, *imd);
      goto end;
   }
   if (objs[0]->Type() == Po_Reg3d
       && objs[1]->Type() == Po_Img3duc
       && objs[2]->Type() == Po_Img3duc) {
      Reg3d* const msk = (Reg3d*)objs[0];
      Img3duc* const ims1 = (Img3duc*)objs[1];
      Img3duc* const ims2 = (Img3duc*)objs[2];
      objd[0] = new Img3duc(ims2->Props());
      Img3duc* const imd = (Img3duc*)objd[0];
      result = PInsertRegion(*msk, *ims1, *ims2, *imd);
      goto end;
   }
   if (objs[0]->Type() == Po_Reg3d
       && objs[1]->Type() == Po_Img3dsl
       && objs[2]->Type() == Po_Img3dsl) {
      Reg3d* const msk = (Reg3d*)objs[0];
      Img3dsl* const ims1 = (Img3dsl*)objs[1];
      Img3dsl* const ims2 = (Img3dsl*)objs[2];
      objd[0] = new Img3dsl(ims2->Props());
      Img3dsl* const imd = (Img3dsl*)objd[0];
      result = PInsertRegion(*msk, *ims1, *ims2, *imd);
      goto end;
   }
   if (objs[0]->Type() == Po_Reg3d
       && objs[1]->Type() == Po_Img3dsf
       && objs[2]->Type() == Po_Img3dsf) {
      Reg3d* const msk = (Reg3d*)objs[0];
      Img3dsf* const ims1 = (Img3dsf*)objs[1];
      Img3dsf* const ims2 = (Img3dsf*)objs[2];
      objd[0] = new Img3dsf(ims2->Props());
      Img3dsf* const imd = (Img3dsf*)objd[0];
      result = PInsertRegion(*msk, *ims1, *ims2, *imd);
      goto end;
   }
   if (objs[0]->Type() == Po_Reg3d
       && objs[1]->Type() == Po_Imc3duc
       && objs[2]->Type() == Po_Imc3duc) {
      Reg3d* const msk = (Reg3d*)objs[0];
      Imc3duc* const ims1 = (Imc3duc*)objs[1];
      Imc3duc* const ims2 = (Imc3duc*)objs[2];
      objd[0] = new Imc3duc(ims2->Props());
      Imc3duc* const imd = (Imc3duc*)objd[0];
      result = PInsertRegion(*msk, *ims1, *ims2, *imd);
      goto end;
   }
   if (objs[0]->Type() == Po_Reg3d
       && objs[1]->Type() == Po_Imc3dsl
       && objs[2]->Type() == Po_Imc3dsl) {
      Reg3d* const msk = (Reg3d*)objs[0];
      Imc3dsl* const ims1 = (Imc3dsl*)objs[1];
      Imc3dsl* const ims2 = (Imc3dsl*)objs[2];
      objd[0] = new Imc3dsl(ims2->Props());
      Imc3dsl* const imd = (Imc3dsl*)objd[0];
      result = PInsertRegion(*msk, *ims1, *ims2, *imd);
      goto end;
   }
   if (objs[0]->Type() == Po_Reg3d
       && objs[1]->Type() == Po_Imc3dsf
       && objs[2]->Type() == Po_Imc3dsf) {
      Reg3d* const msk = (Reg3d*)objs[0];
      Imc3dsf* const ims1 = (Imc3dsf*)objs[1];
      Imc3dsf* const ims2 = (Imc3dsf*)objs[2];
      objd[0] = new Imc3dsf(ims2->Props());
      Imc3dsf* const imd = (Imc3dsf*)objd[0];
      result = PInsertRegion(*msk, *ims1, *ims2, *imd);
      goto end;
   }
   if (objs[0]->Type() == Po_Reg3d
       && objs[1]->Type() == Po_Imx3duc
       && objs[2]->Type() == Po_Imx3duc) {
      Reg3d* const msk = (Reg3d*)objs[0];
      Imx3duc* const ims1 = (Imx3duc*)objs[1];
      Imx3duc* const ims2 = (Imx3duc*)objs[2];
      objd[0] = new Imx3duc(ims2->Props());
      Imx3duc* const imd = (Imx3duc*)objd[0];
      result = PInsertRegion(*msk, *ims1, *ims2, *imd);
      goto end;
   }
   if (objs[0]->Type() == Po_Reg3d
       && objs[1]->Type() == Po_Imx3dsl
       && objs[2]->Type() == Po_Imx3dsl) {
      Reg3d* const msk = (Reg3d*)objs[0];
      Imx3dsl* const ims1 = (Imx3dsl*)objs[1];
      Imx3dsl* const ims2 = (Imx3dsl*)objs[2];
      objd[0] = new Imx3dsl(ims2->Props());
      Imx3dsl* const imd = (Imx3dsl*)objd[0];
      result = PInsertRegion(*msk, *ims1, *ims2, *imd);
      goto end;
   }
   if (objs[0]->Type() == Po_Reg3d
       && objs[1]->Type() == Po_Imx3dsf
       && objs[2]->Type() == Po_Imx3dsf) {
      Reg3d* const msk = (Reg3d*)objs[0];
      Imx3dsf* const ims1 = (Imx3dsf*)objs[1];
      Imx3dsf* const ims2 = (Imx3dsf*)objs[2];
      objd[0] = new Imx3dsf(ims2->Props());
      Imx3dsf* const imd = (Imx3dsf*)objd[0];
      result = PInsertRegion(*msk, *ims1, *ims2, *imd);
      goto end;
   }
   if (objs[0]->Type() == Po_Reg2d
       && objs[1]->Type() == Po_Reg2d
       && objs[2]->Type() == Po_Reg2d) {
      Reg2d* const msk = (Reg2d*)objs[0];
      Reg2d* const ims1 = (Reg2d*)objs[1];
      Reg2d* const ims2 = (Reg2d*)objs[2];
      objd[0] = new Reg2d(ims2->Props());
      Reg2d* const imd = (Reg2d*)objd[0];
      imd->Labels(ims2->Labels());
      result = PInsertRegion(*msk, *ims1, *ims2, *imd);
      goto end;
   }
   if (objs[0]->Type() == Po_Reg3d
       && objs[1]->Type() == Po_Reg3d
       && objs[2]->Type() == Po_Reg3d) {
      Reg3d* const msk = (Reg3d*)objs[0];
      Reg3d* const ims1 = (Reg3d*)objs[1];
      Reg3d* const ims2 = (Reg3d*)objs[2];
      objd[0] = new Reg3d(ims2->Props());
      Reg3d* const imd = (Reg3d*)objd[0];
      imd->Labels(ims2->Labels());
      result = PInsertRegion(*msk, *ims1, *ims2, *imd);
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
