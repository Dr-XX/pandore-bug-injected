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
 * @author Regis Clouard - 2003-07-22
 * @author Regis Clouard - 2004-06-29 (>=, <=)
 * @author Regis Clouard - 2006-06-02 (relation =3 and -3).
 */

#include <map>
#include <pandore.h>
using namespace pandore;

/**
 * @file plocationselection.cpp
 *
 * Selects regions from their location.
 */

struct Attrs_location {
      Ulong xmin;
      Ulong ymin;
      Ulong zmin;
      Ulong xmax;
      Ulong ymax;
      Ulong zmax;
      bool acceptee;
   public:
      Attrs_location(): xmin(MAXLONG), ymin(MAXLONG), zmin(MAXLONG), xmax(0), ymax(0), zmax(0), acceptee(false) { }
};

void PLocationSelection( const Reg2d &rgs, Attrs_location *regions ) {
   
   for (Ulong y = 0; y < (Ulong)rgs.Height(); y++) {
      for (Ulong x = 0 ; x < (Ulong)rgs.Width(); x++) {
	 Ulong r = rgs(y, x);
	 if (x > regions[r].xmax) {
	    regions[r].xmax = x;
	 }
	 if (y > regions[r].ymax) {
	    regions[r].ymax = y;
	 }
	 if (x < regions[r].xmin) {
	    regions[r].xmin = x;
	 }
	 if (y < regions[r].ymin) {
	    regions[r].ymin = y;
	 }
      }
   }
}


void PLocationSelection( const Reg3d &rgs, Attrs_location *regions ) {
   
   for (Ulong z = 0; z < (Ulong)rgs.Depth(); z++) {
      for (Ulong y = 0; y < (Ulong)rgs.Height(); y++) {
	 for (Ulong x = 0; x < (Ulong)rgs.Width(); x++) {
	    Ulong r = rgs(z, y, x);
	    if (x > regions[r].xmax) {
	       regions[r].xmax = x;
	    }
	    if (y > regions[r].ymax) {
	       regions[r].ymax = y;
	    }
	    if (z > regions[r].zmax) {
	       regions[r].zmax = z;
	    }
	    if (x < regions[r].xmin) {
	       regions[r].xmin = x;
	    }
	    if (y < regions[r].ymin) {
	       regions[r].ymin = y;
	    }
	    if (z < regions[r].zmin) {
	       regions[r].zmin = z;
	    }
	 }
      }
   }
}

Errc PLocationSelection( const Reg2d &rgs, Reg2d &rgd, int relation,
			 Ulong xmin, Ulong ymin, Ulong xmax, Ulong ymax ) {
   Ulong nreg = 0;
   
   Attrs_location *regions  = new Attrs_location[rgs.Labels() + 1];

   PLocationSelection(rgs, regions);
   regions[0].acceptee = false;
   // Selection des regions

   for (Ulong r = 1; r <= rgs.Labels(); r++) {
      if (regions[r].xmin >= xmin && regions[r].ymin >= ymin
	  && regions[r].xmax <= xmax && regions[r].ymax <= ymax ) {
	 regions[r].acceptee = relation > 0;
      } else {
	 regions[r].acceptee = relation < 0;
      }
   }

   // Construction de la carte de sortie.
   Reg2d::ValueType *prgs = rgs.Vector();
   Reg2d::ValueType *prgd = rgd.Vector();
   Reg2d::ValueType *end = rgs.Vector()+rgs.VectorSize();
   
   for (; prgs < end; prgs++, prgd++) {
      if (regions[*prgs].acceptee) {
	 *prgd = *prgs;
      } else {
	 *prgd = 0;
      }
   }


   rgd.Labels(rgs.Labels());
   free(regions);
   return nreg;
}


Errc PLocationSelection( const Reg3d &rgs, Reg3d &rgd, int relation,
			 Ulong xmin, Ulong ymin, Ulong zmin, Ulong xmax, Ulong ymax, Ulong zmax ) {
   struct Attrs_location *regions;
   Ulong nreg = 0;
   
   regions = (struct Attrs_location*)calloc(rgs.Labels()+1, sizeof(struct Attrs_location));
   
   PLocationSelection(rgs, regions);
   regions[0].acceptee = false;
   // Selection des regions

   for (Ulong r = 1; r <= rgs.Labels(); r++) {
      if (regions[r].xmin >= xmin && regions[r].ymin >= ymin && regions[r].zmin >= zmin
	  && regions[r].xmax <= xmax && regions[r].ymax <= ymax && regions[r].zmax <= zmax ) {
	 regions[r].acceptee = relation > 0;
      } else {
	 regions[r].acceptee = relation < 0;
      }
   }

   // Construction de la carte de sortie.
   Reg3d::ValueType *prgs = rgs.Vector();
   Reg3d::ValueType *prgd = rgd.Vector();
   Reg3d::ValueType *end = rgs.Vector()+rgs.VectorSize();
   
   for (; prgs < end; prgs++, prgd++) {
      if (regions[*prgs].acceptee) {
	 *prgd = *prgs;
      } else {
	 *prgd = 0;
      }
   }


   rgd.Labels(rgs.Labels());
   free(regions);
   return nreg;
}


#ifdef MAIN

/*
 * Modify only the following constants, and the function call.
 */
#define	USAGE	"usage: %s relation xmin ymin zmin xmax ymax zmax [-m mask] [rg_in|-] [rg_out|-]"
#define	PARC	7
#define	FINC	1
#define	FOUTC	1
#define	MASK	1

int main( int argc, char *argv[] ) {
   Errc result;             // The result code of the execution.
   Pobject* mask;            // The region mask.
   Pobject* objin[FINC+1];   // The input objects.
   Pobject* objs[FINC+1];    // The source objects masked by the mask.
   Pobject* objout[FOUTC+1]; // The output object.
   Pobject* objd[FOUTC+1];   // The result object of the execution.
   char* parv[PARC+1];       // The input parameters.
   
   ReadArgs(argc, argv, PARC, FINC, FOUTC, &mask, objin, objs, objout, objd, parv, USAGE, MASK);
   
   switch (objs[0]->Type()) {
   case Po_Reg2d :{
      Reg2d* const rgs = (Reg2d*)objs[0];
      objd[0] = new Reg2d(rgs->Size());
      Reg2d* const rgd = (Reg2d*)objd[0];
      
      result = PLocationSelection(*rgs, *rgd, atoi(parv[0]),
				  (Ulong)atol(parv[1]), (Ulong)atol(parv[2]),
				  (Ulong)atol(parv[4]), (Ulong)atol(parv[5]));
   } break;
   case Po_Reg3d :{
      Reg3d* const rgs = (Reg3d*)objs[0];
      objd[0] = new Reg3d(rgs->Size());
      Reg3d* const rgd = (Reg3d*)objd[0];
      
      result = PLocationSelection(*rgs, *rgd, atoi(parv[0]),
				  (Ulong)atol(parv[1]), (Ulong)atol(parv[2]), (Ulong)atol(parv[3]), 
				  (Ulong)atol(parv[4]), (Ulong)atol(parv[5]), (Ulong)atol(parv[6]));
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
