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
 * @author Régis Clouard - 1995-11-29
 * @author Régis Clouard - 2001-05-09 (released V 3.00)
 * @author Régis Clouard - 2002-12-22 (released V 4.00)
 * @author Régis Clouard - 2004-01-16 (version 3D 26 neighbours)
 */

#include <pandore.h>
using namespace pandore;

/**
 * @file plabeling.cpp
 * Marquage d'un meme label des regions.
 * Une region est un regroupement de pixels connexes.
 * Remarque: Les pixels de valeurs 0 ne sont pas consideres
 * et se retrouve en sortie avec le label = 0.
 */

/*
 * Merges 2 labels, keeping the smallest.
 */
Ulong MergeLabels( Ulong *color, Ulong c1, Ulong c2 ) {
   Ulong e = c1;
   Ulong b = c2;
   
   // Get the real label of color c1.
   while (color[e] != e) {
      e = color[e];
   }
   
   // Get the real label of color c2.
   while (color[b] != b) {
      b = color[b];
   }
   
   Ulong min, max;
   if (e > b) {
      min = b;
      max = e;
   } else {
      min = e;
      max = b;
   }
   
   color[max] = min;   
   color[c1] = color[c2] = min;
   
   return min;
}




/*
 * connectivity : 6 or 26
 */



/*
 * Numerotation des composantes connexes d'un graphe
 */

Errc PLabeling( const Img2duc &ims, Reg2d &rgd, int connectivity ) {
   Ulong etiquette;
   Ulong* couleur;
   Ulong* labels;
   register int	y, x;
   Ulong nlabelsmax = ims.VectorSize() + 1; // No more label than pixel...
   Ulong i;

   couleur = new Ulong[nlabelsmax];

   // Initialization of the label table.
   for (i = 0; i < nlabelsmax; i++) {
      couleur[i] = i;
   }
   etiquette = 0;

   // 1th line.
   if (ims(0, 0) != 0) {
      rgd(0, 0) = ++etiquette;
   } else {
      rgd(0, 0) = 0;
   }
   for (y = 0, x = 1; x < ims.Width(); x++) {
      if (ims(y, x) != 0) {
	 rgd(y, x) = ((ims(y, x) == ims(y, x - 1)) ? etiquette : ++etiquette);
      } else {
	 rgd(y, x) = 0;
      }
   }
   
   // General case.
   if (connectivity == 4) {
      for (y = 1; y < ims.Height(); y++) {
	 x = 0;
	 if (ims(y, x) != 0) {
	    if (ims(y, x) == ims(y - 1, x)) {
	       rgd(y, x) = rgd(y - 1, x);
	    } else {
	       rgd(y, x) = ++etiquette;
	    }
	 } else {
	    rgd(y, x) = 0;
	 }
	 
	 for (x = 1; x < ims.Width(); x++) {
	    rgd(y, x) = 0;
	    if (ims(y, x) == 0) {
	       continue;
	    }
	    if (ims(y, x) == ims(y - 1, x)) {
		rgd(y, x) = couleur[rgd(y - 1, x)];
	    }
	    if (ims(y, x) == ims(y, x - 1)) {
	       if (rgd(y, x) == 0) {
		  rgd(y, x) = couleur[rgd(y, x - 1)];
	       } else {
		  rgd(y, x) = MergeLabels(couleur, rgd(y, x - 1), rgd(y, x));
	       }
	    }
	    if (rgd(y, x) == 0) {
	       rgd(y, x) = ++etiquette;
	    }
	 }
      }
   } else {
      for (y = 1; y < ims.Height(); y++) {
	 x = 0;
	 if (ims(y, x) != 0) {
	    if (ims(y, x) == ims(y - 1, x)) {
	       rgd(y, x) = rgd(y - 1, x);
	    } else {
	       rgd(y, x) = ++etiquette;
	    }
	 } else {
	    rgd(y, x) = 0;
	 }
	 for (x = 1; x < ims.Width(); x++) {
	    rgd(y, x) = 0;
	    if (ims(y, x) == 0) {
	       continue;
	    }
	    if (ims(y, x) == ims(y - 1, x))
	       rgd(y, x) = couleur[rgd(y - 1, x)];
	    if ((x + 1 < ims.Width()) && (ims(y, x) == ims(y - 1, x + 1))) {
	       if (rgd(y, x) == 0) {
		  rgd(y, x) = couleur[rgd(y - 1, x + 1)];
	       } else {
		  rgd(y, x) = MergeLabels(couleur, rgd(y - 1, x + 1), rgd(y, x));
	       }
	    }
	    if (ims(y, x) == ims(y - 1, x - 1)) {
	       if (rgd(y, x) == 0) {
		  rgd(y, x) = couleur[rgd(y - 1, x - 1)];
	       } else {
		  rgd(y, x) = MergeLabels(couleur, rgd(y - 1, x - 1), rgd(y, x));
	       }
	    }
	    if (ims(y, x) == ims(y, x - 1)) {
	       if (rgd(y, x) == 0) {
		  rgd(y, x) = couleur[rgd(y, x - 1)];
	       } else {
		  rgd(y, x) = MergeLabels(couleur, rgd(y, x - 1), rgd(y, x));
	       }
	    }
	    if (rgd(y, x) == 0) {
	       rgd(y, x) = ++etiquette;
	    }
	 } 
      }
   }
  
   labels = new Ulong[etiquette  +1];
   
   Ulong nbreg = 0;

   // Minimize labels   
   for (i = 1; i <= etiquette; i++) {
      if (couleur[i] != i) {
	 labels[i] = labels[couleur[i]];
      } else {
	 labels[i] = ++nbreg;
      }
   }
   
   // Marking
   for (y = 0; y < ims.Height(); y++) {
      for (x = 0; x < ims.Width(); x++) {
	 if (ims(y, x)) {
	    rgd(y, x) = labels[couleur[rgd(y, x)]];
	 } else {
	    rgd(y, x) = 0;
	 }
      }
   }

   delete[] labels;
   delete[] couleur;

   return rgd.Labels(nbreg);
}

Errc PLabeling( const Img2dsl &ims, Reg2d &rgd, int connectivity ) {
   Ulong etiquette;
   Ulong* couleur;
   Ulong* labels;
   register int	y, x;
   Ulong nlabelsmax = ims.VectorSize() + 1; // No more label than pixel...
   Ulong i;

   couleur = new Ulong[nlabelsmax];

   // Initialization of the label table.
   for (i = 0; i < nlabelsmax; i++) {
      couleur[i] = i;
   }
   etiquette = 0;

   // 1th line.
   if (ims(0, 0) != 0) {
      rgd(0, 0) = ++etiquette;
   } else {
      rgd(0, 0) = 0;
   }
   for (y = 0, x = 1; x < ims.Width(); x++) {
      if (ims(y, x) != 0) {
	 rgd(y, x) = ((ims(y, x) == ims(y, x - 1)) ? etiquette : ++etiquette);
      } else {
	 rgd(y, x) = 0;
      }
   }
   
   // General case.
   if (connectivity == 4) {
      for (y = 1; y < ims.Height(); y++) {
	 x = 0;
	 if (ims(y, x) != 0) {
	    if (ims(y, x) == ims(y - 1, x)) {
	       rgd(y, x) = rgd(y - 1, x);
	    } else {
	       rgd(y, x) = ++etiquette;
	    }
	 } else {
	    rgd(y, x) = 0;
	 }
	 
	 for (x = 1; x < ims.Width(); x++) {
	    rgd(y, x) = 0;
	    if (ims(y, x) == 0) {
	       continue;
	    }
	    if (ims(y, x) == ims(y - 1, x)) {
		rgd(y, x) = couleur[rgd(y - 1, x)];
	    }
	    if (ims(y, x) == ims(y, x - 1)) {
	       if (rgd(y, x) == 0) {
		  rgd(y, x) = couleur[rgd(y, x - 1)];
	       } else {
		  rgd(y, x) = MergeLabels(couleur, rgd(y, x - 1), rgd(y, x));
	       }
	    }
	    if (rgd(y, x) == 0) {
	       rgd(y, x) = ++etiquette;
	    }
	 }
      }
   } else {
      for (y = 1; y < ims.Height(); y++) {
	 x = 0;
	 if (ims(y, x) != 0) {
	    if (ims(y, x) == ims(y - 1, x)) {
	       rgd(y, x) = rgd(y - 1, x);
	    } else {
	       rgd(y, x) = ++etiquette;
	    }
	 } else {
	    rgd(y, x) = 0;
	 }
	 for (x = 1; x < ims.Width(); x++) {
	    rgd(y, x) = 0;
	    if (ims(y, x) == 0) {
	       continue;
	    }
	    if (ims(y, x) == ims(y - 1, x))
	       rgd(y, x) = couleur[rgd(y - 1, x)];
	    if ((x + 1 < ims.Width()) && (ims(y, x) == ims(y - 1, x + 1))) {
	       if (rgd(y, x) == 0) {
		  rgd(y, x) = couleur[rgd(y - 1, x + 1)];
	       } else {
		  rgd(y, x) = MergeLabels(couleur, rgd(y - 1, x + 1), rgd(y, x));
	       }
	    }
	    if (ims(y, x) == ims(y - 1, x - 1)) {
	       if (rgd(y, x) == 0) {
		  rgd(y, x) = couleur[rgd(y - 1, x - 1)];
	       } else {
		  rgd(y, x) = MergeLabels(couleur, rgd(y - 1, x - 1), rgd(y, x));
	       }
	    }
	    if (ims(y, x) == ims(y, x - 1)) {
	       if (rgd(y, x) == 0) {
		  rgd(y, x) = couleur[rgd(y, x - 1)];
	       } else {
		  rgd(y, x) = MergeLabels(couleur, rgd(y, x - 1), rgd(y, x));
	       }
	    }
	    if (rgd(y, x) == 0) {
	       rgd(y, x) = ++etiquette;
	    }
	 } 
      }
   }
  
   labels = new Ulong[etiquette  +1];
   
   Ulong nbreg = 0;

   // Minimize labels   
   for (i = 1; i <= etiquette; i++) {
      if (couleur[i] != i) {
	 labels[i] = labels[couleur[i]];
      } else {
	 labels[i] = ++nbreg;
      }
   }
   
   // Marking
   for (y = 0; y < ims.Height(); y++) {
      for (x = 0; x < ims.Width(); x++) {
	 if (ims(y, x)) {
	    rgd(y, x) = labels[couleur[rgd(y, x)]];
	 } else {
	    rgd(y, x) = 0;
	 }
      }
   }

   delete[] labels;
   delete[] couleur;

   return rgd.Labels(nbreg);
}

