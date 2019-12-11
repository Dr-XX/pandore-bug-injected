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
 * @author Laurent Quesnel - 2002-24-08
 * @author Regis Clouard - 2002-12-12 (GarbageCollector)
 */

#include <pandore.h>
using namespace pandore;

/**
 * @file pmumfordshahmerging.cpp
 * Mumford Shah Algorithm :
 *
 * Le critere de fusion est base sur le calcul de la variation d'energie :
 *
 * delta_energie = energie(R1 U R2) - (energie(R1)+energie(R2))
 * ou l'energie est celle de Mumford Shah
 *
 * d'ou delta_energie=Card(R1)*Card(R2)/(Card(R1)+Card(R2))*(moy(R1)-moy(R2))^2
 *                    - 2*alpha*longueur_frontiere(R1, R2).
 *
 * alpha : parametre
 *
 * On fait une fusion prioritaire: on commence par ceux qui offrent
 * la plus forte reduction d'energie
 */

struct Arete {
   Long ex1;
   Long ex2;
};

/*
 * A MODIFIER SELON LE CRITERE
 */
struct Region {
   Long no;        // number of the region in rg_dest
   double sum;     // sum of pixels in the region
   Long nb;        // number of points in the region
   Long no_node;   // number of node in graph
};

/*
 * Initialisation de la lut
 */
void initialisation( Region *lut, Long nb_lut ) {
   for (int a=0; a < nb_lut; a++) {
      lut[a].no=lut[a].no_node=a;
      lut[a].sum=0.0; 
      lut[a].nb=0;
   }
}

/**
 * Modification de la lut
 */
void modification( Region *lut, Long R12, Long R1, Long R2 ) {
   lut[R12].sum=lut[R1].sum+lut[R2].sum;
   lut[R12].nb=lut[R1].nb+lut[R2].nb;
}

/*
 * Permet d'eliminer du tas, toutes les aretes
 * entre deux regions qui sont maintenant fusionnees
 * de maniere a recuprer de la place.
 */
void GarbageCollector( Heap<Arete, float, int> &tas, Region *lut ) {
   Arete* tab = new Arete[tas.Nrank()];
   float* cle = new float[tas.Nrank()];
   Long i=0;
   while(!tas.Empty()) {
      tab[i]=tas.Pop(&cle[i]);
      Long R1=tab[i].ex1;
      Long R2=tab[i].ex2;
      
      // L'une des regions a deja ete fusionnee -> on passe.
      if (!((lut[R1].no!=R1) || (lut[R2].no!=R2))) i++;
   }

   for (--i;i>=0;--i) {
      tas.Push(tab[i], cle[i]);
   }
   delete tab;
}



/**
 * alcule la ponderation de l'arete entre i et j
 */
double valuation( const Graph2d &gi, Region *lut, Long i, Long j, double alpha ) {
   Long perimetre=0;
   GEdge *ptr;
   
   // on cherche la longueur de la frontiere entre i et j
   for (ptr=gi[lut[i].no_node]->Neighbours();ptr!=NULL;ptr=ptr->Next()) {
      if (ptr->Node() == lut[j].no_node)
	 perimetre = (Long)(ptr->weight-1); // on retire 1 car le poids des arcs sont initialises a 1
   } 
   
   // calcul de la variation d'energie
   double moy_1=(double)lut[j].sum/lut[j].nb;
   double moy_2=(double)lut[i].sum/lut[i].nb;
   
   double tre=(double)SQR(moy_1-moy_2);
   double delta_energie = (double)(lut[j].nb*lut[i].nb)/(lut[j].nb+lut[i].nb)*tre - 2*alpha*perimetre;
   
   return delta_energie;
}
/**
 * alcule la ponderation de l'arete entre i et j
 */
double valuation( const Graph3d &gi, Region *lut, Long i, Long j, double alpha ) {
   Long perimetre=0;
   GEdge *ptr;
   
   // on cherche la longueur de la frontiere entre i et j
   for (ptr=gi[lut[i].no_node]->Neighbours();ptr!=NULL;ptr=ptr->Next()) {
      if (ptr->Node() == lut[j].no_node)
	 perimetre = (Long)(ptr->weight-1); // on retire 1 car le poids des arcs sont initialises a 1
   } 
   
   // calcul de la variation d'energie
   double moy_1=(double)lut[j].sum/lut[j].nb;
   double moy_2=(double)lut[i].sum/lut[i].nb;
   
   double tre=(double)SQR(moy_1-moy_2);
   double delta_energie = (double)(lut[j].nb*lut[i].nb)/(lut[j].nb+lut[i].nb)*tre - 2*alpha*perimetre;
   
   return delta_energie;
}
/**
 * Remplissage de la somme et du nombre de pixels des regions
 * et du perimetre avec chacun de ses voisins.
 * ATTENTION: Les arcs sont intialises a 1, donc le perimetre
 * est trop long de 1 pixel.
 */
void remplissage( const Reg2d &rgs, Graph2d &gi, const Img2duc &ims, Region *lut ) {
   Point2d p;

   for (p.y = 0; p.y < rgs.Height(); p.y++)
   for (p.x = 0; p.x < rgs.Width(); p.x++)
   {
      // on remplit la lut
      lut[rgs[p]].sum += (double)ims[p];
      lut[rgs[p]].nb++;
      // on remplit le graphe gi
      for (int v=0; v<4; v++) {
	 if (rgs.Hold(p+v4[v]) && rgs[p]>0 && rgs[p]!=rgs[p+v4[v]] && rgs[p+v4[v]] >0)
	    gi.Link(rgs[p], rgs[p+v4[v]], 0.5F, true); // 0.5 car on compte deux fois la frontiere (en effet, on liste tout les points)
      }

   }
}

