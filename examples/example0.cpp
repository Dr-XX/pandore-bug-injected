/* -*- mode: c++; c-basic-offset: 3 -*-
 *
 * Copyright (c) 2013, GREYC.
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

/*
 * @author Régis Clouard - 2001-02-01
 */

/*
 * @file example0.cc
 * Example of operator source.
 */


#include <stdio.h>
#include <pandore.h>
using namespace pandore;

/*      
 * Morphologie mathematique:
 * Erosion d'une image: recherche du minimum parmi les voisins.
 */
Errc Erosion( const Img2duc &ims, Img2duc &imd, int connexite = 4 ) {
   Point2d p;
   Img2duc::ValueType min, val;
  
   imd.Frame(0, 1);
   if (connexite == 4) {
      for (p.y = 1; p.y < ims.Height() - 1; p.y++) {
	 for (p.x = 1; p.x < ims.Width() - 1; p.x++) {
	    min  =ims[p];
	    for (int v = 0; v < 4; v++){
	       if ((val = ims[p+v4[v]]) < min) {
		  min = val;
	       }
	    }
	    imd[p] = min;
	 }
      }
   } else { // connexite == 8.
      for (p.y = 1; p.y < ims.Height() - 1; p.y++) {
	 for (p.x = 1; p.x < ims.Width() - 1; p.x++) {
	    min = ims[p];
	    for (int v = 0; v < 8; v++){
	       if ((val = ims[p + v8[v]]) < min) {
		  min = val;	
	       }
	    }
	    imd[p] = min;
	 }
      }
   }
   return SUCCESS;
}

#ifdef MAIN
#define USAGE   "usage: %s connexity [-m mask] [im_in|-] [im_out|-]"
#define PARC    1
#define FINC    1
#define FOUTC   1
#define MASK    1

int main(int argc, char* argv[]) {
   Errc result;	 	        // The result code of the execution.
   Pobject *mask;	        // The mask.
   Pobject *objin[FINC + 1];    // The input objects;
   Pobject *objs[FINC + 1];     // The source objects masked by mask.
   Pobject *objout[FOUTC +1];   // The output object unmasked by mask.
   Pobject *objd[FOUTC + 1];    // The result object.
   char *parv[PARC + 1];        // The input parameters.
  
   ReadArgs(argc, argv, PARC, FINC, FOUTC, &mask, objin, objs, objout, objd, parv, USAGE, MASK);

   switch(objs[0]->Type()){
   case Po_Img2duc : {
      Img2duc* const ims = (Img2duc*)objs[0];
      objd[0] = new Img2duc(ims->Props());
      Img2duc* const imd = (Img2duc*)objd[0];
     
      result = Erosion(*ims, *imd, (int)atoi(parv[0]));
   } break;

   default :
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