Errc PLabeling( const Img2dsf &ims, Reg2d &rgd, int connectivity ) {
   Ulong etiquette;
   Ulong* couleur;
   Ulong* labels;
   register int	y, x;
   Ulong nlabelsmax = ims.VectorSize() + 1; // No more label than pixel...
   Ulong i;

   couleur = new Ulong[nlabelsmax];

   // Initialization of the label table.
   for (i = 0; i < nlabelsmax; i++) {
      couleur[i] = i;
   }
   etiquette = 0;

   // 1th line.
   if (ims(0, 0) != 0) {
      rgd(0, 0) = ++etiquette;
   } else {
      rgd(0, 0) = 0;
   }
   for (y = 0, x = 1; x < ims.Width(); x++) {
      if (ims(y, x) != 0) {
	 rgd(y, x) = ((ims(y, x) == ims(y, x - 1)) ? etiquette : ++etiquette);
      } else {
	 rgd(y, x) = 0;
      }
   }
   
   // General case.
   if (connectivity == 4) {
      for (y = 1; y < ims.Height(); y++) {
	 x = 0;
	 if (ims(y, x) != 0) {
	    if (ims(y, x) == ims(y - 1, x)) {
	       rgd(y, x) = rgd(y - 1, x);
	    } else {
	       rgd(y, x) = ++etiquette;
	    }
	 } else {
	    rgd(y, x) = 0;
	 }
	 
	 for (x = 1; x < ims.Width(); x++) {
	    rgd(y, x) = 0;
	    if (ims(y, x) == 0) {
	       continue;
	    }
	    if (ims(y, x) == ims(y - 1, x)) {
		rgd(y, x) = couleur[rgd(y - 1, x)];
	    }
	    if (ims(y, x) == ims(y, x - 1)) {
	       if (rgd(y, x) == 0) {
		  rgd(y, x) = couleur[rgd(y, x - 1)];
	       } else {
		  rgd(y, x) = MergeLabels(couleur, rgd(y, x - 1), rgd(y, x));
	       }
	    }
	    if (rgd(y, x) == 0) {
	       rgd(y, x) = ++etiquette;
	    }
	 }
      }
   } else {
      for (y = 1; y < ims.Height(); y++) {
	 x = 0;
	 if (ims(y, x) != 0) {
	    if (ims(y, x) == ims(y - 1, x)) {
	       rgd(y, x) = rgd(y - 1, x);
	    } else {
	       rgd(y, x) = ++etiquette;
	    }
	 } else {
	    rgd(y, x) = 0;
	 }
	 for (x = 1; x < ims.Width(); x++) {
	    rgd(y, x) = 0;
	    if (ims(y, x) == 0) {
	       continue;
	    }
	    if (ims(y, x) == ims(y - 1, x))
	       rgd(y, x) = couleur[rgd(y - 1, x)];
	    if ((x + 1 < ims.Width()) && (ims(y, x) == ims(y - 1, x + 1))) {
	       if (rgd(y, x) == 0) {
		  rgd(y, x) = couleur[rgd(y - 1, x + 1)];
	       } else {
		  rgd(y, x) = MergeLabels(couleur, rgd(y - 1, x + 1), rgd(y, x));
	       }
	    }
	    if (ims(y, x) == ims(y - 1, x - 1)) {
	       if (rgd(y, x) == 0) {
		  rgd(y, x) = couleur[rgd(y - 1, x - 1)];
	       } else {
		  rgd(y, x) = MergeLabels(couleur, rgd(y - 1, x - 1), rgd(y, x));
	       }
	    }
	    if (ims(y, x) == ims(y, x - 1)) {
	       if (rgd(y, x) == 0) {
		  rgd(y, x) = couleur[rgd(y, x - 1)];
	       } else {
		  rgd(y, x) = MergeLabels(couleur, rgd(y, x - 1), rgd(y, x));
	       }
	    }
	    if (rgd(y, x) == 0) {
	       rgd(y, x) = ++etiquette;
	    }
	 } 
      }
   }
  
   labels = new Ulong[etiquette  +1];
   
   Ulong nbreg = 0;

   // Minimize labels   
   for (i = 1; i <= etiquette; i++) {
      if (couleur[i] != i) {
	 labels[i] = labels[couleur[i]];
      } else {
	 labels[i] = ++nbreg;
      }
   }
   
   // Marking
   for (y = 0; y < ims.Height(); y++) {
      for (x = 0; x < ims.Width(); x++) {
	 if (ims(y, x)) {
	    rgd(y, x) = labels[couleur[rgd(y, x)]];
	 } else {
	    rgd(y, x) = 0;
	 }
      }
   }

   delete[] labels;
   delete[] couleur;

   return rgd.Labels(nbreg);
}

Errc PLabeling( const Reg2d &ims, Reg2d &rgd, int connectivity ) {
   Ulong etiquette;
   Ulong* couleur;
   Ulong* labels;
   register int	y, x;
   Ulong nlabelsmax = ims.VectorSize() + 1; // No more label than pixel...
   Ulong i;

   couleur = new Ulong[nlabelsmax];

   // Initialization of the label table.
   for (i = 0; i < nlabelsmax; i++) {
      couleur[i] = i;
   }
   etiquette = 0;

   // 1th line.
   if (ims(0, 0) != 0) {
      rgd(0, 0) = ++etiquette;
   } else {
      rgd(0, 0) = 0;
   }
   for (y = 0, x = 1; x < ims.Width(); x++) {
      if (ims(y, x) != 0) {
	 rgd(y, x) = ((ims(y, x) == ims(y, x - 1)) ? etiquette : ++etiquette);
      } else {
	 rgd(y, x) = 0;
      }
   }
   
   // General case.
   if (connectivity == 4) {
      for (y = 1; y < ims.Height(); y++) {
	 x = 0;
	 if (ims(y, x) != 0) {
	    if (ims(y, x) == ims(y - 1, x)) {
	       rgd(y, x) = rgd(y - 1, x);
	    } else {
	       rgd(y, x) = ++etiquette;
	    }
	 } else {
	    rgd(y, x) = 0;
	 }
	 
	 for (x = 1; x < ims.Width(); x++) {
	    rgd(y, x) = 0;
	    if (ims(y, x) == 0) {
	       continue;
	    }
	    if (ims(y, x) == ims(y - 1, x)) {
		rgd(y, x) = couleur[rgd(y - 1, x)];
	    }
	    if (ims(y, x) == ims(y, x - 1)) {
	       if (rgd(y, x) == 0) {
		  rgd(y, x) = couleur[rgd(y, x - 1)];
	       } else {
		  rgd(y, x) = MergeLabels(couleur, rgd(y, x - 1), rgd(y, x));
	       }
	    }
	    if (rgd(y, x) == 0) {
	       rgd(y, x) = ++etiquette;
	    }
	 }
      }
   } else {
      for (y = 1; y < ims.Height(); y++) {
	 x = 0;
	 if (ims(y, x) != 0) {
	    if (ims(y, x) == ims(y - 1, x)) {
	       rgd(y, x) = rgd(y - 1, x);
	    } else {
	       rgd(y, x) = ++etiquette;
	    }
	 } else {
	    rgd(y, x) = 0;
	 }
	 for (x = 1; x < ims.Width(); x++) {
	    rgd(y, x) = 0;
	    if (ims(y, x) == 0) {
	       continue;
	    }
	    if (ims(y, x) == ims(y - 1, x))
	       rgd(y, x) = couleur[rgd(y - 1, x)];
	    if ((x + 1 < ims.Width()) && (ims(y, x) == ims(y - 1, x + 1))) {
	       if (rgd(y, x) == 0) {
		  rgd(y, x) = couleur[rgd(y - 1, x + 1)];
	       } else {
		  rgd(y, x) = MergeLabels(couleur, rgd(y - 1, x + 1), rgd(y, x));
	       }
	    }
	    if (ims(y, x) == ims(y - 1, x - 1)) {
	       if (rgd(y, x) == 0) {
		  rgd(y, x) = couleur[rgd(y - 1, x - 1)];
	       } else {
		  rgd(y, x) = MergeLabels(couleur, rgd(y - 1, x - 1), rgd(y, x));
	       }
	    }
	    if (ims(y, x) == ims(y, x - 1)) {
	       if (rgd(y, x) == 0) {
		  rgd(y, x) = couleur[rgd(y, x - 1)];
	       } else {
		  rgd(y, x) = MergeLabels(couleur, rgd(y, x - 1), rgd(y, x));
	       }
	    }
	    if (rgd(y, x) == 0) {
	       rgd(y, x) = ++etiquette;
	    }
	 } 
      }
   }
  
   labels = new Ulong[etiquette  +1];
   
   Ulong nbreg = 0;

   // Minimize labels   
   for (i = 1; i <= etiquette; i++) {
      if (couleur[i] != i) {
	 labels[i] = labels[couleur[i]];
      } else {
	 labels[i] = ++nbreg;
      }
   }
   
   // Marking
   for (y = 0; y < ims.Height(); y++) {
      for (x = 0; x < ims.Width(); x++) {
	 if (ims(y, x)) {
	    rgd(y, x) = labels[couleur[rgd(y, x)]];
	 } else {
	    rgd(y, x) = 0;
	 }
      }
   }

   delete[] labels;
   delete[] couleur;

   return rgd.Labels(nbreg);
}

