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
 * @author Régis Clouard - 1995-21-07
 */

#include <pandore.h>
using namespace pandore;


/**
 * @file pextractsubimage.cpp
 * Extract subimage from image.
 * Coordinates are  x = parv[0], y = parv[1], z = parv[2]
 * and size is w = parv[3]*h = parv[4]*d = parv[5].
 */
template <typename T>
Errc PExtractSubImage( const Imx2d<T> &ims, Imx2d<T> &imd, Long y, Long x, Long h, Long w ) {
   Point2d p, q;
   q.y = y;
   q.x = x;

   if (x < 0 || y < 0) {
      std::cerr << "Error pextractsubimage: Bad coordinate values: " << x << ", " << y << std::endl;
      return FAILURE;
   }

   if (w < 0) {
      w = ims.Width();
   }
   if (h < 0) {
      h = ims.Height();
   }

   if (w == 0 || h == 0) {
      std::cerr << "Error pextractsubimage: Bad size values: " << w << ", " << h << std::endl;
      return FAILURE;
   }

   if (w + x > ims.Width()) {
      w = ims.Width() - x;
   }
   if (h + y > ims.Height()) {
      h = ims.Height() - y;
   }

   if (w == 0 || h == 0) {
      std::cerr << "Error pextractsubimage: Bad parameter values: out of bound: " << x << ", " << y << ", "<< w << ", " << h << std::endl;
      return FAILURE;
   }

   imd.New(ims.Bands(), h, w);

   for (p.y = 0; p.y < imd.Height(); p.y++) {
      for (p.x = 0;p.x < imd.Width(); p.x++) {
	 for (int b = 0; b < ims.Bands(); b++) {
	    imd[b][p]  =  ims[b][p + q];
	 }
      }
   }
   
   return SUCCESS;
}

template <typename T>
Errc PExtractSubImage( const Imx3d<T> &ims, Imx3d<T> &imd, Long z, Long y, Long x, Long d, Long h, Long w ) {
   Point3d p, q;
   q.z = z;
   q.y = y;
   q.x = x;
   
   if (x < 0 || y < 0 || z < 0) {
      std::cerr << "Error pextractsubimage: Bad coordinate values: " << x << ", " << y << ", " << z << std::endl;
      return FAILURE;
   }

   if (w < 0) {
      w = ims.Width();
   }
   if (h < 0) {
      h = ims.Height();
   }
   if (d < 0) {
      d = ims.Depth();
   }

   if (w == 0 || h == 0 || d == 0) {
      std::cerr << "Error pextractsubimage: Bad size values: " << w << ", " << h << ", " << d << std::endl;
      return FAILURE;
   }

   if (w + x > ims.Width()) {
      w = ims.Width() - x;
   }
   if (h + y > ims.Height()) {
      h = ims.Height() - y;
   }
   if (d + z > ims.Depth()) {
      d = ims.Depth() - z;
   }

   if (w == 0 || h == 0 || d == 0) {
      std::cerr << "Error pextractsubimage: Bad parameter values: out of bound: " << x << ", " << y << ", "<< z << ", " << w << ", " << h << ", "<< d << std::endl;
      return FAILURE;
   }
   
   imd.New(ims.Bands(), d, h, w);

   for (p.z = 0; p.z < imd.Depth(); p.z++) {
      for (p.y = 0; p.y < imd.Height(); p.y++) {
	 for (p.x = 0; p.x < imd.Width(); p.x++) {
	    for (int b = 0; b < ims.Bands(); b++) {
	       imd[b][p]  =  ims[b][p + q];
	    }
	 }
      }
   }
   
   return SUCCESS;
}









#ifdef MAIN
#define	USAGE	"usage: %s x y z width height depth [im_in|-][im_out|-]"
#define	PARC	6
#define	FINC	1
#define	FOUTC	1
#define	MASK	0