/**
 * Fonction operateur.
 */
Errc PMumfordshahMerging( const Reg2d &rgs, const Graph2d &gs, const Img2duc &ims, Reg2d &rgd, Graph2d &gd, Long number, double alpha, float threshold ) {
 Region *lut;  
   GEdge *ptr_1, *ptr_3;
   double min_0=0;
   Arete tmp;
   Long nb_lut=(rgs.Labels()+1);
   Graph2d gi;
   Point2d p;
   int i;
   Heap <Arete, float, int> tas(26*gs.Size());
   
   // supprimer le controle du nombre d'iterations
   if (number<0) 
      number=MAXLONG;
   
   gd=gs;
   gi=gs;
   rgd.Labels(rgs.Labels());
   
   // 1ere PARTIE
   // initialization of array
   lut = new Region [2*nb_lut-1];
   initialisation(lut, 2*nb_lut-1);
   remplissage(rgs, gi, ims, lut);

   // on calcule la valuation des elements du tas, si valuation <= threshold alors on empile
   for (i=1;i<gs.Size();i++) {
      // si le noeud n'existe pas, on passe
      if (gs[i] == NULL) {
	 continue;
      }
      for (ptr_1=gs[i]->Neighbours();ptr_1!=NULL;ptr_1=ptr_1->Next()) {
	 min_0=valuation(gi, lut, i, ptr_1->Node(), alpha);
	 // si la variation d'energie est inferieur au threshold on fusionne
	 if (min_0<=threshold) {
	    tmp.ex1=i;
	    tmp.ex2=ptr_1->Node();
	    tas.Push(tmp, (float)min_0);
	 }
      }
   } // fin du parcours des noeuds du graphe

   // 2eme PARTIE
   // fusion de region
   Long nbre=0;
   while (nbre<number && !tas.Empty()) {
      // On fusionne l'arc minimum du graphe precedent
      // s'il repond au critere        
      tmp=tas.Pop();
      Long R1=tmp.ex1;
      Long R2=tmp.ex2;
      
      // L'une des regions a deja ete fusionnee -> on passe.
      if ((lut[R1].no!=R1) || (lut[R2].no!=R2)) continue;
      
      // on ne conserve que le sommet de numero minimum
      Long G1=MIN(lut[R1].no_node, lut[R2].no_node);
      Long G2=MAX(lut[R1].no_node, lut[R2].no_node);
      
      // on incremente le nombre de regions fusionnees
      // on met a jour la lut.
      Long R12=nb_lut+ ++nbre;
      modification(lut, R12, R1, R2);
      lut[R12].no_node=G1;
      lut[R12].no=R12;
      lut[R2].no_node=G1;
      lut[G1].no=R12;
      lut[R1].no=R12;
      lut[R2].no=R12;
      
      // on met le graphe a jour
      gd[G1]->seed=(gd[G1]->seed+gd[G2]->seed)/2;
      gd.Merge(G1, G2); // G1 reste, G2 disparait.
      gi.Merge(G1, G2); // Merge additionne les valeurs des arcs communs.
      
      for (ptr_3=gd[G1]->Neighbours();ptr_3!=NULL;ptr_3=ptr_3->Next()) {
	 min_0=valuation(gi, lut, R12, lut[ptr_3->Node()].no, alpha);
	 // si la variation d'energie est inferieur au threshold on fusionne
	 // on met le tas a jour
	 if (min_0<=threshold) {
	    tmp.ex1=R12;
	    tmp.ex2=lut[ptr_3->Node()].no;
	    tas.Push(tmp, (float)min_0);
	    if (tas.Full()) GarbageCollector(tas, lut);
	 }
      }
   } // fin du while
   
   // 3eme PARTIE
   // Minimisation des etiquettes.
   for (i = 1; i < (nb_lut+nbre + 1); i++) {
      int j = i;
      while (lut[j].no != j) {
	 j=lut[j].no;
      }
      lut[i].no = lut[j].no_node;
   }
   
   for (p.y = 0; p.y < rgs.Height(); p.y++)
   for (p.x = 0; p.x < rgs.Width(); p.x++)
   {  
      // fusion dans la carte de region
      // pour la region qui correspond au voisin qui a fusionne, on donne le Label du 1er sommet
      rgd[p] = lut[rgs[p]].no;
   }
   
   gi.Delete();
   delete [] lut;
   
   return nbre; // retourne le nombre de regions fusionnees
}


/**
 * Remplissage de la somme et du nombre de pixels des regions
 * et du perimetre avec chacun de ses voisins.
 * ATTENTION: Les arcs sont intialises a 1, donc le perimetre
 * est trop long de 1 pixel.
 */
void remplissage( const Reg2d &rgs, Graph2d &gi, const Img2dsl &ims, Region *lut ) {
   Point2d p;

   for (p.y = 0; p.y < rgs.Height(); p.y++)
   for (p.x = 0; p.x < rgs.Width(); p.x++)
   {
      // on remplit la lut
      lut[rgs[p]].sum += (double)ims[p];
      lut[rgs[p]].nb++;
      // on remplit le graphe gi
      for (int v=0; v<4; v++) {
	 if (rgs.Hold(p+v4[v]) && rgs[p]>0 && rgs[p]!=rgs[p+v4[v]] && rgs[p+v4[v]] >0)
	    gi.Link(rgs[p], rgs[p+v4[v]], 0.5F, true); // 0.5 car on compte deux fois la frontiere (en effet, on liste tout les points)
      }

   }
}