Errc PLabeling( const Imc2duc &ims, Reg2d &rgd, int connectivity ) {
   Img2dsl image(ims.Props());
   Uchar *pr = ims.Vector(0);
   Uchar *pg = ims.Vector(1);
   Uchar *pb = ims.Vector(2);
   Long *ps = image.Vector();
   Uchar *pend = ims.Vector(0) + ims.VectorSize();

   for ( ; pr < pend ; pr++, pg++, pb++, ps++) {
      *ps = *pb << 16 | *pg << 8 | *pr;
   }
   return PLabeling(image, rgd, connectivity);
}

void Propagate26( const Img3duc &ims, Reg3d &rgd, int x, int y, int z, int v, Ulong *couleur ) {
   if (ims(z, y, x) == ims(z + v26z[v], y + v26y[v], x + v26x[v])) {
      if (rgd(z, y, x) == 0) {
	 rgd(z, y, x) = couleur[rgd(z + v26z[v], y + v26y[v], x + v26x[v])];
      } else {
	 rgd(z, y, x) = MergeLabels(couleur, rgd(z + v26z[v], y + v26y[v], x + v26x[v]), rgd(z, y, x));
      }
   }
}

Errc PLabeling( const Img3duc &ims, Reg3d &rgd, int connectivity ) {
   register int x, y, z;
   Ulong etiquette;
   Ulong* couleur;
   Ulong *labels;
   Img3duc::ValueType v;
   Ulong i;
   
   if (connectivity != 6 && connectivity != 26) {
      std::cerr << "Error plabeling: bad connectivity number: "<< connectivity <<std::endl;
      return FAILURE;
   }

   // Maximum labels = total of voxels.
   Ulong nlabelsmax = ims.VectorSize()+1;

   couleur = new Ulong[nlabelsmax];
  
   for (i = 0; i < nlabelsmax; i++) {
      couleur[i] = i;
   }
   etiquette = 0;
  
   if (ims(0, 0, 0) != 0) {
      rgd(0, 0, 0) = ++etiquette;
   } else {
      rgd(0, 0, 0) = 0;
   }

   // 1th slice, 1th row and 1th column.
   for (z = 0, y = 0, x = 1; x < ims.Width(); x++) {
      if ((v = ims(z, y, x)) != 0) {
	 if (v == ims(z, y, x - 1)) {
	    rgd(z, y, x) = rgd(z, y, x - 1);
	 } else {
	    rgd(z, y, x) = ++etiquette;
	 }
      } else {
	 rgd(z, y, x) = 0;
      }
   }
   
   if (connectivity == 26) { // 26-connectivity
      // 1th slice.
      z = 0;
      for (y = 1; y < ims.Height(); y++) {
	 x = 0;
	 if ((v = ims(z, y, x)) != 0) {
	    if (v == ims(z, y - 1, x)) {
	       rgd(z, y, x) = rgd(z, y - 1, x);
	    } else {
	       rgd(z, y, x) = ++etiquette;
	    }
	 } else {
	    rgd(z, y, x) = 0;
	 }
	 
	 for (x = 1; x < ims.Width(); x++) {
	    rgd(z, y, x) = 0;
	    if ((v = ims(z, y, x)) == 0) {
	       continue;
	    }
	    if (v == ims(z, y - 1, x)) { // neighbour 11
	       rgd(z, y, x) = couleur[rgd(z, y - 1, x)];
	    }
	    
	    Propagate26(ims, rgd, x, y, z, 9, couleur);
	    Propagate26(ims, rgd, x, y, z, 10, couleur);
	    if (x + 1 < ims.Width()) {
	       Propagate26(ims, rgd, x, y, z, 12, couleur);
	    }
	    if (rgd(z, y, x) == 0) {
	       rgd(z, y, x) = ++etiquette;
	    }
	 }
      }
      
      //  n-th slices.
      for (z = 1; z < ims.Depth(); z++) {
	 y = x = 0;
	 if ((v = ims(z, y, x))) {
	    if (v == ims(z - 1, y, x)) {
	       rgd(z, y, x) = couleur[rgd(z - 1, y, x)];
	    } else {
	       rgd(z, y, x) = ++etiquette;
	    }
	 } else {
	    rgd(z, y, x) = 0;
	 }
	 // 1th row
	 for (x = 1; x < ims.Width(); x++) {
	    rgd(z, y, x) = 0;
	    if ((v = ims(z, y, x)) == 0) {
	       continue;
	    }
	    if (v == ims(z, y, x - 1)) {
	       rgd(z, y, x) = couleur[rgd(z, y, x - 1)];
	    }

	    Propagate26(ims, rgd, x, y, z, 0, couleur);
	    Propagate26(ims, rgd, x, y, z, 1, couleur);
	    Propagate26(ims, rgd, x, y, z, 5, couleur);
	    Propagate26(ims, rgd, x, y, z, 6, couleur);
	    Propagate26(ims, rgd, x, y, z, 7, couleur);
	    Propagate26(ims, rgd, x, y, z, 8, couleur);
	    Propagate26(ims, rgd, x, y, z, 9, couleur);

	    if (rgd(z, y, x) == 0) {
	       rgd(z, y, x) = ++etiquette;
	    }
	 }
	 
	 for (y = 1; y < ims.Height(); y++) {
	    x = 0;
	    if ((v = ims(z, y, x))) {
	       if (v == ims(z - 1, y, x)) {
		  rgd(z, y, x) = couleur[rgd(z - 1, y, x)];
	       } else {
		  rgd(z, y, x) = 0;
	       }
	       if (v == ims(z, y - 1, x)) {
		  if (rgd(z, y, x) == 0) {
		     rgd(z, y, x) = couleur[rgd(z, y - 1, x)];
		  } else {
		     rgd(z, y, x) = MergeLabels(couleur, rgd(z, y - 1, x), rgd(z, y, x));
		  }
	       }
	       if (rgd(z, y, x) == 0) {
		  rgd(z, y, x) = (Ulong)++etiquette;
	       }
	    }
	    
	    for (x = 1; x < ims.Width(); x++) {
	       if ((v = ims(z, y, x)) == 0) {
		  continue;
	       }
	       if (v == ims(z, y - 1, x)) {
		  rgd(z, y, x) = couleur[rgd(z, y - 1, x)];
	       }
	       Propagate26(ims, rgd, x, y, z, 0, couleur);
	       Propagate26(ims, rgd, x, y, z, 1, couleur);
	       Propagate26(ims, rgd, x, y, z, 2, couleur);
	       Propagate26(ims, rgd, x, y, z, 3, couleur);
	       Propagate26(ims, rgd, x, y, z, 4, couleur);
	       Propagate26(ims, rgd, x, y, z, 5, couleur);
	       Propagate26(ims, rgd, x, y, z, 6, couleur);
	       Propagate26(ims, rgd, x, y, z, 7, couleur);
	       Propagate26(ims, rgd, x, y, z, 8, couleur);
	       Propagate26(ims, rgd, x, y, z, 9, couleur);
	       Propagate26(ims, rgd, x, y, z, 10, couleur);
	       Propagate26(ims, rgd, x, y, z, 11, couleur);
	       if (x + 1 < ims.Width()) {
		  Propagate26(ims, rgd, x, y, z, 12, couleur);
	       }
	       
	       if (rgd(z, y, x) == 0) {
		  rgd(z, y, x) = ++etiquette;
	       }
	    }
	 }
      }
      // end 26 -connectivity"
   } else { // 6-connectivity
      // 1th slice.
      z = 0;
      for ( y = 1; y < ims.Height(); y++) {
	 x = 0;
	 if ((v = ims(z, y, x)) != 0) {
	    if (v == ims(z, y - 1, x)) {
	       rgd(z, y, x) = rgd(z, y - 1, x);
	    } else {
	       rgd(z, y, x) = ++etiquette;
	    }
	 } else {
	    rgd(z, y, x) = 0;
	 }
	 for (x = 1; x < ims.Width(); x++) {
	    rgd(z, y, x) = 0;
	    if ((v=ims(z, y, x)) == 0) {
	       continue;
	    }
	    if ( v == ims(z, y - 1, x)) {
	       rgd(z, y, x) = couleur[rgd(z, y - 1, x)];
	    }
	    if (v == ims(z, y, x - 1)) {
	       if (rgd(z, y, x) == 0) {
		  rgd(z, y, x) = couleur[rgd(z, y, x - 1)];
	       } else {
		  rgd(z, y, x) = MergeLabels(couleur, rgd(z, y, x - 1), rgd(z, y, x));
	       }
	    }
	    if (rgd(z, y, x) == 0) {
	       rgd(z, y, x) = ++etiquette;
	    }
	 }
      }
      
      // n-th slices.
      for (z = 1; z < ims.Depth(); z++) {
	 y = x = 0;
	 if ((v = ims(z, y, x))) {
	    if (v == ims(z - 1, y, x)) {
	       rgd(z, y, x) = couleur[rgd(z - 1, y, x)];
	    } else {
	       rgd(z, y, x) = ++etiquette;
	    }
	 } else {
	    rgd(z, y, x) = 0;
	 }
	 
	 // 1th row
	 for (x = 1; x < ims.Width(); x++) {
	    if ((v = ims(z, y, x)) == 0) {
	       continue;
	    }
	    if (v == ims(z - 1, y, x)) {
	       rgd(z, y, x) = couleur[rgd(z - 1, y, x)];
	    } else {
	       rgd(z, y, x) = 0;
	    }
	    if (v == ims(z, y, z - 1)) {
	       if (rgd(z, y, x) == 0) {
		  rgd(z, y, x) = couleur[rgd(z, y, x - 1)];
	       } else {
		 rgd(z, y, x) = MergeLabels(couleur, rgd(z, y, x - 1), rgd(z, y, x));
	       }
	    }
	    if (rgd(z, y, x) == 0) {
	       rgd(z, y, x) = ++etiquette;
	    }
	 }
	 
	 for (y = 1; y < ims.Height(); y++) {
	    x = 0;
	    if ((v = ims(z, y, x))) {
	       if (v == ims(z - 1, y, x)) {
		  rgd(z, y, x) = couleur[rgd(z - 1, y, x)];
	       } else {
		  rgd(z, y, x) = 0;
	       }
	       if (v == ims(z, y - 1, x)) {
		  if (rgd(z, y, x) == 0) {
		     rgd(z, y, x) = couleur[rgd(z, y - 1, x)];
		  } else {
		     rgd(z, y, x) = MergeLabels(couleur, rgd(z, y - 1, x), rgd(z, y, x));
		  }
	       }
	       if (rgd(z, y, x) == 0) {
		  rgd(z, y, x) = (Ulong)++etiquette;
	       }
	    }
	    
	    for (x = 1; x < ims.Width(); x++) {
	       if ((v = ims(z, y, x))) {
		  if (v == ims(z - 1, y, x)) {
		     rgd(z, y, x) = couleur[rgd(z - 1, y, x)];
		  } else {
		     rgd(z, y, x) = 0;
		  }
		  if (v == ims(z, y - 1, x)) {
		     if (rgd(z, y, x) == 0) {
			rgd(z, y, x) = couleur[rgd(z, y - 1, x)];
		     } else {
			rgd(z, y, x) = MergeLabels(couleur, rgd(z, y - 1, x), rgd(z, y, x));
		     }
		  }
		  if (v == ims(z, y, x - 1)) {
		     if (rgd(z, y, x) == 0) {
			rgd(z, y, x) = couleur[rgd(z, y, x - 1)];
		     } else {
			rgd(z, y, x) = MergeLabels(couleur, rgd(z, y, x - 1), rgd(z, y, x));
		     }
		  }
		  if (rgd(z, y, x) == 0) {
		     rgd(z, y, x) = ++etiquette;
		  }
	       }
	    }
	 }
      }
   } // end 6 -connectivity"

   labels = new Ulong[etiquette+1];
   
   Ulong nbreg = 0;

   // Minimize labels   
   for (i = 1; i <= etiquette; i++) {
      if (couleur[i] != i) {
	 labels[i] = labels[couleur[i]];
      } else {
	 labels[i] = ++nbreg;
      }
   }
   
   // Marking
   for (z = 0; z < ims.Depth(); z++) {
      for (y = 0; y < ims.Height(); y++) {
	 for (x = 0; x < ims.Width(); x++) {
	    if (ims(z, y, x)) {
	       rgd(z, y, x) = labels[couleur[rgd(z, y, x)]];
	    } else {
	       rgd(z, y, x) = 0;
	    }
	 }
      }
   }
   
   delete[] labels;
   delete[] couleur;

   return rgd.Labels(nbreg);
}

