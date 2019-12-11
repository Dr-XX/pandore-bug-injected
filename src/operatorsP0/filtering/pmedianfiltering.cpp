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
 * @author Julien Robiaille - 2000-06-10
 * @author Regis Clouard - 2002-12-31
 * @author Regis Clouard - 2006-11-23 (add borders).
 */

#include <pandore.h>
using namespace pandore;

/**
 * @file pmedianfiltering.cpp
 * Filtrage median standard (i.e. Non recursif) separable
 * par la methode de l'histogramme.
 */

/*
 * Tri bulle du table "tab" de taille "taille".
 */
void BubbleSort(float *tab, int taille) {    
   for (int i=0;i<taille;i++) {
      for (int j=1;j<taille;j++) {
	 if (tab[j-1]>tab[j]) {
	    float temp=tab[j-1];
	    tab[j-1]=tab[j];
	    tab[j]=temp;
	 }
      }
   }
}

static float *window=NULL;  // Global to avoid multiple reallocations.

/*
 * Median for one line...
 * La valeur mediane est calculee à
 * partir du tri des valeurs des pixels
 * de la fenetre de taille halfsize
 * de chaque cote du pixel central.
 */
Errc MedianLigne(float *entree, float *sortie, int length, int halfsize) {
   register int i;
   
   // Recopie des bords.
   for (i=0; i< halfsize;i++) {
      sortie[i]=entree[i];
      sortie[length-1-i]=entree[length-1-i];
   }
   
   // Classement des pixels de la premiere fenetre.
   for (i=0;i<2*halfsize+1; i++) {
      window[i]=entree[i];
   }
   BubbleSort(window,2*halfsize+1);
   
   // 1er resultat.
   sortie[halfsize] = window[halfsize];
   
   // traitement de tout le tableau en glissant la fenetre.
   for (i=halfsize; i<length-halfsize-1; i++) {
      
      // enlever le pixel gauche de la fenetre.
      float aenlever = entree[i-halfsize];
      
      // introduire le pixel a droite de la fenetre.
      float aajouter = entree[i+halfsize+1];
      
      // tri de la nouvelle fenetre.
      for (int j=0;j<2*halfsize+1;j++) {
	 if (window[j]==aenlever) {
	    window[j]=aajouter;
	    if (aenlever >= aajouter) {
	       for (int k=j-1;k>=0;k--) {
		  if (window[k] > aajouter) {
		     window[k+1]=window[k];
		     window[k]=aajouter;
		  } else
		     break;
	       }
	    } else {
	       for (int k=j+1;k<2*halfsize+1;k++) {
		  if (window[k] < aajouter) {
		     window[k-1]=window[k];
		     window[k]=aajouter;
		  } else
		     break;
	       }
	    }
	    break;
	 }
      }
      
      // resultat du traitement de la fenetre.
      sortie[i+1] = window[halfsize];
   }

   return SUCCESS;
}



Errc PMedianFiltering(Img2duc& ims, Img2duc& imd, int halfsize) {
   register int x,y;
   float *entree, *sortie;
   int size;

   // Verification du parametre.
   size = MIN(ims.Width(),ims.Height());
   if (halfsize*2 >= size) halfsize = size/2  -1;
   if (halfsize == 0) halfsize=1;

   // Reservation memoire pour une ligne d'image.
   size = MAX(ims.Width(),ims.Height());
   if ((!(entree = (float *)malloc(size*sizeof(float)))) || 
       (!(sortie = (float *)malloc(size*sizeof(float))))) {
      std::cerr << "Error pmedianfiltering: Not enough memory"<< std::endl;
      return FAILURE;
   }
   // Reservation de la fenetre de lissage.
   if ((window=(float*)malloc((2*halfsize+1)* sizeof(float)))==NULL) {
      std::cerr << "Error pmedianfiltering: Not enough memory"<< std::endl;
      return FAILURE;
   }

   // Traitement median standard selon les colonnes de l'image
   for (y=0; y<ims.Height(); y++) {
      for (x=0; x<ims.Width(); x++) {
	 entree[x]=(float)ims(y,x);
      }
      MedianLigne(entree,sortie,ims.Width(),halfsize);
      for (x=0; x<ims.Width(); x++) {
      	 imd(y,x)=(Img2duc::ValueType)sortie[x];
      }
   }

   // Traitement median standard selon les lignes de l'image
   for (x=0; x<ims.Width(); x++) {
      for (y=0; y<ims.Height(); y++) {
	 entree[y]=(float)imd(y,x);
      }
      MedianLigne(entree,sortie,ims.Height(),halfsize);
      for (y=0; y<ims.Height(); y++) {
	 imd(y,x)=(Img2duc::ValueType)sortie[y];
      }
   }
   
   free(entree);
   free(sortie);
   free(window);
   
   return SUCCESS;
}

