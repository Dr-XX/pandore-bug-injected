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
 * @author Régis Clouard - 2001-05-25
 */

#include <pandore.h>
using namespace pandore;

/**
 * @file pvaluerank.cpp
 * Gets the num th value of the image.
 */


Errc PValueRank( const Img1duc &ims, int num ) {
   int h,g;
   Img1duc::ValueType *hg,val;
   int max;
   Img1duc::ValueType *ps=ims.Vector();
   const Img1duc::ValueType *pf=ims.Vector()+ims.VectorSize();
   
   if (num <=0 || num > MAXUSHORT) { // Un majorant?
      std::cerr << "Error pvaluerank: Parameter 'index' must be an integer within [1..65535]" << std::endl;
   }
   
   hg=(Img1duc::ValueType*)malloc(num*sizeof(Img1duc::ValueType));
   
   for (h=0; h<num;h++)
      hg[h]=Limits<Img1duc::ValueType>::max();
   
   // Compute histogram 
   max=0;
   
   for (; ps < pf; ps++) {
      val = *ps;
      for (h = 0; h < num; h++) {
	 if (val == hg[h]) {
	    if (h > max) {
	       max = h;
	    }
	    break;
	 }
	 if (val < hg[h]) {
	    if (h > max) {
	       max = h;
	    }
	    for (g = num - 1;g > h; g--) {
	       hg[g] = hg[g - 1];
	    }
	    hg[h] = val;
	    break;
	 }
      }
   }
   if (max < num - 1) {
      return FAILURE;
   }
   val= hg[num - 1];
   free(hg);
   return val;
}

Errc PValueRank( const Img1dsl &ims, int num ) {
   int h,g;
   Img1dsl::ValueType *hg,val;
   int max;
   Img1dsl::ValueType *ps=ims.Vector();
   const Img1dsl::ValueType *pf=ims.Vector()+ims.VectorSize();
   
   if (num <=0 || num > MAXUSHORT) { // Un majorant?
      std::cerr << "Error pvaluerank: Parameter 'index' must be an integer within [1..65535]" << std::endl;
   }
   
   hg=(Img1dsl::ValueType*)malloc(num*sizeof(Img1dsl::ValueType));
   
   for (h=0; h<num;h++)
      hg[h]=Limits<Img1dsl::ValueType>::max();
   
   // Compute histogram 
   max=0;
   
   for (; ps < pf; ps++) {
      val = *ps;
      for (h = 0; h < num; h++) {
	 if (val == hg[h]) {
	    if (h > max) {
	       max = h;
	    }
	    break;
	 }
	 if (val < hg[h]) {
	    if (h > max) {
	       max = h;
	    }
	    for (g = num - 1;g > h; g--) {
	       hg[g] = hg[g - 1];
	    }
	    hg[h] = val;
	    break;
	 }
      }
   }
   if (max < num - 1) {
      return FAILURE;
   }
   val= hg[num - 1];
   free(hg);
   return val;
}

Errc PValueRank( const Img1dsf &ims, int num ) {
   int h,g;
   Img1dsf::ValueType *hg,val;
   int max;
   Img1dsf::ValueType *ps=ims.Vector();
   const Img1dsf::ValueType *pf=ims.Vector()+ims.VectorSize();
   
   if (num <=0 || num > MAXUSHORT) { // Un majorant?
      std::cerr << "Error pvaluerank: Parameter 'index' must be an integer within [1..65535]" << std::endl;
   }
   
   hg=(Img1dsf::ValueType*)malloc(num*sizeof(Img1dsf::ValueType));
   
   for (h=0; h<num;h++)
      hg[h]=Limits<Img1dsf::ValueType>::max();
   
   // Compute histogram 
   max=0;
   
   for (; ps < pf; ps++) {
      val = *ps;
      for (h = 0; h < num; h++) {
	 if (val == hg[h]) {
	    if (h > max) {
	       max = h;
	    }
	    break;
	 }
	 if (val < hg[h]) {
	    if (h > max) {
	       max = h;
	    }
	    for (g = num - 1;g > h; g--) {
	       hg[g] = hg[g - 1];
	    }
	    hg[h] = val;
	    break;
	 }
      }
   }
   if (max < num - 1) {
      return FAILURE;
   }
   val= hg[num - 1];
   free(hg);
   return val;
}

