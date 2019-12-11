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
 * @file pzeboudj.cpp
 *
 * A "goodness method" based on inner and outer contrast.
 * 
 * Ref: J-P. Cocquerez, S. Philipp, 
 * "Analyse d'images: filtrage et segmentation", Masson, 1995.
 */
template <typename T>
Errc PZeboudj( const Reg2d &rgs, const Imx3d<T> &ims ) {
   Float *I = (Float*)calloc(rgs.Labels() + 1, sizeof(Float));
   Float *E = (Float*)calloc(rgs.Labels() + 1, sizeof(Float));
   Ulong *A = (Ulong*)calloc(rgs.Labels() + 1, sizeof(Ulong));
   Ulong *F = (Ulong*)calloc((rgs.Labels() + 1), sizeof(Ulong));
   Float *contraste = (Float*)calloc((rgs.Labels() + 1), sizeof(Float));

   Ulong k;
   
   // Computes the mean of each region.
   // + li the length of the boundary
   // + lij the lenght of the common boundary between region i and region j.
   Point2d p;
   T max = ims[0][0][0][0];
   T min = ims[0][0][0][0];
   for (p.y = 0; p.y < rgs.Height(); p.y++) {
      for (p.x = 0; p.x < rgs.Width(); p.x++) {
	 if (ims[0][0][p.y][p.x] > max) {
	    max = ims[0][0][p.y][p.x];
	 }
	 if (ims[0][0][p.y][p.x] < min) {
	    min = ims[0][0][p.y][p.x];
	 }
      }
   }
   Float L = (Float)max - min;
   for (p.y = 0; p.y < rgs.Height(); p.y++) {
      for (p.x = 0;p.x < rgs.Width(); p.x++) {
	 if ((k = rgs[p])) {
	    A[k]++;
	    Float cimax = 0.0F;
	    Float cemax = 0.0F;
	    for (int v = 0; v < 8; v++) {
	       if (!rgs.Hold(p + v8[v])) {
		  continue;
	       }
	       Float c = (Float)ABS(ims[0][0][p.y][p.x] - ims[0][0][p.y + v8y[v]][p.x + v8x[v]]);
	       if (rgs[p + v8[v]] == k) {
		  if (c > cimax) {
		     cimax = c;
		  }
	       } else {
		  if (c > cemax) {
		     cemax = c;
		  }
	       }
	    }
	    I[k] += cimax;
	    E[k] += cemax;
	    if (cemax > 0) {
	       F[k]++;
	    }
	 }
      }
   }
   Float At = 0.0F;
   for ( k = 1; k < rgs.Labels() + 1; k++) {
      At += A[k];
      I[k] = I[k] / (A[k] * L);
      E[k] = E[k] / (F[k] * L);

      if (I[k] > 0.0F && I[k] < E[k]) {
	 contraste[k] = 1 - I[k] / E[k];
      } else {
	 if (I[k] == 0.0F) {
	    contraste[k] = E[k];
	 } else {
	    contraste[k] = 0.0F;
	 }
      }
   }
   
   Float critere = 0.0F;
   for (k = 1; k < rgs.Labels() + 1; k++) {
      critere += contraste[k] * A[k];
   }

   free(contraste);
   free(A);
   free(F);
   free(I);
   free(E);

   return (1.0F / At) * critere;
}

