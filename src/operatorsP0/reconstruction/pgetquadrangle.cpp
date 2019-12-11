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
 * @author Damien LEGRAND & Florian LANNOY - 2006-11-13
 */

#define _USE_MATH_DEFINES
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <pandore.h>
using namespace pandore; 

//#define NDEBUG

/**
 * @file pgetquadrangle.cpp
 *
 * Algorithme pour la detection de quadrangle
 */

const double deg2rad = M_PI/180.0f;

class Triplet {
   public:
      Triplet() {}
      Long rho;
      Long theta;
      Long votes;
};

class Accumulator {
   public:
      Accumulator( int rhoRange, int minTheta, int maxTheta ): _rhoRange(rhoRange), _thetaRange(180), _minTheta(minTheta), _maxTheta(maxTheta) {
	 _votes = new long*[2 * rhoRange];
	 for (int r = 0; r < 2 * rhoRange; r++) {
	    _votes[r] = new long[_thetaRange+1]; // + 1 for 180
	    for (int o = 0; o < _thetaRange+1; o++) {
	       _votes[r][o] = 0;
	    }
	 }
	 _tabCos = new float[_thetaRange + 1];
	 _tabSin = new float[_thetaRange + 1];
 	 for ( int o = 0; o < _thetaRange+1; o++ ) {
 	    _tabSin[o] = (float)(sin(o * deg2rad));
	    _tabCos[o] = (float)(cos(o * deg2rad));
	 }
      }
      ~Accumulator() {   // Free accumulator
	 for (int r = 0; r < 2 * _rhoRange; r++) {
	    delete[] _votes[r];
	 }
	 delete[] _votes;
	 delete[] _tabCos;
	 delete[] _tabSin;
      }
      
      void Add( int y, int x ) {
	 for (int o = _minTheta; o < _maxTheta; o++) {
	    const int angle = o % 180;
 	    const double rho = x * _tabCos[angle] + y * _tabSin[angle];
	    _votes[(int)(rho + _rhoRange + 0.5)][angle]++;
	 }
      }
      void Remove( int y, int x ) {
	 for (int o = 0; o < _thetaRange; o++) {
 	    const double rho = x * _tabCos[o] + y * _tabSin[o];
	    _votes[(int)(rho + _rhoRange + 0.5)][o]--;
	 }
      }

      Triplet GetMaxVotes() {
	 Triplet t;
	 t.votes = 0;
	 // merge 0 and 180.
	 for (int r = 0; r < 2 * _rhoRange; r++) {
	    _votes[r][0] =  _votes[r][_thetaRange];
	    _votes[r][_thetaRange] = 0;
	    for (int o = 0; o < _thetaRange; o++) {
	       if (_votes[r][o] > t.votes) {
		  t.rho = r - _rhoRange;
		  t.theta = o;
		  t.votes = _votes[r][o];
	       }
	    }
	 }
	 return t;
      }
   private:
      int _rhoRange;
      int _thetaRange;
      int _minTheta;
      int _maxTheta;
      float *_tabSin;
      float *_tabCos;
      long **_votes;
};

/**
 * Displays a straight line on the output image imd, and
 * removes the related parameters from the accumulator.
 */
