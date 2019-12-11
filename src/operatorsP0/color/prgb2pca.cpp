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
 * @author Olivier Lezoray - 1997-07-04
 */

#include <math.h>
#include <pandore.h>
using namespace pandore;

/**
 * @author prgb2pca.cpp
 * Reference: Numerical Recipes.
 */

#define ROTATE(a, i, j, k, l) { g = a[i][j]; h = a[k][l]; a[i][j] = ((g) - ((s) * (h + g * tau))); a[k][l] = ((h) + (s) * (g - h * tau)); }

#define DIM 3
#define MAX_B  999999

#define NR_END 1

/*
 * Allocates a float vector with subscript range v[nl..nh]
 */
float *vector( long nl, long nh )  {
   float *v;
  
   v=(float *)malloc((unsigned int) ((nh-nl+1+NR_END)*sizeof(float)));
   if (!v) {
      return NULL;
   }
   return v-nl+NR_END;
}

/*
 * Frees a float vector allocated with vector()
 */
void free_vector( float *v, long nl) {
   free((char*) (v+nl-NR_END));
}

Errc jacobi( float a[][DIM], int n, float d[], float v[][DIM] , int *nrot ) {
   int j,iq,ip,i;
   float tresh,theta,tau,t,sm,s,h,g,c,*b,*z;
  
   b=vector(1,n);
   z=vector(1,n);
   for (ip=1;ip<=n;ip++) {
      for (iq=1;iq<=n;iq++) v[ip-1][iq-1]=0.0;
      v[ip-1][ip-1]=1.0;
   }
   for (ip=1;ip<=n;ip++) {
      b[ip]=d[ip-1]=a[ip-1][ip-1];
      z[ip]=0.0;
   }
   *nrot=0;
   for (i=1;i<=50;i++) {
      sm=0.0;
      for (ip=1;ip<=n-1;ip++) {
	 for (iq=ip+1;iq<=n;iq++)
	    sm += (Float)fabs(a[ip-1][iq-1]);
      }
      if (sm == 0.0) {
	 free_vector(z,1);
	 free_vector(b,1);
	 return SUCCESS;
      }
      if (i < 4)
	 tresh=0.2F*sm/(n*n);
      else
	 tresh=0.0;
      for (ip=1;ip<=n-1;ip++) {
	 for (iq=ip+1;iq<=n;iq++) {
	    g=(Float)(100.0*fabs(a[ip-1][iq-1]));
	    if (i > 4 && (float)(fabs(d[ip-1])+g) == (float)fabs(d[ip-1])
		&& (float)(fabs(d[iq-1])+g) == (float)fabs(d[iq-1]))
	       a[ip-1][iq-1]=0.0;
	    else if (fabs(a[ip-1][iq-1]) > tresh) {
	       h=d[iq-1]-d[ip-1];
	       if ((float)(fabs(h)+g) == (float)fabs(h))
		  t=(a[ip-1][iq-1])/h;
	       else {
		  theta=(Float)(0.5*h/(a[ip-1][iq-1]));
		  t=(Float)(1.0/(fabs(theta)+sqrt(1.0+theta*theta)));
		  if (theta < 0.0) t = -t;
	       }
	       c=(Float)(1.0/sqrt(1+t*t));
	       s=t*c;
	       tau=(Float)(s/(1.0+c));
	       h=t*a[ip-1][iq-1];
	       z[ip] -= h;
	       z[iq] += h;
	       d[ip-1] -= h;
	       d[iq-1] += h;
	       a[ip-1][iq-1]=0.0;

	       for (j=1;j<=ip-1;j++) {
		  ROTATE(a,j-1,ip-1,j-1,iq-1);
	       }
	       for (j=ip+1;j<=iq-1;j++) {
		  ROTATE(a,ip-1,j-1,j-1,iq-1);
	       }
	       for (j=iq+1;j<=n;j++) {
		  ROTATE(a,ip-1,j-1,iq-1,j-1);
	       }
	       for (j=1;j<=n;j++) {
		  ROTATE(v,j-1,ip-1,j-1,iq-1);
	       }
	       ++(*nrot);
	    }
	 }
      }
      for (ip=1;ip<=n;ip++) {
	 b[ip] += z[ip];
	 d[ip-1]=b[ip];
	 z[ip]=0.0;
      }
   }
   return FAILURE; // Too many iterations in routine jacobi
}

