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
 * @author Carlotti & Joguet (ENSEA) - 2000-05-15
 * @author Régis Clouard - 2002-12-26
 * @author Régis Clouard - 2006-06-22 (fix bugs)
 */

#include <math.h>
#include <pandore.h>
using namespace pandore;

/**
 * @file pshen.cpp
 * LISSAGE CAUSAL et ANTICAUSAL HORIZONTAL et VERTICAL d'une image par SHEN
 * GRADIENT avec EXTRACTION des maximas locaux et ETIREMENT.
 * (en flottant) parametre alpha dans l'USAGE.
 */

Errc PShen( const Img2duc &ims, Img2duc &imd, Float alpha ) {
   const float g = (float)exp(-alpha);
  
   Img2dsf inter(ims.Props());

   // Lissage horizontal
   for (int y = 0; y < ims.Height(); y++) {
      float s1 = (float)ims(y, ims.Width() - 1); // point de depart de l'initialisation de la ligne pour le lissage causal
      for (int x = ims.Width() - 2 ; x >= 0; x--) { // On duplique le bord.
	 s1 = (1 - g) * ims(y, x) + g * s1; // initialisation (lissage anticausal)
      }
      inter(y, 0) = s1;	// recopie du point d'initialisation dans la premiere colonne.
      
      for (int x = 1; x < ims.Width(); x++) {
	 inter(y, x) = (1 - g) * ims(y, x) + g * inter(y, x - 1); // lissage causal
	 s1 = (1 - g) * inter(y, x) + g * s1; // deuxieme lissage causal pour initialisation de l'anticausal
      }

      inter(y, ims.Width() - 1) = s1; //recopie du point d'initialisation dans la derniere colonne.

      for (int x = ims.Width() - 2; x >= 0; x--) {
  	 inter(y, x) = (1 - g) * inter(y, x) + g * inter(y, x + 1); //lissage anticausal
      }
   }

   // Lissage vertical
   for (int x = 0; x < ims.Width(); x++) {
      float s1 = inter(ims.Height() - 1, x); //point de depart de l'initialisation de la colonne pour le lissage causal
      for (int y = ims.Height() - 2; y >= 0; y--) {
	 s1 = (1 - g) * inter(y, x) + g * s1; //initialisation (lissage anticausal)
      }
      inter(0, x) = s1;	//recopie du point d'initialisation dans la premiere ligne.
    
      for (int y = 1; y < ims.Height(); y++) {
	 inter(y, x) = (1 - g) * inter(y, x) + g * inter(y - 1, x); //lissage causal
	 s1 = (1 - g) * inter(y, x) + g * s1; //deuxieme lissage causal pour initialisation de l'anticausal
      }
      
      inter(ims.Height() - 1, x) = s1; //recopie du point d'initialisation dans la derniere ligne.
    
      for (int y = ims.Height() - 2; y >= 0 ; y--) {
	 inter(y, x) = (1 - g) * inter(y, x) + g * inter(y + 1, x); //lissage anticausal
      }
   }

   //Gradient----------------------------------------------------
   
   Img2dsf tabdx(ims.Props());
   Img2dsf tabdy(ims.Props());

   // Cas general.
   for (int y = 0; y < ims.Height() - 1; y++) {
      for (int x = 0; x < ims.Width() - 1; x++) {
	 Float dx = inter(y+1, x) - inter(y, x) + inter(y+1, x+1) - inter(y, x+1);
	 Float dy = inter(y, x+1) - inter(y, x) + inter(y+1, x+1) - inter(y+1, x);
	 inter(y, x) = (float)(0.5F * hypot(dx, dy));
	 tabdx(y, x) = dx;
	 tabdy(y, x) = dy;
      }
   }

   // Derniere colonne.
   const int xmax = ims.Width() - 1;
   for (int y = 0; y < ims.Height() - 1; y++) {
      Float dx = inter(y + 1, xmax) - inter(y, xmax);
      Float dy = 0.0;
      inter(y, xmax) = fabs(dx);
      tabdx(y, xmax) = dx;
      tabdy(y, xmax) = dy;
   }

   // Derniere ligne.
   const int ymax = ims.Height() - 1;
   for (int x = 0; x < ims.Width() - 1; x++) {
      Float dx = 0.0F;
      Float dy = fabs(inter(ymax, x + 1) - inter(ims.Height() - 1, x));
      inter(ymax, x) = fabs(dy);
      tabdx(ymax, x) = dx;
      tabdy(ymax, x) = dy;
   }

   // Derniere case.
   inter(ymax, xmax) = 0.0;
   tabdx(ymax, xmax) = 0;
   tabdy(ymax, xmax) = 0;

   // Extraction des maximas locaux
   imd.Frame(0, 1, 1);
   for (int y = 1; y < ymax; y++) {
      for (int x = 1; x <xmax; x++) {
	 Float dx = tabdx(y, x);
	 Float dy = tabdy(y, x);

	 // Cas degeneres
	 if (dx == 0 || dy == 0) {
	    if (dx == dy) {
	       imd(y, x) = 0;
	    } else {
	       if (dx == 0) {
		  if ((inter(y, x) < inter(y, x - 1)) || (inter(y, x) < inter(y, x + 1))) {
		     imd(y, x) = 0;
		  } else {
		     imd(y, x) = (Img2duc::ValueType)inter(y, x);
		  }
	       } else { // dy =  = 0
		  if ((inter(y, x) < inter(y - 1, x)) || (inter(y, x) < inter(y + 1, x))) {
		     imd(y, x) = 0;
		  } else {
		     imd(y, x) = (Img2duc::ValueType)inter(y, x);
		  }
	       }
	    }

	 } else { // dx et dy ! =  0
	    Float gr1 , gr2;
	    if (dx * dy > 0.0F) {
	       if (fabs(dx) >= fabs(dy)) {
		  gr1 = (dy / dx) * inter(y + 1, x + 1) + (1 - (dy / dx)) * inter(y + 1, x);
		  gr2 = (dy / dx) * inter(y - 1, x - 1) + (1 - (dy / dx)) * inter(y - 1, x);
	       } else {
		  gr1 = (dx / dy) * inter(y + 1, x + 1) + (1 - (dx / dy)) * inter(y, x + 1);
		  gr2 = (dx / dy) * inter(y - 1, x - 1) + (1 - (dx / dy)) * inter(y, x - 1);	  
	       }
	    } else {
	       if (fabs(dx) >= fabs(dy)) {
		  gr1 = fabs(dy / dx) * inter(y + 1, x - 1) + (1 - fabs(dy / dx)) * inter(y + 1, x);
		  gr2 = fabs(dy / dx) * inter(y - 1, x + 1) + (1 - fabs(dy / dx)) * inter(y - 1, x);
	       } else {
		  gr1 = fabs(dx / dy) * inter(y + 1, x - 1) + (1 - fabs(dx / dy)) * inter(y, x - 1);
		  gr2 = fabs(dx / dy) * inter(y - 1, x + 1) + (1 - fabs(dx / dy)) * inter(y, x + 1);
	       }
	    }
	    if ((inter(y, x) < gr1) || (inter(y, x) < gr2)) {
	       imd(y, x) = 0;
	    } else {
	       imd(y, x) = (Img2duc::ValueType)inter(y, x);
	    }
	 }
      }
   }
   return SUCCESS; 
}

