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
 * @author Julien Robiaille - 2000-10-23
 * @author Régis Clouard - 2001-11-28
 */

#define _USE_MATH_DEFINES
#include <cmath>
#include <pandore.h>
using namespace pandore;

/**
 * @file pellipsoidalapproximation.cpp
 *
 * Approximation sous forme d'une ellipse de contours fermes.
 */

/**
 * Affiche une ellipse sur une image.
 */
void trace_ellipse( Img2duc &ims, Point2d centre, int a, int b, float alpha ) {
   int t;
   int x,y;
   float t2;
   
   for (t=-720;t<720;t++) {
      t2=(float)(t*M_PI/720);
      x=(int)(a*cos(t2)*cos(alpha)-b*sin(t2)*sin(alpha)+centre.x);
      y=(int)(a*cos(t2)*sin(alpha)+b*sin(t2)*cos(alpha)+centre.y);
      if (y>=0 && y<ims.Height() && x>=0 && x<ims.Width()) {
	 ims[y][x]=255;
      }
   }
}

/**
 * Construction de la matrice de donnees a partir
 * de la liste des points de contours.
 */
Errc determine_S( Fifo<Point2d> &contour, double *S, int nbpts ) {
   int k,i,j;
   double *D[6];
   Point2d pt;
   
   for (i=0;i<6;i++) {
      D[i]=new double[nbpts];
      if (D[i]==NULL) {
	 return FAILURE;
      }
   }
   
   for (i=0;i<36;S[i++]=0) ;
   
   // Nous determinons la matrice D
   // D=[xx xy yy x y 1]
   k=0;
   while(!contour.Empty()) {
      pt = contour.Pop();
      D[0][k]=pt.x*pt.x;
      D[1][k]=pt.x*pt.y;
      D[2][k]=pt.y*pt.y;
      D[3][k]=pt.x;
      D[4][k]=pt.y;
      D[5][k]=1;
      k++;
   }

   for (i=0;i<6;i++) {
      for (j=0;j<6;j++) {
	 for (k=0;k<nbpts;k++) {
	    S[i*6+j]+=D[i][k]*D[j][k];
	 }
      }
   }

   for (i=0;i<6;i++) {
      delete D[i];
   }
   return SUCCESS;
}

/**
 * Construction de la matrice de donnees
 */
Errc determine_S2( Img2duc &ims, double *S ) {
   int x,y,k,i,j;
   int nbpts=0;
   double *D[6];
   
   for (y=0;y<ims.Height();y++) {
      for (x=0;x<ims.Width();x++) {
	 if (ims[y][x]) {
	    nbpts++;
	 }
      }
   }
   for (i=0;i<6;i++) {
      D[i]=new double[nbpts];
      if (D[i]==NULL) {
	 return FAILURE;
      }
   }
  
   for (i=0;i<36;i++) {
      S[i]=0;
   }
  
   // Nous determinons la matrice D
   // D=[xx xy yy x y 1]
   k=0;
   for (y=0;y<ims.Height();y++) {
      for (x=0;x<ims.Width();x++) {
	 if ((ims[y][x])) {
	    D[0][k]=x*x;
	    D[1][k]=x*y;
	    D[2][k]=y*y;
	    D[3][k]=x;
	    D[4][k]=y;
	    D[5][k++]=1;
	 }
      }
   }
   for (i=0;i<6;i++) {
      for (j=0;j<6;j++) {
	 for (k=0;k<nbpts;k++) {
	    S[i*6+j]+=D[i][k]*D[j][k];
	 }
      }
   }
   for (i=0;i<6;i++) {
      delete D[i];
   }
   
   return SUCCESS;
}

/**
 * Decoupe en bloc 3-3 la matrice S.
 */
void  determine_S1_S2( double *S, double *S1, double *S2, double *S3, double *S4 ) {
   int i,j;
  
   for (i=0;i<3;i++)
      for (j=0;j<3;j++) {
	 S1[i*3+j]=S[i*6+j];
	 S2[i*3+j]=S[i*6+j+3];
	 S3[i*3+j]=S[(i+3)*6+j];
	 S4[i*3+j]=S[(i+3)*6+j+3];
      }
}

