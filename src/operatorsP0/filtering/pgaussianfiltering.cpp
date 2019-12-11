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
 * @author Régis Clouard - 1996-02-19
 * @author Régis Clouard - 2006-02-09 (add color and multispectral images)
 */

#include <math.h>
#include <pandore.h>
using namespace pandore;

/**
 * @file pgaussianfiltering.cpp
 * Filtering image with gaussian filter.
 *
 * Calcul du masque gaussien a partir de la taille du filtrage.
 * Calcul recursif suivant les lignes puis les colonnes
 * de l'image gradient resultant de la convolution
 * du masque par l'image source.
 * Taille du filtre =  (6*sigma)
 */

#define	SCALE	50.0	// Used for normalization

template < typename T >
Errc PGaussianFiltering1D( const Imx3d<T> &ims, Imx3d<T> &imd, float sigma ) {
   const int halfsize = (int)ceil(sigma * 3.0);

   if (halfsize < 1) {
      std::cerr << "Error pgaussianfiltering: bad parameter value: "<<sigma<< std::endl;
      return FAILURE;
   }
   
   // Calcul du masque gaussien en fonction de la taille.
   Double somme = 0.0;
   const int size = halfsize * 2 + 1;
   double *filter = new double[size];
   for (int i = 0; i < size; i++) {
      double s = SCALE * exp(-((Double)(i - halfsize) * (i - halfsize)
			       / (2.0 * sigma * sigma)));
      somme += s;
      filter[i] = s;
   }
   
   // Rows filtering
   for (int b = 0; b < ims.Bands(); b++) {
      // Left border
      for (int x = 0; x < halfsize; x++) {
	 double s = 0.0;
	 for (int k = 0; k < halfsize + 1 - x; k++) {
	    s += (double)ims(b, 0, 0, x) * filter[k];
	 }
	 for (int k = halfsize + 1 - x; k < size; k++) {
	    s += (double)ims(b, 0, 0, x + k - halfsize) * filter[k];
	 }
	 imd(b, 0, 0, x) = (T)(s / somme);
      }
      // General case
      for (int x = halfsize; x < ims.Width() - halfsize; x++) {
	 double s = 0.0;
	 for (int k = 0; k < size; k++) {
	    s += (double)ims(b, 0, 0, x + k - halfsize) * filter[k];
	 }
	 imd(b, 0, 0, x) = (T)(s / somme);
      }
      // Right border
      for (int x = ims.Width() - halfsize; x < ims.Width(); x++) {
	 double s = 0.0;
	 int l = 1 + x + halfsize - ims.Width();
	 for (int k = 0; k < size - l; k++) {
	    s += (double)ims(b, 0, 0, x + k - halfsize) * filter[k];
	 }
	 for (int k = size - l; k < size; k++) {
	    s += (double)ims(b, 0, 0, x) * filter[k];
	 }
	 imd(b, 0, 0, x) = (T)(s / somme);
      }
   }
   delete[] filter;
   return SUCCESS;
}

