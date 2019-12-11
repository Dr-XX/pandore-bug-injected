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
 * @author Francois Angot - 1998-01-12
 * @author Philippe Belhomme - 2001-11-15
 * @author Régis Clouard - May 15, 2012 (add color images)
 */

#include <pandore.h>
using namespace pandore;

/**
 * @file pmeanaggregation.cpp
 *
 * On affecte un point a une region si sa valeur est proche de la
 * moyenne de la region (si la difference entre la moyenne d'origine et la
 * valeur du point est inferieure au threshold).
 * La moyenne n'est pas recalculee pour eviter de trop s'eloigner de la
 * situation initiale.
 */

class MeanFeatures {
private:
   double _sum[3];
   Ulong _count;
   
public:
   static Long bands;
   
   MeanFeatures( ) : _count(0) {
      _sum[0] = 0;
      _sum[1] = 0;
      _sum[2] = 0;
   }
   
   void update( double *value ) {
      for (int b = 0; b < bands; b++ ) {
	 _sum[b] += value[b];
      }
      _count++;
   }
   void computeMeans() {
      for (int b = 0; b < bands; b++) {
	 _sum[b] /= _count;
      }
   }
   float distance( const double * value ) {
      float d = 0;
      for (int b = 0; b < bands; b++ ) {
	 d += (float)SQR(_sum[b] - value[b]);
      }
      return d / bands;
   }
};

Long MeanFeatures::bands = 1;


Errc PMeanAggregation( const Reg2d &rgs, const Img2duc &ims, Reg2d &rgd, int connectivity, float threshold ) {
   Reg2d::ValueType r;
   MeanFeatures* meanFeatures;
   Point2d p, p1, pt;
   Ulong nbpix = 0;
   double pixelColor[3]= {0, 0, 0};

   threshold *= threshold; // To avoid sqrt.

   if (rgs.Size() != ims.Size()) {
      std::cerr << "Error pmeanaggregation: incompatible image sizes." << std::endl;
      return FAILURE;
   }
   if (connectivity != 4 && connectivity != 8) {
      std::cerr << "Error pmeanaggregation: bad connectivity value." << std::endl;
      return FAILURE;
   }
   
   Heap<Point2d, float, Ulong> heap(rgs.VectorSize());

   // Allocation et initialisation du tableau des moyennes.
   meanFeatures= new MeanFeatures[rgs.Labels()+1];
   MeanFeatures::bands = ims.Bands();

   // Initialisation : recopie des regions d'entrée (rgs) vers la sortie (rgd) 
   // et calcul des tailles et sommes des régions. Les pixels
   // à 0 dans rgs sont considérés comme non traités, ceux qui appartiennent à
   // une région garderont le même label.
   for (p.y = 0; p.y < ims.Height(); p.y++)
   for (p.x = 0; p.x < ims.Width(); p.x++)
   {
      r = rgd[p] = rgs[p];
      if ((r)) {
	 for (int b = 0; b < ims.Bands(); b++ ) {
	    pixelColor[b] = (double)ims(b, p);
	 }
	 meanFeatures[r].update(pixelColor);
      } else {
	 // p est un point à 0 donc pas encore traité
	 // On va empiler p SI il y a une région dans son voisinage
	 if (connectivity == 4) {
	    for (int v = 0; v < 4; v++) {
	       p1 = p+v4[v];
	       if (rgs.Hold(p1) && (rgs[p1] !=0 )) {
		  heap.Push(p, 0);
		  break;
	       }
	    }
	 } else {
	    for (int v = 0; v < 8; v++) {
	       p1 = p + v8[v];
	       if (rgs.Hold(p1) && (rgs[p1] != 0)) {
		  heap.Push(p, 0);
		  break;
	       }
	    }
	 }
      }
   } // Fin du balayage initial de l'image rgs
   
   for (int r = 1; r < (int)rgs.Labels() + 1; r++) {
      meanFeatures[r].computeMeans();
   }

   // A ce stade tous les pixels voisins d'au moins une région sont empilés...
   
   // Nouveau mode de croissance
   // On dépile un point et on regarde si on peut l'attribuer à une région 
   // voisine en comparant son niveau de gris à la moyenne de la région 
   // (la différence doit être plus petite qu'un certain threshold pour que le
   // point soit aggrégé, sinon il disparaît définitivement de la pile).

   while (!heap.Empty()) {
      p = heap.Pop();      // On dépile un point p.
      if ((rgd[p])) {
	 continue;
      }

      if (connectivity == 4) {
	 Ulong closestRegion = 0;
	 float distanceMin = MAXFLOAT;
	 for (int b = 0; b < ims.Bands(); b++ ) {
	    pixelColor[b] = (double)ims(b, p);
	 }
	 for (int v = 0; v < 4; v++) {
	    p1 = p + v4[v];
	    if (rgd.Hold(p1) && (r = rgd[p1])) {
	       float d = meanFeatures[r].distance(pixelColor);
	       if (d <= threshold && d<distanceMin) {
		  distanceMin = d;
		  closestRegion = r;
	       }
	    }
	 }
	 
	 if (closestRegion > 0) {
	    rgd[p]=closestRegion;
	    nbpix++;
	    // On empile alors tous ses voisins non encore traités 
	    // (ceux à 0 dans rgd)
	    for (int w = 0; w < 4; w++) {
	       pt = p + v4[w];
	       if (rgd.Hold(pt) && !rgd[pt]) {
		  for (int b = 0; b < ims.Bands(); b++ ) {
		     pixelColor[b] = (double)ims(b, p);
		  }
		  heap.Push(pt, meanFeatures[closestRegion].distance(pixelColor));
	       }
	    }
	 }
      } else {
	 Ulong closestRegion = 0;
	 float distanceMin = MAXFLOAT;
	 for (int b = 0; b < ims.Bands(); b++ ) {
	    pixelColor[b] = (double)ims(b, p);
	 }
	 for (int v = 0; v < 8; v++) {
	    p1 = p + v8[v];
	    if (rgd.Hold(p1) && (r = rgd[p1])) {
	       float d = meanFeatures[r].distance(pixelColor);
	       if (d <= threshold && d < distanceMin) {
		  distanceMin = d;
		  closestRegion = r;
	       }
	    }
	 }
	 
	 if (closestRegion > 0) {
	    rgd[p] = closestRegion;
	    nbpix++;
	    // On empile alors tous ses voisins non encore traités 
	    // (ceux à 0 dans rgd)
	    for (int w = 0; w < 8; w++) {
	       pt = p + v8[w];
	       if (rgd.Hold(pt) && !rgd[pt]) {
		  for (int b = 0; b < ims.Bands(); b++ ) {
		     pixelColor[b] = (double)ims(b, p);
		  }
		  heap.Push(pt, meanFeatures[closestRegion].distance(pixelColor));
	       }
	    }
	 }
      }
   }
      
   delete[] meanFeatures;
   rgd.Labels(rgs.Labels());
   return nbpix;
}