void Propagate26( const Img3dsl &ims, Reg3d &rgd, int x, int y, int z, int v, Ulong *couleur ) {
   if (ims(z, y, x) == ims(z + v26z[v], y + v26y[v], x + v26x[v])) {
      if (rgd(z, y, x) == 0) {
	 rgd(z, y, x) = couleur[rgd(z + v26z[v], y + v26y[v], x + v26x[v])];
      } else {
	 rgd(z, y, x) = MergeLabels(couleur, rgd(z + v26z[v], y + v26y[v], x + v26x[v]), rgd(z, y, x));
      }
   }
}

Errc PLabeling( const Img3dsl &ims, Reg3d &rgd, int connectivity ) {
   register int x, y, z;
   Ulong etiquette;
   Ulong* couleur;
   Ulong *labels;
   Img3dsl::ValueType v;
   Ulong i;
   
   if (connectivity != 6 && connectivity != 26) {
      std::cerr << "Error plabeling: bad connectivity number: "<< connectivity <<std::endl;
      return FAILURE;
   }

   // Maximum labels = total of voxels.
   Ulong nlabelsmax = ims.VectorSize()+1;

   couleur = new Ulong[nlabelsmax];
  
   for (i = 0; i < nlabelsmax; i++) {
      couleur[i] = i;
   }
   etiquette = 0;
  
   if (ims(0, 0, 0) != 0) {
      rgd(0, 0, 0) = ++etiquette;
   } else {
      rgd(0, 0, 0) = 0;
   }

   // 1th slice, 1th row and 1th column.
   for (z = 0, y = 0, x = 1; x < ims.Width(); x++) {
      if ((v = ims(z, y, x)) != 0) {
	 if (v == ims(z, y, x - 1)) {
	    rgd(z, y, x) = rgd(z, y, x - 1);
	 } else {
	    rgd(z, y, x) = ++etiquette;
	 }
      } else {
	 rgd(z, y, x) = 0;
      }
   }
   
   if (connectivity == 26) { // 26-connectivity
      // 1th slice.
      z = 0;
      for (y = 1; y < ims.Height(); y++) {
	 x = 0;
	 if ((v = ims(z, y, x)) != 0) {
	    if (v == ims(z, y - 1, x)) {
	       rgd(z, y, x) = rgd(z, y - 1, x);
	    } else {
	       rgd(z, y, x) = ++etiquette;
	    }
	 } else {
	    rgd(z, y, x) = 0;
	 }
	 
	 for (x = 1; x < ims.Width(); x++) {
	    rgd(z, y, x) = 0;
	    if ((v = ims(z, y, x)) == 0) {
	       continue;
	    }
	    if (v == ims(z, y - 1, x)) { // neighbour 11
	       rgd(z, y, x) = couleur[rgd(z, y - 1, x)];
	    }
	    
	    Propagate26(ims, rgd, x, y, z, 9, couleur);
	    Propagate26(ims, rgd, x, y, z, 10, couleur);
	    if (x + 1 < ims.Width()) {
	       Propagate26(ims, rgd, x, y, z, 12, couleur);
	    }
	    if (rgd(z, y, x) == 0) {
	       rgd(z, y, x) = ++etiquette;
	    }
	 }
      }
      
      //  n-th slices.
      for (z = 1; z < ims.Depth(); z++) {
	 y = x = 0;
	 if ((v = ims(z, y, x))) {
	    if (v == ims(z - 1, y, x)) {
	       rgd(z, y, x) = couleur[rgd(z - 1, y, x)];
	    } else {
	       rgd(z, y, x) = ++etiquette;
	    }
	 } else {
	    rgd(z, y, x) = 0;
	 }
	 // 1th row
	 for (x = 1; x < ims.Width(); x++) {
	    rgd(z, y, x) = 0;
	    if ((v = ims(z, y, x)) == 0) {
	       continue;
	    }
	    if (v == ims(z, y, x - 1)) {
	       rgd(z, y, x) = couleur[rgd(z, y, x - 1)];
	    }

	    Propagate26(ims, rgd, x, y, z, 0, couleur);
	    Propagate26(ims, rgd, x, y, z, 1, couleur);
	    Propagate26(ims, rgd, x, y, z, 5, couleur);
	    Propagate26(ims, rgd, x, y, z, 6, couleur);
	    Propagate26(ims, rgd, x, y, z, 7, couleur);
	    Propagate26(ims, rgd, x, y, z, 8, couleur);
	    Propagate26(ims, rgd, x, y, z, 9, couleur);

	    if (rgd(z, y, x) == 0) {
	       rgd(z, y, x) = ++etiquette;
	    }
	 }
	 
	 for (y = 1; y < ims.Height(); y++) {
	    x = 0;
	    if ((v = ims(z, y, x))) {
	       if (v == ims(z - 1, y, x)) {
		  rgd(z, y, x) = couleur[rgd(z - 1, y, x)];
	       } else {
		  rgd(z, y, x) = 0;
	       }
	       if (v == ims(z, y - 1, x)) {
		  if (rgd(z, y, x) == 0) {
		     rgd(z, y, x) = couleur[rgd(z, y - 1, x)];
		  } else {
		     rgd(z, y, x) = MergeLabels(couleur, rgd(z, y - 1, x), rgd(z, y, x));
		  }
	       }
	       if (rgd(z, y, x) == 0) {
		  rgd(z, y, x) = (Ulong)++etiquette;
	       }
	    }
	    
	    for (x = 1; x < ims.Width(); x++) {
	       if ((v = ims(z, y, x)) == 0) {
		  continue;
	       }
	       if (v == ims(z, y - 1, x)) {
		  rgd(z, y, x) = couleur[rgd(z, y - 1, x)];
	       }
	       Propagate26(ims, rgd, x, y, z, 0, couleur);
	       Propagate26(ims, rgd, x, y, z, 1, couleur);
	       Propagate26(ims, rgd, x, y, z, 2, couleur);
	       Propagate26(ims, rgd, x, y, z, 3, couleur);
	       Propagate26(ims, rgd, x, y, z, 4, couleur);
	       Propagate26(ims, rgd, x, y, z, 5, couleur);
	       Propagate26(ims, rgd, x, y, z, 6, couleur);
	       Propagate26(ims, rgd, x, y, z, 7, couleur);
	       Propagate26(ims, rgd, x, y, z, 8, couleur);
	       Propagate26(ims, rgd, x, y, z, 9, couleur);
	       Propagate26(ims, rgd, x, y, z, 10, couleur);
	       Propagate26(ims, rgd, x, y, z, 11, couleur);
	       if (x + 1 < ims.Width()) {
		  Propagate26(ims, rgd, x, y, z, 12, couleur);
	       }
	       
	       if (rgd(z, y, x) == 0) {
		  rgd(z, y, x) = ++etiquette;
	       }
	    }
	 }
      }
      // end 26 -connectivity"
   } else { // 6-connectivity
      // 1th slice.
      z = 0;
      for ( y = 1; y < ims.Height(); y++) {
	 x = 0;
	 if ((v = ims(z, y, x)) != 0) {
	    if (v == ims(z, y - 1, x)) {
	       rgd(z, y, x) = rgd(z, y - 1, x);
	    } else {
	       rgd(z, y, x) = ++etiquette;
	    }
	 } else {
	    rgd(z, y, x) = 0;
	 }
	 for (x = 1; x < ims.Width(); x++) {
	    rgd(z, y, x) = 0;
	    if ((v=ims(z, y, x)) == 0) {
	       continue;
	    }
	    if ( v == ims(z, y - 1, x)) {
	       rgd(z, y, x) = couleur[rgd(z, y - 1, x)];
	    }
	    if (v == ims(z, y, x - 1)) {
	       if (rgd(z, y, x) == 0) {
		  rgd(z, y, x) = couleur[rgd(z, y, x - 1)];
	       } else {
		  rgd(z, y, x) = MergeLabels(couleur, rgd(z, y, x - 1), rgd(z, y, x));
	       }
	    }
	    if (rgd(z, y, x) == 0) {
	       rgd(z, y, x) = ++etiquette;
	    }
	 }
      }
      
      // n-th slices.
      for (z = 1; z < ims.Depth(); z++) {
	 y = x = 0;
	 if ((v = ims(z, y, x))) {
	    if (v == ims(z - 1, y, x)) {
	       rgd(z, y, x) = couleur[rgd(z - 1, y, x)];
	    } else {
	       rgd(z, y, x) = ++etiquette;
	    }
	 } else {
	    rgd(z, y, x) = 0;
	 }
	 
	 // 1th row
	 for (x = 1; x < ims.Width(); x++) {
	    if ((v = ims(z, y, x)) == 0) {
	       continue;
	    }
	    if (v == ims(z - 1, y, x)) {
	       rgd(z, y, x) = couleur[rgd(z - 1, y, x)];
	    } else {
	       rgd(z, y, x) = 0;
	    }
	    if (v == ims(z, y, z - 1)) {
	       if (rgd(z, y, x) == 0) {
		  rgd(z, y, x) = couleur[rgd(z, y, x - 1)];
	       } else {
		 rgd(z, y, x) = MergeLabels(couleur, rgd(z, y, x - 1), rgd(z, y, x));
	       }
	    }
	    if (rgd(z, y, x) == 0) {
	       rgd(z, y, x) = ++etiquette;
	    }
	 }
	 
	 for (y = 1; y < ims.Height(); y++) {
	    x = 0;
	    if ((v = ims(z, y, x))) {
	       if (v == ims(z - 1, y, x)) {
		  rgd(z, y, x) = couleur[rgd(z - 1, y, x)];
	       } else {
		  rgd(z, y, x) = 0;
	       }
	       if (v == ims(z, y - 1, x)) {
		  if (rgd(z, y, x) == 0) {
		     rgd(z, y, x) = couleur[rgd(z, y - 1, x)];
		  } else {
		     rgd(z, y, x) = MergeLabels(couleur, rgd(z, y - 1, x), rgd(z, y, x));
		  }
	       }
	       if (rgd(z, y, x) == 0) {
		  rgd(z, y, x) = (Ulong)++etiquette;
	       }
	    }
	    
	    for (x = 1; x < ims.Width(); x++) {
	       if ((v = ims(z, y, x))) {
		  if (v == ims(z - 1, y, x)) {
		     rgd(z, y, x) = couleur[rgd(z - 1, y, x)];
		  } else {
		     rgd(z, y, x) = 0;
		  }
		  if (v == ims(z, y - 1, x)) {
		     if (rgd(z, y, x) == 0) {
			rgd(z, y, x) = couleur[rgd(z, y - 1, x)];
		     } else {
			rgd(z, y, x) = MergeLabels(couleur, rgd(z, y - 1, x), rgd(z, y, x));
		     }
		  }
		  if (v == ims(z, y, x - 1)) {
		     if (rgd(z, y, x) == 0) {
			rgd(z, y, x) = couleur[rgd(z, y, x - 1)];
		     } else {
			rgd(z, y, x) = MergeLabels(couleur, rgd(z, y, x - 1), rgd(z, y, x));
		     }
		  }
		  if (rgd(z, y, x) == 0) {
		     rgd(z, y, x) = ++etiquette;
		  }
	       }
	    }
	 }
      }
   } // end 6 -connectivity"

   labels = new Ulong[etiquette+1];
   
   Ulong nbreg = 0;

   // Minimize labels   
   for (i = 1; i <= etiquette; i++) {
      if (couleur[i] != i) {
	 labels[i] = labels[couleur[i]];
      } else {
	 labels[i] = ++nbreg;
      }
   }
   
   // Marking
   for (z = 0; z < ims.Depth(); z++) {
      for (y = 0; y < ims.Height(); y++) {
	 for (x = 0; x < ims.Width(); x++) {
	    if (ims(z, y, x)) {
	       rgd(z, y, x) = labels[couleur[rgd(z, y, x)]];
	    } else {
	       rgd(z, y, x) = 0;
	    }
	 }
      }
   }
   
   delete[] labels;
   delete[] couleur;

   return rgd.Labels(nbreg);
}

