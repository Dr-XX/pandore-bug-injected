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

#include <pandore.h>
using namespace pandore;

/**
 * @file plabelsselection.cpp
 * Selects the regions in rgs1 that have a label in rgs2.
 */

Errc PLabelsSelection( const Reg2d &rgs1, const Reg2d &rgs2, Reg2d &rgd ) {
   if (rgs1.Size() != rgs2.Size()) {
      std::cerr << "Error plabelsselection: Incompatible region map sizes." << std::endl;
   }

   char * regions = new char[rgs2.Labels() + 1];
   for (Ulong r = 0; r < rgs2.Labels() + 1; regions[r++] = 0) ;
   
   for (int y = 0; y < rgs2.Height(); y++) {
      for (int x = 0; x < rgs2.Width(); x++) {
	 Ulong r = rgs2(y, x);
	 regions[r] = 1;
      }
   }

   for (int y = 0; y < rgs1.Height(); y++) {
      for (int x = 0; x < rgs1.Width(); x++) {
	 Ulong r = rgs1(y, x);
	 if (regions[r] > 0) {
	    rgd(y, x) = r;
	    regions[r] = 2;
	 } else {
	    rgd(y,x) = 0;
	 }
      }
   }
   
   int nbSelectedRegions = 0;
   for (Ulong r = 0; r < rgs2.Labels() + 1; regions[r++] = 0) {
      if (regions[r] == 2) {
	 nbSelectedRegions++;
      }
   }
   rgd.Labels(rgs2.Labels());
   return nbSelectedRegions;
}

Errc PLabelsSelection( const Reg3d &rgs1, const Reg3d &rgs2, Reg3d &rgd ) {
   if (rgs1.Size() != rgs2.Size()) {
      std::cerr << "Error plabelsselection: Incompatible region map sizes." << std::endl;
   }

   char *regions = new char[rgs2.Labels() + 1];
   for (Ulong r = 0; r < rgs2.Labels() + 1; regions[r++] = 0) ;
   
   for (int z = 0; z < rgs2.Depth(); z++) {
      for (int y = 0; y < rgs2.Height(); y++) {
	 for (int x = 0; x < rgs2.Width(); x++) {
	    regions[rgs2(z, y, x)] = 1;
	 }
      }
   }

   for (int z = 0; z < rgs1.Depth(); z++) {
      for (int y = 0; y < rgs1.Height(); y++) {
	 for (int x = 0; x < rgs1.Width(); x++) {
	    Ulong r = rgs1(z, y, x);
	    if (regions[r] > 0) {
	       rgd(z, y, x) = r;   
	       regions[r] = 2;
	    } else {
	       rgd(z, y, x) = 0;
	    }
	 }
      }
   }
   
   int nbSelectedRegions = 0;
   for (Ulong r = 0; r < rgs2.Labels() + 1; regions[r++] = 0) {
      if (regions[r] == 2) {
	 nbSelectedRegions++;
      }
   }
   rgd.Labels(rgs2.Labels());
   return nbSelectedRegions;
}

#ifdef MAIN

/*
 * Modify only the following constants, and the function call.
 */
#define USAGE	"usage: %s [-m mask] [rg_in1|-] [rg_in2|-] [rg_out|-]"
#define PARC	0
#define FINC	2
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

   if (objs[0]->Type() == Po_Reg2d && objs[1]->Type() == Po_Reg2d) {
      Reg2d* const rgs1 = (Reg2d*)objs[0];
      Reg2d* const rgs2 = (Reg2d*)objs[1];
      objd[0] = new Reg2d(rgs1->Size());
      Reg2d* const rgd = (Reg2d*)objd[0];
      
      result = PLabelsSelection(*rgs1, *rgs2, *rgd);
      
   } else if (objs[0]->Type() == Po_Reg3d &&  objs[1]->Type() == Po_Reg3d) {
      Reg3d* const rgs1 = (Reg3d*)objs[0];
      Reg3d* const rgs2 = (Reg3d*)objs[1];
      objd[0] = new Reg3d(rgs1->Size());
      Reg3d* const rgd = (Reg3d*)objd[0];

      result = PLabelsSelection(*rgs1, *rgs2, *rgd);

   } else {
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
