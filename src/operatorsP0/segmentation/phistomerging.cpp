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

#include <math.h>
#include <pandore.h>
using namespace pandore;

/**
 * @file phistomerging.cpp
 * @brief Fusion de region par correlation d'histogrammes.
 * correlation(R1,R2) = H1*H2 / (|H1|.|H2|)
 * Hi = histogramme de la region * i
 * H1*H2 = produit scalaire entre H1 et H2.
 * |Hi| = norme euclienne = sqrt(sum(Hi[x]^2))
 */

struct Arete {
   Ulong ex1;
   Ulong ex2;
};

/*
 * A MODIFIER SELON LE CRITERE
 */

/*
 * A modifier selon le critere
 */
struct Region {
   Ulong no;
   Ulong no_node;
   Ulong *histo;
   double norme;
};

/*
 * Calcule la correlation entre les histogrammes
 * de la region i et j.
 */
double valuation( Region *lut, int i, int j, Ulong butee ) {
   double scalaire = 0.0;

   if (lut[i].norme == 0.0 || lut[j].norme == 0.0) {
      return 0.0;
   }
  
   for (Ulong h = 0; h < butee; h++) {
      scalaire += (double)lut[i].histo[h] * lut[j].histo[h];
   }
    
   return scalaire / (lut[i].norme * lut[j].norme);
}

/*
 * Initialisation de la lut.
 */
void initialisation( Region *lut, Ulong nb_lut, Ulong butee ) {
   for (Ulong a = 0; a < nb_lut; a++) {
      lut[a].no = lut[a].no_node=a;
      lut[a].norme = 0.0;
      lut[a].histo = new Ulong[butee];
      for (Ulong h = 0; h < butee; h++) {
	 lut[a].histo[h] = 0;
      }
   }
}

/*
 * modification de la lut
 * a modifier selon le critere
 */
void modification( Region *lut, Ulong R12, Ulong R1, Ulong R2, Ulong butee ) {
   for (Ulong h = 0; h < butee; h++) {
      lut[R12].histo[h] = lut[R1].histo[h] + lut[R2].histo[h];
      lut[R12].norme += (double)SQR(lut[R12].histo[h]);
   }
   lut[R12].norme = sqrt(lut[R12].norme);
}


/*
 * Permet d'eliminer du tas, toutes les aretes
 * entre deux regions qui sont maintenant fusionnees
 * de maniere a recuprer de la place.
 */
void GarbageCollector( Heap<Arete,float,int> &tas, Region *lut ) {
   Arete* tab = new Arete[tas.Nrank()];
   float* cle = new float[tas.Nrank()];
   Ulong i=0;
   while (!tas.Empty()) {
      tab[i] = tas.Pop(&cle[i]);
      Ulong R1 = tab[i].ex1;
      Ulong R2 = tab[i].ex2;
      
      // L'une des regions a deja ete fusionnee -> on passe.
      if (!((lut[R1].no != R1) || (lut[R2].no != R2))) {
	 i++;
      }
   }

   for (Ulong j = 0; j < i; j++) {
      tas.Push(tab[j], cle[j]);
   }
   delete tab;
}


/*
 * Remplissage de la lut
 */
void remplissage( const Reg2d &rgs, const Img2duc &ims, Region *lut ) {
   Img2duc::ValueType *p_ims = ims.Vector();
   Ulong *p_rgs = rgs.Vector();
  
   // remplissage du tableau de caracteristiques
   for (unsigned int d = 0; d < rgs.VectorSize(); d++) {
      lut[p_rgs[d]].histo[(int)p_ims[d]]++;
   }
   for (Ulong r = 0; r <= rgs.Labels(); r++) {
      for (int h = 0; h <= Limits<Img2duc::ValueType>::max(); h++) {
	 lut[r].norme += (double)SQR(lut[r].histo[h]);
      }
      lut[r].norme = sqrt(lut[r].norme);
   }
}

