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
 * @author Luc Brun
 */

#include <pandore.h>
#include <iostream>
#include <math.h>
#include <stdlib.h>
#include <float.h>
#include <limits.h>

using namespace std;
using namespace pandore;

#define RETENIR 0
#define RETOURNER 1
#define UN_TIER 1.0 / 3.0

#define REDUCTION 1

typedef struct {
      int I1;
      int I2;
      int I3;
      long freq;
} f_coul;

typedef struct {
      f_coul* tab_freq;
      int* tab;
      int taille;
} coul_freq;


/* definition du type Composante pixel */
typedef unsigned char Comp_rgb; 

/* definition des types matrices */
#define DIM 3
typedef double vecteur[DIM];
typedef double matrice[DIM][DIM];
typedef double mat_covariance[DIM][DIM];
typedef double mat_passage[DIM][DIM];

typedef int  couleur[3];

#define FLOAT_TO_INT(a) (int)(a < 0 ? a - 0.5 : a + 0.5)

/* definition du type arbre */
typedef struct arbre2 {
      struct arbre2 *fils[2];
      int moy_axe[3];
      double ecart;
      mat_covariance cov;
      vecteur axe_princ;
      double val_cut;
      double moment0;
      vecteur moment1;
      int numero;
      bool feuille;
      int * cluster;
      int taille;
} arbre_binaire;

typedef double moments_fils[2][3];
/******************************************************
 * fonctions permettant de choisir
 * comment couper sur l'axe 
 * l'axe a couper
 * le cluster a couper
 *********************************************************/
typedef void (*PFaxecoupe)( mat_covariance, vecteur );
typedef int (*PFdecoupe)( arbre_binaire*, coul_freq*, double*, moments_fils, mat_covariance, mat_covariance, vecteur, double*, PFaxecoupe );
typedef void (*PFinit)( arbre_binaire*, coul_freq* );
typedef double (*PFcluster)( arbre_binaire* );


#ifdef _WIN32
inline double cbrt( double x) {
   if (fabs(x) < DBL_EPSILON ) {
      return 0.0;
   }
   if (x > 0.0) {
      return pow(x, 1.0 / 3.0);
   }
   return -pow(-x, 1.0 / 3.0);	
}
#endif

void trier( coul_freq* , int*, int, vecteur );
double scalaire_vect( vecteur , vecteur );

/********************************************************************************************************/
/* Calcul de l'endroit ou il convient de couper sur l'axe choisi, */
/* ainsi que des moments associes aux deux clusters induits par le */
/* decoupage*/
/********************************************************************************************************/
int decoupe( arbre_binaire* arbre, coul_freq* tab_freq, double*
	    moment0, moments_fils moment1, mat_covariance
	    cov1, mat_covariance cov2, vecteur axe_princ, double*
	    proj_cut, PFaxecoupe donne_dir ) {
   double S0 = arbre->moment0;
   int i, j, k; /* variables de boucle */
   int indice_decoupe = 0;
   double mom0 = 0, m0;
   vecteur x , mom1, m1;
   double max = 0.0;
   double EQM;
   double proj = 0;
   int demi_intervalle = (REDUCTION >> 1);
   int taille = arbre->taille;
   int *cluster = arbre->cluster; 
   f_coul* ens_couleur = tab_freq->tab_freq;
   f_coul* cel;
   mat_covariance cov;
   double X_moins_ya;
   vecteur a, V;

   /* initialisations */
/************************************/
/* a : moyenne du cluster */
/* delta2 norme au carre du verteur tel que delta_i = longueur de */
/* l'intervalle i */
/************************************/
   m0 = 0.0;
   for (i = 0; i < DIM; i++) {		
      a[i] = arbre->moment1[i] / S0;
      m1[i] = 0.0;
      for (j = 0; j < DIM; j++) {
	 cov[i][j] = (arbre->cov[i][j] - (arbre->moment1[i]) * (arbre->moment1[j] / S0)) / S0;
      }
			    
   }
   /* On commence par calculer la direction de coupe*/
   /* le resultat est mis ds V */
   donne_dir(cov, V);

   for (i = 0; i < DIM; i++) {
      for (j = 0; j < DIM; j++) {
	 cov[i][j] = 0.0;
      }
   }
   /* On trie le cluster en fonction du vecteur propre */
   trier(tab_freq, cluster, taille, V);

   /* Calculs */
   for (i = 0; i < taille; i++) {
      cel = ens_couleur + (*cluster);
      x[0] = cel->I1 * REDUCTION + (cel->I1 < 0 ? -demi_intervalle : demi_intervalle);
      x[1] = cel->I2 * REDUCTION + (cel->I2 < 0 ? -demi_intervalle : demi_intervalle);
      x[2] = cel->I3 * REDUCTION + (cel->I3 < 0 ? -demi_intervalle : demi_intervalle);
		
      m0 += cel->freq;
      for (j = 0; j < DIM; j++) {
	 m1[j] += (cel->freq) * x[j];
	 for (k = 0; k < DIM; k++) {
	    cov[j][k] += (cel->freq) * x[j] * x[k];
	 }
      }

      EQM = 0.0;
      if ((m0 != 0.0) && (m0 != S0)) {
	 for (j = 0; j < DIM; j++) {
	    X_moins_ya = m1[j] / m0 - a[j];
	    EQM += X_moins_ya * X_moins_ya;
	 }
	 EQM = (EQM / (S0 - m0)) * m0;
      }

      if (max <  EQM) {
	 max = EQM;
	 proj = scalaire_vect(V, x);/* on retient la valeur de la projection au point de decoupe */
	 indice_decoupe = i + 1;
	 mom0 = m0;
	 for (j = 0; j < DIM; j++) {
	    mom1[j] = m1[j];
	    for (k = 0; k < DIM; k++) {
	       cov1[j][k] = cov[j][k];
	    }
	 }
      }
      cluster++;
   }
   /* transposition des resultats dans les variables de sorties */
   moment0[0] = mom0;
   moment0[1] = S0 - mom0;
   *proj_cut = proj;
   for (i = 0; i < DIM; i++) {
      axe_princ[i]  = V[i];      
      moment1[0][i] = mom1[i];
      moment1[1][i] = arbre->moment1[i] - mom1[i];
      for (j = 0; j < DIM; j++) {
	 cov2[i][j] = arbre->cov[i][j]-cov1[i][j];
      }
   }
   return indice_decoupe;
}

int decoupe_projette( arbre_binaire* arbre, coul_freq* tab_freq, double*
		     moment0, moments_fils moment1, mat_covariance
		     cov1, mat_covariance cov2, vecteur axe_princ, double*
		     proj_cut, PFaxecoupe donne_dir ) {
   double S0 = arbre->moment0;
   int i, j, k; /* variables de boucle */
   int indice_decoupe = 0;
   double  a, En;
   double val;
   double max = 0.0;
   double proj = 0;
   double mom0 = 0, m0;
   vecteur mom1, m1;
   vecteur x;
   vecteur V;
   double EQM = 0;
   int demi_intervalle = (REDUCTION >> 1);
   // bool fin;
   double X_moins_ya;
   // double b2_par_delta2;
   int taille = arbre->taille;
   int *cluster = arbre->cluster; /* ens des couleurs du cluster*/
   f_coul* ens_couleur = tab_freq->tab_freq;
   f_coul* cel;
   mat_covariance cov;
   /* initialisations */
/************************************/
/* sn : cardinal de Ct */
/* En[], En2[] moments d'ordre 1 et 2*/
/* a : moyenne du cluster */
/* delta2 norme au carre du vecteur tel que delta_i = longueur de */
/* l'intervalle i */
/************************************/

   /* initialisations */
/************************************/
/* a : moyenne du cluster */
/* delta2 norme au carre du vecteur tel que delta_i = longueur de */
/* l'intervalle i */
/************************************/
   m0 = 0.0;
   En = 0.0;
   for (i = 0; i < DIM; i++) {		
      m1[i] = 0.0;
      for (j = 0;j < DIM; j++) {
	 cov[i][j] = (arbre->cov[i][j] - (arbre->moment1[i]) * (arbre->moment1[j] / S0)) / S0;
      }
   }
   /* On commence par calculer la direction de coupe */
   donne_dir(cov, V);

   a = 0.0;
   for (i = 0; i < DIM; i++) {
      a += arbre->moment1[i] * V[i];
      for (j = 0; j < DIM; j++) {
	 cov[i][j] = 0.0;
      }
   }
   a /= S0;
   /* On trie le cluster en fonction du vecteur propre */
   trier(tab_freq, cluster, taille, V);
   /* Calculs */
   for (i = 0; i < taille;i++) {
      cel = ens_couleur + (*cluster);
      x[0] = cel->I1 * REDUCTION + (cel->I1 < 0 ? -demi_intervalle : demi_intervalle);
      x[1] = cel->I2 * REDUCTION + (cel->I2 < 0 ? -demi_intervalle : demi_intervalle);
      x[2] = cel->I3 * REDUCTION + (cel->I3 < 0 ? -demi_intervalle : demi_intervalle);

      val = scalaire_vect(x, V);
      m0 += cel->freq;
      for (j = 0; j < DIM; j++) {
	 m1[j] += (cel->freq)*x[j];
	 for (k = 0; k < DIM; k++) {
	    cov[j][k] += (cel->freq) * x[j] * x[k];
	 }
      }

      En += (cel->freq)*val;
      if ((m0 != 0.0)&&(m0 != S0)) {
	 X_moins_ya = En / m0 - a;
	 EQM = X_moins_ya * X_moins_ya;
	 EQM = (EQM / (S0 - m0)) * m0;
      }

      if (max <  EQM) {
	 max = EQM;
	 proj = val;/* on retient la valeur de la projection au point de decoupe */
	 indice_decoupe = i + 1;
	 mom0 = m0;
	 // det2 = b2_par_delta2*max;
	 for (j = 0; j < DIM; j++) {
	    mom1[j] = m1[j];
	    for (k = 0; k < DIM; k++) {
	       cov1[j][k] = cov[j][k];
	    }
	 }
      }
//    fin = ((m0+m0) > S0) && (m0*(S0-m0) <= det2);
      cluster++;
   }
   /* transposition des resultats dans les variables de sorties */
   moment0[0] = mom0;
   moment0[1] = S0 - mom0;
   *proj_cut = proj;
   for (i = 0; i < DIM; i++) {
      axe_princ[i]  = V[i];
      moment1[0][i] = mom1[i];
      moment1[1][i] = arbre->moment1[i] - mom1[i];
      for (j = 0; j < DIM; j++) {
	 cov2[i][j] = arbre->cov[i][j] - cov1[i][j];
      }
   }
   return indice_decoupe;
}

