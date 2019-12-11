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
 * @file popencontourselection.cpp
 * Selects open contour from length.
 * 
 * An open contour is a chain of connected non null pixels with 1 pixel thickness
 * that begins and ends with a end point (a point with only one neighbor)
 *  and which has no junctions. A closed contour and a barb are not
 * considered as open contours.
 */


/**
 * @param lg length of the contour (=-1 when the contour is not an open contour)/
 */
Lifo<Point2d> ContourTracking (Img2duc &ims, Img2duc &imi, Point2d pti, Long &lg) {
   Point2d ptc, p, pt;
   int ngb;
   int v;
   Long lg1;
   Lifo<Point2d> contour, contour1;
 
   // Count the number of neighbors.
   ngb=0;
   for (v=0; v<8; v++) {
      if (ims.Hold(pti+v8[v]) && ims[pti+v8[v]]) {
	 ngb++;
      }
   }

   imi[pti]=0;
   // A junction -> Not an open contour!.
   if (ngb>2) {
      lg=-1;
      imi[pti]=255;
      return contour;
   }
   contour.Push(pti);
   lg=1;
   for (v=0; v<8; v++) {
      if (imi.Hold(pti+v8[v]) && imi[pti+v8[v]]) {
	 contour1=ContourTracking(ims, imi, pti+v8[v], lg1);
	 if (lg1 == -1) { lg=-1; imi[pti]=255; return contour;}
	 lg+=lg1;
	 // Adds contour1 to contour.
	 while(!contour1.Empty()) {
	    contour.Push(contour1.Pop());
	 }
      }
   }
   imi[pti]=255;

   return contour;
}

Errc POpenContourSelection( Img2duc &ims, Img2duc &imd, int relation, int length ) {
#define MAXMAX 250

   Point2d pt, p;
   int nbmax=0;
   Lifo<Point2d> contourmax[MAXMAX];
   Lifo<Point2d> contour;
   int i;
   Long lg;
   int nb=0;
   
   Img2duc imi(ims);
   imd=0;
   imi=ims;
   
   if (relation <= -3 ) length=MAXLONG;
   else if (relation >= 3 ) length=0;
   
   for (p.y = 0; p.y < ims.Height(); p.y++)
   for (p.x = 0; p.x < ims.Width(); p.x++)
   {
      if (imi[p]==0)
	 continue;
      
      // Count the number of neighbors.
      int ngb=0, w=0;
      for (int v=0; v<8; v++) {
	 if (ims.Hold(p+v8[v]) && ims[p+v8[v]]) {
	    w=v;
	    ngb++;
	 }
      }
      // Not an end point  ->break;
      if  (ngb>1) continue;

      imi[p]=0;
      contour.Push(p);
      lg=0;
      // If there exists some neighbors.
      if (ngb == 1)
	 contour = ContourTracking(ims, imi, p+v8[w], lg);

      imi[p]=255;

      // Not an open contour.
      if (lg == -1) {
	 contour.Reset();
	 continue;
      }

      lg++;

      // Max length.
      if (relation >= 3) { 
	 if (lg>length) {
	    for (i=0 ; i< nbmax; i++) {
	       contourmax[i].Reset();
	    }
	    while(!contour.Empty()) {
	       pt = contour.Pop();
	       contourmax[0].Push(pt);
	    }
	    nbmax=1;
	    length=lg;
	 } else if (lg==length) {
	    if (nbmax < MAXMAX) {
	       // Catenate new contour.
	       while(!contour.Empty()) {
		  pt = contour.Pop();
		  contourmax[nbmax].Push(pt);
	       }
	       nbmax++;
	    }
	 } else {
	    contour.Reset();
	 }
	 
	 // >=
      } else if (relation == 2) {
	 if (lg>=length) {
	    while(!contour.Empty()) {
	       pt = contour.Pop();
	       imd[pt] = 255;
	    }
	    nb++;
	 }

	 // >
      } else if (relation == 1) {
	 if (lg>length) {
	    puts("OK");
	    while(!contour.Empty()) {
	       pt = contour.Pop();
 	       imd[pt] = 255;
	       imi[pt]=0;
	    }
	    nb++;
	 }

	 // ==
      } else if (relation == 0) {
	 if (lg==length) {
	    while(!contour.Empty()) {
	       pt = contour.Pop();
	       imd[pt] = 255;
	    }
	    nb++;
	 } else {
	    contour.Reset();
	 }

	 // <
      } else if (relation == -1) {
	 if (lg<length) {
	    while(!contour.Empty()) {
	       pt = contour.Pop();
	       imd[pt] = 255;
	    }
	    nb++;
	 } else {
	       contour.Reset();
	 }

	 //  <=
      } else if (relation == -2) {
	 if (lg<=length) {
	    while(!contour.Empty()) {
	       pt = contour.Pop();
	       imd[pt] = 255;
	    }
	    nb++;
	 } else {
	    contour.Reset();
	 }

	 // Min length.
      } else if (relation <= -3) {
	 if (lg<length) {
	    for (i=0 ; i< nbmax; i++) {
	       contourmax[i].Reset();
	    }
	    while(!contour.Empty()) {
	       pt = contour.Pop();
	       contourmax[0].Push(pt);
	    }
	    nbmax=1;
	    length=lg;
	 } else if (lg==length) {
	    while(!contour.Empty()) {
	       pt = contour.Pop();
	       contourmax[nbmax].Push(pt);
	    }
	    nbmax++;
	 } else {
	    contour.Reset();
	 }
      }
      contour.Reset();
   }
   if (relation > 2 || relation < -2 ) {
      for (i=0; i<nbmax; i++) {
	 while(!contourmax[i].Empty()) {
	    pt = contourmax[i].Pop();
	    imd[pt] = 255;
	 }
	 nb++;
      }
   }
   
   return nb;
}


