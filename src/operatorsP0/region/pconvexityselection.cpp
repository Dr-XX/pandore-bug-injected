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
 * @author Regis Clouard - 1997-07-21
 * @author Regis Clouard - 2003-05-13 (fix bug).
 * @author Regis Clouard - 2004-06-29 (<= >=)
 * @author Regis Clouard - 2006-06-02 (relation =3 and -3).
 * @author Regis Clouard - Jun 15, 2013 (add 3D)
 */

#include <pandore.h>
using namespace pandore;

/**
 * @file pconvexiteselection.cpp
 * Selectionner les regions sur leur degre de convexite.
 */

struct attrs_convexite {
   Ulong surfaceenv;
   Ulong surfacereg;
   float convexite;
   bool	acceptee;
   Long	xd,xf,yd,yf; // rectangle exinscrit.
   Point2d *bord;
   Long	nbord;
};

/*
 * Calcul du nombre de points entre 2 points extremes xd et xf,
 * sur une ligne donnee.
 * (pt1,pt2) enveloppe extreme droite.
 * (pt3,pt4) enveloppe extreme gauche.
 */
static Ulong Largeur( Long ligne, Point2d &pt1, Point2d &pt2, Point2d &pt3, Point2d &pt4 ) {
   Long	xd;
   Long	xf;
   
   if ((pt1.y-pt2.y) == 0)
      xf=(pt1.x<pt2.x)? pt1.x : pt2.x;
   else if (pt1.x < pt2.x)
      xf=pt1.x+Long((Float(ligne-pt1.y)*Float(pt2.x-pt1.x)/Float(pt2.y-pt1.y)));
   else
      xf=pt2.x+Long((Float(ligne-pt2.y)*Float(pt1.x-pt2.x)/Float(pt1.y-pt2.y)));
   
   if ((pt3.y-pt4.y) == 0)
      xd=(pt3.x<pt4.x)? pt3.x : pt4.x;
   else if (pt4.x < pt3.x)
      xd=pt3.x+Long(Float(ligne-pt3.y)*Float(pt4.x-pt3.x)/Float(pt4.y-pt3.y));
   else
      xd=pt4.x+Long(Float(ligne-pt4.y)*Float(pt3.x-pt4.x)/Float(pt3.y-pt4.y));
   
   return(xf-xd+1);
}

/*
 * Une petite fonction qui calcule l'angle entre 2 points.
 */
static Double AngleTeta( Point2d &p1, Point2d &p2 ) {
   Long dx,dy;
   Long ax,ay;
   Double t;
   
   dx = p1.x - p2.x;
   dy = p2.y - p1.y;
   ax = abs(dx);
   ay = abs(dy);
   
   t= ((dx == 0) && (dy == 0))? 0 : (Double)(dy) / (Double) (ax+ay);
   
   if (dx < 0) {
      t -= 2.0;
   } else if (dy < 0) {
      t += 4.0;
   }
   t *= 90.0;
   if (t < 0) {
      t = -t;
   }
   return(t);
}

/*
 * Une petite fonction qui calcule le carre du rayon de deux points.
 */
static Double Rayon( Point2d &p1, Point2d &p2 ) {
   Long	dx,dy;
  
   dx = p1.x - p2.x;
   dy = p2.y - p1.y;
   return(dx*dx+dy*dy);
}

/*
 * Marque les points de bordure des regions apres
 * une simple convexification ligne.
 * Calcule en meme temps la surface des regions.
 */
Errc Bordure( const Reg2d &rgs, struct attrs_convexite *regions ) {
   register int i,j;
   register int d,f;
   Ulong v;
   
   for (Ulong r=1; r<=rgs.Labels(); r++) {
      regions[r].bord = new Point2d[rgs.Height()*2+1];
      regions[r].surfacereg=0;
      regions[r].xf=0;	regions[r].yf=0;
      regions[r].xd=rgs.Width();
      regions[r].yd=rgs.Height();
   }
   
   // Marquage des mini et maxi.
   for (i=0;i<rgs.Height();i++) {
      // Initialisation des valeurs de bord
      d=i;
      f=(2*rgs.Height())-1-i;
      for (Ulong r=1;r<=rgs.Labels();r++) {
	 regions[r].bord[d].x=0;		// maxi
	 regions[r].bord[d].y=i;
	 regions[r].bord[f].x=rgs.Width();	// mini
	 regions[r].bord[f].y=i;
      }
      // Examen des valeurs des labels de regions.
      for (j=0;j<rgs.Width();j++) {
	 if ((v=rgs[i][j]) == 0) {
	    continue;
	 }
	 if (regions[v].yd>i) regions[v].yd=i;
	 if (regions[v].yf<i) regions[v].yf=i;
	 if (regions[v].xd>j) regions[v].xd=j;
	 if (regions[v].xf<j) regions[v].xf=j;
	 
	 // Calcul des points extremes.
	 if (regions[v].bord[d].x<j)		// maxi
	    regions[v].bord[d].x=j;
	 if (regions[v].bord[f].x>j)		// mini
	    regions[v].bord[f].x=j;
	 // Calcul de la surface.
	 regions[v].surfacereg++;
      }
   }
  
   // Construction de la liste des bords en tournant
   // dans l'ordre trigonometrique inverse.
   for (Ulong r=1;r<=rgs.Labels();r++) {
      if (regions[r].surfacereg == 0) {
	 delete[] regions[r].bord;
      } else {
         regions[r].nbord=2*(regions[r].yf-regions[r].yd+1);
         j=0;
         d=regions[r].yd; f=regions[r].yf;
         for (i=d;i<=f;i++) {
	    regions[r].bord[j++]=regions[r].bord[i];
         }
         d = (2*rgs.Height())-1-regions[r].yf;
         f = (2*rgs.Height())-1-regions[r].yd;
         for (i=d;i<=f;i++) {
	    regions[r].bord[j++]=regions[r].bord[i];
         }
      }
   }
   return SUCCESS;
}

