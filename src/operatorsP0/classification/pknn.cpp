/* -*- c-basic-offset: 3; mode: c++ -*-
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
 * @author Alexandre Duret-Lutz - 1999-11-03
 * @author Régis Clouard - 2004-02-09 (fix on knn_)
 * @author Alexis Mignon - 2009-01-15 (fix on classmax)
 */

#include <math.h>
#include <pandore.h>
using namespace pandore;

/**
 * @file knn.cpp
 * K Nearest Neighbours Classification.
 */
void knn_( const Char*const* base, const Ushort* c,  int taille_base,
	    const Char*const* in,  int taille_in,  int nbr_compos,
	    Ushort* out, int K )  {
   /* liste des K premiers voisins, avec pour chacun
      son numero de classe et sa distance par rapport a
      l'individu courrant */
   Ushort* s_classe = new Ushort[K];
   Double* s_dist   = new Double[K];
   int i,a;
   
   /* numero de classe maximum */
   int classmax = 0;
   for (i = 0; i < taille_base ;++i)
      if (classmax < c[i])
	 classmax = c[i];
   
   /* nombre d'occurence de chaque classe dans les Knn */
   int* classoccur = new int[classmax+1]; // fixed bug +1
   
   /* boucle sur les individus a classer */
   for (i = 0; i < taille_in; ++i) {
      /* au debut, les K premiers voisins sont tres loin ! */
      for (a = 0; a < K; ++a)
	 s_dist[a] = MAXDOUBLE;
      /* boucle sur les individus classes lors de l'apprentissage */
      for (int b = 0; b < taille_base; ++b) {
	 Double dist = 0;
	 /* boucle sur les composantes/caracteristique
	    => calcul de la distance (au carre) */
	 for (int n = 0; n < nbr_compos; ++n) {
	    Double d = (double)in[n][i]-(double)base[n][b];
	    dist += d * d;
	 }
	 /* insert l'individu a sa place dans la liste <
	    des K premiers voisins */
	 if (s_dist[K-1]<=dist)
	    continue;
	 int p;
	 for (p = K-2; p>=0 && s_dist[p]>dist; --p)
	    /* NOP */ ;
	 ++p;
	 if (s_dist[p]>dist) {
	    for (int q = K-1; q > p; --q) {
	       s_dist[q] = s_dist[q-1];
	       s_classe[q] = s_classe[q-1];
	    }
	    s_dist[p] = dist;
	    s_classe[p] = c[b];
	 }
      }
      for (a = 0; a <= classmax; ++a)
	 classoccur[a] = 0;
      /* nombre d'occurence pour les K premiers voisins */
      for (int p = 0; p<K; ++p)
	 ++classoccur[s_classe[p]];
      int cmax = 0;
      for (a = 0; a <= classmax; ++a) {
	 if (classoccur[a] > classoccur[cmax])
	    cmax = a;
      }
      out[i] = cmax;
   }
}
void knn_( const Uchar*const* base, const Ushort* c,  int taille_base,
	    const Uchar*const* in,  int taille_in,  int nbr_compos,
	    Ushort* out, int K )  {
   /* liste des K premiers voisins, avec pour chacun
      son numero de classe et sa distance par rapport a
      l'individu courrant */
   Ushort* s_classe = new Ushort[K];
   Double* s_dist   = new Double[K];
   int i,a;
   
   /* numero de classe maximum */
   int classmax = 0;
   for (i = 0; i < taille_base ;++i)
      if (classmax < c[i])
	 classmax = c[i];
   
   /* nombre d'occurence de chaque classe dans les Knn */
   int* classoccur = new int[classmax+1]; // fixed bug +1
   
   /* boucle sur les individus a classer */
   for (i = 0; i < taille_in; ++i) {
      /* au debut, les K premiers voisins sont tres loin ! */
      for (a = 0; a < K; ++a)
	 s_dist[a] = MAXDOUBLE;
      /* boucle sur les individus classes lors de l'apprentissage */
      for (int b = 0; b < taille_base; ++b) {
	 Double dist = 0;
	 /* boucle sur les composantes/caracteristique
	    => calcul de la distance (au carre) */
	 for (int n = 0; n < nbr_compos; ++n) {
	    Double d = (double)in[n][i]-(double)base[n][b];
	    dist += d * d;
	 }
	 /* insert l'individu a sa place dans la liste <
	    des K premiers voisins */
	 if (s_dist[K-1]<=dist)
	    continue;
	 int p;
	 for (p = K-2; p>=0 && s_dist[p]>dist; --p)
	    /* NOP */ ;
	 ++p;
	 if (s_dist[p]>dist) {
	    for (int q = K-1; q > p; --q) {
	       s_dist[q] = s_dist[q-1];
	       s_classe[q] = s_classe[q-1];
	    }
	    s_dist[p] = dist;
	    s_classe[p] = c[b];
	 }
      }
      for (a = 0; a <= classmax; ++a)
	 classoccur[a] = 0;
      /* nombre d'occurence pour les K premiers voisins */
      for (int p = 0; p<K; ++p)
	 ++classoccur[s_classe[p]];
      int cmax = 0;
      for (a = 0; a <= classmax; ++a) {
	 if (classoccur[a] > classoccur[cmax])
	    cmax = a;
      }
      out[i] = cmax;
   }
}
void knn_( const Short*const* base, const Ushort* c,  int taille_base,
	    const Short*const* in,  int taille_in,  int nbr_compos,
	    Ushort* out, int K )  {
   /* liste des K premiers voisins, avec pour chacun
      son numero de classe et sa distance par rapport a
      l'individu courrant */
   Ushort* s_classe = new Ushort[K];
   Double* s_dist   = new Double[K];
   int i,a;
   
   /* numero de classe maximum */
   int classmax = 0;
   for (i = 0; i < taille_base ;++i)
      if (classmax < c[i])
	 classmax = c[i];
   
   /* nombre d'occurence de chaque classe dans les Knn */
   int* classoccur = new int[classmax+1]; // fixed bug +1
   
   /* boucle sur les individus a classer */
   for (i = 0; i < taille_in; ++i) {
      /* au debut, les K premiers voisins sont tres loin ! */
      for (a = 0; a < K; ++a)
	 s_dist[a] = MAXDOUBLE;
      /* boucle sur les individus classes lors de l'apprentissage */
      for (int b = 0; b < taille_base; ++b) {
	 Double dist = 0;
	 /* boucle sur les composantes/caracteristique
	    => calcul de la distance (au carre) */
	 for (int n = 0; n < nbr_compos; ++n) {
	    Double d = (double)in[n][i]-(double)base[n][b];
	    dist += d * d;
	 }
	 /* insert l'individu a sa place dans la liste <
	    des K premiers voisins */
	 if (s_dist[K-1]<=dist)
	    continue;
	 int p;
	 for (p = K-2; p>=0 && s_dist[p]>dist; --p)
	    /* NOP */ ;
	 ++p;
	 if (s_dist[p]>dist) {
	    for (int q = K-1; q > p; --q) {
	       s_dist[q] = s_dist[q-1];
	       s_classe[q] = s_classe[q-1];
	    }
	    s_dist[p] = dist;
	    s_classe[p] = c[b];
	 }
      }
      for (a = 0; a <= classmax; ++a)
	 classoccur[a] = 0;
      /* nombre d'occurence pour les K premiers voisins */
      for (int p = 0; p<K; ++p)
	 ++classoccur[s_classe[p]];
      int cmax = 0;
      for (a = 0; a <= classmax; ++a) {
	 if (classoccur[a] > classoccur[cmax])
	    cmax = a;
      }
      out[i] = cmax;
   }
}
void knn_( const Ushort*const* base, const Ushort* c,  int taille_base,
	    const Ushort*const* in,  int taille_in,  int nbr_compos,
	    Ushort* out, int K )  {
   /* liste des K premiers voisins, avec pour chacun
      son numero de classe et sa distance par rapport a
      l'individu courrant */
   Ushort* s_classe = new Ushort[K];
   Double* s_dist   = new Double[K];
   int i,a;
   
   /* numero de classe maximum */
   int classmax = 0;
   for (i = 0; i < taille_base ;++i)
      if (classmax < c[i])
	 classmax = c[i];
   
   /* nombre d'occurence de chaque classe dans les Knn */
   int* classoccur = new int[classmax+1]; // fixed bug +1
   
   /* boucle sur les individus a classer */
   for (i = 0; i < taille_in; ++i) {
      /* au debut, les K premiers voisins sont tres loin ! */
      for (a = 0; a < K; ++a)
	 s_dist[a] = MAXDOUBLE;
      /* boucle sur les individus classes lors de l'apprentissage */
      for (int b = 0; b < taille_base; ++b) {
	 Double dist = 0;
	 /* boucle sur les composantes/caracteristique
	    => calcul de la distance (au carre) */
	 for (int n = 0; n < nbr_compos; ++n) {
	    Double d = (double)in[n][i]-(double)base[n][b];
	    dist += d * d;
	 }
	 /* insert l'individu a sa place dans la liste <
	    des K premiers voisins */
	 if (s_dist[K-1]<=dist)
	    continue;
	 int p;
	 for (p = K-2; p>=0 && s_dist[p]>dist; --p)
	    /* NOP */ ;
	 ++p;
	 if (s_dist[p]>dist) {
	    for (int q = K-1; q > p; --q) {
	       s_dist[q] = s_dist[q-1];
	       s_classe[q] = s_classe[q-1];
	    }
	    s_dist[p] = dist;
	    s_classe[p] = c[b];
	 }
      }
      for (a = 0; a <= classmax; ++a)
	 classoccur[a] = 0;
      /* nombre d'occurence pour les K premiers voisins */
      for (int p = 0; p<K; ++p)
	 ++classoccur[s_classe[p]];
      int cmax = 0;
      for (a = 0; a <= classmax; ++a) {
	 if (classoccur[a] > classoccur[cmax])
	    cmax = a;
      }
      out[i] = cmax;
   }
}
void knn_( const Long*const* base, const Ushort* c,  int taille_base,
	    const Long*const* in,  int taille_in,  int nbr_compos,
	    Ushort* out, int K )  {
   /* liste des K premiers voisins, avec pour chacun
      son numero de classe et sa distance par rapport a
      l'individu courrant */
   Ushort* s_classe = new Ushort[K];
   Double* s_dist   = new Double[K];
   int i,a;
   
   /* numero de classe maximum */
   int classmax = 0;
   for (i = 0; i < taille_base ;++i)
      if (classmax < c[i])
	 classmax = c[i];
   
   /* nombre d'occurence de chaque classe dans les Knn */
   int* classoccur = new int[classmax+1]; // fixed bug +1
   
   /* boucle sur les individus a classer */
   for (i = 0; i < taille_in; ++i) {
      /* au debut, les K premiers voisins sont tres loin ! */
      for (a = 0; a < K; ++a)
	 s_dist[a] = MAXDOUBLE;
      /* boucle sur les individus classes lors de l'apprentissage */
      for (int b = 0; b < taille_base; ++b) {
	 Double dist = 0;
	 /* boucle sur les composantes/caracteristique
	    => calcul de la distance (au carre) */
	 for (int n = 0; n < nbr_compos; ++n) {
	    Double d = (double)in[n][i]-(double)base[n][b];
	    dist += d * d;
	 }
	 /* insert l'individu a sa place dans la liste <
	    des K premiers voisins */
	 if (s_dist[K-1]<=dist)
	    continue;
	 int p;
	 for (p = K-2; p>=0 && s_dist[p]>dist; --p)
	    /* NOP */ ;
	 ++p;
	 if (s_dist[p]>dist) {
	    for (int q = K-1; q > p; --q) {
	       s_dist[q] = s_dist[q-1];
	       s_classe[q] = s_classe[q-1];
	    }
	    s_dist[p] = dist;
	    s_classe[p] = c[b];
	 }
      }
      for (a = 0; a <= classmax; ++a)
	 classoccur[a] = 0;
      /* nombre d'occurence pour les K premiers voisins */
      for (int p = 0; p<K; ++p)
	 ++classoccur[s_classe[p]];
      int cmax = 0;
      for (a = 0; a <= classmax; ++a) {
	 if (classoccur[a] > classoccur[cmax])
	    cmax = a;
      }
      out[i] = cmax;
   }
}
void knn_( const Ulong*const* base, const Ushort* c,  int taille_base,
	    const Ulong*const* in,  int taille_in,  int nbr_compos,
	    Ushort* out, int K )  {
   /* liste des K premiers voisins, avec pour chacun
      son numero de classe et sa distance par rapport a
      l'individu courrant */
   Ushort* s_classe = new Ushort[K];
   Double* s_dist   = new Double[K];
   int i,a;
   
   /* numero de classe maximum */
   int classmax = 0;
   for (i = 0; i < taille_base ;++i)
      if (classmax < c[i])
	 classmax = c[i];
   
   /* nombre d'occurence de chaque classe dans les Knn */
   int* classoccur = new int[classmax+1]; // fixed bug +1
   
   /* boucle sur les individus a classer */
   for (i = 0; i < taille_in; ++i) {
      /* au debut, les K premiers voisins sont tres loin ! */
      for (a = 0; a < K; ++a)
	 s_dist[a] = MAXDOUBLE;
      /* boucle sur les individus classes lors de l'apprentissage */
      for (int b = 0; b < taille_base; ++b) {
	 Double dist = 0;
	 /* boucle sur les composantes/caracteristique
	    => calcul de la distance (au carre) */
	 for (int n = 0; n < nbr_compos; ++n) {
	    Double d = (double)in[n][i]-(double)base[n][b];
	    dist += d * d;
	 }
	 /* insert l'individu a sa place dans la liste <
	    des K premiers voisins */
	 if (s_dist[K-1]<=dist)
	    continue;
	 int p;
	 for (p = K-2; p>=0 && s_dist[p]>dist; --p)
	    /* NOP */ ;
	 ++p;
	 if (s_dist[p]>dist) {
	    for (int q = K-1; q > p; --q) {
	       s_dist[q] = s_dist[q-1];
	       s_classe[q] = s_classe[q-1];
	    }
	    s_dist[p] = dist;
	    s_classe[p] = c[b];
	 }
      }
      for (a = 0; a <= classmax; ++a)
	 classoccur[a] = 0;
      /* nombre d'occurence pour les K premiers voisins */
      for (int p = 0; p<K; ++p)
	 ++classoccur[s_classe[p]];
      int cmax = 0;
      for (a = 0; a <= classmax; ++a) {
	 if (classoccur[a] > classoccur[cmax])
	    cmax = a;
      }
      out[i] = cmax;
   }
}
void knn_( const Llong*const* base, const Ushort* c,  int taille_base,
	    const Llong*const* in,  int taille_in,  int nbr_compos,
	    Ushort* out, int K )  {
   /* liste des K premiers voisins, avec pour chacun
      son numero de classe et sa distance par rapport a
      l'individu courrant */
   Ushort* s_classe = new Ushort[K];
   Double* s_dist   = new Double[K];
   int i,a;
   
   /* numero de classe maximum */
   int classmax = 0;
   for (i = 0; i < taille_base ;++i)
      if (classmax < c[i])
	 classmax = c[i];
   
   /* nombre d'occurence de chaque classe dans les Knn */
   int* classoccur = new int[classmax+1]; // fixed bug +1
   
   /* boucle sur les individus a classer */
   for (i = 0; i < taille_in; ++i) {
      /* au debut, les K premiers voisins sont tres loin ! */
      for (a = 0; a < K; ++a)
	 s_dist[a] = MAXDOUBLE;
      /* boucle sur les individus classes lors de l'apprentissage */
      for (int b = 0; b < taille_base; ++b) {
	 Double dist = 0;
	 /* boucle sur les composantes/caracteristique
	    => calcul de la distance (au carre) */
	 for (int n = 0; n < nbr_compos; ++n) {
	    Double d = (double)in[n][i]-(double)base[n][b];
	    dist += d * d;
	 }
	 /* insert l'individu a sa place dans la liste <
	    des K premiers voisins */
	 if (s_dist[K-1]<=dist)
	    continue;
	 int p;
	 for (p = K-2; p>=0 && s_dist[p]>dist; --p)
	    /* NOP */ ;
	 ++p;
	 if (s_dist[p]>dist) {
	    for (int q = K-1; q > p; --q) {
	       s_dist[q] = s_dist[q-1];
	       s_classe[q] = s_classe[q-1];
	    }
	    s_dist[p] = dist;
	    s_classe[p] = c[b];
	 }
      }
      for (a = 0; a <= classmax; ++a)
	 classoccur[a] = 0;
      /* nombre d'occurence pour les K premiers voisins */
      for (int p = 0; p<K; ++p)
	 ++classoccur[s_classe[p]];
      int cmax = 0;
      for (a = 0; a <= classmax; ++a) {
	 if (classoccur[a] > classoccur[cmax])
	    cmax = a;
      }
      out[i] = cmax;
   }
}
void knn_( const Ullong*const* base, const Ushort* c,  int taille_base,
	    const Ullong*const* in,  int taille_in,  int nbr_compos,
	    Ushort* out, int K )  {
   /* liste des K premiers voisins, avec pour chacun
      son numero de classe et sa distance par rapport a
      l'individu courrant */
   Ushort* s_classe = new Ushort[K];
   Double* s_dist   = new Double[K];
   int i,a;
   
   /* numero de classe maximum */
   int classmax = 0;
   for (i = 0; i < taille_base ;++i)
      if (classmax < c[i])
	 classmax = c[i];
   
   /* nombre d'occurence de chaque classe dans les Knn */
   int* classoccur = new int[classmax+1]; // fixed bug +1
   
   /* boucle sur les individus a classer */
   for (i = 0; i < taille_in; ++i) {
      /* au debut, les K premiers voisins sont tres loin ! */
      for (a = 0; a < K; ++a)
	 s_dist[a] = MAXDOUBLE;
      /* boucle sur les individus classes lors de l'apprentissage */
      for (int b = 0; b < taille_base; ++b) {
	 Double dist = 0;
	 /* boucle sur les composantes/caracteristique
	    => calcul de la distance (au carre) */
	 for (int n = 0; n < nbr_compos; ++n) {
	    Double d = (double)in[n][i]-(double)base[n][b];
	    dist += d * d;
	 }
	 /* insert l'individu a sa place dans la liste <
	    des K premiers voisins */
	 if (s_dist[K-1]<=dist)
	    continue;
	 int p;
	 for (p = K-2; p>=0 && s_dist[p]>dist; --p)
	    /* NOP */ ;
	 ++p;
	 if (s_dist[p]>dist) {
	    for (int q = K-1; q > p; --q) {
	       s_dist[q] = s_dist[q-1];
	       s_classe[q] = s_classe[q-1];
	    }
	    s_dist[p] = dist;
	    s_classe[p] = c[b];
	 }
      }
      for (a = 0; a <= classmax; ++a)
	 classoccur[a] = 0;
      /* nombre d'occurence pour les K premiers voisins */
      for (int p = 0; p<K; ++p)
	 ++classoccur[s_classe[p]];
      int cmax = 0;
      for (a = 0; a <= classmax; ++a) {
	 if (classoccur[a] > classoccur[cmax])
	    cmax = a;
      }
      out[i] = cmax;
   }
}
void knn_( const Float*const* base, const Ushort* c,  int taille_base,
	    const Float*const* in,  int taille_in,  int nbr_compos,
	    Ushort* out, int K )  {
   /* liste des K premiers voisins, avec pour chacun
      son numero de classe et sa distance par rapport a
      l'individu courrant */
   Ushort* s_classe = new Ushort[K];
   Double* s_dist   = new Double[K];
   int i,a;
   
   /* numero de classe maximum */
   int classmax = 0;
   for (i = 0; i < taille_base ;++i)
      if (classmax < c[i])
	 classmax = c[i];
   
   /* nombre d'occurence de chaque classe dans les Knn */
   int* classoccur = new int[classmax+1]; // fixed bug +1
   
   /* boucle sur les individus a classer */
   for (i = 0; i < taille_in; ++i) {
      /* au debut, les K premiers voisins sont tres loin ! */
      for (a = 0; a < K; ++a)
	 s_dist[a] = MAXDOUBLE;
      /* boucle sur les individus classes lors de l'apprentissage */
      for (int b = 0; b < taille_base; ++b) {
	 Double dist = 0;
	 /* boucle sur les composantes/caracteristique
	    => calcul de la distance (au carre) */
	 for (int n = 0; n < nbr_compos; ++n) {
	    Double d = (double)in[n][i]-(double)base[n][b];
	    dist += d * d;
	 }
	 /* insert l'individu a sa place dans la liste <
	    des K premiers voisins */
	 if (s_dist[K-1]<=dist)
	    continue;
	 int p;
	 for (p = K-2; p>=0 && s_dist[p]>dist; --p)
	    /* NOP */ ;
	 ++p;
	 if (s_dist[p]>dist) {
	    for (int q = K-1; q > p; --q) {
	       s_dist[q] = s_dist[q-1];
	       s_classe[q] = s_classe[q-1];
	    }
	    s_dist[p] = dist;
	    s_classe[p] = c[b];
	 }
      }
      for (a = 0; a <= classmax; ++a)
	 classoccur[a] = 0;
      /* nombre d'occurence pour les K premiers voisins */
      for (int p = 0; p<K; ++p)
	 ++classoccur[s_classe[p]];
      int cmax = 0;
      for (a = 0; a <= classmax; ++a) {
	 if (classoccur[a] > classoccur[cmax])
	    cmax = a;
      }
      out[i] = cmax;
   }
}
void knn_( const Double*const* base, const Ushort* c,  int taille_base,
	    const Double*const* in,  int taille_in,  int nbr_compos,
	    Ushort* out, int K )  {
   /* liste des K premiers voisins, avec pour chacun
      son numero de classe et sa distance par rapport a
      l'individu courrant */
   Ushort* s_classe = new Ushort[K];
   Double* s_dist   = new Double[K];
   int i,a;
   
   /* numero de classe maximum */
   int classmax = 0;
   for (i = 0; i < taille_base ;++i)
      if (classmax < c[i])
	 classmax = c[i];
   
   /* nombre d'occurence de chaque classe dans les Knn */
   int* classoccur = new int[classmax+1]; // fixed bug +1
   
   /* boucle sur les individus a classer */
   for (i = 0; i < taille_in; ++i) {
      /* au debut, les K premiers voisins sont tres loin ! */
      for (a = 0; a < K; ++a)
	 s_dist[a] = MAXDOUBLE;
      /* boucle sur les individus classes lors de l'apprentissage */
      for (int b = 0; b < taille_base; ++b) {
	 Double dist = 0;
	 /* boucle sur les composantes/caracteristique
	    => calcul de la distance (au carre) */
	 for (int n = 0; n < nbr_compos; ++n) {
	    Double d = (double)in[n][i]-(double)base[n][b];
	    dist += d * d;
	 }
	 /* insert l'individu a sa place dans la liste <
	    des K premiers voisins */
	 if (s_dist[K-1]<=dist)
	    continue;
	 int p;
	 for (p = K-2; p>=0 && s_dist[p]>dist; --p)
	    /* NOP */ ;
	 ++p;
	 if (s_dist[p]>dist) {
	    for (int q = K-1; q > p; --q) {
	       s_dist[q] = s_dist[q-1];
	       s_classe[q] = s_classe[q-1];
	    }
	    s_dist[p] = dist;
	    s_classe[p] = c[b];
	 }
      }
      for (a = 0; a <= classmax; ++a)
	 classoccur[a] = 0;
      /* nombre d'occurence pour les K premiers voisins */
      for (int p = 0; p<K; ++p)
	 ++classoccur[s_classe[p]];
      int cmax = 0;
      for (a = 0; a <= classmax; ++a) {
	 if (classoccur[a] > classoccur[cmax])
	    cmax = a;
      }
      out[i] = cmax;
   }
}

