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
 * @author R‰gis Clouard - 2001-04-27 (version 3.00)
 * @author Regis Clouard - 2002-12-06 (Version 4.0)
 */

#include <pandore.h>
using namespace pandore;

/**
 * @file pnodevisu.cpp
 * Coloriage des regions avec les couleurs des sommets
 * du graphe ou la couleur du representant.
 */

Errc PNodeVisu( const Reg2d &rgs, const Graph2d &grs, Img2dsl &imd ) {
   int x,y;
   Ulong u;
   
   for (y=0;y<rgs.Height();y++) {
      for (x=0;x<rgs.Width();x++) {
	 if ((u=rgs[y][x]) && (grs[u]) && (grs[grs[u]->Item()])) {
	    imd[y][x]=(Long)grs[grs[u]->Item()]->value;
	 } else {
	    imd[y][x]=0;
	 }
      }
   }
   return SUCCESS;
}

Errc PNodeVisu( const Reg3d &rgs, const Graph3d &grs, Img3dsl &imd ) {
   int	x,y,z;
   Long	u;
   
   for (z=0;z<rgs.Depth();z++) {
      for (y=0;y<rgs.Height();y++) {
	 for (x=0;x<rgs.Width();x++) {
	    if ((u=rgs[z][y][x]) && (grs[u]) && (grs[grs[u]->Item()])) {
	       imd[z][y][x]=(Long)grs[grs[u]->Item()]->value;
	    } else {
	       imd[z][y][x]=0;
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
#define	MASK	0

int main( int argc, char *argv[] ) {
   Errc	 result;	     // The result code of the execution.
   Pobject* mask;	     // The region mask.
   Pobject* objin[FINC+1];   // The input objects.
   Pobject* objs[FINC+1];    // The source objects masked by the mask.
   Pobject* objout[FOUTC+1]; // The output object.
   Pobject* objd[FOUTC+1];   // The result object of the execution.
   char* parv[PARC+1];	     // The input parameters.
   
   ReadArgs(argc,argv,PARC,FINC,FOUTC,&mask,objin,objs,objout,objd,parv,USAGE,0);
   
   switch(objs[0]->Type()) {
   case Po_Reg2d :{
      Reg2d* const rgs=(Reg2d*)objs[0];
      Graph2d* const gs=(Graph2d*)objs[1];
      objd[0]=new Img2dsl(rgs->Size());
      Img2dsl* const imd=(Img2dsl*)objd[0];
      
      result=PNodeVisu(*rgs,*gs,*imd);
   }break;
   case Po_Reg3d :{
      Reg3d* const rgs=(Reg3d*)objs[0];
      Graph3d* const grs=(Graph3d*)objs[1];
      objd[0]=new Img3dsl(rgs->Size());
      Img3dsl* const imd=(Img3dsl*)objd[0];
      
      result=PNodeVisu(*rgs,*grs,*imd);
   }break;
   default :
      PrintErrorFormat(objin,FINC);
      result=FAILURE;
   }
   
   if (result)
      WriteArgs(argc,argv,PARC,FINC,FOUTC,&mask,objin,objs,objout,objd,MASK);
   
   Exit(result);
   return 0;
}

#endif