/*
 * Convexification d'une liste de points.
 * Pour aller d'un point de l'enveloppe convexe a un autre,
 * il faut trouver l'angle minimum entre ces deux points.
 */
Errc Convexification( const Reg2d &rgs, struct attrs_convexite *regions ) {
   register int i;
   Double maxangle,angle,maxrayon,rayon,anglelimite;
   Long min;
   Long nbenv;
   
   for (Ulong r=1;r<=rgs.Labels();r++) {
      if ( regions[r].surfacereg==0) {
	 continue;
      }
      // une simple ligne.
      if ((regions[r].xd==regions[r].xf) || (regions[r].yd==regions[r].yf)) {
	 regions[r].nbord=2;
	 regions[r].bord[0].x=regions[r].xd;
	 regions[r].bord[0].y=regions[r].yd;
	 regions[r].bord[1].x=regions[r].xf;
	 regions[r].bord[1].y=regions[r].yf;
	 continue;
      }
      nbenv=1; min=1;
      maxangle = 360.1;
      
      // Ajoute le premier en dernier (boucle).
      regions[r].bord[regions[r].nbord]=regions[r].bord[0];
      
      // Parcours des points dans le sens trigo inverse
      while (min<regions[r].nbord) {
	 // Ajout du point courant dans l'enveloppe.
	 anglelimite=maxangle;
	 maxangle=0.0;maxrayon=0.0;
	 for (i=min;i<regions[r].nbord;i++) {
	    if (regions[r].bord[i].x==rgs.Width()) {
	       continue;
	    }
	    angle=AngleTeta(regions[r].bord[i], regions[r].bord[nbenv-1]);
	    rayon=Rayon(regions[r].bord[i],regions[r].bord[nbenv-1]);
	    if ((angle == maxangle)&& (rayon > maxrayon)) {
	       min=i;
	       maxangle=angle;
	       maxrayon=rayon;
	    }
	    if ((angle>maxangle) && (angle<anglelimite)) {
	       min=i;
	       maxangle=angle;
	       maxrayon=rayon;
	    }
	 }
	 regions[r].bord[nbenv++]=regions[r].bord[min++];
      }
      regions[r].nbord = nbenv;
   }
   return SUCCESS;
}

/*
 * Calcul de la surface de l'enveloppe convexe
 * de chaque regions a partir de la liste des points
 * de l'enveloppe convexe.
 */
Errc SurfaceEnveloppe( const Reg2d &rgs, struct attrs_convexite *regions ) {
   register int i,j;
   Long ligne;
   Point2d pt1;
   Point2d pt2;
   Point2d pt3;
   Point2d pt4;
   
   // Pour chaque region calcule la surface de l'enveloppe convexe.
   for (Ulong r=1; r<=rgs.Labels(); r++) {
      if (regions[r].surfacereg==0) {
         continue;
      }
      if (regions[r].nbord < 3) {
	 regions[r].surfaceenv=regions[r].surfacereg;
	 continue;
      }
      regions[r].surfaceenv=0;
      pt1=regions[r].bord[0];
      pt3=pt1;
      pt2=regions[r].bord[i=1];
      pt4=regions[r].bord[j=regions[r].nbord-1];
      ligne=pt1.y;
      while (i<=j) {
	 regions[r].surfaceenv += Largeur(ligne++,pt1,pt2,pt3,pt4);
	 if (ligne > pt4.y) {
	    pt3 = pt4;
	    pt4=regions[r].bord[--j];
	 }
	 if (ligne > pt2.y) {
	    pt1=pt2;
	    pt2=regions[r].bord[++i];
	 }
      }
   }
   return SUCCESS;
}

/*
 * Calcul du degre de convexite de chaque region.
 * <= si relation=-2, < si relation=-11, > si relation=1, >= si relation=2
 */
Errc DegreConvexite( const Reg2d& rgs, struct attrs_convexite* regions ) {
   for (Ulong r=1; r<=rgs.Labels();r++) {
      if ((regions[r].surfacereg*regions[r].surfaceenv) == 0) {
	 regions[r].convexite=0.0F;
      } else {
	 regions[r].convexite=(float)regions[r].surfacereg/regions[r].surfaceenv;
      }
   }
   return SUCCESS;
}

/*
 * Calcul du degre de convexite de chaque region.
 */
Errc Convexite( const Reg2d& rgs, struct attrs_convexite* regions ) {
   // Definit la liste des points de contours.
   Bordure(rgs,regions);
   // Convexifie une suite de points de contours.
   Convexification(rgs,regions);
   // Calcule de la surface de l'enveloppe convexe.
   SurfaceEnveloppe(rgs,regions);
   // Calcule du degre de convexite.
   DegreConvexite(rgs,regions);
   
   return SUCCESS;
}

/*
 * Selectionne les regions sur leur degre de convexite.
 */
