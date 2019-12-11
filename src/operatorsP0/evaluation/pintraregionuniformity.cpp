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
 * @file pintraregionuniformity.cpp
 *
 * Ref: Y.J. Yang, "A survey on evaluation methods for image segmentation", 
 * Pattern recognition, 29(8), 1335-1346, 1996.
 *
 * A "goodness method" based on intra-region uniformity.
 * Critere base sur la somme des variances des regions.
 * @param rgs	region map.
 * @param ims	input image.
 */

template <typename T>
Float PIntraRegionUniformity( const Reg2d &rgs, const Imx3d<T> &ims ) {  
   // Area for region i
   Ulong *area = (Ulong*)calloc(rgs.Labels() + 1, sizeof(Ulong));
   // Sum f(x, y) for region i
   Float *sum = (Float*)calloc(rgs.Labels() + 1, sizeof(Float));
   // Contrast for region i
   Float *contrast = (Float*)calloc(rgs.Labels() + 1, sizeof(Float));
   
   Ulong k;
   
   T min = Limits<T>::max();
   T max = Limits<T>::min();
   
   const Reg2d::ValueType *pend = rgs.Vector()+rgs.VectorSize();
   Reg2d::ValueType *prgs = rgs.Vector();
   T *pims = ims.Vector(0);
   for ( ; prgs < pend; prgs++, pims++) {
      if ((k = *prgs)) {
	 area[k]++;
	 sum[k] += (Float)*pims;
	 if (*pims > max) {
	    max = *pims;
	 }
	 if (*pims < min) {
	    min = *pims;
	 }
      }
   }

   prgs = rgs.Vector();
   pims = ims.Vector(0);
   for ( ; prgs<pend; prgs++, pims++) {
      if ((k = *prgs)) {
	 Float temp = (Float)*pims - (sum[k] / (Float)area[k]);
	 contrast[k] += temp * temp;
      }
   }
   
   Float GU = 0.0F;
   for (k = 1; k < rgs.Labels() + 1; k++) {
      if (area[k] > 0) {
	 GU += contrast[k] / area[k];
      }
   }
   // Normalization (proposed by Sahoo et al.)
   Float C = ((Float)max - (Float)min) / 2.0F;  
   
   free(contrast);
   free(area);
   free(sum);
   
   if (C == 0.0F) {
      return 1.0F;
   } else {
      return (Float)(1 - (GU / (C * C)));
   }
}

template <typename T>
Float PIntraRegionUniformity( const Reg3d &rgs, const Imx3d<T> &ims ) {  
   // Area for region i
   Ulong *area = (Ulong*)calloc(rgs.Labels() + 1, sizeof(Ulong));
   // Sum f(x, y) for region i
   Float *sum = (Float*)calloc(rgs.Labels() + 1, sizeof(Float));
   // Contrast for region i
   Float *contrast = (Float*)calloc(rgs.Labels() + 1, sizeof(Float));
   
   Ulong k;
   
   T min = Limits<T>::max();
   T max = Limits<T>::min();
   
   const Reg3d::ValueType *pend = rgs.Vector()+rgs.VectorSize();
   Reg3d::ValueType *prgs = rgs.Vector();
   T *pims = ims.Vector(0);
   for ( ; prgs < pend; prgs++, pims++) {
      if ((k = *prgs)) {
	 area[k]++;
	 sum[k] += (Float)*pims;
	 if (*pims > max) {
	    max = *pims;
	 }
	 if (*pims < min) {
	    min = *pims;
	 }
      }
   }

   prgs = rgs.Vector();
   pims = ims.Vector(0);
   for ( ; prgs<pend; prgs++, pims++) {
      if ((k = *prgs)) {
	 Float temp = (Float)*pims - (sum[k] / (Float)area[k]);
	 contrast[k] += temp * temp;
      }
   }
   
   Float GU = 0.0F;
   for (k = 1; k < rgs.Labels() + 1; k++) {
      if (area[k] > 0) {
	 GU += contrast[k] / area[k];
      }
   }
   // Normalization (proposed by Sahoo et al.)
   Float C = ((Float)max - (Float)min) / 2.0F;  
   
   free(contrast);
   free(area);
   free(sum);
   
   if (C == 0.0F) {
      return 1.0F;
   } else {
      return (Float)(1 - (GU / (C * C)));
   }
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
     
     result = PIntraRegionUniformity(*rgs, *ims);
     goto end;
   }
   if (objs[0]->Type() == Po_Reg2d && objs[1]->Type() == Po_Img2dsl) {
     Reg2d* const rgs = (Reg2d*)objs[0];
     Img2dsl* const ims = (Img2dsl*)objs[1];
     
     result = PIntraRegionUniformity(*rgs, *ims);
     goto end;
   }
   if (objs[0]->Type() == Po_Reg2d && objs[1]->Type() == Po_Img2dsf) {
     Reg2d* const rgs = (Reg2d*)objs[0];
     Img2dsf* const ims = (Img2dsf*)objs[1];
     
     result = PIntraRegionUniformity(*rgs, *ims);
     goto end;
   }
   if (objs[0]->Type() == Po_Reg3d && objs[1]->Type() == Po_Img3duc) {
     Reg3d* const rgs = (Reg3d*)objs[0];
     Img3duc* const ims = (Img3duc*)objs[1];
     
     result = PIntraRegionUniformity(*rgs, *ims);
     goto end;
   }
   if (objs[0]->Type() == Po_Reg3d && objs[1]->Type() == Po_Img3dsl) {
     Reg3d* const rgs = (Reg3d*)objs[0];
     Img3dsl* const ims = (Img3dsl*)objs[1];
     
     result = PIntraRegionUniformity(*rgs, *ims);
     goto end;
   }
   if (objs[0]->Type() == Po_Reg3d && objs[1]->Type() == Po_Img3dsf) {
     Reg3d* const rgs = (Reg3d*)objs[0];
     Img3dsf* const ims = (Img3dsf*)objs[1];
     
     result = PIntraRegionUniformity(*rgs, *ims);
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
