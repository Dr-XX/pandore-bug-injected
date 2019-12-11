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
 * @author Francois Angot - 1999-12-27
 * @author Régis Clouard - 2001-04-24
 */

#include <math.h>
#include <pandore.h>
using namespace pandore;

/**
 * @file pvariancefiltering.cpp
 * Filtre de variance. 
 * Remplace chaque pixel par la valeur de sa variance avec ses voisins
 * d'intensite comprise entre les deux seuils.
 */

Errc PVarianceFiltering( const Img2duc &ims, Img2dsf &imd, Img2duc::ValueType low, Img2duc::ValueType high ) {
   Point2d p;
   Img2duc::ValueType c;
   unsigned long nb;
   double moy, var;
   
   for (p.y = 0; p.y < ims.Height(); p.y++)
   for (p.x = 0; p.x < ims.Width(); p.x++)
   {
      c = ims[p];
      if ((c >= low) && (c <= high)) {
	 nb = 1;
	 moy = (double)c;
      } else {
	 nb = 0;
	 moy = 0.0;
      }
      for (int v= 0; v < 8; v++) {
	 if (ims.Hold(p + v8[v])) {
	    c = ims[p+v8[v]];
	    if ((c >= low) && (c <= high)) {
	       nb++;
	       moy += (double)c;
	    }
	 }
      }
      if (nb) {
	 moy /= nb;
	 c = ims[p];
	 if ((c >= low) && (c <= high)) {
	    var = (c - moy) * (c - moy);
	 } else {
	    var = 0.0;
	 }
	 for (int v = 0; v < 8; v++) {
	    if (ims.Hold(p + v8[v])) {
	       c  = ims[p + v8[v]];
	       if ((c >= low) && (c <= high)) {
		  var += (c - moy) * (c - moy);
	       }
	    }
	 }
	 imd[p] = (Img2dsf::ValueType)(var / nb);
      } else {
	 imd[p] = 0;
      }
   }
  
  return SUCCESS;
}

Errc PVarianceFiltering( const Img2dsl &ims, Img2dsf &imd, Img2dsl::ValueType low, Img2dsl::ValueType high ) {
   Point2d p;
   Img2dsl::ValueType c;
   unsigned long nb;
   double moy, var;
   
   for (p.y = 0; p.y < ims.Height(); p.y++)
   for (p.x = 0; p.x < ims.Width(); p.x++)
   {
      c = ims[p];
      if ((c >= low) && (c <= high)) {
	 nb = 1;
	 moy = (double)c;
      } else {
	 nb = 0;
	 moy = 0.0;
      }
      for (int v= 0; v < 8; v++) {
	 if (ims.Hold(p + v8[v])) {
	    c = ims[p+v8[v]];
	    if ((c >= low) && (c <= high)) {
	       nb++;
	       moy += (double)c;
	    }
	 }
      }
      if (nb) {
	 moy /= nb;
	 c = ims[p];
	 if ((c >= low) && (c <= high)) {
	    var = (c - moy) * (c - moy);
	 } else {
	    var = 0.0;
	 }
	 for (int v = 0; v < 8; v++) {
	    if (ims.Hold(p + v8[v])) {
	       c  = ims[p + v8[v]];
	       if ((c >= low) && (c <= high)) {
		  var += (c - moy) * (c - moy);
	       }
	    }
	 }
	 imd[p] = (Img2dsf::ValueType)(var / nb);
      } else {
	 imd[p] = 0;
      }
   }
  
  return SUCCESS;
}

