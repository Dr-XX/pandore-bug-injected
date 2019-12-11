/* -*- c-basic-offset: 3 -*-
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
 * @author Jalal Fadili - 2000-11-03
 */

#include <math.h>
#include <pandore.h>
using namespace pandore;

/*
 * @file pfuzzyclustering.cpp
 * c : nbre de classes
 * m : [1..2] parametre definissant le degre de flou
 * (m=1 pas de flou, m=2 maximum).
 */

#define Tab2d( tab, h, w, tp ) {					\
	(tab) = (tp**)malloc((unsigned int)(h) * sizeof(tp*));    \
	*(tab) = (tp*)calloc((unsigned int)(h * w), sizeof(tp));	\
 	for (unsigned int i = 1; i <(unsigned int)h;i++) *(tab + i) = *(tab + i - 1) + w; }

inline Float** Tab2dsf( Ulong nrow, Ulong ncol ) {
   Float** tab;
   Tab2d(tab, nrow, ncol, Float);
   return tab;
}

inline void Free( Float** tab ) {
   free((char*)*tab);
   free((char*)tab);
}

/*
 *  Moyenne.
 */
float moyenne( float data[], int nbcomp ) {
   float ave = 0.0;
   for (int j = 0; j < nbcomp; j++) {
      ave += data[j];
   }

   if (nbcomp != 0) {
      return ave / nbcomp;
   }
   return 0.0F;
}

/*
 *  Moyenne.
 */
float variance( float data[], int nbcomp ) {
   int j;
   float s, ave;

   s = ave = 0.0;
   for (j = 0; j < nbcomp; j++) {
      ave += data[j];
      s += data[j] * data[j];
   }
   if (nbcomp != 0) {
      ave /= nbcomp;
      return (s - (nbcomp * ave * ave)) / nbcomp;
   }
   return 0.0F;
}

/*
 * Calcul distance au centroide.
 */
Double distance( float** V, float** X, int noclus, int nopix, int nbcomp, int nodist ) {
   double som1 = 0.0;
   float covar, ave1, ave2, svar1, svar2;
   int comp;

   switch (nodist) {
   case 1: // Distance de Mahalanobis (avec s=2=> euclidienne) non normalisee.
      for (comp = 0; comp < nbcomp; comp++) {
	 som1 += (V[noclus][comp] - X[nopix][comp]) * (V[noclus][comp] - X[nopix][comp]);
      }
      break;
   case 2:	// Distance euclidienne normalisee: deuc^2=2*(1-cc)
      for (comp = 0; comp < nbcomp; comp++) {
	 som1 += (V[noclus][comp] - X[nopix][comp]) * (V[noclus][comp] - X[nopix][comp]);
      }
      break;
   case 0: // Distance de correlation hyperbolique
      ave2 = moyenne(V[noclus - 1], nbcomp);
      svar2 = variance(V[noclus - 1], nbcomp);
      ave1 = moyenne(X[nopix - 1], nbcomp);
      svar1 = variance(X[nopix - 1], nbcomp);

      for (comp = 0, covar = 0; comp < nbcomp ; comp++) {
	 covar += (V[noclus - 1][comp]-ave2) * (X[nopix - 1][comp] - ave1);
      }
      covar /= ((nbcomp - 1) * sqrt(svar1 * svar2));
      if (!svar2)  {
	 return MAXFLOAT;
      }
      som1 = pow((1. - covar) / (1. + covar), 2.);
   default: break;
   }
   return som1;
}

/*
 * Calcule de la valeur du u[i][k] suivant.
 * m: degre de flou.
 */
float uik_new(float** X, float** V, int i, int k, int nbclus, int nbcomp, float m, int nodist) {
   double som, di, dj;
   int j, minc;

   if (!(di = distance(V, X, i, k, nbcomp, nodist))) {
      return 1.0f;
   }
   if (m > 1) { // If degre de flou > 1
      for (j = 0, som = 0; j < nbclus; j++) {
	 if (j != i) {
	    dj = distance(V, X, j, k, nbcomp, nodist);
				
	    if (di && dj) {
	       som += pow( di / dj, 1. / (m - 1));
	    } else {
	       if (!dj) {
		  return(0);
	       }
	    }
	 } else {
	    som += 1.0f;
	 }
      }
   } else {
      for (j = 0, minc = i; j < nbclus; j++) {
	 if (j != i) {
	    dj = distance(V, X, j, k, nbcomp, nodist);
	    if (dj < di) {
	       minc = j;
	    }
	 }
      }
      if (minc == i) {
	 return 1.0f;
      } else {
	 return 0.0f;
      }
   }
	
   return (float)(1.0f / som);
}

