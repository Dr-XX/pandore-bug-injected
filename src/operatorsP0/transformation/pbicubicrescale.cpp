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
 * @author Régis Clouard - 2006-11-20
 * @author Régis Clouard - 2007-06-28 (clip values in case of char images)
 */

#include <math.h>
#include <pandore.h>
using namespace pandore;

/**
 * @file pbicubiczoom.cpp
 * Rescales the image according to scale factor in each direction.
 * This version uses the bicubic interpolation.
 *
 * @param rescalex scale factor along X.
 * @param rescaley scale factor along Y.
 * @param rescalez scale factor along Z.
 * @return SUCCESS or FAILURE
 * imd(i,j) = ims(i*scaley,j*scalex).
 */
float cubic (float offset, float v0, float v1, float v2, float v3) {
   return ((((-7 * v0 + 21 * v1 - 21 * v2 + 7 * v3) * offset
		 + ( 15* v0 - 36 * v1 + 27 * v2 - 6 * v3)) * offset
		+ (-9 * v0 + 9 * v2) ) * offset + (v0 + 16 * v1 + v2) ) / 18.0f;
}

template <typename T>
float getCubicRow(const Imx2d<T> &ims, int b, int x, int y, float offset) {
   int x0,x1,x2,x3;
   
   if (y<0) {
      y=0;
   } else if (y>ims.Height()-1) {
      y=ims.Height()-1;
   }
   if (x<0) {
      x0=x1=0; x2=1; x3=2;
   } else {
      if (x>ims.Width()-1) {
	 x0=ims.Width()-1; 
      } else {
	 x0=x;
      }
      if (x>ims.Width()-2) {
	 x1=ims.Width()-1;
      } else {
	 x1=x+1;
      }
      if (x>ims.Width()-3) {
	 x2=ims.Width()-1;
      } else {
	 x2=x+2;
      }
      if (x>ims.Width()-4) {
	 x3=ims.Width()-1; 
      } else {
	 x3=x+3;
      }
   }
   
   float v0 =  (float)ims[b][y][x0];
   float v1 =  (float)ims[b][y][x1];
   float v2 =  (float)ims[b][y][x2];
   float v3 =  (float)ims[b][y][x3];

   return cubic(offset, v0, v1, v2, v3);
}

template <typename T>
Errc PBicubicRescale( const Imx2d<T> &ims, Imx2d<T> &imd, float scaley, float scalex ) {
   Point2d p;

   for (int b=0;b<imd.Bands();b++)
      for (p.y=0 ; p.y<imd.Height(); p.y++)
	 for (p.x=0 ; p.x<imd.Width(); p.x++) {
	    const float oldx = p.x / scalex;
	    const float oldy = p.y / scaley;
	    const float xi = floor(oldx);
	    const float yi = floor(oldy);
	    const float dx = oldx - xi;
	    const float dy = oldy - yi;

	    const float v0 = getCubicRow(ims,b,(int)xi-1,(int)yi-1,dx);
	    const float v1 = getCubicRow(ims,b,(int)xi-1,(int)yi,dx);
	    const float v2 = getCubicRow(ims,b,(int)xi-1,(int)yi+1,dx);
	    const float v3 = getCubicRow(ims,b,(int)xi-1,(int)yi+2,dx);

	    float pix = cubic(dy,v0,v1,v2,v3);
	    float min = (Float)Limits<T>::min();
	    float max = (Float)Limits<T>::max();
	    if (pix < min) {
	       pix = min;
	    } else if (pix > max) {
	       pix = max;}
	    
	    imd[b][p] = (T)pix;
	 }

   return SUCCESS;
}




const float FilterRadius = 1.5f;

static inline float Kernel( float x ) {
   const float a = -0.5f;
   if (x<0) x=-x;
   if (x <= 1.0f) return (a+2.0f)*x*x*x - (a+3.0f) *x*x + 1;
   if (x < 2.0f) return a*x*x*x - 5.0f*a*x*x + 8*a*x - 4 *a;
   return 0.0f;
}

// Include the convolution algorithm.
#include "convolution.h"

template <typename T>
inline Errc PBicubicRescale( const Imx3d<T> &ims, Imx3d<T> &imd, const float factorz, const float factory, const float factorx ) {
   return PConvolution(ims,imd,factorz,factory,factorx);
}