int main( int argc, char *argv[] ) {
   Errc result;                // The result code of the execution.
   Pobject* mask;              // The region map.
   Pobject* objin[FINC + 1];   // The input objects.
   Pobject* objs[FINC + 1];    // The source objects masked.
   Pobject* objout[FOUTC + 1]; // The output object.
   Pobject* objd[FOUTC + 1];   // The result object of the execution.
   char* parv[PARC + 1];       // The input parameters.

   ReadArgs(argc, argv, PARC, FINC, FOUTC, &mask, objin, objs, objout, objd, parv, USAGE, MASK); 
   if (objs[0]->Type() == Po_Img2duc) {
      Img2duc* const ims = (Img2duc*)objs[0];
      objd[0] = new Img2duc;
      Img2duc* const imd = (Img2duc*)objd[0];
      result = PExtractSubImage(*ims, *imd, atoi(parv[1]), atoi(parv[0]), atoi(parv[4]), atoi(parv[3]));
      goto end;
   }
   if (objs[0]->Type() == Po_Img2dsl) {
      Img2dsl* const ims = (Img2dsl*)objs[0];
      objd[0] = new Img2dsl;
      Img2dsl* const imd = (Img2dsl*)objd[0];
      result = PExtractSubImage(*ims, *imd, atoi(parv[1]), atoi(parv[0]), atoi(parv[4]), atoi(parv[3]));
      goto end;
   }
   if (objs[0]->Type() == Po_Img2dsf) {
      Img2dsf* const ims = (Img2dsf*)objs[0];
      objd[0] = new Img2dsf;
      Img2dsf* const imd = (Img2dsf*)objd[0];
      result = PExtractSubImage(*ims, *imd, atoi(parv[1]), atoi(parv[0]), atoi(parv[4]), atoi(parv[3]));
      goto end;
   }
   if (objs[0]->Type() == Po_Imc2duc) {
      Imc2duc* const ims = (Imc2duc*)objs[0];
      objd[0] = new Imc2duc;
      Imc2duc* const imd = (Imc2duc*)objd[0];
      result = PExtractSubImage(*ims, *imd, atoi(parv[1]), atoi(parv[0]), atoi(parv[4]), atoi(parv[3]));
      goto end;
   }
   if (objs[0]->Type() == Po_Imc2dsl) {
      Imc2dsl* const ims = (Imc2dsl*)objs[0];
      objd[0] = new Imc2dsl;
      Imc2dsl* const imd = (Imc2dsl*)objd[0];
      result = PExtractSubImage(*ims, *imd, atoi(parv[1]), atoi(parv[0]), atoi(parv[4]), atoi(parv[3]));
      goto end;
   }
   if (objs[0]->Type() == Po_Imc2dsf) {
      Imc2dsf* const ims = (Imc2dsf*)objs[0];
      objd[0] = new Imc2dsf;
      Imc2dsf* const imd = (Imc2dsf*)objd[0];
      result = PExtractSubImage(*ims, *imd, atoi(parv[1]), atoi(parv[0]), atoi(parv[4]), atoi(parv[3]));
      goto end;
   }
   if (objs[0]->Type() == Po_Imx2duc) {
      Imx2duc* const ims = (Imx2duc*)objs[0];
      objd[0] = new Imx2duc;
      Imx2duc* const imd = (Imx2duc*)objd[0];
      result = PExtractSubImage(*ims, *imd, atoi(parv[1]), atoi(parv[0]), atoi(parv[4]), atoi(parv[3]));
      goto end;
   }
   if (objs[0]->Type() == Po_Imx2dsl) {
      Imx2dsl* const ims = (Imx2dsl*)objs[0];
      objd[0] = new Imx2dsl;
      Imx2dsl* const imd = (Imx2dsl*)objd[0];
      result = PExtractSubImage(*ims, *imd, atoi(parv[1]), atoi(parv[0]), atoi(parv[4]), atoi(parv[3]));
      goto end;
   }
   if (objs[0]->Type() == Po_Imx2dsf) {
      Imx2dsf* const ims = (Imx2dsf*)objs[0];
      objd[0] = new Imx2dsf;
      Imx2dsf* const imd = (Imx2dsf*)objd[0];
      result = PExtractSubImage(*ims, *imd, atoi(parv[1]), atoi(parv[0]), atoi(parv[4]), atoi(parv[3]));
      goto end;
   }
if (objs[0]->Type() == Po_Reg2d) {
      Reg2d* const rgs = (Reg2d*)objs[0];
      objd[0] = new Reg2d;
      Reg2d* const rgd = (Reg2d*)objd[0];
      result = PExtractSubImage(*rgs, *rgd, atoi(parv[1]), atoi(parv[0]), atoi(parv[4]), atoi(parv[3]));
      rgd->Labels(rgs->Labels());
      goto end;
   }
   if (objs[0]->Type() == Po_Img3duc) {
      Img3duc* const ims = (Img3duc*)objs[0];
      objd[0] = new Img3duc;
      Img3duc* const imd = (Img3duc*)objd[0];
      result = PExtractSubImage(*ims, *imd, atoi(parv[2]), atoi(parv[1]), atoi(parv[0]), atoi(parv[5]), atoi(parv[4]), atoi(parv[3]));
      goto end;
   }
   if (objs[0]->Type() == Po_Img3dsl) {
      Img3dsl* const ims = (Img3dsl*)objs[0];
      objd[0] = new Img3dsl;
      Img3dsl* const imd = (Img3dsl*)objd[0];
      result = PExtractSubImage(*ims, *imd, atoi(parv[2]), atoi(parv[1]), atoi(parv[0]), atoi(parv[5]), atoi(parv[4]), atoi(parv[3]));
      goto end;
   }
   if (objs[0]->Type() == Po_Img3dsf) {
      Img3dsf* const ims = (Img3dsf*)objs[0];
      objd[0] = new Img3dsf;
      Img3dsf* const imd = (Img3dsf*)objd[0];
      result = PExtractSubImage(*ims, *imd, atoi(parv[2]), atoi(parv[1]), atoi(parv[0]), atoi(parv[5]), atoi(parv[4]), atoi(parv[3]));
      goto end;
   }
   if (objs[0]->Type() == Po_Imc3duc) {
      Imc3duc* const ims = (Imc3duc*)objs[0];
      objd[0] = new Imc3duc;
      Imc3duc* const imd = (Imc3duc*)objd[0];
      result = PExtractSubImage(*ims, *imd, atoi(parv[2]), atoi(parv[1]), atoi(parv[0]), atoi(parv[5]), atoi(parv[4]), atoi(parv[3]));
      goto end;
   }
   if (objs[0]->Type() == Po_Imc3dsl) {
      Imc3dsl* const ims = (Imc3dsl*)objs[0];
      objd[0] = new Imc3dsl;
      Imc3dsl* const imd = (Imc3dsl*)objd[0];
      result = PExtractSubImage(*ims, *imd, atoi(parv[2]), atoi(parv[1]), atoi(parv[0]), atoi(parv[5]), atoi(parv[4]), atoi(parv[3]));
      goto end;
   }
   if (objs[0]->Type() == Po_Imc3dsf) {
      Imc3dsf* const ims = (Imc3dsf*)objs[0];
      objd[0] = new Imc3dsf;
      Imc3dsf* const imd = (Imc3dsf*)objd[0];
      result = PExtractSubImage(*ims, *imd, atoi(parv[2]), atoi(parv[1]), atoi(parv[0]), atoi(parv[5]), atoi(parv[4]), atoi(parv[3]));
      goto end;
   }
   if (objs[0]->Type() == Po_Imx3duc) {
      Imx3duc* const ims = (Imx3duc*)objs[0];
      objd[0] = new Imx3duc;
      Imx3duc* const imd = (Imx3duc*)objd[0];
      result = PExtractSubImage(*ims, *imd, atoi(parv[2]), atoi(parv[1]), atoi(parv[0]), atoi(parv[5]), atoi(parv[4]), atoi(parv[3]));
      goto end;
   }
   if (objs[0]->Type() == Po_Imx3dsl) {
      Imx3dsl* const ims = (Imx3dsl*)objs[0];
      objd[0] = new Imx3dsl;
      Imx3dsl* const imd = (Imx3dsl*)objd[0];
      result = PExtractSubImage(*ims, *imd, atoi(parv[2]), atoi(parv[1]), atoi(parv[0]), atoi(parv[5]), atoi(parv[4]), atoi(parv[3]));
      goto end;
   }
   if (objs[0]->Type() == Po_Imx3dsf) {
      Imx3dsf* const ims = (Imx3dsf*)objs[0];
      objd[0] = new Imx3dsf;
      Imx3dsf* const imd = (Imx3dsf*)objd[0];
      result = PExtractSubImage(*ims, *imd, atoi(parv[2]), atoi(parv[1]), atoi(parv[0]), atoi(parv[5]), atoi(parv[4]), atoi(parv[3]));
      goto end;
   }
   if (objs[0]->Type() == Po_Reg3d) {
      Reg3d* const rgs = (Reg3d*)objs[0];
      objd[0] = new Reg3d;
      Reg3d* const rgd = (Reg3d*)objd[0];
      result = PExtractSubImage(*rgs, *rgd, atoi(parv[2]), atoi(parv[1]), atoi(parv[0]), atoi(parv[5]), atoi(parv[4]), atoi(parv[3]));
      rgd->Labels(rgs->Labels());
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