Errc PValueRank( const Img2duc &ims, int num ) {
   int h,g;
   Img2duc::ValueType *hg,val;
   int max;
   Img2duc::ValueType *ps=ims.Vector();
   const Img2duc::ValueType *pf=ims.Vector()+ims.VectorSize();
   
   if (num <=0 || num > MAXUSHORT) { // Un majorant?
      std::cerr << "Error pvaluerank: Parameter 'index' must be an integer within [1..65535]" << std::endl;
   }
   
   hg=(Img2duc::ValueType*)malloc(num*sizeof(Img2duc::ValueType));
   
   for (h=0; h<num;h++)
      hg[h]=Limits<Img2duc::ValueType>::max();
   
   // Compute histogram 
   max=0;
   
   for (; ps < pf; ps++) {
      val = *ps;
      for (h = 0; h < num; h++) {
	 if (val == hg[h]) {
	    if (h > max) {
	       max = h;
	    }
	    break;
	 }
	 if (val < hg[h]) {
	    if (h > max) {
	       max = h;
	    }
	    for (g = num - 1;g > h; g--) {
	       hg[g] = hg[g - 1];
	    }
	    hg[h] = val;
	    break;
	 }
      }
   }
   if (max < num - 1) {
      return FAILURE;
   }
   val= hg[num - 1];
   free(hg);
   return val;
}

Errc PValueRank( const Img2dsl &ims, int num ) {
   int h,g;
   Img2dsl::ValueType *hg,val;
   int max;
   Img2dsl::ValueType *ps=ims.Vector();
   const Img2dsl::ValueType *pf=ims.Vector()+ims.VectorSize();
   
   if (num <=0 || num > MAXUSHORT) { // Un majorant?
      std::cerr << "Error pvaluerank: Parameter 'index' must be an integer within [1..65535]" << std::endl;
   }
   
   hg=(Img2dsl::ValueType*)malloc(num*sizeof(Img2dsl::ValueType));
   
   for (h=0; h<num;h++)
      hg[h]=Limits<Img2dsl::ValueType>::max();
   
   // Compute histogram 
   max=0;
   
   for (; ps < pf; ps++) {
      val = *ps;
      for (h = 0; h < num; h++) {
	 if (val == hg[h]) {
	    if (h > max) {
	       max = h;
	    }
	    break;
	 }
	 if (val < hg[h]) {
	    if (h > max) {
	       max = h;
	    }
	    for (g = num - 1;g > h; g--) {
	       hg[g] = hg[g - 1];
	    }
	    hg[h] = val;
	    break;
	 }
      }
   }
   if (max < num - 1) {
      return FAILURE;
   }
   val= hg[num - 1];
   free(hg);
   return val;
}

Errc PValueRank( const Img2dsf &ims, int num ) {
   int h,g;
   Img2dsf::ValueType *hg,val;
   int max;
   Img2dsf::ValueType *ps=ims.Vector();
   const Img2dsf::ValueType *pf=ims.Vector()+ims.VectorSize();
   
   if (num <=0 || num > MAXUSHORT) { // Un majorant?
      std::cerr << "Error pvaluerank: Parameter 'index' must be an integer within [1..65535]" << std::endl;
   }
   
   hg=(Img2dsf::ValueType*)malloc(num*sizeof(Img2dsf::ValueType));
   
   for (h=0; h<num;h++)
      hg[h]=Limits<Img2dsf::ValueType>::max();
   
   // Compute histogram 
   max=0;
   
   for (; ps < pf; ps++) {
      val = *ps;
      for (h = 0; h < num; h++) {
	 if (val == hg[h]) {
	    if (h > max) {
	       max = h;
	    }
	    break;
	 }
	 if (val < hg[h]) {
	    if (h > max) {
	       max = h;
	    }
	    for (g = num - 1;g > h; g--) {
	       hg[g] = hg[g - 1];
	    }
	    hg[h] = val;
	    break;
	 }
      }
   }
   if (max < num - 1) {
      return FAILURE;
   }
   val= hg[num - 1];
   free(hg);
   return val;
}

