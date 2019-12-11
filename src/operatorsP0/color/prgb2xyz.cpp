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
 * @author Olivier Lezoray - 2002-05-21
 */

#include <pandore.h>
using namespace pandore;

/** 
 * @file prgb2xyz.cpp
 * Passe du RGB au XYZ
 */
const float RGBtoXYZ[7][3][3] = {
   //illuminant E
   { {2.7690F, 1.7518F, 1.1300F}, 
     {1.000F, 4.5907F, 0.0601F}, 
     {0.000F, 0.0565F, 5.5943F}}, 
   //illuminant primaires CIE-DIN
   { {0.431F, 0.342F, 0.178F}, 
     {0.222F, 0.707F, 0.071F}, 
     {0.020F, 0.130F, 0.939F}}, 
   //illuminant A primaires macbeth colour chart
   { {1.083F*0.9960F, -0.909F*0.0320F, 0.361F*0.135F}, 
     {1.083F*0.353F, 0.909F*0.7010F, -0.361F*0.0540F}, 
     {1.083F*0.033F, -0.909F*0.0970F, 0.361F*1.1300F}}, 
   //illuminant A primaires CIE
   { {0.8920F, 0.3300F, 0.083F}, 
     {0.322F, 0.8630F, 0.0040F}, 
     {0.000F, 0.0110F, 0.4090F}}, 
   //illuminant C primaires NTSC
   { {0.6070F, 0.1740F, 0.2000F}, 
     {0.299F, 0.5870F, 0.1140F}, 
     {0.000F, 0.0660F, 1.1160F}}, 
   //illuminant C primaires CIE
   { {0.1660F, 0.1250F, 0.0930F}, 
     {0.060F, 0.3270F, 0.0050F}, 
     {0.000F, 0.0040F, 0.4600F}}, 
   //illuminant D65 primaires Visu Couleur
   { {0.4780F, 0.2990F, 0.175F}, 
     {0.263F, 0.6550F, 0.0810F}, 
     {0.000F, 0.1600F, 0.9080F}}
};

Errc PRGB2XYZ( const Imc2duc &ims, Imc2dsf &imd, int num ) {
   Point2d p;
   
   imd.ColorSpace(XYZ);

   for (p.y = 0; p.y < ims.Height(); p.y++) {
      for (p.x = 0; p.x < ims.Width(); p.x++) {
	 imd.X[p] = Float(ims.X[p]) / 255.F; 
	 imd.Y[p] = Float(ims.Y[p]) / 255.F;
	 imd.Z[p] = Float(ims.Z[p]) / 255.F;
      }
   }
   for (p.y = 0; p.y < ims.Height(); p.y++) {
      for (p.x = 0;p.x < ims.Width(); p.x++) {
	 float tmpX, tmpY, tmpZ;
	 tmpX = imd.X[p];
	 tmpY = imd.Y[p];
	 tmpZ = imd.Z[p];
	 imd.X[p] = (RGBtoXYZ[num][0][0] * tmpX)
	    + (RGBtoXYZ[num][0][1] * tmpY)
	    + (RGBtoXYZ[num][0][2] * tmpZ);
	 imd.Y[p] = (RGBtoXYZ[num][1][0] * tmpX)
	    + (RGBtoXYZ[num][1][1] * tmpY)
	    + (RGBtoXYZ[num][1][2] * tmpZ);
	 imd.Z[p] = (RGBtoXYZ[num][2][0] * tmpX)
	    + (RGBtoXYZ[num][2][1] * tmpY)
	    + (RGBtoXYZ[num][2][2] * tmpZ);
      }
   }
   return SUCCESS;
}

Errc PRGB2XYZ( const Imc3duc &ims, Imc3dsf &imd, int num ) {
   Point3d p;

   imd.ColorSpace(XYZ);
   
   for (p.z = 0; p.z < ims.Depth(); p.z++) {
      for (p.y = 0; p.y < ims.Height(); p.y++) {
	 for (p.x = 0; p.x < ims.Width(); p.x++) {
	    imd.X[p] = Float(ims.X[p]) / 255.F; 
	    imd.Y[p] = Float(ims.Y[p]) / 255.F;
	    imd.Z[p] = Float(ims.Z[p]) / 255.F;
	 }
      }
   }
   for (p.z = 0; p.z < ims.Depth(); p.z++) {
      for (p.y = 0; p.y < ims.Height(); p.y++) {
	 for (p.x = 0; p.x < ims.Width(); p.x++) {
	    float tmpX, tmpY, tmpZ;
	    tmpX = imd.X[p];
	    tmpY = imd.Y[p];
	    tmpZ  =imd.Z[p];
	    imd.X[p] = (RGBtoXYZ[num][0][0] * tmpX)
	       + (RGBtoXYZ[num][0][1] * tmpY)
	       + (RGBtoXYZ[num][0][2] * tmpZ);
	    imd.Y[p] = (RGBtoXYZ[num][1][0] * tmpX)
	       + (RGBtoXYZ[num][1][1] * tmpY)
	       + (RGBtoXYZ[num][1][2] * tmpZ);
	    imd.Z[p] = (RGBtoXYZ[num][2][0] * tmpX)
	       + (RGBtoXYZ[num][2][1] * tmpY)
	       + (RGBtoXYZ[num][2][2] * tmpZ);
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
   Errc result;                // The result code of the execution.
   Pobject* mask;              // The region mask.
   Pobject* objin[FINC + 1];   // The input objects.
   Pobject* objs[FINC + 1];    // The source objects masked by the mask.
   Pobject* objout[FOUTC + 1]; // The ouput object.
   Pobject* objd[FOUTC + 1];   // The result object of the execution.
   char* parv[PARC + 1];       // The input parameters.
   
   ReadArgs(argc, argv, PARC, FINC, FOUTC, &mask, objin, objs, objout, objd, parv, USAGE, MASK);
   
   switch(objs[0]->Type()) {
       case Po_Imc2duc: { // Image Char
	  Imc2duc* const ims = (Imc2duc*)objs[0];
	  objd[0] = new Imc2dsf(ims->Size());
	  Imc2dsf* const imd = (Imc2dsf*)objd[0];
      
	  result = PRGB2XYZ(*ims, *imd, atoi(parv[0]));
       } break;
       case Po_Imc3duc: { // Image Char
	  Imc3duc* const ims = (Imc3duc*)objs[0];
	  objd[0] = new Imc3dsf(ims->Size());
	  Imc3dsf* const imd = (Imc3dsf*)objd[0];
      
	  result = PRGB2XYZ(*ims, *imd, atoi(parv[0]));
       } break;
       default:
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