template <typename T>
Errc PZeboudj( const Reg3d &rgs, const Imx3d<T> &ims ) {
   Float *I = (Float*)calloc(rgs.Labels() + 1, sizeof(Float));
   Float *E = (Float*)calloc(rgs.Labels() + 1, sizeof(Float));
   Ulong *A = (Ulong*)calloc(rgs.Labels() + 1, sizeof(Ulong));
   Ulong *F = (Ulong*)calloc((rgs.Labels() + 1), sizeof(Ulong));
   Float *contraste = (Float*)calloc((rgs.Labels() + 1), sizeof(Float));

   Ulong k;
   
   // Computes the mean of each region.
   // + li the length of the boundary
   // + lij the lenght of the common boundary between region i and region j.
   Point3d p;
   T max = ims[0][0][0][0];
   T min = ims[0][0][0][0];
   for (p.z = 0; p.z < rgs.Depth(); p.z++) {
      for (p.y = 0; p.y < rgs.Height(); p.y++) {
	 for (p.x = 0; p.x < rgs.Width(); p.x++) {
	    if (ims[0][p.z][p.y][p.x] > max) {
	       max = ims[0][p.z][p.y][p.x];
	    }
	    if (ims[0][p.z][p.y][p.x] < min) {
	       min = ims[0][p.z][p.y][p.x];
	    }
	 }
      }
   }
   Float L = (Float)max - min;
   for (p.z = 0; p.z < rgs.Depth(); p.z++) {
      for (p.y = 0; p.y < rgs.Height(); p.y++) {
	 for (p.x = 0; p.x < rgs.Width(); p.x++) {
	    if ((k = rgs[p])) {
	       A[k]++;
	       Float cimax = 0.0F;
	       Float cemax = 0.0F;
	       for (int v = 0; v < 26; v++) {
		  if (!rgs.Hold(p + v26[v])) {
		     continue;
		  }
		  Float c = (Float)ABS(ims[0][p.z][p.y][p.x] - ims[0][p.z + v26z[v]][p.y + v26y[v]][p.x + v26x[v]]);
		  if (rgs[p + v26[v]] == k) {
		     if (c > cimax) {
			cimax = c;
		     }
		  } else {
		     if (c > cemax) {
			cemax = c;
		     }
		  }
	       }
	       I[k] += cimax;
	       E[k] += cemax;
	       if (cemax > 0) {
		  F[k]++;
	       }
	    }
	 }
      }
   }
   
   Float At = 0.0F;
   for ( k = 1; k < rgs.Labels() + 1; k++) {
      At +=  A[k];
      I[k] = I[k] / (A[k] * L);
      E[k] = E[k] / (F[k] * L);
      
      if (I[k] > 0.0F && I[k] < E[k]) {
	 contraste[k] = 1 - I[k] / E[k];
      } else {
	 if (I[k] == 0.0F) {
	    contraste[k] = E[k];
	 } else {
	    contraste[k] = 0.0F;
	 }
      }
   }
   
   Float critere = 0.0F;
   for (k = 1; k < rgs.Labels() + 1; k++) {
      critere +=  contraste[k] * A[k];
   }
   
   free(contraste);
   free(A);
   free(F);
   free(I);
   free(E);

   return (1.0F/At) * critere;
}



#ifdef MAIN
#define	USAGE	"USAGE : %s [-m mask] [rg_in|-] [im_in|-]"
#define	PARC	0
#define	FINC	2
#define	FOUTC	0
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
if (objs[0]->Type() == Po_Reg2d && objs[1]->Type() == Po_Img2duc) {
   Reg2d* const rgs = (Reg2d*)objs[0];
   Img2duc* const ims = (Img2duc*)objs[1];
      
   result = PZeboudj(*rgs, *ims);
   goto end;
 }
if (objs[0]->Type() == Po_Reg2d && objs[1]->Type() == Po_Img2dsl) {
   Reg2d* const rgs = (Reg2d*)objs[0];
   Img2dsl* const ims = (Img2dsl*)objs[1];
      
   result = PZeboudj(*rgs, *ims);
   goto end;
 }
if (objs[0]->Type() == Po_Reg2d && objs[1]->Type() == Po_Img2dsf) {
   Reg2d* const rgs = (Reg2d*)objs[0];
   Img2dsf* const ims = (Img2dsf*)objs[1];
      
   result = PZeboudj(*rgs, *ims);
   goto end;
 }
if (objs[0]->Type() == Po_Reg3d && objs[1]->Type() == Po_Img3duc) {
   Reg3d* const rgs = (Reg3d*)objs[0];
   Img3duc* const ims = (Img3duc*)objs[1];
      
   result = PZeboudj(*rgs, *ims);
   goto end;
 }
if (objs[0]->Type() == Po_Reg3d && objs[1]->Type() == Po_Img3dsl) {
   Reg3d* const rgs = (Reg3d*)objs[0];
   Img3dsl* const ims = (Img3dsl*)objs[1];
      
   result = PZeboudj(*rgs, *ims);
   goto end;
 }
if (objs[0]->Type() == Po_Reg3d && objs[1]->Type() == Po_Img3dsf) {
   Reg3d* const rgs = (Reg3d*)objs[0];
   Img3dsf* const ims = (Img3dsf*)objs[1];
      
   result = PZeboudj(*rgs, *ims);
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
