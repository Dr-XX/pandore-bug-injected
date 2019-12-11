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
 * @author Régis Clouard - 1995-11-29
 */

#include <pandore.h>
using namespace pandore;

/**
 * @file pvoronoi.cpp
 * Calcul le diagramme de voronoi discret, 
 * en propageant la distance du chanfrein.
 */

// Distance de Chamfrein.
int dist[9] = { 0,  5,  5,  7,  7, 11, 11, 11, 11};

int m_y[9] = { 0, -1,  0, -1,  1, -2,  2, -1,  1};
int m_x[9] = { 0,  0, -1, -1, -1, -1, -1, -2, -2};

Errc PVoronoi( const Reg2d &rgs, Reg2d &rgd, Img2dsl &imd ) {
   register int x, y, v;
   Short v_min;
   Long min, loc;
   Point2d pt;
   
   // Meme nombre de labels en sortie.
   rgd.Labels(rgs.Labels());
  
   // Initialisation des germes
   for (y = 0; y < rgd.Height(); y++) {
      for (x = 0; x < rgd.Width(); x++) {
	 imd[y][x] = (rgs[y][x] != 0) ? 0 : MAXLONG;
	 rgd[y][x] = rgs[y][x];
      }
   }
   /* premier passage sur l'image de distance */
   /* passage video    */
   for (x = 0; x < rgd.Width(); x++) {
      for (y = 0; y < rgd.Height(); y++) {
	 v_min = 0;
	 min = MAXLONG;
	 for (v = 0; v < 9; v++) {
	    pt.y = y + m_y[v];
	    pt.x = x + m_x[v];
	    if (rgd.Hold(pt) && (rgd[pt] != 0) && ((loc = imd[pt] + dist[v]) < min)) {
	       v_min = v;
	       min = loc;
	    }
	 }
	 imd[y][x] = min;
	 rgd[y][x] = rgd[y + m_y[v_min]][x + m_x[v_min]];
      }
   }
   /* deuxieme passage sur l'image de distance */
   /* passage antivideo     */
   for (x = rgd.Width() - 1; x >= 0; x--) {
      for (y = rgd.Height() - 1; y >= 0; y--) {
	 v_min = 0;
	 min = MAXLONG;
	 for (v = 0; v < 9; v++) {
	    pt.x = x - m_x[v];
	    pt.y = y - m_y[v];
	    if (rgd.Hold(pt) && (rgd[pt] != 0) && ((loc = imd[pt] + dist[v]) < min)) {
	       v_min = v;
	       min = loc;
	    }
	 }
	 imd[y][x] = min;
	 rgd[y][x] = rgd[y - m_y[v_min]][x - m_x[v_min]];
      }
   }
   return rgs.Labels();
}

Ulong Distance( Point3d p1, Point3d p2 ) {
   Ulong temp=0;
  
   temp += (p1.x - p2.x) * (p1.x - p2.x);
   temp += (p1.y - p2.y) * (p1.y - p2.y);
   temp += (p1.z - p2.z) * (p1.z - p2.z);
   return(temp);
}

