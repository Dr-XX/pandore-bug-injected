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
 * @author Jalal Fadili - 2010-08-30
 */

#include <math.h>
#include <pandore.h>
using namespace pandore;

/**
 * @file pdivneumann.cpp
 * Computes the divergence by backward finite differences,
 * ie. ud(i,j) = (u1(i,j)-u1(i-1,j))/(h) + (u2(i,j)-u2(i,j-1))/(h),
 * with special care (Neumann) at boundaries (u1(1,j) and -u1(n-1,j)),
 * (u2(i,1) and -u2(i,n-1))
 */

template<typename U>
Errc PDivNeumann( const Img2d<U> &ims1, Img2d<U> &ims2, Img2dsf &imd ) {
   Img2dsf dx,dy;
   register int x,y;
   
   if (ims1.Width() != ims2.Width() || ims1.Height() != ims2.Height()) {
      fprintf(stderr,"Error pdivneumann: Input images must have the same dimensions.\n");
      Exit(FAILURE);
   }

   dy.New(ims1.Props());
   dx.New(ims2.Props());

   for (y=1; y<ims1.Height()-1; y++) {
      for (x=0; x<ims1.Width(); x++) {
	 // Backward differences.
	 dy[y][x] = Float(ims1[y][x] - ims1[y-1][x]);
      }
   }
   for (x=0; x<ims1.Width(); x++) { dy[0][x] = Float(ims1[0][x]);}
   for (x=0; x<ims1.Width(); x++) { dy[ims1.Height()-1][x] = Float(-ims1[ims1.Height()-2][x]);}

   for (x=1; x<ims2.Width()-1; x++)
      for (y=0; y<ims2.Height(); y++) {
	 // Backward differences.
	 dx[y][x] = Float(ims2[y][x] - ims2[y][x-1]);
      }
   for (y=0; y<ims2.Height(); y++) { dx[y][0] = Float(ims2[y][0]);}
   for (y=0; y<ims2.Height(); y++) { dx[y][ims2.Width()-1] = Float(-ims2[y][ims2.Width()-2]);}
   
   for (y=0; y<ims1.Height(); y++)
      for (x=0; x<ims1.Width(); x++)
	 imd[y][x] = dy[y][x]+dx[y][x];
   
   return SUCCESS;
}

#ifdef MAIN

/*
 * Modify only the following constants, and the function call.
 */
#define	USAGE	"Usage: %s[-m mask] [im_in1|-] [im_in2|-] [im_out|-]"
#define	PARC	0
#define	FINC	2
#define	FOUTC	1
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
   if (objs[0]->Type()==Po_Img2duc && objs[1]->Type()==Po_Img2duc) {
      Img2duc* const ims1=(Img2duc*)objs[0];
      Img2duc* const ims2=(Img2duc*)objs[1];

      objd[0]=new Img2dsf(ims1->Size());
      Img2dsf* const imd=(Img2dsf*)objd[0];
      *imd = 0;
      result=PDivNeumann(*ims1,*ims2,*imd);
      goto end;
   }

   if (objs[0]->Type()==Po_Img2dsl && objs[1]->Type()==Po_Img2dsl) {
      Img2dsl* const ims1=(Img2dsl*)objs[0];
      Img2dsl* const ims2=(Img2dsl*)objs[1];

      objd[0]=new Img2dsf(ims1->Size());
      Img2dsf* const imd=(Img2dsf*)objd[0];
      *imd = 0;
      result=PDivNeumann(*ims1,*ims2,*imd);
      goto end;
   }

   if (objs[0]->Type()==Po_Img2dsf && objs[0]->Type()==Po_Img2dsf) {
      Img2dsf* const ims1=(Img2dsf*)objs[0];
      Img2dsf* const ims2=(Img2dsf*)objs[1];

      objd[0]=new Img2dsf(ims1->Size());
      Img2dsf* const imd=(Img2dsf*)objd[0];
      *imd = 0;
      result=PDivNeumann(*ims1,*ims2,*imd);
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