int decoupe_median( arbre_binaire* arbre, coul_freq* tab_freq, double*
		   moment0, moments_fils moment1, mat_covariance
		   cov1, mat_covariance cov2, vecteur axe_princ, double*
		   proj_cut, PFaxecoupe donne_dir ) {

   double S0 = arbre->moment0;
   int i, j, k; /* variables de boucle */
   int indice_decoupe;
   double m0;
   vecteur x;
   vecteur m1, V;
   int demi_intervalle = (REDUCTION >> 1);
   bool fin = false;
   int taille = arbre->taille;
   int *cluster = arbre->cluster; 
   f_coul* ens_couleur = tab_freq->tab_freq;
   f_coul* cel;
   mat_covariance cov;
   /* initialisations */
   /************************************/
   /* m0 : cardinal de Ct */
   /* m1[], cov[][] moments d'ordre 1 et 2*/
   /* a : moyenne du cluster */
   /* delta2 norme au carre du verteur tel que delta_i = longueur de */
   /* l'intervalle i */
   /************************************/

    
   /* initialisations */
   /************************************/
   /* a : moyenne du cluster */
   /* delta2 norme au carre du verteur tel que delta_i = longueur de */
   /* l'intervalle i */
   /************************************/
   m0 = 0.0;
   for (i = 0; i < DIM; i++) {		
      m1[i] = 0.0;
      for (j = 0; j < DIM; j++) {
	 cov[i][j] = (arbre->cov[i][j] - (arbre->moment1[i]) * (arbre->moment1[j] / S0)) / S0;
      }
   }
   /* On commence par calculer la direction de coupe */
   donne_dir(cov, V);

   for (i = 0; i < DIM; i++) {
      for (j = 0; j < DIM; j++) {
	 cov[i][j] = 0.0;
      }
   }

   /* On trie le cluster en fonction du vecteur propre */
   trier(tab_freq, cluster, taille, V);
        
   /* Calculs */
   i = 0;
   while (!fin) {
      cel = ens_couleur + (*cluster);
      x[0] = cel->I1*REDUCTION + (cel->I1 < 0 ? -demi_intervalle : demi_intervalle);
      x[1] = cel->I2*REDUCTION + (cel->I2 < 0 ? -demi_intervalle : demi_intervalle);
      x[2] = cel->I3*REDUCTION + (cel->I3 < 0 ? -demi_intervalle : demi_intervalle);
      
      m0 += cel->freq;
      for (j = 0; j < DIM; j++) {
	 m1[j] += (cel->freq) * x[j];
	 for (k = 0; k < DIM; k++) {
	    cov[j][k] += (cel->freq) * x[j] * x[k];
	 }
      }
      fin = (m0 + m0) > S0;
      cluster++;
      i++;
   }
   indice_decoupe = i;
   /* transposition des resultats dans les variables de sorties */
   moment0[0] = m0;
   moment0[1] = S0 - m0;
   *proj_cut = scalaire_vect(x, V);/* on retient la valeur de la projection au point de decoupe */
   for (i = 0; i < DIM; i++) {
      axe_princ[i]  = V[i];
      moment1[0][i] = m1[i];
      moment1[1][i] = arbre->moment1[i] - m1[i];
      for (j = 0; j < DIM; j++) {
	 cov1[i][j] = cov[i][j];
	 cov2[i][j] = arbre->cov[i][j] - cov[i][j];
      }
   }
  
   return indice_decoupe;
}
/********************************************************************************************************/
/* Calcul des moments d'ordre 0, 1 et 2 d'un cluster. Utilise pour */
/* l'initialisation */
/********************************************************************************************************/
void initialisation( arbre_binaire* arbre, coul_freq* tab_freq ) {
   int i, j, k;
   double m0;
   vecteur m1, x;
   f_coul *cel;
   int demi_intervalle = (REDUCTION>>1);
   int taille = arbre->taille;
   f_coul* ens_couleur = tab_freq->tab_freq;
   int* cluster = tab_freq->tab;
   mat_covariance cov;
  
   m0 = 0.0;
   for (i = 0; i < DIM; i++) {
      m1[i] = 0.0;
      for (j = 0; j < DIM; j++) {
	 cov[i][j] = 0.0;
      }
   }		

   /* Calcul de la somme; de la somme des carres; du cardinal de */
   /* l'ensemble */
   for (i = 0; i < taille; i++) {
      cel = ens_couleur + (*cluster);
      x[0] = cel->I1 * REDUCTION + (cel->I1 < 0 ? -demi_intervalle : demi_intervalle);
      x[1] = cel->I2 * REDUCTION + (cel->I2 < 0 ? -demi_intervalle : demi_intervalle);
      x[2] = cel->I3 * REDUCTION + (cel->I3 < 0 ? -demi_intervalle : demi_intervalle);
      
      m0 += cel->freq;
      for (j = 0; j < DIM; j++) {
	 m1[j] += (cel->freq) * x[j];
	 for (k = 0; k < DIM; k++) {
	    cov[j][k] += (cel->freq) * x[j] * x[k];
	 }
      }
      cluster++;
   }

   /* On transvase dans les variables de sortie */
  
   arbre->moment0 = m0;
   for (i = 0; i < DIM; i++) {
      arbre->moy_axe[i] = (int)(m1[i] / m0);
      arbre->moment1[i] = m1[i];
   }
   for (i = 0; i < DIM; i++) {
      for (j = 0; j < DIM; j++) {
	 arbre->cov[i][j] = cov[i][j];
      }
   }
}

arbre_binaire * creer_fils( arbre_binaire * ) {
   arbre_binaire* fils = (arbre_binaire*)malloc(sizeof(arbre_binaire));
  
   fils->fils[0] = (arbre_binaire*)NULL;
   fils->fils[1] = (arbre_binaire*)NULL;
   fils->ecart = 0.0;
   fils->feuille = true;
  
   return fils;
}

/************************************************************/
/* Calcul de la normale au plan de coupe de chaque cluster */
/**********************************************************/

void vecteur_propre_principal (mat_covariance a, vecteur v_propre, vecteur x0 );
void vecteur_valeur_propres( mat_covariance a, vecteur val_propre, mat_passage P );

void direction_axe_principal( mat_covariance cov, vecteur vect_propre ) {
   static vecteur x0 = {UN_TIER, UN_TIER, UN_TIER};

   vecteur_propre_principal(cov, vect_propre, x0);
}

void direction_moyenne_vect_propre( mat_covariance cov, vecteur V ) {
   int i, j;
   mat_passage P;
   vecteur lambda;
   vecteur alpha;
   double somme_lambda;

   vecteur_valeur_propres(cov, lambda, P);

   somme_lambda = 0.0;
   for (i = 0; i < DIM; i++) {
      somme_lambda += lambda[i];
   }
   for (i = 0; i < DIM; i++) {
      alpha[i] = lambda[i] / somme_lambda;
   }
   for (i = 0; i < DIM; i++) {
      V[i] = 0.0;
      for (j = 0; j < DIM; j++) {
	 V[i] += alpha[j] * P[i][j];
      }
   }
}
  
/******************************************************/
/* Calcul de l'homogneite du cluster */
/* valeur est egale a une de ces fonctions */
/******************************************************/
double eqm_pondere( arbre_binaire* arbre ) {
   int i;
   double erreur = 0.0;
   double m1;
   double S0 = arbre->moment0;
  
   for (i = 0; i < DIM; i++) {
      m1 = arbre->moment1[i];
      erreur += (arbre->cov[i][i] - m1 * (m1 / S0)) / S0;
   }
   return erreur;
}

double eqm_non_pondere( arbre_binaire* arbre ) {
   int i;
   double erreur = 0.0;
   double m1;
   double S0 = arbre->moment0;

   if (S0 == 0) { // TODO
      return 0;
   }
   for (i = 0; i < DIM; i++) {
      m1 = arbre->moment1[i];
      erreur += arbre->cov[i][i] - m1 * (m1 / S0);
   }
   return erreur;
}

double cardinal( arbre_binaire* arbre ) {
   return arbre->moment0;
}

/******************************************************/
void affecte_vect( vecteur v1, vecteur v2 );

