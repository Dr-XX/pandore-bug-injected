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

/**
 * @author Régis Clouard - Jan 03, 2013.
 */

#include <math.h>
#include <pandore.h>
using namespace pandore;

/**
 * @file prgb2cmyk.cpp
 */
template<typename T>
Errc PRgb2Cmyk( const Imx3d<T> &ims, Imx3d<T> &imd ) {
   const T *r, *g, *b;
   if (ims.Bands() == 3) {
      r = ims.Vector(0);
      g = ims.Vector(1);
      b = ims.Vector(2);
   } else if (ims.Bands() == 1) {
      r = ims.Vector(0);
      g = ims.Vector(0);
      b = ims.Vector(0);
   } else {
      std::cerr << "Error prgb2cmyk: Bad image type, RGB color image expected!" << std::endl;
      return FAILURE;
   }

   T *c = imd.Vector(0);
   T *m = imd.Vector(1);
   T *y = imd.Vector(2);
   T *k = imd.Vector(3);

   for (Ulong i=0; i<ims.VectorSize(); i++, r++, g++, b++, c++, m++, y++, k++ ) {
      // BLACK
      if (*r==0 && *g==0 && *b==0) {
	 *k = (T)255;
	 *c = 0;
	 *m = 0;
	 *y = 0;
	 continue;
      }
      float c1 = 1.0f - ((float)*r/255.0f);
      float m1 = 1.0f - ((float)*g/255.0f);
      float y1 = 1.0f - ((float)*b/255.0f);
      
      float minCMY = MIN(c1, MIN(m1,y1));
      *c = (T)(((c1 - minCMY) / (1.0 - minCMY))*255.0);
      *m = (T)(((m1 - minCMY) / (1.0 - minCMY))*255.0);
      *y = (T)(((y1 - minCMY) / (1.0 - minCMY))*255.0);
      *k = (T)(minCMY*255.0);
   }
   return SUCCESS;
}
   


#ifdef MAIN
#define	USAGE	"usage: %s [-m mask] [im_in|-] [im_out|-]"
#define	PARC	0
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
     objd[0] = new Imx2duc(4, ims->Size());
     Imx2duc* const imd = (Imx2duc*)objd[0];
     
     result = PRgb2Cmyk(*ims, *imd);
     goto end;
   }
   if (objs[0]->Type() == Po_Imc3duc) {
     Imc3duc* const ims = (Imc3duc*)objs[0];
     objd[0] = new Imx3duc(4, ims->Size());
     Imx3duc* const imd = (Imx3duc*)objd[0];
     
     result = PRgb2Cmyk(*ims, *imd);
     goto end;
   }
   if (objs[0]->Type() == Po_Img2duc) {
     Img2duc* const ims = (Img2duc*)objs[0];
     objd[0] = new Imx2duc(4, ims->Size());
     Imx2duc* const imd = (Imx2duc*)objd[0];
     
     result = PRgb2Cmyk(*ims, *imd);
     goto end;
   }
   if (objs[0]->Type() == Po_Img3duc) {
     Img3duc* const ims = (Img3duc*)objs[0];
     objd[0] = new Imx3duc(4, ims->Size());
     Imx3duc* const imd = (Imx3duc*)objd[0];
     
     result = PRgb2Cmyk(*ims, *imd);
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