Errc PMeanAggregation( const Reg2d &rgs, const Img2dsl &ims, Reg2d &rgd, int connectivity, float threshold ) {
   Reg2d::ValueType r;
   MeanFeatures* meanFeatures;
   Point2d p, p1, pt;
   Ulong nbpix = 0;
   double pixelColor[3]= {0, 0, 0};

   threshold *= threshold; // To avoid sqrt.

   if (rgs.Size() != ims.Size()) {
      std::cerr << "Error pmeanaggregation: incompatible image sizes." << std::endl;
      return FAILURE;
   }
   if (connectivity != 4 && connectivity != 8) {
      std::cerr << "Error pmeanaggregation: bad connectivity value." << std::endl;
      return FAILURE;
   }
   
   Heap<Point2d, float, Ulong> heap(rgs.VectorSize());

   // Allocation et initialisation du tableau des moyennes.
   meanFeatures= new MeanFeatures[rgs.Labels()+1];
   MeanFeatures::bands = ims.Bands();

   // Initialisation : recopie des regions d'entrée (rgs) vers la sortie (rgd) 
   // et calcul des tailles et sommes des régions. Les pixels
   // à 0 dans rgs sont considérés comme non traités, ceux qui appartiennent à
   // une région garderont le même label.
   for (p.y = 0; p.y < ims.Height(); p.y++)
   for (p.x = 0; p.x < ims.Width(); p.x++)
   {
      r = rgd[p] = rgs[p];
      if ((r)) {
	 for (int b = 0; b < ims.Bands(); b++ ) {
	    pixelColor[b] = (double)ims(b, p);
	 }
	 meanFeatures[r].update(pixelColor);
      } else {
	 // p est un point à 0 donc pas encore traité
	 // On va empiler p SI il y a une région dans son voisinage
	 if (connectivity == 4) {
	    for (int v = 0; v < 4; v++) {
	       p1 = p+v4[v];
	       if (rgs.Hold(p1) && (rgs[p1] !=0 )) {
		  heap.Push(p, 0);
		  break;
	       }
	    }
	 } else {
	    for (int v = 0; v < 8; v++) {
	       p1 = p + v8[v];
	       if (rgs.Hold(p1) && (rgs[p1] != 0)) {
		  heap.Push(p, 0);
		  break;
	       }
	    }
	 }
      }
   } // Fin du balayage initial de l'image rgs
   
   for (int r = 1; r < (int)rgs.Labels() + 1; r++) {
      meanFeatures[r].computeMeans();
   }

   // A ce stade tous les pixels voisins d'au moins une région sont empilés...
   
   // Nouveau mode de croissance
   // On dépile un point et on regarde si on peut l'attribuer à une région 
   // voisine en comparant son niveau de gris à la moyenne de la région 
   // (la différence doit être plus petite qu'un certain threshold pour que le
   // point soit aggrégé, sinon il disparaît définitivement de la pile).

   while (!heap.Empty()) {
      p = heap.Pop();      // On dépile un point p.
      if ((rgd[p])) {
	 continue;
      }

      if (connectivity == 4) {
	 Ulong closestRegion = 0;
	 float distanceMin = MAXFLOAT;
	 for (int b = 0; b < ims.Bands(); b++ ) {
	    pixelColor[b] = (double)ims(b, p);
	 }
	 for (int v = 0; v < 4; v++) {
	    p1 = p + v4[v];
	    if (rgd.Hold(p1) && (r = rgd[p1])) {
	       float d = meanFeatures[r].distance(pixelColor);
	       if (d <= threshold && d<distanceMin) {
		  distanceMin = d;
		  closestRegion = r;
	       }
	    }
	 }
	 
	 if (closestRegion > 0) {
	    rgd[p]=closestRegion;
	    nbpix++;
	    // On empile alors tous ses voisins non encore traités 
	    // (ceux à 0 dans rgd)
	    for (int w = 0; w < 4; w++) {
	       pt = p + v4[w];
	       if (rgd.Hold(pt) && !rgd[pt]) {
		  for (int b = 0; b < ims.Bands(); b++ ) {
		     pixelColor[b] = (double)ims(b, p);
		  }
		  heap.Push(pt, meanFeatures[closestRegion].distance(pixelColor));
	       }
	    }
	 }
      } else {
	 Ulong closestRegion = 0;
	 float distanceMin = MAXFLOAT;
	 for (int b = 0; b < ims.Bands(); b++ ) {
	    pixelColor[b] = (double)ims(b, p);
	 }
	 for (int v = 0; v < 8; v++) {
	    p1 = p + v8[v];
	    if (rgd.Hold(p1) && (r = rgd[p1])) {
	       float d = meanFeatures[r].distance(pixelColor);
	       if (d <= threshold && d < distanceMin) {
		  distanceMin = d;
		  closestRegion = r;
	       }
	    }
	 }
	 
	 if (closestRegion > 0) {
	    rgd[p] = closestRegion;
	    nbpix++;
	    // On empile alors tous ses voisins non encore traités 
	    // (ceux à 0 dans rgd)
	    for (int w = 0; w < 8; w++) {
	       pt = p + v8[w];
	       if (rgd.Hold(pt) && !rgd[pt]) {
		  for (int b = 0; b < ims.Bands(); b++ ) {
		     pixelColor[b] = (double)ims(b, p);
		  }
		  heap.Push(pt, meanFeatures[closestRegion].distance(pixelColor));
	       }
	    }
	 }
      }
   }
      
   delete[] meanFeatures;
   rgd.Labels(rgs.Labels());
   return nbpix;
}

Errc PMeanAggregation( const Reg2d &rgs, const Img2dsf &ims, Reg2d &rgd, int connectivity, float threshold ) {
   Reg2d::ValueType r;
   MeanFeatures* meanFeatures;
   Point2d p, p1, pt;
   Ulong nbpix = 0;
   double pixelColor[3]= {0, 0, 0};

   threshold *= threshold; // To avoid sqrt.

   if (rgs.Size() != ims.Size()) {
      std::cerr << "Error pmeanaggregation: incompatible image sizes." << std::endl;
      return FAILURE;
   }
   if (connectivity != 4 && connectivity != 8) {
      std::cerr << "Error pmeanaggregation: bad connectivity value." << std::endl;
      return FAILURE;
   }
   
   Heap<Point2d, float, Ulong> heap(rgs.VectorSize());

   // Allocation et initialisation du tableau des moyennes.
   meanFeatures= new MeanFeatures[rgs.Labels()+1];
   MeanFeatures::bands = ims.Bands();

   // Initialisation : recopie des regions d'entrée (rgs) vers la sortie (rgd) 
   // et calcul des tailles et sommes des régions. Les pixels
   // à 0 dans rgs sont considérés comme non traités, ceux qui appartiennent à
   // une région garderont le même label.
   for (p.y = 0; p.y < ims.Height(); p.y++)
   for (p.x = 0; p.x < ims.Width(); p.x++)
   {
      r = rgd[p] = rgs[p];
      if ((r)) {
	 for (int b = 0; b < ims.Bands(); b++ ) {
	    pixelColor[b] = (double)ims(b, p);
	 }
	 meanFeatures[r].update(pixelColor);
      } else {
	 // p est un point à 0 donc pas encore traité
	 // On va empiler p SI il y a une région dans son voisinage
	 if (connectivity == 4) {
	    for (int v = 0; v < 4; v++) {
	       p1 = p+v4[v];
	       if (rgs.Hold(p1) && (rgs[p1] !=0 )) {
		  heap.Push(p, 0);
		  break;
	       }
	    }
	 } else {
	    for (int v = 0; v < 8; v++) {
	       p1 = p + v8[v];
	       if (rgs.Hold(p1) && (rgs[p1] != 0)) {
		  heap.Push(p, 0);
		  break;
	       }
	    }
	 }
      }
   } // Fin du balayage initial de l'image rgs
   
   for (int r = 1; r < (int)rgs.Labels() + 1; r++) {
      meanFeatures[r].computeMeans();
   }

   // A ce stade tous les pixels voisins d'au moins une région sont empilés...
   
   // Nouveau mode de croissance
   // On dépile un point et on regarde si on peut l'attribuer à une région 
   // voisine en comparant son niveau de gris à la moyenne de la région 
   // (la différence doit être plus petite qu'un certain threshold pour que le
   // point soit aggrégé, sinon il disparaît définitivement de la pile).

   while (!heap.Empty()) {
      p = heap.Pop();      // On dépile un point p.
      if ((rgd[p])) {
	 continue;
      }

      if (connectivity == 4) {
	 Ulong closestRegion = 0;
	 float distanceMin = MAXFLOAT;
	 for (int b = 0; b < ims.Bands(); b++ ) {
	    pixelColor[b] = (double)ims(b, p);
	 }
	 for (int v = 0; v < 4; v++) {
	    p1 = p + v4[v];
	    if (rgd.Hold(p1) && (r = rgd[p1])) {
	       float d = meanFeatures[r].distance(pixelColor);
	       if (d <= threshold && d<distanceMin) {
		  distanceMin = d;
		  closestRegion = r;
	       }
	    }
	 }
	 
	 if (closestRegion > 0) {
	    rgd[p]=closestRegion;
	    nbpix++;
	    // On empile alors tous ses voisins non encore traités 
	    // (ceux à 0 dans rgd)
	    for (int w = 0; w < 4; w++) {
	       pt = p + v4[w];
	       if (rgd.Hold(pt) && !rgd[pt]) {
		  for (int b = 0; b < ims.Bands(); b++ ) {
		     pixelColor[b] = (double)ims(b, p);
		  }
		  heap.Push(pt, meanFeatures[closestRegion].distance(pixelColor));
	       }
	    }
	 }
      } else {
	 Ulong closestRegion = 0;
	 float distanceMin = MAXFLOAT;
	 for (int b = 0; b < ims.Bands(); b++ ) {
	    pixelColor[b] = (double)ims(b, p);
	 }
	 for (int v = 0; v < 8; v++) {
	    p1 = p + v8[v];
	    if (rgd.Hold(p1) && (r = rgd[p1])) {
	       float d = meanFeatures[r].distance(pixelColor);
	       if (d <= threshold && d < distanceMin) {
		  distanceMin = d;
		  closestRegion = r;
	       }
	    }
	 }
	 
	 if (closestRegion > 0) {
	    rgd[p] = closestRegion;
	    nbpix++;
	    // On empile alors tous ses voisins non encore traités 
	    // (ceux à 0 dans rgd)
	    for (int w = 0; w < 8; w++) {
	       pt = p + v8[w];
	       if (rgd.Hold(pt) && !rgd[pt]) {
		  for (int b = 0; b < ims.Bands(); b++ ) {
		     pixelColor[b] = (double)ims(b, p);
		  }
		  heap.Push(pt, meanFeatures[closestRegion].distance(pixelColor));
	       }
	    }
	 }
      }
   }
      
   delete[] meanFeatures;
   rgd.Labels(rgs.Labels());
   return nbpix;
}

