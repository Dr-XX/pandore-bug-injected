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
 * @author Régis Clouard - 1996-05-21
 * @author Régis Clouard - 2003-04-28
 * @author Régis Clouard - 2007-07-16 (new version with map)
 */

#include <pandore.h>
using namespace pandore;

//#define DEBUG

/**
 * @file pgradientthreshold.cpp
 * Estimates the noise level in a magnitude image
 * from the percentage of true contour points in the input image.
 * @param ims the input image.
 * @param percent the percentage value between [0..1].
 */
template <typename T>
Errc PGradientThreshold( const Imx3d<T> &ims, float percent ) {
   if (percent>1.0F || percent<0.0F) {
      std::cerr << "Error pgradientthreshold: Bad parameter value: "<< percent << std::endl;
      return FAILURE;
   }
   
   // Calculate the histogram.
   T *ps=ims.Vector(0); // Only for gray images.
   const T *pf=ims.Vector(0)+ims.VectorSize();
   std::map<T,Ulong> histo;
   for (; ps < pf; ps++) {
      histo[*ps]++;
   }
   
#ifdef DEBUG
   typename std::map<T,Ulong>::const_iterator h1;
   puts("*** Histogram");
   for (h1 = histo.begin(); h1 != histo.end(); h1++) {
      printf("histo [%f] = %f\n",(float)h1->first,(float)histo[h1->first]);
   }
#endif

   // Remove the 0 value.
   histo[0]=0;

   // Calculate the cumulated histogram.
   typename std::map<T,Ulong>::const_iterator h;
   h=histo.begin();
   Ulong val=0;
   for (h++; h != histo.end(); h++) {
      histo[h->first]+=val;
      val=h->second;
   }

#ifdef DEBUG
   puts("*** Cumulated Histogram");
   for (h1 = histo.begin(); h1 != histo.end(); h1++) {
      printf("histo [%f] = %f\n",(float)h1->first,(float)histo[h1->first]);
   }
#endif

   float percentvalue=val*(1.0F-percent);
#ifdef DEBUG
   printf("total = %d, percentvalue = %f\n",val,percentvalue);
#endif   

   // Find the 1-percent threshold.
   h=histo.begin();
   for (h++; h != histo.end(); h++) {
      if ((float)histo[h->first]>=percentvalue) {
	 break;
      }
   }

   return h->first;
}

/*
 * Modify only the following constants, and the function call.
 */
#ifdef MAIN
#define	USAGE	"usage: %s percent [-m mask] [im_in|-]"
#define	PARC	1
#define	FINC	1
#define	FOUTC	0
#define	MASK	1

int main( int argc, char *argv[] ) {
   Errc result;              // The result code of the execution.
   Pobject* mask;            // The region map.
   Pobject* objin[FINC+1];   // The input objects.
   Pobject* objs[FINC+1];    // The source objects masked.
   Pobject* objout[FOUTC+1]; // The output object.
   Pobject* objd[FOUTC+1];   // The result object of the execution.
   char* parv[PARC+1];       // The input parameters.

   ReadArgs(argc,argv,PARC,FINC,FOUTC,&mask,objin,objs,objout,objd,parv,USAGE,MASK);
   
   switch (objs[0]->Type()) {
   case Po_Img2duc: {
      Img2duc* const ims=(Img2duc*)objs[0];
      
      result=PGradientThreshold(*ims,(Float)atof(parv[0]));
   } break;
   case Po_Img2dsl: {
      Img2dsl* const ims=(Img2dsl*)objs[0];
      
      result=PGradientThreshold(*ims,(Float)atof(parv[0]));
   } break;
   case Po_Img2dsf: {
      Img2dsf* const ims=(Img2dsf*)objs[0];
      
      result=PGradientThreshold(*ims,(Float)atof(parv[0]));
   } break;
   case Po_Img3duc: {
      Img3duc* const ims=(Img3duc*)objs[0];
      
      result=PGradientThreshold(*ims,(Float)atof(parv[0]));
   } break;
   case Po_Img3dsl: {
      Img3dsl* const ims=(Img3dsl*)objs[0];
      
      result=PGradientThreshold(*ims,(Float)atof(parv[0]));
   } break;
   case Po_Img3dsf: {
      Img3dsf* const ims=(Img3dsf*)objs[0];
      
      result=PGradientThreshold(*ims,(Float)atof(parv[0]));
   } break;
   default:
      PrintErrorFormat(objin,FINC);
      result=FAILURE;
   }
   
   if (result) {
      WriteArgs(argc,argv,PARC,FINC,FOUTC,&mask,objin,objs,objout,objd,MASK);
   }
   
   Exit(result);
   return 0;
}

#endif