#ifdef MAIN
#define	USAGE	"usage: %s rescalex rescaley rescalez [im_in|-] [im_out|-]"
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
	 result=PBicubicRescale(*ims,*imd,scaley,scalex);
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
	 result=PBicubicRescale(*ims,*imd,scaley,scalex);
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
	 result=PBicubicRescale(*ims,*imd,scaley,scalex);
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
	 objd[0]=new Imc2duc(Long(ims->Height()*scaley),Long(ims->Width()*scalex));
	 Imc2duc* const imd=(Imc2duc*)objd[0];
	 imd->ColorSpace(ims->ColorSpace());
	 result=PBicubicRescale(*ims,*imd,scaley,scalex);
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
	 objd[0]=new Imc2dsl(Long(ims->Height()*scaley),Long(ims->Width()*scalex));
	 Imc2dsl* const imd=(Imc2dsl*)objd[0];
	 imd->ColorSpace(ims->ColorSpace());
	 result=PBicubicRescale(*ims,*imd,scaley,scalex);
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
	 objd[0]=new Imc2dsf(Long(ims->Height()*scaley),Long(ims->Width()*scalex));
	 Imc2dsf* const imd=(Imc2dsf*)objd[0];
	 imd->ColorSpace(ims->ColorSpace());
	 result=PBicubicRescale(*ims,*imd,scaley,scalex);
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
	 objd[0]=new Imx2duc(ims->Bands(),Long(ims->Height()*scaley),Long(ims->Width()*scalex));
	 Imx2duc* const imd=(Imx2duc*)objd[0];
	 result=PBicubicRescale(*ims,*imd,scaley,scalex);
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
	 objd[0]=new Imx2dsl(ims->Bands(),Long(ims->Height()*scaley),Long(ims->Width()*scalex));
	 Imx2dsl* const imd=(Imx2dsl*)objd[0];
	 result=PBicubicRescale(*ims,*imd,scaley,scalex);
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
	 objd[0]=new Imx2dsf(ims->Bands(),Long(ims->Height()*scaley),Long(ims->Width()*scalex));
	 Imx2dsf* const imd=(Imx2dsf*)objd[0];
	 result=PBicubicRescale(*ims,*imd,scaley,scalex);
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
	 result=PBicubicRescale(*ims,*imd, scalez,scaley, scalex);
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
	 result=PBicubicRescale(*ims,*imd, scalez,scaley, scalex);
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
	 result=PBicubicRescale(*ims,*imd, scalez,scaley, scalex);
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
	 objd[0]=new Imc3duc(ims->Bands(), Long(ims->Depth()*scalez),Long(ims->Height()*scaley),Long(ims->Width()*scalex));
	 Imc3duc* const imd=(Imc3duc*)objd[0];
	 result=PBicubicRescale(*ims,*imd, scalez, scaley, scalex);
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
	 objd[0]=new Imc3dsl(ims->Bands(), Long(ims->Depth()*scalez),Long(ims->Height()*scaley),Long(ims->Width()*scalex));
	 Imc3dsl* const imd=(Imc3dsl*)objd[0];
	 result=PBicubicRescale(*ims,*imd, scalez, scaley, scalex);
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
	 objd[0]=new Imc3dsf(ims->Bands(), Long(ims->Depth()*scalez),Long(ims->Height()*scaley),Long(ims->Width()*scalex));
	 Imc3dsf* const imd=(Imc3dsf*)objd[0];
	 result=PBicubicRescale(*ims,*imd, scalez, scaley, scalex);
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
	 objd[0]=new Imx3duc(ims->Bands(), Long(ims->Depth()*scalez),Long(ims->Height()*scaley),Long(ims->Width()*scalex));
	 Imx3duc* const imd=(Imx3duc*)objd[0];
	 result=PBicubicRescale(*ims,*imd, scalez, scaley, scalex);
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
	 objd[0]=new Imx3dsl(ims->Bands(), Long(ims->Depth()*scalez),Long(ims->Height()*scaley),Long(ims->Width()*scalex));
	 Imx3dsl* const imd=(Imx3dsl*)objd[0];
	 result=PBicubicRescale(*ims,*imd, scalez, scaley, scalex);
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
	 objd[0]=new Imx3dsf(ims->Bands(), Long(ims->Depth()*scalez),Long(ims->Height()*scaley),Long(ims->Width()*scalex));
	 Imx3dsf* const imd=(Imx3dsf*)objd[0];
	 result=PBicubicRescale(*ims,*imd, scalez, scaley, scalex);
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
