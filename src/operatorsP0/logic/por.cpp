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
 * @author Régis Clouard - 1995-07-22
 * @author Regis Clouard - 2001-04-11 (version 3.00)
 * @author Regis Clouard - 2004-07-13 (fix bug on region case)
 * @author Regis Clouard - 2005-12-14 (fix bug on region case too)
 * @author Regis Clouard - 2007-02-26 (fix bug on region case again)
 * @author Regis Clouard - 2013-10-11 (fix bug on region or)
 */

#include <pandore.h>
using namespace pandore;

/**
 * @file por.cpp
 * Images and graphs: Binary or operator. Operation: (pix1 | pix2)
 * Regions = UNION.
 */
Errc POr( const Imx3d<Float> &ims1, const Imx3d<Float> &ims2, Imx3d<Float> &imd ) {
   if (ims1.Size() != ims2.Size()) {
      std::cerr << "Error por: Incompatible size" << std::endl;
      return FAILURE;
   }

   for (int b = 0; b < ims1.Bands(); b++) {
      Float *ps1 = ims1.Vector(b);
      Float *ps2 = ims2.Vector(b);
      Float *pd = imd.Vector(b);;
      for (; ps1 < ims1.Vector(b) + ims1.VectorSize(); ps1++, ps2++, pd++) {
	 *pd = *ps1 + *ps2;
      }
   }
   return SUCCESS;
}

template<typename T>
Errc POr( const Imx3d<T> &ims1, const Imx3d<T> &ims2, Imx3d<T> &imd ) {
   if (ims1.Size() != ims2.Size()) {
      std::cerr << "Error por: Incompatible size" << std::endl;
      return FAILURE;
   }
   for (int b = 0; b < ims1.Bands(); b++) {
      T *ps1 = ims1.Vector(b);
      T *ps2 = ims2.Vector(b);
      T *pd = imd.Vector(b);
      for (; ps1 < ims1.Vector(b) + ims1.VectorSize(); ps1++, ps2++, pd++) {
	 *pd = (T)(*ps1 | *ps2);
      }
   }
   return SUCCESS;
}




Errc POr( const Graph2d &grs1, const Graph2d &grs2, Graph2d &grd ) {
   grd = grs1;
   for (int i = 0; i < grd.Size(); i++) {
      if (grs1[i] && grs2[i]) {
	 grd[i]->value = grs1[i]->value + grs2[i]->value;
      } else {
	 if ((grs2[i])) {
	    grd.Add(i, grs2[i]->Item(), grs2[i]->seed);
	    grd[i]->value = grs2[i]->value;
	 }
      }
   }
   return SUCCESS;
}
Errc POr( const Graph3d &grs1, const Graph3d &grs2, Graph3d &grd ) {
   grd = grs1;
   for (int i = 0; i < grd.Size(); i++) {
      if (grs1[i] && grs2[i]) {
	 grd[i]->value = grs1[i]->value + grs2[i]->value;
      } else {
	 if ((grs2[i])) {
	    grd.Add(i, grs2[i]->Item(), grs2[i]->seed);
	    grd[i]->value = grs2[i]->value;
	 }
      }
   }
   return SUCCESS;
}
/**
 * Union between 2 regions.
 * Algorithm : 1) rgs = Region(ims1) + (Region (ims2) + rgs1.Labels())
 *             2) Minimization of the labels.
 */
Errc POr( const Reg2d &rgs1, const Reg2d &rgs2, Reg2d &rgd ) {
   if (rgs1.Size() != rgs2.Size()) {
      std::cerr << "Error por: Incompatible inputs size" << std::endl;
      return FAILURE;
   }

   std::map<Ulong, Reg2d::ValueType> colors;
   Reg2d::ValueType *prgs1 = rgs1.Vector();
   Reg2d::ValueType *prgs2 = rgs2.Vector();
   Reg2d::ValueType *prgd = rgd.Vector();
   const Reg2d::ValueType *end = rgs1.Vector() + rgs1.VectorSize();
   Ulong nbreg = 0;
   const Ulong offset = rgs1.Labels();

   for (; prgs1 < end; prgs1++, prgs2++) {
      Ulong val;
      if (*prgs2 == 0 ) {
	 val = *prgs1;
      } else {
	 val = *prgs1 + *prgs2 + offset;
      }
      if (val == 0) {
	 *(prgd++) = 0;
      } else {
	 if (!colors[val]) {
	    colors[val] = ++nbreg;
	 }
	 *(prgd++) = colors[val];
      }
   }
   
   rgd.Labels(nbreg);
   return nbreg;
}

