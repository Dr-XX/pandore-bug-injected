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
 * @author Regis Clouard - 2002-10-30
 */

#include <pandore.h>
using namespace pandore;

/** 
 * @file prgb2gray.cpp
 * Convert RGB image to gray image.
 */

template<typename T>
Errc PRGB2Gray( const Imc2d<T> &ims, Img2d<T> &imd, float red, float green, float blue) {
   T *psx = ims.VectorX();
   T *psy = ims.VectorY();
   T *psz = ims.VectorZ();
   T *pd = imd.Vector();
   for (; psx < ims.VectorX() + ims.VectorSize(); psx++, psy++, psz++, pd++) {
      *pd = (T)((red * (*psx) + green * (*psy) + blue * (*psz)) / (red + green + blue));
   }
   
   return SUCCESS;
}

/** 
 * Convert RGB image to gray image.
 */
template<typename T>
Errc PRGB2Gray( const Imc3d<T> &ims, Img3d<T> &imd, float red, float green, float blue ) {
   
   T *psx = ims.VectorX();
   T *psy = ims.VectorY();
   T *psz = ims.VectorZ();
   T *pd = imd.Vector();
   
   for ( ; psx < ims.VectorX() + ims.VectorSize(); psx++, psy++, psz++, pd++) {
      *pd = (T)((red * (*psx) + green * (*psy) + blue * (*psz)) / (red + green + blue));
   }
   
   return SUCCESS;
}



#ifdef MAIN
#define	USAGE	"usage: %s red green blue [-m mask] [im_in|-] [im_out|-]"
#define	PARC	3
#define	FINC	1
#define	FOUTC	1
#define	MASK	0
int main( int argc, char *argv[] ) {
   Errc result;                // The result code of the execution.
   Pobject* mask;              // The region map.
   Pobject* objin[FINC + 1];   // The input objects.
   Pobject* objs[FINC + 1];    // The source objects masked.
   Pobject* objout[FOUTC + 1]; // The output object.
   Pobject* objd[FOUTC + 1];   // The result object of the execution.
   char* parv[PARC + 1];       // The input parameters.

   ReadArgs(argc, argv, PARC, FINC, FOUTC, &mask, objin, objs, objout, objd, parv, USAGE, MASK); 
   if (objs[0]->Type() == Po_Imc2duc) {
      Imc2duc* const ims = (Imc2duc*)objs[0];
      objd[0] = new Img2duc(ims->Size());
      Img2duc* const imd = (Img2duc*)objd[0];
      
      result = PRGB2Gray(*ims, *imd, (float)atof(parv[0]), (float)atof(parv[1]), (float)atof(parv[2]));
      goto end;
   }
   if (objs[0]->Type() == Po_Imc2dsl) {
      Imc2dsl* const ims = (Imc2dsl*)objs[0];
      objd[0] = new Img2dsl(ims->Size());
      Img2dsl* const imd = (Img2dsl*)objd[0];
      
      result = PRGB2Gray(*ims, *imd, (float)atof(parv[0]), (float)atof(parv[1]), (float)atof(parv[2]));
      goto end;
   }
   if (objs[0]->Type() == Po_Imc2dsf) {
      Imc2dsf* const ims = (Imc2dsf*)objs[0];
      objd[0] = new Img2dsf(ims->Size());
      Img2dsf* const imd = (Img2dsf*)objd[0];
      
      result = PRGB2Gray(*ims, *imd, (float)atof(parv[0]), (float)atof(parv[1]), (float)atof(parv[2]));
      goto end;
   }
   if (objs[0]->Type() == Po_Imc3duc) {
      Imc3duc* const ims = (Imc3duc*)objs[0];
      objd[0] = new Img3duc(ims->Size());
      Img3duc* const imd = (Img3duc*)objd[0];
      
      result = PRGB2Gray(*ims, *imd, (float)atof(parv[0]), (float)atof(parv[1]), (float)atof(parv[2]));
      goto end;
   }
   if (objs[0]->Type() == Po_Imc3dsl) {
      Imc3dsl* const ims = (Imc3dsl*)objs[0];
      objd[0] = new Img3dsl(ims->Size());
      Img3dsl* const imd = (Img3dsl*)objd[0];
      
      result = PRGB2Gray(*ims, *imd, (float)atof(parv[0]), (float)atof(parv[1]), (float)atof(parv[2]));
      goto end;
   }
   if (objs[0]->Type() == Po_Imc3dsf) {
      Imc3dsf* const ims = (Imc3dsf*)objs[0];
      objd[0] = new Img3dsf(ims->Size());
      Img3dsf* const imd = (Img3dsf*)objd[0];
      
      result = PRGB2Gray(*ims, *imd, (float)atof(parv[0]), (float)atof(parv[1]), (float)atof(parv[2]));
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