Errc PMedianFiltering(Img2dsl& ims, Img2dsl& imd, int halfsize) {
   register int x,y;
   float *entree, *sortie;
   int size;

   // Verification du parametre.
   size = MIN(ims.Width(),ims.Height());
   if (halfsize*2 >= size) halfsize = size/2  -1;
   if (halfsize == 0) halfsize=1;

   // Reservation memoire pour une ligne d'image.
   size = MAX(ims.Width(),ims.Height());
   if ((!(entree = (float *)malloc(size*sizeof(float)))) || 
       (!(sortie = (float *)malloc(size*sizeof(float))))) {
      std::cerr << "Error pmedianfiltering: Not enough memory"<< std::endl;
      return FAILURE;
   }
   // Reservation de la fenetre de lissage.
   if ((window=(float*)malloc((2*halfsize+1)* sizeof(float)))==NULL) {
      std::cerr << "Error pmedianfiltering: Not enough memory"<< std::endl;
      return FAILURE;
   }

   // Traitement median standard selon les colonnes de l'image
   for (y=0; y<ims.Height(); y++) {
      for (x=0; x<ims.Width(); x++) {
	 entree[x]=(float)ims(y,x);
      }
      MedianLigne(entree,sortie,ims.Width(),halfsize);
      for (x=0; x<ims.Width(); x++) {
      	 imd(y,x)=(Img2dsl::ValueType)sortie[x];
      }
   }

   // Traitement median standard selon les lignes de l'image
   for (x=0; x<ims.Width(); x++) {
      for (y=0; y<ims.Height(); y++) {
	 entree[y]=(float)imd(y,x);
      }
      MedianLigne(entree,sortie,ims.Height(),halfsize);
      for (y=0; y<ims.Height(); y++) {
	 imd(y,x)=(Img2dsl::ValueType)sortie[y];
      }
   }
   
   free(entree);
   free(sortie);
   free(window);
   
   return SUCCESS;
}

Errc PMedianFiltering(Img2dsf& ims, Img2dsf& imd, int halfsize) {
   register int x,y;
   float *entree, *sortie;
   int size;

   // Verification du parametre.
   size = MIN(ims.Width(),ims.Height());
   if (halfsize*2 >= size) halfsize = size/2  -1;
   if (halfsize == 0) halfsize=1;

   // Reservation memoire pour une ligne d'image.
   size = MAX(ims.Width(),ims.Height());
   if ((!(entree = (float *)malloc(size*sizeof(float)))) || 
       (!(sortie = (float *)malloc(size*sizeof(float))))) {
      std::cerr << "Error pmedianfiltering: Not enough memory"<< std::endl;
      return FAILURE;
   }
   // Reservation de la fenetre de lissage.
   if ((window=(float*)malloc((2*halfsize+1)* sizeof(float)))==NULL) {
      std::cerr << "Error pmedianfiltering: Not enough memory"<< std::endl;
      return FAILURE;
   }

   // Traitement median standard selon les colonnes de l'image
   for (y=0; y<ims.Height(); y++) {
      for (x=0; x<ims.Width(); x++) {
	 entree[x]=(float)ims(y,x);
      }
      MedianLigne(entree,sortie,ims.Width(),halfsize);
      for (x=0; x<ims.Width(); x++) {
      	 imd(y,x)=(Img2dsf::ValueType)sortie[x];
      }
   }

   // Traitement median standard selon les lignes de l'image
   for (x=0; x<ims.Width(); x++) {
      for (y=0; y<ims.Height(); y++) {
	 entree[y]=(float)imd(y,x);
      }
      MedianLigne(entree,sortie,ims.Height(),halfsize);
      for (y=0; y<ims.Height(); y++) {
	 imd(y,x)=(Img2dsf::ValueType)sortie[y];
      }
   }
   
   free(entree);
   free(sortie);
   free(window);
   
   return SUCCESS;
}

