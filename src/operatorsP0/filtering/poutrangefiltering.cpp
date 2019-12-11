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
 * @author Regis Clouard - 1994-10-13
 */

#include <pandore.h>
using namespace pandore;

/**
 * @file poutrangefiltering.cpp
 * eps= Valeur d'ecart toleree.
 * Affecter comme valeur du pixel courant 
 * la moyenne des 8 voisins si moyenne > eps
 * l'ancienne valeur sinon.
 */

Errc POutRangeFiltering( const Img2duc &ims, Img2duc &imd, float eps ) {
   int n;
   float s;
   Point2d p;
   Float val;

   for (p.y = 0; p.y < ims.Height(); p.y++)
   for (p.x = 0; p.x < ims.Width(); p.x++)
   {
      s=0.0F; n=0;
      for (int v=0; v<8; v++) {
	 if (ims.Hold(p+v8[v])) {
	    s += (float)ims[p+v8[v]];
	    n++;
	 }
      }
      val = s / n;
      imd[p] = ((ABS(val-(float)ims[p]) <= eps) ? (Img2duc::ValueType)val : ims[p]);
   }

   return SUCCESS;
}

Errc POutRangeFiltering( const Img2dsl &ims, Img2dsl &imd, float eps ) {
   int n;
   float s;
   Point2d p;
   Float val;

   for (p.y = 0; p.y < ims.Height(); p.y++)
   for (p.x = 0; p.x < ims.Width(); p.x++)
   {
      s=0.0F; n=0;
      for (int v=0; v<8; v++) {
	 if (ims.Hold(p+v8[v])) {
	    s += (float)ims[p+v8[v]];
	    n++;
	 }
      }
      val = s / n;
      imd[p] = ((ABS(val-(float)ims[p]) <= eps) ? (Img2dsl::ValueType)val : ims[p]);
   }

   return SUCCESS;
}

Errc POutRangeFiltering( const Img2dsf &ims, Img2dsf &imd, float eps ) {
   int n;
   float s;
   Point2d p;
   Float val;

   for (p.y = 0; p.y < ims.Height(); p.y++)
   for (p.x = 0; p.x < ims.Width(); p.x++)
   {
      s=0.0F; n=0;
      for (int v=0; v<8; v++) {
	 if (ims.Hold(p+v8[v])) {
	    s += (float)ims[p+v8[v]];
	    n++;
	 }
      }
      val = s / n;
      imd[p] = ((ABS(val-(float)ims[p]) <= eps) ? (Img2dsf::ValueType)val : ims[p]);
   }

   return SUCCESS;
}

Errc POutRangeFiltering( const Img3duc &ims, Img3duc &imd, float eps ) {
   int n;
   float s;
   Point3d p;
   Float val;

   for (p.z = 0; p.z < ims.Depth(); p.z++)
   for (p.y = 0; p.y < ims.Height(); p.y++)
   for (p.x = 0; p.x < ims.Width(); p.x++)
   {
      s=0.0F; n=0;
      for (int v=0; v<26; v++) {
	 if (ims.Hold(p+v26[v])) {
	    s += (float)ims[p+v26[v]];
	    n++;
	 }
      }
      val = s / n;
      imd[p] = ((ABS(val-(float)ims[p]) <= eps) ? (Img3duc::ValueType)val : ims[p]);
   }

   return SUCCESS;
}

Errc POutRangeFiltering( const Img3dsl &ims, Img3dsl &imd, float eps ) {
   int n;
   float s;
   Point3d p;
   Float val;

   for (p.z = 0; p.z < ims.Depth(); p.z++)
   for (p.y = 0; p.y < ims.Height(); p.y++)
   for (p.x = 0; p.x < ims.Width(); p.x++)
   {
      s=0.0F; n=0;
      for (int v=0; v<26; v++) {
	 if (ims.Hold(p+v26[v])) {
	    s += (float)ims[p+v26[v]];
	    n++;
	 }
      }
      val = s / n;
      imd[p] = ((ABS(val-(float)ims[p]) <= eps) ? (Img3dsl::ValueType)val : ims[p]);
   }

   return SUCCESS;
}

Errc POutRangeFiltering( const Img3dsf &ims, Img3dsf &imd, float eps ) {
   int n;
   float s;
   Point3d p;
   Float val;

   for (p.z = 0; p.z < ims.Depth(); p.z++)
   for (p.y = 0; p.y < ims.Height(); p.y++)
   for (p.x = 0; p.x < ims.Width(); p.x++)
   {
      s=0.0F; n=0;
      for (int v=0; v<26; v++) {
	 if (ims.Hold(p+v26[v])) {
	    s += (float)ims[p+v26[v]];
	    n++;
	 }
      }
      val = s / n;
      imd[p] = ((ABS(val-(float)ims[p]) <= eps) ? (Img3dsf::ValueType)val : ims[p]);
   }

   return SUCCESS;
}


/*
 * Modify only the following constants, and the function call.
 */
#ifdef MAIN
#define	USAGE	"usage: %s difference [-m mask] [im_in|-] [im_out|-]"
#define	PARC	1
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
   if (objs[0]->Type()==Po_Img2duc) {
      Img2duc* const ims=(Img2duc*)objs[0];
      objd[0]=new Img2duc(ims->Size());
      Img2duc * const imd=(Img2duc*)objd[0];
      
      result=POutRangeFiltering(*ims,*imd,(Float)atof(parv[0]));
      goto end;
   }
   if (objs[0]->Type()==Po_Img2dsl) {
      Img2dsl* const ims=(Img2dsl*)objs[0];
      objd[0]=new Img2dsl(ims->Size());
      Img2dsl * const imd=(Img2dsl*)objd[0];
      
      result=POutRangeFiltering(*ims,*imd,(Float)atof(parv[0]));
      goto end;
   }
   if (objs[0]->Type()==Po_Img2dsf) {
      Img2dsf* const ims=(Img2dsf*)objs[0];
      objd[0]=new Img2dsf(ims->Size());
      Img2dsf * const imd=(Img2dsf*)objd[0];
      
      result=POutRangeFiltering(*ims,*imd,(Float)atof(parv[0]));
      goto end;
   }
   if (objs[0]->Type()==Po_Img3duc) {
      Img3duc* const ims=(Img3duc*)objs[0];
      objd[0]=new Img3duc(ims->Size());
      Img3duc * const imd=(Img3duc*)objd[0];
      
      result=POutRangeFiltering(*ims,*imd,(Float)atof(parv[0]));
      goto end;
   }
   if (objs[0]->Type()==Po_Img3dsl) {
      Img3dsl* const ims=(Img3dsl*)objs[0];
      objd[0]=new Img3dsl(ims->Size());
      Img3dsl * const imd=(Img3dsl*)objd[0];
      
      result=POutRangeFiltering(*ims,*imd,(Float)atof(parv[0]));
      goto end;
   }
   if (objs[0]->Type()==Po_Img3dsf) {
      Img3dsf* const ims=(Img3dsf*)objs[0];
      objd[0]=new Img3dsf(ims->Size());
      Img3dsf * const imd=(Img3dsf*)objd[0];
      
      result=POutRangeFiltering(*ims,*imd,(Float)atof(parv[0]));
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
