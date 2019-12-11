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
 * @author Serge Coudé - 1999-02-19
 * @author Régis Clouard - 2001-04-03
 */

#include <math.h>
#include <pandore.h>
using namespace pandore;

/**
 * @file ppolygonalapproximation.cpp
 * Approximation polygonale d'une image de contours.
 * Parametres	:	error	: 0->oo, distance maximale autorisee.
 * Fonction	:	Approxime un contour par un polygone avec une error 
 *                      maximale de error pixels.
 */


/*
 * Trace une droite entre les deux points.
 */
static void TraceDroite( Img2duc &ims, Point2d pti, Point2d ptf, int coul ) {
   int i,j,max,min;
   float pente;

   if (pti.x!=ptf.x)
      pente=(float)(pti.y-ptf.y)/(pti.x-ptf.x);
   else
      pente=MAXFLOAT;

   if ((pente<-1) || (pente>1)) {
      if (ptf.y<pti.y) {
	 min=ptf.y; max=pti.y;
      }else{
	 min=pti.y; max=ptf.y;
      }
      for (i=max;i>=min;i--) {
	 j=(int)((i-ptf.y)/pente +ptf.x);
	 ims[i][j]=coul;
      }
   }else{
      if (ptf.x<pti.x) {
	 min=ptf.x; max=pti.x;
      } else {
	 min=pti.x; max=ptf.x;
      }
      for (j=max;j>=min;j--) {
	 i=(int)(pente*(j-ptf.x)+ptf.y);
	 ims[i][j]=coul;
      }
   }
}

/*
 *
 */
Ulong DistanceReelle( Point2d pt0, Point2d pt1 ) {
   return ((pt0.x-pt1.x)*(pt0.x-pt1.x)+(pt0.y-pt1.y)*(pt0.y-pt1.y));
}

Ulong DistanceReelleDroite( Point2d ptD0, Point2d ptD1, Point2d pt ) {
   int x2, y2, x3, y3, x, y;
   unsigned long d;
   float A, xbis;
  
   x2 = ptD1.x - ptD0.x;
   y2 = ptD1.y - ptD0.y;
   x3 = pt.x - ptD0.x;
   y3 = pt.y - ptD0.y;
  
   // pour changer du repere ecran a un repere Oij.
   y2 = -y2;
   y3 = -y3;
  
   A = float(y2) / float(x2);
   xbis = (y2*y3 + x2*x3)/(x2 + y2*A);
   x = (int)xbis;
   y = (int)(xbis*A);
  
   // on revient au premier repere 
   y = -y;
  
   d = DistanceReelle(Point2d(x3,-y3),Point2d(x,y));
  
   return d;
}

/*
 * Approxime un contour par un polygone regle par l'error aux contours.
 * ptdeb : dernier point du polygone approxime
 * ptj : point courant
 * ptj_1 : point precedant le point courant
 * pt : point "curseur" se baladant de ptdeb a ptj
 * erreur : distance entre le segment ptdeb-ptj et le point pt
 * erreurmax : distance maximale de l'ensemble des distances erreur
 */
void PPolygonalApproximation( Fifo<Point2d> &contour, Fifo<Point2d> &polygone, Short error ) {
   ListElement<Point2d>* ptdeb=(ListElement<Point2d>*)contour.Head();
   ListElement<Point2d> *ptj = ptdeb->Next();
   ListElement<Point2d> *ptj_1=ptdeb;
   ListElement<Point2d> *pt;
   int erreur, erreurmax;
   
   polygone.Push(ptdeb->Item()); // on empile l'origine du contour
   while (ptj) {
      pt = ptdeb;
      erreurmax = 0;
      
      // on calcule la distance de tous les points du contour
      // (compris entre ptdeb et ptj) et on conserve la distance maximale
      while (pt->Item().x != ptj->Item().x && pt->Item().y != ptj->Item().y) {
	 erreur = DistanceReelleDroite(ptdeb->Item(),ptj->Item(),pt->Item());
	 if (erreur > erreurmax) {
	    erreurmax = erreur;
	 }
	 pt = pt->Next();
      }
      
      // si la distance maximale est superieure a la distance autorisee,
      // alors le point precedant ptj est un sommet du polygone
      if (erreurmax > error) {
	 polygone.Push(ptj_1->Item());
	 ptdeb = ptj_1;
      }
      
      ptj_1 = ptj;
      ptj = ptj->Next(); 
   }
   polygone.Push(ptj_1->Item()); // on empile l'extremite du contour
}