void creer_table( arbre_binaire *arbre, coul_freq* tab_freq, PFdecoupe
	    partition, PFcluster valeur, PFaxecoupe donne_dir ) {
   int i, j, k, indice_decoupe;
   arbre_binaire *fils;
   double moment0[2];
   moments_fils moment1;
   double m0, m1;
   mat_covariance cov[2];
   vecteur axe_cut;
   double val_cut;

   if (arbre->feuille) {
      indice_decoupe = partition(arbre, tab_freq, moment0, moment1, cov[0], cov[1], axe_cut, &val_cut, donne_dir);
      
      arbre->val_cut = val_cut;
      affecte_vect(arbre->axe_princ, axe_cut);
      /* On initialise les fils */
      fils = arbre->fils[0] = creer_fils(arbre);
      fils->cluster = arbre->cluster;
      fils->taille = indice_decoupe;
      
      fils = arbre->fils[1] = creer_fils(arbre);
      fils->cluster = arbre->cluster + indice_decoupe;
      fils->taille = arbre->taille - indice_decoupe;
      
      /* Boucle pour les deux fils (droit et gauche) */
      for (i = 0; i < 2; i++) {
	 fils = arbre->fils[i];
	 m0 = fils->moment0 = moment0[i]; 
	 /* boucle sur les coordonnees */
	 for (j = 0; j < DIM; j++) {	
	    m1 = fils->moment1[j] = moment1[i][j]; 
	    fils->moy_axe[j] = (int)(m1 / m0);
	    for (k = 0; k < DIM; k++) {
	       fils->cov[j][k] =  (cov[i])[j][k];
	    }
	 }
	 fils->ecart = valeur(fils);
      }
      /* On corrige le pere */
      arbre->feuille = false;
      arbre->ecart = MAX(arbre->fils[0]->ecart, arbre->fils[1]->ecart);
   } else if (arbre->fils[0]->ecart > arbre->fils[1]->ecart) {
      creer_table(arbre->fils[0], tab_freq, partition, valeur, donne_dir);
      arbre->ecart = MAX(arbre->fils[0]->ecart, arbre->fils[1]->ecart);
   } else {
      creer_table(arbre->fils[1], tab_freq, partition, valeur, donne_dir);
      arbre->ecart = MAX(arbre->fils[0]->ecart, arbre->fils[1]->ecart);
   }
}

arbre_binaire* creer_racine( coul_freq* tab_freq, PFinit init, PFcluster valeur ) {
   arbre_binaire* racine;
  
   racine = (arbre_binaire*)malloc(sizeof(arbre_binaire));
  
   racine->ecart = 0.0;
   racine->fils[0] = (arbre_binaire*)NULL;
   racine->fils[1] = (arbre_binaire*)NULL;
   racine->cluster = tab_freq->tab;
   racine->taille = tab_freq->taille;
  
   /* Calcul des differents moments */
   init(racine, tab_freq);
   racine->ecart = valeur(racine);
   racine->feuille = true;
  
   return racine;
}

arbre_binaire* creer_la_table( coul_freq*  tab_freq, int*
			      nb_de_couleur, double ecart_max, 
			      int cluster_choisi, int
			      decoupe_choisie, int dir_choisie ) {
   int nb_couleur = 1;
   arbre_binaire *arbre;
   double ecart1, ecart2, ecart_initial;
   bool fin;

   static PFdecoupe choix_decoupe[] = {decoupe, decoupe_projette, decoupe_median}; 
   static PFcluster valeur_cluster[] = { eqm_pondere, eqm_non_pondere, cardinal};
   static PFaxecoupe direction_choisie[] = {direction_axe_principal, direction_moyenne_vect_propre};

   PFcluster valeur = valeur_cluster[cluster_choisi];
   PFdecoupe partition = choix_decoupe[decoupe_choisie];
   PFaxecoupe donne_dir = direction_choisie[dir_choisie];
   PFinit init = initialisation;

   arbre = creer_racine(tab_freq, init, valeur);
   ecart_initial = arbre->ecart;
   ecart1 = 100.0;
   fin = false;
   do {
      creer_table(arbre, tab_freq, partition, valeur, donne_dir);
      nb_couleur = nb_couleur + 1;
      ecart2 = (100 * arbre->ecart) / ecart_initial;
      /* on s' arrete si on est pres de ecart max et si l' */
      /* ecart intra classe decroit lentement*/
      fin = (ecart1 - ecart2 < 2.0) && (ecart2 < ecart_max + 2.0);
      /* On s' arrete de toute facon si on est en dessous de */
      /* ecart max*/
      fin = fin || (ecart2 <= ecart_max);
      ecart1 = ecart2;
   } while (nb_couleur < *nb_de_couleur);
   *nb_de_couleur = nb_couleur;
   return arbre;
}

/*****************************************************************************/
/****** retourne l'indice du cluster d'une couleur donnee */
/****************************************************************************/
void chercher_couleur_ds_table( arbre_binaire* arbre, couleur coul, int * indice_couleur ) {
   int i;
   arbre_binaire *fils_gauche, *fils_droit;
   double proj;

   if (arbre->feuille) {
      *indice_couleur = arbre->numero;
   } else {
      
      fils_gauche = arbre->fils[0];
      fils_droit = arbre->fils[1];
      proj = 0.0;
      for (i = 0; i < DIM; i++) {
	 proj += coul[i] * arbre->axe_princ[i];
      }

      if (proj < arbre->val_cut) {
	 chercher_couleur_ds_table(fils_gauche, coul, indice_couleur);
      } else {
	 chercher_couleur_ds_table(fils_droit, coul, indice_couleur);
      }
   }
}

/************************************************************/
/********** Remplit la table de couleurs  en prenant la couleur */
/********** moyenne de chaque cluster dans l'arbre           */
/************************************************************/
void etiqueter( arbre_binaire* arbre, int *table, int* numero ) {
   int indice;

   if (arbre->feuille) {
      indice = 3 * (*numero);
      for (int i = 0; i < 3; i++) {
	 *(table + indice) = arbre->moy_axe[i];
	 indice++;
      }
      arbre->numero = *numero;
      (*numero)++;
   } else {
      etiqueter(arbre->fils[0], table, numero);
      etiqueter(arbre->fils[1], table, numero);
   }
}

void etiqueter_feuilles( arbre_binaire* arbre, int*  table ) {
   int numero = 0;

   etiqueter(arbre, table, &numero);
}

void detruire_arbre( arbre_binaire* arbre ) {
   if (arbre->feuille) {
      free(arbre);
   } else {
      detruire_arbre(arbre->fils[0]);
      detruire_arbre(arbre->fils[1]);
   }
}


typedef int (*PFconversion)(int, int, int);

/*--------------------------------*/
/* Illuminant C */
/*--------------------------------*/
#define Xr 0.166
#define Yr 0.06
#define Zr 0.0

#define Xg 0.174
#define Yg 0.587
#define Zg 0.066

#define Xb 0.093
#define Yb 0.005
#define Zb 0.46

#define Xw 98.04
#define Yw 100.0f
#define Zw 118.10
#define Uw 0.2009f
#define Vw 0.4609f

#define Rx 6.686
#define Ry -1.832
#define Rz -1.332

#define Gx -0.684
#define Gy 1.893
#define Gz 0.118

#define Bx 0.098
#define By -0.272
#define Bz 2.157

/****************************************************************************/
/* fichier de conversion (R, G, B)--> Espace C1C2C3 */
/* Pour rajouter un espace Il faut : */
/* 1) Rajouter 3 fonctions de type PFconversion RGB_to_Ci */
/* 2) Rajouter 3 fonctions de type PFconversion C1C2C3_to_R, C1C2C3_to_G et C1C2C3_to_B */
/* 3) Toute fonction suplementaire est a ecrire dans conversion_aux.c */
/*    Sa definition est a rajouter dans conv_inter_conv_aux.h */
/* 4) Rajouter ces fonctions dans changer_espace et revenir espace */
/* 5) Aller dans choix.h rajouter le nouvel espace dans les menus sans */
/*    oublier d'incrementer le nombre d'espaces disponibles dans le */
/*    tableau contenant le cardinal de chaque menus*/
/****************************************************************************/

/****************Espace I1 I2 I3 *******************************************/
int RGB_to_I1( int R, int G, int B ) {
   static float inv_quotient = 1.0f / 3.0f;
   int retour  = (int)((R + G + B) * inv_quotient + 0.5);
   
   return retour;
}

int RGB_to_I2( int R, int, int B ) {
   return R - B;
}

int RGB_to_I3( int R, int G, int B ) {
   float I3 = G - (R + B) * 0.5f;
   int retour;

   retour = FLOAT_TO_INT(I3);

   return retour;
}

int I1I2I3_to_R( int I1 , int I2 , int I3 ) {
   static float un_d_3 = 1.0f / 3.0f;
   float R;
   int retour;

   R = I1 + 0.5f * I2 - un_d_3 * I3 + 0.5f;
   retour = (int)MAX(R, 0);
   retour  = MIN(retour, 255);
    
   return retour;
}

int I1I2I3_to_G( int I1 , int /* I2 */, int I3 ) {
   static float deux_d_3 = 2.0f / 3.0f;
   float G;
   int retour;

   G = I1 + deux_d_3 * I3 + 0.5f;
   retour = (int)MAX(G, 0);
   retour  = MIN(retour, 255);
    
   return retour;
}

