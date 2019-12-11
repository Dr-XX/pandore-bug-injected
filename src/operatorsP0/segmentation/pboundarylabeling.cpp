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
 * @author Régis Clouard - 1999-02-19
 * @author Régis Clouard - May 12, 2012 (fixed bug with thick contours)
 */

#include <pandore.h>
using namespace pandore;

/**
 * @file pboundarylabeling.cpp
 * Etiquetage des regions d'une image de contours.
 */

/*
 * Parcourir de l'image ligne/ligne et marquer les "runs"
 * Parcourir ligne/ligne : pour chaque point prendre
 *	le min des voisins (!= 0).
 * Parcourir colonne/colonne : Pour chaque point prendre
 *	le min des voisins. (!=0).
 * eetiqueter l'image finale.
 */


 Errc PBoundaryLabeling( const Img2duc &ims, Reg2d &rgd ) {

    // Marquage des "runs" ligne par parcours causal
    Reg2d::ValueType etiquette = 0;
    Point2d pt;
    bool modif;
    for ( pt.y=0; pt.y<ims.Height(); pt.y++ ) {
       modif=true; // La fin de l'image coupe la ligne!
       for (pt.x=0; pt.x<ims.Width(); pt.x++) {
	  if (ims[pt] == 0) {
	     rgd[pt]=(modif)? (++etiquette) : etiquette;
	     modif=false;
	  } else {
	     rgd[pt]=0; // Marquer les contours a 0.
	     modif=true;
	  }
       }
    }

    Reg2d::ValueType nbreg=etiquette;

    // Regroupement suivant la 4-connexite, 
    // la 8 connexite n'est evidemment pas possible...
    while(1) {
       // Parcours causal. (connexite avec les voisins no.1 et no.2)
       modif = false;
       for (pt.y=1; pt.y<rgd.Height(); pt.y++) {
	  pt.x=0;				// Premier point.
	  if ((rgd[pt]>0) && (rgd[pt+v4[1]]>0)) {
	     rgd[pt]=rgd[pt+v4[1]];
	  }
	  for (pt.x=1; pt.x<rgd.Width(); pt.x++) {  // Autres points
	     if ((etiquette=rgd[pt])>0) {
		for (int k=0; k<2; k++) {
		   if ((rgd[pt+v4[k]]>0) && (rgd[pt+v4[k]]<etiquette)) {
		      etiquette = rgd[pt+v4[k]]; // Utiliser l'etiquette minimale des voisins.
		   }
		}
		if (rgd[pt] != etiquette) {
		   rgd[pt] = etiquette;
		   modif = true;
		}
	     }
	  }
       }

       if (!modif) break;
       // Parcours anti-causal. (connexite avec voisins 3 et 4).
       modif = false;
       for (pt.y=ims.Height()-2; pt.y>=0; pt.y--) {
	  // Dernier point.
	  pt.x = rgd.Width()-1;
	  if ((rgd[pt]>0) && (rgd[pt+v4[3]]>0)) {
	     rgd[pt]=rgd[pt+v4[3]];
	  }
	  for (pt.x=rgd.Width()-2; pt.x>=0; pt.x--) {
	     if ((etiquette=rgd[pt])>0) {
		for (int k=2; k<4; k++) {
		   if ((rgd[pt+v4[k]]>0.0) && (rgd[pt+v4[k]]<etiquette)) {
		      etiquette = rgd[pt+v4[k]];
		   }
		}
		if (rgd[pt] != etiquette) {
		   rgd[pt] = etiquette;
		   modif = true;
		}
	     }
	  }
       }
       if (!modif) break;
    }

    // ----------------------------------------
    // Minimisation des numeros d'etiquettes.
    // et etiquetage des contours (main gauche).
    //

     // Tableau indicé à l'envers (par couleur d'arrivée)
     // pour éviter une taille trop grande et vecteur vide.
    Ulong *colors=(Ulong*)calloc(nbreg+1, sizeof(Ulong)); // + init a 0.
    Fifo<Point2d> pendings;
    nbreg=0;
    for (pt.y=0; pt.y<ims.Height(); pt.y++) {
       for (pt.x=0; pt.x<ims.Width(); pt.x++) {

	  if ((etiquette = rgd[pt]) > 0) { // A label to minimize.
	     if (colors[etiquette]==0) {
		colors[etiquette] = ++nbreg;
	     }
	     rgd[pt] = colors[etiquette];
	  }
       }
    }

    // Label the contours.
    for (pt.y=0; pt.y<ims.Height(); pt.y++) {
       for (pt.x=0; pt.x<ims.Width(); pt.x++) {
	  
	  if (rgd[pt]==0) {
	     bool isolated=true;
	     for (int v=0; v<8; v++) {
		Point2d p1 = pt+v8[v];
		if (ims.Hold(p1) && ims[p1]==0) {
		   rgd[pt] = rgd[p1];
		   isolated=false;
		   break;
		}
	     }
	     if (isolated) {
		pendings.Push(pt);
	     }
	  }
       }
    }
    
    // Settle the boundary pixels.
    while (!pendings.Empty()) {
       pt=pendings.Pop();      // On dépile un point p.
       bool isolated=true;
       for (int v=0; v<8; v++) {
	  Point2d p1 = pt+v8[v];
	  if (rgd.Hold(p1) && rgd[p1]!=0) {
	     rgd[pt] = rgd[p1];
	     isolated=false;
	     break;
	  }
       }
       if (isolated) {
	  pendings.Push(pt);
       }
       
    }
    return rgd.Labels(nbreg);
}

 Errc PBoundaryLabeling( const Img2dsl &ims, Reg2d &rgd ) {

    // Marquage des "runs" ligne par parcours causal
    Reg2d::ValueType etiquette = 0;
    Point2d pt;
    bool modif;
    for ( pt.y=0; pt.y<ims.Height(); pt.y++ ) {
       modif=true; // La fin de l'image coupe la ligne!
       for (pt.x=0; pt.x<ims.Width(); pt.x++) {
	  if (ims[pt] == 0) {
	     rgd[pt]=(modif)? (++etiquette) : etiquette;
	     modif=false;
	  } else {
	     rgd[pt]=0; // Marquer les contours a 0.
	     modif=true;
	  }
       }
    }

    Reg2d::ValueType nbreg=etiquette;

    // Regroupement suivant la 4-connexite, 
    // la 8 connexite n'est evidemment pas possible...
    while(1) {
       // Parcours causal. (connexite avec les voisins no.1 et no.2)
       modif = false;
       for (pt.y=1; pt.y<rgd.Height(); pt.y++) {
	  pt.x=0;				// Premier point.
	  if ((rgd[pt]>0) && (rgd[pt+v4[1]]>0)) {
	     rgd[pt]=rgd[pt+v4[1]];
	  }
	  for (pt.x=1; pt.x<rgd.Width(); pt.x++) {  // Autres points
	     if ((etiquette=rgd[pt])>0) {
		for (int k=0; k<2; k++) {
		   if ((rgd[pt+v4[k]]>0) && (rgd[pt+v4[k]]<etiquette)) {
		      etiquette = rgd[pt+v4[k]]; // Utiliser l'etiquette minimale des voisins.
		   }
		}
		if (rgd[pt] != etiquette) {
		   rgd[pt] = etiquette;
		   modif = true;
		}
	     }
	  }
       }

       if (!modif) break;
       // Parcours anti-causal. (connexite avec voisins 3 et 4).
       modif = false;
       for (pt.y=ims.Height()-2; pt.y>=0; pt.y--) {
	  // Dernier point.
	  pt.x = rgd.Width()-1;
	  if ((rgd[pt]>0) && (rgd[pt+v4[3]]>0)) {
	     rgd[pt]=rgd[pt+v4[3]];
	  }
	  for (pt.x=rgd.Width()-2; pt.x>=0; pt.x--) {
	     if ((etiquette=rgd[pt])>0) {
		for (int k=2; k<4; k++) {
		   if ((rgd[pt+v4[k]]>0.0) && (rgd[pt+v4[k]]<etiquette)) {
		      etiquette = rgd[pt+v4[k]];
		   }
		}
		if (rgd[pt] != etiquette) {
		   rgd[pt] = etiquette;
		   modif = true;
		}
	     }
	  }
       }
       if (!modif) break;
    }

    // ----------------------------------------
    // Minimisation des numeros d'etiquettes.
    // et etiquetage des contours (main gauche).
    //

     // Tableau indicé à l'envers (par couleur d'arrivée)
     // pour éviter une taille trop grande et vecteur vide.
    Ulong *colors=(Ulong*)calloc(nbreg+1, sizeof(Ulong)); // + init a 0.
    Fifo<Point2d> pendings;
    nbreg=0;
    for (pt.y=0; pt.y<ims.Height(); pt.y++) {
       for (pt.x=0; pt.x<ims.Width(); pt.x++) {

	  if ((etiquette = rgd[pt]) > 0) { // A label to minimize.
	     if (colors[etiquette]==0) {
		colors[etiquette] = ++nbreg;
	     }
	     rgd[pt] = colors[etiquette];
	  }
       }
    }

    // Label the contours.
    for (pt.y=0; pt.y<ims.Height(); pt.y++) {
       for (pt.x=0; pt.x<ims.Width(); pt.x++) {
	  
	  if (rgd[pt]==0) {
	     bool isolated=true;
	     for (int v=0; v<8; v++) {
		Point2d p1 = pt+v8[v];
		if (ims.Hold(p1) && ims[p1]==0) {
		   rgd[pt] = rgd[p1];
		   isolated=false;
		   break;
		}
	     }
	     if (isolated) {
		pendings.Push(pt);
	     }
	  }
       }
    }
    
    // Settle the boundary pixels.
    while (!pendings.Empty()) {
       pt=pendings.Pop();      // On dépile un point p.
       bool isolated=true;
       for (int v=0; v<8; v++) {
	  Point2d p1 = pt+v8[v];
	  if (rgd.Hold(p1) && rgd[p1]!=0) {
	     rgd[pt] = rgd[p1];
	     isolated=false;
	     break;
	  }
       }
       if (isolated) {
	  pendings.Push(pt);
       }
       
    }
    return rgd.Labels(nbreg);
}

 Errc PBoundaryLabeling( const Img2dsf &ims, Reg2d &rgd ) {

    // Marquage des "runs" ligne par parcours causal
    Reg2d::ValueType etiquette = 0;
    Point2d pt;
    bool modif;
    for ( pt.y=0; pt.y<ims.Height(); pt.y++ ) {
       modif=true; // La fin de l'image coupe la ligne!
       for (pt.x=0; pt.x<ims.Width(); pt.x++) {
	  if (ims[pt] == 0) {
	     rgd[pt]=(modif)? (++etiquette) : etiquette;
	     modif=false;
	  } else {
	     rgd[pt]=0; // Marquer les contours a 0.
	     modif=true;
	  }
       }
    }

    Reg2d::ValueType nbreg=etiquette;

    // Regroupement suivant la 4-connexite, 
    // la 8 connexite n'est evidemment pas possible...
    while(1) {
       // Parcours causal. (connexite avec les voisins no.1 et no.2)
       modif = false;
       for (pt.y=1; pt.y<rgd.Height(); pt.y++) {
	  pt.x=0;				// Premier point.
	  if ((rgd[pt]>0) && (rgd[pt+v4[1]]>0)) {
	     rgd[pt]=rgd[pt+v4[1]];
	  }
	  for (pt.x=1; pt.x<rgd.Width(); pt.x++) {  // Autres points
	     if ((etiquette=rgd[pt])>0) {
		for (int k=0; k<2; k++) {
		   if ((rgd[pt+v4[k]]>0) && (rgd[pt+v4[k]]<etiquette)) {
		      etiquette = rgd[pt+v4[k]]; // Utiliser l'etiquette minimale des voisins.
		   }
		}
		if (rgd[pt] != etiquette) {
		   rgd[pt] = etiquette;
		   modif = true;
		}
	     }
	  }
       }

       if (!modif) break;
       // Parcours anti-causal. (connexite avec voisins 3 et 4).
       modif = false;
       for (pt.y=ims.Height()-2; pt.y>=0; pt.y--) {
	  // Dernier point.
	  pt.x = rgd.Width()-1;
	  if ((rgd[pt]>0) && (rgd[pt+v4[3]]>0)) {
	     rgd[pt]=rgd[pt+v4[3]];
	  }
	  for (pt.x=rgd.Width()-2; pt.x>=0; pt.x--) {
	     if ((etiquette=rgd[pt])>0) {
		for (int k=2; k<4; k++) {
		   if ((rgd[pt+v4[k]]>0.0) && (rgd[pt+v4[k]]<etiquette)) {
		      etiquette = rgd[pt+v4[k]];
		   }
		}
		if (rgd[pt] != etiquette) {
		   rgd[pt] = etiquette;
		   modif = true;
		}
	     }
	  }
       }
       if (!modif) break;
    }

    // ----------------------------------------
    // Minimisation des numeros d'etiquettes.
    // et etiquetage des contours (main gauche).
    //

     // Tableau indicé à l'envers (par couleur d'arrivée)
     // pour éviter une taille trop grande et vecteur vide.
    Ulong *colors=(Ulong*)calloc(nbreg+1, sizeof(Ulong)); // + init a 0.
    Fifo<Point2d> pendings;
    nbreg=0;
    for (pt.y=0; pt.y<ims.Height(); pt.y++) {
       for (pt.x=0; pt.x<ims.Width(); pt.x++) {

	  if ((etiquette = rgd[pt]) > 0) { // A label to minimize.
	     if (colors[etiquette]==0) {
		colors[etiquette] = ++nbreg;
	     }
	     rgd[pt] = colors[etiquette];
	  }
       }
    }

    // Label the contours.
    for (pt.y=0; pt.y<ims.Height(); pt.y++) {
       for (pt.x=0; pt.x<ims.Width(); pt.x++) {
	  
	  if (rgd[pt]==0) {
	     bool isolated=true;
	     for (int v=0; v<8; v++) {
		Point2d p1 = pt+v8[v];
		if (ims.Hold(p1) && ims[p1]==0) {
		   rgd[pt] = rgd[p1];
		   isolated=false;
		   break;
		}
	     }
	     if (isolated) {
		pendings.Push(pt);
	     }
	  }
       }
    }
    
    // Settle the boundary pixels.
    while (!pendings.Empty()) {
       pt=pendings.Pop();      // On dépile un point p.
       bool isolated=true;
       for (int v=0; v<8; v++) {
	  Point2d p1 = pt+v8[v];
	  if (rgd.Hold(p1) && rgd[p1]!=0) {
	     rgd[pt] = rgd[p1];
	     isolated=false;
	     break;
	  }
       }
       if (isolated) {
	  pendings.Push(pt);
       }
       
    }
    return rgd.Labels(nbreg);
}


