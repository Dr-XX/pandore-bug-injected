/* -*- c-basic-offset: 3; mode: c++ -*-
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
 * @author Régis Clouard - 1999-19-02
 * @author Regis Clouard - 2006-06-02 (relation =3 and -3).
 */

#include <map>
#include <pandore.h>
using namespace pandore;

/**
 * @file pinnerselection.cpp
 * Selects inner regions.
 */


Errc PInnerSelection( const Reg2d &rgs, Reg2d& rgd ) {
   Reg2d::ValueType label;
   std::map<Reg2d::ValueType, Reg2d::ValueType> voisine;   // Dans voisine le numero de la region voisine.
   std::map<Reg2d::ValueType, int> nbvoisine; // Dans nbvoisine le nombre de regions voisines.

   //Calcul du nombre de voisins de chaque region.
   Point2d p;
   for (p.y = 0; p.y < rgs.Height(); p.y++)
   for (p.x = 0; p.x < rgs.Width(); p.x++)
   {
      if ((label = rgs[p])) {
	 for (int v = 0; v < 8; v++) {
	    Point2d q = p  +v8[v];
	    if (rgs.Hold(q)) {
	       if (rgs[q] != label) {
		  if ((nbvoisine[label] != 1) || (voisine[label] != rgs[q])) {
		     nbvoisine[label]++;
		     voisine[label] = rgs[q];
		  }
	       }
	    } else {
	       nbvoisine[label] = 2;
	    }
	 }
      }
   }

   // Construction de la carte de sortie.
   Long nreg=0;
   std::map<Reg2d::ValueType, int> labels;

   for (p.y = 0; p.y < rgs.Height(); p.y++)
   for (p.x = 0; p.x < rgs.Width(); p.x++)
   {
      if (((label = rgs[p])) && (nbvoisine[label] == 1) && (voisine[label] > 0)) {
	 // Count the number of regions.
	 if (!labels[label]) {
	    labels[label] = ++nreg;
	 }
	 rgd[p] = rgs[p];
      } else {
	 rgd[p] = 0;
      }
   }
   rgd.Labels(rgs.Labels());
   
   return nreg;
}

Errc PInnerSelection( const Reg3d &rgs, Reg3d& rgd ) {
   Reg3d::ValueType label;
   std::map<Reg3d::ValueType, Reg3d::ValueType> voisine;   // Dans voisine le numero de la region voisine.
   std::map<Reg3d::ValueType, int> nbvoisine; // Dans nbvoisine le nombre de regions voisines.

   //Calcul du nombre de voisins de chaque region.
   Point3d p;
   for (p.z = 0; p.z < rgs.Depth(); p.z++)
   for (p.y = 0; p.y < rgs.Height(); p.y++)
   for (p.x = 0; p.x < rgs.Width(); p.x++)
   {
      if ((label = rgs[p])) {
	 for (int v = 0; v < 26; v++) {
	    Point3d q = p  +v26[v];
	    if (rgs.Hold(q)) {
	       if (rgs[q] != label) {
		  if ((nbvoisine[label] != 1) || (voisine[label] != rgs[q])) {
		     nbvoisine[label]++;
		     voisine[label] = rgs[q];
		  }
	       }
	    } else {
	       nbvoisine[label] = 2;
	    }
	 }
      }
   }

   // Construction de la carte de sortie.
   Long nreg=0;
   std::map<Reg3d::ValueType, int> labels;

   for (p.z = 0; p.z < rgs.Depth(); p.z++)
   for (p.y = 0; p.y < rgs.Height(); p.y++)
   for (p.x = 0; p.x < rgs.Width(); p.x++)
   {
      if (((label = rgs[p])) && (nbvoisine[label] == 1) && (voisine[label] > 0)) {
	 // Count the number of regions.
	 if (!labels[label]) {
	    labels[label] = ++nreg;
	 }
	 rgd[p] = rgs[p];
      } else {
	 rgd[p] = 0;
      }
   }
   rgd.Labels(rgs.Labels());
   
   return nreg;
}


#ifdef MAIN

/*
 * Modify only the following constants, and the function call.
 */
#define USAGE	"usage: %s [-m mask] [rg_in|-] [rg_out|-]"
#define PARC	0
#define FINC	1
#define FOUTC	1
#define MASK	2

int main( int argc, char *argv[] ) {
   Errc result;                // The result code of the execution.
   Pobject* mask;              // The region map.
   Pobject* objin[FINC + 1];   // The input objects.
   Pobject* objs[FINC + 1];    // The source objects masked.
   Pobject* objout[FOUTC + 1]; // The output object.
   Pobject* objd[FOUTC + 1];   // The result object of the execution.
   char* parv[PARC + 1];       // The input parameters.

   ReadArgs(argc, argv, PARC, FINC, FOUTC, &mask, objin, objs, objout, objd, parv, USAGE, MASK); 
   if (objs[0]->Type() == Po_Reg2d) {
      Reg2d* const rgs = (Reg2d*)objs[0];
      objd[0] = new Reg2d(rgs->Props());
      Reg2d* const rgd = (Reg2d*)objd[0];
      
      result = PInnerSelection(*rgs, *rgd);
      goto end;
   }
   if (objs[0]->Type() == Po_Reg3d) {
      Reg3d* const rgs = (Reg3d*)objs[0];
      objd[0] = new Reg3d(rgs->Props());
      Reg3d* const rgd = (Reg3d*)objd[0];
      
      result = PInnerSelection(*rgs, *rgd);
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
