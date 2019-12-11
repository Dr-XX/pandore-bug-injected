/* -*- c-basic-offset: 3; mode:c++ -*-
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
 * @file psub.cpp
 * Binary soustraction operator.
 */

template <typename T1, typename T2>
Errc PSub( const Imx3d<T1> &ims1, const Imx3d<T1> &ims2, Imx3d<T2> &imd ) {
   if (ims1.Size() != ims2.Size() || ims1.Bands() != ims2.Bands()) {
      std::cerr << "Error psub: Incompatible image size." << std::endl;
      return FAILURE;
   }
   for (int b = 0; b < ims1.Bands(); b++) {
      T1 *ps1 = ims1.Vector(b);
      T1 *ps2 = ims2.Vector(b);
      T2 *pd = imd.Vector(b);
      
      for ( ; ps1 < ims1.Vector(b) + ims1.VectorSize(); ps1++, ps2++, pd++) {   
	 *pd = (T2)((Double)*ps1 - (Double)*ps2);
      }
   }
   return SUCCESS;
}






/**
 * Between 2 graphs (nodes).
 */
Errc PSub( const Graph2d &gs1, const Graph2d &gs2, Graph2d &gd ) {
   gd = gs1;
   for (int i = 0; i < gd.Size(); i++) {
      gd[i]->value = gs1[i]->value - gs2[i]->value;
   }
   return SUCCESS;
}
/**
 * Between 2 graphs (nodes).
 */
Errc PSub( const Graph3d &gs1, const Graph3d &gs2, Graph3d &gd ) {
   gd = gs1;
   for (int i = 0; i < gd.Size(); i++) {
      gd[i]->value = gs1[i]->value - gs2[i]->value;
   }
   return SUCCESS;
}
Errc PSub( const Reg1d &rgs1, const Reg1d &rgs2, Reg1d &rgd ) {
   Reg1d::ValueType *ps1 = rgs1.Vector();
   Reg1d::ValueType *pend = ps1 + rgs1.VectorSize();
   Reg1d::ValueType *ps2 = rgs2.Vector();
   Reg1d::ValueType *pd = rgd.Vector();

   Ulong nbreg = 0;
   Reg1d::ValueType* t1 = (Reg1d::ValueType*)calloc(rgs1.VectorSize() + 1, sizeof(Reg1d::ValueType));
   for ( ; ps1 < pend; ps1++, ps2++, pd++) {   
      if (*ps2 == 0) {
	 if (*ps1 == 0 || t1[*ps1]) {
	    *pd = t1[*ps1];
	 } else {
	    t1[*ps1] = (Reg1d::ValueType)++nbreg;
	    *pd = t1[*ps1];
	 }
      } else {
	 *pd = 0;
      }
   }
    free(t1);
   
    if (nbreg > MAXLONG) {
       std::cerr << "Error psub: Too many regions." << std::endl;
       return FAILURE;
    }

    return rgd.Labels(nbreg);
}

Errc PSub( const Reg2d &rgs1, const Reg2d &rgs2, Reg2d &rgd ) {
   Reg2d::ValueType *ps1 = rgs1.Vector();
   Reg2d::ValueType *pend = ps1 + rgs1.VectorSize();
   Reg2d::ValueType *ps2 = rgs2.Vector();
   Reg2d::ValueType *pd = rgd.Vector();

   Ulong nbreg = 0;
   Reg2d::ValueType* t1 = (Reg2d::ValueType*)calloc(rgs1.VectorSize() + 1, sizeof(Reg2d::ValueType));
   for ( ; ps1 < pend; ps1++, ps2++, pd++) {   
      if (*ps2 == 0) {
	 if (*ps1 == 0 || t1[*ps1]) {
	    *pd = t1[*ps1];
	 } else {
	    t1[*ps1] = (Reg2d::ValueType)++nbreg;
	    *pd = t1[*ps1];
	 }
      } else {
	 *pd = 0;
      }
   }
    free(t1);
   
    if (nbreg > MAXLONG) {
       std::cerr << "Error psub: Too many regions." << std::endl;
       return FAILURE;
    }

    return rgd.Labels(nbreg);
}

