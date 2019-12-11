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
 */

#include <pandore.h>
using namespace pandore;

/**
 * @file pvarianceaggregation.cpp
 * On affecte un point a une region si sa valeur est proche du
 * contraste de la region (si la difference entre le contraste d'origine et la
 * valeur du point est inferieure au threshold).
 * Le contraste n'est pas recalcule pour eviter de trop s'eloigner de la
 * situation initiale.
 */

struct tcontraste {
   double min;
   double max;
   bool taille;
};

void AffecteCritere(tcontraste &m, double val) {
   if (m.taille) {
      m.min = MIN(m.min,val);
      m.max = MAX(m.max,val);
   } else {
      m.taille=true;
      m.min=val;
      m.max=val;
   }
}

double ValeurCritere(tcontraste m, double val) {
   if (val <= m.min) return m.min-val;
   if (val >= m.max) return val-m.max;
   return 0.0;
}


Errc PContrastAggregation( const Reg2d &rgs, const Img2duc &ims, Reg2d &rgd, int connexite, Img2duc::ValueType threshold ) {
   int v,w;
   Reg2d::ValueType r;
   tcontraste* contraste;
   Point2d p, p1, pt;
   Fifo< Point2d > F;
   Ulong nbpix=0;

   rgd.Labels(rgs.Labels());

   // Allocation et initialisation du tableau des contrastes.
   if (!(contraste=(tcontraste*)calloc(rgs.Labels()+1,sizeof(tcontraste)))) {
      std::cerr << "Error pcontrastaggregation: Not enough memory." <<  std::endl;
      return FAILURE;
   }
   // Initialisation : recopie des regions d'entrée (rgs) vers la sortie (rgd) 
   // et calcul des taille et somme des niveaux de gris des régions. Les pixels
   // à 0 dans rgs sont considérés comme non traités, ceux qui appartiennent à
   // une région garderont le même label.
   for (p.y = 0; p.y < ims.Height(); p.y++)
   for (p.x = 0; p.x < ims.Width(); p.x++)
   {
      r=rgd[p]=rgs[p];  // Les points de régions sont marqués comme traités 
      if (r) {          // On ne fait pas le contraste de la région 0
	 AffecteCritere(contraste[r],(double)ims[p]);
      } else {
	 // p est un point à 0 donc pas encore traité
	 // On va empiler p SI dans son voisinage IL Y A une région
	 if (connexite == 4) {
	    for (v=0;v<4;v++) {
	       p1=p+v4[v];
	       if (rgs.Hold(p1) && rgs[p1]) {
		  F.Push(p); // On empile le point qui n'est pas encore traité
		  break ;    // Ce n'est pas la peine de voir les autres voisins
	       }
	    }
	 }
	 if (connexite == 8) {
	    for (v=0;v<8;v++) {
	       p1=p+v8[v];
	       if (rgs.Hold(p1) && rgs[p1]) {
		  F.Push(p); // On empile le point qui n'est pas encore traité
		  break ;    // Ce n'est pas la peine de voir les autres voisins
	       }
	    }
	 }
      }
   } // Fin du balayage initial de l'image rgs
   
   // A ce stade tous les pixels voisins d'au moins une région sont empilés...
   
   // Nouveau mode de croissance
   // On dépile un point et on regarde si on peut l'attribuer à une région 
   // voisine en comparant son niveau de gris au contraste de la région 
   // (la différence doit être plus petite qu'un certain threshold pour que le
   // point soit aggrégé, sinon il disparaît définitivement de la pile).
   while (!F.Empty()) {
      p=F.Pop();      // On dépile un point p.

      if (connexite == 4) {
	 for (v=0;v<4;v++) {
	    p1=p+v4[v];
	    if (rgd.Hold(p1)) {
	       if ((r=rgd[p1])) {          // Info lue dans la DESTINATION
		  Img2duc::ValueType m;
		  m=(Img2duc::ValueType)ValeurCritere(contraste[r],(double)ims[p]);
		  if (m <= threshold) {
		     if (!rgd[p]) {  // il n'a pas été aggrégé depuis son empilement
			rgd[p]=r;     // il reçoit le label de sa région voisine
			nbpix++;
			// On empile alors tous ses voisins non encore traités 
			// (ceux à 0 dans rgd)
			for (w=0;w<4;w++) {
			   pt=p+v4[w];
			   if (rgd.Hold(pt)) {
			      if (!rgd[pt]) {
				 F.Push(pt);
			      }
			   }
			}
		     }
		  }
	       }
	    }
	 }
      } else if (connexite == 8) {
	 for (v=0;v<8;v++) {
	    p1=p+v8[v];
	    if (rgd.Hold(p1)) {
	       if ((r=rgd[p1])) {          // Info lue dans la DESTINATION
		  Img2duc::ValueType m;
		  m=(Img2duc::ValueType)ValeurCritere(contraste[r],(double)ims[p]);
		  if (m <= threshold) {
		     if (!rgd[p]) {  // il n'a pas été aggrégé depuis son empilement
			rgd[p]=r;     // il reçoit le label de sa région voisine
			nbpix++;
			// On empile alors tous ses voisins non encore traités 
			// (ceux à 0 dans rgd)
			for (w=0;w<8;w++) {
			   pt=p+v8[w];
			   if (rgd.Hold(pt)) {
			      if (!rgd[pt]) {
				 F.Push(pt);
			      }
			   }
			}
		     }
		  }
	       }
	    }
	 }
      }
   }
   free(contraste);   
   return nbpix;
}

