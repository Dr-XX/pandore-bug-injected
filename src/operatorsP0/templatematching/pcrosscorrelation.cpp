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

/*
 * @author Régis Clouard - May 29, 2013
 */
#include<cmath>
#include <pandore.h>
using namespace pandore;

Errc PCrossCorrelation( const Img2duc &ims1, const Img2duc &ims2, Img2dsf &imd ) {
   if ( ims1.Width() < ims2.Width() || ims1.Height() < ims2.Height() ) {
      std::cerr << "Error pcrosscorrelation: Image size lower than pattern image size." << std::endl;
      return FAILURE;
   }

   // 1. Compute the mean of the template
   float meanTemplate = 0;
   for (int v=0; v<ims2.Height(); v++) {
      for (int u=0; u<ims2.Width(); u++) {
	 meanTemplate += (float)ims2(v,u);
      }
   }
   meanTemplate /= (float)ims2.VectorSize();

   
   // 2. Compute the integral image
   Img2dsf integral(ims1.Size());
   float sum = 0;
   for (int x = 0; x < ims1.Width(); x++) {
      sum += (float)ims1(0, x);
      integral(0, x) = sum;
   }
   for (int y = 1; y < ims1.Height(); y++) {
      float sum = 0;
      for (int x = 0; x < ims1.Width(); x++) {
	 sum += (float)ims1(y, x);
	 integral(y, x) = integral(y - 1, x) + sum;
      }
   }

   // 3. Cross-Correlation
   const int yoffset = ims2.Height() / 2;
   const int yend = ims2.Height() - yoffset;
   const int xoffset = ims2.Width() / 2;
   const int xend = ims2.Width() - xoffset;

   imd.Frame(0, yoffset, xoffset);
   for (int y = yoffset + 1; y < ims1.Height() - yend; y++) {
      for (int x = xoffset + 1; x < ims1.Width() - xend; x++) {
	 const int y1 = y - yoffset;
	 const int y2 = y + yend - 1;
	 
	 const int x1 = x - xoffset;
	 const int x2 = x + xend - 1;

	 float meanImage = (integral(y2, x2) - integral(y1 - 1, x2) - integral(y2, x1 - 1) + integral(y1 - 1, x1 - 1)) / (float)ims2.VectorSize();
	 float a = 0;
 	 float aa = 0;
 	 float bb = 0;
	 for (int v = -yoffset; v < yend; v++) {
	    for (int u = -xoffset; u < xend; u++) {
	       a  += (ims1(y + v, x + u) - meanImage) * (ims2(v + yoffset, u + xoffset) - meanTemplate);
     	       aa += (ims1(y + v,x + u) - meanImage) * (ims1(y + v, x + u) - meanImage);
    	       bb += (ims2(v + yoffset, u + xoffset) - meanTemplate) * (ims2(v + yoffset, u + xoffset) - meanTemplate);
	    }
	 }
 	 imd(y, x) = (float)fabs(a / sqrt(aa * bb));
      }
   }

   return SUCCESS;
}

#ifdef MAIN
	
/*
 * Modify only the following constants, and the function call.
 */
#define	USAGE	"usage: %s  [-m mask] [im_in|-] [pattern_in|-] [im_out|-]"
#define	PARC	0
#define	FINC	2
#define	FOUTC	1
#define	MASK	0

int main( int argc, char *argv[] ) {
   Errc result;              // The result code of the execution.
   Pobject* mask;            // The region map.
   Pobject* objin[FINC+1];   // The input objects.
   Pobject* objs[FINC+1];    // The source objects masked.
   Pobject* objout[FOUTC+1]; // The output object.
   Pobject* objd[FOUTC+1];   // The result object of the execution.
   char* parv[PARC+1];       // The input parameters.

   ReadArgs(argc, argv, PARC, FINC, FOUTC, &mask, objin, objs, objout, objd, parv, USAGE, MASK);
   if (objs[0]->Type() == Po_Img2duc && objs[1]->Type() == Po_Img2duc) {
      Img2duc* const ims1 = (Img2duc*)objs[0];
      Img2duc* const ims2 = (Img2duc*)objs[1];
      objd[0] = new Img2dsf(ims1->Props());
      Img2dsf* const imd = (Img2dsf*)objd[0];
      
      result = PCrossCorrelation(*ims1, *ims2, *imd);
      goto end;
   }
end:
   if (result) {
	WriteArgs(argc, argv, PARC, FINC, FOUTC, &mask, objin, objs, objout, objd, MASK);
   }
  Exit(result);
  return 0;
}
#endif