float covariance( int comp1, int comp2, Imc2duc& ims, float m[] ) {
   Point2d p;
   float somme = 0.F;
   
   for (p.y=0;p.y<ims.Height();p.y++) {
      for (p.x=0;p.x<ims.Width();p.x++) {
	 somme+=(float(ims(comp1,p.y,p.x))/256.0F-m[comp1])*(float(ims(comp2,p.y,p.x))/256.0F-m[comp2]);
      }
   }
   
   return somme/float(ims.VectorSize());
}

float covariance( int comp1, int comp2, Imc3duc& ims, float m[] ) {
   Point3d p;
   float somme = 0.F;
   
   for (p.z=0;p.z<ims.Depth();p.z++) {
      for (p.y=0;p.y<ims.Height();p.y++) {
	 for (p.x=0;p.x<ims.Width();p.x++) {
	    somme+=(float(ims(comp1,p.z,p.y,p.x))/256.0F-m[comp1])*(float(ims(comp2,p.z,p.y,p.x))/256.0F-m[comp2]);
	 }
      }
   }
   
   return somme/float(ims.VectorSize());
}

/*
 * Transposition d'une matrice n*n
 */
void transpose( float m[DIM][DIM], float im[DIM][DIM] ) {
   int y, x;
   
   for (y=0; y<3; y++) {
      for (x=0; x<3; x++) {
	 im[y][x] = m[x][y];
      }
   }
}

/*
 * Tri des valeurs propres par ordre decroissant (tri a bulles)
 */

void tri( float tab[], int pos[] ) {
   int i, itmp;
   float ftmp;
   
   i = 0;
   while (i<2) {
      if (tab[i] >= tab[i+1]) {
	 i++;
      } else { 
	 ftmp = tab[i]; tab[i] = tab[i+1]; tab[i+1] = ftmp;
	 itmp = pos[i]; pos[i] = pos[i+1]; pos[i+1] = itmp;
	 i = 0;
      }
   }
}

