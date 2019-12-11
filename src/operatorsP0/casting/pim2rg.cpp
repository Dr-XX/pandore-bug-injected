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
 * @author Régis Clouard - 1995-07-22
 * @author Regis Clouard - 2001-04-11 (version 3.00)
 */

#include <map>
#include <pandore.h>
using namespace pandore;

/**
 * @file pim2rg.cpp
 * Convert an image (of labels) to a region map.
 */

template <typename T>
struct Triplet {
   Triplet( const T& a, const T& b, const T& c): v1(a), v2(b), v3(c) { }
   T v1;
   T v2;
   T v3;
};

template <typename T>
struct compare {
   bool operator() (const Triplet<T>& a, const Triplet<T>& b) const {     
     if (a.v1 < b.v1) {
	return true;
     } else if (a.v1 == b.v1) {
	if (a.v2 < b.v2) {
	   return true;
	} else if (a.v2 == b.v2) {
	   if (a.v3 < b.v3) {
	      return true;
	   } else {
	      return false;
	   }
	} else {
	   return false;
	}
     } else {
	return false;
     }
  }
};



Errc PIm2Rg( const Img2duc &ims, Reg2d &rgd ) {
   Ulong nbreg = 0;   
   Img2duc::ValueType *pims = ims.Vector();
   Reg2d::ValueType *prgd = rgd.Vector();
   
   for (; pims < ims.Vector() + ims.VectorSize(); pims++, prgd++) {
      if (*pims <= 0) {
	 *prgd = 0;
      } else {
	 if (nbreg < (Ulong)*pims) {
	    nbreg = (Ulong)*pims;
	 }
	 *prgd = (Reg2d::ValueType)*pims;
      }
   }
   return rgd.Labels(nbreg);
}

Errc PIm2Rg( const Img2dsl &ims, Reg2d &rgd ) {
   Ulong nbreg = 0;   
   Img2dsl::ValueType *pims = ims.Vector();
   Reg2d::ValueType *prgd = rgd.Vector();
   
   for (; pims < ims.Vector() + ims.VectorSize(); pims++, prgd++) {
      if (*pims <= 0) {
	 *prgd = 0;
      } else {
	 if (nbreg < (Ulong)*pims) {
	    nbreg = (Ulong)*pims;
	 }
	 *prgd = (Reg2d::ValueType)*pims;
      }
   }
   return rgd.Labels(nbreg);
}

Errc PIm2Rg( const Img2dsf &ims, Reg2d &rgd ) {
   Ulong nbreg = 0;   
   Img2dsf::ValueType *pims = ims.Vector();
   Reg2d::ValueType *prgd = rgd.Vector();
   
   for (; pims < ims.Vector() + ims.VectorSize(); pims++, prgd++) {
      if (*pims <= 0) {
	 *prgd = 0;
      } else {
	 if (nbreg < (Ulong)*pims) {
	    nbreg = (Ulong)*pims;
	 }
	 *prgd = (Reg2d::ValueType)*pims;
      }
   }
   return rgd.Labels(nbreg);
}

Errc PIm2Rg( const Imc2duc &ims, Reg2d &rgd ) {
   Ulong nbreg = 0;
   std::map< Triplet<Imc2duc::ValueType>, Ulong, compare<Imc2duc::ValueType> > labels;

   for (int y = 0; y < ims.Height(); y++) {
      for (int x = 0; x < ims.Width(); x++) {
	 if (ims(0, y, x) == 0 && ims(1, y, x) == 0 && ims(2, y, x) == 0) {
	    rgd(y, x) = 0;
   	 } else {
	    Triplet<Imc2duc::ValueType> triplet(ims(0, y, x), ims(1, y, x), ims(2, y, x));
	    
	    if (labels[triplet] == 0) {
	       labels[triplet] = ++nbreg;
	    }
	    rgd(y, x) = labels[triplet];
         }
      }
   }

   return rgd.Labels(nbreg);
}

Errc PIm2Rg( const Imc2dsl &ims, Reg2d &rgd ) {
   Ulong nbreg = 0;
   std::map< Triplet<Imc2dsl::ValueType>, Ulong, compare<Imc2dsl::ValueType> > labels;

   for (int y = 0; y < ims.Height(); y++) {
      for (int x = 0; x < ims.Width(); x++) {
	 if (ims(0, y, x) == 0 && ims(1, y, x) == 0 && ims(2, y, x) == 0) {
	    rgd(y, x) = 0;
   	 } else {
	    Triplet<Imc2dsl::ValueType> triplet(ims(0, y, x), ims(1, y, x), ims(2, y, x));
	    
	    if (labels[triplet] == 0) {
	       labels[triplet] = ++nbreg;
	    }
	    rgd(y, x) = labels[triplet];
         }
      }
   }

   return rgd.Labels(nbreg);
}

