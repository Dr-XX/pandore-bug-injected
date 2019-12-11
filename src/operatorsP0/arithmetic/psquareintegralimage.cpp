/* -*- c-basic-offset: 3;mode:c++ -*-
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

template <typename T1>
Errc PSquareIntegralImage( const Img2d<T1> &ims, Collection &cold ) {
   const int width = ims.Width();
   Double *accumulator = new Double[ims.Width() * ims.Height()];

   accumulator[0] = (Double)ims(0,0)*ims(0,0);

   Point2d p;
   p.y = 0;
   for (p.x = 1; p.x < ims.Width(); p.x++) {
      accumulator[p.x] = (Double)ims[p] * ims[p] + accumulator[p.x - 1];
   }
   for (p.y = 1; p.y < ims.Height(); p.y++) {
      Double s = 0;
      for (p.x = 0; p.x < ims.Width(); p.x++) {
	 s += (Double)ims[p] * ims[p];
	 accumulator[p.y  *width + p.x] = accumulator[(p.y - 1) * width + p.x] = s;
      }
   }

   cold.SETARRAY("internal_array", Double, accumulator, ims.Width() * ims.Height());
   
   return SUCCESS;
}


template <typename T1>
Errc PSquareIntegralImage( const Img3d<T1> &ims, Collection &cold ) {
   const int width = ims.Width();
   const int height = ims.Height();
   Double *accumulator = new Double[ims.Width() * ims.Height()];

   accumulator[0] = (Double)ims(0,0,0) * ims(0,0,0);
   Point3d p;
   Img3dsf s1(ims.Props());
   Img3dsf s2(ims.Props());
   
   for (p.z = 0; p.z < ims.Depth(); p.z++) {
      for (p.y = 0; p.y < ims.Height(); p.y++) {
	 for (p.x = 0; p.x<ims.Width(); p.x++) {
	    if (p.y == 0) {
	       s1[p] = (Float)ims[p] * ims[p];
	    } else {
	       s1[p] = s1(p.z, p.y - 1, p.x) + ims[p] * ims[p];
	    }
	    if (p.z == 0) {
	       s2[p] = s1[p];
	    } else {
	       s2[p] = s2(p.z - 1, p.y, p.x) + s1[p];
	    }
	    if (p.x == 0) {
	       accumulator[p.z * (width * height) + p.y * width + p.x] = s2[p];
	    } else {
	       accumulator[p.z * (width * height) + p.y * width + p.x] = accumulator[p.z * (height * width) + p.y * width + p.x - 1] + s2[p];
	    }
	 }
      }
   }
   
   cold.SETARRAY("internal_array", Double, accumulator, ims.Depth()*ims.Width()*ims.Height());

   return SUCCESS;
}

#ifdef MAIN
#define USAGE	"usage: %s [im_in|-] [col_out|-]"
#define PARC	0
#define FINC	1
#define FOUTC	1
#define MASK	0

int main( int argc, char *argv[] ) {
   Errc result;              // The result code of the execution.
   Pobject* mask;            // The region map.
   Pobject* objin[FINC+1];   // The input objects.
   Pobject* objs[FINC+1];    // The source objects masked.
   Pobject* objout[FOUTC+1]; // The output object.
   Pobject* objd[FOUTC+1];   // The result object of the execution.
   char* parv[PARC+1];       // The input parameters.
   
   ReadArgs(argc,argv,PARC,FINC,FOUTC,&mask,objin,objs,objout,objd,parv,USAGE,MASK);
   if (objs[0]->Type() == Po_Img2duc) {
      Img2duc* const ims = (Img2duc*)objs[0];
      objd[0] = new Collection;
      Collection* const imd = (Collection*)objd[0];
      
      result = PSquareIntegralImage(*ims, *imd);
      goto end;
   }
   if (objs[0]->Type() == Po_Img2dsl) {
      Img2dsl* const ims = (Img2dsl*)objs[0];
      objd[0] = new Collection;
      Collection* const imd = (Collection*)objd[0];
      
      result = PSquareIntegralImage(*ims, *imd);
      goto end;
   }   
   if (objs[0]->Type() == Po_Img2dsf) {
      Img2dsf* const ims = (Img2dsf*)objs[0];
      objd[0] = new Collection;
      Collection* const imd = (Collection*)objd[0];
      
      result = PSquareIntegralImage(*ims, *imd);
      goto end;
   }

   if (objs[0]->Type() == Po_Img3duc) {
      Img3duc* const ims = (Img3duc*)objs[0];
      objd[0] = new Collection;
      Collection* const imd = (Collection*)objd[0];
      
      result = PSquareIntegralImage(*ims, *imd);
      goto end;
   }
   if (objs[0]->Type() == Po_Img3dsl) {
      Img3dsl* const ims = (Img3dsl*)objs[0];
      objd[0] = new Collection;
      Collection* const imd = (Collection*)objd[0];
      
      result = PSquareIntegralImage(*ims, *imd);
      goto end;
   }   
   if (objs[0]->Type() == Po_Img3dsf) {
      Img3dsf* const ims = (Img3dsf*)objs[0];
      objd[0] = new Collection;
      Collection* const imd = (Collection*)objd[0];
      
      result = PSquareIntegralImage(*ims, *imd);
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