static void removeCloseLines( const Point2d pti, const Point2d ptf, const Img2duc &ims, Accumulator &accumulator, int lineThickness ) {
   int x, y, max, min;
   float pente;

   // Write the related line into the output image.
   if (pti.x != ptf.x) {
      pente = (float)(pti.y - ptf.y) / (pti.x - ptf.x);
   } else {
      pente = MAXFLOAT;
   }
   
   if ((pente < -1) || (pente > 1)) {
      if (ptf.y < pti.y) {
	 min = ptf.y; max = pti.y;
      } else {
	 min  =pti.y; max = ptf.y;
      }
      for (y = max; y >= min; y--) {
	 x = (int)((y - ptf.y) / pente + ptf.x);
	 if (ims.Hold(y, x)) {
	    // Remove related parameters from the accumulator
	    for (int x1 = x - lineThickness; x1 <= x + lineThickness; x1++) {
	       if (x1 >= 0 && x1 < ims.Width() && ims(y, x1) > 0) {
		  accumulator.Remove(y, x1);
	       }
	    }
	 }
      }
   } else {
      if (ptf.x < pti.x) {
	 min = ptf.x; max = pti.x;
      } else {
	 min = pti.x; max = ptf.x;
      }
      for (x  =max;  x >= min; x--) {
	 y = (int)(pente*(x - ptf.x) + ptf.y);
	 if (ims.Hold(y, x)) {
	    // Remove related parameters from the accumulator
	    for (int y1 = y-lineThickness; y1 <= y + lineThickness; y1++) {
	       if (y1 >= 0 && y1 < ims.Height() && ims(y1, x) > 0) {
		  accumulator.Remove(y1, x);
	       }
	    }
	 }
      }
   }
}

/**
 * Compte le nombre de pixels de contour se trouvant sous
 * le segment [pti, ptf], dans une bande de 3 pixels.
 * @param : Img2duc &ims : image des contours
 *          Point2d pti : point de depart du segement
 *          Point2d ptf : point de fin du segment
 * @return : le nombre de pixels de contour.
 */
int comptePointsSousLigne( const Img2duc &ims, Point2d pti, Point2d ptf ) {
   const int ecart_c = 3;
   float pente;
   int count = 0;

   if (pti.x != ptf.x) {
      pente = (float)(pti.y - ptf.y) / (pti.x - ptf.x);
   } else {
      pente = MAXFLOAT;
   }
   int max, min;
   if ((pente < -1) || (pente > 1)) {
      if (ptf.y < pti.y) {
	 min = ptf.y;
	 max = pti.y;
      } else {
	 min = pti.y;
	 max = ptf.y;
      }
      
      for (int i = max;i >= min; i--) {
	 if (i >= 0 && i < ims.Height()) {
	    int j = (int)((i - ptf.y) / pente + ptf.x);
	    for (int k = j - ecart_c; k <= j + ecart_c; k++) {
	       if (k >= 0 && k < ims.Width()) {
		  if (ims(i, k)) {
		     count++;
		     break;
		  }
	       }
	    }
	 }
      }
   } else {
      if (ptf.x < pti.x) {
	 min = ptf.x; max = pti.x;
      } else {
	 min  =pti.x; max = ptf.x;

      }
      for (int j = max; j >= min; j--) {
	 if (j >= 0 && j < ims.Width()) {
	    int i = (int)(pente*(j - ptf.x) + ptf.y);
	    for (int k = i - ecart_c; k <= i + ecart_c; k++) { 
	       if (k >= 0 && k < ims.Height()) {
		  if (ims(k, j)) {
		     count++;
		     break;
		  }
	       }
	    }
	 }
      }
   }

   return count;
}

int differenceAngle( const Triplet t1, const Triplet t2 ) {
   int difference  = ABS(t1.theta - t2.theta);
   if (difference > 90) {
      return 180 - difference;
   } else {
      return difference;
   }
}

