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
 * @author Régis Clouard - 2006-01-09
 * @author Régis Clouard - 2007-02-16 (color : use the vectorial approach)
 */

#include <pandore.h>
#include <math.h>
using namespace pandore;

/**
 * @file plineartransform.cpp
 * @brief Gray level transformation using linear transform.
 * 
 * Expands or compresses gray-levels using a linear transform of the gray-levels.
 * s=c.r where s is output gray-level and r the input gray-level, 
 * Uses the following transformation:
 * s = c . (r-smin)
 * where c is use to normalize value from min and max
 * c=(max-min) / (smax-smin)
 */
template <typename T>
Errc PLinearTransform( const Imx3d<T> &ims, const Imx3d<T> &imd, int inverse, float min, float max ) {
   if (min > max) {
      min=(float)Limits<T>::min();
      max=(float)Limits<T>::max();
   } else if (min < (float)Limits<T>::min() || max > (float)Limits<T>::max()) {
      std::cerr<< "Error plineartransform: Bad parameter values out of bound"<<std::endl;
      return FAILURE;
   }

   float smin, smax;
   T* ps;
   smin = smax = (float) *(ims.Vector(0));
   int b;
   for (b=0; b<ims.Bands(); b++) {
      // Get min, max.
      ps=ims.Vector(b);
      for ( ; ps<ims.Vector(b)+ims.VectorSize(); ps++ ) {  
	 if (smax < (float)*ps) smax = (float)*ps;
	 if (smin > (float)*ps) smin = (float)*ps;
      }
   }

   for (b=0; b<ims.Bands(); b++) {
      ps=ims.Vector(b);
      T* pd=imd.Vector(b);
      double c=(((double)max-min) / ((double)smax-smin));
      if (!inverse) {
	 for (;ps<ims.Vector(b)+ims.VectorSize();ps++, pd++)
	    *pd=(T)((c*(*ps-smin)) + min);
      } else {
	 // Exp transformation.
	 for (;ps<ims.Vector(b)+ims.VectorSize();ps++, pd++) {
	    *pd=(T)((c*(smax-*ps)) + min);
	 }
      }
   }
   return SUCCESS;
}



/*
 * Modify only the following constants, and the function call.
 */
