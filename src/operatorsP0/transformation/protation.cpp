/* -*- c-basic-offset: 3 ; mode: c++ -*-
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
 * @author Régis Clouard - 2001-05-30
 * @author Régis Clouard - 2007-01-26 (use bilinear interpolation).
 */

#define _USE_MATH_DEFINES
#include <math.h>
#include <pandore.h>
using namespace pandore;

/**
 * @file protation.cpp
 *
 * Rotates the content of the image without
 * changing the size of the image.
 * @param angle specifies the angle in degrees (can be negative).
 */
template <typename T>
Errc PRotation( const Imx2d<T> &ims, Imx2d<T> &imd, int axis, const float angle ) {
   Point2d p;
   int b;

   if (axis < 0 || axis > 2) {
      ::std::cerr << "Error protation: bad parameter values: " << axis << ", "<<angle << ::std::endl;
      return FAILURE;
   }

   // precompute the cosine and sine values used to speed things up.
   const float cosas = (float)cos((angle*M_PI)/180);
   const float sinas = (float)sin((angle*M_PI)/180);

   // initialization
   for (b = 0; b < ims.Bands(); b++) {
      for (p.x = 0; p.x < imd.Width(); p.x++) {
	 imd[b][imd.Height()-1][p.x] = 0;
      }
      for (p.y = 0; p.y < imd.Height(); p.y++) {
	 imd[b][p.y][imd.Width()-1] = 0;
      }
   }

   for (b = 0; b < ims.Bands(); b++) {
      for (p.y = 0; p.y < imd.Height()-1; p.y++) {
	 for (p.x = 0; p.x < imd.Width()-1; p.x++) {	 
	    // compute the center of the translation image.
	    const float xc = (float)(p.x-ims.Width()/2.0F);
	    const float yc = (float)(p.y-ims.Height()/2.0F);
	    
	    // compute the related tx ty coordinates in ims.
	    const float oldx = ( (xc * cosas) - (yc * sinas) ) + (float)ims.Width()/2;
	    const float oldy = ( (xc * sinas) + (yc * cosas) ) + (float)ims.Height()/2;
	    const int sx = (int)oldx;
	    const int sy = (int)oldy;
	    const float dx = oldx - sx;
	    const float dy = oldy - sy;
	    
	    //clip to black any pixels that fall off of the image.
  	    if (sx>0 && sy>0 && sx<ims.Width()-1 && sy<ims.Height()-1) {
	       imd[b][p] = (T)((1.0f-dx) * (1.0f-dy) * ims[b][sy][sx]
			       + (1.0f-dx)*dy * ims[b][sy+1][sx]
			       + dx * (1.0f-dy) * ims[b][sy][sx+1]
			       + dx * dy * ims[b][sy+1][sx+1]);
 	    } else {
 	       imd[b][p] = 0;
	    }
	 }
      }
   }
   return SUCCESS;
}





