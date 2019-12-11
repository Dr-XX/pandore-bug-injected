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
 * @author Francois Angot - 1997-07-08
 * @author Regis Clouard - 2002-06-07
 */

#include <pandore.h>
using namespace pandore;

/**
 * @file pmeanprojection.cpp
 *
 */
#define PLANX 0
#define PLANY 1
#define PLANZ 2



Errc PMeanProjection( const Img2duc &ims, Img1d<Img2duc::ValueType> &imd, int plan ) {
   Point2d p;
   double somme;
   Long nb;
   
   switch(plan) {
      case PLANY:
	 imd.New(ims.Width());
	 for (p.x = 0; p.x < ims.Width(); p.x++) {
	    somme = 0.0; nb = 0;
	    for (p.y = 0; p.y < ims.Height(); p.y++) {
	       somme += ims[p];
	       nb++;
	    }
	    if (nb) {
	       imd[p.x] = (Img2duc::ValueType)(somme/nb);
	    } else {
	       imd[p.x] = 0;
	    }
	 }
	 break;
      case PLANX:
	 imd.New(ims.Height());
	 for (p.y = 0; p.y < ims.Height(); p.y++) {
	    somme = 0.0; nb = 0;
	    for (p.x = 0; p.x < ims.Width(); p.x++) {
	       somme += ims[p];
	       nb++;
	    }
	    if (nb) {
	       imd[p.y] = (Img2duc::ValueType)(somme/nb);
	    } else {
	       imd[p.y] = 0;
	    }
	 }
	 break;
      default: {
	 std::cerr << "PMeanProject: bad parameter value: "<< plan <<std::endl;
	 return FAILURE;
      }
   }
   
   return SUCCESS;
}

Errc PMeanProjection( const Img2dsl &ims, Img1d<Img2dsl::ValueType> &imd, int plan ) {
   Point2d p;
   double somme;
   Long nb;
   
   switch(plan) {
      case PLANY:
	 imd.New(ims.Width());
	 for (p.x = 0; p.x < ims.Width(); p.x++) {
	    somme = 0.0; nb = 0;
	    for (p.y = 0; p.y < ims.Height(); p.y++) {
	       somme += ims[p];
	       nb++;
	    }
	    if (nb) {
	       imd[p.x] = (Img2dsl::ValueType)(somme/nb);
	    } else {
	       imd[p.x] = 0;
	    }
	 }
	 break;
      case PLANX:
	 imd.New(ims.Height());
	 for (p.y = 0; p.y < ims.Height(); p.y++) {
	    somme = 0.0; nb = 0;
	    for (p.x = 0; p.x < ims.Width(); p.x++) {
	       somme += ims[p];
	       nb++;
	    }
	    if (nb) {
	       imd[p.y] = (Img2dsl::ValueType)(somme/nb);
	    } else {
	       imd[p.y] = 0;
	    }
	 }
	 break;
      default: {
	 std::cerr << "PMeanProject: bad parameter value: "<< plan <<std::endl;
	 return FAILURE;
      }
   }
   
   return SUCCESS;
}

Errc PMeanProjection( const Img2dsf &ims, Img1d<Img2dsf::ValueType> &imd, int plan ) {
   Point2d p;
   double somme;
   Long nb;
   
   switch(plan) {
      case PLANY:
	 imd.New(ims.Width());
	 for (p.x = 0; p.x < ims.Width(); p.x++) {
	    somme = 0.0; nb = 0;
	    for (p.y = 0; p.y < ims.Height(); p.y++) {
	       somme += ims[p];
	       nb++;
	    }
	    if (nb) {
	       imd[p.x] = (Img2dsf::ValueType)(somme/nb);
	    } else {
	       imd[p.x] = 0;
	    }
	 }
	 break;
      case PLANX:
	 imd.New(ims.Height());
	 for (p.y = 0; p.y < ims.Height(); p.y++) {
	    somme = 0.0; nb = 0;
	    for (p.x = 0; p.x < ims.Width(); p.x++) {
	       somme += ims[p];
	       nb++;
	    }
	    if (nb) {
	       imd[p.y] = (Img2dsf::ValueType)(somme/nb);
	    } else {
	       imd[p.y] = 0;
	    }
	 }
	 break;
      default: {
	 std::cerr << "PMeanProject: bad parameter value: "<< plan <<std::endl;
	 return FAILURE;
      }
   }
   
   return SUCCESS;
}

