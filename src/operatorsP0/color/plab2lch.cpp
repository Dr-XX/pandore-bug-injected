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
 * @author Olivier Lezoray - 2001-04-13
 * @author Olivier Lezoray - 2003-03-11
 */

#define _USE_MATH_DEFINES
#include <cmath>
#include <pandore.h>
using namespace pandore;

/**
 * @file plab2lch.cpp
 * Passe du LAB au LCH
 */

template <typename T>
Errc PLAB2LCH( const T &ims, T &imd ) {
   int k;
   Float a,b;
   
   imd.ColorSpace(LCH);

   Float *psx=ims.Vector(0);
   Float *psy=ims.Vector(1);
   Float *psz=ims.Vector(2);

   Float *pdx=imd.Vector(0);
   Float *pdy=imd.Vector(1);
   Float *pdz=imd.Vector(2);
   
   for (; psx < ims.Vector(0) + ims.VectorSize(); psx++, psy++, psz++, pdx++, pdy++, pdz++) {
      a = *psy;
      b = *psz;
      *pdx = *psx;
      *pdy = (float)sqrt(a * a + b * b);
      if (a == 0) {
	 *pdz = 0.F;
      } else {
	 if (a >= 0 && b >= 0) {
	    k = 0;
	 } else if (a > 0 && b < 0) {
	    k = 1;
	 } else if (a < 0 && b < 0) {
	    k = 2;
	 } else {
	    k = 3;
	 }
	 *pdz = (float)((atan(b / a) + k * M_PI / 2) / (2 * M_PI));
	 if (*pdz < 0) {
	    *pdz += (float)(2 * M_PI);
	 }
	 if (*pdz > 2 * M_PI) {
	    *pdz -= (float)(2 * M_PI);
	 }
      }
   }
   
   return SUCCESS;
}

#ifdef MAIN

/*
 * Modify only the following constants, and the operatOr switches.
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
   case Po_Imc2dsf :{
      Imc2dsf* const ims=(Imc2dsf*)objs[0];
      objd[0]=new Imc2dsf(ims->Size());
      Imc2dsf* const imd=(Imc2dsf*)objd[0];

      result=PLAB2LCH(*ims,*imd);
   }break;
   case Po_Imc3dsf :{
      Imc3dsf* const ims=(Imc3dsf*)objs[0];
      objd[0]=new Imc3dsf(ims->Props());
      Imc3dsf* const imd=(Imc3dsf*)objd[0];

      result=PLAB2LCH(*ims,*imd);
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
