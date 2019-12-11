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
 * @author Olivier Lezoray - 1997-02-26
 * @author Olivier Lezoray - 2002-05-21
 * @author Regis Couard - Sep 27, 2012 (border)
 */

#include <math.h>
#include <pandore.h>
using namespace pandore;

/**
 * @file ppeergroupfiltering.cpp
 * PeerGroupFiltering
 */

float Distance( const Imc2duc &ims, int y1, int x1, int y2, int x2 ) {
   if (x2==x1 && y2==y1) return 0.F;
   return float(sqrt((double)SQR(ims.X[y1][x1]-ims.X[y2][x2])+
		     SQR(ims.Y[y1][x1]-ims.Y[y2][x2])+
		     SQR(ims.Z[y1][x1]-ims.Z[y2][x2])));
}

float Distance( const Imc2dsl &ims, int i, int j, int i1, int j1 ) {
   if (i1==i && j1==j) return 0.F;
   float tmp=float(sqrt(pow((double)(ims.X[i][j]-ims.X[i1][j1]),2.0)+
			pow((double)(ims.Y[i][j]-ims.Y[i1][j1]),2.0)+
			pow((double)(ims.Z[i][j]-ims.Z[i1][j1]),2.0)));
   return tmp;
}

float Distance( const Imc2dsf &ims, int i, int j, int i1, int j1 ) {
   if (i1==i && j1==j) return 0.F;
   float tmp=float(sqrt(pow((double)(ims.X[i][j]-ims.X[i1][j1]),2.0)+
			pow((double)(ims.Y[i][j]-ims.Y[i1][j1]),2.0)+
			pow((double)(ims.Z[i][j]-ims.Z[i1][j1]),2.0)));
   return tmp;
}

/**
 * Bubble sort!
 */
void Ordonne( float* tableau, int *voisinsy, int *voisinsx, int nombre ) {
   bool echange=true;
   
   while (echange) {
      echange = false;
      for (int i = 0; i < nombre; i++) {
	 if (tableau[i] > tableau[i + 1]) {
	    echange = true;
	    float temp = tableau[i];
	    tableau[i] = tableau[i + 1];
	    tableau[i + 1] = temp;
	    int tmp = voisinsy[i];
	    voisinsy[i] = voisinsy[i + 1];
	    voisinsy[i + 1] = tmp;
	    tmp = voisinsx[i];
	    voisinsx[i] = voisinsx[i + 1];
	    voisinsx[i + 1] = tmp;
	 }
      }
   }
}

