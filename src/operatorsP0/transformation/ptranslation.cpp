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
 * @author Regis Clouard - 1998-07-01
 */

#include <pandore.h>
using namespace pandore;

template <typename T>
Errc PTranslation( const Imx3d<T> &ims, Imx3d<T> &imd, Uchar direction, Long offset ) {
   Point3d p;

   switch(direction) {
   case 0:
      if (offset > 0) {
	 for (p.z = 0; p.z < imd.Depth(); p.z++) {
	    for (p.y = 0; p.y < imd.Height(); p.y++) {
	       for (p.x = 0; p.x < offset; p.x++) {
		  for (int b = 0; b < ims.Bands(); b++) {
		     imd[b][p] = 0;
		  }
	       }
	       for (p.x = offset; p.x < imd.Width(); p.x++) {
		  for (int b = 0; b < ims.Bands(); b++) {
		     imd[b][p] = ims[b][p.z][p.y][p.x-offset];
		  }
	       }
	    }
	 }
      } else {
	 for (p.z = 0; p.z < imd.Depth(); p.z++) {
	    for (p.y = 0; p.y < imd.Height(); p.y++) {
	       for (p.x = 0; p.x < imd.Width()+offset; p.x++) {
		  for (int b = 0; b < ims.Bands(); b++) {
		     imd[b][p] = ims[b][p.z][p.y][p.x-offset];
		  }
	       }
	       for (p.x = imd.Width()+offset; p.x < imd.Width(); p.x++) {
		  for (int b = 0; b < ims.Bands(); b++) {
		     imd[b][p] = 0;
		  }
	       }
	    }
	 }
      }
      break;
      // y axis
   case 1:
      if (offset > 0) {
	 for (p.z = 0; p.z < imd.Depth(); p.z++) {
	    for (p.x = 0; p.x < imd.Width(); p.x++) {
	       for (p.y = 0; p.y < offset; p.y++) {
		  for (int b = 0; b < ims.Bands(); b++) {
		     imd[b][p] = 0;
		  }
	       }
	       for (p.y = offset; p.y < imd.Height(); p.y++) {
		  for (int b = 0; b < ims.Bands(); b++) {
		     imd[b][p] = ims[b][p.z][p.y-offset][p.x];
		  }
	       }
	    }
	 }
      } else {
	 for (p.z = 0; p.z < imd.Depth(); p.z++) {
	    for (p.x = 0; p.x < imd.Width(); p.x++) {
	       for (p.y = 0; p.y < imd.Height()+offset; p.y++) {
		  for (int b = 0; b < ims.Bands(); b++) {
		     imd[b][p] = ims[b][p.z][p.y-offset][p.x];
		  }
	       }
	       for (p.y = imd.Height()+offset; p.y < imd.Height(); p.y++) {
		  for (int b = 0; b < ims.Bands(); b++) {
		     imd[b][p] = 0;
		  }
	       }
	    }
	 }
      }
      break;
   case 2:
      if (offset > 0) {
	 for (p.x = 0; p.x < imd.Width(); p.x++) {
	    for (p.y = 0; p.y < imd.Height(); p.y++) {
	       for (p.z = 0; p.z < offset; p.z++) {
		  for (int b = 0; b < ims.Bands(); b++) {
		     imd[b][p] = 0;
		  }
	       }
	       for (p.z = offset; p.z < imd.Depth(); p.z++) {
		  for (int b = 0; b < ims.Bands(); b++) {
		     imd[b][p] = ims[b][p.z-offset][p.y][p.x];
		  }
	       }
	    }
	 }
      } else {
	 for (p.x = 0; p.x < imd.Width(); p.x++) {
	    for (p.y = 0; p.y < imd.Height(); p.y++) {
	       for (p.z = 0; p.z < imd.Depth()+offset; p.z++) {
		  for (int b = 0; b < ims.Bands(); b++) {
		     imd[b][p] = ims[b][p.z-offset][p.y][p.x];
		  }
	       }
	       for (p.z = imd.Depth()+offset; p.z < imd.Depth(); p.z++) {
		  for (int b = 0; b < ims.Bands(); b++) {
		     imd[b][p] = 0;
		  }
	       }
	    }
	 }
      }
      break;
   default: return FAILURE;
   }
   
   return SUCCESS;
}

/**
 * @file ptranslation.cpp
 * Construit le translate d'une image.
 */