Errc PMedianFiltering( const Img3duc &ims, Img3duc &imd, int halfsize ) {
   register int x,y,z;
   float *entree, *sortie;
   int size;

   // Verification du parametre.
   size = MIN(ims.Width(),ims.Height());
   size = MIN(size,ims.Depth());
   if (halfsize*2 >= size)  halfsize = size / 2 -1;
   if (halfsize == 0) halfsize=1;
   
   // Reservation memoire pour une ligne d'image.
   size = MAX(ims.Width(),ims.Height());
   size = MAX(size,ims.Depth());
   if ((!(entree = (float *)calloc(size, sizeof(float)))) ||
       (!(sortie = (float *)calloc(size, sizeof(float))))) {
      std::cerr << "Error pmedianfiltering: Not enough memory"<< std::endl;
      return FAILURE;
   }
   // Reservation de la fenetre de lissage.
   if ((window=(float*)malloc((2*halfsize+1)* sizeof(float)))==NULL) {
      std::cerr << "Error pmedianfiltering: Not enough memory"<< std::endl;
      return FAILURE;
   }

   // Traitement median standard selon les colonnes de l'image
   for (z=0; z<ims.Depth(); z++) {
      for (y=0; y<ims.Height(); y++) {
	 for (x=0; x<ims.Width(); x++) {
	    entree[x]=(float)ims(z,y,x);
	 }
	 MedianLigne(entree,sortie,ims.Width(),halfsize);
	 for (x=0; x<ims.Width(); x++) {
	    imd(z,y,x)=(Img3duc::ValueType)sortie[x];
	 }
      }
   }
   // Traitement median standard selon les lignes de l'image
   for (z=0; z<ims.Depth(); z++) {
      for (x=0; x<ims.Width(); x++) {
	 for (y=0; y<ims.Height(); y++) {
	    entree[y]=(float)imd(z,y,x);
	 }
	 MedianLigne(entree,sortie,ims.Height(),halfsize);
	 for (y=0; y<ims.Height(); y++) {
	    imd(z,y,x)=(Img3duc::ValueType)sortie[y];
	 }
      }
   }
   // Traitement median standard selon les coupes de l'image
   for (y=0; y<ims.Height(); y++) {
      for (x=0; x<ims.Width(); x++) {
	 for (z=0; z<ims.Depth(); z++) {
	    entree[z]=(float)imd(z,y,x);
	 }
	 MedianLigne(entree,sortie,ims.Depth(),halfsize);
	 for (z=0; z<ims.Depth(); z++) {
	    imd(z,y,x)=(Img3duc::ValueType)sortie[z];
	 }
      }
   }
   free(entree);
   free(sortie);
   free(window);
  
   return SUCCESS;
}

Errc PMedianFiltering( const Img3dsl &ims, Img3dsl &imd, int halfsize ) {
   register int x,y,z;
   float *entree, *sortie;
   int size;

   // Verification du parametre.
   size = MIN(ims.Width(),ims.Height());
   size = MIN(size,ims.Depth());
   if (halfsize*2 >= size)  halfsize = size / 2 -1;
   if (halfsize == 0) halfsize=1;
   
   // Reservation memoire pour une ligne d'image.
   size = MAX(ims.Width(),ims.Height());
   size = MAX(size,ims.Depth());
   if ((!(entree = (float *)calloc(size, sizeof(float)))) ||
       (!(sortie = (float *)calloc(size, sizeof(float))))) {
      std::cerr << "Error pmedianfiltering: Not enough memory"<< std::endl;
      return FAILURE;
   }
   // Reservation de la fenetre de lissage.
   if ((window=(float*)malloc((2*halfsize+1)* sizeof(float)))==NULL) {
      std::cerr << "Error pmedianfiltering: Not enough memory"<< std::endl;
      return FAILURE;
   }

   // Traitement median standard selon les colonnes de l'image
   for (z=0; z<ims.Depth(); z++) {
      for (y=0; y<ims.Height(); y++) {
	 for (x=0; x<ims.Width(); x++) {
	    entree[x]=(float)ims(z,y,x);
	 }
	 MedianLigne(entree,sortie,ims.Width(),halfsize);
	 for (x=0; x<ims.Width(); x++) {
	    imd(z,y,x)=(Img3dsl::ValueType)sortie[x];
	 }
      }
   }
   // Traitement median standard selon les lignes de l'image
   for (z=0; z<ims.Depth(); z++) {
      for (x=0; x<ims.Width(); x++) {
	 for (y=0; y<ims.Height(); y++) {
	    entree[y]=(float)imd(z,y,x);
	 }
	 MedianLigne(entree,sortie,ims.Height(),halfsize);
	 for (y=0; y<ims.Height(); y++) {
	    imd(z,y,x)=(Img3dsl::ValueType)sortie[y];
	 }
      }
   }
   // Traitement median standard selon les coupes de l'image
   for (y=0; y<ims.Height(); y++) {
      for (x=0; x<ims.Width(); x++) {
	 for (z=0; z<ims.Depth(); z++) {
	    entree[z]=(float)imd(z,y,x);
	 }
	 MedianLigne(entree,sortie,ims.Depth(),halfsize);
	 for (z=0; z<ims.Depth(); z++) {
	    imd(z,y,x)=(Img3dsl::ValueType)sortie[z];
	 }
      }
   }
   free(entree);
   free(sortie);
   free(window);
  
   return SUCCESS;
}

