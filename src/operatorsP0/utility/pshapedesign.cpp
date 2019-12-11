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
 * @author Jean-Marie Janik - 1997-04-01
 * @author Regis Clouard - 2001-05-18
 * @author Regis Clouard - 2003-01-02 (parallelipede)
 */

#include <math.h>
#include <pandore.h>
using namespace pandore; 

/**
 * @file pshapedesign.cpp
 * Creates a new image with a specified shape.
 */

Long distance ( Long z, Long y, Long x, Long c, Long b, Long a ) {
   float res; 
   
   res = (float)(z - c) * (z - c) + (y - b) * (y - b) + (x - a)* ( x - a); 
   return (Long) floor(sqrt(res)); 
}

void Distance_26( Img3duc &imt, Ushort rayon ) {
   unsigned short x, y, z, v, compt1 = 0; 
   Long a, b, c; 
   const long ini = 5000; 
   long distance = 0, test; 
   Fifo<Point3d> liste, surface; 
   Point3d p; 
   
   Img3dsl imd(imt.Size()); 
   
   // Initialisation des images.
   imd = ini; 
   
   // Recherche de la surface f(P) = 0
   for (z = 0; z<imt.Depth(); z++) {
      for (y = 0; y<imt.Height(); y++) {
	 for (x = 0; x<imt.Width(); x++) {
	    if (imt[z][y][x] == MAXUCHAR) {   // On selectionne les pts
	       liste.Push(Point3d(z, y, x)); // appartenant a la c_con
	    }
	 }
      }
   }
   
   while (!liste.Empty()) {
      p = liste.Pop(); 
      test = 0; 
      for (v = 0; v < 26; v++) {
	 c = p.z + v26[v].z; 
	 b = p.y + v26[v].y; 
	 a = p.x + v26[v].x; 
	 if ((imt.Hold(c, b, a)) && (imt[c][b][a] == 0)) {
	    test = 1; // il existe 1 voisin = 0
	 }
      }
      if (test == 1) {	
	 surface.Push(p); 
	 compt1 += 1; 
	 imd[p.z][p.y][p.x] = distance; 
      }
   }
   
   // Evolution de la surface.
   while (!surface.Empty()) {
      p = surface.Pop(); 
      if (imd[p.z][p.y][p.x]<rayon) {
	 distance = imd[p.z][p.y][p.x] + 1; 
	 for (v = 0; v < 26; v++) {
	    c = p.z + v26[v].z; 
	    b = p.y + v26[v].y; 
	    a = p.x + v26[v].x; 
	    if ((imt.Hold(c, b, a))&&(imd[c][b][a] == ini)) {
	       surface.Push(Point3d(c, b, a)); 	       
	       imd[c][b][a] = distance; 
	    }
	 }
      } else {
	 while (!surface.Empty()) {
	    p = surface.Pop(); 
	 }
      }
   }
   
   for (z = 0; z<imt.Depth(); z++) {
      for (y = 0; y<imt.Height(); y++) {
	 for (x = 0; x<imt.Width(); x++) {
	    if (imd[z][y][x] != ini) {
	       imt[z][y][x] = MAXUCHAR; 
	    }
	 }
      }
   }
}

