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

/*
 * @author Francois Angot - 1995-02-08
 * @author Régis Clouard - 2001-04-27 (version 3.00)
 */

#include <pandore.h>
using namespace pandore;

/**
 * @file pgr2rg.cpp		
 * Selection region of a region map from a graph.
 * and color regions with the Item number.
 */

Errc PGr2Rg( const Graph2d &grs, const Reg2d &rgs, Reg2d &rgd ) {
   for (int y = 0; y < rgs.Height(); y++) {
      for (int x = 0; x < rgs.Width(); x++) {
	 Long u;
	 if ((u = rgs(y, x)) && grs[u]) {
	    rgd(y, x) = u;
	 } else {
	    rgd(y, x) = 0;
	 }
      }
   }

   rgd.Labels(rgs.Labels());
   return SUCCESS;
}

Errc PGr2Rg( const Graph3d &grs, const Reg3d &rgs, Reg3d &rgd ) {
   for (int z = 0; z < rgs.Depth(); z++) {
      for (int y = 0; y < rgs.Height(); y++) {
	 for (int x = 0; x < rgs.Width(); x++) {
	    Long u;
	    if ((u = rgs(z, y, x)) && grs[u]) {
	       rgd(z, y, x) = u;
	    } else {
	       rgd(z, y, x) = 0;
	    }
	 }
      }
   }

   rgd.Labels(rgs.Labels());
   return SUCCESS;
}

#ifdef MAIN

/*
 * Modify only the following constants, and the function call.
 */
#define	USAGE	"usage: %s [gr_in|-] [rg_in|-] [rg_out|-]"
#define	PARC	0
#define	FINC	2
#define	FOUTC	1
#define	MASK	0

int main( int argc, char *argv[] ) {
  Errc	 result;	    // The result code of the execution.
  Pobject* mask;	    // The region mask.
  Pobject* objin[FINC+1];   // The input objects.
  Pobject* objs[FINC+1];    // The source objects masked by the mask.
  Pobject* objout[FOUTC+1]; // The output object.
  Pobject* objd[FOUTC+1];   // The result object of the execution.
  char* parv[PARC+1];	    // The input parameters.

  ReadArgs(argc, argv, PARC, FINC, FOUTC, &mask, objin, objs, objout, objd, parv, USAGE, MASK);

  switch(objs[0]->Type()) {
  case Po_Graph2d :{
    Graph2d* const grs = (Graph2d*)objs[0];
    Reg2d* const rgs = (Reg2d*)objs[1];
    objd[0] = new Reg2d(rgs->Size());
    Reg2d* const rgd = (Reg2d*)objd[0];

    result = PGr2Rg(*grs, *rgs, *rgd);
  } break;
  case Po_Graph3d :{
    Graph3d* const grs = (Graph3d*)objs[0];
    Reg3d* const rgs = (Reg3d*)objs[1];
    objd[0] = new Reg3d(rgs->Size());
    Reg3d* const rgd = (Reg3d*)objd[0];

    result = PGr2Rg(*grs, *rgs, *rgd);
  } break;
  default :
    PrintErrorFormat(objin, FINC);
    result=FAILURE;
  }

  if (result) {
     WriteArgs(argc, argv, PARC, FINC, FOUTC, &mask, objin, objs, objout, objd);
  }

  Exit(result);
  return 0;
}

#endif
