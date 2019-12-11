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
 * @author Regis Clouard - 1994-10-13
 */

#include <math.h>
#include <pandore.h>
using namespace pandore;

/**
 * @file pexponentialfiltering.cpp
 *
 * Filtre exponentiel symetrique.
 * alpha: Valeur de lissage. (0 fort, 1 nul).
 * Construction d'une image intermediaire par passage
 * d'un filtre exponentielle sur l'image source.
 * Renouvellement de l'operation par passage du meme
 * filtre en sens inverse pour construire l'image destination.
 */
template <typename T>
void expo_x( const Imx3d<T> &ims, Imx3dsf &imd, float alpha ) {
   Float *h1 = (Float*)malloc(ims.Width()*sizeof(Float));
   Float *h2 = (Float*)malloc(ims.Width()*sizeof(Float));

   for (int b=0; b<ims.Bands(); b++) {
      for (int z=0; z<ims.Depth(); z++) {
	 for (int y=0; y<ims.Height(); y++) {

	    h1[0]=(float)ims(b, z, y, 0);
	    for (int x=1; x<ims.Width(); x++) {
	       h1[x] = alpha*((float)ims(b, z, y, x)-h1[x-1]) + h1[x-1];
	    }

	    h2[ims.Width()-1]=h1[ims.Width()-1];
	    for (int x=ims.Width()-2; x>=0; x--) {
	       h2[x] = alpha*(h1[x]-h2[x+1]) + h2[x+1];
	    }

	    for (int x=0; x<ims.Width(); x++) {
	       imd(b, z, y, x) = h2[x];
	    }
	 }
      }
   }
   free(h1);
   free(h2);
}

template <typename T>
void expo_y( const Imx3dsf &ims, Imx3d<T> &imd, float alpha ) {
   Float *h1 = (Float*)malloc(ims.Height()*sizeof(Float));
   Float *h2 = (Float*)malloc(ims.Height()*sizeof(Float));
   
   for (int b=0; b<ims.Bands(); b++) {
      for (int z=0; z<ims.Depth(); z++) {
	 for (int x=0; x<ims.Width(); x++) {
	    h1[0]=ims(b, z, 0, x);
	    Long y;
	    for (y=1; y<ims.Height(); y++) {
	       h1[y]=alpha*(ims(b, z, y, x)-h1[y-1])+h1[y-1];
	    }
	    h2[ims.Height()-1]=h1[ims.Height()-1];
	    for (y=ims.Height()-2; y>=0; y--) {
	       h2[y]=alpha*(h1[y]-h2[y+1])+h2[y+1];
	    }
	    for (y=0; y<ims.Height(); y++) {
	       imd(b, z, y, x)=(T)h2[y];
	    }
	 }
      }
   }
   free(h1);
   free(h2);
}

template <typename T>
void expo_z( const Imx3dsf &ims, Imx3d<T> &imd, float alpha ) {
   Float *h1 = (Float*)malloc(ims.Depth()*sizeof(Float));
   Float *h2 = (Float*)malloc(ims.Depth()*sizeof(Float));
   
   for (int b=0; b<ims.Bands(); b++) {
      for (int y=0; y<ims.Height(); y++) {
	 for (int x=0; x<ims.Width(); x++) {
	    h1[0] = ims(b, 0, y, x);
	    for (int z=1; z<ims.Depth(); z++) {
	       h1[z]=alpha*(ims(b, z, y, x)-h1[z-1]) + h1[z-1];
	    }

	    h2[ims.Depth()-1]=h1[ims.Depth()-1];
	    for (int z=ims.Depth()-2; z>=0; z--) {
	       h2[z]=alpha*(h1[z]-h2[z+1]) + h2[z+1];
	    }

	    for (int z=0; z<ims.Depth(); z++) {
	       imd(b, z, y, x) = (T)h2[z];
	    }
	 }
      }
   }
   free(h1);
   free(h2);
}