// Calcul distance 6 voisins.
void Distance_6( Img3duc &imt, Ushort rayon ) {
   unsigned short i, j, k, v, compt1 = 0; 
   Long a, b, c; 
   const long ini = 500; 
   long distance = 0, test; 
   Fifo<Point3d> liste, surface; 
   Point3d p; 
   
   Img3dsl imd(imt.Size()); 
   
   // Initialisation des images
   imd = ini; 

   // Recherche de la surface f(P) = 0
   for (k = 0; k<imt.Depth(); k++) {
      for (j = 0; j<imt.Height(); j++) {
	 for (i = 0; i<imt.Width(); i++) {
	    if (imt[k][j][i] == MAXUCHAR) {    // On selectionne les pts.
	       liste.Push(Point3d(k, j, i)); // appartenant a la c_con
	    }
	 }
      }
   }
   // Traitement de la premiere surface.
   while (!liste.Empty()) {
      p = liste.Pop(); 
      test = 0; 
      for (v = 0; v<6; v++) {
	 c = p.z+v6z[v]; 
	 b = p.y+v6y[v]; 
	 a = p.x+v6x[v]; 
	 if ((imt.Hold(c, b, a)) && (imt[c][b][a] == 0)) {
	    test = 1; /* il existe 1 voisin = 0 */
	 }
      }

      if (test == 1) {	
	 surface.Push(p); 
	 compt1 += 1; 
	 imd[p.z][p.y][p.x] = distance; 
      }
   }

   
   // Evolution de la surface
   while (!surface.Empty()) {
      p = surface.Pop(); 
      if (abs(imd[p.z][p.y][p.x])<rayon) {
	 distance = imd[p.z][p.y][p.x]+1; 
	 for (v = 0; v<6; v++) {
	    c = p.z+v6[v].z; 
	    b = p.y+v6[v].y; 
	    a = p.x+v6[v].x; 
	    if ((imt.Hold(c, b, a))&&(imd[c][b][a] == ini)) {
	       surface.Push(Point3d(c, b, a)); 
	       imd[c][b][a] = distance; 
	    }
	 }
      } else {
	 while (!surface.Empty()) {
	    p = surface.Pop(); 
	 }
      }
   }
   
   for (k = 0; k<imt.Depth(); k++) {
      for (j = 0; j<imt.Height(); j++) {
	 for (i = 0; i<imt.Width(); i++) {			  
	    if (imd[k][j][i] != ini) {
	       imt[k][j][i] = MAXUCHAR; 
	    }
	 }
      }
   }
}

void Sphere( Img3duc &imd, int diametre ) {
   int v; 
   int radius; 
   Fifo<Point3d> liste; 
   
   Point3d p, p1, p0(imd.Depth() / 2, imd.Height() / 2, imd.Width() / 2); 
   
   radius = MIN(imd.Depth(), imd.Height()); 
   radius = MIN(radius, imd.Width()); 
   if (radius > diametre) {
      radius = diametre + 1; 
   }
   radius /= 2; 
   
   // Generation de la sphere.
   imd[p0] = MAXUCHAR; 
   liste.Push(p0); 
   
   while (!liste.Empty()) {
      p = liste.Pop();       // Selectionne le point courant dans la liste
      for (v = 0; v<26; v++) {   // Parcours des 26 voisins du point courant
	 p1 = p+v26[v]; 
	 
	 if (imd.Hold(p1)) { // Teste si le voisin est dans l image
	    if (imd[p1] == 0) {   // point non etiquete
	       if (distance(p0.z, p0.y, p0.x, p1.z, p1.y, p1.x)<radius) {	
		  imd[p1] = MAXUCHAR; 
		  liste.Push(p1); 
	       }
	    }
	 }
      }
   }
}

/**
 * Calcul de la Distance Euclidienne par rapport a une Surface
 */
