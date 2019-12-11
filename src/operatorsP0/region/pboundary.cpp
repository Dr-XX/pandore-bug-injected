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
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE." *
 * 
 * For more information, refer to:
 * https://clouard.users.greyc.fr/Pandore
 */

#include <pandore.h>
using namespace pandore;

/**
 * @file pboundary.cpp
 *
 * Localisation of the region boundary.
 * A point is a boundary point if at least one of its neigbours
 * is not in the same region.
 */


Errc PBoundary( const Img2duc &ims, Img2duc &imd, int connexity ) {
   Point2d p;

   if (connexity == 4) {
   for (p.y = 0; p.y < ims.Height(); p.y++)
   for (p.x = 0; p.x < ims.Width(); p.x++)
      {
	 imd[p] = 0;
	 for (int v = 0; v < 4; v++) {
	    Point2d p1 = p + v4[v];
	    if (ims.Hold(p1) && ims[p] != ims[p1] && ims[p] != 0) {
	       imd[p] = Limits<Img2duc::ValueType>::max();
	       break;
	    }
	 }
      } 
   } else {
   for (p.y = 0; p.y < ims.Height(); p.y++)
   for (p.x = 0; p.x < ims.Width(); p.x++)
      {
	 imd[p] = 0;
	 for (int v = 0; v < 8; v++) {
	    Point2d p1 = p + v8[v];
	    if (ims.Hold(p1) && ims[p] != ims[p1] && ims[p] != 0) {
	       imd[p] = Limits<Img2duc::ValueType>::max();
	       break;
	    }
	 }
      } 
   }

   return SUCCESS;
}
Errc PBoundary( const Img2dsl &ims, Img2duc &imd, int connexity ) {
   Point2d p;

   if (connexity == 4) {
   for (p.y = 0; p.y < ims.Height(); p.y++)
   for (p.x = 0; p.x < ims.Width(); p.x++)
      {
	 imd[p] = 0;
	 for (int v = 0; v < 4; v++) {
	    Point2d p1 = p + v4[v];
	    if (ims.Hold(p1) && ims[p] != ims[p1] && ims[p] != 0) {
	       imd[p] = Limits<Img2duc::ValueType>::max();
	       break;
	    }
	 }
      } 
   } else {
   for (p.y = 0; p.y < ims.Height(); p.y++)
   for (p.x = 0; p.x < ims.Width(); p.x++)
      {
	 imd[p] = 0;
	 for (int v = 0; v < 8; v++) {
	    Point2d p1 = p + v8[v];
	    if (ims.Hold(p1) && ims[p] != ims[p1] && ims[p] != 0) {
	       imd[p] = Limits<Img2duc::ValueType>::max();
	       break;
	    }
	 }
      } 
   }

   return SUCCESS;
}
Errc PBoundary( const Img2dsf &ims, Img2duc &imd, int connexity ) {
   Point2d p;

   if (connexity == 4) {
   for (p.y = 0; p.y < ims.Height(); p.y++)
   for (p.x = 0; p.x < ims.Width(); p.x++)
      {
	 imd[p] = 0;
	 for (int v = 0; v < 4; v++) {
	    Point2d p1 = p + v4[v];
	    if (ims.Hold(p1) && ims[p] != ims[p1] && ims[p] != 0) {
	       imd[p] = Limits<Img2duc::ValueType>::max();
	       break;
	    }
	 }
      } 
   } else {
   for (p.y = 0; p.y < ims.Height(); p.y++)
   for (p.x = 0; p.x < ims.Width(); p.x++)
      {
	 imd[p] = 0;
	 for (int v = 0; v < 8; v++) {
	    Point2d p1 = p + v8[v];
	    if (ims.Hold(p1) && ims[p] != ims[p1] && ims[p] != 0) {
	       imd[p] = Limits<Img2duc::ValueType>::max();
	       break;
	    }
	 }
      } 
   }

   return SUCCESS;
}
Errc PBoundary( const Img3duc &ims, Img3duc &imd, int connexity ) {
   Point3d p;

   if (connexity == 6) {
   for (p.z = 0; p.z < ims.Depth(); p.z++)
   for (p.y = 0; p.y < ims.Height(); p.y++)
   for (p.x = 0; p.x < ims.Width(); p.x++)
      {
	 imd[p] = 0;
	 for (int v = 0; v < 6; v++) {
	    Point3d p1 = p + v6[v];
	    if (ims.Hold(p1) && ims[p] != ims[p1] && ims[p] != 0) {
	       imd[p] = Limits<Img3duc::ValueType>::max();
	       break;
	    }
	 }
      } 
   } else {
   for (p.z = 0; p.z < ims.Depth(); p.z++)
   for (p.y = 0; p.y < ims.Height(); p.y++)
   for (p.x = 0; p.x < ims.Width(); p.x++)
      {
	 imd[p] = 0;
	 for (int v = 0; v < 26; v++) {
	    Point3d p1 = p + v26[v];
	    if (ims.Hold(p1) && ims[p] != ims[p1] && ims[p] != 0) {
	       imd[p] = Limits<Img3duc::ValueType>::max();
	       break;
	    }
	 }
      } 
   }

   return SUCCESS;
}
Errc PBoundary( const Img3dsl &ims, Img3duc &imd, int connexity ) {
   Point3d p;

   if (connexity == 6) {
   for (p.z = 0; p.z < ims.Depth(); p.z++)
   for (p.y = 0; p.y < ims.Height(); p.y++)
   for (p.x = 0; p.x < ims.Width(); p.x++)
      {
	 imd[p] = 0;
	 for (int v = 0; v < 6; v++) {
	    Point3d p1 = p + v6[v];
	    if (ims.Hold(p1) && ims[p] != ims[p1] && ims[p] != 0) {
	       imd[p] = Limits<Img3duc::ValueType>::max();
	       break;
	    }
	 }
      } 
   } else {
   for (p.z = 0; p.z < ims.Depth(); p.z++)
   for (p.y = 0; p.y < ims.Height(); p.y++)
   for (p.x = 0; p.x < ims.Width(); p.x++)
      {
	 imd[p] = 0;
	 for (int v = 0; v < 26; v++) {
	    Point3d p1 = p + v26[v];
	    if (ims.Hold(p1) && ims[p] != ims[p1] && ims[p] != 0) {
	       imd[p] = Limits<Img3duc::ValueType>::max();
	       break;
	    }
	 }
      } 
   }

   return SUCCESS;
}
Errc PBoundary( const Img3dsf &ims, Img3duc &imd, int connexity ) {
   Point3d p;

   if (connexity == 6) {
   for (p.z = 0; p.z < ims.Depth(); p.z++)
   for (p.y = 0; p.y < ims.Height(); p.y++)
   for (p.x = 0; p.x < ims.Width(); p.x++)
      {
	 imd[p] = 0;
	 for (int v = 0; v < 6; v++) {
	    Point3d p1 = p + v6[v];
	    if (ims.Hold(p1) && ims[p] != ims[p1] && ims[p] != 0) {
	       imd[p] = Limits<Img3duc::ValueType>::max();
	       break;
	    }
	 }
      } 
   } else {
   for (p.z = 0; p.z < ims.Depth(); p.z++)
   for (p.y = 0; p.y < ims.Height(); p.y++)
   for (p.x = 0; p.x < ims.Width(); p.x++)
      {
	 imd[p] = 0;
	 for (int v = 0; v < 26; v++) {
	    Point3d p1 = p + v26[v];
	    if (ims.Hold(p1) && ims[p] != ims[p1] && ims[p] != 0) {
	       imd[p] = Limits<Img3duc::ValueType>::max();
	       break;
	    }
	 }
      } 
   }

   return SUCCESS;
}
Errc PBoundary( const Reg2d &ims, Img2duc &imd, int connexity ) {
   Point2d p;

   if (connexity == 4) {
   for (p.y = 0; p.y < ims.Height(); p.y++)
   for (p.x = 0; p.x < ims.Width(); p.x++)
      {
	 imd[p] = 0;
	 for (int v = 0; v < 4; v++) {
	    Point2d p1 = p + v4[v];
	    if (ims.Hold(p1) && ims[p] != ims[p1] && ims[p] != 0) {
	       imd[p] = Limits<Img2duc::ValueType>::max();
	       break;
	    }
	 }
      } 
   } else {
   for (p.y = 0; p.y < ims.Height(); p.y++)
   for (p.x = 0; p.x < ims.Width(); p.x++)
      {
	 imd[p] = 0;
	 for (int v = 0; v < 8; v++) {
	    Point2d p1 = p + v8[v];
	    if (ims.Hold(p1) && ims[p] != ims[p1] && ims[p] != 0) {
	       imd[p] = Limits<Img2duc::ValueType>::max();
	       break;
	    }
	 }
      } 
   }

   return SUCCESS;
}
Errc PBoundary( const Reg3d &ims, Img3duc &imd, int connexity ) {
   Point3d p;

   if (connexity == 6) {
   for (p.z = 0; p.z < ims.Depth(); p.z++)
   for (p.y = 0; p.y < ims.Height(); p.y++)
   for (p.x = 0; p.x < ims.Width(); p.x++)
      {
	 imd[p] = 0;
	 for (int v = 0; v < 6; v++) {
	    Point3d p1 = p + v6[v];
	    if (ims.Hold(p1) && ims[p] != ims[p1] && ims[p] != 0) {
	       imd[p] = Limits<Img3duc::ValueType>::max();
	       break;
	    }
	 }
      } 
   } else {
   for (p.z = 0; p.z < ims.Depth(); p.z++)
   for (p.y = 0; p.y < ims.Height(); p.y++)
   for (p.x = 0; p.x < ims.Width(); p.x++)
      {
	 imd[p] = 0;
	 for (int v = 0; v < 26; v++) {
	    Point3d p1 = p + v26[v];
	    if (ims.Hold(p1) && ims[p] != ims[p1] && ims[p] != 0) {
	       imd[p] = Limits<Img3duc::ValueType>::max();
	       break;
	    }
	 }
      } 
   }

   return SUCCESS;
}
Errc PBoundary( const Graph2d &grs, Graph2d &grd, int ) {
   GEdge *ptr;
   Graph2d::ValueType u;

   grd = grs;
   // Initialisation a 0
   // sauf les points terminaux qui sont donc sur la bordure.
   for (Long i = 0; i < grs.Size(); i++) {
      if ((grd[i])) {
	 int nbvois = 0;
	 for (ptr = grs[i]->Neighbours(); ptr; ptr = ptr->Next()) {
	    nbvois++;
	 }
	 if (nbvois > 1) {
	    grd[i]->value = 0;
	 } else {
	    grd[i]->value = 255.0;
	 }
      }
   }

   for (Long i = 0; i < grs.Size(); i++) {
      if ((grs[i])) {
	 u = grs[i]->value;
	 for (ptr = grs[i]->Neighbours(); ptr; ptr = ptr->Next()) {
	    if ((grs[ptr->Node()]->value!=u) && (grd[ptr->Node()]->value == 0)) {
	       grd[i]->value = 255.0;
	    }
	 }
      }
   }
   
   return SUCCESS;
}
Errc PBoundary( const Graph3d &grs, Graph3d &grd, int ) {
   GEdge *ptr;
   Graph3d::ValueType u;

   grd = grs;
   // Initialisation a 0
   // sauf les points terminaux qui sont donc sur la bordure.
   for (Long i = 0; i < grs.Size(); i++) {
      if ((grd[i])) {
	 int nbvois = 0;
	 for (ptr = grs[i]->Neighbours(); ptr; ptr = ptr->Next()) {
	    nbvois++;
	 }
	 if (nbvois > 1) {
	    grd[i]->value = 0;
	 } else {
	    grd[i]->value = 255.0;
	 }
      }
   }

   for (Long i = 0; i < grs.Size(); i++) {
      if ((grs[i])) {
	 u = grs[i]->value;
	 for (ptr = grs[i]->Neighbours(); ptr; ptr = ptr->Next()) {
	    if ((grs[ptr->Node()]->value!=u) && (grd[ptr->Node()]->value == 0)) {
	       grd[i]->value = 255.0;
	    }
	 }
      }
   }
   
   return SUCCESS;
}