Errc PConvexitySelection( const Reg2d &rgs, Reg2d &rgd, int relation, float threshold ) {
   struct attrs_convexite *regions;
  
   regions = (struct attrs_convexite*)calloc(rgs.Labels()+1,sizeof(struct attrs_convexite));
   Convexite(rgs,regions);

   // Selection des regions
   float temp;
   Ulong nreg=0;

   if (relation <= -3) {
      temp=regions[1].convexite;
      for (Ulong r=2; r<=rgs.Labels(); r++) {
	 if (regions[r].convexite<temp) {
	    temp=regions[r].convexite;
	 }
      }
      for (Ulong r=1; r<=rgs.Labels(); r++) {
	 if (regions[r].convexite==temp) {
	    regions[r].acceptee=true;
	    ++nreg;
	 } else {
	    regions[r].acceptee=false;
	 }
      }
   } else if (relation >= 3) {
      temp=regions[1].convexite;
      for (Ulong r=2; r<=rgs.Labels(); r++) {
	 if (regions[r].convexite>temp) {
	    temp=regions[r].convexite;
	 }
      }
      for (Ulong r=1; r<=rgs.Labels(); r++) {
	 if (regions[r].convexite==temp) {
	    regions[r].acceptee=true;
	    ++nreg;
	 } else {
	    regions[r].acceptee=false;
	 }
      }
   } else if (relation == 2) {
      for (Ulong r=1; r<=rgs.Labels(); r++) {
	 if (regions[r].convexite>=threshold) {
	    regions[r].acceptee=true;
	 } else {
	    regions[r].acceptee=false;
	 }
      }
   } else if (relation ==1) {
      for (Ulong r=1;r<=rgs.Labels();r++) {
	 if (regions[r].convexite>threshold) {
	    regions[r].acceptee=true;
	 } else {
	    regions[r].acceptee=false;
	 }
      }
   } else if (relation == 0) {
      for (Ulong r=1;r<=rgs.Labels();r++) {
	 if (regions[r].convexite == threshold) {
	    regions[r].acceptee=true;
	 } else {
	    regions[r].acceptee=false;
	 }
      }
   } else if (relation == -1) {
      for (Ulong r=1; r<=rgs.Labels(); r++) {
	 if (regions[r].convexite < threshold) {
	    regions[r].acceptee=true;
	 } else {
	    regions[r].acceptee=false;
	 }
      }
   } else if (relation == -2) {
      for (Ulong r=1; r<=rgs.Labels(); r++) {
	 if (regions[r].convexite <= threshold) {
	    regions[r].acceptee=true;
	 } else {
	    regions[r].acceptee=false;
	 }
      }
   }
  
   // Construction de la carte de sortie.
   for (int y=0;  y<rgs.Height(); y++) {
      for (int x=0; x<rgs.Width(); x++) {
	 if (regions[rgs(y,x)].acceptee) {
	    rgd(y,x) = rgs(y,x);
	 } else {
	    rgd(y,x) = 0;
	 }
      }
   }
   free(regions);
   rgd.Labels(rgs.Labels());
   return nreg;
}


// 3D

/*
This code is described in "Computational Geometry in C" (Second Edition),
Chapter 4.  It is not written to be comprehensible without the 
explanation in that book.

Written by Joseph O'Rourke, with contributions by 
  Kristy Anderson, John Kutcher, Catherine Schevon, Susan Weller.
Last modified: May 2000
Questions to orourke@cs.smith.edu.

--------------------------------------------------------------------
This code is Copyright 2000 by Joseph O'Rourke.  It may be freely 
redistributed in its entirety provided that this copyright notice is 
not removed.
--------------------------------------------------------------------
*/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

/* Define vertex indices. */
#define X   0
#define Y   1
#define Z   2

/* Define structures for vertices, edges and faces */
typedef struct tVertexStructure tsVertex;
typedef tsVertex *tVertex;

typedef struct tEdgeStructure tsEdge;
typedef tsEdge *tEdge;

typedef struct tFaceStructure tsFace;
typedef tsFace *tFace;

struct tVertexStructure {
   int      v[3];
   int	    vnum;
   tEdge    duplicate;	        /* pointer to incident cone edge (or NULL) */
   bool     onhull;		/* T iff point on hull. */
   bool	    mark;		/* T iff point already processed. */
   tVertex  next, prev;
};

struct tEdgeStructure {
   tFace    adjface[2];
   tVertex  endpts[2];
   tFace    newface;            /* pointer to incident cone face. */
   bool     isdeleted;		/* T iff edge should be delete. */
   tEdge    next, prev;
};

struct tFaceStructure {
   tEdge    edge[3];
   tVertex  vertex[3];
   bool	    visible;	        /* T iff face visible from new point. */
   tFace    next, prev;
};

/* Define flags */
#define ONHULL   	true
#define REMOVED  	true
#define VISIBLE  	true
#define PROCESSED	true
#define SAFE		1000000		/* Range of safe coord values. */

/* Global variable definitions */
/* tVertex vertices = NULL; */
/* tEdge edges    	 = NULL; */
/* tFace faces    	 = NULL; */

/* The previous global variables have been
 * integrated in the class of region features.
 */
class RegionFeatures {
 public:
   tVertex vertices;
   tEdge edges;
   tFace faces;
   Ulong regionVolume;
   Ulong convexHullVolume;
   float convexity;
   bool selected;
   
   RegionFeatures(): vertices(0), edges(0), faces(0),regionVolume(0),convexHullVolume(0) { }
};

/* Function declarations */
bool	AddOne( RegionFeatures *region, tVertex p ); 
int     VolumeSign(tFace f, tVertex p);
tFace	MakeConeFace( RegionFeatures *region, tEdge e, tVertex p );
void    MakeCcw( tFace f, tEdge e, tVertex p );
tEdge   MakeNullEdge( RegionFeatures *region );
tFace   MakeNullFace( RegionFeatures *region ); 
tFace   MakeFace( RegionFeatures *region, tVertex v0, tVertex v1, tVertex v2, tFace f );
void    CleanUp( RegionFeatures *region, tVertex *pvnext );
void    CleanEdges( RegionFeatures *region );
void    CleanFaces( RegionFeatures *region );
void    CleanVertices( RegionFeatures *region, tVertex *pvnext );
bool	Collinear( tVertex a, tVertex b, tVertex c );

/*====================================================================
    macros.h
 
 	macros used to access data structures and perform quick tests.

  ====================================================================*/

/* general-purpose macros */
#define SWAP(t,x,y)	{ t = x; x = y; y = t; }

#define NEW(p,type)	if ((p=(type *) malloc (sizeof(type))) == NULL) {\
				printf ("Out of Memory!\n");\
				exit(0);\
			}

#define FREE(p)		if (p) { free ((char *) p); p = NULL; }


