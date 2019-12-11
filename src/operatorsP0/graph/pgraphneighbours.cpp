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
 * @author Francois Angot - 1995-02-08
 */

#include <pandore.h>
using namespace pandore;

/**
 * @file pgraphneighbours.cpp
 * Attribue a chaque valeur de noeud le nombre de ses voisins.
 */
Errc PGraphNeighbours( const Graph2d &grs, Graph2d &grd ) {
   Long i;
   Ulong u;
   GEdge *ptr;
   
   grd = grs;
   
   for (i = 0; i < grs.Size(); i++) {
      if (!grs[i]) {
	 continue;
      }
      u = 0;
      for (ptr = grs[i]->Neighbours(); ptr != NULL; ptr = ptr->Next()) {
	 u++;
      }
      grd[i]->value = (Graph2d::ValueType)u;
   }
   
   return SUCCESS;
}

Errc PGraphNeighbours( const Graph3d &grs, Graph3d &grd ) {
   Long i;
   Ulong u;
   GEdge *ptr;
  
   grd  =grs;
   
   for (i = 0; i < grs.Size(); i++) {
      if (!grs[i]) {
	 continue;
      }
      u = 0;
      for (ptr = grs[i]->Neighbours(); ptr != NULL; ptr = ptr->Next()) {
	 u++;
      }
      grd[i]->value = (Graph3d::ValueType)u;
   }
   
   return SUCCESS;
}

#ifdef MAIN

/*
 * Modify only the following constants, and the function call.
 */
#define	USAGE	"usage: %s [gr_in|-] [gr_out|-]"
#define	PARC	0
#define	FINC	1
#define	FOUTC	1
#define	MASK	1

int main( int argc, char *argv[] ) {
   Errc result;                // The result code of the execution.
   Pobject* mask;              // The region mask.
   Pobject* objin[FINC + 1];   // The input objects.
   Pobject* objs[FINC + 1];    // The source objects masked.
   Pobject* objout[FOUTC + 1]; // The output object.
   Pobject* objd[FOUTC + 1];   // The result object of the execution.
   char* parv[PARC + 1];       // The input parameters.
 
   ReadArgs(argc, argv, PARC, FINC, FOUTC, &mask, objin, objs, objout, objd, parv, USAGE, MASK);

   switch(objs[0]->Type()) {

   case Po_Graph2d : {
      Graph2d* const grs=(Graph2d*)objs[0];
      objd[0]=new Graph2d(grs->Size());
      Graph2d* const grd=(Graph2d*)objd[0];
      result=PGraphNeighbours(*grs, *grd);
   } break;

   case Po_Graph3d : {
      Graph3d* const grs=(Graph3d*)objs[0];
      objd[0]=new Graph3d(grs->Size());
      Graph3d* const grd=(Graph3d*)objd[0];
      result=PGraphNeighbours(*grs, *grd);
   } break;
      
   default :{
      PrintErrorFormat(objin, FINC);
      result = FAILURE;
   }
   }
   
   if (result) {
      WriteArgs(argc, argv, PARC, FINC, FOUTC, &mask, objin, objs, objout, objd, MASK);
   }

   Exit(result);
   return 0;
}

#endif
