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
 * @author Francois Angot - 1995-11-29
 * @author Régis Clouard - 2003-02-17 (fix bug: rgd=0)
 */

#include <pandore.h>
using namespace pandore;

/**
 * @file pboundingbox.cpp
 * Construction des rectangles exincrits des régions.
 */
Errc PBoundingBox( const Reg2d &rgs, Reg2d &rgd ) {
   Long x, y;
   Ulong c = rgs.Labels();

   Long *ymin = new Long[c + 1];
   Long *ymax = new Long[c + 1];
   Long *xmin = new Long[c + 1];
   Long *xmax = new Long[c + 1];
   Ulong *size = new Ulong[c + 1];

   if (!ymin || ! ymax || !xmin || !xmax) {
      std::cerr << "Error pboundingbox: not enough memory." << std::endl;
      return FAILURE;
   }

   for (c = 1; c <= rgs.Labels(); c++) {
      ymin[c] = xmin[c] = MAXLONG;
      ymax[c] = xmax[c] = 0;
      size[c] = 0;
   }
   
   for (y = 0; y < rgs.Height(); y++) {
      for (x = 0; x < rgs.Width(); x++) {
	 if ((c = rgs(y, x))) {
	    if (y < ymin[c]) { ymin[c] = y; }
	    if (y > ymax[c]) { ymax[c] = y; }
	    if (x < xmin[c]) { xmin[c] = x; }
	    if (x > xmax[c]) { xmax[c] = x; }
	    size[c]++;
	 }
	 rgd(y, x) = 0;
      }
   }
   for (c = 1; c <= rgs.Labels(); c++)
      for (y = ymin[c]; y <= ymax[c]; y++)
	 for (x = xmin[c]; x <= xmax[c]; x++) {
	    if (rgd(y, x) == 0 || size[c] < size[rgd(y, x)]) {
	       rgd(y, x) = c;
	    }
	 }
   
   rgd.Labels(rgs.Labels());
   
   delete[] ymin;
   delete[] ymax;
   delete[] xmin;
   delete[] xmax;
   delete[] size;
  
   return SUCCESS;
}

Errc PBoundingBox( const Reg3d &rgs, Reg3d &rgd ) {
   Long x, y, z;
   Ulong c=rgs.Labels();
  
   Long *zmin = new Long[c + 1];
   Long *zmax = new Long[c + 1];
   Long *ymin = new Long[c + 1];
   Long *ymax = new Long[c + 1];
   Long *xmin = new Long[c + 1];
   Long *xmax = new Long[c + 1];
   Ulong *size = new Ulong[c + 1];
   
   if (!zmin || !zmax || !ymin || ! ymax || !xmin || !xmax) {
      std::cerr << "Error pboundingbox: not enough memory." << std::endl;
      return FAILURE;
   }

   for (c = 1; c <= rgs.Labels(); c++) {
      zmin[c] = ymin[c] = xmin[c] = MAXLONG;
      zmax[c] = ymax[c] = xmax[c] = -1;
   }
  
   for (z = 0; z < rgs.Depth(); z++) {
      for (y = 0; y < rgs.Height(); y++) {
	 for (x = 0; x < rgs.Width(); x++) {
	    if ((c = rgs(z, y, x))) {
	       if (z < zmin[c]) { zmin[c] = z; }
	       if (z > zmax[c]) { zmax[c] = z; }
	       if (y < ymin[c]) { ymin[c] = y; }
	       if (y > ymax[c]) { ymax[c] = y; }
	       if (x < xmin[c]) { xmin[c] = x; }
	       if (x > xmax[c]) { xmax[c] = x; }
	    }
	    rgd(z, y, x) = 0;
	 }
      }
   }
   for (c = 1; c <= rgs.Labels(); c++) {
      for (z = zmin[c]; z <= zmax[c]; z++) {
	 for (y = ymin[c]; y <= ymax[c]; y++) {
	    for (x = xmin[c];x <= xmax[c];x++) {
	       if (rgd(z, y, x) == 0 || size[c] < size[rgd(z, y, x)]) {
		  rgd(z, y, x) = c;
	       }
	    }
	 }
      }
   }
   rgd.Labels(rgs.Labels());
   
   delete[] zmin;
   delete[] zmax;
   delete[] ymin;
   delete[] ymax;
   delete[] xmin;
   delete[] xmax;
   delete[] size;
  
   return SUCCESS;
}

#ifdef MAIN

/*
 * Modify only the following constants, and the function call.
 */
#define	USAGE	"usage: %s [-m mask] [rg_in|-] [rg_out|-]"
#define	PARC	0
#define	FINC	1
#define	FOUTC	1
#define	MASK	2 // Only masking .

int main( int argc, char *argv[] ) {
   Errc result;              // The result code of the execution.
   Pobject* mask;            // The region mask.
   Pobject* objin[FINC+1];   // The input objects.
   Pobject* objs[FINC+1];    // The source objects masked by the mask.
   Pobject* objout[FOUTC+1]; // The output object.
   Pobject* objd[FOUTC+1];   // The result object of the execution.
   char* parv[PARC+1];    // The input parameters.
 
   ReadArgs(argc, argv, PARC, FINC, FOUTC, &mask, objin, objs, objout, objd, parv, USAGE, 0);

   switch(objs[0]->Type()) {

   case Po_Reg2d : {
      Reg2d* const rgs=(Reg2d*)objs[0];
      objd[0]=new Reg2d(rgs->Size());
      Reg2d* const rgd=(Reg2d*)objd[0];

      result=PBoundingBox(*rgs, *rgd);
   } break;
   case Po_Reg3d : {
      Reg3d* const rgs=(Reg3d*)objs[0];
      objd[0]=new Reg3d(rgs->Size());
      Reg3d* const rgd=(Reg3d*)objd[0];

      result=PBoundingBox(*rgs, *rgd);
   } break;

   default:
      PrintErrorFormat(objin, FINC);
      result=FAILURE;
   }
   
   if (result) {
      WriteArgs(argc, argv, PARC, FINC, FOUTC, &mask, objin, objs, objout, objd, MASK);
   }
   Exit(result);
   return 0;
}

#endif