Errc PPeerGroupFiltering( const Imc2duc &ims, Imc2duc &imd, float alpha ) {
   int taille=3;
   int halfsize=taille/2;
   int k=taille*taille-1;
   int k1=k+1;
   int i,j,x,y;
   float *distance=new float [k1];
   int *voisinsx=new int[k1];
   int *voisinsy=new int[k1];

   float *J=new float [k1];
   float *f=new float [k1];
   bool *AConsiderer= new bool[k1];
   float a1,a2,s1,s2;
   float temp;
   float X,Y,Z;
   int max;
   int Gauss[3][3]={{1,2,1},{2,4,2},{1,2,1}};
   int somme;

   for (y=halfsize;y<ims.Height()-halfsize;y++) {
      for (x=halfsize;x<ims.Width()-halfsize;x++) {

	 // Initialisation
	 for (i=0; i<k1; i++) {
	    AConsiderer[i]=true;
	    J[i]=f[i]=0;
	 }

	 i=0;
	 for (int y1=-halfsize;y1<=halfsize;y1++) {
	    for (int x1=-halfsize;x1<=halfsize;x1++) {
	       distance[i]=Distance(ims,y,x,y+y1,x+x1);
	       voisinsx[i]=x1;
	       voisinsy[i]=y1;
	       i++;
	    }
	 }
	 // Tri par ordre croissant.
	 Ordonne(distance,voisinsy,voisinsx,k);

	 for (i=0;i<k;i++) {
	    f[i]=distance[i+1]-distance[i];
	 }
	 for (i=0;i<=halfsize;i++) {
	    if (f[i]<=alpha) {
	       for (j=i;j>=0;j--) {
		  AConsiderer[j]=false;
	       }
	    }
	 }
	 
	 for (i=k-1;i>=k-1-halfsize;i--) {
	    if (f[i]<=alpha) {
	       for (j=i;j<=k;j++) {
		  AConsiderer[j]=false;
	       }
	    }
	 }

	 for (i=1;i<=k;i++) {
	    J[i]=0.F;
	    if (!AConsiderer[i]) continue;

	    a1=a2=0.F;
	    for (j=0;j<=i-1;j++)
	       a1+=distance[j];
	    for (j=i;j<=k;j++)
	       a2+=distance[j];
	    if (a1!=0.F) a1/=float(i);
	    if (a2!=0.F) a2/=float(k+1-i);

	    s1=s2=0.F;
	    for (j=0;j<=i-1;j++)
	       s1+=SQR(distance[j]-a1);
	    for (j=i;j<=k;j++)
	       s2+=SQR(distance[j]-a2);

	    if ((s1+s2)!=0.F) 
	       J[i]=SQR(a1-a2)/(s1+s2);
	 }
		
	 //trouver le max des J
	 temp = J[1];
	 max = 1;
	 for (i = 2; i <= k; i++) {
	    if (temp < J[i]) {
	       temp = J[i];
	       max = i;
	    }
	 }
	 X = Y = Z = 0.0F;
	 somme = Gauss[1][1];
	 X += (float)ims.X[y][x] *somme;
	 Y += (float)ims.Y[y][x] * somme;
	 Z += (float)ims.Z[y][x] *somme;

	 for (i = 1; i < max; i++) {
	    int coeff = Gauss[voisinsy[i]+1][voisinsx[i]+1];
	    somme += coeff;
	    X += coeff * (float)ims.X[y+voisinsy[i]][x+voisinsx[i]];
	    Y += coeff * (float)ims.Y[y+voisinsy[i]][x+voisinsx[i]];
	    Z += coeff * (float)ims.Z[y+voisinsy[i]][x+voisinsx[i]];
	 }

	 imd.X[y][x] = (Imc2duc::ValueType)(X / somme);
	 imd.Y[y][x] = (Imc2duc::ValueType)(Y / somme);
	 imd.Z[y][x] = (Imc2duc::ValueType)(Z / somme);
      }
   }
   imd.Frame(ims,1);
   return SUCCESS;
}

