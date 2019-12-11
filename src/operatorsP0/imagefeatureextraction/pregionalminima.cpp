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
 * @author Régis Clouard  - 2014-12-09
 */

#include <pandore.h>
using namespace pandore;

/**
 * @file pregionalminima.cpp
 */


Errc PRegionalMinima( const Img1duc &ims, Img1duc &imd, int length ) {
   if (length > ims.Width()) {
      std::cerr << "Error pregionalminima: length > image size" << std::endl;
      return FAILURE;
   }
   
   Img1duc imi(ims.Props());
   
   Point1d p;
   for (p.x = 0; p.x < ims.Width(); p.x++) {
      imi[p] = ims[p];
      for (int v = 0; v < 2; v++) {
	 if (ims.Hold(p + v2[v]) && ims[p + v2[v]] < ims[p]) {
	    imi[p] = Limits<Img1duc::ValueType>::max();
	    break;
	 }
      }
   }
   
   for (p.x = 0 ; p.x < imi.Width(); p.x++) {
      Img1duc::ValueType min = imi[p];
      for (int k = -length/2; k < length/2; k++) {
   	 if (imi.Hold(p + k) && imi[p.x + k] < min) {
   	    min = imi[p + k];
   	 }
      }
      if (min == Limits<Img1duc::ValueType>::max()) {
	 min = 0;
      }
      if (imi[p.x] == min) {
	 imd[p] = 255;
      } else {
	 imd[p] = 0;
      }
   }
   return SUCCESS;
}

Errc PRegionalMinima( const Img1dsl &ims, Img1dsl &imd, int length ) {
   if (length > ims.Width()) {
      std::cerr << "Error pregionalminima: length > image size" << std::endl;
      return FAILURE;
   }
   
   Img1dsl imi(ims.Props());
   
   Point1d p;
   for (p.x = 0; p.x < ims.Width(); p.x++) {
      imi[p] = ims[p];
      for (int v = 0; v < 2; v++) {
	 if (ims.Hold(p + v2[v]) && ims[p + v2[v]] < ims[p]) {
	    imi[p] = Limits<Img1dsl::ValueType>::max();
	    break;
	 }
      }
   }
   
   for (p.x = 0 ; p.x < imi.Width(); p.x++) {
      Img1dsl::ValueType min = imi[p];
      for (int k = -length/2; k < length/2; k++) {
   	 if (imi.Hold(p + k) && imi[p.x + k] < min) {
   	    min = imi[p + k];
   	 }
      }
      if (min == Limits<Img1dsl::ValueType>::max()) {
	 min = 0;
      }
      if (imi[p.x] == min) {
	 imd[p] = 255;
      } else {
	 imd[p] = 0;
      }
   }
   return SUCCESS;
}

Errc PRegionalMinima( const Img1dsf &ims, Img1dsf &imd, int length ) {
   if (length > ims.Width()) {
      std::cerr << "Error pregionalminima: length > image size" << std::endl;
      return FAILURE;
   }
   
   Img1dsf imi(ims.Props());
   
   Point1d p;
   for (p.x = 0; p.x < ims.Width(); p.x++) {
      imi[p] = ims[p];
      for (int v = 0; v < 2; v++) {
	 if (ims.Hold(p + v2[v]) && ims[p + v2[v]] < ims[p]) {
	    imi[p] = Limits<Img1dsf::ValueType>::max();
	    break;
	 }
      }
   }
   
   for (p.x = 0 ; p.x < imi.Width(); p.x++) {
      Img1dsf::ValueType min = imi[p];
      for (int k = -length/2; k < length/2; k++) {
   	 if (imi.Hold(p + k) && imi[p.x + k] < min) {
   	    min = imi[p + k];
   	 }
      }
      if (min == Limits<Img1dsf::ValueType>::max()) {
	 min = 0;
      }
      if (imi[p.x] == min) {
	 imd[p] = 255;
      } else {
	 imd[p] = 0;
      }
   }
   return SUCCESS;
}

Errc PRegionalMinima( const Img2duc &ims, Img2duc &imd, int length ) {
   if (length > ims.Width()) {
      std::cerr << "Error pregionalminima: length > image size" << std::endl;
      return FAILURE;
   }
   
   Img2duc imi(ims.Props());
   
   Point2d p;
   for (p.y = 0; p.y < ims.Height(); p.y++) {
      for (p.x = 0; p.x < ims.Width(); p.x++) {
	 imi[p] = ims[p];
	 for (int v = 0; v < 8; v++) {
	    if (ims.Hold(p + v8[v]) && ims[p + v8[v]] < ims[p]) {
	       imi[p] = Limits<Img2duc::ValueType>::max();
	       break;
	    }
	 }
      }
   }
   
   for (p.y = 0 ; p.y < imi.Height(); p.y++) {
      for (p.x = 0 ; p.x < imi.Width(); p.x++) {
	 Img2duc::ValueType min = imi[p];
	 Point2d q;
	 for (q.y = -length/2; q.y < length/2; q.y++) {
	    for (q.x = -length/2; q.x < length/2; q.x++) {
	       if (imi.Hold(p + q) && imi[p + q] < min) {
		  min = imi[p + q];
	       }
	    }
	 }
	 if (min == Limits<Img2duc::ValueType>::max()) {
	    imd[p] = 0;
	 } else if (imi[p] == min) {
	    imd[p] = 255;
	 } else {
	    imd[p] = 0;
	 }
      }
   }
   return SUCCESS;
}

