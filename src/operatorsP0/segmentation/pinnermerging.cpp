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
 * @file pinneremerging_englobee.cpp
 * Fusionne les regions englobees dans d'autres.
 */

Errc PInnerMerging( const Reg2d &rgs1, const Reg2d &rgs2, Reg2d &rgd ) {
   Reg2d::ValueType label;
   Long nbreg;
   Reg2d::ValueType* voisine; // le numero de la region voisine
   Ushort* nbvoisine; // le nombre de regions voisines.
   register int y,x;
   int v;
   Ulong i;
   
   voisine = new Reg2d::ValueType[rgs2.Labels()+1];
   nbvoisine = new Ushort[rgs2.Labels()+1];
   for (i = 0; i < rgs2.Labels() + 1; i++) {
      voisine[i] = nbvoisine[i] = 0;
   }
   
   // Compte le nombre de voisins dans rgs1
   // de chaque region de rgs2.
   for (y=0;y<rgs1.Height();y++) {
      for (x=0;x<rgs1.Width();x++) {
	 if ((label=rgs2[y][x])) {
	    for (v=0; v<8; v++) {
	       if (rgs2.Hold(y+v8[v].y,x+v8[v].x) && (rgs2[y+v8[v].y][x+v8[v].x] != label)) { // Un point du bord.
		  if (nbvoisine[rgs2[y][x]] == 0) {
		     voisine[rgs2[y][x]] = rgs1[y+v8[v].y][x+v8[v].x];
		     nbvoisine[rgs2[y][x]]=1;
		  } else {
		     if (voisine[rgs2[y][x]] != rgs1[y+v8[v].y][x+v8[v].x]) {
			nbvoisine[rgs2[y][x]] ++;
		     }
		  }
	       }
	    }
	 }
      }
   }
   rgd = rgs1;
   // Construction de la carte de sortie.
   for (y=0;y<rgs1.Height();y++) {
      for (x=0;x<rgs1.Width();x++) {
	 if ((nbvoisine[rgs2[y][x]] == 1) && (voisine[rgs2[y][x]])) {
	    rgd[y][x] = voisine[rgs2[y][x]];
	 }
      }
   }

   // Calcul du nombre de regions fusionnees.
   for (nbreg=0,i=0;i<rgs2.Labels()+1;i++) {
      if (nbvoisine[i] == 1) {
	 nbreg++;
      }
   }
   
   delete[] voisine;
   delete[] nbvoisine;
   return(nbreg);
}

Errc PInnerMerging( const Reg3d &rgs1, const Reg3d &rgs2, Reg3d &rgd ) {
   Reg3d::ValueType label;
   Long nbreg;
   Reg3d::ValueType* voisine; // le numero de la region voisine
   Ushort* nbvoisine; // le nombre de regions voisines.
   register int z,y,x;
   int v;
   Ulong i;
   
   voisine = new Reg3d::ValueType[rgs2.Labels()+1];
   nbvoisine = new Ushort[rgs2.Labels()+1];
   for (i=0;i<rgs2.Labels()+1;i++) {
      voisine[i] = nbvoisine[i] = 0;
   }
   // Compte le nombre de voisins dans rgs1
   // de chaque region de rgs2.
   for (z=0;z<rgs1.Depth();z++) {
      for (y=0;y<rgs1.Height();y++) {
	 for (x=0;x<rgs1.Width();x++) {
	    if ((label=rgs2[z][y][x])) {
	       for (v=0; v<26; v++) {
		  if (rgs2.Hold(z+v26[v].z,y+v26[v].y,x+v26[v].x) && (rgs2[z+v26[v].z][y+v26[v].y][x+v26[v].x] != label)) { // Un point du bord.
		     if (nbvoisine[rgs2[z][y][x]] == 0) {
			voisine[rgs2[z][y][x]] = rgs1[z+v26[v].z][y+v26[v].y][x+v26[v].x];
			nbvoisine[rgs2[z][y][x]]=1;
		     } else {
			if (voisine[rgs2[z][y][x]] != rgs1[z+v26[v].z][y+v26[v].y][x+v26[v].x]) {
			   nbvoisine[rgs2[z][y][x]] ++;
			}
		     }
		  }
	       }
	    }
	 }
      }
   }
   
   rgd = rgs1;
   // Construction de la carte de sortie.
   for (z=0;z<rgs1.Depth();z++) {
      for (y=0;y<rgs1.Height();y++) {
	 for (x=0;x<rgs1.Width();x++) {
	    if ((nbvoisine[rgs2[z][y][x]] == 1) && (voisine[rgs2[z][y][x]])) {
	       rgd[z][y][x] = voisine[rgs2[z][y][x]];
	    }
	 }
      }
   }
   
   // Calcul du nombre de regions fusionnees.
   for (nbreg=0,i=0;i<rgs2.Labels()+1;i++) {
      if (nbvoisine[i] == 1) {
	 nbreg++;
      }
   }
   
   delete[] voisine;
   delete[] nbvoisine;
   return(nbreg);
}

#ifdef MAIN

/*
 * Modify only the following constants, and the function call.
 */
#define	USAGE	"usage: %s [rg_in1|-] [rg_in2|-] [rg_out|-]"
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

      result=PInnerMerging(*rgs1,*rgs2,*rgd);
   } break;

   case Po_Reg3d : {
      Reg3d* const rgs1=(Reg3d*)objs[0];
      Reg3d* const rgs2=(Reg3d*)objs[1];
      objd[0]=new Reg3d(rgs1->Size());
      Reg3d* const rgd=(Reg3d*)objd[0];

      result=PInnerMerging(*rgs1,*rgs2,*rgd);
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