Errc PRGB2PCA( Imc2duc &ims, Imc2duc &imd ) {
   float e[3]={0.F,0.F,0.F}, s[3]={0.F,0.F,0.F}, a[3][3], m[3]={0.F,0.F,0.F};
   float vp[3]={0.F,0.F,0.F}, vcp[3][3], bornes[3][3], ia[3][3];
   int i,j,k, nrot;
   int pos[3];

   Point2d p;
   for (i=0; i<3; i++) {
      bornes[i][0]=MAX_B;
      bornes[i][1]=-MAX_B;
   }
   
   for (i=0;i<3;i++) {
      for (j=0;j<3;j++) {
	 ia[i][j]=a[i][j]=vcp[i][j]=bornes[i][j]=0.F;
      }
   }
   
   for (i=0; i<3; i++) {
      pos[i] = i;
   }
   
   /* Calcul des valeurs moyennes */
   for (p.y=0;p.y<ims.Height();p.y++) {
      for (p.x=0;p.x<ims.Width();p.x++) {
	 if (ims.X[p]!=0) {
	    m[0] += float(ims.X[p])/256.0F;
	 }
	 if (ims.Y[p]!=0) {
	    m[1] += float(ims.Y[p])/256.0F;
	 }
	 if (ims.Z[p]!=0) {
	    m[2] += float(ims.Z[p])/256.0F;
	 }
      }
   }
   if (m[0] != 0.F) {
      m[0] /= (ims.VectorSize());
   }
   if (m[1] != 0.F) {
      m[1] /= (ims.VectorSize());
   }
   if (m[2] != 0.F) {
      m[2] /= (ims.VectorSize());
   }
   
   /* Calcul de la matrice de covariance */  
   for (i=0; i<3; i++) {
      for (j=0; j<3; j++) {
	 a[i][j] = covariance(i, j, ims, m);
      }
   }
   
   /* Diagonalisation et calcul des valeurs et vecteurs propres */  
   if (!jacobi(a, 3, vp, vcp, &nrot)) {
      return FAILURE;
   }
   tri(vp, pos); // vp : vecteurs propres.
   transpose(vcp, ia); // vcp : vecteurs propres normalises.
   
   /* Calcul des bornes dans l'image de destination */
   for (p.y=0;p.y<ims.Height();p.y++) {
      for (p.x=0;p.x<ims.Width();p.x++) {
	 e[0]=(float)ims.X[p]/256.0F - m[0];
	 e[1]=(float)ims.Y[p]/256.0F - m[1];
	 e[2]=(float)ims.Z[p]/256.0F - m[2];
	 
	 //s[0] = e[0]*vcp[0][0]+e[1]*vcp[1][0]+e[2]*vcp[2][0];
	 //s[1] = e[0]*vcp[0][1]+e[1]*vcp[1][1]+e[2]*vcp[2][1];
	 //s[2] = e[0]*vcp[0][2]+e[1]*vcp[1][2]+e[2]*vcp[2][2];
	 
	 s[0] = e[0]*ia[0][0]+e[1]*ia[1][0]+e[2]*ia[2][0];
	 s[1] = e[0]*ia[0][1]+e[1]*ia[1][1]+e[2]*ia[2][1];
	 s[2] = e[0]*ia[0][2]+e[1]*ia[1][2]+e[2]*ia[2][2];
	 
	 for (k=0; k<3; k++) {
	    if (s[k] < bornes[k][0]) {
	       bornes[k][0] = s[k];
	    }
	    if (s[k] > bornes[k][1]) {
	       bornes[k][1] = s[k];
	    }
	 }
      }
   } 
   for (p.y=0;p.y<ims.Height();p.y++) {
      for (p.x=0;p.x<ims.Width();p.x++) {

	 e[0]=(float)ims.X[p]/256.0F - m[0];
	 e[1]=(float)ims.Y[p]/256.0F - m[1];
	 e[2]=(float)ims.Z[p]/256.0F - m[2];
	
	 //s[0] = e[0]*vcp[0][0]+e[1]*vcp[1][0]+e[2]*vcp[2][0];
	 //s[1] = e[0]*vcp[0][1]+e[1]*vcp[1][1]+e[2]*vcp[2][1];
	 //s[2] = e[0]*vcp[0][2]+e[1]*vcp[1][2]+e[2]*vcp[2][2];  
	
	 s[0] = e[0]*ia[0][0]+e[1]*ia[1][0]+e[2]*ia[2][0];
	 s[1] = e[0]*ia[0][1]+e[1]*ia[1][1]+e[2]*ia[2][1];
	 s[2] = e[0]*ia[0][2]+e[1]*ia[1][2]+e[2]*ia[2][2];
	
	 //imd.X[p]=Uchar((s[0]-bornes[0][0])/(bornes[0][1]-bornes[0][0])*256.0);
	 //imd.Y[p]=Uchar((s[1]-bornes[1][0])/(bornes[1][1]-bornes[1][0])*256.0);
	 //imd.Z[p]=Uchar((s[2]-bornes[2][0])/(bornes[2][1]-bornes[2][0])*256.0);
	
	 imd.X[p]=Uchar((s[pos[0]]-bornes[pos[0]][0])/(bornes[pos[0]][1]-bornes[pos[0]][0])*256.0);
	 imd.Y[p]=Uchar((s[pos[1]]-bornes[pos[1]][0])/(bornes[pos[1]][1]-bornes[pos[1]][0])*256.0);
	 imd.Z[p]=Uchar((s[pos[2]]-bornes[pos[2]][0])/(bornes[pos[2]][1]-bornes[pos[2]][0])*256.0);
	
      }
   }  
   return SUCCESS;
}

