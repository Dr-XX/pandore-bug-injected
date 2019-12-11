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
 * @author Régis Clouard, Abder Elmoataz - 2003-06-19
 */

/**
 * @file pareaopening.cpp
 * Area opening to remove clear objects with area lower than a given area.
 */

#include <pandore.h>
using namespace pandore;
#define UNVISITED 0
#define MARKED 1
#define CANDIDATE 2


Errc PAreaOpening( const Img2duc &ims, Img2duc &imd, int connexity, int area ) {
   Point2d p, p1, p2;
   float height;
   Lifo<Point2d> marked;
   int v, nbpix;

   Heap<Point2d, float, int> candidate(ims.VectorSize());
   Img2duc imi(ims.Size());

   imd = ims;
   imi = UNVISITED;

   for (p.y = 0; p.y < imd.Height(); p.y++)
   for (p.x = 0; p.x < imd.Width(); p.x++)
   {
      // Uniquement les points non encore traites.
      if (imi[p] == MARKED) {
	 continue;
      }

      // 1- * echerche d'un maximum local
      // ims[p] >=voisins et au moins 1 ims[voisin] < ims[p].
      bool maximum = true;
      bool maximumstrict = false;

      if (connexity == 4) {
	 for (v = 0; v < 4; v++) {
	    p1 = p + v4[v];
	    if (imd.Hold(p1)) {
	       if (imd[p] < imd[p1]) {
		  maximum = false;
		  break;
	       }
	       if (imd[p] > imd[p1])  {
		  maximumstrict = true;
	       }
	    }
	 }
      } else {
	 for (v = 0; v < 8; v++) {
	    p1 = p + v8[v];
	    if (imd.Hold(p1)) {
	       if (imd[p] < imd[p1]) {
		  maximum = false;
		  break;
	       }
	       if (imd[p] > imd[p1]) {
		  maximumstrict = true;
	       }
	    }
	 }
      }

      if (!maximum || !maximumstrict) {
	 continue;
      }

      // C'est un maximum : Empiler ce point qui devient la reference.
      // Attention: dans un tas Pandore c'est le minimum qui est en tete.
      height = (float)imd[p];
      candidate.Push(p, -height);
      nbpix = 0;
      imi[p] = CANDIDATE;

      // Recherche de la taille de la surface de la region
      // dont p est l'extrema local.
      while (nbpix < area && !candidate.Empty()) {
	 p1 = candidate.Pop();
	 
	 // La region remonte en intensite-> on arrete.
	 if ((float)imd[p1] > height) {
	    imi[p1] -= CANDIDATE;
	    break;
	 }

	 height = (float)imd[p1];
	 marked.Push(p1);
	 nbpix++;

	 // Empiler tous ses voisins non encore traites.
	 // A l'envers pour empiler dans le sens clockwise.
	 // Plus efficace dans le parcours? a tester.
	 if (connexity == 4) {
	    for (v = 4 - 1; v >= 0; v--) {
	       p2 = p1 + v4[v];
	       if (imd.Hold(p2) && imi[p2] < CANDIDATE) {
		  candidate.Push(p2, -(float)imd[p2]);
		  imi[p2] += CANDIDATE; // Positionne a entasse.
	       }
	    }
	 } else {
	    for (v = 8 - 1; v >= 0; v--) {
	       p2 = p1 + v8[v];
	       if (imd.Hold(p2) && imi[p2] < CANDIDATE) {
		  candidate.Push(p2, -(float)imd[p2]);
		  imi[p2] += CANDIDATE; // Positionne a entasse.
	       }
	    }
	 }
      }

      // Les remettre a "non visites".
      while (!candidate.Empty()) {
	 p2 = candidate.Pop();
  	 imi[p2] -= CANDIDATE;
      }

      // Mettre tous les pixels de la pile à la valeur height.
      while (!marked.Empty()) {
	 p2 = marked.Pop();
	 imd[p2] = (Img2duc::ValueType)height;
  	 imi[p2] = MARKED;
      }
   }

   return SUCCESS;
}

