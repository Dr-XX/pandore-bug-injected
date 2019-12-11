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

Errc PImgs2Imx( Pobject* ims[], Imx2duc &imd ) {
   for (int i = 1; i < imd.Bands(); i++) {
      if (((Img2duc*)ims[i])->Size() != ((Img2duc*)ims[0])->Size()) {
	 fprintf(stderr, "Error pim2imx: Incompatible input images\n");
	 return FAILURE;
      }
   }

   for (int b = 0; b < imd.Bands(); b++) {
      Img2duc::ValueType *ps = ((Img2duc*)ims[b])->Vector();
      Imx2duc::ValueType *pd = imd.Vector(b);

      for (Ulong x=0; x< imd.VectorSize(); x++) {
	 *(pd++) = *(ps++);
      }
   }

   return SUCCESS;
}

Errc PImgs2Imx( Pobject* ims[], Imx2dsl &imd ) {
   for (int i = 1; i < imd.Bands(); i++) {
      if (((Img2dsl*)ims[i])->Size() != ((Img2dsl*)ims[0])->Size()) {
	 fprintf(stderr, "Error pim2imx: Incompatible input images\n");
	 return FAILURE;
      }
   }

   for (int b = 0; b < imd.Bands(); b++) {
      Img2dsl::ValueType *ps = ((Img2dsl*)ims[b])->Vector();
      Imx2dsl::ValueType *pd = imd.Vector(b);

      for (Ulong x=0; x< imd.VectorSize(); x++) {
	 *(pd++) = *(ps++);
      }
   }

   return SUCCESS;
}

Errc PImgs2Imx( Pobject* ims[], Imx2dsf &imd ) {
   for (int i = 1; i < imd.Bands(); i++) {
      if (((Img2dsf*)ims[i])->Size() != ((Img2dsf*)ims[0])->Size()) {
	 fprintf(stderr, "Error pim2imx: Incompatible input images\n");
	 return FAILURE;
      }
   }

   for (int b = 0; b < imd.Bands(); b++) {
      Img2dsf::ValueType *ps = ((Img2dsf*)ims[b])->Vector();
      Imx2dsf::ValueType *pd = imd.Vector(b);

      for (Ulong x=0; x< imd.VectorSize(); x++) {
	 *(pd++) = *(ps++);
      }
   }

   return SUCCESS;
}

Errc PImgs2Imx( Pobject* ims[], Imx3duc &imd ) {
   for (int i = 1; i < imd.Bands(); i++) {
      if (((Img3duc*)ims[i])->Size() != ((Img3duc*)ims[0])->Size()) {
	 fprintf(stderr, "Error pim2imx: Incompatible input images\n");
	 return FAILURE;
      }
   }

   for (int b = 0; b < imd.Bands(); b++) {
      Img3duc::ValueType *ps = ((Img3duc*)ims[b])->Vector();
      Imx3duc::ValueType *pd = imd.Vector(b);

      for (Ulong x=0; x< imd.VectorSize(); x++) {
	 *(pd++) = *(ps++);
      }
   }

   return SUCCESS;
}

Errc PImgs2Imx( Pobject* ims[], Imx3dsl &imd ) {
   for (int i = 1; i < imd.Bands(); i++) {
      if (((Img3dsl*)ims[i])->Size() != ((Img3dsl*)ims[0])->Size()) {
	 fprintf(stderr, "Error pim2imx: Incompatible input images\n");
	 return FAILURE;
      }
   }

   for (int b = 0; b < imd.Bands(); b++) {
      Img3dsl::ValueType *ps = ((Img3dsl*)ims[b])->Vector();
      Imx3dsl::ValueType *pd = imd.Vector(b);

      for (Ulong x=0; x< imd.VectorSize(); x++) {
	 *(pd++) = *(ps++);
      }
   }

   return SUCCESS;
}

Errc PImgs2Imx( Pobject* ims[], Imx3dsf &imd ) {
   for (int i = 1; i < imd.Bands(); i++) {
      if (((Img3dsf*)ims[i])->Size() != ((Img3dsf*)ims[0])->Size()) {
	 fprintf(stderr, "Error pim2imx: Incompatible input images\n");
	 return FAILURE;
      }
   }

   for (int b = 0; b < imd.Bands(); b++) {
      Img3dsf::ValueType *ps = ((Img3dsf*)ims[b])->Vector();
      Imx3dsf::ValueType *pd = imd.Vector(b);

      for (Ulong x=0; x< imd.VectorSize(); x++) {
	 *(pd++) = *(ps++);
      }
   }

   return SUCCESS;
}