Errc PExponentialFiltering( const Img2duc &ims, Img2duc &imd, float alpha ) {
   Img2dsf sf1(ims.Size());

   expo_x(ims, sf1, alpha);
   expo_y(sf1, imd, alpha);
  
   return SUCCESS;
}
Errc PExponentialFiltering( const Img2dsl &ims, Img2dsl &imd, float alpha ) {
   Img2dsf sf1(ims.Size());

   expo_x(ims, sf1, alpha);
   expo_y(sf1, imd, alpha);
  
   return SUCCESS;
}
Errc PExponentialFiltering( const Img2dsf &ims, Img2dsf &imd, float alpha ) {
   Img2dsf sf1(ims.Size());

   expo_x(ims, sf1, alpha);
   expo_y(sf1, imd, alpha);
  
   return SUCCESS;
}
Errc PExponentialFiltering( const Img3duc &ims, Img3duc &imd, float alpha ) {
   Img3dsf sf1(ims.Size());
   Img3dsf sf2(ims.Size());

   expo_x(ims, sf2, alpha);
   expo_y(sf2, sf1, alpha);
   expo_z(sf1, imd, alpha);

   return SUCCESS;
}
Errc PExponentialFiltering( const Img3dsl &ims, Img3dsl &imd, float alpha ) {
   Img3dsf sf1(ims.Size());
   Img3dsf sf2(ims.Size());

   expo_x(ims, sf2, alpha);
   expo_y(sf2, sf1, alpha);
   expo_z(sf1, imd, alpha);

   return SUCCESS;
}
Errc PExponentialFiltering( const Img3dsf &ims, Img3dsf &imd, float alpha ) {
   Img3dsf sf1(ims.Size());
   Img3dsf sf2(ims.Size());

   expo_x(ims, sf2, alpha);
   expo_y(sf2, sf1, alpha);
   expo_z(sf1, imd, alpha);

   return SUCCESS;
}
Errc PExponentialFiltering( const Imc2duc &ims, Imc2duc &imd, float alpha ) {
   Imc2dsf imi1(ims.Props());
  
   expo_x(ims, imi1, alpha);
   expo_y(imi1, imd, alpha);

   return SUCCESS;
}

Errc PExponentialFiltering( const Imc2dsl &ims, Imc2dsl &imd, float alpha ) {
   Imc2dsf imi1(ims.Props());
  
   expo_x(ims, imi1, alpha);
   expo_y(imi1, imd, alpha);

   return SUCCESS;
}

Errc PExponentialFiltering( const Imc2dsf &ims, Imc2dsf &imd, float alpha ) {
   Imc2dsf imi1(ims.Props());
  
   expo_x(ims, imi1, alpha);
   expo_y(imi1, imd, alpha);

   return SUCCESS;
}

Errc PExponentialFiltering( const Imc3duc &ims, Imc3duc &imd, float alpha ) {
   Imc3dsf sf1(ims.Props());
   Imc3dsf sf2(ims.Props());
  
   expo_x(ims, sf2, alpha);
   expo_y(sf2, sf1, alpha);
   expo_z(sf1, imd, alpha);

   return SUCCESS;
}

Errc PExponentialFiltering( const Imc3dsl &ims, Imc3dsl &imd, float alpha ) {
   Imc3dsf sf1(ims.Props());
   Imc3dsf sf2(ims.Props());
  
   expo_x(ims, sf2, alpha);
   expo_y(sf2, sf1, alpha);
   expo_z(sf1, imd, alpha);

   return SUCCESS;
}

Errc PExponentialFiltering( const Imc3dsf &ims, Imc3dsf &imd, float alpha ) {
   Imc3dsf sf1(ims.Props());
   Imc3dsf sf2(ims.Props());
  
   expo_x(ims, sf2, alpha);
   expo_y(sf2, sf1, alpha);
   expo_z(sf1, imd, alpha);

   return SUCCESS;
}

Errc PExponentialFiltering( const Imx2duc &ims, Imx2duc &imd, float alpha) {
   Imx2dsf imi1(ims.Props());
  
   expo_x(ims, imi1, alpha);
   expo_y(imi1, imd, alpha);

   return SUCCESS;
}

Errc PExponentialFiltering( const Imx2dsl &ims, Imx2dsl &imd, float alpha) {
   Imx2dsf imi1(ims.Props());
  
   expo_x(ims, imi1, alpha);
   expo_y(imi1, imd, alpha);

   return SUCCESS;
}