Errc PAreaOpening( const Img2dsl &ims, Img2dsl &imd, int connexity, int area ) {
   Point2d p, p1, p2;
   float height;
   Lifo<Point2d> marked;
   int v, nbpix;

   Heap<Point2d, float, int> candidate(ims.VectorSize());
   Img2duc imi(ims.Size());

   imd = ims;
   imi = UNVISITED;

   for (p.y = 0; p.y < imd.Height(); p.y++)
   for (p.x = 0; p.x < imd.Width(); p.x++)
   {
      // Uniquement les points non encore traites.
      if (imi[p] == MARKED) {
	 continue;
      }

      // 1- * echerche d'un maximum local
      // ims[p] >=voisins et au moins 1 ims[voisin] < ims[p].
      bool maximum = true;
      bool maximumstrict = false;

      if (connexity == 4) {
	 for (v = 0; v < 4; v++) {
	    p1 = p + v4[v];
	    if (imd.Hold(p1)) {
	       if (imd[p] < imd[p1]) {
		  maximum = false;
		  break;
	       }
	       if (imd[p] > imd[p1])  {
		  maximumstrict = true;
	       }
	    }
	 }
      } else {
	 for (v = 0; v < 8; v++) {
	    p1 = p + v8[v];
	    if (imd.Hold(p1)) {
	       if (imd[p] < imd[p1]) {
		  maximum = false;
		  break;
	       }
	       if (imd[p] > imd[p1]) {
		  maximumstrict = true;
	       }
	    }
	 }
      }

      if (!maximum || !maximumstrict) {
	 continue;
      }

      // C'est un maximum : Empiler ce point qui devient la reference.
      // Attention: dans un tas Pandore c'est le minimum qui est en tete.
      height = (float)imd[p];
      candidate.Push(p, -height);
      nbpix = 0;
      imi[p] = CANDIDATE;

      // Recherche de la taille de la surface de la region
      // dont p est l'extrema local.
      while (nbpix < area && !candidate.Empty()) {
	 p1 = candidate.Pop();
	 
	 // La region remonte en intensite-> on arrete.
	 if ((float)imd[p1] > height) {
	    imi[p1] -= CANDIDATE;
	    break;
	 }

	 height = (float)imd[p1];
	 marked.Push(p1);
	 nbpix++;

	 // Empiler tous ses voisins non encore traites.
	 // A l'envers pour empiler dans le sens clockwise.
	 // Plus efficace dans le parcours? a tester.
	 if (connexity == 4) {
	    for (v = 4 - 1; v >= 0; v--) {
	       p2 = p1 + v4[v];
	       if (imd.Hold(p2) && imi[p2] < CANDIDATE) {
		  candidate.Push(p2, -(float)imd[p2]);
		  imi[p2] += CANDIDATE; // Positionne a entasse.
	       }
	    }
	 } else {
	    for (v = 8 - 1; v >= 0; v--) {
	       p2 = p1 + v8[v];
	       if (imd.Hold(p2) && imi[p2] < CANDIDATE) {
		  candidate.Push(p2, -(float)imd[p2]);
		  imi[p2] += CANDIDATE; // Positionne a entasse.
	       }
	    }
	 }
      }

      // Les remettre a "non visites".
      while (!candidate.Empty()) {
	 p2 = candidate.Pop();
  	 imi[p2] -= CANDIDATE;
      }

      // Mettre tous les pixels de la pile à la valeur height.
      while (!marked.Empty()) {
	 p2 = marked.Pop();
	 imd[p2] = (Img2dsl::ValueType)height;
  	 imi[p2] = MARKED;
      }
   }

   return SUCCESS;
}