Errc PValueRank( const Img3duc &ims, int num ) {
   int h,g;
   Img3duc::ValueType *hg,val;
   int max;
   Img3duc::ValueType *ps=ims.Vector();
   const Img3duc::ValueType *pf=ims.Vector()+ims.VectorSize();
   
   if (num <=0 || num > MAXUSHORT) { // Un majorant?
      std::cerr << "Error pvaluerank: Parameter 'index' must be an integer within [1..65535]" << std::endl;
   }
   
   hg=(Img3duc::ValueType*)malloc(num*sizeof(Img3duc::ValueType));
   
   for (h=0; h<num;h++)
      hg[h]=Limits<Img3duc::ValueType>::max();
   
   // Compute histogram 
   max=0;
   
   for (; ps < pf; ps++) {
      val = *ps;
      for (h = 0; h < num; h++) {
	 if (val == hg[h]) {
	    if (h > max) {
	       max = h;
	    }
	    break;
	 }
	 if (val < hg[h]) {
	    if (h > max) {
	       max = h;
	    }
	    for (g = num - 1;g > h; g--) {
	       hg[g] = hg[g - 1];
	    }
	    hg[h] = val;
	    break;
	 }
      }
   }
   if (max < num - 1) {
      return FAILURE;
   }
   val= hg[num - 1];
   free(hg);
   return val;
}

Errc PValueRank( const Img3dsl &ims, int num ) {
   int h,g;
   Img3dsl::ValueType *hg,val;
   int max;
   Img3dsl::ValueType *ps=ims.Vector();
   const Img3dsl::ValueType *pf=ims.Vector()+ims.VectorSize();
   
   if (num <=0 || num > MAXUSHORT) { // Un majorant?
      std::cerr << "Error pvaluerank: Parameter 'index' must be an integer within [1..65535]" << std::endl;
   }
   
   hg=(Img3dsl::ValueType*)malloc(num*sizeof(Img3dsl::ValueType));
   
   for (h=0; h<num;h++)
      hg[h]=Limits<Img3dsl::ValueType>::max();
   
   // Compute histogram 
   max=0;
   
   for (; ps < pf; ps++) {
      val = *ps;
      for (h = 0; h < num; h++) {
	 if (val == hg[h]) {
	    if (h > max) {
	       max = h;
	    }
	    break;
	 }
	 if (val < hg[h]) {
	    if (h > max) {
	       max = h;
	    }
	    for (g = num - 1;g > h; g--) {
	       hg[g] = hg[g - 1];
	    }
	    hg[h] = val;
	    break;
	 }
      }
   }
   if (max < num - 1) {
      return FAILURE;
   }
   val= hg[num - 1];
   free(hg);
   return val;
}

Errc PValueRank( const Img3dsf &ims, int num ) {
   int h,g;
   Img3dsf::ValueType *hg,val;
   int max;
   Img3dsf::ValueType *ps=ims.Vector();
   const Img3dsf::ValueType *pf=ims.Vector()+ims.VectorSize();
   
   if (num <=0 || num > MAXUSHORT) { // Un majorant?
      std::cerr << "Error pvaluerank: Parameter 'index' must be an integer within [1..65535]" << std::endl;
   }
   
   hg=(Img3dsf::ValueType*)malloc(num*sizeof(Img3dsf::ValueType));
   
   for (h=0; h<num;h++)
      hg[h]=Limits<Img3dsf::ValueType>::max();
   
   // Compute histogram 
   max=0;
   
   for (; ps < pf; ps++) {
      val = *ps;
      for (h = 0; h < num; h++) {
	 if (val == hg[h]) {
	    if (h > max) {
	       max = h;
	    }
	    break;
	 }
	 if (val < hg[h]) {
	    if (h > max) {
	       max = h;
	    }
	    for (g = num - 1;g > h; g--) {
	       hg[g] = hg[g - 1];
	    }
	    hg[h] = val;
	    break;
	 }
      }
   }
   if (max < num - 1) {
      return FAILURE;
   }
   val= hg[num - 1];
   free(hg);
   return val;
}