Errc PMeanProjection( const Img3duc &ims, Img2d<Img3duc::ValueType> &imd, int plan ) {
   Point3d p;
   double somme;
   Long nb;
   
   switch(plan) {
   case PLANZ:
      imd.New(ims.Height(), ims.Width());
      for (p.y = 0; p.y < ims.Height(); p.y++) {
	 for (p.x = 0; p.x < ims.Width(); p.x++) {
	    somme = 0.0; nb = 0;
	    for (p.z = 0; p.z < ims.Depth(); p.z++) {
	       somme += ims[p];
	       nb++;
	    }
	    if (nb) {
	       imd[p.y][p.x] = (Img3duc::ValueType)(somme/nb);
	    } else {
	       imd[p.y][p.x] = 0;
	    }
	 }
      }
      break;
   case PLANY:
      imd.New(ims.Depth(), ims.Width());
      for (p.z = 0; p.z < ims.Depth(); p.z++) {
	 for (p.x = 0; p.x < ims.Width(); p.x++) {
	    somme = 0.0; nb = 0;
	    for (p.y = 0; p.y < ims.Height(); p.y++) {
	       somme += ims[p];
	       nb++;
	    }
	    if (nb) {
	       imd[p.z][p.x] = (Img3duc::ValueType)(somme/nb);
	    } else {
	       imd[p.z][p.x] = 0;
	    }
	 }
      }
      break;
   case PLANX:
      imd.New(ims.Depth(), ims.Height());
      for (p.z = 0;p.z < ims.Depth(); p.z++) {
	 for (p.y = 0; p.y < ims.Height(); p.y++) {
	    somme = 0.0; nb = 0;
	    for (p.x = 0; p.x < ims.Width(); p.x++) {
	       somme += ims[p];
	       nb++;
	    }
	    if (nb) {
	       imd[p.z][p.y] = (Img3duc::ValueType)(somme/nb);
	    } else {
	       imd[p.z][p.y] = 0;
	    }
	 }
      }
      break;
      default: {
	 std::cerr << "PMeanProject: bad parameter value: " << plan << std::endl;
	 return FAILURE;
      }
   }
   
   return SUCCESS;
}

Errc PMeanProjection( const Img3dsl &ims, Img2d<Img3dsl::ValueType> &imd, int plan ) {
   Point3d p;
   double somme;
   Long nb;
   
   switch(plan) {
   case PLANZ:
      imd.New(ims.Height(), ims.Width());
      for (p.y = 0; p.y < ims.Height(); p.y++) {
	 for (p.x = 0; p.x < ims.Width(); p.x++) {
	    somme = 0.0; nb = 0;
	    for (p.z = 0; p.z < ims.Depth(); p.z++) {
	       somme += ims[p];
	       nb++;
	    }
	    if (nb) {
	       imd[p.y][p.x] = (Img3dsl::ValueType)(somme/nb);
	    } else {
	       imd[p.y][p.x] = 0;
	    }
	 }
      }
      break;
   case PLANY:
      imd.New(ims.Depth(), ims.Width());
      for (p.z = 0; p.z < ims.Depth(); p.z++) {
	 for (p.x = 0; p.x < ims.Width(); p.x++) {
	    somme = 0.0; nb = 0;
	    for (p.y = 0; p.y < ims.Height(); p.y++) {
	       somme += ims[p];
	       nb++;
	    }
	    if (nb) {
	       imd[p.z][p.x] = (Img3dsl::ValueType)(somme/nb);
	    } else {
	       imd[p.z][p.x] = 0;
	    }
	 }
      }
      break;
   case PLANX:
      imd.New(ims.Depth(), ims.Height());
      for (p.z = 0;p.z < ims.Depth(); p.z++) {
	 for (p.y = 0; p.y < ims.Height(); p.y++) {
	    somme = 0.0; nb = 0;
	    for (p.x = 0; p.x < ims.Width(); p.x++) {
	       somme += ims[p];
	       nb++;
	    }
	    if (nb) {
	       imd[p.z][p.y] = (Img3dsl::ValueType)(somme/nb);
	    } else {
	       imd[p.z][p.y] = 0;
	    }
	 }
      }
      break;
      default: {
	 std::cerr << "PMeanProject: bad parameter value: " << plan << std::endl;
	 return FAILURE;
      }
   }
   
   return SUCCESS;
}