Errc PKnn( const std::string& a_base,  const Collection& c_base, 
	   const std::string& a_in,    const Collection& c_in,
	   const std::string& a_out,         Collection& c_out,
	   int K ) {
   Long nbrcomp ;
   std::string type;
   Long nbase;
   if (!c_base.NbOf(a_base,type,nbrcomp,nbase)) {
      std::cerr<<"Error pknn: No data in input collection."<<std::endl;
      return FAILURE;
   }
   if (type == "Array:Char") {
      Ushort* c;
      if (K < 1)
	 K = 1;
      // Si K = 1, on accepte que la liste des classes
      // n'existe pas :  le numero du plus proche voisin
      // servira de numero classe.
      if (K == 1 && !c_base.Exists(a_base + ".C")) {
	 c = new Ushort[nbase];
	 for (int i = 0; i < nbase; ++i)
	    c[i] = i+1;
      } else 
	 c = c_base.GETARRAY(a_base + ".C",Ushort);

      Long nin;
      Char** base = c_base.GETNARRAYS(a_base,Char,nbrcomp,nbase);
      Char** in = c_in.GETNARRAYS(a_in,Char,nbrcomp,nin);

      Ushort* out = new Ushort[nin];
		
      knn_(base,c,nbase,in,nin,nbrcomp,out,K);
      c_out.SETARRAY(a_out,Ushort,out,nin);
   } else
   if (type == "Array:Uchar") {
      Ushort* c;
      if (K < 1)
	 K = 1;
      // Si K = 1, on accepte que la liste des classes
      // n'existe pas :  le numero du plus proche voisin
      // servira de numero classe.
      if (K == 1 && !c_base.Exists(a_base + ".C")) {
	 c = new Ushort[nbase];
	 for (int i = 0; i < nbase; ++i)
	    c[i] = i+1;
      } else 
	 c = c_base.GETARRAY(a_base + ".C",Ushort);

      Long nin;
      Uchar** base = c_base.GETNARRAYS(a_base,Uchar,nbrcomp,nbase);
      Uchar** in = c_in.GETNARRAYS(a_in,Uchar,nbrcomp,nin);

      Ushort* out = new Ushort[nin];
		
      knn_(base,c,nbase,in,nin,nbrcomp,out,K);
      c_out.SETARRAY(a_out,Ushort,out,nin);
   } else
   if (type == "Array:Short") {
      Ushort* c;
      if (K < 1)
	 K = 1;
      // Si K = 1, on accepte que la liste des classes
      // n'existe pas :  le numero du plus proche voisin
      // servira de numero classe.
      if (K == 1 && !c_base.Exists(a_base + ".C")) {
	 c = new Ushort[nbase];
	 for (int i = 0; i < nbase; ++i)
	    c[i] = i+1;
      } else 
	 c = c_base.GETARRAY(a_base + ".C",Ushort);

      Long nin;
      Short** base = c_base.GETNARRAYS(a_base,Short,nbrcomp,nbase);
      Short** in = c_in.GETNARRAYS(a_in,Short,nbrcomp,nin);

      Ushort* out = new Ushort[nin];
		
      knn_(base,c,nbase,in,nin,nbrcomp,out,K);
      c_out.SETARRAY(a_out,Ushort,out,nin);
   } else
   if (type == "Array:Ushort") {
      Ushort* c;
      if (K < 1)
	 K = 1;
      // Si K = 1, on accepte que la liste des classes
      // n'existe pas :  le numero du plus proche voisin
      // servira de numero classe.
      if (K == 1 && !c_base.Exists(a_base + ".C")) {
	 c = new Ushort[nbase];
	 for (int i = 0; i < nbase; ++i)
	    c[i] = i+1;
      } else 
	 c = c_base.GETARRAY(a_base + ".C",Ushort);

      Long nin;
      Ushort** base = c_base.GETNARRAYS(a_base,Ushort,nbrcomp,nbase);
      Ushort** in = c_in.GETNARRAYS(a_in,Ushort,nbrcomp,nin);

      Ushort* out = new Ushort[nin];
		
      knn_(base,c,nbase,in,nin,nbrcomp,out,K);
      c_out.SETARRAY(a_out,Ushort,out,nin);
   } else
   if (type == "Array:Long") {
      Ushort* c;
      if (K < 1)
	 K = 1;
      // Si K = 1, on accepte que la liste des classes
      // n'existe pas :  le numero du plus proche voisin
      // servira de numero classe.
      if (K == 1 && !c_base.Exists(a_base + ".C")) {
	 c = new Ushort[nbase];
	 for (int i = 0; i < nbase; ++i)
	    c[i] = i+1;
      } else 
	 c = c_base.GETARRAY(a_base + ".C",Ushort);

      Long nin;
      Long** base = c_base.GETNARRAYS(a_base,Long,nbrcomp,nbase);
      Long** in = c_in.GETNARRAYS(a_in,Long,nbrcomp,nin);

      Ushort* out = new Ushort[nin];
		
      knn_(base,c,nbase,in,nin,nbrcomp,out,K);
      c_out.SETARRAY(a_out,Ushort,out,nin);
   } else
   if (type == "Array:Ulong") {
      Ushort* c;
      if (K < 1)
	 K = 1;
      // Si K = 1, on accepte que la liste des classes
      // n'existe pas :  le numero du plus proche voisin
      // servira de numero classe.
      if (K == 1 && !c_base.Exists(a_base + ".C")) {
	 c = new Ushort[nbase];
	 for (int i = 0; i < nbase; ++i)
	    c[i] = i+1;
      } else 
	 c = c_base.GETARRAY(a_base + ".C",Ushort);

      Long nin;
      Ulong** base = c_base.GETNARRAYS(a_base,Ulong,nbrcomp,nbase);
      Ulong** in = c_in.GETNARRAYS(a_in,Ulong,nbrcomp,nin);

      Ushort* out = new Ushort[nin];
		
      knn_(base,c,nbase,in,nin,nbrcomp,out,K);
      c_out.SETARRAY(a_out,Ushort,out,nin);
   } else
   if (type == "Array:Llong") {
      Ushort* c;
      if (K < 1)
	 K = 1;
      // Si K = 1, on accepte que la liste des classes
      // n'existe pas :  le numero du plus proche voisin
      // servira de numero classe.
      if (K == 1 && !c_base.Exists(a_base + ".C")) {
	 c = new Ushort[nbase];
	 for (int i = 0; i < nbase; ++i)
	    c[i] = i+1;
      } else 
	 c = c_base.GETARRAY(a_base + ".C",Ushort);

      Long nin;
      Llong** base = c_base.GETNARRAYS(a_base,Llong,nbrcomp,nbase);
      Llong** in = c_in.GETNARRAYS(a_in,Llong,nbrcomp,nin);

      Ushort* out = new Ushort[nin];
		
      knn_(base,c,nbase,in,nin,nbrcomp,out,K);
      c_out.SETARRAY(a_out,Ushort,out,nin);
   } else
   if (type == "Array:Ullong") {
      Ushort* c;
      if (K < 1)
	 K = 1;
      // Si K = 1, on accepte que la liste des classes
      // n'existe pas :  le numero du plus proche voisin
      // servira de numero classe.
      if (K == 1 && !c_base.Exists(a_base + ".C")) {
	 c = new Ushort[nbase];
	 for (int i = 0; i < nbase; ++i)
	    c[i] = i+1;
      } else 
	 c = c_base.GETARRAY(a_base + ".C",Ushort);

      Long nin;
      Ullong** base = c_base.GETNARRAYS(a_base,Ullong,nbrcomp,nbase);
      Ullong** in = c_in.GETNARRAYS(a_in,Ullong,nbrcomp,nin);

      Ushort* out = new Ushort[nin];
		
      knn_(base,c,nbase,in,nin,nbrcomp,out,K);
      c_out.SETARRAY(a_out,Ushort,out,nin);
   } else
   if (type == "Array:Float") {
      Ushort* c;
      if (K < 1)
	 K = 1;
      // Si K = 1, on accepte que la liste des classes
      // n'existe pas :  le numero du plus proche voisin
      // servira de numero classe.
      if (K == 1 && !c_base.Exists(a_base + ".C")) {
	 c = new Ushort[nbase];
	 for (int i = 0; i < nbase; ++i)
	    c[i] = i+1;
      } else 
	 c = c_base.GETARRAY(a_base + ".C",Ushort);

      Long nin;
      Float** base = c_base.GETNARRAYS(a_base,Float,nbrcomp,nbase);
      Float** in = c_in.GETNARRAYS(a_in,Float,nbrcomp,nin);

      Ushort* out = new Ushort[nin];
		
      knn_(base,c,nbase,in,nin,nbrcomp,out,K);
      c_out.SETARRAY(a_out,Ushort,out,nin);
   } else
   if (type == "Array:Double") {
      Ushort* c;
      if (K < 1)
	 K = 1;
      // Si K = 1, on accepte que la liste des classes
      // n'existe pas :  le numero du plus proche voisin
      // servira de numero classe.
      if (K == 1 && !c_base.Exists(a_base + ".C")) {
	 c = new Ushort[nbase];
	 for (int i = 0; i < nbase; ++i)
	    c[i] = i+1;
      } else 
	 c = c_base.GETARRAY(a_base + ".C",Ushort);

      Long nin;
      Double** base = c_base.GETNARRAYS(a_base,Double,nbrcomp,nbase);
      Double** in = c_in.GETNARRAYS(a_in,Double,nbrcomp,nin);

      Ushort* out = new Ushort[nin];
		
      knn_(base,c,nbase,in,nin,nbrcomp,out,K);
      c_out.SETARRAY(a_out,Ushort,out,nin);
   } else
      return FAILURE;
   return SUCCESS;
}

