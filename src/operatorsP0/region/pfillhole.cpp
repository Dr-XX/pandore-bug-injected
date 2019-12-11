/*  -*- mode: c++; c-basic-offset: 3 -*-
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
 * @author Régis Clouard - 1999-02-19
 * @author Régis Clouard - 2001-11-07
 * @author Régis Clouard - 2004-06-01 (3D)
 * @author Régis Clouard - 2008-09-01 (add connexity)
 * @author Régis Clouard - May 03, 2012 (fix bug on border)
 */

#include <pandore.h>
using namespace pandore;

/**
 * @file pfillhole.cpp	
 * Fill holes in regions.
 * A hole is a background region only surrounded by ONE region.
 * The image border is not taken into account, 
 * otherwise an image with no region would be filled.
 */

/**
 * Merges the values of two labels l1, l2 and
 * propagates the minimum.
 */
void merge( Reg2d::ValueType *t, Reg2d::ValueType l1, Reg2d::ValueType l2 ) {
   if (l1 == l2 || t[l1] == t[l2]) {
      return;
   }
   Reg2d::ValueType inf, sup, m;

   if (t[l1] > t[l2]) {
      sup = l1;
      inf = l2;
   } else {
      sup = l2;
      inf = l1;
   }
   
   while (t[sup] != t[inf]) {
      m = t[sup];
      t[sup] = t[inf];
      sup = m;
   }
   t[0] = 0;
}

/**
 * @param connexity the component connexity (4 or 8).
 */
Errc PFillHole( const Reg2d &rgs, Reg2d &rgd , int connexity) {
   if (connexity != 4 && connexity != 8 ) {
      std::cerr << "Error pfillhole: bad connexity value:" << connexity << std::endl;
   }
   int x, y;
   Reg2d::ValueType i, j;
   Reg2d::ValueType *couleur;
   Ulong* labels;
   Long label;
   Reg2d::ValueType etiquette = 0;
   Ulong nbreg;
   
   const Reg2d::ValueType maxetiquette = (rgs.VectorSize() + 1) / 4; // Estimated maximum for 8-connexity.
   couleur = new Reg2d::ValueType[maxetiquette];
   
   // Initialization of the lut.
   for (i = 0; i < maxetiquette; i++) {
      couleur[i] = i;
   }

   Img2dsl imi(rgs.Size());
   imi = 0;

   // Labelization of the background
   // using the specified connexity.
   // The borders keep the same label than in the input map.
   if (connexity == 4) {
      for (y = 1; y < imi.Height() - 1; y++) {
	 for (x = 1; x < imi.Width() - 1; x++) {
	    if (rgs(y, x) != 0) {
	       continue;
	    }
	    label = -1;
	    for (int v = 0; v < 2; v++) {
	       if (rgs(y+v4[v].y, x+v4[v].x)!=0) {
		  continue;
	       }
	       if (label < 0) {
		  label = imi(y, x) = couleur[imi(y + v4[v].y, x + v4[v].x)];
	       } else {
		  merge(couleur, imi(y+v4[v].y, x + v4[v].x), imi(y, x));
	       }
	    }
	    if (label < 0) {
	       imi(y, x) = ++etiquette;
	    }
	 }
      }
   } else {
      for (y = 1; y < imi.Height() - 1; y++) {
	 for (x = 1; x < imi.Width() - 1; x++) {
	    if (rgs(y, x) != 0) {
	       continue;
	    }
	    label = -1;
	    for (int v = 0; v < 4; v++) {
	       if (rgs(y+v8[v].y, x+v8[v].x)!=0) {
		  continue;
	       }
	       if (label < 0) {
		  label = imi(y, x) = couleur[imi(y + v8[v].y, x + v8[v].x)];
	       } else {
		  merge(couleur, imi(y + v8[v].y, x + v8[v].x), imi(y, x));
	       }
	    }
	    if (label < 0) {
	       imi(y, x) = ++etiquette;
	    }
	 }
      }
   }

   labels = new Ulong[etiquette + 1];

   // Merges colors with the same value.
   label = 0;
   for (i = 1; i <= etiquette; i++) {
      if (couleur[i] != i) {
	 for (j = 0; j <= etiquette; j++) {
	    if (couleur[j] == i) {
	       couleur[j] = couleur[i];
	    }
	 }
      } else {
	 labels[i] = ++label;
      }
   }

   // Discards regions that touch the border.
   for (x = 0; x < imi.Width(); x++) {
      labels[couleur[imi(1, x)]] = 0;
      labels[couleur[imi(imi.Height() - 2, x)]] = 0;
   }
   for (y = 0; y < imi.Height(); y++) {
      labels[couleur[imi(y, 1)]] = 0;
      labels[couleur[imi(y, imi.Width() - 2)]] = 0;
   }

   // Merges regions.
   for (y = 0; y < imi.Height(); y++) {
      for (x = 0; x < imi.Width(); x++) {
	 if (rgs(y, x) == 0) {
	    imi(y, x) = labels[couleur[imi(y, x)]];
	 } else {
	    imi(y, x) = 0;
	 }
      }
   }

   for (i = 0; i < maxetiquette; i++) {
      couleur[i] = 0;
   }

   for (i = 0; i <= etiquette; i++) {
      labels[i] = 0;
   }

   // Computes the number of neighbours of each region
   // and keeps only those with one neighbour.

   for (y = 0; y < imi.Height(); y++) {
      for (x = 0; x < imi.Width(); x++) {
	 if (imi(y, x) != 0) {  // WARNING: border must be set to 0.
	    for (int v = 0; v < 4; v++) {
	       if (rgs[y+v4[v].y][x+v4[v].x] != 0) {
		  if (couleur[imi(y, x)] == 0) {
		     couleur[imi(y, x)] = rgs[y + v4[v].y][x + v4[v].x];
		     labels[imi(y, x)]++;
		  } else {
		     if (couleur[imi(y, x)] != rgs[y + v4[v].y][x + v4[v].x]) {
			labels[imi(y, x)] ++;
		     }
		  }
	       }
	    }
	 }
      }
   }
   rgd = rgs;
   for (y = 0; y < imi.Height(); y++) {
      for (x = 0; x < imi.Width(); x++) {
	 if (labels[imi(y, x)] == 1) {
	    rgd(y, x) = couleur[imi(y, x)];
	 }
      }
   }
   // Computes the number of merged regions.
   for (nbreg = 0, i = 0; i <= etiquette; i++) {
      if (labels[i] == 1) {
	 nbreg++;
      }
   }
   delete[] couleur;
   delete[] labels;
   return nbreg;
}