Errc PVarianceFiltering( const Img2dsf &ims, Img2dsf &imd, Img2dsf::ValueType low, Img2dsf::ValueType high ) {
   Point2d p;
   Img2dsf::ValueType c;
   unsigned long nb;
   double moy, var;
   
   for (p.y = 0; p.y < ims.Height(); p.y++)
   for (p.x = 0; p.x < ims.Width(); p.x++)
   {
      c = ims[p];
      if ((c >= low) && (c <= high)) {
	 nb = 1;
	 moy = (double)c;
      } else {
	 nb = 0;
	 moy = 0.0;
      }
      for (int v= 0; v < 8; v++) {
	 if (ims.Hold(p + v8[v])) {
	    c = ims[p+v8[v]];
	    if ((c >= low) && (c <= high)) {
	       nb++;
	       moy += (double)c;
	    }
	 }
      }
      if (nb) {
	 moy /= nb;
	 c = ims[p];
	 if ((c >= low) && (c <= high)) {
	    var = (c - moy) * (c - moy);
	 } else {
	    var = 0.0;
	 }
	 for (int v = 0; v < 8; v++) {
	    if (ims.Hold(p + v8[v])) {
	       c  = ims[p + v8[v]];
	       if ((c >= low) && (c <= high)) {
		  var += (c - moy) * (c - moy);
	       }
	    }
	 }
	 imd[p] = (Img2dsf::ValueType)(var / nb);
      } else {
	 imd[p] = 0;
      }
   }
  
  return SUCCESS;
}

Errc PVarianceFiltering( const Img3duc &ims, Img3dsf &imd, Img3duc::ValueType low, Img3duc::ValueType high ) {
   Point3d p;
   Img3duc::ValueType c;
   unsigned long nb;
   double moy, var;
   
   for (p.z = 0; p.z < ims.Depth(); p.z++)
   for (p.y = 0; p.y < ims.Height(); p.y++)
   for (p.x = 0; p.x < ims.Width(); p.x++)
   {
      c = ims[p];
      if ((c >= low) && (c <= high)) {
	 nb = 1;
	 moy = (double)c;
      } else {
	 nb = 0;
	 moy = 0.0;
      }
      for (int v= 0; v < 26; v++) {
	 if (ims.Hold(p + v26[v])) {
	    c = ims[p+v26[v]];
	    if ((c >= low) && (c <= high)) {
	       nb++;
	       moy += (double)c;
	    }
	 }
      }
      if (nb) {
	 moy /= nb;
	 c = ims[p];
	 if ((c >= low) && (c <= high)) {
	    var = (c - moy) * (c - moy);
	 } else {
	    var = 0.0;
	 }
	 for (int v = 0; v < 26; v++) {
	    if (ims.Hold(p + v26[v])) {
	       c  = ims[p + v26[v]];
	       if ((c >= low) && (c <= high)) {
		  var += (c - moy) * (c - moy);
	       }
	    }
	 }
	 imd[p] = (Img3dsf::ValueType)(var / nb);
      } else {
	 imd[p] = 0;
      }
   }
  
  return SUCCESS;
}

Errc PVarianceFiltering( const Img3dsl &ims, Img3dsf &imd, Img3dsl::ValueType low, Img3dsl::ValueType high ) {
   Point3d p;
   Img3dsl::ValueType c;
   unsigned long nb;
   double moy, var;
   
   for (p.z = 0; p.z < ims.Depth(); p.z++)
   for (p.y = 0; p.y < ims.Height(); p.y++)
   for (p.x = 0; p.x < ims.Width(); p.x++)
   {
      c = ims[p];
      if ((c >= low) && (c <= high)) {
	 nb = 1;
	 moy = (double)c;
      } else {
	 nb = 0;
	 moy = 0.0;
      }
      for (int v= 0; v < 26; v++) {
	 if (ims.Hold(p + v26[v])) {
	    c = ims[p+v26[v]];
	    if ((c >= low) && (c <= high)) {
	       nb++;
	       moy += (double)c;
	    }
	 }
      }
      if (nb) {
	 moy /= nb;
	 c = ims[p];
	 if ((c >= low) && (c <= high)) {
	    var = (c - moy) * (c - moy);
	 } else {
	    var = 0.0;
	 }
	 for (int v = 0; v < 26; v++) {
	    if (ims.Hold(p + v26[v])) {
	       c  = ims[p + v26[v]];
	       if ((c >= low) && (c <= high)) {
		  var += (c - moy) * (c - moy);
	       }
	    }
	 }
	 imd[p] = (Img3dsf::ValueType)(var / nb);
      } else {
	 imd[p] = 0;
      }
   }
  
  return SUCCESS;
}

