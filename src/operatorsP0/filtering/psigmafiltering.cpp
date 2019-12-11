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
 */

#include <pandore.h>
using namespace pandore;

/**
 * @file psigmafiltering.cpp
 * On ne prend en compte dans la moyenne que les points
 * dont la difference avec le point central est inferieure
 * a un intervalle donne.Si le nombre de point
 * de la somme n'est pas suffisant (< nbmim),
 * on remplace le point central par la somme de
 * ses 4 voisins, sinon on prend la moyenne.
 * Parametres :	halfsize : Taille de la fenetre de filtrage.
 *		nbmin : Nombre de points minimal acceptes.
 *		eps :Valeur d'ecart tolere avec le centre de la fenetre.
 */



Errc PSigmaFiltering( const Img2duc &ims, Img2duc &imd, int halfsize, int eps, int nbmin ) {
   Point2d p;
   const int nbt=(2*halfsize+1)*(2*halfsize+1);

   // Recopie des bords de l'image source dans l'image destination.
   imd.Frame(ims,halfsize);
  
   for (p.y = halfsize; p.y < ims.Height() - halfsize; p.y++)
   for (p.x = halfsize; p.x < ims.Width() - halfsize; p.x++)
   {
      Img2duc::ValueType inter;
      Float s=0.0;
      Float moy=0.0;
      Long nb=0;
      
      inter = ims[p];
      for (int k=-halfsize; k<=halfsize; k++) {
	 for (int l=-halfsize; l<=halfsize; l++) {
	    moy += (float)ims[p.y+k][p.x+l];
	    if (ABS(ims[p.y+k][p.x+l]-inter) < (Img2duc::ValueType)eps) {
	       s += ims[p.y+k][p.x+l];
	       nb++;
	    }
	 }
      }
      imd[p] = (nb>nbmin)? (Img2duc::ValueType)(s/nb) : (Img2duc::ValueType)(moy/nbt);
   }
   
   return SUCCESS;
}

Errc PSigmaFiltering( const Img2dsl &ims, Img2dsl &imd, int halfsize, int eps, int nbmin ) {
   Point2d p;
   const int nbt=(2*halfsize+1)*(2*halfsize+1);

   // Recopie des bords de l'image source dans l'image destination.
   imd.Frame(ims,halfsize);
  
   for (p.y = halfsize; p.y < ims.Height() - halfsize; p.y++)
   for (p.x = halfsize; p.x < ims.Width() - halfsize; p.x++)
   {
      Img2dsl::ValueType inter;
      Float s=0.0;
      Float moy=0.0;
      Long nb=0;
      
      inter = ims[p];
      for (int k=-halfsize; k<=halfsize; k++) {
	 for (int l=-halfsize; l<=halfsize; l++) {
	    moy += (float)ims[p.y+k][p.x+l];
	    if (ABS(ims[p.y+k][p.x+l]-inter) < (Img2dsl::ValueType)eps) {
	       s += ims[p.y+k][p.x+l];
	       nb++;
	    }
	 }
      }
      imd[p] = (nb>nbmin)? (Img2dsl::ValueType)(s/nb) : (Img2dsl::ValueType)(moy/nbt);
   }
   
   return SUCCESS;
}

Errc PSigmaFiltering( const Img2dsf &ims, Img2dsf &imd, int halfsize, int eps, int nbmin ) {
   Point2d p;
   const int nbt=(2*halfsize+1)*(2*halfsize+1);

   // Recopie des bords de l'image source dans l'image destination.
   imd.Frame(ims,halfsize);
  
   for (p.y = halfsize; p.y < ims.Height() - halfsize; p.y++)
   for (p.x = halfsize; p.x < ims.Width() - halfsize; p.x++)
   {
      Img2dsf::ValueType inter;
      Float s=0.0;
      Float moy=0.0;
      Long nb=0;
      
      inter = ims[p];
      for (int k=-halfsize; k<=halfsize; k++) {
	 for (int l=-halfsize; l<=halfsize; l++) {
	    moy += (float)ims[p.y+k][p.x+l];
	    if (ABS(ims[p.y+k][p.x+l]-inter) < (Img2dsf::ValueType)eps) {
	       s += ims[p.y+k][p.x+l];
	       nb++;
	    }
	 }
      }
      imd[p] = (nb>nbmin)? (Img2dsf::ValueType)(s/nb) : (Img2dsf::ValueType)(moy/nbt);
   }
   
   return SUCCESS;
}

Errc PSigmaFiltering( const Img3duc &ims, Img3duc &imd, int halfsize, int eps, int nbmin ) {
   Point3d p;
   const int nbt=(2*halfsize+1)*(2*halfsize+1);

   // * ecopie des bords de l'image source dans l'image destination.
   imd.Frame(ims,halfsize);
  
   for (p.z = halfsize; p.z < ims.Depth() - halfsize; p.z++)
   for (p.y = halfsize; p.y < ims.Height() - halfsize; p.y++)
   for (p.x = halfsize; p.x < ims.Width() - halfsize; p.x++)
   {
      Img3duc::ValueType inter;
      Float s=0.0;
      Float moy=0.0;
      Long nb=0;
      
      inter = ims[p];
      for (int j=-halfsize; j<=halfsize; j++) {
	 for (int k=-halfsize; k<=halfsize; k++) {
	    for (int l=-halfsize; l<=halfsize; l++) {
	       moy += (float)ims[p.z+j][p.y+k][p.x+l];
	       if (ABS(ims[p.z+j][p.y+k][p.x+l]-inter) < (Img3duc::ValueType)eps) {
		  s += ims[p.z+j][p.y+k][p.x+l];
		  nb++;
	       }
	    }
	 }
      }
      imd[p] = (nb>nbmin)? (Img3duc::ValueType)(s/nb) : (Img3duc::ValueType)(moy/nbt);
   }
   
   return SUCCESS;
}