/**
 * @param connexity the component connexity (6 or 26).
 */
Errc PFillHole( const Reg3d &rgs, Reg3d &rgd, int connexity ) {
   Point3d pt;
   int x, y, z;
   Reg3d::ValueType i, j;
   Reg3d::ValueType* couleur;
   Ulong* labels;
   Reg3d::ValueType etiquette=0;
   int label;
   Ulong nbreg;
   int v;
   
   if ( connexity != 6 && connexity != 26 ) {
      std::cerr << "Error pfillhole: bad connexity value:" << connexity << std::endl;
   }
   
   Img3dsl imi(rgs.Size());
   const Reg3d::ValueType maxetiquette=(rgs.VectorSize()+1)/8; // Estimated maximum of label number.
   couleur= new Reg3d::ValueType[maxetiquette];
   
   // Initialization of the lut.
   for (i=0;i<maxetiquette; i++) {
      couleur[i] = i;
   }
   imi=0;
   
   // Labelization of connex components of the background
   // using the specified connexity.
   // The borders keep the same label than in the input map.
   if (connexity == 6) {
      for (pt.z=1;pt.z<imi.Depth()-1;pt.z++) {
	 for (pt.y=1;pt.y<imi.Height()-1;pt.y++) {
	    for (pt.x=1;pt.x<imi.Width()-1;pt.x++) {
	       if (rgs[pt] == 0) { // background
		  label = -1;
		  for (v=0; v<3; v++) { // Already visited neighbours
		     if (rgs[pt+v6[v]]==0) {
			if (label < 0) {
			   label = imi[pt] = couleur[imi[pt+v6[v]]];
			} else {
			   merge(couleur, imi[pt+v6[v]], imi[pt]);
			}
		     }
		  }
		  if (label < 0) {
		     imi[pt] = ++etiquette;
		  }
	       }
	    }
	 }
      }
   } else {
      for (pt.z=1;pt.z<imi.Depth()-1;pt.z++) {
	 for (pt.y=1;pt.y<imi.Height()-1;pt.y++) {
	    for (pt.x=1;pt.x<imi.Width()-1;pt.x++) {
	       if (rgs[pt] == 0) { // background
		  label = -1;
		  for (v=0; v<13; v++) { // Already visited neighbours
		     if (rgs[pt+v26[v]]==0) {
			if (label < 0) {
			   label = imi[pt] = couleur[imi[pt+v26[v]]];
			} else {
			   merge(couleur, imi[pt+v26[v]], imi[pt]);
			}
		     }
		  }
		  if (label < 0) {
		     imi[pt] = ++etiquette;
		  }
	       }
	    }
	 }
      }
   }
   
   
   // Merges colors.
   labels= new Ulong[etiquette+1];
   label = 0;
   labels[0]=0;

   for (i=1;i<=etiquette; i++) {
      if (couleur[i] != i) {
	 for (j=1;j<=etiquette; j++) {
	    if (couleur[j] == i) {
	       couleur[j] = couleur[i];
	    }
	 }
      } else {
	 labels[i] = ++label;
      }
   }
   // Discards regions that touch the border.
   for (z=0;z<imi.Depth();z++) {
      for (y=0;y<imi.Height();y++) {
	 labels[couleur[imi(z, y, 0)]] = 0;
	 labels[couleur[imi(z, y, imi.Width()-1)]] = 0;
      }
   }
   for (z=0;z<imi.Depth();z++) {
      for (x=1;x<imi.Width()-1;x++) {
	 labels[couleur[imi(z, 0, x)]] = 0;
	 labels[couleur[imi(z, imi.Height()-1, x)]] = 0;
      }
   }
   for (y=1;y<imi.Height()-1;y++) {
      for (x=1;x<imi.Width()-1;x++) {
	 labels[couleur[imi(0, y, x)]] = 0;
	 labels[couleur[imi(imi.Depth()-1, y, x)]] = 0;
      }
   }
   // Merges regions.
   for (z=0;z<imi.Depth();z++) {
      for (y=0;y<imi.Height();y++) {
	 for (x=0;x<imi.Width();x++) {
	    if (rgs(z, y, x)==0) {
	       imi(z, y, x) = labels[couleur[imi(z, y, x)]];
	    } else {
	       imi(z, y, x)=0;
	    }
	 }
      }
   }
   rgd.Labels(etiquette);
   for (pt.z=0;pt.z<imi.Depth();pt.z++) {
      for (pt.y=0;pt.y<imi.Height();pt.y++) {
	 for (pt.x=0;pt.x<imi.Width();pt.x++) {
	    if (rgs[pt]==0) {
	       rgd[pt]=imi[pt];
	    } else {
	       rgd[pt]=rgs[pt];
	    }
	 }
      }
   }
   for (i=0;i<maxetiquette;i++) {
      couleur[i] = 0;
   }

   for (i=0;i<=etiquette;i++) {
      labels[i] = 0;
   }
      
   // Computes the number of neighbours of each region
   // and keeps only those with one neighbour.
   for (z=0;z<imi.Depth();z++) {
      for (y=0;y<imi.Height();y++) {
	 for (x=0;x<imi.Width();x++) {
	    if (imi(z, y, x)!=0) { // WARNING: border must be set to 0.
	       for (v=0; v<26; v++) {
		  if (rgs(z+v26[v].z, y+v26[v].y, x+v26[v].x) != 0) {
		     if (couleur[imi(z, y, x)] == 0) {
			couleur[imi(z, y, x)] = rgs(z+v26[v].z, y+v26[v].y, x+v26[v].x);
			labels[imi(z, y, x)]++;
		     } else {
			if (couleur[imi(z, y, x)] != rgs(z+v26[v].z, y+v26[v].y, x+v26[v].x)) {
			   labels[imi(z, y, x)] ++;
			}
		     }
		  }
	       }
	    }
	 }
      }
   }
   rgd = rgs;
   for (z=0;z<imi.Depth();z++) {
      for (y=0;y<imi.Height();y++) {
	 for (x=0;x<imi.Width();x++) {
	    if (labels[imi(z, y, x)] == 1) {
	       rgd(z, y, x) = couleur[imi(z, y, x)];
	    }
	 }
      }
   }
   // Computes the number of merged regions.
   for (nbreg=0, i=0;i<=etiquette;i++) {
      if (labels[i] == 1) {
	 nbreg++;
      }
   }
   delete[] couleur;
   delete[] labels;
   return nbreg;
}