/**
 * Fusion
 */
Errc PHistoMerging( const Reg2d &rgs, const Graph2d &gs, const Img2duc &ims, Reg2d &rgd, Graph2d &gd, Long number, float threshold ) {
   Ulong *p_rgd = rgd.Vector();
   Ulong *p_rgs = rgs.Vector();
   GEdge *ptr_1, *ptr_3;
   Arete tmp;
   Ulong nb_lut = (rgs.Labels() + 1);
   Ulong butee  =(Ulong)(Limits<Img2duc::ValueType>::max() + 1);
   int i;
   Heap <Arete,float,int> tas(26 * gs.Size());

   // supprimer le controle du nombre d'iterations
   if (number < 0) {
      number = MAXLONG;
   }
   gd = gs;

   // 1ere partie : Initialisation
   Region *lut = new Region[2 * nb_lut - 1];
   initialisation(lut, 2 * nb_lut - 1, butee);
   remplissage(rgs, ims, lut);
 
   // on calcule la valuation des aretes,
   // si valuation <= threshold alors on empile
   for (i = 1; i < gs.Size(); i++) {
      if (gs[i] == NULL) {
	 continue;
      }
      for (ptr_1 = gs[i]->Neighbours(); ptr_1 != NULL; ptr_1 = ptr_1->Next()) {
	 double min_0 = valuation(lut, i, ptr_1->Node(), butee);  
	 if (min_0 >= threshold) {
	    tmp.ex1 = i;
	    tmp.ex2 = ptr_1->Node();
	    tas.Push(tmp, (float) (1.0F - min_0)); // Inversion -> max en haut du tas.
	 }
      }
   } // fin du parcours des noeuds du graphe

   // 2eme partie: fusion
   Long nbre = 0;
   while (nbre < number && !tas.Empty()) {
      //On fusionne l'arc minimum du graphe precedent
      // s'il repond au critere        
      tmp = tas.Pop();
      Ulong R1 = tmp.ex1;
      Ulong R2 = tmp.ex2;
     
      // L'une des regions a deja ete fusionnee -> on passe.
      if ((lut[R1].no != R1) || (lut[R2].no != R2)) {
	 continue;
      }

      // on ne conserve que le sommet de numero minimum
      Ulong G1 = MIN(lut[R1].no_node, lut[R2].no_node);
      Ulong G2 = MAX(lut[R1].no_node, lut[R2].no_node);

      // on incremente le nombre de regions fusionnees
      // on met a jour la lut.
      Ulong R12 = nb_lut + ++nbre;
      modification(lut, R12, R1, R2, butee);
      lut[R12].no_node = G1;
      lut[R12].no = R12;
      lut[R2].no_node = G1;
      lut[G1].no = R12;
      lut[R1].no = R12;
      lut[R2].no = R12;

      // on met le graphe a jour
      gd[G1]->seed = (gd[G1]->seed + gd[G2]->seed) / 2;
      gd.Merge(G1, G2); // G1 reste, G2 disparait.
      for (ptr_3 = gd[G1]->Neighbours(); ptr_3 != NULL; ptr_3 = ptr_3->Next()) {
	 // on met le tas a jour
	 double min_0 = valuation(lut, R12, lut[ptr_3->Node()].no, butee);
	 if (min_0 >= threshold) {
	    tmp.ex1 = R12;
	    tmp.ex2 = lut[ptr_3->Node()].no;
	    tas.Push(tmp, (float)(1.0F - min_0));  // Inversion -> max en haut du tas.
	    if (tas.Full()) {
	       GarbageCollector(tas, lut);
	    }
	 }
      }
   } // fin du while

   // 3eme partie : reetiquetage des regions 

   // Minimisation des etiquettes.
   for (i = 1; i < (int)(nb_lut + nbre); i++) {
      Ulong j = i;
      while (lut[j].no != j) {
	 j = lut[j].no;
      } 
      lut[i].no = lut[j].no_node;
   }
   
   // fusion dans la carte de region
   for (unsigned int f = 0; f < rgd.VectorSize(); f++) {
      // pour la region qui correspond au voisin qui a fusionne, on donne le Label du 1er sommet
      p_rgd[f] = (Ulong)lut[p_rgs[f]].no;
   } 

   // Destruction des histogrammes.
   for (Ulong v = 0; v < (2 * nb_lut - 1); v++) {
      delete [] lut[v].histo;
   }
   delete [] lut;

   rgd.Labels(rgs.Labels());
   return nbre; // retourne le nombre de regions fusionnees
}