Errc PValueRank( const Reg1d &ims, int num ) {
   int h,g;
   Reg1d::ValueType *hg,val;
   int max;
   Reg1d::ValueType *ps=ims.Vector();
   const Reg1d::ValueType *pf=ims.Vector()+ims.VectorSize();
   
   if (num <=0 || num > MAXUSHORT) { // Un majorant?
      std::cerr << "Error pvaluerank: Parameter 'index' must be an integer within [1..65535]" << std::endl;
   }
   
   hg=(Reg1d::ValueType*)malloc(num*sizeof(Reg1d::ValueType));
   
   for (h=0; h<num;h++)
      hg[h]=Limits<Reg1d::ValueType>::max();
   
   // Compute histogram 
   max=0;
   
   for (; ps < pf; ps++) {
      val = *ps;
      for (h = 0; h < num; h++) {
	 if (val == hg[h]) {
	    if (h > max) {
	       max = h;
	    }
	    break;
	 }
	 if (val < hg[h]) {
	    if (h > max) {
	       max = h;
	    }
	    for (g = num - 1;g > h; g--) {
	       hg[g] = hg[g - 1];
	    }
	    hg[h] = val;
	    break;
	 }
      }
   }
   if (max < num - 1) {
      return FAILURE;
   }
   val= hg[num - 1];
   free(hg);
   return val;
}

Errc PValueRank( const Reg2d &ims, int num ) {
   int h,g;
   Reg2d::ValueType *hg,val;
   int max;
   Reg2d::ValueType *ps=ims.Vector();
   const Reg2d::ValueType *pf=ims.Vector()+ims.VectorSize();
   
   if (num <=0 || num > MAXUSHORT) { // Un majorant?
      std::cerr << "Error pvaluerank: Parameter 'index' must be an integer within [1..65535]" << std::endl;
   }
   
   hg=(Reg2d::ValueType*)malloc(num*sizeof(Reg2d::ValueType));
   
   for (h=0; h<num;h++)
      hg[h]=Limits<Reg2d::ValueType>::max();
   
   // Compute histogram 
   max=0;
   
   for (; ps < pf; ps++) {
      val = *ps;
      for (h = 0; h < num; h++) {
	 if (val == hg[h]) {
	    if (h > max) {
	       max = h;
	    }
	    break;
	 }
	 if (val < hg[h]) {
	    if (h > max) {
	       max = h;
	    }
	    for (g = num - 1;g > h; g--) {
	       hg[g] = hg[g - 1];
	    }
	    hg[h] = val;
	    break;
	 }
      }
   }
   if (max < num - 1) {
      return FAILURE;
   }
   val= hg[num - 1];
   free(hg);
   return val;
}

Errc PValueRank( const Reg3d &ims, int num ) {
   int h,g;
   Reg3d::ValueType *hg,val;
   int max;
   Reg3d::ValueType *ps=ims.Vector();
   const Reg3d::ValueType *pf=ims.Vector()+ims.VectorSize();
   
   if (num <=0 || num > MAXUSHORT) { // Un majorant?
      std::cerr << "Error pvaluerank: Parameter 'index' must be an integer within [1..65535]" << std::endl;
   }
   
   hg=(Reg3d::ValueType*)malloc(num*sizeof(Reg3d::ValueType));
   
   for (h=0; h<num;h++)
      hg[h]=Limits<Reg3d::ValueType>::max();
   
   // Compute histogram 
   max=0;
   
   for (; ps < pf; ps++) {
      val = *ps;
      for (h = 0; h < num; h++) {
	 if (val == hg[h]) {
	    if (h > max) {
	       max = h;
	    }
	    break;
	 }
	 if (val < hg[h]) {
	    if (h > max) {
	       max = h;
	    }
	    for (g = num - 1;g > h; g--) {
	       hg[g] = hg[g - 1];
	    }
	    hg[h] = val;
	    break;
	 }
      }
   }
   if (max < num - 1) {
      return FAILURE;
   }
   val= hg[num - 1];
   free(hg);
   return val;
}

Errc PValueRank( const Graph2d &grs, int num ) {
   int h,g;
   Graph2d::ValueType *hg,val;
   int max;
   
   if (num <=0)
      num = 1;
   else if (num > MAXUSHORT)
      num = MAXUSHORT;
   
   hg=(Graph2d::ValueType*)malloc(num*sizeof(Graph2d::ValueType));
   
   for (h=0; h<num;h++)
      hg[h]=Limits<Graph2d::ValueType>::max();
   
   max=0;
   for (int i=0;i<grs.Size();i++) {
      if (grs[i]) {
	 val=grs[i]->value;
	 for (h=0; h<num;h++) {
	    if (val==hg[h]) {
	       if (h>max) max =h;
	       break;
	    }
	    if (val<hg[h]) {
	       if (h>max) max=h;
	       for (g=num-1;g>h;g--)
		  hg[g]=hg[g-1];
	       hg[h]=val;
	       break;
	    }
	 }
      }
   }
   if (max < num-1)
      return FAILURE;
   val= hg[num-1];
   free(hg);
   return val;
}