void Propagate26( const Img3dsf &ims, Reg3d &rgd, int x, int y, int z, int v, Ulong *couleur ) {
   if (ims(z, y, x) == ims(z + v26z[v], y + v26y[v], x + v26x[v])) {
      if (rgd(z, y, x) == 0) {
	 rgd(z, y, x) = couleur[rgd(z + v26z[v], y + v26y[v], x + v26x[v])];
      } else {
	 rgd(z, y, x) = MergeLabels(couleur, rgd(z + v26z[v], y + v26y[v], x + v26x[v]), rgd(z, y, x));
      }
   }
}

Errc PLabeling( const Img3dsf &ims, Reg3d &rgd, int connectivity ) {
   register int x, y, z;
   Ulong etiquette;
   Ulong* couleur;
   Ulong *labels;
   Img3dsf::ValueType v;
   Ulong i;
   
   if (connectivity != 6 && connectivity != 26) {
      std::cerr << "Error plabeling: bad connectivity number: "<< connectivity <<std::endl;
      return FAILURE;
   }

   // Maximum labels = total of voxels.
   Ulong nlabelsmax = ims.VectorSize()+1;

   couleur = new Ulong[nlabelsmax];
  
   for (i = 0; i < nlabelsmax; i++) {
      couleur[i] = i;
   }
   etiquette = 0;
  
   if (ims(0, 0, 0) != 0) {
      rgd(0, 0, 0) = ++etiquette;
   } else {
      rgd(0, 0, 0) = 0;
   }

   // 1th slice, 1th row and 1th column.
   for (z = 0, y = 0, x = 1; x < ims.Width(); x++) {
      if ((v = ims(z, y, x)) != 0) {
	 if (v == ims(z, y, x - 1)) {
	    rgd(z, y, x) = rgd(z, y, x - 1);
	 } else {
	    rgd(z, y, x) = ++etiquette;
	 }
      } else {
	 rgd(z, y, x) = 0;
      }
   }
   
   if (connectivity == 26) { // 26-connectivity
      // 1th slice.
      z = 0;
      for (y = 1; y < ims.Height(); y++) {
	 x = 0;
	 if ((v = ims(z, y, x)) != 0) {
	    if (v == ims(z, y - 1, x)) {
	       rgd(z, y, x) = rgd(z, y - 1, x);
	    } else {
	       rgd(z, y, x) = ++etiquette;
	    }
	 } else {
	    rgd(z, y, x) = 0;
	 }
	 
	 for (x = 1; x < ims.Width(); x++) {
	    rgd(z, y, x) = 0;
	    if ((v = ims(z, y, x)) == 0) {
	       continue;
	    }
	    if (v == ims(z, y - 1, x)) { // neighbour 11
	       rgd(z, y, x) = couleur[rgd(z, y - 1, x)];
	    }
	    
	    Propagate26(ims, rgd, x, y, z, 9, couleur);
	    Propagate26(ims, rgd, x, y, z, 10, couleur);
	    if (x + 1 < ims.Width()) {
	       Propagate26(ims, rgd, x, y, z, 12, couleur);
	    }
	    if (rgd(z, y, x) == 0) {
	       rgd(z, y, x) = ++etiquette;
	    }
	 }
      }
      
      //  n-th slices.
      for (z = 1; z < ims.Depth(); z++) {
	 y = x = 0;
	 if ((v = ims(z, y, x))) {
	    if (v == ims(z - 1, y, x)) {
	       rgd(z, y, x) = couleur[rgd(z - 1, y, x)];
	    } else {
	       rgd(z, y, x) = ++etiquette;
	    }
	 } else {
	    rgd(z, y, x) = 0;
	 }
	 // 1th row
	 for (x = 1; x < ims.Width(); x++) {
	    rgd(z, y, x) = 0;
	    if ((v = ims(z, y, x)) == 0) {
	       continue;
	    }
	    if (v == ims(z, y, x - 1)) {
	       rgd(z, y, x) = couleur[rgd(z, y, x - 1)];
	    }

	    Propagate26(ims, rgd, x, y, z, 0, couleur);
	    Propagate26(ims, rgd, x, y, z, 1, couleur);
	    Propagate26(ims, rgd, x, y, z, 5, couleur);
	    Propagate26(ims, rgd, x, y, z, 6, couleur);
	    Propagate26(ims, rgd, x, y, z, 7, couleur);
	    Propagate26(ims, rgd, x, y, z, 8, couleur);
	    Propagate26(ims, rgd, x, y, z, 9, couleur);

	    if (rgd(z, y, x) == 0) {
	       rgd(z, y, x) = ++etiquette;
	    }
	 }
	 
	 for (y = 1; y < ims.Height(); y++) {
	    x = 0;
	    if ((v = ims(z, y, x))) {
	       if (v == ims(z - 1, y, x)) {
		  rgd(z, y, x) = couleur[rgd(z - 1, y, x)];
	       } else {
		  rgd(z, y, x) = 0;
	       }
	       if (v == ims(z, y - 1, x)) {
		  if (rgd(z, y, x) == 0) {
		     rgd(z, y, x) = couleur[rgd(z, y - 1, x)];
		  } else {
		     rgd(z, y, x) = MergeLabels(couleur, rgd(z, y - 1, x), rgd(z, y, x));
		  }
	       }
	       if (rgd(z, y, x) == 0) {
		  rgd(z, y, x) = (Ulong)++etiquette;
	       }
	    }
	    
	    for (x = 1; x < ims.Width(); x++) {
	       if ((v = ims(z, y, x)) == 0) {
		  continue;
	       }
	       if (v == ims(z, y - 1, x)) {
		  rgd(z, y, x) = couleur[rgd(z, y - 1, x)];
	       }
	       Propagate26(ims, rgd, x, y, z, 0, couleur);
	       Propagate26(ims, rgd, x, y, z, 1, couleur);
	       Propagate26(ims, rgd, x, y, z, 2, couleur);
	       Propagate26(ims, rgd, x, y, z, 3, couleur);
	       Propagate26(ims, rgd, x, y, z, 4, couleur);
	       Propagate26(ims, rgd, x, y, z, 5, couleur);
	       Propagate26(ims, rgd, x, y, z, 6, couleur);
	       Propagate26(ims, rgd, x, y, z, 7, couleur);
	       Propagate26(ims, rgd, x, y, z, 8, couleur);
	       Propagate26(ims, rgd, x, y, z, 9, couleur);
	       Propagate26(ims, rgd, x, y, z, 10, couleur);
	       Propagate26(ims, rgd, x, y, z, 11, couleur);
	       if (x + 1 < ims.Width()) {
		  Propagate26(ims, rgd, x, y, z, 12, couleur);
	       }
	       
	       if (rgd(z, y, x) == 0) {
		  rgd(z, y, x) = ++etiquette;
	       }
	    }
	 }
      }
      // end 26 -connectivity"
   } else { // 6-connectivity
      // 1th slice.
      z = 0;
      for ( y = 1; y < ims.Height(); y++) {
	 x = 0;
	 if ((v = ims(z, y, x)) != 0) {
	    if (v == ims(z, y - 1, x)) {
	       rgd(z, y, x) = rgd(z, y - 1, x);
	    } else {
	       rgd(z, y, x) = ++etiquette;
	    }
	 } else {
	    rgd(z, y, x) = 0;
	 }
	 for (x = 1; x < ims.Width(); x++) {
	    rgd(z, y, x) = 0;
	    if ((v=ims(z, y, x)) == 0) {
	       continue;
	    }
	    if ( v == ims(z, y - 1, x)) {
	       rgd(z, y, x) = couleur[rgd(z, y - 1, x)];
	    }
	    if (v == ims(z, y, x - 1)) {
	       if (rgd(z, y, x) == 0) {
		  rgd(z, y, x) = couleur[rgd(z, y, x - 1)];
	       } else {
		  rgd(z, y, x) = MergeLabels(couleur, rgd(z, y, x - 1), rgd(z, y, x));
	       }
	    }
	    if (rgd(z, y, x) == 0) {
	       rgd(z, y, x) = ++etiquette;
	    }
	 }
      }
      
      // n-th slices.
      for (z = 1; z < ims.Depth(); z++) {
	 y = x = 0;
	 if ((v = ims(z, y, x))) {
	    if (v == ims(z - 1, y, x)) {
	       rgd(z, y, x) = couleur[rgd(z - 1, y, x)];
	    } else {
	       rgd(z, y, x) = ++etiquette;
	    }
	 } else {
	    rgd(z, y, x) = 0;
	 }
	 
	 // 1th row
	 for (x = 1; x < ims.Width(); x++) {
	    if ((v = ims(z, y, x)) == 0) {
	       continue;
	    }
	    if (v == ims(z - 1, y, x)) {
	       rgd(z, y, x) = couleur[rgd(z - 1, y, x)];
	    } else {
	       rgd(z, y, x) = 0;
	    }
	    if (v == ims(z, y, z - 1)) {
	       if (rgd(z, y, x) == 0) {
		  rgd(z, y, x) = couleur[rgd(z, y, x - 1)];
	       } else {
		 rgd(z, y, x) = MergeLabels(couleur, rgd(z, y, x - 1), rgd(z, y, x));
	       }
	    }
	    if (rgd(z, y, x) == 0) {
	       rgd(z, y, x) = ++etiquette;
	    }
	 }
	 
	 for (y = 1; y < ims.Height(); y++) {
	    x = 0;
	    if ((v = ims(z, y, x))) {
	       if (v == ims(z - 1, y, x)) {
		  rgd(z, y, x) = couleur[rgd(z - 1, y, x)];
	       } else {
		  rgd(z, y, x) = 0;
	       }
	       if (v == ims(z, y - 1, x)) {
		  if (rgd(z, y, x) == 0) {
		     rgd(z, y, x) = couleur[rgd(z, y - 1, x)];
		  } else {
		     rgd(z, y, x) = MergeLabels(couleur, rgd(z, y - 1, x), rgd(z, y, x));
		  }
	       }
	       if (rgd(z, y, x) == 0) {
		  rgd(z, y, x) = (Ulong)++etiquette;
	       }
	    }
	    
	    for (x = 1; x < ims.Width(); x++) {
	       if ((v = ims(z, y, x))) {
		  if (v == ims(z - 1, y, x)) {
		     rgd(z, y, x) = couleur[rgd(z - 1, y, x)];
		  } else {
		     rgd(z, y, x) = 0;
		  }
		  if (v == ims(z, y - 1, x)) {
		     if (rgd(z, y, x) == 0) {
			rgd(z, y, x) = couleur[rgd(z, y - 1, x)];
		     } else {
			rgd(z, y, x) = MergeLabels(couleur, rgd(z, y - 1, x), rgd(z, y, x));
		     }
		  }
		  if (v == ims(z, y, x - 1)) {
		     if (rgd(z, y, x) == 0) {
			rgd(z, y, x) = couleur[rgd(z, y, x - 1)];
		     } else {
			rgd(z, y, x) = MergeLabels(couleur, rgd(z, y, x - 1), rgd(z, y, x));
		     }
		  }
		  if (rgd(z, y, x) == 0) {
		     rgd(z, y, x) = ++etiquette;
		  }
	       }
	    }
	 }
      }
   } // end 6 -connectivity"

   labels = new Ulong[etiquette+1];
   
   Ulong nbreg = 0;

   // Minimize labels   
   for (i = 1; i <= etiquette; i++) {
      if (couleur[i] != i) {
	 labels[i] = labels[couleur[i]];
      } else {
	 labels[i] = ++nbreg;
      }
   }
   
   // Marking
   for (z = 0; z < ims.Depth(); z++) {
      for (y = 0; y < ims.Height(); y++) {
	 for (x = 0; x < ims.Width(); x++) {
	    if (ims(z, y, x)) {
	       rgd(z, y, x) = labels[couleur[rgd(z, y, x)]];
	    } else {
	       rgd(z, y, x) = 0;
	    }
	 }
      }
   }
   
   delete[] labels;
   delete[] couleur;

   return rgd.Labels(nbreg);
}