Errc PSub( const Reg3d &rgs1, const Reg3d &rgs2, Reg3d &rgd ) {
   Reg3d::ValueType *ps1 = rgs1.Vector();
   Reg3d::ValueType *pend = ps1 + rgs1.VectorSize();
   Reg3d::ValueType *ps2 = rgs2.Vector();
   Reg3d::ValueType *pd = rgd.Vector();

   Ulong nbreg = 0;
   Reg3d::ValueType* t1 = (Reg3d::ValueType*)calloc(rgs1.VectorSize() + 1, sizeof(Reg3d::ValueType));
   for ( ; ps1 < pend; ps1++, ps2++, pd++) {   
      if (*ps2 == 0) {
	 if (*ps1 == 0 || t1[*ps1]) {
	    *pd = t1[*ps1];
	 } else {
	    t1[*ps1] = (Reg3d::ValueType)++nbreg;
	    *pd = t1[*ps1];
	 }
      } else {
	 *pd = 0;
      }
   }
    free(t1);
   
    if (nbreg > MAXLONG) {
       std::cerr << "Error psub: Too many regions." << std::endl;
       return FAILURE;
    }

    return rgd.Labels(nbreg);
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
   if ((objs[0]->Type() == Po_Img1duc) && (objs[1]->Type() == Po_Img1duc)) {
      Img1duc* const ims1 = (Img1duc*)objs[0];
      Img1duc* const ims2 = (Img1duc*)objs[1];
      objd[0] = new Select<Img1duc,Img1duc>::LargestSigned(ims1->Props());
      
      Select<Img1duc,Img1duc>::LargestSigned* const imd = (Select<Img1duc,Img1duc>::LargestSigned*)objd[0];
      
      result = PSub(*ims1, *ims2, *imd);
      goto end;
   }
   if ((objs[0]->Type() == Po_Img1dsl) && (objs[1]->Type() == Po_Img1dsl)) {
      Img1dsl* const ims1 = (Img1dsl*)objs[0];
      Img1dsl* const ims2 = (Img1dsl*)objs[1];
      objd[0] = new Select<Img1dsl,Img1dsl>::LargestSigned(ims1->Props());
      
      Select<Img1dsl,Img1dsl>::LargestSigned* const imd = (Select<Img1dsl,Img1dsl>::LargestSigned*)objd[0];
      
      result = PSub(*ims1, *ims2, *imd);
      goto end;
   }
   if ((objs[0]->Type() == Po_Img1dsf) && (objs[1]->Type() == Po_Img1dsf)) {
      Img1dsf* const ims1 = (Img1dsf*)objs[0];
      Img1dsf* const ims2 = (Img1dsf*)objs[1];
      objd[0] = new Select<Img1dsf,Img1dsf>::LargestSigned(ims1->Props());
      
      Select<Img1dsf,Img1dsf>::LargestSigned* const imd = (Select<Img1dsf,Img1dsf>::LargestSigned*)objd[0];
      
      result = PSub(*ims1, *ims2, *imd);
      goto end;
   }
   if ((objs[0]->Type() == Po_Img2duc) && (objs[1]->Type() == Po_Img2duc)) {
      Img2duc* const ims1 = (Img2duc*)objs[0];
      Img2duc* const ims2 = (Img2duc*)objs[1];
      objd[0] = new Select<Img2duc,Img2duc>::LargestSigned(ims1->Props());
      
      Select<Img2duc,Img2duc>::LargestSigned* const imd = (Select<Img2duc,Img2duc>::LargestSigned*)objd[0];
      
      result = PSub(*ims1, *ims2, *imd);
      goto end;
   }
   if ((objs[0]->Type() == Po_Img2dsl) && (objs[1]->Type() == Po_Img2dsl)) {
      Img2dsl* const ims1 = (Img2dsl*)objs[0];
      Img2dsl* const ims2 = (Img2dsl*)objs[1];
      objd[0] = new Select<Img2dsl,Img2dsl>::LargestSigned(ims1->Props());
      
      Select<Img2dsl,Img2dsl>::LargestSigned* const imd = (Select<Img2dsl,Img2dsl>::LargestSigned*)objd[0];
      
      result = PSub(*ims1, *ims2, *imd);
      goto end;
   }
   if ((objs[0]->Type() == Po_Img2dsf) && (objs[1]->Type() == Po_Img2dsf)) {
      Img2dsf* const ims1 = (Img2dsf*)objs[0];
      Img2dsf* const ims2 = (Img2dsf*)objs[1];
      objd[0] = new Select<Img2dsf,Img2dsf>::LargestSigned(ims1->Props());
      
      Select<Img2dsf,Img2dsf>::LargestSigned* const imd = (Select<Img2dsf,Img2dsf>::LargestSigned*)objd[0];
      
      result = PSub(*ims1, *ims2, *imd);
      goto end;
   }
   if ((objs[0]->Type() == Po_Img3duc) && (objs[1]->Type() == Po_Img3duc)) {
      Img3duc* const ims1 = (Img3duc*)objs[0];
      Img3duc* const ims2 = (Img3duc*)objs[1];
      objd[0] = new Select<Img3duc,Img3duc>::LargestSigned(ims1->Props());
      
      Select<Img3duc,Img3duc>::LargestSigned* const imd = (Select<Img3duc,Img3duc>::LargestSigned*)objd[0];
      
      result = PSub(*ims1, *ims2, *imd);
      goto end;
   }
   if ((objs[0]->Type() == Po_Img3dsl) && (objs[1]->Type() == Po_Img3dsl)) {
      Img3dsl* const ims1 = (Img3dsl*)objs[0];
      Img3dsl* const ims2 = (Img3dsl*)objs[1];
      objd[0] = new Select<Img3dsl,Img3dsl>::LargestSigned(ims1->Props());
      
      Select<Img3dsl,Img3dsl>::LargestSigned* const imd = (Select<Img3dsl,Img3dsl>::LargestSigned*)objd[0];
      
      result = PSub(*ims1, *ims2, *imd);
      goto end;
   }
   if ((objs[0]->Type() == Po_Img3dsf) && (objs[1]->Type() == Po_Img3dsf)) {
      Img3dsf* const ims1 = (Img3dsf*)objs[0];
      Img3dsf* const ims2 = (Img3dsf*)objs[1];
      objd[0] = new Select<Img3dsf,Img3dsf>::LargestSigned(ims1->Props());
      
      Select<Img3dsf,Img3dsf>::LargestSigned* const imd = (Select<Img3dsf,Img3dsf>::LargestSigned*)objd[0];
      
      result = PSub(*ims1, *ims2, *imd);
      goto end;
   }
   if ((objs[0]->Type() == Po_Imc2duc) && (objs[1]->Type() == Po_Imc2duc)) {
      Imc2duc* const ims1 = (Imc2duc*)objs[0];
      Imc2duc* const ims2 = (Imc2duc*)objs[1];
      objd[0] = new Select<Imc2duc,Imc2duc>::LargestSigned(ims1->Props());
      
      Select<Imc2duc,Imc2duc>::LargestSigned* const imd = (Select<Imc2duc,Imc2duc>::LargestSigned*)objd[0];
      
      result = PSub(*ims1, *ims2, *imd);
      goto end;
   }
   if ((objs[0]->Type() == Po_Imc2dsl) && (objs[1]->Type() == Po_Imc2dsl)) {
      Imc2dsl* const ims1 = (Imc2dsl*)objs[0];
      Imc2dsl* const ims2 = (Imc2dsl*)objs[1];
      objd[0] = new Select<Imc2dsl,Imc2dsl>::LargestSigned(ims1->Props());
      
      Select<Imc2dsl,Imc2dsl>::LargestSigned* const imd = (Select<Imc2dsl,Imc2dsl>::LargestSigned*)objd[0];
      
      result = PSub(*ims1, *ims2, *imd);
      goto end;
   }
   if ((objs[0]->Type() == Po_Imc2dsf) && (objs[1]->Type() == Po_Imc2dsf)) {
      Imc2dsf* const ims1 = (Imc2dsf*)objs[0];
      Imc2dsf* const ims2 = (Imc2dsf*)objs[1];
      objd[0] = new Select<Imc2dsf,Imc2dsf>::LargestSigned(ims1->Props());
      
      Select<Imc2dsf,Imc2dsf>::LargestSigned* const imd = (Select<Imc2dsf,Imc2dsf>::LargestSigned*)objd[0];
      
      result = PSub(*ims1, *ims2, *imd);
      goto end;
   }
   if ((objs[0]->Type() == Po_Imc3duc) && (objs[1]->Type() == Po_Imc3duc)) {
      Imc3duc* const ims1 = (Imc3duc*)objs[0];
      Imc3duc* const ims2 = (Imc3duc*)objs[1];
      objd[0] = new Select<Imc3duc,Imc3duc>::LargestSigned(ims1->Props());
      
      Select<Imc3duc,Imc3duc>::LargestSigned* const imd = (Select<Imc3duc,Imc3duc>::LargestSigned*)objd[0];
      
      result = PSub(*ims1, *ims2, *imd);
      goto end;
   }
   if ((objs[0]->Type() == Po_Imc3dsl) && (objs[1]->Type() == Po_Imc3dsl)) {
      Imc3dsl* const ims1 = (Imc3dsl*)objs[0];
      Imc3dsl* const ims2 = (Imc3dsl*)objs[1];
      objd[0] = new Select<Imc3dsl,Imc3dsl>::LargestSigned(ims1->Props());
      
      Select<Imc3dsl,Imc3dsl>::LargestSigned* const imd = (Select<Imc3dsl,Imc3dsl>::LargestSigned*)objd[0];
      
      result = PSub(*ims1, *ims2, *imd);
      goto end;
   }
   if ((objs[0]->Type() == Po_Imc3dsf) && (objs[1]->Type() == Po_Imc3dsf)) {
      Imc3dsf* const ims1 = (Imc3dsf*)objs[0];
      Imc3dsf* const ims2 = (Imc3dsf*)objs[1];
      objd[0] = new Select<Imc3dsf,Imc3dsf>::LargestSigned(ims1->Props());
      
      Select<Imc3dsf,Imc3dsf>::LargestSigned* const imd = (Select<Imc3dsf,Imc3dsf>::LargestSigned*)objd[0];
      
      result = PSub(*ims1, *ims2, *imd);
      goto end;
   }
   if ((objs[0]->Type() == Po_Imx1duc) && (objs[1]->Type() == Po_Imx1duc)) {
      Imx1duc* const ims1 = (Imx1duc*)objs[0];
      Imx1duc* const ims2 = (Imx1duc*)objs[1];
      objd[0] = new Select<Imx1duc,Imx1duc>::LargestSigned(ims1->Props());
      
      Select<Imx1duc,Imx1duc>::LargestSigned* const imd = (Select<Imx1duc,Imx1duc>::LargestSigned*)objd[0];
      
      result = PSub(*ims1, *ims2, *imd);
      goto end;
   }
   if ((objs[0]->Type() == Po_Imx1dsl) && (objs[1]->Type() == Po_Imx1dsl)) {
      Imx1dsl* const ims1 = (Imx1dsl*)objs[0];
      Imx1dsl* const ims2 = (Imx1dsl*)objs[1];
      objd[0] = new Select<Imx1dsl,Imx1dsl>::LargestSigned(ims1->Props());
      
      Select<Imx1dsl,Imx1dsl>::LargestSigned* const imd = (Select<Imx1dsl,Imx1dsl>::LargestSigned*)objd[0];
      
      result = PSub(*ims1, *ims2, *imd);
      goto end;
   }
   if ((objs[0]->Type() == Po_Imx1dsf) && (objs[1]->Type() == Po_Imx1dsf)) {
      Imx1dsf* const ims1 = (Imx1dsf*)objs[0];
      Imx1dsf* const ims2 = (Imx1dsf*)objs[1];
      objd[0] = new Select<Imx1dsf,Imx1dsf>::LargestSigned(ims1->Props());
      
      Select<Imx1dsf,Imx1dsf>::LargestSigned* const imd = (Select<Imx1dsf,Imx1dsf>::LargestSigned*)objd[0];
      
      result = PSub(*ims1, *ims2, *imd);
      goto end;
   }
   if ((objs[0]->Type() == Po_Imx2duc) && (objs[1]->Type() == Po_Imx2duc)) {
      Imx2duc* const ims1 = (Imx2duc*)objs[0];
      Imx2duc* const ims2 = (Imx2duc*)objs[1];
      objd[0] = new Select<Imx2duc,Imx2duc>::LargestSigned(ims1->Props());
      
      Select<Imx2duc,Imx2duc>::LargestSigned* const imd = (Select<Imx2duc,Imx2duc>::LargestSigned*)objd[0];
      
      result = PSub(*ims1, *ims2, *imd);
      goto end;
   }
   if ((objs[0]->Type() == Po_Imx2dsl) && (objs[1]->Type() == Po_Imx2dsl)) {
      Imx2dsl* const ims1 = (Imx2dsl*)objs[0];
      Imx2dsl* const ims2 = (Imx2dsl*)objs[1];
      objd[0] = new Select<Imx2dsl,Imx2dsl>::LargestSigned(ims1->Props());
      
      Select<Imx2dsl,Imx2dsl>::LargestSigned* const imd = (Select<Imx2dsl,Imx2dsl>::LargestSigned*)objd[0];
      
      result = PSub(*ims1, *ims2, *imd);
      goto end;
   }
   if ((objs[0]->Type() == Po_Imx2dsf) && (objs[1]->Type() == Po_Imx2dsf)) {
      Imx2dsf* const ims1 = (Imx2dsf*)objs[0];
      Imx2dsf* const ims2 = (Imx2dsf*)objs[1];
      objd[0] = new Select<Imx2dsf,Imx2dsf>::LargestSigned(ims1->Props());
      
      Select<Imx2dsf,Imx2dsf>::LargestSigned* const imd = (Select<Imx2dsf,Imx2dsf>::LargestSigned*)objd[0];
      
      result = PSub(*ims1, *ims2, *imd);
      goto end;
   }
   if ((objs[0]->Type() == Po_Imx3duc) && (objs[1]->Type() == Po_Imx3duc)) {
      Imx3duc* const ims1 = (Imx3duc*)objs[0];
      Imx3duc* const ims2 = (Imx3duc*)objs[1];
      objd[0] = new Select<Imx3duc,Imx3duc>::LargestSigned(ims1->Props());
      
      Select<Imx3duc,Imx3duc>::LargestSigned* const imd = (Select<Imx3duc,Imx3duc>::LargestSigned*)objd[0];
      
      result = PSub(*ims1, *ims2, *imd);
      goto end;
   }
   if ((objs[0]->Type() == Po_Imx3dsl) && (objs[1]->Type() == Po_Imx3dsl)) {
      Imx3dsl* const ims1 = (Imx3dsl*)objs[0];
      Imx3dsl* const ims2 = (Imx3dsl*)objs[1];
      objd[0] = new Select<Imx3dsl,Imx3dsl>::LargestSigned(ims1->Props());
      
      Select<Imx3dsl,Imx3dsl>::LargestSigned* const imd = (Select<Imx3dsl,Imx3dsl>::LargestSigned*)objd[0];
      
      result = PSub(*ims1, *ims2, *imd);
      goto end;
   }
   if ((objs[0]->Type() == Po_Imx3dsf) && (objs[1]->Type() == Po_Imx3dsf)) {
      Imx3dsf* const ims1 = (Imx3dsf*)objs[0];
      Imx3dsf* const ims2 = (Imx3dsf*)objs[1];
      objd[0] = new Select<Imx3dsf,Imx3dsf>::LargestSigned(ims1->Props());
      
      Select<Imx3dsf,Imx3dsf>::LargestSigned* const imd = (Select<Imx3dsf,Imx3dsf>::LargestSigned*)objd[0];
      
      result = PSub(*ims1, *ims2, *imd);
      goto end;
   }
   if ((objs[0]->Type() == Po_Graph2d) && (objs[1]->Type() == Po_Graph2d)) {
      Graph2d* const gs1 = (Graph2d*)objs[0];
      Graph2d* const gs2 = (Graph2d*)objs[1];
      objd[0] = new Graph2d(gs1->Size());
      Graph2d* const gd = (Graph2d*)objd[0];
      
      result = PSub(*gs1, *gs2, *gd);
      goto end;
   }
   if ((objs[0]->Type() == Po_Graph3d) && (objs[1]->Type() == Po_Graph3d)) {
      Graph3d* const gs1 = (Graph3d*)objs[0];
      Graph3d* const gs2 = (Graph3d*)objs[1];
      objd[0] = new Graph3d(gs1->Size());
      Graph3d* const gd = (Graph3d*)objd[0];
      
      result = PSub(*gs1, *gs2, *gd);
      goto end;
   }
   if (objs[0]->Type() == Po_Reg1d) {
      Reg1d* const rgs1 = (Reg1d*)objs[0];
      Reg1d* const rgs2 = (Reg1d*)objs[1];
      objd[0] = new Reg1d(rgs1->Size());
      
      Reg1d* const rgd = (Reg1d*)objd[0];
      
      result = PSub(*rgs1, *rgs2, *rgd);
      goto end;
   }
   if (objs[0]->Type() == Po_Reg2d) {
      Reg2d* const rgs1 = (Reg2d*)objs[0];
      Reg2d* const rgs2 = (Reg2d*)objs[1];
      objd[0] = new Reg2d(rgs1->Size());
      
      Reg2d* const rgd = (Reg2d*)objd[0];
      
      result = PSub(*rgs1, *rgs2, *rgd);
      goto end;
   }
   if (objs[0]->Type() == Po_Reg3d) {
      Reg3d* const rgs1 = (Reg3d*)objs[0];
      Reg3d* const rgs2 = (Reg3d*)objs[1];
      objd[0] = new Reg3d(rgs1->Size());
      
      Reg3d* const rgd = (Reg3d*)objd[0];
      
      result = PSub(*rgs1, *rgs2, *rgd);
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
