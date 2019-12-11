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
 * @author Regis Clouard - 2001-06-07
 */

#include <pandore.h>
using namespace pandore;

/**
 * @file pedgeclosing.cpp
 * Fermeture de contours a partir de l'image d'amplitude du gradient.
 *
 * Parametres:	voisinage
 *		Longueur maximale de contours manquants.
 * Consult:	Par voisinage.
 * Fonction:	Marquer les bords de l'image comme contours.
 *		Suppression des points isoles.
 *		Pour chaque pixel terminal de contours
 *		Recherche des trois directions de poursuite.
 *		       Choix de la direction
 *			Maximum  du gradient si anticipation =1
 *			Critere de maximisation sur anticipation pixels.
 */

Errc PEdgeClosing( const Img2duc &ims, Img2duc &ima, Img2duc &imd, int angle, int longueur ) {
   Point2d pt,pt1;
   Fifo<Point2d> f;
   int v,w=0;
   Ulong nbpt=0;
   
   if ((angle > 2) || (angle < 0)) {
      std::cerr << "Error pedgeclosing: Bad parameter value for angle: " << angle << std::endl;
      return FAILURE;
   }
   imd=ims; // On recupere tous les contours.
   imd.Frame(MAXUCHAR,1,1); // Ferme l'image par un contour.
   
   // PEdgeClosing.
   for (pt.y=1;pt.y<imd.Height()-1; pt.y++) {
      for (pt.x=1;pt.x<imd.Width()-1; pt.x++) {

	 if ((imd[pt])) {
	    // Recherche d'un point terminal en comptant le nombre de voisins.
	    int nvois=0;
	    for (v=0;v<8;v++) {
	       if (imd[pt+v8[v]]) {
		  nvois++;
		  w = (v+4)%8; // w donne la direction du contour.
	       }
	    }
	    
	    if (nvois == 1) {	// point terminal.
	       pt1=pt;
	       // Rechercher un autre contour eloigne au max de longueur.
	       for (int l=0;l<longueur;l++) {
		  int max = w; // direction priviligee en cas de non maxima.
		  // Recherche du gradient maximum parmi les voisins autorises.
		  for (v=8-angle;v<=8+angle;v++) {
		     if (ima[pt1+v8[max]]<ima[pt1+v8[(w+v)%8]]) {
			max = (w+v)%8;
		     }
		  }
		  pt1 = pt1+v8[max];
		  
		  if (imd[pt1]>0) { // Retour a un contour -> contour fermé.
		     while(!f.Empty()) { // On marque le nouveau contour.
			pt1=f.Pop();
			imd[pt1] = MAXUCHAR ;
		     }
		     nbpt++;
		     break;
		  }
		  if (ima[pt1]==0) // Plus de gradient: on arrete.
		     break;
		  
		  //  Toujours pas de contour: on continue.
		  f.Push(pt1);
		  w = max;
	       }
	       // Trop loin.
	       while(!f.Empty()) {
		  f.Pop();
	       }
	    }
	 }
      }
   }
   return nbpt;
}



Errc PEdgeClosing( const Img2duc &ims, Img2dsl &ima, Img2duc &imd, int angle, int longueur ) {
   Point2d pt,pt1;
   Fifo<Point2d> f;
   int v,w=0;
   Ulong nbpt=0;
   
   if ((angle > 2) || (angle < 0)) {
      std::cerr << "Error pedgeclosing: Bad parameter value for angle: " << angle << std::endl;
      return FAILURE;
   }
   imd=ims; // On recupere tous les contours.
   imd.Frame(MAXUCHAR,1,1); // Ferme l'image par un contour.
   
   // PEdgeClosing.
   for (pt.y=1;pt.y<imd.Height()-1; pt.y++) {
      for (pt.x=1;pt.x<imd.Width()-1; pt.x++) {

	 if ((imd[pt])) {
	    // Recherche d'un point terminal en comptant le nombre de voisins.
	    int nvois=0;
	    for (v=0;v<8;v++) {
	       if (imd[pt+v8[v]]) {
		  nvois++;
		  w = (v+4)%8; // w donne la direction du contour.
	       }
	    }
	    
	    if (nvois == 1) {	// point terminal.
	       pt1=pt;
	       // Rechercher un autre contour eloigne au max de longueur.
	       for (int l=0;l<longueur;l++) {
		  int max = w; // direction priviligee en cas de non maxima.
		  // Recherche du gradient maximum parmi les voisins autorises.
		  for (v=8-angle;v<=8+angle;v++) {
		     if (ima[pt1+v8[max]]<ima[pt1+v8[(w+v)%8]]) {
			max = (w+v)%8;
		     }
		  }
		  pt1 = pt1+v8[max];
		  
		  if (imd[pt1]>0) { // Retour a un contour -> contour fermé.
		     while(!f.Empty()) { // On marque le nouveau contour.
			pt1=f.Pop();
			imd[pt1] = MAXUCHAR ;
		     }
		     nbpt++;
		     break;
		  }
		  if (ima[pt1]==0) // Plus de gradient: on arrete.
		     break;
		  
		  //  Toujours pas de contour: on continue.
		  f.Push(pt1);
		  w = max;
	       }
	       // Trop loin.
	       while(!f.Empty()) {
		  f.Pop();
	       }
	    }
	 }
      }
   }
   return nbpt;
}




#ifdef MAIN
#define USAGE	"usage: %s angle length [-m mask] [im_in|-] [im_amp|-] [im_out|-]"
#define PARC	2
#define FINC	2
#define FOUTC	1
#define MASK	1

int main( int argc, char *argv[] ) {
   Errc result;                // The result code of the execution.
   Pobject* mask;              // The region map.
   Pobject* objin[FINC + 1];   // The input objects.
   Pobject* objs[FINC + 1];    // The source objects masked.
   Pobject* objout[FOUTC + 1]; // The output object.
   Pobject* objd[FOUTC + 1];   // The result object of the execution.
   char* parv[PARC + 1];       // The input parameters.

   ReadArgs(argc, argv, PARC, FINC, FOUTC, &mask, objin, objs, objout, objd, parv, USAGE, MASK); 
   if ((objs[0]->Type() == Po_Img2duc) && (objs[1]->Type() == Po_Img2duc)) {
      Img2duc* const ims1=(Img2duc*)objs[0];
      Img2duc* const ims2=(Img2duc*)objs[1];
      objd[0]=new Img2duc(ims1->Size());
      Img2duc* const imd=(Img2duc*)objd[0];
      
      result=PEdgeClosing(*ims1,*ims2,*imd,atoi(parv[0]),atoi(parv[1]));
      goto end;
   }
   if ((objs[0]->Type() == Po_Img2duc) && (objs[1]->Type() == Po_Img2dsl)) {
      Img2duc* const ims1=(Img2duc*)objs[0];
      Img2dsl* const ims2=(Img2dsl*)objs[1];
      objd[0]=new Img2duc(ims1->Size());
      Img2duc* const imd=(Img2duc*)objd[0];
      
      result=PEdgeClosing(*ims1,*ims2,*imd,atoi(parv[0]),atoi(parv[1]));
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