void Propagate26( const Reg3d &ims, Reg3d &rgd, int x, int y, int z, int v, Ulong *couleur ) {
   if (ims(z, y, x) == ims(z + v26z[v], y + v26y[v], x + v26x[v])) {
      if (rgd(z, y, x) == 0) {
	 rgd(z, y, x) = couleur[rgd(z + v26z[v], y + v26y[v], x + v26x[v])];
      } else {
	 rgd(z, y, x) = MergeLabels(couleur, rgd(z + v26z[v], y + v26y[v], x + v26x[v]), rgd(z, y, x));
      }
   }
}

Errc PLabeling( const Reg3d &ims, Reg3d &rgd, int connectivity ) {
   register int x, y, z;
   Ulong etiquette;
   Ulong* couleur;
   Ulong *labels;
   Reg3d::ValueType v;
   Ulong i;
   
   if (connectivity != 6 && connectivity != 26) {
      std::cerr << "Error plabeling: bad connectivity number: "<< connectivity <<std::endl;
      return FAILURE;
   }

   // Maximum labels = total of voxels.
   Ulong nlabelsmax = ims.VectorSize()+1;

   couleur = new Ulong[nlabelsmax];
  
   for (i = 0; i < nlabelsmax; i++) {
      couleur[i] = i;
   }
   etiquette = 0;
  
   if (ims(0, 0, 0) != 0) {
      rgd(0, 0, 0) = ++etiquette;
   } else {
      rgd(0, 0, 0) = 0;
   }

   // 1th slice, 1th row and 1th column.
   for (z = 0, y = 0, x = 1; x < ims.Width(); x++) {
      if ((v = ims(z, y, x)) != 0) {
	 if (v == ims(z, y, x - 1)) {
	    rgd(z, y, x) = rgd(z, y, x - 1);
	 } else {
	    rgd(z, y, x) = ++etiquette;
	 }
      } else {
	 rgd(z, y, x) = 0;
      }
   }
   
   if (connectivity == 26) { // 26-connectivity
      // 1th slice.
      z = 0;
      for (y = 1; y < ims.Height(); y++) {
	 x = 0;
	 if ((v = ims(z, y, x)) != 0) {
	    if (v == ims(z, y - 1, x)) {
	       rgd(z, y, x) = rgd(z, y - 1, x);
	    } else {
	       rgd(z, y, x) = ++etiquette;
	    }
	 } else {
	    rgd(z, y, x) = 0;
	 }
	 
	 for (x = 1; x < ims.Width(); x++) {
	    rgd(z, y, x) = 0;
	    if ((v = ims(z, y, x)) == 0) {
	       continue;
	    }
	    if (v == ims(z, y - 1, x)) { // neighbour 11
	       rgd(z, y, x) = couleur[rgd(z, y - 1, x)];
	    }
	    
	    Propagate26(ims, rgd, x, y, z, 9, couleur);
	    Propagate26(ims, rgd, x, y, z, 10, couleur);
	    if (x + 1 < ims.Width()) {
	       Propagate26(ims, rgd, x, y, z, 12, couleur);
	    }
	    if (rgd(z, y, x) == 0) {
	       rgd(z, y, x) = ++etiquette;
	    }
	 }
      }
      
      //  n-th slices.
      for (z = 1; z < ims.Depth(); z++) {
	 y = x = 0;
	 if ((v = ims(z, y, x))) {
	    if (v == ims(z - 1, y, x)) {
	       rgd(z, y, x) = couleur[rgd(z - 1, y, x)];
	    } else {
	       rgd(z, y, x) = ++etiquette;
	    }
	 } else {
	    rgd(z, y, x) = 0;
	 }
	 // 1th row
	 for (x = 1; x < ims.Width(); x++) {
	    rgd(z, y, x) = 0;
	    if ((v = ims(z, y, x)) == 0) {
	       continue;
	    }
	    if (v == ims(z, y, x - 1)) {
	       rgd(z, y, x) = couleur[rgd(z, y, x - 1)];
	    }

	    Propagate26(ims, rgd, x, y, z, 0, couleur);
	    Propagate26(ims, rgd, x, y, z, 1, couleur);
	    Propagate26(ims, rgd, x, y, z, 5, couleur);
	    Propagate26(ims, rgd, x, y, z, 6, couleur);
	    Propagate26(ims, rgd, x, y, z, 7, couleur);
	    Propagate26(ims, rgd, x, y, z, 8, couleur);
	    Propagate26(ims, rgd, x, y, z, 9, couleur);

	    if (rgd(z, y, x) == 0) {
	       rgd(z, y, x) = ++etiquette;
	    }
	 }
	 
	 for (y = 1; y < ims.Height(); y++) {
	    x = 0;
	    if ((v = ims(z, y, x))) {
	       if (v == ims(z - 1, y, x)) {
		  rgd(z, y, x) = couleur[rgd(z - 1, y, x)];
	       } else {
		  rgd(z, y, x) = 0;
	       }
	       if (v == ims(z, y - 1, x)) {
		  if (rgd(z, y, x) == 0) {
		     rgd(z, y, x) = couleur[rgd(z, y - 1, x)];
		  } else {
		     rgd(z, y, x) = MergeLabels(couleur, rgd(z, y - 1, x), rgd(z, y, x));
		  }
	       }
	       if (rgd(z, y, x) == 0) {
		  rgd(z, y, x) = (Ulong)++etiquette;
	       }
	    }
	    
	    for (x = 1; x < ims.Width(); x++) {
	       if ((v = ims(z, y, x)) == 0) {
		  continue;
	       }
	       if (v == ims(z, y - 1, x)) {
		  rgd(z, y, x) = couleur[rgd(z, y - 1, x)];
	       }
	       Propagate26(ims, rgd, x, y, z, 0, couleur);
	       Propagate26(ims, rgd, x, y, z, 1, couleur);
	       Propagate26(ims, rgd, x, y, z, 2, couleur);
	       Propagate26(ims, rgd, x, y, z, 3, couleur);
	       Propagate26(ims, rgd, x, y, z, 4, couleur);
	       Propagate26(ims, rgd, x, y, z, 5, couleur);
	       Propagate26(ims, rgd, x, y, z, 6, couleur);
	       Propagate26(ims, rgd, x, y, z, 7, couleur);
	       Propagate26(ims, rgd, x, y, z, 8, couleur);
	       Propagate26(ims, rgd, x, y, z, 9, couleur);
	       Propagate26(ims, rgd, x, y, z, 10, couleur);
	       Propagate26(ims, rgd, x, y, z, 11, couleur);
	       if (x + 1 < ims.Width()) {
		  Propagate26(ims, rgd, x, y, z, 12, couleur);
	       }
	       
	       if (rgd(z, y, x) == 0) {
		  rgd(z, y, x) = ++etiquette;
	       }
	    }
	 }
      }
      // end 26 -connectivity"
   } else { // 6-connectivity
      // 1th slice.
      z = 0;
      for ( y = 1; y < ims.Height(); y++) {
	 x = 0;
	 if ((v = ims(z, y, x)) != 0) {
	    if (v == ims(z, y - 1, x)) {
	       rgd(z, y, x) = rgd(z, y - 1, x);
	    } else {
	       rgd(z, y, x) = ++etiquette;
	    }
	 } else {
	    rgd(z, y, x) = 0;
	 }
	 for (x = 1; x < ims.Width(); x++) {
	    rgd(z, y, x) = 0;
	    if ((v=ims(z, y, x)) == 0) {
	       continue;
	    }
	    if ( v == ims(z, y - 1, x)) {
	       rgd(z, y, x) = couleur[rgd(z, y - 1, x)];
	    }
	    if (v == ims(z, y, x - 1)) {
	       if (rgd(z, y, x) == 0) {
		  rgd(z, y, x) = couleur[rgd(z, y, x - 1)];
	       } else {
		  rgd(z, y, x) = MergeLabels(couleur, rgd(z, y, x - 1), rgd(z, y, x));
	       }
	    }
	    if (rgd(z, y, x) == 0) {
	       rgd(z, y, x) = ++etiquette;
	    }
	 }
      }
      
      // n-th slices.
      for (z = 1; z < ims.Depth(); z++) {
	 y = x = 0;
	 if ((v = ims(z, y, x))) {
	    if (v == ims(z - 1, y, x)) {
	       rgd(z, y, x) = couleur[rgd(z - 1, y, x)];
	    } else {
	       rgd(z, y, x) = ++etiquette;
	    }
	 } else {
	    rgd(z, y, x) = 0;
	 }
	 
	 // 1th row
	 for (x = 1; x < ims.Width(); x++) {
	    if ((v = ims(z, y, x)) == 0) {
	       continue;
	    }
	    if (v == ims(z - 1, y, x)) {
	       rgd(z, y, x) = couleur[rgd(z - 1, y, x)];
	    } else {
	       rgd(z, y, x) = 0;
	    }
	    if (v == ims(z, y, z - 1)) {
	       if (rgd(z, y, x) == 0) {
		  rgd(z, y, x) = couleur[rgd(z, y, x - 1)];
	       } else {
		 rgd(z, y, x) = MergeLabels(couleur, rgd(z, y, x - 1), rgd(z, y, x));
	       }
	    }
	    if (rgd(z, y, x) == 0) {
	       rgd(z, y, x) = ++etiquette;
	    }
	 }
	 
	 for (y = 1; y < ims.Height(); y++) {
	    x = 0;
	    if ((v = ims(z, y, x))) {
	       if (v == ims(z - 1, y, x)) {
		  rgd(z, y, x) = couleur[rgd(z - 1, y, x)];
	       } else {
		  rgd(z, y, x) = 0;
	       }
	       if (v == ims(z, y - 1, x)) {
		  if (rgd(z, y, x) == 0) {
		     rgd(z, y, x) = couleur[rgd(z, y - 1, x)];
		  } else {
		     rgd(z, y, x) = MergeLabels(couleur, rgd(z, y - 1, x), rgd(z, y, x));
		  }
	       }
	       if (rgd(z, y, x) == 0) {
		  rgd(z, y, x) = (Ulong)++etiquette;
	       }
	    }
	    
	    for (x = 1; x < ims.Width(); x++) {
	       if ((v = ims(z, y, x))) {
		  if (v == ims(z - 1, y, x)) {
		     rgd(z, y, x) = couleur[rgd(z - 1, y, x)];
		  } else {
		     rgd(z, y, x) = 0;
		  }
		  if (v == ims(z, y - 1, x)) {
		     if (rgd(z, y, x) == 0) {
			rgd(z, y, x) = couleur[rgd(z, y - 1, x)];
		     } else {
			rgd(z, y, x) = MergeLabels(couleur, rgd(z, y - 1, x), rgd(z, y, x));
		     }
		  }
		  if (v == ims(z, y, x - 1)) {
		     if (rgd(z, y, x) == 0) {
			rgd(z, y, x) = couleur[rgd(z, y, x - 1)];
		     } else {
			rgd(z, y, x) = MergeLabels(couleur, rgd(z, y, x - 1), rgd(z, y, x));
		     }
		  }
		  if (rgd(z, y, x) == 0) {
		     rgd(z, y, x) = ++etiquette;
		  }
	       }
	    }
	 }
      }
   } // end 6 -connectivity"

   labels = new Ulong[etiquette+1];
   
   Ulong nbreg = 0;

   // Minimize labels   
   for (i = 1; i <= etiquette; i++) {
      if (couleur[i] != i) {
	 labels[i] = labels[couleur[i]];
      } else {
	 labels[i] = ++nbreg;
      }
   }
   
   // Marking
   for (z = 0; z < ims.Depth(); z++) {
      for (y = 0; y < ims.Height(); y++) {
	 for (x = 0; x < ims.Width(); x++) {
	    if (ims(z, y, x)) {
	       rgd(z, y, x) = labels[couleur[rgd(z, y, x)]];
	    } else {
	       rgd(z, y, x) = 0;
	    }
	 }
      }
   }
   
   delete[] labels;
   delete[] couleur;

   return rgd.Labels(nbreg);
}

