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
 * @author Laurent Quesnel - 2002-07-08
 * @author Regis Clouard - 2002-12-12 (GarbageCollector)
 */

#include <pandore.h>
using namespace pandore;

/**
 * @file pmeanmerging.cpp
 * @brief Fusion de regions sur critere de moyenne interieure.
 */

struct Arete {
   Ulong ex1;
   Ulong ex2;
};

/*
 * A MODIFIER SELON LE CRITERE
 */

struct Region {
   Ulong no_node;
   Ulong no;
   double sum;
   Ulong nb;
};

/*
 * Calcules la ponderation de l'arete entre i et j.
 * A modifier selon le critere
 */
double valuation( Region *lut, int i, int j ) {
   double tmp=lut[i].sum/lut[i].nb-lut[j].sum/lut[j].nb;
   return ABS(tmp);
}

/*
 * Initialisation du tableau des caracteristiques.
 * A modifier selon le critere
 */
void initialisation( Region *lut, Ulong nb_lut ) {
   for (Ulong a=0; a < nb_lut; a++) {
      lut[a].no=lut[a].no_node=a;
      lut[a].sum=0.0F; 
      lut[a].nb=0;
   }
}

/*
 * Modification de la lut
 * A modifier selon le critere
 */
void modification( Region *lut, Ulong R12, Ulong R1, Ulong R2 ) {
   lut[R12].sum=lut[R1].sum+lut[R2].sum;
   lut[R12].nb=lut[R1].nb+lut[R2].nb;
}

/*
 * Permet d'eliminer du tas, toutes les aretes
 * entre deux regions qui sont maintenant fusionnees
 * de maniere a recuperer de la place.
 */
void GarbageCollector( Heap<Arete,float,int> &tas, Region *lut ) {
   Arete* tab = new Arete[tas.Nrank()];
   float* cle = new float[tas.Nrank()];
   Ulong i=0;
   while(!tas.Empty()) {
      tab[i]=tas.Pop(&cle[i]);
      Ulong R1=tab[i].ex1;
      Ulong R2=tab[i].ex2;
      
      // L'une des regions a deja ete fusionnee -> on passe.
      if (!((lut[R1].no!=R1) || (lut[R2].no!=R2))) i++;
   }

   for (Ulong j=0; j<i;j++)
      tas.Push(tab[j],cle[j]);
   delete tab;
}


/*
 * Remplissage de la lut avec les caracteristiques.
 * A modifier selon le critere
 */
void remplissage( const Reg2d &rgs, const Img2duc &ims, Region *lut ) {
   Img2duc::ValueType *p_ims = ims.Vector();
   Ulong *p_rgs = rgs.Vector();

   // remplissage du tableau de caracteristiques
   for (unsigned int d=0; d < rgs.VectorSize(); d++) {
      // on additionne la valeur du pixel dans la r�gion associ�e
      lut[p_rgs[d]].sum += (double) p_ims[d];
      // on incremente le nombre de pixel de cette region
      lut[p_rgs[d]].nb++;   
   }
}

/**
 * INVARIANT SELON LE CRITERE
 */
