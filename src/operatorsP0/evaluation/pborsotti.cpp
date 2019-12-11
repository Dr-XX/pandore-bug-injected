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

#include <math.h>
#include <pandore.h>
using namespace pandore;

#define DEBUG
/**
 * @file pborsotti.cpp
 * 
 * Ref: M. Borsotti, P. Campadelli, R. Schettini, "Quantitative evaluation
 * of color image segmentation results", Pattern Recoginition Letters, 
 * 19:741-747, 1998.
 */
template <typename T>
Errc PBorsotti( const Reg2d &rgs, const Imx3d<T> &ims ) { 
   Float *sum = (Float*)malloc((rgs.Labels() + 1)*sizeof(Float));
   Ulong *area = (Ulong*)calloc(rgs.Labels() + 1, sizeof(Ulong));
   Ulong *R = (Ulong*)calloc(rgs.Labels() + 1, sizeof(Ulong));
   Float *var = (Float*)calloc((rgs.Labels() + 1), sizeof(Float));
   Ulong totalarea = 0;	// The number of non null labels.

   Reg2d::ValueType *prgs;
   T *pims;
   const Reg2d::ValueType *pend = rgs.Vector() + rgs.VectorSize();
   
   // Computes area(Ri), total area.
   prgs = rgs.Vector();   
   for (; prgs < pend; prgs++) {
      // Only non null regions.
      if ((*prgs)) {
	 area[*prgs] ++;
	 totalarea++;
      }
   }

   // For each band (for example: red, green blue...)
   for (int b = 0; b < ims.Bands(); b++) {
      prgs = rgs.Vector();
      pims = ims.Vector(b);
      
      // Resets sum for each band
      memset(sum, 0, rgs.Labels() + 1);
      // Computes sum(Ri) for band b.
      for (; prgs < pend; prgs++, pims++) {
	 // Only non null regions.
	 if ((*prgs)) {
	    sum[*prgs] += *pims;
	 }
      }

      // Computes the Variance(Ri) = sum of each band variance.
      prgs = rgs.Vector();
      pims = ims.Vector(b);
      
      for (; prgs < pend; prgs++, pims++) {
	 if ((*prgs)) {
	    Float temp = (Float)*pims - (sum[*prgs] / area[*prgs]);
	    var[*prgs] += SQR(temp);
	 }
      }
   }
   
   // The number of regions
   Ulong N = rgs.Labels();
   Ulong k;
   for (k = 1; k < rgs.Labels() + 1; k++) {
      if (area[k] == 0) {
	 N--;
      } else {
	 for (Ulong i = 1; i < rgs.Labels() + 1; i++) {
	    if (area[i] == area[k]) {
	       R[k]++;
	    }
	 }
      }
   }
   
   Float critere = 0.0F;
   for (k = 0; k < rgs.Labels() + 1; k++) {
      if (area[k] > 0) {
	 critere += (var[k] / (1 + log((float)area[k]))) + ((R[k] / area[k]) * (R[k] / area[k]));
      }
   }
   
#ifdef DEBUG
   fprintf(stderr, "Nombre de regions %d\n", N);
   fprintf(stderr, "Surface totale des regions: %u pixels\n", totalarea);
   for (k = 1; k < rgs.Labels() + 1; k++) {
      fprintf(stderr, "Region %d: R(Ai) = %u\n", k, R[k]);
      fprintf(stderr, "           Ni = %u pixels\n", area[k]);
      fprintf(stderr, "           ei = %f\n", var[k]);
   }
#endif

   free(sum);
   free(area);
   free(R);
   free(var);
   return (Float)(sqrt((float)N) / (10000.0 * totalarea) * critere);
}

