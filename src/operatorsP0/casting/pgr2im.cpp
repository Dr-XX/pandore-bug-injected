/* -*- c-basic-offset: 3 -*-
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
 * @author François Angot - 1997-07-18
 * @author Regis Clouard - 2001-04-11 (version 3.00)
 */

#include <pandore.h>
using namespace pandore;

/**
 * @file pgr2im.cpp
 * @brief Cast a graph to an image.
 */

/*      
 * Trace une droite de pixels entre 2 points, avec la couleur donnee.
 */
void TraceDroite( Img2dsf &ims, const Point2d pti, const Point2d ptf, float coul ) {
   int i, j, max, min;
   float pente;
   
   if (pti.x != ptf.x) {
      pente = (float)(pti.y - ptf.y) / (pti.x - ptf.x);
   } else {
      pente = MAXFLOAT;
   }

   if ((pente < -1) || (pente > 1)) {
      if (ptf.y < pti.y) {
	 min = ptf.y; max = pti.y;
      } else {
	 min = pti.y; max = ptf.y;
      }
      for (i = max; i >= min; i--) {
	 j = (int)((i - ptf.y) / pente + ptf.x);
	 ims[i][j] = coul;
      }
   } else {
      if (ptf.x < pti.x) {
	 min = ptf.x; max = pti.x;
      } else {
	 min = pti.x; max = ptf.x;
      }
      for (j = max; j >= min; j--) {
	 i = (int)(pente * (j - ptf.x) + ptf.y);
	 ims[i][j] = coul;
      }
   }
}

/*
 * Representation des sommets non nuls.
 * Trace un germe et l'ensemble des arcs avec ses voisins dans le graphe.
 */
Errc PGr2Im( const Graph2d &g, Img2dsf &imd, int type ) {
   register int i;
   float val;
  
   switch(type) {

   case 2:
      for (i = 0; i < g.Size(); i++) {
	 if (g[i] && (val = (float)g[i]->value)) {
	    imd[g[i]->seed.y][g[i]->seed.x] = val;
	    for (GEdge *ptr = g[i]->Neighbours(); ptr != NULL; ptr = ptr->Next()) {
	       if (g[ptr->Node()]->value == val) {
		  TraceDroite(imd, g[i]->seed, g[ptr->Node()]->seed, (float)val);
	       }
	    }
	 }
      }
      break;

   case 3:
      for (i = 0; i < g.Size(); i++) {
	 if (g[i] &&(val = (float)g[i]->value)) {
	    imd[g[i]->seed.y][g[i]->seed.x] = val;
	    for (GEdge *p = g[i]->Neighbours(); p != NULL; p = p->Next()) {
	       if (p->weight) {
		  TraceDroite(imd, g[i]->seed, g[p->Node()]->seed, (float)p->weight);
	       }
	    }
	 }
      }
      break;

   default:
      for (i = 0; i < g.Size(); i++) {
	 if (g[i] && (val = (float)g[i]->value)) {
	    imd[g[i]->seed] = val;
	    for (GEdge *p = g[i]->Neighbours(); p != NULL; p = p->Next()) {
	       if ((g[p->Node()]) && (g[p->Node()]->value)) {
		  TraceDroite(imd, g[i]->seed, g[p->Node()]->seed, (float)val);
	       }
	    }
	 }
      }
      break;
   }
   
   return SUCCESS;
}

void TraceDroite( Img3dsf &imd, Point3d pti, Point3d ptf, float coul ) {
   Long xmin, ymin, zmin;
   Long xmax, ymax, zmax;
   Long i, j, k;
   Long d;
   float penteyx, pentezx;
   float penteyz;
  
   if (pti.x == ptf.x) {
      penteyx = pentezx = MAXFLOAT;
      xmin = xmax = ptf.x;
   } else {
      d = ptf.x - pti.x;
      penteyx = (float)(ptf.y - pti.y) / d;
      pentezx = (float)(ptf.z - pti.z) / d;
      if (pti.x < ptf.x) { 
	 xmin = pti.x; xmax = ptf.x;
      } else {
	 xmin = ptf.x; xmax = pti.x;
      }
   }
   if (pti.y == ptf.y) {
      penteyx = penteyz = 0.0;
      ymin = ymax = ptf.x;
   } else {
      if (pti.y < ptf.y) {
	 ymin = pti.y; ymax = ptf.y;
      } else {
	 ymin = ptf.y; ymax = pti.y;
      }
   }
   if (pti.z == ptf.z) {
      penteyz = MAXFLOAT;
      pentezx = 0.0;
      zmin = zmax = ptf.z;
   } else {
      penteyz = (float)(ptf.y - pti.y) / (ptf.z - pti.z);
      if (pti.z < ptf.z) {
	 zmin = pti.z; zmax = ptf.z; 
      } else { 
	 zmin = ptf.z; zmax = pti.z;
      }
   }
  
   if ((pentezx < -1) || (pentezx > 1)) {
      if ((penteyx < -1)||(penteyx > 1)) {
	 if (((pentezx<-1) && (penteyx<pentezx)) || ((pentezx>1) && (penteyx>pentezx))) {
	    for (j = ymin; j <= ymax; j++) {
	       d = j - ptf.y;
	       k = (int)(d/penteyx + ptf.x);
	       i = (int)(d/penteyz + ptf.z);
	       imd[i][j][k] = coul;
	    }
	 } else {
	    for (i = zmin; i <= zmax; i++) {
	       d = i - ptf.z;
	       j = (int)(d*penteyz+ptf.y);
	       k = (int)(d/pentezx+ptf.x);
	       imd[i][j][k] = coul;
	    }
	 }
      } else {
	 for (k = xmin; k <= xmax; k++) {
	    d = k  -ptf.x;
	    j = (int)(d*penteyx+ptf.y);
	    i = (int)(d*pentezx+ptf.z);
	    imd[i][j][k] = coul;
	 }
      }
   } else {
      if ((penteyx<-1) || (penteyx>1)) {
	 for (j = ymin; j <= ymax; j++) {
	    d = j - ptf.y;
	    k = (int)(d/penteyx+ptf.x);
	    i = (int)(d/penteyz+ptf.z);
	    imd[i][j][k] = coul;
	 }
      } else {
	 for (k = xmin; k <= xmax; k++) {
	    d = k - ptf.x;
	    j = (int)(d*penteyx+ptf.y);
	    i = (int)(d*pentezx+ptf.z);
	    imd[i][j][k] = coul;
	 }
      }
   }
}