template <typename T>
Errc PRotation( const Imx3d<T> &ims, Imx3d<T> &imd, const int axis, const float angle ) {
   Point3d p;
   int b;

   if (axis <0 || axis > 2) {
      ::std::cerr << "Error protation: bad parameter values: " << axis << ", "<<angle<<::std::endl;
      return FAILURE;
   }

   //precompute the cosine and sine values used to speed things up
   const float cosas = (float)cos((angle*M_PI)/180);
   const float sinas = (float)sin((angle*M_PI)/180);

   if (axis == 0) {
      for (b=0; b<imd.Bands(); b++) {
	 for (p.z=0; p.z<imd.Depth(); p.z++) {
	    for (p.y=0; p.y<imd.Height(); p.y++) {
	       for (p.x=0; p.x<imd.Width(); p.x++) {
		  
		  //compute the center of the translation image.
		  const float xc = (float)(p.x-ims.Width()/2);
		  const float yc = (float)(p.y-ims.Height()/2);
		  
		  //compute the related tx ty coordinates in ims.
		  const float oldx = ( (xc * cosas) - (yc * sinas) ) + (float)ims.Width()/2;
		  const float oldy = ( (xc * sinas) + (yc * cosas) ) + (float)ims.Height()/2;
		  const int sx = (int)oldx;
		  const int sy = (int)oldy;
		  const float dx = oldx - sx;
		  const float dy = oldy - sy;
		  
		  //clip to black any voxels that fall off of the image.
		  if (sx>0 && sy>0 && sx<ims.Width()-1 && sy<ims.Height()-1) {
		     imd[b][p] = (T)((1.0f-dx) * (1.0f-dy) * ims[b][p.z][sy][sx]
				     + (1.0f-dx)*dy * ims[b][p.z][sy+1][sx]
				     + dx * (1.0f-dy) * ims[b][p.z][sy][sx+1]
				     + dx * dy * ims[b][p.z][sy+1][sx+1]);
		  } else {
		     imd[b][p] = 0;
		  }
	       }
	    }
	 }
      }
   } else if (axis==1) {
      for (b=0; b<imd.Bands(); b++) {
	 for (p.z=0; p.z<imd.Depth(); p.z++) {
	    for (p.y=0; p.y<imd.Height(); p.y++) {
	       for (p.x=0; p.x<imd.Width(); p.x++) {
		  
		  //compute the center of the translation image.
		  const float xc = (float)(p.x-ims.Width()/2);
		  const float zc = (float)(p.z-ims.Depth()/2);
		  
		  //compute the related tx ty coordinates in ims.
		  const float oldx = ( (xc * cosas) - (zc * sinas) ) + (float)ims.Width()/2;
		  const float oldz = ( (xc * sinas) + (zc * cosas) ) + (float)ims.Depth()/2;
		  const int sx = (int)oldx;
		  const int sz = (int)oldz;
		  const float dx = oldx - sx;
		  const float dz = oldz - sz;
		  
		  //clip to black any voxels that fall off of the image.
		  if (sx>0 && sz>0 && sx<ims.Width()-1 && sz<ims.Depth()-1) {
		     imd[b][p] = (T)((1.0f-dx) * (1.0f-dz) * ims[b][sz][p.y][sx]
				     + (1.0f-dx)*dz * ims[b][sz+1][p.y][sx]
				     + dx * (1.0f-dz) * ims[b][sz][p.y][sx+1]
				     + dx * dz * ims[b][sz+1][p.y][sx+1]);
		  } else {
		     imd[b][p] = 0;
		  }
	       }
	    }
	 }
      }
   } else if (axis==2) {
      for (b=0; b<imd.Bands(); b++) {
	 for (p.z=0; p.z<imd.Depth(); p.z++) {
	    for (p.y=0; p.y<imd.Height(); p.y++)
	       for (p.x=0; p.x<imd.Width(); p.x++) {
		  
		  //compute the center of the translation image.
		  const float yc = (float)(p.y-ims.Height() / 2);
		  const float zc = (float)(p.z-ims.Depth() / 2);
		  
		  //compute the related tx ty coordinates in ims.
		  const float oldy = ( (yc * cosas) - (zc * sinas) ) + (float)ims.Height()/2;
		  const float oldz = ( (zc * sinas) + (zc * cosas) ) + (float)ims.Depth()/2;
		  const int sy = (int)oldy;
		  const int sz = (int)oldz;
		  const float dy = oldy - sy;
		  const float dz = oldz - sz;
		  
		  //clip to black any voxels that fall off of the image.
		  if (sy>0 && sz>0 && sy<ims.Height()-1 && sz<ims.Depth()-1) {
		     imd[b][p] = (T)((1.0f-dy) * (1.0f-dz) * ims[b][sz][sy][p.x]
				     + (1.0f-dy)*dz * ims[b][sz+1][sy][p.x]
				     + dy * (1.0f-dz) * ims[b][sz][sy+1][p.x]
				     + dy * dz * ims[b][sz+1][sy+1][p.x]);
		  } else {
		     imd[b][p] = 0;
		  }
	       }
	 }
      }
   }
   
   return SUCCESS;
}