template < typename T >
Errc PGaussianFiltering2D( const Imx3d<T> &ims, Imx3d<T> &imd, float sigma ) {
   const int halfsize = (int)ceil(sigma * 3.0);

   if (halfsize < 1) {
      std::cerr << "Error pgaussianfiltering: bad parameter value: "<<sigma<< std::endl;
      return FAILURE;
   }
   
   // Calcul du masque gaussien en fonction de la taille.
   Double somme = 0.0;
   const int size = halfsize * 2 + 1;
   double *filter = new double[size];
   for (int i = 0; i < size; i++) {
      double s= SCALE * exp(-((Double)(i - halfsize) * (i - halfsize)
			      / (2.0 * sigma * sigma)));
      somme += s;
      filter[i] = s;
   }
   
   Imx2dsf imi(ims.Props());	// Intermediate image
   
   // Rows filtering
   for (int b = 0; b < ims.Bands(); b++) {
      for (int y = 0; y < ims.Height(); y++) {
	 // Left border
	 for (int x = 0; x < halfsize; x++) {
	    double s = 0.0;
	    for (int k = 0; k < halfsize + 1 - x; k++) {
	       s += (double)ims(b, 0, y, x) * filter[k];
	    }
	    for (int k = halfsize + 1 - x; k < size; k++) {
	       s += (double)ims(b, 0, y, x + k - halfsize) * filter[k];
	    }
	    imi(b, y, x) = (float)(s / somme);
	 }
	 // General case
	 for (int x = halfsize; x < ims.Width() - halfsize; x++) {
	    double s = 0.0;
	    for (int k = 0; k < size; k++) {
	       s += (double)ims(b, 0, y, x+k-halfsize) * filter[k];
	    }
	    imi(b, y, x) = (float)(s / somme);
	 }
	 // Right border
	 for (int x = ims.Width() - halfsize; x < ims.Width(); x++) {
	    double s = 0.0;
	    int l = 1 + x + halfsize - ims.Width();
	    for (int k = 0; k < size - l; k++) {
	       s += (double)ims(b, 0, y, x + k - halfsize) * filter[k];
	    }
	    for (int k = size - l; k < size; k++) {
	       s += (double)ims(b, 0, y, x) * filter[k];
	    }
	    imi(b, y, x) = (float)(s / somme);
	 }
      }
      
      // Columns filtering
      for (int x = 0; x < imi.Width(); x++) {
	 // Upper border
	 for (int y = 0; y < halfsize; y++) {
	    double s = 0.0;
	    for (int k = 0; k < halfsize + 1 - y; k++) {
	       s += (double)imi(b, y, x) * filter[k];
	    }
	    for (int k=halfsize+1-y; k<size; k++) {
	       s += (double)imi(b, y+k-halfsize, x) * filter[k];
	    }
	    imd(b, 0, y, x) = (T)(s / somme);
	 }
	 // General case
	 for (int y = halfsize; y < imi.Height() - halfsize; y++) {
	    double s = 0.0;
	    for (int k = 0; k < size; k++) {
	       s += imi(b, y + k - halfsize, x) * filter[k];
	    }
	    imd(b, 0, y, x) = (T)(s / somme);
	 }
	 // Lower border
	 for (int y = imi.Height() - halfsize; y < imi.Height(); y++) {
	    double s = 0.0;
	    int l = 1 + y + halfsize - imi.Height();
	    for (int k = 0; k <size - l; k++) {
	       s += (double)imi(b, y+k-halfsize, x) * filter[k];
	    }
	    for (int k = size - l; k < size; k++) {
	       s += (double)imi(b, y, x) * filter[k];
	    }
	    imd(b, 0, y, x) = (T)(s / somme);
	 }
      }
   }
   delete[] filter;
   return SUCCESS;
}

template < typename T >
Errc PGaussianFiltering3D( const Imx3d<T> &ims, Imx3d<T> &imd, float sigma ) {
   const int halfsize = (int)ceil(sigma * 3.0);
   
   if (halfsize < 1) {
      std::cerr << "Error pgaussianfiltering: bad parameter value: "<<sigma<< std::endl;
      return FAILURE;
   }

   // Calcul du masque gaussien en fonction de la size.
   const int size = halfsize * 2 + 1;
   double *filter= new double[size];
   double somme = 0.0;
   for (int i = 0; i < size; i++) {
      double s = SCALE * exp(-((double)(i - halfsize) * (i - halfsize)
			       / (2.0 * sigma * sigma)));
      somme += s;
      filter[i] = s;
   }

   Imx3dsf imi1(ims.Props());	// Image intermediaire
   Imx3dsf imi2(ims.Props());	// Image intermediaire

   for (int b = 0;  b < ims.Bands(); b++) {
      // Filtrage largeur.
      // Gestion des bords en coupant.
      for (int z = 0; z < ims.Depth(); z++) {
	 for (int y = 0; y < ims.Height(); y++) {
	    for (int x = 0; x < ims.Width(); x++) {
	       double s = 0.0;
	       for (int k = 0; k < size; k++) {
		  int x1 = x + k - halfsize ;
		  if (x1 >= 0 && x1 < ims.Width()) {
		     s += (double)ims(b, z, y, x1) * filter[k];
		  }
	       }
	       imi1(b, z, y, x)  = (float)(s / somme);
	    }
	 }
      }
      // Filtrage profondeur.
      // Gestion des bords en coupant.
      for (int z = 0; z < imi1.Depth(); z++) {
	 for (int y = 0; y < imi1.Height(); y++) {
	    for (int x = 0; x < imi1.Width(); x++) {
	       double s = 0.0;
	       for (int k = 0; k < size; k++) {
		  int y1 = y + k - halfsize;
		  if (y1 >= 0 && y1 < imi1.Height()) {
		     s += (double)imi1(b, z, y1, x) * filter[k];
		  }
	       }
	       imi2(b, z, y, x)  = (float)(s / somme);
	    }
	 }
      }
      
      // Filtrage profondeur.
      // Gestion des bords en coupant.
      for (int z = 0; z < imi2.Depth(); z++) {
	 for (int y = 0; y < imi2.Height(); y++) {
	    for (int x = 0; x < imi2.Width(); x++) {
	       double s = 0.0;
	       for (int k = 0; k < size; k++) {
		  int z1 = z + k - halfsize;
		  if (z1 >= 0 && z1 < imi2.Depth()) {
		     s += (double)imi2(b, z1, y, x) * filter[k];
		  }
	       }
	       imd(b, z, y, x) = (T)(s / somme);
	    }
	 }
      }
   }
   delete[] filter;
   return SUCCESS;
}




