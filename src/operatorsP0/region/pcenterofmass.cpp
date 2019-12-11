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
 * @author Régis Clouard - 1997-21-07
 * @author Marianne Bedin, Christophe Deymier - 1999-20-11
 * @author Régis Clouard - 2002-12-26 (change Long* gx,gy,gz ->float* gx,gy,gz)
 */

#include <pandore.h>
using namespace pandore;

/**
 * @file pcenterofmass.cpp	
 * Fonction de calcul du barycentre.
 */
Errc PCenterOfMass( const Reg2d &rgs, Reg2d &rgd ) {
   float *gy=(float*)calloc(rgs.Labels()+1,sizeof(float));
   float *gx=(float*)calloc(rgs.Labels()+1,sizeof(float));
   float *gn=(float*)calloc(rgs.Labels()+1,sizeof(float));
   
   if (!gx || ! gy || !gn) {
      std::cerr << "Error pcenterofmass: Not enough memory."<< std::endl;
      return FAILURE;
   }
   Reg2d::ValueType l;
   for (int y=0;y<rgs.Height();y++) {
      for (int x=0;x<rgs.Width();x++) {
	 if ((l=rgs[y][x])) {
	    gy[l]+=(float)y;
	    gx[l]+=(float)x;
	    gn[l]+=1.0F;
	 }
      }
   }
   rgd=0;
   float t;
   for (Ulong c=1;c<=rgs.Labels();c++) {
      if ((t=gn[c])) {
	 rgd[(int)(gy[c]/t)][(int)(gx[c]/t)]=c;
      }
   }
   free(gy);
   free(gx);
   free(gn);
   
   rgd.Labels(rgs.Labels());
   return SUCCESS;
}

Errc PCenterOfMass( const Reg3d &rgs, Reg3d &rgd ) {
   float *gz=(float*)calloc(rgs.Labels()+1,sizeof(float));
   float *gy=(float*)calloc(rgs.Labels()+1,sizeof(float));
   float *gx=(float*)calloc(rgs.Labels()+1,sizeof(float));
   float *gn=(float*)calloc(rgs.Labels()+1,sizeof(float));

   if (!gx || ! gy || !gz || !gn) {
      std::cerr << "Error pcenterofmass: Not enough memory."<< std::endl;
      return FAILURE;
   }   
   Reg3d::ValueType l;
   for (int z=0;z<rgs.Depth();z++) {
      for (int y=0;y<rgs.Height();y++) {
	 for (int x=0;x<rgs.Width();x++) {
	    if ((l=rgs[z][y][x])) {
	       gz[l]+=(float)z;
	       gy[l]+=(float)y;
	       gx[l]+=(float)x;
	       gn[l]+=1.0F;
	    }
	 }
      }
   }
   rgd=0;
   float t;
   for (Ulong c=1;c<=rgs.Labels();c++) {
      if ((t=gn[c])) {
	 rgd[(int)(gz[c]/t)][(int)(gy[c]/t)][(int)(gx[c]/t)]=c;
      }
   }
   free(gz);
   free(gy);
   free(gx);
   free(gn);
   
   rgd.Labels(rgs.Labels());   
   return SUCCESS;
}

#ifdef MAIN

/*
 * Modify only the following constants, and the function call.
 */
#define USAGE	"usage: %s [-m mask] [rg_in|-] [rg_out|-]"
#define PARC	0
#define FINC	1
#define FOUTC	1
#define MASK	2

int main( int argc, char *argv[] ) {
   Errc result;              // The result code of the execution.
   Pobject* mask;            // The region mask.
   Pobject* objin[FINC+1];   // The input objects.
   Pobject* objs[FINC+1];    // The source objects masked by the mask.
   Pobject* objout[FOUTC+1]; // The output object.
   Pobject* objd[FOUTC+1];   // The result object of the execution.
   char* parv[PARC+1];       // The input parameters.
   
   ReadArgs(argc,argv,PARC,FINC,FOUTC,&mask,objin,objs,objout,objd,parv,USAGE,MASK);
   
   switch(objs[0]->Type()) {
   case Po_Reg2d : {
      Reg2d* const rgs=(Reg2d*)objs[0];
      objd[0]=new Reg2d(rgs->Size());
      Reg2d* const rgd=(Reg2d*)objd[0];

      result=PCenterOfMass(*rgs,*rgd);
   } break;
   
   case Po_Reg3d : {
      Reg3d* const rgs=(Reg3d*)objs[0];
      objd[0]=new Reg3d(rgs->Size());
      Reg3d* const rgd=(Reg3d*)objd[0];
      result=PCenterOfMass(*rgs,*rgd);
   } break;
   
   default :
      PrintErrorFormat(objin,FINC);
      result = FAILURE;
   }  
   
   if (result)
      WriteArgs(argc,argv,PARC,FINC,FOUTC,&mask,objin,objs,objout,objd,MASK);
   Exit(result);
   return 0;
}

#endif