Errc PMeanProjection( const Img3dsf &ims, Img2d<Img3dsf::ValueType> &imd, int plan ) {
   Point3d p;
   double somme;
   Long nb;
   
   switch(plan) {
   case PLANZ:
      imd.New(ims.Height(), ims.Width());
      for (p.y = 0; p.y < ims.Height(); p.y++) {
	 for (p.x = 0; p.x < ims.Width(); p.x++) {
	    somme = 0.0; nb = 0;
	    for (p.z = 0; p.z < ims.Depth(); p.z++) {
	       somme += ims[p];
	       nb++;
	    }
	    if (nb) {
	       imd[p.y][p.x] = (Img3dsf::ValueType)(somme/nb);
	    } else {
	       imd[p.y][p.x] = 0;
	    }
	 }
      }
      break;
   case PLANY:
      imd.New(ims.Depth(), ims.Width());
      for (p.z = 0; p.z < ims.Depth(); p.z++) {
	 for (p.x = 0; p.x < ims.Width(); p.x++) {
	    somme = 0.0; nb = 0;
	    for (p.y = 0; p.y < ims.Height(); p.y++) {
	       somme += ims[p];
	       nb++;
	    }
	    if (nb) {
	       imd[p.z][p.x] = (Img3dsf::ValueType)(somme/nb);
	    } else {
	       imd[p.z][p.x] = 0;
	    }
	 }
      }
      break;
   case PLANX:
      imd.New(ims.Depth(), ims.Height());
      for (p.z = 0;p.z < ims.Depth(); p.z++) {
	 for (p.y = 0; p.y < ims.Height(); p.y++) {
	    somme = 0.0; nb = 0;
	    for (p.x = 0; p.x < ims.Width(); p.x++) {
	       somme += ims[p];
	       nb++;
	    }
	    if (nb) {
	       imd[p.z][p.y] = (Img3dsf::ValueType)(somme/nb);
	    } else {
	       imd[p.z][p.y] = 0;
	    }
	 }
      }
      break;
      default: {
	 std::cerr << "PMeanProject: bad parameter value: " << plan << std::endl;
	 return FAILURE;
      }
   }
   
   return SUCCESS;
}


#ifdef MAIN
#define USAGE	"usage: %s axis [im_in|- ] [im_out|-]"
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
   if (objs[0]->Type() == Po_Img2duc) {
      Img2duc* const ims = (Img2duc*)objs[0];
      objd[0] = new Img1d<Img2duc::ValueType>;
      Img1d<Img2duc::ValueType>* const imd = (Img1d<Img2duc::ValueType>*)objd[0];
      result = PMeanProjection(*ims, *imd, atoi(parv[0]));
      goto end;
   }

   if (objs[0]->Type() == Po_Img2dsl) {
      Img2dsl* const ims = (Img2dsl*)objs[0];
      objd[0] = new Img1d<Img2dsl::ValueType>;
      Img1d<Img2dsl::ValueType>* const imd = (Img1d<Img2dsl::ValueType>*)objd[0];
      result = PMeanProjection(*ims, *imd, atoi(parv[0]));
      goto end;
   }

   if (objs[0]->Type() == Po_Img2dsf) {
      Img2dsf* const ims = (Img2dsf*)objs[0];
      objd[0] = new Img1d<Img2dsf::ValueType>;
      Img1d<Img2dsf::ValueType>* const imd = (Img1d<Img2dsf::ValueType>*)objd[0];
      result = PMeanProjection(*ims, *imd, atoi(parv[0]));
      goto end;
   }

   if (objs[0]->Type() == Po_Img3duc) {
      Img3duc* const ims = (Img3duc*)objs[0];
      objd[0] = new Img2d<Img3duc::ValueType>;
      Img2d<Img3duc::ValueType>* const imd = (Img2d<Img3duc::ValueType>*)objd[0];
      result = PMeanProjection(*ims, *imd, atoi(parv[0]));
      goto end;
   }

   if (objs[0]->Type() == Po_Img3dsl) {
      Img3dsl* const ims = (Img3dsl*)objs[0];
      objd[0] = new Img2d<Img3dsl::ValueType>;
      Img2d<Img3dsl::ValueType>* const imd = (Img2d<Img3dsl::ValueType>*)objd[0];
      result = PMeanProjection(*ims, *imd, atoi(parv[0]));
      goto end;
   }

   if (objs[0]->Type() == Po_Img3dsf) {
      Img3dsf* const ims = (Img3dsf*)objs[0];
      objd[0] = new Img2d<Img3dsf::ValueType>;
      Img2d<Img3dsf::ValueType>* const imd = (Img2d<Img3dsf::ValueType>*)objd[0];
      result = PMeanProjection(*ims, *imd, atoi(parv[0]));
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
