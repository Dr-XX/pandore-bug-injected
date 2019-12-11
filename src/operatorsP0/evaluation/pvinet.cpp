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
 * @file pvinet.cpp
 *
 * Discrepancy measure based on the number of mis-segmented pixels.
 * Ref: L. Vinet, "Segmentation et mise en correspondance
 * de regions de paires d'images steroscopiques", These de l'Universite
 * de Paris IX Dauphine, 1991.
 * Le principe consiste à determiner les couples de regions
 * ayant un taux de recouvrement maximum entre les deux segmentations
 * et a caracteriser la dissimilarite par la proportion de pixels ne
 * participant pas a ce recouvrement. Pour cela, on definit la table de
 * superposition des etiquettes:
 * T(i, hj)=card(Ri union Uj), =1, .., m et j=1, ..., n
 * La dissimilarite est calculee par : (N-sum(Ck))/N où1 N est
 * le nombre de pixels et Ck=max(T[i, j])
 */

/**
 * @param rgs1	a region map.
 * @param rgs2	a region map.
 */
Float PVinet( const Reg2d &rgs1, const Reg2d &rgs2 ) {
   Ulong i;
   Ulong C;
   int n = rgs1.VectorSize(); // The number of labels.

   if (rgs1.Size() != rgs2.Size()) {
      std::cerr << "Error pvinet: Input region maps have differente size." << std::endl;
      return FAILURE;
   }

   // If one of the input map contains no region.
   if (rgs1.Labels() * rgs2.Labels() == 0) {
      // If both are null
      if (rgs1.Labels() == 0 && rgs2.Labels() == 0) {
	 return 0.0F; // Equals
      } else {
	 return 1.0F; // Completely different
      }
   }
   Ulong **T = (Ulong**)malloc((rgs1.Labels() + 1) * sizeof(Ulong*));
   for (i = 0; i < rgs1.Labels() + 1; i++) {
      T[i] = (Ulong*)calloc((rgs2.Labels() + 1), sizeof(Ulong));
   }

   // Computes the superposition table.
   for (int y = 0; y < rgs1.Height(); y++) {
      for (int x = 0; x < rgs1.Width(); x++) {
	 T[rgs1[y][x]][rgs2[y][x]]++;
      }
   }
   // Discards label 0.
   n -= T[0][0];
   T[0][0] = 0;

   // Get couple of regions with the
   // the maximum cover rate.
   char *R = (char*)calloc(rgs1.Labels(), sizeof(char));
   char *U = (char*)calloc(rgs2.Labels(), sizeof(char));
   C = 0;
   int k = MIN(rgs1.Labels(), rgs2.Labels()) + 1;
   for (; k > 0; k--) {
      int r1max = 0;
      int r2max = 0;
      for (Ulong r1 = 1; r1 < rgs1.Labels() + 1; r1++) {
	 for (Ulong r2 = 1; r2 < rgs2.Labels() + 1; r2++) {
	    if (!R[r1] && !U[r2] && T[r1][r2] > T[r1max][r2max]) {
	       r1max = r1;
	       r2max = r2;
	    }
	 }
      }
      C += T[r1max][r2max];
      T[r1max][r2max] = 0;   
      R[r1max] = 1;
      U[r2max] = 1;
   }
  
   for (i = 0; i < rgs1.Labels() + 1; i++) {
      free(T[i]);
   }
   free(T);
   free(R);
   free(U);
   
   return ((Float)n - C) / n; 
}

/**
 * @param rgs1	a region map.
 * @param rgs2	a region map.
 */
