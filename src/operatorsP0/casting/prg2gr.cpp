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
 * @author François Angot - 1997-07-18
 * @author Regis Clouard - 2001-04-11 (version 3.00)
 */

#include <pandore.h>
using namespace pandore;

/**
 * @file prg2gr.cpp
 * Convert a region map to a graph.
 * Seed are the barycenter of the region:
 * Could be out of the region boundaries....
 */

Errc PRg2Gr( const Reg2d &rgs, Graph2d &grd ) {
   Ulong e;
   float u;
   float *tab;

   grd.Init(rgs);
   
   if ((tab = (float*)calloc((rgs.Labels() + 1) * 3, sizeof(float))) == NULL) {
      return FAILURE;
   }

   // Compute center of mass
   for (int y = 0; y < rgs.Height(); y++) {
      for (int x = 0; x < rgs.Width(); x++) {
	 if ((e = rgs[y][x]) != 0) {
	    tab[e * 3 + 0] += (float)x;
	    tab[e * 3 + 1] += (float)y;
	    tab[e * 3 + 2]++;
	 }
      }
   }

   for (Ulong i = 1; i <= rgs.Labels(); i++) {
      if ((u = tab[i * 3 + 2])) {
	 grd[i]->seed.x = (Long)(tab[i * 3 + 0] / u);
	 grd[i]->seed.y = (Long)(tab[i * 3 + 1] / u);
      }
   }

   free(tab);
   
   return SUCCESS;
}

Errc PRg2Gr( const Reg3d &rgs, Graph3d &grd ) {
   float *tab;
   float u;
   Ulong e;
   
   grd.Init(rgs);
   
   if ((tab = (float*)calloc((rgs.Labels() + 1) * 4, sizeof(float))) == NULL) {
      return FAILURE;
   }
   
   for (int z = 0; z < rgs.Depth(); z++) {
      for (int y = 0; y < rgs.Height(); y++) {
	 for (int x = 0; x < rgs.Width(); x++) {
	    if ((e = rgs[z][y][x]) != 0) {
	       tab[e * 3 + 0] += (float)x;
	       tab[e * 3 + 1] += (float)y;
	       tab[e * 3 + 2] += (float)z;
	       tab[e * 3 + 3]++;
	    }
	 }
      }
   }

   puts("HERE2");

   for (Ulong i = 1; i <= rgs.Labels(); i++) {
      if ((u = tab[i * 3 + 3]) != 0) {
	 if (grd[i] == NULL) {
	    printf("pb %d\n",i);
	 }
	 grd[i]->seed.x = (Long)(tab[i * 3 + 0] / u);
	 grd[i]->seed.y = (Long)(tab[i * 3 + 1] / u);
	 grd[i]->seed.z = (Long)(tab[i * 3 + 2] / u);
      }
   }

   free(tab);
   
   return SUCCESS;
}

#ifdef MAIN

/*
 * Modify only the following constants, and the function call.
 */
#define	USAGE	"usage: %s [-m mask] [rg_in|-] [gr_out|-]"
#define	PARC	0
#define	FINC	1
#define	FOUTC	1
#define	MASK	2

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
   case Po_Reg2d :{
      Reg2d* const rgs = (Reg2d*)objs[0];
      objd[0] = new Graph2d;
      Graph2d* const grd = (Graph2d*)objd[0];

      result = PRg2Gr(*rgs, *grd);
   }break;
   case Po_Reg3d :{
      Reg3d* const rgs = (Reg3d*)objs[0];
      objd[0] = new Graph3d;
      Graph3d* const grd = (Graph3d*)objd[0];

      result = PRg2Gr(*rgs, *grd);
   }break;
   default:
      {
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
