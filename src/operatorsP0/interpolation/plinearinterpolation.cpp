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

/**
 * @author Régis Clouard - 1994-10-13
 * @author Régis Clouard - 2003-01-02
 * @author Régis Clouard - 2004-03-03 (bord).
 * @author Régis Clouard - 2005-10-21 (+ new parameter).
 * @author Régis Clouard - May 09, 2010 (with borders).
 * @author Régis Clouard - Jun 11, 2013 (new algorithm with integral image).
 */

#include <pandore.h>
using namespace pandore;

template < typename T >
Errc PBilinearInterpolation( const Imx3d<T> &ims, const Imx3duc &imm, Imx3d<T> &imd, int windowDepth, int windowHeight, int windowWidth ) {
   if (ims.Size() != imm.Size()) {
      std::cerr << "Error plinearinterpolation: Incompatible image size." << std::endl;
      return FAILURE;
   }
   const int dz = windowDepth / 2;
   const int dy = windowHeight / 2;
   const int dx = windowWidth / 2;
   for (int b = 0; b < ims.Bands(); b++) {
      for (int z = 0; z < ims.Depth(); z++) {
	 for (int y = 0; y < ims.Height(); y++) {
	    for (int x = 0; x < ims.Width(); x++) {
	       if (imm(0, z, y, x) == 0) {
		  imd(b, z, y, x) = ims(b, z, y, x);
	       } else {
		  float sigma = 0;
		  int count = 0;
		  for (int m = -dz; m <= dz; m++) {
		     for (int l = -dy; l <= dy; l++) {
		  	for (int k = -dx; k <= dx; k++) {
		  	   if (ims.Hold(z + m, y + l, x + k)
		  	       && !imm(0, z + m, y + l, x + k)) {
		  	      sigma += (float)ims(b, z + m, y + l, x + k);
		  	      count ++;
		  	   }
		  	}
		     }
		  }
		  if (count == 0) {
		     imd(b, z, y, x) = 0;
		  } else {
		     imd(b, z, y, x) = (T)(sigma / count);
		  }
	       }
	    }
	 }
      }
   }
   return SUCCESS;
}





#ifdef MAIN
	
/*
 * Modify only the following constants, and the function call.
 */
