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
 * @author Regis Clouard - 10 Feb 2011
 */

#include <pandore.h>
using namespace pandore;

/**
 * @file psizeselection.cpp
 * Selects regions from their size (depth, height or/and width).
 */
struct SizeFeatures {
      Long minDepth;
      Long maxDepth;
      Long minHeight;
      Long maxHeight;
      Long minWidth;
      Long maxWidth;
      bool acceptee;

      SizeFeatures(): minDepth(MAXLONG), 
		      maxDepth(0),
		      minHeight(MAXLONG),
		      maxHeight(0),
		      minWidth(MAXLONG),
		      maxWidth(0) {  }
};

/*
 * Fonction qui calcule la surface de chacune des regions presentes.
 * Rem : la surface est calculee en pixels.
 */
Errc PSizeSelection( const Reg2d &rgs, SizeFeatures *regions ) {
   for (int y = 0; y < rgs.Height(); y++) {
      for (int x = 0; x < rgs.Width(); x++) {
	 Ulong label = rgs(y, x);
	 if (label > 0) {
	    if (regions[label].minHeight > y) {
	       regions[label].minHeight = y;
	    }
	    if (regions[label].maxHeight < y) {
	       regions[label].maxHeight = y;
	    }
	    if (regions[label].minWidth > x) {
	       regions[label].minWidth = x;
	    }
	    if (regions[label].maxWidth < x) {
	       regions[label].maxWidth = x;
	    }
	 }
      }
   }
   return SUCCESS;
}
/*
 * Fonction qui calcule la surface de chacune des regions presentes.
 * Rem : la surface est calculee en pixels.
 */
Errc PSizeSelection( const Reg3d &rgs, SizeFeatures *regions ) {
   for ( int z = 0; z < rgs.Depth(); z++ ) {
      for ( int y = 0; y < rgs.Height(); y++ ) {
	 for ( int x = 0; x < rgs.Width(); x++ ) {
	    Ulong label = rgs(z, y, x);
	    if (label > 0) {
	       if (regions[label].minDepth >  z) {
		  regions[label].minDepth = z;
	       }
	       if (regions[label].maxDepth < z) {
		  regions[label].maxDepth = z;
	       }
	       if (regions[label].minHeight > y) {
		  regions[label].minHeight = y;
	       }
	       if (regions[label].maxHeight < y) {
		  regions[label].maxHeight = y;
	       }
	       if (regions[label].minWidth > x) {
		  regions[label].minWidth = x;
	       }
	       if (regions[label].maxWidth < x) {
		  regions[label].maxWidth = x;
	       }
	    }
	 }
      }
   }
   return SUCCESS;
}

/*
 * keep only regions that have its feature value
 * <= if relation=-2, < if relation=-1, > if relation=1, >= if relation=2
 * to the given value.
 */
Errc PSizeSelection( const Reg2d &rgs, Reg2d &rgd, int relation, long width, long height ) {
   SizeFeatures *regions = new SizeFeatures[rgs.Labels() + 1];
   Ulong nreg = 0, r;
   
   PSizeSelection(rgs, regions);
   regions[0].acceptee = false;

   // Selection des regions
   if (relation == 2) { // >=
      for (r = 1; r <= rgs.Labels(); r++) {
	 if (  (width < 0 || regions[r].maxWidth - regions[r].minWidth + 1 >= width)
	       && 
	       (height < 0 || regions[r].maxHeight - regions[r].minHeight + 1 >= height)) {
	    regions[r].acceptee = true;
	    ++nreg;
	 } else {
	    regions[r].acceptee = false;
	 }
      }
   } else if (relation == 1) { // >
      for (r = 1; r <= rgs.Labels(); r++) {
	 if (  ((width < 0) || ((regions[r].maxWidth - regions[r].minWidth + 1) > width))
	       && 
	       ((height < 0) || (regions[r].maxHeight - regions[r].minHeight + 1) > height)) {
	    regions[r].acceptee = true;
	    ++nreg;
	 } else {
	    regions[r].acceptee = false;
	 }
      }
   } else if (relation == 0) { // ==
      for (r = 1; r <= rgs.Labels(); r++) {
	 if (  (width<0 || (regions[r].maxWidth - regions[r].minWidth+1) == width)
	       && 
	       (height<0 || (regions[r].maxHeight - regions[r].minHeight + 1) == height)) {
	    regions[r].acceptee = true;
	    ++nreg;
	 } else {
	    regions[r].acceptee = false;
	 }
      }
   } else if (relation == -1) { // <
      for (r = 1; r <= rgs.Labels(); r++) {
	 if (  (width < 0 || (regions[r].maxWidth - regions[r].minWidth + 1) < width)
	       && 
	       (height < 0 || (regions[r].maxHeight - regions[r].minHeight + 1) < height)) {
	    regions[r].acceptee = true;
	    ++nreg;
	 } else {
	    regions[r].acceptee = false;
	 }
      }
   } else if (relation == -2) { // <=
      for (r = 1; r <= rgs.Labels(); r++) {
	 if (  (width < 0 || (regions[r].maxWidth - regions[r].minWidth + 1) <= width)
	       && 
	       (height < 0 || (regions[r].maxHeight - regions[r].minHeight + 1) <= height)) {
	    regions[r].acceptee = true;
	    ++nreg;
	 } else {
	    regions[r].acceptee = false;
	 }
      }
   }
   
   // Construction de la carte de sortie.
   for (int y = 0; y < rgs.Height(); y++) {
      for (int x = 0; x < rgs.Width(); x++) {
	 if (regions[rgs(y, x)].acceptee) {
	    rgd(y, x) = rgs(y, x);
	 } else {
	    rgd(y, x) = 0;
	 }
      }
   }
   rgd.Labels(rgs.Labels());
   delete[] regions;
   return nreg;
}

