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
 * @author Francois Angot - 1996-09-12
 * @author Regis Clouard - 2002-06-25
 */

#include <pandore.h>
using namespace pandore;

/**
 * @file prelabelingwithgraph.cpp	
 * Minimisation des numeros d'etiquette.
 */
Errc PRelabelingWithGraph( const Reg2d &rgs, const Graph2d &grs, Reg2d &rgd, Graph2d &grd ) {
   Long y,x,i;
   Long u;
   Long *tab; // Tableau de conversion
   Long nb;
   GEdge *ptr;

   // Calloc -> initialisation a 0.
   if ((tab=(Long*)calloc(rgs.Labels()+1,sizeof(Long))) == NULL) {
      return FAILURE;
   }
   nb=0;
   
   for (y=0;y<rgs.Height();y++) {
      for (x=0;x<rgs.Width();x++) {
	 if ((u=rgs[y][x])) {
	    if (tab[u]==0) {
	       rgd[y][x] = tab[u]=++nb;
	    } else {
	       rgd[y][x] = tab[u];
	    }
	 } else {
	    rgd[y][x]=0;
	 }
      }

   }
   // Renumerotation du graphe associe.
   grd.New(nb+1,grs.Height(),grs.Width());

   for (i=0;i<grs.Size();i++) {
      if (!grs[i]) {
	 continue;
      }
      grd.Add(tab[i],tab[i],grs[i]->seed);
   }

   for (i=0;i<grs.Size();i++) {
      if (!grs[i]) {
	 continue;
      }
      for (ptr=grs[i]->Neighbours();ptr;ptr=ptr->Next())
	 grd.Link(tab[i],tab[ptr->Node()]);
   }

   free(tab);
   return rgd.Labels(nb);
}

Errc PRelabelingWithGraph( const Reg3d &rgs, const Graph3d &grs, Reg3d &rgd, Graph3d &grd ) {
   Long z,y,x,i;
   Long u;
   Long *tab;
   Long nb;
   GEdge *ptr;

   // Calloc -> initialisation a 0.
   if ((tab=(Long*)calloc(rgs.Labels()+1,sizeof(Long))) == NULL) {
      return FAILURE;
   }
   nb=0;
   for (z=0;z<rgs.Depth();z++) {
      for (y=0;y<rgs.Height();y++) {
	 for (x=0;x<rgs.Width();x++) {
	    if ((u=rgs[z][y][x])) {
	       if (tab[u]==0) {
		  rgd[z][y][x] = tab[u]=++nb;
	       } else {
		  rgd[z][y][x] = tab[u];
	       }
	    } else {
	       rgd[z][y][x]=0;
	    }
	 }
      }
   }
   
   // Renumerotation du graphe associe.
   grd.New(nb+1,grs.Depth(),grs.Height(),grs.Width());

   for (i=0;i<grs.Size();i++) {
      if (!grs[i]) {
	 continue;
      }
      grd.Add(tab[i],tab[i],grs[i]->seed);
   }

   for (i=0;i<grs.Size();i++) {
      if (!grs[i]) {
	 continue;
      }
      for (ptr=grs[i]->Neighbours();ptr;ptr=ptr->Next()) {
	 grd.Link(tab[i],tab[ptr->Node()]);
      }
   }
   
   free(tab);
   return rgd.Labels(nb);
}

/*
 * Modify only the following constants, and the function call.
 */
#ifdef MAIN

#define USAGE	"usage: %s [rg_in|-] [gr_in|-] [rg_out|-] [gr_out|-]"
#define PARC	0
#define FINC	2
#define FOUTC	2
#define MASK	0

int main( int argc, char *argv[] ) {
   Errc result;              // The result code of the execution.
   Pobject* mask;            // The region mask.
   Pobject* objin[FINC+1];   // The input objects.
   Pobject* objs[FINC+1];    // The source objects masked by the mask.
   Pobject* objout[FOUTC+1]; // The output object.
   Pobject* objd[FOUTC+1];   // The result object of the execution.
   char* parv[PARC+1];       // The input parameters.
 
   ReadArgs(argc,argv,PARC,FINC,FOUTC,&mask,objin,objs,objout,objd,parv,USAGE,MASK);

   if (objs[0]->Type()== Po_Reg2d && objs[1]->Type()== Po_Graph2d) {
      Reg2d* const rgs=(Reg2d*)objs[0];
      Graph2d* const grs=(Graph2d*)objs[1];
      objd[0]=new Reg2d(rgs->Size());
      Reg2d* const rgd=(Reg2d*)objd[0];
      objd[1]=new Graph2d;
      Graph2d* const grd=(Graph2d*)objd[1];
      
      result=PRelabelingWithGraph(*rgs,*grs,*rgd,*grd);
   } else {  
      if (objs[0]->Type()== Po_Reg3d && objs[1]->Type()== Po_Graph3d) {
	 Reg3d* const rgs=(Reg3d*)objs[0];
	 Graph3d* const grs=(Graph3d*)objs[1];
	 objd[0]=new Reg3d(rgs->Size());
	 Reg3d* const rgd=(Reg3d*)objd[0];
	 objd[1]=new Graph3d(grs->Size());
	 Graph3d* const grd=(Graph3d*)objd[1];
	 
	 result=PRelabelingWithGraph(*rgs,*grs,*rgd,*grd);
      } else {
	 PrintErrorFormat(objin,FINC);
	 
	 result=FAILURE;
      }
   }
   
   if (result) {
      WriteArgs(argc,argv,PARC,FINC,FOUTC,&mask,objin,objs,objout,objd,MASK);
   }

   Exit(result);
   return 0;
}

#endif
