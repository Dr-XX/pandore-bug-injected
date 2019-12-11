/* -*- mode: c++; c-basic-offset: 3 -*-
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
 * @author Regis Clouard - 2003-05-22
 */

#define _USE_MATH_DEFINES
#include <math.h>
#include <pandore.h>
using namespace pandore;
#include "morphology/structelt.h"

/**
 * @file psedesign.cpp
 * Builds a new structuring element as an image.
 */

Img2duc* PSEDesign2D( int numSE, int halfsize ) {
   StructuringElement2d *se=new StructuringElement2d(numSE, halfsize);   
   Img2duc *imse = new Img2duc(halfsize*2+1,halfsize*2+1);
   Point2d shift(halfsize,halfsize);

   *imse=0;
   for (int i=0; i<se->length; i++) {
      Point2d p=shift;
      se->Move(i,p);
      (*imse)[p]=255;
   }
   return imse;
}

Img3duc* PSEDesign3D( int numSE, int halfsize ) {
   StructuringElement3d *se=new StructuringElement3d(numSE-10, halfsize);   
   Img3duc *imse = new Img3duc(halfsize*2+1, halfsize*2+1,halfsize*2+1);
   Point3d shift(halfsize,halfsize,halfsize);

   *imse=0;
   for (int i=0; i<se->length; i++) {
      Point3d p=shift;
      se->Move(i,p);
      (*imse)[p]=255;
   }

   return imse;
}

#ifdef MAIN
#define	USAGE	"usage: %s num_se halfsize [im_out|-]"
#define	PARC	2
#define	FINC	0
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

   if ((atoi(parv[0]) >= 0) && (atoi(parv[0]) < 9)) {
      objd[0]=PSEDesign2D(atoi(parv[0]),atoi(parv[1]));
      goto end;
   }
   if ((atoi(parv[0]) > 9) && (atoi(parv[0]) < 23)) {
      objd[0]=PSEDesign3D(atoi(parv[0]),atoi(parv[1]));
      goto end;
   }
   {
      PrintErrorFormat(objin,FINC);
      result = FAILURE;
   }	
   
end:
  WriteArgs(argc,argv,PARC,FINC,FOUTC,&mask,objin,objs,objout,objd,MASK);
  Exit(result);
  return 0;
}
#endif
