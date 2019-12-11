/* -*- c-basic-offset: 3; mode: c++ -*-
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
 * @author Stéphanie Jéhan Besson - 2006-01-31
 * @author Guillaume Née - 2006-01-31
 * @author Yohann Pitrey Helpiquet - 2006-01-31
 * @author Guillaume Née - 2007-01-17 revision add parameter seuil_ssd_min
 */

#include <limits>
#include <math.h>
#include <pandore.h>
//#include <pviewer.h>
using namespace pandore;

#include <iostream>
using namespace std;

/**
 * @file pblockmatching.cpp
 *
 * Motion estimation algorithm using block matching.
 */


double MAX = std::numeric_limits<double>::max();

/*
 * @brief Calculates the Sum of Square Differences between 2 blocs.
 *
 * Calculates the Sum of Square Differences between a reference
 * block and block translated with vector (u.v).
 *
 * @param im_in_1 reference image.
 * @param im_in_2 destination image.
 *
 * @param xd, yd upper left coordinate of the reference block.
 * @param taillebloc size of the block.
 * @param u translation vector component.
 * @param v translation vector component.
 *
 * @return THE SSD VALUE.
 */
double SSD(const Img2duc &im_in_1, const Img2duc &im_in_2,
	   int xd, int yd, short taillebloc, int u, int v ) {

   long somme = 0;
   int cpt = 0;

   for (int x=xd; x<(xd+taillebloc); x++) {
      for (int y=yd; y<(yd+taillebloc); y++) {
	 if (im_in_1.Hold(y,x) && im_in_2.Hold(y+v,x+u)) {
	    cpt++;
	    long int tmp = (im_in_1[y][x] - im_in_2[y+v][x+u]);
	    somme += (tmp * tmp);
	 }
      }
   }
   return ((cpt > ((taillebloc * taillebloc)/4))? somme/cpt : MAX);
}

/**
 * Performs the block matching.
 *
 * @param im_in_1 image de référence
 * @param im_in_2 image de destination
 * @param im_out_deplacements image multispectrale de sortie representant les vecteurs deplacements par pixel (band 0 : abscisse, band 1 : ordonnee)
 * @param taillebloc taille des blocs
 * @param dmax taille du voisinage considere pour la recherche des blocs
 * @param seuil_ssd_min borne inferieure de la ssd
 */
Errc pblockmatching( const Img2duc &im_in_1, const Img2duc &im_in_2,
		     Imx2dsf &im_out_deplacements,
		     short taillebloc,
		     short dmax,
		     short seuil_ssd_min) {

   int width = im_in_1.Width();
   int height = im_in_2.Height();

   Img2duc imgview(height,width);
//    Viewer disp(imgview,"Blockmatching Viewer",0);

   /* Parcours sur les sous-blocs de l'image */ 
   for (int xbloc=0; xbloc<width; xbloc+=taillebloc) {
      for (int ybloc=0; ybloc<height; ybloc+=taillebloc) {
	 
	 /* calcul de la SSD min du bloc: */
	 // initialisations
	 double ssdmin = std::numeric_limits<double>::max();
	 double ssd00 = 0;
	 int umin = 0;
	 int vmin = 0;

	 // parcours de tous les deplacements pour trouver la SSD minimale
	 for (int u=-dmax; u<=dmax; u++) {
	    for (int v=-dmax; v<=dmax; v++) {
	       double ssd = SSD(im_in_1, im_in_2, xbloc, ybloc, taillebloc, u, v);
	       if (u==0 && v==0) 
		  ssd00=ssd;
	       if ( ssd < ssdmin ) {
		  ssdmin = ssd;
		  umin = u;
		  vmin = v;
	       }
	    }
	 }

	 // affichage de la trace de l'algorithme
	 /* cout << "ssdmin: " << ssdmin << endl
	    << "umin: " << umin << endl
	    << "vmin: " << vmin << endl
	    << endl; */

	 // If SSDmin is lesser than the threshold
	 // then do not consider the motion.
	 if (ssd00 - ssdmin <= seuil_ssd_min) {
	    for (int x=xbloc; x<(xbloc+taillebloc); x++) {
	       for (int y=ybloc; y<(ybloc+taillebloc); y++) {
		  if (im_in_1.Hold(y,x)) { // pour plus de securite
		     im_out_deplacements[0][y][x] = 0;
		     im_out_deplacements[1][y][x] = 0;
		     imgview[y][x]=im_in_1[y][x];
		  }
	       }
	    }
	 } else {	 // else consider the motion
	    for (int x=xbloc; x<(xbloc+taillebloc); x++) {
	       for (int y=ybloc; y<(ybloc+taillebloc); y++) {
		  if (im_in_1.Hold(y,x)) {
		     im_out_deplacements[0][y][x] = Float(umin);
		     im_out_deplacements[1][y][x] = Float(vmin);
		     if (imgview.Hold(y+vmin,x+umin))
			imgview[y+vmin][x+umin]=im_in_1[y][x];
		  }
	       }
	    }
	 }
// 	 disp.display(imgview);
      }// fin boucle sur y
   }//fin boucle sur x

//    while (!disp.key)
//       disp.wait();

   return SUCCESS;
}

#ifdef MAIN
#define	USAGE	"usage: %s block_size search_size ssd_min im_in_ref im_in_dest im_out_dep"
#define	PARC	3
#define	FINC	2
#define	FOUTC	1
#define	MASK	2 // only masking

int main( int argc, char *argv[] ) {
   Errc result;              // The result code of the execution.
   Pobject* mask;            // The region map.
   Pobject* objin[FINC+1];   // The input objects.
   Pobject* objs[FINC+1];    // The source objects masked.
   Pobject* objout[FOUTC+1]; // The output object.
   Pobject* objd[FOUTC+1];   // The result object of the execution.
   char* parv[PARC+1];       // The input parameters.

   ReadArgs(argc,argv,PARC,FINC,FOUTC,&mask,objin,objs,objout,objd,parv,USAGE,MASK);
   switch (objs[0]->Type()) {
   case Po_Img2duc: {
      Img2duc* const im_in_1=(Img2duc*)objs[0];
      Img2duc* const im_in_2=(Img2duc*)objs[1];

      /* Récupération des paramètres */
      short taillebloc = (short)atoi(parv[0]);
      short dmax = (short)atoi(parv[1]);
      short seuil_ssd_min = (short)atoi(parv[2]);

      objd[0]=new Imx2dsf(2, im_in_1->Height(), im_in_1->Width());
      Imx2dsf* const im_out_deplacements=(Imx2dsf*)objd[0];

      result=pblockmatching(*im_in_1,*im_in_2,
			    *im_out_deplacements, 
			    taillebloc,
			    dmax,
			    seuil_ssd_min);
      break;
   }
   default:
      PrintErrorFormat(objin,FINC);
      result=FAILURE;
   }
  
   if (result) WriteArgs(argc,argv,PARC,FINC,FOUTC,&mask,objin,objs,objout,objd,MASK);
   
   Exit(result);
   return 0;
}
#endif
