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
 * @file plabelselection.cpp
 * Selects the specified region. Does not relabel
 * the selected region.
 * @param num	the number of the region to be selected.
 * if num=-1 then selects the last region.
 */

Errc PLabelSelection( const Reg2d &rgs, Reg2d &rgd, Long num ) {
   if ((num < 0) || ((Ulong)num > rgs.Labels())) {
      num = rgs.Labels();
   }
   Long nb = 0;
   for (int y = 0; y < rgs.Height(); y++) {
      for (int x = 0; x < rgs.Width(); x++) {
	 Long v;
	 if ((v = rgs(y, x)) == num) {
	    rgd(y, x) = rgs(y, x);
	    nb++;
	 } else {
	    rgd(y, x) = 0;
	 }
      }
   }
   rgd.Labels(num);
   return nb;
}

Errc PLabelSelection( const Reg3d &rgs, Reg3d &rgd, Long num ) {
   if ((num < 0) || ((Ulong)num > rgs.Labels())) {
      num = rgs.Labels();
   }
   
   Long nb = 0;
   for (int z = 0; z < rgs.Depth(); z++) {
      for (int y = 0; y < rgs.Height(); y++) {
	 for (int x = 0; x < rgs.Width(); x++) {
	    Long v;
	    if ((v = rgs(z, y, x)) == num) {
	       rgd(z, y, x) = rgs(z, y ,x);
	       nb++;
	    } else {
	       rgd(z, y, x) = 0;
	    }
	 }
      }
   }
   rgd.Labels(num);
   return nb;
}

#ifdef MAIN

/*
 * Modify only the following constants, and the function call.
 */
#define USAGE	"usage: %s label [-m mask] [rg_in|-] [rg_out|-]"
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

   switch(objs[0]->Type()) {
   case Po_Reg2d : {
      Reg2d* const rgs = (Reg2d*)objs[0];
      objd[0] = new Reg2d(rgs->Size());
      Reg2d* const rgd = (Reg2d*)objd[0];

      result = PLabelSelection(*rgs, *rgd, (Long)atoi(parv[0]));

   } break;

   case Po_Reg3d : {
      Reg3d* const rgs = (Reg3d*)objs[0];
      objd[0] = new Reg3d(rgs->Size());
      Reg3d* const rgd = (Reg3d*)objd[0];

      result = PLabelSelection(*rgs, *rgd, (Long)atoi(parv[0]));

   } break;

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
