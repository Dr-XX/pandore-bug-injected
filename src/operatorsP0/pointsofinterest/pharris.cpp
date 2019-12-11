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
 * @file pharris.cpp
 *
 * Harris corner detector.
 *
 * Reference: C.G. Harris and M.J. Stephens. "A combined corner and edge detector", 
 * Proceedings Fourth Alvey Vision Conference, Manchester, pp 147-151, 1988.
 *
 * For each pixel, use its neigborhood (7x7) to form
 * the followinf matrix:
 *    | Ixx Ixy |    | A  C|
 *  c=|         | = |     |
 *    | Ixy Iyy |    | C  B|
 * where Ixx Iyy Ixy are image gradient components.
 * Response = A.B-C.C - kappa.(A+B)
 * Remarks : to make the result robust, use Gaussian filter before.
 * Harris suggests that kappa=0.04.
 */

#ifdef MAIN
# undef MAIN
# include "filtering/pgaussianfiltering.cpp"
# define MAIN
#else
# include "filtering/pgaussianfiltering.cpp"
#endif



Errc PHarris( const Img2duc &ims, Img2dsf &imd, float sigma, float kappa ) {
   float gx, gy;
   int x, y, k, l;
   int radius;
   
   radius = (int)(3.0 * sigma);
   
   if (radius < 1) {
      std::cerr << "Error pharris: bad value for sigma." << std::endl;
      return FAILURE;
   }

   Img2dsf Ixx(ims.Props());
   Img2dsf Iyy(ims.Props());
   Img2dsf Ixy(ims.Props());
   
   // Computes x, y gradient images.
   Ixx.Frame(0, 1);
   Iyy.Frame(0, 1);
   Ixy.Frame(0, 1);
   for (y = 1; y < ims.Height() - 1; y++) {
      for (x = 1; x < ims.Width() - 1; x++) {
	 gx = (Float(ims[y][x + 1]) - Float(ims[y][x - 1]));
	 gy = (Float(ims[y + 1][x])-Float(ims[y - 1][x]));
	 Ixx[y][x] = gx * gx;
	 Iyy[y][x] = gy * gy;
	 Ixy[y][x] = gx * gy;
      }
   }
   
   // Smooth the squared gradient images.
   if (PGaussianFiltering(Ixx, Ixx, sigma) == FAILURE) {
      return FAILURE;
   }
   if (PGaussianFiltering(Iyy, Iyy, sigma) == FAILURE) {
      return FAILURE;
   }
   if (PGaussianFiltering(Ixy, Ixy, sigma) == FAILURE) {
      return FAILURE;
   }
   
   // Compute the Harris corner measure.
   for (y = 0;y < ims.Height(); y++) {
      for (x = 0;x < ims.Width(); x++) {
	 imd[y][x] = (Ixx[y][x]*Iyy[y][x] - Ixy[y][x]*Ixy[y][x]) - kappa*((Ixx[y][x]+Iyy[y][x])*(Ixx[y][x]+Iyy[y][x]));
      }
   }
   // Apply non-maximum suppression of corner strengths to find the corners.
   for (y = 0; y < ims.Height(); y++) {
      for (x = 0; x < ims.Width(); x++) {
	 for (l = -radius;  l < radius; l++) {
	    for (k = -radius; k < radius; k++) {
	       if ((l != 0 || k != 0) && imd.Hold(y + l, x + k) && imd[y + l][x + k] >= imd[y][x]) {
		  imd[y][x] = 0;
	       }
	    }
	 }
      }
   }
   return SUCCESS;
}