Errc PContrastAggregation( const Reg2d &rgs, const Img2dsl &ims, Reg2d &rgd, int connexite, Img2dsl::ValueType threshold ) {
   int v,w;
   Reg2d::ValueType r;
   tcontraste* contraste;
   Point2d p, p1, pt;
   Fifo< Point2d > F;
   Ulong nbpix=0;

   rgd.Labels(rgs.Labels());

   // Allocation et initialisation du tableau des contrastes.
   if (!(contraste=(tcontraste*)calloc(rgs.Labels()+1,sizeof(tcontraste)))) {
      std::cerr << "Error pcontrastaggregation: Not enough memory." <<  std::endl;
      return FAILURE;
   }
   // Initialisation : recopie des regions d'entrée (rgs) vers la sortie (rgd) 
   // et calcul des taille et somme des niveaux de gris des régions. Les pixels
   // à 0 dans rgs sont considérés comme non traités, ceux qui appartiennent à
   // une région garderont le même label.
   for (p.y = 0; p.y < ims.Height(); p.y++)
   for (p.x = 0; p.x < ims.Width(); p.x++)
   {
      r=rgd[p]=rgs[p];  // Les points de régions sont marqués comme traités 
      if (r) {          // On ne fait pas le contraste de la région 0
	 AffecteCritere(contraste[r],(double)ims[p]);
      } else {
	 // p est un point à 0 donc pas encore traité
	 // On va empiler p SI dans son voisinage IL Y A une région
	 if (connexite == 4) {
	    for (v=0;v<4;v++) {
	       p1=p+v4[v];
	       if (rgs.Hold(p1) && rgs[p1]) {
		  F.Push(p); // On empile le point qui n'est pas encore traité
		  break ;    // Ce n'est pas la peine de voir les autres voisins
	       }
	    }
	 }
	 if (connexite == 8) {
	    for (v=0;v<8;v++) {
	       p1=p+v8[v];
	       if (rgs.Hold(p1) && rgs[p1]) {
		  F.Push(p); // On empile le point qui n'est pas encore traité
		  break ;    // Ce n'est pas la peine de voir les autres voisins
	       }
	    }
	 }
      }
   } // Fin du balayage initial de l'image rgs
   
   // A ce stade tous les pixels voisins d'au moins une région sont empilés...
   
   // Nouveau mode de croissance
   // On dépile un point et on regarde si on peut l'attribuer à une région 
   // voisine en comparant son niveau de gris au contraste de la région 
   // (la différence doit être plus petite qu'un certain threshold pour que le
   // point soit aggrégé, sinon il disparaît définitivement de la pile).
   while (!F.Empty()) {
      p=F.Pop();      // On dépile un point p.

      if (connexite == 4) {
	 for (v=0;v<4;v++) {
	    p1=p+v4[v];
	    if (rgd.Hold(p1)) {
	       if ((r=rgd[p1])) {          // Info lue dans la DESTINATION
		  Img2dsl::ValueType m;
		  m=(Img2dsl::ValueType)ValeurCritere(contraste[r],(double)ims[p]);
		  if (m <= threshold) {
		     if (!rgd[p]) {  // il n'a pas été aggrégé depuis son empilement
			rgd[p]=r;     // il reçoit le label de sa région voisine
			nbpix++;
			// On empile alors tous ses voisins non encore traités 
			// (ceux à 0 dans rgd)
			for (w=0;w<4;w++) {
			   pt=p+v4[w];
			   if (rgd.Hold(pt)) {
			      if (!rgd[pt]) {
				 F.Push(pt);
			      }
			   }
			}
		     }
		  }
	       }
	    }
	 }
      } else if (connexite == 8) {
	 for (v=0;v<8;v++) {
	    p1=p+v8[v];
	    if (rgd.Hold(p1)) {
	       if ((r=rgd[p1])) {          // Info lue dans la DESTINATION
		  Img2dsl::ValueType m;
		  m=(Img2dsl::ValueType)ValeurCritere(contraste[r],(double)ims[p]);
		  if (m <= threshold) {
		     if (!rgd[p]) {  // il n'a pas été aggrégé depuis son empilement
			rgd[p]=r;     // il reçoit le label de sa région voisine
			nbpix++;
			// On empile alors tous ses voisins non encore traités 
			// (ceux à 0 dans rgd)
			for (w=0;w<8;w++) {
			   pt=p+v8[w];
			   if (rgd.Hold(pt)) {
			      if (!rgd[pt]) {
				 F.Push(pt);
			      }
			   }
			}
		     }
		  }
	       }
	    }
	 }
      }
   }
   free(contraste);   
   return nbpix;
}

