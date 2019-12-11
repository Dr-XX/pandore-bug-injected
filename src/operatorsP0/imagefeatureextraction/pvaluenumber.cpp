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
 * @author Régis Clouard - 1997-07-23
 */

#include <pandore.h>
using namespace pandore;

/**
 * @file pvaluenumber.cpp
 * Calculates the number of non null pixel values.
 */



Ulong PValueNumber( const Img1duc &ims ) {
   Img1duc::ValueType *ps=ims.Vector();
   const Img1duc::ValueType *pf=ims.Vector()+ims.VectorSize();
   
   Ulong nb=0;
   
   for ( ; ps<pf ; ) {
      if ((*ps++)) nb++;
   }
   return nb;
}

Ulong PValueNumber( const Img1dsl &ims ) {
   Img1dsl::ValueType *ps=ims.Vector();
   const Img1dsl::ValueType *pf=ims.Vector()+ims.VectorSize();
   
   Ulong nb=0;
   
   for ( ; ps<pf ; ) {
      if ((*ps++)) nb++;
   }
   return nb;
}

Ulong PValueNumber( const Img1dsf &ims ) {
   Img1dsf::ValueType *ps=ims.Vector();
   const Img1dsf::ValueType *pf=ims.Vector()+ims.VectorSize();
   
   Ulong nb=0;
   
   for ( ; ps<pf ; ) {
      if ((*ps++)) nb++;
   }
   return nb;
}

Ulong PValueNumber( const Img2duc &ims ) {
   Img2duc::ValueType *ps=ims.Vector();
   const Img2duc::ValueType *pf=ims.Vector()+ims.VectorSize();
   
   Ulong nb=0;
   
   for ( ; ps<pf ; ) {
      if ((*ps++)) nb++;
   }
   return nb;
}

Ulong PValueNumber( const Img2dsl &ims ) {
   Img2dsl::ValueType *ps=ims.Vector();
   const Img2dsl::ValueType *pf=ims.Vector()+ims.VectorSize();
   
   Ulong nb=0;
   
   for ( ; ps<pf ; ) {
      if ((*ps++)) nb++;
   }
   return nb;
}

Ulong PValueNumber( const Img2dsf &ims ) {
   Img2dsf::ValueType *ps=ims.Vector();
   const Img2dsf::ValueType *pf=ims.Vector()+ims.VectorSize();
   
   Ulong nb=0;
   
   for ( ; ps<pf ; ) {
      if ((*ps++)) nb++;
   }
   return nb;
}

Ulong PValueNumber( const Img3duc &ims ) {
   Img3duc::ValueType *ps=ims.Vector();
   const Img3duc::ValueType *pf=ims.Vector()+ims.VectorSize();
   
   Ulong nb=0;
   
   for ( ; ps<pf ; ) {
      if ((*ps++)) nb++;
   }
   return nb;
}

Ulong PValueNumber( const Img3dsl &ims ) {
   Img3dsl::ValueType *ps=ims.Vector();
   const Img3dsl::ValueType *pf=ims.Vector()+ims.VectorSize();
   
   Ulong nb=0;
   
   for ( ; ps<pf ; ) {
      if ((*ps++)) nb++;
   }
   return nb;
}

Ulong PValueNumber( const Img3dsf &ims ) {
   Img3dsf::ValueType *ps=ims.Vector();
   const Img3dsf::ValueType *pf=ims.Vector()+ims.VectorSize();
   
   Ulong nb=0;
   
   for ( ; ps<pf ; ) {
      if ((*ps++)) nb++;
   }
   return nb;
}

Ulong PValueNumber( const Reg1d &ims ) {
   Reg1d::ValueType *ps=ims.Vector();
   const Reg1d::ValueType *pf=ims.Vector()+ims.VectorSize();
   
   Ulong nb=0;
   
   for ( ; ps<pf ; ) {
      if ((*ps++)) nb++;
   }
   return nb;
}

Ulong PValueNumber( const Reg2d &ims ) {
   Reg2d::ValueType *ps=ims.Vector();
   const Reg2d::ValueType *pf=ims.Vector()+ims.VectorSize();
   
   Ulong nb=0;
   
   for ( ; ps<pf ; ) {
      if ((*ps++)) nb++;
   }
   return nb;
}

