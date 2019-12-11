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
 * @file pgetsubband.cpp
 * Extract subimage from dwt image.
 */

/**
 * cx,cy les coordonnees du coin superieur gauche de
 * la sous image à extraire.
 */
Errc PGetSubband( const Img2dsf &ims, Img2dsf &imd, int cy, int cx ) {
   Point2d p,q;
   
   q.x = cx;
   q.y = cy;
   for (p.y = 0; p.y < imd.Height(); p.y++) {
      for (p.x = 0; p.x < imd.Width(); p.x++) {
	 imd[p] = ims[p + q];
      }
   }
   
   return SUCCESS;
}

#ifdef MAIN
#define	USAGE	"usage: %s scale number [im_in|-][im_out|-]"
#define	PARC	2
#define	FINC	1
#define	FOUTC	1
#define	MASK	0

int main( int argc, char *argv[] ) {
  Errc result;                // The result code of the execution.
  Pobject *mask;              // The mask.
  Pobject *objin[FINC + 1];   // The input objects.
  Pobject *objs[FINC + 1];    // The source objects masked by mask.
  Pobject *objout[FOUTC + 1]; // The output object unmasked by mask.
  Pobject *objd[FOUTC + 1];   // The result object.
  char* parv[PARC + 1];       // The input parameters.
  
  ReadArgs(argc, argv, PARC, FINC, FOUTC, &mask, objin, objs, objout, objd, parv, USAGE, MASK);
  
  switch(objs[0]->Type()) {
  case Po_Img2dsf : {
     int scale = atoi(parv[0]);
     Img2dsf* const ims = (Img2dsf*)objs[0];
     
     int I = ims->Height() >> (scale);
     int J = ims->Width() >> (scale);
     
     objd[0] = new Img2dsf(I, J);
     
     Img2dsf* const imd = (Img2dsf*)objd[0];
     
     if (atoi(parv[1]) == 1) {
	result = PGetSubband(*ims, *imd, 0, 0);
     } else if (atoi(parv[1]) == 2) {
	result = PGetSubband(*ims, *imd, 0, J);
     } else if (atoi(parv[1]) == 3) {
	result = PGetSubband(*ims, *imd, I, 0);
     } else if (atoi(parv[1]) ==4 ) {
	result = PGetSubband(*ims, *imd, I, J);
     }
  } break;
  default:
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
