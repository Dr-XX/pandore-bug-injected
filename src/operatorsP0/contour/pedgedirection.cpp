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
 * @author Régis Clouard - Jul 04, 2011
 */

#define _USE_MATH_DEFINES
#include <cmath>
#include <pandore.h>
using namespace pandore;

void smooth( Fifo<Point2d> &f1, Fifo<Point2d> &f2, Img2dsf &imy, Img2dsf &imx ) {
   Point2d pt1, pt2;

   if (!f1.Empty()) {
      Point2d pt1 = f1.Pop(); f2.Push(pt1);
   }
   if (!f1.Empty()) {
      Point2d pt2 = f1.Pop(); f2.Push(pt2);
   }
   
   while(!f1.Empty()) {
      Point2d pt3 = f1.Pop();
      f2.Push(pt3);
      // Smooth avec the filter 1 2 1.
      imx[pt2] = (imx[pt1] + 2 * imx[pt2] + imx[pt3]) / 4;
      imy[pt2] = (imy[pt1] + 2 * imy[pt2] + imy[pt3]) / 4;
      pt1 = pt2;
      pt2 = pt3;
   }
}

/**
 * Computes the direction of chain of contours.
 */
template <typename T>
Errc PEdgeDirection( const Img2d<T> &ims, Img2dsf &imd ) {
   Img2d<T> imi(ims.Props());
   Img2dsf imx(ims.Props());
   Img2dsf imy(ims.Props());
   Fifo<Point2d> f1;
   Fifo<Point2d> f2;

   imx = 0;
   imy = 0;
   imi = ims;

   // Compute first approximation of the normal direction
   Point2d p;

   for (p.y = 0; p.y < imi.Height(); p.y++ ) {
      for (p.x = 0; p.x < imi.Width(); p.x++ ) {
	 if ((imi[p])) {

	    Point2d p1 = p;
	    // use only the first neighbor
	    for (int v = 4; v < 12; v++) {
	       Point2d pt = p1 + v8[v%8];
	       if ((imi[pt])) {
		  imy[p1] = (Float)v8y[(v + 2) % 8];
		  imx[p1] = (Float)v8x[(v + 2) % 8];
		  f1.Push(pt);
		  p1 = pt;
		  break;
	       }
	    }
	    if (f1.Empty()) {
	       imi[p1] = 0;
	    }
	    while ((imi[p1])) {
	       imi[p1] = 0;
	       // use only the first neighbor
	       for (int v = 4; v < 12; v++) {
		  Point2d pt = p1 + v8[v % 8];
		  if ((imi[pt])) {
		     imy[p1] = (Float)v8y[(v + 2) % 8];
		     imx[p1] = (Float)v8x[(v + 2) % 8];
		     f1.Push(pt);
		     p1 = pt;
		     break;
		  }
	       }
	    }
 	    smooth(f1, f2, imy, imx);
 	    smooth(f2, f1, imy, imx);
	    f1.Reset();
	    f2.Reset();
	 }
      }
   }
 
   for (p.y = 0; p.y < imi.Height(); p.y++ ) {
      for (p.x = 0; p.x < imi.Width(); p.x++ ) {
	 if ((!ims[p])) {
	    imd[p] = 0.0;
	 } else {
	    double angle = atan2(-imy[p], imx[p]);
	    if (angle >= M_PI) {
	       imd[p] = Float(angle - M_PI);
	    } else if (angle >= 0) {
	       imd[p] = Float(angle);
	    } else if (angle < -M_PI) {
	       imd[p] = Float((2 * M_PI) + angle);
	    } else {
	       imd[p] = Float(M_PI + angle);
	    }
	 }
      }
   }

   return SUCCESS;
}

#ifdef MAIN
#define USAGE	"usage: %s [-m mask] [im_in|-] [im_out|-]"
#define PARC	0
#define FINC	1
#define FOUTC	1
#define MASK	1

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
      objd[0] = new Img2dsf(ims->Props());
      Img2dsf* const imd = (Img2dsf*)objd[0];
      
      result = PEdgeDirection(*ims, *imd);
      goto end;
   }
   if (objs[0]->Type() == Po_Reg2d) {
      Reg2d* const ims = (Reg2d*)objs[0];
      objd[0] = new Img2dsf(ims->Props());
      Img2dsf* const imd = (Img2dsf*)objd[0];
      
      result = PEdgeDirection(*ims, *imd);
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