/**
 * Determine les racines de x^3+ax^2+b^x+c=0
 */
void  resolution_degre_3( double a, double b, double c, double *racines ) {
   double Q,R,teta;
  
   Q=(a*a-3*b)/9;
   R=(2*a*a*a-9*a*b+27*c)/54;
  
   teta=acos(R/sqrt(Q*Q*Q));
  
   racines[0]=-2*sqrt(Q)*cos(teta/3)-a/3;
   racines[1]=-2*sqrt(Q)*cos(teta/3+2*M_PI/3)-a/3;
   racines[2]=-2*sqrt(Q)*cos(teta/3-2*M_PI/3)-a/3;
}

/**
 * recherche le vecteur propre de T associe a la valeur propre l.
 */
void correspond_vecteur( double *T, double l, double *x ) {
   x[2]=1;
   x[1]=-(T[7]*(T[0]-l)-T[6]*T[1])/((T[4]-l)*(T[0]-l)-T[3]*T[1]);
   x[0]=-(x[1]*T[3]+T[6])/(T[0]-l);
}

/**
 * Empile tous les poin ts non nuls depuis (yd,xd).
 * Retourne le nombre de points empilés.
 */
static int EmpileTousLesPoints( const Img2duc &ims, Fifo<Point2d> &contour ) {
   int nbpts=0;
   Point2d pt;
   
   for (pt.y=0; pt.y<ims.Height();pt.y++) {
      for (pt.x=0;pt.x<ims.Width();pt.x++) {
	 if (ims[pt]) {
	    nbpts++;
	    contour.Push(pt);
	 }
      }
   }
   return nbpts;
}

/*
 * Suivi de contours.
 * Empile tous les points connexe d'un contour debutant a (i,j),
 */
static int EmpileLaChaine( Img2duc &ims, Fifo<Point2d> &contour, int yd, int xd ) {
   int v;
   int nbvoisins, lgcontour;
   Point2d pti,pts, ptc;
   Fifo<Point2d> bifurcation;
   
   ptc.x = xd;ptc.y = yd;
   lgcontour=1;
   contour.Push(ptc);
   
   // tant qu'il y a des points de contours ...
   while (1) {
      ims[ptc] = 0;
      
      nbvoisins = 0;
      // On cherche le point suivant, en calculant le nombre de voisins,
      // sans recompter celui que l'on vient de quitter.
      for (v=0; v<8; v++) {
	 pti = ptc + v8[v];
	 if (ims.Hold(pti) && (ims[pti]>0)) {
	    pts = pti;
	    nbvoisins++;
	 }
      }

      if (nbvoisins == 0) {
	 if (bifurcation.Empty()) {
	    break; // Plus de point.
	 }
	 ptc = bifurcation.Pop();
      } else {
	 lgcontour++;
	 contour.Push(ptc);
	 if (nbvoisins>1) {
	    bifurcation.Push(ptc);
	 }
	 ptc = pts;
      }
   }
   return lgcontour;
}

/*
 * Procede par suivi de contours :
 * 	Empile tous les points de contours.
 *	Traite ensuite tous les points de contours.
 * mode=0: tous les points
 * mode=1: chaine / chaine.
 * Retourne le nombre de points.
 */