Errc PMeanAggregation( const Reg2d &rgs, const Imc2duc &ims, Reg2d &rgd, int connectivity, float threshold ) {
   Reg2d::ValueType r;
   MeanFeatures* meanFeatures;
   Point2d p, p1, pt;
   Ulong nbpix = 0;
   double pixelColor[3]= {0, 0, 0};

   threshold *= threshold; // To avoid sqrt.

   if (rgs.Size() != ims.Size()) {
      std::cerr << "Error pmeanaggregation: incompatible image sizes." << std::endl;
      return FAILURE;
   }
   if (connectivity != 4 && connectivity != 8) {
      std::cerr << "Error pmeanaggregation: bad connectivity value." << std::endl;
      return FAILURE;
   }
   
   Heap<Point2d, float, Ulong> heap(rgs.VectorSize());

   // Allocation et initialisation du tableau des moyennes.
   meanFeatures= new MeanFeatures[rgs.Labels()+1];
   MeanFeatures::bands = ims.Bands();

   // Initialisation : recopie des regions d'entrée (rgs) vers la sortie (rgd) 
   // et calcul des tailles et sommes des régions. Les pixels
   // à 0 dans rgs sont considérés comme non traités, ceux qui appartiennent à
   // une région garderont le même label.
   for (p.y = 0; p.y < ims.Height(); p.y++)
   for (p.x = 0; p.x < ims.Width(); p.x++)
   {
      r = rgd[p] = rgs[p];
      if ((r)) {
	 for (int b = 0; b < ims.Bands(); b++ ) {
	    pixelColor[b] = (double)ims(b, p);
	 }
	 meanFeatures[r].update(pixelColor);
      } else {
	 // p est un point à 0 donc pas encore traité
	 // On va empiler p SI il y a une région dans son voisinage
	 if (connectivity == 4) {
	    for (int v = 0; v < 4; v++) {
	       p1 = p+v4[v];
	       if (rgs.Hold(p1) && (rgs[p1] !=0 )) {
		  heap.Push(p, 0);
		  break;
	       }
	    }
	 } else {
	    for (int v = 0; v < 8; v++) {
	       p1 = p + v8[v];
	       if (rgs.Hold(p1) && (rgs[p1] != 0)) {
		  heap.Push(p, 0);
		  break;
	       }
	    }
	 }
      }
   } // Fin du balayage initial de l'image rgs
   
   for (int r = 1; r < (int)rgs.Labels() + 1; r++) {
      meanFeatures[r].computeMeans();
   }

   // A ce stade tous les pixels voisins d'au moins une région sont empilés...
   
   // Nouveau mode de croissance
   // On dépile un point et on regarde si on peut l'attribuer à une région 
   // voisine en comparant son niveau de gris à la moyenne de la région 
   // (la différence doit être plus petite qu'un certain threshold pour que le
   // point soit aggrégé, sinon il disparaît définitivement de la pile).

   while (!heap.Empty()) {
      p = heap.Pop();      // On dépile un point p.
      if ((rgd[p])) {
	 continue;
      }

      if (connectivity == 4) {
	 Ulong closestRegion = 0;
	 float distanceMin = MAXFLOAT;
	 for (int b = 0; b < ims.Bands(); b++ ) {
	    pixelColor[b] = (double)ims(b, p);
	 }
	 for (int v = 0; v < 4; v++) {
	    p1 = p + v4[v];
	    if (rgd.Hold(p1) && (r = rgd[p1])) {
	       float d = meanFeatures[r].distance(pixelColor);
	       if (d <= threshold && d<distanceMin) {
		  distanceMin = d;
		  closestRegion = r;
	       }
	    }
	 }
	 
	 if (closestRegion > 0) {
	    rgd[p]=closestRegion;
	    nbpix++;
	    // On empile alors tous ses voisins non encore traités 
	    // (ceux à 0 dans rgd)
	    for (int w = 0; w < 4; w++) {
	       pt = p + v4[w];
	       if (rgd.Hold(pt) && !rgd[pt]) {
		  for (int b = 0; b < ims.Bands(); b++ ) {
		     pixelColor[b] = (double)ims(b, p);
		  }
		  heap.Push(pt, meanFeatures[closestRegion].distance(pixelColor));
	       }
	    }
	 }
      } else {
	 Ulong closestRegion = 0;
	 float distanceMin = MAXFLOAT;
	 for (int b = 0; b < ims.Bands(); b++ ) {
	    pixelColor[b] = (double)ims(b, p);
	 }
	 for (int v = 0; v < 8; v++) {
	    p1 = p + v8[v];
	    if (rgd.Hold(p1) && (r = rgd[p1])) {
	       float d = meanFeatures[r].distance(pixelColor);
	       if (d <= threshold && d < distanceMin) {
		  distanceMin = d;
		  closestRegion = r;
	       }
	    }
	 }
	 
	 if (closestRegion > 0) {
	    rgd[p] = closestRegion;
	    nbpix++;
	    // On empile alors tous ses voisins non encore traités 
	    // (ceux à 0 dans rgd)
	    for (int w = 0; w < 8; w++) {
	       pt = p + v8[w];
	       if (rgd.Hold(pt) && !rgd[pt]) {
		  for (int b = 0; b < ims.Bands(); b++ ) {
		     pixelColor[b] = (double)ims(b, p);
		  }
		  heap.Push(pt, meanFeatures[closestRegion].distance(pixelColor));
	       }
	    }
	 }
      }
   }
      
   delete[] meanFeatures;
   rgd.Labels(rgs.Labels());
   return nbpix;
}

Errc PMeanAggregation( const Reg2d &rgs, const Imc2dsl &ims, Reg2d &rgd, int connectivity, float threshold ) {
   Reg2d::ValueType r;
   MeanFeatures* meanFeatures;
   Point2d p, p1, pt;
   Ulong nbpix = 0;
   double pixelColor[3]= {0, 0, 0};

   threshold *= threshold; // To avoid sqrt.

   if (rgs.Size() != ims.Size()) {
      std::cerr << "Error pmeanaggregation: incompatible image sizes." << std::endl;
      return FAILURE;
   }
   if (connectivity != 4 && connectivity != 8) {
      std::cerr << "Error pmeanaggregation: bad connectivity value." << std::endl;
      return FAILURE;
   }
   
   Heap<Point2d, float, Ulong> heap(rgs.VectorSize());

   // Allocation et initialisation du tableau des moyennes.
   meanFeatures= new MeanFeatures[rgs.Labels()+1];
   MeanFeatures::bands = ims.Bands();

   // Initialisation : recopie des regions d'entrée (rgs) vers la sortie (rgd) 
   // et calcul des tailles et sommes des régions. Les pixels
   // à 0 dans rgs sont considérés comme non traités, ceux qui appartiennent à
   // une région garderont le même label.
   for (p.y = 0; p.y < ims.Height(); p.y++)
   for (p.x = 0; p.x < ims.Width(); p.x++)
   {
      r = rgd[p] = rgs[p];
      if ((r)) {
	 for (int b = 0; b < ims.Bands(); b++ ) {
	    pixelColor[b] = (double)ims(b, p);
	 }
	 meanFeatures[r].update(pixelColor);
      } else {
	 // p est un point à 0 donc pas encore traité
	 // On va empiler p SI il y a une région dans son voisinage
	 if (connectivity == 4) {
	    for (int v = 0; v < 4; v++) {
	       p1 = p+v4[v];
	       if (rgs.Hold(p1) && (rgs[p1] !=0 )) {
		  heap.Push(p, 0);
		  break;
	       }
	    }
	 } else {
	    for (int v = 0; v < 8; v++) {
	       p1 = p + v8[v];
	       if (rgs.Hold(p1) && (rgs[p1] != 0)) {
		  heap.Push(p, 0);
		  break;
	       }
	    }
	 }
      }
   } // Fin du balayage initial de l'image rgs
   
   for (int r = 1; r < (int)rgs.Labels() + 1; r++) {
      meanFeatures[r].computeMeans();
   }

   // A ce stade tous les pixels voisins d'au moins une région sont empilés...
   
   // Nouveau mode de croissance
   // On dépile un point et on regarde si on peut l'attribuer à une région 
   // voisine en comparant son niveau de gris à la moyenne de la région 
   // (la différence doit être plus petite qu'un certain threshold pour que le
   // point soit aggrégé, sinon il disparaît définitivement de la pile).

   while (!heap.Empty()) {
      p = heap.Pop();      // On dépile un point p.
      if ((rgd[p])) {
	 continue;
      }

      if (connectivity == 4) {
	 Ulong closestRegion = 0;
	 float distanceMin = MAXFLOAT;
	 for (int b = 0; b < ims.Bands(); b++ ) {
	    pixelColor[b] = (double)ims(b, p);
	 }
	 for (int v = 0; v < 4; v++) {
	    p1 = p + v4[v];
	    if (rgd.Hold(p1) && (r = rgd[p1])) {
	       float d = meanFeatures[r].distance(pixelColor);
	       if (d <= threshold && d<distanceMin) {
		  distanceMin = d;
		  closestRegion = r;
	       }
	    }
	 }
	 
	 if (closestRegion > 0) {
	    rgd[p]=closestRegion;
	    nbpix++;
	    // On empile alors tous ses voisins non encore traités 
	    // (ceux à 0 dans rgd)
	    for (int w = 0; w < 4; w++) {
	       pt = p + v4[w];
	       if (rgd.Hold(pt) && !rgd[pt]) {
		  for (int b = 0; b < ims.Bands(); b++ ) {
		     pixelColor[b] = (double)ims(b, p);
		  }
		  heap.Push(pt, meanFeatures[closestRegion].distance(pixelColor));
	       }
	    }
	 }
      } else {
	 Ulong closestRegion = 0;
	 float distanceMin = MAXFLOAT;
	 for (int b = 0; b < ims.Bands(); b++ ) {
	    pixelColor[b] = (double)ims(b, p);
	 }
	 for (int v = 0; v < 8; v++) {
	    p1 = p + v8[v];
	    if (rgd.Hold(p1) && (r = rgd[p1])) {
	       float d = meanFeatures[r].distance(pixelColor);
	       if (d <= threshold && d < distanceMin) {
		  distanceMin = d;
		  closestRegion = r;
	       }
	    }
	 }
	 
	 if (closestRegion > 0) {
	    rgd[p] = closestRegion;
	    nbpix++;
	    // On empile alors tous ses voisins non encore traités 
	    // (ceux à 0 dans rgd)
	    for (int w = 0; w < 8; w++) {
	       pt = p + v8[w];
	       if (rgd.Hold(pt) && !rgd[pt]) {
		  for (int b = 0; b < ims.Bands(); b++ ) {
		     pixelColor[b] = (double)ims(b, p);
		  }
		  heap.Push(pt, meanFeatures[closestRegion].distance(pixelColor));
	       }
	    }
	 }
      }
   }
      
   delete[] meanFeatures;
   rgd.Labels(rgs.Labels());
   return nbpix;
}

