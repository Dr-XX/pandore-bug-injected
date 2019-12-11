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
 * @author Régis Clouard - 1997-07-22
 * @author Régis Clouard - 2003-06-04 (add demitaille).
 * @author Régis Clouard - Jul 09, 2011 (new parameters).
 */

#define _USE_MATH_DEFINES
#include <cmath>
#include <pandore.h>
using namespace pandore;

#include "morphology/structelt.h"

/**
 * @file plineardilatation.cpp
 *
 * Dilation based on a linear structuring element, 
 * with a given orientation and a given size.
 */
template <typename T>
Errc PLinearDilation( const Imx2d<T> &ims, Imx2d<T> &imd, int orientation1, int orientation2, const int halfsize ) {
   if (halfsize < 1) {
      std::cerr << "Error plineardilation: bad structuring element size: " << halfsize << std::endl;
      return FAILURE;
   }
   orientation1 = orientation1 % 360;

   StructuringElement2d *se = new StructuringElement2d(orientation1, orientation2, halfsize);

   Point2d p, p1;
   for (int b = 0; b < ims.Bands(); b++) {
      for (p.y = 0; p.y < ims.Height(); p.y++) {
	 for (p.x = 0; p.x < ims.Width(); p.x++) {
	    T max = Limits<T>::min(), val;
	    for (int i = 0; i < se->length; i++) {
	       p1 = p;
	       se->Move(i, p1);
	       if (ims.Hold(p1) && ((val = ims[b][p1]) > max)) {
		  max = val;
	       }
	    }
	    imd[b][p] = max;
	 }
      }
   }
   
   delete se;
   return SUCCESS;
}



template <typename T>
Errc PLinearDilation( const Imx3d<T> &ims, Imx3d<T> &imd, int orientation1, int orientation2, int halfsize ) {
   if (halfsize < 1) {
      std::cerr << "Error plineardilation: bad structuring element size: " << halfsize << std::endl;
      return FAILURE;
   }
   orientation1 = orientation1 % 360;
   orientation2 =orientation2 % 360;
   StructuringElement3d *se = new StructuringElement3d(orientation1, orientation2, halfsize);

   Point3d p, p1;
   for (int b = 0; b < ims.Bands(); b++) {
      for (p.z = 0; p.z < ims.Depth(); p.z++) {
	 for (p.y = 0; p.y < ims.Height(); p.y++) {
	    for (p.x = 0; p.x < ims.Width(); p.x++) {
	       T max = Limits<T>::min(), val;
	       for (int i = 0; i<se->length; i++) {
		  p1 = p;
		  se->Move(i, p1);
		  if (ims.Hold(p1) && ((val = ims[b][p1]) > max)) {
		     max = val;
		  }
	       }
	       imd[b][p] = max;
	    }
	 }
      }
   }
   delete se;
   return SUCCESS;
}


#ifndef __GREATER_H__
#define __GREATER_H__
/*
 * Lexicographic ordering between 2 colors (cx, cy, cz) and (cx1, cy1, cz1)
 */
template<typename T>
bool Greater(T cx, T cy, T cz, T cx1, T cy1, T cz1) {
   if (cx1 < cx) {
      return true;
   } else if (cx1 > cx) {
      return false;
   }
   if (cy1 < cy) {
      return true;
   } else if (cy1 > cy) {
      return false;
   }
   if (cz1 < cz) {
      return true;
   } else if (cz1 > cz) {
      return false;
   } else {
      return false;
   }
}
#endif

template <typename T>
Errc PLinearDilation( const Imc2d<T> &ims, Imc2d<T> &imd, int orientation1, int orientation2,  int halfsize ) {
   if (halfsize < 1) {
      std::cerr << "Error plineardilation: bad structuring element size: " << halfsize << std::endl;
      return FAILURE;
   }
   orientation1 = orientation1 % 360;

   StructuringElement2d *se = new StructuringElement2d(orientation1, orientation2, halfsize);

   Point2d p, p1;
   for (p.y = 0; p.y < ims.Height(); p.y++) {
      for (p.x = 0; p.x < ims.Width(); p.x++) {
 	 T maxX = Limits<T>::min();
 	 T maxY = Limits<T>::min();
 	 T maxZ = Limits<T>::min();
	 for (int i = 0; i < se->length; i++) {
	    p1 = p;
	    se->Move(i, p1);
	    if (ims.Hold(p1) && Greater(ims.X[p1], ims.Y[p1], ims.Z[p1], maxX, maxY, maxZ)) {
	       maxX = ims.X[p1];
	       maxY = ims.Y[p1];
	       maxZ = ims.Z[p1];
	    }
	 }
	 imd.X[p] = maxX;
	 imd.Y[p] = maxY;
	 imd.Z[p] = maxZ;
      }
   }

   delete se;
   return SUCCESS;
}