#ifdef MAIN
#define	USAGE	"usage: %s connexity [-m mask] [im_in|-] [im_out|-]"
#define	PARC	1
#define	FINC	1
#define	FOUTC	1
#define	MASK	2 // Only Masking

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
      objd[0] = new Img2duc(ims->Size());
      Img2duc* const imd = (Img2duc*)objd[0];
      
      result = PBoundary(*ims, *imd, atoi(parv[0]));
      goto end;
   }
   if (objs[0]->Type() == Po_Img2dsl) {
      Img2dsl* const ims = (Img2dsl*)objs[0];
      objd[0] = new Img2duc(ims->Size());
      Img2duc* const imd = (Img2duc*)objd[0];
      
      result = PBoundary(*ims, *imd, atoi(parv[0]));
      goto end;
   }
   if (objs[0]->Type() == Po_Img2dsf) {
      Img2dsf* const ims = (Img2dsf*)objs[0];
      objd[0] = new Img2duc(ims->Size());
      Img2duc* const imd = (Img2duc*)objd[0];
      
      result = PBoundary(*ims, *imd, atoi(parv[0]));
      goto end;
   }
   if (objs[0]->Type() == Po_Img3duc) {
      Img3duc* const ims = (Img3duc*)objs[0];
      objd[0] = new Img3duc(ims->Size());
      Img3duc* const imd = (Img3duc*)objd[0];
      
      result = PBoundary(*ims, *imd, atoi(parv[0]));
      goto end;
   }
   if (objs[0]->Type() == Po_Img3dsl) {
      Img3dsl* const ims = (Img3dsl*)objs[0];
      objd[0] = new Img3duc(ims->Size());
      Img3duc* const imd = (Img3duc*)objd[0];
      
      result = PBoundary(*ims, *imd, atoi(parv[0]));
      goto end;
   }
   if (objs[0]->Type() == Po_Img3dsf) {
      Img3dsf* const ims = (Img3dsf*)objs[0];
      objd[0] = new Img3duc(ims->Size());
      Img3duc* const imd = (Img3duc*)objd[0];
      
      result = PBoundary(*ims, *imd, atoi(parv[0]));
      goto end;
   }
   if (objs[0]->Type() == Po_Reg2d) {
      Reg2d* const ims = (Reg2d*)objs[0];
      objd[0] = new Img2duc(ims->Size());
      Img2duc* const imd = (Img2duc*)objd[0];
      
      result = PBoundary(*ims, *imd, atoi(parv[0]));
      goto end;
   }
   if (objs[0]->Type() == Po_Reg3d) {
      Reg3d* const ims = (Reg3d*)objs[0];
      objd[0] = new Img3duc(ims->Size());
      Img3duc* const imd = (Img3duc*)objd[0];
      
      result = PBoundary(*ims, *imd, atoi(parv[0]));
      goto end;
   }
   if (objs[0]->Type() == Po_Graph2d) {
      Graph2d* const grs = (Graph2d*)objs[0];
      objd[0] = new Graph2d(grs->Size());
      Graph2d* const grd = (Graph2d*)objd[0];
      
      result = PBoundary(*grs, *grd, atoi(parv[0]));
      goto end;
   }
   if (objs[0]->Type() == Po_Graph3d) {
      Graph3d* const grs = (Graph3d*)objs[0];
      objd[0] = new Graph3d(grs->Size());
      Graph3d* const grd = (Graph3d*)objd[0];
      
      result = PBoundary(*grs, *grd, atoi(parv[0]));
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