Errc PMeanMerging( const Reg2d &rgs, const Graph2d &gs, const Img2duc &ims, Reg2d &rgd, Graph2d &gd, Long number, float threshold ) {
 Region *lut;  
   Ulong *p_rgd = rgd.Vector();
   Ulong *p_rgs = rgs.Vector();
   GEdge *ptr_1, *ptr_3;
   double min_0=0;
   Arete tmp;
   Ulong nb_lut=(rgs.Labels()+1);
   int i;
   Heap <Arete,float,int> tas(26*gs.Size()); // Majorant du nombre d'aretes???

   // supprimer le controle du nombre d'iterations
   if (number<0)
      number=MAXLONG;

   gd=gs;

   // 1ere partie : Initialisation du tableau des caracteristiques.
   lut = new Region [2*nb_lut-1]; // 2*nb_lut-1 car ...
   initialisation(lut,2*nb_lut-1);
   remplissage(rgs,ims,lut);

   // on calcule la valuation des aretes, si valuation<=threshold alors on empile
   for (i=1;i<gd.Size();i++) {
      // si le noeud n'existe pas, on passe
      if (gd[i]==NULL) continue;
      for (ptr_1=gd[i]->Neighbours();ptr_1!=NULL;ptr_1=ptr_1->Next()) {
	 min_0=valuation(lut,i,ptr_1->Node());  
	 if (min_0<=threshold) {
	    tmp.ex1=i;
	    tmp.ex2=ptr_1->Node();
	    tas.Push(tmp,(float) (min_0));
	 }
      }
   } // fin du parcours des noeuds du graphe

   // 2eme partie : fusion
   Long nbre=0;
   while (nbre<number && !tas.Empty()) {
      // On fusionne l'arc minimum du graphe precedent
      // s'il repond au critere.
      tmp=tas.Pop();
      Ulong R1=tmp.ex1;
      Ulong R2=tmp.ex2;
      
      // L'une des regions a deja ete fusionnee -> on passe.
      if ((lut[R1].no!=R1) || (lut[R2].no!=R2)) continue;
      
      // on ne conserve que le sommet du numero minimum
      Ulong G1=MIN(lut[R1].no_node,lut[R2].no_node);
      Ulong G2=MAX(lut[R1].no_node,lut[R2].no_node);
      
      // on incremente le nombre de regions fusionnees
      // on met a jour la lut.
      Ulong R12=nb_lut+ ++nbre;
      modification(lut,R12,R1,R2);
      lut[R12].no_node=G1;
      lut[R12].no=R12;
      lut[R2].no_node=G1;
      lut[G1].no=R12;
      lut[R1].no=R12;
      lut[R2].no=R12;

      // on met le graphe a jour
      gd[G1]->seed=(gd[G1]->seed+gd[G2]->seed)/2;
      gd.Merge(G1,G2); // G1 reste, G2 disparait.
      for (ptr_3=gd[G1]->Neighbours();ptr_3!=NULL;ptr_3=ptr_3->Next()) {
	 // on met le tas a jour
	 min_0=valuation(lut,R12,lut[ptr_3->Node()].no);
	 if (min_0<=threshold) {
	    tmp.ex1=R12;
	    tmp.ex2=lut[ptr_3->Node()].no;
	    tas.Push(tmp,(float) (min_0));
	    if (tas.Full()) GarbageCollector(tas,lut);
	 }
      }
   } // fin du while

   // 3eme partie : reetiquetage des regions 

   // Minimisation des etiquettes.
   for (i = 1; i < (int)(nb_lut + nbre); i++) {
      Ulong j = i;
      while (lut[j].no!=j) {
	 j = lut[j].no;
      } 
      lut[i].no = lut[j].no_node;
   }
   // fusion dans la carte de region
   for (unsigned int f = 0; f < rgd.VectorSize(); f++) {
      // pour la region qui correspond au voisin qui a fusionne, on donne le Label du 1er sommet
      p_rgd[f] = (Ulong)lut[p_rgs[f]].no;
   }
   
   rgd.Labels(rgs.Labels());
   
   delete [] lut;
   
   return nbre; // retourne le nombre de regions fusionnees
}


/*
 * Remplissage de la lut avec les caracteristiques.
 * A modifier selon le critere
 */
void remplissage( const Reg2d &rgs, const Img2dsl &ims, Region *lut ) {
   Img2dsl::ValueType *p_ims = ims.Vector();
   Ulong *p_rgs = rgs.Vector();

   // remplissage du tableau de caracteristiques
   for (unsigned int d=0; d < rgs.VectorSize(); d++) {
      // on additionne la valeur du pixel dans la r�gion associ�e
      lut[p_rgs[d]].sum += (double) p_ims[d];
      // on incremente le nombre de pixel de cette region
      lut[p_rgs[d]].nb++;   
   }
}

/**
 * INVARIANT SELON LE CRITERE
 */
