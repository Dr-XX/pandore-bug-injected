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
 * @author Regis Clouard - 2000-06-27
 */

#include <string.h>
#include <pandore.h>
using namespace pandore;

/**
 * @file pimg2imx.cpp
 * Convert a set of images into a multispectral image.
 * Input images must have the same properties.
 */

Errc PImg2Imx( Img2duc &ims, Imx2duc &imd ) {
   for (int b = 0; b < imd.Bands(); b++) {
      Img2duc::ValueType *ps = ims.Vector();
      Imx2duc::ValueType *pd = imd.Vector(b);
      
      for (Ulong x = 0; x < imd.VectorSize(); x++) {
	 *(pd++) = *(ps++);
      }
   }
   return SUCCESS;
}

Errc PImg2Imx( Img2dsl &ims, Imx2dsl &imd ) {
   for (int b = 0; b < imd.Bands(); b++) {
      Img2dsl::ValueType *ps = ims.Vector();
      Imx2dsl::ValueType *pd = imd.Vector(b);
      
      for (Ulong x = 0; x < imd.VectorSize(); x++) {
	 *(pd++) = *(ps++);
      }
   }
   return SUCCESS;
}

Errc PImg2Imx( Img2dsf &ims, Imx2dsf &imd ) {
   for (int b = 0; b < imd.Bands(); b++) {
      Img2dsf::ValueType *ps = ims.Vector();
      Imx2dsf::ValueType *pd = imd.Vector(b);
      
      for (Ulong x = 0; x < imd.VectorSize(); x++) {
	 *(pd++) = *(ps++);
      }
   }
   return SUCCESS;
}

Errc PImg2Imx( Img3duc &ims, Imx3duc &imd ) {
   for (int b = 0; b < imd.Bands(); b++) {
      Img3duc::ValueType *ps = ims.Vector();
      Imx3duc::ValueType *pd = imd.Vector(b);
      
      for (Ulong x = 0; x < imd.VectorSize(); x++) {
	 *(pd++) = *(ps++);
      }
   }
   return SUCCESS;
}

Errc PImg2Imx( Img3dsl &ims, Imx3dsl &imd ) {
   for (int b = 0; b < imd.Bands(); b++) {
      Img3dsl::ValueType *ps = ims.Vector();
      Imx3dsl::ValueType *pd = imd.Vector(b);
      
      for (Ulong x = 0; x < imd.VectorSize(); x++) {
	 *(pd++) = *(ps++);
      }
   }
   return SUCCESS;
}

Errc PImg2Imx( Img3dsf &ims, Imx3dsf &imd ) {
   for (int b = 0; b < imd.Bands(); b++) {
      Img3dsf::ValueType *ps = ims.Vector();
      Imx3dsf::ValueType *pd = imd.Vector(b);
      
      for (Ulong x = 0; x < imd.VectorSize(); x++) {
	 *(pd++) = *(ps++);
      }
   }
   return SUCCESS;
}


#ifdef MAIN
#define	USAGE	"usage: %s dimension [-m mask] [im_in|-] [im_out|-]"
#define	PARC	1
#define	FINC	1
#define	FOUTC	1
#define	MASK	2

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
      int dimension = (int)atoi(parv[0]);
      Img2duc* const ims = (Img2duc*)objs[0];
      
      objd[0] = new Imx2duc(dimension, ims->Size());
      Imx2duc* const imd = (Imx2duc*)objd[0];
      
      result = PImg2Imx(*ims, *imd);
      goto end;
   }
   if (objs[0]->Type() == Po_Img2dsl) {
      int dimension = (int)atoi(parv[0]);
      Img2dsl* const ims = (Img2dsl*)objs[0];
      
      objd[0] = new Imx2dsl(dimension, ims->Size());
      Imx2dsl* const imd = (Imx2dsl*)objd[0];
      
      result = PImg2Imx(*ims, *imd);
      goto end;
   }
   if (objs[0]->Type() == Po_Img2dsf) {
      int dimension = (int)atoi(parv[0]);
      Img2dsf* const ims = (Img2dsf*)objs[0];
      
      objd[0] = new Imx2dsf(dimension, ims->Size());
      Imx2dsf* const imd = (Imx2dsf*)objd[0];
      
      result = PImg2Imx(*ims, *imd);
      goto end;
   }
   if (objs[0]->Type() == Po_Img3duc) {
      int dimension = (int)atoi(parv[0]);
      Img3duc* const ims = (Img3duc*)objs[0];
      
      objd[0] = new Imx3duc(dimension, ims->Size());
      Imx3duc* const imd = (Imx3duc*)objd[0];
      
      result = PImg2Imx(*ims, *imd);
      goto end;
   }
   if (objs[0]->Type() == Po_Img3dsl) {
      int dimension = (int)atoi(parv[0]);
      Img3dsl* const ims = (Img3dsl*)objs[0];
      
      objd[0] = new Imx3dsl(dimension, ims->Size());
      Imx3dsl* const imd = (Imx3dsl*)objd[0];
      
      result = PImg2Imx(*ims, *imd);
      goto end;
   }
   if (objs[0]->Type() == Po_Img3dsf) {
      int dimension = (int)atoi(parv[0]);
      Img3dsf* const ims = (Img3dsf*)objs[0];
      
      objd[0] = new Imx3dsf(dimension, ims->Size());
      Imx3dsf* const imd = (Imx3dsf*)objd[0];
      
      result = PImg2Imx(*ims, *imd);
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