#define ADD( head, p )  if ( head )  { \
				p->next = head; \
				p->prev = head->prev; \
				head->prev = p; \
				p->prev->next = p; \
			} else { \
				head = p; \
				head->next = head->prev = p; \
			}

#define DELETE( head, p ) if ( head )  { \
				if ( head == head->next ) \
					head = NULL;  \
				else if ( p == head ) \
					head = head->next; \
				p->next->prev = p->prev;  \
				p->prev->next = p->next;  \
				FREE( p ); \
			} 

/*---------------------------------------------------------------------
MakeNullVertex: Makes a vertex, nulls out fields.
---------------------------------------------------------------------*/
tVertex	MakeNullVertex( RegionFeatures *region ) {
   tVertex  v;
   
   NEW( v, tsVertex );
   v->duplicate = NULL;
   v->onhull = !ONHULL;
   v->mark = !PROCESSED;

   ADD( region->vertices, v );
   return v;
}

/*---------------------------------------------------------------------
 DoubleTriangle builds the initial double triangle.  It first finds 3 
 noncollinear points and makes two faces out of them, in opposite order.
 It then finds a fourth point that is not coplanar with that face.  The  
 vertices are stored in the face structure in counterclockwise order so 
 that the volume between the face and the point is negative. Lastly, the
 3 newfaces to the fourth point are constructed and the data structures
 are cleaned up. 
---------------------------------------------------------------------*/
bool DoubleTriangle( RegionFeatures *region ) {
   tVertex  v0, v1, v2, v3;
   tFace    f0, f1 = NULL;
   int      vol;

   /* Find 3 noncollinear points. */
   v0 = region->vertices;
   while ( Collinear( v0, v0->next, v0->next->next ) ) {
      if ( ( v0 = v0->next ) == region->vertices ) {
//          printf("DoubleTriangle:  All points are Collinear!\n");
	 return false;
      }
   }
   v1 = v0->next;
   v2 = v1->next;

	
   /* Mark the vertices as processed. */
   v0->mark = PROCESSED;
   v1->mark = PROCESSED;
   v2->mark = PROCESSED;
   
   /* Create the two "twin" faces. */
   f0 = MakeFace( region, v0, v1, v2, f1 );
   f1 = MakeFace( region, v2, v1, v0, f0 );

   /* Link adjacent face fields. */
   f0->edge[0]->adjface[1] = f1;
   f0->edge[1]->adjface[1] = f1;
   f0->edge[2]->adjface[1] = f1;
   f1->edge[0]->adjface[1] = f0;
   f1->edge[1]->adjface[1] = f0;
   f1->edge[2]->adjface[1] = f0;
	
   /* Find a fourth, noncoplanar point to form tetrahedron. */
   v3 = v2->next;
   vol = VolumeSign( f0, v3 );
   while ( !vol ) {
      if ( ( v3 = v3->next ) == v0 )  {
//          printf("DoubleTriangle:  All points are coplanar!\n");
	 return false;
      }
      vol = VolumeSign( f0, v3 );
   }
	
   /* Insure that v3 will be the first added. */
   region->vertices = v3;
   return true;
}
  
/*---------------------------------------------------------------------
ConstructHull adds the vertices to the hull one at a time.  The hull
vertices are those in the list marked as onhull.
---------------------------------------------------------------------*/
void ConstructHull( RegionFeatures *region ) {
   tVertex  v, vnext;
/*   bool	    changed;*/	/* T if addition changes hull; not used. */

   v = region->vertices;
   do {
      vnext = v->next;
      if ( !v->mark ) {
         v->mark = PROCESSED;
	 /* changed =*/ AddOne( region , v );
	 CleanUp( region, &vnext ); /* Pass down vnext in case it gets deleted. */
      }
      v = vnext;
   } while ( v != region->vertices );
}

/*---------------------------------------------------------------------
AddOne is passed a vertex.  It first determines all faces visible from 
that point.  If none are visible then the point is marked as not 
onhull.  Next is a loop over edges.  If both faces adjacent to an edge
are visible, then the edge is marked for deletion.  If just one of the
adjacent faces is visible then a new face is constructed.
---------------------------------------------------------------------*/
bool 	AddOne( RegionFeatures *region, tVertex p ) {
   tFace  f; 
   tEdge  e, temp;
   int 	  vol;
   bool	  vis = false;

   /* Mark faces visible from p. */
   f = region->faces;
   do {
      vol = VolumeSign( f, p );
      if ( vol < 0 ) {
	 f->visible = VISIBLE;  
	 vis = true;                      
      }
      f = f->next;
   } while ( f != region->faces );

   /* If no faces are visible from p, then p is inside the hull. */
   if ( !vis ) {
      p->onhull = !ONHULL;  
      return false; 
   }

   /* Mark edges in interior of visible region for deletion.
      Erect a newface based on each border edge. */
   e = region->edges;
   do {
      temp = e->next;
      if ( e->adjface[0]->visible && e->adjface[1]->visible ) {
	 /* e interior: mark for deletion. */
	 e->isdeleted = REMOVED;
      } else if ( e->adjface[0]->visible || e->adjface[1]->visible )  {
	 /* e border: make a new face. */
	 e->newface = MakeConeFace( region, e, p );
      }
      e = temp;
   } while ( e != region->edges );
   return true;
}