void Distance_Euclidienne( Img3duc &ims, Ushort rayon ) {
   unsigned short i, j, k, v, v1, test; 
   unsigned long compt = 0, l = 0; 
   Long a, b, c, a1, b1, c1; 
   unsigned short  dist, dist_temp; 
   const short ini_short = MAXSHORT; 
   Long ref, ref_temp; 

   Img3dsf im_croiss(ims.Size()); 
   Img3dsf im_dist(ims.Size()); 
   Img3dsf im_marqueur(ims.Size()); 
  
   Fifo<Point3d> liste, surface; 
   Point3d p, *p_adr, *surf; 
  
   // Initialisation des images intermediaires.
   for (k = 0; k<(ims.Depth()); k++) {
      for (j = 0; j<(ims.Height()); j++) {
	 for (i = 0; i<(ims.Width()); i++) {
	    im_croiss[k][j][i] = ini_short; 
	    im_dist[k][j][i] = ini_short; 
	 }
      }
   }
   
   // Recherche des points de la surface
   for (k = 0; k<(ims.Depth()); k++)
      for (j = 0; j<(ims.Height()); j++)
	 for (i = 0; i<(ims.Width()); i++) {
	    if (ims[k][j][i] == MAXUCHAR) {	
	       ims[k][j][i] = MAXUCHAR; 
	       test = 0; 
	       for (v = 0; v<6; v++) {
		  c = k+v6z[v]; 
		  b = j+v6y[v]; 
		  a = i+v6x[v]; 
		  if ( (ims.Hold(c, b, a))&&(ims[c][b][a] == 0) ) {
		     test = 1; 
		  }
		  if (test == 1) {
		     liste.Push(Point3d(k, j, i)); 
		     surface.Push(Point3d(k, j, i)); 
		     compt += 1; 
		  }
	       }	
	       
	    }
	 }	

   surf = new Point3d[compt]; /*on va ranger les points de la surface dans un tableau 
			       dont l'indexation va permettre de les reperer */
   while (!surface.Empty()) {
      p = surface.Pop(); 	      
      *(surf+l) = p; 
      l += 1; 
   }
   
   for (i = 0; i<compt; i++) {
      p_adr = surf+i; 
      im_croiss[p_adr->z][p_adr->y][p_adr->x] = i; // on reference les points de la surface dans im_croiss
      im_dist[p_adr->z][p_adr->y][p_adr->x] = 0; 	// ces points ont une distance nulle par rapport a la surface.
   }
   
   // premiere evolution de la surface
   // --> traitement particulier pour localiser les points 
   // exterieurs ou interieurs a la surface 
   while (!liste.Empty()) {
      p = liste.Pop(); 
      for (v = 0; v<6; v++) {
	 c = p.z+v6z[v]; 
	 b = p.y+v6y[v]; 
	 a = p.x+v6x[v]; 
	 if ( im_croiss.Hold(c, b, a)&&(im_marqueur[c][b][a] == 0)) {
	    ref = -1; 
	    dist = ini_short; 
	    for (v1 = 0; v1<6; v1++) {
	       c1 = c+v6z[v1]; 
	       b1 = b+v6y[v1]; 
	       a1 = a+v6x[v1]; 
	       if (im_croiss.Hold(c1, b1, a1) ) {
		  ref_temp = (Ulong)im_croiss[c1][b1][a1]; 
		  if ( (ref_temp != ini_short)&&(ref_temp != ref) ) {
		     p_adr = surf+ref_temp; 
		     dist_temp = (unsigned short)distance(c, b, a, p_adr->z, p_adr->y, p_adr->x); 
		     if ( (dist_temp <= rayon) && (dist_temp < dist) ) {
			dist = dist_temp; 
			ref = ref_temp; 
			im_croiss[c][b][a] = (float)ref; 
			im_dist[c][b][a] = dist; 
			liste.Push(Point3d(c, b, a)); 
			im_marqueur[c][b][a] = 1; 
		     }
		  }
	       }
	    }
	 }
      }
   }
   
   for (k = 0; k<(ims.Depth()); k++) {
      for (j = 0; j<(ims.Height()); j++) {
	 for (i = 0; i<(ims.Width()); i++) {
	    if ( (ims[k][j][i] != MAXUCHAR)&&(im_dist[k][j][i] != ini_short) ) {
	       ims[k][j][i] = MAXUCHAR; 
	    }
	 }
      }
   }
}

/*
 * Construit un segment au milieu de l'image
 * entre les points p1 et p2..
 */