Errc PContrastAggregation( const Reg2d &rgs, const Img2dsf &ims, Reg2d &rgd, int connexite, Img2dsf::ValueType threshold ) {
   int v,w;
   Reg2d::ValueType r;
   tcontraste* contraste;
   Point2d p, p1, pt;
   Fifo< Point2d > F;
   Ulong nbpix=0;

   rgd.Labels(rgs.Labels());

   // Allocation et initialisation du tableau des contrastes.
   if (!(contraste=(tcontraste*)calloc(rgs.Labels()+1,sizeof(tcontraste)))) {
      std::cerr << "Error pcontrastaggregation: Not enough memory." <<  std::endl;
      return FAILURE;
   }
   // Initialisation : recopie des regions d'entrée (rgs) vers la sortie (rgd) 
   // et calcul des taille et somme des niveaux de gris des régions. Les pixels
   // à 0 dans rgs sont considérés comme non traités, ceux qui appartiennent à
   // une région garderont le même label.
   for (p.y = 0; p.y < ims.Height(); p.y++)
   for (p.x = 0; p.x < ims.Width(); p.x++)
   {
      r=rgd[p]=rgs[p];  // Les points de régions sont marqués comme traités 
      if (r) {          // On ne fait pas le contraste de la région 0
	 AffecteCritere(contraste[r],(double)ims[p]);
      } else {
	 // p est un point à 0 donc pas encore traité
	 // On va empiler p SI dans son voisinage IL Y A une région
	 if (connexite == 4) {
	    for (v=0;v<4;v++) {
	       p1=p+v4[v];
	       if (rgs.Hold(p1) && rgs[p1]) {
		  F.Push(p); // On empile le point qui n'est pas encore traité
		  break ;    // Ce n'est pas la peine de voir les autres voisins
	       }
	    }
	 }
	 if (connexite == 8) {
	    for (v=0;v<8;v++) {
	       p1=p+v8[v];
	       if (rgs.Hold(p1) && rgs[p1]) {
		  F.Push(p); // On empile le point qui n'est pas encore traité
		  break ;    // Ce n'est pas la peine de voir les autres voisins
	       }
	    }
	 }
      }
   } // Fin du balayage initial de l'image rgs
   
   // A ce stade tous les pixels voisins d'au moins une région sont empilés...
   
   // Nouveau mode de croissance
   // On dépile un point et on regarde si on peut l'attribuer à une région 
   // voisine en comparant son niveau de gris au contraste de la région 
   // (la différence doit être plus petite qu'un certain threshold pour que le
   // point soit aggrégé, sinon il disparaît définitivement de la pile).
   while (!F.Empty()) {
      p=F.Pop();      // On dépile un point p.

      if (connexite == 4) {
	 for (v=0;v<4;v++) {
	    p1=p+v4[v];
	    if (rgd.Hold(p1)) {
	       if ((r=rgd[p1])) {          // Info lue dans la DESTINATION
		  Img2dsf::ValueType m;
		  m=(Img2dsf::ValueType)ValeurCritere(contraste[r],(double)ims[p]);
		  if (m <= threshold) {
		     if (!rgd[p]) {  // il n'a pas été aggrégé depuis son empilement
			rgd[p]=r;     // il reçoit le label de sa région voisine
			nbpix++;
			// On empile alors tous ses voisins non encore traités 
			// (ceux à 0 dans rgd)
			for (w=0;w<4;w++) {
			   pt=p+v4[w];
			   if (rgd.Hold(pt)) {
			      if (!rgd[pt]) {
				 F.Push(pt);
			      }
			   }
			}
		     }
		  }
	       }
	    }
	 }
      } else if (connexite == 8) {
	 for (v=0;v<8;v++) {
	    p1=p+v8[v];
	    if (rgd.Hold(p1)) {
	       if ((r=rgd[p1])) {          // Info lue dans la DESTINATION
		  Img2dsf::ValueType m;
		  m=(Img2dsf::ValueType)ValeurCritere(contraste[r],(double)ims[p]);
		  if (m <= threshold) {
		     if (!rgd[p]) {  // il n'a pas été aggrégé depuis son empilement
			rgd[p]=r;     // il reçoit le label de sa région voisine
			nbpix++;
			// On empile alors tous ses voisins non encore traités 
			// (ceux à 0 dans rgd)
			for (w=0;w<8;w++) {
			   pt=p+v8[w];
			   if (rgd.Hold(pt)) {
			      if (!rgd[pt]) {
				 F.Push(pt);
			      }
			   }
			}
		     }
		  }
	       }
	    }
	 }
      }
   }
   free(contraste);   
   return nbpix;
}

