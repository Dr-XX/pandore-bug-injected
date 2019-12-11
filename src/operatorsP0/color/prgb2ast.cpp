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
3 *
 * 
 * For more information, refer to:
 * https://clouard.users.greyc.fr/Pandore
 */

/**
 * @author Olivier Lezoray - 1997-12-12
 */

#define _USE_MATH_DEFINES
#include <cmath>
#include <pandore.h>
using namespace pandore;

/**
 * @file prgb2ast.cpp
 *
 * Convert color space R, G, B to A,S,T
 */
Errc PRGB2AST( const Imc2duc &ims, Imc2duc &imd ) {
   Point2d p;
   Float A, C1, C2, s, t;
   double lx, ly, lz, cst;

   imd.ColorSpace(AST);
   double log255 = log(255.0);
   
   cst = sqrt(3.0) / 2.0;
   for (p.y = 0; p.y < ims.Height(); p.y++) {
      for (p.x = 0; p.x < ims.Width(); p.x++) {
	 if (ims.X[p] == 0) {
	    lx = 0;
	 } else {
	    lx = log((double)ims.X[p]);
	 }
	 if (ims.Y[p] == 0) {
	    ly = 0;
	 } else {
	    ly = log((double)ims.Y[p]);
	 }
	 if (ims.Z[p] == 0) {
	    lz = 0;
	 } else {
	    lz = log((double)ims.Z[p]);
	 }
	 
	 A  = (Float)( (lx + ly + lz) / 3.0);
	 C1 = (Float)(cst * (lx - ly));
	 C2 = (Float)(lz - 0.5*(lx + ly));
	 
	 s = (Float)sqrt((double)C1 * C1 + C2 * C2);
	 if (s == 0.0) {
	    t = (Float)acos(0.);
	 } else {
	    t = (Float)acos(C1 / s);
	 }
	 if (t < 0) {
	    t = (Float)(2 * M_PI - t);
	 }
	 
	 imd.X[p] = (Uchar)((A * 255.0) / log255);
	 imd.Y[p] = (Uchar)((s * 255.0) / (cst * log255));
	 imd.Z[p] = (Uchar)((t * 255.0) / (M_PI));
      }
   }
   return SUCCESS;
}

Errc PRGB2AST( const Imc3duc &ims, Imc3duc &imd ) {
   Point3d p;
   Float A, C1, C2, s, t;
   Double lx, ly, lz, cst;
   double log255 = log(255.0);

   imd.ColorSpace(AST);
   cst = sqrt(3.0) / 2.0;
   for (p.z = 0; p.z < ims.Depth(); p.z++) {
      for (p.y = 0; p.y < ims.Height(); p.y++) {
	 for (p.x = 0; p.x < ims.Width(); p.x++) {
	    if (ims.X[p] == 0) {
	       lx = 0;
	    } else {
	       lx = log((double)ims.X[p]);
	    }
	    if (ims.Y[p] == 0) {
	       ly = 0;
	    } else {
	       ly = log((double)ims.Y[p]);
	    }
	    if (ims.Z[p] == 0) {
	       lz = 0;
	    } else {
	       lz = log((double)ims.Z[p]);
	    }
	    
	    A = (Float)( (lx + ly + lz) / 3.0);
	    C1= (Float)(cst * (lx - ly));
	    C2= (Float)(lz - 0.5*(lx + ly));
	    
	    s = (Float)sqrt((double)C1 * C1 + C2 * C2);
	    if (s == 0.0) {
	       t = (Float)acos(0.);
	    } else {
	       t = (Float)acos(C1 / s);
	    }
	    if (t < 0) {
	       t = (Float)(2 * M_PI - t);
	    }
	    
	    imd.X[p] = (Uchar)((A * 255.0) / log255);
	    imd.Y[p] = (Uchar)((s * 255.0) / (cst * log255));
	    imd.Z[p] = (Uchar)((t * 255.0) / (M_PI));
	 }
      }
   }
   return SUCCESS;
}

#ifdef MAIN

/*
 * Modify only the following constants, and the operator switches.
 */
#define	USAGE	"usage: %s [-m mask] [im_in|-] [im_out|-]"
#define	PARC	0
#define	FINC	1
#define	FOUTC	1
#define	MASK	0

int main( int argc, char *argv[] ) {
   Errc result;                // The result code of the execution.
   Pobject* mask;              // The region mask.
   Pobject* objin[FINC + 1];   // The input objects.
   Pobject* objs[FINC + 1];    // The source objects masked by the mask.
   Pobject* objout[FOUTC + 1]; // The ouput object.
   Pobject* objd[FOUTC + 1];   // The result object of the execution.
   char* parv[PARC + 1];       // The input parameters.

   ReadArgs(argc, argv, PARC, FINC, FOUTC, &mask, objin, objs, objout, objd, parv, USAGE, MASK);

   switch(objs[0]->Type()) {
   case Po_Imc2duc :{
      Imc2duc* const ims = (Imc2duc*)objs[0];
      objd[0] = new Imc2duc(ims->Size());
      Imc2duc* const imd = (Imc2duc*)objd[0];

      result = PRGB2AST(*ims, *imd);
   }break;
   case Po_Imc3duc :{
      Imc3duc* const ims = (Imc3duc*)objs[0];
      objd[0] = new Imc3duc(ims->Size());
      Imc3duc* const imd = (Imc3duc*)objd[0];

      result = PRGB2AST(*ims, *imd);
   }break;
   default :
      PrintErrorFormat(objin, FINC);
      result = FAILURE;
   }

   if (result) {
      WriteArgs(argc, argv, PARC, FINC, FOUTC, &mask, objin, objs, objout, objd, MASK);
   }
   Exit(result);
   return 0;
}

#endif

