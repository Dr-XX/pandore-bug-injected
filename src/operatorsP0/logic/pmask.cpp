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
 * @file pmask.cpp
 * Masking operation.
 * if (pixel(ims2)) != 0)
 *     pixel(imd) = pixel(ims1)
 * else
 *     pixel(imd) = 0;
 */

/**
 * T2 is a gray level image or a region map.
 */
template <typename T2>
Errc PMask( const Reg2d &ims1, const Imx2d<T2> &ims2, Reg2d &imd ) { 
   if (ims1.Size() != ims2.Size()) {
      std::cerr << "Error pmask: Incompatible size between input images." << std::endl;
      return FAILURE;
   }
   Ulong *ps1 = ims1.Vector();
   T2 *ps2 = ims2.Vector(0);
   Ulong *pd = imd.Vector();
   
   for (; ps1 < ims1.Vector() + ims1.VectorSize(); ps1++, ps2++, pd++) {
      if (*ps2) {
	 *pd = *ps1;
      } else {
	 *pd = 0;
      }
   }

   imd.Labels(ims1.Labels());
   return SUCCESS;
}

template <typename T2>
Errc PMask( const Reg3d &ims1, const Imx3d<T2> &ims2, Reg3d &imd ) { 
   if (ims1.Size() != ims2.Size()) {
      std::cerr << "Error pmask: Incompatible size between input images." << std::endl;
      return FAILURE;
   }
   Ulong *ps1 = ims1.Vector();
   T2 *ps2 = ims2.Vector(0);
   Ulong *pd = imd.Vector();
   
   for (; ps1 < ims1.Vector() + ims1.VectorSize(); ps1++, ps2++, pd++) {
      if (*ps2) {
	 *pd = *ps1;
      } else {
	 *pd = 0;
      }
   }

   imd.Labels(ims1.Labels());
   return SUCCESS;
}

template <typename T1, typename T2>
Errc PMask( const Imx3d<T1> &ims1, const Imx3d<T2> &ims2, Imx3d<T1> &imd ) { 
   if (ims1.Size() != ims2.Size()) {
      std::cerr << "Error pmask: Incompatible size between input images." << std::endl;
      return FAILURE;
   }
   for (int b = 0; b < ims1.Bands(); b++) {
      T1 *ps1 = ims1.Vector(b);
      T2 *ps2 = ims2.Vector(0);
      T1 *pd = imd.Vector(b);

      for (; ps1 < ims1.Vector(b) + ims1.VectorSize(); ps1++, ps2++, pd++) {
	 if (*ps2) {
	    *pd = *ps1;
	 } else {
	    *pd = (T1)0;
	 }
      }
   }

   return SUCCESS;
}













Errc PMask( const Graph2d &grs1, const Graph2d &grs2, Graph2d &grd ) {
   grd = grs1;
   for (int i = 0; i < grd.Size(); i++) {
      if (grs2[i]) {
	 grd[i]->value = grs1[i]->value;
      } else {
	 if ((grd[i])) {
	    grd[i]->value = 0;
	 }
      }
   }
   return SUCCESS;
}
Errc PMask( const Graph3d &grs1, const Graph3d &grs2, Graph3d &grd ) {
   grd = grs1;
   for (int i = 0; i < grd.Size(); i++) {
      if (grs2[i]) {
	 grd[i]->value = grs1[i]->value;
      } else {
	 if ((grd[i])) {
	    grd[i]->value = 0;
	 }
      }
   }
   return SUCCESS;
}

