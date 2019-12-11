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
 * @author Francois Angot - 1995-02-08
 * @author Régis Clouard - 2001-06-08
 * @author Régis Clouard - 2002-07-09 (change the measure)
 * @author Régis Clouard - Jun 06, 2010 (add output vector).
 */

#include <pandore.h>
using namespace pandore;

/**
 * @file pcontrast1value.cpp
 * Calcule le contraste global de l'image.
 */

template < typename T>
Float PContrast1Value( const Imx2d<T> &ims, Collection &col_out ) {
   Float *t = new Float[ims.Bands()];
   Point2d ps;

   for (int b=0; b<ims.Bands(); ++b) {
      double contrast = 0.0;
      for ( ps.y=0; ps.y < ims.Height(); ps.y++) {
	 for ( ps.x=0; ps.x < ims.Width(); ps.x++) {
	    double maxCst = 0.0;
	    for (int v=0; v<8; v++) {
	       Point2d pt = ps + v8[v];
	       if (!ims.Hold(pt)) {
		  continue;
	       }
	       double Cst = (double)ABS(ims[b][ps]-ims[b][pt]);
	       if (Cst > maxCst) {
		  maxCst = Cst;
	       }
	    }
	    contrast += maxCst;
	 }
      }
      t[b] = (Float)(contrast / ims.VectorSize());
   }
   col_out.SETARRAY("internal_array", Float, t, ims.Bands());

   return t[0];
}


template < typename T>
Float PContrast1Value( const Imx3d<T> &ims, Collection &col_out ) {
   Float *t = new Float[ims.Bands()];
   Point3d ps;

   for (int b=0; b<ims.Bands(); ++b) {
      double contrast = 0.0;
      for ( ps.z=0; ps.z < ims.Depth(); ps.z++) {
	 for ( ps.y=0; ps.y < ims.Height(); ps.y++) {
	    for ( ps.x=0; ps.x < ims.Width(); ps.x++) {
	       double maxCst = 0.0;
	       for (int v=0; v<26; v++) {
		  Point3d pt = ps + v26[v];
		  if (!ims.Hold(pt)) {
		     continue;
		  }
	          double Cst=(double)ABS(ims[b][ps]-ims[b][pt]);
		  if (Cst > maxCst) {
		     maxCst = Cst;
		  }
	       }
	       contrast += maxCst;
	    }
	 }
      }
      t[b] = (Float)(contrast / ims.VectorSize());
   }
   col_out.SETARRAY("internal_array", Float, t, ims.Bands());

   return t[0];
}



Float PContrast1Value( const Graph2d &grs, Collection &col_out) {
   Float *t = new Float[1];
   int i;
   double contrast=0.0;
   double Cst, maxCst;
   GEdge *ptr;
   long nb=0;
   
   for (i = 0; i < grs.Size(); i++) {
      if ((grs[i])) {
	 maxCst = Limits<Graph2d::ValueType>::min();
	 for (ptr = grs[i]->Neighbours(); ptr != NULL; ptr = ptr->Next()) {
	    Cst = (double)ABS(grs[i]->value - grs[ptr->Node()]->value); 
	    if (Cst > maxCst) {
	       maxCst = Cst;
	    }
	 }
	 nb++;
	 contrast += maxCst;
      }
   } 
   if (nb) {
      t[0] = Float(contrast / nb);
   } else {
      t[0] = 0;
   }
   col_out.SETARRAY("internal_array", Float, t, 1);
   
   return t[0];
}
Float PContrast1Value( const Graph3d &grs, Collection &col_out) {
   Float *t = new Float[1];
   int i;
   double contrast=0.0;
   double Cst, maxCst;
   GEdge *ptr;
   long nb=0;
   
   for (i = 0; i < grs.Size(); i++) {
      if ((grs[i])) {
	 maxCst = Limits<Graph3d::ValueType>::min();
	 for (ptr = grs[i]->Neighbours(); ptr != NULL; ptr = ptr->Next()) {
	    Cst = (double)ABS(grs[i]->value - grs[ptr->Node()]->value); 
	    if (Cst > maxCst) {
	       maxCst = Cst;
	    }
	 }
	 nb++;
	 contrast += maxCst;
      }
   } 
   if (nb) {
      t[0] = Float(contrast / nb);
   } else {
      t[0] = 0;
   }
   col_out.SETARRAY("internal_array", Float, t, 1);
   
   return t[0];
}

#ifdef MAIN
#define USAGE	"usage: %s [im_in|-] [col_out|-]"
#define PARC	0
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
      Img2duc* const ims=(Img2duc*)objs[0];
      objd[0]=new Collection;
      Collection* const cold=(Collection*)objd[0];

      result= PContrast1Value(*ims, *cold);
      goto end;
   }
   if (objs[0]->Type() == Po_Img2dsl) {
      Img2dsl* const ims=(Img2dsl*)objs[0];
      objd[0]=new Collection;
      Collection* const cold=(Collection*)objd[0];

      result= PContrast1Value(*ims, *cold);
      goto end;
   }
   if (objs[0]->Type() == Po_Img2dsf) {
      Img2dsf* const ims=(Img2dsf*)objs[0];
      objd[0]=new Collection;
      Collection* const cold=(Collection*)objd[0];

      result= PContrast1Value(*ims, *cold);
      goto end;
   }
   if (objs[0]->Type() == Po_Imc2duc) {
      Imc2duc* const ims=(Imc2duc*)objs[0];
      objd[0]=new Collection;
      Collection* const cold=(Collection*)objd[0];

      result= PContrast1Value(*ims, *cold);
      goto end;
   }
   if (objs[0]->Type() == Po_Imc2dsl) {
      Imc2dsl* const ims=(Imc2dsl*)objs[0];
      objd[0]=new Collection;
      Collection* const cold=(Collection*)objd[0];

      result= PContrast1Value(*ims, *cold);
      goto end;
   }
   if (objs[0]->Type() == Po_Imc2dsf) {
      Imc2dsf* const ims=(Imc2dsf*)objs[0];
      objd[0]=new Collection;
      Collection* const cold=(Collection*)objd[0];

      result= PContrast1Value(*ims, *cold);
      goto end;
   }
   if (objs[0]->Type() == Po_Imx2duc) {
      Imx2duc* const ims=(Imx2duc*)objs[0];
      objd[0]=new Collection;
      Collection* const cold=(Collection*)objd[0];

      result= PContrast1Value(*ims, *cold);
      goto end;
   }
   if (objs[0]->Type() == Po_Imx2dsl) {
      Imx2dsl* const ims=(Imx2dsl*)objs[0];
      objd[0]=new Collection;
      Collection* const cold=(Collection*)objd[0];

      result= PContrast1Value(*ims, *cold);
      goto end;
   }
   if (objs[0]->Type() == Po_Imx2dsf) {
      Imx2dsf* const ims=(Imx2dsf*)objs[0];
      objd[0]=new Collection;
      Collection* const cold=(Collection*)objd[0];

      result= PContrast1Value(*ims, *cold);
      goto end;
   }
   if (objs[0]->Type() == Po_Graph2d) {
      Graph2d* const grs = (Graph2d*)objs[0];
      objd[0] = new Collection;
      Collection* const cold = (Collection*)objd[0];
      
      result = PContrast1Value(*grs, *cold);
      goto end;
   }
   if (objs[0]->Type() == Po_Graph3d) {
      Graph3d* const grs = (Graph3d*)objs[0];
      objd[0] = new Collection;
      Collection* const cold = (Collection*)objd[0];
      
      result = PContrast1Value(*grs, *cold);
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