Errc PGr2Im( const Graph3d &g, Img3dsf &imd, int type ) {
   Long i;
   float val;
   
   switch(type) {
      
   case 2:
      for (i = 0; i < g.Size(); i++) {
	 if (g[i] && (val = (float)g[i]->value)) {
	    imd[g[i]->seed.z][g[i]->seed.y][g[i]->seed.x] = val;
	    for (GEdge *ptr = g[i]->Neighbours(); ptr != NULL; ptr = ptr->Next()) {
	       if (g[ptr->Node()]->value == val) {
		  TraceDroite(imd, g[i]->seed, g[ptr->Node()]->seed, val);
	       }
	    }
	 }
      }
      break;

   case 3:
      for (i = 0; i < g.Size(); i++) {
	 if (g[i] && (val = (float)g[i]->value)) {
	    imd[g[i]->seed.z][g[i]->seed.y][g[i]->seed.x] = val;
	    for (GEdge *p = g[i]->Neighbours(); p != NULL; p = p->Next()) {
	       if (p->weight) {
		  TraceDroite(imd, g[i]->seed, g[p->Node()]->seed, (float)p->weight);
	       }
	    }
	 }
      } 
      break;

   default:
      for (i = 0; i < g.Size(); i++) {
	 if (g[i] && (val = (float)g[i]->value)) {
	    imd[g[i]->seed.z][g[i]->seed.y][g[i]->seed.x] = val;
	    for (GEdge *p = g[i]->Neighbours(); p != NULL; p = p->Next()) {
	       if (g[p->Node()]->value) {
		  TraceDroite(imd, g[i]->seed, g[p->Node()]->seed, (float)val);
	       }
	    }
	 }
      }
      break;
   }
  
   return SUCCESS;
}

#ifdef MAIN

/*
 * Modify only the following constants, and the function call.
 */
#define	USAGE	"usage: %s type [-m mask] [gr_in|-] [im_out|-]"
#define	PARC	1
#define	FINC	1
#define	FOUTC	1
#define	MASK	1

int main( int argc, char *argv[] ) {
   Errc result;              // The result code of the execution.
   Pobject* mask;         // The region mask.
   Pobject* objin[FINC+1];   // The input objects.
   Pobject* objs[FINC+1];    // The source objects masked by the mask.
   Pobject* objout[FOUTC+1]; // The output object.
   Pobject* objd[FOUTC+1];   // The result object of the execution.
   char* parv[PARC+1];       // The input parameters.

   ReadArgs(argc, argv, PARC, FINC, FOUTC, &mask, objin, objs, objout, objd, parv, USAGE, MASK);

   switch (objs[0]->Type()) {

      case Po_Graph2d : {
	 Graph2d* const gs = (Graph2d*)objs[0];
	 objd[0] = new Img2dsf(gs->ImageSize());
	 Img2dsf* const imd = (Img2dsf*)objd[0];

	 result = PGr2Im(*gs, *imd, atoi(parv[0]));
      } break;

      case Po_Graph3d : {
	 Graph3d* const gs = (Graph3d*)objs[0];
	 objd[0] = new Img3dsf(gs->ImageSize());
	 Img3dsf* const imd = (Img3dsf*)objd[0];

	 result = PGr2Im(*gs, *imd, atoi(parv[0]));
      } break;

      default :
	 PrintErrorFormat(objin, FINC);
	 result = FAILURE;
      }

   if (result) {
      WriteArgs(argc, argv, PARC, FINC, FOUTC, &mask, objin, objs, objout, objd, MASK);
   }

   Exit(result);
   return 0;
}

#endif