int I1I2I3_to_B( int I1 , int I2 , int I3 ) {
   static float un_d_3 = 1.0f / 3.0f;
   float B;
   int retour;
    
   B = I1 - 0.5f * I2 - un_d_3 * I3 + 0.5f;
   retour = (int)MAX (B, 0);
   retour  = MIN(retour, 255);
    
   return retour;
}

/****************Espace H1 H2 H3 *******************************************/
int RGB_to_H2( int R, int G, int /* B */) {
   return R - G;
}

int RGB_to_H3( int R, int G, int B ) {
   float H3;
   int retour;
  
   H3 = (0.5f * (R + G) - B);
   retour = FLOAT_TO_INT(H3);

   return retour;
}

int H1H2H3_to_R( int H1, int H2, int H3 ) {
   static float un_tier = 1.0f/3.0f;
   float R;
   int retour;
	
   R = H1 + 0.5f * H2 + un_tier  *H3 + 0.5f;
   retour = (int)MAX(R, 0);
   retour  = MIN(retour, 255);
    
   return retour;
}

int H1H2H3_to_G( int H1, int H2, int H3 ) {
   static float un_tier = 1.0f / 3.0f;
   float G;
   int retour;

   G = H1 - 0.5f * H2 + un_tier  *H3 + 0.5f;
   retour = (int)MAX(G, 0);
   retour  = MIN(retour, 255);
    
   return retour;
}	

int H1H2H3_to_B( int H1, int /* H2*/, int H3 ) {
   static float deux_tier = 2.0f / 3.0f;
   float B;
   int retour;
	
   B =  H1 - deux_tier * H3 + 0.5f;
   retour = (int)MAX(B, 0);
   retour  = MIN(retour, 255);
    
   return retour;
}
 
/****************Espace L u v  *******************************************/

float RGB_to_Luv( int R, int G, int B );
int XYZ_to_R( float X, float Y, float Z );
void RGB_to_XYZ( int R, int G, int B, float *X, float*Y, float*Z );

int RGB_to_L( int R, int G, int B ) {
   int retour;

   retour = (int)(RGB_to_Luv(R, G, B) + 0.5f);

   return retour;
}

int RGB_to_u( int R, int G, int B ) {
   float X, Y, Z, up, u;
   int retour;
   float L = RGB_to_Luv(R, G, B);
   RGB_to_XYZ(R, G, B, &X, &Y, &Z);

   if (X == 0 && Y == 0 && Z == 0) {
      retour = 0;
   } else {
      up = 4 * X / (X + 15 * Y + 3 * Z);
      u = 13 * L * (up - Uw);
      retour = FLOAT_TO_INT(u);
   }
   return retour;
}

int RGB_to_v( int R, int G, int B ) {
   float X, Y, Z, vp, v;
   int retour;
   int L = RGB_to_L(R, G, B);

   RGB_to_XYZ(R, G, B, &X, &Y, &Z);

   if (X == 0 && Y == 0 && Z == 0) {
      retour = 0;
   } else {
      vp = 9 * Y / (X + 15 * Y + 3 * Z);
      v = 13 * L * (vp - Vw);
      retour = FLOAT_TO_INT(v);
   }
   return retour;
}   

void Luv_to_XYZ( int L, int u, int v, float *X, float *Y, float *Z );

int Luv_to_R( int L, int u, int v ) {
   int retour;
   float X, Y, Z;

   Luv_to_XYZ(L, u, v, &X, &Y, &Z);
   retour = XYZ_to_R(X, Y, Z);

   return retour;
}

int XYZ_to_G( float X, float Y, float Z );
int XYZ_to_B( float X, float Y, float Z );

int Luv_to_G( int L, int u, int v ) {
   int retour;
   float X, Y, Z;

   Luv_to_XYZ(L, u, v, &X, &Y, &Z);
   retour = XYZ_to_G(X, Y, Z);

   return retour;
}

int Luv_to_B( int L, int u, int v ) {
   int retour;
   float X, Y, Z;

   Luv_to_XYZ(L, u, v, &X, &Y, &Z);
   retour = XYZ_to_B(X, Y, Z);

   return retour;
}

/****************Espace Y I Q  *******************************************/
int RGB_to_Y( int R, int G, int B ) {
   static float tab_Y[3] = {0.299f, 0.587f, 0.133f};
   int Y;

   Y = (int)(tab_Y[0] * R + tab_Y[1] * G + tab_Y[2] * B + 0.5f);
   return Y;
}

int RGB_to_I( int R, int G, int B ) {
   static float tab_I[3] = {0.596f, -0.274f, -0.322f};
   float I;

   I = tab_I[0] * R + tab_I[1] * G + tab_I[2] * B;
   I = (I < 0 ? I - 0.5f : I + 0.5f);
   return (int)I;
}

int RGB_to_Q( int R, int G, int B ) {
   static float tab_Q[3] = {0.211f, -0.253f, 0.312f};
   float Q;

   Q = tab_Q[0] * R + tab_Q[1] * G + tab_Q[2] * B;
   Q = (Q < 0 ? Q - 0.5f : Q + 0.5f);
   return  (int)Q;
}

int YIQ_to_R( int Y, int I, int Q ) {
   static float tab_R[3] = {0.797f, 1.012f, 0.753f};
   float R;

   R = tab_R[0] * Y + tab_R[1] * I + tab_R[2] * Q + 0.5f;

   return (int)R;
}

int YIQ_to_G( int Y, int I, int Q ) {
   static float tab_G[3] = {1.212f, -0.330f, -0.784f};
   float G;

   G = tab_G[0] * Y + tab_G[1] * I + tab_G[2] * Q + 0.5f;

   return (int)G;
}

int YIQ_to_B( int Y, int I, int Q ) {
   static float tab_B[3] = {0.444f, -0.952f, 2.060f};
   float B;

   B = tab_B[0] * Y + tab_B[1] * I + tab_B[2] * Q + 0.5f;

   return (int)B;
}

/****************Espace R G B ***********************************************************/

int RGB_to_R( int R, int /* G */, int /* B */ ) {
   return R;
}

int RGB_to_G( int /* R */, int G, int /* B */ ) {
   return G;
}

int RGB_to_B( int /* R */, int /* G */, int B ) {
   return B;
}

/****************Espace  K1K2K3 ***********************************************************/

int RGB_to_K1( int R, int G, int /* B */ ) {
   int K1 = R + G;

   return K1;
}

int RGB_to_K2( int R, int G, int /* B */ ) {
   int K2 = R - G;

   return K2;
}

int RGB_to_K3( int R, int G, int B ) {
   int K3 = (int)(B - 0.5f * (R + G));

   return K3;
}

int K1K2K3_to_R( int K1, int K2, int /* K3 */ ) {
   int retour;

   float R = (K1 + K2) * 0.5f + 0.5f;

   retour = (int)MAX(R, 0);
   retour  = MIN(retour, 255);    
    
   return retour;
}

int K1K2K3_to_G( int K1, int K2, int /* K3 */ ) {
   int retour;

   float G = 0.5f * (K1 - K2) + 0.5f;

   retour = (int)MAX(G, 0);
   retour  = MIN(retour, 255);    
    
   return retour;
}

int K1K2K3_to_B( int K1, int /* K2 */ , int K3 ) {
   int retour;

   float B = 0.5f * K1 + K3 + 0.5f;

   retour = (int)MAX(B, 0);
   retour  = MIN(retour, 255);    
    
   return retour;
}

/****************Espace  test ***********************************************************/

int RGB_to_T1( int R, int G, int /* B */) {
   int K1 = (int)((R + G) * 0.5f);

   return K1;

}

int RGB_to_T2( int R, int G, int /* B */ ) {
   int K2 = (int)((R - G) * 0.5f);

   return K2;
}

int RGB_to_T3( int R, int G, int B ) {
   int K3 = (int)((B - 0.5f * (R + G)) * 0.5f);

   return K3;
}

int T1T2T3_to_R( int T1, int T2, int /* T3 */ ) {
   int retour;

   float R = (float)(T1 + T2);

   retour = (int)MAX(R, 0);
   retour  = MIN(retour, 255);    
    
   return retour;
}

int T1T2T3_to_G( int T1, int T2, int /* T3 */ ) {
   int retour;

   float G = (float)(T1-T2);

   retour = (int)MAX(G, 0);
   retour = MIN(retour, 255);    
    
   return retour;
}

int T1T2T3_to_B( int T1, int /* T2 */, int T3 ) {
   int retour;
  
   float B = T1 + 2.0f * T3;

   retour = (int)MAX(B, 0);
   retour  = MIN(retour, 255);    
  
   return retour;  
}

/****************************************************************************/
/**************** Espace  des vecteurs propres  *****************************/
/****************************************************************************/
void RGB_to_vecteur_propre( mat_passage, vecteur, vecteur, int , int );
void vecteur_propre_to_RGB( mat_passage P, vecteur V, vecteur lambda, int ligne, int flag );

int RGB_to_V1( int R, int G, int B ) {
   static bool retenu = false;
   static vecteur V1_to;
   vecteur val;
   int ligne = 0;
   matrice P;
   float V1;
   
   if (!retenu) {
      RGB_to_vecteur_propre(P, V1_to, val, ligne, RETOURNER);
      retenu = true;
   }
   
   V1 = (float)(V1_to[0] * R + V1_to[1] * G + V1_to[2] * B);
   
   int retour = FLOAT_TO_INT(V1);
   return retour;
}