Errc PHarris( const Img2dsl &ims, Img2dsf &imd, float sigma, float kappa ) {
   float gx, gy;
   int x, y, k, l;
   int radius;
   
   radius = (int)(3.0 * sigma);
   
   if (radius < 1) {
      std::cerr << "Error pharris: bad value for sigma." << std::endl;
      return FAILURE;
   }

   Img2dsf Ixx(ims.Props());
   Img2dsf Iyy(ims.Props());
   Img2dsf Ixy(ims.Props());
   
   // Computes x, y gradient images.
   Ixx.Frame(0, 1);
   Iyy.Frame(0, 1);
   Ixy.Frame(0, 1);
   for (y = 1; y < ims.Height() - 1; y++) {
      for (x = 1; x < ims.Width() - 1; x++) {
	 gx = (Float(ims[y][x + 1]) - Float(ims[y][x - 1]));
	 gy = (Float(ims[y + 1][x])-Float(ims[y - 1][x]));
	 Ixx[y][x] = gx * gx;
	 Iyy[y][x] = gy * gy;
	 Ixy[y][x] = gx * gy;
      }
   }
   
   // Smooth the squared gradient images.
   if (PGaussianFiltering(Ixx, Ixx, sigma) == FAILURE) {
      return FAILURE;
   }
   if (PGaussianFiltering(Iyy, Iyy, sigma) == FAILURE) {
      return FAILURE;
   }
   if (PGaussianFiltering(Ixy, Ixy, sigma) == FAILURE) {
      return FAILURE;
   }
   
   // Compute the Harris corner measure.
   for (y = 0;y < ims.Height(); y++) {
      for (x = 0;x < ims.Width(); x++) {
	 imd[y][x] = (Ixx[y][x]*Iyy[y][x] - Ixy[y][x]*Ixy[y][x]) - kappa*((Ixx[y][x]+Iyy[y][x])*(Ixx[y][x]+Iyy[y][x]));
      }
   }
   // Apply non-maximum suppression of corner strengths to find the corners.
   for (y = 0; y < ims.Height(); y++) {
      for (x = 0; x < ims.Width(); x++) {
	 for (l = -radius;  l < radius; l++) {
	    for (k = -radius; k < radius; k++) {
	       if ((l != 0 || k != 0) && imd.Hold(y + l, x + k) && imd[y + l][x + k] >= imd[y][x]) {
		  imd[y][x] = 0;
	       }
	    }
	 }
      }
   }
   return SUCCESS;
}

Errc PHarris( const Img2dsf &ims, Img2dsf &imd, float sigma, float kappa ) {
   float gx, gy;
   int x, y, k, l;
   int radius;
   
   radius = (int)(3.0 * sigma);
   
   if (radius < 1) {
      std::cerr << "Error pharris: bad value for sigma." << std::endl;
      return FAILURE;
   }

   Img2dsf Ixx(ims.Props());
   Img2dsf Iyy(ims.Props());
   Img2dsf Ixy(ims.Props());
   
   // Computes x, y gradient images.
   Ixx.Frame(0, 1);
   Iyy.Frame(0, 1);
   Ixy.Frame(0, 1);
   for (y = 1; y < ims.Height() - 1; y++) {
      for (x = 1; x < ims.Width() - 1; x++) {
	 gx = (Float(ims[y][x + 1]) - Float(ims[y][x - 1]));
	 gy = (Float(ims[y + 1][x])-Float(ims[y - 1][x]));
	 Ixx[y][x] = gx * gx;
	 Iyy[y][x] = gy * gy;
	 Ixy[y][x] = gx * gy;
      }
   }
   
   // Smooth the squared gradient images.
   if (PGaussianFiltering(Ixx, Ixx, sigma) == FAILURE) {
      return FAILURE;
   }
   if (PGaussianFiltering(Iyy, Iyy, sigma) == FAILURE) {
      return FAILURE;
   }
   if (PGaussianFiltering(Ixy, Ixy, sigma) == FAILURE) {
      return FAILURE;
   }
   
   // Compute the Harris corner measure.
   for (y = 0;y < ims.Height(); y++) {
      for (x = 0;x < ims.Width(); x++) {
	 imd[y][x] = (Ixx[y][x]*Iyy[y][x] - Ixy[y][x]*Ixy[y][x]) - kappa*((Ixx[y][x]+Iyy[y][x])*(Ixx[y][x]+Iyy[y][x]));
      }
   }
   // Apply non-maximum suppression of corner strengths to find the corners.
   for (y = 0; y < ims.Height(); y++) {
      for (x = 0; x < ims.Width(); x++) {
	 for (l = -radius;  l < radius; l++) {
	    for (k = -radius; k < radius; k++) {
	       if ((l != 0 || k != 0) && imd.Hold(y + l, x + k) && imd[y + l][x + k] >= imd[y][x]) {
		  imd[y][x] = 0;
	       }
	    }
	 }
      }
   }
   return SUCCESS;
}