Errc PRGB2PCA( Imc3duc &ims, Imc3duc &imd ) {
   float e[3]={0.F,0.F,0.F}, s[3]={0.F,0.F,0.F}, a[3][3], m[3]={0.F,0.F,0.F};
   float vp[3]={0.F,0.F,0.F}, vcp[3][3], bornes[3][3], ia[3][3];
   int i,j,k, nrot;
   int pos[3];
   
   Point3d p;
   for (i=0; i<3; i++) {
      bornes[i][0]=MAX_B;
      bornes[i][1]=-MAX_B;
   }
   
   for (i=0;i<3;i++) {
      for (j=0;j<3;j++) {
	 ia[i][j]=a[i][j]=vcp[i][j]=bornes[i][j]=0.F;
      }
   }
   
   for (i=0; i<3; i++) pos[i] = i;
   
   /* Calcul des valeurs moyennes */
   for (p.z=0;p.z<ims.Depth();p.z++) {
      for (p.y=0;p.y<ims.Height();p.y++) {
	 for (p.x=0;p.x<ims.Width();p.x++) {
	    if (ims.X[p]!=0) m[0] += float(ims.X[p])/256.0F;
	    if (ims.Y[p]!=0) m[1] += float(ims.Y[p])/256.0F;
	    if (ims.Z[p]!=0) m[2] += float(ims.Z[p])/256.0F;
	 }
      }
   }
   if (m[0]!=0.F) m[0] /= (ims.VectorSize());
   if (m[1]!=0.F) m[1] /= (ims.VectorSize());
   if (m[2]!=0.F) m[2] /= (ims.VectorSize());
   
   /* Calcul de la matrice de covariance */  
   for (i=0; i<3; i++)
      for (j=0; j<3; j++) 
	 a[i][j] = covariance(i, j, ims, m);
   
   /* Diagonalisation et calcul des valeurs et vecteurs propres */  
   if (!jacobi(a, 3, vp, vcp, &nrot))
      return FAILURE;
   tri(vp, pos); // vp : vecteurs propres.
   transpose(vcp, ia); // vcp : vecteurs propres normalises.
   
   /* Calcul des bornes dans l'image de destination */
   for (p.y=0;p.y<ims.Height();p.y++) {
      for (p.x=0;p.x<ims.Width();p.x++) {
	 e[0]=(float)ims.X[p]/256.0F - m[0];
	 e[1]=(float)ims.Y[p]/256.0F - m[1];
	 e[2]=(float)ims.Z[p]/256.0F - m[2];
	 
	 //s[0] = e[0]*vcp[0][0]+e[1]*vcp[1][0]+e[2]*vcp[2][0];
	 //s[1] = e[0]*vcp[0][1]+e[1]*vcp[1][1]+e[2]*vcp[2][1];
	 //s[2] = e[0]*vcp[0][2]+e[1]*vcp[1][2]+e[2]*vcp[2][2];
	 
	 s[0] = e[0]*ia[0][0]+e[1]*ia[1][0]+e[2]*ia[2][0];
	 s[1] = e[0]*ia[0][1]+e[1]*ia[1][1]+e[2]*ia[2][1];
	 s[2] = e[0]*ia[0][2]+e[1]*ia[1][2]+e[2]*ia[2][2];
	 
	 for (k=0; k<3; k++) {
	    if (s[k] < bornes[k][0]) {
	       bornes[k][0] = s[k];
	    }
	    if (s[k] > bornes[k][1]) {
	       bornes[k][1] = s[k];
	    }
	 }
      }
   }
   for (p.y=0;p.y<ims.Height();p.y++) {
      for (p.x=0;p.x<ims.Width();p.x++) {

	 e[0]=(float)ims.X[p]/256.0F - m[0];
	 e[1]=(float)ims.Y[p]/256.0F - m[1];
	 e[2]=(float)ims.Z[p]/256.0F - m[2];
	
	 //s[0] = e[0]*vcp[0][0]+e[1]*vcp[1][0]+e[2]*vcp[2][0];
	 //s[1] = e[0]*vcp[0][1]+e[1]*vcp[1][1]+e[2]*vcp[2][1];
	 //s[2] = e[0]*vcp[0][2]+e[1]*vcp[1][2]+e[2]*vcp[2][2];  
	
	 s[0] = e[0]*ia[0][0]+e[1]*ia[1][0]+e[2]*ia[2][0];
	 s[1] = e[0]*ia[0][1]+e[1]*ia[1][1]+e[2]*ia[2][1];
	 s[2] = e[0]*ia[0][2]+e[1]*ia[1][2]+e[2]*ia[2][2];
	
	 //imd.X[p]=Uchar((s[0]-bornes[0][0])/(bornes[0][1]-bornes[0][0])*256.0);
	 //imd.Y[p]=Uchar((s[1]-bornes[1][0])/(bornes[1][1]-bornes[1][0])*256.0);
	 //imd.Z[p]=Uchar((s[2]-bornes[2][0])/(bornes[2][1]-bornes[2][0])*256.0);
	
	 imd.X[p]=Uchar((s[pos[0]]-bornes[pos[0]][0])/(bornes[pos[0]][1]-bornes[pos[0]][0])*256.0);
	 imd.Y[p]=Uchar((s[pos[1]]-bornes[pos[1]][0])/(bornes[pos[1]][1]-bornes[pos[1]][0])*256.0);
	 imd.Z[p]=Uchar((s[pos[2]]-bornes[pos[2]][0])/(bornes[pos[2]][1]-bornes[pos[2]][0])*256.0);
	
      }
   }
   return SUCCESS;
} 