int RGB_to_V2( int R, int G, int B ) {
   static bool retenu = false;
   static vecteur V2_to;
   vecteur val;
   int ligne = 1;
   matrice P;
   float V2;
   
   if (!retenu) {
      RGB_to_vecteur_propre(P, V2_to, val, ligne, RETOURNER);
      retenu = true;
   }
   
   V2 = (float)(V2_to[0]*R + V2_to[1]*G + V2_to[2]*B);
   
   return FLOAT_TO_INT(V2);
}

int RGB_to_V3( int R, int G, int B ) {
   static bool retenu = false;
   static vecteur V3_to;
   vecteur val;
   int ligne = 2;
   matrice P;
   float V3;
   
   if (!retenu) {
      RGB_to_vecteur_propre(P, V3_to, val, ligne, RETOURNER);
      retenu = true;
   }
   
   V3 = (float)(V3_to[0] * R + V3_to[1] * G + V3_to[2] * B);
   
   return FLOAT_TO_INT(V3);
}

void calcul_mat_cov( Comp_rgb* image, int largeur, int hauteur, mat_covariance M ) {
   Comp_rgb* img_cour = image;
   int R,G,B;
   double mom0;
   vecteur mom1;
   mat_covariance mom2;
   int i,j;
   mom0 = 0.0;
   mom1[0] = 0.0;
   mom1[1] = 0.0;
   mom1[2] = 0.0;
   
   mom2[0][0] = 0;
   mom2[1][1] = 0;
   mom2[2][2] = 0;
   mom2[1][0] = 0;
   mom2[2][0] = 0;
   mom2[2][1] = 0;
   
   for (i = 0; i < hauteur; i++) {
      for (j = 0; j < largeur; j++) {
	 R = *img_cour;img_cour++;
	 G = *img_cour;img_cour++;
	 B = *img_cour;img_cour++;
	 
	 mom0++;
	 
	 mom1[0] += R;
	 mom1[1] += G;
	 mom1[2] += B;
	 
	 mom2[0][0] += R*R;
	 mom2[1][1] += G*G;
	 mom2[2][2] += B*B;
	 mom2[1][0] += G*R;
	 mom2[2][0] += B*R;
	 mom2[2][1] += B*G;
      }
   }
   
   for (i = 0; i < DIM; i++) {
      for (j = 0;j <= i; j++) {
	 M[i][j] = (mom2[i][j] - mom1[i] * (mom1[j] / mom0) ) / mom0;
      }
   }
   for (i = 0; i < DIM; i++) {
      for (j = i + 1 ;j < DIM; j++) {
	 M[i][j] = M[j][i];
      }
   }
}

void val_vect_propre( mat_covariance a, vecteur val_propre, mat_passage P, matrice x0 ) {
   vecteur x,y;
   double norme_x,norme_ymx,e,lambda;
   int i,j,k;
   
   for (i = 0; i < DIM - 1; i++) {
      if (i <DIM - 1) {
	 for (j = 0; j < DIM; j++) {
	    x[j] = x0[i][j];
	 }
      }
      do {
	 /*  Calcul de norme de x */
	 norme_x = 0.0;
	 for (j = 0; j < DIM; j++) {
	    norme_x += x[j] * x[j];
	 }
	  norme_x = sqrt(norme_x);
	  
	  /* calcul de y */
	  
	  for (j = 0; j < DIM; j++) {
	     y[j] = 0.0;
	     for (k = 0; k < DIM; k++) {
		y[j] += a[j][k] * x[k];
	     }
	     y[j] = y[j] / norme_x;
	  }
	  /* calcul de convergence */
	  norme_ymx = 0.0;
	  for (j = 0; j < DIM; j++) {
	     norme_ymx +=(y[j] - x[j]) * (y[j] - x[j]);
	  }
	  norme_ymx = sqrt(norme_ymx);
	  e = norme_ymx/norme_x;
	  
	  /* x = y*/
	  for (j = 0; j < DIM; j++) {
	     x[j] = y[j];
	  }
       } while(e > 0.0001);
       
       k = 0;
       while ((x[k] == 0) && (k < DIM)) {
	  k++;
       }
       if (k == DIM) {
	  printf("PB vecteur propre nul\n");
	  exit(0);
       } else {
	  /* On normalise x */
	  for (j = 0; j < DIM; j++) {
	     norme_x += x[j] * x[j];
	  }
	  norme_x = sqrt(norme_x);
	  for (j = 0; j < DIM; j++) {
	     x[j] = x[j]/norme_x;
	  }
	  y[k] = 0.0;
	  for (j = 0; j < DIM; j++) {
	     y[k] += a[k][j] * x[j];
	  }
	  
	  lambda = y[k] / x[k];
       }
       
       for (j = 0; j < DIM; j++) {
	  for (k = 0; k < DIM; k++) {
	     a[j][k] -= lambda * x[k] * x[j];
	  }
       }
       for (j = 0; j < DIM; j++) {
	  P[j][i] = x[j];
       }
       val_propre[i] = lambda; 
    }
    /* La on utilise le fait que la matrice */
    /* est 3*3 pour en deduire le dernier vecteur propre*/
    norme_x =0.0;
    for (i = 0; i < DIM; i++) {
       j= (i + 1) % DIM;
       k= (i + 2) % DIM;
       P[i][2] = P[j][0] * P[k][1] - P[k][0] * P[j][1];
       norme_x += P[i][2] * P[i][2];
    }
    for (j = 0; j < DIM; j++) {
       P[j][2] = P[j][2] / norme_x;
    }
    k = 0;
    while ((P[k][2] == 0) && (k < DIM)) {
       k++;
    }
    if (k == DIM) {
       printf("PB vecteur propre nul\n");
       exit(0);
    } else {
       lambda = 0.0;
       for (j = 0; j < DIM; j++) {
	  lambda += a[k][j] * P[j][2];
       }
       val_propre[2] = lambda / P[k][2];
    }
    // printf("lambda2 = %f",val_propre[2]);
    // printf(" x2 = ( ",i);
    // for (j=0;j<2;j++)
    // 	    printf("%f,",P[j][2]);
    // printf("%f)\n",P[2][2]);
    // printf("\n");    
}

void Calcul_vecteur_propre( Comp_rgb* image, int largeur, int hauteur ) {    
   matrice  val_initiale = {
      {UN_TIER,UN_TIER,UN_TIER},
      { 1.0,0,-1.0},
      { -0.5,1.0,-0.5},
   };
   
   mat_covariance cov;
   mat_passage P;
   vecteur V,val_propre;
   
   calcul_mat_cov(image, largeur, hauteur, cov);    
   val_vect_propre(cov, val_propre, P, val_initiale);
   
   RGB_to_vecteur_propre(P, V, val_propre, 1, RETENIR);
   vecteur_propre_to_RGB(P, V, val_propre, 1, RETENIR);
}

int V1V2V3_to_R( int V1, int V2, int V3 ) {
   static bool retenu = false;
   static vecteur R_to;
   static vecteur val_propre;
   int ligne = 0;
   matrice P;
   float R;

   if (!retenu) {
      vecteur_propre_to_RGB(P, R_to, val_propre, ligne, RETOURNER);
      retenu = true;
   }

   R = (float)((R_to[0] * V1 + R_to[1] * V2 + R_to[2] * V3) + 0.5f);

   int retour = (int)MAX(R, 0);
   retour = MIN(retour, 255);    

   return retour;
}

int V1V2V3_to_G( int V1, int V2, int V3 ) {
   static bool retenu = false;
   static vecteur G_to;
   static vecteur val_propre;
   int ligne = 1;
   matrice P;
   float G;
   int retour;

   if (!retenu) {
      vecteur_propre_to_RGB(P, G_to, val_propre, ligne, RETOURNER);
      retenu = true;
   }

   G = (float)((G_to[0]*V1 + G_to[1]*V2 + G_to[2]*V3) + 0.5f);

   retour = (int)MAX(G, 0);
   retour  = MIN(retour, 255);    

   return retour;
}

int V1V2V3_to_B( int V1, int V2, int V3 ) {
   static bool retenu = false;
   static vecteur B_to;
   static vecteur val_propre;
   int ligne = 2;
   matrice P;
   float B;
   int retour;

   if (!retenu) {
      vecteur_propre_to_RGB(P, B_to, val_propre, ligne, RETOURNER);
      retenu = true;
   }

   B = (float)((B_to[0] * V1 + B_to[1] * V2 + B_to[2] * V3) + 0.5f);

   retour = (int)MAX(B, 0);
   retour  = MIN(retour, 255);    

   return retour;
}    

PFconversion* changer_espace( int choix ) {
   static PFconversion conversion[][3] = {
      {RGB_to_R, RGB_to_G, RGB_to_B}, 
      {RGB_to_I1, RGB_to_I2, RGB_to_I3}, 
      {RGB_to_I1, RGB_to_H2, RGB_to_H3}, 
      {RGB_to_L, RGB_to_u, RGB_to_v}, 
      {RGB_to_Y, RGB_to_I, RGB_to_Q}, 
      {RGB_to_V1, RGB_to_V2, RGB_to_V3}, 
      {RGB_to_K1, RGB_to_K2, RGB_to_K3}, 
      {RGB_to_T1, RGB_to_T2, RGB_to_T3}, 
   };
   return conversion[choix];
}