/*---------------------------------------------------------------------
VolumeSign returns the sign of the volume of the tetrahedron determined by f
and p.  VolumeSign is +1 iff p is on the negative side of f,
where the positive side is determined by the rh-rule.  So the volume 
is positive if the ccw normal to f points outside the tetrahedron.
The final fewer-multiplications form is due to Bob Williamson.
---------------------------------------------------------------------*/
int  VolumeSign( tFace f, tVertex p ) {
   double  vol;
   double  ax, ay, az, bx, by, bz, cx, cy, cz;

   ax = f->vertex[0]->v[X] - p->v[X];
   ay = f->vertex[0]->v[Y] - p->v[Y];
   az = f->vertex[0]->v[Z] - p->v[Z];
   bx = f->vertex[1]->v[X] - p->v[X];
   by = f->vertex[1]->v[Y] - p->v[Y];
   bz = f->vertex[1]->v[Z] - p->v[Z];
   cx = f->vertex[2]->v[X] - p->v[X];
   cy = f->vertex[2]->v[Y] - p->v[Y];
   cz = f->vertex[2]->v[Z] - p->v[Z];

   vol =   ax * (by*cz - bz*cy)
         + ay * (bz*cx - bx*cz)
         + az * (bx*cy - by*cx);

   /* The volume should be an integer. */
   if      ( vol >  0.5 )  return  1;
   else if ( vol < -0.5 )  return -1;
   else                    return  0;
}
/*---------------------------------------------------------------------
Same computation, but computes using ints, and returns the actual volume.
---------------------------------------------------------------------*/
int  Volumei( tFace f, tVertex p ) {
   int  vol;
   int  ax, ay, az, bx, by, bz, cx, cy, cz;

   ax = f->vertex[0]->v[X] - p->v[X];
   ay = f->vertex[0]->v[Y] - p->v[Y];
   az = f->vertex[0]->v[Z] - p->v[Z];
   bx = f->vertex[1]->v[X] - p->v[X];
   by = f->vertex[1]->v[Y] - p->v[Y];
   bz = f->vertex[1]->v[Z] - p->v[Z];
   cx = f->vertex[2]->v[X] - p->v[X];
   cy = f->vertex[2]->v[Y] - p->v[Y];
   cz = f->vertex[2]->v[Z] - p->v[Z];

   vol =  (ax * (by*cz - bz*cy)
         + ay * (bz*cx - bx*cz)
         + az * (bx*cy - by*cx));

   return vol;
}

/*---------------------------------------------------------------------
MakeConeFace makes a new face and two new edges between the 
edge and the point that are passed to it. It returns a pointer to
the new face.
---------------------------------------------------------------------*/
tFace MakeConeFace( RegionFeatures *region, tEdge e, tVertex p ) {
   tEdge  new_edge[2];
   tFace  new_face;
   int 	  i, j;

   /* Make two new edges (if don't already exist). */
   for ( i=0; i < 2; ++i )  {
      /* If the edge exists, copy it into new_edge. */
      if ( !( new_edge[i] = e->endpts[i]->duplicate) ) {
	 /* Otherwise (duplicate is NULL), MakeNullEdge. */
	 new_edge[i] = MakeNullEdge( region );
	 new_edge[i]->endpts[0] = e->endpts[i];
	 new_edge[i]->endpts[1] = p;
	 e->endpts[i]->duplicate = new_edge[i];
      }
   }

   /* Make the new face. */
   new_face = MakeNullFace( region );   
   new_face->edge[0] = e;
   new_face->edge[1] = new_edge[0];
   new_face->edge[2] = new_edge[1];
   MakeCcw( new_face, e, p ); 
        
   /* Set the adjacent face pointers. */
   for ( i=0; i < 2; ++i ) {
      for ( j=0; j < 2; ++j ) {  
	 /* Only one NULL link should be set to new_face. */
	 if ( !new_edge[i]->adjface[j] ) {
	    new_edge[i]->adjface[j] = new_face;
	    break;
	 }
      }
   }
   
   return new_face;
}

/*---------------------------------------------------------------------
MakeCcw puts the vertices in the face structure in counterclock wise 
order.  We want to store the vertices in the same 
order as in the visible face.  The third vertex is always p.

Although no specific ordering of the edges of a face are used
by the code, the following condition is maintained for each face f:
one of the two endpoints of f->edge[i] matches f->vertex[i]. 
But note that this does not imply that f->edge[i] is between
f->vertex[i] and f->vertex[(i+1)%3].  (Thanks to Bob Williamson.)
---------------------------------------------------------------------*/
void	MakeCcw( tFace f, tEdge e, tVertex p ) {
   tFace  fv;   /* The visible face adjacent to e */
   int    i;    /* Index of e->endpoint[0] in fv. */
   tEdge  s;	/* Temporary, for swapping */
      
   if  ( e->adjface[0]->visible ) {
        fv = e->adjface[0];
   } else { fv = e->adjface[1]; }
       
   /* Set vertex[0] & [1] of f to have the same orientation
      as do the corresponding vertices of fv. */ 
   for ( i=0; fv->vertex[i] != e->endpts[0]; ++i )
      ;
   /* Orient f the same as fv. */
   if ( fv->vertex[ (i+1) % 3 ] != e->endpts[1] ) {
      f->vertex[0] = e->endpts[1];  
      f->vertex[1] = e->endpts[0];    
   } else {                               
      f->vertex[0] = e->endpts[0];   
      f->vertex[1] = e->endpts[1];      
      SWAP( s, f->edge[1], f->edge[2] );
   }
   /* This swap is tricky. e is edge[0]. edge[1] is based on endpt[0],
      edge[2] on endpt[1].  So if e is oriented "forwards," we
      need to move edge[1] to follow [0], because it precedes. */
   
   f->vertex[2] = p;
}
 
/*---------------------------------------------------------------------
MakeNullEdge creates a new cell and initializes all pointers to NULL
and sets all flags to off.  It returns a pointer to the empty cell.
---------------------------------------------------------------------*/
tEdge MakeNullEdge( RegionFeatures *region ) {
   tEdge  e;

   NEW( e, tsEdge );
   e->adjface[0] = e->adjface[1] = e->newface = NULL;
   e->endpts[0] = e->endpts[1] = NULL;
   e->isdeleted = !REMOVED;

   ADD( region->edges, e );
   return e;
}