Ulong PValueNumber( const Reg3d &ims ) {
   Reg3d::ValueType *ps=ims.Vector();
   const Reg3d::ValueType *pf=ims.Vector()+ims.VectorSize();
   
   Ulong nb=0;
   
   for ( ; ps<pf ; ) {
      if ((*ps++)) nb++;
   }
   return nb;
}

Ulong PValueNumber( const Imc2duc &ims ) {
   Ulong nb=0;
   Imc2duc::ValueType *px=ims.VectorX();
   Imc2duc::ValueType *py=ims.VectorY();
   Imc2duc::ValueType *pz=ims.VectorZ();
   const Imc2duc::ValueType *pf=ims.VectorX()+ims.VectorSize();
   
   for (;px<pf; px++,py++,pz++) {
      if ((*px) && (py) && (pz)) nb++;
   }
   return nb;
}

Ulong PValueNumber( const Imc2dsl &ims ) {
   Ulong nb=0;
   Imc2dsl::ValueType *px=ims.VectorX();
   Imc2dsl::ValueType *py=ims.VectorY();
   Imc2dsl::ValueType *pz=ims.VectorZ();
   const Imc2dsl::ValueType *pf=ims.VectorX()+ims.VectorSize();
   
   for (;px<pf; px++,py++,pz++) {
      if ((*px) && (py) && (pz)) nb++;
   }
   return nb;
}

Ulong PValueNumber( const Imc2dsf &ims ) {
   Ulong nb=0;
   Imc2dsf::ValueType *px=ims.VectorX();
   Imc2dsf::ValueType *py=ims.VectorY();
   Imc2dsf::ValueType *pz=ims.VectorZ();
   const Imc2dsf::ValueType *pf=ims.VectorX()+ims.VectorSize();
   
   for (;px<pf; px++,py++,pz++) {
      if ((*px) && (py) && (pz)) nb++;
   }
   return nb;
}

Ulong PValueNumber( const Imc3duc &ims ) {
   Ulong nb=0;
   Imc3duc::ValueType *px=ims.VectorX();
   Imc3duc::ValueType *py=ims.VectorY();
   Imc3duc::ValueType *pz=ims.VectorZ();
   const Imc3duc::ValueType *pf=ims.VectorX()+ims.VectorSize();
   
   for (;px<pf; px++,py++,pz++) {
      if ((*px) && (py) && (pz)) nb++;
   }
   return nb;
}

Ulong PValueNumber( const Imc3dsl &ims ) {
   Ulong nb=0;
   Imc3dsl::ValueType *px=ims.VectorX();
   Imc3dsl::ValueType *py=ims.VectorY();
   Imc3dsl::ValueType *pz=ims.VectorZ();
   const Imc3dsl::ValueType *pf=ims.VectorX()+ims.VectorSize();
   
   for (;px<pf; px++,py++,pz++) {
      if ((*px) && (py) && (pz)) nb++;
   }
   return nb;
}

Ulong PValueNumber( const Imc3dsf &ims ) {
   Ulong nb=0;
   Imc3dsf::ValueType *px=ims.VectorX();
   Imc3dsf::ValueType *py=ims.VectorY();
   Imc3dsf::ValueType *pz=ims.VectorZ();
   const Imc3dsf::ValueType *pf=ims.VectorX()+ims.VectorSize();
   
   for (;px<pf; px++,py++,pz++) {
      if ((*px) && (py) && (pz)) nb++;
   }
   return nb;
}

Ulong PValueNumber( const Graph2d &grs ) {
   Ulong som =0;  
   
   for (int i=0;i<grs.Size();i++)
      if ((grs[i]) && (grs[i]->value))
	 som++;
   
   return som;
}

Ulong PValueNumber( const Graph3d &grs ) {
   Ulong som =0;  
   
   for (int i=0;i<grs.Size();i++)
      if ((grs[i]) && (grs[i]->value))
	 som++;
   
   return som;
}


#ifdef MAIN
#define USAGE	"usage: %s [-m mask] [im_in|-]"
#define PARC	0
#define FINC	1
#define FOUTC	0
#define MASK	1

