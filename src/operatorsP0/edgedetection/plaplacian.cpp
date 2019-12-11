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

#include <pandore.h>
using namespace pandore;

/**
 * @file plaplacian.cpp
 * Laplacian approximation.
 *
 * Consultation by masks.
 *	 0 -1  0			 -1 -1 -1
 *	-1  4 -1 for 4-connexity       	 -1  8 -1 for 8-connexity
 *	 0 -1  0			 -1 -1 -1
 *
 * Remark: for unsigned images, shifts pixel value in order
 * to set 0 at the max()/2 position. For example, in case of
 * Uchar set 0 at 127: all pixel values below 0 are negative
 * and all pixel values above are positive.
 */
static signed char mq4[3][3] = {{0, -1, 0}, {-1, 4, -1}, {0, -1, 0}};
static signed char mq8[3][3] = {{-1, -1, -1}, {-1, 8, -1}, {-1, -1, -1}};



Errc PLaplacian( const Img2duc &ims, Img2duc &imd, int connexity ) {
   double gxy, amp;
   int k, l;
   Point2d p;
   Img2duc::ValueType offset = (Limits<Img2duc::ValueType>::max()+1)/2;

   imd.Frame(offset, 1, 1);
   if (connexity <= 4)		// 4-connexity
   for (p.y = 1; p.y < ims.Height() - 1; p.y++)
   for (p.x = 1; p.x < ims.Width() - 1; p.x++)
      {
	 // Convolution.
	 gxy = 0.0;
	 for (k = -1; k <= 1; k++) {
	    for (l = -1; l <= 1; l++) {
	       gxy += ims[p.y + l][p.x + k] * (double)mq4[l + 1][k + 1];
	    }
	 }
	 // Decalage pour que 0 = max()/2;
	 amp = gxy / 4 + offset;
	 if (amp > Limits<Img2duc::ValueType>::max()) {
	    imd[p] = Limits<Img2duc::ValueType>::max();
	 } else if (amp < 0) {
	    imd[p] = 0;
	 } else {
	    imd[p] = (Img2duc::ValueType)amp;
	 }
      }
   else // 8-connexity
   for (p.y = 1; p.y < ims.Height() - 1; p.y++)
   for (p.x = 1; p.x < ims.Width() - 1; p.x++)
      {
	 // Convolution.
	 gxy = 0.0;
	 for (k = -1; k <= 1; k++) {
	    for (l = -1; l <= 1; l++) {
	       gxy += ims[p.y + l][p.x + k] * (double)mq8[l + 1][k + 1];
	    }
	 }
	 // Shift in order to set 0 at max()/2 for unsigned images.
	 amp = gxy / 8 + offset;
	 if (amp > Limits<Img2duc::ValueType>::max()) {
	    imd[p] = Limits<Img2duc::ValueType>::max();
	 } else if (amp < 0) {
	    imd[p] = 0;
	 } else {
	    imd[p] = (Img2duc::ValueType)amp;
	 }
      }
   return SUCCESS;
}

Errc PLaplacian( const Img2dsl &ims, Img2dsl &imd, int connexity ) {
   double gxy, amp;
   int k, l;
   Point2d p;

   imd.Frame(0, 1, 1);
   if (connexity <= 4)		// 4-connexity
   for (p.y = 1; p.y < ims.Height() - 1; p.y++)
   for (p.x = 1; p.x < ims.Width() - 1; p.x++)
      {
	 // Convolution.
	 gxy = 0.0;
	 for (k = -1; k <= 1; k++) {
	    for (l=-1; l<=1; l++) {
	       gxy += ims[p.y + l][p.x + k] * (double)mq4[l + 1][k + 1];
	    }
	 }
	 amp = gxy / 4;
	 if (amp > Limits<Img2dsl::ValueType>::max()) {
	    imd[p] = Limits<Img2dsl::ValueType>::max();
	 } else if (amp < Limits<Img2dsl::ValueType>::min()) {
	    imd[p] = Limits<Img2dsl::ValueType>::min();
	 } else {
	    imd[p] = (Img2dsl::ValueType)amp;
	 }
      }
   else // 8-connexity
   for (p.y = 1; p.y < ims.Height() - 1; p.y++)
   for (p.x = 1; p.x < ims.Width() - 1; p.x++)
      {
	 // Convolution.
	 gxy = 0.0;
	 for (k = -1; k <= 1; k++) {
	    for (l = -1; l <= 1; l++) {
	       gxy += ims[p.y + l][p.x + k] * (double)mq8[l + 1][k + 1];
	    }
	 }
	 amp = gxy / 8;
	 if (amp > Limits<Img2dsl::ValueType>::max()) {
	    imd[p] = Limits<Img2dsl::ValueType>::max();
	 } else if (amp < Limits<Img2dsl::ValueType>::min()) {
	    imd[p] = Limits<Img2dsl::ValueType>::min();
	 } else {
	    imd[p] = (Img2dsl::ValueType)amp;
	 }
      }
   return SUCCESS;
}