/*--------------------------------------------------------------------
MakeNullFace creates a new face structure and initializes all of its
flags to NULL and sets all the flags to off.  It returns a pointer
to the empty cell.
---------------------------------------------------------------------*/
tFace 	MakeNullFace( RegionFeatures *region ) {
   tFace  f;
   int    i;

   NEW( f, tsFace);
   for ( i=0; i < 3; ++i ) {
      f->edge[i] = NULL;
      f->vertex[i] = NULL;
   }
   f->visible = !VISIBLE;
   ADD( region->faces, f );
   return f;
}

/*---------------------------------------------------------------------
MakeFace creates a new face structure from three vertices (in ccw
order).  It returns a pointer to the face.
---------------------------------------------------------------------*/
tFace MakeFace( RegionFeatures *region, tVertex v0, tVertex v1, tVertex v2, tFace fold ) {
   tFace  f;
   tEdge  e0, e1, e2;

   /* Create edges of the initial triangle. */
   if ( !fold ) {
     e0 = MakeNullEdge(region);
     e1 = MakeNullEdge(region);
     e2 = MakeNullEdge(region);
   } else { /* Copy from fold, in reverse order. */
     e0 = fold->edge[2];
     e1 = fold->edge[1];
     e2 = fold->edge[0];
   }
   e0->endpts[0] = v0;              e0->endpts[1] = v1;
   e1->endpts[0] = v1;              e1->endpts[1] = v2;
   e2->endpts[0] = v2;              e2->endpts[1] = v0;
	
   /* Create face for triangle. */
   f = MakeNullFace(region);
   f->edge[0]   = e0;  f->edge[1]   = e1; f->edge[2]   = e2;
   f->vertex[0] = v0;  f->vertex[1] = v1; f->vertex[2] = v2;
	
   /* Link edges to face. */
   e0->adjface[0] = e1->adjface[0] = e2->adjface[0] = f;
	
   return f;
}

/*---------------------------------------------------------------------
CleanUp goes through each data structure list and clears all
flags and NULLs out some pointers.  The order of processing
(edges, faces, vertices) is important.
---------------------------------------------------------------------*/
void	CleanUp( RegionFeatures *region, tVertex *pvnext ) {
   CleanEdges(region);
   CleanFaces(region);
   CleanVertices( region, pvnext );
}

/*---------------------------------------------------------------------
CleanEdges runs through the edge list and cleans up the structure.
If there is a newface then it will put that face in place of the 
visible face and NULL out newface. It also deletes so marked edges.
---------------------------------------------------------------------*/
void	CleanEdges( RegionFeatures *region ) {
   tEdge  e;	/* Primary index into edge list. */
   tEdge  t;	/* Temporary edge pointer. */
		
   /* Integrate the newface's into the data structure. */
   /* Check every edge. */
   e = region->edges;
   do {
      if ( e->newface ) { 
	 if ( e->adjface[0]->visible ) {
	    e->adjface[0] = e->newface; 
	 } else	{ e->adjface[1] = e->newface; }
	 e->newface = NULL;
      }
      e = e->next;
   } while ( e != region->edges );

   /* Delete any region->edges marked for deletion. */
   while ( region->edges && region->edges->isdeleted ) { 
      e = region->edges;
      DELETE( region->edges, e );
   }
   e = region->edges->next;
   do {
      if ( e->isdeleted ) {
	 t = e;
	 e = e->next;
	 DELETE( region->edges, t );
      } else { e = e->next; }
   } while ( e != region->edges );
}

/*---------------------------------------------------------------------
CleanFaces runs through the face list and deletes any face marked visible.
---------------------------------------------------------------------*/
void	CleanFaces( RegionFeatures *region ) {
   tFace  f;	/* Primary pointer into face list. */
   tFace  t;	/* Temporary pointer, for deleting. */

   while ( region->faces && region->faces->visible ) { 
      f = region->faces;
      DELETE( region->faces, f );
   }
   f = region->faces->next;
   do {
      if ( f->visible ) {
	 t = f;
	 f = f->next;
	 DELETE( region->faces, t );
      } else { f = f->next; }
   } while ( f != region->faces );
}

/*---------------------------------------------------------------------
CleanVertices runs through the vertex list and deletes the 
vertices that are marked as processed but are not incident to any 
undeleted edges. 
The pointer to vnext, pvnext, is used to alter vnext in
ConstructHull() if we are about to delete vnext.
---------------------------------------------------------------------*/
void	CleanVertices( RegionFeatures *region, tVertex *pvnext ) {
   tEdge    e;
   tVertex  v, t;
	
   /* Mark all vertices incident to some undeleted edge as on the hull. */
   e = region->edges;
   do {
      e->endpts[0]->onhull = e->endpts[1]->onhull = ONHULL;
      e = e->next;
   } while (e != region->edges);
	
   /* Delete all vertices that have been processed but
      are not on the hull. */
   while ( region->vertices && region->vertices->mark && !region->vertices->onhull ) { 
      /* If about to delete vnext, advance it first. */
      v = region->vertices;
      if ( v == *pvnext )
         *pvnext = v->next;
      DELETE( region->vertices, v );
   }
   v = region->vertices->next;
   do {
      if ( v->mark && !v->onhull ) {    
	 t = v; 
	 v = v->next;
	 if ( t == *pvnext ) {
	    *pvnext = t->next;
	 }
	 DELETE( region->vertices, t );
      } else { v = v->next; }
   } while ( v != region->vertices );
	
   /* Reset flags. */
   v = region->vertices;
   do {
      v->duplicate = NULL; 
      v->onhull = !ONHULL; 
      v = v->next;
   } while ( v != region->vertices );
}

