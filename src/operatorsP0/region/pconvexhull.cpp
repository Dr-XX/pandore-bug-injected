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
 * @author Régis Clouard - 1997-12-19
 * @author Régis Clouard - Oct 11, 2010 (fix bug on output region initialization)
 */

#include <pandore.h>
using namespace pandore;

/**
 * @file pconvexhull.cpp
 * Construit l'enveloppe convexe des regions.
 */

struct attrs { // changements a reporter dans ec-convexite.cpp
   Long	xd,xf,yd,yf; // rectangle exinscrit.
   Point2d *bord;
   Long nbord;
};

/*
 * Calcul du nombre de points entre 2 points extremes xd et xf,
 * sur une ligne donnee.
 * (pt1,pt2) enveloppe extreme droite.
 * (pt3,pt4) enveloppe extreme gauche.
 */
static void Dessine( Long ligne, Point2d &pt1, Point2d &pt2, Point2d &pt3, Point2d &pt4, int couleur, Reg2d &rgd ) {
   register int i;
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
  
   for (i=xd; i<xf;i++)
      rgd[ligne][i]=couleur;
}

/*
 * Une petite fonction qui calcule l'angle entre 2 points.
 */
static Double AngleTeta( Point2d &p1, Point2d &p2 ) {
   Long	dx,dy;
   Long	ax,ay;
   Double	t;

   dx = p1.x - p2.x;
   dy = p2.y - p1.y;
   ax = abs(dx);
   ay = abs(dy);

   t= ((dx == 0) && (dy == 0))? 0 : (Double)(dy) / (Double) (ax+ay);

   if (dx < 0)
      t -= 2.0;
   else
      if (dy < 0)
	 t += 4.0;
   t *= 90.0;
   if (t < 0)
      t = -t;
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
 * @param rgs a region map.
 * @param regions the region features array.
 */
Errc Bordure( const Reg2d &rgs, struct attrs *regions ) {
   Ulong r;

   // Marquage des mini et maxi.
   for (int y=0;y<rgs.Height();y++) {
      // Initialisation des valeurs de bord
      int d=y;
      int f=(2*rgs.Height())-1-y;
      // Examen des valeurs des labels de regions.
      for (int x=0;x<rgs.Width();x++) {
	 if ((r=rgs[y][x]) == 0)
	    continue;
         // set the bounding box.
         if (regions[r].bord == NULL) {
            regions[r].bord=new Point2d[rgs.Height()*2+1];
            regions[r].xf=x;	regions[r].yf=y;
            regions[r].xd=x;	regions[r].yd=y;
	    for (int i=0; i<rgs.Height(); i++) {
	       regions[r].bord[i].x=0;		// maxi
	       regions[r].bord[i].y=i;
	       regions[r].bord[(2*rgs.Height())-1-i].x=rgs.Width();	// mini
	       regions[r].bord[(2*rgs.Height())-1-i].y=i;
            }
	    regions[r].bord[d].x=x;
	    regions[r].bord[f].x=x;
         } else {
	    if (regions[r].yd>y) regions[r].yd=y;
	    if (regions[r].yf<y) regions[r].yf=y;
	    if (regions[r].xd>x) regions[r].xd=x;
	    if (regions[r].xf<x) regions[r].xf=x;
	    if (regions[r].bord[d].x<x)		// maxi
	       regions[r].bord[d].x=x;
	    if (regions[r].bord[f].x>x)		// mini
	       regions[r].bord[f].x=x;
         }
      }
   }

   // Construction de la liste des bords en tournant
   // dans l'ordre trigonometrique inverse.
   for (r=1;r<=rgs.Labels();r++) {
      if (regions[r].bord != NULL) {
	 regions[r].nbord=2*(regions[r].yf-regions[r].yd+1);
	 int j=0;
	 int d=regions[r].yd;
	 int f=regions[r].yf;
	 int i;
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
Errc Convexification( const Reg2d &rgs, struct attrs *regions ) {
   register int i;
   Double maxangle,angle,maxrayon,rayon,anglelimite;
   Long min;
   Long nbenv;

   for (Ulong r=1;r<=rgs.Labels();r++) {
      if (regions[r].bord == NULL) continue;
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
	    angle=AngleTeta(regions[r].bord[i],
			    regions[r].bord[nbenv-1]);
	    rayon=Rayon(regions[r].bord[i],
			regions[r].bord[nbenv-1]);
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
 * Calcule de la surface de l'enveloppe convexe
 * de chaque regions a partir de la liste des points
 * de l'enveloppe convexe.
 */
Errc SurfaceEnveloppe( const Reg2d &rgs, struct attrs *regions, Reg2d &rgd ) {
   register int i,j;
   Long ligne;
   Point2d pt1;
   Point2d pt2;
   Point2d pt3;
   Point2d pt4;

   // draw the convex hull for each region.
   for (Ulong r=1;r<=rgs.Labels();r++) {
      if (regions[r].bord == NULL || regions[r].nbord < 3) continue;
      pt1=regions[r].bord[0];
      pt3=pt1;
      pt2=regions[r].bord[i=1];
      pt4=regions[r].bord[j=regions[r].nbord-1];

      ligne=pt1.y;
      while (i<=j) {
	 Dessine(ligne++,pt1,pt2,pt3,pt4,r,rgd);
	 // Dessiner de pt1,pt2,p3pt4 sur la ligne.
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

Errc PConvexHull( const Reg2d &rgs, Reg2d &rgd ) {
   struct attrs* regions;

   regions = (struct attrs*)calloc(rgs.Labels()+1,sizeof(struct attrs));
   rgd=0;
   rgd.Labels(rgs.Labels());
   Bordure(rgs,regions);
   Convexification(rgs,regions);
   SurfaceEnveloppe(rgs,regions,rgd);

   return SUCCESS;
}

Ulong Etape(Reg3d& rgs, Reg3d& rgd) {
   Ulong compt;
   Long i,j,k;
   Ulong a,b,c;
   Long u,v,min,min1,min2,min3,max;
   Uchar vs;

   compt=0;
   u=v=0;

   for (i=0;i<rgs.Depth();i++)
      for (j=0;j<rgs.Height();j++)
	 for (k=0;k<rgs.Width();k++) {	// On traite chaque voxel.

	    v=rgd[i][j][k]=rgs[i][j][k];
	    min=min1=min2=min3=MAXLONG;
	    if (v) continue;

	    vs=0;
	    max=0;
	    a=i+v26z[vs];
	    b=j+v26y[vs];
	    c=k+v26x[vs];
	    if (rgs.Hold(a,b,c))
	       if ((u=rgs[a][b][c])>max) max=u;

	    a=i+v26z[vs+13];
	    b=j+v26y[vs+13];
	    c=k+v26x[vs+13];
	    if (rgs.Hold(a,b,c))
	       if ((u=rgs[a][b][c])>max) max=u;

	    if (max<min1) min1=max;

	    vs=3;
	    max=0;
	    a=i+v26z[vs];
	    b=j+v26y[vs];
	    c=k+v26x[vs];
	    if (rgs.Hold(a,b,c))
	       if ((u=rgs[a][b][c])>max) max=u;

	    a=i+v26z[vs+13];
	    b=j+v26y[vs+13];
	    c=k+v26x[vs+13];
	    if (rgs.Hold(a,b,c))
	       if ((u=rgs[a][b][c])>max) max=u;

	    if (max<min1) min1=max;

	    vs=12;
	    max=0;
	    a=i+v26z[vs];
	    b=j+v26y[vs];
	    c=k+v26x[vs];
	    if (rgs.Hold(a,b,c))
	       if ((u=rgs[a][b][c])>max) max=u;

	    a=i+v26z[vs+13];
	    b=j+v26y[vs+13];
	    c=k+v26x[vs+13];
	    if (rgs.Hold(a,b,c))
	       if ((u=rgs[a][b][c])>max) max=u;

	    if (max<min1) min1=max;

	    vs=18;
	    max=0;
	    a=i+v26z[vs];
	    b=j+v26y[vs];
	    c=k+v26x[vs];
	    if (rgs.Hold(a,b,c))
	       if ((u=rgs[a][b][c])>max) max=u;

	    a=i+v26z[(vs+13)%26];
	    b=j+v26y[(vs+13)%26];
	    c=k+v26x[(vs+13)%26];
	    if (rgs.Hold(a,b,c))
	       if ((u=rgs[a][b][c])>max) max=u;

	    if (max<min1) min1=max;

	    vs=0;
	    max=0;
	    a=i+v26z[vs];
	    b=j+v26y[vs];
	    c=k+v26x[vs];
	    if (rgs.Hold(a,b,c))
	       if ((u=rgs[a][b][c])>max) max=u;

	    a=i+v26z[vs+13];
	    b=j+v26y[vs+13];
	    c=k+v26x[vs+13];
	    if (rgs.Hold(a,b,c))
	       if ((u=rgs[a][b][c])>max) max=u;

	    if (max<min2) min2=max;

	    vs=1;
	    max=0;
	    a=i+v26z[vs];
	    b=j+v26y[vs];
	    c=k+v26x[vs];
	    if (rgs.Hold(a,b,c))
	       if ((u=rgs[a][b][c])>max) max=u;

	    a=i+v26z[vs+13];
	    b=j+v26y[vs+13];
	    c=k+v26x[vs+13];
	    if (rgs.Hold(a,b,c))
	       if ((u=rgs[a][b][c])>max) max=u;

	    if (max<min2) min2=max;

	    vs=10;
	    max=0;
	    a=i+v26z[vs];
	    b=j+v26y[vs];
	    c=k+v26x[vs];
	    if (rgs.Hold(a,b,c))
	       if ((u=rgs[a][b][c])>max) max=u;

	    a=i+v26z[vs+13];
	    b=j+v26y[vs+13];
	    c=k+v26x[vs+13];
	    if (rgs.Hold(a,b,c))
	       if ((u=rgs[a][b][c])>max) max=u;

	    if (max<min2) min2=max;

	    vs=20;
	    max=0;
	    a=i+v26z[vs];
	    b=j+v26y[vs];
	    c=k+v26x[vs];
	    if (rgs.Hold(a,b,c))
	       if ((u=rgs[a][b][c])>max) max=u;

	    a=i+v26z[(vs+13)%26];
	    b=j+v26y[(vs+13)%26];
	    c=k+v26x[(vs+13)%26];
	    if (rgs.Hold(a,b,c))
	       if ((u=rgs[a][b][c])>max) max=u;

	    if (max<min2) min2=max;

	    vs=9;
	    max=0;
	    a=i+v26z[vs];
	    b=j+v26y[vs];
	    c=k+v26x[vs];
	    if (rgs.Hold(a,b,c))
	       if ((u=rgs[a][b][c])>max) max=u;

	    a=i+v26z[vs+13];
	    b=j+v26y[vs+13];
	    c=k+v26x[vs+13];
	    if (rgs.Hold(a,b,c))
	       if ((u=rgs[a][b][c])>max) max=u;

	    if (max<min3) min3=max;

	    vs=10;
	    max=0;
	    a=i+v26z[vs];
	    b=j+v26y[vs];
	    c=k+v26x[vs];
	    if (rgs.Hold(a,b,c))
	       if ((u=rgs[a][b][c])>max) max=u;

	    a=i+v26z[vs+13];
	    b=j+v26y[vs+13];
	    c=k+v26x[vs+13];
	    if (rgs.Hold(a,b,c))
	       if ((u=rgs[a][b][c])>max) max=u;

	    if (max<min3) min3=max;

	    vs=11;
	    max=0;
	    a=i+v26z[vs];
	    b=j+v26y[vs];
	    c=k+v26x[vs];
	    if (rgs.Hold(a,b,c))
	       if ((u=rgs[a][b][c])>max) max=u;

	    a=i+v26z[vs+13];
	    b=j+v26y[vs+13];
	    c=k+v26x[vs+13];
	    if (rgs.Hold(a,b,c))
	       if ((u=rgs[a][b][c])>max) max=u;

	    if (max<min3) min3=max;

	    vs=12;
	    max=0;
	    a=i+v26z[vs];
	    b=j+v26y[vs];
	    c=k+v26x[vs];
	    if (rgs.Hold(a,b,c))
	       if ((u=rgs[a][b][c])>max) max=u;

	    a=i+v26z[vs+13];
	    b=j+v26y[vs+13];
	    c=k+v26x[vs+13];
	    if (rgs.Hold(a,b,c))
	       if ((u=rgs[a][b][c])>max) max=u;

	    if (max<min3) min3=max;

	    min=min1;
	    if (min2>min) min=min2;
	    if (min3>min) min=min3;

	    if ((min1)||(min2)||(min3)) {
	       compt+=1;
	       rgd[i][j][k]=min;
	    }

	 }

   return compt;
}

/*
 *
 */
Errc PConvexHull( Reg3d &rgs, Reg3d &rgd ) {
   Long i,j,k;
   Ulong compt;
   
   rgd.Labels(rgs.Labels());
   while ((compt=Etape(rgs,rgd))) {
      for (i=0;i<rgs.Depth();i++) {
	 for (j=0;j<rgs.Height();j++) {
	    for (k=0;k<rgs.Width();k++) {
	       rgs[i][j][k]=rgd[i][j][k];
	       rgd[i][j][k]=0;
	    }
	 }
      }
   }
   
   return SUCCESS;
}

#ifdef MAIN

/*
 * Modify only the following constants, and the function call.
 */
#define	USAGE	"usage: %s [-m mask] [rg_in|-] [rg_out|-]"
#define	PARC	0
#define	FINC	1
#define	FOUTC	1
#define	MASK	1

int main( int argc, char *argv[] ) {
   Errc result;              // The result code of the execution.
   Pobject* mask;            // The region mask.
   Pobject* objin[FINC+1];   // The input objects.
   Pobject* objs[FINC+1];    // The source objects masked by the mask.
   Pobject* objout[FOUTC+1]; // The output object.
   Pobject* objd[FOUTC+1];   // The result object of the execution.
   char* parv[PARC+1];       // The input parameters.

   ReadArgs(argc,argv,PARC,FINC,FOUTC,&mask,objin,objs,objout,objd,parv,USAGE,MASK);

   switch(objs[0]->Type()) {

   case Po_Reg2d :{
      Reg2d* const rgs=(Reg2d*)objs[0];
      objd[0]=new Reg2d(rgs->Size());
      Reg2d* const rgd=(Reg2d*)objd[0];

      result=PConvexHull(*rgs,*rgd);
   }break;

   case Po_Reg3d : {
      Reg3d* const rgs=(Reg3d*)objs[0];
      objd[0]=new Reg3d(rgs->Size());
      Reg3d* const rgd=(Reg3d*)objd[0];

      result=PConvexHull(*rgs,*rgd);

   } break;
   default:
      PrintErrorFormat(objin,FINC);
      result=FAILURE;
   }

   if (result) {
      WriteArgs(argc,argv,PARC,FINC,FOUTC,&mask,objin,objs,objout,objd,MASK);
   }

   Exit(result);
   return 0 ;
}

#endif
