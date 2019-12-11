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
 * @author Regis Clouard - 2003-05-23
 */

#define _USE_MATH_DEFINES
#include <math.h>
#include <pandore.h>
using namespace pandore;
#include "morphology/structelt.h"

/**
 * @file pseerosion.cpp
 * Morphological erosion with defined structuring element.
 * halfsize : halfsize of the structuring element.
 */



/*
 * Lexicographic order between 2 colors (cx, cy, cz) and (cx1, cy1, cz1)
 */
template<typename T>
static bool Lower(T cx1, T cy1, T cz1, T cx, T cy, T cz) {
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






Errc PSEErosion( const Img2duc &ims, const Img2duc &imse, Img2duc &imd, int size ) {
   if (size < 1) {
      std::cerr << "Error pseerosion: bad structuring element size:" << size << std::endl;
      return FAILURE;
   }

   Img2duc imi(ims.Props());
   StructuringElement2d *se = new StructuringElement2d(imse);

   Point2d p, p1;
   imi = ims;
   for (int i = 0; i < size; i++) {
      for (p.y = 0; p.y < imi.Height(); p.y++) {
	 for (p.x = 0; p.x < imi.Width(); p.x++) {
	    Img2duc::ValueType min = Limits<Img2duc::ValueType>::max(), val;
	    for (int i = 0; i < se->length; i++) {
 	       p1 = p;
 	       se->Move(i, p1);
	       if (imi.Hold(p1) && ((val = imi[p1]) < min)) {
		  min = val;
	       }
	    }
	    imd[p] = min;
	 }
      }
      imi = imd;
   }
   
   delete se;
   return SUCCESS;
}

Errc PSEErosion( const Img2dsl &ims, const Img2duc &imse, Img2dsl &imd, int size ) {
   if (size < 1) {
      std::cerr << "Error pseerosion: bad structuring element size:" << size << std::endl;
      return FAILURE;
   }

   Img2dsl imi(ims.Props());
   StructuringElement2d *se = new StructuringElement2d(imse);

   Point2d p, p1;
   imi = ims;
   for (int i = 0; i < size; i++) {
      for (p.y = 0; p.y < imi.Height(); p.y++) {
	 for (p.x = 0; p.x < imi.Width(); p.x++) {
	    Img2dsl::ValueType min = Limits<Img2dsl::ValueType>::max(), val;
	    for (int i = 0; i < se->length; i++) {
 	       p1 = p;
 	       se->Move(i, p1);
	       if (imi.Hold(p1) && ((val = imi[p1]) < min)) {
		  min = val;
	       }
	    }
	    imd[p] = min;
	 }
      }
      imi = imd;
   }
   
   delete se;
   return SUCCESS;
}

Errc PSEErosion( const Img2dsf &ims, const Img2duc &imse, Img2dsf &imd, int size ) {
   if (size < 1) {
      std::cerr << "Error pseerosion: bad structuring element size:" << size << std::endl;
      return FAILURE;
   }

   Img2dsf imi(ims.Props());
   StructuringElement2d *se = new StructuringElement2d(imse);

   Point2d p, p1;
   imi = ims;
   for (int i = 0; i < size; i++) {
      for (p.y = 0; p.y < imi.Height(); p.y++) {
	 for (p.x = 0; p.x < imi.Width(); p.x++) {
	    Img2dsf::ValueType min = Limits<Img2dsf::ValueType>::max(), val;
	    for (int i = 0; i < se->length; i++) {
 	       p1 = p;
 	       se->Move(i, p1);
	       if (imi.Hold(p1) && ((val = imi[p1]) < min)) {
		  min = val;
	       }
	    }
	    imd[p] = min;
	 }
      }
      imi = imd;
   }
   
   delete se;
   return SUCCESS;
}

Errc PSEErosion( const Img3duc &ims, const Img3duc &imse, Img3duc &imd, int size ) {
   if (size < 1) {
      std::cerr << "Error pseerosion: bad structuring element size:" << size << std::endl;
      return FAILURE;
   }

   StructuringElement3d *se = new StructuringElement3d(imse);
   Img3duc imi(ims.Props());

   Point3d p, p1;
   imi = ims;
   for (int i = 0; i < size; i++) {
      for (p.z = 0; p.z < ims.Depth(); p.z++) {
	 for (p.y = 0; p.y < ims.Height(); p.y++) {
	    for (p.x = 0; p.x < ims.Width(); p.x++) {
	       Img3duc::ValueType min = Limits<Img3duc::ValueType>::max(), val;
	       for (int i = 0; i < se->length; i++) {
		  p1 = p;
		  se->Move(i, p1);
		  if (ims.Hold(p1) && ((val = ims[p1]) < min)) {
		     min = val;
		  }
	       }
	       imd[p] = min;
	    }
	 }
      }

      imi = imd;
   }
   
   delete se;
   return SUCCESS;
}

Errc PSEErosion( const Img3dsl &ims, const Img3duc &imse, Img3dsl &imd, int size ) {
   if (size < 1) {
      std::cerr << "Error pseerosion: bad structuring element size:" << size << std::endl;
      return FAILURE;
   }

   StructuringElement3d *se = new StructuringElement3d(imse);
   Img3dsl imi(ims.Props());

   Point3d p, p1;
   imi = ims;
   for (int i = 0; i < size; i++) {
      for (p.z = 0; p.z < ims.Depth(); p.z++) {
	 for (p.y = 0; p.y < ims.Height(); p.y++) {
	    for (p.x = 0; p.x < ims.Width(); p.x++) {
	       Img3dsl::ValueType min = Limits<Img3dsl::ValueType>::max(), val;
	       for (int i = 0; i < se->length; i++) {
		  p1 = p;
		  se->Move(i, p1);
		  if (ims.Hold(p1) && ((val = ims[p1]) < min)) {
		     min = val;
		  }
	       }
	       imd[p] = min;
	    }
	 }
      }

      imi = imd;
   }
   
   delete se;
   return SUCCESS;
}

Errc PSEErosion( const Img3dsf &ims, const Img3duc &imse, Img3dsf &imd, int size ) {
   if (size < 1) {
      std::cerr << "Error pseerosion: bad structuring element size:" << size << std::endl;
      return FAILURE;
   }

   StructuringElement3d *se = new StructuringElement3d(imse);
   Img3dsf imi(ims.Props());

   Point3d p, p1;
   imi = ims;
   for (int i = 0; i < size; i++) {
      for (p.z = 0; p.z < ims.Depth(); p.z++) {
	 for (p.y = 0; p.y < ims.Height(); p.y++) {
	    for (p.x = 0; p.x < ims.Width(); p.x++) {
	       Img3dsf::ValueType min = Limits<Img3dsf::ValueType>::max(), val;
	       for (int i = 0; i < se->length; i++) {
		  p1 = p;
		  se->Move(i, p1);
		  if (ims.Hold(p1) && ((val = ims[p1]) < min)) {
		     min = val;
		  }
	       }
	       imd[p] = min;
	    }
	 }
      }

      imi = imd;
   }
   
   delete se;
   return SUCCESS;
}

Errc PSEErosion( const Imc2duc &ims, const Img2duc &imse, Imc2duc &imd, int size ) {
   if (size < 1) {
      std::cerr << "Error pseerosion: bad structuring element size:" << size << std::endl;
      return FAILURE;
   }

   StructuringElement2d *se = new StructuringElement2d(imse);
   Imc2duc imi(ims.Props());

   Point2d p, p1;
   imi = ims;
   for (int i = 0; i < size; i++) {
      for (p.y=0; p.y<ims.Height(); p.y++) {
	 for (p.x=0; p.x<ims.Width(); p.x++) {
	    Imc2duc::ValueType minX = Limits<Imc2duc::ValueType>::max();
	    Imc2duc::ValueType minY = Limits<Imc2duc::ValueType>::max();
	    Imc2duc::ValueType minZ = Limits<Imc2duc::ValueType>::max();
	    for (int i = 0; i < se->length; i++) {
 	       p1 = p;
 	       se->Move(i, p1);
	       if (ims.Hold(p1) && Lower(ims.X[p1], ims.Y[p1], ims.Z[p1], minX, minY, minZ)) {
		  minX = ims.X[p1];
		  minY = ims.Y[p1];
		  minZ = ims.Z[p1];
	       }
	    }
	    imd.X[p] = minX;
	    imd.Y[p] = minY;
	    imd.Z[p] = minZ;
	 }
      }

      imi = imd;
   }

   delete se;
   return SUCCESS;
}

Errc PSEErosion( const Imc2dsl &ims, const Img2duc &imse, Imc2dsl &imd, int size ) {
   if (size < 1) {
      std::cerr << "Error pseerosion: bad structuring element size:" << size << std::endl;
      return FAILURE;
   }

   StructuringElement2d *se = new StructuringElement2d(imse);
   Imc2dsl imi(ims.Props());

   Point2d p, p1;
   imi = ims;
   for (int i = 0; i < size; i++) {
      for (p.y=0; p.y<ims.Height(); p.y++) {
	 for (p.x=0; p.x<ims.Width(); p.x++) {
	    Imc2dsl::ValueType minX = Limits<Imc2dsl::ValueType>::max();
	    Imc2dsl::ValueType minY = Limits<Imc2dsl::ValueType>::max();
	    Imc2dsl::ValueType minZ = Limits<Imc2dsl::ValueType>::max();
	    for (int i = 0; i < se->length; i++) {
 	       p1 = p;
 	       se->Move(i, p1);
	       if (ims.Hold(p1) && Lower(ims.X[p1], ims.Y[p1], ims.Z[p1], minX, minY, minZ)) {
		  minX = ims.X[p1];
		  minY = ims.Y[p1];
		  minZ = ims.Z[p1];
	       }
	    }
	    imd.X[p] = minX;
	    imd.Y[p] = minY;
	    imd.Z[p] = minZ;
	 }
      }

      imi = imd;
   }

   delete se;
   return SUCCESS;
}

Errc PSEErosion( const Imc2dsf &ims, const Img2duc &imse, Imc2dsf &imd, int size ) {
   if (size < 1) {
      std::cerr << "Error pseerosion: bad structuring element size:" << size << std::endl;
      return FAILURE;
   }

   StructuringElement2d *se = new StructuringElement2d(imse);
   Imc2dsf imi(ims.Props());

   Point2d p, p1;
   imi = ims;
   for (int i = 0; i < size; i++) {
      for (p.y=0; p.y<ims.Height(); p.y++) {
	 for (p.x=0; p.x<ims.Width(); p.x++) {
	    Imc2dsf::ValueType minX = Limits<Imc2dsf::ValueType>::max();
	    Imc2dsf::ValueType minY = Limits<Imc2dsf::ValueType>::max();
	    Imc2dsf::ValueType minZ = Limits<Imc2dsf::ValueType>::max();
	    for (int i = 0; i < se->length; i++) {
 	       p1 = p;
 	       se->Move(i, p1);
	       if (ims.Hold(p1) && Lower(ims.X[p1], ims.Y[p1], ims.Z[p1], minX, minY, minZ)) {
		  minX = ims.X[p1];
		  minY = ims.Y[p1];
		  minZ = ims.Z[p1];
	       }
	    }
	    imd.X[p] = minX;
	    imd.Y[p] = minY;
	    imd.Z[p] = minZ;
	 }
      }

      imi = imd;
   }

   delete se;
   return SUCCESS;
}

Errc PSEErosion( const Imc3duc &ims, const Img3duc &imse, Imc3duc &imd, int size ) {
   if (size < 1) {
      std::cerr << "Error pseerosion: bad structuring element size:" << size << std::endl;
      return FAILURE;
   }

   StructuringElement3d *se = new StructuringElement3d(imse);
   Imc3duc imi(ims.Props());

   Point3d p, p1;
   imi = ims;
   for (int i = 0; i < size; i++) {
      for (p.z = 0; p.z < ims.Depth(); p.z++) {
	 for (p.y = 0; p.y < ims.Height(); p.y++) {
	    for (p.x = 0; p.x < ims.Width(); p.x++) {
	       Imc3duc::ValueType minX = Limits<Imc3duc::ValueType>::max();
	       Imc3duc::ValueType minY = Limits<Imc3duc::ValueType>::max();
	       Imc3duc::ValueType minZ = Limits<Imc3duc::ValueType>::max();
	       for (int i = 0; i < se->length; i++) {
		  p1 = p;
		  se->Move(i, p1);
		  if (ims.Hold(p1) && Lower(ims.X[p1], ims.Y[p1], ims.Z[p1], minX, minY, minZ)) {
		     minX = ims.X[p1];
		     minY = ims.Y[p1];
		     minZ = ims.Z[p1];
		  }
	       }
	       imd.X[p] = minX;
	       imd.Y[p] = minY;
	       imd.Z[p] = minZ;
	    }
	 }
      }

      imi = imd;
   }

   delete se;
   return SUCCESS;
}

Errc PSEErosion( const Imc3dsl &ims, const Img3duc &imse, Imc3dsl &imd, int size ) {
   if (size < 1) {
      std::cerr << "Error pseerosion: bad structuring element size:" << size << std::endl;
      return FAILURE;
   }

   StructuringElement3d *se = new StructuringElement3d(imse);
   Imc3dsl imi(ims.Props());

   Point3d p, p1;
   imi = ims;
   for (int i = 0; i < size; i++) {
      for (p.z = 0; p.z < ims.Depth(); p.z++) {
	 for (p.y = 0; p.y < ims.Height(); p.y++) {
	    for (p.x = 0; p.x < ims.Width(); p.x++) {
	       Imc3dsl::ValueType minX = Limits<Imc3dsl::ValueType>::max();
	       Imc3dsl::ValueType minY = Limits<Imc3dsl::ValueType>::max();
	       Imc3dsl::ValueType minZ = Limits<Imc3dsl::ValueType>::max();
	       for (int i = 0; i < se->length; i++) {
		  p1 = p;
		  se->Move(i, p1);
		  if (ims.Hold(p1) && Lower(ims.X[p1], ims.Y[p1], ims.Z[p1], minX, minY, minZ)) {
		     minX = ims.X[p1];
		     minY = ims.Y[p1];
		     minZ = ims.Z[p1];
		  }
	       }
	       imd.X[p] = minX;
	       imd.Y[p] = minY;
	       imd.Z[p] = minZ;
	    }
	 }
      }

      imi = imd;
   }

   delete se;
   return SUCCESS;
}

Errc PSEErosion( const Imc3dsf &ims, const Img3duc &imse, Imc3dsf &imd, int size ) {
   if (size < 1) {
      std::cerr << "Error pseerosion: bad structuring element size:" << size << std::endl;
      return FAILURE;
   }

   StructuringElement3d *se = new StructuringElement3d(imse);
   Imc3dsf imi(ims.Props());

   Point3d p, p1;
   imi = ims;
   for (int i = 0; i < size; i++) {
      for (p.z = 0; p.z < ims.Depth(); p.z++) {
	 for (p.y = 0; p.y < ims.Height(); p.y++) {
	    for (p.x = 0; p.x < ims.Width(); p.x++) {
	       Imc3dsf::ValueType minX = Limits<Imc3dsf::ValueType>::max();
	       Imc3dsf::ValueType minY = Limits<Imc3dsf::ValueType>::max();
	       Imc3dsf::ValueType minZ = Limits<Imc3dsf::ValueType>::max();
	       for (int i = 0; i < se->length; i++) {
		  p1 = p;
		  se->Move(i, p1);
		  if (ims.Hold(p1) && Lower(ims.X[p1], ims.Y[p1], ims.Z[p1], minX, minY, minZ)) {
		     minX = ims.X[p1];
		     minY = ims.Y[p1];
		     minZ = ims.Z[p1];
		  }
	       }
	       imd.X[p] = minX;
	       imd.Y[p] = minY;
	       imd.Z[p] = minZ;
	    }
	 }
      }

      imi = imd;
   }

   delete se;
   return SUCCESS;
}

Errc PSEErosion( const Reg2d &ims, const Img2duc &imse, Reg2d &imd, int size ) {
   if (size < 1) {
      std::cerr << "Error pseerosion: bad structuring element size:" << size << std::endl;
      return FAILURE;
   }

   StructuringElement2d *se = new StructuringElement2d(imse);
   Reg2d imi(ims.Props());

   Point2d p, p1;
   imi = ims;
   for (int i = 0; i < size; i++) {
      for (p.y  =0; p.y < ims.Height(); p.y++) {
	 for (p.x = 0; p.x < ims.Width(); p.x++) {
	    Reg2d::ValueType min = Limits<Reg2d::ValueType>::max(), val;
	    for (int i = 0; i < se->length; i++) {
 	       p1 = p;
 	       se->Move(i, p1);
	       if (ims.Hold(p1) && ((val = ims[p1]) < min)) {
		  min = val;
	       }
	    }
	    imd[p] = min;
	 }
      }
      imi = imd;
   }
   
   delete se;
   return SUCCESS;
}

Errc PSEErosion( const Reg3d &ims, const Img3duc &imse, Reg3d &imd, int size ) {
   if (size < 1) {
      std::cerr << "Error pseerosion: bad structuring element size:" << size << std::endl;
      return FAILURE;
   }

   StructuringElement3d *se = new StructuringElement3d(imse);
   Reg3d imi(ims.Props());

   Point3d p, p1;
   imi = ims;
   for (int i = 0; i < size; i++) {
      for (p.z = 0; p.z < ims.Depth(); p.z++) {
	 for (p.y = 0; p.y < ims.Height(); p.y++) {
	    for (p.x = 0; p.x < ims.Width(); p.x++) {
	       Reg3d::ValueType min = Limits<Reg3d::ValueType>::max(), val;
	       for (int i = 0; i < se->length; i++) {
		  p1 = p;
		  se->Move(i, p1);
		  if (ims.Hold(p1) && ((val = ims[p1]) < min)) {
		     min = val;
		  }
	       }
	       imd[p] = min;
	    }
	 }
      }

      imi = imd;
   }

   delete se;
   return SUCCESS;
}


#ifdef MAIN
#define	USAGE	"usage: %s iteration [-m mask] [im_in|-] [im_se|-] [im_out|-]"
#define	PARC	1
#define	FINC	2
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
   if (objs[1]->Type() == Po_Img2duc) {
      Img2duc* const ims = (Img2duc*)objs[1];
      Img2duc* const imse = (Img2duc*)objs[0];
      objd[0] = new Img2duc(ims->Props());
      Img2duc* const imd = (Img2duc*)objd[0];
      
      result = PSEErosion(*ims, *imse, *imd, atoi(parv[0]));
      goto end;
   }
   if (objs[1]->Type() == Po_Img2dsl) {
      Img2dsl* const ims = (Img2dsl*)objs[1];
      Img2duc* const imse = (Img2duc*)objs[0];
      objd[0] = new Img2dsl(ims->Props());
      Img2dsl* const imd = (Img2dsl*)objd[0];
      
      result = PSEErosion(*ims, *imse, *imd, atoi(parv[0]));
      goto end;
   }
   if (objs[1]->Type() == Po_Img2dsf) {
      Img2dsf* const ims = (Img2dsf*)objs[1];
      Img2duc* const imse = (Img2duc*)objs[0];
      objd[0] = new Img2dsf(ims->Props());
      Img2dsf* const imd = (Img2dsf*)objd[0];
      
      result = PSEErosion(*ims, *imse, *imd, atoi(parv[0]));
      goto end;
   }
   if (objs[1]->Type() == Po_Img3duc) {
      Img3duc* const ims = (Img3duc*)objs[1];
      Img3duc* const imse = (Img3duc*)objs[0];
      objd[0] = new Img3duc(ims->Props());
      Img3duc* const imd = (Img3duc*)objd[0];
      
      result = PSEErosion(*ims, *imse, *imd, atoi(parv[0]));
      goto end;
   }
   if (objs[1]->Type() == Po_Img3dsl) {
      Img3dsl* const ims = (Img3dsl*)objs[1];
      Img3duc* const imse = (Img3duc*)objs[0];
      objd[0] = new Img3dsl(ims->Props());
      Img3dsl* const imd = (Img3dsl*)objd[0];
      
      result = PSEErosion(*ims, *imse, *imd, atoi(parv[0]));
      goto end;
   }
   if (objs[1]->Type() == Po_Img3dsf) {
      Img3dsf* const ims = (Img3dsf*)objs[1];
      Img3duc* const imse = (Img3duc*)objs[0];
      objd[0] = new Img3dsf(ims->Props());
      Img3dsf* const imd = (Img3dsf*)objd[0];
      
      result = PSEErosion(*ims, *imse, *imd, atoi(parv[0]));
      goto end;
   }
   if (objs[1]->Type() == Po_Imc2duc) {
      Imc2duc* const ims = (Imc2duc*)objs[1];
      Img2duc* const imse = (Img2duc*)objs[0];
      objd[0] = new Imc2duc(ims->Props());
      Imc2duc* const imd = (Imc2duc*)objd[0];
      
      result = PSEErosion(*ims, *imse, *imd, atoi(parv[0]));
      goto end;
   }
   if (objs[1]->Type() == Po_Imc2dsl) {
      Imc2dsl* const ims = (Imc2dsl*)objs[1];
      Img2duc* const imse = (Img2duc*)objs[0];
      objd[0] = new Imc2dsl(ims->Props());
      Imc2dsl* const imd = (Imc2dsl*)objd[0];
      
      result = PSEErosion(*ims, *imse, *imd, atoi(parv[0]));
      goto end;
   }
   if (objs[1]->Type() == Po_Imc2dsf) {
      Imc2dsf* const ims = (Imc2dsf*)objs[1];
      Img2duc* const imse = (Img2duc*)objs[0];
      objd[0] = new Imc2dsf(ims->Props());
      Imc2dsf* const imd = (Imc2dsf*)objd[0];
      
      result = PSEErosion(*ims, *imse, *imd, atoi(parv[0]));
      goto end;
   }
   if (objs[1]->Type() == Po_Imc3duc) {
      Imc3duc* const ims = (Imc3duc*)objs[1];
      Img3duc* const imse = (Img3duc*)objs[0];
      objd[0] = new Imc3duc(ims->Props());
      Imc3duc* const imd = (Imc3duc*)objd[0];
      
      result = PSEErosion(*ims, *imse, *imd, atoi(parv[0]));
      goto end;
   }
   if (objs[1]->Type() == Po_Imc3dsl) {
      Imc3dsl* const ims = (Imc3dsl*)objs[1];
      Img3duc* const imse = (Img3duc*)objs[0];
      objd[0] = new Imc3dsl(ims->Props());
      Imc3dsl* const imd = (Imc3dsl*)objd[0];
      
      result = PSEErosion(*ims, *imse, *imd, atoi(parv[0]));
      goto end;
   }
   if (objs[1]->Type() == Po_Imc3dsf) {
      Imc3dsf* const ims = (Imc3dsf*)objs[1];
      Img3duc* const imse = (Img3duc*)objs[0];
      objd[0] = new Imc3dsf(ims->Props());
      Imc3dsf* const imd = (Imc3dsf*)objd[0];
      
      result = PSEErosion(*ims, *imse, *imd, atoi(parv[0]));
      goto end;
   }
   if (objs[1]->Type() == Po_Reg2d) {
      Reg2d* const ims = (Reg2d*)objs[1];
      Img2duc* const imse = (Img2duc*)objs[0];
      objd[0] = new Reg2d(ims->Props());
      Reg2d* const imd = (Reg2d*)objd[0];
      
      result = PSEErosion(*ims, *imse, *imd, atoi(parv[0]));
      goto end;
   }
   if (objs[1]->Type() == Po_Reg3d) {
      Reg3d* const ims = (Reg3d*)objs[1];
      Img3duc* const imse = (Img3duc*)objs[0];
      objd[0] = new Reg3d(ims->Props());
      Reg3d* const imd = (Reg3d*)objd[0];
      
      result = PSEErosion(*ims, *imse, *imd, atoi(parv[0]));
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
