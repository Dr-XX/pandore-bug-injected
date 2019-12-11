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
 * @author Régis Clouard - 1997-07-22
 */

#include <pandore.h>
using namespace pandore;

/**
 * @file pcontourextensionrect.cpp 
 * @brief Extension des points terminaux.
 *
 * Parametres : (dh, dl) taille de l'extension (hauteur,largeur).
 * Consult    : Voisinage.
 * Fonction   : Si le point est un point terminal (1 voisin)
 *		etendre ce point avec un rectangle de dl,dh
 *		points dans le sens de la poursuite.
 */
Errc PContourExtensionRect( const Img2duc &ims, Img2duc &imd, int  dh, int dl ) {
   Point2d pt,pt1,pt2;
   int k,l;
   int dirext;
   Ulong nb=0;

   imd = ims;
   imd.Frame(ims,1,1); // Recopie des bords de l'image source dans l'image destination
   
   for (pt.y=1;pt.y<ims.Height()-1; pt.y++) {
      for (pt.x=1;pt.x<ims.Width()-1; pt.x++) {
	 if (!ims[pt]) {
	    continue;
	 }
	 // Recherche d'un point terminal
	 dirext =0;
	 for (int v=0; v<8; v++) {
	    if (ims[pt+v8[v]]) {
	       if (dirext) {
		  dirext = 0; // plus d'1 voisin donc pas terminal.
		  break;
	       } else {
		  dirext = v+4; // +4 pour eviter la valeur 0.
	       }
	    }
	 }
	 
	 if (dirext) { // point terminal de direction dirext
	    nb++;
	    pt1=pt;
	    dirext=dirext%8;
	    for (k=0; k<(dh+1)/2; k++) {
	       pt2 = pt1;
	       for (l=0;l<dl;l++) {
		  if (imd.Hold(pt2)) {
		     imd[pt2] = MAXUCHAR/2;
		  }
		  pt2 = pt2+v8[dirext];
	       }
	       pt1 = pt1 + v8[((dirext/2)*2+2)%8];
	    }
	    pt1=pt;
	    for (k=0; k< (dh+1)/2; k++) {
	       pt2=pt1;
	       for (l=0; l < dl; l++) {
		  if (imd.Hold(pt2)) {
		     imd[pt2] = MAXUCHAR/2;
		  }
		  pt2 = pt2+v8[dirext];
	       }
	       pt1 = pt1 + v8[(((dirext+4)/2)*2+2)%8];
	    }
	 }
      }
   }
   return nb;
}

/*
 * Parametres : (dh, dl) taille de l'extension (hauteur,largeur).
 * Consult    : Voisinage.
 * Fonction   : Si le point est un point terminal (1 voisin)
 *		etendre ce point avec un rectangle de dl,dh
 *		points dans le sens de la poursuite.
 */
Errc PContourExtensionRect( const Img3duc &ims, Img3duc &imd, int /*dh*/ , int dl ) {
   Point3d pt,pt1,pt2;
   int dirext;
   Ulong nb=0;
   
   imd = ims;
   imd.Frame(ims,1,1,1); // Recopie des bords de l'image source dans l'image destination
   
   for (pt.z=1;pt.z<ims.Depth()-1; pt.z++)
      for (pt.y=1;pt.y<ims.Height()-1; pt.y++)
	 for (pt.x=1;pt.x<ims.Width()-1; pt.x++) {
	    if (!ims[pt]) {
	       continue;
	    }
	    // Recherche d'un point terminal
	    // dirext = direction du contour au point terminal
	    dirext =0;
	    for (int v=0; v<26; v++) {
	       if (ims[pt+v26[v]]) {
		  if (dirext) {
		     dirext = -1; // plus d'1 voisin donc pas terminal.
		     break;
		  } else {
 		     dirext = (v+13)%26; // -> direction = opposee.
		  }
	       }
	    }	    
	    if (dirext>0) { // point terminal de direction dirext
	       nb++;
	       pt1=pt+v26[dirext]+v26[dirext];
	       for (int l=0; l<dl; l++) {
		  for (int v=0; v< 26; v++) {
		     pt2 = pt1+v26[dirext];
		     if (imd.Hold(pt2)) {
			imd[pt2]=MAXUCHAR/2;
		     }
		  }
		  // Faire un cube de dl*dh*dh autour de pt2.
		  pt1 = pt1+v26[dirext];
	       }
	    }
	 }
   return nb;
}

#ifdef MAIN

/*
 * Modify only the following constants, and the function call.
 */
#define	USAGE	"usage: %s width height [-m mask] [im_in|-] [im_out|-]"
#define	PARC	2
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
      result=PContourExtensionRect(*ims,*imd,atoi(parv[1]),atoi(parv[0]));
   }break;
   case Po_Img3duc : {
      Img3duc* const ims=(Img3duc*)objs[0];
      objd[0]=new Img3duc(ims->Props());
      Img3duc* const imd=(Img3duc*)objd[0];

      result=PContourExtensionRect(*ims,*imd,atoi(parv[1]),atoi(parv[0]));
   }break;
   default :
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

