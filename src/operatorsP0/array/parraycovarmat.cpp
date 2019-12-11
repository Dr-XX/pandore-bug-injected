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
 * @author Alexandre Duret-Lutz - 1999-11-02
 */

#include <math.h>
#include <pandore.h>
using namespace pandore;

/**
 * @file parraycovarmat.cpp
 * Calcul de - moyenne des catacteristiques 
 *           - matrice de covariance A
 *           - determinant de A
 *           - inverse de A
 */

#define TINY 1.0e-20F

/* (C) Copr. 1986-92 Numerical Recipes Software 'j9#. */
void ludcmp( Float **a, int n, int *indx, Float *d ) {
   int i,imax=0,j,k;
   Float big,dum,sum,temp;
   Float *vv = new Float[n];

   *d=1.0;
   for (i=0;i<n;i++) {
      big=0.0;
      for (j=0;j<n;j++)
	 if ((temp=fabs(a[i][j])) > big) big=temp;
      if (big == 0.0) {
	 fprintf(stderr,"Error parraycovarmat: Singular matrix in routine ludcmp");
	 Exit(FAILURE);
      }
      vv[i]=1.0F/big;
   }
   for (j=0;j<n;j++) {
      for (i=0;i<j;i++) {
	 sum=a[i][j];
	 for (k=0;k<i;k++) sum -= a[i][k]*a[k][j];
	 a[i][j]=sum;
      }
      big=0.0;
      for (i=j;i<n;i++) {
	 sum=a[i][j];
	 for (k=0;k<j;k++)
	    sum -= a[i][k]*a[k][j];
	 a[i][j]=sum;
	 if ( (dum=vv[i]*fabs(sum)) >= big) {
	    big=dum;
	    imax=i;

	 }
      }
      if (j != imax) {
	 for (k=0;k<n;k++) {
	    dum=a[imax][k];
	    a[imax][k]=a[j][k];
	    a[j][k]=dum;
	 }
	 *d = -(*d);
	 vv[imax]=vv[j];
      }
      indx[j]=imax;
      if (a[j][j] == 0.0F) a[j][j]=TINY;
      if (j != n-1) {
	 dum=1.0F/(a[j][j]);
	 for (i=j+1;i<n;i++) a[i][j] *= dum;
      }
   }
   delete[] vv;
}
#undef TINY

/* (C) Copr. 1986-92 Numerical Recipes Software 'j9#. */
void lubksb( Float **a, int n, int *indx, Float b[] ) {
   int i,ii=-1,ip,j;
   Float sum;

   for (i=0;i<n;i++) {
      ip=indx[i];
      sum=b[ip];
      b[ip]=b[i];
      if (ii>=0)
	 for (j=ii;j<=i-1;j++) 
	    sum -= a[i][j]*b[j];
      else if (sum) ii=i;
      b[i]=sum;
   }
   for (i=n-1;i>=0;i--) {
      sum=b[i];
      for (j=i+1;j<n;j++) sum -= a[i][j]*b[j];
      b[i]=sum/a[i][i];
   }
}

Float** covarmatcalc( int caracN, int elemN, Float **x, Float *m ) {
   int i;
   Float** A = new Float*[caracN];
   Float* tmp = new Float[caracN*caracN];
   for (i = 0; i < caracN; ++i, tmp+=caracN)
      A[i] = tmp;

   // calcul de la matrice de covariance

   for (i = 0; i < caracN; ++i) {
      for (int j = 0; j < caracN; ++j) {
	 Float somme = 0;
	 for (int l = 0; l < elemN; ++l)
	    somme += (x[i][l] - m[i])*(x[j][l] - m[j]);
	 somme /= elemN - 1;
	 A[i][j] = somme;
      }
   }

   return A;
}

#if DEBUG
Float** debugmat( int n, Float **A, char *msg ) {
   fprintf(stderr,msg);
   for (int i = 0 ; i < n; ++i) {
      for (int j = 0 ; j < n; ++j)
	 fprintf(stderr,"%12g ",A[i][j]);
      fprintf(stderr,"\n");		
   }
}
#endif

