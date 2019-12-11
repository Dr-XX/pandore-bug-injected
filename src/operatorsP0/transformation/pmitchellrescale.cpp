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
 * @author Régis Clouard - Feb 12, 2011
 */

#include <pandore.h>
using namespace pandore;

/**
 * @file pmitchellrescale.cpp
 *
 * Rescales the image according to Mitchell interpolation
 * algorithm.
 * This implementation uses convolution between the image
 * and a kernel.
 */

#include <cmath>

const int FilterRadius = 2;

static inline float Kernel( float x ) {
   if (x <= -2.0 || x >= 2.0) return 0.0;
   if (x<0) x=-x;
   const float B = 1.0f/3.0f;
   const float C = 1.0f/3.0f;

   const float tt=x*x; 
   if (x < 1.0) {
      return (((12 - 9*B - 6*C) * (x*tt))
	   + ((-18 + 12*B + 6*C) * tt)
	   + (6 -2*B)) / 6.0f;
   }
   return (((-1*B - 6*C) * (x*tt)) 
	   + ((6*B  + 30*C) *tt) 
	   + ((-12*B-48*C)*x)
	   + (8*B+24*C)) / 6.0f;
}

// Include the convolution algorithm.
#include "convolution.h"
template <typename T>
inline Errc PMitchellRescale( const Imx2d<T> &ims, Imx2d<T> &imd, const float factory, const float factorx ) {
   return PConvolution(ims,imd,factory,factorx);
}



template <typename T>
inline Errc PMitchellRescale( const Imx3d<T> &ims, Imx3d<T> &imd, const float factorz, const float factory, const float factorx ) {
   return PConvolution(ims,imd,factorz,factory,factorx);
}