Errc PMeanMerging( const Reg2d &rgs, const Graph2d &gs, const Img2dsl &ims, Reg2d &rgd, Graph2d &gd, Long number, float threshold ) {
 Region *lut;  
   Ulong *p_rgd = rgd.Vector();
   Ulong *p_rgs = rgs.Vector();
   GEdge *ptr_1, *ptr_3;
   double min_0=0;
   Arete tmp;
   Ulong nb_lut=(rgs.Labels()+1);
   int i;
   Heap <Arete,float,int> tas(26*gs.Size()); // Majorant du nombre d'aretes???

   // supprimer le controle du nombre d'iterations
   if (number<0)
      number=MAXLONG;

   gd=gs;

   // 1ere partie : Initialisation du tableau des caracteristiques.
   lut = new Region [2*nb_lut-1]; // 2*nb_lut-1 car ...
   initialisation(lut,2*nb_lut-1);
   remplissage(rgs,ims,lut);

   // on calcule la valuation des aretes, si valuation<=threshold alors on empile
   for (i=1;i<gd.Size();i++) {
      // si le noeud n'existe pas, on passe
      if (gd[i]==NULL) continue;
      for (ptr_1=gd[i]->Neighbours();ptr_1!=NULL;ptr_1=ptr_1->Next()) {
	 min_0=valuation(lut,i,ptr_1->Node());  
	 if (min_0<=threshold) {
	    tmp.ex1=i;
	    tmp.ex2=ptr_1->Node();
	    tas.Push(tmp,(float) (min_0));
	 }
      }
   } // fin du parcours des noeuds du graphe

   // 2eme partie : fusion
   Long nbre=0;
   while (nbre<number && !tas.Empty()) {
      // On fusionne l'arc minimum du graphe precedent
      // s'il repond au critere.
      tmp=tas.Pop();
      Ulong R1=tmp.ex1;
      Ulong R2=tmp.ex2;
      
      // L'une des regions a deja ete fusionnee -> on passe.
      if ((lut[R1].no!=R1) || (lut[R2].no!=R2)) continue;
      
      // on ne conserve que le sommet du numero minimum
      Ulong G1=MIN(lut[R1].no_node,lut[R2].no_node);
      Ulong G2=MAX(lut[R1].no_node,lut[R2].no_node);
      
      // on incremente le nombre de regions fusionnees
      // on met a jour la lut.
      Ulong R12=nb_lut+ ++nbre;
      modification(lut,R12,R1,R2);
      lut[R12].no_node=G1;
      lut[R12].no=R12;
      lut[R2].no_node=G1;
      lut[G1].no=R12;
      lut[R1].no=R12;
      lut[R2].no=R12;

      // on met le graphe a jour
      gd[G1]->seed=(gd[G1]->seed+gd[G2]->seed)/2;
      gd.Merge(G1,G2); // G1 reste, G2 disparait.
      for (ptr_3=gd[G1]->Neighbours();ptr_3!=NULL;ptr_3=ptr_3->Next()) {
	 // on met le tas a jour
	 min_0=valuation(lut,R12,lut[ptr_3->Node()].no);
	 if (min_0<=threshold) {
	    tmp.ex1=R12;
	    tmp.ex2=lut[ptr_3->Node()].no;
	    tas.Push(tmp,(float) (min_0));
	    if (tas.Full()) GarbageCollector(tas,lut);
	 }
      }
   } // fin du while

   // 3eme partie : reetiquetage des regions 

   // Minimisation des etiquettes.
   for (i = 1; i < (int)(nb_lut + nbre); i++) {
      Ulong j = i;
      while (lut[j].no!=j) {
	 j = lut[j].no;
      } 
      lut[i].no = lut[j].no_node;
   }
   // fusion dans la carte de region
   for (unsigned int f = 0; f < rgd.VectorSize(); f++) {
      // pour la region qui correspond au voisin qui a fusionne, on donne le Label du 1er sommet
      p_rgd[f] = (Ulong)lut[p_rgs[f]].no;
   }
   
   rgd.Labels(rgs.Labels());
   
   delete [] lut;
   
   return nbre; // retourne le nombre de regions fusionnees
}


/*
 * Remplissage de la lut avec les caracteristiques.
 * A modifier selon le critere
 */
void remplissage( const Reg2d &rgs, const Img2dsf &ims, Region *lut ) {
   Img2dsf::ValueType *p_ims = ims.Vector();
   Ulong *p_rgs = rgs.Vector();

   // remplissage du tableau de caracteristiques
   for (unsigned int d=0; d < rgs.VectorSize(); d++) {
      // on additionne la valeur du pixel dans la r�gion associ�e
      lut[p_rgs[d]].sum += (double) p_ims[d];
      // on incremente le nombre de pixel de cette region
      lut[p_rgs[d]].nb++;   
   }
}

/**
 * INVARIANT SELON LE CRITERE
 */
