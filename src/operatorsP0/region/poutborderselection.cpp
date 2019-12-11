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
 * @author Régis Clouard - 1997-12-23
 */

#include <math.h>
#include <pandore.h>
using namespace pandore;

/**
 * @file poutborderselection.cpp
 * Selectionne les regions qui ne touchent pas le bord (cadre) de l'image.
 * Une region du bord a au moins 1 pixel dans le cadre (epaisseur p, h ou l).
 */
Errc POutBorderSelection( const Reg2d &rgs, Reg2d &rgd, int w, int h) {
   Ulong r;
   int y,x;
   Long nreg;
   bool *eliminer;

   eliminer = new bool[rgs.Labels()+1];
   for (r=0; r<=rgs.Labels(); eliminer[r++]=false) ;
   
   // Recherche des regions appartenant au cadre.
   for (y=0; y<rgs.Height(); y++) {
      for (x=0; x<w; x++) {
	 eliminer[rgs[y][x]] = true;
	 eliminer[rgs[y][rgs.Width()-1-x]] = true;
      }
   }
   for (x=0; x<rgs.Width(); x++) {
      for (y=0; y<h;y++) {
	 eliminer[rgs[y][x]] = true;
	 eliminer[rgs[rgs.Height()-1-y][x]] = true;
      }
   }
   nreg=0;
   for (r=1; r<=rgs.Labels(); r++) {
      if (eliminer[r]) {
	 nreg++;
      }
   }

   // Relabelisation.
   for (y=0; y<rgs.Height(); y++) {
      for (x=0; x<rgs.Width(); x++) {
	 if (eliminer[rgs[y][x]]) {
	    rgd[y][x] = 0;
	 } else {
	    rgd[y][x] = rgs[y][x];
	 }
      }
   }
   delete[] eliminer;
   return rgd.Labels(rgs.Labels()) - nreg;
}

Errc POutBorderSelection( const Reg3d &rgs, Reg3d &rgd, int w, int h, int d ) {
   Ulong r;
   int z,y,x;
   Long nreg;
   bool *eliminer;
   
   eliminer = new bool[rgs.Labels()+1];
   for (r = 0; r <= rgs.Labels(); eliminer[r++] = false) ;

   for (z=0; z<d; z++) {
      for (y=0; y<rgs.Height(); y++) {
	 for (x=0; x<rgs.Width(); x++) {
	    eliminer[rgs[z][y][x]] = true;
	    eliminer[rgs[rgs.Depth()-z-1][y][x]] = true;
	 }
      }
   }
   for (z=0; z<rgs.Depth(); z++) {
      for (y=0; y<h; y++) {
	 for (x=0; x<rgs.Width(); x++) {
	    eliminer[rgs[z][y][x]] = true;
	    eliminer[rgs[z][rgs.Height()-1-y][x]] = true;
	 }
      }
   }
   for (z=0; z<rgs.Depth(); z++) {
      for (y=0; y<rgs.Height(); y++) {
	 for (x=0; x<w; x++) {
	    eliminer[rgs[z][y][x]] = true;
	    eliminer[rgs[z][y][rgs.Width()-1-x]] = true;
	 }
      }
   }
  
   nreg=0;
   for (r=1; r<=rgs.Labels(); r++) {
      if (eliminer[r]) {
	 nreg++;
      }
   }
   // Relabelisation.
   for (z=0; z<rgs.Depth(); z++) {
      for (y=0; y<rgs.Height(); y++) {
	 for (x=0; x<rgs.Width(); x++) {
	    if (eliminer[rgs[z][y][x]]) {
	       rgd[z][y][x] = 0;
	    } else {
	       rgd[z][y][x] = rgs[z][y][x];
	    }
	 }
      }
   }
   delete[] eliminer;   
   return rgd.Labels(rgs.Labels()) - nreg;
}

#ifdef MAIN

/*
 * Modify only the following constants, and the function call.
 */
#define USAGE	"usage: %s width height depth [-m mask] [rg_in|-] [rg_out|-]"
#define PARC	3
#define FINC	1
#define FOUTC	1
#define MASK	1

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

   case Po_Reg2d :{
      Reg2d* const rgs=(Reg2d*)objs[0];
      objd[0]=new Reg2d(rgs->Size());
      Reg2d* const rgd=(Reg2d*)objd[0];

      result=POutBorderSelection(*rgs,*rgd,atoi(parv[0]),atoi(parv[1]));
   }break;

   case Po_Reg3d :{
      Reg3d* const rgs=(Reg3d*)objs[0];
      objd[0]=new Reg3d(rgs->Size());
      Reg3d* const rgd=(Reg3d*)objd[0];

      result=POutBorderSelection(*rgs,*rgd,atoi(parv[0]),atoi(parv[1]),atoi(parv[2]));
   }break;

   default:
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