#ifdef MAIN

/*
 * Modify only the following constants, and the operator switches.
 */
#define USAGE	"usage: %s [-m mask] [im_in|-] [im_out|-]"
#define PARC	0
#define FINC	1
#define FOUTC	1
#define MASK	2

int main( int argc, char *argv[] ) {
   Errc result;              // The result code of the execution.
   Pobject* mask;            // The region mask.
   Pobject* objin[FINC+1];   // The input objects.
   Pobject* objs[FINC+1];    // The source objects masked by the mask.
   Pobject* objout[FOUTC+1]; // The ouput object.
   Pobject* objd[FOUTC+1];   // The result object of the execution.
   char* parv[PARC+1];       // The input parameters.
  
   ReadArgs(argc,argv,PARC,FINC,FOUTC,&mask,objin,objs,objout,objd,parv,USAGE,MASK);

   switch(objs[0]->Type()) {
   case Po_Imc2duc :{
      Imc2duc* const ims=(Imc2duc*)objs[0];
      objd[0]=new Imc2duc(ims->Props());
      Imc2duc* const imd=(Imc2duc*)objd[0];
    
      result=PRGB2PCA(*ims,*imd);
   } break;
   case Po_Imc3duc :{
      Imc3duc* const ims=(Imc3duc*)objs[0];
      objd[0]=new Imc3duc(ims->Props());
      Imc3duc* const imd=(Imc3duc*)objd[0];
    
      result=PRGB2PCA(*ims,*imd);
   } break;
   default :
      PrintErrorFormat(objin,FINC);
      result = FAILURE;
   }
  
   if (result) {
      WriteArgs(argc,argv,PARC,FINC,FOUTC,&mask,objin,objs,objout,objd,MASK);
   }
   Exit(result);
   return 0;
}

#endif