/*---------------------------------------------------------------------
Collinear checks to see if the three points given are collinear,
by checking to see if each element of the cross product is zero.
---------------------------------------------------------------------*/
bool Collinear( tVertex a, tVertex b, tVertex c ) {
   return 
         ( c->v[Z] - a->v[Z] ) * ( b->v[Y] - a->v[Y] ) -
         ( b->v[Z] - a->v[Z] ) * ( c->v[Y] - a->v[Y] ) == 0
      && ( b->v[Z] - a->v[Z] ) * ( c->v[X] - a->v[X] ) -
         ( b->v[X] - a->v[X] ) * ( c->v[Z] - a->v[Z] ) == 0
      && ( b->v[X] - a->v[X] ) * ( c->v[Y] - a->v[Y] ) -
         ( b->v[Y] - a->v[Y] ) * ( c->v[X] - a->v[X] ) == 0  ;
}

/*------------------------------------------------------------------
  EdgeOrderOnFaces: puts e0 between v0 and v1, e1 between v1 and v2,
  e2 between v2 and v0 on each face.  This should be unnecessary, alas.
  Not used in code, but useful for other purposes.
------------------------------------------------------------------*/
void EdgeOrderOnFaces ( RegionFeatures *region ) {
   tFace f = region->faces;
   tEdge newEdge;
   int i,j;
   
   do {
      for (i = 0; i < 3; i++) {
	 if (!(((f->edge[i]->endpts[0] == f->vertex[i]) &&
		(f->edge[i]->endpts[1] == f->vertex[(i+1)%3])) ||
	       ((f->edge[i]->endpts[1] == f->vertex[i]) &&
		(f->edge[i]->endpts[0] == f->vertex[(i+1)%3])))) {
	    /* Change the order of the edges² on the face: */
	    for (j = 0; j < 3; j ++) {
	       /* find the edge that should be there */
	       if (((f->edge[j]->endpts[0] == f->vertex[i]) &&
		    (f->edge[j]->endpts[1] == f->vertex[(i+1)%3])) ||
		   ((f->edge[j]->endpts[1] == f->vertex[i]) &&
		    (f->edge[j]->endpts[0] == f->vertex[(i+1)%3]))) {
		  /* Swap it with the one erroneously put into its place: */
		  newEdge = f->edge[i];
		  f->edge[i] = f->edge[j];
		  f->edge[j] = newEdge;
	       }
	    }
	 }
      }
      f = f->next;
   } while (f != region->faces);
}

/** The remainder of the code is specific to Pandore **/

struct Vector {
      int x;
      int y;
      int z;
};

/****** Calcul du volume. */
float SignedVolumeOfTriangle (struct Vector p1, struct Vector p2, struct Vector p3 ) {
   int v321 = p3.x*p2.y*p1.z;
   int v231 = p2.x*p3.y*p1.z;
   int v312 = p3.x*p1.y*p2.z;
   int v132 = p1.x*p3.y*p2.z;
   int v213 = p2.x*p1.y*p3.z;
   int v123 = p1.x*p2.y*p3.z;

   return (1.0f/6.0f)*(-v321 + v231 + v312 - v132 - v213 + v123);
}

/*-------------------------------------------------------------------*/
void ComputeVolume( RegionFeatures *region ) {
   struct Vector p1, p2, p3;
   int volume=0;
   tFace temp = region->faces;

   if (temp) {
      do {
	 p1.x = region->faces->vertex[0]->v[X]; p1.y = region->faces->vertex[0]->v[Y]; p1.z = region->faces->vertex[0]->v[Z];
	 p2.x = region->faces->vertex[1]->v[X]; p2.y = region->faces->vertex[1]->v[Y]; p2.z = region->faces->vertex[1]->v[Z];
	 p3.x = region->faces->vertex[2]->v[X]; p3.y = region->faces->vertex[2]->v[Y]; p3.z = region->faces->vertex[2]->v[Z];
	 volume += (int)SignedVolumeOfTriangle(p1,p2,p3);

	 region->faces= region->faces->next;
      } while ( region->faces != temp );
   }
   if (volume>0) {
      region->convexHullVolume = volume;
   } else {
      region->convexHullVolume = - volume;
   }
}

void Delete( RegionFeatures *region ) {
   {
      tEdge temp = region->edges;
      if (temp) {
	 temp = temp->next;
	 while ( region->edges != temp ) {
	    tEdge temp2 = temp;
	    temp = temp->next;
	    delete (temp2);
	 } 
	 free(region->edges);
      }
   }
   {
      tVertex temp = region->vertices;
      if (temp) {
	 temp = temp->next;
	 while ( region->vertices != temp ) {
	    tVertex temp2 = temp;
	    temp = temp->next;
	    delete (temp2);
	 } 
	 free(region->vertices);
      }
   }
   {
      tFace temp = region->faces;
      if (temp) {
	 temp = temp->next;
	 while ( region->faces != temp ) {
	    tFace temp2 = temp;
	    temp = temp->next;
	    delete (temp2);
	 } 
      }
      free(region->faces);
   }
}

/*-------------------------------------------------------------------*/
void BuildConvexHull( RegionFeatures *region ) {
    if (DoubleTriangle(region)) {
      ConstructHull(region);
      EdgeOrderOnFaces(region);
      ComputeVolume(region);
   } else {
      region->convexHullVolume= region->regionVolume;
   }
}