Errc PPeerGroupFiltering( const Imc2dsl &ims, Imc2dsl &imd, float alpha ) {
   int w=3;
   int halfsize=w/2;
   int k=w*w-1;
   int k1=w*w-1+1,i,j,c,l,c1,l1;
   float *distance=new float [k1];
   int *voisinsx=new int[k1];
   int *voisinsy=new int[k1];
   float *J=new float [k1];
   float *f=new float [k];
   int *AConsiderer= new int[k1];
   float a1,a2,s1,s2;
   float temp;
   int X,Y,Z,max;
   int Gauss[3][3]={{1,2,1},{2,4,2},{1,2,1}};
   int somme;
    
   for (l=halfsize;l<ims.Height()-halfsize;l++) {
      for (c=halfsize;c<ims.Width()-halfsize;c++) {
	 for (i=0;i<k1;i++) {
	    distance[i]=0.F;
	    AConsiderer[i]=1;
	    voisinsy[i]=voisinsx[i]=0;
	    J[i]=f[i]=0;
	 }
	 i=0;
	 for (l1=-halfsize;l1<=halfsize;l1++) {
	    for (c1=-halfsize;c1<=halfsize;c1++) {
	       distance[i]=Distance(ims,l,c,l+l1,c+c1);
	       voisinsx[i]=c1;
	       voisinsy[i]=l1;
	       i++;
	    }
	 }
	 Ordonne(distance,voisinsy,voisinsx,k);
						
	 for (i=0;i<k;i++)
	    f[i]=distance[i+1]-distance[i];
		
	 for (i=0;i<=halfsize;i++) {
	    if (f[i]<=alpha) {
	       for (j=i;j>=0;j--) {
		  AConsiderer[j]=0;
	       }
	    }
	 }
	 for (i=k1-1;i>=k-1-halfsize;i--) {
	    if (f[i]<=alpha)
	       for (j=i;j<=k;j++)
		  AConsiderer[j]=0;
	 }

	 for (i=1;i<=k;i++) {
	    J[i]=0.F;
	    if (!AConsiderer[i]) continue;
	    a1=a2=0.F;
	    for (j=0;j<=i-1;j++)
	       a1+=distance[j];
	    for (j=i;j<=k;j++)
	       a2+=distance[j];
	    if (a1!=0.F) a1/=float(i);
	    if (a2!=0.F) a2/=float(k+1-i);
		   
	    s1=s2=0.F;
	    for (j=0;j<=i-1;j++)
	       s1+=SQR(distance[j]-a1);
	    for (j=i;j<=k;j++)
	       s2+=SQR(distance[j]-a2);
		    		    
	    if ((s1+s2)!=0.F) 
	       J[i]=SQR(a1-a2)/(s1+s2);
	 }
		
	 //trouver le max des J
	 i=1;
	 temp=J[i];
	 max=1;
	 for (i=1;i<=k;i++) 
	    if (temp<J[i]) {
	       temp=J[i];
	       max=i;
	    }
	 X=Y=Z=0;
	 i=0;
	 somme=Gauss[1][1];
	 X+=(int)ims.X[l][c]*somme;
	 Y+=(int)ims.Y[l][c]*somme;
	 Z+=(int)ims.Z[l][c]*somme;
		
		
	 for (i = 1; i < max; i++) {
	    int coeff=Gauss[voisinsy[i]+1][voisinsx[i]+1];
	    somme+=coeff;
	    X+=coeff*(int)ims.X[l+voisinsy[i]][c+voisinsx[i]];
	    Y+=coeff*(int)ims.Y[l+voisinsy[i]][c+voisinsx[i]];
	    Z+=coeff*(int)ims.Z[l+voisinsy[i]][c+voisinsx[i]];
	 }
		
	 imd.X[l][c] = X / somme;
	 imd.Y[l][c] = Y / somme;
	 imd.Z[l][c] = Z / somme;		
      }
   }
   imd.Frame(ims,1);

   return SUCCESS;
}