Errc PHarris( const Imc2duc &ims, Img2dsf &imd, float sigma, float kappa ) {
   float gRx, gRy, gGx, gGy, gBx, gBy;
   int x, y, k, l;
   int radius;
   
   radius = (int)(3.0*sigma);
   
   if (radius < 1) {
      std::cerr << "Error pharris: bad value for sigma." << std::endl;
      return FAILURE;
   }

   Img2dsf Ixx(ims.Props());
   Img2dsf Iyy(ims.Props());
   Img2dsf Ixy(ims.Props());
   
   // Computes x, y gradient images.
   Ixx.Frame(0, 1);
   Iyy.Frame(0, 1);
   Ixy.Frame(0, 1);
   
   for (y = 1; y < ims.Height() - 1; y++) {
      for (x = 1; x < ims.Width() - 1; x++) {
	 gRx = (Float(ims.X[y][x + 1]) - Float(ims.X[y][x - 1]));
	 gRy = (Float(ims.X[y + 1][x]) - Float(ims.X[y - 1][x]));
	 gGx = (Float(ims.Y[y][x + 1]) - Float(ims.Y[y][x - 1]));
	 gGy = (Float(ims.Y[y + 1][x]) - Float(ims.Y[y - 1][x]));
	 gBx = (Float(ims.Z[y][x + 1]) - Float(ims.Z[y][x - 1]));
	 gBy = (Float(ims.Z[y + 1][x]) - Float(ims.Z[y - 1][x]));
	 Ixx[y][x] = (gRx * gRx + gGx * gGx + gBx * gBx) / 9;
	 Iyy[y][x] = (gRy * gRy + gGy * gGy + gBy * gBy) / 9;
	 Ixy[y][x] = (gRx * gRy + gGx * gGy + gBx * gBy) / 9;
      }
   }
   
   // Smooth the squared gradient images.
   if (PGaussianFiltering(Ixx, Ixx, sigma) == FAILURE) {
      return FAILURE;
   }
   if (PGaussianFiltering(Iyy, Iyy, sigma) == FAILURE) {
      return FAILURE;
   }
   if (PGaussianFiltering(Ixy, Ixy, sigma) == FAILURE) {
      return FAILURE;
   }
   
  // Compute the Harris corner measure.
   for (y = 0; y < ims.Height(); y++) {
      for (x = 0; x < ims.Width(); x++) {
	 imd[y][x] = (Ixx[y][x]*Iyy[y][x] - Ixy[y][x]*Ixy[y][x]) - kappa*((Ixx[y][x]+Iyy[y][x])*(Ixx[y][x]+Iyy[y][x]));
      }
   }
   // Apply non-maximum suppression of corner strengths to find the corners.
   for (y = 0; y < ims.Height(); y++) {
      for (x = 0; x < ims.Width(); x++) {
	 for (l = -radius; l < radius; l++) {
	    for (k = -radius; k < radius; k++) {
	       if ((l != 0 || k != 0) && imd.Hold(y + l, x + k) && imd[y + l][x + k] >= imd[y][x]) {
		  imd[y][x] = 0;
	       }
	    }
	 }
      }
   }
   return SUCCESS;
}