/**
 * Fonction operateur.
 */
Errc PMumfordshahMerging( const Reg2d &rgs, const Graph2d &gs, const Img2dsl &ims, Reg2d &rgd, Graph2d &gd, Long number, double alpha, float threshold ) {
 Region *lut;  
   GEdge *ptr_1, *ptr_3;
   double min_0=0;
   Arete tmp;
   Long nb_lut=(rgs.Labels()+1);
   Graph2d gi;
   Point2d p;
   int i;
   Heap <Arete, float, int> tas(26*gs.Size());
   
   // supprimer le controle du nombre d'iterations
   if (number<0) 
      number=MAXLONG;
   
   gd=gs;
   gi=gs;
   rgd.Labels(rgs.Labels());
   
   // 1ere PARTIE
   // initialization of array
   lut = new Region [2*nb_lut-1];
   initialisation(lut, 2*nb_lut-1);
   remplissage(rgs, gi, ims, lut);

   // on calcule la valuation des elements du tas, si valuation <= threshold alors on empile
   for (i=1;i<gs.Size();i++) {
      // si le noeud n'existe pas, on passe
      if (gs[i] == NULL) {
	 continue;
      }
      for (ptr_1=gs[i]->Neighbours();ptr_1!=NULL;ptr_1=ptr_1->Next()) {
	 min_0=valuation(gi, lut, i, ptr_1->Node(), alpha);
	 // si la variation d'energie est inferieur au threshold on fusionne
	 if (min_0<=threshold) {
	    tmp.ex1=i;
	    tmp.ex2=ptr_1->Node();
	    tas.Push(tmp, (float)min_0);
	 }
      }
   } // fin du parcours des noeuds du graphe

   // 2eme PARTIE
   // fusion de region
   Long nbre=0;
   while (nbre<number && !tas.Empty()) {
      // On fusionne l'arc minimum du graphe precedent
      // s'il repond au critere        
      tmp=tas.Pop();
      Long R1=tmp.ex1;
      Long R2=tmp.ex2;
      
      // L'une des regions a deja ete fusionnee -> on passe.
      if ((lut[R1].no!=R1) || (lut[R2].no!=R2)) continue;
      
      // on ne conserve que le sommet de numero minimum
      Long G1=MIN(lut[R1].no_node, lut[R2].no_node);
      Long G2=MAX(lut[R1].no_node, lut[R2].no_node);
      
      // on incremente le nombre de regions fusionnees
      // on met a jour la lut.
      Long R12=nb_lut+ ++nbre;
      modification(lut, R12, R1, R2);
      lut[R12].no_node=G1;
      lut[R12].no=R12;
      lut[R2].no_node=G1;
      lut[G1].no=R12;
      lut[R1].no=R12;
      lut[R2].no=R12;
      
      // on met le graphe a jour
      gd[G1]->seed=(gd[G1]->seed+gd[G2]->seed)/2;
      gd.Merge(G1, G2); // G1 reste, G2 disparait.
      gi.Merge(G1, G2); // Merge additionne les valeurs des arcs communs.
      
      for (ptr_3=gd[G1]->Neighbours();ptr_3!=NULL;ptr_3=ptr_3->Next()) {
	 min_0=valuation(gi, lut, R12, lut[ptr_3->Node()].no, alpha);
	 // si la variation d'energie est inferieur au threshold on fusionne
	 // on met le tas a jour
	 if (min_0<=threshold) {
	    tmp.ex1=R12;
	    tmp.ex2=lut[ptr_3->Node()].no;
	    tas.Push(tmp, (float)min_0);
	    if (tas.Full()) GarbageCollector(tas, lut);
	 }
      }
   } // fin du while
   
   // 3eme PARTIE
   // Minimisation des etiquettes.
   for (i = 1; i < (nb_lut+nbre + 1); i++) {
      int j = i;
      while (lut[j].no != j) {
	 j=lut[j].no;
      }
      lut[i].no = lut[j].no_node;
   }
   
   for (p.y = 0; p.y < rgs.Height(); p.y++)
   for (p.x = 0; p.x < rgs.Width(); p.x++)
   {  
      // fusion dans la carte de region
      // pour la region qui correspond au voisin qui a fusionne, on donne le Label du 1er sommet
      rgd[p] = lut[rgs[p]].no;
   }
   
   gi.Delete();
   delete [] lut;
   
   return nbre; // retourne le nombre de regions fusionnees
}


/**
 * Remplissage de la somme et du nombre de pixels des regions
 * et du perimetre avec chacun de ses voisins.
 * ATTENTION: Les arcs sont intialises a 1, donc le perimetre
 * est trop long de 1 pixel.
 */
void remplissage( const Reg2d &rgs, Graph2d &gi, const Img2dsf &ims, Region *lut ) {
   Point2d p;

   for (p.y = 0; p.y < rgs.Height(); p.y++)
   for (p.x = 0; p.x < rgs.Width(); p.x++)
   {
      // on remplit la lut
      lut[rgs[p]].sum += (double)ims[p];
      lut[rgs[p]].nb++;
      // on remplit le graphe gi
      for (int v=0; v<4; v++) {
	 if (rgs.Hold(p+v4[v]) && rgs[p]>0 && rgs[p]!=rgs[p+v4[v]] && rgs[p+v4[v]] >0)
	    gi.Link(rgs[p], rgs[p+v4[v]], 0.5F, true); // 0.5 car on compte deux fois la frontiere (en effet, on liste tout les points)
      }

   }
}