/*
 * keep only regions that have its feature value
 * <= if relation=-2, < if relation=-1, > if relation=1, >= if relation=2
 * to the given value.
 */
Errc PSizeSelection( const Reg3d &rgs, Reg3d &rgd, int relation, long width, long height, long depth ) {
   SizeFeatures *regions = new SizeFeatures[rgs.Labels() + 1];
   Ulong nreg = 0, r;

   PSizeSelection(rgs, regions);
   regions[0].acceptee = false;

   // Selection des regions
   if (relation == 2) {
      for (r = 1; r <= rgs.Labels(); r++) {
	 if (  (depth < 0 || regions[r].maxDepth - regions[r].minDepth + 1 >= depth)
	       && 
	       (width < 0 || regions[r].maxWidth - regions[r].minWidth + 1 >= width)
	       && 
	       (height < 0 || regions[r].maxHeight-regions[r].minHeight + 1 >= height)) {
	    regions[r].acceptee = true;
	    ++nreg;
	 } else {
	    regions[r].acceptee = false;
	 }
      }
   } else if (relation == 1) {
      for (r = 1; r <= rgs.Labels(); r++) {
	 if (  (depth < 0 || regions[r].maxDepth - regions[r].minDepth + 1 >= depth)
	       && 
	       ((width < 0) || ((regions[r].maxWidth - regions[r].minWidth + 1) > width))
	       && 
	       ((height < 0) || (regions[r].maxHeight - regions[r].minHeight + 1) > height)) {
	    regions[r].acceptee = true;
	    ++nreg;
	 } else {
	    regions[r].acceptee = false;
	 }
      }
   } else if (relation == 0) {
      for (r = 1; r <= rgs.Labels(); r++) {
	 if (  (depth < 0 || regions[r].maxDepth-regions[r].minDepth + 1 >= depth)
	       && 
	       (width < 0 || (regions[r].maxWidth-regions[r].minWidth + 1) == width)
	       && 
	       (height < 0 || (regions[r].maxHeight-regions[r].minHeight + 1) == height)) {
	    regions[r].acceptee = true;
	    ++nreg;
	 } else {
	    regions[r].acceptee = false;
	 }
      }
   } else if (relation ==-1) {
      for (r = 1; r <= rgs.Labels(); r++) {
	 if (  (depth<0 || regions[r].maxDepth - regions[r].minDepth + 1 >= depth)
	       && 
	       (width<0 || (regions[r].maxWidth - regions[r].minWidth + 1) < width)
	       && 
	       (height<0 || (regions[r].maxHeight - regions[r].minHeight + 1) < height)) {
	    regions[r].acceptee = true;
	    ++nreg;
	 } else {
	    regions[r].acceptee = false;
	 }
      }
   } else if (relation == -2) {
      for (r = 1; r <= rgs.Labels(); r++) {
	 if (  (depth < 0 || regions[r].maxDepth - regions[r].minDepth + 1 >= depth)
	       && 
	       (width < 0 || (regions[r].maxWidth - regions[r].minWidth + 1) <= width)
	       && 
	       (height < 0 || (regions[r].maxHeight - regions[r].minHeight + 1) <= height)) {
	    regions[r].acceptee = true;
	    ++nreg;
	 } else {
	    regions[r].acceptee = false;
	 }
      }
   }
   
   // Construction de la carte de sortie.
   for (int z = 0; z < rgs.Depth(); z++) {
      for (int y = 0; y < rgs.Height(); y++) {
	 for (int x = 0; x < rgs.Width(); x++) {
	    if (regions[rgs(z, y, x)].acceptee) {
	       rgd(z, y, x) = rgs(z, y, x);
	    } else {
	       rgd(z, y, x) = 0;
	    }
	 }
      }
   }

   rgd.Labels(rgs.Labels());
   delete[] regions;
   return nreg;
}

#ifdef MAIN

/*
 * Modify only the following constants, and the function call.
 */
#define	USAGE	"usage: %s relation width height depth [-m mask] [rg_in|-] [rg_out|-]"
#define	PARC	4
#define	FINC	1
#define	FOUTC	1
#define	MASK	1

int main( int argc, char *argv[] ) {
   Errc result;               // The result code of the execution.
   Pobject* mask;              // The region mask.
   Pobject* objin[FINC + 1];   // The input objects.
   Pobject* objs[FINC + 1];    // The source objects masked by the mask.
   Pobject* objout[FOUTC + 1]; // The output object.
   Pobject* objd[FOUTC + 1];   // The result object of the execution.
   char* parv[PARC + 1];       // The input parameters.
   
   ReadArgs(argc, argv, PARC, FINC, FOUTC, &mask, objin, objs, objout, objd, parv, USAGE, MASK);
   
   switch(objs[0]->Type()) {
   case Po_Reg2d :{
      Reg2d* const rgs = (Reg2d*)objs[0];
      objd[0] = new Reg2d(rgs->Size());
      Reg2d* const rgd = (Reg2d*)objd[0];
      
      result = PSizeSelection(*rgs, *rgd, atoi(parv[0]), atol(parv[1]), atol(parv[2]));
   }break;
   case Po_Reg3d :{
      Reg3d* const rgs = (Reg3d*)objs[0];
      objd[0] = new Reg3d(rgs->Size());
      Reg3d* const rgd = (Reg3d*)objd[0];
      
      result = PSizeSelection(*rgs, *rgd, atoi(parv[0]), atol(parv[1]), atol(parv[2]), atol(parv[3]));
   }break;
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
