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

//--------------------------------------------------------------------------
// IMPORTANT NOTICE :
//--------------------
// The source code of this Pandore operator is governed by a specific
// Free-Software License (the CeCiLL License), also applying to the
// CImg Library. Please read it carefully, if you want to use this module
// in your own project (file CImg.h).
// IN PARTICULAR, YOU ARE NOT ALLOWED TO USE THIS PANDORE MODULE IN A
// CLOSED-SOURCE PROPRIETARY PROJECT WITHOUT ASKING AN AUTHORIZATION
// TO THE CIMG LIBRARY AUTHOR ( http://www.greyc.ensicaen.fr/~dtschump/ )
//--------------------------------------------------------------------------

/**
 * @author David Tschumperlé - 2005-08-31
 */
#include <stdio.h>
#define cimg_OS 0
#define cimg_display_type 0
#include "CImg1-16.h"
using namespace cimg_library1_16;
#include <pandore.h>
using namespace pandore;

/**
 * @file pregistrationPDE.cpp
 * @brief Estimation du champ de déplacement entre deux images.
 */

// get_warp() : Return the image src warped by the motion field u.
//------------
template<typename T> CImg<T> getwarp( const CImg<T> &src, const CImg<> &u ) {
   CImg<T> warp(src);
   cimg_mapXY(warp,x,y) warp(x,y) = (T)src.linear_pix2d(x - u(x,y,0), y - u(x,y,1));
   return warp;
}

// optmonoflow() : Register images for one scale ( semi-implicite PDE scheme ) between I2->I1
//---------------
CImg<> optmonoflow( const CImg<> &I1,const CImg<> &I2, const CImg<> &u0, 
		    const float smooth, const float precision ) {
   
   CImg<> u = u0.get_resize(I1.dimx(),I1.dimy(),1,2,3),dI(u);
   CImg_3x3(I,float);
   float dt=2,E=1e20f;
   
   // compute first derivatives of I2
   cimg_map3x3(I2,x,y,0,0,I) {
      dI(x,y,0) = 0.5f*(Inc-Ipc);
      dI(x,y,1) = 0.5f*(Icn-Icp);
   }
   
   // Main PDE iteration
   for (unsigned int iter=0; iter<100000; iter++) {
      const float Eold = E;
      E = 0;
      cimg_3mapXY(u,x,y) {
	 const float 
	    X = x + u(x,y,0),
	    Y = y + u(x,y,1),
	    deltaI = (float)(I2.linear_pix2d(X,Y) - I1(x,y));
	 float tmpf = 0;
	 cimg_mapV(u,k) {
	    const float
	       ux  = 0.5f*(u(_nx,y,k)-u(_px,y,k)),
	       uy  = 0.5f*(u(x,_ny,k)-u(x,_py,k));
	    u(x,y,k) = (float)( u(x,y,k) +
				dt*(
				    -deltaI*dI.linear_pix2d(X,Y,k) +
				    smooth* ( u(_nx,y,k) + u(_px,y,k) + u(x,_ny,k) + u(x,_py,k) )
				    )
				)/(1+4*smooth*dt);
	    tmpf += ux*ux + uy*uy;
	 }
	 E += deltaI*deltaI + smooth * tmpf;
      }
      if (cimg::abs(Eold-E)<precision) break;
      if (Eold<E) dt*=0.5;
   }
   return u;
}

// optflow() : multiscale version of the image registration algorithm
//-----------
CImg<> optflow(const CImg<>& xsrc,const CImg<>& xdest,
               const float smooth,const float precision,const unsigned int pnb_scale) {
   const CImg<>
      src  = xsrc.get_norm_pointwise(1).resize(xdest.dimx(),xdest.dimy(),1,1,3).normalize(0,1),
      dest = xdest.get_norm_pointwise(1).resize(xdest.dimx(),xdest.dimy(),1,1,3).normalize(0,1);
   CImg<> u = CImg<>(src.dimx(),src.dimy(),1,2).fill(0);
   
   const unsigned int nb_scale = pnb_scale>0?pnb_scale:(unsigned int)(2*log((double)(cimg::max(src.dimx(),src.dimy()))));
   for (int scale=nb_scale-1; scale>=0; scale--) {
      const CImg<> I1 = src.get_resize((int)(src.dimx()/pow(1.5,scale)), (int)(src.dimy()/pow(1.5,scale)) ,1,1,3);
      const CImg<> I2 = dest.get_resize((int)(src.dimx()/pow(1.5,scale)), (int)(src.dimy()/pow(1.5,scale)) ,1,1,3);
      u*=1.5;
      u = optmonoflow(I1,I2,u,smooth,(float)(precision/pow(2.25,1+scale)));
   }
   return u;
}

template<typename T>
Errc PRegistrationPDE( const Imx2d<T> &ims1, const Imx2d<T> &ims2, Imx2dsf &imd, Float smoothness=0.1,Float precision=0.7 ) {
   CImg<T> img1(ims1.Width(),ims1.Height(),1,ims1.Bands());
   cimg_mapXYV(img1,x,y,k1) img1(x,y,k1) = ims1[k1][y][x];
   CImg<T> img2(ims2.Width(),ims2.Height(),1,ims2.Bands());
   cimg_mapXYV(img2,x,y,k2) img2(x,y,k2) = ims2[k2][y][x];
   CImg<> dest = optflow(img1,img2,smoothness,precision,0); 
   cimg_mapXYV(dest,x,y,k3) imd[k3][y][x] = dest(x,y,k3);
   return SUCCESS;
}