/**
 * Fonction operateur.
 */
Errc PMumfordshahMerging( const Reg2d &rgs, const Graph2d &gs, const Img2dsf &ims, Reg2d &rgd, Graph2d &gd, Long number, double alpha, float threshold ) {
 Region *lut;  
   GEdge *ptr_1, *ptr_3;
   double min_0=0;
   Arete tmp;
   Long nb_lut=(rgs.Labels()+1);
   Graph2d gi;
   Point2d p;
   int i;
   Heap <Arete, float, int> tas(26*gs.Size());
   
   // supprimer le controle du nombre d'iterations
   if (number<0) 
      number=MAXLONG;
   
   gd=gs;
   gi=gs;
   rgd.Labels(rgs.Labels());
   
   // 1ere PARTIE
   // initialization of array
   lut = new Region [2*nb_lut-1];
   initialisation(lut, 2*nb_lut-1);
   remplissage(rgs, gi, ims, lut);

   // on calcule la valuation des elements du tas, si valuation <= threshold alors on empile
   for (i=1;i<gs.Size();i++) {
      // si le noeud n'existe pas, on passe
      if (gs[i] == NULL) {
	 continue;
      }
      for (ptr_1=gs[i]->Neighbours();ptr_1!=NULL;ptr_1=ptr_1->Next()) {
	 min_0=valuation(gi, lut, i, ptr_1->Node(), alpha);
	 // si la variation d'energie est inferieur au threshold on fusionne
	 if (min_0<=threshold) {
	    tmp.ex1=i;
	    tmp.ex2=ptr_1->Node();
	    tas.Push(tmp, (float)min_0);
	 }
      }
   } // fin du parcours des noeuds du graphe

   // 2eme PARTIE
   // fusion de region
   Long nbre=0;
   while (nbre<number && !tas.Empty()) {
      // On fusionne l'arc minimum du graphe precedent
      // s'il repond au critere        
      tmp=tas.Pop();
      Long R1=tmp.ex1;
      Long R2=tmp.ex2;
      
      // L'une des regions a deja ete fusionnee -> on passe.
      if ((lut[R1].no!=R1) || (lut[R2].no!=R2)) continue;
      
      // on ne conserve que le sommet de numero minimum
      Long G1=MIN(lut[R1].no_node, lut[R2].no_node);
      Long G2=MAX(lut[R1].no_node, lut[R2].no_node);
      
      // on incremente le nombre de regions fusionnees
      // on met a jour la lut.
      Long R12=nb_lut+ ++nbre;
      modification(lut, R12, R1, R2);
      lut[R12].no_node=G1;
      lut[R12].no=R12;
      lut[R2].no_node=G1;
      lut[G1].no=R12;
      lut[R1].no=R12;
      lut[R2].no=R12;
      
      // on met le graphe a jour
      gd[G1]->seed=(gd[G1]->seed+gd[G2]->seed)/2;
      gd.Merge(G1, G2); // G1 reste, G2 disparait.
      gi.Merge(G1, G2); // Merge additionne les valeurs des arcs communs.
      
      for (ptr_3=gd[G1]->Neighbours();ptr_3!=NULL;ptr_3=ptr_3->Next()) {
	 min_0=valuation(gi, lut, R12, lut[ptr_3->Node()].no, alpha);
	 // si la variation d'energie est inferieur au threshold on fusionne
	 // on met le tas a jour
	 if (min_0<=threshold) {
	    tmp.ex1=R12;
	    tmp.ex2=lut[ptr_3->Node()].no;
	    tas.Push(tmp, (float)min_0);
	    if (tas.Full()) GarbageCollector(tas, lut);
	 }
      }
   } // fin du while
   
   // 3eme PARTIE
   // Minimisation des etiquettes.
   for (i = 1; i < (nb_lut+nbre + 1); i++) {
      int j = i;
      while (lut[j].no != j) {
	 j=lut[j].no;
      }
      lut[i].no = lut[j].no_node;
   }
   
   for (p.y = 0; p.y < rgs.Height(); p.y++)
   for (p.x = 0; p.x < rgs.Width(); p.x++)
   {  
      // fusion dans la carte de region
      // pour la region qui correspond au voisin qui a fusionne, on donne le Label du 1er sommet
      rgd[p] = lut[rgs[p]].no;
   }
   
   gi.Delete();
   delete [] lut;
   
   return nbre; // retourne le nombre de regions fusionnees
}


/**
 * Remplissage de la somme et du nombre de pixels des regions
 * et du perimetre avec chacun de ses voisins.
 * ATTENTION: Les arcs sont intialises a 1, donc le perimetre
 * est trop long de 1 pixel.
 */
void remplissage( const Reg3d &rgs, Graph3d &gi, const Img3duc &ims, Region *lut ) {
   Point3d p;

   for (p.z = 0; p.z < rgs.Depth(); p.z++)
   for (p.y = 0; p.y < rgs.Height(); p.y++)
   for (p.x = 0; p.x < rgs.Width(); p.x++)
   {
      // on remplit la lut
      lut[rgs[p]].sum += (double)ims[p];
      lut[rgs[p]].nb++;
      // on remplit le graphe gi
      for (int v=0; v<6; v++) {
	 if (rgs.Hold(p+v6[v]) && rgs[p]>0 && rgs[p]!=rgs[p+v6[v]] && rgs[p+v6[v]] >0)
	    gi.Link(rgs[p], rgs[p+v6[v]], 0.5F, true); // 0.5 car on compte deux fois la frontiere (en effet, on liste tout les points)
      }

   }
}

