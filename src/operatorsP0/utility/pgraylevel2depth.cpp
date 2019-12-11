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
 * @author Jean-Marie Janik - 1998-07-19
 * @author Regis Clouard - 2002-06-06
 * @author Regis Clouard - 2004-03-02
 */

#include <math.h>
#include <pandore.h>
using namespace pandore;

/**
 * @file pgraylevel2depth.cpp
 * Build 3D image from 2D image where gray levels
 * are converted to depth.
 * Result image is binary (0 , 255).
 */

/**
 * Convert gray level to depth value.
 */

Errc PGraylevel2Depth( const Img2duc &ims, Img3duc &imd, long size ) {
   register int x, y, z;
   double max = ims[0][0];
   double min =  ims[0][0];
   for (y = 0; y < ims.Height(); y++) {
      for (x = 0; x < ims.Width(); x++) {
	 if (max < ims(y, x)) {
	    max = ims(y, x);
	 }
	 if (min > ims(y, x)) {
	    min = ims(y, x);
	 }
      }
   }
   
   for (y = 0; y < ims.Height(); y++) {
      for (x = 0; x < ims.Width(); x++) {
	 z = (long)floor(((ims(y, x) - min) * (float)size) / (max - min));
	 Uchar val = (z > MAXUCHAR)?  MAXUCHAR: (Uchar)z;
 	 for (--z; z >= 0; z--) {
	    imd(imd.Depth() - 1 - z, y, x) = val;
	 }
      }
   }
   return SUCCESS;
}

Errc PGraylevel2Depth( const Img2dsl &ims, Img3duc &imd, long size ) {
   register int x, y, z;
   double max = ims[0][0];
   double min =  ims[0][0];
   for (y = 0; y < ims.Height(); y++) {
      for (x = 0; x < ims.Width(); x++) {
	 if (max < ims(y, x)) {
	    max = ims(y, x);
	 }
	 if (min > ims(y, x)) {
	    min = ims(y, x);
	 }
      }
   }
   
   for (y = 0; y < ims.Height(); y++) {
      for (x = 0; x < ims.Width(); x++) {
	 z = (long)floor(((ims(y, x) - min) * (float)size) / (max - min));
	 Uchar val = (z > MAXUCHAR)?  MAXUCHAR: (Uchar)z;
 	 for (--z; z >= 0; z--) {
	    imd(imd.Depth() - 1 - z, y, x) = val;
	 }
      }
   }
   return SUCCESS;
}

Errc PGraylevel2Depth( const Img2dsf &ims, Img3duc &imd, long size ) {
   register int x, y, z;
   double max = ims[0][0];
   double min =  ims[0][0];
   for (y = 0; y < ims.Height(); y++) {
      for (x = 0; x < ims.Width(); x++) {
	 if (max < ims(y, x)) {
	    max = ims(y, x);
	 }
	 if (min > ims(y, x)) {
	    min = ims(y, x);
	 }
      }
   }
   
   for (y = 0; y < ims.Height(); y++) {
      for (x = 0; x < ims.Width(); x++) {
	 z = (long)floor(((ims(y, x) - min) * (float)size) / (max - min));
	 Uchar val = (z > MAXUCHAR)?  MAXUCHAR: (Uchar)z;
 	 for (--z; z >= 0; z--) {
	    imd(imd.Depth() - 1 - z, y, x) = val;
	 }
      }
   }
   return SUCCESS;
}


#ifdef MAIN
#define	USAGE	"usage: %s depth [im_in|-] [im_out|-]"
#define	PARC	1
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
   if (objs[0]->Type() == Po_Img2duc) {
      int depth = atoi(parv[0]);
      if (depth < 0) {
	 fprintf(stderr, "Error pgraylevel2depth: Bad depth value: %d\n", depth);
	 Exit(FAILURE);
      }

      Img2duc* const ims = (Img2duc*)objs[0];
      objd[0] = new Img3duc(depth, ims->Height(), ims->Width());
      Img3duc* const imd = (Img3duc*)objd[0];

      result = PGraylevel2Depth(*ims, *imd, depth);
      goto end;
   }
   if (objs[0]->Type() == Po_Img2dsl) {
      int depth = atoi(parv[0]);
      if (depth < 0) {
	 fprintf(stderr, "Error pgraylevel2depth: Bad depth value: %d\n", depth);
	 Exit(FAILURE);
      }

      Img2dsl* const ims = (Img2dsl*)objs[0];
      objd[0] = new Img3duc(depth, ims->Height(), ims->Width());
      Img3duc* const imd = (Img3duc*)objd[0];

      result = PGraylevel2Depth(*ims, *imd, depth);
      goto end;
   }
   if (objs[0]->Type() == Po_Img2dsf) {
      int depth = atoi(parv[0]);
      if (depth < 0) {
	 fprintf(stderr, "Error pgraylevel2depth: Bad depth value: %d\n", depth);
	 Exit(FAILURE);
      }

      Img2dsf* const ims = (Img2dsf*)objs[0];
      objd[0] = new Img3duc(depth, ims->Height(), ims->Width());
      Img3duc* const imd = (Img3duc*)objd[0];

      result = PGraylevel2Depth(*ims, *imd, depth);
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