Errc PVarianceFiltering( const Img3dsf &ims, Img3dsf &imd, Img3dsf::ValueType low, Img3dsf::ValueType high ) {
   Point3d p;
   Img3dsf::ValueType c;
   unsigned long nb;
   double moy, var;
   
   for (p.z = 0; p.z < ims.Depth(); p.z++)
   for (p.y = 0; p.y < ims.Height(); p.y++)
   for (p.x = 0; p.x < ims.Width(); p.x++)
   {
      c = ims[p];
      if ((c >= low) && (c <= high)) {
	 nb = 1;
	 moy = (double)c;
      } else {
	 nb = 0;
	 moy = 0.0;
      }
      for (int v= 0; v < 26; v++) {
	 if (ims.Hold(p + v26[v])) {
	    c = ims[p+v26[v]];
	    if ((c >= low) && (c <= high)) {
	       nb++;
	       moy += (double)c;
	    }
	 }
      }
      if (nb) {
	 moy /= nb;
	 c = ims[p];
	 if ((c >= low) && (c <= high)) {
	    var = (c - moy) * (c - moy);
	 } else {
	    var = 0.0;
	 }
	 for (int v = 0; v < 26; v++) {
	    if (ims.Hold(p + v26[v])) {
	       c  = ims[p + v26[v]];
	       if ((c >= low) && (c <= high)) {
		  var += (c - moy) * (c - moy);
	       }
	    }
	 }
	 imd[p] = (Img3dsf::ValueType)(var / nb);
      } else {
	 imd[p] = 0;
      }
   }
  
  return SUCCESS;
}


/*
 * Modify only the following constants, and the function call.
 */
#ifdef MAIN
#define	USAGE	"usage: %s low high [-m mask] [im_in|-] [im_out|-]"
#define	PARC	2
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
    objd[0] = new Img2dsf(ims->Size());
    Img2dsf* const imd = (Img2dsf*)objd[0];
    
    result = PVarianceFiltering(*ims, *imd, (Img2duc::ValueType)atof(parv[0]), (Img2duc::ValueType)atof(parv[1]));
    goto end;
   }
   if (objs[0]->Type() == Po_Img2dsl) {
    Img2dsl* const ims = (Img2dsl*)objs[0];
    objd[0] = new Img2dsf(ims->Size());
    Img2dsf* const imd = (Img2dsf*)objd[0];
    
    result = PVarianceFiltering(*ims, *imd, (Img2dsl::ValueType)atof(parv[0]), (Img2dsl::ValueType)atof(parv[1]));
    goto end;
   }
   if (objs[0]->Type() == Po_Img2dsf) {
    Img2dsf* const ims = (Img2dsf*)objs[0];
    objd[0] = new Img2dsf(ims->Size());
    Img2dsf* const imd = (Img2dsf*)objd[0];
    
    result = PVarianceFiltering(*ims, *imd, (Img2dsf::ValueType)atof(parv[0]), (Img2dsf::ValueType)atof(parv[1]));
    goto end;
   }
   if (objs[0]->Type() == Po_Img3duc) {
    Img3duc* const ims = (Img3duc*)objs[0];
    objd[0] = new Img3dsf(ims->Size());
    Img3dsf* const imd = (Img3dsf*)objd[0];
    
    result = PVarianceFiltering(*ims, *imd, (Img3duc::ValueType)atof(parv[0]), (Img3duc::ValueType)atof(parv[1]));
    goto end;
   }
   if (objs[0]->Type() == Po_Img3dsl) {
    Img3dsl* const ims = (Img3dsl*)objs[0];
    objd[0] = new Img3dsf(ims->Size());
    Img3dsf* const imd = (Img3dsf*)objd[0];
    
    result = PVarianceFiltering(*ims, *imd, (Img3dsl::ValueType)atof(parv[0]), (Img3dsl::ValueType)atof(parv[1]));
    goto end;
   }
   if (objs[0]->Type() == Po_Img3dsf) {
    Img3dsf* const ims = (Img3dsf*)objs[0];
    objd[0] = new Img3dsf(ims->Size());
    Img3dsf* const imd = (Img3dsf*)objd[0];
    
    result = PVarianceFiltering(*ims, *imd, (Img3dsf::ValueType)atof(parv[0]), (Img3dsf::ValueType)atof(parv[1]));
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
