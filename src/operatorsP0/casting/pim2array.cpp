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
 * @author Alexandre Duret-Lutz - 1995-10-26
 * @author Regis Clouard - 2004-08-03 (version 5)
 */

#include <pandore.h>
using namespace pandore;

#if (defined _MSC_VER) && (!defined snprintf)
#define snprintf _snprintf
#endif

/**
 * @file pim2array.cpp
 * Converts an image to array(s) (1 for gray image, 3 for color images).
 * If the mask is given just put the unmasked pixel in the array.
 */
template <typename T>
Errc PIm2Array( const Imx3d<T> &ims, Collection &cold, std::string name, const Pobject * const mask ) {
#define MAXLENGTH 10
   char number[MAXLENGTH];
   Long nbrindiv = ims.VectorSize(); // Number of pixels per band.
   int nbvalues; // Real number of values in the array.
   
   for (int b = 0; b < ims.Bands(); b++) {
      snprintf(number, sizeof(number), ".%d", b + 1);
      number[sizeof(number) - 1] = 0;
      std::string realname;
      if (ims.Bands() > 1) {
	 realname = name +number;
      } else {
	 realname = name;
      }
      T* pixels = new T[nbrindiv];
      T* ps = ims.Vector(b);
      T* px = pixels;
      if (mask) {
	 Long *pm = reinterpret_cast<const Imx3d<Long>*>(mask)->Vector(0);
	 nbvalues = 0;
	 for (; ps < ims.Vector(b) + ims.VectorSize(); ps++, pm++) {
	    if (*pm) {
	       *(px++) = *ps;
	       nbvalues++;
	    }
	 }
      } else {
	 nbvalues = nbrindiv;
	 for (; ps < ims.Vector(b) + ims.VectorSize(); ps++, px++) {
	    *px= *ps;
	 }
      }
      cold.SETARRAY(realname, T, pixels, nbvalues);
   }
   return SUCCESS;
}