/**
 * Union between 2 regions.
 * Algorithm : 1) rgs = Region(ims1) + (Region (ims2) + rgs1.Labels())
 *             2) Minimization of the labels.
 */
Errc POr( const Reg3d &rgs1, const Reg3d &rgs2, Reg3d &rgd ) {
   if (rgs1.Size() != rgs2.Size()) {
      std::cerr << "Error por: Incompatible inputs size" << std::endl;
      return FAILURE;
   }

   std::map<Ulong, Reg3d::ValueType> colors;
   Reg3d::ValueType *prgs1 = rgs1.Vector();
   Reg3d::ValueType *prgs2 = rgs2.Vector();
   Reg3d::ValueType *prgd = rgd.Vector();
   const Reg3d::ValueType *end = rgs1.Vector() + rgs1.VectorSize();
   Ulong nbreg = 0;
   const Ulong offset = rgs1.Labels();

   for (; prgs1 < end; prgs1++, prgs2++) {
      Ulong val;
      if (*prgs2 == 0 ) {
	 val = *prgs1;
      } else {
	 val = *prgs1 + *prgs2 + offset;
      }
      if (val == 0) {
	 *(prgd++) = 0;
      } else {
	 if (!colors[val]) {
	    colors[val] = ++nbreg;
	 }
	 *(prgd++) = colors[val];
      }
   }
   
   rgd.Labels(nbreg);
   return nbreg;
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
  if (objs[0]->Type() == Po_Img2duc && objs[1]->Type() == Po_Img2duc) {
    Img2duc* const ims1 = (Img2duc*)objs[0];
    Img2duc* const ims2 = (Img2duc*)objs[1];
    objd[0] = new Img2duc(ims1->Props());

    Img2duc* const imd = (Img2duc*)objd[0];

    result = POr(*ims1, *ims2, *imd);
    goto end;
}
  if (objs[0]->Type() == Po_Img2dsl && objs[1]->Type() == Po_Img2dsl) {
    Img2dsl* const ims1 = (Img2dsl*)objs[0];
    Img2dsl* const ims2 = (Img2dsl*)objs[1];
    objd[0] = new Img2dsl(ims1->Props());

    Img2dsl* const imd = (Img2dsl*)objd[0];

    result = POr(*ims1, *ims2, *imd);
    goto end;
}
  if (objs[0]->Type() == Po_Img2dsf && objs[1]->Type() == Po_Img2dsf) {
    Img2dsf* const ims1 = (Img2dsf*)objs[0];
    Img2dsf* const ims2 = (Img2dsf*)objs[1];
    objd[0] = new Img2dsf(ims1->Props());

    Img2dsf* const imd = (Img2dsf*)objd[0];

    result = POr(*ims1, *ims2, *imd);
    goto end;
}
  if (objs[0]->Type() == Po_Img3duc && objs[1]->Type() == Po_Img3duc) {
    Img3duc* const ims1 = (Img3duc*)objs[0];
    Img3duc* const ims2 = (Img3duc*)objs[1];
    objd[0] = new Img3duc(ims1->Props());

    Img3duc* const imd = (Img3duc*)objd[0];

    result = POr(*ims1, *ims2, *imd);
    goto end;
}
  if (objs[0]->Type() == Po_Img3dsl && objs[1]->Type() == Po_Img3dsl) {
    Img3dsl* const ims1 = (Img3dsl*)objs[0];
    Img3dsl* const ims2 = (Img3dsl*)objs[1];
    objd[0] = new Img3dsl(ims1->Props());

    Img3dsl* const imd = (Img3dsl*)objd[0];

    result = POr(*ims1, *ims2, *imd);
    goto end;
}
  if (objs[0]->Type() == Po_Img3dsf && objs[1]->Type() == Po_Img3dsf) {
    Img3dsf* const ims1 = (Img3dsf*)objs[0];
    Img3dsf* const ims2 = (Img3dsf*)objs[1];
    objd[0] = new Img3dsf(ims1->Props());

    Img3dsf* const imd = (Img3dsf*)objd[0];

    result = POr(*ims1, *ims2, *imd);
    goto end;
}
  if (objs[0]->Type() == Po_Imc2duc && objs[1]->Type() == Po_Imc2duc) {
    Imc2duc* const ims1 = (Imc2duc*)objs[0];
    Imc2duc* const ims2 = (Imc2duc*)objs[1];
    objd[0] = new Imc2duc(ims1->Props());

    Imc2duc* const imd = (Imc2duc*)objd[0];

    result = POr(*ims1, *ims2, *imd);
    goto end;
}
  if (objs[0]->Type() == Po_Imc2dsl && objs[1]->Type() == Po_Imc2dsl) {
    Imc2dsl* const ims1 = (Imc2dsl*)objs[0];
    Imc2dsl* const ims2 = (Imc2dsl*)objs[1];
    objd[0] = new Imc2dsl(ims1->Props());

    Imc2dsl* const imd = (Imc2dsl*)objd[0];

    result = POr(*ims1, *ims2, *imd);
    goto end;
}
  if (objs[0]->Type() == Po_Imc2dsf && objs[1]->Type() == Po_Imc2dsf) {
    Imc2dsf* const ims1 = (Imc2dsf*)objs[0];
    Imc2dsf* const ims2 = (Imc2dsf*)objs[1];
    objd[0] = new Imc2dsf(ims1->Props());

    Imc2dsf* const imd = (Imc2dsf*)objd[0];

    result = POr(*ims1, *ims2, *imd);
    goto end;
}
  if (objs[0]->Type() == Po_Imc3duc && objs[1]->Type() == Po_Imc3duc) {
    Imc3duc* const ims1 = (Imc3duc*)objs[0];
    Imc3duc* const ims2 = (Imc3duc*)objs[1];
    objd[0] = new Imc3duc(ims1->Props());

    Imc3duc* const imd = (Imc3duc*)objd[0];

    result = POr(*ims1, *ims2, *imd);
    goto end;
}
  if (objs[0]->Type() == Po_Imc3dsl && objs[1]->Type() == Po_Imc3dsl) {
    Imc3dsl* const ims1 = (Imc3dsl*)objs[0];
    Imc3dsl* const ims2 = (Imc3dsl*)objs[1];
    objd[0] = new Imc3dsl(ims1->Props());

    Imc3dsl* const imd = (Imc3dsl*)objd[0];

    result = POr(*ims1, *ims2, *imd);
    goto end;
}
  if (objs[0]->Type() == Po_Imc3dsf && objs[1]->Type() == Po_Imc3dsf) {
    Imc3dsf* const ims1 = (Imc3dsf*)objs[0];
    Imc3dsf* const ims2 = (Imc3dsf*)objs[1];
    objd[0] = new Imc3dsf(ims1->Props());

    Imc3dsf* const imd = (Imc3dsf*)objd[0];

    result = POr(*ims1, *ims2, *imd);
    goto end;
}
  if (objs[0]->Type() == Po_Imx2duc && objs[1]->Type() == Po_Imx2duc) {
    Imx2duc* const ims1 = (Imx2duc*)objs[0];
    Imx2duc* const ims2 = (Imx2duc*)objs[1];
    objd[0] = new Imx2duc(ims1->Props());

    Imx2duc* const imd = (Imx2duc*)objd[0];

    result = POr(*ims1, *ims2, *imd);
    goto end;
}
  if (objs[0]->Type() == Po_Imx2dsl && objs[1]->Type() == Po_Imx2dsl) {
    Imx2dsl* const ims1 = (Imx2dsl*)objs[0];
    Imx2dsl* const ims2 = (Imx2dsl*)objs[1];
    objd[0] = new Imx2dsl(ims1->Props());

    Imx2dsl* const imd = (Imx2dsl*)objd[0];

    result = POr(*ims1, *ims2, *imd);
    goto end;
}
  if (objs[0]->Type() == Po_Imx2dsf && objs[1]->Type() == Po_Imx2dsf) {
    Imx2dsf* const ims1 = (Imx2dsf*)objs[0];
    Imx2dsf* const ims2 = (Imx2dsf*)objs[1];
    objd[0] = new Imx2dsf(ims1->Props());

    Imx2dsf* const imd = (Imx2dsf*)objd[0];

    result = POr(*ims1, *ims2, *imd);
    goto end;
}
  if (objs[0]->Type() == Po_Imx3duc && objs[1]->Type() == Po_Imx3duc) {
    Imx3duc* const ims1 = (Imx3duc*)objs[0];
    Imx3duc* const ims2 = (Imx3duc*)objs[1];
    objd[0] = new Imx3duc(ims1->Props());

    Imx3duc* const imd = (Imx3duc*)objd[0];

    result = POr(*ims1, *ims2, *imd);
    goto end;
}
  if (objs[0]->Type() == Po_Imx3dsl && objs[1]->Type() == Po_Imx3dsl) {
    Imx3dsl* const ims1 = (Imx3dsl*)objs[0];
    Imx3dsl* const ims2 = (Imx3dsl*)objs[1];
    objd[0] = new Imx3dsl(ims1->Props());

    Imx3dsl* const imd = (Imx3dsl*)objd[0];

    result = POr(*ims1, *ims2, *imd);
    goto end;
}
  if (objs[0]->Type() == Po_Imx3dsf && objs[1]->Type() == Po_Imx3dsf) {
    Imx3dsf* const ims1 = (Imx3dsf*)objs[0];
    Imx3dsf* const ims2 = (Imx3dsf*)objs[1];
    objd[0] = new Imx3dsf(ims1->Props());

    Imx3dsf* const imd = (Imx3dsf*)objd[0];

    result = POr(*ims1, *ims2, *imd);
    goto end;
}
   if (objs[0]->Type() ==  Po_Graph2d && objs[1]->Type() == Po_Graph2d) {
      Graph2d* const gs1 = (Graph2d*)objs[0];
      Graph2d* const gs2 = (Graph2d*)objs[1];
      objd[0] = new Graph2d(gs1->Size());
      Graph2d* const gd = (Graph2d*)objd[0];
      
      result = POr(*gs1, *gs2, *gd);
      goto end;
   }
   if (objs[0]->Type() ==  Po_Graph3d && objs[1]->Type() == Po_Graph3d) {
      Graph3d* const gs1 = (Graph3d*)objs[0];
      Graph3d* const gs2 = (Graph3d*)objs[1];
      objd[0] = new Graph3d(gs1->Size());
      Graph3d* const gd = (Graph3d*)objd[0];
      
      result = POr(*gs1, *gs2, *gd);
      goto end;
   }
   if (objs[0]->Type() == Po_Reg2d && objs[1]->Type() == Po_Reg2d) {
      Reg2d* const rgs1 = (Reg2d*)objs[0];
      Reg2d* const rgs2 = (Reg2d*)objs[1];
      objd[0] = new Reg2d(rgs1->Props());
 
      Reg2d* const rgd = (Reg2d*)objd[0];
      result = POr(*rgs1, *rgs2, *rgd);
      goto end;
   }
   if (objs[0]->Type() == Po_Reg3d && objs[1]->Type() == Po_Reg3d) {
      Reg3d* const rgs1 = (Reg3d*)objs[0];
      Reg3d* const rgs2 = (Reg3d*)objs[1];
      objd[0] = new Reg3d(rgs1->Props());
 
      Reg3d* const rgd = (Reg3d*)objd[0];
      result = POr(*rgs1, *rgs2, *rgd);
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