#ifdef MAIN
#define	USAGE	"usage: %s dimension [-m mask] [im_in|-]* [im_out|-]"
#define	PARC	1
#define	FINC	5
#define	FOUTC	1
#define	MASK	2

int main( int argc, char *argv[] ) {
   Errc result;          // The result code of the execution.
   Pobject* mask;        // The region map.
   Pobject* objin[50];   // The input objects;
   Pobject* objs[50];    // The source objects masked.
   Pobject* objout[50];  // The output object.
   Pobject* objd[50];    // The result object of the execution.
   char* parv[PARC+1];   // The input parameters.
   
   int dimension = 0;
   if (argc > 1 ) {
      if (!strcmp(argv[1], "-p")) {
	 return 0;
      } else if ( strcmp(argv[1], "-h")) {
	 dimension = atoi(argv[1]);
      }
   }
   
   ReadArgs(argc, argv, PARC, dimension, FOUTC, &mask, objin, objs, objout, objd, parv, USAGE, MASK);

   if ((objs[0]->Type() == Po_Img2duc)) {
      // Check compatibility of input images.
      for (int i = 1; i < dimension; i++) {
	 if (objs[i]->Type() != Po_Img2duc) {
	    goto error;
	 }
      }

      objd[0] = new Imx2duc(dimension, ((Img2duc*)objs[0])->Size());
      Imx2duc* const imd = (Imx2duc*)objd[0];

      result = PImgs2Imx(objs, *imd);
      goto end;
   }
   if ((objs[0]->Type() == Po_Img2dsl)) {
      // Check compatibility of input images.
      for (int i = 1; i < dimension; i++) {
	 if (objs[i]->Type() != Po_Img2dsl) {
	    goto error;
	 }
      }

      objd[0] = new Imx2dsl(dimension, ((Img2dsl*)objs[0])->Size());
      Imx2dsl* const imd = (Imx2dsl*)objd[0];

      result = PImgs2Imx(objs, *imd);
      goto end;
   }
   if ((objs[0]->Type() == Po_Img2dsf)) {
      // Check compatibility of input images.
      for (int i = 1; i < dimension; i++) {
	 if (objs[i]->Type() != Po_Img2dsf) {
	    goto error;
	 }
      }

      objd[0] = new Imx2dsf(dimension, ((Img2dsf*)objs[0])->Size());
      Imx2dsf* const imd = (Imx2dsf*)objd[0];

      result = PImgs2Imx(objs, *imd);
      goto end;
   }
   if ((objs[0]->Type() == Po_Img3duc)) {
      // Check compatibility of input images.
      for (int i = 1; i < dimension; i++) {
	 if (objs[i]->Type() != Po_Img3duc) {
	    goto error;
	 }
      }

      objd[0] = new Imx3duc(dimension, ((Img3duc*)objs[0])->Size());
      Imx3duc* const imd = (Imx3duc*)objd[0];

      result = PImgs2Imx(objs, *imd);
      goto end;
   }
   if ((objs[0]->Type() == Po_Img3dsl)) {
      // Check compatibility of input images.
      for (int i = 1; i < dimension; i++) {
	 if (objs[i]->Type() != Po_Img3dsl) {
	    goto error;
	 }
      }

      objd[0] = new Imx3dsl(dimension, ((Img3dsl*)objs[0])->Size());
      Imx3dsl* const imd = (Imx3dsl*)objd[0];

      result = PImgs2Imx(objs, *imd);
      goto end;
   }
   if ((objs[0]->Type() == Po_Img3dsf)) {
      // Check compatibility of input images.
      for (int i = 1; i < dimension; i++) {
	 if (objs[i]->Type() != Po_Img3dsf) {
	    goto error;
	 }
      }

      objd[0] = new Imx3dsf(dimension, ((Img3dsf*)objs[0])->Size());
      Imx3dsf* const imd = (Imx3dsf*)objd[0];

      result = PImgs2Imx(objs, *imd);
      goto end;
   }

error:
  {
     PrintErrorFormat(objin, dimension);
     result = FAILURE;
  }	

end:
   if (result) {
	WriteArgs(argc, argv, PARC, dimension, FOUTC, &mask, objin, objs, objout, objd, MASK);
   }
  Exit(result);
  return 0;
}
#endif
