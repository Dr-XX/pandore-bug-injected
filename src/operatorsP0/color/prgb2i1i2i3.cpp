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

#include <pandore.h>
using namespace pandore;

/**
 * @file prgb2i1i2i3.cpp
 *
 */

template <typename T>
Errc PRGB2I1I2I3( const Imc2d<T> &ims, Imc2dsf &imd ) {
   Point2d p;

   imd.ColorSpace(I1I2I3);
   
   for (p.y=0;p.y<ims.Height();p.y++) {
      for (p.x=0;p.x<ims.Width();p.x++) {
	 imd.X[p]= (float)((ims.Z[p] + ims.Y[p] + ims.X[p])/3.F);
	 imd.Y[p]= (float)((ims.Z[p]- ims.X[p])/2.F);
	 imd.Z[p]= (float)((2*ims.Y[p] - ims.Z[p] - ims.X[p])/4.F);
      }
   }
   return SUCCESS;
}

template <typename T>
Errc PRGB2I1I2I3( const Imc3d<T> &ims, Imc3dsf &imd ) {
   Point3d p;
   
   imd.ColorSpace(I1I2I3);
   
   for (p.z=0;p.z<ims.Depth();p.z++) {
      for (p.y=0;p.y<ims.Height();p.y++) {
	 for (p.x=0;p.x<ims.Width();p.x++) {
	    imd.X[p]= (float)((ims.Z[p] + ims.Y[p] + ims.X[p])/3.F);
	    imd.Y[p]= (float)((ims.Z[p]- ims.X[p])/2.F);
	    imd.Z[p]= (float)((2*ims.Y[p] - ims.Z[p] - ims.X[p])/4.F);
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
	      
      result=PRGB2I1I2I3(*ims,*imd);
   }break;
   case Po_Imc2dsl :{
      Imc2dsl* const ims=(Imc2dsl*)objs[0];
      objd[0]=new Imc2dsf(ims->Size());
      Imc2dsf* const imd=(Imc2dsf*)objd[0];
	      
      result=PRGB2I1I2I3(*ims,*imd);
   }break;
   case Po_Imc2duc :{
      Imc2duc* const ims=(Imc2duc*)objs[0];
      objd[0]=new Imc2dsf(ims->Size());
      Imc2dsf* const imd=(Imc2dsf*)objd[0];
		
      result=PRGB2I1I2I3(*ims,*imd);
   }break;
   case Po_Imc3dsf :{
      Imc3dsf* const ims=(Imc3dsf*)objs[0];
      objd[0]=new Imc3dsf(ims->Size());
      Imc3dsf* const imd=(Imc3dsf*)objd[0];
	      
      result=PRGB2I1I2I3(*ims,*imd);
   }break;
   case Po_Imc3dsl :{
      Imc3dsl* const ims=(Imc3dsl*)objs[0];
      objd[0]=new Imc3dsf(ims->Size());
      Imc3dsf* const imd=(Imc3dsf*)objd[0];
	      
      result=PRGB2I1I2I3(*ims,*imd);
   }break;
   case Po_Imc3duc :{
      Imc3duc* const ims=(Imc3duc*)objs[0];
      objd[0]=new Imc3dsf(ims->Size());
      Imc3dsf* const imd=(Imc3dsf*)objd[0];
		
      result=PRGB2I1I2I3(*ims,*imd);
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