/**
 * @param lg length of the contour (=-1 when the contour is not an open contour)/
 */
Lifo<Point3d> ContourTracking (Img3duc &ims, Img3duc &imi, Point3d pti, Long &lg) {
   Point3d ptc, p, pt;
   int ngb;
   int v;
   Long lg1;
   Lifo<Point3d> contour, contour1;
 
   // Count the number of neighbors.
   ngb=0;
   for (v=0; v<26; v++) {
      if (ims.Hold(pti+v26[v]) && ims[pti+v26[v]]) {
	 ngb++;
      }
   }

   imi[pti]=0;
   // A junction -> Not an open contour!.
   if (ngb>2) {
      lg=-1;
      imi[pti]=255;
      return contour;
   }
   contour.Push(pti);
   lg=1;
   for (v=0; v<26; v++) {
      if (imi.Hold(pti+v26[v]) && imi[pti+v26[v]]) {
	 contour1=ContourTracking(ims, imi, pti+v26[v], lg1);
	 if (lg1 == -1) { lg=-1; imi[pti]=255; return contour;}
	 lg+=lg1;
	 // Adds contour1 to contour.
	 while(!contour1.Empty()) {
	    contour.Push(contour1.Pop());
	 }
      }
   }
   imi[pti]=255;

   return contour;
}