Float PVinet( const Reg3d &rgs1, const Reg3d &rgs2 ) {
   Ulong i;
   Ulong C;
   int n = rgs1.VectorSize(); // The number of labels.
   
   if (rgs1.Size() != rgs2.Size()) {
      std::cerr << "Error pvinet: Input region maps have differente size." << std::endl;
      return FAILURE;
   }
   
   // If one of the input map contains no region.
   if (rgs1.Labels() * rgs2.Labels() == 0 ) {
      // If both are null
      if (rgs1.Labels() == 0 && rgs2.Labels() == 0) {
	 return 0.0F; // Equals
      } else {
	 return 1.0F; // Completely different
      }
   }
   Ulong **T = (Ulong**)malloc((rgs1.Labels() + 1) * sizeof(Ulong*));
   for (i = 0; i < rgs1.Labels() + 1; i++) {
      T[i] = (Ulong*)calloc((rgs2.Labels() + 1), sizeof(Ulong));
   }
   
   // Computes the superposition table.
   for (int z = 0; z < rgs1.Depth(); z++) {
      for (int y = 0; y <rgs1.Height(); y++) {
	 for (int x = 0; x < rgs1.Width(); x++) {
	    T[rgs1[z][y][x]][rgs2[z][y][x]]++;
	 }
      }
   }
   // Discards label 0.
   n -= T[0][0];
   T[0][0] = 0;

   // Get couple of regions with
   // maximum cover rate.
   char *R  =(char*)calloc(rgs1.Labels(), sizeof(char));
   char *U = (char*)calloc(rgs2.Labels(), sizeof(char));
   C = 0;
   int k = MIN(rgs1.Labels(), rgs2.Labels()) + 1;
   for (; k > 0; k--) {
      int r1max = 0;
      int r2max = 0;
      for (Ulong r1 = 1; r1 < rgs1.Labels() + 1; r1++) {
	 for (Ulong r2 = 1; r2 < rgs2.Labels() + 1; r2++) {
	    if (!R[r1] && !U[r2] && T[r1][r2] > T[r1max][r2max]) {
	       r1max = r1;
	       r2max = r2;
	    }
	 }
      }
      C += T[r1max][r2max];
      T[r1max][r2max] = 0;   
      R[r1max] = 1;
      U[r2max] = 1;
   }
  
   for (i = 0; i < rgs1.Labels() + 1; i++) {
      free(T[i]);
   }
   free(T);
   free(R);
   free(U);
  
   return ((Float)n - C ) / n; 
}

#ifdef MAIN
#define	USAGE	"USAGE : %s [-m mask] [rg1_in|-] [rg2_in|-]"
#define	PARC	0
#define	FINC	2
#define	FOUTC	0
#define	MASK	1

int main( int argc, char *argv[] ) {
   Errc result;              // The result code of the execution.
   Pobject* mask;            // The region map.
   Pobject* objin[FINC+1];   // The input objects.
   Pobject* objs[FINC+1];    // The source objects masked.
   Pobject* objout[FOUTC+1]; // The output object.
   Pobject* objd[FOUTC+1];   // The result object of the execution.
   char* parv[PARC+1];       // The input parameters.

   ReadArgs(argc, argv, PARC, FINC, FOUTC, &mask, objin, objs, objout, objd, parv, USAGE, MASK);

   if (objs[0]->Type() == Po_Reg2d && objs[1]->Type() == Po_Reg2d) {
      Reg2d* const rgs1 = (Reg2d*)objs[0];
      Reg2d* const rgs2 = (Reg2d*)objs[1];
      
      result = PVinet(*rgs1, *rgs2);
      goto end;
   }

   if (objs[0]->Type() == Po_Reg3d && objs[1]->Type() == Po_Reg3d) {
      Reg3d* const rgs1 = (Reg3d*)objs[0];
      Reg3d* const rgs2 = (Reg3d*)objs[1];
     
      result = PVinet(*rgs1, *rgs2);
      goto end;
   }
   {
      PrintErrorFormat(objin, FINC);
      result = FAILURE;
   }	

 end:
   WriteArgs(argc, argv, PARC, FINC, FOUTC, &mask, objin, objs, objout, objd, MASK);
   Exit(result);
   return 0;
}

#endif