/*
 * Remplissage de la lut
 */
void remplissage( const Reg3d &rgs, const Img3duc &ims, Region *lut ) {
   Img3duc::ValueType *p_ims = ims.Vector();
   Ulong *p_rgs = rgs.Vector();
  
   // remplissage du tableau de caracteristiques
   for (unsigned int d = 0; d < rgs.VectorSize(); d++) {
      lut[p_rgs[d]].histo[(int)p_ims[d]]++;
   }
   for (Ulong r = 0; r <= rgs.Labels(); r++) {
      for (int h = 0; h <= Limits<Img3duc::ValueType>::max(); h++) {
	 lut[r].norme += (double)SQR(lut[r].histo[h]);
      }
      lut[r].norme = sqrt(lut[r].norme);
   }
}

/**
 * Fusion
 */
Errc PHistoMerging( const Reg3d &rgs, const Graph3d &gs, const Img3duc &ims, Reg3d &rgd, Graph3d &gd, Long number, float threshold ) {
   Ulong *p_rgd = rgd.Vector();
   Ulong *p_rgs = rgs.Vector();
   GEdge *ptr_1, *ptr_3;
   Arete tmp;
   Ulong nb_lut = (rgs.Labels() + 1);
   Ulong butee  =(Ulong)(Limits<Img3duc::ValueType>::max() + 1);
   int i;
   Heap <Arete,float,int> tas(26 * gs.Size());

   // supprimer le controle du nombre d'iterations
   if (number < 0) {
      number = MAXLONG;
   }
   gd = gs;

   // 1ere partie : Initialisation
   Region *lut = new Region[2 * nb_lut - 1];
   initialisation(lut, 2 * nb_lut - 1, butee);
   remplissage(rgs, ims, lut);
 
   // on calcule la valuation des aretes,
   // si valuation <= threshold alors on empile
   for (i = 1; i < gs.Size(); i++) {
      if (gs[i] == NULL) {
	 continue;
      }
      for (ptr_1 = gs[i]->Neighbours(); ptr_1 != NULL; ptr_1 = ptr_1->Next()) {
	 double min_0 = valuation(lut, i, ptr_1->Node(), butee);  
	 if (min_0 >= threshold) {
	    tmp.ex1 = i;
	    tmp.ex2 = ptr_1->Node();
	    tas.Push(tmp, (float) (1.0F - min_0)); // Inversion -> max en haut du tas.
	 }
      }
   } // fin du parcours des noeuds du graphe

   // 2eme partie: fusion
   Long nbre = 0;
   while (nbre < number && !tas.Empty()) {
      //On fusionne l'arc minimum du graphe precedent
      // s'il repond au critere        
      tmp = tas.Pop();
      Ulong R1 = tmp.ex1;
      Ulong R2 = tmp.ex2;
     
      // L'une des regions a deja ete fusionnee -> on passe.
      if ((lut[R1].no != R1) || (lut[R2].no != R2)) {
	 continue;
      }

      // on ne conserve que le sommet de numero minimum
      Ulong G1 = MIN(lut[R1].no_node, lut[R2].no_node);
      Ulong G2 = MAX(lut[R1].no_node, lut[R2].no_node);

      // on incremente le nombre de regions fusionnees
      // on met a jour la lut.
      Ulong R12 = nb_lut + ++nbre;
      modification(lut, R12, R1, R2, butee);
      lut[R12].no_node = G1;
      lut[R12].no = R12;
      lut[R2].no_node = G1;
      lut[G1].no = R12;
      lut[R1].no = R12;
      lut[R2].no = R12;

      // on met le graphe a jour
      gd[G1]->seed = (gd[G1]->seed + gd[G2]->seed) / 2;
      gd.Merge(G1, G2); // G1 reste, G2 disparait.
      for (ptr_3 = gd[G1]->Neighbours(); ptr_3 != NULL; ptr_3 = ptr_3->Next()) {
	 // on met le tas a jour
	 double min_0 = valuation(lut, R12, lut[ptr_3->Node()].no, butee);
	 if (min_0 >= threshold) {
	    tmp.ex1 = R12;
	    tmp.ex2 = lut[ptr_3->Node()].no;
	    tas.Push(tmp, (float)(1.0F - min_0));  // Inversion -> max en haut du tas.
	    if (tas.Full()) {
	       GarbageCollector(tas, lut);
	    }
	 }
      }
   } // fin du while

   // 3eme partie : reetiquetage des regions 

   // Minimisation des etiquettes.
   for (i = 1; i < (int)(nb_lut + nbre); i++) {
      Ulong j = i;
      while (lut[j].no != j) {
	 j = lut[j].no;
      } 
      lut[i].no = lut[j].no_node;
   }
   
   // fusion dans la carte de region
   for (unsigned int f = 0; f < rgd.VectorSize(); f++) {
      // pour la region qui correspond au voisin qui a fusionne, on donne le Label du 1er sommet
      p_rgd[f] = (Ulong)lut[p_rgs[f]].no;
   } 

   // Destruction des histogrammes.
   for (Ulong v = 0; v < (2 * nb_lut - 1); v++) {
      delete [] lut[v].histo;
   }
   delete [] lut;

   rgd.Labels(rgs.Labels());
   return nbre; // retourne le nombre de regions fusionnees
}