Errc PMeanAggregation( const Reg2d &rgs, const Imc2dsf &ims, Reg2d &rgd, int connectivity, float threshold ) {
   Reg2d::ValueType r;
   MeanFeatures* meanFeatures;
   Point2d p, p1, pt;
   Ulong nbpix = 0;
   double pixelColor[3]= {0, 0, 0};

   threshold *= threshold; // To avoid sqrt.

   if (rgs.Size() != ims.Size()) {
      std::cerr << "Error pmeanaggregation: incompatible image sizes." << std::endl;
      return FAILURE;
   }
   if (connectivity != 4 && connectivity != 8) {
      std::cerr << "Error pmeanaggregation: bad connectivity value." << std::endl;
      return FAILURE;
   }
   
   Heap<Point2d, float, Ulong> heap(rgs.VectorSize());

   // Allocation et initialisation du tableau des moyennes.
   meanFeatures= new MeanFeatures[rgs.Labels()+1];
   MeanFeatures::bands = ims.Bands();

   // Initialisation : recopie des regions d'entrée (rgs) vers la sortie (rgd) 
   // et calcul des tailles et sommes des régions. Les pixels
   // à 0 dans rgs sont considérés comme non traités, ceux qui appartiennent à
   // une région garderont le même label.
   for (p.y = 0; p.y < ims.Height(); p.y++)
   for (p.x = 0; p.x < ims.Width(); p.x++)
   {
      r = rgd[p] = rgs[p];
      if ((r)) {
	 for (int b = 0; b < ims.Bands(); b++ ) {
	    pixelColor[b] = (double)ims(b, p);
	 }
	 meanFeatures[r].update(pixelColor);
      } else {
	 // p est un point à 0 donc pas encore traité
	 // On va empiler p SI il y a une région dans son voisinage
	 if (connectivity == 4) {
	    for (int v = 0; v < 4; v++) {
	       p1 = p+v4[v];
	       if (rgs.Hold(p1) && (rgs[p1] !=0 )) {
		  heap.Push(p, 0);
		  break;
	       }
	    }
	 } else {
	    for (int v = 0; v < 8; v++) {
	       p1 = p + v8[v];
	       if (rgs.Hold(p1) && (rgs[p1] != 0)) {
		  heap.Push(p, 0);
		  break;
	       }
	    }
	 }
      }
   } // Fin du balayage initial de l'image rgs
   
   for (int r = 1; r < (int)rgs.Labels() + 1; r++) {
      meanFeatures[r].computeMeans();
   }

   // A ce stade tous les pixels voisins d'au moins une région sont empilés...
   
   // Nouveau mode de croissance
   // On dépile un point et on regarde si on peut l'attribuer à une région 
   // voisine en comparant son niveau de gris à la moyenne de la région 
   // (la différence doit être plus petite qu'un certain threshold pour que le
   // point soit aggrégé, sinon il disparaît définitivement de la pile).

   while (!heap.Empty()) {
      p = heap.Pop();      // On dépile un point p.
      if ((rgd[p])) {
	 continue;
      }

      if (connectivity == 4) {
	 Ulong closestRegion = 0;
	 float distanceMin = MAXFLOAT;
	 for (int b = 0; b < ims.Bands(); b++ ) {
	    pixelColor[b] = (double)ims(b, p);
	 }
	 for (int v = 0; v < 4; v++) {
	    p1 = p + v4[v];
	    if (rgd.Hold(p1) && (r = rgd[p1])) {
	       float d = meanFeatures[r].distance(pixelColor);
	       if (d <= threshold && d<distanceMin) {
		  distanceMin = d;
		  closestRegion = r;
	       }
	    }
	 }
	 
	 if (closestRegion > 0) {
	    rgd[p]=closestRegion;
	    nbpix++;
	    // On empile alors tous ses voisins non encore traités 
	    // (ceux à 0 dans rgd)
	    for (int w = 0; w < 4; w++) {
	       pt = p + v4[w];
	       if (rgd.Hold(pt) && !rgd[pt]) {
		  for (int b = 0; b < ims.Bands(); b++ ) {
		     pixelColor[b] = (double)ims(b, p);
		  }
		  heap.Push(pt, meanFeatures[closestRegion].distance(pixelColor));
	       }
	    }
	 }
      } else {
	 Ulong closestRegion = 0;
	 float distanceMin = MAXFLOAT;
	 for (int b = 0; b < ims.Bands(); b++ ) {
	    pixelColor[b] = (double)ims(b, p);
	 }
	 for (int v = 0; v < 8; v++) {
	    p1 = p + v8[v];
	    if (rgd.Hold(p1) && (r = rgd[p1])) {
	       float d = meanFeatures[r].distance(pixelColor);
	       if (d <= threshold && d < distanceMin) {
		  distanceMin = d;
		  closestRegion = r;
	       }
	    }
	 }
	 
	 if (closestRegion > 0) {
	    rgd[p] = closestRegion;
	    nbpix++;
	    // On empile alors tous ses voisins non encore traités 
	    // (ceux à 0 dans rgd)
	    for (int w = 0; w < 8; w++) {
	       pt = p + v8[w];
	       if (rgd.Hold(pt) && !rgd[pt]) {
		  for (int b = 0; b < ims.Bands(); b++ ) {
		     pixelColor[b] = (double)ims(b, p);
		  }
		  heap.Push(pt, meanFeatures[closestRegion].distance(pixelColor));
	       }
	    }
	 }
      }
   }
      
   delete[] meanFeatures;
   rgd.Labels(rgs.Labels());
   return nbpix;
}

Errc PMeanAggregation( const Reg3d &rgs, const Img3duc &ims, Reg3d &rgd, int connectivity, float threshold ) {
   Reg3d::ValueType r;
   MeanFeatures* meanFeatures;
   Point3d p, p1, pt;
   Ulong nbpix = 0;
   double pixelColor[3]= {0, 0, 0};

   threshold *= threshold; // To avoid sqrt.

   if (rgs.Size() != ims.Size()) {
      std::cerr << "Error pmeanaggregation: incompatible image sizes." << std::endl;
      return FAILURE;
   }
   if (connectivity != 6 && connectivity != 26) {
      std::cerr << "Error pmeanaggregation: bad connectivity value." << std::endl;
      return FAILURE;
   }
   
   Heap<Point3d, float, Ulong> heap(rgs.VectorSize());

   // Allocation et initialisation du tableau des moyennes.
   meanFeatures= new MeanFeatures[rgs.Labels()+1];
   MeanFeatures::bands = ims.Bands();

   // Initialisation : recopie des regions d'entrée (rgs) vers la sortie (rgd) 
   // et calcul des tailles et sommes des régions. Les pixels
   // à 0 dans rgs sont considérés comme non traités, ceux qui appartiennent à
   // une région garderont le même label.
   for (p.z = 0; p.z < ims.Depth(); p.z++)
   for (p.y = 0; p.y < ims.Height(); p.y++)
   for (p.x = 0; p.x < ims.Width(); p.x++)
   {
      r = rgd[p] = rgs[p];
      if ((r)) {
	 for (int b = 0; b < ims.Bands(); b++ ) {
	    pixelColor[b] = (double)ims(b, p);
	 }
	 meanFeatures[r].update(pixelColor);
      } else {
	 // p est un point à 0 donc pas encore traité
	 // On va empiler p SI il y a une région dans son voisinage
	 if (connectivity == 6) {
	    for (int v = 0; v < 6; v++) {
	       p1 = p+v6[v];
	       if (rgs.Hold(p1) && (rgs[p1] !=0 )) {
		  heap.Push(p, 0);
		  break;
	       }
	    }
	 } else {
	    for (int v = 0; v < 26; v++) {
	       p1 = p + v26[v];
	       if (rgs.Hold(p1) && (rgs[p1] != 0)) {
		  heap.Push(p, 0);
		  break;
	       }
	    }
	 }
      }
   } // Fin du balayage initial de l'image rgs
   
   for (int r = 1; r < (int)rgs.Labels() + 1; r++) {
      meanFeatures[r].computeMeans();
   }

   // A ce stade tous les pixels voisins d'au moins une région sont empilés...
   
   // Nouveau mode de croissance
   // On dépile un point et on regarde si on peut l'attribuer à une région 
   // voisine en comparant son niveau de gris à la moyenne de la région 
   // (la différence doit être plus petite qu'un certain threshold pour que le
   // point soit aggrégé, sinon il disparaît définitivement de la pile).

   while (!heap.Empty()) {
      p = heap.Pop();      // On dépile un point p.
      if ((rgd[p])) {
	 continue;
      }

      if (connectivity == 6) {
	 Ulong closestRegion = 0;
	 float distanceMin = MAXFLOAT;
	 for (int b = 0; b < ims.Bands(); b++ ) {
	    pixelColor[b] = (double)ims(b, p);
	 }
	 for (int v = 0; v < 6; v++) {
	    p1 = p + v6[v];
	    if (rgd.Hold(p1) && (r = rgd[p1])) {
	       float d = meanFeatures[r].distance(pixelColor);
	       if (d <= threshold && d<distanceMin) {
		  distanceMin = d;
		  closestRegion = r;
	       }
	    }
	 }
	 
	 if (closestRegion > 0) {
	    rgd[p]=closestRegion;
	    nbpix++;
	    // On empile alors tous ses voisins non encore traités 
	    // (ceux à 0 dans rgd)
	    for (int w = 0; w < 6; w++) {
	       pt = p + v6[w];
	       if (rgd.Hold(pt) && !rgd[pt]) {
		  for (int b = 0; b < ims.Bands(); b++ ) {
		     pixelColor[b] = (double)ims(b, p);
		  }
		  heap.Push(pt, meanFeatures[closestRegion].distance(pixelColor));
	       }
	    }
	 }
      } else {
	 Ulong closestRegion = 0;
	 float distanceMin = MAXFLOAT;
	 for (int b = 0; b < ims.Bands(); b++ ) {
	    pixelColor[b] = (double)ims(b, p);
	 }
	 for (int v = 0; v < 26; v++) {
	    p1 = p + v26[v];
	    if (rgd.Hold(p1) && (r = rgd[p1])) {
	       float d = meanFeatures[r].distance(pixelColor);
	       if (d <= threshold && d < distanceMin) {
		  distanceMin = d;
		  closestRegion = r;
	       }
	    }
	 }
	 
	 if (closestRegion > 0) {
	    rgd[p] = closestRegion;
	    nbpix++;
	    // On empile alors tous ses voisins non encore traités 
	    // (ceux à 0 dans rgd)
	    for (int w = 0; w < 26; w++) {
	       pt = p + v26[w];
	       if (rgd.Hold(pt) && !rgd[pt]) {
		  for (int b = 0; b < ims.Bands(); b++ ) {
		     pixelColor[b] = (double)ims(b, p);
		  }
		  heap.Push(pt, meanFeatures[closestRegion].distance(pixelColor));
	       }
	    }
	 }
      }
   }
      
   delete[] meanFeatures;
   rgd.Labels(rgs.Labels());
   return nbpix;
}