Errc PMeanMerging( const Reg2d &rgs, const Graph2d &gs, const Img2dsf &ims, Reg2d &rgd, Graph2d &gd, Long number, float threshold ) {
 Region *lut;  
   Ulong *p_rgd = rgd.Vector();
   Ulong *p_rgs = rgs.Vector();
   GEdge *ptr_1, *ptr_3;
   double min_0=0;
   Arete tmp;
   Ulong nb_lut=(rgs.Labels()+1);
   int i;
   Heap <Arete,float,int> tas(26*gs.Size()); // Majorant du nombre d'aretes???

   // supprimer le controle du nombre d'iterations
   if (number<0)
      number=MAXLONG;

   gd=gs;

   // 1ere partie : Initialisation du tableau des caracteristiques.
   lut = new Region [2*nb_lut-1]; // 2*nb_lut-1 car ...
   initialisation(lut,2*nb_lut-1);
   remplissage(rgs,ims,lut);

   // on calcule la valuation des aretes, si valuation<=threshold alors on empile
   for (i=1;i<gd.Size();i++) {
      // si le noeud n'existe pas, on passe
      if (gd[i]==NULL) continue;
      for (ptr_1=gd[i]->Neighbours();ptr_1!=NULL;ptr_1=ptr_1->Next()) {
	 min_0=valuation(lut,i,ptr_1->Node());  
	 if (min_0<=threshold) {
	    tmp.ex1=i;
	    tmp.ex2=ptr_1->Node();
	    tas.Push(tmp,(float) (min_0));
	 }
      }
   } // fin du parcours des noeuds du graphe

   // 2eme partie : fusion
   Long nbre=0;
   while (nbre<number && !tas.Empty()) {
      // On fusionne l'arc minimum du graphe precedent
      // s'il repond au critere.
      tmp=tas.Pop();
      Ulong R1=tmp.ex1;
      Ulong R2=tmp.ex2;
      
      // L'une des regions a deja ete fusionnee -> on passe.
      if ((lut[R1].no!=R1) || (lut[R2].no!=R2)) continue;
      
      // on ne conserve que le sommet du numero minimum
      Ulong G1=MIN(lut[R1].no_node,lut[R2].no_node);
      Ulong G2=MAX(lut[R1].no_node,lut[R2].no_node);
      
      // on incremente le nombre de regions fusionnees
      // on met a jour la lut.
      Ulong R12=nb_lut+ ++nbre;
      modification(lut,R12,R1,R2);
      lut[R12].no_node=G1;
      lut[R12].no=R12;
      lut[R2].no_node=G1;
      lut[G1].no=R12;
      lut[R1].no=R12;
      lut[R2].no=R12;

      // on met le graphe a jour
      gd[G1]->seed=(gd[G1]->seed+gd[G2]->seed)/2;
      gd.Merge(G1,G2); // G1 reste, G2 disparait.
      for (ptr_3=gd[G1]->Neighbours();ptr_3!=NULL;ptr_3=ptr_3->Next()) {
	 // on met le tas a jour
	 min_0=valuation(lut,R12,lut[ptr_3->Node()].no);
	 if (min_0<=threshold) {
	    tmp.ex1=R12;
	    tmp.ex2=lut[ptr_3->Node()].no;
	    tas.Push(tmp,(float) (min_0));
	    if (tas.Full()) GarbageCollector(tas,lut);
	 }
      }
   } // fin du while

   // 3eme partie : reetiquetage des regions 

   // Minimisation des etiquettes.
   for (i = 1; i < (int)(nb_lut + nbre); i++) {
      Ulong j = i;
      while (lut[j].no!=j) {
	 j = lut[j].no;
      } 
      lut[i].no = lut[j].no_node;
   }
   // fusion dans la carte de region
   for (unsigned int f = 0; f < rgd.VectorSize(); f++) {
      // pour la region qui correspond au voisin qui a fusionne, on donne le Label du 1er sommet
      p_rgd[f] = (Ulong)lut[p_rgs[f]].no;
   }
   
   rgd.Labels(rgs.Labels());
   
   delete [] lut;
   
   return nbre; // retourne le nombre de regions fusionnees
}


/*
 * Remplissage de la lut avec les caracteristiques.
 * A modifier selon le critere
 */
void remplissage( const Reg3d &rgs, const Img3duc &ims, Region *lut ) {
   Img3duc::ValueType *p_ims = ims.Vector();
   Ulong *p_rgs = rgs.Vector();

   // remplissage du tableau de caracteristiques
   for (unsigned int d=0; d < rgs.VectorSize(); d++) {
      // on additionne la valeur du pixel dans la r�gion associ�e
      lut[p_rgs[d]].sum += (double) p_ims[d];
      // on incremente le nombre de pixel de cette region
      lut[p_rgs[d]].nb++;   
   }
}

/**
 * INVARIANT SELON LE CRITERE
 */