void Segment( Img3duc &imd, Point3d pti, Point3d ptf ) {
   Long xmin, ymin, zmin; 
   Long xmax, ymax, zmax; 
   Long i, j, k; 
   Long d; 
   float penteyx, pentezx; 
   float penteyz; 

   if (pti.x == ptf.x) {
      penteyx = pentezx = MAXFLOAT; 
      xmin = xmax = ptf.x; 
   } else  {
      d = ptf.x-pti.x; 
      penteyx = (float)(ptf.y-pti.y)/d; 
      pentezx = (float)(ptf.z-pti.z)/d; 
      if (pti.x<ptf.x) {
	 xmin = pti.x; 
	 xmax = ptf.x; 
      } else {
	 xmin = ptf.x; 
	 xmax = pti.x; 
      }
   }
   if (pti.y == ptf.y) {
      penteyx = penteyz = 0.0; 
      ymin = ymax = ptf.x; 
   } else {
      if (pti.y<ptf.y) {
	 ymin = pti.y; 
	 ymax = ptf.y; 
      } else {
	 ymin = ptf.y; 
	 ymax = pti.y; 
      }
   }
   if (pti.z == ptf.z) {
      penteyz = MAXFLOAT; 
      pentezx = 0.0; 
      zmin = zmax = ptf.z; 
   } else {
      penteyz = (float)(ptf.y-pti.y)/(ptf.z-pti.z); 
      if (pti.z<ptf.z) {
	 zmin = pti.z; 
	 zmax = ptf.z; 
      } else {
	 zmin = ptf.z; 
	 zmax = pti.z; 
      }
   }
  
   if ((pentezx<-1)||(pentezx>1)) {
      if ((penteyx<-1)||(penteyx>1)) {
	 if (((pentezx<-1)&&(penteyx<pentezx)) || ((pentezx>1)&&(penteyx>pentezx))) {
	    for (j = ymin; j <= ymax; j++) {
	       d = j-ptf.y; 
	       k = (int)(d/penteyx+ptf.x); 
	       i = (int)(d/penteyz+ptf.z); 
	       imd[i][j][k] = MAXUCHAR; 
	    }
	 } else {
	    for (i = zmin; i <= zmax; i++) {
	       d = i-ptf.z; 
	       j = (int)(d*penteyz+ptf.y); 
	       k = (int)(d/pentezx+ptf.x); 
	       imd[i][j][k] = MAXUCHAR; 
	    }
	 }
      } else {
	 for (k = xmin; k <= xmax; k++) {
	    d = k-ptf.x; 
	    j = (int)(d*penteyx+ptf.y); 
	    i = (int)(d*pentezx+ptf.z); 
	    imd[i][j][k] = MAXUCHAR; 
	 }
      }
   } else {
      if ((penteyx<-1) || (penteyx>1)) {
	 for (j = ymin; j <= ymax; j++) {
	    d = j-ptf.y; 
	    k = (int)(d/penteyx+ptf.x); 
	    i = (int)(d/penteyz+ptf.z); 
	    imd[i][j][k] = MAXUCHAR; 
	 }
      } else {
	 for (k = xmin; k <= xmax; k++) {
	    d = k-ptf.x; 
	    j = (int)(d*penteyx+ptf.y); 
	    i = (int)(d*pentezx+ptf.z); 
	    imd[i][j][k] = MAXUCHAR; 
	 }
      }
   }
}

void Parallelepipede( Img3duc &imd, int diametre, int longueur )  {
   int radius; 
   
   radius = MIN(imd.Depth(), imd.Height()); 
   radius = MIN(radius, diametre); 
   longueur = MIN(imd.Width()-2, longueur); 
   for (int z = (imd.Depth() - radius) / 2; z < (imd.Depth() + radius) / 2; z++) {
      for (int y = (imd.Height() - radius) / 2; y < (imd.Height() + radius) / 2; y++) {
	 Segment(imd, Point3d(z, y, (imd.Width() - longueur) / 2), Point3d(z, y, (imd.Width() - longueur) / 2 + longueur - 1)); 
      }
   }
}


/*
 * radius = min (depth, width)
 */
void Cylindre( Img3duc &imd, int diametre, int longueur ) {
   int z = imd.Depth() / 2; 
   int x = imd.Width() / 2; 
   int radius; 

   radius = MIN(imd.Depth(), imd.Height()); 
   radius = MIN(radius, imd.Width()); 
   if (radius > diametre) {
      radius = diametre; 
   }
   radius /= 2; 

   Segment(imd, Point3d(z, (imd.Height() - longueur) / 2, x), Point3d(z, (imd.Height() + longueur) / 2, x)); 
   Distance_Euclidienne(imd, radius); 
}

void Cube( Img3duc &imd, int diametre ) {
   int z = imd.Depth() / 2; 
   int y = imd.Height() /2; 
   int x = imd.Width() / 2; 
   int radius; 

   radius = MIN(imd.Depth(), imd.Height()); 
   radius = MIN(radius, imd.Width()); 
   if (radius > diametre) {
      radius = diametre; 
   }
   radius /= 2; 
   imd[z][y][x] = MAXUCHAR; 
   Distance_26(imd, radius); 
}

/*
 * Compute radius, center with image dimensions.
 */ 
void Bipyramide( Img3duc &imd, int diametre ) {
   int z = imd.Depth()/2; 
   int y = imd.Height()/2; 
   int x = imd.Width()/2; 
   int radius; 

   radius = MIN(imd.Depth(), imd.Height()); 
   radius = MIN(radius, imd.Width()); 
   if (radius > diametre) {
      radius = diametre; 
   }
   radius /= 2; 
   imd[z][y][x] = MAXUCHAR; 
   Distance_6(imd, radius); 
}