Errc PEllipsoidalApproximation( Fifo<Point2d> &contour, int nbpt, Point2d *centre, double *a2, double *b2, float *alpha) {
   register int i,j;
   static double S[36],S1[9],S2[9],S3[9],S4[9];
   static double SS4[6], L[6], SSS1[9], SSS[9], wr[3], A[6], T[9];
   
   if (determine_S(contour,S,nbpt)== FAILURE) {
      std::cerr << "Error pellipsoidalapproximation: Not enough memory." << std::endl;
      return FAILURE;
   }
   
   determine_S1_S2(S,S1,S2,S3,S4);
   
   // ecriture sous forme reduite (S4 symetrique)
   int s=0;
   for (i=0;i<3;i++) {
      for (j=0;j<i+1;j++) {
	 SS4[s++]=S4[i*3+j];
      }
   }
   // decomposition de Cholesky (SS4=L'L) SS4 devient L
   // dppfa(SS4,3,info); //fonction de la bibliotheque Sun
   
   L[0]=sqrt(SS4[0]);
   L[1]=SS4[1]/L[0];
   L[2]=sqrt(SS4[2]-SS4[1]*SS4[1]/SS4[0]);
   L[3]=SS4[3]/L[0];
   L[4]=(SS4[4]-SS4[1]*SS4[3]/SS4[0])/L[2];
   L[5]=sqrt(SS4[5]-SS4[3]*SS4[3]/SS4[0]-L[4]*L[4]);
   
   for (s=0;s<6;s++) {
      SS4[s]=L[s];
   }
   
   // inversion de SS4 (SS4 devient SS4^(-1))
   //  dppdi(SS4,3,ddet,11); //fonction de la bibliotheque Sun
   L[0]=1/SS4[0];
   L[2]=1/SS4[2];
   L[5]=1/SS4[5];
   L[1]=-L[2]*SS4[1]*L[0];
   L[4]=-L[5]*SS4[4]*L[2];
   L[3]=(-L[4]*SS4[1]-L[5]*SS4[3])*L[0];
   
   SS4[0]=L[0]*L[0]+L[1]*L[1]+L[3]*L[3];
   SS4[1]=L[1]*L[2]+L[3]*L[4];
   SS4[2]=L[2]*L[2]+L[4]*L[4];
   SS4[3]=L[3]*L[5];
   SS4[4]=L[4]*L[5];
   SS4[5]=L[5]*L[5];
   
   //  ON CONSTUIT S1+S2*S4^(-1)(-S3)=SSS   ET ON RESOUT SSS*A1=LAMBDA*C1*A1
   //  S4 devient S4^(-1)
   s=0;
   for (i=0;i<3;i++) {
      for (j=0;j<i+1;j++) {
	 S4[i*3+j]=S4[j*3+i]=SS4[s++];
      }
   }
   // calcul de SSS
   //  S3 devient -S3
   for (i=0;i<3;i++)
      for (j=0;j<3;j++)
	 S3[i*3+j]=-S3[i*3+j];
   
   int k;
   double som_int; // somme intermediaire
   for (i=0;i<3;i++) {
      for (j=0;j<3;j++) {
	 som_int=0;
	 for (k=0;k<3;k++) {
	    som_int+=S4[i*3+k]*S3[k*3+j];
	 }
	 SSS1[i*3+j]=som_int;
      }
   }
   for (i=0;i<3;i++) {
      for (j=0;j<3;j++) {
	 som_int=0;
	 for (k=0;k<3;k++) {
	    som_int+=S2[i*3+k]*SSS1[k*3+j];
	 }
	 SSS[i*3+j]=som_int+S1[i*3+j];
      }
   }
   // determination de T tel que T*A1=LAMBDA*A1
   for (i=0;i<3;i++) {
      for (j=0;j<3;j++) {
	 T[j*3+i]=-SSS[(2-i)*3+j]/2;
      }
   }
   for (j=0;j<3;j++) {
      T[j*3+1]=+SSS[1*3+j];
   }
   
   
   // recherche des valeurs propres wr et vecteurs propres vr de T
   // dgeev('n','v',3,T,3,wr,wi,scratch,1,vr,9,info); //bibliotheque Sun
   
   // LAMBDA ET A1 SONT DETERMINEES
   // on determine A2 puis A (a,b,c,d,e,f) caracterise l'ellipse
   // A2=S4^(-1)(-S3*A1)=SSS1*A1
   
   // recherche des valeurs propres
   resolution_degre_3(-(T[8]+T[4]+T[0]),
		      -(T[1]*T[3]+T[2]*T[6]+T[5]*T[7]-T[4]*T[8]-T[0]*T[8]-T[0]*T[4]),
		      -(T[0]*T[4]*T[8]+T[1]*T[5]*T[6]+T[2]*T[3]*T[7]
			-T[0]*T[5]*T[7]-T[4]*T[2]*T[6]-T[8]*T[1]*T[3]),
		      wr);
   // recherche du vecteur propre associe a la valeur propre negative (methode)
   if (wr[0]<wr[1] && wr[0]<wr[2]) correspond_vecteur(T,wr[0],A);
   if (wr[2]<wr[1] && wr[2]<wr[0]) correspond_vecteur(T,wr[2],A);
   if (wr[1]<wr[2] && wr[1]<wr[0]) correspond_vecteur(T,wr[1],A);
   
   // determination de A qui caracterise l'ellipse
   for (i=0;i<3;i++) {
      som_int=0;
      for (j=0;j<3;j++) {
	 som_int+=SSS1[i*3+j]*A[j];
      }
      A[i+3]=som_int;
   }
   
   // passage de l'equation implicite a l'equation parametrique
   *alpha=(float)atan(A[1]/(A[0]-A[2]))/2.0F;
   double x02=((2*A[3]*A[2]-A[4]*A[1])/(A[1]*A[1]-4*A[0]*A[2]));
   double y02=((2*A[4]*A[0]-A[3]*A[1])/(A[1]*A[1]-4*A[0]*A[2]));
   
   double facteurK=4*(x02*x02*A[0]+y02*y02*A[2]+x02*y02*A[1]-A[5])
      /((A[0]+A[2])*(A[0]+A[2])-(A[1]/sin(2* (*alpha)))*(A[1]/sin(2*(*alpha))));
   
   *a2=sqrt(facteurK*(A[0]+A[2]-A[1]/sin(2*(*alpha)))/2);
   *b2=sqrt(facteurK*(A[0]+A[2]+A[1]/sin(2*(*alpha)))/2);
   
   centre->x=(Long)x02;   centre->y=(Long)y02;
   return SUCCESS;
}