Errc PPeerGroupFiltering( const Imc2dsf &ims, Imc2dsf &imd, float alpha ) {
   int w=3;
   int halfsize=w/2;
   int k=w*w-1;
   int k1=w*w-1+1,i,j,c,l,c1,l1;
   float *distance=new float [k1];
   int *voisinsx=new int[k1];
   int *voisinsy=new int[k1];
   float *J=new float [k1];
   float *f=new float [k];
   int *AConsiderer= new int[k1];
   float a1,a2,s1,s2;
   float temp;
   int X,Y,Z,max;
   int Gauss[3][3]={{1,2,1},{2,4,2},{1,2,1}};
   int somme;
    
   for (l=halfsize;l<ims.Height()-halfsize;l++) {
      for (c=halfsize;c<ims.Width()-halfsize;c++) {
	 for (i=0;i<k1;i++) {
	    distance[i]=0.F;
	    AConsiderer[i]=1;
	    voisinsy[i]=voisinsx[i]=0;
	    J[i]=f[i]=0;
	 }
	 i=0;
	 for (l1=-halfsize;l1<=halfsize;l1++) {
	    for (c1=-halfsize;c1<=halfsize;c1++) {
	       distance[i]=Distance(ims,l,c,l+l1,c+c1);
	       voisinsx[i]=c1;
	       voisinsy[i]=l1;
	       i++;
	    }
	 }
	 Ordonne(distance,voisinsy,voisinsx,k);
						
	 for (i=0;i<k;i++)
	    f[i]=distance[i+1]-distance[i];
		
	 for (i=0;i<=halfsize;i++) {
	    if (f[i]<=alpha)
	       for (j=i;j>=0;j--)
		  AConsiderer[j]=0;
	 }
	 for (i=k1-1;i>=k-1-halfsize;i--) {
	    if (f[i]<=alpha)
	       for (j=i;j<=k;j++)
		  AConsiderer[j]=0;
	 }
				
	 for (i=1;i<=k;i++) {
	    J[i]=0.F;
	    if (!AConsiderer[i]) continue;
	    a1=a2=0.F;
	    for (j=0;j<=i-1;j++)
	       a1+=distance[j];
	    for (j=i;j<=k;j++)
	       a2+=distance[j];
	    if (a1!=0.F) a1/=float(i);
	    if (a2!=0.F) a2/=float(k+1-i);
		   
	    s1=s2=0.F;
	    for (j=0;j<=i-1;j++)
	       s1+=SQR(distance[j]-a1);
	    for (j=i;j<=k;j++)
	       s2+=SQR(distance[j]-a2);
		    		    
	    if ((s1+s2)!=0.F) 
	       J[i]=SQR(a1-a2)/(s1+s2);
	 }
		
	 //trouver le max des J
	 i=1;
	 temp=J[i];
	 max=1;
	 for (i=1;i<=k;i++) 
	    if (temp<J[i]) {
	       temp=J[i];
	       max=i;
	    }
	 X=Y=Z=0;
	 i=0;
	 somme=Gauss[1][1];
	 X+=(int)ims.X[l][c]*somme;
	 Y+=(int)ims.Y[l][c]*somme;
	 Z+=(int)ims.Z[l][c]*somme;
		
		
	 for (i = 1; i < max; i++) {
	    int coeff = Gauss[voisinsy[i]+1][voisinsx[i]+1];
	    somme += coeff;
	    X += coeff*(int)ims.X[l+voisinsy[i]][c+voisinsx[i]];
	    Y += coeff*(int)ims.Y[l+voisinsy[i]][c+voisinsx[i]];
	    Z += coeff*(int)ims.Z[l+voisinsy[i]][c+voisinsx[i]];
	 }
		
	 imd.X[l][c] = (float)X / somme;
	 imd.Y[l][c] = (float)Y / somme;
	 imd.Z[l][c] = (float)Z / somme;		
      }
   }

   imd.Frame(ims,1);
   return SUCCESS;
}

#ifdef MAIN

/*
 * Modify only the following constants, and the operator switches.
 */
#define	USAGE	"usage: %s alpha [-m mask] [im_in|-] [im_out|-]"
#define	PARC	1
#define	FINC	1
#define	FOUTC	1
#define	MASK	3

int main( int argc, char *argv[] ) {
   Errc result;              // The result code of the execution.
   Pobject* mask;            // The region mask.
   Pobject* objin[FINC+1];   // The input objects.
   Pobject* objs[FINC+1];    // The source objects masked by the mask.
   Pobject* objout[FOUTC+1]; // The output objects.
   Pobject* objd[FOUTC+1];   // The result object of the execution.
   char* parv[PARC+1];       // The input parameters.
  
   ReadArgs(argc,argv,PARC,FINC,FOUTC,&mask,objin,objs,objout,objd,parv,USAGE,MASK);

   switch(objs[0]->Type()) {
   case Po_Imc2duc : {
      Imc2duc* const ims=(Imc2duc*)objs[0];
      objd[0]=new Imc2duc(ims->Props());
      Imc2duc* const imd=(Imc2duc*)objd[0];

      result=PPeerGroupFiltering(*ims,*imd,(float)atof(parv[0]));
   }break;
   case Po_Imc2dsl : {
      Imc2dsl* const ims=(Imc2dsl*)objs[0];
      objd[0]=new Imc2dsl(ims->Props());
      Imc2dsl* const imd=(Imc2dsl*)objd[0];

      result=PPeerGroupFiltering(*ims,*imd,(float)atof(parv[0]));
   }break;
   case Po_Imc2dsf : {
      Imc2dsf* const ims=(Imc2dsf*)objs[0];
      objd[0]=new Imc2dsf(ims->Props());
      Imc2dsf* const imd=(Imc2dsf*)objd[0];

      result=PPeerGroupFiltering(*ims,*imd,(float)atof(parv[0]));
   }break;
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