Errc PSigmaFiltering( const Img3dsl &ims, Img3dsl &imd, int halfsize, int eps, int nbmin ) {
   Point3d p;
   const int nbt=(2*halfsize+1)*(2*halfsize+1);

   // * ecopie des bords de l'image source dans l'image destination.
   imd.Frame(ims,halfsize);
  
   for (p.z = halfsize; p.z < ims.Depth() - halfsize; p.z++)
   for (p.y = halfsize; p.y < ims.Height() - halfsize; p.y++)
   for (p.x = halfsize; p.x < ims.Width() - halfsize; p.x++)
   {
      Img3dsl::ValueType inter;
      Float s=0.0;
      Float moy=0.0;
      Long nb=0;
      
      inter = ims[p];
      for (int j=-halfsize; j<=halfsize; j++) {
	 for (int k=-halfsize; k<=halfsize; k++) {
	    for (int l=-halfsize; l<=halfsize; l++) {
	       moy += (float)ims[p.z+j][p.y+k][p.x+l];
	       if (ABS(ims[p.z+j][p.y+k][p.x+l]-inter) < (Img3dsl::ValueType)eps) {
		  s += ims[p.z+j][p.y+k][p.x+l];
		  nb++;
	       }
	    }
	 }
      }
      imd[p] = (nb>nbmin)? (Img3dsl::ValueType)(s/nb) : (Img3dsl::ValueType)(moy/nbt);
   }
   
   return SUCCESS;
}

Errc PSigmaFiltering( const Img3dsf &ims, Img3dsf &imd, int halfsize, int eps, int nbmin ) {
   Point3d p;
   const int nbt=(2*halfsize+1)*(2*halfsize+1);

   // * ecopie des bords de l'image source dans l'image destination.
   imd.Frame(ims,halfsize);
  
   for (p.z = halfsize; p.z < ims.Depth() - halfsize; p.z++)
   for (p.y = halfsize; p.y < ims.Height() - halfsize; p.y++)
   for (p.x = halfsize; p.x < ims.Width() - halfsize; p.x++)
   {
      Img3dsf::ValueType inter;
      Float s=0.0;
      Float moy=0.0;
      Long nb=0;
      
      inter = ims[p];
      for (int j=-halfsize; j<=halfsize; j++) {
	 for (int k=-halfsize; k<=halfsize; k++) {
	    for (int l=-halfsize; l<=halfsize; l++) {
	       moy += (float)ims[p.z+j][p.y+k][p.x+l];
	       if (ABS(ims[p.z+j][p.y+k][p.x+l]-inter) < (Img3dsf::ValueType)eps) {
		  s += ims[p.z+j][p.y+k][p.x+l];
		  nb++;
	       }
	    }
	 }
      }
      imd[p] = (nb>nbmin)? (Img3dsf::ValueType)(s/nb) : (Img3dsf::ValueType)(moy/nbt);
   }
   
   return SUCCESS;
}


/*
 * Modify only the following constants, and the function call.
 */
#ifdef MAIN
#define	USAGE	"usage: %s halfsize eps nbmin [-m mask] [im_in|-] [im_out|-]"
#define	PARC	3
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
      
      result=PSigmaFiltering(*ims,*imd,atoi(parv[0]),atoi(parv[1]),atoi(parv[2]));
      goto end;
   }
   if (objs[0]->Type()==Po_Img2dsl) {
      Img2dsl* const ims=(Img2dsl*)objs[0];
      objd[0]=new Img2dsl(ims->Props());
      Img2dsl* const imd=(Img2dsl*)objd[0];
      
      result=PSigmaFiltering(*ims,*imd,atoi(parv[0]),atoi(parv[1]),atoi(parv[2]));
      goto end;
   }
   if (objs[0]->Type()==Po_Img2dsf) {
      Img2dsf* const ims=(Img2dsf*)objs[0];
      objd[0]=new Img2dsf(ims->Props());
      Img2dsf* const imd=(Img2dsf*)objd[0];
      
      result=PSigmaFiltering(*ims,*imd,atoi(parv[0]),atoi(parv[1]),atoi(parv[2]));
      goto end;
   }
   if (objs[0]->Type()==Po_Img3duc) {
      Img3duc* const ims=(Img3duc*)objs[0];
      objd[0]=new Img3duc(ims->Props());
      Img3duc* const imd=(Img3duc*)objd[0];
      
      result=PSigmaFiltering(*ims,*imd,atoi(parv[0]),atoi(parv[1]),atoi(parv[2]));
      goto end;
   }
   if (objs[0]->Type()==Po_Img3dsl) {
      Img3dsl* const ims=(Img3dsl*)objs[0];
      objd[0]=new Img3dsl(ims->Props());
      Img3dsl* const imd=(Img3dsl*)objd[0];
      
      result=PSigmaFiltering(*ims,*imd,atoi(parv[0]),atoi(parv[1]),atoi(parv[2]));
      goto end;
   }
   if (objs[0]->Type()==Po_Img3dsf) {
      Img3dsf* const ims=(Img3dsf*)objs[0];
      objd[0]=new Img3dsf(ims->Props());
      Img3dsf* const imd=(Img3dsf*)objd[0];
      
      result=PSigmaFiltering(*ims,*imd,atoi(parv[0]),atoi(parv[1]),atoi(parv[2]));
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