#ifdef MAIN
#define USAGE   "usage: %s smoothness precision [im_src|-] [im_dest|-] [im_out|-]"
#define	PARC	2
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
   if (objs[0]->Type()==Po_Img2duc) {
      Img2duc* const ims1=(Img2duc*)objs[0];
      Img2duc* const ims2=(Img2duc*)objs[1];
      objd[0] = new Imx2dsf(2,ims2->Height(),ims2->Width());
      Imx2dsf *imd = (Imx2dsf*)objd[0];
      result = PRegistrationPDE(*ims1,*ims2,*imd,(Float)atof(parv[0]),(Float)atof(parv[1]));
      goto end;
   }
   if (objs[0]->Type()==Po_Img2dsl) {
      Img2dsl* const ims1=(Img2dsl*)objs[0];
      Img2dsl* const ims2=(Img2dsl*)objs[1];
      objd[0] = new Imx2dsf(2,ims2->Height(),ims2->Width());
      Imx2dsf *imd = (Imx2dsf*)objd[0];
      result = PRegistrationPDE(*ims1,*ims2,*imd,(Float)atof(parv[0]),(Float)atof(parv[1]));
      goto end;
   }
   if (objs[0]->Type()==Po_Img2dsf) {
      Img2dsf* const ims1=(Img2dsf*)objs[0];
      Img2dsf* const ims2=(Img2dsf*)objs[1];
      objd[0] = new Imx2dsf(2,ims2->Height(),ims2->Width());
      Imx2dsf *imd = (Imx2dsf*)objd[0];
      result = PRegistrationPDE(*ims1,*ims2,*imd,(Float)atof(parv[0]),(Float)atof(parv[1]));
      goto end;
   }
   if (objs[0]->Type()==Po_Imc2duc) {
      Imc2duc* const ims1=(Imc2duc*)objs[0];
      Imc2duc* const ims2=(Imc2duc*)objs[1];
      objd[0] = new Imx2dsf(2,ims2->Height(),ims2->Width());
      Imx2dsf *imd = (Imx2dsf*)objd[0];
      result = PRegistrationPDE(*ims1,*ims2,*imd,(Float)atof(parv[0]),(Float)atof(parv[1]));
      goto end;
   }
   if (objs[0]->Type()==Po_Imc2dsl) {
      Imc2dsl* const ims1=(Imc2dsl*)objs[0];
      Imc2dsl* const ims2=(Imc2dsl*)objs[1];
      objd[0] = new Imx2dsf(2,ims2->Height(),ims2->Width());
      Imx2dsf *imd = (Imx2dsf*)objd[0];
      result = PRegistrationPDE(*ims1,*ims2,*imd,(Float)atof(parv[0]),(Float)atof(parv[1]));
      goto end;
   }
   if (objs[0]->Type()==Po_Imc2dsf) {
      Imc2dsf* const ims1=(Imc2dsf*)objs[0];
      Imc2dsf* const ims2=(Imc2dsf*)objs[1];
      objd[0] = new Imx2dsf(2,ims2->Height(),ims2->Width());
      Imx2dsf *imd = (Imx2dsf*)objd[0];
      result = PRegistrationPDE(*ims1,*ims2,*imd,(Float)atof(parv[0]),(Float)atof(parv[1]));
      goto end;
   }
   if (objs[0]->Type()==Po_Imx2duc) {
      Imx2duc* const ims1=(Imx2duc*)objs[0];
      Imx2duc* const ims2=(Imx2duc*)objs[1];
      objd[0] = new Imx2dsf(2,ims2->Height(),ims2->Width());
      Imx2dsf *imd = (Imx2dsf*)objd[0];
      result = PRegistrationPDE(*ims1,*ims2,*imd,(Float)atof(parv[0]),(Float)atof(parv[1]));
      goto end;
   }
   if (objs[0]->Type()==Po_Imx2dsl) {
      Imx2dsl* const ims1=(Imx2dsl*)objs[0];
      Imx2dsl* const ims2=(Imx2dsl*)objs[1];
      objd[0] = new Imx2dsf(2,ims2->Height(),ims2->Width());
      Imx2dsf *imd = (Imx2dsf*)objd[0];
      result = PRegistrationPDE(*ims1,*ims2,*imd,(Float)atof(parv[0]),(Float)atof(parv[1]));
      goto end;
   }
   if (objs[0]->Type()==Po_Imx2dsf) {
      Imx2dsf* const ims1=(Imx2dsf*)objs[0];
      Imx2dsf* const ims2=(Imx2dsf*)objs[1];
      objd[0] = new Imx2dsf(2,ims2->Height(),ims2->Width());
      Imx2dsf *imd = (Imx2dsf*)objd[0];
      result = PRegistrationPDE(*ims1,*ims2,*imd,(Float)atof(parv[0]),(Float)atof(parv[1]));
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