/**
 * Fonction operateur.
 */
Errc PMumfordshahMerging( const Reg3d &rgs, const Graph3d &gs, const Img3duc &ims, Reg3d &rgd, Graph3d &gd, Long number, double alpha, float threshold ) {
 Region *lut;  
   GEdge *ptr_1, *ptr_3;
   double min_0=0;
   Arete tmp;
   Long nb_lut=(rgs.Labels()+1);
   Graph3d gi;
   Point3d p;
   int i;
   Heap <Arete, float, int> tas(26*gs.Size());
   
   // supprimer le controle du nombre d'iterations
   if (number<0) 
      number=MAXLONG;
   
   gd=gs;
   gi=gs;
   rgd.Labels(rgs.Labels());
   
   // 1ere PARTIE
   // initialization of array
   lut = new Region [2*nb_lut-1];
   initialisation(lut, 2*nb_lut-1);
   remplissage(rgs, gi, ims, lut);

   // on calcule la valuation des elements du tas, si valuation <= threshold alors on empile
   for (i=1;i<gs.Size();i++) {
      // si le noeud n'existe pas, on passe
      if (gs[i] == NULL) {
	 continue;
      }
      for (ptr_1=gs[i]->Neighbours();ptr_1!=NULL;ptr_1=ptr_1->Next()) {
	 min_0=valuation(gi, lut, i, ptr_1->Node(), alpha);
	 // si la variation d'energie est inferieur au threshold on fusionne
	 if (min_0<=threshold) {
	    tmp.ex1=i;
	    tmp.ex2=ptr_1->Node();
	    tas.Push(tmp, (float)min_0);
	 }
      }
   } // fin du parcours des noeuds du graphe

   // 2eme PARTIE
   // fusion de region
   Long nbre=0;
   while (nbre<number && !tas.Empty()) {
      // On fusionne l'arc minimum du graphe precedent
      // s'il repond au critere        
      tmp=tas.Pop();
      Long R1=tmp.ex1;
      Long R2=tmp.ex2;
      
      // L'une des regions a deja ete fusionnee -> on passe.
      if ((lut[R1].no!=R1) || (lut[R2].no!=R2)) continue;
      
      // on ne conserve que le sommet de numero minimum
      Long G1=MIN(lut[R1].no_node, lut[R2].no_node);
      Long G2=MAX(lut[R1].no_node, lut[R2].no_node);
      
      // on incremente le nombre de regions fusionnees
      // on met a jour la lut.
      Long R12=nb_lut+ ++nbre;
      modification(lut, R12, R1, R2);
      lut[R12].no_node=G1;
      lut[R12].no=R12;
      lut[R2].no_node=G1;
      lut[G1].no=R12;
      lut[R1].no=R12;
      lut[R2].no=R12;
      
      // on met le graphe a jour
      gd[G1]->seed=(gd[G1]->seed+gd[G2]->seed)/2;
      gd.Merge(G1, G2); // G1 reste, G2 disparait.
      gi.Merge(G1, G2); // Merge additionne les valeurs des arcs communs.
      
      for (ptr_3=gd[G1]->Neighbours();ptr_3!=NULL;ptr_3=ptr_3->Next()) {
	 min_0=valuation(gi, lut, R12, lut[ptr_3->Node()].no, alpha);
	 // si la variation d'energie est inferieur au threshold on fusionne
	 // on met le tas a jour
	 if (min_0<=threshold) {
	    tmp.ex1=R12;
	    tmp.ex2=lut[ptr_3->Node()].no;
	    tas.Push(tmp, (float)min_0);
	    if (tas.Full()) GarbageCollector(tas, lut);
	 }
      }
   } // fin du while
   
   // 3eme PARTIE
   // Minimisation des etiquettes.
   for (i = 1; i < (nb_lut+nbre + 1); i++) {
      int j = i;
      while (lut[j].no != j) {
	 j=lut[j].no;
      }
      lut[i].no = lut[j].no_node;
   }
   
   for (p.z = 0; p.z < rgs.Depth(); p.z++)
   for (p.y = 0; p.y < rgs.Height(); p.y++)
   for (p.x = 0; p.x < rgs.Width(); p.x++)
   {  
      // fusion dans la carte de region
      // pour la region qui correspond au voisin qui a fusionne, on donne le Label du 1er sommet
      rgd[p] = lut[rgs[p]].no;
   }
   
   gi.Delete();
   delete [] lut;
   
   return nbre; // retourne le nombre de regions fusionnees
}


/**
 * Remplissage de la somme et du nombre de pixels des regions
 * et du perimetre avec chacun de ses voisins.
 * ATTENTION: Les arcs sont intialises a 1, donc le perimetre
 * est trop long de 1 pixel.
 */
void remplissage( const Reg3d &rgs, Graph3d &gi, const Img3dsl &ims, Region *lut ) {
   Point3d p;

   for (p.z = 0; p.z < rgs.Depth(); p.z++)
   for (p.y = 0; p.y < rgs.Height(); p.y++)
   for (p.x = 0; p.x < rgs.Width(); p.x++)
   {
      // on remplit la lut
      lut[rgs[p]].sum += (double)ims[p];
      lut[rgs[p]].nb++;
      // on remplit le graphe gi
      for (int v=0; v<6; v++) {
	 if (rgs.Hold(p+v6[v]) && rgs[p]>0 && rgs[p]!=rgs[p+v6[v]] && rgs[p+v6[v]] >0)
	    gi.Link(rgs[p], rgs[p+v6[v]], 0.5F, true); // 0.5 car on compte deux fois la frontiere (en effet, on liste tout les points)
      }

   }
}

