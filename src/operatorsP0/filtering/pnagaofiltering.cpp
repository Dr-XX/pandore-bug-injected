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
 * @author Regis Clouard - 1997-02-26
 * @author Regis Clouard - 2003-04-14
 */

#include <pandore.h>
using namespace pandore;

/**
 * @file pnagaofiltering.cpp
 * Filtre de nagao : lissage par maximum d'homogeneite.
 * echerche du domaine le plus homogene en terme de variance.
 * emplacement du point central par la moyenne
 * du domaine le plus homogene.
 * ref: "Analyse d'images: filtrage et segmentation",
 * J.P. Cocquerez & S. Philipp., Masson, 1995.
 */

static	Uchar	msq[9][5][5] = {{ {0,1,1,1,0},
				  {0,1,1,1,0},
				  {0,0,1,0,0},
				  {0,0,0,0,0},
				  {0,0,0,0,0}},

				{ {0,0,0,1,1},
				  {0,0,0,1,1},
				  {0,0,1,1,1},
				  {0,0,0,0,0},
				  {0,0,0,0,0}},
				  
				{ {0,0,0,0,0},
				  {0,0,0,1,1},
				  {0,0,1,1,1},
				  {0,0,0,1,1},
				  {0,0,0,0,0}},
				  
				{ {0,0,0,0,0},
				  {0,0,0,0,0},
				  {0,0,1,1,1},
				  {0,0,0,1,1},
				  {0,0,0,1,1}},

				{ {0,0,0,0,0},
				  {0,0,0,0,0},
				  {0,0,1,0,0},
				  {0,1,1,1,0},
				  {0,1,1,1,0}},

				{ {0,0,0,0,0},
				  {0,0,0,0,0},
				  {1,1,1,0,0},
				  {1,1,0,0,0},
				  {1,1,0,0,0}},

				{ {0,0,0,0,0},
				  {1,1,0,0,0},
				  {1,1,1,0,0},
				  {1,1,0,0,0},
				  {0,0,0,0,0}},
				 
				{ {1,1,0,0,0},
				  {1,1,0,0,0},
				  {1,1,1,0,0},
				  {0,0,0,0,0},
				  {0,0,0,0,0}},

				{ {0,0,0,0,0},
				  {0,1,1,1,0},
				  {0,1,1,1,0},
				  {0,1,0,1,0},
				  {0,0,0,0,0}}};


Errc PNagaoFiltering( const Img2duc &ims, Img2duc &imd ) {
   int x,y,k,l;
   int d;
   float sum1, sum2;
   Ulong n;
   float variance,var;
   Img2duc::ValueType moyenne;
   
   // Copie des bords de l'image source dans l'image destination.
   imd.Frame(ims,2,2);
   
   for (y=2;y<ims.Height()-2;y++) {
      for (x=2;x<ims.Width()-2;x++) {

	 // For each mask.
	 variance=(float)MAXFLOAT;
	 moyenne=0;
	 for (d=0; d<9; d++) {
	    sum1=sum2=0.0F; n=0;
	    for (k=-2; k<=2; k++) {
	       for (l=-2; l<=2; l++) {
		  if (msq[d][k+2][l+2]) {
		     sum2+=(float)ims[y+k][x+l]*ims[y+k][x+l];
		     sum1+=(float)ims[y+k][x+l];
		     n++;
		  }
	       }
	    }
	    sum2/=n;
	    sum1/=n;
	    if ((var=(sum2-sum1*sum1))<variance) {
	       variance = var;
	       moyenne = (Img2duc::ValueType)sum1;
	    }
	 }
	 
	 imd[y][x] = moyenne;
      }
   }
   return SUCCESS;
}

Errc PNagaoFiltering( const Img2dsl &ims, Img2dsl &imd ) {
   int x,y,k,l;
   int d;
   float sum1, sum2;
   Ulong n;
   float variance,var;
   Img2dsl::ValueType moyenne;
   
   // Copie des bords de l'image source dans l'image destination.
   imd.Frame(ims,2,2);
   
   for (y=2;y<ims.Height()-2;y++) {
      for (x=2;x<ims.Width()-2;x++) {

	 // For each mask.
	 variance=(float)MAXFLOAT;
	 moyenne=0;
	 for (d=0; d<9; d++) {
	    sum1=sum2=0.0F; n=0;
	    for (k=-2; k<=2; k++) {
	       for (l=-2; l<=2; l++) {
		  if (msq[d][k+2][l+2]) {
		     sum2+=(float)ims[y+k][x+l]*ims[y+k][x+l];
		     sum1+=(float)ims[y+k][x+l];
		     n++;
		  }
	       }
	    }
	    sum2/=n;
	    sum1/=n;
	    if ((var=(sum2-sum1*sum1))<variance) {
	       variance = var;
	       moyenne = (Img2dsl::ValueType)sum1;
	    }
	 }
	 
	 imd[y][x] = moyenne;
      }
   }
   return SUCCESS;
}

Errc PNagaoFiltering( const Img2dsf &ims, Img2dsf &imd ) {
   int x,y,k,l;
   int d;
   float sum1, sum2;
   Ulong n;
   float variance,var;
   Img2dsf::ValueType moyenne;
   
   // Copie des bords de l'image source dans l'image destination.
   imd.Frame(ims,2,2);
   
   for (y=2;y<ims.Height()-2;y++) {
      for (x=2;x<ims.Width()-2;x++) {

	 // For each mask.
	 variance=(float)MAXFLOAT;
	 moyenne=0;
	 for (d=0; d<9; d++) {
	    sum1=sum2=0.0F; n=0;
	    for (k=-2; k<=2; k++) {
	       for (l=-2; l<=2; l++) {
		  if (msq[d][k+2][l+2]) {
		     sum2+=(float)ims[y+k][x+l]*ims[y+k][x+l];
		     sum1+=(float)ims[y+k][x+l];
		     n++;
		  }
	       }
	    }
	    sum2/=n;
	    sum1/=n;
	    if ((var=(sum2-sum1*sum1))<variance) {
	       variance = var;
	       moyenne = (Img2dsf::ValueType)sum1;
	    }
	 }
	 
	 imd[y][x] = moyenne;
      }
   }
   return SUCCESS;
}


/*
 * Modify only the following constants, and the function call.
 */
#ifdef MAIN
#define	USAGE	"usage: %s [-m mask] [im_in|-] [im_out|-]"
#define	PARC	0
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
   if (objs[0]->Type()==Po_Img2duc) {
      Img2duc* const ims=(Img2duc*)objs[0];
      objd[0]=new Img2duc(ims->Size());
      Img2duc* const imd=(Img2duc*)objd[0];
      
      result=PNagaoFiltering(*ims,*imd);
      goto end;
   }
   if (objs[0]->Type()==Po_Img2dsl) {
      Img2dsl* const ims=(Img2dsl*)objs[0];
      objd[0]=new Img2dsl(ims->Size());
      Img2dsl* const imd=(Img2dsl*)objd[0];
      
      result=PNagaoFiltering(*ims,*imd);
      goto end;
   }
   if (objs[0]->Type()==Po_Img2dsf) {
      Img2dsf* const ims=(Img2dsf*)objs[0];
      objd[0]=new Img2dsf(ims->Size());
      Img2dsf* const imd=(Img2dsf*)objd[0];
      
      result=PNagaoFiltering(*ims,*imd);
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
