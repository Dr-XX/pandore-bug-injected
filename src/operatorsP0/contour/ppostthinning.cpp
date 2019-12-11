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
 * @author Régis Clouard - 1996-02-19
 */

#include <pandore.h>
using namespace pandore;

/**
 * @file ppostthinning.cpp
 * Suppression des points non necessaires
 * a la preservation de la connexite.
 */

Errc PPostThinning( const Img2duc &ims, Img2duc &imd ) {
   Point2d p;
   int	v;
   
   imd = ims;    // * ecopie de l'image src dans dest.
   
   for (p.y = 1; p.y < ims.Height() - 1; p.y++)
      for (p.x = 1; p.x < ims.Width() - 1; p.x++) {
	 if ((imd[p])) {
	    for (v = 0; v < 8; v+= 2) {
	       if (imd[p + v8[v]] && imd[p + v8[(v + 2) % 8]] && (!imd[p + v8[(v + 5) % 8]])) {
		  imd[p] = 0;
		  break;
	       }
	    }
	 }
      }
   
   return SUCCESS;
}


Errc PPostThinning( const Img2dsl &ims, Img2dsl &imd ) {
   Point2d p;
   int	v;
   
   imd = ims;    // * ecopie de l'image src dans dest.
   
   for (p.y = 1; p.y < ims.Height() - 1; p.y++)
      for (p.x = 1; p.x < ims.Width() - 1; p.x++) {
	 if ((imd[p])) {
	    for (v = 0; v < 8; v+= 2) {
	       if (imd[p + v8[v]] && imd[p + v8[(v + 2) % 8]] && (!imd[p + v8[(v + 5) % 8]])) {
		  imd[p] = 0;
		  break;
	       }
	    }
	 }
      }
   
   return SUCCESS;
}


Errc PPostThinning( const Img2dsf &ims, Img2dsf &imd ) {
   Point2d p;
   int	v;
   
   imd = ims;    // * ecopie de l'image src dans dest.
   
   for (p.y = 1; p.y < ims.Height() - 1; p.y++)
      for (p.x = 1; p.x < ims.Width() - 1; p.x++) {
	 if ((imd[p])) {
	    for (v = 0; v < 8; v+= 2) {
	       if (imd[p + v8[v]] && imd[p + v8[(v + 2) % 8]] && (!imd[p + v8[(v + 5) % 8]])) {
		  imd[p] = 0;
		  break;
	       }
	    }
	 }
      }
   
   return SUCCESS;
}



#ifdef MAIN
#define	USAGE	"usage: %s [-m mask] [im_in|-] [im_out|-]"
#define	PARC	0
#define	FINC	1
#define	FOUTC	1
#define	MASK	1

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
      objd[0] = new Img2duc(ims->Size());
      Img2duc* const imd = (Img2duc*)objd[0];
      
      result = PPostThinning(*ims, *imd);
      goto end;
}
   if (objs[0]->Type() == Po_Img2dsl) {
      Img2dsl* const ims = (Img2dsl*)objs[0];
      objd[0] = new Img2dsl(ims->Size());
      Img2dsl* const imd = (Img2dsl*)objd[0];
      
      result = PPostThinning(*ims, *imd);
      goto end;
}
   if (objs[0]->Type() == Po_Img2dsf) {
      Img2dsf* const ims = (Img2dsf*)objs[0];
      objd[0] = new Img2dsf(ims->Size());
      Img2dsf* const imd = (Img2dsf*)objd[0];
      
      result = PPostThinning(*ims, *imd);
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
