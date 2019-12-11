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
 * @author Alexandre Duret-Lutz - 1999-11-03
 * @author Régis Clouard - 2004-02-09 (fix bug on line 103)
 */

#include <math.h>
#include <pandore.h>
using namespace pandore;

/**
 * @file pgaussclassification.cpp
 * Classification en utilisant un modèle gaussienne
 * de la distribution des classes.
 */

#define EPS 2.2204e-16

void PGaussClassification_( int nClasses, int nCarac, const Float* logdet, 
			   const Float*const* m, Float const*const*const*inv, 
			   Float const* ap, int nin, const Float*const* input, 
			   Ushort *output ) {
   Float *xmm= new Float[nCarac]; // vecteur servant a stocker x-m
   // (x individu considere, m classe testee).
   
   // boucle sur les individus a classer
   for (int ac = 0; ac < nin; ++ac ) {
      Float mv = MAXFLOAT;	// valeur min
      Ushort mc = 0;		// classe correspondante
      
      // boucle sur les classes
      for (int c = 0; c < nClasses; ++c) {
	 // calcul de x-m
	 for (int j = 0; j < nCarac; ++j)
	    xmm[j] = input[j][ac] - m[c][j];
      
	 const Float*const* Ai = inv[c];
	 // calcul de  (x-m)' . Ai . (x-m)
	 Float p1 = 0;
	 for (int k = 0; k < nCarac; ++k) {
	    Float p2 = 0;
	    for (int l = 0; l < nCarac; ++l)
	       p2 += Ai[k][l] * xmm[l];
	    p1 += p2 * xmm[k];
	 }
      
	 // ajout des termes en ln puis comparaison
	 p1 += logdet[c] - ap[c];			
	 if (p1 < mv) {
	    mv = p1;
	    mc = c;
	 }
      }
      output[ac] = mc + 1;
   }
}

Float*** GetMatrices( const std::string& a,  const Collection& c, 
		      int nClasses, int nCarac ) {
   Float* matline = c.GETARRAY(a,Float);
   int matlinesize= c.GETARRAYSIZE(a,Float);

   if (matlinesize != nCarac*nCarac*nClasses) {
      fprintf(stderr,"Error pgaussclassification: Wrong size for %s : %d should be (%d^2)*%d\n",
	      a.c_str(), matlinesize, nCarac, nClasses);
      Exit(FAILURE);
   }

   Float*** out = new Float**[nClasses];
   Float** tmp  = new Float*[nCarac*nClasses];
   for (int i = 0; i< nClasses; ++i, tmp+=nCarac) {
      out[i] = tmp;
      for (int j = 0; j< nCarac; j++, matline += nCarac)
	 tmp[j] = matline;
   }
   return out;
}

Errc PGaussClassification( const std::string& a_base,  const Collection& c_base, 
			   const std::string& a_in,    const Collection& c_in,
			   const std::string& a_out,         Collection& c_out) {
   Float* moy = NULL;
   Float* det = NULL;
   int nClasses = 0;
   int nCarac = 0;
   Float** m;
   int i;

   moy=c_base.GETARRAY(a_base+".mean",Float);
   det = c_base.GETARRAY(a_base+".det",Float);
   nClasses = c_base.GETARRAYSIZE(a_base+".det",Float);
   nCarac = c_base.GETARRAYSIZE(a_base+".mean",Float) / nClasses;  

   if (moy == NULL) return FAILURE;
   m= new Float*[nClasses];
   for (i = 0; i < nClasses; ++i, moy+= nCarac) {
      m[i] = moy;
      det[i] = (float)log(det[i]+EPS);
   }
  
   Float*** inv = NULL;
   inv= GetMatrices(a_base+".inv",c_base,nClasses,nCarac);

   Long nin;
   Float** input = c_in.GETNARRAYS(a_in,Float,nCarac,nin);

   Ushort* output = new Ushort[nin];

   Float* ap;		// probas a priori pour chaque classe
   int apsize;
   if (c_base.Exists(a_base+".pap")) {
      ap = c_base.GETARRAY(a_base+".pap",Float);
      apsize = c_base.GETARRAYSIZE(a_base+".pap",Float);
   } else {
      ap = new Float[nClasses];
      apsize = 0;
   }
   for (;apsize<nClasses;++apsize) {
      ap[apsize] = 1.0;
   }
   for (i = 0;i<nClasses;++i) {
      ap[i] = (float)(2.0*log(ap[i]+EPS));
   }
   PGaussClassification_(nClasses, nCarac,
			det, m, inv, ap,
			nin, input,
			output);

   c_out.SETARRAY(a_out,Ushort,output,nin);
   return SUCCESS;
}

#ifdef MAIN

/*
 * Modify only the following constants, and the function call.
 */
#define	USAGE	"usage: %s base_attr in_attr out_attr [col_base|-] [col_in|-] [col_out|-]"
#define	PARC	3
#define	FINC	2
#define	FOUTC	1
#define	MASK	0

int main( int argc, char *argv[] ) {
   Errc result;              // The result code of the execution.
   Pobject* mask;            // The region map.
   Pobject* objin[FINC+1];   // The input objects.
   Pobject* objs[FINC+1];    // The source objects masked.
   Pobject* objout[FOUTC+1]; // The output object.
   Pobject* objd[FOUTC+1];   // The result object of the execution.
   char* parv[PARC+1];       // The input parameters.

   ReadArgs(argc,argv,PARC,FINC,FOUTC,&mask,objin,objs,objout,objd,parv,USAGE,MASK);

   if (objs[0]->Type() == Po_Collection && objs[1]->Type() == Po_Collection) {
      Collection* ims1=(Collection*)objs[0];
      Collection* ims2=(Collection*)objs[1];
      Collection* imd1=new Collection;
      objd[0] = imd1;
      result = PGaussClassification(argv[1],*ims1,argv[2],*ims2,argv[3],*imd1);
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