bool QuadrangleVerification( const Img2duc &ims, Triplet * lignes, Point2d * start, Point2d * end, Point2d * quadrilatere, int nbLignes ) {
   const int width = ims.Width();
   const int height = ims.Height();
   
#ifndef NDEBUG
   printf("\n\n******* Verification **** \n");
#endif

   // Essaye toutes les combinaisons 4x4.
   bool test = false;
   int tab[4];
   int pointsMax = 0;
   int perimetreMax = 1;

   for  (int i = 0; i < nbLignes - 3; i++) { 
      for (int j = i + 1; j < nbLignes - 2; j++) {
	 for (int k = j + 1; k < nbLignes - 1; k++) {
	    for (int l = k + 1; l< nbLignes; l++) {
	       // 1. L'angle entre deux lignes opposées < 30 degrés.
	       tab[0] = i;
	       if (differenceAngle(lignes[i], lignes[j]) < 30) {
		  tab[2] = j;
		  tab[1] = k;
		  tab[3] = l;
	       } else if (differenceAngle(lignes[i], lignes[k]) < 30) {
		  tab[2] = k;
		  tab[1] = j;
		  tab[3] = l;
	       } else if (differenceAngle(lignes[i], lignes[l]) < 30) {
		  tab[2] = l;
		  tab[1] = j;
		  tab[3] = k;
	       } else {
		  continue;
	       }

	       if (differenceAngle(lignes[tab[1]], lignes[tab[3]]) > 30) {
		  continue;
	       }
	       // 2. La distance entre les lignes opposees doit etre superieure au cinquieme
	       // de la hauteur ou de la largeur de l'image.
	       if (lignes[tab[0]].theta<45 || lignes[tab[0]].theta>135) {
		  if ((fabs((float)lignes[tab[0]].rho-lignes[tab[2]].rho) < height/3.0f)
		      || (fabs((float)lignes[tab[1]].rho-lignes[tab[3]].rho) < width/3.0f)) {
		     continue;
		  }
	       } else {
		  if ((fabs((float)lignes[tab[0]].rho - lignes[tab[2]].rho) < width / 3.0f)
		      || (fabs((float)lignes[tab[1]].rho - lignes[tab[3]].rho) < height / 3.0f)) {
		     continue;
		  }
	       }

	       // 3. L'angle entre deux lignes voisines doit etre 90° +:-30°
	       if (differenceAngle(lignes[tab[0]], lignes[tab[1]]) > 120 || differenceAngle(lignes[tab[0]], lignes[tab[1]]) < 60
		   || differenceAngle(lignes[tab[1]], lignes[tab[2]]) > 120 || differenceAngle(lignes[tab[1]], lignes[tab[2]]) < 60
		   || differenceAngle(lignes[tab[2]], lignes[tab[3]]) > 120 || differenceAngle(lignes[tab[2]], lignes[tab[3]]) < 60
		   || differenceAngle(lignes[tab[0]], lignes[tab[3]]) > 120 || differenceAngle(lignes[tab[0]], lignes[tab[3]]) < 60)
		  continue;

	       float coef[4][2];
	       Point2d coin[4];

	       // Calcul les coordonnees des droites comme suit : coef[m][0]*x+coef[m][1]*y=1
	       for (int m = 0; m < 4; m++) {
		  if (start[tab[m]].x == end[tab[m]].x) {
		     coef[m][0] = 1.0F / start[tab[m]].x;
		     coef[m][1] = 0.0F;
		  } else if (start[tab[m]].y == end[tab[m]].y) {
		     coef[m][0] = 0.0F;
		     coef[m][1] = 1.0F / start[tab[m]].y;
		  } else { 
		     coef[m][0] = (float)(start[tab[m]].y - end[tab[m]].y) / (start[tab[m]].y * end[tab[m]].x - end[tab[m]].y * start[tab[m]].x);
		     coef[m][1] = (float)(start[tab[m]].x - end[tab[m]].x) / (start[tab[m]].x * end[tab[m]].y - end[tab[m]].x * start[tab[m]].y);
		  }
	       }
	       
	       // Calcul des coordonnees des 4 coins quand les deux droites
	       // se rencontrent (x1=x2 et y1=y2).
	       bool insideImage = true;
	       for (int m = 0; m < 4; m++) {
		  int n = (m + 1) % 4;
		  coin[m].x = (Long)((coef[n][1] - coef[m][1]) / (coef[m][0] * coef[n][1] - coef[m][1] * coef[n][0]));
		  if (coin[m].x >= width || coin[m].x < 0) {
		     insideImage = false;
		  }
		  coin[m].y=(Long)((coef[m][0]-coef[n][0]) / (coef[m][0]*coef[n][1]-coef[m][1]*coef[n][0]));
		  if (coin[m].y >= height || coin[m].y < 0) {
		     insideImage = false;
		  }
	       }
	       if (!insideImage) {
		  continue;
	       }

	       // Calcul du nombre de points de contours.
	       Long perimetre = 0;
	       int points = 0;
	       for (int m = 0; m < 4; m++) {
		  int n = (m + 1) % 4;
		  perimetre += (Long)sqrt((float)(SQR(coin[m].x - coin[n].x) + SQR(coin[m].y - coin[n].y)));
		  points += comptePointsSousLigne(ims, coin[m], coin[n]);
	       }

	       // 4. Perimetre > (W+H)/4
	       if ( perimetre < (width + height) / 4) {
		  continue;
	       }

	       if (points*perimetreMax > pointsMax*perimetre) {
		  perimetreMax = perimetre;
		  pointsMax = points;
		  for (int m = 0; m < 4; m++) {
		     quadrilatere[m] = coin[m];
		  }
#ifndef NDEBUG
		  printf(" Maximum: %d %d %d %d\n", tab[0], tab[1], tab[2], tab[3]);
#endif
		  test = true;
	       }
	    }
	 }
      }
   }

   return test;
}

