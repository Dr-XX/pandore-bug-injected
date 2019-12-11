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
 * @author Régis Clouard - 1997-07-23
 */

#include <math.h>
#include <pandore.h>
using namespace pandore;

/**
 * @file pareadisorderfactor.cpp
 * Calculates the Area Disorder degree.
 */

double PAreaDisorderFactor( const Reg2d &rgs ) {
   Long *tregion;
   Long nb = 0;
   Reg2d::ValueType t;
   Ulong i;
   double tmp, moy = 0, mom2 = 0, var = 0;
   Reg2d::ValueType *ps = rgs.Vector();
   const Reg2d::ValueType *pf = rgs.Vector() + rgs.VectorSize();
   
   tregion = new Long[rgs.Labels() + 1];
   for (i = 0; i < rgs.Labels() + 1; tregion[i++] = 0) ;
   
   // Calculate area.
   for ( ; ps < pf ; ps++ ) {
      if ((t = *ps)) {// On ne compte pas la region 0.
	 tregion[t]++;
      }
   }
   
   for (i = 1; i < rgs.Labels() + 1; i++) {
      if ((tregion[i])) {
	 moy += (double)tregion[i];
	 mom2 += (double)tregion[i] * tregion[i];
	 nb++;
      }
   }
   delete [] tregion;
   
   moy /= nb;
   mom2 /= nb;
   var = mom2 - (moy * moy);
   tmp=1 - 1 / (1 + (sqrt(var) / moy));
   
   return tmp;
}

double PAreaDisorderFactor( const Reg3d &rgs ) {
   Long *tregion;
   Ulong i;
   Long nb = 0;
   Reg3d::ValueType t;
   double tmp, moy = 0, mom2 = 0, var = 0;
   Reg2d::ValueType *ps=rgs.Vector();
   const Reg2d::ValueType *pf=rgs.Vector()+rgs.VectorSize();
   
   tregion = new Long[rgs.Labels()+1];
   for (i = 1; i < rgs.Labels() + 1; tregion[i++] = 0) ;
   
   // Calculate area.
   for (; ps < pf; ps++) {
      if ( (t = *ps) ) { // On ne compte pas la region 0.
	 tregion[t]++;
      }
   }
   for (i=1;i<rgs.Labels()+1;i++) {
      if (tregion[i]) {
	 moy += (double)tregion[i];
	 mom2 +=( double)tregion[i]*tregion[i];
	 nb++;
      }
   }
   delete [] tregion;
   
   moy /= nb;
   mom2 /= nb;
   var = mom2 - (moy * moy);
   
   tmp=1 - 1 / (1 + (sqrt(var) / moy));
   
  return tmp;
}

/*
 * Modify only the following constants, and the function call.
 */
#ifdef MAIN
#define USAGE	"usage: %s [-m mask] [rg_in|-]"
#define PARC	0
#define FINC	1
#define FOUTC	0
#define MASK	1

int main( int argc, char *argv[] ) {
  Errc	 result;	      // The result code of the execution.
  Pobject* mask;	      // The region stencil.
  Pobject* objin[FINC + 1];   // The input objects.
  Pobject* objs[FINC + 1];    // The source objects masked by the stencil.
  Pobject* objout[FOUTC + 1]; // The output object.
  Pobject* objd[FOUTC + 1];   // The result object of the execution.
  char* parv[PARC + 1];	      // The input parameters.

  ReadArgs(argc, argv, PARC, FINC, FOUTC, &mask, objin, objs, objout, objd, parv, USAGE, 0);

  switch(objs[0]->Type()) {
  case Po_Reg2d : {
    Reg2d* const rgs = (Reg2d*)objs[0];

    result=PAreaDisorderFactor(*rgs);
  }break;
  case Po_Reg3d :{
    Reg3d* const rgs = (Reg3d*)objs[0];

    result = PAreaDisorderFactor(*rgs);
  }break;
  default :
    PrintErrorFormat(objin, FINC);
    result = FAILURE;
  }

  if (result) {
      WriteArgs(argc, argv, PARC, FINC, FOUTC, &mask, objin, objs, objout, objd, MASK);
  }

  Exit(result);
  return(0);
}

#endif