Errc POpenContourSelection( Img3duc &ims, Img3duc &imd, int relation, int length ) {
#define MAXMAX 250

   Point3d pt, p;
   int nbmax=0;
   Lifo<Point3d> contourmax[MAXMAX];
   Lifo<Point3d> contour;
   int i;
   Long lg;
   int nb=0;
   
   Img3duc imi(ims);
   imd=0;
   imi=ims;
   
   if (relation <= -3 ) length=MAXLONG;
   else if (relation >= 3 ) length=0;
   
   for (p.z = 0; p.z < ims.Depth(); p.z++)
   for (p.y = 0; p.y < ims.Height(); p.y++)
   for (p.x = 0; p.x < ims.Width(); p.x++)
   {
      if (imi[p]==0)
	 continue;
      
      // Count the number of neighbors.
      int ngb=0, w=0;
      for (int v=0; v<26; v++) {
	 if (ims.Hold(p+v26[v]) && ims[p+v26[v]]) {
	    w=v;
	    ngb++;
	 }
      }
      // Not an end point  ->break;
      if  (ngb>1) continue;

      imi[p]=0;
      contour.Push(p);
      lg=0;
      // If there exists some neighbors.
      if (ngb == 1)
	 contour = ContourTracking(ims, imi, p+v26[w], lg);

      imi[p]=255;

      // Not an open contour.
      if (lg == -1) {
	 contour.Reset();
	 continue;
      }

      lg++;

      // Max length.
      if (relation >= 3) { 
	 if (lg>length) {
	    for (i=0 ; i< nbmax; i++) {
	       contourmax[i].Reset();
	    }
	    while(!contour.Empty()) {
	       pt = contour.Pop();
	       contourmax[0].Push(pt);
	    }
	    nbmax=1;
	    length=lg;
	 } else if (lg==length) {
	    if (nbmax < MAXMAX) {
	       // Catenate new contour.
	       while(!contour.Empty()) {
		  pt = contour.Pop();
		  contourmax[nbmax].Push(pt);
	       }
	       nbmax++;
	    }
	 } else {
	    contour.Reset();
	 }
	 
	 // >=
      } else if (relation == 2) {
	 if (lg>=length) {
	    while(!contour.Empty()) {
	       pt = contour.Pop();
	       imd[pt] = 255;
	    }
	    nb++;
	 }

	 // >
      } else if (relation == 1) {
	 if (lg>length) {
	    puts("OK");
	    while(!contour.Empty()) {
	       pt = contour.Pop();
 	       imd[pt] = 255;
	       imi[pt]=0;
	    }
	    nb++;
	 }

	 // ==
      } else if (relation == 0) {
	 if (lg==length) {
	    while(!contour.Empty()) {
	       pt = contour.Pop();
	       imd[pt] = 255;
	    }
	    nb++;
	 } else {
	    contour.Reset();
	 }

	 // <
      } else if (relation == -1) {
	 if (lg<length) {
	    while(!contour.Empty()) {
	       pt = contour.Pop();
	       imd[pt] = 255;
	    }
	    nb++;
	 } else {
	       contour.Reset();
	 }

	 //  <=
      } else if (relation == -2) {
	 if (lg<=length) {
	    while(!contour.Empty()) {
	       pt = contour.Pop();
	       imd[pt] = 255;
	    }
	    nb++;
	 } else {
	    contour.Reset();
	 }

	 // Min length.
      } else if (relation <= -3) {
	 if (lg<length) {
	    for (i=0 ; i< nbmax; i++) {
	       contourmax[i].Reset();
	    }
	    while(!contour.Empty()) {
	       pt = contour.Pop();
	       contourmax[0].Push(pt);
	    }
	    nbmax=1;
	    length=lg;
	 } else if (lg==length) {
	    while(!contour.Empty()) {
	       pt = contour.Pop();
	       contourmax[nbmax].Push(pt);
	    }
	    nbmax++;
	 } else {
	    contour.Reset();
	 }
      }
      contour.Reset();
   }
   if (relation > 2 || relation < -2 ) {
      for (i=0; i<nbmax; i++) {
	 while(!contourmax[i].Empty()) {
	    pt = contourmax[i].Pop();
	    imd[pt] = 255;
	 }
	 nb++;
      }
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
      Img2duc* const ims = (Img2duc*)objs[0];
      objd[0] = new Img2duc(ims->Props());
      Img2duc* const imd = (Img2duc*)objd[0];
      
      result = POpenContourSelection(*ims, *imd, atoi(parv[0]), atoi(parv[1]));
      goto end;
   }
   if (objs[0]->Type() == Po_Img3duc) {
      Img3duc* const ims = (Img3duc*)objs[0];
      objd[0] = new Img3duc(ims->Props());
      Img3duc* const imd = (Img3duc*)objd[0];
      
      result = POpenContourSelection(*ims, *imd, atoi(parv[0]), atoi(parv[1]));
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