Errc PExponentialFiltering( const Imx2dsf &ims, Imx2dsf &imd, float alpha) {
   Imx2dsf imi1(ims.Props());
  
   expo_x(ims, imi1, alpha);
   expo_y(imi1, imd, alpha);

   return SUCCESS;
}

Errc PExponentialFiltering( const Imx3duc &ims, Imx3duc &imd, float alpha ) {
   Imx3dsf sf1(ims.Props());
   Imx3dsf sf2(ims.Props());

   expo_x(ims, sf2, alpha);
   expo_y(sf2, sf1, alpha);
   expo_z(sf1, imd, alpha);

   return SUCCESS;
}

Errc PExponentialFiltering( const Imx3dsl &ims, Imx3dsl &imd, float alpha ) {
   Imx3dsf sf1(ims.Props());
   Imx3dsf sf2(ims.Props());

   expo_x(ims, sf2, alpha);
   expo_y(sf2, sf1, alpha);
   expo_z(sf1, imd, alpha);

   return SUCCESS;
}

Errc PExponentialFiltering( const Imx3dsf &ims, Imx3dsf &imd, float alpha ) {
   Imx3dsf sf1(ims.Props());
   Imx3dsf sf2(ims.Props());

   expo_x(ims, sf2, alpha);
   expo_y(sf2, sf1, alpha);
   expo_z(sf1, imd, alpha);

   return SUCCESS;
}