Errc PShen( const Img2dsl &ims, Img2dsl &imd, Float alpha ) {
   const float g = (float)exp(-alpha);
  
   Img2dsf inter(ims.Props());

   // Lissage horizontal
   for (int y = 0; y < ims.Height(); y++) {
      float s1 = (float)ims(y, ims.Width() - 1); // point de depart de l'initialisation de la ligne pour le lissage causal
      for (int x = ims.Width() - 2 ; x >= 0; x--) { // On duplique le bord.
	 s1 = (1 - g) * ims(y, x) + g * s1; // initialisation (lissage anticausal)
      }
      inter(y, 0) = s1;	// recopie du point d'initialisation dans la premiere colonne.
      
      for (int x = 1; x < ims.Width(); x++) {
	 inter(y, x) = (1 - g) * ims(y, x) + g * inter(y, x - 1); // lissage causal
	 s1 = (1 - g) * inter(y, x) + g * s1; // deuxieme lissage causal pour initialisation de l'anticausal
      }

      inter(y, ims.Width() - 1) = s1; //recopie du point d'initialisation dans la derniere colonne.

      for (int x = ims.Width() - 2; x >= 0; x--) {
  	 inter(y, x) = (1 - g) * inter(y, x) + g * inter(y, x + 1); //lissage anticausal
      }
   }

   // Lissage vertical
   for (int x = 0; x < ims.Width(); x++) {
      float s1 = inter(ims.Height() - 1, x); //point de depart de l'initialisation de la colonne pour le lissage causal
      for (int y = ims.Height() - 2; y >= 0; y--) {
	 s1 = (1 - g) * inter(y, x) + g * s1; //initialisation (lissage anticausal)
      }
      inter(0, x) = s1;	//recopie du point d'initialisation dans la premiere ligne.
    
      for (int y = 1; y < ims.Height(); y++) {
	 inter(y, x) = (1 - g) * inter(y, x) + g * inter(y - 1, x); //lissage causal
	 s1 = (1 - g) * inter(y, x) + g * s1; //deuxieme lissage causal pour initialisation de l'anticausal
      }
      
      inter(ims.Height() - 1, x) = s1; //recopie du point d'initialisation dans la derniere ligne.
    
      for (int y = ims.Height() - 2; y >= 0 ; y--) {
	 inter(y, x) = (1 - g) * inter(y, x) + g * inter(y + 1, x); //lissage anticausal
      }
   }

   //Gradient----------------------------------------------------
   
   Img2dsf tabdx(ims.Props());
   Img2dsf tabdy(ims.Props());

   // Cas general.
   for (int y = 0; y < ims.Height() - 1; y++) {
      for (int x = 0; x < ims.Width() - 1; x++) {
	 Float dx = inter(y+1, x) - inter(y, x) + inter(y+1, x+1) - inter(y, x+1);
	 Float dy = inter(y, x+1) - inter(y, x) + inter(y+1, x+1) - inter(y+1, x);
	 inter(y, x) = (float)(0.5F * hypot(dx, dy));
	 tabdx(y, x) = dx;
	 tabdy(y, x) = dy;
      }
   }

   // Derniere colonne.
   const int xmax = ims.Width() - 1;
   for (int y = 0; y < ims.Height() - 1; y++) {
      Float dx = inter(y + 1, xmax) - inter(y, xmax);
      Float dy = 0.0;
      inter(y, xmax) = fabs(dx);
      tabdx(y, xmax) = dx;
      tabdy(y, xmax) = dy;
   }

   // Derniere ligne.
   const int ymax = ims.Height() - 1;
   for (int x = 0; x < ims.Width() - 1; x++) {
      Float dx = 0.0F;
      Float dy = fabs(inter(ymax, x + 1) - inter(ims.Height() - 1, x));
      inter(ymax, x) = fabs(dy);
      tabdx(ymax, x) = dx;
      tabdy(ymax, x) = dy;
   }

   // Derniere case.
   inter(ymax, xmax) = 0.0;
   tabdx(ymax, xmax) = 0;
   tabdy(ymax, xmax) = 0;

   // Extraction des maximas locaux
   imd.Frame(0, 1, 1);
   for (int y = 1; y < ymax; y++) {
      for (int x = 1; x <xmax; x++) {
	 Float dx = tabdx(y, x);
	 Float dy = tabdy(y, x);

	 // Cas degeneres
	 if (dx == 0 || dy == 0) {
	    if (dx == dy) {
	       imd(y, x) = 0;
	    } else {
	       if (dx == 0) {
		  if ((inter(y, x) < inter(y, x - 1)) || (inter(y, x) < inter(y, x + 1))) {
		     imd(y, x) = 0;
		  } else {
		     imd(y, x) = (Img2dsl::ValueType)inter(y, x);
		  }
	       } else { // dy =  = 0
		  if ((inter(y, x) < inter(y - 1, x)) || (inter(y, x) < inter(y + 1, x))) {
		     imd(y, x) = 0;
		  } else {
		     imd(y, x) = (Img2dsl::ValueType)inter(y, x);
		  }
	       }
	    }

	 } else { // dx et dy ! =  0
	    Float gr1 , gr2;
	    if (dx * dy > 0.0F) {
	       if (fabs(dx) >= fabs(dy)) {
		  gr1 = (dy / dx) * inter(y + 1, x + 1) + (1 - (dy / dx)) * inter(y + 1, x);
		  gr2 = (dy / dx) * inter(y - 1, x - 1) + (1 - (dy / dx)) * inter(y - 1, x);
	       } else {
		  gr1 = (dx / dy) * inter(y + 1, x + 1) + (1 - (dx / dy)) * inter(y, x + 1);
		  gr2 = (dx / dy) * inter(y - 1, x - 1) + (1 - (dx / dy)) * inter(y, x - 1);	  
	       }
	    } else {
	       if (fabs(dx) >= fabs(dy)) {
		  gr1 = fabs(dy / dx) * inter(y + 1, x - 1) + (1 - fabs(dy / dx)) * inter(y + 1, x);
		  gr2 = fabs(dy / dx) * inter(y - 1, x + 1) + (1 - fabs(dy / dx)) * inter(y - 1, x);
	       } else {
		  gr1 = fabs(dx / dy) * inter(y + 1, x - 1) + (1 - fabs(dx / dy)) * inter(y, x - 1);
		  gr2 = fabs(dx / dy) * inter(y - 1, x + 1) + (1 - fabs(dx / dy)) * inter(y, x + 1);
	       }
	    }
	    if ((inter(y, x) < gr1) || (inter(y, x) < gr2)) {
	       imd(y, x) = 0;
	    } else {
	       imd(y, x) = (Img2dsl::ValueType)inter(y, x);
	    }
	 }
      }
   }
   return SUCCESS; 
}