#ifndef NDEBUG
/**
 * Trace le segment [pti, ptf].
 */
static void TraceDroite( Img2duc &imd, Point2d pti, Point2d ptf, int couleur = 255 ) {
   int i, j, max, min;
   float pente;

   if (pti.x != ptf.x) {
      pente  =(float)(pti.y - ptf.y) / (pti.x - ptf.x);
   } else {
      pente = MAXFLOAT;
   }
   if ((pente < -1) || (pente > 1)) {
      if (ptf.y < pti.y) {
	 min = ptf.y; max = pti.y;
      } else {
	 min = pti.y; max = ptf.y;
      }
      for (i = max; i >= min; i--) {
	 j = (int)((i - ptf.y) / pente + ptf.x);
	 if (i > 0 && i < imd.Height() && j > 0 && j < imd.Width()) {
	    imd(i, j) = couleur;
	 }
      }
   } else {
      if (ptf.x < pti.x) {
	 min=  ptf.x; max = pti.x;
      } else {
	 min = pti.x; max = ptf.x;
      }

      for (j = max; j >= min; j--) {
	 i = (int)(pente*(j - ptf.x) + ptf.y);
	 if (i > 0 && i < imd.Height() && j > 0 && j < imd.Width()) {
	    imd(i, j) = couleur;
	 }
      }
   }
}
#endif

/**
 * Extrait nbLignes dans l'image des contours ims, 
 * suivant l'algorithme de Hough.
 */