Errc PMeanAggregation( const Reg3d &rgs, const Img3dsl &ims, Reg3d &rgd, int connectivity, float threshold ) {
   Reg3d::ValueType r;
   MeanFeatures* meanFeatures;
   Point3d p, p1, pt;
   Ulong nbpix = 0;
   double pixelColor[3]= {0, 0, 0};

   threshold *= threshold; // To avoid sqrt.

   if (rgs.Size() != ims.Size()) {
      std::cerr << "Error pmeanaggregation: incompatible image sizes." << std::endl;
      return FAILURE;
   }
   if (connectivity != 6 && connectivity != 26) {
      std::cerr << "Error pmeanaggregation: bad connectivity value." << std::endl;
      return FAILURE;
   }
   
   Heap<Point3d, float, Ulong> heap(rgs.VectorSize());

   // Allocation et initialisation du tableau des moyennes.
   meanFeatures= new MeanFeatures[rgs.Labels()+1];
   MeanFeatures::bands = ims.Bands();

   // Initialisation : recopie des regions d'entrée (rgs) vers la sortie (rgd) 
   // et calcul des tailles et sommes des régions. Les pixels
   // à 0 dans rgs sont considérés comme non traités, ceux qui appartiennent à
   // une région garderont le même label.
   for (p.z = 0; p.z < ims.Depth(); p.z++)
   for (p.y = 0; p.y < ims.Height(); p.y++)
   for (p.x = 0; p.x < ims.Width(); p.x++)
   {
      r = rgd[p] = rgs[p];
      if ((r)) {
	 for (int b = 0; b < ims.Bands(); b++ ) {
	    pixelColor[b] = (double)ims(b, p);
	 }
	 meanFeatures[r].update(pixelColor);
      } else {
	 // p est un point à 0 donc pas encore traité
	 // On va empiler p SI il y a une région dans son voisinage
	 if (connectivity == 6) {
	    for (int v = 0; v < 6; v++) {
	       p1 = p+v6[v];
	       if (rgs.Hold(p1) && (rgs[p1] !=0 )) {
		  heap.Push(p, 0);
		  break;
	       }
	    }
	 } else {
	    for (int v = 0; v < 26; v++) {
	       p1 = p + v26[v];
	       if (rgs.Hold(p1) && (rgs[p1] != 0)) {
		  heap.Push(p, 0);
		  break;
	       }
	    }
	 }
      }
   } // Fin du balayage initial de l'image rgs
   
   for (int r = 1; r < (int)rgs.Labels() + 1; r++) {
      meanFeatures[r].computeMeans();
   }

   // A ce stade tous les pixels voisins d'au moins une région sont empilés...
   
   // Nouveau mode de croissance
   // On dépile un point et on regarde si on peut l'attribuer à une région 
   // voisine en comparant son niveau de gris à la moyenne de la région 
   // (la différence doit être plus petite qu'un certain threshold pour que le
   // point soit aggrégé, sinon il disparaît définitivement de la pile).

   while (!heap.Empty()) {
      p = heap.Pop();      // On dépile un point p.
      if ((rgd[p])) {
	 continue;
      }

      if (connectivity == 6) {
	 Ulong closestRegion = 0;
	 float distanceMin = MAXFLOAT;
	 for (int b = 0; b < ims.Bands(); b++ ) {
	    pixelColor[b] = (double)ims(b, p);
	 }
	 for (int v = 0; v < 6; v++) {
	    p1 = p + v6[v];
	    if (rgd.Hold(p1) && (r = rgd[p1])) {
	       float d = meanFeatures[r].distance(pixelColor);
	       if (d <= threshold && d<distanceMin) {
		  distanceMin = d;
		  closestRegion = r;
	       }
	    }
	 }
	 
	 if (closestRegion > 0) {
	    rgd[p]=closestRegion;
	    nbpix++;
	    // On empile alors tous ses voisins non encore traités 
	    // (ceux à 0 dans rgd)
	    for (int w = 0; w < 6; w++) {
	       pt = p + v6[w];
	       if (rgd.Hold(pt) && !rgd[pt]) {
		  for (int b = 0; b < ims.Bands(); b++ ) {
		     pixelColor[b] = (double)ims(b, p);
		  }
		  heap.Push(pt, meanFeatures[closestRegion].distance(pixelColor));
	       }
	    }
	 }
      } else {
	 Ulong closestRegion = 0;
	 float distanceMin = MAXFLOAT;
	 for (int b = 0; b < ims.Bands(); b++ ) {
	    pixelColor[b] = (double)ims(b, p);
	 }
	 for (int v = 0; v < 26; v++) {
	    p1 = p + v26[v];
	    if (rgd.Hold(p1) && (r = rgd[p1])) {
	       float d = meanFeatures[r].distance(pixelColor);
	       if (d <= threshold && d < distanceMin) {
		  distanceMin = d;
		  closestRegion = r;
	       }
	    }
	 }
	 
	 if (closestRegion > 0) {
	    rgd[p] = closestRegion;
	    nbpix++;
	    // On empile alors tous ses voisins non encore traités 
	    // (ceux à 0 dans rgd)
	    for (int w = 0; w < 26; w++) {
	       pt = p + v26[w];
	       if (rgd.Hold(pt) && !rgd[pt]) {
		  for (int b = 0; b < ims.Bands(); b++ ) {
		     pixelColor[b] = (double)ims(b, p);
		  }
		  heap.Push(pt, meanFeatures[closestRegion].distance(pixelColor));
	       }
	    }
	 }
      }
   }
      
   delete[] meanFeatures;
   rgd.Labels(rgs.Labels());
   return nbpix;
}