Errc PValueRank( const Graph3d &grs, int num ) {
   int h,g;
   Graph3d::ValueType *hg,val;
   int max;
   
   if (num <=0)
      num = 1;
   else if (num > MAXUSHORT)
      num = MAXUSHORT;
   
   hg=(Graph3d::ValueType*)malloc(num*sizeof(Graph3d::ValueType));
   
   for (h=0; h<num;h++)
      hg[h]=Limits<Graph3d::ValueType>::max();
   
   max=0;
   for (int i=0;i<grs.Size();i++) {
      if (grs[i]) {
	 val=grs[i]->value;
	 for (h=0; h<num;h++) {
	    if (val==hg[h]) {
	       if (h>max) max =h;
	       break;
	    }
	    if (val<hg[h]) {
	       if (h>max) max=h;
	       for (g=num-1;g>h;g--)
		  hg[g]=hg[g-1];
	       hg[h]=val;
	       break;
	    }
	 }
      }
   }
   if (max < num-1)
      return FAILURE;
   val= hg[num-1];
   free(hg);
   return val;
}


#ifdef MAIN
#define	USAGE	"usage: %s index [-m mask] [im_in|-]"
#define	PARC	1
#define	FINC	1
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
   if (objs[0]->Type()==Po_Img1duc) {
      Img1duc* const ims=(Img1duc*)objs[0];
      
      result=PValueRank(*ims,atoi(parv[0]));
      goto end;
   }
   if (objs[0]->Type()==Po_Img1dsl) {
      Img1dsl* const ims=(Img1dsl*)objs[0];
      
      result=PValueRank(*ims,atoi(parv[0]));
      goto end;
   }
   if (objs[0]->Type()==Po_Img1dsf) {
      Img1dsf* const ims=(Img1dsf*)objs[0];
      
      result=PValueRank(*ims,atoi(parv[0]));
      goto end;
   }
   if (objs[0]->Type()==Po_Img2duc) {
      Img2duc* const ims=(Img2duc*)objs[0];
      
      result=PValueRank(*ims,atoi(parv[0]));
      goto end;
   }
   if (objs[0]->Type()==Po_Img2dsl) {
      Img2dsl* const ims=(Img2dsl*)objs[0];
      
      result=PValueRank(*ims,atoi(parv[0]));
      goto end;
   }
   if (objs[0]->Type()==Po_Img2dsf) {
      Img2dsf* const ims=(Img2dsf*)objs[0];
      
      result=PValueRank(*ims,atoi(parv[0]));
      goto end;
   }
   if (objs[0]->Type()==Po_Img3duc) {
      Img3duc* const ims=(Img3duc*)objs[0];
      
      result=PValueRank(*ims,atoi(parv[0]));
      goto end;
   }
   if (objs[0]->Type()==Po_Img3dsl) {
      Img3dsl* const ims=(Img3dsl*)objs[0];
      
      result=PValueRank(*ims,atoi(parv[0]));
      goto end;
   }
   if (objs[0]->Type()==Po_Img3dsf) {
      Img3dsf* const ims=(Img3dsf*)objs[0];
      
      result=PValueRank(*ims,atoi(parv[0]));
      goto end;
   }
   if (objs[0]->Type()==Po_Reg1d) {
      Reg1d* const ims=(Reg1d*)objs[0];
      
      result=PValueRank(*ims,atoi(parv[0]));
      goto end;
   }
   if (objs[0]->Type()==Po_Reg2d) {
      Reg2d* const ims=(Reg2d*)objs[0];
      
      result=PValueRank(*ims,atoi(parv[0]));
      goto end;
   }
   if (objs[0]->Type()==Po_Reg3d) {
      Reg3d* const ims=(Reg3d*)objs[0];
      
      result=PValueRank(*ims,atoi(parv[0]));
      goto end;
   }
   if (objs[0]->Type()==Po_Graph2d) {
      Graph2d* const grs=(Graph2d*)objs[0];
      
      result=PValueRank(*grs,atoi(parv[0]));
      goto end;
   }
   if (objs[0]->Type()==Po_Graph3d) {
      Graph3d* const grs=(Graph3d*)objs[0];
      
      result=PValueRank(*grs,atoi(parv[0]));
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