PFconversion* revenir_espace( int choix ) {
   static PFconversion conversion_inverse[][3] = {
      {RGB_to_R, RGB_to_G, RGB_to_B}, 
      {I1I2I3_to_R, I1I2I3_to_G, I1I2I3_to_B}, 
      {H1H2H3_to_R, H1H2H3_to_G, H1H2H3_to_B}, 
      {Luv_to_R, Luv_to_G, Luv_to_B}, 
      {YIQ_to_R, YIQ_to_G, YIQ_to_B}, 
      {V1V2V3_to_R, V1V2V3_to_G, V1V2V3_to_B}, 
      {K1K2K3_to_R, K1K2K3_to_G, K1K2K3_to_B}, 
      {T1T2T3_to_R, T1T2T3_to_G, T1T2T3_to_B}, 
   };
   return conversion_inverse[choix];
}

/****************************************************************************/
/* Ce fichier contient uniquement des fonctions auxilieres utilisees */
/* par les fonctions de conversion du fichier conversion.c */
/****************************************************************************/
void RGB_to_XYZ( int R, int G, int B, float *X, float *Y, float *Z ) {
   *X = (float)(Xr * R + Xg * G + Xb * B);
   *Y = (float)(Yr * R + Yg * G + Yb * B);
   *Z = (float)(Zr * R + Zg * G + Zb * B);
}

float RGB_to_xYz( int R, int G, int B ) {
   float retour;

   retour = (float)(Yr * R + Yg * G + Yb * B);
    
   return retour;
}

void Luv_to_XYZ( int L, int u, int v, float *X, float *Y, float *Z ) {
   float up, vp;
   static float neuf_par_quatre = 9.0f / 4.0f;
   static float cent_seize_p_3 = 116 * 116 * 116;

   up = u / (13.0f * L) + Uw;
   vp = v / (13.0f * L) + Vw;

   *Y = (float)(Yw * (L + 16) * (L + 16) * (L + 16) / cent_seize_p_3);
   *X = neuf_par_quatre * up * (*Y) / vp;
   *Z = -(9 * up + 60 * vp - 36) * (*Y) / (12 * vp);
}

int XYZ_to_R( float X, float Y, float Z ) {
   float R = (float)(Rx * X + Ry * Y + Rz * Z + 0.5f);

   int retour = (int)MAX(R, 0);
   retour = MIN(retour, 255);
    
   return retour;
}

int XYZ_to_G( float X, float Y, float Z ) {
   float G = (float)(Gx * X + Gy * Y + Gz * Z + 0.5f);

   int retour = (int)MAX(G, 0);
   retour = MIN(retour, 255);

   return retour;
}

int XYZ_to_B( float X, float Y, float Z ) {
   float B = (float)(Bx * X + By * Y + Bz * Z + 0.5f);

   int retour = (int)MAX(B, 0);
   retour = MIN(retour, 255);
    
   return retour;
}

float RGB_to_Luv( int R, int G, int B ) {
   float Y = RGB_to_xYz(R, G, B);
   float YparYw = Y / Yw;
   float retour;
    
   if (YparYw < 0.01f) {
      retour = 903.3f * YparYw;
   } else {
      retour = (float)(116.0f * cbrt(YparYw) - 16.0f);
   }
   return retour;
}

void RGB_to_vecteur_propre( mat_passage P, vecteur V, vecteur lambda, int ligne, int flag ) {
   static mat_passage Pass;
   static vecteur lambda_par_somme;
   int i, j;
   double somme;

   if (flag == RETENIR) {
      for (i = 0; i < DIM; i++) {
	 for (j = 0; j < DIM; j++) {
	    Pass[i][j] = P[j][i];
	 }
      }
      somme = 0.0;
      for (i = 0; i < DIM; i++) {
	 somme += lambda[i];
      }
      for (i = 0; i < DIM; i++) {
	 lambda_par_somme[i] = lambda[i] / somme;
      }
   } else {
      if (flag == RETOURNER) {
	 for (j = 0; j < DIM; j++) {
	    V[j] = Pass[ligne][j];
	    lambda[j] = lambda_par_somme[j];
	 }
      } else {
// 	 printf("Erreur dans RGB_to_vecteur_propre\n");
// 	 printf("flag incorect\n");
      }
   }
}

void vecteur_propre_to_RGB( mat_passage P, vecteur V, vecteur lambda, int ligne, int flag ) {
   static mat_passage Pass;
   static vecteur somme_par_lambda;
   double somme;

   if (flag == RETENIR) {
      for (int i = 0; i < DIM; i++) {
	 for (int j = 0; j < DIM; j++) {
	    Pass[i][j] = P[i][j];
	 }
      }
      somme = 0.0;
      for (int i = 0; i < DIM; i++) {
	 somme += lambda[i];
      }
      for (int i = 0; i < DIM; i++) {
	 somme_par_lambda[i] = somme / lambda[i];
      }
   } else {
      if (flag == RETOURNER) {
	 for (int j = 0; j < DIM;j ++) {
	    V[j] = Pass[ligne][j];
	    lambda[j] = somme_par_lambda[j];
	 }
      } else {
	 printf("Erreur dans_vecteur_propre to RGB\n");
	 printf("flag incorect\n");
      }
   }
}

// /*
//  *  File:	hash.c
//  *  Author:	Robert Strandh
//  *  Purpose:
//  *      Fash general hash tables.
//  *
//  *  $Id: hash.c, v 1.2 1992/03/19 21:22:57 david Exp $
//  */

typedef void *hash_anything;
typedef void *hash_object;

typedef struct hash_table_structure *hash_table;
typedef struct hash_binding_structure *hash_binding;

struct hash_table_structure {
      int hash_table_size; 
      int number_of_bindings;
      hash_binding *hash_table;
      hash_binding *cursor;
      hash_anything (*malloc)(int size);
      void (*free)( hash_anything );
      int (*equal)( hash_object, hash_object );
      void (*deallocate)( hash_object );
      int (*hash)( hash_object );
      void (*print)( hash_object );
};

struct hash_binding_structure {
      int hashval;
      hash_object obj;
      hash_binding next;
};

/* minimum hash table size in pointers */

#ifndef MIN_HASH_TABLE_SIZE
#define MIN_HASH_TABLE_SIZE 3
#endif

#ifndef MAX_FILL_DEGREE
#define MAX_FILL_DEGREE 5
#endif

hash_table hash_create(hash_anything (*malloc)(int size), 
	    void (*free)( hash_anything ), 
	    int (*hash)( hash_object ), 
	    int (*equal)( hash_object, hash_object ), 
	    void (*deallocate)( hash_object )) {
   hash_table temp = (hash_table) (*malloc)(sizeof (struct hash_table_structure));
   temp->hash_table_size = MIN_HASH_TABLE_SIZE;
   temp->number_of_bindings = 0;
   temp->hash_table = (hash_binding *) (*malloc)(MIN_HASH_TABLE_SIZE * sizeof(hash_binding));
   for (int i = 0; i < MIN_HASH_TABLE_SIZE; i++) {
      temp->hash_table[i] = NULL;
   }
   temp->cursor = NULL;
   temp->malloc = malloc;
   temp->free = free;
   temp->equal = equal;
   temp->hash = hash;
   temp->deallocate = deallocate;
   return temp;
}

void hash_destroy( hash_table ht ) {
   hash_binding bind, temp;
   for (int i = 0; i < ht->hash_table_size; i++) {
      for (bind = ht->hash_table[i]; bind; ) {
	 temp = bind->next;
	 (*(ht->deallocate))(bind->obj);
	 (*(ht->free))(bind);
	 bind = temp;
      }
   }
   (*(ht->free))(ht->hash_table);
}

int hash_position( hash_table ht, hash_object obj ) {
   int index = (*(ht->hash))(obj) % ht->hash_table_size;
   hash_binding *bind = &(ht->hash_table[index]);
   for (; *bind; bind = &((*bind)->next)) {
      if ((*(ht->equal))((*bind)->obj, obj)) {
	 ht->cursor = bind;
	 return 1;
      }
   }
   ht->cursor = bind;
   return 0;
}

static void resize_table( hash_table ht, int new_hash_table_size ) {
   int i;
    
   hash_binding *new_hash_table = (hash_binding *)
      (*(ht->malloc))(new_hash_table_size * sizeof(hash_binding));
   for (i = 0; i < new_hash_table_size; i++) {
      new_hash_table[i] = NULL;
   }
   for (i = 0; i < ht->hash_table_size; i++) {
      hash_binding bind, temp;
      for (bind = ht->hash_table[i]; bind; ) {
	 int index = bind->hashval % new_hash_table_size;
	 temp = bind->next;
	 bind->next = new_hash_table[index];
	 new_hash_table[index] = bind;
	 bind = temp;
      }
   }
   (*(ht->free))(ht->hash_table);
   ht->hash_table = new_hash_table;
   ht->hash_table_size = new_hash_table_size;
}

static void possibly_increase_size( hash_table ht ) {
   if (ht->number_of_bindings > ht->hash_table_size * MAX_FILL_DEGREE) {
      resize_table(ht, (ht->hash_table_size) * 2 + 1);
   }
}

void hash_insert( hash_table ht, hash_object obj ) {
   hash_binding old = *(ht->cursor);
   (*(ht->cursor)) = (hash_binding)(*(ht->malloc))(sizeof(struct hash_binding_structure));
   (*(ht->cursor))->hashval = (*(ht->hash))(obj);
   (*(ht->cursor))->next = old;
   (*(ht->cursor))->obj = obj;
   (ht->number_of_bindings)++;
   possibly_increase_size(ht);
}