Errc PRegionalMinima( const Img2dsl &ims, Img2dsl &imd, int length ) {
   if (length > ims.Width()) {
      std::cerr << "Error pregionalminima: length > image size" << std::endl;
      return FAILURE;
   }
   
   Img2dsl imi(ims.Props());
   
   Point2d p;
   for (p.y = 0; p.y < ims.Height(); p.y++) {
      for (p.x = 0; p.x < ims.Width(); p.x++) {
	 imi[p] = ims[p];
	 for (int v = 0; v < 8; v++) {
	    if (ims.Hold(p + v8[v]) && ims[p + v8[v]] < ims[p]) {
	       imi[p] = Limits<Img2dsl::ValueType>::max();
	       break;
	    }
	 }
      }
   }
   
   for (p.y = 0 ; p.y < imi.Height(); p.y++) {
      for (p.x = 0 ; p.x < imi.Width(); p.x++) {
	 Img2dsl::ValueType min = imi[p];
	 Point2d q;
	 for (q.y = -length/2; q.y < length/2; q.y++) {
	    for (q.x = -length/2; q.x < length/2; q.x++) {
	       if (imi.Hold(p + q) && imi[p + q] < min) {
		  min = imi[p + q];
	       }
	    }
	 }
	 if (min == Limits<Img2dsl::ValueType>::max()) {
	    imd[p] = 0;
	 } else if (imi[p] == min) {
	    imd[p] = 255;
	 } else {
	    imd[p] = 0;
	 }
      }
   }
   return SUCCESS;
}

Errc PRegionalMinima( const Img2dsf &ims, Img2dsf &imd, int length ) {
   if (length > ims.Width()) {
      std::cerr << "Error pregionalminima: length > image size" << std::endl;
      return FAILURE;
   }
   
   Img2dsf imi(ims.Props());
   
   Point2d p;
   for (p.y = 0; p.y < ims.Height(); p.y++) {
      for (p.x = 0; p.x < ims.Width(); p.x++) {
	 imi[p] = ims[p];
	 for (int v = 0; v < 8; v++) {
	    if (ims.Hold(p + v8[v]) && ims[p + v8[v]] < ims[p]) {
	       imi[p] = Limits<Img2dsf::ValueType>::max();
	       break;
	    }
	 }
      }
   }
   
   for (p.y = 0 ; p.y < imi.Height(); p.y++) {
      for (p.x = 0 ; p.x < imi.Width(); p.x++) {
	 Img2dsf::ValueType min = imi[p];
	 Point2d q;
	 for (q.y = -length/2; q.y < length/2; q.y++) {
	    for (q.x = -length/2; q.x < length/2; q.x++) {
	       if (imi.Hold(p + q) && imi[p + q] < min) {
		  min = imi[p + q];
	       }
	    }
	 }
	 if (min == Limits<Img2dsf::ValueType>::max()) {
	    imd[p] = 0;
	 } else if (imi[p] == min) {
	    imd[p] = 255;
	 } else {
	    imd[p] = 0;
	 }
      }
   }
   return SUCCESS;
}


#ifdef MAIN
#define	USAGE	"usage: %s length [-m mask] [im_in|-] [im_out|-]"
#define	PARC	1
#define	FINC	1
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
   if (objs[0]->Type() == Po_Img1duc) {
      Img1duc* const ims = (Img1duc*)objs[0];
      objd[0] = new Img1duc(ims->Size());
      Img1duc* const imd = (Img1duc*)objd[0];
      
      result = PRegionalMinima(*ims, *imd, atoi(parv[0]));
      goto end;
   }
   if (objs[0]->Type() == Po_Img1dsl) {
      Img1dsl* const ims = (Img1dsl*)objs[0];
      objd[0] = new Img1dsl(ims->Size());
      Img1dsl* const imd = (Img1dsl*)objd[0];
      
      result = PRegionalMinima(*ims, *imd, atoi(parv[0]));
      goto end;
   }
   if (objs[0]->Type() == Po_Img1dsf) {
      Img1dsf* const ims = (Img1dsf*)objs[0];
      objd[0] = new Img1dsf(ims->Size());
      Img1dsf* const imd = (Img1dsf*)objd[0];
      
      result = PRegionalMinima(*ims, *imd, atoi(parv[0]));
      goto end;
   }
   if (objs[0]->Type() == Po_Img2duc) {
      Img2duc* const ims = (Img2duc*)objs[0];
      objd[0] = new Img2duc(ims->Size());
      Img2duc* const imd = (Img2duc*)objd[0];
      
      result = PRegionalMinima(*ims, *imd, atoi(parv[0]));
      goto end;
   }
   if (objs[0]->Type() == Po_Img2dsl) {
      Img2dsl* const ims = (Img2dsl*)objs[0];
      objd[0] = new Img2dsl(ims->Size());
      Img2dsl* const imd = (Img2dsl*)objd[0];
      
      result = PRegionalMinima(*ims, *imd, atoi(parv[0]));
      goto end;
   }
   if (objs[0]->Type() == Po_Img2dsf) {
      Img2dsf* const ims = (Img2dsf*)objs[0];
      objd[0] = new Img2dsf(ims->Size());
      Img2dsf* const imd = (Img2dsf*)objd[0];
      
      result = PRegionalMinima(*ims, *imd, atoi(parv[0]));
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
