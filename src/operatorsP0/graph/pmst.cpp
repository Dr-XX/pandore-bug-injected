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
 * @author Francois Angot - 1997-07-22
 * @author Olivier Lezoray - 2007-05-10 (fix bug (sel>0))
 */

#include <pandore.h>
using namespace pandore;

/**
 * @file pmst.cpp		
 * Calcul de l'arbre de recouvrement.
 * Recherche de l'arbre de recouvrement minimal: Minimum Spanning Tree.
 * Utilisation des poids des arcs et algorithme de Prim.
 */
Errc PMst( const Graph2d &gs, Graph2d &gd ) {
   int	i,sel;
   GEdge *ptr;
   Double *poids;
   Double dist;
   Short *pred;
   Long	tmp;
  
   poids=new Double[gs.Size()];
   pred=new Short[gs.Size()];
  
   // Initialisation des tableaux.
   for (i=0;i<gs.Size();i++) {
      poids[i]=(Double)MAXLONG;
      pred[i]=-MAXSHORT;
   }
   
   while (true) {  
      // Selection d'un sommet de depart (poids>0 et pred<0).
      for (sel=gs.Size()-1;((sel>0) && (poids[sel]<0.0) && (pred[sel]>0)) || ((sel>0) && gs[sel]==NULL); sel--) ;
      if (sel<=0) {
	 break;
      }
      poids[sel]=(Double)-MAXLONG; // marquage
      
      while (true) {
	 // Selection du sommet le plus interessant.
	 tmp=0;
	 for (i=1;i<gs.Size();i++) {
	    if (gs[i] && (pred[i]<0) && (poids[i]<poids[tmp])) {
	       tmp=i;
	    }
	 }
	 // Calcul des poids des sommets voisins.
	 if (tmp<=0) break;
	 pred[tmp]=-pred[tmp];
	 for (ptr=gs[tmp]->Neighbours();ptr!=NULL;ptr=ptr->Next()) {
	    dist=ptr->weight;
	    if ((pred[ptr->Node()]<0)&&(dist<poids[ptr->Node()])) {
	       pred[ptr->Node()]=-(Short)tmp;
	       poids[ptr->Node()]=dist;
	    }
	 }
      }
   }
   
   gd=gs;
   // Construction de l'arbre de sortie.
   for (i=1;i<gs.Size();i++) {
      if ((gd[i])) {
	 gd[i]->value = gs[i]->value;
	 gd[i]->seed = gs[i]->seed;
	 for (ptr=gd[i]->Neighbours();ptr!=NULL;ptr=ptr->Next()) {
	    if (pred[ptr->Node()]!=i) {
	       gd.Unlink(i,ptr->Node());
	    }
	 }
 	 if ((pred[i]>0)&&(pred[i]!=MAXSHORT)) {
 	    gd.Link(i,pred[i],1);
	 }
      }
   }
   delete [] pred;
   delete [] poids;
  
   return SUCCESS;
}

Errc PMst( const Graph3d &gs, Graph3d &gd ) {
   int	i,sel;
   GEdge *ptr;
   Double *poids;
   Double dist;
   Short *pred;
   Long	tmp;
  
   poids=new Double[gs.Size()];
   pred=new Short[gs.Size()];
  
   // Initialisation des tableaux.
   for (i=0;i<gs.Size();i++) {
      poids[i]=(Double)MAXLONG;
      pred[i]=-MAXSHORT;
   }
   
   while (true) {  
      // Selection d'un sommet de depart (poids>0 et pred<0).
      for (sel=gs.Size()-1;((sel>0) && (poids[sel]<0) && (pred[sel]>0)) || (sel>0 && gs[sel]==NULL); sel--) ;
      if (sel<=0) break;
      poids[sel]=(Double)-MAXLONG; // marquage
      
      while (true) {
	 // Selection du sommet le plus interessant.
	 tmp=0;
	 for (i=1;i<gs.Size();i++) {
	    if (gs[i] && (pred[i]<0) && (poids[i]<poids[tmp])) {
	       tmp=i;
	    }
	 }
	 // Calcul des poids des sommets voisins.
	 if (tmp<=0) break;
	 pred[tmp]=-pred[tmp];
	 for (ptr=gs[tmp]->Neighbours();ptr!=NULL;ptr=ptr->Next()) {
	    dist=ptr->weight;
	    if ((pred[ptr->Node()]<0)&&(dist<poids[ptr->Node()])) {
	       pred[ptr->Node()]=-(Short)tmp;
	       poids[ptr->Node()]=dist;
	    }
	 }
      }
   }
   
   gd=gs;
   // Construction de l'arbre de sortie.
   for (i=1;i<gs.Size();i++) {
      if ((gd[i])) {
	 gd[i]->value = gs[i]->value;
	 gd[i]->seed = gs[i]->seed;
	 for (ptr=gd[i]->Neighbours();ptr!=NULL;ptr=ptr->Next())
	    if (pred[ptr->Node()]!=i) {
	       gd.Unlink(i,ptr->Node());
	    }
	 if ((pred[i]>0)&&(pred[i]!=MAXSHORT)) {
	    gd.Link(i,pred[i],1);
	 }
      }
   }
   delete [] pred;
   delete [] poids;
   
   return SUCCESS;
}


#ifdef MAIN

/*
 * Modify only the following constants, and the function call.
 */
#define	USAGE	"usage: %s [-m mask] [gr_in|-] [gr_out|-]"
#define	PARC	0
#define	FINC	1
#define	FOUTC	1
#define	MASK	1

int main( int argc, char *argv[] ) {
   Errc result;              // The result code of the execution.
   Pobject* mask;            // The region mask.
   Pobject* objin[FINC+1];   // The input objects.
   Pobject* objs[FINC+1];    // The source objects masked.
   Pobject* objout[FOUTC+1]; // The output object.
   Pobject* objd[FOUTC+1];   // The result object of the execution.
   char* parv[PARC+1];       // The input parameters.

   ReadArgs(argc,argv,PARC,FINC,FOUTC,&mask,objin,objs,objout,objd,parv,USAGE,MASK);

   switch(objs[0]->Type()) {
   case Po_Graph2d : {
      Graph2d* const gs=(Graph2d*)objs[0];
      objd[0]=new Graph2d(gs->Size(),gs->ImageSize());
      Graph2d* const gd=(Graph2d*)objd[0];

      result=PMst(*gs,*gd);
   }break;
   case Po_Graph3d : {
      Graph3d* const gs=(Graph3d*)objs[0];
      objd[0]=new Graph3d(gs->Size(),gs->ImageSize());
      Graph3d* const gd=(Graph3d*)objd[0];

      result=PMst(*gs,*gd);
   }break;
   default: {
      PrintErrorFormat(objin,FINC);
      result = FAILURE;
   }
   }
   
   if (result) {
      WriteArgs(argc,argv,PARC,FINC,FOUTC,&mask,objin,objs,objout,objd,MASK);
   }
   Exit(result);
   return 0;
}

#endif