Errc PIm2Rg( const Imc2dsf &ims, Reg2d &rgd ) {
   Ulong nbreg = 0;
   std::map< Triplet<Imc2dsf::ValueType>, Ulong, compare<Imc2dsf::ValueType> > labels;

   for (int y = 0; y < ims.Height(); y++) {
      for (int x = 0; x < ims.Width(); x++) {
	 if (ims(0, y, x) == 0 && ims(1, y, x) == 0 && ims(2, y, x) == 0) {
	    rgd(y, x) = 0;
   	 } else {
	    Triplet<Imc2dsf::ValueType> triplet(ims(0, y, x), ims(1, y, x), ims(2, y, x));
	    
	    if (labels[triplet] == 0) {
	       labels[triplet] = ++nbreg;
	    }
	    rgd(y, x) = labels[triplet];
         }
      }
   }

   return rgd.Labels(nbreg);
}

Errc PIm2Rg( const Img3duc &ims, Reg2d &rgd ) {
   Ulong nbreg = 0;   
   Img3duc::ValueType *pims = ims.Vector();
   Reg2d::ValueType *prgd = rgd.Vector();
   
   for (; pims < ims.Vector() + ims.VectorSize(); pims++, prgd++) {
      if (*pims <= 0) {
	 *prgd = 0;
      } else {
	 if (nbreg < (Ulong)*pims) {
	    nbreg = (Ulong)*pims;
	 }
	 *prgd = (Reg2d::ValueType)*pims;
      }
   }
   return rgd.Labels(nbreg);
}

Errc PIm2Rg( const Img3dsl &ims, Reg2d &rgd ) {
   Ulong nbreg = 0;   
   Img3dsl::ValueType *pims = ims.Vector();
   Reg2d::ValueType *prgd = rgd.Vector();
   
   for (; pims < ims.Vector() + ims.VectorSize(); pims++, prgd++) {
      if (*pims <= 0) {
	 *prgd = 0;
      } else {
	 if (nbreg < (Ulong)*pims) {
	    nbreg = (Ulong)*pims;
	 }
	 *prgd = (Reg2d::ValueType)*pims;
      }
   }
   return rgd.Labels(nbreg);
}

Errc PIm2Rg( const Img3dsf &ims, Reg2d &rgd ) {
   Ulong nbreg = 0;   
   Img3dsf::ValueType *pims = ims.Vector();
   Reg2d::ValueType *prgd = rgd.Vector();
   
   for (; pims < ims.Vector() + ims.VectorSize(); pims++, prgd++) {
      if (*pims <= 0) {
	 *prgd = 0;
      } else {
	 if (nbreg < (Ulong)*pims) {
	    nbreg = (Ulong)*pims;
	 }
	 *prgd = (Reg2d::ValueType)*pims;
      }
   }
   return rgd.Labels(nbreg);
}

Errc PIm2Rg( const Imc3duc &ims, Reg3d &rgd ) {
   Ulong nbreg = 0;
   std::map< Triplet<Imc3duc::ValueType>, Ulong, compare<Imc3duc::ValueType> > labels;

   for (int z = 0; z < ims.Depth(); z++) {
      for (int y = 0; y < ims.Height(); y++) {
	 for (int x = 0; x < ims.Width(); x++) {
	    if (ims(0, z, y, x) == 0 && ims(1, z, y, x) == 0 && ims(2, z, y, x) == 0) {
	       rgd(z, y, x) = 0;
	    } else {
	       Triplet<Imc3duc::ValueType> triplet(ims(0, z, y, x), ims(1, z, y, x), ims(2, z, y, x));
	       
	       if (labels[triplet] == 0) {
		  labels[triplet] = ++nbreg;
	       }
	       rgd(z, y, x) = labels[triplet];
	    }
	 }
      }
   }
   return rgd.Labels(nbreg);
}

Errc PIm2Rg( const Imc3dsl &ims, Reg3d &rgd ) {
   Ulong nbreg = 0;
   std::map< Triplet<Imc3dsl::ValueType>, Ulong, compare<Imc3dsl::ValueType> > labels;

   for (int z = 0; z < ims.Depth(); z++) {
      for (int y = 0; y < ims.Height(); y++) {
	 for (int x = 0; x < ims.Width(); x++) {
	    if (ims(0, z, y, x) == 0 && ims(1, z, y, x) == 0 && ims(2, z, y, x) == 0) {
	       rgd(z, y, x) = 0;
	    } else {
	       Triplet<Imc3dsl::ValueType> triplet(ims(0, z, y, x), ims(1, z, y, x), ims(2, z, y, x));
	       
	       if (labels[triplet] == 0) {
		  labels[triplet] = ++nbreg;
	       }
	       rgd(z, y, x) = labels[triplet];
	    }
	 }
      }
   }
   return rgd.Labels(nbreg);
}