Errc PMeanAggregation( const Reg3d &rgs, const Img3dsf &ims, Reg3d &rgd, int connectivity, float threshold ) {
   Reg3d::ValueType r;
   MeanFeatures* meanFeatures;
   Point3d p, p1, pt;
   Ulong nbpix = 0;
   double pixelColor[3]= {0, 0, 0};

   threshold *= threshold; // To avoid sqrt.

   if (rgs.Size() != ims.Size()) {
      std::cerr << "Error pmeanaggregation: incompatible image sizes." << std::endl;
      return FAILURE;
   }
   if (connectivity != 6 && connectivity != 26) {
      std::cerr << "Error pmeanaggregation: bad connectivity value." << std::endl;
      return FAILURE;
   }
   
   Heap<Point3d, float, Ulong> heap(rgs.VectorSize());

   // Allocation et initialisation du tableau des moyennes.
   meanFeatures= new MeanFeatures[rgs.Labels()+1];
   MeanFeatures::bands = ims.Bands();

   // Initialisation : recopie des regions d'entrée (rgs) vers la sortie (rgd) 
   // et calcul des tailles et sommes des régions. Les pixels
   // à 0 dans rgs sont considérés comme non traités, ceux qui appartiennent à
   // une région garderont le même label.
   for (p.z = 0; p.z < ims.Depth(); p.z++)
   for (p.y = 0; p.y < ims.Height(); p.y++)
   for (p.x = 0; p.x < ims.Width(); p.x++)
   {
      r = rgd[p] = rgs[p];
      if ((r)) {
	 for (int b = 0; b < ims.Bands(); b++ ) {
	    pixelColor[b] = (double)ims(b, p);
	 }
	 meanFeatures[r].update(pixelColor);
      } else {
	 // p est un point à 0 donc pas encore traité
	 // On va empiler p SI il y a une région dans son voisinage
	 if (connectivity == 6) {
	    for (int v = 0; v < 6; v++) {
	       p1 = p+v6[v];
	       if (rgs.Hold(p1) && (rgs[p1] !=0 )) {
		  heap.Push(p, 0);
		  break;
	       }
	    }
	 } else {
	    for (int v = 0; v < 26; v++) {
	       p1 = p + v26[v];
	       if (rgs.Hold(p1) && (rgs[p1] != 0)) {
		  heap.Push(p, 0);
		  break;
	       }
	    }
	 }
      }
   } // Fin du balayage initial de l'image rgs
   
   for (int r = 1; r < (int)rgs.Labels() + 1; r++) {
      meanFeatures[r].computeMeans();
   }

   // A ce stade tous les pixels voisins d'au moins une région sont empilés...
   
   // Nouveau mode de croissance
   // On dépile un point et on regarde si on peut l'attribuer à une région 
   // voisine en comparant son niveau de gris à la moyenne de la région 
   // (la différence doit être plus petite qu'un certain threshold pour que le
   // point soit aggrégé, sinon il disparaît définitivement de la pile).

   while (!heap.Empty()) {
      p = heap.Pop();      // On dépile un point p.
      if ((rgd[p])) {
	 continue;
      }

      if (connectivity == 6) {
	 Ulong closestRegion = 0;
	 float distanceMin = MAXFLOAT;
	 for (int b = 0; b < ims.Bands(); b++ ) {
	    pixelColor[b] = (double)ims(b, p);
	 }
	 for (int v = 0; v < 6; v++) {
	    p1 = p + v6[v];
	    if (rgd.Hold(p1) && (r = rgd[p1])) {
	       float d = meanFeatures[r].distance(pixelColor);
	       if (d <= threshold && d<distanceMin) {
		  distanceMin = d;
		  closestRegion = r;
	       }
	    }
	 }
	 
	 if (closestRegion > 0) {
	    rgd[p]=closestRegion;
	    nbpix++;
	    // On empile alors tous ses voisins non encore traités 
	    // (ceux à 0 dans rgd)
	    for (int w = 0; w < 6; w++) {
	       pt = p + v6[w];
	       if (rgd.Hold(pt) && !rgd[pt]) {
		  for (int b = 0; b < ims.Bands(); b++ ) {
		     pixelColor[b] = (double)ims(b, p);
		  }
		  heap.Push(pt, meanFeatures[closestRegion].distance(pixelColor));
	       }
	    }
	 }
      } else {
	 Ulong closestRegion = 0;
	 float distanceMin = MAXFLOAT;
	 for (int b = 0; b < ims.Bands(); b++ ) {
	    pixelColor[b] = (double)ims(b, p);
	 }
	 for (int v = 0; v < 26; v++) {
	    p1 = p + v26[v];
	    if (rgd.Hold(p1) && (r = rgd[p1])) {
	       float d = meanFeatures[r].distance(pixelColor);
	       if (d <= threshold && d < distanceMin) {
		  distanceMin = d;
		  closestRegion = r;
	       }
	    }
	 }
	 
	 if (closestRegion > 0) {
	    rgd[p] = closestRegion;
	    nbpix++;
	    // On empile alors tous ses voisins non encore traités 
	    // (ceux à 0 dans rgd)
	    for (int w = 0; w < 26; w++) {
	       pt = p + v26[w];
	       if (rgd.Hold(pt) && !rgd[pt]) {
		  for (int b = 0; b < ims.Bands(); b++ ) {
		     pixelColor[b] = (double)ims(b, p);
		  }
		  heap.Push(pt, meanFeatures[closestRegion].distance(pixelColor));
	       }
	    }
	 }
      }
   }
      
   delete[] meanFeatures;
   rgd.Labels(rgs.Labels());
   return nbpix;
}

Errc PMeanAggregation( const Reg3d &rgs, const Imc3duc &ims, Reg3d &rgd, int connectivity, float threshold ) {
   Reg3d::ValueType r;
   MeanFeatures* meanFeatures;
   Point3d p, p1, pt;
   Ulong nbpix = 0;
   double pixelColor[3]= {0, 0, 0};

   threshold *= threshold; // To avoid sqrt.

   if (rgs.Size() != ims.Size()) {
      std::cerr << "Error pmeanaggregation: incompatible image sizes." << std::endl;
      return FAILURE;
   }
   if (connectivity != 6 && connectivity != 26) {
      std::cerr << "Error pmeanaggregation: bad connectivity value." << std::endl;
      return FAILURE;
   }
   
   Heap<Point3d, float, Ulong> heap(rgs.VectorSize());

   // Allocation et initialisation du tableau des moyennes.
   meanFeatures= new MeanFeatures[rgs.Labels()+1];
   MeanFeatures::bands = ims.Bands();

   // Initialisation : recopie des regions d'entrée (rgs) vers la sortie (rgd) 
   // et calcul des tailles et sommes des régions. Les pixels
   // à 0 dans rgs sont considérés comme non traités, ceux qui appartiennent à
   // une région garderont le même label.
   for (p.z = 0; p.z < ims.Depth(); p.z++)
   for (p.y = 0; p.y < ims.Height(); p.y++)
   for (p.x = 0; p.x < ims.Width(); p.x++)
   {
      r = rgd[p] = rgs[p];
      if ((r)) {
	 for (int b = 0; b < ims.Bands(); b++ ) {
	    pixelColor[b] = (double)ims(b, p);
	 }
	 meanFeatures[r].update(pixelColor);
      } else {
	 // p est un point à 0 donc pas encore traité
	 // On va empiler p SI il y a une région dans son voisinage
	 if (connectivity == 6) {
	    for (int v = 0; v < 6; v++) {
	       p1 = p+v6[v];
	       if (rgs.Hold(p1) && (rgs[p1] !=0 )) {
		  heap.Push(p, 0);
		  break;
	       }
	    }
	 } else {
	    for (int v = 0; v < 26; v++) {
	       p1 = p + v26[v];
	       if (rgs.Hold(p1) && (rgs[p1] != 0)) {
		  heap.Push(p, 0);
		  break;
	       }
	    }
	 }
      }
   } // Fin du balayage initial de l'image rgs
   
   for (int r = 1; r < (int)rgs.Labels() + 1; r++) {
      meanFeatures[r].computeMeans();
   }

   // A ce stade tous les pixels voisins d'au moins une région sont empilés...
   
   // Nouveau mode de croissance
   // On dépile un point et on regarde si on peut l'attribuer à une région 
   // voisine en comparant son niveau de gris à la moyenne de la région 
   // (la différence doit être plus petite qu'un certain threshold pour que le
   // point soit aggrégé, sinon il disparaît définitivement de la pile).

   while (!heap.Empty()) {
      p = heap.Pop();      // On dépile un point p.
      if ((rgd[p])) {
	 continue;
      }

      if (connectivity == 6) {
	 Ulong closestRegion = 0;
	 float distanceMin = MAXFLOAT;
	 for (int b = 0; b < ims.Bands(); b++ ) {
	    pixelColor[b] = (double)ims(b, p);
	 }
	 for (int v = 0; v < 6; v++) {
	    p1 = p + v6[v];
	    if (rgd.Hold(p1) && (r = rgd[p1])) {
	       float d = meanFeatures[r].distance(pixelColor);
	       if (d <= threshold && d<distanceMin) {
		  distanceMin = d;
		  closestRegion = r;
	       }
	    }
	 }
	 
	 if (closestRegion > 0) {
	    rgd[p]=closestRegion;
	    nbpix++;
	    // On empile alors tous ses voisins non encore traités 
	    // (ceux à 0 dans rgd)
	    for (int w = 0; w < 6; w++) {
	       pt = p + v6[w];
	       if (rgd.Hold(pt) && !rgd[pt]) {
		  for (int b = 0; b < ims.Bands(); b++ ) {
		     pixelColor[b] = (double)ims(b, p);
		  }
		  heap.Push(pt, meanFeatures[closestRegion].distance(pixelColor));
	       }
	    }
	 }
      } else {
	 Ulong closestRegion = 0;
	 float distanceMin = MAXFLOAT;
	 for (int b = 0; b < ims.Bands(); b++ ) {
	    pixelColor[b] = (double)ims(b, p);
	 }
	 for (int v = 0; v < 26; v++) {
	    p1 = p + v26[v];
	    if (rgd.Hold(p1) && (r = rgd[p1])) {
	       float d = meanFeatures[r].distance(pixelColor);
	       if (d <= threshold && d < distanceMin) {
		  distanceMin = d;
		  closestRegion = r;
	       }
	    }
	 }
	 
	 if (closestRegion > 0) {
	    rgd[p] = closestRegion;
	    nbpix++;
	    // On empile alors tous ses voisins non encore traités 
	    // (ceux à 0 dans rgd)
	    for (int w = 0; w < 26; w++) {
	       pt = p + v26[w];
	       if (rgd.Hold(pt) && !rgd[pt]) {
		  for (int b = 0; b < ims.Bands(); b++ ) {
		     pixelColor[b] = (double)ims(b, p);
		  }
		  heap.Push(pt, meanFeatures[closestRegion].distance(pixelColor));
	       }
	    }
	 }
      }
   }
      
   delete[] meanFeatures;
   rgd.Labels(rgs.Labels());
   return nbpix;
}