Errc PLaplacian( const Img2dsf &ims, Img2dsf &imd, int connexity ) {
   double gxy, amp;
   int k, l;
   Point2d p;

   imd.Frame(0, 1, 1);
   if (connexity <= 4)		// 4-connexity
   for (p.y = 1; p.y < ims.Height() - 1; p.y++)
   for (p.x = 1; p.x < ims.Width() - 1; p.x++)
      {
	 // Convolution.
	 gxy = 0.0;
	 for (k = -1; k <= 1; k++) {
	    for (l=-1; l<=1; l++) {
	       gxy += ims[p.y + l][p.x + k] * (double)mq4[l + 1][k + 1];
	    }
	 }
	 amp = gxy / 4;
	 if (amp > Limits<Img2dsf::ValueType>::max()) {
	    imd[p] = Limits<Img2dsf::ValueType>::max();
	 } else if (amp < Limits<Img2dsf::ValueType>::min()) {
	    imd[p] = Limits<Img2dsf::ValueType>::min();
	 } else {
	    imd[p] = (Img2dsf::ValueType)amp;
	 }
      }
   else // 8-connexity
   for (p.y = 1; p.y < ims.Height() - 1; p.y++)
   for (p.x = 1; p.x < ims.Width() - 1; p.x++)
      {
	 // Convolution.
	 gxy = 0.0;
	 for (k = -1; k <= 1; k++) {
	    for (l = -1; l <= 1; l++) {
	       gxy += ims[p.y + l][p.x + k] * (double)mq8[l + 1][k + 1];
	    }
	 }
	 amp = gxy / 8;
	 if (amp > Limits<Img2dsf::ValueType>::max()) {
	    imd[p] = Limits<Img2dsf::ValueType>::max();
	 } else if (amp < Limits<Img2dsf::ValueType>::min()) {
	    imd[p] = Limits<Img2dsf::ValueType>::min();
	 } else {
	    imd[p] = (Img2dsf::ValueType)amp;
	 }
      }
   return SUCCESS;
}


#ifdef MAIN
#define	USAGE	"usage: %s connectivity [-m mask] [im_src|-] [im_out|-]"
#define	PARC	1
#define	FINC	1
#define	FOUTC	1
#define MASK	2

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
      Img2duc* const ims = (Img2duc*)objs[0];
      objd[0] = new Img2duc(ims->Props());		  
      Img2duc* const imd = (Img2duc*)objd[0];
      
      result = PLaplacian(*ims, *imd, atoi(parv[0]));
      goto end;
   }

   if (objs[0]->Type() == Po_Img2dsl) {
      Img2dsl* const ims = (Img2dsl*)objs[0];
      objd[0] = new Img2dsl(ims->Props());		  
      Img2dsl* const imd = (Img2dsl*)objd[0];
      
      result = PLaplacian(*ims, *imd, atoi(parv[0]));
      goto end;
   }

   if (objs[0]->Type() == Po_Img2dsf) {
      Img2dsf* const ims = (Img2dsf*)objs[0];
      objd[0] = new Img2dsf(ims->Props());		  
      Img2dsf* const imd = (Img2dsf*)objd[0];
      
      result = PLaplacian(*ims, *imd, atoi(parv[0]));
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