template <typename T>
Errc PLinearDilation( const Imc3d<T> &ims, Imc3d<T> &imd, int orientation1, int orientation2, int halfsize ) {
   if (halfsize < 1) {
      std::cerr << "Error plineardilation: bad structuring element size: " << halfsize << std::endl;
      return FAILURE;
   }
   orientation1 = orientation1 % 360;
   orientation2 = orientation2 % 360;
   
   StructuringElement3d *se = new StructuringElement3d(orientation1, orientation2, halfsize);
   
   Point3d p, p1;
   for (p.z = 0; p.z < ims.Depth(); p.z++) {
      for (p.y = 0; p.y < ims.Height(); p.y++) {
	 for (p.x = 0; p.x < ims.Width(); p.x++) {
	    T maxX = Limits<T>::min();
	    T maxY = Limits<T>::min();
	    T maxZ = Limits<T>::min();
	    for (int i = 0; i < se->length; i++) {
	       p1 = p;
	       se->Move(i, p1);
	       if (ims.Hold(p1) && Greater(ims.X[p1], ims.Y[p1], ims.Z[p1], maxX, maxY, maxZ)) {
		  maxX = ims.X[p1];
		  maxY = ims.Y[p1];
		  maxZ = ims.Z[p1];
	       }
	    }
	    imd.X[p] = maxX;
	    imd.Y[p] = maxY;
	    imd.Z[p] = maxZ;
	 }
      }
   }

   delete se;
   return SUCCESS;
}


Errc PLinearDilation( const Reg2d &ims, Reg2d &imd, int orientation1, int orientation2, int halfsize ) {
   if (halfsize < 1) {
      std::cerr << "Error plineardilation: bad structuring element size: " << halfsize << std::endl;
      return FAILURE;
   }
   orientation1 = orientation1 % 360;

   StructuringElement2d *se = new StructuringElement2d(orientation1, orientation2, halfsize);

   Point2d p, p1;
   for (p.y = 0; p.y < ims.Height(); p.y++) {
      for (p.x = 0; p.x < ims.Width(); p.x++) {
 	 Reg2d::ValueType max = Limits<Reg2d::ValueType>::min(), val;
	 for (int i = 0; i < se->length; i++) {
	    p1 = p;
	    se->Move(i, p1);
	    if (ims.Hold(p1) && ((val = ims[p1]) > max)) {
	       max = val;
	    }
	 }
	 imd[p] = max;
      }
   }
   imd.Labels(ims.Labels());
   delete se;
   return SUCCESS;
}


Errc PLinearDilation( const Reg3d &ims, Reg3d &imd, int orientation1, int orientation2, int halfsize ) {
   if (halfsize < 1) {
      std::cerr << "Error plineardilation: bad structuring element size: " << halfsize << std::endl;
      return FAILURE;
   }
   orientation1 = orientation1 % 360;
   orientation2 = orientation2 % 360;

   StructuringElement3d *se = new StructuringElement3d(orientation1, orientation2, halfsize);

   Point3d p, p1;
   for (p.z = 0; p.z < ims.Depth(); p.z++) {
      for (p.y = 0; p.y < ims.Height(); p.y++) {
	 for (p.x = 0; p.x < ims.Width(); p.x++) {
	    Reg3d::ValueType max = Limits<Reg3d::ValueType>::min(), val;
	    for (int i = 0; i < se->length; i++) {
	       p1 = p;
	       se->Move(i, p1);
	       if (ims.Hold(p1) && ((val = ims[p1]) > max)) {
		  max = val;
	       }
	    }
	    imd[p] = max;
	 }
      }
   }

   imd.Labels(ims.Labels());
   delete se;
   return SUCCESS;
}