template <typename T>
Errc PBorsotti( const Reg3d &rgs, const Imx3d<T> &ims ) {  
   Float *sum = (Float*)malloc((rgs.Labels() + 1) * sizeof(Float));
   Ulong *area = (Ulong*)calloc(rgs.Labels() + 1, sizeof(Ulong));
   Ulong *R = (Ulong*)calloc(rgs.Labels() + 1, sizeof(Ulong));
   Float *var = (Float*)calloc((rgs.Labels() + 1), sizeof(Float));
   Ulong totalarea = 0;	// The number of non null labels.

   Reg3d::ValueType *prgs;
   T *pims;
   const Reg3d::ValueType *pend = rgs.Vector() + rgs.VectorSize();
   prgs = rgs.Vector();
   
   // Computes Area(Ri), total Area.
   for (; prgs < pend; prgs++) {
      // Only non null regions.
      if ((*prgs)) {
	 area[*prgs] ++;
	 totalarea++;
      }
   }
   
   // For each band (for example: red, green blue...)
   for (int b = 0; b < ims.Bands(); b++) {
      prgs = rgs.Vector();
      pims = ims.Vector(b);
      
      // Resets sum for each band
      memset(sum, 0, rgs.Labels() + 1);
      // Computes sum(Ri) for band b.
      for (; prgs < pend; prgs++, pims++) {
	 // Only non null regions.
	 if ((*prgs)) {
	    sum[*prgs] += *pims;
	 }
      }

      // Computes the Variance(Ri) = sum of ech band variance.
      prgs = rgs.Vector();
      pims = ims.Vector(b);
      pend = rgs.Vector() + rgs.VectorSize();
      
      for (; prgs < pend; prgs++, pims++) {
	 if ((*prgs)) {
	    Float temp = (Float)*pims - (sum[*prgs] / area[*prgs]);
	    var[*prgs] += SQR(temp);
	 }
      }
   }
   
   // The number of regions
   Ulong N = rgs.Labels();
   Ulong k;
   for (k = 1; k < rgs.Labels() + 1; k++) {
      if (area[k] == 0) {
	 N--;
      } else {
	 for (Ulong i = 1; i < rgs.Labels() + 1; i++) {
	    if (area[i] == area[k]) {
	       R[k]++;
	    }
	 }
      }
   }
   
   Float critere = 0.0F;
   for (k = 1; k < rgs.Labels() + 1; k++) {
      if (area[k] > 0) {
	 critere += (var[k] / (1 + log((float)area[k]))) +  ((R[k] / area[k]) * (R[k] / area[k]));
      }
   }
   
#ifdef DEBUG
   fprintf(stderr, "Nombre de regions %d\n", N);
   fprintf(stderr, "Surface totale des regions: %u pixels\n", totalarea);
   for (k = 1; k < rgs.Labels() + 1; k++) {
      fprintf(stderr, "Region %d: R(Ai) = %u\n", k, R[k]);
      fprintf(stderr, "           Ni = %u pixels\n", area[k]);
      fprintf(stderr, "           ei = %f\n", var[k]);
   }
#endif
   free(sum);
   free(area);
   free(R);
   free(var);
   
   return (Float)(sqrt((float)N) / (1000.0 * totalarea) * critere);
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
      
      result = PBorsotti(*rgs, *ims);
      goto end;
   }
   if (objs[0]->Type() == Po_Reg2d && objs[1]->Type() == Po_Img2dsl) {
      Reg2d* const rgs = (Reg2d*)objs[0];
      Img2dsl* const ims = (Img2dsl*)objs[1];
      
      result = PBorsotti(*rgs, *ims);
      goto end;
   }
   if (objs[0]->Type() == Po_Reg2d && objs[1]->Type() == Po_Img2dsf) {
      Reg2d* const rgs = (Reg2d*)objs[0];
      Img2dsf* const ims = (Img2dsf*)objs[1];
      
      result = PBorsotti(*rgs, *ims);
      goto end;
   }
   if (objs[0]->Type() == Po_Reg2d && objs[1]->Type() == Po_Imc2duc) {
      Reg2d* const rgs = (Reg2d*)objs[0];
      Imc2duc* const ims = (Imc2duc*)objs[1];
      
      result = PBorsotti(*rgs, *ims);
      goto end;
   }
   if (objs[0]->Type() == Po_Reg2d && objs[1]->Type() == Po_Imc2dsl) {
      Reg2d* const rgs = (Reg2d*)objs[0];
      Imc2dsl* const ims = (Imc2dsl*)objs[1];
      
      result = PBorsotti(*rgs, *ims);
      goto end;
   }
   if (objs[0]->Type() == Po_Reg2d && objs[1]->Type() == Po_Imc2dsf) {
      Reg2d* const rgs = (Reg2d*)objs[0];
      Imc2dsf* const ims = (Imc2dsf*)objs[1];
      
      result = PBorsotti(*rgs, *ims);
      goto end;
   }
   if (objs[0]->Type() == Po_Reg2d && objs[1]->Type() == Po_Imx2duc) {
      Reg2d* const rgs = (Reg2d*)objs[0];
      Imx2duc* const ims = (Imx2duc*)objs[1];
      
      result = PBorsotti(*rgs, *ims);
      goto end;
   }
   if (objs[0]->Type() == Po_Reg2d && objs[1]->Type() == Po_Imx2dsl) {
      Reg2d* const rgs = (Reg2d*)objs[0];
      Imx2dsl* const ims = (Imx2dsl*)objs[1];
      
      result = PBorsotti(*rgs, *ims);
      goto end;
   }
   if (objs[0]->Type() == Po_Reg2d && objs[1]->Type() == Po_Imx2dsf) {
      Reg2d* const rgs = (Reg2d*)objs[0];
      Imx2dsf* const ims = (Imx2dsf*)objs[1];
      
      result = PBorsotti(*rgs, *ims);
      goto end;
   }
   if (objs[0]->Type() == Po_Reg3d && objs[1]->Type() == Po_Img3duc) {
      Reg3d* const rgs = (Reg3d*)objs[0];
      Img3duc* const ims = (Img3duc*)objs[1];
      
      result = PBorsotti(*rgs, *ims);
      goto end;
   }
   if (objs[0]->Type() == Po_Reg3d && objs[1]->Type() == Po_Img3dsl) {
      Reg3d* const rgs = (Reg3d*)objs[0];
      Img3dsl* const ims = (Img3dsl*)objs[1];
      
      result = PBorsotti(*rgs, *ims);
      goto end;
   }
   if (objs[0]->Type() == Po_Reg3d && objs[1]->Type() == Po_Img3dsf) {
      Reg3d* const rgs = (Reg3d*)objs[0];
      Img3dsf* const ims = (Img3dsf*)objs[1];
      
      result = PBorsotti(*rgs, *ims);
      goto end;
   }
   if (objs[0]->Type() == Po_Reg3d && objs[1]->Type() == Po_Imc3duc) {
      Reg3d* const rgs = (Reg3d*)objs[0];
      Imc3duc* const ims = (Imc3duc*)objs[1];
      
      result = PBorsotti(*rgs, *ims);
      goto end;
   }
   if (objs[0]->Type() == Po_Reg3d && objs[1]->Type() == Po_Imc3dsl) {
      Reg3d* const rgs = (Reg3d*)objs[0];
      Imc3dsl* const ims = (Imc3dsl*)objs[1];
      
      result = PBorsotti(*rgs, *ims);
      goto end;
   }
   if (objs[0]->Type() == Po_Reg3d && objs[1]->Type() == Po_Imc3dsf) {
      Reg3d* const rgs = (Reg3d*)objs[0];
      Imc3dsf* const ims = (Imc3dsf*)objs[1];
      
      result = PBorsotti(*rgs, *ims);
      goto end;
   }
   if (objs[0]->Type() == Po_Reg3d && objs[1]->Type() == Po_Imx3duc) {
      Reg3d* const rgs = (Reg3d*)objs[0];
      Imx3duc* const ims = (Imx3duc*)objs[1];
      
      result = PBorsotti(*rgs, *ims);
      goto end;
   }
   if (objs[0]->Type() == Po_Reg3d && objs[1]->Type() == Po_Imx3dsl) {
      Reg3d* const rgs = (Reg3d*)objs[0];
      Imx3dsl* const ims = (Imx3dsl*)objs[1];
      
      result = PBorsotti(*rgs, *ims);
      goto end;
   }
   if (objs[0]->Type() == Po_Reg3d && objs[1]->Type() == Po_Imx3dsf) {
      Reg3d* const rgs = (Reg3d*)objs[0];
      Imx3dsf* const ims = (Imx3dsf*)objs[1];
      
      result = PBorsotti(*rgs, *ims);
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
