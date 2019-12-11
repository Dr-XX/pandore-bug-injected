/* -*- c-basic-offset: 3 -*-
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
 * @author Ludovic SOLTYS - 02-02-2003
 */

#include <pandore.h>
using namespace pandore;

/**
 * @file psetsubband.cpp
 * insertion de 4 sous-image en 1 seule.
 */
 
Errc PSetSubband( const Img2dsf &ims1, const Img2dsf &ims2, Img2dsf &imd, int I, int J ) {
   int y,x;
   int a=ims2.Height();
   int b=ims2.Width();
   
   for (y = 0; y < ims1.Height(); y++) {
      for (x = 0; x < ims1.Width(); x++) {
	 imd[y][x] = ims1[y][x];
      }
   }
   
   for (y = 0; y < a; y++) {
      for (x = 0; x < b; x++) {
	 imd[y + I][x + J] = ims2[y][x];
      }
   }
   
   return SUCCESS;
}

#ifdef MAIN
#define	USAGE	"usage: %s scale num [im_in1|-] [im-in2|-] [im_out|-]"
#define	PARC	2
#define	FINC	2
#define	FOUTC	1
#define	MASK	0

int main( int argc, char *argv[] ) {
   Errc result;              // The result code of the execution.
   Pobject* mask;            // The region map.
   Pobject* objin[FINC+1];   // The input objects.
   Pobject* objs[FINC+1];    // The source objects masked.
   Pobject* objout[FOUTC+1]; // The output object.
   Pobject* objd[FOUTC+1];   // The result object of the execution.
   char* parv[PARC+1];       // The input parameters.
   
   ReadArgs(argc,argv,PARC,FINC,FOUTC,&mask,objin,objs,objout,objd,parv,USAGE,MASK);
   
   int scale = atoi(parv[0]);
   
   if (objs[0]->Type() == Po_Img2dsf) {
      // ims1 est la dwt d'une image
      // ims2 est la sous-image extraite avec "extractsub"
      Img2dsf* const ims1 = (Img2dsf*)objs[0];
      Img2dsf* const ims2 = (Img2dsf*)objs[1];
      
      int a = ims2->Height() << scale;
      int b = ims2->Width() << scale;
      
      if (a == ims1->Height() && b == ims1->Width()) {
	 objd[0] = new Img2dsf(ims1->Size());
	 Img2dsf* const imd = (Img2dsf*)objd[0];
	 
	 int I = ims2->Height();
	 int J = ims2->Width();
	 
	 if (atoi(parv[1]) == 1) {
	    result = PSetSubband(*ims1, *ims2, *imd, 0, 0);
	 
	 } else if (atoi(parv[1]) == 2) {
	    result = PSetSubband(*ims1, *ims2, *imd, 0, J);
	 
	 } else if (atoi(parv[1]) == 3) {
	    result = PSetSubband(*ims1, *ims2, *imd, I, 0);
	 
	 } else if (atoi(parv[1]) ==4 ) {
	    result = PSetSubband(*ims1, *ims2, *imd, I, J);
	 }
      } else {
	 std::cerr << "Error psetsubband: Wrong file size" << std::endl;
	 result = FAILURE;
      }
   } else {
      PrintErrorFormat(objin, FINC);
      result = FAILURE;
   }
   
   if (result) {
      WriteArgs(argc, argv, PARC, FINC, FOUTC, &mask, objin, objs, objout, objd, MASK);
   }
   Exit(result);
   return 0;
}
#endif
