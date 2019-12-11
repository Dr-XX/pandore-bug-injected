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
 * @author Regis Clouard - 2003-03-04
 */

#include <pandore.h>
using namespace pandore;

/** 
 * @file pxyz2rgb.cpp
 * Passe du XYZ au RGB
 */

const float XYZtoRGB[7][3][3]={
  //illuminant E
   { {4.1845e-01F,-1.5868e-01F,-8.2818e-02F},
     {-9.1177e-02F, 2.5247e-01F, 1.5705e-02F},
     {9.2085e-04F,-2.5498e-03F, 1.7859e-01F}},
   //illuminant primaires CIE-DIN
   { {3.059590F,-1.392746F,-0.474677F},
     {-0.967629F,1.874841F,0.041666F},
     {0.068797F,-0.229898F,1.069305F}},
   //illuminant A primaires macbeth colour chart
   { {0.921104F,0.026999F,-0.108753F},
     {-0.558788F,1.563411F,0.141470F},
     {-0.201479F,0.335562F,2.491507F}},
   //illuminant A primaires CIE
   { {1.299226F,-0.493508F,-0.258831F},
     {-0.484824F,1.343052F,0.085252F},
     {0.013039F,-0.036121F,2.442695F}},
   //illuminant C primaires NTSC
   { {1.910458F,-0.533940F,-0.287834F},
     {-0.984436F,1.998504F,-0.027726F},
     {0.058219F,-0.118191F,0.897697F}},
   //illuminant C primaires CIE
   { {1.6600e-01F,1.2500e-01F,9.3000e-02F},
     {6.0000e-02F,3.2700e-01F,5.0000e-03F},
     {1.0842e-19F,4.0000e-03F,4.6000e-01F}},
   //illuminant D65 primaires Visu Couleur
   { {2.717932F,-1.137534F,-0.422354F},
     {-1.115633F,2.027653F,0.034136F},
     {0.196587F,-0.357296F,1.095306F}}
};

Errc PXYZ2RGB( const Imc2dsf &ims, Imc2duc &imd, int num ) {
   Point2d p;

   imd.ColorSpace(RGB);

   for (p.y=0;p.y<ims.Height();p.y++) {
      for (p.x=0;p.x<ims.Width();p.x++) {
	 float tmpX,tmpY,tmpZ;
	 tmpX=ims.X[p]*255;
	 tmpY=ims.Y[p]*255;
	 tmpZ=ims.Z[p]*255;
	 imd.X[p]= (Uchar)((XYZtoRGB[num][0][0]*tmpX)
			      + (XYZtoRGB[num][0][1]*tmpY)
			      + (XYZtoRGB[num][0][2]*tmpZ));
	 imd.Y[p]= (Uchar)((XYZtoRGB[num][1][0]*tmpX)
			      + (XYZtoRGB[num][1][1]*tmpY)
			      + (XYZtoRGB[num][1][2]*tmpZ));
	 imd.Z[p]= (Uchar)((XYZtoRGB[num][2][0]*tmpX)
			      + (XYZtoRGB[num][2][1]*tmpY)
			      + (XYZtoRGB[num][2][2]*tmpZ));
      }
   }
   return SUCCESS;
}

Errc PXYZ2RGB( const Imc3dsf &ims, Imc3duc &imd, int num ) {
   Point3d p;

   imd.ColorSpace(RGB);
   
   for (p.z=0;p.z<ims.Depth();p.z++) {
      for (p.y=0;p.y<ims.Height();p.y++) {
	 for (p.x=0;p.x<ims.Width();p.x++) {
	    float tmpX,tmpY,tmpZ;
	    tmpX=ims.X[p]*255;
	    tmpY=ims.Y[p]*255;
	    tmpZ=ims.Z[p]*255;
	    imd.X[p]= (Uchar)((XYZtoRGB[num][0][0]*tmpX)
				 + (XYZtoRGB[num][0][1]*tmpY)
				 + (XYZtoRGB[num][0][2]*tmpZ));
	    imd.Y[p]= (Uchar)((XYZtoRGB[num][1][0]*tmpX)
				 + (XYZtoRGB[num][1][1]*tmpY)
				 + (XYZtoRGB[num][1][2]*tmpZ));
	    imd.Z[p]= (Uchar)((XYZtoRGB[num][2][0]*tmpX)
				 + (XYZtoRGB[num][2][1]*tmpY)
				 + (XYZtoRGB[num][2][2]*tmpZ));
	 }
      }
   }
   return SUCCESS;
}

#ifdef MAIN

/*
 * Modify only the following constants, and the operator switches.
 */
#define	USAGE	"usage: %s primaires [-m mask] [im_in|-] [im_out|-]"
#define	PARC	1
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
   case Po_Imc2dsf :{ // Image Char
      Imc2dsf* const ims=(Imc2dsf*)objs[0];
      objd[0]=new Imc2duc(ims->Size());
      Imc2duc* const imd=(Imc2duc*)objd[0];
      
      result=PXYZ2RGB(*ims,*imd,atoi(parv[0]));
   } break;
   case Po_Imc3dsf :{ // Image Char
      Imc3dsf* const ims=(Imc3dsf*)objs[0];
      objd[0]=new Imc3duc(ims->Size());
      Imc3duc* const imd=(Imc3duc*)objd[0];
      
      result=PXYZ2RGB(*ims,*imd,atoi(parv[0]));
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