Errc PMedianFiltering( const Img3dsf &ims, Img3dsf &imd, int halfsize ) {
   register int x,y,z;
   float *entree, *sortie;
   int size;

   // Verification du parametre.
   size = MIN(ims.Width(),ims.Height());
   size = MIN(size,ims.Depth());
   if (halfsize*2 >= size)  halfsize = size / 2 -1;
   if (halfsize == 0) halfsize=1;
   
   // Reservation memoire pour une ligne d'image.
   size = MAX(ims.Width(),ims.Height());
   size = MAX(size,ims.Depth());
   if ((!(entree = (float *)calloc(size, sizeof(float)))) ||
       (!(sortie = (float *)calloc(size, sizeof(float))))) {
      std::cerr << "Error pmedianfiltering: Not enough memory"<< std::endl;
      return FAILURE;
   }
   // Reservation de la fenetre de lissage.
   if ((window=(float*)malloc((2*halfsize+1)* sizeof(float)))==NULL) {
      std::cerr << "Error pmedianfiltering: Not enough memory"<< std::endl;
      return FAILURE;
   }

   // Traitement median standard selon les colonnes de l'image
   for (z=0; z<ims.Depth(); z++) {
      for (y=0; y<ims.Height(); y++) {
	 for (x=0; x<ims.Width(); x++) {
	    entree[x]=(float)ims(z,y,x);
	 }
	 MedianLigne(entree,sortie,ims.Width(),halfsize);
	 for (x=0; x<ims.Width(); x++) {
	    imd(z,y,x)=(Img3dsf::ValueType)sortie[x];
	 }
      }
   }
   // Traitement median standard selon les lignes de l'image
   for (z=0; z<ims.Depth(); z++) {
      for (x=0; x<ims.Width(); x++) {
	 for (y=0; y<ims.Height(); y++) {
	    entree[y]=(float)imd(z,y,x);
	 }
	 MedianLigne(entree,sortie,ims.Height(),halfsize);
	 for (y=0; y<ims.Height(); y++) {
	    imd(z,y,x)=(Img3dsf::ValueType)sortie[y];
	 }
      }
   }
   // Traitement median standard selon les coupes de l'image
   for (y=0; y<ims.Height(); y++) {
      for (x=0; x<ims.Width(); x++) {
	 for (z=0; z<ims.Depth(); z++) {
	    entree[z]=(float)imd(z,y,x);
	 }
	 MedianLigne(entree,sortie,ims.Depth(),halfsize);
	 for (z=0; z<ims.Depth(); z++) {
	    imd(z,y,x)=(Img3dsf::ValueType)sortie[z];
	 }
      }
   }
   free(entree);
   free(sortie);
   free(window);
  
   return SUCCESS;
}


/*
 * Modify only the following constants, and the function call.
 */
#ifdef MAIN
#define	USAGE	"usage: %s halfsize [-m mask] [im_in|-] [im_out|-]"
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
      objd[0]=new Img2duc(ims->Size());
      Img2duc* const imd=(Img2duc*)objd[0];
      
      result=PMedianFiltering(*ims,*imd,atoi(parv[0]));
      goto end;
   }
   if (objs[0]->Type()==Po_Img2dsl) {
      Img2dsl* const ims=(Img2dsl*)objs[0];
      objd[0]=new Img2dsl(ims->Size());
      Img2dsl* const imd=(Img2dsl*)objd[0];
      
      result=PMedianFiltering(*ims,*imd,atoi(parv[0]));
      goto end;
   }
   if (objs[0]->Type()==Po_Img2dsf) {
      Img2dsf* const ims=(Img2dsf*)objs[0];
      objd[0]=new Img2dsf(ims->Size());
      Img2dsf* const imd=(Img2dsf*)objd[0];
      
      result=PMedianFiltering(*ims,*imd,atoi(parv[0]));
      goto end;
   }
   if (objs[0]->Type()==Po_Img3duc) {
      Img3duc* const ims=(Img3duc*)objs[0];
      objd[0]=new Img3duc(ims->Size());
      Img3duc* const imd=(Img3duc*)objd[0];
      
      result=PMedianFiltering(*ims,*imd,atoi(parv[0]));
      goto end;
   }
   if (objs[0]->Type()==Po_Img3dsl) {
      Img3dsl* const ims=(Img3dsl*)objs[0];
      objd[0]=new Img3dsl(ims->Size());
      Img3dsl* const imd=(Img3dsl*)objd[0];
      
      result=PMedianFiltering(*ims,*imd,atoi(parv[0]));
      goto end;
   }
   if (objs[0]->Type()==Po_Img3dsf) {
      Img3dsf* const ims=(Img3dsf*)objs[0];
      objd[0]=new Img3dsf(ims->Size());
      Img3dsf* const imd=(Img3dsf*)objd[0];
      
      result=PMedianFiltering(*ims,*imd,atoi(parv[0]));
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