/*
 * Calcul du Vil suivant.
 */
float vil_new( float** U, float** X, int i, int l, int nbpix, float m ) {
   float som1, som2, uikm;
   int k;
   
   for (k = 0, som1 = som2 = 0; k < nbpix; k++) {
      uikm = (Float)pow(U[i][k], m);
      som1 += uikm * X[k][l];
      som2 += uikm;
   }
   return som1 / som2;
}

/*
 * Unsupervised Fuzzy Cluster Analysis;
 * nbcluster: number of clusters.
 */
void UFCA( float** X, float** U, float** V, int nbpix, int nbclus, int nbcomp, float m, float epsilon, int nodist ) {
   float unew, max, J;
   int i, k, l, nit;

   for (nit = 1, max = 1; max >= epsilon; nit++) {
      for (i = 0; i < nbclus; i++) {
	 for (l = 0; l < nbcomp ; l++) {
	    V[i][l] = vil_new(U, X, i, l, nbpix, m);
	 }
      }
      for (i = 0, J = 0, max = MINFLOAT; i < nbclus; i++) {
	 for (k = 0; k < nbpix; k++) {
	    unew = uik_new(X, V, i, k, nbclus, nbcomp, m, nodist);
	    if (unew < 0. || unew > 1.) {
	       break;
	    }
	    max = (fabs(unew - U[i][k]) > max ? fabs(unew - U[i][k]) : max);
	    U[i][k] = unew;
	    J += U[i][k]*(float)distance(V, X, i, k, nbcomp, nodist);
	 }
      }
   }
}

/*
 * Initialisation des degres d'appartenance.
 * 2 methodes possibles (nomethode = 1 ou 2).
 * nomethode = 1 : initialisation aleatoire.
 * nomethode = 2 : initialisation a partir des vecteurs initiaux vinit.
 * Resultat dans u.
 */
float** Initialisation( float** X, int nbclus, int nbpix, int nbcomp, float m, float** Vinit, int nodist, int nomethode ) {
   float **U, som, ran = 0.F;
   int i, k;
   
   U = Tab2dsf(nbclus, nbpix);
   switch (nomethode) {
   case 1: // Methode aleatoire.
      srand(1);
      for (k = 0; k < nbpix ; k++) {
	 for (i = 0, som= 0; i < nbclus; i++) {
	    ran = (float)rand();
	    som += ran;
	    U[i][k] = ran;
	 }
	 for (i = 0; i < nbclus; i++) {
	    U[i][k] /= som;
	 }
      }
      break;
   case 2: // A partir des vecteurs initiaux Vinit
      for (k = 0; k < nbpix ; k++) {
	 for (i = 0 ; i < nbclus; i++) {
	    U[i][k] = uik_new(X, Vinit, i, k, nbclus, nbcomp, m, nodist);
	 }
      }
      break;
   }
   return U;
}

/*
 * Classifieur de X dans U.
 * Parametres :
 * @param X : tableau [pixel][composant]
 * Nombre de cluster a priori *c
 * epsilon : seuil d'erreur accepte.
 * m : Fuzziness index
 * distype= 0 : Modified correlation; distantype = 1 distance euclidienne.
 */
float** PFuzzyClusterAnalysis( float** X, float** Vinit, int npixels, int p, int nbclus, float m, float epsilon, int nodist) {
   float** U, ** Vtmp;
   
   Vtmp = Tab2dsf(nbclus, p);
   
   // Initialisation des degres d'appartenance pour chaque pixel (methode aleatoire).
   U = Initialisation(X, nbclus, npixels, p, m, Vinit, nodist, 1);

   // Calcul des valeurs d'appartenance.
   UFCA(X, U, Vtmp, npixels, nbclus, p, m, epsilon, nodist);

   Free(Vtmp);
   return(U);
}

/*
 * nbcluster: Le nombre de classes
 * m : [1..2] parametre definissant le degre de flou
 * (m=1 pas de flou, m=2 maximum).
 */