void hash_map( hash_table ht, void (*fun)(hash_object) ) {
   hash_binding bind;
   for (int i = 0; i < ht->hash_table_size; i++) {
      for (bind = ht->hash_table[i]; bind; bind = bind->next) {
	 (*fun)(bind->obj);
      }
   }
}

hash_object hash_get( hash_table ht ) {
   return (*(ht->cursor))->obj;
}

hash_anything alloue( int size ) {
   hash_anything retour = malloc(size);
   return retour;
}

void desalloue( hash_object pcel ) {
   free(pcel);
}

void init_tab( int taille, int** tab ) {
   *tab = (int*)malloc(taille * sizeof(int));
   for (int i = 0; i < taille; i++) {
      (*tab)[i] = i;
   }
}

void coul_freq_detruit( coul_freq *c_freq ) {
   free(c_freq->tab_freq);
   free(c_freq->tab);
}

void transfer( hash_object pcel_in ) {
   f_coul * pcel = (f_coul*)pcel_in;
   static int compteur;
   static f_coul* tab_freq;
    
   if (pcel->I1 == INT_MIN) {
      compteur = 0;
      tab_freq = (f_coul*)pcel->freq;
   } else {
      tab_freq[compteur].I1 = pcel->I1;
      tab_freq[compteur].I2 = pcel->I2;
      tab_freq[compteur].I3 = pcel->I3;
      tab_freq[compteur].freq = pcel->freq;
      compteur++;
   }
}

int f_coul_egal( hash_object pcel1, hash_object pcel2 ) {
   f_coul *pcel11 = (f_coul*)pcel1;
   f_coul *pcel12 = (f_coul*)pcel2;
   bool egal_I1 = (pcel11->I1 == pcel12->I1);
   bool egal_I2 = (pcel11->I2 == pcel12->I2);
   bool egal_I3 = (pcel11->I3 == pcel12->I3);
   
   return (egal_I1&&egal_I2&&egal_I3);
}

int cle( hash_object pcel ) {
   f_coul *pcel1 =  (f_coul*)pcel;
   unsigned int aI1 = ABS(pcel1->I1);
   unsigned int aI2 = ABS(pcel1->I2);
   unsigned int aI3 = ABS(pcel1->I3);
   
   int retour = (aI1 << 16) | (aI2 << 8) | aI3;
   
   return retour;
}

/*#define IMAGE_DIST*/
void map_image( arbre_binaire* arbre, Comp_rgb **table_rgb, int hauteur, int largeur, Comp_rgb *image, int nb_de_couleur, int espace_choisi ) {
   int i, j;
   int R, G, B;
   int indice_couleur;
   Comp_rgb *pixel;
   int *table_cour;
   Comp_rgb *table_cour_rgb;
   couleur C;

   PFconversion *c1c2c3_to_rgb = revenir_espace(espace_choisi);
   PFconversion *rgb_to_c1c2c3 = changer_espace(espace_choisi);
   int* table_C1C2C3;

   table_C1C2C3 = (int*)malloc(3 * nb_de_couleur * sizeof(int));
   *table_rgb = (Comp_rgb*)malloc(sizeof(Comp_rgb) * 3 * nb_de_couleur);
   etiqueter_feuilles(arbre, table_C1C2C3);    


   /* On passe la table de couleurs en RGB */
   table_cour = table_C1C2C3;
   table_cour_rgb = *table_rgb;
   for (i = 0; i < nb_de_couleur; i++) {
      C[0] = *table_cour;
      C[1] = *(table_cour + 1);
      C[2] = *(table_cour + 2);
      
      /* On calcul et minore */
      
      R = c1c2c3_to_rgb[0](C[0], C[1], C[2]);
      G = c1c2c3_to_rgb[1](C[0], C[1], C[2]);
      B = c1c2c3_to_rgb[2](C[0], C[1], C[2]);

      *table_cour = *table_cour_rgb = R;
      *(table_cour + 1) = *(table_cour_rgb + 1) = G;
      *(table_cour + 2) = *(table_cour_rgb + 2) = B;
      table_cour += 3;
      table_cour_rgb += 3;
   }
   free(table_C1C2C3);    
   /* On passe l' image en n couleurs */

   pixel = image;

   for (i = 0; i < hauteur; i++) {
      for (j = 0; j < largeur; j++) {
	 R = (int)(*pixel);
	 G = (int)(*(pixel+1));
	 B = (int)(*(pixel+2));
	 C[0] = rgb_to_c1c2c3[0](R, G, B);
	 C[1] = rgb_to_c1c2c3[1](R, G, B);
	 C[2] = rgb_to_c1c2c3[2](R, G, B);

	 chercher_couleur_ds_table(arbre, C, &indice_couleur);
	 table_cour_rgb = *table_rgb + 3 * indice_couleur;
			
	 *(pixel) = *table_cour_rgb;
	 *(pixel+1) = *(table_cour_rgb + 1);
	 *(pixel+2) = *(table_cour_rgb + 2);
	
	 pixel += 3;
      }
   }
}

static f_coul* donnee; /* vecteur contenant les couleurs */
static vecteur vp; /* vecteur propre par rapport a qui l'on va trier */

float projette( double x, double y, double z ) {
   return (float)(vp[0] * x + vp[1] * y + vp[2] * z);
}
	
int compare( const void *k, const void *l ) {
   const int i = *(reinterpret_cast<const int*>(k));
   const int j = *(reinterpret_cast<const int*>(l));
  
   float val1 = projette(donnee[i].I1, donnee[i].I2, donnee[i].I3);
   float val2 = projette(donnee[j].I1, donnee[j].I2, donnee[j].I3);

   if (val1 < val2) {
      return -1;
   }
   if (val1 == val2) {
      return 0;
   }
  
   return 1;
}

void trier( coul_freq* tab_freq, int* tab, int taille, vecteur v ) {
   /* On recopie les donnees dans les variables statiques */
   for (int i = 0; i < DIM; i++) {
      vp[i] = v[i];
   }
   donnee = tab_freq->tab_freq;
/* Fin recopie, lancement du quicksort */
   qsort(tab, taille, sizeof(int), compare);
}

void affecte_vect( vecteur v1, vecteur v2 ) {
   for (int i = 0; i < DIM; i++) {
      v1[i] = v2[i];
   }
}

double scalaire_vect( vecteur v1, vecteur v2 ) {
   double res = 0.0;

   for (int i = 0; i < DIM; i++) {
      res += v1[i] * v2[i];
   }
  
   return res;
}

void vecteur_propre_principal( mat_covariance a, vecteur v_propre, vecteur x0 ) {
   vecteur x, y;
   double norme_x, norme_ymx, e;
   int i, j;

   for (int i = 0; i < DIM; i++) {
      x[i] = x0[i];
   }
   
   do {
      /*  Calcul de norme de x */
      norme_x = scalaire_vect(x, x);
      norme_x = sqrt(norme_x);
	
      /* calcul de y */		
      for (i = 0; i < DIM; i++) {
	 y[i] = 0.0;
	 for (j = 0; j < DIM; j++) {
	    y[i] += a[i][j] * x[j];
	 }
	 y[i] = y[i] / norme_x;
      }
      
      /* calcul de convergence */
      norme_ymx = 0.0;
      for (i = 0; i < DIM; i++) {
	 norme_ymx += (y[i] - x[i]) * (y[i] - x[i]);
      }
      norme_ymx = sqrt(norme_ymx);
      e = norme_ymx / norme_x;
	
      /* x = y*/
      for (i = 0; i < DIM; i++) {
	 x[i] = y[i];
      }
   } while (e > 0.0001);

   i = 0;
   while (i< DIM && x[i] == 0) {
      i++;
   }
   if (i == DIM) {
      printf("PB vecteur propre nul\n");
      exit(0);
   } else {
      /* On normalise x */
      norme_x = scalaire_vect(x, x);
      norme_x = sqrt(norme_x);

      for (j = 0; j < DIM; j++) {
	 x[j] = x[j] / norme_x;
      }
      y[i] = 0.0;
      for (j = 0; j < DIM; j++) {
	 y[i] += a[i][j] * x[j];
      }
   }

   /* On recupere le resultat */
   for (i = 0; i < DIM; i++) {
      v_propre[i] = x[i];
   }
}

