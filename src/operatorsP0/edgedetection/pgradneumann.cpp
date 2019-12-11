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
 * https://clouard.users.greyc.fr/Pandore/
 */

/**
 * @author Jalal Fadili - 2010-08-30
 */

#include <math.h>
#include <pandore.h>
using namespace pandore;

/**
 * @file pgradneumann.cpp
 * Computes the discrete gradient of an image by forward
 * finite differences and Neuman boundary conditions,
 * ie. ud1(i,j) = (u(i+1,j)-u(i,j))/(h),
 * ud2(i,j) = (u(i,j+1)-u(i,j))/(h), with ud(n-1) = 0.
 * Returns ud1 and ud2.
 */

template<typename U>
Errc PGradNeumann( const Img2d<U> &ims, Img2d<U> &imd1, Img2d<U> &imd2 ) {
   register int x,y;

   imd1 = 0;
   imd2 = 0;

   for (y=0; y<ims.Height()-1; y++)
      for (x=0; x<ims.Width()-1; x++) {
	 // Forward differences.
	 imd1[y][x] = ims[y+1][x] - ims[y][x];
	 imd2[y][x] = ims[y][x+1] - ims[y][x];
      }

   for (y=0; y<ims.Height()-1; y++)  imd1[y][x] = ims[y+1][x] - ims[y][x];
   for (x=0; x<ims.Width()-1; x++)   imd2[y][x] = ims[y][x+1] - ims[y][x];
   
   return SUCCESS;
}

#ifdef MAIN

/*
 * Modify only the following constants, and the function call.
 */
#define	USAGE	"Usage: %s[-m mask] [im_in|-] [im_out1|-] [im_out2|-]"
#define	PARC	0
#define	FINC	1
#define	FOUTC	2
#define	MASK	2 // Only masking operation.

int main( int argc, char *argv[] ) {
   Errc result;              // The result code of the execution.
   Pobject* mask;            // The region map.
   Pobject* objin[FINC+1];   // The input objects.
   Pobject* objs[FINC+1];    // The source objects masked.
   Pobject* objout[FOUTC+1]; // The output object.
   Pobject* objd[FOUTC+1];   // The result object of the execution.
   char* parv[PARC+1];       // The input parameters.

   ReadArgs(argc,argv,PARC,FINC,FOUTC,&mask,objin,objs,objout,objd,parv,USAGE,MASK);
   if (objs[0]->Type()==Po_Img2duc) {
      Img2duc* const ims=(Img2duc*)objs[0];
      objd[0]=new Img2duc(ims->Size());
      objd[1]=new Img2duc(ims->Size());
      Img2duc* const imd1=(Img2duc*)objd[0];
      Img2duc* const imd2=(Img2duc*)objd[1];
      result=PGradNeumann(*ims,*imd1,*imd2);
      goto end;
   }

   if (objs[0]->Type()==Po_Img2dsl) {
      Img2dsl* const ims=(Img2dsl*)objs[0];
      objd[0]=new Img2dsl(ims->Size());
      objd[1]=new Img2dsl(ims->Size());
      Img2dsl* const imd1=(Img2dsl*)objd[0];
      Img2dsl* const imd2=(Img2dsl*)objd[1];
      result=PGradNeumann(*ims,*imd1,*imd2);
      goto end;
   }

   if (objs[0]->Type()==Po_Img2dsf) {
      Img2dsf* const ims=(Img2dsf*)objs[0];
      objd[0]=new Img2dsf(ims->Size());
      objd[1]=new Img2dsf(ims->Size());
      Img2dsf* const imd1=(Img2dsf*)objd[0];
      Img2dsf* const imd2=(Img2dsf*)objd[1];
      result=PGradNeumann(*ims,*imd1,*imd2);
      goto end;
   }

  {
     PrintErrorFormat(objin,FINC);
     result = FAILURE;
  }	

end:
  if (result)
	WriteArgs(argc,argv,PARC,FINC,FOUTC,&mask,objin,objs,objout,objd,MASK);
  Exit(result);
  return 0;
}

#endif