Errc PFuzzyClustering( Imc2duc& ims, Reg2d& rgd, int nbclus, float m ) {
   float** X, **U, max;
   float** InitV; 
   int nbpix, k, p, imax;
   Point2d pt;
   const int nbcomp = 3; // Nombre de composante du vecteur de caracteristiques (ici 3 d'ou Imc2dsf).

   nbpix = ims.VectorSize();
   X = Tab2dsf(nbpix, nbcomp);
   InitV = Tab2dsf(nbclus, nbcomp);

   // Initialisation des centres de gravite des classes
   // par repartition sur l'espace des niveaux de gris.
   float v = (float)(Limits<Imc2duc::ValueType>::max() / nbclus);
   max = v;
   for (k = 0; k < nbclus; k++) {
      for (p = 0; p < nbcomp; p++) {
	 InitV[k][p] = max;
      }
      max+=v;
   }

   // Construction du vecteur X avec l'image initiale.
   for (pt.y = 0; pt.y < ims.Height(); pt.y++) {
      for (pt.x = 0; pt.x < ims.Width(); pt.x++) {
	 X[pt.y * ims.Width() + pt.x][0] = ims.X[pt];
	 X[pt.y * ims.Width() + pt.x][1] = ims.Y[pt];
	 X[pt.y * ims.Width() + pt.x][2] = ims.Z[pt];
      }
   }
   
   // Fuzzyfication de X dans U.
   U =  PFuzzyClusterAnalysis(X, InitV, nbpix, nbcomp, nbclus, m, 0.01F, 1 /* distance  Mahalanobis.*/);

   // Construction de la carte de sortie.
   int px = 0;
   for (pt.y = 0; pt.y < ims.Height(); pt.y++) {
      for (pt.x = 0; pt.x < ims.Width(); pt.x++, px++) {
	 imax = 0;
	 max = U[0][px];
	 // Recherche de la classe du pixel pt.
	 for (k = 1; k < nbclus; k++) {
	    if (U[k][px] > max) {
	       max = U[k][px];
	       imax = k;
	    }
	 }
	 rgd[pt] = (Reg2d::ValueType)imax + 1;
      }
   }
   rgd.Labels(nbclus);

   Free(X);
   Free(U);
   Free(InitV);

   return SUCCESS;
}

/*
 * @param nbclus: Le nombre de classes
 * @param m : [1..2] parametre definissant le degre de flou
 * (m=1 pas de flou, m=2 maximum).
 */
Errc PFuzzyClustering( Img2duc& ims, Reg2d &rgd, int nbclus, float m ) {
   float** X, **U, max;
   float** InitV; 
   int nbpix, k, p, imax;
   const int nbcomp = 1; // Nombre de composante du vecteur de caracteristiques (ici 3 d'ou Img2dsf).

   nbpix = ims.VectorSize();
   X = Tab2dsf(nbpix, nbcomp);
   InitV = Tab2dsf(nbclus, nbcomp);

   // Initialisation des centres de gravite des classes
   // par repartition sur l'espace des niveaux de gris.
   float v = (float)(Limits<Img2duc::ValueType>::max() / nbclus);
   max = v;
   for (k = 0; k < nbclus; k++) {
      for (p = 0; p < nbcomp; p++) {
	 InitV[k][p] = max;
      }
      max += v;
   }
   
   // Construction du vecteur X avec l'image initiale.
   int px;
   Img2duc::ValueType *pims = ims.Vector();
   for (px = 0; px < nbpix;) {
      X[px++][0] = *(pims++);
   }
   // Fuzzyfication de X dans U.
   U =  PFuzzyClusterAnalysis(X, InitV, nbpix, nbcomp, nbclus, m, 0.01F, 1);

   Reg2d::ValueType *prgd = rgd.Vector();
   for (px = 0; px < nbpix; px++) {
      imax = 0;
      max = U[0][px];
      // Recherche de la classe du pixel pt.
      for (k = 1; k < nbclus; k++) {
	 if (U[k][px] > max) {
	    max = U[k][px];
	    imax = k;
	 }
      }
      *(prgd++) = (Reg2d::ValueType)imax + 1;
   }
   rgd.Labels(nbclus);

   Free(X);
   Free(U);
   Free(InitV);
   return SUCCESS;
}

/*
 * @param nbclus: Le nombre de classes
 * @param m : [1..2] parametre definissant le degre de flou
 * (m=1 pas de flou, m=2 maximum).
 */
