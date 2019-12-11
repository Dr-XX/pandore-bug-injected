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
 * @author Olivier Lezoray - 2002-05-21 (Initial Program by Luc BRUN)
 */

#include <math.h>
#include <pandore.h>
using namespace pandore;

/**
 * @file colocube.cpp
 * Crée une image d'une cube connant la répartition des couleurs
 * dans l'espace.
 */

/*** COULEUR DU FOND ***/
#define FondR 127
#define FondG 127
#define FondB 127

/*** POINT VISE ***/
#define Xg 128
#define Yg 128
#define Zg 128

/*** FOCALE ***/
#define Fo 600

#define DIM 3
typedef double _matrice[DIM][DIM];
typedef double vecteur[DIM];

/*** VARIABLES GLOBALES ***/
float CosT, SinT, CosP, SinP, CosT_CosP, CosT_SinP, SinT_CosP, SinT_SinP, Ro;

/*
 * Initialisation
 */
void initialisation( Imx3duc &imd, Imx3dsf &imp, int Xv, int Yv, int Zv ) {
   int x, y, z;
   float dx = (float)(Xv - Xg);
   float dy = (float)(Yv - Yg);
   float dz = (float)(Zv - Zg);
   float dh = sqrt(dx * dx + dy * dy);
   float dl = sqrt(dx * dx + dy * dy + dz * dz);
   
   for (z = 0; z < imd.Depth(); z++) {
      for (y = 0; y < imd.Height(); y++) {
	 for (x = 0; x < imd.Width(); x++) {
	    imd[0][z][y][x] = FondR;
	    imd[1][z][y][x] = FondG;
	    imd[2][z][y][x] = FondB;
	    imp[0][z][y][x] = 100000.00F;	//profondeur infinie
	 }
      }
   }
   CosT = dx / dh;
   SinT = dy / dh;
   CosP = dh / dl;
   SinP = dz / dl;
   CosT_CosP = CosT * CosP;
   CosT_SinP = CosT * SinP;
   SinT_CosP = SinT * CosP;
   SinT_SinP = SinT * SinP;
   Ro = dl;
}

/*
 * Transformation 3D/2D
 */
void persp( Point3d p3, Point2d *p2, float *z, int Lx, int Ly ) {
   float x0, y0, z0;
   float x1, y1, z1;
   
   x0 = (float)(p3.x - Xg);
   y0 = (float)(p3.y - Yg);
   z0 = (float)(p3.z - Zg);
   
   x1 = -x0 * SinT      + y0 * CosT;
   y1 = -x0 * CosT_SinP - y0 * SinT_SinP + z0 * CosP;
   z1 = -x0 * CosT_CosP - y0 * SinT_CosP - z0 * SinP + Ro;
   
   (*p2).x = (int)(Fo * x1 / z1 + Lx / 2);
   (*p2).y = (int)(Fo * y1 / z1 + Ly / 2);
   *z = z1;
}

/*
 * Affiche un point
 */
void affiche( Point3d p3, int r, int g, int b, Imc2duc &imd, Img2dsf &imp ) {
   Point2d p2;
   float z;
   
   persp(p3, &p2, &z, imd.Height(), imd.Width());
   if ((p2.x >= 0)
       && (p2.x < imd.Height())
       && (p2.y >= 0)
       && (p2.y < imd.Width())) { 
      if (z < imp[p2.x][p2.y]) {
	 imd.X[p2] =  (r * 200 / 255);
	 imd.Y[p2] =  (g * 200 / 255);
	 imd.Z[p2] =  (b * 200 / 255);
	 imp[p2] = z;
      }
   }
}

void calcul_mat_cov( const Imc2duc &ims, _matrice M ) {    
   int R, G, B;
   double mom0;
   vecteur mom1;
   _matrice mom2;
   int i, j;
   int x, y;
   
   mom0 = 0.0;
   for (i = 0; i < DIM; i++) {
      mom1[i] = 0.0;
      for (j = 0;j < DIM; j++) {
	 mom2[i][j] = 0.0;
      }
   }
    
   for (y = 0; y < ims.Height(); y++) {
      for (x = 0; x < ims.Width(); x++) {
	 R = ims.X[y][x];
	 G = ims.Y[y][x];
	 B = ims.Z[y][x];
	 
	 mom0++;
	 
	 mom1[0] += R;
	 mom1[1] += G;
	 mom1[2] += B;
	 
	 mom2[0][0] += R * R;
	 mom2[1][1] += G * G;
	 mom2[2][2] += B * B;
	 mom2[1][0] += G * R;
	 mom2[2][0] += B * R;
	 mom2[2][1] += B * G;
      }
   }
   for (i = 0; i < DIM; i++) {
      for (j = 0; j <= i; j++) {
	 M[i][j] = (mom2[i][j] - mom1[i] * (mom1[j] / mom0) ) / mom0;
      }
   }
   for (i = 0; i < DIM; i++) {
      for (j = i + 1; j < DIM; j++) {
	 M[i][j] = M[j][i];
      }
   }
}

