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
 * @file pinterergioncontrast.cpp
 *
 * Ref: Y.J. Yang, "A survey on evaluation methods for image segmentation", 
 * Pattern Recognition, 29(8), 1335-1346, 1996.
 *
 * A "goodness method" based on intra-region uniformity.
 * Critere base sur la somme des variances des regions.
 * @param rgs	region map.
 * @param ims	input image.
 */
template <typename T>
Errc PInterRegionContrast( const Reg2d &rgs, const Imx3d<T> &ims ) {  
   Ulong i, j, k;
   
   Float *sum = (Float*)calloc(rgs.Labels() + 1, sizeof(Float));
   Ulong *perimeter = (Ulong*)calloc(rgs.Labels() + 1, sizeof(Float));
   Float *area = (Float*)calloc(rgs.Labels() + 1, sizeof(Float));  
   Ulong **boundary = (Ulong**)malloc((rgs.Labels() + 1)*sizeof(Ulong*));
   for (i = 0; i < rgs.Labels() + 1; i++) {
      boundary[i] = (Ulong*)calloc((rgs.Labels() + 1), sizeof(Ulong));
   }
   Float *contrast = (Float*)calloc((rgs.Labels() + 1), sizeof(Float));
   
   // Computes the mean of each region.
   // + li the length of the boundary
   // + lij the length of the common boundary between region i and region j.
   Point2d p;
   for (p.y = 0; p.y < rgs.Height(); p.y++) {
      for (p.x = 0; p.x < rgs.Width(); p.x++) {
	 if ((k = rgs[p])) {
	    area[k]++;
	    sum[k] += (Float)ims[0][0][p.y][p.x];
	    for (int v = 0; v < 8; v++) {
	       if (rgs.Hold(p + v8[v]) && rgs[p] != rgs[p + v8[v]]) {
		  perimeter[k]++;
		  boundary[k][rgs[p + v8[v]]]++;
	       }
	    }
	 }
      }
   }
   for (i = 1; i < rgs.Labels() + 1; i++) {
      for (j = 1; j < rgs.Labels() + 1; j++) {
	 if (i == j) {
	    continue;
	 }
	 printf("boundary[%d][%d] = %d\n", i, j, boundary[i][j]);
	 if (boundary[i][j] == 0) {
	    contrast[i] = 0;
	 } else {
	    Float mi = sum[i] / area[i];
	    Float mj = sum[j] / area[j];
	    Float temp = mi - mj;
	    printf("temp = %f\n", temp);
	    contrast[i] +=  (boundary[i][j] * ABS(temp)) / (perimeter[i] * (mi + mj));
	 }
      }
#ifndef NDEBUG
      fprintf(stderr, "Region I contraste = %f\n", contrast[i]);
#endif
   }
   
   Float critere  = 0.0F;
   for (i = 1; i < rgs.Labels() + 1; i++) {
      critere +=  area[i] * contrast[i] / area[i];
   }
   
   free(sum);
   free(perimeter);
   free(area);
   for (i = 0; i < rgs.Labels() + 1; i++) {
      free(boundary[i]);
   }
   free(boundary);
   free(contrast);
   
   return critere;
}

template <typename T>
Errc PInterRegionContrast( const Reg3d &rgs, const Imx3d<T> &ims ) {  
   Ulong i, j, k;
   Float *sum = (Float*)calloc(rgs.Labels() + 1, sizeof(Float));   
   Ulong *perimeter = (Ulong*)calloc(rgs.Labels() + 1, sizeof(Float));
   Float *area = (Float*)calloc(rgs.Labels() + 1, sizeof(Float));
   Ulong **boundary = (Ulong**)malloc((rgs.Labels() + 1)*sizeof(Ulong*));
   for (i = 0; i < rgs.Labels() + 1; i++) {
      boundary[i] = (Ulong*)calloc((rgs.Labels() + 1), sizeof(Ulong));
   }
   Float *contrast = (Float*)calloc((rgs.Labels() + 1), sizeof(Float));
   
   // Computes the mean of each region.
   // + li the length of the boundary
   // + lij the lenght of the common boundary between region i and region j.
   Point3d p;
   for (p.z = 0; p.z < rgs.Depth(); p.z++) {
      for (p.y = 0; p.y < rgs.Height(); p.y++) {
	 for (p.x = 0; p.x < rgs.Width(); p.x++) {
	    if ((k = rgs[p])) {
	       area[k]++;
	       sum[k] += (Float)ims[0][p.z][p.y][p.x];
	       for (int v = 0; v < 26; v++) {
		  if (rgs.Hold(p + v26[v]) && rgs[p] != rgs[p + v26[v]]) {
		     perimeter[k]++;
		     boundary[k][rgs[p + v26[v]]]++;
		  }
	       }
	    }
	 }
      }
   }
   for (i = 1; i < rgs.Labels() + 1; i++) {
      for (j = 1; j < rgs.Labels() + 1; j++) {
	 if (i == j) continue;
	 if (boundary[i][j] == 0) {
	    contrast[i] = 0;
	 } else {
	    Float mi = sum[i] / area[i];
	    Float mj = sum[j] / area[j];
	    Float temp = mi - mj;
	    contrast[i] +=  (boundary[i][j] * ABS(temp)) / (perimeter[i] * (mi + mj));
	 }
      }
   }
   Float critere  = 0.0F;
   for (i = 1; i < rgs.Labels() + 1; i++) {
      critere +=  area[i] * contrast[i] / area[i];
   }

   free(sum);
   free(perimeter);
   free(area);
   for (i = 0; i < rgs.Labels() + 1; i++) {
      free(boundary[i]);
   }
   free(boundary);
   free(contrast);
   
   return critere;
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
      
      result = PInterRegionContrast(*rgs, *ims);
      goto end;
   }
   if (objs[0]->Type() == Po_Reg2d && objs[1]->Type() == Po_Img2dsl) {
      Reg2d* const rgs = (Reg2d*)objs[0];
      Img2dsl* const ims = (Img2dsl*)objs[1];
      
      result = PInterRegionContrast(*rgs, *ims);
      goto end;
   }
   if (objs[0]->Type() == Po_Reg2d && objs[1]->Type() == Po_Img2dsf) {
      Reg2d* const rgs = (Reg2d*)objs[0];
      Img2dsf* const ims = (Img2dsf*)objs[1];
      
      result = PInterRegionContrast(*rgs, *ims);
      goto end;
   }
   if (objs[0]->Type() == Po_Reg3d && objs[1]->Type() == Po_Img3duc) {
      Reg3d* const rgs = (Reg3d*)objs[0];
      Img3duc* const ims = (Img3duc*)objs[1];
      
      result = PInterRegionContrast(*rgs, *ims);
      goto end;
   }
   if (objs[0]->Type() == Po_Reg3d && objs[1]->Type() == Po_Img3dsl) {
      Reg3d* const rgs = (Reg3d*)objs[0];
      Img3dsl* const ims = (Img3dsl*)objs[1];
      
      result = PInterRegionContrast(*rgs, *ims);
      goto end;
   }
   if (objs[0]->Type() == Po_Reg3d && objs[1]->Type() == Po_Img3dsf) {
      Reg3d* const rgs = (Reg3d*)objs[0];
      Img3dsf* const ims = (Img3dsf*)objs[1];
      
      result = PInterRegionContrast(*rgs, *ims);
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