Errc PHarris( const Imc2dsl &ims, Img2dsf &imd, float sigma, float kappa ) {
   float gRx, gRy, gGx, gGy, gBx, gBy;
   int x, y, k, l;
   int radius;
   
   radius = (int)(3.0*sigma);
   
   if (radius < 1) {
      std::cerr << "Error pharris: bad value for sigma." << std::endl;
      return FAILURE;
   }

   Img2dsf Ixx(ims.Props());
   Img2dsf Iyy(ims.Props());
   Img2dsf Ixy(ims.Props());
   
   // Computes x, y gradient images.
   Ixx.Frame(0, 1);
   Iyy.Frame(0, 1);
   Ixy.Frame(0, 1);
   
   for (y = 1; y < ims.Height() - 1; y++) {
      for (x = 1; x < ims.Width() - 1; x++) {
	 gRx = (Float(ims.X[y][x + 1]) - Float(ims.X[y][x - 1]));
	 gRy = (Float(ims.X[y + 1][x]) - Float(ims.X[y - 1][x]));
	 gGx = (Float(ims.Y[y][x + 1]) - Float(ims.Y[y][x - 1]));
	 gGy = (Float(ims.Y[y + 1][x]) - Float(ims.Y[y - 1][x]));
	 gBx = (Float(ims.Z[y][x + 1]) - Float(ims.Z[y][x - 1]));
	 gBy = (Float(ims.Z[y + 1][x]) - Float(ims.Z[y - 1][x]));
	 Ixx[y][x] = (gRx * gRx + gGx * gGx + gBx * gBx) / 9;
	 Iyy[y][x] = (gRy * gRy + gGy * gGy + gBy * gBy) / 9;
	 Ixy[y][x] = (gRx * gRy + gGx * gGy + gBx * gBy) / 9;
      }
   }
   
   // Smooth the squared gradient images.
   if (PGaussianFiltering(Ixx, Ixx, sigma) == FAILURE) {
      return FAILURE;
   }
   if (PGaussianFiltering(Iyy, Iyy, sigma) == FAILURE) {
      return FAILURE;
   }
   if (PGaussianFiltering(Ixy, Ixy, sigma) == FAILURE) {
      return FAILURE;
   }
   
  // Compute the Harris corner measure.
   for (y = 0; y < ims.Height(); y++) {
      for (x = 0; x < ims.Width(); x++) {
	 imd[y][x] = (Ixx[y][x]*Iyy[y][x] - Ixy[y][x]*Ixy[y][x]) - kappa*((Ixx[y][x]+Iyy[y][x])*(Ixx[y][x]+Iyy[y][x]));
      }
   }
   // Apply non-maximum suppression of corner strengths to find the corners.
   for (y = 0; y < ims.Height(); y++) {
      for (x = 0; x < ims.Width(); x++) {
	 for (l = -radius; l < radius; l++) {
	    for (k = -radius; k < radius; k++) {
	       if ((l != 0 || k != 0) && imd.Hold(y + l, x + k) && imd[y + l][x + k] >= imd[y][x]) {
		  imd[y][x] = 0;
	       }
	    }
	 }
      }
   }
   return SUCCESS;
}