Errc PAreaOpening( const Img2dsf &ims, Img2dsf &imd, int connexity, int area ) {
   Point2d p, p1, p2;
   float height;
   Lifo<Point2d> marked;
   int v, nbpix;

   Heap<Point2d, float, int> candidate(ims.VectorSize());
   Img2duc imi(ims.Size());

   imd = ims;
   imi = UNVISITED;

   for (p.y = 0; p.y < imd.Height(); p.y++)
   for (p.x = 0; p.x < imd.Width(); p.x++)
   {
      // Uniquement les points non encore traites.
      if (imi[p] == MARKED) {
	 continue;
      }

      // 1- * echerche d'un maximum local
      // ims[p] >=voisins et au moins 1 ims[voisin] < ims[p].
      bool maximum = true;
      bool maximumstrict = false;

      if (connexity == 4) {
	 for (v = 0; v < 4; v++) {
	    p1 = p + v4[v];
	    if (imd.Hold(p1)) {
	       if (imd[p] < imd[p1]) {
		  maximum = false;
		  break;
	       }
	       if (imd[p] > imd[p1])  {
		  maximumstrict = true;
	       }
	    }
	 }
      } else {
	 for (v = 0; v < 8; v++) {
	    p1 = p + v8[v];
	    if (imd.Hold(p1)) {
	       if (imd[p] < imd[p1]) {
		  maximum = false;
		  break;
	       }
	       if (imd[p] > imd[p1]) {
		  maximumstrict = true;
	       }
	    }
	 }
      }

      if (!maximum || !maximumstrict) {
	 continue;
      }

      // C'est un maximum : Empiler ce point qui devient la reference.
      // Attention: dans un tas Pandore c'est le minimum qui est en tete.
      height = (float)imd[p];
      candidate.Push(p, -height);
      nbpix = 0;
      imi[p] = CANDIDATE;

      // Recherche de la taille de la surface de la region
      // dont p est l'extrema local.
      while (nbpix < area && !candidate.Empty()) {
	 p1 = candidate.Pop();
	 
	 // La region remonte en intensite-> on arrete.
	 if ((float)imd[p1] > height) {
	    imi[p1] -= CANDIDATE;
	    break;
	 }

	 height = (float)imd[p1];
	 marked.Push(p1);
	 nbpix++;

	 // Empiler tous ses voisins non encore traites.
	 // A l'envers pour empiler dans le sens clockwise.
	 // Plus efficace dans le parcours? a tester.
	 if (connexity == 4) {
	    for (v = 4 - 1; v >= 0; v--) {
	       p2 = p1 + v4[v];
	       if (imd.Hold(p2) && imi[p2] < CANDIDATE) {
		  candidate.Push(p2, -(float)imd[p2]);
		  imi[p2] += CANDIDATE; // Positionne a entasse.
	       }
	    }
	 } else {
	    for (v = 8 - 1; v >= 0; v--) {
	       p2 = p1 + v8[v];
	       if (imd.Hold(p2) && imi[p2] < CANDIDATE) {
		  candidate.Push(p2, -(float)imd[p2]);
		  imi[p2] += CANDIDATE; // Positionne a entasse.
	       }
	    }
	 }
      }

      // Les remettre a "non visites".
      while (!candidate.Empty()) {
	 p2 = candidate.Pop();
  	 imi[p2] -= CANDIDATE;
      }

      // Mettre tous les pixels de la pile à la valeur height.
      while (!marked.Empty()) {
	 p2 = marked.Pop();
	 imd[p2] = (Img2dsf::ValueType)height;
  	 imi[p2] = MARKED;
      }
   }

   return SUCCESS;
}