Errc PLabeling( const Imc3duc &ims, Reg3d &rgd, int connectivity ) {
   Img3dsl image(ims.Props());
   Uchar *pr = ims.Vector(0);
   Uchar *pg = ims.Vector(1);
   Uchar *pb = ims.Vector(2);
   Long *ps = image.Vector();
   Uchar *pend = ims.Vector(0) + ims.VectorSize();

   for ( ; pr < pend ; pr++, pg++, pb++, ps++) {
      *ps = *pb << 16 | *pg << 8 | *pr;
   }
   return PLabeling(image, rgd, connectivity);
}

Errc PLabeling( const Graph2d &gs, Graph2d &gd, int /*connectivity*/ ) {
   Long i, j;
   Graph2d::ValueType tmp;
   Fifo<Point2d> f;
   Point2d pt;
   GEdge *ptr;
   Ulong nbreg = 0;
  
   gd = gs;
   
   for (i = 0; i < gd.Size(); i++) {
      if ((gd[i])) {
	 gd[i]->value = 0;
      }
   }
   for (i = 0; i< gs.Size(); i++) {
      if (gs[i] && (gs[i]->value) && (!gd[i]->value)) {
	 nbreg++;
	 gd[i]->value = (Graph2d::ValueType)nbreg;
	 f.Push(Point2d(i));
	 while (!f.Empty()) {
	    j = (f.Pop()).x;
	    pt = gd[j]->seed;
	    ptr = gd[j]->Neighbours();
	    while (ptr != NULL) {
	       if (!gd[ptr->Node()]->value) {
		  if (gs[ptr->Node()]->value >= gs[i]->value) {
		     tmp = gs[ptr->Node()]->value-gs[i]->value;
		  } else {
		     tmp = gs[i]->value - gs[ptr->Node()]->value;
		  }
		  if (tmp == 0) {
		     gd[ptr->Node()]->value = (Graph2d::ValueType)nbreg;
		     f.Push(Point2d(ptr->Node()));
		  }
	       }
	       ptr = ptr->Next();
	    }
	 }
      }
   }
   for (i = 0; i < gd.Size(); i++) {
      if ((gd[i])) {
	 for (ptr = gd[i]->Neighbours(); ptr != NULL; ptr = ptr->Next()) {
	    if ((gd[i]->value) != (gd[ptr->Node()]->value)) {
	       ptr->weight = 0;
	    }
	 }
      }
   }
   
   if (nbreg>Limits<Graph2d::ValueType>::max()) {
      std::cerr << "Error plabeling: too many regions."<<std::endl;
      return FAILURE;
   }
   return nbreg;
}