Errc PFuzzyClustering( Img3duc& ims, Reg3d &rgd, int nbclus, float m ) {
   float** X, **U, max;
   float** InitV; 
   int nbpix, k, p, imax;
   const int nbcomp = 1; // Nombre de composante du vecteur de caracteristiques (ici 3 d'ou Img2dsf).

   nbpix = ims.VectorSize();
   X = Tab2dsf(nbpix, nbcomp);
   InitV = Tab2dsf(nbclus, nbcomp);

   // Initialisation des centres de gravite des classes
   // par repartition sur l'espace des niveaux de gris.
   float v = (float)(Limits<Img3duc::ValueType>::max() / nbclus);
   max = v;
   for (k = 0; k < nbclus; k++) {
      for (p = 0; p < nbcomp; p++) {
	 InitV[k][p] = max;
      }
      max += v;
   }
   // Construction du vecteur X avec l'image initiale.
   int px;
   Img3duc::ValueType *pims = ims.Vector();
   for (px = 0; px < nbpix;) {
      X[px++][0] = *(pims++);
   }
   // Fuzzyfication de X dans U.
   U = PFuzzyClusterAnalysis(X, InitV, nbpix, nbcomp, nbclus, m, 0.01F, 1);

   Reg3d::ValueType *prgd = rgd.Vector();
   for (px = 0; px < nbpix; px++) {
      imax = 0;
      max = U[0][px];
      // Recherche de la classe du pixel pt.
      for (k = 1; k < nbclus; k++) {
	 if (U[k][px] > max) {
	    max = U[k][px];
	    imax = k;
	 }
      }
      *(prgd++) = (Reg3d::ValueType)imax + 1;
   }
   rgd.Labels(nbclus);
   
   Free(X);
   Free(U);
   Free(InitV);
   return SUCCESS;
}

/*
 * @param nbcluster: Le nombre de classes
 * @param m : [1..2] parametre definissant le degre de flou
 * (m=1 pas de flou, m=2 maximum).
 */
Errc PFuzzyClustering( Imc3duc& ims, Reg3d& rgd, int nbclus, float m ) {
   float** X, **U, max;
   float** InitV; 
   int nbpix, k, p, imax;
   Point3d pt;
   const int nbcomp = 3; // Nombre de composante du vecteur de caracteristiques (ici 3 d'ou Imc2dsf).

   nbpix = ims.VectorSize();
   X = Tab2dsf(nbpix, nbcomp);
   InitV = Tab2dsf(nbclus, nbcomp);

   // Initialisation des centres de gravite des classes
   // par repartition sur l'espace des niveaux de gris.
   float v = (float)(Limits<Imc3duc::ValueType>::max() / nbclus);
   max = v;
   for (k = 0; k < nbclus; k++) {
      for (p = 0; p < nbcomp;p++) {
	 InitV[k][p] = max;
      }
      max += v;
   }

   // Construction du vecteur X avec l'image initiale.
   for (pt.z = 0; pt.z < ims.Depth(); pt.z++) {
      for (pt.y = 0;pt.y < ims.Height(); pt.y++) {
	 for (pt.x = 0; pt.x < ims.Width(); pt.x++) {
	    X[pt.y * ims.Width() + pt.x][0] = ims.X[pt];
	    X[pt.y * ims.Width() + pt.x][1] = ims.Y[pt];
	    X[pt.y * ims.Width() + pt.x][2] = ims.Z[pt];
	 }
      }
   }
   
   // Fuzzyfication de X dans U.
   U = PFuzzyClusterAnalysis(X, InitV, nbpix, nbcomp, nbclus, m, 0.01F, 1 /* distance  Mahalanobis.*/);
   
   // Construction de la carte de sortie.
   int px = 0;
   for (pt.z = 0; pt.z < ims.Depth(); pt.z++) {
      for (pt.y = 0;pt.y < ims.Height(); pt.y++) {
	 for (pt.x = 0;pt.x < ims.Width(); pt.x++, px++) {
	    imax = 0;
	    max = U[0][px];
	    // Recherche de la classe du pixel pt.
	    for (k = 1; k < nbclus; k++) {
	       if (U[k][px] > max) {
		  max = U[k][px];
		  imax = k;
	       }
	    }
	    rgd[pt] = (Reg3d::ValueType)imax + 1;
	 }
      }
   }
   rgd.Labels(nbclus);
   
   Free(X);
   Free(U);
   Free(InitV);
 
   return SUCCESS;
}