void val_vect_propre( _matrice a, double *val_propre, _matrice P ) {
   vecteur x, y;
   double norme_x, norme_ymx, e, lambda;
   int i, j, k;
   _matrice x0 = { {1.0/3.0, 1.0/3.0, 1.0/3.0}, 
		   {1.0,     0.0,    -1.0}, 
		   {-0.5,    1.0,    -0.5} };
   
   for (i = 0; i < DIM - 1; i++) {
      for (j = 0; j < DIM; j++) {
	 x[j] = x0[i][j]; 
      }
      do {
	 //  Calcul de norme de x 
	 norme_x = 0.0;
	 for (j = 0; j < DIM; j++) {
	    norme_x += x[j] * x[j];
	 }
	 norme_x = sqrt(norme_x);
	 
	 // calcul de y 
	 for (j = 0; j < DIM; j++) {
	    y[j] = 0.0;
	    for (k = 0; k < DIM; k++) {
	       y[j] += a[j][k] * x[k];
	    }
	    y[j] = y[j] / norme_x;
	 }
	 // calcul de convergence 
	 norme_ymx = 0.0;
	 for (j = 0; j < DIM; j++) {
	    norme_ymx += (y[j] - x[j]) * (y[j] - x[j]);
	 }
	 norme_ymx = sqrt(norme_ymx);
	 e = norme_ymx / norme_x;
	 
	 // x = y
	 for (j = 0; j < DIM; j++) {
	    x[j] = y[j];
	 }	    
      } while(e > 0.0001);
      
      k = 0;
      while (k < DIM && x[k] == 0) {
	 k++;
      }
      norme_x = 0.0;
      for (j = 0; j < DIM; j++) {
	 norme_x += x[j] * x[j];
      }
      
      norme_x = sqrt(norme_x);
      for (j = 0; j < DIM; j++) {
	 x[j] = x[j]/norme_x;
      }
      
      if (k == DIM) {
	 std::cerr << "Error pcolorcube: null eigenvector" << std::endl;
	 exit(0);
      } else {
	 // On normalise x 
	 y[k] = 0.0;
	 for (j = 0; j < DIM; j++) {
	    y[k] += a[k][j] * x[j];
	 }
	 
	 lambda = y[k] / x[k];
      }
      
      for (j = 0; j < DIM;j++) {
	 for (k = 0; k < DIM; k++) {
	    a[j][k] -= lambda * x[k] * x[j];
	 }
      }
      
      for (j = 0; j < DIM; j++) {
	 P[j][i] = x[j];
      }
      val_propre[i] = lambda; 
   }
   // La on utilise le fait que la matrice 
   // est 3*3 pour en deduire le dernier vecteur propre
   norme_x = 0.0;
   for (i = 0; i < DIM; i++) {
      j = (i + 1) % DIM;
      k = (i + 2) % DIM;
      P[i][2] = P[j][0] * P[k][1] - P[k][0] * P[j][1];
      norme_x += P[i][2] * P[i][2];
   }
   
   for (j = 0; j < DIM; j++) {
      P[j][2] = P[j][2] / norme_x;
   }

   k = 0;
   while (k < DIM && P[k][2] == 0) {
      k++;
   }
   
   if (k == DIM) {
      std::cerr << "Error pcolorcube: null eigenvector" << std::endl;
      exit(0);
   } else {
      lambda = 0.0;
      for (j = 0; j < DIM; j++) {
	 lambda += a[k][j] * P[j][2];
      }
      val_propre[2] = lambda / P[k][2];
   }
}