Errc PMeanMerging( const Reg3d &rgs, const Graph3d &gs, const Img3duc &ims, Reg3d &rgd, Graph3d &gd, Long number, float threshold ) {
 Region *lut;  
   Ulong *p_rgd = rgd.Vector();
   Ulong *p_rgs = rgs.Vector();
   GEdge *ptr_1, *ptr_3;
   double min_0=0;
   Arete tmp;
   Ulong nb_lut=(rgs.Labels()+1);
   int i;
   Heap <Arete,float,int> tas(26*gs.Size()); // Majorant du nombre d'aretes???

   // supprimer le controle du nombre d'iterations
   if (number<0)
      number=MAXLONG;

   gd=gs;

   // 1ere partie : Initialisation du tableau des caracteristiques.
   lut = new Region [2*nb_lut-1]; // 2*nb_lut-1 car ...
   initialisation(lut,2*nb_lut-1);
   remplissage(rgs,ims,lut);

   // on calcule la valuation des aretes, si valuation<=threshold alors on empile
   for (i=1;i<gd.Size();i++) {
      // si le noeud n'existe pas, on passe
      if (gd[i]==NULL) continue;
      for (ptr_1=gd[i]->Neighbours();ptr_1!=NULL;ptr_1=ptr_1->Next()) {
	 min_0=valuation(lut,i,ptr_1->Node());  
	 if (min_0<=threshold) {
	    tmp.ex1=i;
	    tmp.ex2=ptr_1->Node();
	    tas.Push(tmp,(float) (min_0));
	 }
      }
   } // fin du parcours des noeuds du graphe

   // 2eme partie : fusion
   Long nbre=0;
   while (nbre<number && !tas.Empty()) {
      // On fusionne l'arc minimum du graphe precedent
      // s'il repond au critere.
      tmp=tas.Pop();
      Ulong R1=tmp.ex1;
      Ulong R2=tmp.ex2;
      
      // L'une des regions a deja ete fusionnee -> on passe.
      if ((lut[R1].no!=R1) || (lut[R2].no!=R2)) continue;
      
      // on ne conserve que le sommet du numero minimum
      Ulong G1=MIN(lut[R1].no_node,lut[R2].no_node);
      Ulong G2=MAX(lut[R1].no_node,lut[R2].no_node);
      
      // on incremente le nombre de regions fusionnees
      // on met a jour la lut.
      Ulong R12=nb_lut+ ++nbre;
      modification(lut,R12,R1,R2);
      lut[R12].no_node=G1;
      lut[R12].no=R12;
      lut[R2].no_node=G1;
      lut[G1].no=R12;
      lut[R1].no=R12;
      lut[R2].no=R12;

      // on met le graphe a jour
      gd[G1]->seed=(gd[G1]->seed+gd[G2]->seed)/2;
      gd.Merge(G1,G2); // G1 reste, G2 disparait.
      for (ptr_3=gd[G1]->Neighbours();ptr_3!=NULL;ptr_3=ptr_3->Next()) {
	 // on met le tas a jour
	 min_0=valuation(lut,R12,lut[ptr_3->Node()].no);
	 if (min_0<=threshold) {
	    tmp.ex1=R12;
	    tmp.ex2=lut[ptr_3->Node()].no;
	    tas.Push(tmp,(float) (min_0));
	    if (tas.Full()) GarbageCollector(tas,lut);
	 }
      }
   } // fin du while

   // 3eme partie : reetiquetage des regions 

   // Minimisation des etiquettes.
   for (i = 1; i < (int)(nb_lut + nbre); i++) {
      Ulong j = i;
      while (lut[j].no!=j) {
	 j = lut[j].no;
      } 
      lut[i].no = lut[j].no_node;
   }
   // fusion dans la carte de region
   for (unsigned int f = 0; f < rgd.VectorSize(); f++) {
      // pour la region qui correspond au voisin qui a fusionne, on donne le Label du 1er sommet
      p_rgd[f] = (Ulong)lut[p_rgs[f]].no;
   }
   
   rgd.Labels(rgs.Labels());
   
   delete [] lut;
   
   return nbre; // retourne le nombre de regions fusionnees
}


/*
 * Remplissage de la lut avec les caracteristiques.
 * A modifier selon le critere
 */
void remplissage( const Reg3d &rgs, const Img3dsl &ims, Region *lut ) {
   Img3dsl::ValueType *p_ims = ims.Vector();
   Ulong *p_rgs = rgs.Vector();

   // remplissage du tableau de caracteristiques
   for (unsigned int d=0; d < rgs.VectorSize(); d++) {
      // on additionne la valeur du pixel dans la r�gion associ�e
      lut[p_rgs[d]].sum += (double) p_ims[d];
      // on incremente le nombre de pixel de cette region
      lut[p_rgs[d]].nb++;   
   }
}

/**
 * INVARIANT SELON LE CRITERE
 */