/**
 * Fonction operateur.
 */
Errc PMumfordshahMerging( const Reg3d &rgs, const Graph3d &gs, const Img3dsl &ims, Reg3d &rgd, Graph3d &gd, Long number, double alpha, float threshold ) {
 Region *lut;  
   GEdge *ptr_1, *ptr_3;
   double min_0=0;
   Arete tmp;
   Long nb_lut=(rgs.Labels()+1);
   Graph3d gi;
   Point3d p;
   int i;
   Heap <Arete, float, int> tas(26*gs.Size());
   
   // supprimer le controle du nombre d'iterations
   if (number<0) 
      number=MAXLONG;
   
   gd=gs;
   gi=gs;
   rgd.Labels(rgs.Labels());
   
   // 1ere PARTIE
   // initialization of array
   lut = new Region [2*nb_lut-1];
   initialisation(lut, 2*nb_lut-1);
   remplissage(rgs, gi, ims, lut);

   // on calcule la valuation des elements du tas, si valuation <= threshold alors on empile
   for (i=1;i<gs.Size();i++) {
      // si le noeud n'existe pas, on passe
      if (gs[i] == NULL) {
	 continue;
      }
      for (ptr_1=gs[i]->Neighbours();ptr_1!=NULL;ptr_1=ptr_1->Next()) {
	 min_0=valuation(gi, lut, i, ptr_1->Node(), alpha);
	 // si la variation d'energie est inferieur au threshold on fusionne
	 if (min_0<=threshold) {
	    tmp.ex1=i;
	    tmp.ex2=ptr_1->Node();
	    tas.Push(tmp, (float)min_0);
	 }
      }
   } // fin du parcours des noeuds du graphe

   // 2eme PARTIE
   // fusion de region
   Long nbre=0;
   while (nbre<number && !tas.Empty()) {
      // On fusionne l'arc minimum du graphe precedent
      // s'il repond au critere        
      tmp=tas.Pop();
      Long R1=tmp.ex1;
      Long R2=tmp.ex2;
      
      // L'une des regions a deja ete fusionnee -> on passe.
      if ((lut[R1].no!=R1) || (lut[R2].no!=R2)) continue;
      
      // on ne conserve que le sommet de numero minimum
      Long G1=MIN(lut[R1].no_node, lut[R2].no_node);
      Long G2=MAX(lut[R1].no_node, lut[R2].no_node);
      
      // on incremente le nombre de regions fusionnees
      // on met a jour la lut.
      Long R12=nb_lut+ ++nbre;
      modification(lut, R12, R1, R2);
      lut[R12].no_node=G1;
      lut[R12].no=R12;
      lut[R2].no_node=G1;
      lut[G1].no=R12;
      lut[R1].no=R12;
      lut[R2].no=R12;
      
      // on met le graphe a jour
      gd[G1]->seed=(gd[G1]->seed+gd[G2]->seed)/2;
      gd.Merge(G1, G2); // G1 reste, G2 disparait.
      gi.Merge(G1, G2); // Merge additionne les valeurs des arcs communs.
      
      for (ptr_3=gd[G1]->Neighbours();ptr_3!=NULL;ptr_3=ptr_3->Next()) {
	 min_0=valuation(gi, lut, R12, lut[ptr_3->Node()].no, alpha);
	 // si la variation d'energie est inferieur au threshold on fusionne
	 // on met le tas a jour
	 if (min_0<=threshold) {
	    tmp.ex1=R12;
	    tmp.ex2=lut[ptr_3->Node()].no;
	    tas.Push(tmp, (float)min_0);
	    if (tas.Full()) GarbageCollector(tas, lut);
	 }
      }
   } // fin du while
   
   // 3eme PARTIE
   // Minimisation des etiquettes.
   for (i = 1; i < (nb_lut+nbre + 1); i++) {
      int j = i;
      while (lut[j].no != j) {
	 j=lut[j].no;
      }
      lut[i].no = lut[j].no_node;
   }
   
   for (p.z = 0; p.z < rgs.Depth(); p.z++)
   for (p.y = 0; p.y < rgs.Height(); p.y++)
   for (p.x = 0; p.x < rgs.Width(); p.x++)
   {  
      // fusion dans la carte de region
      // pour la region qui correspond au voisin qui a fusionne, on donne le Label du 1er sommet
      rgd[p] = lut[rgs[p]].no;
   }
   
   gi.Delete();
   delete [] lut;
   
   return nbre; // retourne le nombre de regions fusionnees
}


/**
 * Remplissage de la somme et du nombre de pixels des regions
 * et du perimetre avec chacun de ses voisins.
 * ATTENTION: Les arcs sont intialises a 1, donc le perimetre
 * est trop long de 1 pixel.
 */
