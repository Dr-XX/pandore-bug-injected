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
 * @file pcmyk2rgb.cpp
 */
Errc PCmyk2Rgb( const Imx3duc &ims, Imx3duc &imd ) {

   if (ims.Bands()!=4) {
      std::cerr << "Error pcmyk2rgb: Bad image type, CMYK color image expected!" << std::endl;
   }
   
   // convert byte color to 0-1 double color

   const Uchar *c = ims.Vector(0);
   const Uchar *m = ims.Vector(1);
   const Uchar *y = ims.Vector(2);
   const Uchar *k = ims.Vector(3);
   
   Uchar *r = imd.Vector(0);
   Uchar *g = imd.Vector(1);
   Uchar *b = imd.Vector(2);

   for (Ulong i=0; i<ims.VectorSize(); i++, r++, g++, b++, c++, m++, y++, k++ ) {      
      double dcmyk[4];
      dcmyk[0] = *c/256.0;
      dcmyk[1] = *m/256.0;
      dcmyk[2] = *y/256.0;
      dcmyk[3] = *k/256.0;
      
      *r = (Uchar)(255*(1-dcmyk[0])*(1-dcmyk[3]));
      *g = (Uchar)(255*(1-dcmyk[1])*(1-dcmyk[3]));
      *b = (Uchar)(255*(1-dcmyk[2])*(1-dcmyk[3]));
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
   if (objs[0]->Type() == Po_Imx2duc) {
     Imx2duc* const ims = (Imx2duc*)objs[0];
     objd[0] = new Imc2duc(4, ims->Size());
     Imc2duc* const imd = (Imc2duc*)objd[0];
     
     result = PCmyk2Rgb(*ims, *imd);
     goto end;
   }
   if (objs[0]->Type() == Po_Imx3duc) {
     Imx3duc* const ims = (Imx3duc*)objs[0];
     objd[0] = new Imc3duc(4, ims->Size());
     Imc3duc* const imd = (Imc3duc*)objd[0];
     
     result = PCmyk2Rgb(*ims, *imd);
     goto end;
   }
   if (objs[0]->Type() == Po_Imx2duc) {
     Imx2duc* const ims = (Imx2duc*)objs[0];
     objd[0] = new Imc2duc(4, ims->Size());
     Imc2duc* const imd = (Imc2duc*)objd[0];
     
     result = PCmyk2Rgb(*ims, *imd);
     goto end;
   }
   if (objs[0]->Type() == Po_Imx3duc) {
     Imx3duc* const ims = (Imx3duc*)objs[0];
     objd[0] = new Imc3duc(4, ims->Size());
     Imc3duc* const imd = (Imc3duc*)objd[0];
     
     result = PCmyk2Rgb(*ims, *imd);
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
