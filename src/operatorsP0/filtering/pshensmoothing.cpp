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
 * @author Carlotti & Joguet - 2000-05-15
 */

#include <math.h>
#include <pandore.h>
using namespace pandore;

/**
 * @file pshensmooting.cpp
 * Lissage causal et anticausal horizontal et vertical d'une image par SHEN
 * (en flottant). Parametre alpha dans l`USAGE.
 */

Errc PShenSmoothing( const Img2duc &ims, Img2duc &imd, Float alpha ) {
   Double g;
   g = exp(-alpha);	
   
   register int i,j;
   
   Img2dsf inter(ims.Size());  
   Float s1;
   
   // Lissage horizontal
   for (i = 0; i < ims.Height(); i++) {
      s1 = (float)ims[i][ims.Width() - 1]; //point de depart de l'initialisation de la ligne pour le lissage causal
      for (j = ims.Width() - 2; j >= 0; j--) {
	 s1 = (float)((1 - g) * ims[i][j] + g * s1); //initialisation (lissage anticausal)
      }
      inter[i][0] = s1;	//recopie du point d'initialisation dans la premiere colonne.
      
      for (j = 1; j < ims.Width(); j++) {
	 inter[i][j] = (float)((1 - g) * ims[i][j] + g * inter[i][j - 1]); //lissage causal
	 s1=(float)((1 - g) * inter[i][j] + g * s1); //deuxieme lissage causal pour initialisation de l'anticausal
      }
      
      inter[i][ims.Width() - 1] = s1; //recopie du point d'initialisation dans la derniere colonne.
      
      for (j = ims.Width() - 2; j >= 0; j--) {
	 inter[i][j] = (float)((1 - g) * inter[i][j] + g * inter[i][j + 1]); //lissage anticausal
      }
   }
   
   // Lissage vertical
   for (j = 0; j < ims.Width(); j++) {
      s1 = (float)inter[ims.Height() - 1][j]; //point de depart de l'initialisation de la colonne pour le lissage causal
      for (i = ims.Height() - 2; i >= 0; i--) {
	 s1 = (float)((1 - g) * inter[i][j] + g * s1); //initialisation (lissage anticausal)
      }
      inter[0][j]=s1;	//recopie du point d'initialisation dans la premiere ligne.
      
      for (i = 1; i < ims.Height(); i++) {
	 inter[i][j] = (float)((1 - g) * inter[i][j] + g * inter[i - 1][j]); //lissage causal
	 s1 = (float)((1 - g) * inter[i][j] + g * s1); //deuxieme lissage causal pour initialisation de l'anticausal
      }
      
      inter[ims.Height()-1][j] = s1; //recopie du point d'initialisation dans la derniere ligne.
      
      for (i = ims.Height() - 2; i >= 0; i--) {
	 inter[i][j] = (float)((1 - g) * inter[i][j] + g*inter[i + 1][j]); //lissage anticausal
      }
   }
   
   for (i = 0; i < ims.Height(); i++) {
      for (j = 0; j < ims.Width(); j++) {
	 imd[i][j] = (int)inter[i][j];
      }
   }
   return SUCCESS;
}

Errc PShenSmoothing( const Img2dsl &ims, Img2duc &imd, Float alpha ) {
   Double g;
   g = exp(-alpha);	
   
   register int i,j;
   
   Img2dsf inter(ims.Size());  
   Float s1;
   
   // Lissage horizontal
   for (i = 0; i < ims.Height(); i++) {
      s1 = (float)ims[i][ims.Width() - 1]; //point de depart de l'initialisation de la ligne pour le lissage causal
      for (j = ims.Width() - 2; j >= 0; j--) {
	 s1 = (float)((1 - g) * ims[i][j] + g * s1); //initialisation (lissage anticausal)
      }
      inter[i][0] = s1;	//recopie du point d'initialisation dans la premiere colonne.
      
      for (j = 1; j < ims.Width(); j++) {
	 inter[i][j] = (float)((1 - g) * ims[i][j] + g * inter[i][j - 1]); //lissage causal
	 s1=(float)((1 - g) * inter[i][j] + g * s1); //deuxieme lissage causal pour initialisation de l'anticausal
      }
      
      inter[i][ims.Width() - 1] = s1; //recopie du point d'initialisation dans la derniere colonne.
      
      for (j = ims.Width() - 2; j >= 0; j--) {
	 inter[i][j] = (float)((1 - g) * inter[i][j] + g * inter[i][j + 1]); //lissage anticausal
      }
   }
   
   // Lissage vertical
   for (j = 0; j < ims.Width(); j++) {
      s1 = (float)inter[ims.Height() - 1][j]; //point de depart de l'initialisation de la colonne pour le lissage causal
      for (i = ims.Height() - 2; i >= 0; i--) {
	 s1 = (float)((1 - g) * inter[i][j] + g * s1); //initialisation (lissage anticausal)
      }
      inter[0][j]=s1;	//recopie du point d'initialisation dans la premiere ligne.
      
      for (i = 1; i < ims.Height(); i++) {
	 inter[i][j] = (float)((1 - g) * inter[i][j] + g * inter[i - 1][j]); //lissage causal
	 s1 = (float)((1 - g) * inter[i][j] + g * s1); //deuxieme lissage causal pour initialisation de l'anticausal
      }
      
      inter[ims.Height()-1][j] = s1; //recopie du point d'initialisation dans la derniere ligne.
      
      for (i = ims.Height() - 2; i >= 0; i--) {
	 inter[i][j] = (float)((1 - g) * inter[i][j] + g*inter[i + 1][j]); //lissage anticausal
      }
   }
   
   for (i = 0; i < ims.Height(); i++) {
      for (j = 0; j < ims.Width(); j++) {
	 imd[i][j] = (int)inter[i][j];
      }
   }
   return SUCCESS;
}