Errc PIm2Rg( const Imc3dsf &ims, Reg3d &rgd ) {
   Ulong nbreg = 0;
   std::map< Triplet<Imc3dsf::ValueType>, Ulong, compare<Imc3dsf::ValueType> > labels;

   for (int z = 0; z < ims.Depth(); z++) {
      for (int y = 0; y < ims.Height(); y++) {
	 for (int x = 0; x < ims.Width(); x++) {
	    if (ims(0, z, y, x) == 0 && ims(1, z, y, x) == 0 && ims(2, z, y, x) == 0) {
	       rgd(z, y, x) = 0;
	    } else {
	       Triplet<Imc3dsf::ValueType> triplet(ims(0, z, y, x), ims(1, z, y, x), ims(2, z, y, x));
	       
	       if (labels[triplet] == 0) {
		  labels[triplet] = ++nbreg;
	       }
	       rgd(z, y, x) = labels[triplet];
	    }
	 }
      }
   }
   return rgd.Labels(nbreg);
}


#ifdef MAIN
#define	USAGE	"usage: %s [-m mask] [im_in|-] [im_out|-]"
#define	PARC	0
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
   if (objs[0]->Type() == Po_Img2duc) {
      Img2duc* const ims = (Img2duc*)objs[0];
      objd[0] = new Reg2d(ims->Props());      
      Reg2d* const rgd = (Reg2d*)objd[0];
      
      result = PIm2Rg(*ims, *rgd);
      goto end;
   }
   if (objs[0]->Type() == Po_Img2dsl) {
      Img2dsl* const ims = (Img2dsl*)objs[0];
      objd[0] = new Reg2d(ims->Props());      
      Reg2d* const rgd = (Reg2d*)objd[0];
      
      result = PIm2Rg(*ims, *rgd);
      goto end;
   }
   if (objs[0]->Type() == Po_Img2dsf) {
      Img2dsf* const ims = (Img2dsf*)objs[0];
      objd[0] = new Reg2d(ims->Props());      
      Reg2d* const rgd = (Reg2d*)objd[0];
      
      result = PIm2Rg(*ims, *rgd);
      goto end;
   }
   if (objs[0]->Type() == Po_Imc2duc) {
      Imc2duc* const ims = (Imc2duc*)objs[0];
      objd[0] = new Reg2d(ims->Props());      
      Reg2d* const rgd = (Reg2d*)objd[0];
      
      result = PIm2Rg(*ims, *rgd);
      goto end;
   }
   if (objs[0]->Type() == Po_Imc2dsl) {
      Imc2dsl* const ims = (Imc2dsl*)objs[0];
      objd[0] = new Reg2d(ims->Props());      
      Reg2d* const rgd = (Reg2d*)objd[0];
      
      result = PIm2Rg(*ims, *rgd);
      goto end;
   }
   if (objs[0]->Type() == Po_Imc2dsf) {
      Imc2dsf* const ims = (Imc2dsf*)objs[0];
      objd[0] = new Reg2d(ims->Props());      
      Reg2d* const rgd = (Reg2d*)objd[0];
      
      result = PIm2Rg(*ims, *rgd);
      goto end;
   }
   if (objs[0]->Type() == Po_Img3duc) {
      Img3duc* const ims = (Img3duc*)objs[0];
      objd[0] = new Reg2d(ims->Props());      
      Reg2d* const rgd = (Reg2d*)objd[0];
      
      result = PIm2Rg(*ims, *rgd);
      goto end;
   }
   if (objs[0]->Type() == Po_Img3dsl) {
      Img3dsl* const ims = (Img3dsl*)objs[0];
      objd[0] = new Reg2d(ims->Props());      
      Reg2d* const rgd = (Reg2d*)objd[0];
      
      result = PIm2Rg(*ims, *rgd);
      goto end;
   }
   if (objs[0]->Type() == Po_Img3dsf) {
      Img3dsf* const ims = (Img3dsf*)objs[0];
      objd[0] = new Reg2d(ims->Props());      
      Reg2d* const rgd = (Reg2d*)objd[0];
      
      result = PIm2Rg(*ims, *rgd);
      goto end;
   }
   if (objs[0]->Type() == Po_Imc3duc) {
      Imc3duc* const ims = (Imc3duc*)objs[0];
      objd[0] = new Reg3d(ims->Props());      
      Reg3d* const rgd = (Reg3d*)objd[0];
      
      result = PIm2Rg(*ims, *rgd);
      goto end;
   }
   if (objs[0]->Type() == Po_Imc3dsl) {
      Imc3dsl* const ims = (Imc3dsl*)objs[0];
      objd[0] = new Reg3d(ims->Props());      
      Reg3d* const rgd = (Reg3d*)objd[0];
      
      result = PIm2Rg(*ims, *rgd);
      goto end;
   }
   if (objs[0]->Type() == Po_Imc3dsf) {
      Imc3dsf* const ims = (Imc3dsf*)objs[0];
      objd[0] = new Reg3d(ims->Props());      
      Reg3d* const rgd = (Reg3d*)objd[0];
      
      result = PIm2Rg(*ims, *rgd);
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
