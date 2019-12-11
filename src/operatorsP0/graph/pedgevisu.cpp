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
 * @author Christophe Deymier et Marianne Bedin - 2001-01-30
 * @author Regis Clouard - 2002-12-06 (Version 4.0)
 */

#include <pandore.h>
using namespace pandore;

/**
 * @file pedgevidsu.cpp
 * Visualisation (dans une image) des poids des aretes d'un graphe.
 * Contours des regions avec la valuation des arcs...
 */
Errc PEdgeVisu( const Reg2d &rgs, const Graph2d &gs, Img2dsl &imd ) {
   Long	x,y,k;
   Ulong u,v;
   GEdge *ptr;
   char vs;
   Point2d pt;
  
   for (y=0;y<rgs.Height();y++) {
      for (x=0;x<rgs.Width();x++) {
	 u=rgs[y][x];
	 vs=0;
      
	 for (k=0;k<8;k++) {
	    if ((rgs.Hold(y+v8y[k],x+v8x[k])) && ((v=rgs[y+v8y[k]][x+v8x[k]])!=u)) {
	       vs++;
	       pt=Point2d(y+v8y[k],x+v8x[k]);
	    }
	 }
	 if (vs) {
	    v=rgs[pt];
	    if (u && v && gs[u] && gs[v]) {
	       u=gs[u]->Item();
	       v=gs[v]->Item();
	       if ((u!=v) && (ptr=gs[u]->Search(v))) {
		  imd[y][x]=(Long)ptr->weight;
	       }
	    }
	 }	
      }
   }
   return SUCCESS;
}

Errc PEdgeVisu( const Reg3d &rgs, const Graph3d &gs, Img3dsl &imd ) {
   Long	x,y,z,k;
   Ulong u,v;
   GEdge *ptr;
   char vs;
   Point3d pt;
  
   for (z=0;z<rgs.Depth();z++) {
      for (y=0;y<rgs.Height();y++) {
	 for (x=0;x<rgs.Width();x++) {
	    u=rgs[z][y][x];
	    vs=0;
	   
	    for (k=0;k<26;k++) {
	       if ((rgs.Hold(z+v26z[z],y+v26y[k],x+v26x[k])) && ((v=rgs[z+v26z[k]][y+v26y[k]][x+v26x[k]])!=u)) {
		  vs++;
		  pt=Point3d(z+v26z[k],y+v26y[k],x+v26x[k]);
	       }
	    }
	    if (vs) {
	       v=rgs[pt];
	       if (u && v && gs[u] && gs[v]) {
		  u=gs[u]->Item();
		  v=gs[v]->Item();
		  if ((u!=v)&&(ptr=gs[u]->Search(v))) {
		     imd[z][y][x]=(Long)ptr->weight;
		  }
	       }
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
#define	USAGE	"usage: %s [rg_in|-] [gr_in|-] [im_out|-]"
#define	PARC	0
#define	FINC	2
#define	FOUTC	1
#define	MASK	2

int main( int argc, char *argv[] ) {
   Errc	 result;	    // The result code of the execution.
   Pobject* mask;	    // The region mask.
   Pobject* objin[FINC+1];   // The input objects.
   Pobject* objs[FINC+1];    // The source objects masked by the mask.
   Pobject* objout[FOUTC+1]; // The output object.
   Pobject* objd[FOUTC+1];   // The result object of the execution.
   char* parv[PARC+1];	    // The input parameters.

   ReadArgs(argc,argv,PARC,FINC,FOUTC,&mask,objin,objs,objout,objd,parv,USAGE,MASK);

   switch(objs[0]->Type()) {
   case Po_Reg2d :{
      Reg2d* const rgs=(Reg2d*)objs[0];
      Graph2d* const gs=(Graph2d*)objs[1];
      objd[0]=new Img2dsl(rgs->Size());
      Img2dsl* const imd=(Img2dsl*)objd[0];

      result=PEdgeVisu(*rgs,*gs,*imd);
   }break;
   case Po_Reg3d :{
      Reg3d* const rgs=(Reg3d*)objs[0];
      Graph3d* const gs=(Graph3d*)objs[1];
      objd[0]=new Img3dsl(rgs->Size());
      Img3dsl* const imd=(Img3dsl*)objd[0];

      result=PEdgeVisu(*rgs,*gs,*imd);
   }break;
   default :
      PrintErrorFormat(objin,FINC);
      result=FAILURE;
   }

   WriteArgs(argc,argv,PARC,FINC,FOUTC,&mask,objin,objs,objout,objd,MASK);

   Exit(result);
   return 0;
}

#endif