Errc PLabeling( const Graph3d &gs, Graph3d &gd, int /*connectivity*/ ) {
   Long i, j;
   Graph3d::ValueType tmp;
   Fifo<Point3d> f;
   Point3d pt;
   GEdge *ptr;
   Ulong nbreg = 0;
  
   gd = gs;
   
   for (i = 0; i < gd.Size(); i++) {
      if ((gd[i])) {
	 gd[i]->value = 0;
      }
   }
   for (i = 0; i< gs.Size(); i++) {
      if (gs[i] && (gs[i]->value) && (!gd[i]->value)) {
	 nbreg++;
	 gd[i]->value = (Graph3d::ValueType)nbreg;
	 f.Push(Point3d(i));
	 while (!f.Empty()) {
	    j = (f.Pop()).x;
	    pt = gd[j]->seed;
	    ptr = gd[j]->Neighbours();
	    while (ptr != NULL) {
	       if (!gd[ptr->Node()]->value) {
		  if (gs[ptr->Node()]->value >= gs[i]->value) {
		     tmp = gs[ptr->Node()]->value-gs[i]->value;
		  } else {
		     tmp = gs[i]->value - gs[ptr->Node()]->value;
		  }
		  if (tmp == 0) {
		     gd[ptr->Node()]->value = (Graph3d::ValueType)nbreg;
		     f.Push(Point3d(ptr->Node()));
		  }
	       }
	       ptr = ptr->Next();
	    }
	 }
      }
   }
   for (i = 0; i < gd.Size(); i++) {
      if ((gd[i])) {
	 for (ptr = gd[i]->Neighbours(); ptr != NULL; ptr = ptr->Next()) {
	    if ((gd[i]->value) != (gd[ptr->Node()]->value)) {
	       ptr->weight = 0;
	    }
	 }
      }
   }
   
   if (nbreg>Limits<Graph3d::ValueType>::max()) {
      std::cerr << "Error plabeling: too many regions."<<std::endl;
      return FAILURE;
   }
   return nbreg;
}


/*
 * Modify only the following constants, and the function call.
 */
#ifdef MAIN
#define	USAGE	"usage: %s connectivity [-m mask] [im_in|-] [rg_out|-]"
#define	PARC	1
#define	FINC	1
#define	FOUTC	1
#define	MASK	2 // Masking.

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
      objd[0] = new Reg2d(ims->Size());
      Reg2d* const rgd = (Reg2d*)objd[0];
      
      result = PLabeling(*ims, *rgd, atoi(parv[0]));
      goto end;
}
   if (objs[0]->Type() == Po_Img2dsl) {
      Img2dsl* const ims = (Img2dsl*)objs[0];
      objd[0] = new Reg2d(ims->Size());
      Reg2d* const rgd = (Reg2d*)objd[0];
      
      result = PLabeling(*ims, *rgd, atoi(parv[0]));
      goto end;
}
   if (objs[0]->Type() == Po_Img2dsf) {
      Img2dsf* const ims = (Img2dsf*)objs[0];
      objd[0] = new Reg2d(ims->Size());
      Reg2d* const rgd = (Reg2d*)objd[0];
      
      result = PLabeling(*ims, *rgd, atoi(parv[0]));
      goto end;
}
   if (objs[0]->Type() == Po_Reg2d) {
      Reg2d* const ims = (Reg2d*)objs[0];
      objd[0] = new Reg2d(ims->Size());
      Reg2d* const rgd = (Reg2d*)objd[0];
      
      result = PLabeling(*ims, *rgd, atoi(parv[0]));
      goto end;
}
   if (objs[0]->Type() == Po_Imc2duc) {
      Imc2duc* const ims = (Imc2duc*)objs[0];
      objd[0] = new Reg2d(ims->Size());
      Reg2d* const rgd = (Reg2d*)objd[0];
      
      result = PLabeling(*ims, *rgd, atoi(parv[0]));
      goto end;
}
   if (objs[0]->Type() == Po_Img3duc) {
      Img3duc* const ims = (Img3duc*)objs[0];
      objd[0] = new Reg3d(ims->Size());
      Reg3d* const rgd = (Reg3d*)objd[0];
      
      result = PLabeling(*ims, *rgd, atoi(parv[0]));
      goto end;
   }
   if (objs[0]->Type() == Po_Img3dsl) {
      Img3dsl* const ims = (Img3dsl*)objs[0];
      objd[0] = new Reg3d(ims->Size());
      Reg3d* const rgd = (Reg3d*)objd[0];
      
      result = PLabeling(*ims, *rgd, atoi(parv[0]));
      goto end;
   }
   if (objs[0]->Type() == Po_Img3dsf) {
      Img3dsf* const ims = (Img3dsf*)objs[0];
      objd[0] = new Reg3d(ims->Size());
      Reg3d* const rgd = (Reg3d*)objd[0];
      
      result = PLabeling(*ims, *rgd, atoi(parv[0]));
      goto end;
   }
   if (objs[0]->Type() == Po_Reg3d) {
      Reg3d* const ims = (Reg3d*)objs[0];
      objd[0] = new Reg3d(ims->Size());
      Reg3d* const rgd = (Reg3d*)objd[0];
      
      result = PLabeling(*ims, *rgd, atoi(parv[0]));
      goto end;
   }
   if (objs[0]->Type() == Po_Imc3duc) {
      Imc3duc* const ims = (Imc3duc*)objs[0];
      objd[0] = new Reg3d(ims->Size());
      Reg3d* const rgd = (Reg3d*)objd[0];
      
      result = PLabeling(*ims, *rgd, atoi(parv[0]));
      goto end;
}
   if (objs[0]->Type() == Po_Graph2d) {
      Graph2d* const grs = (Graph2d*)objs[0];
      objd[0] = new Graph2d(grs->Size());
      Graph2d* const grd = (Graph2d*)objd[0];
      result = PLabeling(*grs, *grd, atoi(parv[0]));
      goto end;
   }
   if (objs[0]->Type() == Po_Graph3d) {
      Graph3d* const grs = (Graph3d*)objs[0];
      objd[0] = new Graph3d(grs->Size());
      Graph3d* const grd = (Graph3d*)objd[0];
      result = PLabeling(*grs, *grd, atoi(parv[0]));
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