Float** dupmat( int n, Float **A ) {
   Float** t;
   t = new Float*[n];
   Float* tmp = new Float[n*n];
   for (int i = 0; i < n; ++i, tmp+=n)
      t[i] = tmp;
   memcpy(t[0],A[0],n*n*sizeof(Float));
   return t;
}

Errc PArrayCovarMat( const std::string& a_in,    const Collection &c_in,
	       const std::string& a_out,         Collection &c_out ) {
   Long caracN;
   Long elemN;
   std::string type;
   int i,j;
   if (!c_in.NbOf(a_in,type,caracN,elemN))
      return FAILURE;
	
   Float** in = c_in.GETNARRAYS(a_in,Float,caracN,elemN);

   // calcul de la moyenne
   Float* m = new Float[caracN];
   for (i = 0; i < caracN; ++i) {
      Float tmp = 0;
      for (int j = 0; j < elemN; ++j) 
	 tmp += in[i][j];
      m[i] = tmp/elemN;
   }
	
   Float** mat = covarmatcalc(caracN,elemN,in,m);

   Float** lu = dupmat(caracN,mat);

   int* indx = new int[caracN];
   Float det;
   ludcmp(lu,caracN,indx,&det);
   for ( j=0;j<caracN;++j) det *= lu[j][j];
		
   Float* col= new Float[caracN];
   Float** inv = new Float*[caracN];
   Float* tmp = new Float[caracN*caracN];
   for (i = 0; i < caracN; ++i, tmp+=caracN)
      inv[i] = tmp;
   for (j = 0; j < caracN; ++j) {
      for (i = 0; i< caracN; ++i) {
	 col[i] = 0.0;
      }
      col[j] = 1.0;
      lubksb(lu,caracN,indx,col);
      for (i = 0; i < caracN; ++i) {
	 inv[i][j] = col[i];
      }
   }

#if DEBUG
   debugmat(caracN,mat,"matrice de covariance\n");
   debugmat(caracN,lu,"decomposition LU\n");
   fprintf(stderr,"det = %g\n",det);
   debugmat(caracN,inv,"inverse\n");
#endif
   c_out.SETARRAY(a_out+".mat",Float,mat[0],caracN*caracN);
   c_out.SETARRAY(a_out+".inv",Float,inv[0],caracN*caracN);
   c_out.SETVALUE(a_out+".det",Float,det);
   c_out.SETARRAY(a_out+".mean",Float,m,caracN);
   return SUCCESS;
}


#ifdef MAIN

/*
 * Modify only the following constants, and the function call.
 */
#define	USAGE	"usage: %s name_in name_out [col_in|-] [col_out|-]"
#define	PARC	2
#define	FINC	1
#define	FOUTC	1
#define	MASK	0

int main( int argc, char *argv[] ) {
   Errc  result;             // The result code of the execution.
   Pobject* mask;            // The region mask.
   Pobject* objin[FINC+1];   // The input objects.
   Pobject* objs[FINC+1];    // The source objects masked by the mask.
   Pobject* objout[FOUTC+1]; // The output object.
   Pobject* objd[FOUTC+1];   // The result object of the execution.
   char* parv[PARC+1];       // The input parameters.

   ReadArgs(argc,argv,PARC,FINC,FOUTC,&mask,objin,objs,objout,objd,parv,USAGE,MASK);

   if (objs[0]->Type() == Po_Collection) {
      Collection* ims1=(Collection*)objs[0];
      Collection* imd1=new Collection;
      objd[0] = imd1;
      result = PArrayCovarMat(argv[1],*ims1,argv[2],*imd1);
   } else {
      PrintErrorFormat(objin,FINC);
      result = FAILURE;
   }

   if (result)
      WriteArgs(argc,argv,PARC,FINC,FOUTC,&mask,objin,objs,objout,objd,MASK);
   Exit(result);
   return 0;
}

#endif
