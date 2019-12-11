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
 * @author Régis Clouard - 2001-06-01
 */

#include <pandore.h>
using namespace pandore;

/**
 * @file pbarbremoval.cpp
 * Removes barbs from their length.
 * A barb is a chain of connected non null pixels
 * with 1 pixel thickness that begins with a end point
 * (a point with only one neighbor) and that ends
 * at an intersection (a point with more than two neighbors)
 */


Errc PBarbRemoval( const Img2duc &ims, Img2duc &imd, int relation, int length ) {
   int v,w,lg,nvois;
   Point2d pt,p;
   Lifo<Point2d> contour;
   Ulong nb=0;

   // On recopie l'image initiale.
   imd=ims;

   for (p.y = 0; p.y < ims.Height(); p.y++)
   for (p.x = 0; p.x < ims.Width(); p.x++)
   {
      // Not an contour point.
      if (imd[p]==0)
	 continue;

      // Check if the point has exactly one neighbor.
      nvois=0; w=0;
      for (v=0; v<8; v++) {
	 if (ims.Hold(p+v8[v]) && ims[p+v8[v]]) {
	    nvois++;
	    w = v;
	 }
      }
      // Not an end point.
      if (nvois > 1)
	 continue;

      // Push the beginning of the chain.
      lg=1;
      pt=p;
      contour.Push(pt);
      
      // Follow the contour until > 2 neighbors.
      do {
	 pt = pt + v8[w];
	 if (!ims.Hold(pt)) break;
	 nvois=0;
	 int beg=w+(8/2)+1;
	 int end=w+(3*8)/2-1;
	 for (v=beg; v<end; v++) {
	    int vi=v%8;
	    if (ims.Hold(pt+v8[vi]) && ims[pt+v8[vi]]) {
	       nvois++;
	       w=vi;
	    }
	 }
	 if (nvois==1) {
	    contour.Push(pt);
	    lg ++;
	 }
      } while (nvois==1);
      
      // Not barb.
      if (nvois <= 1) {
	 while (!contour.Empty()) {
	    pt = contour.Pop();
	 }
	 continue;
      }
      
      // Suppresion de la chaine.
      if (relation > 1) {
	 if (lg>=length) {
	    nb++;
	    while(!contour.Empty()) {
	       pt = contour.Pop();
	       imd[pt] = 0;
	    }
	 }
      } else if (relation == 1) {
	 if (lg>length) {
	    nb++;
	    while(!contour.Empty()) {
	       pt = contour.Pop();
	       imd[pt] = 0;
	    }
	 }
      } else if (relation == 0) {
	 if (lg==length) {
	    nb++;
	    while(!contour.Empty()) {
	       pt = contour.Pop();
	       imd[pt] = 0;
	    }
	 } else { // On depile simplement.
	    while (!contour.Empty())
	       pt = contour.Pop();
	 }
      } else if (relation == -1) {
	 if (lg<length) {
	    nb++;
	    while(!contour.Empty()) {
	       pt = contour.Pop();
	       imd[pt] = 0;
	    }
	 }
      } else if (relation <-1) {
	 if (lg<=length) {
	    nb++;
	    while(!contour.Empty()) {
	       pt = contour.Pop();
	       imd[pt] = 0;
	    }
	 }
      }
      contour.Reset();
  }

  return nb;
}


Errc PBarbRemoval( const Img3duc &ims, Img3duc &imd, int relation, int length ) {
   int v,w,lg,nvois;
   Point3d pt,p;
   Lifo<Point3d> contour;
   Ulong nb=0;

   // On recopie l'image initiale.
   imd=ims;

   for (p.z = 0; p.z < ims.Depth(); p.z++)
   for (p.y = 0; p.y < ims.Height(); p.y++)
   for (p.x = 0; p.x < ims.Width(); p.x++)
   {
      // Not an contour point.
      if (imd[p]==0)
	 continue;

      // Check if the point has exactly one neighbor.
      nvois=0; w=0;
      for (v=0; v<26; v++) {
	 if (ims.Hold(p+v26[v]) && ims[p+v26[v]]) {
	    nvois++;
	    w = v;
	 }
      }
      // Not an end point.
      if (nvois > 1)
	 continue;

      // Push the beginning of the chain.
      lg=1;
      pt=p;
      contour.Push(pt);
      
      // Follow the contour until > 2 neighbors.
      do {
	 pt = pt + v26[w];
	 if (!ims.Hold(pt)) break;
	 nvois=0;
	 int beg=w+(26/2)+1;
	 int end=w+(3*26)/2-1;
	 for (v=beg; v<end; v++) {
	    int vi=v%26;
	    if (ims.Hold(pt+v26[vi]) && ims[pt+v26[vi]]) {
	       nvois++;
	       w=vi;
	    }
	 }
	 if (nvois==1) {
	    contour.Push(pt);
	    lg ++;
	 }
      } while (nvois==1);
      
      // Not barb.
      if (nvois <= 1) {
	 while (!contour.Empty()) {
	    pt = contour.Pop();
	 }
	 continue;
      }
      
      // Suppresion de la chaine.
      if (relation > 1) {
	 if (lg>=length) {
	    nb++;
	    while(!contour.Empty()) {
	       pt = contour.Pop();
	       imd[pt] = 0;
	    }
	 }
      } else if (relation == 1) {
	 if (lg>length) {
	    nb++;
	    while(!contour.Empty()) {
	       pt = contour.Pop();
	       imd[pt] = 0;
	    }
	 }
      } else if (relation == 0) {
	 if (lg==length) {
	    nb++;
	    while(!contour.Empty()) {
	       pt = contour.Pop();
	       imd[pt] = 0;
	    }
	 } else { // On depile simplement.
	    while (!contour.Empty())
	       pt = contour.Pop();
	 }
      } else if (relation == -1) {
	 if (lg<length) {
	    nb++;
	    while(!contour.Empty()) {
	       pt = contour.Pop();
	       imd[pt] = 0;
	    }
	 }
      } else if (relation <-1) {
	 if (lg<=length) {
	    nb++;
	    while(!contour.Empty()) {
	       pt = contour.Pop();
	       imd[pt] = 0;
	    }
	 }
      }
      contour.Reset();
  }

  return nb;
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
      objd[0]=new Img2duc(ims->Size());
      Img2duc* const imd=(Img2duc*)objd[0];
      
      result=PBarbRemoval(*ims,*imd,atoi(parv[0]),atoi(parv[1]));
      goto end;
   }
   if (objs[0]->Type() == Po_Img3duc) {
      Img3duc* const ims=(Img3duc*)objs[0];
      objd[0]=new Img3duc(ims->Size());
      Img3duc* const imd=(Img3duc*)objd[0];
      
      result=PBarbRemoval(*ims,*imd,atoi(parv[0]),atoi(parv[1]));
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
