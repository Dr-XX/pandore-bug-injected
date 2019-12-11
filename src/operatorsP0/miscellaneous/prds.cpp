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
 * @author Régis Clouard (Inspired from W.A. Sterer) - 1999-11-19
 */

#include <pandore.h>
using namespace pandore;

/**
 * @file prds.cpp
 * Random Dot Stereogram.
 */

// Resolution.
#define XDPI 75
#define INCH 2.54F
// Separation des yeux.
#define EYESEP (6.0F * XDPI / INCH)
// Distance de l'observateur.
#define	OBSDISTANCE (55.0F * XDPI / INCH)

/**
 * Use a random tile for the stereogram.
 */
Errc PRds( const Img2duc &ims, Img2duc &imd ) {
   register int i, j;
   int *lookL, *lookR, *color;
   int sep;
   int right, left;
   bool reset;
   float featurez;

   srand(0);
   lookL = new int[ims.Width()];
   lookR = new int[ims.Width()];
   color = new int[ims.Width()];

   for (i = 0; i < ims.Height(); i++) {  
      // Init linking arrays.
      for (j = 0; j < imd.Width(); j++) {
	 lookR[j] = -1;
	 lookL[j] = -1;
      }

      for (j = 0; j < ims.Width(); j++) {
	 featurez = OBSDISTANCE - (float)ims[i][j];
	 sep = (int)((EYESEP * featurez) / (OBSDISTANCE + featurez));      
	 left = j - (sep / 2);
	 right = left + sep;
	 reset = true;
	 if ((left >= 0) && (right < ims.Width())) {
	    if (lookL[right] !=- 1) {
	       if (lookL[right] < left) {
		  lookR[lookL[right]] = -1;				 
		  lookL[right] = -1;
	       } else {
		  reset = false;
	       }

	    }
	    if (lookR[left] != -1) {
	       if (lookR[left] > right) {
		  lookL[lookR[left]] = -1;
		  lookR[left] = -1; 
	       }  else {
		  reset = false;
	       }
	    }
	    if (reset) {
	       lookL[right] = left;
	       lookR[left] = right;
	    }
	 }
      }
    
      // Labeling.
      for (j = 0; j < ims.Width(); j++) {
	 if (lookL[j] == -1) {
	    color[j] = rand() % 256;
	 } else {
	    color[j] = color[lookL[j]];
	 }
      }
    
      for (j = 0; j < ims.Width(); j++) {
	 imd[i][j] = color[j];
      }
   }
  
   delete [] lookL;
   delete [] lookR;
   delete [] color;

   return SUCCESS;
}

/**
 * Use a random tile for the stereogram.
 */
Errc PRds( const Img2dsl &ims, Img2duc &imd ) {
   register int i, j;
   int *lookL, *lookR, *color;
   int sep;
   int right, left;
   bool reset;
   float featurez;

   srand(0);
   lookL = new int[ims.Width()];
   lookR = new int[ims.Width()];
   color = new int[ims.Width()];

   for (i = 0; i < ims.Height(); i++) {  
      // Init linking arrays.
      for (j = 0; j < imd.Width(); j++) {
	 lookR[j] = -1;
	 lookL[j] = -1;
      }

      for (j = 0; j < ims.Width(); j++) {
	 featurez = OBSDISTANCE - (float)ims[i][j];
	 sep = (int)((EYESEP * featurez) / (OBSDISTANCE + featurez));      
	 left = j - (sep / 2);
	 right=  left + sep;
	 reset  =true;
	 if ((left >= 0) && (right < ims.Width())) {
	    if (lookL[right] != -1) {
	       if (lookL[right] < left) {
		  lookR[lookL[right]] = -1;				 
		  lookL[right] =-1;	    
	       }else {
		  reset = false;
	       }

	    }
	    if (lookR[left] != -1) {
	       if (lookR[left] > right) {
		  lookL[lookR[left]] = -1;
		  lookR[left] = -1; 
	       } else {
		  reset = false;
	       }
	    }
	    if (reset) {
	       lookL[right] = left;
	       lookR[left] = right;
	    }
	 }
      }
    
      // Labeling.
      for (j = 0; j < ims.Width(); j++) {
	 if (lookL[j] == -1) {
	    color[j] = rand() % 256;
	 } else {
	    color[j] = color[lookL[j]];
	 }
      }
    
      for (j = 0; j < ims.Width(); j++) {
	 imd[i][j] = color[j];
      }
   }
  
   delete [] lookL;
   delete [] lookR;
   delete [] color;

   return SUCCESS;
}

#ifdef MAIN

/*
 * Modify only the following constants, and the function call.
 */
#define	USAGE	"usage: %s [in_im|-][out_im|-]"
#define	PARC	0
#define	FINC	1
#define	FOUTC	1
#define	MASK	0

int main( int argc, char *argv[] ) {
   Errc	 result;	      // The result code of the execution.
   Pobject* mask;	      // The region mask.
   Pobject* objin[FINC+1];    // The input objects.
   Pobject* objs[FINC+1];     // The source objects masked by the mask.
   Pobject* objout[FOUTC+1];  // The output object.
   Pobject* objd[FOUTC+1];    // The result object of the execution.
   char* parv[PARC+1];	      // The input parameters.

   ReadArgs(argc, argv, PARC, FINC, FOUTC, &mask, objin, objs, objout, objd, parv, USAGE, MASK);

   switch(objs[0]->Type()) {
   case Po_Img2duc: {
      Img2duc* const ims = (Img2duc*)objs[0];
      objd[0] = new Img2duc(ims->Size());
      Img2duc* const imd = (Img2duc*)objd[0];
     
      result=PRds(*ims, *imd);
   }break;
   case Po_Img2dsl: {
      Img2dsl* const ims = (Img2dsl*)objs[0];
      objd[0] = new Img2duc(ims->Size());
      Img2duc* const imd = (Img2duc*)objd[0];
     
      result = PRds(*ims, *imd);
   }break;
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