template <typename T>
Errc Normalisation( const Imx3d<T> &ims, Imx3d<Uchar> &imd, float min, float max ) {
   T maxi, mini;
   Float a, b;
   
   // Recherche du min et max de l'image.
   for (int i = 0; i < ims.Bands(); i++) {
      T *ps = ims.Vector(i);
      maxi = mini = *(ps++);
      for (; ps < ims.Vector(i) + ims.VectorSize(); ps++) {
	 maxi = MAX(*ps, maxi);
	 mini = MIN(*ps, mini);
      }
      a = (Float)((max - min) / (Float)(maxi - mini));
      b = (Float)(((min * maxi) - (max * mini)) / (Float)(maxi - mini));
      
      // Normalisation des pixels.
      Uchar *pd = imd.Vector(i);
      ps=ims.Vector(i);
      for (; ps < ims.Vector(i) + ims.VectorSize(); ps++, pd++) {
	 *pd = (Uchar)(a* (*ps) + b);
      }
   }
   return SUCCESS;
}

Errc PFuzzyClustering( const Img2dsl &ims, Reg2d &rgd, int nbclus, float m ) {
   Img2duc ims1(ims.Size());
   Normalisation(ims, ims1, 0.0F, 255.0F);
   PFuzzyClustering(ims1, rgd, nbclus, m);
   return SUCCESS;
}

Errc PFuzzyClustering( const Img2dsf &ims, Reg2d &rgd, int nbclus, float m ) {
   Img2duc ims1(ims.Size());
   Normalisation(ims, ims1, 0.0F, 255.0F);
   PFuzzyClustering(ims1, rgd, nbclus, m);
   return SUCCESS;
}

Errc PFuzzyClustering( const Imc2dsl &ims, Reg2d &rgd, int nbclus, float m ) {
   Imc2duc ims1(ims.Size());
   Normalisation(ims, ims1, 0.0F, 255.0F);
   PFuzzyClustering(ims1, rgd, nbclus, m);
   return SUCCESS;
}

Errc PFuzzyClustering( const Imc2dsf &ims, Reg2d &rgd, int nbclus, float m ) {
   Imc2duc ims1(ims.Size());
   Normalisation(ims, ims1, 0.0F, 255.0F);
   PFuzzyClustering(ims1, rgd, nbclus, m);
   return SUCCESS;
}

Errc PFuzzyClustering( const Img3dsl &ims, Reg3d &rgd, int nbclus, float m ) {
   Img3duc ims1(ims.Size());
   Normalisation(ims, ims1, 0.0F, 255.0F);
   PFuzzyClustering(ims1, rgd, nbclus, m);
   return SUCCESS;
}

Errc PFuzzyClustering( const Img3dsf &ims, Reg3d &rgd, int nbclus, float m ) {
   Img3duc ims1(ims.Size());
   Normalisation(ims, ims1, 0.0F, 255.0F);
   PFuzzyClustering(ims1, rgd, nbclus, m);
   return SUCCESS;
}	

Errc PFuzzyClustering( const Imc3dsl &ims, Reg3d &rgd, int nbclus, float m ) {
   Imc3duc ims1(ims.Size());
   Normalisation(ims, ims1, 0.0F, 255.0F);
   PFuzzyClustering(ims1, rgd, nbclus, m);
   return SUCCESS;
}	

Errc PFuzzyClustering( const Imc3dsf &ims, Reg3d &rgd, int nbclus, float m ) {
   Imc3duc ims1(ims.Size());
   Normalisation(ims, ims1, 0.0F, 255.0F);
   PFuzzyClustering(ims1, rgd, nbclus, m);
   return SUCCESS;
}	

#ifdef MAIN

/*
 * Modify only the following constants, and the function call.
 */