Errc PGaussianFiltering( const Img1duc &ims, Img1duc &imd, float sigma ) {
   return PGaussianFiltering1D( ims, imd, sigma );
}

Errc PGaussianFiltering( const Img1dsl &ims, Img1dsl &imd, float sigma ) {
   return PGaussianFiltering1D( ims, imd, sigma );
}

Errc PGaussianFiltering( const Img1dsf &ims, Img1dsf &imd, float sigma ) {
   return PGaussianFiltering1D( ims, imd, sigma );
}

Errc PGaussianFiltering( const Img2duc &ims, Img2duc &imd, float sigma ) {
   return PGaussianFiltering2D( ims, imd, sigma );
}

Errc PGaussianFiltering( const Img2dsl &ims, Img2dsl &imd, float sigma ) {
   return PGaussianFiltering2D( ims, imd, sigma );
}

Errc PGaussianFiltering( const Img2dsf &ims, Img2dsf &imd, float sigma ) {
   return PGaussianFiltering2D( ims, imd, sigma );
}

Errc PGaussianFiltering( const Imc2duc &ims, Imc2duc &imd, float sigma ) {
   return PGaussianFiltering2D( ims, imd, sigma );
}

Errc PGaussianFiltering( const Imc2dsl &ims, Imc2dsl &imd, float sigma ) {
   return PGaussianFiltering2D( ims, imd, sigma );
}

Errc PGaussianFiltering( const Imc2dsf &ims, Imc2dsf &imd, float sigma ) {
   return PGaussianFiltering2D( ims, imd, sigma );
}

Errc PGaussianFiltering( const Imx2duc &ims, Imx2duc &imd, float sigma ) {
   return PGaussianFiltering2D( ims, imd, sigma );
}

Errc PGaussianFiltering( const Imx2dsl &ims, Imx2dsl &imd, float sigma ) {
   return PGaussianFiltering2D( ims, imd, sigma );
}

Errc PGaussianFiltering( const Imx2dsf &ims, Imx2dsf &imd, float sigma ) {
   return PGaussianFiltering2D( ims, imd, sigma );
}

Errc PGaussianFiltering( const Img3duc &ims, Img3duc &imd, float sigma ) {
   return PGaussianFiltering3D( ims, imd, sigma );
}

Errc PGaussianFiltering( const Img3dsl &ims, Img3dsl &imd, float sigma ) {
   return PGaussianFiltering3D( ims, imd, sigma );
}

Errc PGaussianFiltering( const Img3dsf &ims, Img3dsf &imd, float sigma ) {
   return PGaussianFiltering3D( ims, imd, sigma );
}

Errc PGaussianFiltering( const Imc3duc &ims, Imc3duc &imd, float sigma ) {
   return PGaussianFiltering3D( ims, imd, sigma );
}

Errc PGaussianFiltering( const Imc3dsl &ims, Imc3dsl &imd, float sigma ) {
   return PGaussianFiltering3D( ims, imd, sigma );
}

Errc PGaussianFiltering( const Imc3dsf &ims, Imc3dsf &imd, float sigma ) {
   return PGaussianFiltering3D( ims, imd, sigma );
}

Errc PGaussianFiltering( const Imx3duc &ims, Imx3duc &imd, float sigma ) {
   return PGaussianFiltering3D( ims, imd, sigma );
}

Errc PGaussianFiltering( const Imx3dsl &ims, Imx3dsl &imd, float sigma ) {
   return PGaussianFiltering3D( ims, imd, sigma );
}