#ifdef MAIN
#define	USAGE	"usage: %s inverse min max [-m mask] [im_in|-] [im_out|-]"
#define	PARC	3
#define	FINC	1
#define	FOUTC	1
#define	MASK	3
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
      objd[0] = new Img2duc(ims->Props());
      Img2duc* const imd = (Img2duc*)objd[0];
      
      result = PLinearTransform(*ims, *imd, atoi(parv[0]), (float)atof(parv[1]), (float)atof(parv[2]));
      goto end;
   }
   if (objs[0]->Type() == Po_Img2dsl) {
      Img2dsl* const ims = (Img2dsl*)objs[0];
      objd[0] = new Img2dsl(ims->Props());
      Img2dsl* const imd = (Img2dsl*)objd[0];
      
      result = PLinearTransform(*ims, *imd, atoi(parv[0]), (float)atof(parv[1]), (float)atof(parv[2]));
      goto end;
   }
   if (objs[0]->Type() == Po_Img2dsf) {
      Img2dsf* const ims = (Img2dsf*)objs[0];
      objd[0] = new Img2dsf(ims->Props());
      Img2dsf* const imd = (Img2dsf*)objd[0];
      
      result = PLinearTransform(*ims, *imd, atoi(parv[0]), (float)atof(parv[1]), (float)atof(parv[2]));
      goto end;
   }
   if (objs[0]->Type() == Po_Img3duc) {
      Img3duc* const ims = (Img3duc*)objs[0];
      objd[0] = new Img3duc(ims->Props());
      Img3duc* const imd = (Img3duc*)objd[0];
      
      result = PLinearTransform(*ims, *imd, atoi(parv[0]), (float)atof(parv[1]), (float)atof(parv[2]));
      goto end;
   }
   if (objs[0]->Type() == Po_Img3dsl) {
      Img3dsl* const ims = (Img3dsl*)objs[0];
      objd[0] = new Img3dsl(ims->Props());
      Img3dsl* const imd = (Img3dsl*)objd[0];
      
      result = PLinearTransform(*ims, *imd, atoi(parv[0]), (float)atof(parv[1]), (float)atof(parv[2]));
      goto end;
   }
   if (objs[0]->Type() == Po_Img3dsf) {
      Img3dsf* const ims = (Img3dsf*)objs[0];
      objd[0] = new Img3dsf(ims->Props());
      Img3dsf* const imd = (Img3dsf*)objd[0];
      
      result = PLinearTransform(*ims, *imd, atoi(parv[0]), (float)atof(parv[1]), (float)atof(parv[2]));
      goto end;
   }
   if (objs[0]->Type() == Po_Imc2duc) {
      Imc2duc* const ims = (Imc2duc*)objs[0];
      objd[0] = new Imc2duc(ims->Props());
      Imc2duc* const imd = (Imc2duc*)objd[0];
      
      result = PLinearTransform(*ims, *imd, atoi(parv[0]), (float)atof(parv[1]), (float)atof(parv[2]));
      goto end;
   }
   if (objs[0]->Type() == Po_Imc2dsl) {
      Imc2dsl* const ims = (Imc2dsl*)objs[0];
      objd[0] = new Imc2dsl(ims->Props());
      Imc2dsl* const imd = (Imc2dsl*)objd[0];
      
      result = PLinearTransform(*ims, *imd, atoi(parv[0]), (float)atof(parv[1]), (float)atof(parv[2]));
      goto end;
   }
   if (objs[0]->Type() == Po_Imc2dsf) {
      Imc2dsf* const ims = (Imc2dsf*)objs[0];
      objd[0] = new Imc2dsf(ims->Props());
      Imc2dsf* const imd = (Imc2dsf*)objd[0];
      
      result = PLinearTransform(*ims, *imd, atoi(parv[0]), (float)atof(parv[1]), (float)atof(parv[2]));
      goto end;
   }
   if (objs[0]->Type() == Po_Imc3duc) {
      Imc3duc* const ims = (Imc3duc*)objs[0];
      objd[0] = new Imc3duc(ims->Props());
      Imc3duc* const imd = (Imc3duc*)objd[0];
      
      result = PLinearTransform(*ims, *imd, atoi(parv[0]), (float)atof(parv[1]), (float)atof(parv[2]));
      goto end;
   }
   if (objs[0]->Type() == Po_Imc3dsl) {
      Imc3dsl* const ims = (Imc3dsl*)objs[0];
      objd[0] = new Imc3dsl(ims->Props());
      Imc3dsl* const imd = (Imc3dsl*)objd[0];
      
      result = PLinearTransform(*ims, *imd, atoi(parv[0]), (float)atof(parv[1]), (float)atof(parv[2]));
      goto end;
   }
   if (objs[0]->Type() == Po_Imc3dsf) {
      Imc3dsf* const ims = (Imc3dsf*)objs[0];
      objd[0] = new Imc3dsf(ims->Props());
      Imc3dsf* const imd = (Imc3dsf*)objd[0];
      
      result = PLinearTransform(*ims, *imd, atoi(parv[0]), (float)atof(parv[1]), (float)atof(parv[2]));
      goto end;
   }
   if (objs[0]->Type() == Po_Imx2duc) {
      Imx2duc* const ims = (Imx2duc*)objs[0];
      objd[0] = new Imx2duc(ims->Props());
      Imx2duc* const imd = (Imx2duc*)objd[0];
      
      result = PLinearTransform(*ims, *imd, atoi(parv[0]), (float)atof(parv[1]), (float)atof(parv[2]));
      goto end;
   }
   if (objs[0]->Type() == Po_Imx2dsl) {
      Imx2dsl* const ims = (Imx2dsl*)objs[0];
      objd[0] = new Imx2dsl(ims->Props());
      Imx2dsl* const imd = (Imx2dsl*)objd[0];
      
      result = PLinearTransform(*ims, *imd, atoi(parv[0]), (float)atof(parv[1]), (float)atof(parv[2]));
      goto end;
   }
   if (objs[0]->Type() == Po_Imx2dsf) {
      Imx2dsf* const ims = (Imx2dsf*)objs[0];
      objd[0] = new Imx2dsf(ims->Props());
      Imx2dsf* const imd = (Imx2dsf*)objd[0];
      
      result = PLinearTransform(*ims, *imd, atoi(parv[0]), (float)atof(parv[1]), (float)atof(parv[2]));
      goto end;
   }
   if (objs[0]->Type() == Po_Imx3duc) {
      Imx3duc* const ims = (Imx3duc*)objs[0];
      objd[0] = new Imx3duc(ims->Props());
      Imx3duc* const imd = (Imx3duc*)objd[0];
      
      result = PLinearTransform(*ims, *imd, atoi(parv[0]), (float)atof(parv[1]), (float)atof(parv[2]));
      goto end;
   }
   if (objs[0]->Type() == Po_Imx3dsl) {
      Imx3dsl* const ims = (Imx3dsl*)objs[0];
      objd[0] = new Imx3dsl(ims->Props());
      Imx3dsl* const imd = (Imx3dsl*)objd[0];
      
      result = PLinearTransform(*ims, *imd, atoi(parv[0]), (float)atof(parv[1]), (float)atof(parv[2]));
      goto end;
   }
   if (objs[0]->Type() == Po_Imx3dsf) {
      Imx3dsf* const ims = (Imx3dsf*)objs[0];
      objd[0] = new Imx3dsf(ims->Props());
      Imx3dsf* const imd = (Imx3dsf*)objd[0];
      
      result = PLinearTransform(*ims, *imd, atoi(parv[0]), (float)atof(parv[1]), (float)atof(parv[2]));
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
