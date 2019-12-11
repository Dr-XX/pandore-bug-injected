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
 * @author Régis Clouard - 1999-02-19
 */

/**
 * @file pinnermostmerging.cpp
 * Fusionne les regions dans les regions les plus englobantes.
 */

#include <map>
#include <pandore.h>
using namespace pandore;

Errc PInnerMostMerging( const Reg2d &rgs1, const Reg2d &rgs2, Reg2d &rgd ) {
   if (rgs1.Size() != rgs2.Size()) {
      std::cerr<<"Error pinnermostmerging: Incompatible image sizes."<<std::endl;
   }

   int **voisine = (int**)malloc((rgs2.Labels()+1)*sizeof(int*));
   for (Ulong i=0; i<rgs2.Labels()+1; i++) {
      voisine[i] = (int*)calloc(rgs1.Labels()+1,sizeof(int));
   }
   
   // Calcul le graphe d'adjacence et le nombre de points communs.
   Point2d p;
   for (p.y=0; p.y<rgs2.Height(); p.y++) {
      for (p.x=0; p.x<rgs2.Width(); p.x++) {
	 Ulong label = rgs2[p];
	 if (label != 0) {
	    for (int v=0; v<8; v++) {
	       Point2d q = p+v8[v];
	       if (rgs2.Hold(q) && rgs2[q] != rgs2[p]) {
		  voisine[label][rgs1[q]]++;
	       }
	    }
	 }
      }
   }
   
   // Recherche de la plus englobante => voisine[i][0]
   int max;
   for (Ulong i=1; i<rgs2.Labels()+1; i++) {
      max = 0;
      voisine[i][0] = 0;
      for (Ulong j = 1; j<rgs1.Labels()+1; j++) {
	 if (voisine[i][max] < voisine[i][j]) {
	    max = j;
	 }
      }
      voisine[i][0] = max;
   }

   //
   for (Ulong i=1; i<rgs2.Labels()+1; i++) {
      Ulong label = voisine[i][0];
      if (label == 0) continue;
      std::map< Ulong, Ulong > passed;
      passed[label] = 1;
      bool cycle = false;
      
      while (voisine[label][0] > 0) {
         if (passed.count(voisine[label][0]) == 1) {
            cycle = true;
            break;
         }
         label = voisine[label][0];
         passed[label] = 1;
      }
      if (cycle == true)
         voisine[label][0] = 0;
      if (label != 0) {
	 voisine[i][0] = label;
      }
      for (std::map<Ulong, Ulong>::iterator it2 = passed.begin(); it2 != passed.end(); ++it2) {
         voisine[it2->first][0] = label;
      }
   }

   rgd = rgs1;
   // Construction de la carte de sortie.
   for (p.y=0; p.y<rgs1.Height(); p.y++) {
      for (p.x=0; p.x<rgs1.Width(); p.x++) {
	 if (voisine[rgs2[p]][0]) {
	    rgd[p] = voisine[rgs2[p]][0];
	 }
      }
   }
   
   // Calcul du nombre de regions fusionnees.
   Ulong nbreg=0;
   for (Ulong i=0;i<rgs2.Labels()+1;i++) {
      if (voisine[i][0]) {
	 nbreg++;
      }
   }

   for (Ulong i=0; i<rgs2.Labels()+1; i++) {
      free(voisine[i]);
   }
   free(voisine);

   return nbreg;
}