Errc PShenSmoothing( const Img2dsf &ims, Img2duc &imd, Float alpha ) {
   Double g;
   g = exp(-alpha);	
   
   register int i,j;
   
   Img2dsf inter(ims.Size());  
   Float s1;
   
   // Lissage horizontal
   for (i = 0; i < ims.Height(); i++) {
      s1 = (float)ims[i][ims.Width() - 1]; //point de depart de l'initialisation de la ligne pour le lissage causal
      for (j = ims.Width() - 2; j >= 0; j--) {
	 s1 = (float)((1 - g) * ims[i][j] + g * s1); //initialisation (lissage anticausal)
      }
      inter[i][0] = s1;	//recopie du point d'initialisation dans la premiere colonne.
      
      for (j = 1; j < ims.Width(); j++) {
	 inter[i][j] = (float)((1 - g) * ims[i][j] + g * inter[i][j - 1]); //lissage causal
	 s1=(float)((1 - g) * inter[i][j] + g * s1); //deuxieme lissage causal pour initialisation de l'anticausal
      }
      
      inter[i][ims.Width() - 1] = s1; //recopie du point d'initialisation dans la derniere colonne.
      
      for (j = ims.Width() - 2; j >= 0; j--) {
	 inter[i][j] = (float)((1 - g) * inter[i][j] + g * inter[i][j + 1]); //lissage anticausal
      }
   }
   
   // Lissage vertical
   for (j = 0; j < ims.Width(); j++) {
      s1 = (float)inter[ims.Height() - 1][j]; //point de depart de l'initialisation de la colonne pour le lissage causal
      for (i = ims.Height() - 2; i >= 0; i--) {
	 s1 = (float)((1 - g) * inter[i][j] + g * s1); //initialisation (lissage anticausal)
      }
      inter[0][j]=s1;	//recopie du point d'initialisation dans la premiere ligne.
      
      for (i = 1; i < ims.Height(); i++) {
	 inter[i][j] = (float)((1 - g) * inter[i][j] + g * inter[i - 1][j]); //lissage causal
	 s1 = (float)((1 - g) * inter[i][j] + g * s1); //deuxieme lissage causal pour initialisation de l'anticausal
      }
      
      inter[ims.Height()-1][j] = s1; //recopie du point d'initialisation dans la derniere ligne.
      
      for (i = ims.Height() - 2; i >= 0; i--) {
	 inter[i][j] = (float)((1 - g) * inter[i][j] + g*inter[i + 1][j]); //lissage anticausal
      }
   }
   
   for (i = 0; i < ims.Height(); i++) {
      for (j = 0; j < ims.Width(); j++) {
	 imd[i][j] = (int)inter[i][j];
      }
   }
   return SUCCESS;
}


#ifdef MAIN
#define	USAGE	"usage: %s coefficient [-m maks] [im_in|-] [im_out|-]"
#define	PARC	1
#define	FINC	1
#define	FOUTC	1
#define	MASK	3

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
      objd[0] = new Img2duc(ims->Size());
      Img2duc* const imd = (Img2duc*)objd[0];
      
      result = PShenSmoothing(*ims, *imd, (Float)atof(parv[0]));
      goto end;
   }

   if (objs[0]->Type() == Po_Img2dsl) {
      Img2dsl* const ims = (Img2dsl*)objs[0];
      objd[0] = new Img2duc(ims->Size());
      Img2duc* const imd = (Img2duc*)objd[0];
      
      result = PShenSmoothing(*ims, *imd, (Float)atof(parv[0]));
      goto end;
   }

   if (objs[0]->Type() == Po_Img2dsf) {
      Img2dsf* const ims = (Img2dsf*)objs[0];
      objd[0] = new Img2duc(ims->Size());
      Img2duc* const imd = (Img2duc*)objd[0];
      
      result = PShenSmoothing(*ims, *imd, (Float)atof(parv[0]));
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