Errc PMeanAggregation( const Reg3d &rgs, const Imc3dsl &ims, Reg3d &rgd, int connectivity, float threshold ) {
   Reg3d::ValueType r;
   MeanFeatures* meanFeatures;
   Point3d p, p1, pt;
   Ulong nbpix = 0;
   double pixelColor[3]= {0, 0, 0};

   threshold *= threshold; // To avoid sqrt.

   if (rgs.Size() != ims.Size()) {
      std::cerr << "Error pmeanaggregation: incompatible image sizes." << std::endl;
      return FAILURE;
   }
   if (connectivity != 6 && connectivity != 26) {
      std::cerr << "Error pmeanaggregation: bad connectivity value." << std::endl;
      return FAILURE;
   }
   
   Heap<Point3d, float, Ulong> heap(rgs.VectorSize());

   // Allocation et initialisation du tableau des moyennes.
   meanFeatures= new MeanFeatures[rgs.Labels()+1];
   MeanFeatures::bands = ims.Bands();

   // Initialisation : recopie des regions d'entrée (rgs) vers la sortie (rgd) 
   // et calcul des tailles et sommes des régions. Les pixels
   // à 0 dans rgs sont considérés comme non traités, ceux qui appartiennent à
   // une région garderont le même label.
   for (p.z = 0; p.z < ims.Depth(); p.z++)
   for (p.y = 0; p.y < ims.Height(); p.y++)
   for (p.x = 0; p.x < ims.Width(); p.x++)
   {
      r = rgd[p] = rgs[p];
      if ((r)) {
	 for (int b = 0; b < ims.Bands(); b++ ) {
	    pixelColor[b] = (double)ims(b, p);
	 }
	 meanFeatures[r].update(pixelColor);
      } else {
	 // p est un point à 0 donc pas encore traité
	 // On va empiler p SI il y a une région dans son voisinage
	 if (connectivity == 6) {
	    for (int v = 0; v < 6; v++) {
	       p1 = p+v6[v];
	       if (rgs.Hold(p1) && (rgs[p1] !=0 )) {
		  heap.Push(p, 0);
		  break;
	       }
	    }
	 } else {
	    for (int v = 0; v < 26; v++) {
	       p1 = p + v26[v];
	       if (rgs.Hold(p1) && (rgs[p1] != 0)) {
		  heap.Push(p, 0);
		  break;
	       }
	    }
	 }
      }
   } // Fin du balayage initial de l'image rgs
   
   for (int r = 1; r < (int)rgs.Labels() + 1; r++) {
      meanFeatures[r].computeMeans();
   }

   // A ce stade tous les pixels voisins d'au moins une région sont empilés...
   
   // Nouveau mode de croissance
   // On dépile un point et on regarde si on peut l'attribuer à une région 
   // voisine en comparant son niveau de gris à la moyenne de la région 
   // (la différence doit être plus petite qu'un certain threshold pour que le
   // point soit aggrégé, sinon il disparaît définitivement de la pile).

   while (!heap.Empty()) {
      p = heap.Pop();      // On dépile un point p.
      if ((rgd[p])) {
	 continue;
      }

      if (connectivity == 6) {
	 Ulong closestRegion = 0;
	 float distanceMin = MAXFLOAT;
	 for (int b = 0; b < ims.Bands(); b++ ) {
	    pixelColor[b] = (double)ims(b, p);
	 }
	 for (int v = 0; v < 6; v++) {
	    p1 = p + v6[v];
	    if (rgd.Hold(p1) && (r = rgd[p1])) {
	       float d = meanFeatures[r].distance(pixelColor);
	       if (d <= threshold && d<distanceMin) {
		  distanceMin = d;
		  closestRegion = r;
	       }
	    }
	 }
	 
	 if (closestRegion > 0) {
	    rgd[p]=closestRegion;
	    nbpix++;
	    // On empile alors tous ses voisins non encore traités 
	    // (ceux à 0 dans rgd)
	    for (int w = 0; w < 6; w++) {
	       pt = p + v6[w];
	       if (rgd.Hold(pt) && !rgd[pt]) {
		  for (int b = 0; b < ims.Bands(); b++ ) {
		     pixelColor[b] = (double)ims(b, p);
		  }
		  heap.Push(pt, meanFeatures[closestRegion].distance(pixelColor));
	       }
	    }
	 }
      } else {
	 Ulong closestRegion = 0;
	 float distanceMin = MAXFLOAT;
	 for (int b = 0; b < ims.Bands(); b++ ) {
	    pixelColor[b] = (double)ims(b, p);
	 }
	 for (int v = 0; v < 26; v++) {
	    p1 = p + v26[v];
	    if (rgd.Hold(p1) && (r = rgd[p1])) {
	       float d = meanFeatures[r].distance(pixelColor);
	       if (d <= threshold && d < distanceMin) {
		  distanceMin = d;
		  closestRegion = r;
	       }
	    }
	 }
	 
	 if (closestRegion > 0) {
	    rgd[p] = closestRegion;
	    nbpix++;
	    // On empile alors tous ses voisins non encore traités 
	    // (ceux à 0 dans rgd)
	    for (int w = 0; w < 26; w++) {
	       pt = p + v26[w];
	       if (rgd.Hold(pt) && !rgd[pt]) {
		  for (int b = 0; b < ims.Bands(); b++ ) {
		     pixelColor[b] = (double)ims(b, p);
		  }
		  heap.Push(pt, meanFeatures[closestRegion].distance(pixelColor));
	       }
	    }
	 }
      }
   }
      
   delete[] meanFeatures;
   rgd.Labels(rgs.Labels());
   return nbpix;
}

Errc PMeanAggregation( const Reg3d &rgs, const Imc3dsf &ims, Reg3d &rgd, int connectivity, float threshold ) {
   Reg3d::ValueType r;
   MeanFeatures* meanFeatures;
   Point3d p, p1, pt;
   Ulong nbpix = 0;
   double pixelColor[3]= {0, 0, 0};

   threshold *= threshold; // To avoid sqrt.

   if (rgs.Size() != ims.Size()) {
      std::cerr << "Error pmeanaggregation: incompatible image sizes." << std::endl;
      return FAILURE;
   }
   if (connectivity != 6 && connectivity != 26) {
      std::cerr << "Error pmeanaggregation: bad connectivity value." << std::endl;
      return FAILURE;
   }
   
   Heap<Point3d, float, Ulong> heap(rgs.VectorSize());

   // Allocation et initialisation du tableau des moyennes.
   meanFeatures= new MeanFeatures[rgs.Labels()+1];
   MeanFeatures::bands = ims.Bands();

   // Initialisation : recopie des regions d'entrée (rgs) vers la sortie (rgd) 
   // et calcul des tailles et sommes des régions. Les pixels
   // à 0 dans rgs sont considérés comme non traités, ceux qui appartiennent à
   // une région garderont le même label.
   for (p.z = 0; p.z < ims.Depth(); p.z++)
   for (p.y = 0; p.y < ims.Height(); p.y++)
   for (p.x = 0; p.x < ims.Width(); p.x++)
   {
      r = rgd[p] = rgs[p];
      if ((r)) {
	 for (int b = 0; b < ims.Bands(); b++ ) {
	    pixelColor[b] = (double)ims(b, p);
	 }
	 meanFeatures[r].update(pixelColor);
      } else {
	 // p est un point à 0 donc pas encore traité
	 // On va empiler p SI il y a une région dans son voisinage
	 if (connectivity == 6) {
	    for (int v = 0; v < 6; v++) {
	       p1 = p+v6[v];
	       if (rgs.Hold(p1) && (rgs[p1] !=0 )) {
		  heap.Push(p, 0);
		  break;
	       }
	    }
	 } else {
	    for (int v = 0; v < 26; v++) {
	       p1 = p + v26[v];
	       if (rgs.Hold(p1) && (rgs[p1] != 0)) {
		  heap.Push(p, 0);
		  break;
	       }
	    }
	 }
      }
   } // Fin du balayage initial de l'image rgs
   
   for (int r = 1; r < (int)rgs.Labels() + 1; r++) {
      meanFeatures[r].computeMeans();
   }

   // A ce stade tous les pixels voisins d'au moins une région sont empilés...
   
   // Nouveau mode de croissance
   // On dépile un point et on regarde si on peut l'attribuer à une région 
   // voisine en comparant son niveau de gris à la moyenne de la région 
   // (la différence doit être plus petite qu'un certain threshold pour que le
   // point soit aggrégé, sinon il disparaît définitivement de la pile).

   while (!heap.Empty()) {
      p = heap.Pop();      // On dépile un point p.
      if ((rgd[p])) {
	 continue;
      }

      if (connectivity == 6) {
	 Ulong closestRegion = 0;
	 float distanceMin = MAXFLOAT;
	 for (int b = 0; b < ims.Bands(); b++ ) {
	    pixelColor[b] = (double)ims(b, p);
	 }
	 for (int v = 0; v < 6; v++) {
	    p1 = p + v6[v];
	    if (rgd.Hold(p1) && (r = rgd[p1])) {
	       float d = meanFeatures[r].distance(pixelColor);
	       if (d <= threshold && d<distanceMin) {
		  distanceMin = d;
		  closestRegion = r;
	       }
	    }
	 }
	 
	 if (closestRegion > 0) {
	    rgd[p]=closestRegion;
	    nbpix++;
	    // On empile alors tous ses voisins non encore traités 
	    // (ceux à 0 dans rgd)
	    for (int w = 0; w < 6; w++) {
	       pt = p + v6[w];
	       if (rgd.Hold(pt) && !rgd[pt]) {
		  for (int b = 0; b < ims.Bands(); b++ ) {
		     pixelColor[b] = (double)ims(b, p);
		  }
		  heap.Push(pt, meanFeatures[closestRegion].distance(pixelColor));
	       }
	    }
	 }
      } else {
	 Ulong closestRegion = 0;
	 float distanceMin = MAXFLOAT;
	 for (int b = 0; b < ims.Bands(); b++ ) {
	    pixelColor[b] = (double)ims(b, p);
	 }
	 for (int v = 0; v < 26; v++) {
	    p1 = p + v26[v];
	    if (rgd.Hold(p1) && (r = rgd[p1])) {
	       float d = meanFeatures[r].distance(pixelColor);
	       if (d <= threshold && d < distanceMin) {
		  distanceMin = d;
		  closestRegion = r;
	       }
	    }
	 }
	 
	 if (closestRegion > 0) {
	    rgd[p] = closestRegion;
	    nbpix++;
	    // On empile alors tous ses voisins non encore traités 
	    // (ceux à 0 dans rgd)
	    for (int w = 0; w < 26; w++) {
	       pt = p + v26[w];
	       if (rgd.Hold(pt) && !rgd[pt]) {
		  for (int b = 0; b < ims.Bands(); b++ ) {
		     pixelColor[b] = (double)ims(b, p);
		  }
		  heap.Push(pt, meanFeatures[closestRegion].distance(pixelColor));
	       }
	    }
	 }
      }
   }
      
   delete[] meanFeatures;
   rgd.Labels(rgs.Labels());
   return nbpix;
}