#ifdef MAIN
#define	USAGE	"usage: %s [-m mask] [im_in|-] [rg_out|-]"
#define	PARC	0
#define	FINC	1
#define	FOUTC	1
#define	MASK	1

int main( int argc, char *argv[] ) {
   Errc result;                // The result code of the execution.
   Pobject* mask;              // The region map.
   Pobject* objin[FINC + 1];   // The input objects.
   Pobject* objs[FINC + 1];    // The source objects masked.
   Pobject* objout[FOUTC + 1]; // The output object.
   Pobject* objd[FOUTC + 1];   // The result object of the execution.
   char* parv[PARC + 1];       // The input parameters.

   ReadArgs(argc, argv, PARC, FINC, FOUTC, &mask, objin, objs, objout, objd, parv, USAGE, MASK); 
   if (objs[0]->Type() == Po_Img2duc) {
      Img2duc* const ims = (Img2duc*)objs[0];
      objd[0] = new Reg2d(ims->Size());
      Reg2d* const rgd = (Reg2d*)objd[0];
      
      result = PBoundaryLabeling(*ims, *rgd);
      goto end;
   }
   if (objs[0]->Type() == Po_Img2dsl) {
      Img2dsl* const ims = (Img2dsl*)objs[0];
      objd[0] = new Reg2d(ims->Size());
      Reg2d* const rgd = (Reg2d*)objd[0];
      
      result = PBoundaryLabeling(*ims, *rgd);
      goto end;
   }
   if (objs[0]->Type() == Po_Img2dsf) {
      Img2dsf* const ims = (Img2dsf*)objs[0];
      objd[0] = new Reg2d(ims->Size());
      Reg2d* const rgd = (Reg2d*)objd[0];
      
      result = PBoundaryLabeling(*ims, *rgd);
      goto end;
   }
  {
     PrintErrorFormat(objin, FINC, argv); 
     result = FAILURE; 
  }	

end:
  if (result) {
	WriteArgs(argc, argv, PARC, FINC, FOUTC, &mask, objin, objs, objout, objd, MASK); 
  }
  Exit(result); 
  return 0; 
}
#endif