/*
 * Suivi de contours.
 * Empile tous les points du contour debutant a (i,j),
 * en enroulant dans le sens trigonometrique inverse.
 * Remarque : Si un point a plusieurs voisins, il devient un point
 * fixe a conserver obligatoirement.
 */
void SuiviContour( Img2duc &ims, Fifo<Point2d> &contour, int yd, int xd ) {
   int v;
   int exdir, nwdir;
   int nbvoisins, lgcontour;
   Point2d pts, ptc;
  
   ptc.x = xd;ptc.y = yd;
   nwdir = 4;
   lgcontour=0;
   
   // tant qu'il y a des points de contours ...
   while (1) {
      contour.Push(ptc);
      ims[ptc] = 0;
      nbvoisins = 0;
      exdir = nwdir+4;
      // On cherche le point suivant, en calculant le nombre de voisins,
      // sans recompter celui que l'on vient de quitter.
      for (v=exdir+7; v>exdir; v--) {
	 pts = ptc + v8[v%8];
	 if (ims.Hold(pts) && (ims[pts]>0)) {
	    nwdir = v%8;
	    nbvoisins++;
	 }
      }
      lgcontour++;
      if (nbvoisins == 0)	// Plus de voisins.
	 break;
      if ((lgcontour > 1) && (nbvoisins>1)) { // Plusieurs voisins : Point a conserver
	 ims[ptc] = MAXUCHAR;
	 break;
      }else{
	 ptc = ptc + v8[nwdir];
      }
   }
}

/*
 * Procede par suivi de contours :
 * 	Empile tous les points de contours.
 *	Traite ensuite tous les points de contours.
 * couleur : couleur du polygone courant
 */
Errc PPolygonalApproximation( Img2duc &ims, Img2duc &imd, int error ) {
   register int x,y;
   Uchar couleur;
   Point2d pt0, pt1;
   Fifo<Point2d> contour,polygone;
  
   imd=0; // initialisation de l'image resultat
   
   couleur=0;
   // Balayage video de l'image.
   for (y=1; y<ims.Height()-1; y++) {
      for (x=1; x<ims.Width()-1; x++) {
	 // Recherche d'un premier point de contour.
	 if (ims[y][x]==0) {
	    continue;
	 }
	 
	 // Empilement de tous les points connexes.
	 // et suppression des points de l'ims.
	 SuiviContour(ims,contour,y,x);
      
	 // Approximation polygonale du contour.
	 PPolygonalApproximation(contour,polygone,error*error);
      
	 // Coloriage de l'image finale.
	 couleur = (couleur+1)%(MAXUCHAR+1); // polygone -> une couleur (purement joli! mais< 255).
	 pt0 = polygone.Pop();
	 while (!polygone.Empty()) {
	    pt1 = polygone.Pop();
	    TraceDroite(imd,pt0,pt1,couleur);
	    pt0 = pt1;
	 }

	 // on vide la pile contenant le contour.
	 while (!contour.Empty()) {
	    contour.Pop();
	 }
      }
   }
   return SUCCESS;
}

#ifdef MAIN

#define USAGE "usage: %s error [-m mask] [im_in|-] [im_out|-]"
#define PARC  1 
#define FINC  1
#define FOUTC 1
#define MASK  1

int main( int argc, char *argv[] ) {
   Errc result;
   Pobject *mask;
   Pobject *objin[FINC+1];
   Pobject *objs[FINC+1];
   Pobject *objout[FOUTC+1];
   Pobject *objd[FOUTC+1];
   char* parv[PARC+1];
   
   ReadArgs(argc,argv,PARC,FINC,FOUTC,&mask,objin,objs,objout,objd,parv,USAGE,MASK);
   
   switch(objs[0]->Type()) {
      
   case Po_Img2duc: {
      Img2duc *const ims=(Img2duc*)objs[0];
      objd[0] = new Img2duc(ims->Props());
      Img2duc *const imd=(Img2duc*)objd[0];
      
      result = PPolygonalApproximation(*ims,*imd,atoi(parv[0]));
   } break;
   
   default:
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