#ifdef MAIN
#define	USAGE	"usage: %s name [-m mask] [im_in|-] [col_out|-]"
#define	PARC	1
#define	FINC	1
#define	FOUTC	1
#define	MASK	2
int main( int argc, char *argv[] ) {
   Errc result;                // The result code of the execution.
   Pobject* mask;              // The region map.
   Pobject* objin[FINC + 1];   // The input objects.
   Pobject* objs[FINC + 1];    // The source objects masked.
   Pobject* objout[FOUTC + 1]; // The output object.
   Pobject* objd[FOUTC + 1];   // The result object of the execution.
   char* parv[PARC + 1];       // The input parameters.

   ReadArgs(argc, argv, PARC, FINC, FOUTC, &mask, objin, objs, objout, objd, parv, USAGE, MASK); 
   if (objs[0]->Type() == Po_Img1duc) {
      const Img1duc* const ims = (Img1duc*)objs[0];
      objd[0] = new Collection;
      Collection* const imd = (Collection*)objd[0];
      result = PIm2Array(*ims, *imd, argv[1], mask);
      goto end;
   }
   if (objs[0]->Type() == Po_Img1dsl) {
      const Img1dsl* const ims = (Img1dsl*)objs[0];
      objd[0] = new Collection;
      Collection* const imd = (Collection*)objd[0];
      result = PIm2Array(*ims, *imd, argv[1], mask);
      goto end;
   }
   if (objs[0]->Type() == Po_Img1dsf) {
      const Img1dsf* const ims = (Img1dsf*)objs[0];
      objd[0] = new Collection;
      Collection* const imd = (Collection*)objd[0];
      result = PIm2Array(*ims, *imd, argv[1], mask);
      goto end;
   }
   if (objs[0]->Type() == Po_Img2duc) {
      const Img2duc* const ims = (Img2duc*)objs[0];
      objd[0] = new Collection;
      Collection* const imd = (Collection*)objd[0];
      result = PIm2Array(*ims, *imd, argv[1], mask);
      goto end;
   }
   if (objs[0]->Type() == Po_Img2dsl) {
      const Img2dsl* const ims = (Img2dsl*)objs[0];
      objd[0] = new Collection;
      Collection* const imd = (Collection*)objd[0];
      result = PIm2Array(*ims, *imd, argv[1], mask);
      goto end;
   }
   if (objs[0]->Type() == Po_Img2dsf) {
      const Img2dsf* const ims = (Img2dsf*)objs[0];
      objd[0] = new Collection;
      Collection* const imd = (Collection*)objd[0];
      result = PIm2Array(*ims, *imd, argv[1], mask);
      goto end;
   }
   if (objs[0]->Type() == Po_Img3duc) {
      const Img3duc* const ims = (Img3duc*)objs[0];
      objd[0] = new Collection;
      Collection* const imd = (Collection*)objd[0];
      result = PIm2Array(*ims, *imd, argv[1], mask);
      goto end;
   }
   if (objs[0]->Type() == Po_Img3dsl) {
      const Img3dsl* const ims = (Img3dsl*)objs[0];
      objd[0] = new Collection;
      Collection* const imd = (Collection*)objd[0];
      result = PIm2Array(*ims, *imd, argv[1], mask);
      goto end;
   }
   if (objs[0]->Type() == Po_Img3dsf) {
      const Img3dsf* const ims = (Img3dsf*)objs[0];
      objd[0] = new Collection;
      Collection* const imd = (Collection*)objd[0];
      result = PIm2Array(*ims, *imd, argv[1], mask);
      goto end;
   }
   if (objs[0]->Type() == Po_Imc2duc) {
      const Imc2duc* const ims = (Imc2duc*)objs[0];
      objd[0] = new Collection;
      Collection* const imd = (Collection*)objd[0];
      result = PIm2Array(*ims, *imd, argv[1], mask);
      goto end;
   }
   if (objs[0]->Type() == Po_Imc2dsl) {
      const Imc2dsl* const ims = (Imc2dsl*)objs[0];
      objd[0] = new Collection;
      Collection* const imd = (Collection*)objd[0];
      result = PIm2Array(*ims, *imd, argv[1], mask);
      goto end;
   }
   if (objs[0]->Type() == Po_Imc2dsf) {
      const Imc2dsf* const ims = (Imc2dsf*)objs[0];
      objd[0] = new Collection;
      Collection* const imd = (Collection*)objd[0];
      result = PIm2Array(*ims, *imd, argv[1], mask);
      goto end;
   }
   if (objs[0]->Type() == Po_Imc3duc) {
      const Imc3duc* const ims = (Imc3duc*)objs[0];
      objd[0] = new Collection;
      Collection* const imd = (Collection*)objd[0];
      result = PIm2Array(*ims, *imd, argv[1], mask);
      goto end;
   }
   if (objs[0]->Type() == Po_Imc3dsl) {
      const Imc3dsl* const ims = (Imc3dsl*)objs[0];
      objd[0] = new Collection;
      Collection* const imd = (Collection*)objd[0];
      result = PIm2Array(*ims, *imd, argv[1], mask);
      goto end;
   }
   if (objs[0]->Type() == Po_Imc3dsf) {
      const Imc3dsf* const ims = (Imc3dsf*)objs[0];
      objd[0] = new Collection;
      Collection* const imd = (Collection*)objd[0];
      result = PIm2Array(*ims, *imd, argv[1], mask);
      goto end;
   }
   if (objs[0]->Type() == Po_Imx1duc) {
      const Imx1duc* const ims = (Imx1duc*)objs[0];
      objd[0] = new Collection;
      Collection* const imd = (Collection*)objd[0];
      result = PIm2Array(*ims, *imd, argv[1], mask);
      goto end;
   }
   if (objs[0]->Type() == Po_Imx1dsl) {
      const Imx1dsl* const ims = (Imx1dsl*)objs[0];
      objd[0] = new Collection;
      Collection* const imd = (Collection*)objd[0];
      result = PIm2Array(*ims, *imd, argv[1], mask);
      goto end;
   }
   if (objs[0]->Type() == Po_Imx1dsf) {
      const Imx1dsf* const ims = (Imx1dsf*)objs[0];
      objd[0] = new Collection;
      Collection* const imd = (Collection*)objd[0];
      result = PIm2Array(*ims, *imd, argv[1], mask);
      goto end;
   }
   if (objs[0]->Type() == Po_Imx2duc) {
      const Imx2duc* const ims = (Imx2duc*)objs[0];
      objd[0] = new Collection;
      Collection* const imd = (Collection*)objd[0];
      result = PIm2Array(*ims, *imd, argv[1], mask);
      goto end;
   }
   if (objs[0]->Type() == Po_Imx2dsl) {
      const Imx2dsl* const ims = (Imx2dsl*)objs[0];
      objd[0] = new Collection;
      Collection* const imd = (Collection*)objd[0];
      result = PIm2Array(*ims, *imd, argv[1], mask);
      goto end;
   }
   if (objs[0]->Type() == Po_Imx2dsf) {
      const Imx2dsf* const ims = (Imx2dsf*)objs[0];
      objd[0] = new Collection;
      Collection* const imd = (Collection*)objd[0];
      result = PIm2Array(*ims, *imd, argv[1], mask);
      goto end;
   }
   if (objs[0]->Type() == Po_Imx3duc) {
      const Imx3duc* const ims = (Imx3duc*)objs[0];
      objd[0] = new Collection;
      Collection* const imd = (Collection*)objd[0];
      result = PIm2Array(*ims, *imd, argv[1], mask);
      goto end;
   }
   if (objs[0]->Type() == Po_Imx3dsl) {
      const Imx3dsl* const ims = (Imx3dsl*)objs[0];
      objd[0] = new Collection;
      Collection* const imd = (Collection*)objd[0];
      result = PIm2Array(*ims, *imd, argv[1], mask);
      goto end;
   }
   if (objs[0]->Type() == Po_Imx3dsf) {
      const Imx3dsf* const ims = (Imx3dsf*)objs[0];
      objd[0] = new Collection;
      Collection* const imd = (Collection*)objd[0];
      result = PIm2Array(*ims, *imd, argv[1], mask);
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