Errc PContrastAggregation( const Reg3d &rgs, const Img3duc &ims, Reg3d &rgd, int connexite, Img3duc::ValueType threshold ) {
   int v,w;
   Reg3d::ValueType r;
   tcontraste* contraste;
   Point3d p, p1, pt;
   Fifo< Point3d > F;
   Ulong nbpix=0;

   rgd.Labels(rgs.Labels());

   // Allocation et initialisation du tableau des contrastes.
   if (!(contraste=(tcontraste*)calloc(rgs.Labels()+1,sizeof(tcontraste)))) {
      std::cerr << "Error pcontrastaggregation: Not enough memory." <<  std::endl;
      return FAILURE;
   }
   // Initialisation : recopie des regions d'entrée (rgs) vers la sortie (rgd) 
   // et calcul des taille et somme des niveaux de gris des régions. Les pixels
   // à 0 dans rgs sont considérés comme non traités, ceux qui appartiennent à
   // une région garderont le même label.
   for (p.z = 0; p.z < ims.Depth(); p.z++)
   for (p.y = 0; p.y < ims.Height(); p.y++)
   for (p.x = 0; p.x < ims.Width(); p.x++)
   {
      r=rgd[p]=rgs[p];  // Les points de régions sont marqués comme traités 
      if (r) {          // On ne fait pas le contraste de la région 0
	 AffecteCritere(contraste[r],(double)ims[p]);
      } else {
	 // p est un point à 0 donc pas encore traité
	 // On va empiler p SI dans son voisinage IL Y A une région
	 if (connexite == 6) {
	    for (v=0;v<6;v++) {
	       p1=p+v6[v];
	       if (rgs.Hold(p1) && rgs[p1]) {
		  F.Push(p); // On empile le point qui n'est pas encore traité
		  break ;    // Ce n'est pas la peine de voir les autres voisins
	       }
	    }
	 }
	 if (connexite == 26) {
	    for (v=0;v<26;v++) {
	       p1=p+v26[v];
	       if (rgs.Hold(p1) && rgs[p1]) {
		  F.Push(p); // On empile le point qui n'est pas encore traité
		  break ;    // Ce n'est pas la peine de voir les autres voisins
	       }
	    }
	 }
      }
   } // Fin du balayage initial de l'image rgs
   
   // A ce stade tous les pixels voisins d'au moins une région sont empilés...
   
   // Nouveau mode de croissance
   // On dépile un point et on regarde si on peut l'attribuer à une région 
   // voisine en comparant son niveau de gris au contraste de la région 
   // (la différence doit être plus petite qu'un certain threshold pour que le
   // point soit aggrégé, sinon il disparaît définitivement de la pile).
   while (!F.Empty()) {
      p=F.Pop();      // On dépile un point p.

      if (connexite == 6) {
	 for (v=0;v<6;v++) {
	    p1=p+v6[v];
	    if (rgd.Hold(p1)) {
	       if ((r=rgd[p1])) {          // Info lue dans la DESTINATION
		  Img3duc::ValueType m;
		  m=(Img3duc::ValueType)ValeurCritere(contraste[r],(double)ims[p]);
		  if (m <= threshold) {
		     if (!rgd[p]) {  // il n'a pas été aggrégé depuis son empilement
			rgd[p]=r;     // il reçoit le label de sa région voisine
			nbpix++;
			// On empile alors tous ses voisins non encore traités 
			// (ceux à 0 dans rgd)
			for (w=0;w<6;w++) {
			   pt=p+v6[w];
			   if (rgd.Hold(pt)) {
			      if (!rgd[pt]) {
				 F.Push(pt);
			      }
			   }
			}
		     }
		  }
	       }
	    }
	 }
      } else if (connexite == 26) {
	 for (v=0;v<26;v++) {
	    p1=p+v26[v];
	    if (rgd.Hold(p1)) {
	       if ((r=rgd[p1])) {          // Info lue dans la DESTINATION
		  Img3duc::ValueType m;
		  m=(Img3duc::ValueType)ValeurCritere(contraste[r],(double)ims[p]);
		  if (m <= threshold) {
		     if (!rgd[p]) {  // il n'a pas été aggrégé depuis son empilement
			rgd[p]=r;     // il reçoit le label de sa région voisine
			nbpix++;
			// On empile alors tous ses voisins non encore traités 
			// (ceux à 0 dans rgd)
			for (w=0;w<26;w++) {
			   pt=p+v26[w];
			   if (rgd.Hold(pt)) {
			      if (!rgd[pt]) {
				 F.Push(pt);
			      }
			   }
			}
		     }
		  }
	       }
	    }
	 }
      }
   }
   free(contraste);   
   return nbpix;
}

