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
 * @author Sébastien Bernery - 2013-04-16
 */

#include <cmath>
#include <algorithm>
#include <pandore.h>
using namespace pandore;

/**
 * @file padaptivemeanbinarization.cpp
 * 
 * Binarization using the local mean.
 *
 * Reference: B. Bradleya and G. Rothb, "Adaptive Thersholding
 *            using the Integral Image", Journal of Graphics, GPU, 
 *            and Game Tools, 12(2), pp. 13-21, 2007.
 */

Errc PAdaptiveMeanBinarization( const Img2duc &ims, Img2duc &imd, const int radius_x, const int radius_y, const int percent ) {
   // Compute the integral image
   Img2dsf integral(ims.Size());

   float sum = 0;
   for (int x = 0; x < ims.Width(); x++) {
      sum += (float)ims(0, x);
      integral(0, x) = sum;
   }
   for (int y = 1; y < ims.Height(); y++) {
      float sum = 0;
      for (int x = 0; x < ims.Width(); x++) {
	 sum += (float)ims(y, x);
	 integral(y, x) = integral(y - 1, x) + sum;
      }
   }

   // Binarization
   for (int y = 0; y < ims.Height(); y++) {
      for (int x = 0; x < ims.Width(); x++) {
	 const int y1 = (y - radius_y <= 1) ? 1 : y - radius_y;
	 const int y2 = (y + radius_y >= ims.Height()) ? ims.Height() - 1 : y + radius_y;
	 
	 const int x1 = (x - radius_x <= 1) ? 1 : x - radius_x;
	 const int x2 = (x + radius_x >= ims.Width()) ? ims.Width() - 1 : x + radius_x;
	 const float count = (float)((x2 - x1 + 1) * (y2 - y1 + 1));
	 const float sum = integral(y2, x2)
	    - integral(y1 - 1, x2)
	    - integral(y2, x1 - 1)
	    + integral(y1 - 1, x1 - 1);
	 if ((ims(y, x) * count) <= (sum * (100.0f - percent) / 100.f)) {
	    imd(y, x) = 0;
	 } else {
	    imd(y, x) = 255;
	 }
      }
   }
   return SUCCESS;
}

Errc PAdaptiveMeanBinarization( const Img3duc &ims, Img3duc &imd, const int radius_x, const int radius_y, const int radius_z, const int percent ) {
   // Compute the integral image
   Img3dsf integral(ims.Size());

   for (int z = 0; z < ims.Depth(); z++) {
      for (int y = 0; y < ims.Height(); y++) {
	 float sum = 0;
	 for (int x = 0; x < ims.Width(); x++) {
	    sum += (float)ims(z, y, x);
	    if (z == 0) {
	       if (y == 0) {
		  integral(z, y, x) = sum;
	       } else {
		  integral(z, y, x) = integral(z, y - 1, x)  + sum;
	       }
	    } else {
	       if (y == 0) {
		  integral(z, y, x) = integral(z - 1, y, x) + sum;
	       } else {	
		  integral(z, y, x) = integral(z - 1, y, x) 
		     + integral(z, y - 1, x)
		     - integral(z-1, y - 1, x)
		     + sum;
	       }
	    }
	 }
      }
   }

   // Binarization
   for (int z = 0; z < ims.Depth(); z++) {
      for (int y = 0; y < ims.Height(); y++) {
	 for (int x = 0; x < ims.Width(); x++) {
	    const int z1 = (z - radius_z <= 1) ? 1 : z - radius_z;
	    const int z2 = (z + radius_z >= ims.Depth()) ? ims.Depth() - 1 : z + radius_z;
	    const int y1 = (y - radius_y <= 1) ? 1 : y - radius_y;
	    const int y2 = (y + radius_y >= ims.Height()) ? ims.Height() - 1 : y + radius_y;
	    
	    const int x1 = (x - radius_x <= 1) ? 1 : x - radius_x;
	    const int x2 = (x + radius_x >= ims.Width()) ? ims.Width() - 1 : x + radius_x;
	    const float count = (float)((x2 - x1 + 1) * (y2 - y1 + 1) * (z2 - z1 + 1));

	    const float sum1 = integral(z2, y2, x2)
	       - integral(z2, y2, x1 - 1) - integral(z1 - 1, y2, x2)
	       + integral(z1 - 1, y2, x1 - 1);
	    const float sum2 = integral(z2, y1 - 1, x2)
	       - integral(z2, y1 - 1, x1 - 1)
	       - integral(z1 - 1, y1 - 1, x2)
	       + integral(z1 - 1, y1 - 1, x1 - 1);

	    const float sum = sum1 - sum2;
	    if ((ims(z, y, x) * count) <= (sum * (100 - percent) / 100.f)) {
	       imd(z, y, x) = 0;
	    } else {
	       imd(z, y, x) = 255;
	    }
	 }
      }
   }
   return SUCCESS;
}

#ifdef MAIN

 /*
  * Modify only the following constants, and the operator switches.
  */
#define	USAGE	"usage: %s radius_x radius_y radius_z percent [-m mask] [im_in|-] [im_out|-]"
#define	PARC	4  // Number of parameters
#define	FINC	1  // Number of input images
#define	FOUTC	1  // Number of output images
#define	MASK	0  // Level of masking

int main( int argc, char *argv[] ) {
   Errc result;			// The result code of the execution.
   Pobject* mask;		// The mask.
   Pobject* objin[FINC + 1];	// The input objects.
   Pobject* objs[FINC + 1];	// The source objects masked by the mask.
   Pobject* objout[FOUTC + 1];	// The output objects.
   Pobject* objd[FOUTC + 1];	// The result objects of the execution.
   char* parv[PARC + 1];        // The input parameters.
   
   ReadArgs(argc, argv, PARC, FINC, FOUTC, &mask, objin, objs, objout, objd, parv, USAGE, MASK);
   
   switch (objs[0]->Type()) {
   case Po_Img2duc: {
      Img2duc* const ims = (Img2duc*)objs[0];
      objd[0] = new Img2duc(ims->Props());
      Img2duc* const imd = (Img2duc*)objd[0];
      
      result = PAdaptiveMeanBinarization(*ims, *imd, atoi(parv[0]), atoi(parv[1]), atoi(parv[3]));
      break;
   }
   case Po_Img3duc: {
      Img3duc* const ims = (Img3duc*)objs[0];
      objd[0] = new Img3duc(ims->Props());
      Img3duc* const imd = (Img3duc*)objd[0];
      
      result = PAdaptiveMeanBinarization(*ims, *imd, atoi(parv[0]), atoi(parv[1]), atoi(parv[2]), atoi(parv[3]));
      break;
   }
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