#ifdef MAIN

/*
 * Modify only the following constants, and the operator switches.
 */
#define	USAGE	"usage: %s number threshold [-m mask] [rg_in|-] [gr_in|-] [im_in|-] [rg_out|-] [gr_out|-]"
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
   if (objs[0]->Type() == Po_Reg2d
       && objs[1]->Type() == Po_Graph2d
       && objs[2]->Type() == Po_Img2duc) {
      Reg2d* const rgs = (Reg2d*)objs[0];  
      Graph2d* const gs = (Graph2d*)objs[1];  
      Img2duc* const ims = (Img2duc*)objs[2];
      objd[0] = new Reg2d(rgs->Size());
      Reg2d* const rgd = (Reg2d*)objd[0];
      objd[1] = new Graph2d(gs->Size());
      Graph2d* const gd = (Graph2d*)objd[1];
      
      result = PHistoMerging(*rgs, *gs, *ims, *rgd, *gd, (Long)atoi(parv[0]), (float)atof(parv[1]));
      goto end;
   }
   // verifier toutes les entrees 
   if (objs[0]->Type() == Po_Reg3d
       && objs[1]->Type() == Po_Graph3d
       && objs[2]->Type() == Po_Img3duc) {
      Reg3d* const rgs = (Reg3d*)objs[0];  
      Graph3d* const gs = (Graph3d*)objs[1];  
      Img3duc* const ims = (Img3duc*)objs[2];
      objd[0] = new Reg3d(rgs->Size());
      Reg3d* const rgd = (Reg3d*)objd[0];
      objd[1] = new Graph3d(gs->Size());
      Graph3d* const gd = (Graph3d*)objd[1];
      
      result = PHistoMerging(*rgs, *gs, *ims, *rgd, *gd, (Long)atoi(parv[0]), (float)atof(parv[1]));
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
