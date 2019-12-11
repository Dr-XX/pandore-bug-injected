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
 * @author Regis Clouard - 2013-12-23
 */

#include <pandore.h>
using namespace pandore;

/**
 * @file psetband.cpp
 */
template <typename T>
Errc PSetBand( const Img2d<T> &ims, Imx2d<T> &imd, int band ) {
   if (band < 0) {
      band = 0;
   } else if (band >= ims.Bands() - 1) {
      band = ims.Bands() - 1;
   }
   T *pg = ims.Vector();
   T *pc = imd.Vector(band);
   for (Ulong x = 0; x < imd.VectorSize(); x++) {
      *(pc++) = *(pg++);
   }
   return band;
}

template <typename T>
Errc PSetBand( const Img3d<T> &ims, Imx3d<T> &imd, int band ) {
   if (band < 0) {
      band = 0;
   } else if (band >= ims.Bands() - 1) {
      band = ims.Bands() - 1;
   }
   T *pg = ims.Vector();
   T *pc = imd.Vector(band);
   for (Ulong x = 0; x < imd.VectorSize(); x++) {
      *(pc++) = *(pg++);
   }
   return band;
}





#ifdef MAIN
#define	USAGE	"usage: %s band [-m mask] [im_in|-] [im_out|-]"
#define	PARC	1
#define	FINC	2
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
      Img2duc* const ims1 = (Img2duc*)objs[0];
      Img2duc* const ims2 = (Img2duc*)objs[1];
      objd[0] = ims2;
      Img2duc* const imd = (Img2duc*)objd[0];

      result = PSetBand(*ims1, *imd, atoi(parv[0]));
      goto end;
   }
   if (objs[0]->Type() == Po_Img2duc) {
      Img2duc* const ims1 = (Img2duc*)objs[0];
      Imc2duc* const ims2 = (Imc2duc*)objs[1];
      objd[0] = ims2;
      Imc2duc* const imd = (Imc2duc*)objd[0];

      result = PSetBand(*ims1, *imd, atoi(parv[0]));
      goto end;
   }
   if (objs[0]->Type() == Po_Img2duc) {
      Img2duc* const ims1 = (Img2duc*)objs[0];
      Imx2duc* const ims2 = (Imx2duc*)objs[1];
      objd[0] = ims2;
      Imx2duc* const imd = (Imx2duc*)objd[0];

      result = PSetBand(*ims1, *imd, atoi(parv[0]));
      goto end;
   }
   if (objs[0]->Type() == Po_Img2dsl) {
      Img2dsl* const ims1 = (Img2dsl*)objs[0];
      Img2dsl* const ims2 = (Img2dsl*)objs[1];
      objd[0] = ims2;
      Img2dsl* const imd = (Img2dsl*)objd[0];

      result = PSetBand(*ims1, *imd, atoi(parv[0]));
      goto end;
   }
   if (objs[0]->Type() == Po_Img2dsl) {
      Img2dsl* const ims1 = (Img2dsl*)objs[0];
      Imc2dsl* const ims2 = (Imc2dsl*)objs[1];
      objd[0] = ims2;
      Imc2dsl* const imd = (Imc2dsl*)objd[0];

      result = PSetBand(*ims1, *imd, atoi(parv[0]));
      goto end;
   }
   if (objs[0]->Type() == Po_Img2dsl) {
      Img2dsl* const ims1 = (Img2dsl*)objs[0];
      Imx2dsl* const ims2 = (Imx2dsl*)objs[1];
      objd[0] = ims2;
      Imx2dsl* const imd = (Imx2dsl*)objd[0];

      result = PSetBand(*ims1, *imd, atoi(parv[0]));
      goto end;
   }
   if (objs[0]->Type() == Po_Img2dsf) {
      Img2dsf* const ims1 = (Img2dsf*)objs[0];
      Img2dsf* const ims2 = (Img2dsf*)objs[1];
      objd[0] = ims2;
      Img2dsf* const imd = (Img2dsf*)objd[0];

      result = PSetBand(*ims1, *imd, atoi(parv[0]));
      goto end;
   }
   if (objs[0]->Type() == Po_Img2dsf) {
      Img2dsf* const ims1 = (Img2dsf*)objs[0];
      Imc2dsf* const ims2 = (Imc2dsf*)objs[1];
      objd[0] = ims2;
      Imc2dsf* const imd = (Imc2dsf*)objd[0];

      result = PSetBand(*ims1, *imd, atoi(parv[0]));
      goto end;
   }
   if (objs[0]->Type() == Po_Img2dsf) {
      Img2dsf* const ims1 = (Img2dsf*)objs[0];
      Imx2dsf* const ims2 = (Imx2dsf*)objs[1];
      objd[0] = ims2;
      Imx2dsf* const imd = (Imx2dsf*)objd[0];

      result = PSetBand(*ims1, *imd, atoi(parv[0]));
      goto end;
   }
   if (objs[0]->Type() == Po_Img3duc) {
      Img3duc* const ims1 = (Img3duc*)objs[0];
      Img3duc* const ims2 = (Img3duc*)objs[1];
      objd[0] = ims2;
      Img3duc* const imd = (Img3duc*)objd[0];

      result = PSetBand(*ims1, *imd, atoi(parv[0]));
      goto end;
   }
   if (objs[0]->Type() == Po_Img3duc) {
      Img3duc* const ims1 = (Img3duc*)objs[0];
      Imc3duc* const ims2 = (Imc3duc*)objs[1];
      objd[0] = ims2;
      Imc3duc* const imd = (Imc3duc*)objd[0];

      result = PSetBand(*ims1, *imd, atoi(parv[0]));
      goto end;
   }
   if (objs[0]->Type() == Po_Img3duc) {
      Img3duc* const ims1 = (Img3duc*)objs[0];
      Imx3duc* const ims2 = (Imx3duc*)objs[1];
      objd[0] = ims2;
      Imx3duc* const imd = (Imx3duc*)objd[0];

      result = PSetBand(*ims1, *imd, atoi(parv[0]));
      goto end;
   }
   if (objs[0]->Type() == Po_Img3dsl) {
      Img3dsl* const ims1 = (Img3dsl*)objs[0];
      Img3dsl* const ims2 = (Img3dsl*)objs[1];
      objd[0] = ims2;
      Img3dsl* const imd = (Img3dsl*)objd[0];

      result = PSetBand(*ims1, *imd, atoi(parv[0]));
      goto end;
   }
   if (objs[0]->Type() == Po_Img3dsl) {
      Img3dsl* const ims1 = (Img3dsl*)objs[0];
      Imc3dsl* const ims2 = (Imc3dsl*)objs[1];
      objd[0] = ims2;
      Imc3dsl* const imd = (Imc3dsl*)objd[0];

      result = PSetBand(*ims1, *imd, atoi(parv[0]));
      goto end;
   }
   if (objs[0]->Type() == Po_Img3dsl) {
      Img3dsl* const ims1 = (Img3dsl*)objs[0];
      Imx3dsl* const ims2 = (Imx3dsl*)objs[1];
      objd[0] = ims2;
      Imx3dsl* const imd = (Imx3dsl*)objd[0];

      result = PSetBand(*ims1, *imd, atoi(parv[0]));
      goto end;
   }
   if (objs[0]->Type() == Po_Img3dsf) {
      Img3dsf* const ims1 = (Img3dsf*)objs[0];
      Img3dsf* const ims2 = (Img3dsf*)objs[1];
      objd[0] = ims2;
      Img3dsf* const imd = (Img3dsf*)objd[0];

      result = PSetBand(*ims1, *imd, atoi(parv[0]));
      goto end;
   }
   if (objs[0]->Type() == Po_Img3dsf) {
      Img3dsf* const ims1 = (Img3dsf*)objs[0];
      Imc3dsf* const ims2 = (Imc3dsf*)objs[1];
      objd[0] = ims2;
      Imc3dsf* const imd = (Imc3dsf*)objd[0];

      result = PSetBand(*ims1, *imd, atoi(parv[0]));
      goto end;
   }
   if (objs[0]->Type() == Po_Img3dsf) {
      Img3dsf* const ims1 = (Img3dsf*)objs[0];
      Imx3dsf* const ims2 = (Imx3dsf*)objs[1];
      objd[0] = ims2;
      Imx3dsf* const imd = (Imx3dsf*)objd[0];

      result = PSetBand(*ims1, *imd, atoi(parv[0]));
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