Errc PGetQuadrangle( const Img2duc &ims1, Img2duc ims2, Img2duc &imd ) {
   Errc result = FAILURE;
   const int nbLignes = 12;
   const int lineThickness=3;

   // Min and max theta.
   const Long minangle = 0;
   const Long maxangle = 180;
   
   const Long minTheta = (450 - maxangle) % 360;
   const Long maxTheta = (450 - minangle) % 360 + 360;

   const int rhoRange = (Long)ceil(sqrt((double)SQR(ims1.Height()) + SQR(ims1.Width())));

   // Accumulator is used to store all line coordinates.
   Accumulator accumulator(rhoRange, minTheta, maxTheta); 

   // Transform into polar coordinates: r >= 0 et 2*M_PI>=o >= 0
   for (int y = 0; y < ims1.Height(); y++) {
      for (int x = 0; x < ims1.Width(); x++) {
	 if ((ims1(y, x))) { // a contour point
	    accumulator.Add(y, x);
	 }
      }
   }

   // Detect the requested number of lines ...
   Point2d start[nbLignes];
   Point2d end[nbLignes];
   Triplet lignes[nbLignes];

#ifndef NDEBUG
   imd = 0; 
#endif
   int noligne = 0;
   while (noligne < nbLignes) {
      // Search for the maximum votes.
      lignes[noligne] = accumulator.GetMaxVotes();
 
      const long max_r = lignes[noligne].rho;
      const long max_o = lignes[noligne].theta;
      
      if (lignes[noligne].votes < 1) {
	 break;
      }
   
      const double deg2rad = M_PI/180.0f;

      // the line equation is: c = ax + by --> y = (-ax + c)/b
      double a = cos(max_o * deg2rad);
      double b = sin(max_o * deg2rad);
      double c = max_r;
      
      int x0 = -imd.Width();
      int x1 = imd.Width();
      double y0, y1;
      
      if (b != 0) {
	 y0 = (-a * x0 + c) / b;
	 y1 = (-a * x1 + c) / b;

	 start[noligne] = Point2d((int)(y0 + 0.5), x0);
	 end[noligne]   = Point2d((int)(y1 + 0.5), x1);
	 removeCloseLines(start[noligne], end[noligne], ims1, accumulator, lineThickness);
      } else {
	 x0 = max_r;
	 y0 = 0;
	 x1 = max_r;
	 y1 =imd.Height();
	 
	 start[noligne] = Point2d((int)(y0 + 0.5), x0);
	 end[noligne] = Point2d((int)(y1 + 0.5), x1);
	 removeCloseLines(start[noligne], end[noligne], ims1, accumulator, lineThickness);
      }
            
#ifndef NDEBUG
      printf("noligne %d\n", noligne);
      printf("   Max value = %ld in rho=%ld and teta=%ld\n", (long)lignes[noligne].votes, (long)lignes[noligne].rho, (long)lignes[noligne].theta);
      printf("   begin (%d, %d)\n", start[noligne].x, start[noligne].y);
      printf("   end   (%d, %d)\n", end[noligne].x, end[noligne].y);
      TraceDroite(imd, start[noligne], end[noligne], noligne);
      result = SUCCESS;
#endif
      if (start[noligne].x != end[noligne].x || start[noligne].y != end[noligne].y) {
	 noligne++;
      }
   }

   Point2d *quadrilatere = new Point2d[4]; //index des 4 lignes formant le quadrangle

   if (QuadrangleVerification(ims2, lignes, start, end, quadrilatere, noligne+1)) {

#ifndef NDEBUG
      // on initialise l'image de sortie en noir
      imd = 0;
      for (int l = 0; l< 4; l++) {
	 int n = (l + 1) % 4;	
	 if (imd.Hold(quadrilatere[l]) && imd.Hold(quadrilatere[n])) {
	    TraceDroite(imd, quadrilatere[l], quadrilatere[n]); 
	 }
      }
#else
      // on initialise l'image de sortie en noir
      imd = 0;
      for (int l = 0; l < 4; l++) {
	 imd(quadrilatere[l]) = 255;
      }
#endif     
      result = SUCCESS;
   }
   
   delete[] quadrilatere;

   if (result == FAILURE)  {
      std::cerr << "Error pgetquadrangle: no quadrangle detected" << std::endl;
   }

return result;
}

#ifdef MAIN

/*
 * Modify only the following constants, and the operator switches.
 */

#define	USAGE	"usage: %s [-m mask] [im_lines] [im_bin|-] [im_out|-]"
#define	PARC	0
#define	FINC	2
#define	FOUTC	1
#define	MASK	1

int main( int argc, char *argv[] ) {
   Errc result;                // The result code of the execution.
   Pobject* mask;              // The region map.
   Pobject* objin[FINC + 1];   // The input objects;
   Pobject* objs[FINC + 1];    // The source objects masked.
   Pobject* objout[FOUTC + 1]; // The output object.
   Pobject* objd[FOUTC + 1];   // The result object of the execution.
   char* parv[PARC + 1];       // The input parameters.

   ReadArgs(argc, argv, PARC, FINC, FOUTC, &mask, objin, objs, objout, objd, parv, USAGE, MASK);

   if (objs[0]->Type() == Po_Img2duc) {
      Img2duc* const ims1 = (Img2duc*)objs[0];
      Img2duc* const ims2 = (Img2duc*)objs[1];
      objd[0] = new Img2duc(ims1->Size());
      Img2duc* const imd = (Img2duc*)objd[0];

      result = PGetQuadrangle(*ims1, *ims2, *imd);
   } else {
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