Errc PMeanMerging( const Reg3d &rgs, const Graph3d &gs, const Img3dsl &ims, Reg3d &rgd, Graph3d &gd, Long number, float threshold ) {
 Region *lut;  
   Ulong *p_rgd = rgd.Vector();
   Ulong *p_rgs = rgs.Vector();
   GEdge *ptr_1, *ptr_3;
   double min_0=0;
   Arete tmp;
   Ulong nb_lut=(rgs.Labels()+1);
   int i;
   Heap <Arete,float,int> tas(26*gs.Size()); // Majorant du nombre d'aretes???

   // supprimer le controle du nombre d'iterations
   if (number<0)
      number=MAXLONG;

   gd=gs;

   // 1ere partie : Initialisation du tableau des caracteristiques.
   lut = new Region [2*nb_lut-1]; // 2*nb_lut-1 car ...
   initialisation(lut,2*nb_lut-1);
   remplissage(rgs,ims,lut);

   // on calcule la valuation des aretes, si valuation<=threshold alors on empile
   for (i=1;i<gd.Size();i++) {
      // si le noeud n'existe pas, on passe
      if (gd[i]==NULL) continue;
      for (ptr_1=gd[i]->Neighbours();ptr_1!=NULL;ptr_1=ptr_1->Next()) {
	 min_0=valuation(lut,i,ptr_1->Node());  
	 if (min_0<=threshold) {
	    tmp.ex1=i;
	    tmp.ex2=ptr_1->Node();
	    tas.Push(tmp,(float) (min_0));
	 }
      }
   } // fin du parcours des noeuds du graphe

   // 2eme partie : fusion
   Long nbre=0;
   while (nbre<number && !tas.Empty()) {
      // On fusionne l'arc minimum du graphe precedent
      // s'il repond au critere.
      tmp=tas.Pop();
      Ulong R1=tmp.ex1;
      Ulong R2=tmp.ex2;
      
      // L'une des regions a deja ete fusionnee -> on passe.
      if ((lut[R1].no!=R1) || (lut[R2].no!=R2)) continue;
      
      // on ne conserve que le sommet du numero minimum
      Ulong G1=MIN(lut[R1].no_node,lut[R2].no_node);
      Ulong G2=MAX(lut[R1].no_node,lut[R2].no_node);
      
      // on incremente le nombre de regions fusionnees
      // on met a jour la lut.
      Ulong R12=nb_lut+ ++nbre;
      modification(lut,R12,R1,R2);
      lut[R12].no_node=G1;
      lut[R12].no=R12;
      lut[R2].no_node=G1;
      lut[G1].no=R12;
      lut[R1].no=R12;
      lut[R2].no=R12;

      // on met le graphe a jour
      gd[G1]->seed=(gd[G1]->seed+gd[G2]->seed)/2;
      gd.Merge(G1,G2); // G1 reste, G2 disparait.
      for (ptr_3=gd[G1]->Neighbours();ptr_3!=NULL;ptr_3=ptr_3->Next()) {
	 // on met le tas a jour
	 min_0=valuation(lut,R12,lut[ptr_3->Node()].no);
	 if (min_0<=threshold) {
	    tmp.ex1=R12;
	    tmp.ex2=lut[ptr_3->Node()].no;
	    tas.Push(tmp,(float) (min_0));
	    if (tas.Full()) GarbageCollector(tas,lut);
	 }
      }
   } // fin du while

   // 3eme partie : reetiquetage des regions 

   // Minimisation des etiquettes.
   for (i = 1; i < (int)(nb_lut + nbre); i++) {
      Ulong j = i;
      while (lut[j].no!=j) {
	 j = lut[j].no;
      } 
      lut[i].no = lut[j].no_node;
   }
   // fusion dans la carte de region
   for (unsigned int f = 0; f < rgd.VectorSize(); f++) {
      // pour la region qui correspond au voisin qui a fusionne, on donne le Label du 1er sommet
      p_rgd[f] = (Ulong)lut[p_rgs[f]].no;
   }
   
   rgd.Labels(rgs.Labels());
   
   delete [] lut;
   
   return nbre; // retourne le nombre de regions fusionnees
}


/*
 * Remplissage de la lut avec les caracteristiques.
 * A modifier selon le critere
 */
void remplissage( const Reg3d &rgs, const Img3dsf &ims, Region *lut ) {
   Img3dsf::ValueType *p_ims = ims.Vector();
   Ulong *p_rgs = rgs.Vector();

   // remplissage du tableau de caracteristiques
   for (unsigned int d=0; d < rgs.VectorSize(); d++) {
      // on additionne la valeur du pixel dans la r�gion associ�e
      lut[p_rgs[d]].sum += (double) p_ims[d];
      // on incremente le nombre de pixel de cette region
      lut[p_rgs[d]].nb++;   
   }
}

/**
 * INVARIANT SELON LE CRITERE
 */