#ifdef MAIN
#define	USAGE	"usage: %s axis angle [-m mask] [im_in|-] [im_out|-]"
#define	PARC	2
#define	FINC	1
#define	FOUTC	1
#define	MASK	1

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
      objd[0] = new Img2duc(ims->Props());
      Img2duc* const imd = (Img2duc*)objd[0];
      
      result = PRotation(*ims, *imd, atoi(parv[0]), (float)atof(parv[1]));
      goto end;
   }
   if (objs[0]->Type() == Po_Img2dsl) {
      Img2dsl* const ims = (Img2dsl*)objs[0];
      objd[0] = new Img2dsl(ims->Props());
      Img2dsl* const imd = (Img2dsl*)objd[0];
      
      result = PRotation(*ims, *imd, atoi(parv[0]), (float)atof(parv[1]));
      goto end;
   }
   if (objs[0]->Type() == Po_Img2dsf) {
      Img2dsf* const ims = (Img2dsf*)objs[0];
      objd[0] = new Img2dsf(ims->Props());
      Img2dsf* const imd = (Img2dsf*)objd[0];
      
      result = PRotation(*ims, *imd, atoi(parv[0]), (float)atof(parv[1]));
      goto end;
   }
   if (objs[0]->Type() == Po_Img3duc) {
      Img3duc* const ims = (Img3duc*)objs[0];
      objd[0] = new Img3duc(ims->Props());
      Img3duc* const imd = (Img3duc*)objd[0];
      
      result = PRotation(*ims, *imd, atoi(parv[0]), (float)atof(parv[1]));
      goto end;
   }
   if (objs[0]->Type() == Po_Img3dsl) {
      Img3dsl* const ims = (Img3dsl*)objs[0];
      objd[0] = new Img3dsl(ims->Props());
      Img3dsl* const imd = (Img3dsl*)objd[0];
      
      result = PRotation(*ims, *imd, atoi(parv[0]), (float)atof(parv[1]));
      goto end;
   }
   if (objs[0]->Type() == Po_Img3dsf) {
      Img3dsf* const ims = (Img3dsf*)objs[0];
      objd[0] = new Img3dsf(ims->Props());
      Img3dsf* const imd = (Img3dsf*)objd[0];
      
      result = PRotation(*ims, *imd, atoi(parv[0]), (float)atof(parv[1]));
      goto end;
   }
   if (objs[0]->Type() == Po_Imc2duc) {
      Imc2duc* const ims = (Imc2duc*)objs[0];
      objd[0] = new Imc2duc(ims->Props());
      Imc2duc* const imd = (Imc2duc*)objd[0];
      
      result = PRotation(*ims, *imd, atoi(parv[0]), (float)atof(parv[1]));
      goto end;
   }
   if (objs[0]->Type() == Po_Imc2dsl) {
      Imc2dsl* const ims = (Imc2dsl*)objs[0];
      objd[0] = new Imc2dsl(ims->Props());
      Imc2dsl* const imd = (Imc2dsl*)objd[0];
      
      result = PRotation(*ims, *imd, atoi(parv[0]), (float)atof(parv[1]));
      goto end;
   }
   if (objs[0]->Type() == Po_Imc2dsf) {
      Imc2dsf* const ims = (Imc2dsf*)objs[0];
      objd[0] = new Imc2dsf(ims->Props());
      Imc2dsf* const imd = (Imc2dsf*)objd[0];
      
      result = PRotation(*ims, *imd, atoi(parv[0]), (float)atof(parv[1]));
      goto end;
   }
   if (objs[0]->Type() == Po_Imc3duc) {
      Imc3duc* const ims = (Imc3duc*)objs[0];
      objd[0] = new Imc3duc(ims->Props());
      Imc3duc* const imd = (Imc3duc*)objd[0];
      
      result = PRotation(*ims, *imd, atoi(parv[0]), (float)atof(parv[1]));
      goto end;
   }
   if (objs[0]->Type() == Po_Imc3dsl) {
      Imc3dsl* const ims = (Imc3dsl*)objs[0];
      objd[0] = new Imc3dsl(ims->Props());
      Imc3dsl* const imd = (Imc3dsl*)objd[0];
      
      result = PRotation(*ims, *imd, atoi(parv[0]), (float)atof(parv[1]));
      goto end;
   }
   if (objs[0]->Type() == Po_Imc3dsf) {
      Imc3dsf* const ims = (Imc3dsf*)objs[0];
      objd[0] = new Imc3dsf(ims->Props());
      Imc3dsf* const imd = (Imc3dsf*)objd[0];
      
      result = PRotation(*ims, *imd, atoi(parv[0]), (float)atof(parv[1]));
      goto end;
   }
   if (objs[0]->Type() == Po_Imx2duc) {
      Imx2duc* const ims = (Imx2duc*)objs[0];
      objd[0] = new Imx2duc(ims->Props());
      Imx2duc* const imd = (Imx2duc*)objd[0];
      
      result = PRotation(*ims, *imd, atoi(parv[0]), (float)atof(parv[1]));
      goto end;
   }
   if (objs[0]->Type() == Po_Imx2dsl) {
      Imx2dsl* const ims = (Imx2dsl*)objs[0];
      objd[0] = new Imx2dsl(ims->Props());
      Imx2dsl* const imd = (Imx2dsl*)objd[0];
      
      result = PRotation(*ims, *imd, atoi(parv[0]), (float)atof(parv[1]));
      goto end;
   }
   if (objs[0]->Type() == Po_Imx2dsf) {
      Imx2dsf* const ims = (Imx2dsf*)objs[0];
      objd[0] = new Imx2dsf(ims->Props());
      Imx2dsf* const imd = (Imx2dsf*)objd[0];
      
      result = PRotation(*ims, *imd, atoi(parv[0]), (float)atof(parv[1]));
      goto end;
   }
   if (objs[0]->Type() == Po_Imx3duc) {
      Imx3duc* const ims = (Imx3duc*)objs[0];
      objd[0] = new Imx3duc(ims->Props());
      Imx3duc* const imd = (Imx3duc*)objd[0];
      
      result = PRotation(*ims, *imd, atoi(parv[0]), (float)atof(parv[1]));
      goto end;
   }
   if (objs[0]->Type() == Po_Imx3dsl) {
      Imx3dsl* const ims = (Imx3dsl*)objs[0];
      objd[0] = new Imx3dsl(ims->Props());
      Imx3dsl* const imd = (Imx3dsl*)objd[0];
      
      result = PRotation(*ims, *imd, atoi(parv[0]), (float)atof(parv[1]));
      goto end;
   }
   if (objs[0]->Type() == Po_Imx3dsf) {
      Imx3dsf* const ims = (Imx3dsf*)objs[0];
      objd[0] = new Imx3dsf(ims->Props());
      Imx3dsf* const imd = (Imx3dsf*)objd[0];
      
      result = PRotation(*ims, *imd, atoi(parv[0]), (float)atof(parv[1]));
      goto end;
   }
   if (objs[0]->Type() == Po_Reg2d) {
      Reg2d* const rgs = (Reg2d*)objs[0];
      objd[0] = new Reg2d(rgs->Props());
      Reg2d* const rgd = (Reg2d*)objd[0];
      
      result = PRotation(*rgs, *rgd, atoi(parv[0]), (float)atof(parv[1]));
      goto end;
   }
   if (objs[0]->Type() == Po_Reg3d) {
      Reg3d* const rgs = (Reg3d*)objs[0];
      objd[0] = new Reg3d(rgs->Props());
      Reg3d* const rgd = (Reg3d*)objd[0];
      
      result = PRotation(*rgs, *rgd, atoi(parv[0]), (float)atof(parv[1]));
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