Errc PInnerMostMerging( const Reg3d &rgs1, const Reg3d &rgs2, Reg3d &rgd ) {
   if (rgs1.Size()!=rgs2.Size()) {
      std::cerr<<"Error pinnermostmerging: Incompatible image sizes."<<std::endl;
   }
   
   std::map< Ulong, std::map<Ulong, Ulong> > neighbors;
 
   // Calcul le graphe d'adjacence et le nombre de points communs.
   Point3d p;
   for (p.z=0; p.z<rgs2.Depth(); p.z++) {
      for (p.y=0; p.y<rgs2.Height(); p.y++) {
	 for (p.x=0; p.x<rgs2.Width(); p.x++) {
	    Ulong label=rgs2[p];
	    if (label!=0) {
	       for (int v=0; v<26; v++) {
		  Point3d q = p+v26[v];
		  if (rgs2.Hold(q) && rgs2[q]!=rgs2[p]) { // Boundary point
		     neighbors[label][rgs1[q]]++;
		  }
	       }
	    }
	 }
      }
   }
   
   // Recherche de la plus englobante => neighbors[i][0]
   neighbors[0][0] = 0;
   for (std::map< Ulong, std::map<Ulong, Ulong> >::iterator it = neighbors.begin(); it != neighbors.end(); ++it) {
      int max = 0;
      it->second[0] = 0;
      for (std::map<Ulong, Ulong>::iterator it2 = it->second.begin(); it2 != it->second.end(); it2++) {
	 if (it->second[max] < it2->second) {
	    max = it2->first;
	 }
      }
      it->second[0] = max;
   }

   // relabelisation avec des étiquettes uniquement de rgs1.
   for (std::map< Ulong, std::map<Ulong, Ulong> >::iterator it = neighbors.begin(); it != neighbors.end(); ++it) {
      Ulong label = it->second[0];
      if (label == 0) continue;
      std::map< Ulong, Ulong > passed;
      passed[label] = 1;
      bool cycle = false;
      
      while (neighbors[label][0] > 0) { // Un label de rgs2.
         if (passed.count(neighbors[label][0]) == 1) {
            cycle = true;
            break;
         }
         label = neighbors[label][0];
         passed[label] = 1;
      }
      if (cycle == true)
         neighbors[label][0] = 0;
      if (label != 0) {
	 it->second[0] = label;
      }
      for (std::map<Ulong, Ulong>::iterator it2 = passed.begin(); it2 != passed.end(); ++it2) {
         neighbors[it2->first][0] = label;
      }
      
   }

   // Construction de la carte de sortie.
   for (p.z=0; p.z<rgs1.Depth(); p.z++) {
      for (p.y=0; p.y<rgs1.Height(); p.y++) {
	 for (p.x=0; p.x<rgs1.Width(); p.x++) {
	    Ulong label = rgs2[p];
	    if (neighbors[label][0]>0) {
	       rgd[p] = neighbors[label][0];
	    } else {
	       rgd[p] = rgs1[p];
	    }
	 }
      }
   }

   // Calcul du nombre de regions fusionnees.
   Ulong nbreg=0;
   for (Ulong i=0; i<rgs2.Labels()+1; i++) { // TODO: Par cours de la map
      if (neighbors[i][0]) {
	 nbreg++;
      }
   }
   rgd.Labels(rgs1.Labels());
   return nbreg;
}

#ifdef MAIN

/*
 * Modify only the following constants, and the function call.
 */
#define	USAGE	"usage: %s [-m mask] [rg_in1|-] [rg_in2|-] [rg_out|-]"
#define	PARC	0
#define	FINC	2
#define	FOUTC	1
#define	MASK	0

int main( int argc, char *argv[] ) {
   Errc result;              // The result code of the execution.
   Pobject* mask;            // The region mask.
   Pobject* objin[FINC+1];   // The input objects.
   Pobject* objs[FINC+1];    // The source objects masked by the mask.
   Pobject* objout[FOUTC+1]; // The output object.
   Pobject* objd[FOUTC+1];   // The result object of the execution.
   char* parv[PARC+1];       // The input parameters.

   ReadArgs(argc,argv,PARC,FINC,FOUTC,&mask,objin,objs,objout,objd,parv,USAGE,MASK);

   switch(objs[0]->Type()) {
   case Po_Reg2d : {
      Reg2d* const rgs1=(Reg2d*)objs[0];
      Reg2d* const rgs2=(Reg2d*)objs[1];
      objd[0]=new Reg2d(rgs1->Size());
      Reg2d* const rgd=(Reg2d*)objd[0];

      result=PInnerMostMerging(*rgs1,*rgs2,*rgd);
   } break;

   case Po_Reg3d : {
      Reg3d* const rgs1=(Reg3d*)objs[0];
      Reg3d* const rgs2=(Reg3d*)objs[1];
      objd[0]=new Reg3d(rgs1->Size());
      Reg3d* const rgd=(Reg3d*)objd[0];

      result=PInnerMostMerging(*rgs1,*rgs2,*rgd);
   } break;

   default:
      PrintErrorFormat(objin,FINC);
      result=FAILURE;
   }

   if (result) {
      WriteArgs(argc,argv,PARC,FINC,FOUTC,&mask,objin,objs,objout,objd,MASK);
   }

   Exit(result);
   return 0;
}

#endif