Errc PMeanMerging( const Reg3d &rgs, const Graph3d &gs, const Img3dsf &ims, Reg3d &rgd, Graph3d &gd, Long number, float threshold ) {
 Region *lut;  
   Ulong *p_rgd = rgd.Vector();
   Ulong *p_rgs = rgs.Vector();
   GEdge *ptr_1, *ptr_3;
   double min_0=0;
   Arete tmp;
   Ulong nb_lut=(rgs.Labels()+1);
   int i;
   Heap <Arete,float,int> tas(26*gs.Size()); // Majorant du nombre d'aretes???

   // supprimer le controle du nombre d'iterations
   if (number<0)
      number=MAXLONG;

   gd=gs;

   // 1ere partie : Initialisation du tableau des caracteristiques.
   lut = new Region [2*nb_lut-1]; // 2*nb_lut-1 car ...
   initialisation(lut,2*nb_lut-1);
   remplissage(rgs,ims,lut);

   // on calcule la valuation des aretes, si valuation<=threshold alors on empile
   for (i=1;i<gd.Size();i++) {
      // si le noeud n'existe pas, on passe
      if (gd[i]==NULL) continue;
      for (ptr_1=gd[i]->Neighbours();ptr_1!=NULL;ptr_1=ptr_1->Next()) {
	 min_0=valuation(lut,i,ptr_1->Node());  
	 if (min_0<=threshold) {
	    tmp.ex1=i;
	    tmp.ex2=ptr_1->Node();
	    tas.Push(tmp,(float) (min_0));
	 }
      }
   } // fin du parcours des noeuds du graphe

   // 2eme partie : fusion
   Long nbre=0;
   while (nbre<number && !tas.Empty()) {
      // On fusionne l'arc minimum du graphe precedent
      // s'il repond au critere.
      tmp=tas.Pop();
      Ulong R1=tmp.ex1;
      Ulong R2=tmp.ex2;
      
      // L'une des regions a deja ete fusionnee -> on passe.
      if ((lut[R1].no!=R1) || (lut[R2].no!=R2)) continue;
      
      // on ne conserve que le sommet du numero minimum
      Ulong G1=MIN(lut[R1].no_node,lut[R2].no_node);
      Ulong G2=MAX(lut[R1].no_node,lut[R2].no_node);
      
      // on incremente le nombre de regions fusionnees
      // on met a jour la lut.
      Ulong R12=nb_lut+ ++nbre;
      modification(lut,R12,R1,R2);
      lut[R12].no_node=G1;
      lut[R12].no=R12;
      lut[R2].no_node=G1;
      lut[G1].no=R12;
      lut[R1].no=R12;
      lut[R2].no=R12;

      // on met le graphe a jour
      gd[G1]->seed=(gd[G1]->seed+gd[G2]->seed)/2;
      gd.Merge(G1,G2); // G1 reste, G2 disparait.
      for (ptr_3=gd[G1]->Neighbours();ptr_3!=NULL;ptr_3=ptr_3->Next()) {
	 // on met le tas a jour
	 min_0=valuation(lut,R12,lut[ptr_3->Node()].no);
	 if (min_0<=threshold) {
	    tmp.ex1=R12;
	    tmp.ex2=lut[ptr_3->Node()].no;
	    tas.Push(tmp,(float) (min_0));
	    if (tas.Full()) GarbageCollector(tas,lut);
	 }
      }
   } // fin du while

   // 3eme partie : reetiquetage des regions 

   // Minimisation des etiquettes.
   for (i = 1; i < (int)(nb_lut + nbre); i++) {
      Ulong j = i;
      while (lut[j].no!=j) {
	 j = lut[j].no;
      } 
      lut[i].no = lut[j].no_node;
   }
   // fusion dans la carte de region
   for (unsigned int f = 0; f < rgd.VectorSize(); f++) {
      // pour la region qui correspond au voisin qui a fusionne, on donne le Label du 1er sommet
      p_rgd[f] = (Ulong)lut[p_rgs[f]].no;
   }
   
   rgd.Labels(rgs.Labels());
   
   delete [] lut;
   
   return nbre; // retourne le nombre de regions fusionnees
}



#ifdef MAIN

/*
 * Modify only the following constants, and the operator switches.
 */