Errc PShen( const Img2dsf &ims, Img2dsf &imd, Float alpha ) {
   const float g = (float)exp(-alpha);
  
   Img2dsf inter(ims.Props());

   // Lissage horizontal
   for (int y = 0; y < ims.Height(); y++) {
      float s1 = (float)ims(y, ims.Width() - 1); // point de depart de l'initialisation de la ligne pour le lissage causal
      for (int x = ims.Width() - 2 ; x >= 0; x--) { // On duplique le bord.
	 s1 = (1 - g) * ims(y, x) + g * s1; // initialisation (lissage anticausal)
      }
      inter(y, 0) = s1;	// recopie du point d'initialisation dans la premiere colonne.
      
      for (int x = 1; x < ims.Width(); x++) {
	 inter(y, x) = (1 - g) * ims(y, x) + g * inter(y, x - 1); // lissage causal
	 s1 = (1 - g) * inter(y, x) + g * s1; // deuxieme lissage causal pour initialisation de l'anticausal
      }

      inter(y, ims.Width() - 1) = s1; //recopie du point d'initialisation dans la derniere colonne.

      for (int x = ims.Width() - 2; x >= 0; x--) {
  	 inter(y, x) = (1 - g) * inter(y, x) + g * inter(y, x + 1); //lissage anticausal
      }
   }

   // Lissage vertical
   for (int x = 0; x < ims.Width(); x++) {
      float s1 = inter(ims.Height() - 1, x); //point de depart de l'initialisation de la colonne pour le lissage causal
      for (int y = ims.Height() - 2; y >= 0; y--) {
	 s1 = (1 - g) * inter(y, x) + g * s1; //initialisation (lissage anticausal)
      }
      inter(0, x) = s1;	//recopie du point d'initialisation dans la premiere ligne.
    
      for (int y = 1; y < ims.Height(); y++) {
	 inter(y, x) = (1 - g) * inter(y, x) + g * inter(y - 1, x); //lissage causal
	 s1 = (1 - g) * inter(y, x) + g * s1; //deuxieme lissage causal pour initialisation de l'anticausal
      }
      
      inter(ims.Height() - 1, x) = s1; //recopie du point d'initialisation dans la derniere ligne.
    
      for (int y = ims.Height() - 2; y >= 0 ; y--) {
	 inter(y, x) = (1 - g) * inter(y, x) + g * inter(y + 1, x); //lissage anticausal
      }
   }

   //Gradient----------------------------------------------------
   
   Img2dsf tabdx(ims.Props());
   Img2dsf tabdy(ims.Props());

   // Cas general.
   for (int y = 0; y < ims.Height() - 1; y++) {
      for (int x = 0; x < ims.Width() - 1; x++) {
	 Float dx = inter(y+1, x) - inter(y, x) + inter(y+1, x+1) - inter(y, x+1);
	 Float dy = inter(y, x+1) - inter(y, x) + inter(y+1, x+1) - inter(y+1, x);
	 inter(y, x) = (float)(0.5F * hypot(dx, dy));
	 tabdx(y, x) = dx;
	 tabdy(y, x) = dy;
      }
   }

   // Derniere colonne.
   const int xmax = ims.Width() - 1;
   for (int y = 0; y < ims.Height() - 1; y++) {
      Float dx = inter(y + 1, xmax) - inter(y, xmax);
      Float dy = 0.0;
      inter(y, xmax) = fabs(dx);
      tabdx(y, xmax) = dx;
      tabdy(y, xmax) = dy;
   }

   // Derniere ligne.
   const int ymax = ims.Height() - 1;
   for (int x = 0; x < ims.Width() - 1; x++) {
      Float dx = 0.0F;
      Float dy = fabs(inter(ymax, x + 1) - inter(ims.Height() - 1, x));
      inter(ymax, x) = fabs(dy);
      tabdx(ymax, x) = dx;
      tabdy(ymax, x) = dy;
   }

   // Derniere case.
   inter(ymax, xmax) = 0.0;
   tabdx(ymax, xmax) = 0;
   tabdy(ymax, xmax) = 0;

   // Extraction des maximas locaux
   imd.Frame(0, 1, 1);
   for (int y = 1; y < ymax; y++) {
      for (int x = 1; x <xmax; x++) {
	 Float dx = tabdx(y, x);
	 Float dy = tabdy(y, x);

	 // Cas degeneres
	 if (dx == 0 || dy == 0) {
	    if (dx == dy) {
	       imd(y, x) = 0;
	    } else {
	       if (dx == 0) {
		  if ((inter(y, x) < inter(y, x - 1)) || (inter(y, x) < inter(y, x + 1))) {
		     imd(y, x) = 0;
		  } else {
		     imd(y, x) = (Img2dsf::ValueType)inter(y, x);
		  }
	       } else { // dy =  = 0
		  if ((inter(y, x) < inter(y - 1, x)) || (inter(y, x) < inter(y + 1, x))) {
		     imd(y, x) = 0;
		  } else {
		     imd(y, x) = (Img2dsf::ValueType)inter(y, x);
		  }
	       }
	    }

	 } else { // dx et dy ! =  0
	    Float gr1 , gr2;
	    if (dx * dy > 0.0F) {
	       if (fabs(dx) >= fabs(dy)) {
		  gr1 = (dy / dx) * inter(y + 1, x + 1) + (1 - (dy / dx)) * inter(y + 1, x);
		  gr2 = (dy / dx) * inter(y - 1, x - 1) + (1 - (dy / dx)) * inter(y - 1, x);
	       } else {
		  gr1 = (dx / dy) * inter(y + 1, x + 1) + (1 - (dx / dy)) * inter(y, x + 1);
		  gr2 = (dx / dy) * inter(y - 1, x - 1) + (1 - (dx / dy)) * inter(y, x - 1);	  
	       }
	    } else {
	       if (fabs(dx) >= fabs(dy)) {
		  gr1 = fabs(dy / dx) * inter(y + 1, x - 1) + (1 - fabs(dy / dx)) * inter(y + 1, x);
		  gr2 = fabs(dy / dx) * inter(y - 1, x + 1) + (1 - fabs(dy / dx)) * inter(y - 1, x);
	       } else {
		  gr1 = fabs(dx / dy) * inter(y + 1, x - 1) + (1 - fabs(dx / dy)) * inter(y, x - 1);
		  gr2 = fabs(dx / dy) * inter(y - 1, x + 1) + (1 - fabs(dx / dy)) * inter(y, x + 1);
	       }
	    }
	    if ((inter(y, x) < gr1) || (inter(y, x) < gr2)) {
	       imd(y, x) = 0;
	    } else {
	       imd(y, x) = (Img2dsf::ValueType)inter(y, x);
	    }
	 }
      }
   }
   return SUCCESS; 
}