void vecteur_valeur_propres( mat_covariance a, vecteur val_propre, mat_passage P ) {
   vecteur x, y;
   double norme_x, norme_ymx, e, lambda;
   int i, j, k;
   int nb_vect_propre;

/* Ds les cas 2 et 3 des optimisation sont effectuees a la fin */
   if ((DIM == 2) || (DIM == 3)) {
      nb_vect_propre = DIM - 1;
   } else {
      nb_vect_propre = DIM;
   }
  
   for (i = 0; i < nb_vect_propre; i++) {
      for (j = 0; j < DIM; j++) {
	 x[j] = j;
      }
      do {
	 /*  Calcul de norme de x */
	 norme_x = scalaire_vect(x, x);
	 norme_x = sqrt(norme_x);
	  
	 /* calcul de y */
	 for (j = 0; j < DIM; j++) {
	    y[j] = 0.0;
	    for (k = 0; k < DIM; k++) {
	       y[j] += a[j][k] * x[k];
	    }
	    y[j] = y[j] / norme_x;
	 }
	 /* calcul de convergence */
	 norme_ymx = 0.0;
	 for (j = 0; j < DIM; j++) {
	    norme_ymx += (y[j] - x[j]) * (y[j] - x[j]);
	 }
	 norme_ymx = sqrt(norme_ymx);
	 e = norme_ymx/norme_x;
	  
	 /* x = y*/
	 for (j = 0; j < DIM; j++) {
	    x[j] = y[j];
	 }
      } while (e > 0.0001);
      
      k = 0;
      while (k < DIM && x[k] == 0) {
	 k++;
      }

      norme_x = scalaire_vect(x, x);
      norme_x = sqrt(norme_x);

      for (j = 0; j < DIM; j++) {
	 x[j] = x[j] / norme_x;
      }
      if (k == DIM) {
	 printf("PB vecteur propre nul\n");
	 exit(0);
      } else {
	 /* On normalise x */
	 y[k] = 0.0;
	 for (j = 0; j < DIM; j++) {				   
	    y[k] += a[k][j] * x[j];
	 }
	 lambda = y[k] / x[k];
      }


      for (j = 0; j < DIM; j++) {
	 for (k = 0; k < DIM; k++) {
	    a[j][k] -= lambda * x[k] * x[j];
	 }
      }
      /* On stocke les resultats */
      for (j = 0; j < DIM; j++) {
	 P[j][i] = x[j];
      }
      val_propre[i] = lambda; 
		
   } /* Fin boucle principale */

   if ((DIM == 2) || (DIM == 3) ) {
      /* Calcul du dernier vecteur propre, val propre */
      if (DIM == 3) {
	 /* La on utilise le fait que la matrice */
	 /* est 3*3 pour en deduire le dernier vecteur propre*/
	 /* Par un produit vectoriel */
	 for (i = 0; i < DIM; i++) {
	    j = (i + 1) % DIM;
	    k = (i + 2) % DIM;
	    P[i][2] = P[j][0] * P[k][1] - P[k][0] * P[j][1];
	 }
      } else {
	 if (DIM == 2) {
	    P[0][1] = P[1][0];
	    P[1][1] = -P[0][0];
	 }
      }
      norme_x = 0;
      for (j = 0; j < DIM; j++) {
	 norme_x += P[j][DIM - 1] * P[j][DIM - 1];
      }
      
      for (j = 0; j < DIM; j++) {
	 P[j][DIM - 1] = P[j][DIM - 1] / norme_x;
      }
      
      k = 0;
      while ((P[k][DIM - 1] == 0) && (k < DIM)) {
	 k++;
      }
      
      if (k == DIM) {
	 fprintf(stderr, "PB vecteur propre nul\n");
	 exit(0);
      } else {
	 lambda = 0.0;
	 for (j = 0; j < DIM; j++) {
	    lambda += a[k][j] * P[j][DIM - 1];
	 }
	 val_propre[DIM - 1] = lambda / P[k][DIM - 1];
      }
   }
}


/**
 * cluster_choisie: un entier [1,3] qui définit le cluster à decouper
 * 1  Plus forte EQM pondere
 * 2  Plus forte EQM non pondere
 * 3  Plus grand cardinal
 *
 * dir_choisie: un entier qui définit la direction de coupe
 * 1  Couper perpendiculairement a l'axe principal du cluster
 * 2  Couper perpendiculairement a une moyenne des vecteurs propres
 *
 * decoupe_choisie: Choix de la position du plan de coupe
 * 1  Minimisation somme EQM
 * 2  Minimisation de la somme des Variances sur un axe
 * 3  Coupe mediane
 */
Errc PColorQuantization( Comp_rgb *image, int largeur, int hauteur, 
	   int nb_de_couleur, double ecart_max, int cluster_choisi, 
	   int decoupe_choisie, int espace_choisi, int dir_choisie ) {
   int i, j;
   int deplacementij;
   int R, G, B;
   arbre_binaire *arbre;
   Comp_rgb *pixel;
   
   int nb_couleur;
   coul_freq c_freq;
   int nb_cel = 0;
   f_coul *tab_freq;
   int *tab;
   f_coul cel, *pcel;
   hash_table h_table;
   float un_par_reduction = 1.0 / REDUCTION;
   PFconversion *convert = changer_espace(espace_choisi);
   Comp_rgb *table_couleur;

   if (nb_de_couleur <= 0.0) {
      fprintf(stderr, "Error pcolorquantification: bad number of color_number %d\n", nb_de_couleur);
      return FAILURE;
   }
   if (espace_choisi < 0 || espace_choisi > 6 ) {
      fprintf(stderr, "Error pcolorquantification: bad color_space value %d\n", espace_choisi);
      return FAILURE;
   }

   if (espace_choisi == 5) {
      Calcul_vecteur_propre(image, largeur, hauteur);
   }

   nb_couleur = nb_de_couleur;
   h_table = hash_create(alloue, free, cle, f_coul_egal, desalloue);

   for (i = 0; i < hauteur; i++) {              /* on balaye  le fichier  */   
      for (j = 0 ; j < largeur; j++) {	 
	 deplacementij = 3 * (i * largeur + j);
	 pixel = image + deplacementij;
	 
	 R = (int)(*pixel);
	 G = (int)(*(pixel + 1));
	 B = (int)(*(pixel + 2));
	 
	 cel.I1 = (int)(convert[0](R, G, B) * un_par_reduction);
	 cel.I2 = (int)(convert[1](R, G, B) * un_par_reduction);
	 cel.I3 = (int)(convert[2](R, G, B) * un_par_reduction);
	 cel.freq = 1;
	 if (hash_position(h_table, &cel)) {
	    pcel = (f_coul*)hash_get(h_table);
	    pcel->freq++;
	 } else {
	    pcel = (f_coul*)malloc(sizeof(f_coul));
	    pcel->I1 = cel.I1;
	    pcel->I2 = cel.I2;
	    pcel->I3 = cel.I3;
	    pcel->freq = cel.freq;
	    nb_cel++;
	    hash_insert(h_table, pcel);
	 }
      }
   }
   tab_freq = (f_coul*) malloc(nb_cel * sizeof(f_coul));
   
   init_tab(nb_cel, &tab);
   cel.I1 = INT_MIN;
   cel.freq = (long)tab_freq;
   transfer(&cel);                     /* on positionne tab_freq ds tranfer */
   
   hash_map(h_table, transfer);        /* On tranfere la table dans tab_freq*/
   hash_destroy(h_table);              /* on detruit la table */
   
   c_freq.tab_freq = tab_freq;
   c_freq.tab = tab;
   c_freq.taille = nb_cel;
   
   arbre = creer_la_table(&c_freq, &nb_couleur, ecart_max, cluster_choisi, decoupe_choisie, dir_choisie);    
   coul_freq_detruit(&c_freq);

   map_image(arbre, &table_couleur, hauteur, largeur, image, nb_couleur, espace_choisi);
   detruire_arbre(arbre);

   return nb_couleur;
}

void convertRRGGBBtoRGBRGB( const Imc2duc &ims, Uchar* image ) {
      // conversion from rrrggbb to rgbrgbrgb
   Uchar *dataR = ims.Vector(0);
   Uchar *dataV = ims.Vector(1);
   Uchar *dataB = ims.Vector(2);
   
   for (int i = 0; i < (int)ims.VectorSize(); i++) {
      image[i * 3] = dataR[i];
      image[i * 3 + 1] = dataV[i];
      image[i * 3 + 2] = dataB[i];
   }
}

void convertRGBRGBtoRRGGBB( const Uchar* image, Imc2duc &imd ) {
      Uchar *dataR = imd.Vector(0);
      Uchar *dataV = imd.Vector(1);
      Uchar *dataB = imd.Vector(2);
      
      for (int i = 0; i < (int)imd.VectorSize(); i++) {
	 dataR[i] = image[i * 3];
	 dataV[i] = image[i * 3 + 1];
	 dataB[i] = image[i * 3 + 2];
      }
}

#ifdef MAIN
#define	USAGE	"usage: %s color_number color_space [-m mask] [im_in|-] [im_out|-]"
#define	PARC	2
#define	FINC	1
#define	FOUTC	1
#define	MASK	0

int main( int argc, char *argv[] ) {
   Errc result;                // The result code of the execution.
   Pobject* mask;              // The region map.
   Pobject* objin[FINC + 1];   // The input objects.
   Pobject* objs[FINC + 1];    // The source objects masked.
   Pobject* objout[FOUTC + 1]; // The output object.
   Pobject* objd[FOUTC + 1];   // The result object of the execution.
   char* parv[PARC + 1];       // The input parameters.

   ReadArgs(argc, argv, PARC, FINC, FOUTC, &mask, objin, objs, objout, objd, parv, USAGE, MASK);
   if (objs[0]->Type() == Po_Imc2duc) {
      Imc2duc* const ims = (Imc2duc*)objs[0];
      objd[0] = new Imc2duc(ims->Props());
      Imc2duc* const imd = (Imc2duc*)objd[0];

      Uchar* image = new Uchar[ims->VectorSize() * 3];
      convertRRGGBBtoRGBRGB(*ims, image);
      const double ecart_max = 30.0;
      result = PColorQuantization(image, ims->Width(), ims->Height(), atoi(parv[0]), ecart_max, 1, 0, atoi(parv[1]), 0);
      convertRGBRGBtoRRGGBB(image, *imd);
      delete[] image;
      
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