#ifdef MAIN
#define	USAGE	"usage: %s [-m mask] [im_in1|-] [im_in2|-] [im_out|-]"
#define	PARC	0
#define	FINC	2
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
   if (objs[0]->Type() == Po_Img1duc && objs[1]->Type() == Po_Img1duc) {
      Img1duc* const ims1 = (Img1duc*)objs[0];
      Img1duc* const ims2 = (Img1duc*)objs[1];
      objd[0] = new Img1duc(ims1->Props());
      Img1duc* const imd = (Img1duc*)objd[0];
      
      result = PMask(*ims1, *ims2, *imd);
      goto end;
   }
   if (objs[0]->Type() == Po_Img1duc && objs[1]->Type() == Po_Img1dsl) {
      Img1duc* const ims1 = (Img1duc*)objs[0];
      Img1dsl* const ims2 = (Img1dsl*)objs[1];
      objd[0] = new Img1duc(ims1->Props());
      Img1duc* const imd = (Img1duc*)objd[0];
      
      result = PMask(*ims1, *ims2, *imd);
      goto end;
   }
   if (objs[0]->Type() == Po_Img1duc && objs[1]->Type() == Po_Img1dsf) {
      Img1duc* const ims1 = (Img1duc*)objs[0];
      Img1dsf* const ims2 = (Img1dsf*)objs[1];
      objd[0] = new Img1duc(ims1->Props());
      Img1duc* const imd = (Img1duc*)objd[0];
      
      result = PMask(*ims1, *ims2, *imd);
      goto end;
   }
   if (objs[0]->Type() == Po_Img1dsl && objs[1]->Type() == Po_Img1duc) {
      Img1dsl* const ims1 = (Img1dsl*)objs[0];
      Img1duc* const ims2 = (Img1duc*)objs[1];
      objd[0] = new Img1dsl(ims1->Props());
      Img1dsl* const imd = (Img1dsl*)objd[0];
      
      result = PMask(*ims1, *ims2, *imd);
      goto end;
   }
   if (objs[0]->Type() == Po_Img1dsl && objs[1]->Type() == Po_Img1dsl) {
      Img1dsl* const ims1 = (Img1dsl*)objs[0];
      Img1dsl* const ims2 = (Img1dsl*)objs[1];
      objd[0] = new Img1dsl(ims1->Props());
      Img1dsl* const imd = (Img1dsl*)objd[0];
      
      result = PMask(*ims1, *ims2, *imd);
      goto end;
   }
   if (objs[0]->Type() == Po_Img1dsl && objs[1]->Type() == Po_Img1dsf) {
      Img1dsl* const ims1 = (Img1dsl*)objs[0];
      Img1dsf* const ims2 = (Img1dsf*)objs[1];
      objd[0] = new Img1dsl(ims1->Props());
      Img1dsl* const imd = (Img1dsl*)objd[0];
      
      result = PMask(*ims1, *ims2, *imd);
      goto end;
   }
   if (objs[0]->Type() == Po_Img1dsf && objs[1]->Type() == Po_Img1duc) {
      Img1dsf* const ims1 = (Img1dsf*)objs[0];
      Img1duc* const ims2 = (Img1duc*)objs[1];
      objd[0] = new Img1dsf(ims1->Props());
      Img1dsf* const imd = (Img1dsf*)objd[0];
      
      result = PMask(*ims1, *ims2, *imd);
      goto end;
   }
   if (objs[0]->Type() == Po_Img1dsf && objs[1]->Type() == Po_Img1dsl) {
      Img1dsf* const ims1 = (Img1dsf*)objs[0];
      Img1dsl* const ims2 = (Img1dsl*)objs[1];
      objd[0] = new Img1dsf(ims1->Props());
      Img1dsf* const imd = (Img1dsf*)objd[0];
      
      result = PMask(*ims1, *ims2, *imd);
      goto end;
   }
   if (objs[0]->Type() == Po_Img1dsf && objs[1]->Type() == Po_Img1dsf) {
      Img1dsf* const ims1 = (Img1dsf*)objs[0];
      Img1dsf* const ims2 = (Img1dsf*)objs[1];
      objd[0] = new Img1dsf(ims1->Props());
      Img1dsf* const imd = (Img1dsf*)objd[0];
      
      result = PMask(*ims1, *ims2, *imd);
      goto end;
   }
   if (objs[0]->Type() == Po_Img2duc && objs[1]->Type() == Po_Img2duc) {
      Img2duc* const ims1 = (Img2duc*)objs[0];
      Img2duc* const ims2 = (Img2duc*)objs[1];
      objd[0] = new Img2duc(ims1->Props());
      Img2duc* const imd = (Img2duc*)objd[0];
      
      result = PMask(*ims1, *ims2, *imd);
      goto end;
   }
   if (objs[0]->Type() == Po_Img2duc && objs[1]->Type() == Po_Img2dsl) {
      Img2duc* const ims1 = (Img2duc*)objs[0];
      Img2dsl* const ims2 = (Img2dsl*)objs[1];
      objd[0] = new Img2duc(ims1->Props());
      Img2duc* const imd = (Img2duc*)objd[0];
      
      result = PMask(*ims1, *ims2, *imd);
      goto end;
   }
   if (objs[0]->Type() == Po_Img2duc && objs[1]->Type() == Po_Img2dsf) {
      Img2duc* const ims1 = (Img2duc*)objs[0];
      Img2dsf* const ims2 = (Img2dsf*)objs[1];
      objd[0] = new Img2duc(ims1->Props());
      Img2duc* const imd = (Img2duc*)objd[0];
      
      result = PMask(*ims1, *ims2, *imd);
      goto end;
   }
   if (objs[0]->Type() == Po_Img2dsl && objs[1]->Type() == Po_Img2duc) {
      Img2dsl* const ims1 = (Img2dsl*)objs[0];
      Img2duc* const ims2 = (Img2duc*)objs[1];
      objd[0] = new Img2dsl(ims1->Props());
      Img2dsl* const imd = (Img2dsl*)objd[0];
      
      result = PMask(*ims1, *ims2, *imd);
      goto end;
   }
   if (objs[0]->Type() == Po_Img2dsl && objs[1]->Type() == Po_Img2dsl) {
      Img2dsl* const ims1 = (Img2dsl*)objs[0];
      Img2dsl* const ims2 = (Img2dsl*)objs[1];
      objd[0] = new Img2dsl(ims1->Props());
      Img2dsl* const imd = (Img2dsl*)objd[0];
      
      result = PMask(*ims1, *ims2, *imd);
      goto end;
   }
   if (objs[0]->Type() == Po_Img2dsl && objs[1]->Type() == Po_Img2dsf) {
      Img2dsl* const ims1 = (Img2dsl*)objs[0];
      Img2dsf* const ims2 = (Img2dsf*)objs[1];
      objd[0] = new Img2dsl(ims1->Props());
      Img2dsl* const imd = (Img2dsl*)objd[0];
      
      result = PMask(*ims1, *ims2, *imd);
      goto end;
   }
   if (objs[0]->Type() == Po_Img2dsf && objs[1]->Type() == Po_Img2duc) {
      Img2dsf* const ims1 = (Img2dsf*)objs[0];
      Img2duc* const ims2 = (Img2duc*)objs[1];
      objd[0] = new Img2dsf(ims1->Props());
      Img2dsf* const imd = (Img2dsf*)objd[0];
      
      result = PMask(*ims1, *ims2, *imd);
      goto end;
   }
   if (objs[0]->Type() == Po_Img2dsf && objs[1]->Type() == Po_Img2dsl) {
      Img2dsf* const ims1 = (Img2dsf*)objs[0];
      Img2dsl* const ims2 = (Img2dsl*)objs[1];
      objd[0] = new Img2dsf(ims1->Props());
      Img2dsf* const imd = (Img2dsf*)objd[0];
      
      result = PMask(*ims1, *ims2, *imd);
      goto end;
   }
   if (objs[0]->Type() == Po_Img2dsf && objs[1]->Type() == Po_Img2dsf) {
      Img2dsf* const ims1 = (Img2dsf*)objs[0];
      Img2dsf* const ims2 = (Img2dsf*)objs[1];
      objd[0] = new Img2dsf(ims1->Props());
      Img2dsf* const imd = (Img2dsf*)objd[0];
      
      result = PMask(*ims1, *ims2, *imd);
      goto end;
   }
   if (objs[0]->Type() == Po_Img2duc && objs[1]->Type() == Po_Reg2d) {
      Img2duc* const ims1 = (Img2duc*)objs[0];
      Reg2d* const ims2 = (Reg2d*)objs[1];
      objd[0] = new Img2duc(ims1->Props());
      Img2duc* const imd = (Img2duc*)objd[0];
      
      result = PMask(*ims1, *ims2, *imd);
      goto end;
   }
   if (objs[0]->Type() == Po_Img2dsl && objs[1]->Type() == Po_Reg2d) {
      Img2dsl* const ims1 = (Img2dsl*)objs[0];
      Reg2d* const ims2 = (Reg2d*)objs[1];
      objd[0] = new Img2dsl(ims1->Props());
      Img2dsl* const imd = (Img2dsl*)objd[0];
      
      result = PMask(*ims1, *ims2, *imd);
      goto end;
   }
   if (objs[0]->Type() == Po_Img2dsf && objs[1]->Type() == Po_Reg2d) {
      Img2dsf* const ims1 = (Img2dsf*)objs[0];
      Reg2d* const ims2 = (Reg2d*)objs[1];
      objd[0] = new Img2dsf(ims1->Props());
      Img2dsf* const imd = (Img2dsf*)objd[0];
      
      result = PMask(*ims1, *ims2, *imd);
      goto end;
   }
   if (objs[0]->Type() == Po_Img3duc && objs[1]->Type() == Po_Img3duc) {
      Img3duc* const ims1 = (Img3duc*)objs[0];
      Img3duc* const ims2 = (Img3duc*)objs[1];
      objd[0] = new Img3duc(ims1->Props());
      Img3duc* const imd = (Img3duc*)objd[0];
      
      result = PMask(*ims1, *ims2, *imd);
      goto end;
   }
   if (objs[0]->Type() == Po_Img3duc && objs[1]->Type() == Po_Img3dsl) {
      Img3duc* const ims1 = (Img3duc*)objs[0];
      Img3dsl* const ims2 = (Img3dsl*)objs[1];
      objd[0] = new Img3duc(ims1->Props());
      Img3duc* const imd = (Img3duc*)objd[0];
      
      result = PMask(*ims1, *ims2, *imd);
      goto end;
   }
   if (objs[0]->Type() == Po_Img3duc && objs[1]->Type() == Po_Img3dsf) {
      Img3duc* const ims1 = (Img3duc*)objs[0];
      Img3dsf* const ims2 = (Img3dsf*)objs[1];
      objd[0] = new Img3duc(ims1->Props());
      Img3duc* const imd = (Img3duc*)objd[0];
      
      result = PMask(*ims1, *ims2, *imd);
      goto end;
   }
   if (objs[0]->Type() == Po_Img3dsl && objs[1]->Type() == Po_Img3duc) {
      Img3dsl* const ims1 = (Img3dsl*)objs[0];
      Img3duc* const ims2 = (Img3duc*)objs[1];
      objd[0] = new Img3dsl(ims1->Props());
      Img3dsl* const imd = (Img3dsl*)objd[0];
      
      result = PMask(*ims1, *ims2, *imd);
      goto end;
   }
   if (objs[0]->Type() == Po_Img3dsl && objs[1]->Type() == Po_Img3dsl) {
      Img3dsl* const ims1 = (Img3dsl*)objs[0];
      Img3dsl* const ims2 = (Img3dsl*)objs[1];
      objd[0] = new Img3dsl(ims1->Props());
      Img3dsl* const imd = (Img3dsl*)objd[0];
      
      result = PMask(*ims1, *ims2, *imd);
      goto end;
   }
   if (objs[0]->Type() == Po_Img3dsl && objs[1]->Type() == Po_Img3dsf) {
      Img3dsl* const ims1 = (Img3dsl*)objs[0];
      Img3dsf* const ims2 = (Img3dsf*)objs[1];
      objd[0] = new Img3dsl(ims1->Props());
      Img3dsl* const imd = (Img3dsl*)objd[0];
      
      result = PMask(*ims1, *ims2, *imd);
      goto end;
   }
   if (objs[0]->Type() == Po_Img3dsf && objs[1]->Type() == Po_Img3duc) {
      Img3dsf* const ims1 = (Img3dsf*)objs[0];
      Img3duc* const ims2 = (Img3duc*)objs[1];
      objd[0] = new Img3dsf(ims1->Props());
      Img3dsf* const imd = (Img3dsf*)objd[0];
      
      result = PMask(*ims1, *ims2, *imd);
      goto end;
   }
   if (objs[0]->Type() == Po_Img3dsf && objs[1]->Type() == Po_Img3dsl) {
      Img3dsf* const ims1 = (Img3dsf*)objs[0];
      Img3dsl* const ims2 = (Img3dsl*)objs[1];
      objd[0] = new Img3dsf(ims1->Props());
      Img3dsf* const imd = (Img3dsf*)objd[0];
      
      result = PMask(*ims1, *ims2, *imd);
      goto end;
   }
   if (objs[0]->Type() == Po_Img3dsf && objs[1]->Type() == Po_Img3dsf) {
      Img3dsf* const ims1 = (Img3dsf*)objs[0];
      Img3dsf* const ims2 = (Img3dsf*)objs[1];
      objd[0] = new Img3dsf(ims1->Props());
      Img3dsf* const imd = (Img3dsf*)objd[0];
      
      result = PMask(*ims1, *ims2, *imd);
      goto end;
   }
   if (objs[0]->Type() == Po_Img3duc && objs[1]->Type() == Po_Reg3d) {
      Img3duc* const ims1 = (Img3duc*)objs[0];
      Reg3d* const ims2 = (Reg3d*)objs[1];
      objd[0] = new Img3duc(ims1->Props());
      Img3duc* const imd = (Img3duc*)objd[0];
      
      result = PMask(*ims1, *ims2, *imd);
      goto end;
   }
   if (objs[0]->Type() == Po_Img3dsl && objs[1]->Type() == Po_Reg3d) {
      Img3dsl* const ims1 = (Img3dsl*)objs[0];
      Reg3d* const ims2 = (Reg3d*)objs[1];
      objd[0] = new Img3dsl(ims1->Props());
      Img3dsl* const imd = (Img3dsl*)objd[0];
      
      result = PMask(*ims1, *ims2, *imd);
      goto end;
   }
   if (objs[0]->Type() == Po_Img3dsf && objs[1]->Type() == Po_Reg3d) {
      Img3dsf* const ims1 = (Img3dsf*)objs[0];
      Reg3d* const ims2 = (Reg3d*)objs[1];
      objd[0] = new Img3dsf(ims1->Props());
      Img3dsf* const imd = (Img3dsf*)objd[0];
      
      result = PMask(*ims1, *ims2, *imd);
      goto end;
   }
   if (objs[0]->Type() == Po_Imc2duc && objs[1]->Type() == Po_Img2duc) {
      Imc2duc* const ims1 = (Imc2duc*)objs[0];
      Img2duc* const ims2 = (Img2duc*)objs[1];
      objd[0] = new Imc2duc(ims1->Props());
      Imc2duc* const imd = (Imc2duc*)objd[0];
      
      result = PMask(*ims1, *ims2, *imd);
      goto end;
   }
   if (objs[0]->Type() == Po_Imc2duc && objs[1]->Type() == Po_Img2dsl) {
      Imc2duc* const ims1 = (Imc2duc*)objs[0];
      Img2dsl* const ims2 = (Img2dsl*)objs[1];
      objd[0] = new Imc2duc(ims1->Props());
      Imc2duc* const imd = (Imc2duc*)objd[0];
      
      result = PMask(*ims1, *ims2, *imd);
      goto end;
   }
   if (objs[0]->Type() == Po_Imc2duc && objs[1]->Type() == Po_Img2dsf) {
      Imc2duc* const ims1 = (Imc2duc*)objs[0];
      Img2dsf* const ims2 = (Img2dsf*)objs[1];
      objd[0] = new Imc2duc(ims1->Props());
      Imc2duc* const imd = (Imc2duc*)objd[0];
      
      result = PMask(*ims1, *ims2, *imd);
      goto end;
   }
   if (objs[0]->Type() == Po_Imc2dsl && objs[1]->Type() == Po_Img2duc) {
      Imc2dsl* const ims1 = (Imc2dsl*)objs[0];
      Img2duc* const ims2 = (Img2duc*)objs[1];
      objd[0] = new Imc2dsl(ims1->Props());
      Imc2dsl* const imd = (Imc2dsl*)objd[0];
      
      result = PMask(*ims1, *ims2, *imd);
      goto end;
   }
   if (objs[0]->Type() == Po_Imc2dsl && objs[1]->Type() == Po_Img2dsl) {
      Imc2dsl* const ims1 = (Imc2dsl*)objs[0];
      Img2dsl* const ims2 = (Img2dsl*)objs[1];
      objd[0] = new Imc2dsl(ims1->Props());
      Imc2dsl* const imd = (Imc2dsl*)objd[0];
      
      result = PMask(*ims1, *ims2, *imd);
      goto end;
   }
   if (objs[0]->Type() == Po_Imc2dsl && objs[1]->Type() == Po_Img2dsf) {
      Imc2dsl* const ims1 = (Imc2dsl*)objs[0];
      Img2dsf* const ims2 = (Img2dsf*)objs[1];
      objd[0] = new Imc2dsl(ims1->Props());
      Imc2dsl* const imd = (Imc2dsl*)objd[0];
      
      result = PMask(*ims1, *ims2, *imd);
      goto end;
   }
   if (objs[0]->Type() == Po_Imc2dsf && objs[1]->Type() == Po_Img2duc) {
      Imc2dsf* const ims1 = (Imc2dsf*)objs[0];
      Img2duc* const ims2 = (Img2duc*)objs[1];
      objd[0] = new Imc2dsf(ims1->Props());
      Imc2dsf* const imd = (Imc2dsf*)objd[0];
      
      result = PMask(*ims1, *ims2, *imd);
      goto end;
   }
   if (objs[0]->Type() == Po_Imc2dsf && objs[1]->Type() == Po_Img2dsl) {
      Imc2dsf* const ims1 = (Imc2dsf*)objs[0];
      Img2dsl* const ims2 = (Img2dsl*)objs[1];
      objd[0] = new Imc2dsf(ims1->Props());
      Imc2dsf* const imd = (Imc2dsf*)objd[0];
      
      result = PMask(*ims1, *ims2, *imd);
      goto end;
   }
   if (objs[0]->Type() == Po_Imc2dsf && objs[1]->Type() == Po_Img2dsf) {
      Imc2dsf* const ims1 = (Imc2dsf*)objs[0];
      Img2dsf* const ims2 = (Img2dsf*)objs[1];
      objd[0] = new Imc2dsf(ims1->Props());
      Imc2dsf* const imd = (Imc2dsf*)objd[0];
      
      result = PMask(*ims1, *ims2, *imd);
      goto end;
   }
   if (objs[0]->Type() == Po_Imc2duc && objs[1]->Type() == Po_Reg2d) {
      Imc2duc* const ims1 = (Imc2duc*)objs[0];
      Reg2d* const ims2 = (Reg2d*)objs[1];
      objd[0] = new Imc2duc(ims1->Props());
      Imc2duc* const imd = (Imc2duc*)objd[0];
      
      result = PMask(*ims1, *ims2, *imd);
      goto end;
   }
   if (objs[0]->Type() == Po_Imc2dsl && objs[1]->Type() == Po_Reg2d) {
      Imc2dsl* const ims1 = (Imc2dsl*)objs[0];
      Reg2d* const ims2 = (Reg2d*)objs[1];
      objd[0] = new Imc2dsl(ims1->Props());
      Imc2dsl* const imd = (Imc2dsl*)objd[0];
      
      result = PMask(*ims1, *ims2, *imd);
      goto end;
   }
   if (objs[0]->Type() == Po_Imc2dsf && objs[1]->Type() == Po_Reg2d) {
      Imc2dsf* const ims1 = (Imc2dsf*)objs[0];
      Reg2d* const ims2 = (Reg2d*)objs[1];
      objd[0] = new Imc2dsf(ims1->Props());
      Imc2dsf* const imd = (Imc2dsf*)objd[0];
      
      result = PMask(*ims1, *ims2, *imd);
      goto end;
   }
   if (objs[0]->Type() == Po_Imc3duc && objs[1]->Type() == Po_Img3duc) {
      Imc3duc* const ims1 = (Imc3duc*)objs[0];
      Img3duc* const ims2 = (Img3duc*)objs[1];
      objd[0] = new Imc3duc(ims1->Props());
      Imc3duc* const imd = (Imc3duc*)objd[0];
      
      result = PMask(*ims1, *ims2, *imd);
      goto end;
   }
   if (objs[0]->Type() == Po_Imc3duc && objs[1]->Type() == Po_Img3dsl) {
      Imc3duc* const ims1 = (Imc3duc*)objs[0];
      Img3dsl* const ims2 = (Img3dsl*)objs[1];
      objd[0] = new Imc3duc(ims1->Props());
      Imc3duc* const imd = (Imc3duc*)objd[0];
      
      result = PMask(*ims1, *ims2, *imd);
      goto end;
   }
   if (objs[0]->Type() == Po_Imc3duc && objs[1]->Type() == Po_Img3dsf) {
      Imc3duc* const ims1 = (Imc3duc*)objs[0];
      Img3dsf* const ims2 = (Img3dsf*)objs[1];
      objd[0] = new Imc3duc(ims1->Props());
      Imc3duc* const imd = (Imc3duc*)objd[0];
      
      result = PMask(*ims1, *ims2, *imd);
      goto end;
   }
   if (objs[0]->Type() == Po_Imc3dsl && objs[1]->Type() == Po_Img3duc) {
      Imc3dsl* const ims1 = (Imc3dsl*)objs[0];
      Img3duc* const ims2 = (Img3duc*)objs[1];
      objd[0] = new Imc3dsl(ims1->Props());
      Imc3dsl* const imd = (Imc3dsl*)objd[0];
      
      result = PMask(*ims1, *ims2, *imd);
      goto end;
   }
   if (objs[0]->Type() == Po_Imc3dsl && objs[1]->Type() == Po_Img3dsl) {
      Imc3dsl* const ims1 = (Imc3dsl*)objs[0];
      Img3dsl* const ims2 = (Img3dsl*)objs[1];
      objd[0] = new Imc3dsl(ims1->Props());
      Imc3dsl* const imd = (Imc3dsl*)objd[0];
      
      result = PMask(*ims1, *ims2, *imd);
      goto end;
   }
   if (objs[0]->Type() == Po_Imc3dsl && objs[1]->Type() == Po_Img3dsf) {
      Imc3dsl* const ims1 = (Imc3dsl*)objs[0];
      Img3dsf* const ims2 = (Img3dsf*)objs[1];
      objd[0] = new Imc3dsl(ims1->Props());
      Imc3dsl* const imd = (Imc3dsl*)objd[0];
      
      result = PMask(*ims1, *ims2, *imd);
      goto end;
   }
   if (objs[0]->Type() == Po_Imc3dsf && objs[1]->Type() == Po_Img3duc) {
      Imc3dsf* const ims1 = (Imc3dsf*)objs[0];
      Img3duc* const ims2 = (Img3duc*)objs[1];
      objd[0] = new Imc3dsf(ims1->Props());
      Imc3dsf* const imd = (Imc3dsf*)objd[0];
      
      result = PMask(*ims1, *ims2, *imd);
      goto end;
   }
   if (objs[0]->Type() == Po_Imc3dsf && objs[1]->Type() == Po_Img3dsl) {
      Imc3dsf* const ims1 = (Imc3dsf*)objs[0];
      Img3dsl* const ims2 = (Img3dsl*)objs[1];
      objd[0] = new Imc3dsf(ims1->Props());
      Imc3dsf* const imd = (Imc3dsf*)objd[0];
      
      result = PMask(*ims1, *ims2, *imd);
      goto end;
   }
   if (objs[0]->Type() == Po_Imc3dsf && objs[1]->Type() == Po_Img3dsf) {
      Imc3dsf* const ims1 = (Imc3dsf*)objs[0];
      Img3dsf* const ims2 = (Img3dsf*)objs[1];
      objd[0] = new Imc3dsf(ims1->Props());
      Imc3dsf* const imd = (Imc3dsf*)objd[0];
      
      result = PMask(*ims1, *ims2, *imd);
      goto end;
   }
   if (objs[0]->Type() == Po_Imc3duc && objs[1]->Type() == Po_Reg3d) {
      Imc3duc* const ims1 = (Imc3duc*)objs[0];
      Reg3d* const ims2 = (Reg3d*)objs[1];
      objd[0] = new Imc3duc(ims1->Props());
      Imc3duc* const imd = (Imc3duc*)objd[0];
      
      result = PMask(*ims1, *ims2, *imd);
      goto end;
   }
   if (objs[0]->Type() == Po_Imc3dsl && objs[1]->Type() == Po_Reg3d) {
      Imc3dsl* const ims1 = (Imc3dsl*)objs[0];
      Reg3d* const ims2 = (Reg3d*)objs[1];
      objd[0] = new Imc3dsl(ims1->Props());
      Imc3dsl* const imd = (Imc3dsl*)objd[0];
      
      result = PMask(*ims1, *ims2, *imd);
      goto end;
   }
   if (objs[0]->Type() == Po_Imc3dsf && objs[1]->Type() == Po_Reg3d) {
      Imc3dsf* const ims1 = (Imc3dsf*)objs[0];
      Reg3d* const ims2 = (Reg3d*)objs[1];
      objd[0] = new Imc3dsf(ims1->Props());
      Imc3dsf* const imd = (Imc3dsf*)objd[0];
      
      result = PMask(*ims1, *ims2, *imd);
      goto end;
   }
   if (objs[0]->Type() == Po_Imx2duc && objs[1]->Type() == Po_Img2duc) {
      Imx2duc* const ims1 = (Imx2duc*)objs[0];
      Img2duc* const ims2 = (Img2duc*)objs[1];
      objd[0] = new Imx2duc(ims1->Props());
      Imx2duc* const imd = (Imx2duc*)objd[0];
      
      result = PMask(*ims1, *ims2, *imd);
      goto end;
   }
   if (objs[0]->Type() == Po_Imx2duc && objs[1]->Type() == Po_Img2dsl) {
      Imx2duc* const ims1 = (Imx2duc*)objs[0];
      Img2dsl* const ims2 = (Img2dsl*)objs[1];
      objd[0] = new Imx2duc(ims1->Props());
      Imx2duc* const imd = (Imx2duc*)objd[0];
      
      result = PMask(*ims1, *ims2, *imd);
      goto end;
   }
   if (objs[0]->Type() == Po_Imx2duc && objs[1]->Type() == Po_Img2dsf) {
      Imx2duc* const ims1 = (Imx2duc*)objs[0];
      Img2dsf* const ims2 = (Img2dsf*)objs[1];
      objd[0] = new Imx2duc(ims1->Props());
      Imx2duc* const imd = (Imx2duc*)objd[0];
      
      result = PMask(*ims1, *ims2, *imd);
      goto end;
   }
   if (objs[0]->Type() == Po_Imx2dsl && objs[1]->Type() == Po_Img2duc) {
      Imx2dsl* const ims1 = (Imx2dsl*)objs[0];
      Img2duc* const ims2 = (Img2duc*)objs[1];
      objd[0] = new Imx2dsl(ims1->Props());
      Imx2dsl* const imd = (Imx2dsl*)objd[0];
      
      result = PMask(*ims1, *ims2, *imd);
      goto end;
   }
   if (objs[0]->Type() == Po_Imx2dsl && objs[1]->Type() == Po_Img2dsl) {
      Imx2dsl* const ims1 = (Imx2dsl*)objs[0];
      Img2dsl* const ims2 = (Img2dsl*)objs[1];
      objd[0] = new Imx2dsl(ims1->Props());
      Imx2dsl* const imd = (Imx2dsl*)objd[0];
      
      result = PMask(*ims1, *ims2, *imd);
      goto end;
   }
   if (objs[0]->Type() == Po_Imx2dsl && objs[1]->Type() == Po_Img2dsf) {
      Imx2dsl* const ims1 = (Imx2dsl*)objs[0];
      Img2dsf* const ims2 = (Img2dsf*)objs[1];
      objd[0] = new Imx2dsl(ims1->Props());
      Imx2dsl* const imd = (Imx2dsl*)objd[0];
      
      result = PMask(*ims1, *ims2, *imd);
      goto end;
   }
   if (objs[0]->Type() == Po_Imx2dsf && objs[1]->Type() == Po_Img2duc) {
      Imx2dsf* const ims1 = (Imx2dsf*)objs[0];
      Img2duc* const ims2 = (Img2duc*)objs[1];
      objd[0] = new Imx2dsf(ims1->Props());
      Imx2dsf* const imd = (Imx2dsf*)objd[0];
      
      result = PMask(*ims1, *ims2, *imd);
      goto end;
   }
   if (objs[0]->Type() == Po_Imx2dsf && objs[1]->Type() == Po_Img2dsl) {
      Imx2dsf* const ims1 = (Imx2dsf*)objs[0];
      Img2dsl* const ims2 = (Img2dsl*)objs[1];
      objd[0] = new Imx2dsf(ims1->Props());
      Imx2dsf* const imd = (Imx2dsf*)objd[0];
      
      result = PMask(*ims1, *ims2, *imd);
      goto end;
   }
   if (objs[0]->Type() == Po_Imx2dsf && objs[1]->Type() == Po_Img2dsf) {
      Imx2dsf* const ims1 = (Imx2dsf*)objs[0];
      Img2dsf* const ims2 = (Img2dsf*)objs[1];
      objd[0] = new Imx2dsf(ims1->Props());
      Imx2dsf* const imd = (Imx2dsf*)objd[0];
      
      result = PMask(*ims1, *ims2, *imd);
      goto end;
   }
   if (objs[0]->Type() == Po_Imx2duc && objs[1]->Type() == Po_Reg2d) {
      Imx2duc* const ims1 = (Imx2duc*)objs[0];
      Reg2d* const ims2 = (Reg2d*)objs[1];
      objd[0] = new Imx2duc(ims1->Props());
      Imx2duc* const imd = (Imx2duc*)objd[0];
      
      result = PMask(*ims1, *ims2, *imd);
      goto end;
   }
   if (objs[0]->Type() == Po_Imx2dsl && objs[1]->Type() == Po_Reg2d) {
      Imx2dsl* const ims1 = (Imx2dsl*)objs[0];
      Reg2d* const ims2 = (Reg2d*)objs[1];
      objd[0] = new Imx2dsl(ims1->Props());
      Imx2dsl* const imd = (Imx2dsl*)objd[0];
      
      result = PMask(*ims1, *ims2, *imd);
      goto end;
   }
   if (objs[0]->Type() == Po_Imx2dsf && objs[1]->Type() == Po_Reg2d) {
      Imx2dsf* const ims1 = (Imx2dsf*)objs[0];
      Reg2d* const ims2 = (Reg2d*)objs[1];
      objd[0] = new Imx2dsf(ims1->Props());
      Imx2dsf* const imd = (Imx2dsf*)objd[0];
      
      result = PMask(*ims1, *ims2, *imd);
      goto end;
   }
   if (objs[0]->Type() == Po_Imx3duc && objs[1]->Type() == Po_Img3duc) {
      Imx3duc* const ims1 = (Imx3duc*)objs[0];
      Img3duc* const ims2 = (Img3duc*)objs[1];
      objd[0] = new Imx3duc(ims1->Props());
      Imx3duc* const imd = (Imx3duc*)objd[0];
      
      result = PMask(*ims1, *ims2, *imd);
      goto end;
   }
   if (objs[0]->Type() == Po_Imx3duc && objs[1]->Type() == Po_Img3dsl) {
      Imx3duc* const ims1 = (Imx3duc*)objs[0];
      Img3dsl* const ims2 = (Img3dsl*)objs[1];
      objd[0] = new Imx3duc(ims1->Props());
      Imx3duc* const imd = (Imx3duc*)objd[0];
      
      result = PMask(*ims1, *ims2, *imd);
      goto end;
   }
   if (objs[0]->Type() == Po_Imx3duc && objs[1]->Type() == Po_Img3dsf) {
      Imx3duc* const ims1 = (Imx3duc*)objs[0];
      Img3dsf* const ims2 = (Img3dsf*)objs[1];
      objd[0] = new Imx3duc(ims1->Props());
      Imx3duc* const imd = (Imx3duc*)objd[0];
      
      result = PMask(*ims1, *ims2, *imd);
      goto end;
   }
   if (objs[0]->Type() == Po_Imx3dsl && objs[1]->Type() == Po_Img3duc) {
      Imx3dsl* const ims1 = (Imx3dsl*)objs[0];
      Img3duc* const ims2 = (Img3duc*)objs[1];
      objd[0] = new Imx3dsl(ims1->Props());
      Imx3dsl* const imd = (Imx3dsl*)objd[0];
      
      result = PMask(*ims1, *ims2, *imd);
      goto end;
   }
   if (objs[0]->Type() == Po_Imx3dsl && objs[1]->Type() == Po_Img3dsl) {
      Imx3dsl* const ims1 = (Imx3dsl*)objs[0];
      Img3dsl* const ims2 = (Img3dsl*)objs[1];
      objd[0] = new Imx3dsl(ims1->Props());
      Imx3dsl* const imd = (Imx3dsl*)objd[0];
      
      result = PMask(*ims1, *ims2, *imd);
      goto end;
   }
   if (objs[0]->Type() == Po_Imx3dsl && objs[1]->Type() == Po_Img3dsf) {
      Imx3dsl* const ims1 = (Imx3dsl*)objs[0];
      Img3dsf* const ims2 = (Img3dsf*)objs[1];
      objd[0] = new Imx3dsl(ims1->Props());
      Imx3dsl* const imd = (Imx3dsl*)objd[0];
      
      result = PMask(*ims1, *ims2, *imd);
      goto end;
   }
   if (objs[0]->Type() == Po_Imx3dsf && objs[1]->Type() == Po_Img3duc) {
      Imx3dsf* const ims1 = (Imx3dsf*)objs[0];
      Img3duc* const ims2 = (Img3duc*)objs[1];
      objd[0] = new Imx3dsf(ims1->Props());
      Imx3dsf* const imd = (Imx3dsf*)objd[0];
      
      result = PMask(*ims1, *ims2, *imd);
      goto end;
   }
   if (objs[0]->Type() == Po_Imx3dsf && objs[1]->Type() == Po_Img3dsl) {
      Imx3dsf* const ims1 = (Imx3dsf*)objs[0];
      Img3dsl* const ims2 = (Img3dsl*)objs[1];
      objd[0] = new Imx3dsf(ims1->Props());
      Imx3dsf* const imd = (Imx3dsf*)objd[0];
      
      result = PMask(*ims1, *ims2, *imd);
      goto end;
   }
   if (objs[0]->Type() == Po_Imx3dsf && objs[1]->Type() == Po_Img3dsf) {
      Imx3dsf* const ims1 = (Imx3dsf*)objs[0];
      Img3dsf* const ims2 = (Img3dsf*)objs[1];
      objd[0] = new Imx3dsf(ims1->Props());
      Imx3dsf* const imd = (Imx3dsf*)objd[0];
      
      result = PMask(*ims1, *ims2, *imd);
      goto end;
   }
   if (objs[0]->Type() == Po_Imx3duc && objs[1]->Type() == Po_Reg3d) {
      Imx3duc* const ims1 = (Imx3duc*)objs[0];
      Reg3d* const ims2 = (Reg3d*)objs[1];
      objd[0] = new Imx3duc(ims1->Props());
      Imx3duc* const imd = (Imx3duc*)objd[0];
      
      result = PMask(*ims1, *ims2, *imd);
      goto end;
   }
   if (objs[0]->Type() == Po_Imx3dsl && objs[1]->Type() == Po_Reg3d) {
      Imx3dsl* const ims1 = (Imx3dsl*)objs[0];
      Reg3d* const ims2 = (Reg3d*)objs[1];
      objd[0] = new Imx3dsl(ims1->Props());
      Imx3dsl* const imd = (Imx3dsl*)objd[0];
      
      result = PMask(*ims1, *ims2, *imd);
      goto end;
   }
   if (objs[0]->Type() == Po_Imx3dsf && objs[1]->Type() == Po_Reg3d) {
      Imx3dsf* const ims1 = (Imx3dsf*)objs[0];
      Reg3d* const ims2 = (Reg3d*)objs[1];
      objd[0] = new Imx3dsf(ims1->Props());
      Imx3dsf* const imd = (Imx3dsf*)objd[0];
      
      result = PMask(*ims1, *ims2, *imd);
      goto end;
   }
   if (objs[0]->Type() == Po_Reg2d && objs[1]->Type() == Po_Reg2d) {
      Reg2d* const ims1 = (Reg2d*)objs[0];
      Reg2d* const ims2 = (Reg2d*)objs[1];
      objd[0] = new Reg2d(ims1->Props());
      Reg2d* const imd = (Reg2d*)objd[0];
      
      result = PMask(*ims1, *ims2, *imd);
      goto end;
   }
   if (objs[0]->Type() == Po_Reg3d && objs[1]->Type() == Po_Reg3d) {
      Reg3d* const ims1 = (Reg3d*)objs[0];
      Reg3d* const ims2 = (Reg3d*)objs[1];
      objd[0] = new Reg3d(ims1->Props());
      Reg3d* const imd = (Reg3d*)objd[0];
      
      result = PMask(*ims1, *ims2, *imd);
      goto end;
   }
   if (objs[0]->Type() == Po_Reg2d && objs[1]->Type() == Po_Img2duc) {
      Reg2d* const ims1 = (Reg2d*)objs[0];
      Img2duc* const ims2 = (Img2duc*)objs[1];
      objd[0] = new Reg2d(ims1->Props());
      Reg2d* const imd = (Reg2d*)objd[0];
      
      result = PMask(*ims1, *ims2, *imd);
      goto end;
   }
   if (objs[0]->Type() == Po_Reg3d && objs[1]->Type() == Po_Img3duc) {
      Reg3d* const ims1 = (Reg3d*)objs[0];
      Img3duc* const ims2 = (Img3duc*)objs[1];
      objd[0] = new Reg3d(ims1->Props());
      Reg3d* const imd = (Reg3d*)objd[0];
      
      result = PMask(*ims1, *ims2, *imd);
      goto end;
   }
   if (objs[0]->Type() == Po_Graph2d && objs[1]->Type() == Po_Graph2d) {
      Graph2d* const gs1 = (Graph2d*)objs[0];
      Graph2d* const gs2 = (Graph2d*)objs[1];
      objd[0] = new Graph2d(gs1->Size());
      Graph2d* const gd = (Graph2d*)objd[0];
      
      result = PMask(*gs1, *gs2, *gd);
      goto end;
   }
   if (objs[0]->Type() == Po_Graph3d && objs[1]->Type() == Po_Graph3d) {
      Graph3d* const gs1 = (Graph3d*)objs[0];
      Graph3d* const gs2 = (Graph3d*)objs[1];
      objd[0] = new Graph3d(gs1->Size());
      Graph3d* const gd = (Graph3d*)objd[0];
      
      result = PMask(*gs1, *gs2, *gd);
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