#ifdef MAIN
#define	USAGE	"usage: %s alpha [-m mask] [im_in|-] [im_out|-]"
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
   if (objs[0]->Type()==Po_Img2duc) {
      Img2duc* const ims=(Img2duc*)objs[0];
      objd[0]=new Img2duc(ims->Props());
      Img2duc* const imd=(Img2duc*)objd[0];
      
      result=PExponentialFiltering(*ims, *imd, (Float)atof(parv[0]));
      goto end;
   }
   if (objs[0]->Type()==Po_Img2dsl) {
      Img2dsl* const ims=(Img2dsl*)objs[0];
      objd[0]=new Img2dsl(ims->Props());
      Img2dsl* const imd=(Img2dsl*)objd[0];
      
      result=PExponentialFiltering(*ims, *imd, (Float)atof(parv[0]));
      goto end;
   }
   if (objs[0]->Type()==Po_Img2dsf) {
      Img2dsf* const ims=(Img2dsf*)objs[0];
      objd[0]=new Img2dsf(ims->Props());
      Img2dsf* const imd=(Img2dsf*)objd[0];
      
      result=PExponentialFiltering(*ims, *imd, (Float)atof(parv[0]));
      goto end;
   }
   if (objs[0]->Type()==Po_Img3duc) {
      Img3duc* const ims=(Img3duc*)objs[0];
      objd[0]=new Img3duc(ims->Props());
      Img3duc* const imd=(Img3duc*)objd[0];
      
      result=PExponentialFiltering(*ims, *imd, (Float)atof(parv[0]));
      goto end;
   }
   if (objs[0]->Type()==Po_Img3dsl) {
      Img3dsl* const ims=(Img3dsl*)objs[0];
      objd[0]=new Img3dsl(ims->Props());
      Img3dsl* const imd=(Img3dsl*)objd[0];
      
      result=PExponentialFiltering(*ims, *imd, (Float)atof(parv[0]));
      goto end;
   }
   if (objs[0]->Type()==Po_Img3dsf) {
      Img3dsf* const ims=(Img3dsf*)objs[0];
      objd[0]=new Img3dsf(ims->Props());
      Img3dsf* const imd=(Img3dsf*)objd[0];
      
      result=PExponentialFiltering(*ims, *imd, (Float)atof(parv[0]));
      goto end;
   }
   if (objs[0]->Type()==Po_Imc2duc) {
      Imc2duc* const ims=(Imc2duc*)objs[0];
      objd[0]=new Imc2duc(ims->Props());
      Imc2duc* const imd=(Imc2duc*)objd[0];
      
      result=PExponentialFiltering(*ims, *imd, (Float)atof(parv[0]));
      goto end;
   }
   if (objs[0]->Type()==Po_Imc2dsl) {
      Imc2dsl* const ims=(Imc2dsl*)objs[0];
      objd[0]=new Imc2dsl(ims->Props());
      Imc2dsl* const imd=(Imc2dsl*)objd[0];
      
      result=PExponentialFiltering(*ims, *imd, (Float)atof(parv[0]));
      goto end;
   }
   if (objs[0]->Type()==Po_Imc2dsf) {
      Imc2dsf* const ims=(Imc2dsf*)objs[0];
      objd[0]=new Imc2dsf(ims->Props());
      Imc2dsf* const imd=(Imc2dsf*)objd[0];
      
      result=PExponentialFiltering(*ims, *imd, (Float)atof(parv[0]));
      goto end;
   }
   if (objs[0]->Type()==Po_Imc3duc) {
      Imc3duc* const ims=(Imc3duc*)objs[0];
      objd[0]=new Imc3duc(ims->Props());
      Imc3duc* const imd=(Imc3duc*)objd[0];
      
      result=PExponentialFiltering(*ims, *imd, (Float)atof(parv[0]));
      goto end;
   }
   if (objs[0]->Type()==Po_Imc3dsl) {
      Imc3dsl* const ims=(Imc3dsl*)objs[0];
      objd[0]=new Imc3dsl(ims->Props());
      Imc3dsl* const imd=(Imc3dsl*)objd[0];
      
      result=PExponentialFiltering(*ims, *imd, (Float)atof(parv[0]));
      goto end;
   }
   if (objs[0]->Type()==Po_Imc3dsf) {
      Imc3dsf* const ims=(Imc3dsf*)objs[0];
      objd[0]=new Imc3dsf(ims->Props());
      Imc3dsf* const imd=(Imc3dsf*)objd[0];
      
      result=PExponentialFiltering(*ims, *imd, (Float)atof(parv[0]));
      goto end;
   }
   if (objs[0]->Type()==Po_Imx2duc) {
      Imx2duc* const ims=(Imx2duc*)objs[0];
      objd[0]=new Imx2duc(ims->Props());
      Imx2duc* const imd=(Imx2duc*)objd[0];
      
      result=PExponentialFiltering(*ims, *imd, (Float)atof(parv[0]));
      goto end;
   }
   if (objs[0]->Type()==Po_Imx2dsl) {
      Imx2dsl* const ims=(Imx2dsl*)objs[0];
      objd[0]=new Imx2dsl(ims->Props());
      Imx2dsl* const imd=(Imx2dsl*)objd[0];
      
      result=PExponentialFiltering(*ims, *imd, (Float)atof(parv[0]));
      goto end;
   }
   if (objs[0]->Type()==Po_Imx2dsf) {
      Imx2dsf* const ims=(Imx2dsf*)objs[0];
      objd[0]=new Imx2dsf(ims->Props());
      Imx2dsf* const imd=(Imx2dsf*)objd[0];
      
      result=PExponentialFiltering(*ims, *imd, (Float)atof(parv[0]));
      goto end;
   }
   if (objs[0]->Type()==Po_Imx3duc) {
      Imx3duc* const ims=(Imx3duc*)objs[0];
      objd[0]=new Imx3duc(ims->Props());
      Imx3duc* const imd=(Imx3duc*)objd[0];
      
      result=PExponentialFiltering(*ims, *imd, (Float)atof(parv[0]));
      goto end;
   }
   if (objs[0]->Type()==Po_Imx3dsl) {
      Imx3dsl* const ims=(Imx3dsl*)objs[0];
      objd[0]=new Imx3dsl(ims->Props());
      Imx3dsl* const imd=(Imx3dsl*)objd[0];
      
      result=PExponentialFiltering(*ims, *imd, (Float)atof(parv[0]));
      goto end;
   }
   if (objs[0]->Type()==Po_Imx3dsf) {
      Imx3dsf* const ims=(Imx3dsf*)objs[0];
      objd[0]=new Imx3dsf(ims->Props());
      Imx3dsf* const imd=(Imx3dsf*)objd[0];
      
      result=PExponentialFiltering(*ims, *imd, (Float)atof(parv[0]));
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