#ifdef MAIN

/*
 * Modify only the following constants, and the function call.
 */
#define USAGE	"usage: %s base_attr in_attr out_attr k [col_base|-] [col_in|-] [col_out|-]"
#define PARC	4
#define FINC	2
#define FOUTC	1
#define MASK	0

int main( int argc, char *argv[] ) {
   Errc result;              // The result code of the execution.
   Pobject* mask;            // The region mask.
   Pobject* objin[FINC+1];   // The input objects;
   Pobject* objs[FINC+1];    // The source objects masked by the mask.
   Pobject* objout[FOUTC+1]; // The output object.
   Pobject* objd[FOUTC+1];   // The result object of the execution.
   char* parv[PARC+1];       // The input parameters.

   ReadArgs(argc,argv,PARC,FINC,FOUTC,&mask,objin,objs,objout,objd,parv,USAGE,MASK);

   if (objs[0]->Type() == Po_Collection && objs[1]->Type() == Po_Collection) {
      Collection* ims1=(Collection*)objs[0];
      Collection* ims2=(Collection*)objs[1];
      Collection* imd1=new Collection;
      objd[0] = imd1;
      result = PKnn(argv[1],*ims1,argv[2],*ims2,argv[3],*imd1,atoi(argv[4]));
   } else {
      fprintf(stderr,"Error pknn: Pandore type not supported by this operator\n");
      result = FAILURE;
   }
   
   if (result)
      WriteArgs(argc,argv,PARC,FINC,FOUTC,&mask,objin,objs,objout,objd,MASK);
   Exit(result);
   return 0;
}

#endif