Errc PHarris( const Imc2dsf &ims, Img2dsf &imd, float sigma, float kappa ) {
   float gRx, gRy, gGx, gGy, gBx, gBy;
   int x, y, k, l;
   int radius;
   
   radius = (int)(3.0*sigma);
   
   if (radius < 1) {
      std::cerr << "Error pharris: bad value for sigma." << std::endl;
      return FAILURE;
   }

   Img2dsf Ixx(ims.Props());
   Img2dsf Iyy(ims.Props());
   Img2dsf Ixy(ims.Props());
   
   // Computes x, y gradient images.
   Ixx.Frame(0, 1);
   Iyy.Frame(0, 1);
   Ixy.Frame(0, 1);
   
   for (y = 1; y < ims.Height() - 1; y++) {
      for (x = 1; x < ims.Width() - 1; x++) {
	 gRx = (Float(ims.X[y][x + 1]) - Float(ims.X[y][x - 1]));
	 gRy = (Float(ims.X[y + 1][x]) - Float(ims.X[y - 1][x]));
	 gGx = (Float(ims.Y[y][x + 1]) - Float(ims.Y[y][x - 1]));
	 gGy = (Float(ims.Y[y + 1][x]) - Float(ims.Y[y - 1][x]));
	 gBx = (Float(ims.Z[y][x + 1]) - Float(ims.Z[y][x - 1]));
	 gBy = (Float(ims.Z[y + 1][x]) - Float(ims.Z[y - 1][x]));
	 Ixx[y][x] = (gRx * gRx + gGx * gGx + gBx * gBx) / 9;
	 Iyy[y][x] = (gRy * gRy + gGy * gGy + gBy * gBy) / 9;
	 Ixy[y][x] = (gRx * gRy + gGx * gGy + gBx * gBy) / 9;
      }
   }
   
   // Smooth the squared gradient images.
   if (PGaussianFiltering(Ixx, Ixx, sigma) == FAILURE) {
      return FAILURE;
   }
   if (PGaussianFiltering(Iyy, Iyy, sigma) == FAILURE) {
      return FAILURE;
   }
   if (PGaussianFiltering(Ixy, Ixy, sigma) == FAILURE) {
      return FAILURE;
   }
   
  // Compute the Harris corner measure.
   for (y = 0; y < ims.Height(); y++) {
      for (x = 0; x < ims.Width(); x++) {
	 imd[y][x] = (Ixx[y][x]*Iyy[y][x] - Ixy[y][x]*Ixy[y][x]) - kappa*((Ixx[y][x]+Iyy[y][x])*(Ixx[y][x]+Iyy[y][x]));
      }
   }
   // Apply non-maximum suppression of corner strengths to find the corners.
   for (y = 0; y < ims.Height(); y++) {
      for (x = 0; x < ims.Width(); x++) {
	 for (l = -radius; l < radius; l++) {
	    for (k = -radius; k < radius; k++) {
	       if ((l != 0 || k != 0) && imd.Hold(y + l, x + k) && imd[y + l][x + k] >= imd[y][x]) {
		  imd[y][x] = 0;
	       }
	    }
	 }
      }
   }
   return SUCCESS;
}


#ifdef MAIN
#define	USAGE	"usage: %s sigma kappa [-m mask] [im_in|-] [im_out|-]"
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
      objd[0] = new Img2dsf(ims->Size());
      Img2dsf * const imd = (Img2dsf*)objd[0];
      
      result = PHarris(*ims, *imd, (float)atof(parv[0]), (float)atof(parv[1]));
      goto end;
   }
   if (objs[0]->Type() == Po_Img2dsl) {
      Img2dsl* const ims = (Img2dsl*)objs[0];
      objd[0] = new Img2dsf(ims->Size());
      Img2dsf * const imd = (Img2dsf*)objd[0];
      
      result = PHarris(*ims, *imd, (float)atof(parv[0]), (float)atof(parv[1]));
      goto end;
   }
   if (objs[0]->Type() == Po_Img2dsf) {
      Img2dsf* const ims = (Img2dsf*)objs[0];
      objd[0] = new Img2dsf(ims->Size());
      Img2dsf * const imd = (Img2dsf*)objd[0];
      
      result = PHarris(*ims, *imd, (float)atof(parv[0]), (float)atof(parv[1]));
      goto end;
   }
   if (objs[0]->Type() == Po_Imc2duc) {
      Imc2duc* const ims = (Imc2duc*)objs[0];
      objd[0] = new Img2dsf(ims->Size());
      Img2dsf * const imd = (Img2dsf*)objd[0];
      
      result = PHarris(*ims, *imd, (float)atof(parv[0]), (float)atof(parv[1]));
      goto end;
   }
   if (objs[0]->Type() == Po_Imc2dsl) {
      Imc2dsl* const ims = (Imc2dsl*)objs[0];
      objd[0] = new Img2dsf(ims->Size());
      Img2dsf * const imd = (Img2dsf*)objd[0];
      
      result = PHarris(*ims, *imd, (float)atof(parv[0]), (float)atof(parv[1]));
      goto end;
   }
   if (objs[0]->Type() == Po_Imc2dsf) {
      Imc2dsf* const ims = (Imc2dsf*)objs[0];
      objd[0] = new Img2dsf(ims->Size());
      Img2dsf * const imd = (Img2dsf*)objd[0];
      
      result = PHarris(*ims, *imd, (float)atof(parv[0]), (float)atof(parv[1]));
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