#define	USAGE	"usage: %s class-count fuzzy-degree [-m mask] [im_in|-] [im_out|-]"
#define	PARC	2
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
   
   switch (objs[0]->Type()) {
       case Po_Img2duc: {
	  Img2duc* const ims = (Img2duc*)objs[0];
	  objd[0] = new Reg2d(ims->Size());
	  Reg2d* const imd = (Reg2d*)objd[0];
	  
	  result = PFuzzyClustering(*ims, *imd, atoi(parv[0]), (Float)atof(parv[1]));
       } break;
       case Po_Img2dsl: {
	  Img2dsl* const ims = (Img2dsl*)objs[0];
	  objd[0] = new Reg2d(ims->Size());
	  Reg2d* const imd = (Reg2d*)objd[0];
	  
	  result = PFuzzyClustering(*ims, *imd, atoi(parv[0]), (Float)atof(parv[1]));
       } break;
       case Po_Img2dsf: {
	  Img2dsf* const ims = (Img2dsf*)objs[0];
	  objd[0] = new Reg2d(ims->Size());
	  Reg2d* const imd = (Reg2d*)objd[0];
	  
	  result = PFuzzyClustering(*ims, *imd, atoi(parv[0]), (Float)atof(parv[1]));
       } break;
	  
       case Po_Imc2duc: {
	  Imc2duc* const ims = (Imc2duc*)objs[0];
	  objd[0] = new Reg2d(ims->Size());
	  Reg2d* const imd = (Reg2d*)objd[0];
	  
	  result = PFuzzyClustering(*ims, *imd, atoi(parv[0]), (Float)atof(parv[1]));
       } break;
       case Po_Imc2dsl: {
	  Imc2dsl* const ims = (Imc2dsl*)objs[0];
	  objd[0] = new Reg2d(ims->Size());
	  Reg2d* const imd = (Reg2d*)objd[0];
	  
	  result = PFuzzyClustering(*ims, *imd, atoi(parv[0]), (Float)atof(parv[1]));
       } break;
       case Po_Imc2dsf: {
	  Imc2dsf* const ims = (Imc2dsf*)objs[0];
	  objd[0] = new Reg2d(ims->Size());
	  Reg2d* const imd = (Reg2d*)objd[0];
	  
	  result = PFuzzyClustering(*ims, *imd, atoi(parv[0]), (Float)atof(parv[1]));
       } break;
	  
       case Po_Img3duc: {
	  Img3duc* const ims = (Img3duc*)objs[0];
	  objd[0] = new Reg3d(ims->Size());
	  Reg3d* const imd = (Reg3d*)objd[0];
	  
	  result = PFuzzyClustering(*ims, *imd, atoi(parv[0]), (Float)atof(parv[1]));
       } break;
       case Po_Img3dsl: {
	  Img3dsl* const ims = (Img3dsl*)objs[0];
	  objd[0] = new Reg3d(ims->Size());
	  Reg3d* const imd = (Reg3d*)objd[0];
	  
	  result = PFuzzyClustering(*ims, *imd, atoi(parv[0]), (Float)atof(parv[1]));
       } break;
       case Po_Img3dsf: { 
	  Img3dsf* const ims = (Img3dsf*)objs[0];
	  objd[0] = new Reg3d(ims->Size());
	  Reg3d* const imd = (Reg3d*)objd[0];
	  
	  result = PFuzzyClustering(*ims, *imd, atoi(parv[0]), (Float)atof(parv[1]));
       } break;
	  
       case Po_Imc3duc: {
	  Imc3duc* const ims = (Imc3duc*)objs[0];
	  objd[0] = new Reg3d(ims->Size());
	  Reg3d* const imd = (Reg3d*)objd[0];
	  
	  result = PFuzzyClustering(*ims, *imd, atoi(parv[0]), (Float)atof(parv[1]));
       } break;
       case Po_Imc3dsl: {
	  Imc3dsl* const ims = (Imc3dsl*)objs[0];
	  objd[0] = new Reg3d(ims->Size());
	  Reg3d* const imd = (Reg3d*)objd[0];
	  
	  result = PFuzzyClustering(*ims, *imd, atoi(parv[0]), (Float)atof(parv[1]));
       } break;
       case Po_Imc3dsf: {
	  Imc3dsf* const ims = (Imc3dsf*)objs[0];
	  objd[0] = new Reg3d(ims->Size());
	  Reg3d* const imd = (Reg3d*)objd[0];
	  
	  result = PFuzzyClustering(*ims, *imd, atoi(parv[0]), (Float)atof(parv[1]));
       } break;
       default:
	  PrintErrorFormat(objin, FINC);
	  result = FAILURE;
   }
   
   if (result) {
      WriteArgs(argc, argv, PARC, FINC, FOUTC, &mask, objin, objs, objout, objd, MASK);
   }
   
   Exit(result);
   return 0;
}

#endif
