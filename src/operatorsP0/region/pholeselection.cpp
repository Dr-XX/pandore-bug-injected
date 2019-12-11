/*  -*- mode: c++; c-basic-offset: 3 -*-
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
 * @author Régis Clouard - Apr 25, 2014
 */

#include <pandore.h>
using namespace pandore;

namespace segmentation {
#ifdef MAIN
#undef MAIN
#include "segmentation/plabeling.cpp"
#define MAIN
#else
#include "segmentation/plabeling.cpp"
#endif
}

#define NOTHOLE -2
#define UNVISITED -1

/**
 * @param connexity the component connexity (4 or 8).
 */
Errc PHoleSelection( const Reg2d &rgs, Reg2d &rgd, int connexity ) {
   if ( connexity != 4 && connexity != 8 ) {
      std::cerr << "Error pholeselection: bad connexity value:" << connexity << std::endl;
   }

   Img2duc mismatchedImage(rgs.Props());

   // Build the mask image with the detected regions only.
   Point2d p;
   for (p.y = 0; p.y < rgs.Height(); p.y++) {
      for ( p.x = 0; p.x < rgs.Width(); p.x++) {
	 int r = rgs[p];
	 if (r == 0 ) {
	    mismatchedImage[p] = 255;
	 } else {
	    mismatchedImage[p] = 0;
	 }
      }
   }

   Reg2d holeImage(mismatchedImage.Props());
   Ulong numberOfLabels = segmentation::PLabeling(mismatchedImage, holeImage, connexity);

   long *neighbors = new long[numberOfLabels + 1];
   for (int r = 1; r < (int)numberOfLabels + 1; r++) {
      neighbors[r] = UNVISITED;
   }

   // A hole is a region that has only one neighbor.
   for (p.y = 0; p.y < holeImage.Height(); p.y++) {
      for (p.x = 0; p.x < holeImage.Width(); p.x++) {
	 Ulong label = holeImage[p];
	 int segment = neighbors[label];
	 if (label > 0 && segment != NOTHOLE ) {
	    for (int v = 0; v < connexity; v++) {
	       Point2d offset = reinterpret_cast<const Point2d*>(vc[connexity])[v];
	       Point2d q = p + offset;
	       if (holeImage.Hold(q)) {
		  if (holeImage[q] == 0) { // Hole boundary.
		     long region = neighbors[label];
		     if (region == UNVISITED) {
			neighbors[label] = rgs[q];
		     } else if (neighbors[label] != (long)rgs[q]) {
			neighbors[label] = NOTHOLE;
		     }
		  }
	       } else {
		  neighbors[label] = NOTHOLE;
	       }
	    }
	 }
      }
   }
   
   // Count the mean of hole ratio (|hole|/|segment|)
   Ulong labels = 0;
   for (p.y = 0; p.y < holeImage.Height(); p.y++) {
      for (p.x = 0; p.x < holeImage.Width(); p.x++) {
	 if (neighbors[holeImage[p]] > 0) {
	    rgd[p] = holeImage[p];
	    if (labels < holeImage[p]) {
	       labels = holeImage[p];
	    }
	 } else {
	    rgd[p] = 0;
	 }
      }
   }
   rgd.Labels(labels);
   
   delete[] neighbors;
   return rgd.Labels();
}

#ifdef MAIN

/*
 * Modify only the following constants, and the function call.
 */
#define USAGE	"usage: %s connexity [-m mask] [rg_in|-] [rg_out|-]"
#define PARC	1
#define FINC	1
#define FOUTC	1
#define MASK	1

int main( int argc, char *argv[] ) {
   Errc result;                // The result code of the execution.
   Pobject* mask;              // The region mask.
   Pobject* objin[FINC + 1];   // The input objects.
   Pobject* objs[FINC + 1];    // The source objects masked by the mask.
   Pobject* objout[FOUTC + 1]; // The output object.
   Pobject* objd[FOUTC + 1];   // The result object of the execution.
   char* parv[PARC + 1];       // The input parameters.

   ReadArgs(argc, argv, PARC, FINC, FOUTC, &mask, objin, objs, objout, objd, parv, USAGE, MASK);

   switch (objs[0]->Type()) {

      case Po_Reg2d : {
	 Reg2d* const rgs = (Reg2d*)objs[0];
	 objd[0] = new Reg2d(rgs->Props());
	 Reg2d* const rgd = (Reg2d*)objd[0];

	 result = PHoleSelection(*rgs, *rgd, atoi(parv[0]));
      } break;

      default:
	 PrintErrorFormat(objin,FINC);
	 result = FAILURE;
   }

   if (result) {
      WriteArgs(argc, argv, PARC, FINC, FOUTC, &mask, objin, objs, objout, objd, MASK);
   }

   Exit(result);
   return(0);
}

#endif