#ifdef MAIN

/*
 * Modify only the following constants, and the function call.
 */
#define USAGE	"usage: %s connexity [-m mask] [rg_in|-] [rg_out|-]"
#define PARC	1
#define FINC	1
#define FOUTC	1
#define MASK	1

int main( int argc, char *argv[] ) {
   Errc result;              // The result code of the execution.
   Pobject* mask;            // The region mask.
   Pobject* objin[FINC+1];   // The input objects.
   Pobject* objs[FINC+1];    // The source objects masked by the mask.
   Pobject* objout[FOUTC+1]; // The output object.
   Pobject* objd[FOUTC+1];   // The result object of the execution.
   char* parv[PARC+1];       // The input parameters.

   ReadArgs(argc, argv, PARC, FINC, FOUTC, &mask, objin, objs, objout, objd, parv, USAGE, MASK);

   switch(objs[0]->Type()) {

   case Po_Reg2d :{
      Reg2d* const rgs = (Reg2d*)objs[0];
      objd[0] = new Reg2d(rgs->Props());
      Reg2d* const rgd = (Reg2d*)objd[0];

      result = PFillHole(*rgs, *rgd, atoi(parv[0]));
   } break;

   case Po_Reg3d :{
      Reg3d* const rgs = (Reg3d*)objs[0];
      objd[0] = new Reg3d(rgs->Props());
      Reg3d* const rgd=(Reg3d*)objd[0];

      result = PFillHole(*rgs, *rgd, atoi(parv[0]));
   } break;

   default:
      PrintErrorFormat(objin, FINC);
      result = FAILURE;
   }

   if (result) {
      WriteArgs(argc, argv, PARC, FINC, FOUTC, &mask, objin, objs, objout, objd, MASK);
   }

   Exit(result);
   return(0);
}

#endif
