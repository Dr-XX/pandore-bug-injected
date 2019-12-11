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

#include <map>
#include <pandore.h>
using namespace pandore;

/**
 * @file prankthresholding.cpp
 * Thresholding.
 * If low <= high -> [low..high]
 * If low > high  -> [min..low[ U ]high..max]
 */
template <typename T>
Errc PRankThresholding( const Imx3d<T> &ims, Imx3d<T> &imd, int low, int high ) {
   int nlow = low;
   int nhigh = high;
   
   const Long maxrank = ims.VectorSize();

   if (low < 0) {
      nlow = 0;
   } else if (low > maxrank) {
      nlow = maxrank;
   }
   if (high > maxrank) {
      nhigh = maxrank;
   } else if (high < 0) {
      nhigh = 0;
   }

   typedef std::map<T, int> Rank;
   Rank rank;

   T *ps = ims.Vector();
   const T *pe = ps + ims.VectorSize();
   for ( ; ps < pe; ) {
      rank[*ps++] = 1;
   }

   typename Rank::iterator iter;
   int i;
   for (i = 0, iter = rank.begin(); iter != rank.end(); ++iter, ++i) {
      iter->second = i;
   }
   
   ps = ims.Vector();
   pe = ps + ims.VectorSize();
   T *pd = imd.Vector();

   if (high < low) {
      for ( ; ps < pe; ps++, pd++) {
	 if ((rank[*ps] >= low) || (rank[*ps] < high)) {
	    *pd = *ps;
	 }  else {
	    *pd = 0;
	 }
      }
   } else {
      for ( ; ps< pe; ps++, pd++) { 
	 if ((rank[*ps] >= nlow) && (rank[*ps] <= nhigh)) {
	    *pd = *ps;
	 }  else {
	    *pd = 0;
	 }
      }
   }
   return SUCCESS;
}



#ifdef MAIN
#define	USAGE	"usage: %s low high [-m mask] [im_in|-][im_out|-]"
#define	PARC	2
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
   if (objs[0]->Type() == Po_Img1duc) {
      Img1duc* const ims = (Img1duc*)objs[0];
      objd[0] = new Img1duc(ims->Props());
      
      Img1duc* const imd = (Img1duc*)objd[0];
      
      result = PRankThresholding(*ims, *imd, atoi(parv[0]), atoi(parv[1]));
      goto end;
   }
   if (objs[0]->Type() == Po_Img1dsl) {
      Img1dsl* const ims = (Img1dsl*)objs[0];
      objd[0] = new Img1dsl(ims->Props());
      
      Img1dsl* const imd = (Img1dsl*)objd[0];
      
      result = PRankThresholding(*ims, *imd, atoi(parv[0]), atoi(parv[1]));
      goto end;
   }
   if (objs[0]->Type() == Po_Img1dsf) {
      Img1dsf* const ims = (Img1dsf*)objs[0];
      objd[0] = new Img1dsf(ims->Props());
      
      Img1dsf* const imd = (Img1dsf*)objd[0];
      
      result = PRankThresholding(*ims, *imd, atoi(parv[0]), atoi(parv[1]));
      goto end;
   }
   if (objs[0]->Type() == Po_Img2duc) {
      Img2duc* const ims = (Img2duc*)objs[0];
      objd[0] = new Img2duc(ims->Props());
      
      Img2duc* const imd = (Img2duc*)objd[0];
      
      result = PRankThresholding(*ims, *imd, atoi(parv[0]), atoi(parv[1]));
      goto end;
   }
   if (objs[0]->Type() == Po_Img2dsl) {
      Img2dsl* const ims = (Img2dsl*)objs[0];
      objd[0] = new Img2dsl(ims->Props());
      
      Img2dsl* const imd = (Img2dsl*)objd[0];
      
      result = PRankThresholding(*ims, *imd, atoi(parv[0]), atoi(parv[1]));
      goto end;
   }
   if (objs[0]->Type() == Po_Img2dsf) {
      Img2dsf* const ims = (Img2dsf*)objs[0];
      objd[0] = new Img2dsf(ims->Props());
      
      Img2dsf* const imd = (Img2dsf*)objd[0];
      
      result = PRankThresholding(*ims, *imd, atoi(parv[0]), atoi(parv[1]));
      goto end;
   }
   if (objs[0]->Type() == Po_Img3duc) {
      Img3duc* const ims = (Img3duc*)objs[0];
      objd[0] = new Img3duc(ims->Props());
      
      Img3duc* const imd = (Img3duc*)objd[0];
      
      result = PRankThresholding(*ims, *imd, atoi(parv[0]), atoi(parv[1]));
      goto end;
   }
   if (objs[0]->Type() == Po_Img3dsl) {
      Img3dsl* const ims = (Img3dsl*)objs[0];
      objd[0] = new Img3dsl(ims->Props());
      
      Img3dsl* const imd = (Img3dsl*)objd[0];
      
      result = PRankThresholding(*ims, *imd, atoi(parv[0]), atoi(parv[1]));
      goto end;
   }
   if (objs[0]->Type() == Po_Img3dsf) {
      Img3dsf* const ims = (Img3dsf*)objs[0];
      objd[0] = new Img3dsf(ims->Props());
      
      Img3dsf* const imd = (Img3dsf*)objd[0];
      
      result = PRankThresholding(*ims, *imd, atoi(parv[0]), atoi(parv[1]));
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