Errc PContrastAggregation( const Reg3d &rgs, const Img3dsl &ims, Reg3d &rgd, int connexite, Img3dsl::ValueType threshold ) {
   int v,w;
   Reg3d::ValueType r;
   tcontraste* contraste;
   Point3d p, p1, pt;
   Fifo< Point3d > F;
   Ulong nbpix=0;

   rgd.Labels(rgs.Labels());

   // Allocation et initialisation du tableau des contrastes.
   if (!(contraste=(tcontraste*)calloc(rgs.Labels()+1,sizeof(tcontraste)))) {
      std::cerr << "Error pcontrastaggregation: Not enough memory." <<  std::endl;
      return FAILURE;
   }
   // Initialisation : recopie des regions d'entrée (rgs) vers la sortie (rgd) 
   // et calcul des taille et somme des niveaux de gris des régions. Les pixels
   // à 0 dans rgs sont considérés comme non traités, ceux qui appartiennent à
   // une région garderont le même label.
   for (p.z = 0; p.z < ims.Depth(); p.z++)
   for (p.y = 0; p.y < ims.Height(); p.y++)
   for (p.x = 0; p.x < ims.Width(); p.x++)
   {
      r=rgd[p]=rgs[p];  // Les points de régions sont marqués comme traités 
      if (r) {          // On ne fait pas le contraste de la région 0
	 AffecteCritere(contraste[r],(double)ims[p]);
      } else {
	 // p est un point à 0 donc pas encore traité
	 // On va empiler p SI dans son voisinage IL Y A une région
	 if (connexite == 6) {
	    for (v=0;v<6;v++) {
	       p1=p+v6[v];
	       if (rgs.Hold(p1) && rgs[p1]) {
		  F.Push(p); // On empile le point qui n'est pas encore traité
		  break ;    // Ce n'est pas la peine de voir les autres voisins
	       }
	    }
	 }
	 if (connexite == 26) {
	    for (v=0;v<26;v++) {
	       p1=p+v26[v];
	       if (rgs.Hold(p1) && rgs[p1]) {
		  F.Push(p); // On empile le point qui n'est pas encore traité
		  break ;    // Ce n'est pas la peine de voir les autres voisins
	       }
	    }
	 }
      }
   } // Fin du balayage initial de l'image rgs
   
   // A ce stade tous les pixels voisins d'au moins une région sont empilés...
   
   // Nouveau mode de croissance
   // On dépile un point et on regarde si on peut l'attribuer à une région 
   // voisine en comparant son niveau de gris au contraste de la région 
   // (la différence doit être plus petite qu'un certain threshold pour que le
   // point soit aggrégé, sinon il disparaît définitivement de la pile).
   while (!F.Empty()) {
      p=F.Pop();      // On dépile un point p.

      if (connexite == 6) {
	 for (v=0;v<6;v++) {
	    p1=p+v6[v];
	    if (rgd.Hold(p1)) {
	       if ((r=rgd[p1])) {          // Info lue dans la DESTINATION
		  Img3dsl::ValueType m;
		  m=(Img3dsl::ValueType)ValeurCritere(contraste[r],(double)ims[p]);
		  if (m <= threshold) {
		     if (!rgd[p]) {  // il n'a pas été aggrégé depuis son empilement
			rgd[p]=r;     // il reçoit le label de sa région voisine
			nbpix++;
			// On empile alors tous ses voisins non encore traités 
			// (ceux à 0 dans rgd)
			for (w=0;w<6;w++) {
			   pt=p+v6[w];
			   if (rgd.Hold(pt)) {
			      if (!rgd[pt]) {
				 F.Push(pt);
			      }
			   }
			}
		     }
		  }
	       }
	    }
	 }
      } else if (connexite == 26) {
	 for (v=0;v<26;v++) {
	    p1=p+v26[v];
	    if (rgd.Hold(p1)) {
	       if ((r=rgd[p1])) {          // Info lue dans la DESTINATION
		  Img3dsl::ValueType m;
		  m=(Img3dsl::ValueType)ValeurCritere(contraste[r],(double)ims[p]);
		  if (m <= threshold) {
		     if (!rgd[p]) {  // il n'a pas été aggrégé depuis son empilement
			rgd[p]=r;     // il reçoit le label de sa région voisine
			nbpix++;
			// On empile alors tous ses voisins non encore traités 
			// (ceux à 0 dans rgd)
			for (w=0;w<26;w++) {
			   pt=p+v26[w];
			   if (rgd.Hold(pt)) {
			      if (!rgd[pt]) {
				 F.Push(pt);
			      }
			   }
			}
		     }
		  }
	       }
	    }
	 }
      }
   }
   free(contraste);   
   return nbpix;
}

