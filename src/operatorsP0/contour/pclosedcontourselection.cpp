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
 * @author Régis Clouard - 1999-02-19
 */

#include <stdio.h>
#include <pandore.h>
#include <set>
using namespace pandore;

#ifdef MAIN
#undef MAIN
#include "segmentation/pboundarylabeling.cpp"
#define MAIN
#else
#include "segmentation/pboundarylabeling.cpp"
#endif

/**
 * @file pclosedcontourselection.cpp
 *
 * Selects closed contour from length.
 * A closed contour is a chain of connected non null pixels
 * with 1 pixel thickness that forms a loop.
 */



bool isAcceptable( int length, int threshold, int relation, int extremaLength) {
   if (length <1) return false;
   if (relation == 0 && length==threshold) { return true; }
   if (relation == -1 && length < threshold) { return true; }
   if (relation == -2 && length <= threshold) { return true; }
   if (relation == -3 && length == extremaLength) { return true; }
   if (relation == 1  && length > threshold) { return true; }
   if (relation == 2  && length >= threshold) { return true; }
   if (relation == 3  && length == extremaLength) { return true; }
   return false;
}

Errc PClosedContourSelection( Img2duc &ims, Img2duc &imd, int relation, int length ) {
   Reg2d rgi(ims.Props());
   
   PBoundaryLabeling(ims,rgi);
   
   Img1dsl regions(rgi.Labels()+1);
   regions=0;
   
   Point2d p;
   std::set<int> setR;
   for (p.y = 0; p.y < rgi.Height(); p.y++)
   for (p.x = 0; p.x < rgi.Width(); p.x++)
   {
      if (ims[p]) {
	 for (int v=0; v<8; v++) {
	    if (ims.Hold(p+v8[v]) && ims[p+v8[v]]==0) {
	       setR.insert(rgi[p+v8[v]]);
	    }
	 }
	 std::set<int>::const_iterator sit(setR.begin()), send(setR.end());
	 for ( ;sit!=send;++sit) {
	    regions(*sit)++;
	 }
	 setR.clear();
      }
   }

   // Remove regions that touch the border
   for (p.x=0, p.y=0; p.y < ims.Height(); p.y++ ) {
      if ( ims[p]== 0) {
	 regions(rgi[p]) = 0;
      }
   }
   for (p.x=ims.Width()-1, p.y=0; p.y < ims.Height(); p.y++ ) {
      if ( ims[p]== 0) {
	 regions(rgi[p]) = 0;
      }
   }
   for (p.y=ims.Height()-1, p.x=ims.Width()-1, p.x=0; p.x < ims.Width(); p.x++ ) {
      if ( ims[p]== 0) {
	 regions(rgi[p]) = 0;
      }
   }
   for (p.y=ims.Height()-1, p.x=0; p.x < ims.Width(); p.x++ ) {
      if ( ims[p]== 0) {
	 regions(rgi[p]) = 0;
      }
   }

   imd=0;
   int count = 0;
   Long extremaLength = 0;
   if ( relation == 3) {
      extremaLength = 0;
      for (int r=1; r < regions.Width(); r++) {
	 if (regions(r)>extremaLength) {
	    extremaLength=regions(r);
	 }
      }
   } else if (relation == -3) {
      extremaLength = MAXLONG;
      for (int r=1; r < regions.Width(); r++) {
	 if (regions(r)>0 && regions(r)<extremaLength) {
	    extremaLength=regions(r);
	 }
      }
   }

   for (p.y = 0; p.y < ims.Height(); p.y++)
   for (p.x = 0; p.x < ims.Width(); p.x++)
   {
      if (ims[p]==0) {
  	 continue;
      }
      
      for (int v=0; v<8; v++ ) {
	 Point2d p1 =  p+v8[v];
	 if (ims.Hold(p1) && (isAcceptable(regions[rgi[p1]], length, relation, extremaLength))) {
	    imd[p] = 255;
	    count++;
	 }
      } 
   }
   return count;
}
   


#ifdef MAIN
#define USAGE "usage: %s relation length [-m mask] [im_in|-] [im_out|-]"
#define PARC  2
#define FINC  1
#define FOUTC 1
#define MASK  1

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
      Img2duc* const ims=(Img2duc*)objs[0];
      objd[0]=new Img2duc(ims->Props());
      Img2duc* const imd=(Img2duc*)objd[0];
      result=PClosedContourSelection(*ims,*imd,atoi(parv[0]),atoi(parv[1]));
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