Errc PShapeDesign( Img3duc &imd, long num, int diametre, int longueur ) {
   imd = 0;   
   switch(num) {
   case 10 : return SUCCESS; 
   case 11 : {
      Sphere(imd, diametre); 
      break; 
   }
   case 12 : {
      Cube(imd, diametre); 
      break; 
   }
   case 13 : {
      Parallelepipede(imd, diametre, longueur); 
      break; 
   }
   case 14 : {
      Cylindre(imd, diametre, longueur); 
      break; 
   }
   case 15 : {
      Bipyramide(imd, diametre); 
      break; 
   }	
   default:
      return FAILURE; 
   }
   return SUCCESS; 
}


void Disque( Img2duc &imd, int diametre ) {
   int v; 
   int radius; 
   Long a, b; 
   Fifo<Point2d> liste; 
   Point2d p, p0(imd.Height() / 2, imd.Width() / 2); 
   
   radius = MIN(imd.Width(), imd.Height()); 
   radius = MIN(radius, diametre+1); 
   radius /= 2; 

   // Generation du disque.
   imd[p0] = MAXUCHAR; 
   liste.Push(p0); 
   
   while (!liste.Empty()) {
      p = liste.Pop();       // Selectionne le point courant dans la liste
      for (v = 0; v < 8; v++) {   // Parcours des 8 voisins du point courant
	 b = p.y + v8y[v]; 
	 a = p.x + v8x[v]; 
	 
	 if (imd.Hold(b, a)) { // Test si le voisin est dans l image
	    if (imd[b][a] == 0) {   // point non etiquete
	       if (distance(0, p0.y, p0.x, 0, b, a)<radius) {	
		  imd[b][a] = MAXUCHAR; 
		  liste.Push(Point2d(b, a)); 					
	       }
	    }
	 }
      }
   }
}

void Rectangle( Img2duc &imd, int diametre, int longueur ) {
   int y, x; 
   
   if (imd.Height() < diametre) {
      diametre = imd.Height(); 
   }
   if (imd.Width() < longueur) {
      longueur = imd.Width();
   }
   
   for (y = (imd.Height() - diametre) / 2; y < (imd.Height() + diametre) / 2; y++) {
      for (x = (imd.Width() - longueur) / 2; x < (imd.Width() + longueur) / 2; x++) {
	 imd[y][x] = MAXUCHAR; 
      }
   }
}


void Carre( Img2duc &imd, int diametre ) {
   int y, x, d; 
   
   d = MIN(imd.Height(), imd.Width()); 
   if (d < diametre) {
      diametre = d; 
   }
   
   for (y = (imd.Height() - diametre) / 2; y < (imd.Height() + diametre) / 2; y++) {
      for (x = (imd.Width() - diametre)  /2; x < (imd.Width() + diametre) / 2; x++) {
	 imd[y][x] = MAXUCHAR; 
      }
   }
}

Errc PShapeDesign( Img2duc &imd, long num, int diameter, int length ) {
   imd = 0;   
   switch(num) {
   case 0 : return SUCCESS; 
   case 1 : {
      Disque(imd, diameter); 
      break; 
   }
   case 2 : {
      Carre(imd, diameter); 
      break; 
   }
   case 3 : {
      Rectangle(imd, diameter, length); 
      break; 
   }
   default:
      return FAILURE; 
   }
   return SUCCESS; 
}


#ifdef MAIN

/*
 * Modify only the following constants, and the function call.
 */
#define	USAGE	"usage: %s width height depth type radius length [im_out|-]"
#define	PARC	6
#define	FINC	0
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
   
   if (atoi(parv[3]) < 10) {
      objd[0] = new Img2duc((Long)atoi(parv[1]), (Long)atoi(parv[0])); 
      Img2duc* const imd = (Img2duc*)objd[0]; 
      result = PShapeDesign(*imd, (long)atoi(parv[3]), (int)atoi(parv[4]), (int)atoi(parv[5])); 
   } else {
      objd[0] = new Img3duc((Long)atoi(parv[2]), (Long)atoi(parv[1]), (Long)atoi(parv[0])); 
      Img3duc* const imd = (Img3duc*)objd[0]; 
      result = PShapeDesign(*imd, (long)atoi(parv[3]), (int)atoi(parv[4]), (int)atoi(parv[5])); 
   }
   
   if (result) {
      WriteArgs(argc, argv, PARC, FINC, FOUTC, &mask, objin, objs, objout, objd, MASK); 
   }
   
   Exit(result); 
   return 0; 
}

#endif