#ifdef MAIN
#define	USAGE	"usage: %s direction offset [-m mask] [im_in|-] [im_out|-]"
#define	PARC	2
#define	FINC	1
#define	FOUTC	1
#define	MASK	1

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
      Img1duc* const rgs = (Img1duc*)objs[0];
      objd[0] = new Img1duc(rgs->Props());
      Img1duc* const rgd = (Img1duc*)objd[0];
      
      result = PTranslation(*rgs, *rgd, (Uchar)atoi(parv[0]), atol(parv[1]));
      goto end;
   }
   if (objs[0]->Type() == Po_Img1dsl) {
      Img1dsl* const rgs = (Img1dsl*)objs[0];
      objd[0] = new Img1dsl(rgs->Props());
      Img1dsl* const rgd = (Img1dsl*)objd[0];
      
      result = PTranslation(*rgs, *rgd, (Uchar)atoi(parv[0]), atol(parv[1]));
      goto end;
   }
   if (objs[0]->Type() == Po_Img1dsf) {
      Img1dsf* const rgs = (Img1dsf*)objs[0];
      objd[0] = new Img1dsf(rgs->Props());
      Img1dsf* const rgd = (Img1dsf*)objd[0];
      
      result = PTranslation(*rgs, *rgd, (Uchar)atoi(parv[0]), atol(parv[1]));
      goto end;
   }
   if (objs[0]->Type() == Po_Img2duc) {
      Img2duc* const rgs = (Img2duc*)objs[0];
      objd[0] = new Img2duc(rgs->Props());
      Img2duc* const rgd = (Img2duc*)objd[0];
      
      result = PTranslation(*rgs, *rgd, (Uchar)atoi(parv[0]), atol(parv[1]));
      goto end;
   }
   if (objs[0]->Type() == Po_Img2dsl) {
      Img2dsl* const rgs = (Img2dsl*)objs[0];
      objd[0] = new Img2dsl(rgs->Props());
      Img2dsl* const rgd = (Img2dsl*)objd[0];
      
      result = PTranslation(*rgs, *rgd, (Uchar)atoi(parv[0]), atol(parv[1]));
      goto end;
   }
   if (objs[0]->Type() == Po_Img2dsf) {
      Img2dsf* const rgs = (Img2dsf*)objs[0];
      objd[0] = new Img2dsf(rgs->Props());
      Img2dsf* const rgd = (Img2dsf*)objd[0];
      
      result = PTranslation(*rgs, *rgd, (Uchar)atoi(parv[0]), atol(parv[1]));
      goto end;
   }
   if (objs[0]->Type() == Po_Img3duc) {
      Img3duc* const rgs = (Img3duc*)objs[0];
      objd[0] = new Img3duc(rgs->Props());
      Img3duc* const rgd = (Img3duc*)objd[0];
      
      result = PTranslation(*rgs, *rgd, (Uchar)atoi(parv[0]), atol(parv[1]));
      goto end;
   }
   if (objs[0]->Type() == Po_Img3dsl) {
      Img3dsl* const rgs = (Img3dsl*)objs[0];
      objd[0] = new Img3dsl(rgs->Props());
      Img3dsl* const rgd = (Img3dsl*)objd[0];
      
      result = PTranslation(*rgs, *rgd, (Uchar)atoi(parv[0]), atol(parv[1]));
      goto end;
   }
   if (objs[0]->Type() == Po_Img3dsf) {
      Img3dsf* const rgs = (Img3dsf*)objs[0];
      objd[0] = new Img3dsf(rgs->Props());
      Img3dsf* const rgd = (Img3dsf*)objd[0];
      
      result = PTranslation(*rgs, *rgd, (Uchar)atoi(parv[0]), atol(parv[1]));
      goto end;
   }
   if (objs[0]->Type() == Po_Reg1d) {
      Reg1d* const rgs = (Reg1d*)objs[0];
      objd[0] = new Reg1d(rgs->Props());
      Reg1d* const rgd = (Reg1d*)objd[0];
      
      result = PTranslation(*rgs, *rgd, (Uchar)atoi(parv[0]), atol(parv[1]));
      goto end;
   }
   if (objs[0]->Type() == Po_Reg2d) {
      Reg2d* const rgs = (Reg2d*)objs[0];
      objd[0] = new Reg2d(rgs->Props());
      Reg2d* const rgd = (Reg2d*)objd[0];
      
      result = PTranslation(*rgs, *rgd, (Uchar)atoi(parv[0]), atol(parv[1]));
      goto end;
   }
   if (objs[0]->Type() == Po_Reg3d) {
      Reg3d* const rgs = (Reg3d*)objs[0];
      objd[0] = new Reg3d(rgs->Props());
      Reg3d* const rgd = (Reg3d*)objd[0];
      
      result = PTranslation(*rgs, *rgd, (Uchar)atoi(parv[0]), atol(parv[1]));
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
