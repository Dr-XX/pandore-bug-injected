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
 * @author Abderrahim Elmoataz - 2003-01-08
 */

#include <math.h>
#include <pandore.h>
using namespace pandore;

/**
 * @file psig.cpp
 * Construction du graphe d'influence d'un graphe.
 */

static double Distancee( Point2d p1, Point2d p2 ) {
   return((double)((p1.x-p2.x)*(p1.x-p2.x) + (p1.y-p2.y)*(p1.y-p2.y)));
}

static double Distancee( Point3d p1, Point3d p2 ) {
   return((double)((p1.x-p2.x)*(p1.x-p2.x) + (p1.y-p2.y)*(p1.y-p2.y) + (p1.z-p2.x)*(p1.z-p2.z) ));
}

/*
 * Coupure des arcs pour lesquels les spheres
 * d'influence des sommets sont disjointes.
 */
Errc PSig( const Graph2d &gs, Graph2d &gd ) {
   int	i;
   GEdge *ptr;
   Ulong dist,mindist;
   gd=gs;
   
   // Calcul de la distance minimale entre noeuds voisins    
   for (i=0;i<gs.Size();i++) {
      if (!gs[i]) {
	 continue;
      }
      mindist=MAXULONG;
      for (ptr=gs[i]->Neighbours();ptr!=NULL;ptr=ptr->Next()) {
	 dist=(Ulong)sqrt(Distancee(gs[i]->seed,gs[ptr->Node()]->seed));
	 if (dist<mindist) {
	    mindist=dist;
	 }
      }
      gd[i]->value=(Graph2d::ValueType)mindist;	  
   }
   
   // Si la distance d'un noeud a un autre est > la somme
   // des valuations des 2 noeuds alors on coupe l'arcs.
   for (i=0;i<gs.Size();i++) {
      if ((gd[i])) {
	 for (ptr=gd[i]->Neighbours();ptr!=NULL;ptr=ptr->Next()) {
	    ptr->weight=1;
	    dist=(Ulong)sqrt((double)Distancee(gd[i]->seed,gd[ptr->Node()]->seed));
	    if (dist>(gd[i]->value+gd[ptr->Node()]->value)) {
	       ptr->weight=0;
	    }
	 }
      }
   }
   
   for (i=0;i<gd.Size();i++) {
      if ((gd[i])) {
	 gd[i]->value=1;
	 for (ptr=gd[i]->Neighbours();ptr!=NULL;ptr=ptr->Next()) {
	    if (ptr->weight==0) {
	       gd.Unlink(i,ptr->Node());
	    }
	 }
      }
   }
   
   return SUCCESS;
}

/*
 * Coupure des arcs pour lesquels les spheres d'influence des sommets
 * sont disjointes.
 */
Errc PSig( const Graph3d &gs, Graph3d &gd ) {
   int	i;
   GEdge *ptr;
   Ulong dist, mindist;
   
   gd=gs;
 
   // Calcul de la distance minimale entre noeuds voisins    
   for (i=0;i<gs.Size();i++) {
      if (!gs[i]) {
	 continue;
      }
      mindist=MAXULONG;
      for (ptr=gs[i]->Neighbours();ptr!=NULL;ptr=ptr->Next()) {
	 dist=(Ulong)sqrt(Distancee(gs[i]->seed,gs[ptr->Node()]->seed));
	 if (dist<mindist) {
	    mindist=dist;
	 }
      }
      gd[i]->value=(Graph3d::ValueType)mindist;	  
   }

   // Si la distance d'une noeud a un autre est > la somme
   // des valuations des 2 noeuds alors on coupe l'arcs.
   for (i=0;i<gs.Size();i++) {
      if ((gd[i])) {
	 for (ptr=gd[i]->Neighbours();ptr!=NULL;ptr=ptr->Next()) {
	    ptr->weight=1;
	    dist=(Ulong)sqrt((double)Distancee(gd[i]->seed,gd[ptr->Node()]->seed));
	    if (dist>(gs[i]->value+gs[ptr->Node()]->value)) {
	       ptr->weight=0;
	    }
	 }
      }
   }
   
   for (i=0;i<gs.Size();i++) {
      if ((gd[i])) {
	 gd[i]->value=1;
	 for (ptr=gd[i]->Neighbours();ptr!=NULL;ptr=ptr->Next()) {
	    if (ptr->weight==0) {
	       gd.Unlink(i,ptr->Node());
	    }
	 }
      }
   }
   
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
   case Po_Graph2d :{
      Graph2d* const gs=(Graph2d*)objs[0];
      objd[0]=new Graph2d(gs->Size());
      Graph2d* const gd=(Graph2d*)objd[0];

      result=PSig(*gs,*gd);
   }break;
   case Po_Graph3d :{
      Graph3d* const gs=(Graph3d*)objs[0];
      objd[0]=new Graph3d(gs->Size());
      Graph3d* const gd=(Graph3d*)objd[0];
      
      result=PSig(*gs,*gd);
   }break;
   default: {
      PrintErrorFormat(objin,FINC);
      result = FAILURE;
   }
   }
   
   if (result)
      WriteArgs(argc,argv,PARC,FINC,FOUTC,&mask,objin,objs,objout,objd,MASK);
   Exit(result);
   return 0;
}

#endif