#ifdef MAIN
#define	USAGE	"usage: %s zoomx zoomy zoomz [im_in|-] [im_out|-]"
#define	PARC	3
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
      const float scalex = (float)atof(parv[0]);
      const float scaley = (float)atof(parv[1]);
      if ( scalex<=0 || scaley<=0)
	 result=FAILURE;
      else {
	 Img2duc* const ims=(Img2duc*)objs[0];
	 objd[0]=new Img2duc(Long(ims->Height()*scaley),Long(ims->Width()*scalex));
	 Img2duc* const imd=(Img2duc*)objd[0];
	 result=PMitchellRescale(*ims,*imd, scaley, scalex);
      }
      goto end;
   }
   if (objs[0]->Type() == Po_Img2dsl) {
      const float scalex = (float)atof(parv[0]);
      const float scaley = (float)atof(parv[1]);
      if ( scalex<=0 || scaley<=0)
	 result=FAILURE;
      else {
	 Img2dsl* const ims=(Img2dsl*)objs[0];
	 objd[0]=new Img2dsl(Long(ims->Height()*scaley),Long(ims->Width()*scalex));
	 Img2dsl* const imd=(Img2dsl*)objd[0];
	 result=PMitchellRescale(*ims,*imd, scaley, scalex);
      }
      goto end;
   }
   if (objs[0]->Type() == Po_Img2dsf) {
      const float scalex = (float)atof(parv[0]);
      const float scaley = (float)atof(parv[1]);
      if ( scalex<=0 || scaley<=0)
	 result=FAILURE;
      else {
	 Img2dsf* const ims=(Img2dsf*)objs[0];
	 objd[0]=new Img2dsf(Long(ims->Height()*scaley),Long(ims->Width()*scalex));
	 Img2dsf* const imd=(Img2dsf*)objd[0];
	 result=PMitchellRescale(*ims,*imd, scaley, scalex);
      }
      goto end;
   }
   if (objs[0]->Type() == Po_Imc2duc) {
      const float scalex = (float)atof(parv[0]);
      const float scaley = (float)atof(parv[1]);
      if ( scalex<=0 || scaley<=0)
	 result=FAILURE;
      else {
	 Imc2duc* const ims=(Imc2duc*)objs[0];
	 objd[0]=new Imc2duc(ims->Bands(), Long(ims->Height()*scaley),Long(ims->Width()*scalex));
	 Imc2duc* const imd=(Imc2duc*)objd[0];
	 result=PMitchellRescale(*ims,*imd, scaley, scalex);
      }
      goto end;
   }
   if (objs[0]->Type() == Po_Imc2dsl) {
      const float scalex = (float)atof(parv[0]);
      const float scaley = (float)atof(parv[1]);
      if ( scalex<=0 || scaley<=0)
	 result=FAILURE;
      else {
	 Imc2dsl* const ims=(Imc2dsl*)objs[0];
	 objd[0]=new Imc2dsl(ims->Bands(), Long(ims->Height()*scaley),Long(ims->Width()*scalex));
	 Imc2dsl* const imd=(Imc2dsl*)objd[0];
	 result=PMitchellRescale(*ims,*imd, scaley, scalex);
      }
      goto end;
   }
   if (objs[0]->Type() == Po_Imc2dsf) {
      const float scalex = (float)atof(parv[0]);
      const float scaley = (float)atof(parv[1]);
      if ( scalex<=0 || scaley<=0)
	 result=FAILURE;
      else {
	 Imc2dsf* const ims=(Imc2dsf*)objs[0];
	 objd[0]=new Imc2dsf(ims->Bands(), Long(ims->Height()*scaley),Long(ims->Width()*scalex));
	 Imc2dsf* const imd=(Imc2dsf*)objd[0];
	 result=PMitchellRescale(*ims,*imd, scaley, scalex);
      }
      goto end;
   }
   if (objs[0]->Type() == Po_Imx2duc) {
      const float scalex = (float)atof(parv[0]);
      const float scaley = (float)atof(parv[1]);
      if ( scalex<=0 || scaley<=0)
	 result=FAILURE;
      else {
	 Imx2duc* const ims=(Imx2duc*)objs[0];
	 objd[0]=new Imx2duc(ims->Bands(), Long(ims->Height()*scaley),Long(ims->Width()*scalex));
	 Imx2duc* const imd=(Imx2duc*)objd[0];
	 result=PMitchellRescale(*ims,*imd, scaley, scalex);
      }
      goto end;
   }
   if (objs[0]->Type() == Po_Imx2dsl) {
      const float scalex = (float)atof(parv[0]);
      const float scaley = (float)atof(parv[1]);
      if ( scalex<=0 || scaley<=0)
	 result=FAILURE;
      else {
	 Imx2dsl* const ims=(Imx2dsl*)objs[0];
	 objd[0]=new Imx2dsl(ims->Bands(), Long(ims->Height()*scaley),Long(ims->Width()*scalex));
	 Imx2dsl* const imd=(Imx2dsl*)objd[0];
	 result=PMitchellRescale(*ims,*imd, scaley, scalex);
      }
      goto end;
   }
   if (objs[0]->Type() == Po_Imx2dsf) {
      const float scalex = (float)atof(parv[0]);
      const float scaley = (float)atof(parv[1]);
      if ( scalex<=0 || scaley<=0)
	 result=FAILURE;
      else {
	 Imx2dsf* const ims=(Imx2dsf*)objs[0];
	 objd[0]=new Imx2dsf(ims->Bands(), Long(ims->Height()*scaley),Long(ims->Width()*scalex));
	 Imx2dsf* const imd=(Imx2dsf*)objd[0];
	 result=PMitchellRescale(*ims,*imd, scaley, scalex);
      }
      goto end;
   }
   if (objs[0]->Type() == Po_Img3duc) {
      const float scalex = (float)atof(parv[0]);
      const float scaley = (float)atof(parv[1]);
      const float scalez = (float)atof(parv[2]);
      if ( scalex<=0 || scaley<=0 || scalez<=0) {
	 result=FAILURE;
      } else {
	 Img3duc* const ims=(Img3duc*)objs[0];
	 objd[0]=new Img3duc(Long(ims->Depth()*scalez),Long(ims->Height()*scaley),Long(ims->Width()*scalex));
	 Img3duc* const imd=(Img3duc*)objd[0];
	 result=PMitchellRescale(*ims,*imd,scalez,scaley,scalex);
      }
      goto end;
   }
   if (objs[0]->Type() == Po_Img3dsl) {
      const float scalex = (float)atof(parv[0]);
      const float scaley = (float)atof(parv[1]);
      const float scalez = (float)atof(parv[2]);
      if ( scalex<=0 || scaley<=0 || scalez<=0) {
	 result=FAILURE;
      } else {
	 Img3dsl* const ims=(Img3dsl*)objs[0];
	 objd[0]=new Img3dsl(Long(ims->Depth()*scalez),Long(ims->Height()*scaley),Long(ims->Width()*scalex));
	 Img3dsl* const imd=(Img3dsl*)objd[0];
	 result=PMitchellRescale(*ims,*imd,scalez,scaley,scalex);
      }
      goto end;
   }
   if (objs[0]->Type() == Po_Img3dsf) {
      const float scalex = (float)atof(parv[0]);
      const float scaley = (float)atof(parv[1]);
      const float scalez = (float)atof(parv[2]);
      if ( scalex<=0 || scaley<=0 || scalez<=0) {
	 result=FAILURE;
      } else {
	 Img3dsf* const ims=(Img3dsf*)objs[0];
	 objd[0]=new Img3dsf(Long(ims->Depth()*scalez),Long(ims->Height()*scaley),Long(ims->Width()*scalex));
	 Img3dsf* const imd=(Img3dsf*)objd[0];
	 result=PMitchellRescale(*ims,*imd,scalez,scaley,scalex);
      }
      goto end;
   }
   if (objs[0]->Type() == Po_Imc3duc) {
      const float scalex = (float)atof(parv[0]);
      const float scaley = (float)atof(parv[1]);
      const float scalez = (float)atof(parv[2]);
      if ( scalex<=0 || scaley<=0 || scalez<=0) {
	 result=FAILURE;
      } else {
	 Imc3duc* const ims=(Imc3duc*)objs[0];
	 objd[0]=new Imc3duc(ims->Bands(),Long(ims->Depth()*scalez),Long(ims->Height()*scaley), Long(ims->Width()*scalex));
	 Imc3duc* const imd=(Imc3duc*)objd[0];
	 result=PMitchellRescale(*ims,*imd,scalez,scaley,scalex);
      }
      goto end;
   }
   if (objs[0]->Type() == Po_Imc3dsl) {
      const float scalex = (float)atof(parv[0]);
      const float scaley = (float)atof(parv[1]);
      const float scalez = (float)atof(parv[2]);
      if ( scalex<=0 || scaley<=0 || scalez<=0) {
	 result=FAILURE;
      } else {
	 Imc3dsl* const ims=(Imc3dsl*)objs[0];
	 objd[0]=new Imc3dsl(ims->Bands(),Long(ims->Depth()*scalez),Long(ims->Height()*scaley), Long(ims->Width()*scalex));
	 Imc3dsl* const imd=(Imc3dsl*)objd[0];
	 result=PMitchellRescale(*ims,*imd,scalez,scaley,scalex);
      }
      goto end;
   }
   if (objs[0]->Type() == Po_Imc3dsf) {
      const float scalex = (float)atof(parv[0]);
      const float scaley = (float)atof(parv[1]);
      const float scalez = (float)atof(parv[2]);
      if ( scalex<=0 || scaley<=0 || scalez<=0) {
	 result=FAILURE;
      } else {
	 Imc3dsf* const ims=(Imc3dsf*)objs[0];
	 objd[0]=new Imc3dsf(ims->Bands(),Long(ims->Depth()*scalez),Long(ims->Height()*scaley), Long(ims->Width()*scalex));
	 Imc3dsf* const imd=(Imc3dsf*)objd[0];
	 result=PMitchellRescale(*ims,*imd,scalez,scaley,scalex);
      }
      goto end;
   }
   if (objs[0]->Type() == Po_Imx3duc) {
      const float scalex = (float)atof(parv[0]);
      const float scaley = (float)atof(parv[1]);
      const float scalez = (float)atof(parv[2]);
      if ( scalex<=0 || scaley<=0 || scalez<=0) {
	 result=FAILURE;
      } else {
	 Imx3duc* const ims=(Imx3duc*)objs[0];
	 objd[0]=new Imx3duc(ims->Bands(),Long(ims->Depth()*scalez),Long(ims->Height()*scaley), Long(ims->Width()*scalex));
	 Imx3duc* const imd=(Imx3duc*)objd[0];
	 result=PMitchellRescale(*ims,*imd,scalez,scaley,scalex);
      }
      goto end;
   }
   if (objs[0]->Type() == Po_Imx3dsl) {
      const float scalex = (float)atof(parv[0]);
      const float scaley = (float)atof(parv[1]);
      const float scalez = (float)atof(parv[2]);
      if ( scalex<=0 || scaley<=0 || scalez<=0) {
	 result=FAILURE;
      } else {
	 Imx3dsl* const ims=(Imx3dsl*)objs[0];
	 objd[0]=new Imx3dsl(ims->Bands(),Long(ims->Depth()*scalez),Long(ims->Height()*scaley), Long(ims->Width()*scalex));
	 Imx3dsl* const imd=(Imx3dsl*)objd[0];
	 result=PMitchellRescale(*ims,*imd,scalez,scaley,scalex);
      }
      goto end;
   }
   if (objs[0]->Type() == Po_Imx3dsf) {
      const float scalex = (float)atof(parv[0]);
      const float scaley = (float)atof(parv[1]);
      const float scalez = (float)atof(parv[2]);
      if ( scalex<=0 || scaley<=0 || scalez<=0) {
	 result=FAILURE;
      } else {
	 Imx3dsf* const ims=(Imx3dsf*)objs[0];
	 objd[0]=new Imx3dsf(ims->Bands(),Long(ims->Depth()*scalez),Long(ims->Height()*scaley), Long(ims->Width()*scalex));
	 Imx3dsf* const imd=(Imx3dsf*)objd[0];
	 result=PMitchellRescale(*ims,*imd,scalez,scaley,scalex);
      }
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
 