Errc PVoronoi( const Reg3d &rgs, Reg3d &rgd, Img3dsl &imd ) {
   Long x, y, z, nb = 0;
   Long dist, mindist = 0;
   Long a, b, c;
   Ulong u, v;
   char vs, nv;
   Ulong mincoul = 0;
   Img3duc imt(rgs.Size()); /* point traite ou non */
   Img3dsl imn(rgs.Size()); /* numero de point source */
   Point3d point, pt, minpt, *pttab;
   Ulong *coultab;
  
   // File Ordonnee de taille maximale
   OrderedFifo< Point3d > F(rgs.Depth() * rgs.Depth() + rgs.Height() * rgs.Height() + rgs.Width() * rgs.Width());
  
   // Nombre de point pour chaque region d'entree
   rgd = 0;  
   rgd.Labels(rgs.Labels());
  
   // Initialisation de l'image de couleurs
   for (z = 0; z < rgs.Depth(); z++) {
      for (y = 0; y < rgs.Height(); y++) {
	 for (x = 0; x < rgs.Width(); x++) {
	    imt[z][y][x] = 0;
	    imn[z][y][x] = 0;
	 }
      }
   }
  
   // Comptage des points de l'image d'entree
   // Recherche des centres de gravite des regions d'entree??
   // Lien des sommets correspondants a des regions adjascentes??
   nb = 0;
   for (z = 0; z < rgs.Depth(); z++) { {
      for (y = 0; y < rgs.Height(); y++)
	 for (x = 0; x < rgs.Width(); x++) {
	    if (rgs[z][y][x]) {
	       nb++;
	       imn[z][y][x] = nb;
	    }
	 }
      }
   }
  
   // Tableau des points des bords des regions d'entree
   // Tableau des numeros de couleurs
   pttab = (Point3d*)calloc(nb + 1, sizeof(Point3d));
   if (pttab == NULL) {
      std::cerr << "Error pvoronoi: Not enough memory." << std::endl;
      return FAILURE;
   }
   coultab = (Ulong*)calloc(nb + 1, sizeof(Ulong));
   if (coultab == NULL) {
      std::cerr << "Error pvoronoi: Not enough memory." << std::endl;
      return FAILURE;
   }

   // Initialisation de la pile avec les points des bords
   nb = 0;
   for (z = 0; z < rgs.Depth(); z++) {
      for (y = 0; y < rgs.Height(); y++) {
	 for (x = 0; x < rgs.Width(); x++) {
	    if ((u = rgs[z][y][x])) {
	       nb++;
	       coultab[nb] = u;
	       pttab[nb] = Point3d(z, y, x);
	       imt[z][y][x] = 2;
	       imd[z][y][x] = 0;
	       rgd[z][y][x] = u;
	    } else {
	       imt[z][y][x] = 0;
	       imd[z][y][x] = MAXLONG;
	       point = Point3d(z, y, x);
	       vs = 0;
	       for (nv = 0; nv < 6; nv++) {
		  a = z + v6z[(int)nv];
		  b = y + v6y[(int)nv];
		  c = x + v6x[(int)nv];
		  if ((rgs.Hold(a, b, c)) && rgs[a][b][c]) {
		     pt = Point3d(a, b, c);
		     dist = Distance(point, pt);
		     if ((!vs) || ((vs) && (dist < mindist))) {
			minpt = pt;
			mindist = dist;
			mincoul = imn[a][b][c];
		     }
		     vs++;
		  }
	       }
	       if (vs) {
		  if (!imt[z][y][x]) {
		     imn[z][y][x] = mincoul;
		     imt[z][y][x] = 2;
		     imd[z][y][x] = 1;
		     F.Push(Point3d(z, y, x), 1);
		  } else {
		     if (mindist<imd[z][y][x]) {
			imn[z][y][x] = mincoul;
			imd[z][y][x] = 1;
			F.Push(Point3d(z, y, x), 1);
		     }
		  }
	       }
	    }
	 }
      }
   }
   // Croissance des nouvelles regions
   // (initialement reduites aux points des bords)
   while (!F.Empty()) {
      pt = F.Pop();
      z = pt.z;
      y = pt.y;
      x = pt.x;
      imt[z][y][x] = 2;
      v = imn[z][y][x];
      pt = pttab[v];
      u = imn[pt.z][pt.y][pt.x];
      v = rgd[z][y][x];
      point = pt;
      if (v == 0) {
	 imn[z][y][x] = u;
	 for (nv = 0; nv < 6; nv++) {
	    a = z + v6z[(int)nv];
	    b = y + v6y[(int)nv];
	    c = x + v6x[(int)nv];
	    if (rgs.Hold(a, b, c)) {
	       if (!imt[a][b][c]) {
		  pt = Point3d(a, b, c);
		  dist = Distance(point, pt);
		  if (dist < imd[a][b][c]) {
		     imd[a][b][c] = dist;
		     imn[a][b][c] = imn[z][y][x];
		     F.Push(Point3d(a, b, c), dist);
		  }
	       } else {
		  u = rgd[z][y][x];
		  v = rgd[a][b][c];
	       }
	       if ((v = imn[z][y][x])) {
		  rgd[z][y][x] = coultab[v];
	       }
	    }
	 }
      }
   }

   free(pttab);
   free(coultab);
  
   return rgs.Labels();
}

#ifdef MAIN

/*
 * Modify only the following constants, and the function call.
 */
#define	USAGE	"usage: %s [rg_in|-] [rg_out|-] [im_out|-]"
#define	PARC	0
#define	FINC	1
#define	FOUTC	2
#define	MASK	0

int main( int argc, char *argv[] ) {
   Errc result;
   Pobject* mask;
   Pobject* objin[FINC + 1];
   Pobject* objs[FINC + 1];
   Pobject* objout[FOUTC + 1];
   Pobject* objd[FOUTC + 1];
   char* parv[PARC + 1];
  
   ReadArgs(argc, argv, PARC, FINC, FOUTC, &mask, objin, objs, objout, objd, parv, USAGE, MASK); 
  
   switch(objs[0]->Type()) {
   case Po_Reg2d : {
      Reg2d* const rgs = (Reg2d*)objs[0];
    
      objd[0] = new Reg2d(rgs->Size());
      Reg2d* const rgd = (Reg2d*)objd[0];
    
      objd[1] = new Img2dsl(rgs->Size());
      Img2dsl* const imd = (Img2dsl*)objd[1];
    
      result = PVoronoi(*rgs, *rgd, *imd);
   }break;
   case Po_Reg3d : {
      Reg3d* const rgs = (Reg3d*)objs[0];
    
      objd[0] = new Reg3d(rgs->Size());
      Reg3d* const rgd = (Reg3d*)objd[0];
    
      objd[1] = new Img3dsl(rgs->Size());
      Img3dsl* const imd = (Img3dsl*)objd[1];
    
      result = PVoronoi(*rgs, *rgd, *imd);
   }break;
   default:
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