#define	USAGE	"usage: %s window_width window_height window_depth [-m mask] [im_in|-] [im_msk|-] [im_out|-]"
#define	PARC	3
#define	FINC	2
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
   if (objs[0]->Type() == Po_Img2duc && objs[1]->Type() == Po_Img2duc) {
      Img2duc* const ims = (Img2duc*)objs[0];
      Img2duc* const imm = (Img2duc*)objs[1];
      objd[0] = new Img2duc(ims->Props());
      Img2duc* const imd = (Img2duc*)objd[0];
      
      result = PBilinearInterpolation(*ims, *imm, *imd, 0, atoi(parv[1]), atoi(parv[0]));
      goto end;
   }
   if (objs[0]->Type() == Po_Img2dsl && objs[1]->Type() == Po_Img2duc) {
      Img2dsl* const ims = (Img2dsl*)objs[0];
      Img2duc* const imm = (Img2duc*)objs[1];
      objd[0] = new Img2dsl(ims->Props());
      Img2dsl* const imd = (Img2dsl*)objd[0];
      
      result = PBilinearInterpolation(*ims, *imm, *imd, 0, atoi(parv[1]), atoi(parv[0]));
      goto end;
   }
   if (objs[0]->Type() == Po_Img2dsf && objs[1]->Type() == Po_Img2duc) {
      Img2dsf* const ims = (Img2dsf*)objs[0];
      Img2duc* const imm = (Img2duc*)objs[1];
      objd[0] = new Img2dsf(ims->Props());
      Img2dsf* const imd = (Img2dsf*)objd[0];
      
      result = PBilinearInterpolation(*ims, *imm, *imd, 0, atoi(parv[1]), atoi(parv[0]));
      goto end;
   }
   if (objs[0]->Type() == Po_Imc2duc && objs[1]->Type() == Po_Img2duc) {
      Imc2duc* const ims = (Imc2duc*)objs[0];
      Img2duc* const imm = (Img2duc*)objs[1];
      objd[0] = new Imc2duc(ims->Props());
      Imc2duc* const imd = (Imc2duc*)objd[0];
      
      result = PBilinearInterpolation(*ims, *imm, *imd, 0, atoi(parv[1]), atoi(parv[0]));
      goto end;
   }
   if (objs[0]->Type() == Po_Imc2dsl && objs[1]->Type() == Po_Img2duc) {
      Imc2dsl* const ims = (Imc2dsl*)objs[0];
      Img2duc* const imm = (Img2duc*)objs[1];
      objd[0] = new Imc2dsl(ims->Props());
      Imc2dsl* const imd = (Imc2dsl*)objd[0];
      
      result = PBilinearInterpolation(*ims, *imm, *imd, 0, atoi(parv[1]), atoi(parv[0]));
      goto end;
   }
   if (objs[0]->Type() == Po_Imc2dsf && objs[1]->Type() == Po_Img2duc) {
      Imc2dsf* const ims = (Imc2dsf*)objs[0];
      Img2duc* const imm = (Img2duc*)objs[1];
      objd[0] = new Imc2dsf(ims->Props());
      Imc2dsf* const imd = (Imc2dsf*)objd[0];
      
      result = PBilinearInterpolation(*ims, *imm, *imd, 0, atoi(parv[1]), atoi(parv[0]));
      goto end;
   }
   if (objs[0]->Type() == Po_Imx2duc && objs[1]->Type() == Po_Img2duc) {
      Imx2duc* const ims = (Imx2duc*)objs[0];
      Img2duc* const imm = (Img2duc*)objs[1];
      objd[0] = new Imx2duc(ims->Props());
      Imx2duc* const imd = (Imx2duc*)objd[0];
      
      result = PBilinearInterpolation(*ims, *imm, *imd, 0, atoi(parv[1]), atoi(parv[0]));
      goto end;
   }
   if (objs[0]->Type() == Po_Imx2dsl && objs[1]->Type() == Po_Img2duc) {
      Imx2dsl* const ims = (Imx2dsl*)objs[0];
      Img2duc* const imm = (Img2duc*)objs[1];
      objd[0] = new Imx2dsl(ims->Props());
      Imx2dsl* const imd = (Imx2dsl*)objd[0];
      
      result = PBilinearInterpolation(*ims, *imm, *imd, 0, atoi(parv[1]), atoi(parv[0]));
      goto end;
   }
   if (objs[0]->Type() == Po_Imx2dsf && objs[1]->Type() == Po_Img2duc) {
      Imx2dsf* const ims = (Imx2dsf*)objs[0];
      Img2duc* const imm = (Img2duc*)objs[1];
      objd[0] = new Imx2dsf(ims->Props());
      Imx2dsf* const imd = (Imx2dsf*)objd[0];
      
      result = PBilinearInterpolation(*ims, *imm, *imd, 0, atoi(parv[1]), atoi(parv[0]));
      goto end;
   }
   if (objs[0]->Type() == Po_Img3duc && objs[1]->Type() == Po_Img3duc) {
      Img3duc* const ims = (Img3duc*)objs[0];
      Img3duc* const imm = (Img3duc*)objs[1];
      objd[0] = new Img3duc(ims->Props());
      Img3duc* const imd = (Img3duc*)objd[0];
      
      result = PBilinearInterpolation(*ims, *imm, *imd, atoi(parv[2]), atoi(parv[1]), atoi(parv[0]));
      goto end;
   }
   if (objs[0]->Type() == Po_Img3dsl && objs[1]->Type() == Po_Img3duc) {
      Img3dsl* const ims = (Img3dsl*)objs[0];
      Img3duc* const imm = (Img3duc*)objs[1];
      objd[0] = new Img3dsl(ims->Props());
      Img3dsl* const imd = (Img3dsl*)objd[0];
      
      result = PBilinearInterpolation(*ims, *imm, *imd, atoi(parv[2]), atoi(parv[1]), atoi(parv[0]));
      goto end;
   }
   if (objs[0]->Type() == Po_Img3dsf && objs[1]->Type() == Po_Img3duc) {
      Img3dsf* const ims = (Img3dsf*)objs[0];
      Img3duc* const imm = (Img3duc*)objs[1];
      objd[0] = new Img3dsf(ims->Props());
      Img3dsf* const imd = (Img3dsf*)objd[0];
      
      result = PBilinearInterpolation(*ims, *imm, *imd, atoi(parv[2]), atoi(parv[1]), atoi(parv[0]));
      goto end;
   }
   if (objs[0]->Type() == Po_Imc3duc && objs[1]->Type() == Po_Img3duc) {
      Imc3duc* const ims = (Imc3duc*)objs[0];
      Img3duc* const imm = (Img3duc*)objs[1];
      objd[0] = new Imc3duc(ims->Props());
      Imc3duc* const imd = (Imc3duc*)objd[0];
      
      result = PBilinearInterpolation(*ims, *imm, *imd, atoi(parv[2]), atoi(parv[1]), atoi(parv[0]));
      goto end;
   }
   if (objs[0]->Type() == Po_Imc3dsl && objs[1]->Type() == Po_Img3duc) {
      Imc3dsl* const ims = (Imc3dsl*)objs[0];
      Img3duc* const imm = (Img3duc*)objs[1];
      objd[0] = new Imc3dsl(ims->Props());
      Imc3dsl* const imd = (Imc3dsl*)objd[0];
      
      result = PBilinearInterpolation(*ims, *imm, *imd, atoi(parv[2]), atoi(parv[1]), atoi(parv[0]));
      goto end;
   }
   if (objs[0]->Type() == Po_Imc3dsf && objs[1]->Type() == Po_Img3duc) {
      Imc3dsf* const ims = (Imc3dsf*)objs[0];
      Img3duc* const imm = (Img3duc*)objs[1];
      objd[0] = new Imc3dsf(ims->Props());
      Imc3dsf* const imd = (Imc3dsf*)objd[0];
      
      result = PBilinearInterpolation(*ims, *imm, *imd, atoi(parv[2]), atoi(parv[1]), atoi(parv[0]));
      goto end;
   }
   if (objs[0]->Type() == Po_Imx3duc && objs[1]->Type() == Po_Img3duc) {
      Imx3duc* const ims = (Imx3duc*)objs[0];
      Img3duc* const imm = (Img3duc*)objs[1];
      objd[0] = new Imx3duc(ims->Props());
      Imx3duc* const imd = (Imx3duc*)objd[0];
      
      result = PBilinearInterpolation(*ims, *imm, *imd, atoi(parv[2]), atoi(parv[1]), atoi(parv[0]));
      goto end;
   }
   if (objs[0]->Type() == Po_Imx3dsl && objs[1]->Type() == Po_Img3duc) {
      Imx3dsl* const ims = (Imx3dsl*)objs[0];
      Img3duc* const imm = (Img3duc*)objs[1];
      objd[0] = new Imx3dsl(ims->Props());
      Imx3dsl* const imd = (Imx3dsl*)objd[0];
      
      result = PBilinearInterpolation(*ims, *imm, *imd, atoi(parv[2]), atoi(parv[1]), atoi(parv[0]));
      goto end;
   }
   if (objs[0]->Type() == Po_Imx3dsf && objs[1]->Type() == Po_Img3duc) {
      Imx3dsf* const ims = (Imx3dsf*)objs[0];
      Img3duc* const imm = (Img3duc*)objs[1];
      objd[0] = new Imx3dsf(ims->Props());
      Imx3dsf* const imd = (Imx3dsf*)objd[0];
      
      result = PBilinearInterpolation(*ims, *imm, *imd, atoi(parv[2]), atoi(parv[1]), atoi(parv[0]));
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