Errc PColorCube( const Imc2duc &ims, Imc2duc &imd, int x, int y, int z ) {
   Img2dsf improf(imd.Size());
   imd.ColorSpace(RGB);
   
   Point3d p;
   //valeurs simples
   int i, l;

   initialisation(imd, improf, x, y, z);

   // Trace des axes.
   for (i = 0; i < 256; i++) {
      p.x=i;p.y=0;p.z=0;
      affiche(p,  i,  0,  0, imd, improf);
      p.x=i;p.y=255;p.z=0;
      affiche(p,  i,  255,  0, imd, improf);
      p.x=i;p.y=255;p.z=255;
      affiche(p,  i,  255,  255, imd, improf);
      p.x=i;p.y=0;p.z=255;
      affiche(p,  i,  0,  255, imd, improf);

      p.x=0;p.y=i;p.z=0;
      affiche(p,  0,  i,  0, imd, improf);
      p.x=255;p.y=i;p.z=0;
      affiche(p,  255,  i,  0, imd, improf);
      p.x=255;p.y=i;p.z=255;
      affiche(p,  255,  i,  255, imd, improf);
      p.x=0;p.y=i;p.z=255;
      affiche(p,  0,  i,  255, imd, improf);

      p.x=0;p.y=0;p.z=i;
      affiche(p,  0,  0,  i, imd, improf);
      p.x=255;p.y=0;p.z=i;
      affiche(p,  255,  0,  i, imd, improf);
      p.x=255;p.y=255;p.z=i;
      affiche(p,  255,  255,  i, imd, improf);
      p.x=0;p.y=255;p.z=i;
      affiche(p,  0,  255,  i, imd, improf);
   }

   // On calcule et trace les directions de plus grande variance 
   _matrice mat;
   vecteur lambda;
   _matrice P;
   double somme;

   calcul_mat_cov(ims, mat);
   val_vect_propre(mat, lambda, P);

   somme = 0.0;
   for (i = 0; i < DIM; i++) {
      somme += lambda[i];
   }
   for (i = 0; i < DIM; i++) {
      lambda[i] /= somme;
   }
   for (i = 0; i < DIM; i++) {
      for (l = 0; l < 512; l++) {
	 p.x = (int)(lambda[i] * P[0][i] * l);
	 p.y = (int)(lambda[i] * P[1][i] * l);
	 p.z = (int)(lambda[i] * P[2][i] * l);
	 affiche(p, 255, 255, 255, imd, improf);
      }
   }
   
   for (y = 0; y < ims.Height(); y++) {
      for (x = 0; x < ims.Width(); x++) {
	 p.x = ims.X[y][x];
	 p.y = ims.Y[y][x];
	 p.z = ims.Z[y][x];
	 
	 affiche(p, p.x, p.y, p.z, imd, improf);
	 // Trace des ombres
	 //p.z=0;affiche(p, 0, 0, 0, imd, improf);
	 //p.x=255;p.z=ims.Z[y][x];affiche(p, 0, 0, 0, imd, improf);
	 //p.x=ims.X[y][x];p.y=255;affiche(p, 0, 0, 0, imd, improf);
      }
   }
   return SUCCESS;
}
  
#ifdef MAIN

/*
 * Modify only the following constants, and the operator switches.
 */
#define	USAGE	"usage: %s x y z [-m mask] [im_in|-] [im_out|-]"
#define	PARC	3
#define	FINC	1
#define	FOUTC	1
#define	MASK	2

int main( int argc, char *argv[] ) {
   Errc result;              // The result code of the execution.
   Pobject* mask;            // The region mask.
   Pobject* objin[FINC+1];   // The input objects.
   Pobject* objs[FINC+1];    // The source objects masked by the mask.
   Pobject* objout[FOUTC+1]; // The ouput object.
   Pobject* objd[FOUTC+1];   // The result object of the execution.
   char* parv[PARC+1];       // The input parameters.
  
   ReadArgs(argc, argv, PARC, FINC, FOUTC, &mask, objin, objs, objout, objd, parv, USAGE, MASK);
  
   switch(objs[0]->Type()) {
   case Po_Imc2duc : {
      Imc2duc* const ims = (Imc2duc*)objs[0];
      objd[0] = new Imc2duc(500, 500);
      Imc2duc* const imd = (Imc2duc*)objd[0];
    
      result = PColorCube(*ims, *imd, atoi(parv[0]), atoi(parv[1]), atoi(parv[2]));
   }break;
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
