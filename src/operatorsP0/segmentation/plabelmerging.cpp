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
 * @author Francois Angot - 1997-11-06
 * @author Regis Clouard - 2002-07-11 (region + graphe)
 */

#include <pandore.h>
using namespace pandore;

/**
 * @file plabelmerging.cpp
 * Fusion elementaire de 2 régions.
 */

/*
 * Pour les régions, la région résultante conserve le numero r1.
 * Pour le graphe, les coordonnées du nouveau sommet sont placées
 * au milieu des 2 sommets fusionnés.
 * La liste des voisins de chacun des sommets est fusionnée dans le nouveau.
 * Les valeurs de Item() est celle du premier sommet.
 */

Errc PLabelMerging( const Reg2d &rgs, const Graph2d &grs, Reg2d &rgd, Graph2d &grd, Reg2d::ValueType r1, Reg2d::ValueType r2 ) {
   Point2d p;
   
   if (r1 == r2 || r1 > (Ulong)rgs.Labels() || r2 > (Ulong)rgs.Labels()) {
      std::cerr << "Error plabelmerging: Bad label number:" << r1 << ","<< r2<< std::endl;
      return FAILURE;
   }
   rgd = rgs;

   for (p.y = 0; p.y < rgs.Height(); p.y++)
   for (p.x = 0; p.x < rgs.Width(); p.x++)
   if (rgs[p] == r2) {
      rgd[p] = r1;
   }
   
   grd = grs;
   
   if ((r1 != r2) && grd[r1] && grd[r2]) {
      grd[r1]->seed = (grs[r1]->seed + grs[r2]->seed) / 2;
   } else {
      std::cerr << "Error plabelmerging: Bad label number:" << r1 << "," << r2 << std::endl;
      return FAILURE;
   }
   
   if (!grd.Merge(r1, r2)) {
      std::cerr << "Error plabelmerging: Bad label number:" << r1 << ","<< r2 << std::endl;
      return FAILURE;
   }
   return SUCCESS;
}

Errc PLabelMerging( const Reg3d &rgs, const Graph3d &grs, Reg3d &rgd, Graph3d &grd, Reg3d::ValueType r1, Reg3d::ValueType r2 ) {
   Point3d p;
   
   if (r1 == r2 || r1 > (Ulong)rgs.Labels() || r2 > (Ulong)rgs.Labels()) {
      std::cerr << "Error plabelmerging: Bad label number:" << r1 << ","<< r2<< std::endl;
      return FAILURE;
   }
   rgd = rgs;

   for (p.z = 0; p.z < rgs.Depth(); p.z++)
   for (p.y = 0; p.y < rgs.Height(); p.y++)
   for (p.x = 0; p.x < rgs.Width(); p.x++)
   if (rgs[p] == r2) {
      rgd[p] = r1;
   }
   
   grd = grs;
   
   if ((r1 != r2) && grd[r1] && grd[r2]) {
      grd[r1]->seed = (grs[r1]->seed + grs[r2]->seed) / 2;
   } else {
      std::cerr << "Error plabelmerging: Bad label number:" << r1 << "," << r2 << std::endl;
      return FAILURE;
   }
   
   if (!grd.Merge(r1, r2)) {
      std::cerr << "Error plabelmerging: Bad label number:" << r1 << ","<< r2 << std::endl;
      return FAILURE;
   }
   return SUCCESS;
}


#ifdef MAIN
/*
 * Modify only the following constants, and the operator switches.
 */
#define	USAGE	"usage: %s label1 label2 [rg_in|-] [gr_in|-] [rg_out|-] [gr_out|-]"
#define	PARC	2
#define	FINC	2
#define	FOUTC	2
#define	MASK	0

int main( int argc, char *argv[] ) {
   Errc result;                // The result code of the execution.
   Pobject* mask;              // The region map.
   Pobject* objin[FINC + 1];   // The input objects.
   Pobject* objs[FINC + 1];    // The source objects masked.
   Pobject* objout[FOUTC + 1]; // The output object.
   Pobject* objd[FOUTC + 1];   // The result object of the execution.
   char* parv[PARC + 1];       // The input parameters.

   ReadArgs(argc, argv, PARC, FINC, FOUTC, &mask, objin, objs, objout, objd, parv, USAGE, MASK); 
// verifier toutes les entrees
   if (objs[0]->Type() == Po_Reg2d && objs[1]->Type() == Po_Graph2d) {
      Reg2d* const rgs = (Reg2d*)objs[0];
      Graph2d* const grs = (Graph2d*)objs[1];  
      objd[0] = new Reg2d(rgs->Size());
      Reg2d* const rgd = (Reg2d*)objd[0];
      objd[1] = new Graph2d(grs->Size());
      Graph2d* const grd = (Graph2d*)objd[1];
      
      result = PLabelMerging(*rgs, *grs, *rgd, *grd, (Reg2d::ValueType)atof(parv[0]), (Reg2d::ValueType)atof(parv[1]));
      goto end;
   }
// verifier toutes les entrees
   if (objs[0]->Type() == Po_Reg3d && objs[1]->Type() == Po_Graph3d) {
      Reg3d* const rgs = (Reg3d*)objs[0];
      Graph3d* const grs = (Graph3d*)objs[1];  
      objd[0] = new Reg3d(rgs->Size());
      Reg3d* const rgd = (Reg3d*)objd[0];
      objd[1] = new Graph3d(grs->Size());
      Graph3d* const grd = (Graph3d*)objd[1];
      
      result = PLabelMerging(*rgs, *grs, *rgd, *grd, (Reg3d::ValueType)atof(parv[0]), (Reg3d::ValueType)atof(parv[1]));
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