#define	USAGE	"usage: %s number_iterations threshold [-m mask] [rg_in|-] [gr_in|-] [im_in|-] [rg_out|-] [gr_out|-]"
#define	PARC	2  // Number of parameters
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
   if (objs[0]->Type() == Po_Reg2d && objs[1]->Type() == Po_Graph2d && objs[2]->Type() == Po_Img2duc) {
      Reg2d* const rgs = (Reg2d*)objs[0];  
      Graph2d* const gs = (Graph2d*)objs[1];  
      Img2duc* const ims = (Img2duc*)objs[2];
      objd[0] = new Reg2d(rgs->Size());
      Reg2d* const rgd = (Reg2d*)objd[0];
      objd[1] = new Graph2d(gs->Size());
      Graph2d* const gd = (Graph2d*)objd[1];
      
      result = PMeanMerging(*rgs, *gs, *ims, *rgd, *gd, (Long)atoi(parv[0]), (float)atof(parv[1]));
      goto end;
   }
   // verifier toutes les entrees
   if (objs[0]->Type() == Po_Reg2d && objs[1]->Type() == Po_Graph2d && objs[2]->Type() == Po_Img2dsl) {
      Reg2d* const rgs = (Reg2d*)objs[0];  
      Graph2d* const gs = (Graph2d*)objs[1];  
      Img2dsl* const ims = (Img2dsl*)objs[2];
      objd[0] = new Reg2d(rgs->Size());
      Reg2d* const rgd = (Reg2d*)objd[0];
      objd[1] = new Graph2d(gs->Size());
      Graph2d* const gd = (Graph2d*)objd[1];
      
      result = PMeanMerging(*rgs, *gs, *ims, *rgd, *gd, (Long)atoi(parv[0]), (float)atof(parv[1]));
      goto end;
   }
   // verifier toutes les entrees
   if (objs[0]->Type() == Po_Reg2d && objs[1]->Type() == Po_Graph2d && objs[2]->Type() == Po_Img2dsf) {
      Reg2d* const rgs = (Reg2d*)objs[0];  
      Graph2d* const gs = (Graph2d*)objs[1];  
      Img2dsf* const ims = (Img2dsf*)objs[2];
      objd[0] = new Reg2d(rgs->Size());
      Reg2d* const rgd = (Reg2d*)objd[0];
      objd[1] = new Graph2d(gs->Size());
      Graph2d* const gd = (Graph2d*)objd[1];
      
      result = PMeanMerging(*rgs, *gs, *ims, *rgd, *gd, (Long)atoi(parv[0]), (float)atof(parv[1]));
      goto end;
   }
   // verifier toutes les entrees
   if (objs[0]->Type() == Po_Reg3d && objs[1]->Type() == Po_Graph3d && objs[2]->Type() == Po_Img3duc) {
      Reg3d* const rgs = (Reg3d*)objs[0];  
      Graph3d* const gs = (Graph3d*)objs[1];  
      Img3duc* const ims = (Img3duc*)objs[2];
      objd[0] = new Reg3d(rgs->Size());
      Reg3d* const rgd = (Reg3d*)objd[0];
      objd[1] = new Graph3d(gs->Size());
      Graph3d* const gd = (Graph3d*)objd[1];
      
      result = PMeanMerging(*rgs, *gs, *ims, *rgd, *gd, (Long)atoi(parv[0]), (float)atof(parv[1]));
      goto end;
   }
   // verifier toutes les entrees
   if (objs[0]->Type() == Po_Reg3d && objs[1]->Type() == Po_Graph3d && objs[2]->Type() == Po_Img3dsl) {
      Reg3d* const rgs = (Reg3d*)objs[0];  
      Graph3d* const gs = (Graph3d*)objs[1];  
      Img3dsl* const ims = (Img3dsl*)objs[2];
      objd[0] = new Reg3d(rgs->Size());
      Reg3d* const rgd = (Reg3d*)objd[0];
      objd[1] = new Graph3d(gs->Size());
      Graph3d* const gd = (Graph3d*)objd[1];
      
      result = PMeanMerging(*rgs, *gs, *ims, *rgd, *gd, (Long)atoi(parv[0]), (float)atof(parv[1]));
      goto end;
   }
   // verifier toutes les entrees
   if (objs[0]->Type() == Po_Reg3d && objs[1]->Type() == Po_Graph3d && objs[2]->Type() == Po_Img3dsf) {
      Reg3d* const rgs = (Reg3d*)objs[0];  
      Graph3d* const gs = (Graph3d*)objs[1];  
      Img3dsf* const ims = (Img3dsf*)objs[2];
      objd[0] = new Reg3d(rgs->Size());
      Reg3d* const rgd = (Reg3d*)objd[0];
      objd[1] = new Graph3d(gs->Size());
      Graph3d* const gd = (Graph3d*)objd[1];
      
      result = PMeanMerging(*rgs, *gs, *ims, *rgd, *gd, (Long)atoi(parv[0]), (float)atof(parv[1]));
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