void ComputeConvexity( const Reg3d &rgs , RegionFeatures *regions) {
   int vnum = 0;
   Point3d p;
   
   for (p.z=0; p.z<rgs.Depth() ; p.z++) {
      for (p.y=0; p.y<rgs.Height() ; p.y++) {
	 for (p.x=0; p.x<rgs.Width() ; p.x++) {
	    Ulong r = rgs[p];
	    if (r>0) {
	       regions[r].regionVolume ++;
	       for (int v=0; v<6; v++) {
		  Point3d p1 = p+v6[v];
		  if (rgs.Hold(p1) && rgs[p]!=rgs[p1]) {
		     tVertex  v = MakeNullVertex(&regions[r]);
		     v->v[X] = p.x;
		     v->v[Y] = p.y;
		     v->v[Z] = p.z;
		     v->vnum = vnum++;
		  }
	       }
	    }
	 }
      }
   }

   for (Ulong r = 0; r<rgs.Labels()+1; r++) {
      if (regions[r].vertices!=0) {
// 	 printf("region r %d\n", r);
	 BuildConvexHull(&regions[r]);
// 	 printf("  volume region = %d :: enveloppe convexe %d = convexite %f\n", regions[r].regionVolume, regions[r].convexHullVolume, (float)regions[r].regionVolume / regions[r].convexHullVolume);
	 regions[r].convexity = (float)regions[r].regionVolume / regions[r].convexHullVolume;
	 Delete(&regions[r]);
      } else {
	 regions[r].convexity = 0;
      }
   }
}

/*
 * Selectionne les regions sur leur degre de convexity.
 */
Errc PConvexitySelection( const Reg3d &rgs, Reg3d &rgd, int relation, float threshold ) {
   RegionFeatures *regions = new RegionFeatures[rgs.Labels()+1];

   ComputeConvexity(rgs, regions);
   
   // Selection des regions
   Ulong nreg=0;
   float temp;
   if (relation <= -3) {
      temp=regions[1].convexity;
      for (Ulong r=2; r<=rgs.Labels(); r++) {
	 if (regions[r].convexity<temp) {
	    temp=regions[r].convexity;
	 }
      }
      for (Ulong r=1; r<=rgs.Labels(); r++) {
	 if (regions[r].convexity==temp) {
	    regions[r].selected=true;
	    ++nreg;
	 } else {
	    regions[r].selected=false;
	 }
      }
   } else if (relation >= 3) {
      temp=regions[1].convexity;
      for (Ulong r=2; r<=rgs.Labels(); r++) {
	 if (regions[r].convexity>temp)
	    temp=regions[r].convexity;
      }
      for (Ulong r=1; r<=rgs.Labels(); r++) {
	 if (regions[r].convexity==temp) {
	    regions[r].selected=true;
	    ++nreg;
	 } else {
	    regions[r].selected=false;
	 }
      }
   } else if (relation == 2) {
      for (Ulong r=1; r<=rgs.Labels(); r++) {
	 if (regions[r].convexity>=threshold) {
	    regions[r].selected=true;
	 } else {
	    regions[r].selected=false;
	 }
      }
   } else if (relation ==1) {
      for (Ulong r=1; r<=rgs.Labels(); r++) {
	 if (regions[r].convexity>threshold) {
	    regions[r].selected=true;
	 } else {
	    regions[r].selected=false;
	 }
      }
   } else if (relation == 0) {
      for (Ulong r=1; r<=rgs.Labels(); r++) {
	 if (regions[r].convexity == threshold) {
	    regions[r].selected=true;
	 } else {
	    regions[r].selected=false;
	 }
      }
   } else if (relation == -1) {
      for (Ulong r=1; r<=rgs.Labels(); r++) {
	 if (regions[r].convexity < threshold) {
	    regions[r].selected=true;
	 } else {
	    regions[r].selected=false;
	 }
      }
   } else if (relation == -2) {
      for (Ulong r=1; r<=rgs.Labels(); r++) {
	 if (regions[r].convexity <= threshold) {
	    regions[r].selected=true;
	 } else {
	    regions[r].selected=false;
	 }
      }
   }
  
   // Construction de la carte de sortie.
   for (int z=0; z<rgs.Depth(); z++) {
      for (int y=0; y<rgs.Height(); y++) {
	 for (int x=0; x<rgs.Width(); x++) {
	    if (regions[rgs(z,y,x)].selected) {
	       rgd(z,y,x) = rgs(z,y,x);
	    } else {
	       rgd(z,y,x) = 0;
	    }
	 }
      }
   }
   
   rgd.Labels(rgs.Labels());
   delete[] regions;
   
   return(nreg);
}

#ifdef MAIN

/*
 * Modify only the following constants, and the function call.
 */
#define	USAGE	"usage: %s relation threshold [-m mask] [rg_in|-] [rg_out|-]"
#define	PARC	2
#define	FINC	1
#define	FOUTC	1
#define	MASK	2

int main( int argc, char *argv[] ) {
   Errc result;              // The result code of the execution.
   Pobject* mask;            // The region map.
   Pobject* objin[FINC+1];   // The input objects.
   Pobject* objs[FINC+1];    // The source objects masked by the map.
   Pobject* objout[FOUTC+1]; // The output object.
   Pobject* objd[FOUTC+1];   // The result object of the execution.
   char* parv[PARC+1];       // The input parameters.
   
   ReadArgs(argc,argv,PARC,FINC,FOUTC,&mask,objin,objs,objout,objd,parv,USAGE,MASK);
   
   switch(objs[0]->Type()) {
   case Po_Reg2d :{
      Reg2d* const rgs=(Reg2d*)objs[0];
      objd[0]=new Reg2d(rgs->Size());
      Reg2d* const rgd=(Reg2d*)objd[0];
      
      result=PConvexitySelection(*rgs,*rgd,atoi(parv[0]),(float)atof(parv[1]));
   }break;
   case Po_Reg3d :{
      Reg3d* const rgs=(Reg3d*)objs[0];
      objd[0]=new Reg3d(rgs->Size());
      Reg3d* const rgd=(Reg3d*)objd[0];
      
      result=PConvexitySelection(*rgs,*rgd,atoi(parv[0]),(float)atof(parv[1]));
   }break;
   default :
      PrintErrorFormat(objin,FINC);
      result=FAILURE;
   }
   
   if (result) {
      WriteArgs(argc,argv,PARC,FINC,FOUTC,&mask,objin,objs,objout,objd,MASK);
   }
   
   Exit(result);
   return 0;
}

#endif