#ifdef MAIN
#define USAGE   "usage: %s connexity threshold [-m mask] [rg_in|-] [im_in|-] [rg_out|-]"
#define PARC    2
#define FINC    2
#define FOUTC   1
#define MASK    1

int main( int argc, char *argv[] ) {
   Errc result;                // The result code of the execution.
   Pobject* mask;              // The region map.
   Pobject* objin[FINC + 1];   // The input objects.
   Pobject* objs[FINC + 1];    // The source objects masked.
   Pobject* objout[FOUTC + 1]; // The output object.
   Pobject* objd[FOUTC + 1];   // The result object of the execution.
   char* parv[PARC + 1];       // The input parameters.

   ReadArgs(argc, argv, PARC, FINC, FOUTC, &mask, objin, objs, objout, objd, parv, USAGE, MASK); 
   if ((objs[0]->Type() == Po_Reg2d) && (objs[1]->Type() == Po_Img2duc)) {
      Reg2d* const rgs = (Reg2d*)objs[0];
      Img2duc* const ims = (Img2duc*)objs[1];
      objd[0] = new Reg2d(rgs->Size());
      Reg2d* const rgd = (Reg2d*)objd[0];

      result = PMeanAggregation(*rgs, *ims, *rgd, (Long)atoi(parv[0]), (float)atof(parv[1]));
      goto end;
   }

   if ((objs[0]->Type() == Po_Reg2d) && (objs[1]->Type() == Po_Img2dsl)) {
      Reg2d* const rgs = (Reg2d*)objs[0];
      Img2dsl* const ims = (Img2dsl*)objs[1];
      objd[0] = new Reg2d(rgs->Size());
      Reg2d* const rgd = (Reg2d*)objd[0];

      result = PMeanAggregation(*rgs, *ims, *rgd, (Long)atoi(parv[0]), (float)atof(parv[1]));
      goto end;
   }

   if ((objs[0]->Type() == Po_Reg2d) && (objs[1]->Type() == Po_Img2dsf)) {
      Reg2d* const rgs = (Reg2d*)objs[0];
      Img2dsf* const ims = (Img2dsf*)objs[1];
      objd[0] = new Reg2d(rgs->Size());
      Reg2d* const rgd = (Reg2d*)objd[0];

      result = PMeanAggregation(*rgs, *ims, *rgd, (Long)atoi(parv[0]), (float)atof(parv[1]));
      goto end;
   }

   if ((objs[0]->Type() == Po_Reg2d) && (objs[1]->Type() == Po_Imc2duc)) {
      Reg2d* const rgs = (Reg2d*)objs[0];
      Imc2duc* const ims = (Imc2duc*)objs[1];
      objd[0] = new Reg2d(rgs->Size());
      Reg2d* const rgd = (Reg2d*)objd[0];

      result = PMeanAggregation(*rgs, *ims, *rgd, (Long)atoi(parv[0]), (float)atof(parv[1]));
      goto end;
   }

   if ((objs[0]->Type() == Po_Reg2d) && (objs[1]->Type() == Po_Imc2dsl)) {
      Reg2d* const rgs = (Reg2d*)objs[0];
      Imc2dsl* const ims = (Imc2dsl*)objs[1];
      objd[0] = new Reg2d(rgs->Size());
      Reg2d* const rgd = (Reg2d*)objd[0];

      result = PMeanAggregation(*rgs, *ims, *rgd, (Long)atoi(parv[0]), (float)atof(parv[1]));
      goto end;
   }

   if ((objs[0]->Type() == Po_Reg2d) && (objs[1]->Type() == Po_Imc2dsf)) {
      Reg2d* const rgs = (Reg2d*)objs[0];
      Imc2dsf* const ims = (Imc2dsf*)objs[1];
      objd[0] = new Reg2d(rgs->Size());
      Reg2d* const rgd = (Reg2d*)objd[0];

      result = PMeanAggregation(*rgs, *ims, *rgd, (Long)atoi(parv[0]), (float)atof(parv[1]));
      goto end;
   }

   if ((objs[0]->Type() == Po_Reg3d) && (objs[1]->Type() == Po_Img3duc)) {
      Reg3d* const rgs = (Reg3d*)objs[0];
      Img3duc* const ims = (Img3duc*)objs[1];
      objd[0] = new Reg3d(rgs->Size());
      Reg3d* const rgd = (Reg3d*)objd[0];

      result = PMeanAggregation(*rgs, *ims, *rgd, (Long)atoi(parv[0]), (float)atof(parv[1]));
      goto end;
   }

   if ((objs[0]->Type() == Po_Reg3d) && (objs[1]->Type() == Po_Img3dsl)) {
      Reg3d* const rgs = (Reg3d*)objs[0];
      Img3dsl* const ims = (Img3dsl*)objs[1];
      objd[0] = new Reg3d(rgs->Size());
      Reg3d* const rgd = (Reg3d*)objd[0];

      result = PMeanAggregation(*rgs, *ims, *rgd, (Long)atoi(parv[0]), (float)atof(parv[1]));
      goto end;
   }

   if ((objs[0]->Type() == Po_Reg3d) && (objs[1]->Type() == Po_Img3dsf)) {
      Reg3d* const rgs = (Reg3d*)objs[0];
      Img3dsf* const ims = (Img3dsf*)objs[1];
      objd[0] = new Reg3d(rgs->Size());
      Reg3d* const rgd = (Reg3d*)objd[0];

      result = PMeanAggregation(*rgs, *ims, *rgd, (Long)atoi(parv[0]), (float)atof(parv[1]));
      goto end;
   }

   if ((objs[0]->Type() == Po_Reg3d) && (objs[1]->Type() == Po_Imc3duc)) {
      Reg3d* const rgs = (Reg3d*)objs[0];
      Imc3duc* const ims = (Imc3duc*)objs[1];
      objd[0] = new Reg3d(rgs->Size());
      Reg3d* const rgd = (Reg3d*)objd[0];

      result = PMeanAggregation(*rgs, *ims, *rgd, (Long)atoi(parv[0]), (float)atof(parv[1]));
      goto end;
   }

   if ((objs[0]->Type() == Po_Reg3d) && (objs[1]->Type() == Po_Imc3dsl)) {
      Reg3d* const rgs = (Reg3d*)objs[0];
      Imc3dsl* const ims = (Imc3dsl*)objs[1];
      objd[0] = new Reg3d(rgs->Size());
      Reg3d* const rgd = (Reg3d*)objd[0];

      result = PMeanAggregation(*rgs, *ims, *rgd, (Long)atoi(parv[0]), (float)atof(parv[1]));
      goto end;
   }

   if ((objs[0]->Type() == Po_Reg3d) && (objs[1]->Type() == Po_Imc3dsf)) {
      Reg3d* const rgs = (Reg3d*)objs[0];
      Imc3dsf* const ims = (Imc3dsf*)objs[1];
      objd[0] = new Reg3d(rgs->Size());
      Reg3d* const rgd = (Reg3d*)objd[0];

      result = PMeanAggregation(*rgs, *ims, *rgd, (Long)atoi(parv[0]), (float)atof(parv[1]));
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