#ifdef MAIN
#define USAGE	"usage: %s orientation1 orientation2 halfsize [-m mask] [im_in|-] [im_out|-]"
#define PARC	3
#define FINC	1
#define FOUTC	1
#define MASK	1

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
      
      result = PLinearDilation(*ims, *imd, atoi(parv[0]), atoi(parv[1]), atoi(parv[2]));
      goto end;
   }
   if (objs[0]->Type() == Po_Img2dsl) {
      Img2dsl* const ims = (Img2dsl*)objs[0];
      objd[0] = new Img2dsl(ims->Props());
      Img2dsl* const imd = (Img2dsl*)objd[0];
      
      result = PLinearDilation(*ims, *imd, atoi(parv[0]), atoi(parv[1]), atoi(parv[2]));
      goto end;
   }
   if (objs[0]->Type() == Po_Img2dsf) {
      Img2dsf* const ims = (Img2dsf*)objs[0];
      objd[0] = new Img2dsf(ims->Props());
      Img2dsf* const imd = (Img2dsf*)objd[0];
      
      result = PLinearDilation(*ims, *imd, atoi(parv[0]), atoi(parv[1]), atoi(parv[2]));
      goto end;
   }
   if (objs[0]->Type() == Po_Imx2duc) {
      Imx2duc* const ims = (Imx2duc*)objs[0];
      objd[0] = new Imx2duc(ims->Props());
      Imx2duc* const imd = (Imx2duc*)objd[0];
      
      result = PLinearDilation(*ims, *imd, atoi(parv[0]), atoi(parv[1]), atoi(parv[2]));
      goto end;
   }
   if (objs[0]->Type() == Po_Imx2dsl) {
      Imx2dsl* const ims = (Imx2dsl*)objs[0];
      objd[0] = new Imx2dsl(ims->Props());
      Imx2dsl* const imd = (Imx2dsl*)objd[0];
      
      result = PLinearDilation(*ims, *imd, atoi(parv[0]), atoi(parv[1]), atoi(parv[2]));
      goto end;
   }
   if (objs[0]->Type() == Po_Imx2dsf) {
      Imx2dsf* const ims = (Imx2dsf*)objs[0];
      objd[0] = new Imx2dsf(ims->Props());
      Imx2dsf* const imd = (Imx2dsf*)objd[0];
      
      result = PLinearDilation(*ims, *imd, atoi(parv[0]), atoi(parv[1]), atoi(parv[2]));
      goto end;
   }
   if (objs[0]->Type() == Po_Reg2d) {
      Reg2d* const ims = (Reg2d*)objs[0];
      objd[0] = new Reg2d(ims->Props());
      Reg2d* const imd = (Reg2d*)objd[0];
      
      result = PLinearDilation(*ims, *imd, atoi(parv[0]), atoi(parv[1]), atoi(parv[2]));
      goto end;
   }
   if (objs[0]->Type() == Po_Imc2duc) {
      Imc2duc* const ims = (Imc2duc*)objs[0];
      objd[0] = new Imc2duc(ims->Props());
      Imc2duc* const imd = (Imc2duc*)objd[0];
      
      result = PLinearDilation(*ims, *imd, atoi(parv[0]), atoi(parv[1]), atoi(parv[2]));
      goto end;
   }
   if (objs[0]->Type() == Po_Imc2dsl) {
      Imc2dsl* const ims = (Imc2dsl*)objs[0];
      objd[0] = new Imc2dsl(ims->Props());
      Imc2dsl* const imd = (Imc2dsl*)objd[0];
      
      result = PLinearDilation(*ims, *imd, atoi(parv[0]), atoi(parv[1]), atoi(parv[2]));
      goto end;
   }
   if (objs[0]->Type() == Po_Imc2dsf) {
      Imc2dsf* const ims = (Imc2dsf*)objs[0];
      objd[0] = new Imc2dsf(ims->Props());
      Imc2dsf* const imd = (Imc2dsf*)objd[0];
      
      result = PLinearDilation(*ims, *imd, atoi(parv[0]), atoi(parv[1]), atoi(parv[2]));
      goto end;
   }
   if (objs[0]->Type() == Po_Imc3duc) {
      Imc3duc* const ims = (Imc3duc*)objs[0];
      objd[0] = new Imc3duc(ims->Props());
      Imc3duc* const imd = (Imc3duc*)objd[0];
      
      result = PLinearDilation(*ims, *imd, atoi(parv[0]), atoi(parv[1]), atoi(parv[2]));
      goto end;
   }
   if (objs[0]->Type() == Po_Imc3dsl) {
      Imc3dsl* const ims = (Imc3dsl*)objs[0];
      objd[0] = new Imc3dsl(ims->Props());
      Imc3dsl* const imd = (Imc3dsl*)objd[0];
      
      result = PLinearDilation(*ims, *imd, atoi(parv[0]), atoi(parv[1]), atoi(parv[2]));
      goto end;
   }
   if (objs[0]->Type() == Po_Imc3dsf) {
      Imc3dsf* const ims = (Imc3dsf*)objs[0];
      objd[0] = new Imc3dsf(ims->Props());
      Imc3dsf* const imd = (Imc3dsf*)objd[0];
      
      result = PLinearDilation(*ims, *imd, atoi(parv[0]), atoi(parv[1]), atoi(parv[2]));
      goto end;
   }
   if (objs[0]->Type() == Po_Img3duc) {
      Img3duc* const ims = (Img3duc*)objs[0];
      objd[0] = new Img3duc(ims->Props());
      Img3duc* const imd = (Img3duc*)objd[0];
      
      result = PLinearDilation(*ims, *imd, atoi(parv[0]), atoi(parv[1]), atoi(parv[2]));
      goto end;
   }
   if (objs[0]->Type() == Po_Img3dsl) {
      Img3dsl* const ims = (Img3dsl*)objs[0];
      objd[0] = new Img3dsl(ims->Props());
      Img3dsl* const imd = (Img3dsl*)objd[0];
      
      result = PLinearDilation(*ims, *imd, atoi(parv[0]), atoi(parv[1]), atoi(parv[2]));
      goto end;
   }
   if (objs[0]->Type() == Po_Img3dsf) {
      Img3dsf* const ims = (Img3dsf*)objs[0];
      objd[0] = new Img3dsf(ims->Props());
      Img3dsf* const imd = (Img3dsf*)objd[0];
      
      result = PLinearDilation(*ims, *imd, atoi(parv[0]), atoi(parv[1]), atoi(parv[2]));
      goto end;
   }
   if (objs[0]->Type() == Po_Imx3duc) {
      Imx3duc* const ims = (Imx3duc*)objs[0];
      objd[0] = new Imx3duc(ims->Props());
      Imx3duc* const imd = (Imx3duc*)objd[0];
      
      result = PLinearDilation(*ims, *imd, atoi(parv[0]), atoi(parv[1]), atoi(parv[2]));
      goto end;
   }
   if (objs[0]->Type() == Po_Imx3dsl) {
      Imx3dsl* const ims = (Imx3dsl*)objs[0];
      objd[0] = new Imx3dsl(ims->Props());
      Imx3dsl* const imd = (Imx3dsl*)objd[0];
      
      result = PLinearDilation(*ims, *imd, atoi(parv[0]), atoi(parv[1]), atoi(parv[2]));
      goto end;
   }
   if (objs[0]->Type() == Po_Imx3dsf) {
      Imx3dsf* const ims = (Imx3dsf*)objs[0];
      objd[0] = new Imx3dsf(ims->Props());
      Imx3dsf* const imd = (Imx3dsf*)objd[0];
      
      result = PLinearDilation(*ims, *imd, atoi(parv[0]), atoi(parv[1]), atoi(parv[2]));
      goto end;
   }
   if (objs[0]->Type() == Po_Reg3d) {
      Reg3d* const ims = (Reg3d*)objs[0];
      objd[0] = new Reg3d(ims->Props());
      Reg3d* const imd = (Reg3d*)objd[0];
      
      result = PLinearDilation(*ims, *imd, atoi(parv[0]), atoi(parv[1]), atoi(parv[2]));
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