Errc PAreaOpening( const Img3duc &ims, Img3duc &imd, int connexity, int area ) {
   Point3d p, p1, p2;
   float height;
   Lifo<Point3d> marked;
   int v, nbpix;

   Heap<Point3d, float, int> candidate(ims.VectorSize());
   Img3duc imi(ims.Size());

   imd = ims;
   imi = UNVISITED;

   for (p.z = 0; p.z < imd.Depth(); p.z++)
   for (p.y = 0; p.y < imd.Height(); p.y++)
   for (p.x = 0; p.x < imd.Width(); p.x++)
   {
      // Uniquement les points non encore traites.
      if (imi[p] == MARKED) {
	 continue;
      }

      // 1- * echerche d'un maximum local
      // ims[p] >=voisins et au moins 1 ims[voisin] < ims[p].
      bool maximum = true;
      bool maximumstrict = false;

      if (connexity == 6) {
	 for (v = 0; v < 6; v++) {
	    p1 = p + v6[v];
	    if (imd.Hold(p1)) {
	       if (imd[p] < imd[p1]) {
		  maximum = false;
		  break;
	       }
	       if (imd[p] > imd[p1])  {
		  maximumstrict = true;
	       }
	    }
	 }
      } else {
	 for (v = 0; v < 26; v++) {
	    p1 = p + v26[v];
	    if (imd.Hold(p1)) {
	       if (imd[p] < imd[p1]) {
		  maximum = false;
		  break;
	       }
	       if (imd[p] > imd[p1]) {
		  maximumstrict = true;
	       }
	    }
	 }
      }

      if (!maximum || !maximumstrict) {
	 continue;
      }

      // C'est un maximum : Empiler ce point qui devient la reference.
      // Attention: dans un tas Pandore c'est le minimum qui est en tete.
      height = (float)imd[p];
      candidate.Push(p, -height);
      nbpix = 0;
      imi[p] = CANDIDATE;

      // Recherche de la taille de la surface de la region
      // dont p est l'extrema local.
      while (nbpix < area && !candidate.Empty()) {
	 p1 = candidate.Pop();
	 
	 // La region remonte en intensite-> on arrete.
	 if ((float)imd[p1] > height) {
	    imi[p1] -= CANDIDATE;
	    break;
	 }

	 height = (float)imd[p1];
	 marked.Push(p1);
	 nbpix++;

	 // Empiler tous ses voisins non encore traites.
	 // A l'envers pour empiler dans le sens clockwise.
	 // Plus efficace dans le parcours? a tester.
	 if (connexity == 6) {
	    for (v = 6 - 1; v >= 0; v--) {
	       p2 = p1 + v6[v];
	       if (imd.Hold(p2) && imi[p2] < CANDIDATE) {
		  candidate.Push(p2, -(float)imd[p2]);
		  imi[p2] += CANDIDATE; // Positionne a entasse.
	       }
	    }
	 } else {
	    for (v = 26 - 1; v >= 0; v--) {
	       p2 = p1 + v26[v];
	       if (imd.Hold(p2) && imi[p2] < CANDIDATE) {
		  candidate.Push(p2, -(float)imd[p2]);
		  imi[p2] += CANDIDATE; // Positionne a entasse.
	       }
	    }
	 }
      }

      // Les remettre a "non visites".
      while (!candidate.Empty()) {
	 p2 = candidate.Pop();
  	 imi[p2] -= CANDIDATE;
      }

      // Mettre tous les pixels de la pile à la valeur height.
      while (!marked.Empty()) {
	 p2 = marked.Pop();
	 imd[p2] = (Img3duc::ValueType)height;
  	 imi[p2] = MARKED;
      }
   }

   return SUCCESS;
}