void remplissage( const Reg3d &rgs, Graph3d &gi, const Img3dsf &ims, Region *lut ) {
   Point3d p;

   for (p.z = 0; p.z < rgs.Depth(); p.z++)
   for (p.y = 0; p.y < rgs.Height(); p.y++)
   for (p.x = 0; p.x < rgs.Width(); p.x++)
   {
      // on remplit la lut
      lut[rgs[p]].sum += (double)ims[p];
      lut[rgs[p]].nb++;
      // on remplit le graphe gi
      for (int v=0; v<6; v++) {
	 if (rgs.Hold(p+v6[v]) && rgs[p]>0 && rgs[p]!=rgs[p+v6[v]] && rgs[p+v6[v]] >0)
	    gi.Link(rgs[p], rgs[p+v6[v]], 0.5F, true); // 0.5 car on compte deux fois la frontiere (en effet, on liste tout les points)
      }

   }
}

/**
 * Fonction operateur.
 */
Errc PMumfordshahMerging( const Reg3d &rgs, const Graph3d &gs, const Img3dsf &ims, Reg3d &rgd, Graph3d &gd, Long number, double alpha, float threshold ) {
 Region *lut;  
   GEdge *ptr_1, *ptr_3;
   double min_0=0;
   Arete tmp;
   Long nb_lut=(rgs.Labels()+1);
   Graph3d gi;
   Point3d p;
   int i;
   Heap <Arete, float, int> tas(26*gs.Size());
   
   // supprimer le controle du nombre d'iterations
   if (number<0) 
      number=MAXLONG;
   
   gd=gs;
   gi=gs;
   rgd.Labels(rgs.Labels());
   
   // 1ere PARTIE
   // initialization of array
   lut = new Region [2*nb_lut-1];
   initialisation(lut, 2*nb_lut-1);
   remplissage(rgs, gi, ims, lut);

   // on calcule la valuation des elements du tas, si valuation <= threshold alors on empile
   for (i=1;i<gs.Size();i++) {
      // si le noeud n'existe pas, on passe
      if (gs[i] == NULL) {
	 continue;
      }
      for (ptr_1=gs[i]->Neighbours();ptr_1!=NULL;ptr_1=ptr_1->Next()) {
	 min_0=valuation(gi, lut, i, ptr_1->Node(), alpha);
	 // si la variation d'energie est inferieur au threshold on fusionne
	 if (min_0<=threshold) {
	    tmp.ex1=i;
	    tmp.ex2=ptr_1->Node();
	    tas.Push(tmp, (float)min_0);
	 }
      }
   } // fin du parcours des noeuds du graphe

   // 2eme PARTIE
   // fusion de region
   Long nbre=0;
   while (nbre<number && !tas.Empty()) {
      // On fusionne l'arc minimum du graphe precedent
      // s'il repond au critere        
      tmp=tas.Pop();
      Long R1=tmp.ex1;
      Long R2=tmp.ex2;
      
      // L'une des regions a deja ete fusionnee -> on passe.
      if ((lut[R1].no!=R1) || (lut[R2].no!=R2)) continue;
      
      // on ne conserve que le sommet de numero minimum
      Long G1=MIN(lut[R1].no_node, lut[R2].no_node);
      Long G2=MAX(lut[R1].no_node, lut[R2].no_node);
      
      // on incremente le nombre de regions fusionnees
      // on met a jour la lut.
      Long R12=nb_lut+ ++nbre;
      modification(lut, R12, R1, R2);
      lut[R12].no_node=G1;
      lut[R12].no=R12;
      lut[R2].no_node=G1;
      lut[G1].no=R12;
      lut[R1].no=R12;
      lut[R2].no=R12;
      
      // on met le graphe a jour
      gd[G1]->seed=(gd[G1]->seed+gd[G2]->seed)/2;
      gd.Merge(G1, G2); // G1 reste, G2 disparait.
      gi.Merge(G1, G2); // Merge additionne les valeurs des arcs communs.
      
      for (ptr_3=gd[G1]->Neighbours();ptr_3!=NULL;ptr_3=ptr_3->Next()) {
	 min_0=valuation(gi, lut, R12, lut[ptr_3->Node()].no, alpha);
	 // si la variation d'energie est inferieur au threshold on fusionne
	 // on met le tas a jour
	 if (min_0<=threshold) {
	    tmp.ex1=R12;
	    tmp.ex2=lut[ptr_3->Node()].no;
	    tas.Push(tmp, (float)min_0);
	    if (tas.Full()) GarbageCollector(tas, lut);
	 }
      }
   } // fin du while
   
   // 3eme PARTIE
   // Minimisation des etiquettes.
   for (i = 1; i < (nb_lut+nbre + 1); i++) {
      int j = i;
      while (lut[j].no != j) {
	 j=lut[j].no;
      }
      lut[i].no = lut[j].no_node;
   }
   
   for (p.z = 0; p.z < rgs.Depth(); p.z++)
   for (p.y = 0; p.y < rgs.Height(); p.y++)
   for (p.x = 0; p.x < rgs.Width(); p.x++)
   {  
      // fusion dans la carte de region
      // pour la region qui correspond au voisin qui a fusionne, on donne le Label du 1er sommet
      rgd[p] = lut[rgs[p]].no;
   }
   
   gi.Delete();
   delete [] lut;
   
   return nbre; // retourne le nombre de regions fusionnees
}



#ifdef MAIN

/*
 * Modify only the following constants, and the operator switches.
 */
#define	USAGE	"usage: %s number alpha threshold [-m mask] [rg_in|-] [gr_in|-] [im_in|-] [rg_out|-] [gr_out|-]"
#define	PARC	3  // Number of parameters
#define	FINC	3  // Number of input images
#define	FOUTC	2  // Number of output images
#define	MASK	1  // Level of masking