Errc PGaussianFiltering( const Imx3dsf &ims, Imx3dsf &imd, float sigma ) {
   return PGaussianFiltering3D( ims, imd, sigma );
}


#ifdef MAIN
#define USAGE	"usage: %s sigma [-m mask] [im_in|-] [im_out|-]"
#define PARC	1
#define FINC	1
#define FOUTC	1
#define MASK	2

int main( int argc, char *argv[] ) {
   Errc result;                // The result code of the execution.
   Pobject* mask;              // The region map.
   Pobject* objin[FINC + 1];   // The input objects.
   Pobject* objs[FINC + 1];    // The source objects masked.
   Pobject* objout[FOUTC + 1]; // The output object.
   Pobject* objd[FOUTC + 1];   // The result object of the execution.
   char* parv[PARC + 1];       // The input parameters.

   ReadArgs(argc, argv, PARC, FINC, FOUTC, &mask, objin, objs, objout, objd, parv, USAGE, MASK); 
   if (objs[0]->Type() == Po_Img1duc) {
      Img1duc* const ims = (Img1duc*)objs[0];
      objd[0] = new Img1duc(ims->Props());
      Img1duc * const imd = (Img1duc*)objd[0];
      
      result = PGaussianFiltering(*ims, *imd, (Float)atof(parv[0]));
      goto end;
   }
   if (objs[0]->Type() == Po_Img1dsl) {
      Img1dsl* const ims = (Img1dsl*)objs[0];
      objd[0] = new Img1dsl(ims->Props());
      Img1dsl * const imd = (Img1dsl*)objd[0];
      
      result = PGaussianFiltering(*ims, *imd, (Float)atof(parv[0]));
      goto end;
   }
   if (objs[0]->Type() == Po_Img1dsf) {
      Img1dsf* const ims = (Img1dsf*)objs[0];
      objd[0] = new Img1dsf(ims->Props());
      Img1dsf * const imd = (Img1dsf*)objd[0];
      
      result = PGaussianFiltering(*ims, *imd, (Float)atof(parv[0]));
      goto end;
   }
   if (objs[0]->Type() == Po_Img2duc) {
      Img2duc* const ims = (Img2duc*)objs[0];
      objd[0] = new Img2duc(ims->Props());
      Img2duc * const imd = (Img2duc*)objd[0];
      
      result = PGaussianFiltering(*ims, *imd, (Float)atof(parv[0]));
      goto end;
   }
   if (objs[0]->Type() == Po_Img2dsl) {
      Img2dsl* const ims = (Img2dsl*)objs[0];
      objd[0] = new Img2dsl(ims->Props());
      Img2dsl * const imd = (Img2dsl*)objd[0];
      
      result = PGaussianFiltering(*ims, *imd, (Float)atof(parv[0]));
      goto end;
   }
   if (objs[0]->Type() == Po_Img2dsf) {
      Img2dsf* const ims = (Img2dsf*)objs[0];
      objd[0] = new Img2dsf(ims->Props());
      Img2dsf * const imd = (Img2dsf*)objd[0];
      
      result = PGaussianFiltering(*ims, *imd, (Float)atof(parv[0]));
      goto end;
   }
   if (objs[0]->Type() == Po_Imc2duc) {
      Imc2duc* const ims = (Imc2duc*)objs[0];
      objd[0] = new Imc2duc(ims->Props());
      Imc2duc * const imd = (Imc2duc*)objd[0];
      
      result = PGaussianFiltering(*ims, *imd, (Float)atof(parv[0]));
      goto end;
   }
   if (objs[0]->Type() == Po_Imc2dsl) {
      Imc2dsl* const ims = (Imc2dsl*)objs[0];
      objd[0] = new Imc2dsl(ims->Props());
      Imc2dsl * const imd = (Imc2dsl*)objd[0];
      
      result = PGaussianFiltering(*ims, *imd, (Float)atof(parv[0]));
      goto end;
   }
   if (objs[0]->Type() == Po_Imc2dsf) {
      Imc2dsf* const ims = (Imc2dsf*)objs[0];
      objd[0] = new Imc2dsf(ims->Props());
      Imc2dsf * const imd = (Imc2dsf*)objd[0];
      
      result = PGaussianFiltering(*ims, *imd, (Float)atof(parv[0]));
      goto end;
   }
   if (objs[0]->Type() == Po_Imx2duc) {
      Imx2duc* const ims = (Imx2duc*)objs[0];
      objd[0] = new Imx2duc(ims->Props());
      Imx2duc * const imd = (Imx2duc*)objd[0];
      
      result = PGaussianFiltering(*ims, *imd, (Float)atof(parv[0]));
      goto end;
   }
   if (objs[0]->Type() == Po_Imx2dsl) {
      Imx2dsl* const ims = (Imx2dsl*)objs[0];
      objd[0] = new Imx2dsl(ims->Props());
      Imx2dsl * const imd = (Imx2dsl*)objd[0];
      
      result = PGaussianFiltering(*ims, *imd, (Float)atof(parv[0]));
      goto end;
   }
   if (objs[0]->Type() == Po_Imx2dsf) {
      Imx2dsf* const ims = (Imx2dsf*)objs[0];
      objd[0] = new Imx2dsf(ims->Props());
      Imx2dsf * const imd = (Imx2dsf*)objd[0];
      
      result = PGaussianFiltering(*ims, *imd, (Float)atof(parv[0]));
      goto end;
   }
   if (objs[0]->Type() == Po_Img3duc) {
      Img3duc* const ims = (Img3duc*)objs[0];
      objd[0] = new Img3duc(ims->Props());
      Img3duc * const imd = (Img3duc*)objd[0];
      
      result = PGaussianFiltering(*ims, *imd, (Float)atof(parv[0]));
      goto end;
   }
   if (objs[0]->Type() == Po_Img3dsl) {
      Img3dsl* const ims = (Img3dsl*)objs[0];
      objd[0] = new Img3dsl(ims->Props());
      Img3dsl * const imd = (Img3dsl*)objd[0];
      
      result = PGaussianFiltering(*ims, *imd, (Float)atof(parv[0]));
      goto end;
   }
   if (objs[0]->Type() == Po_Img3dsf) {
      Img3dsf* const ims = (Img3dsf*)objs[0];
      objd[0] = new Img3dsf(ims->Props());
      Img3dsf * const imd = (Img3dsf*)objd[0];
      
      result = PGaussianFiltering(*ims, *imd, (Float)atof(parv[0]));
      goto end;
   }
   if (objs[0]->Type() == Po_Imc3duc) {
      Imc3duc* const ims = (Imc3duc*)objs[0];
      objd[0] = new Imc3duc(ims->Props());
      Imc3duc * const imd = (Imc3duc*)objd[0];
      
      result = PGaussianFiltering(*ims, *imd, (Float)atof(parv[0]));
      goto end;
   }
   if (objs[0]->Type() == Po_Imc3dsl) {
      Imc3dsl* const ims = (Imc3dsl*)objs[0];
      objd[0] = new Imc3dsl(ims->Props());
      Imc3dsl * const imd = (Imc3dsl*)objd[0];
      
      result = PGaussianFiltering(*ims, *imd, (Float)atof(parv[0]));
      goto end;
   }
   if (objs[0]->Type() == Po_Imc3dsf) {
      Imc3dsf* const ims = (Imc3dsf*)objs[0];
      objd[0] = new Imc3dsf(ims->Props());
      Imc3dsf * const imd = (Imc3dsf*)objd[0];
      
      result = PGaussianFiltering(*ims, *imd, (Float)atof(parv[0]));
      goto end;
   }
   if (objs[0]->Type() == Po_Imx3duc) {
      Imx3duc* const ims = (Imx3duc*)objs[0];
      objd[0] = new Imx3duc(ims->Props());
      Imx3duc * const imd = (Imx3duc*)objd[0];
      
      result = PGaussianFiltering(*ims, *imd, (Float)atof(parv[0]));
      goto end;
   }
   if (objs[0]->Type() == Po_Imx3dsl) {
      Imx3dsl* const ims = (Imx3dsl*)objs[0];
      objd[0] = new Imx3dsl(ims->Props());
      Imx3dsl * const imd = (Imx3dsl*)objd[0];
      
      result = PGaussianFiltering(*ims, *imd, (Float)atof(parv[0]));
      goto end;
   }
   if (objs[0]->Type() == Po_Imx3dsf) {
      Imx3dsf* const ims = (Imx3dsf*)objs[0];
      objd[0] = new Imx3dsf(ims->Props());
      Imx3dsf * const imd = (Imx3dsf*)objd[0];
      
      result = PGaussianFiltering(*ims, *imd, (Float)atof(parv[0]));
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