#ifdef MAIN
#define	USAGE	"usage: %s strength [-m mask] [im_in|-] [im_out|-]"
#define	PARC	1
#define	FINC	1
#define	FOUTC	1
#define	MASK	2 // Masking

int main( int argc, char *argv[] ) {
   Errc result;                // The result code of the execution.
   Pobject* mask;              // The region map.
   Pobject* objin[FINC + 1];   // The input objects.
   Pobject* objs[FINC + 1];    // The source objects masked.
   Pobject* objout[FOUTC + 1]; // The output object.
   Pobject* objd[FOUTC + 1];   // The result object of the execution.
   char* parv[PARC + 1];       // The input parameters.

   ReadArgs(argc, argv, PARC, FINC, FOUTC, &mask, objin, objs, objout, objd, parv, USAGE, MASK); 
   if (objs[0]->Type() ==  Po_Img2duc) {
      Img2duc* const ims = (Img2duc*)objs[0];
      objd[0] = new Img2duc(ims->Props());
      Img2duc* const imd = (Img2duc*)objd[0];
      
      result = PShen(*ims, *imd, (Float)atof(parv[0]));
      goto end;
   }

   if (objs[0]->Type() ==  Po_Img2dsl) {
      Img2dsl* const ims = (Img2dsl*)objs[0];
      objd[0] = new Img2dsl(ims->Props());
      Img2dsl* const imd = (Img2dsl*)objd[0];
      
      result = PShen(*ims, *imd, (Float)atof(parv[0]));
      goto end;
   }

   if (objs[0]->Type() ==  Po_Img2dsf) {
      Img2dsf* const ims = (Img2dsf*)objs[0];
      objd[0] = new Img2dsf(ims->Props());
      Img2dsf* const imd = (Img2dsf*)objd[0];
      
      result = PShen(*ims, *imd, (Float)atof(parv[0]));
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