/**
 * Detection des points a elliptiser.
 */
Errc PEllipsoidalApproximation( Img2duc &ims, Img2duc &imd, int mode ) {
   Fifo<Point2d> contour;
   int nbpt;
   Errc retour=FAILURE;

   // Parametre de l'ellipse.
   Point2d centre;
   double a2,b2;
   float alpha;
   
   imd=0; // initialisation de l'image resultat
   if (mode == 0) { // Cas tous les points
      nbpt=EmpileTousLesPoints(ims,contour);
      retour=PEllipsoidalApproximation(contour,nbpt,&centre,&a2,&b2,&alpha);
      trace_ellipse(imd,centre,(int)a2,(int)b2,alpha);
   } else { // Cas toutes les chaines.
      // Balayage video de l'image.
      for (int y=0; y<ims.Height(); y++)
	 for (int x=0; x<ims.Width(); x++) {	 
	    // Recherche d'un premier point de contour.
	    if (ims[y][x]==0) {
	       continue;
	    }
	    // Empilement de tous les points connexes.
	    // et suppression des points de l'ims.
	    nbpt=EmpileLaChaine(ims,contour,y,x);
	    retour=PEllipsoidalApproximation(contour,nbpt,&centre,&a2,&b2,&alpha);
	    trace_ellipse(imd,centre,(int)a2,(int)b2,alpha);
	 }
   }
   return retour;
}

#ifdef MAIN

/*
 * Modify only the following constants, and the operator switches.
 */
#define	USAGE	"usage: %s mode [-m mask] [im_in|-] [im_out|-]"
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
      objd[0] = new Img2duc(ims->Size());
      Img2duc *const imd=(Img2duc*)objd[0];
      
      result = PEllipsoidalApproximation(*ims,*imd,atoi(parv[0]));
   } break;
   
   default:
      PrintErrorFormat(objin,FINC);
      result=FAILURE;
   }
   
   if (result)
      WriteArgs(argc,argv,PARC,FINC,FOUTC,&mask,objin,objs,objout,objd,MASK);
   Exit(result);
   return 0;
}

#endif