int main( int argc, char *argv[] ) {
   Errc result;                // The result code of the execution.
   Pobject* mask;              // The region map.
   Pobject* objin[FINC + 1];   // The input objects.
   Pobject* objs[FINC + 1];    // The source objects masked.
   Pobject* objout[FOUTC + 1]; // The output object.
   Pobject* objd[FOUTC + 1];   // The result object of the execution.
   char* parv[PARC + 1];       // The input parameters.

   ReadArgs(argc, argv, PARC, FINC, FOUTC, &mask, objin, objs, objout, objd, parv, USAGE, MASK); 
// verifier toutes les entrees
   if (objs[0]->Type()==Po_Reg2d && objs[1]->Type()==Po_Graph2d && objs[2]->Type() == Po_Img2duc)  {
      Reg2d* const rgs=(Reg2d*)objs[0];  
      Graph2d* const gs=(Graph2d*)objs[1];  
      Img2duc* const ims=(Img2duc*)objs[2];
      objd[0]=new Reg2d(rgs->Size());
      Reg2d* const rgd=(Reg2d*)objd[0];
      objd[1]=new Graph2d(gs->Size());
      Graph2d* const gd=(Graph2d*)objd[1];
      
      result = PMumfordshahMerging(*rgs, *gs, *ims, *rgd, *gd, (Long)atoi(parv[0]), (float)atof(parv[1]), (float)atof(parv[2]));
      goto end;
   }
// verifier toutes les entrees
   if (objs[0]->Type()==Po_Reg2d && objs[1]->Type()==Po_Graph2d && objs[2]->Type() == Po_Img2dsl)  {
      Reg2d* const rgs=(Reg2d*)objs[0];  
      Graph2d* const gs=(Graph2d*)objs[1];  
      Img2dsl* const ims=(Img2dsl*)objs[2];
      objd[0]=new Reg2d(rgs->Size());
      Reg2d* const rgd=(Reg2d*)objd[0];
      objd[1]=new Graph2d(gs->Size());
      Graph2d* const gd=(Graph2d*)objd[1];
      
      result = PMumfordshahMerging(*rgs, *gs, *ims, *rgd, *gd, (Long)atoi(parv[0]), (float)atof(parv[1]), (float)atof(parv[2]));
      goto end;
   }
// verifier toutes les entrees
   if (objs[0]->Type()==Po_Reg2d && objs[1]->Type()==Po_Graph2d && objs[2]->Type() == Po_Img2dsf)  {
      Reg2d* const rgs=(Reg2d*)objs[0];  
      Graph2d* const gs=(Graph2d*)objs[1];  
      Img2dsf* const ims=(Img2dsf*)objs[2];
      objd[0]=new Reg2d(rgs->Size());
      Reg2d* const rgd=(Reg2d*)objd[0];
      objd[1]=new Graph2d(gs->Size());
      Graph2d* const gd=(Graph2d*)objd[1];
      
      result = PMumfordshahMerging(*rgs, *gs, *ims, *rgd, *gd, (Long)atoi(parv[0]), (float)atof(parv[1]), (float)atof(parv[2]));
      goto end;
   }
// verifier toutes les entrees
   if (objs[0]->Type()==Po_Reg3d && objs[1]->Type()==Po_Graph3d && objs[2]->Type() == Po_Img3duc)  {
      Reg3d* const rgs=(Reg3d*)objs[0];  
      Graph3d* const gs=(Graph3d*)objs[1];  
      Img3duc* const ims=(Img3duc*)objs[2];
      objd[0]=new Reg3d(rgs->Size());
      Reg3d* const rgd=(Reg3d*)objd[0];
      objd[1]=new Graph3d(gs->Size());
      Graph3d* const gd=(Graph3d*)objd[1];
      
      result = PMumfordshahMerging(*rgs, *gs, *ims, *rgd, *gd, (Long)atoi(parv[0]), (float)atof(parv[1]), (float)atof(parv[2]));
      goto end;
   }
// verifier toutes les entrees
   if (objs[0]->Type()==Po_Reg3d && objs[1]->Type()==Po_Graph3d && objs[2]->Type() == Po_Img3dsl)  {
      Reg3d* const rgs=(Reg3d*)objs[0];  
      Graph3d* const gs=(Graph3d*)objs[1];  
      Img3dsl* const ims=(Img3dsl*)objs[2];
      objd[0]=new Reg3d(rgs->Size());
      Reg3d* const rgd=(Reg3d*)objd[0];
      objd[1]=new Graph3d(gs->Size());
      Graph3d* const gd=(Graph3d*)objd[1];
      
      result = PMumfordshahMerging(*rgs, *gs, *ims, *rgd, *gd, (Long)atoi(parv[0]), (float)atof(parv[1]), (float)atof(parv[2]));
      goto end;
   }
// verifier toutes les entrees
   if (objs[0]->Type()==Po_Reg3d && objs[1]->Type()==Po_Graph3d && objs[2]->Type() == Po_Img3dsf)  {
      Reg3d* const rgs=(Reg3d*)objs[0];  
      Graph3d* const gs=(Graph3d*)objs[1];  
      Img3dsf* const ims=(Img3dsf*)objs[2];
      objd[0]=new Reg3d(rgs->Size());
      Reg3d* const rgd=(Reg3d*)objd[0];
      objd[1]=new Graph3d(gs->Size());
      Graph3d* const gd=(Graph3d*)objd[1];
      
      result = PMumfordshahMerging(*rgs, *gs, *ims, *rgd, *gd, (Long)atoi(parv[0]), (float)atof(parv[1]), (float)atof(parv[2]));
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



