/* -*- c-basic-offset: 3; mode: c++ -*-
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
 * @author regis Clouard - 2003-06-25 (add 3D).
 */

#include <math.h>
#include <pandore.h>
using namespace pandore;

/**
 * @file pxyz2luv.cpp
 * Passe du XYZ au LUV
 */

const float Blanc[7][3] = {
   //illuminant E
   {1.0000f, 1.0f, 1.0000f},
   //illuminant primaires CIE-DIN
   {0.951f, 1.f, 1.089f},
   //illuminant A primaires macbeth colour chart
   {1.0983f, 1.0f, 0.3555f},
   //illuminant A primaires CIE
   {1.0983f, 1.0f, 0.3555F},
   //illuminant C primaires NTSC
   {0.9804f, 1.0f, 1.1810f},
   //illuminant C primaires CIE
   {0.9804f, 1.0f, 1.1810f},
   //illuminant D65 primaires Visu Couleur
   {0.9502f, 1.0f, 1.0881f}
};


Errc PXYZ2LUV( const Imc2dsf &ims, Imc2dsf &imd, int num ) {
   Point2d p;
   Float up, vp, upn, vpn, t, tn, rap;

   if (num < 0 || num > 6) {
      std::cerr << "Error pxyz2luv: Bad primaries value." << std::endl;
      return FAILURE;
   }

   imd.ColorSpace(LUV);

   for (p.y = 0; p.y < ims.Height(); p.y++)
   for (p.x = 0; p.x < ims.Width(); p.x++)
   {
      tn = Float(Blanc[num][0] + 15. * Blanc[num][1] + 3. * Blanc[num][2]);
      upn = Float(4.*  Blanc[num][0] / tn);
      vpn = vp = Float(9. * Blanc[num][1] / tn);
      
      t = Float(ims.X[p] + 15. * ims.Y[p] + 3. * ims.Z[p]);
      
      if (t == 0.F) {
	 up = 0.F;
	 vp = 0.F;
      } else {
	 up = Float(4. * ims.X[p] / t);
	 vp = Float(9. * ims.Y[p] / t);
      }
      
      rap = ims.Y[p] / Blanc[num][1];
      if (rap > 0.008856) {
	 imd.X[p] = Float(116. * pow(rap, 1.F / 3.F) - 16.);
      } else {
	 imd.X[p] = Float(903.3 * rap);
      }
      
      imd.Y[p] = Float(13. * imd.X[p] * (up - upn));
      imd.Z[p] = Float(13. * imd.X[p] * (vp - vpn));
   }
   
   return SUCCESS;
}
Errc PXYZ2LUV( const Imc3dsf &ims, Imc3dsf &imd, int num ) {
   Point3d p;
   Float up, vp, upn, vpn, t, tn, rap;

   if (num < 0 || num > 6) {
      std::cerr << "Error pxyz2luv: Bad primaries value." << std::endl;
      return FAILURE;
   }

   imd.ColorSpace(LUV);

   for (p.z = 0; p.z < ims.Depth(); p.z++)
   for (p.y = 0; p.y < ims.Height(); p.y++)
   for (p.x = 0; p.x < ims.Width(); p.x++)
   {
      tn = Float(Blanc[num][0] + 15. * Blanc[num][1] + 3. * Blanc[num][2]);
      upn = Float(4.*  Blanc[num][0] / tn);
      vpn = vp = Float(9. * Blanc[num][1] / tn);
      
      t = Float(ims.X[p] + 15. * ims.Y[p] + 3. * ims.Z[p]);
      
      if (t == 0.F) {
	 up = 0.F;
	 vp = 0.F;
      } else {
	 up = Float(4. * ims.X[p] / t);
	 vp = Float(9. * ims.Y[p] / t);
      }
      
      rap = ims.Y[p] / Blanc[num][1];
      if (rap > 0.008856) {
	 imd.X[p] = Float(116. * pow(rap, 1.F / 3.F) - 16.);
      } else {
	 imd.X[p] = Float(903.3 * rap);
      }
      
      imd.Y[p] = Float(13. * imd.X[p] * (up - upn));
      imd.Z[p] = Float(13. * imd.X[p] * (vp - vpn));
   }
   
   return SUCCESS;
}

#ifdef MAIN
#define USAGE	"usage: %s primaries [im_in|-] [im_out|-]"
#define PARC	1
#define FINC	1
#define FOUTC	1
#define MASK	0
int main( int argc, char *argv[] ) {
   Errc result;                // The result code of the execution.
   Pobject* mask;              // The region map.
   Pobject* objin[FINC + 1];   // The input objects.
   Pobject* objs[FINC + 1];    // The source objects masked.
   Pobject* objout[FOUTC + 1]; // The output object.
   Pobject* objd[FOUTC + 1];   // The result object of the execution.
   char* parv[PARC + 1];       // The input parameters.

   ReadArgs(argc, argv, PARC, FINC, FOUTC, &mask, objin, objs, objout, objd, parv, USAGE, MASK); 
   if (objs[0]->Type() == Po_Imc2dsf) {
      Imc2dsf* const ims = (Imc2dsf*)objs[0];
      objd[0] = new Imc2dsf(ims->Size());
      Imc2dsf* const imd = (Imc2dsf*)objd[0];
      
      result = PXYZ2LUV(*ims, *imd, atoi(parv[0]));
      goto end;
   }
   if (objs[0]->Type() == Po_Imc3dsf) {
      Imc3dsf* const ims = (Imc3dsf*)objs[0];
      objd[0] = new Imc3dsf(ims->Size());
      Imc3dsf* const imd = (Imc3dsf*)objd[0];
      
      result = PXYZ2LUV(*ims, *imd, atoi(parv[0]));
      goto end;
   }
  {
     PrintErrorFormat(objin, FINC, argv); 
     result = FAILURE; 
  }	

end:
  if (result) {
	WriteArgs(argc, argv, PARC, FINC, FOUTC, &mask, objin, objs, objout, objd, MASK); 
  }
  Exit(result); 
  return 0; 
}
#endif