Errc PAreaOpening( const Img3dsl &ims, Img3dsl &imd, int connexity, int area ) {
   Point3d p, p1, p2;
   float height;
   Lifo<Point3d> marked;
   int v, nbpix;

   Heap<Point3d, float, int> candidate(ims.VectorSize());
   Img3duc imi(ims.Size());

   imd = ims;
   imi = UNVISITED;

   for (p.z = 0; p.z < imd.Depth(); p.z++)
   for (p.y = 0; p.y < imd.Height(); p.y++)
   for (p.x = 0; p.x < imd.Width(); p.x++)
   {
      // Uniquement les points non encore traites.
      if (imi[p] == MARKED) {
	 continue;
      }

      // 1- * echerche d'un maximum local
      // ims[p] >=voisins et au moins 1 ims[voisin] < ims[p].
      bool maximum = true;
      bool maximumstrict = false;

      if (connexity == 6) {
	 for (v = 0; v < 6; v++) {
	    p1 = p + v6[v];
	    if (imd.Hold(p1)) {
	       if (imd[p] < imd[p1]) {
		  maximum = false;
		  break;
	       }
	       if (imd[p] > imd[p1])  {
		  maximumstrict = true;
	       }
	    }
	 }
      } else {
	 for (v = 0; v < 26; v++) {
	    p1 = p + v26[v];
	    if (imd.Hold(p1)) {
	       if (imd[p] < imd[p1]) {
		  maximum = false;
		  break;
	       }
	       if (imd[p] > imd[p1]) {
		  maximumstrict = true;
	       }
	    }
	 }
      }

      if (!maximum || !maximumstrict) {
	 continue;
      }

      // C'est un maximum : Empiler ce point qui devient la reference.
      // Attention: dans un tas Pandore c'est le minimum qui est en tete.
      height = (float)imd[p];
      candidate.Push(p, -height);
      nbpix = 0;
      imi[p] = CANDIDATE;

      // Recherche de la taille de la surface de la region
      // dont p est l'extrema local.
      while (nbpix < area && !candidate.Empty()) {
	 p1 = candidate.Pop();
	 
	 // La region remonte en intensite-> on arrete.
	 if ((float)imd[p1] > height) {
	    imi[p1] -= CANDIDATE;
	    break;
	 }

	 height = (float)imd[p1];
	 marked.Push(p1);
	 nbpix++;

	 // Empiler tous ses voisins non encore traites.
	 // A l'envers pour empiler dans le sens clockwise.
	 // Plus efficace dans le parcours? a tester.
	 if (connexity == 6) {
	    for (v = 6 - 1; v >= 0; v--) {
	       p2 = p1 + v6[v];
	       if (imd.Hold(p2) && imi[p2] < CANDIDATE) {
		  candidate.Push(p2, -(float)imd[p2]);
		  imi[p2] += CANDIDATE; // Positionne a entasse.
	       }
	    }
	 } else {
	    for (v = 26 - 1; v >= 0; v--) {
	       p2 = p1 + v26[v];
	       if (imd.Hold(p2) && imi[p2] < CANDIDATE) {
		  candidate.Push(p2, -(float)imd[p2]);
		  imi[p2] += CANDIDATE; // Positionne a entasse.
	       }
	    }
	 }
      }

      // Les remettre a "non visites".
      while (!candidate.Empty()) {
	 p2 = candidate.Pop();
  	 imi[p2] -= CANDIDATE;
      }

      // Mettre tous les pixels de la pile à la valeur height.
      while (!marked.Empty()) {
	 p2 = marked.Pop();
	 imd[p2] = (Img3dsl::ValueType)height;
  	 imi[p2] = MARKED;
      }
   }

   return SUCCESS;
}

Errc PAreaOpening( const Img3dsf &ims, Img3dsf &imd, int connexity, int area ) {
   Point3d p, p1, p2;
   float height;
   Lifo<Point3d> marked;
   int v, nbpix;

   Heap<Point3d, float, int> candidate(ims.VectorSize());
   Img3duc imi(ims.Size());

   imd = ims;
   imi = UNVISITED;

   for (p.z = 0; p.z < imd.Depth(); p.z++)
   for (p.y = 0; p.y < imd.Height(); p.y++)
   for (p.x = 0; p.x < imd.Width(); p.x++)
   {
      // Uniquement les points non encore traites.
      if (imi[p] == MARKED) {
	 continue;
      }

      // 1- * echerche d'un maximum local
      // ims[p] >=voisins et au moins 1 ims[voisin] < ims[p].
      bool maximum = true;
      bool maximumstrict = false;

      if (connexity == 6) {
	 for (v = 0; v < 6; v++) {
	    p1 = p + v6[v];
	    if (imd.Hold(p1)) {
	       if (imd[p] < imd[p1]) {
		  maximum = false;
		  break;
	       }
	       if (imd[p] > imd[p1])  {
		  maximumstrict = true;
	       }
	    }
	 }
      } else {
	 for (v = 0; v < 26; v++) {
	    p1 = p + v26[v];
	    if (imd.Hold(p1)) {
	       if (imd[p] < imd[p1]) {
		  maximum = false;
		  break;
	       }
	       if (imd[p] > imd[p1]) {
		  maximumstrict = true;
	       }
	    }
	 }
      }

      if (!maximum || !maximumstrict) {
	 continue;
      }

      // C'est un maximum : Empiler ce point qui devient la reference.
      // Attention: dans un tas Pandore c'est le minimum qui est en tete.
      height = (float)imd[p];
      candidate.Push(p, -height);
      nbpix = 0;
      imi[p] = CANDIDATE;

      // Recherche de la taille de la surface de la region
      // dont p est l'extrema local.
      while (nbpix < area && !candidate.Empty()) {
	 p1 = candidate.Pop();
	 
	 // La region remonte en intensite-> on arrete.
	 if ((float)imd[p1] > height) {
	    imi[p1] -= CANDIDATE;
	    break;
	 }

	 height = (float)imd[p1];
	 marked.Push(p1);
	 nbpix++;

	 // Empiler tous ses voisins non encore traites.
	 // A l'envers pour empiler dans le sens clockwise.
	 // Plus efficace dans le parcours? a tester.
	 if (connexity == 6) {
	    for (v = 6 - 1; v >= 0; v--) {
	       p2 = p1 + v6[v];
	       if (imd.Hold(p2) && imi[p2] < CANDIDATE) {
		  candidate.Push(p2, -(float)imd[p2]);
		  imi[p2] += CANDIDATE; // Positionne a entasse.
	       }
	    }
	 } else {
	    for (v = 26 - 1; v >= 0; v--) {
	       p2 = p1 + v26[v];
	       if (imd.Hold(p2) && imi[p2] < CANDIDATE) {
		  candidate.Push(p2, -(float)imd[p2]);
		  imi[p2] += CANDIDATE; // Positionne a entasse.
	       }
	    }
	 }
      }

      // Les remettre a "non visites".
      while (!candidate.Empty()) {
	 p2 = candidate.Pop();
  	 imi[p2] -= CANDIDATE;
      }

      // Mettre tous les pixels de la pile à la valeur height.
      while (!marked.Empty()) {
	 p2 = marked.Pop();
	 imd[p2] = (Img3dsf::ValueType)height;
  	 imi[p2] = MARKED;
      }
   }

   return SUCCESS;
}


