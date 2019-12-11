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
 * @file pmultval.cpp
 *
 * Multiplies each band of the input image with values
 * stored in a collection.
 */
template <typename T>
Errc PMultVal( const Collection &cols, const Imx3d<T> &ims, Imx3d<T> &imd ) {
   const Float* p = cols.GETARRAY("internal_array", Float);
   Long size = cols.GETARRAYSIZE("internal_array", Float);

   if ( size != ims.Bands() ) {
      std::cerr << "Error pmultval: Bad value array size: " << size << std::endl;
      return FAILURE;
   }

   for (int b = 0; b < ims.Bands(); b++) {
      T *ps = ims.Vector(b);
      T *pd = imd.Vector(b);
 
      for ( ;ps < ims.Vector(b) + ims.VectorSize(); ps++, pd++ ) {
	 Float val = *ps * p[b];
	 if (val > Limits<T>::max()) {
	    *pd = Limits<T>::max();
	 } else if (val<Limits<T>::min()) {
	    *pd = Limits<T>::min();
	 } else {
	    *pd = (T)val;
	 }
      }
   }
   return SUCCESS;
}




#ifdef MAIN
#define	USAGE	"usage: %s [-m mask] [col_in|-] [im_in|-] [im_out|-]"
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
   if ((objs[0]->Type() == Po_Collection) && (objs[1]->Type() == Po_Img1duc)) {
      Collection* const cols = (Collection*)objs[0];
      Img1duc* const ims = (Img1duc*)objs[1];
      objd[0] = new Img1duc(ims->Props());
      Img1duc* const imd = (Img1duc*)objd[0];
      
      result = PMultVal(*cols, *ims, *imd);
      goto end;
   }
   if ((objs[0]->Type() == Po_Collection) && (objs[1]->Type() == Po_Img1dsl)) {
      Collection* const cols = (Collection*)objs[0];
      Img1dsl* const ims = (Img1dsl*)objs[1];
      objd[0] = new Img1dsl(ims->Props());
      Img1dsl* const imd = (Img1dsl*)objd[0];
      
      result = PMultVal(*cols, *ims, *imd);
      goto end;
   }
   if ((objs[0]->Type() == Po_Collection) && (objs[1]->Type() == Po_Img1dsf)) {
      Collection* const cols = (Collection*)objs[0];
      Img1dsf* const ims = (Img1dsf*)objs[1];
      objd[0] = new Img1dsf(ims->Props());
      Img1dsf* const imd = (Img1dsf*)objd[0];
      
      result = PMultVal(*cols, *ims, *imd);
      goto end;
   }
   if ((objs[0]->Type() == Po_Collection) && (objs[1]->Type() == Po_Img2duc)) {
      Collection* const cols = (Collection*)objs[0];
      Img2duc* const ims = (Img2duc*)objs[1];
      objd[0] = new Img2duc(ims->Props());
      Img2duc* const imd = (Img2duc*)objd[0];
      
      result = PMultVal(*cols, *ims, *imd);
      goto end;
   }
   if ((objs[0]->Type() == Po_Collection) && (objs[1]->Type() == Po_Img2dsl)) {
      Collection* const cols = (Collection*)objs[0];
      Img2dsl* const ims = (Img2dsl*)objs[1];
      objd[0] = new Img2dsl(ims->Props());
      Img2dsl* const imd = (Img2dsl*)objd[0];
      
      result = PMultVal(*cols, *ims, *imd);
      goto end;
   }
   if ((objs[0]->Type() == Po_Collection) && (objs[1]->Type() == Po_Img2dsf)) {
      Collection* const cols = (Collection*)objs[0];
      Img2dsf* const ims = (Img2dsf*)objs[1];
      objd[0] = new Img2dsf(ims->Props());
      Img2dsf* const imd = (Img2dsf*)objd[0];
      
      result = PMultVal(*cols, *ims, *imd);
      goto end;
   }
   if ((objs[0]->Type() == Po_Collection) && (objs[1]->Type() == Po_Img3duc)) {
      Collection* const cols = (Collection*)objs[0];
      Img3duc* const ims = (Img3duc*)objs[1];
      objd[0] = new Img3duc(ims->Props());
      Img3duc* const imd = (Img3duc*)objd[0];
      
      result = PMultVal(*cols, *ims, *imd);
      goto end;
   }
   if ((objs[0]->Type() == Po_Collection) && (objs[1]->Type() == Po_Img3dsl)) {
      Collection* const cols = (Collection*)objs[0];
      Img3dsl* const ims = (Img3dsl*)objs[1];
      objd[0] = new Img3dsl(ims->Props());
      Img3dsl* const imd = (Img3dsl*)objd[0];
      
      result = PMultVal(*cols, *ims, *imd);
      goto end;
   }
   if ((objs[0]->Type() == Po_Collection) && (objs[1]->Type() == Po_Img3dsf)) {
      Collection* const cols = (Collection*)objs[0];
      Img3dsf* const ims = (Img3dsf*)objs[1];
      objd[0] = new Img3dsf(ims->Props());
      Img3dsf* const imd = (Img3dsf*)objd[0];
      
      result = PMultVal(*cols, *ims, *imd);
      goto end;
   }
   if ((objs[0]->Type() == Po_Collection) && (objs[1]->Type() == Po_Imc2duc)) {
      Collection* const cols = (Collection*)objs[0];
      Imc2duc* const ims = (Imc2duc*)objs[1];
      objd[0] = new Imc2duc(ims->Props());
      Imc2duc* const imd = (Imc2duc*)objd[0];
      
      result = PMultVal(*cols, *ims, *imd);
      goto end;
   }
   if ((objs[0]->Type() == Po_Collection) && (objs[1]->Type() == Po_Imc2dsl)) {
      Collection* const cols = (Collection*)objs[0];
      Imc2dsl* const ims = (Imc2dsl*)objs[1];
      objd[0] = new Imc2dsl(ims->Props());
      Imc2dsl* const imd = (Imc2dsl*)objd[0];
      
      result = PMultVal(*cols, *ims, *imd);
      goto end;
   }
   if ((objs[0]->Type() == Po_Collection) && (objs[1]->Type() == Po_Imc2dsf)) {
      Collection* const cols = (Collection*)objs[0];
      Imc2dsf* const ims = (Imc2dsf*)objs[1];
      objd[0] = new Imc2dsf(ims->Props());
      Imc2dsf* const imd = (Imc2dsf*)objd[0];
      
      result = PMultVal(*cols, *ims, *imd);
      goto end;
   }
   if ((objs[0]->Type() == Po_Collection) && (objs[1]->Type() == Po_Imc3duc)) {
      Collection* const cols = (Collection*)objs[0];
      Imc3duc* const ims = (Imc3duc*)objs[1];
      objd[0] = new Imc3duc(ims->Props());
      Imc3duc* const imd = (Imc3duc*)objd[0];
      
      result = PMultVal(*cols, *ims, *imd);
      goto end;
   }
   if ((objs[0]->Type() == Po_Collection) && (objs[1]->Type() == Po_Imc3dsl)) {
      Collection* const cols = (Collection*)objs[0];
      Imc3dsl* const ims = (Imc3dsl*)objs[1];
      objd[0] = new Imc3dsl(ims->Props());
      Imc3dsl* const imd = (Imc3dsl*)objd[0];
      
      result = PMultVal(*cols, *ims, *imd);
      goto end;
   }
   if ((objs[0]->Type() == Po_Collection) && (objs[1]->Type() == Po_Imc3dsf)) {
      Collection* const cols = (Collection*)objs[0];
      Imc3dsf* const ims = (Imc3dsf*)objs[1];
      objd[0] = new Imc3dsf(ims->Props());
      Imc3dsf* const imd = (Imc3dsf*)objd[0];
      
      result = PMultVal(*cols, *ims, *imd);
      goto end;
   }
   if ((objs[0]->Type() == Po_Collection) && (objs[1]->Type() == Po_Imx1duc)) {
      Collection* const cols = (Collection*)objs[0];
      Imx1duc* const ims = (Imx1duc*)objs[1];
      objd[0] = new Imx1duc(ims->Props());
      Imx1duc* const imd = (Imx1duc*)objd[0];
      
      result = PMultVal(*cols, *ims, *imd);
      goto end;
   }
   if ((objs[0]->Type() == Po_Collection) && (objs[1]->Type() == Po_Imx1dsl)) {
      Collection* const cols = (Collection*)objs[0];
      Imx1dsl* const ims = (Imx1dsl*)objs[1];
      objd[0] = new Imx1dsl(ims->Props());
      Imx1dsl* const imd = (Imx1dsl*)objd[0];
      
      result = PMultVal(*cols, *ims, *imd);
      goto end;
   }
   if ((objs[0]->Type() == Po_Collection) && (objs[1]->Type() == Po_Imx1dsf)) {
      Collection* const cols = (Collection*)objs[0];
      Imx1dsf* const ims = (Imx1dsf*)objs[1];
      objd[0] = new Imx1dsf(ims->Props());
      Imx1dsf* const imd = (Imx1dsf*)objd[0];
      
      result = PMultVal(*cols, *ims, *imd);
      goto end;
   }
   if ((objs[0]->Type() == Po_Collection) && (objs[1]->Type() == Po_Imx2duc)) {
      Collection* const cols = (Collection*)objs[0];
      Imx2duc* const ims = (Imx2duc*)objs[1];
      objd[0] = new Imx2duc(ims->Props());
      Imx2duc* const imd = (Imx2duc*)objd[0];
      
      result = PMultVal(*cols, *ims, *imd);
      goto end;
   }
   if ((objs[0]->Type() == Po_Collection) && (objs[1]->Type() == Po_Imx2dsl)) {
      Collection* const cols = (Collection*)objs[0];
      Imx2dsl* const ims = (Imx2dsl*)objs[1];
      objd[0] = new Imx2dsl(ims->Props());
      Imx2dsl* const imd = (Imx2dsl*)objd[0];
      
      result = PMultVal(*cols, *ims, *imd);
      goto end;
   }
   if ((objs[0]->Type() == Po_Collection) && (objs[1]->Type() == Po_Imx2dsf)) {
      Collection* const cols = (Collection*)objs[0];
      Imx2dsf* const ims = (Imx2dsf*)objs[1];
      objd[0] = new Imx2dsf(ims->Props());
      Imx2dsf* const imd = (Imx2dsf*)objd[0];
      
      result = PMultVal(*cols, *ims, *imd);
      goto end;
   }
   if ((objs[0]->Type() == Po_Collection) && (objs[1]->Type() == Po_Imx3duc)) {
      Collection* const cols = (Collection*)objs[0];
      Imx3duc* const ims = (Imx3duc*)objs[1];
      objd[0] = new Imx3duc(ims->Props());
      Imx3duc* const imd = (Imx3duc*)objd[0];
      
      result = PMultVal(*cols, *ims, *imd);
      goto end;
   }
   if ((objs[0]->Type() == Po_Collection) && (objs[1]->Type() == Po_Imx3dsl)) {
      Collection* const cols = (Collection*)objs[0];
      Imx3dsl* const ims = (Imx3dsl*)objs[1];
      objd[0] = new Imx3dsl(ims->Props());
      Imx3dsl* const imd = (Imx3dsl*)objd[0];
      
      result = PMultVal(*cols, *ims, *imd);
      goto end;
   }
   if ((objs[0]->Type() == Po_Collection) && (objs[1]->Type() == Po_Imx3dsf)) {
      Collection* const cols = (Collection*)objs[0];
      Imx3dsf* const ims = (Imx3dsf*)objs[1];
      objd[0] = new Imx3dsf(ims->Props());
      Imx3dsf* const imd = (Imx3dsf*)objd[0];
      
      result = PMultVal(*cols, *ims, *imd);
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