Errc PContrastAggregation( const Reg3d &rgs, const Img3dsf &ims, Reg3d &rgd, int connexite, Img3dsf::ValueType threshold ) {
   int v,w;
   Reg3d::ValueType r;
   tcontraste* contraste;
   Point3d p, p1, pt;
   Fifo< Point3d > F;
   Ulong nbpix=0;

   rgd.Labels(rgs.Labels());

   // Allocation et initialisation du tableau des contrastes.
   if (!(contraste=(tcontraste*)calloc(rgs.Labels()+1,sizeof(tcontraste)))) {
      std::cerr << "Error pcontrastaggregation: Not enough memory." <<  std::endl;
      return FAILURE;
   }
   // Initialisation : recopie des regions d'entrée (rgs) vers la sortie (rgd) 
   // et calcul des taille et somme des niveaux de gris des régions. Les pixels
   // à 0 dans rgs sont considérés comme non traités, ceux qui appartiennent à
   // une région garderont le même label.
   for (p.z = 0; p.z < ims.Depth(); p.z++)
   for (p.y = 0; p.y < ims.Height(); p.y++)
   for (p.x = 0; p.x < ims.Width(); p.x++)
   {
      r=rgd[p]=rgs[p];  // Les points de régions sont marqués comme traités 
      if (r) {          // On ne fait pas le contraste de la région 0
	 AffecteCritere(contraste[r],(double)ims[p]);
      } else {
	 // p est un point à 0 donc pas encore traité
	 // On va empiler p SI dans son voisinage IL Y A une région
	 if (connexite == 6) {
	    for (v=0;v<6;v++) {
	       p1=p+v6[v];
	       if (rgs.Hold(p1) && rgs[p1]) {
		  F.Push(p); // On empile le point qui n'est pas encore traité
		  break ;    // Ce n'est pas la peine de voir les autres voisins
	       }
	    }
	 }
	 if (connexite == 26) {
	    for (v=0;v<26;v++) {
	       p1=p+v26[v];
	       if (rgs.Hold(p1) && rgs[p1]) {
		  F.Push(p); // On empile le point qui n'est pas encore traité
		  break ;    // Ce n'est pas la peine de voir les autres voisins
	       }
	    }
	 }
      }
   } // Fin du balayage initial de l'image rgs
   
   // A ce stade tous les pixels voisins d'au moins une région sont empilés...
   
   // Nouveau mode de croissance
   // On dépile un point et on regarde si on peut l'attribuer à une région 
   // voisine en comparant son niveau de gris au contraste de la région 
   // (la différence doit être plus petite qu'un certain threshold pour que le
   // point soit aggrégé, sinon il disparaît définitivement de la pile).
   while (!F.Empty()) {
      p=F.Pop();      // On dépile un point p.

      if (connexite == 6) {
	 for (v=0;v<6;v++) {
	    p1=p+v6[v];
	    if (rgd.Hold(p1)) {
	       if ((r=rgd[p1])) {          // Info lue dans la DESTINATION
		  Img3dsf::ValueType m;
		  m=(Img3dsf::ValueType)ValeurCritere(contraste[r],(double)ims[p]);
		  if (m <= threshold) {
		     if (!rgd[p]) {  // il n'a pas été aggrégé depuis son empilement
			rgd[p]=r;     // il reçoit le label de sa région voisine
			nbpix++;
			// On empile alors tous ses voisins non encore traités 
			// (ceux à 0 dans rgd)
			for (w=0;w<6;w++) {
			   pt=p+v6[w];
			   if (rgd.Hold(pt)) {
			      if (!rgd[pt]) {
				 F.Push(pt);
			      }
			   }
			}
		     }
		  }
	       }
	    }
	 }
      } else if (connexite == 26) {
	 for (v=0;v<26;v++) {
	    p1=p+v26[v];
	    if (rgd.Hold(p1)) {
	       if ((r=rgd[p1])) {          // Info lue dans la DESTINATION
		  Img3dsf::ValueType m;
		  m=(Img3dsf::ValueType)ValeurCritere(contraste[r],(double)ims[p]);
		  if (m <= threshold) {
		     if (!rgd[p]) {  // il n'a pas été aggrégé depuis son empilement
			rgd[p]=r;     // il reçoit le label de sa région voisine
			nbpix++;
			// On empile alors tous ses voisins non encore traités 
			// (ceux à 0 dans rgd)
			for (w=0;w<26;w++) {
			   pt=p+v26[w];
			   if (rgd.Hold(pt)) {
			      if (!rgd[pt]) {
				 F.Push(pt);
			      }
			   }
			}
		     }
		  }
	       }
	    }
	 }
      }
   }
   free(contraste);   
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
      Reg2d* const rgs=(Reg2d*)objs[0];
      Img2duc* const ims=(Img2duc*)objs[1];
      objd[0]=new Reg2d(rgs->Size());
      Reg2d* const rgd=(Reg2d*)objd[0];
      
      result=PContrastAggregation(*rgs,*ims,*rgd,atoi(parv[0]),(Img2duc::ValueType)atof(parv[1]));
      goto end;
   }

   if ((objs[0]->Type() == Po_Reg2d) && (objs[1]->Type() == Po_Img2dsl)) {
      Reg2d* const rgs=(Reg2d*)objs[0];
      Img2dsl* const ims=(Img2dsl*)objs[1];
      objd[0]=new Reg2d(rgs->Size());
      Reg2d* const rgd=(Reg2d*)objd[0];
      
      result=PContrastAggregation(*rgs,*ims,*rgd,atoi(parv[0]),(Img2dsl::ValueType)atof(parv[1]));
      goto end;
   }

   if ((objs[0]->Type() == Po_Reg2d) && (objs[1]->Type() == Po_Img2dsf)) {
      Reg2d* const rgs=(Reg2d*)objs[0];
      Img2dsf* const ims=(Img2dsf*)objs[1];
      objd[0]=new Reg2d(rgs->Size());
      Reg2d* const rgd=(Reg2d*)objd[0];
      
      result=PContrastAggregation(*rgs,*ims,*rgd,atoi(parv[0]),(Img2dsf::ValueType)atof(parv[1]));
      goto end;
   }

   if ((objs[0]->Type() == Po_Reg3d) && (objs[1]->Type() == Po_Img3duc)) {
      Reg3d* const rgs=(Reg3d*)objs[0];
      Img3duc* const ims=(Img3duc*)objs[1];
      objd[0]=new Reg3d(rgs->Size());
      Reg3d* const rgd=(Reg3d*)objd[0];
      
      result=PContrastAggregation(*rgs,*ims,*rgd,atoi(parv[0]),(Img3duc::ValueType)atof(parv[1]));
      goto end;
   }

   if ((objs[0]->Type() == Po_Reg3d) && (objs[1]->Type() == Po_Img3dsl)) {
      Reg3d* const rgs=(Reg3d*)objs[0];
      Img3dsl* const ims=(Img3dsl*)objs[1];
      objd[0]=new Reg3d(rgs->Size());
      Reg3d* const rgd=(Reg3d*)objd[0];
      
      result=PContrastAggregation(*rgs,*ims,*rgd,atoi(parv[0]),(Img3dsl::ValueType)atof(parv[1]));
      goto end;
   }

   if ((objs[0]->Type() == Po_Reg3d) && (objs[1]->Type() == Po_Img3dsf)) {
      Reg3d* const rgs=(Reg3d*)objs[0];
      Img3dsf* const ims=(Img3dsf*)objs[1];
      objd[0]=new Reg3d(rgs->Size());
      Reg3d* const rgd=(Reg3d*)objd[0];
      
      result=PContrastAggregation(*rgs,*ims,*rgd,atoi(parv[0]),(Img3dsf::ValueType)atof(parv[1]));
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