#undef UNVISITED
#undef CANDIDATE
#undef MARKED

#ifdef MAIN
#define	USAGE	"usage: %s connexity area [-m mask] [im_in|-] [im_out|-]"
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
   if (objs[0]->Type() == Po_Img2duc) {
      Img2duc* const ims = (Img2duc*)objs[0];
      objd[0] = new Img2duc(ims->Props());
      Img2duc* const imd = (Img2duc*)objd[0];
      
      result = PAreaOpening(*ims, *imd, atoi(parv[0]), atoi(parv[1]));
      goto end;
   }
   if (objs[0]->Type() == Po_Img2dsl) {
      Img2dsl* const ims = (Img2dsl*)objs[0];
      objd[0] = new Img2dsl(ims->Props());
      Img2dsl* const imd = (Img2dsl*)objd[0];
      
      result = PAreaOpening(*ims, *imd, atoi(parv[0]), atoi(parv[1]));
      goto end;
   }
   if (objs[0]->Type() == Po_Img2dsf) {
      Img2dsf* const ims = (Img2dsf*)objs[0];
      objd[0] = new Img2dsf(ims->Props());
      Img2dsf* const imd = (Img2dsf*)objd[0];
      
      result = PAreaOpening(*ims, *imd, atoi(parv[0]), atoi(parv[1]));
      goto end;
   }
   if (objs[0]->Type() == Po_Img3duc) {
      Img3duc* const ims = (Img3duc*)objs[0];
      objd[0] = new Img3duc(ims->Props());
      Img3duc* const imd = (Img3duc*)objd[0];
      
      result = PAreaOpening(*ims, *imd, atoi(parv[0]), atoi(parv[1]));
      goto end;
   }
   if (objs[0]->Type() == Po_Img3dsl) {
      Img3dsl* const ims = (Img3dsl*)objs[0];
      objd[0] = new Img3dsl(ims->Props());
      Img3dsl* const imd = (Img3dsl*)objd[0];
      
      result = PAreaOpening(*ims, *imd, atoi(parv[0]), atoi(parv[1]));
      goto end;
   }
   if (objs[0]->Type() == Po_Img3dsf) {
      Img3dsf* const ims = (Img3dsf*)objs[0];
      objd[0] = new Img3dsf(ims->Props());
      Img3dsf* const imd = (Img3dsf*)objd[0];
      
      result = PAreaOpening(*ims, *imd, atoi(parv[0]), atoi(parv[1]));
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
