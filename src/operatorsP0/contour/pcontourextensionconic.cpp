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
 * @author Arnaud Renouf - 2004-09-10
 * @author régis Clouard - May 14, 2012 (enhance diagonal direction.)
 */

#include <pandore.h>
using namespace pandore;

/**
 * @file pcontourextensionconic.cpp
 * @brief Cone-shaped extension of end-points.
 */

#define NB_VOISIN_ORTHO 15

/*
 * Parametres : dl taille de l'extension en longueur,
 * Consult    : Voisinage,
 * Fonction   : Si le point est un point terminal (1 voisin)
 *		etendre ce point avec un cone de longueur dl
 *		points dans le sens de la poursuite.
 */
Errc PContourExtensionConic( const Img2duc &ims, Img2duc &imd, int dl ) {
   Ulong nb=0;
   
   imd = ims;
   Point2d pt;
   for (pt.y=1; pt.y<ims.Height()-1; pt.y++) {
      for (pt.x=1; pt.x<ims.Width()-1; pt.x++) {
	 if (!ims[pt]) {
	    continue;
	 }
	 // Search for an end-point (a point with only 1 neighbour).
	 Uchar dirext = 0;
	 for (int v=0; v<8; v++) {
	    if (ims[pt+v8[v]]) {
	       if ((dirext)) {
		  dirext = 0;   // Not an end-point
		  break;
	       } else {
		  dirext = v+4; // +4 to avoid value 0 (not set).
	       }
	    }
	 }

	 if ((dirext)) { // end-point
	    Point2d pt1=pt;
	    dirext=dirext%8;
	    nb++;
	    
	    // Vertical or horizontal direction
	    if ( (dirext%2) == 0) {
	       for (int k=0; k<dl; k++) {
		  pt1 = pt1+v8[dirext]; //+v8[(dirext-2)%8];
		  if (!imd.Hold(pt1)) continue;
		  imd[pt1] = MAXUCHAR/2;
		  Point2d pt2 = pt1;
		  Point2d pt3 = pt1;
		  for (int l=0; l<k+1; l++) {
		     pt2 = pt2+v8[(dirext+2)%8];
		     if (imd.Hold(pt2)) {
			imd[pt2] = MAXUCHAR/2;
		     }
		     pt3 = pt3+v8[(dirext+6)%8];
		     if (imd.Hold(pt3)) {
			imd[pt3] = MAXUCHAR/2;
		     }
		  }
	       }
	    } else {  // diagonal direction
	       for (int k=0; k<dl; k++) {
		  pt1=pt1+v8[(dirext+7)%8];
		  if (!imd.Hold(pt1)) continue;
		  Point2d pt2= pt1;
		  for (int l=0;l<2+k;l++) {
		     if (imd.Hold(pt2)) {
			imd[pt2] = MAXUCHAR/2;
		     }
		     pt2 = pt2+v8[(dirext+2)%8];
		  }
	       }
 	    }
 	 }
      }
   }
   return nb;
}


// Defines masks that are perpendicular to a direction
// (the direction of the terminal point)
Uchar voisin_ortho[26][NB_VOISIN_ORTHO]={{9,11,22,24,255},
					 {18,5,24,11,13,22,255},
					 {22,13,24,255},
					 {9,22,24,7,13,20,255},
					 {13,9,24,255},
					 {1,11,24,9,13,14,255},
					 {13,9,11,255},
					 {11,3,13,16,9,22,255},
					 {13,22,11,255},
					 {11,13,24,0,255},
					 {13,25,24,12,22,0,255},
					 {13,9,22,0,255},
					 {9,10,23,24,0,13,255},
					 {9,11,22,24,255},
					 {18,5,24,11,0,9,255},
					 {9,11,0,255},
					 {0,7,11,20,9,22,255},
					 {22,11,0,255},
					 {1,11,24,14,22,0,255},
					 {0,24,22,255},
					 {9,22,24,16,0,3,255},
					 {9,24,0,255},
					 {11,13,24,0,255},
					 {9,25,11,12,0,13,255},
					 {13,9,22,0,255},
					 {10,11,22,23,0,13,255}};

static Errc remplir( Img3duc &imd, Point3d pt, int iter, Uchar dir ) {
   Point3d pt1;
   pt+=v26[dir];
   if (imd.Hold(pt)) {
      imd[pt]=MAXUCHAR;

      for (int i=0;i<NB_VOISIN_ORTHO;i++) {
	 if (voisin_ortho[dir][i]!=255) {
	    pt1=pt+v26[voisin_ortho[dir][i]];
	    if (imd.Hold(pt1)) {
	       imd[pt1]=MAXUCHAR;
	       if (iter>1) {
		  remplir(imd,pt1,iter-1,dir);
	       }
	    }
	 }
	 else 
	    break;
      }
   }
   return SUCCESS;
}

/*
 * Parametres : dl taille de l'extension en longueur,
 * Consult    : Voisinage
 * Fonction   : Si le point est un point terminal (1 voisin)
 *		etendre ce point avec un cone de longueur dl
 *		points dans le sens de la poursuite.
 */
Errc PContourExtensionConic( const Img3duc &ims, Img3duc &imd, int dl ) {
   Point3d pt;
   Uchar dirext;
  
   Ulong nb=0;
   
   imd = ims;
   
   for (pt.z=1; pt.z<ims.Depth()-1; pt.z++)
      for (pt.y=1; pt.y<ims.Height()-1; pt.y++)
	 for (pt.x=1; pt.x<ims.Width()-1; pt.x++) {
	    if (!ims[pt])
	       continue;
	    // Recherche d'un point terminal
	    dirext =0;
	       
	    for (int v=0; v<26; v++) {		  
	       if (ims[pt+v26[v]]) {
		  if (dirext) {
		     dirext = 0; // plus d'1 voisin donc pas terminal.
		     break;
		  }else
		     dirext = v+13; // -> direction = opposee.
	       }
	    }
	    if (dirext) { // point terminal de direction dirext
	       nb++;
	       dirext%=26;
	       remplir(imd,pt,dl,dirext);
	    }
	 }
   return nb;
}

#ifdef MAIN

/*
 * Modify only the following constants, and the function call.
 */
#define	USAGE	"usage: %s length [-m mask] [im_in|-] [im_out|-]"
#define	PARC	1
#define	FINC	1
#define	FOUTC	1
#define	MASK	1

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
   case Po_Img2duc :{
      Img2duc* const ims=(Img2duc*)objs[0];
      objd[0]=new Img2duc(ims->Props());
      Img2duc* const imd=(Img2duc*)objd[0];
      
      result=PContourExtensionConic(*ims,*imd,atoi(parv[0]));
   }break;
   case Po_Img3duc :{
      Img3duc* const ims=(Img3duc*)objs[0];
      objd[0]=new Img3duc(ims->Props());
      Img3duc* const imd=(Img3duc*)objd[0];
      
      result=PContourExtensionConic(*ims,*imd,atoi(parv[0]));
   }break;
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