int main( int argc, char *argv[] ) {
   Errc result;                // The result code of the execution.
   Pobject* mask;              // The region map.
   Pobject* objin[FINC + 1];   // The input objects.
   Pobject* objs[FINC + 1];    // The source objects masked.
   Pobject* objout[FOUTC + 1]; // The output object.
   Pobject* objd[FOUTC + 1];   // The result object of the execution.
   char* parv[PARC + 1];       // The input parameters.

   ReadArgs(argc, argv, PARC, FINC, FOUTC, &mask, objin, objs, objout, objd, parv, USAGE, MASK); 
   if (objs[0]->Type() == Po_Img1duc) {
      Img1duc* const ims=(Img1duc*)objs[0];
      
      result= PValueNumber(*ims);
      goto end;
   }
   if (objs[0]->Type() == Po_Img1dsl) {
      Img1dsl* const ims=(Img1dsl*)objs[0];
      
      result= PValueNumber(*ims);
      goto end;
   }
   if (objs[0]->Type() == Po_Img1dsf) {
      Img1dsf* const ims=(Img1dsf*)objs[0];
      
      result= PValueNumber(*ims);
      goto end;
   }
   if (objs[0]->Type() == Po_Img2duc) {
      Img2duc* const ims=(Img2duc*)objs[0];
      
      result= PValueNumber(*ims);
      goto end;
   }
   if (objs[0]->Type() == Po_Img2dsl) {
      Img2dsl* const ims=(Img2dsl*)objs[0];
      
      result= PValueNumber(*ims);
      goto end;
   }
   if (objs[0]->Type() == Po_Img2dsf) {
      Img2dsf* const ims=(Img2dsf*)objs[0];
      
      result= PValueNumber(*ims);
      goto end;
   }
   if (objs[0]->Type() == Po_Img3duc) {
      Img3duc* const ims=(Img3duc*)objs[0];
      
      result= PValueNumber(*ims);
      goto end;
   }
   if (objs[0]->Type() == Po_Img3dsl) {
      Img3dsl* const ims=(Img3dsl*)objs[0];
      
      result= PValueNumber(*ims);
      goto end;
   }
   if (objs[0]->Type() == Po_Img3dsf) {
      Img3dsf* const ims=(Img3dsf*)objs[0];
      
      result= PValueNumber(*ims);
      goto end;
   }
   if (objs[0]->Type() == Po_Reg1d) {
      Reg1d* const ims=(Reg1d*)objs[0];
      
      result= PValueNumber(*ims);
      goto end;
   }
   if (objs[0]->Type() == Po_Reg2d) {
      Reg2d* const ims=(Reg2d*)objs[0];
      
      result= PValueNumber(*ims);
      goto end;
   }
   if (objs[0]->Type() == Po_Reg3d) {
      Reg3d* const ims=(Reg3d*)objs[0];
      
      result= PValueNumber(*ims);
      goto end;
   }
   if (objs[0]->Type() == Po_Imc2duc) {
      Imc2duc* const ims=(Imc2duc*)objs[0];
      
      result= PValueNumber(*ims);
      goto end;
   }
   if (objs[0]->Type() == Po_Imc2dsl) {
      Imc2dsl* const ims=(Imc2dsl*)objs[0];
      
      result= PValueNumber(*ims);
      goto end;
   }
   if (objs[0]->Type() == Po_Imc2dsf) {
      Imc2dsf* const ims=(Imc2dsf*)objs[0];
      
      result= PValueNumber(*ims);
      goto end;
   }
   if (objs[0]->Type() == Po_Imc3duc) {
      Imc3duc* const ims=(Imc3duc*)objs[0];
      
      result= PValueNumber(*ims);
      goto end;
   }
   if (objs[0]->Type() == Po_Imc3dsl) {
      Imc3dsl* const ims=(Imc3dsl*)objs[0];
      
      result= PValueNumber(*ims);
      goto end;
   }
   if (objs[0]->Type() == Po_Imc3dsf) {
      Imc3dsf* const ims=(Imc3dsf*)objs[0];
      
      result= PValueNumber(*ims);
      goto end;
   }
   if (objs[0]->Type() == Po_Graph2d) {
      Graph2d* const grs=(Graph2d*)objs[0];
      
      result= PValueNumber(*grs);
      goto end;
   }
   if (objs[0]->Type() == Po_Graph3d) {
      Graph3d* const grs=(Graph3d*)objs[0];
      
      result= PValueNumber(*grs);
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
