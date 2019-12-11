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
 * @author Régis Clouard - 1999-02-19
 */

#include <pandore.h>
using namespace pandore;

/**
 * @file pzerocross.cpp
 * Localisation des passages par une valeur donnee.
 * Pour chaque point, si l'un de ses voisins n'est pas
 * du meme signe (i.e. inferieur ou superieur)
 * mettre le point a VRAI, sinon a FAUX.
 */



Errc PZeroCross( const Img2duc &ims, Img2duc &imd, int connexite, Img2duc::ValueType valeur ) {
   int v;
   Point2d p;

   if (connexite == 4) {
   for (p.y = 0; p.y < ims.Height(); p.y++)
   for (p.x = 0; p.x < ims.Width(); p.x++)
      {
	 imd[p]=0;
	 for (v=0; v<4/2; v++) {
	    if (ims.Hold(p+v4[v]) &&
		(
		 ((ims[p]<=valeur) && (ims[p+v4[v]]>valeur) && (imd[p+v4[v]]==0))
		 ||
		 ((ims[p]>valeur) && (ims[p+v4[v]]<=valeur) && (imd[p+v4[v]]==0))
		 )
		) {
	       imd[p] = Limits<Img2duc::ValueType>::max();
	       break;
	    }
	 }
      }
   }  else {
   for (p.y = 0; p.y < ims.Height(); p.y++)
   for (p.x = 0; p.x < ims.Width(); p.x++)
      {
	 imd[p]=0;
	 for (v=0; v<8/2; v++) {
	    if (ims.Hold(p+v8[v]) &&
		(((ims[p]<=valeur) && (ims[p+v8[v]]>valeur) && (imd[p+v8[v]] == 0))
		 ||
		 ((ims[p]>valeur) && (ims[p+v8[v]]<=valeur) && (imd[p+v8[v]] == 0))
		 )) {
	       imd[p] = Limits<Img2duc::ValueType>::max();
	       break;
	    }
	 }
      }
   }
   return SUCCESS;
}

Errc PZeroCross( const Img2dsl &ims, Img2duc &imd, int connexite, Img2dsl::ValueType valeur ) {
   int v;
   Point2d p;

   if (connexite == 4) {
   for (p.y = 0; p.y < ims.Height(); p.y++)
   for (p.x = 0; p.x < ims.Width(); p.x++)
      {
	 imd[p]=0;
	 for (v=0; v<4/2; v++) {
	    if (ims.Hold(p+v4[v]) &&
		(
		 ((ims[p]<=valeur) && (ims[p+v4[v]]>valeur) && (imd[p+v4[v]]==0))
		 ||
		 ((ims[p]>valeur) && (ims[p+v4[v]]<=valeur) && (imd[p+v4[v]]==0))
		 )
		) {
	       imd[p] = Limits<Img2duc::ValueType>::max();
	       break;
	    }
	 }
      }
   }  else {
   for (p.y = 0; p.y < ims.Height(); p.y++)
   for (p.x = 0; p.x < ims.Width(); p.x++)
      {
	 imd[p]=0;
	 for (v=0; v<8/2; v++) {
	    if (ims.Hold(p+v8[v]) &&
		(((ims[p]<=valeur) && (ims[p+v8[v]]>valeur) && (imd[p+v8[v]] == 0))
		 ||
		 ((ims[p]>valeur) && (ims[p+v8[v]]<=valeur) && (imd[p+v8[v]] == 0))
		 )) {
	       imd[p] = Limits<Img2duc::ValueType>::max();
	       break;
	    }
	 }
      }
   }
   return SUCCESS;
}

Errc PZeroCross( const Img2dsf &ims, Img2duc &imd, int connexite, Img2dsf::ValueType valeur ) {
   int v;
   Point2d p;

   if (connexite == 4) {
   for (p.y = 0; p.y < ims.Height(); p.y++)
   for (p.x = 0; p.x < ims.Width(); p.x++)
      {
	 imd[p]=0;
	 for (v=0; v<4/2; v++) {
	    if (ims.Hold(p+v4[v]) &&
		(
		 ((ims[p]<=valeur) && (ims[p+v4[v]]>valeur) && (imd[p+v4[v]]==0))
		 ||
		 ((ims[p]>valeur) && (ims[p+v4[v]]<=valeur) && (imd[p+v4[v]]==0))
		 )
		) {
	       imd[p] = Limits<Img2duc::ValueType>::max();
	       break;
	    }
	 }
      }
   }  else {
   for (p.y = 0; p.y < ims.Height(); p.y++)
   for (p.x = 0; p.x < ims.Width(); p.x++)
      {
	 imd[p]=0;
	 for (v=0; v<8/2; v++) {
	    if (ims.Hold(p+v8[v]) &&
		(((ims[p]<=valeur) && (ims[p+v8[v]]>valeur) && (imd[p+v8[v]] == 0))
		 ||
		 ((ims[p]>valeur) && (ims[p+v8[v]]<=valeur) && (imd[p+v8[v]] == 0))
		 )) {
	       imd[p] = Limits<Img2duc::ValueType>::max();
	       break;
	    }
	 }
      }
   }
   return SUCCESS;
}

Errc PZeroCross( const Img3duc &ims, Img3duc &imd, int connexite, Img3duc::ValueType valeur ) {
   int v;
   Point3d p;

   if (connexite == 6) {
   for (p.z = 0; p.z < ims.Depth(); p.z++)
   for (p.y = 0; p.y < ims.Height(); p.y++)
   for (p.x = 0; p.x < ims.Width(); p.x++)
      {
	 imd[p]=0;
	 for (v=0; v<6/2; v++) {
	    if (ims.Hold(p+v6[v]) &&
		(
		 ((ims[p]<=valeur) && (ims[p+v6[v]]>valeur) && (imd[p+v6[v]]==0))
		 ||
		 ((ims[p]>valeur) && (ims[p+v6[v]]<=valeur) && (imd[p+v6[v]]==0))
		 )
		) {
	       imd[p] = Limits<Img3duc::ValueType>::max();
	       break;
	    }
	 }
      }
   }  else {
   for (p.z = 0; p.z < ims.Depth(); p.z++)
   for (p.y = 0; p.y < ims.Height(); p.y++)
   for (p.x = 0; p.x < ims.Width(); p.x++)
      {
	 imd[p]=0;
	 for (v=0; v<26/2; v++) {
	    if (ims.Hold(p+v26[v]) &&
		(((ims[p]<=valeur) && (ims[p+v26[v]]>valeur) && (imd[p+v26[v]] == 0))
		 ||
		 ((ims[p]>valeur) && (ims[p+v26[v]]<=valeur) && (imd[p+v26[v]] == 0))
		 )) {
	       imd[p] = Limits<Img3duc::ValueType>::max();
	       break;
	    }
	 }
      }
   }
   return SUCCESS;
}

Errc PZeroCross( const Img3dsl &ims, Img3duc &imd, int connexite, Img3dsl::ValueType valeur ) {
   int v;
   Point3d p;

   if (connexite == 6) {
   for (p.z = 0; p.z < ims.Depth(); p.z++)
   for (p.y = 0; p.y < ims.Height(); p.y++)
   for (p.x = 0; p.x < ims.Width(); p.x++)
      {
	 imd[p]=0;
	 for (v=0; v<6/2; v++) {
	    if (ims.Hold(p+v6[v]) &&
		(
		 ((ims[p]<=valeur) && (ims[p+v6[v]]>valeur) && (imd[p+v6[v]]==0))
		 ||
		 ((ims[p]>valeur) && (ims[p+v6[v]]<=valeur) && (imd[p+v6[v]]==0))
		 )
		) {
	       imd[p] = Limits<Img3duc::ValueType>::max();
	       break;
	    }
	 }
      }
   }  else {
   for (p.z = 0; p.z < ims.Depth(); p.z++)
   for (p.y = 0; p.y < ims.Height(); p.y++)
   for (p.x = 0; p.x < ims.Width(); p.x++)
      {
	 imd[p]=0;
	 for (v=0; v<26/2; v++) {
	    if (ims.Hold(p+v26[v]) &&
		(((ims[p]<=valeur) && (ims[p+v26[v]]>valeur) && (imd[p+v26[v]] == 0))
		 ||
		 ((ims[p]>valeur) && (ims[p+v26[v]]<=valeur) && (imd[p+v26[v]] == 0))
		 )) {
	       imd[p] = Limits<Img3duc::ValueType>::max();
	       break;
	    }
	 }
      }
   }
   return SUCCESS;
}

Errc PZeroCross( const Img3dsf &ims, Img3duc &imd, int connexite, Img3dsf::ValueType valeur ) {
   int v;
   Point3d p;

   if (connexite == 6) {
   for (p.z = 0; p.z < ims.Depth(); p.z++)
   for (p.y = 0; p.y < ims.Height(); p.y++)
   for (p.x = 0; p.x < ims.Width(); p.x++)
      {
	 imd[p]=0;
	 for (v=0; v<6/2; v++) {
	    if (ims.Hold(p+v6[v]) &&
		(
		 ((ims[p]<=valeur) && (ims[p+v6[v]]>valeur) && (imd[p+v6[v]]==0))
		 ||
		 ((ims[p]>valeur) && (ims[p+v6[v]]<=valeur) && (imd[p+v6[v]]==0))
		 )
		) {
	       imd[p] = Limits<Img3duc::ValueType>::max();
	       break;
	    }
	 }
      }
   }  else {
   for (p.z = 0; p.z < ims.Depth(); p.z++)
   for (p.y = 0; p.y < ims.Height(); p.y++)
   for (p.x = 0; p.x < ims.Width(); p.x++)
      {
	 imd[p]=0;
	 for (v=0; v<26/2; v++) {
	    if (ims.Hold(p+v26[v]) &&
		(((ims[p]<=valeur) && (ims[p+v26[v]]>valeur) && (imd[p+v26[v]] == 0))
		 ||
		 ((ims[p]>valeur) && (ims[p+v26[v]]<=valeur) && (imd[p+v26[v]] == 0))
		 )) {
	       imd[p] = Limits<Img3duc::ValueType>::max();
	       break;
	    }
	 }
      }
   }
   return SUCCESS;
}

Errc PZeroCross( const Graph2d &grs, Graph2d &grd, int /*connexite*/, Graph2d::ValueType valeur ) {
   Long i;
   GEdge *ptr;
   Graph2d::ValueType u;
   
   grd=grs;
   // Initialisation a 0
   // sauf les points terminaux qui sont donc sur la bordure.
   for (i=0;i<grs.Size();i++) {
      if ((grd[i])) {
	 grd[i]->value=0;
      }
   }
   
   for (i=0;i<grs.Size();i++) {
      if ((grs[i])) {
	 u=grs[i]->value;
	 for (ptr=grs[i]->Neighbours();ptr;ptr=ptr->Next())
	    if (( (u> valeur) && (grs[ptr->Node()]->value<valeur) && (grd[ptr->Node()]->value==0))
		||
		(( (u< valeur) && (grs[ptr->Node()]->value>valeur) && (grd[ptr->Node()]->value==0))))
	       grd[i]->value=255.0;
      }
   }
   
   return SUCCESS;
}

Errc PZeroCross( const Graph3d &grs, Graph3d &grd, int /*connexite*/, Graph3d::ValueType valeur ) {
   Long i;
   GEdge *ptr;
   Graph3d::ValueType u;
   
   grd=grs;
   // Initialisation a 0
   // sauf les points terminaux qui sont donc sur la bordure.
   for (i=0;i<grs.Size();i++) {
      if ((grd[i])) {
	 grd[i]->value=0;
      }
   }
   
   for (i=0;i<grs.Size();i++) {
      if ((grs[i])) {
	 u=grs[i]->value;
	 for (ptr=grs[i]->Neighbours();ptr;ptr=ptr->Next())
	    if (( (u> valeur) && (grs[ptr->Node()]->value<valeur) && (grd[ptr->Node()]->value==0))
		||
		(( (u< valeur) && (grs[ptr->Node()]->value>valeur) && (grd[ptr->Node()]->value==0))))
	       grd[i]->value=255.0;
      }
   }
   
   return SUCCESS;
}


#ifdef MAIN
#define	USAGE	"usage: %s connectivity threshold [-m mask] [im_in|-] [im_out|-]"
#define	PARC	2
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
      Img2duc* const ims=(Img2duc*)objs[0];
      objd[0]=new Img2duc(ims->Size());
      Img2duc* const imd=(Img2duc*)objd[0];
      
      result=PZeroCross(*ims,*imd,atoi(parv[0]), (Img2duc::ValueType)atof(parv[1]));
      goto end;
   }
   if (objs[0]->Type() == Po_Img2dsl) {
      Img2dsl* const ims=(Img2dsl*)objs[0];
      objd[0]=new Img2duc(ims->Size());
      Img2duc* const imd=(Img2duc*)objd[0];
      
      result=PZeroCross(*ims,*imd,atoi(parv[0]), (Img2dsl::ValueType)atof(parv[1]));
      goto end;
   }
   if (objs[0]->Type() == Po_Img2dsf) {
      Img2dsf* const ims=(Img2dsf*)objs[0];
      objd[0]=new Img2duc(ims->Size());
      Img2duc* const imd=(Img2duc*)objd[0];
      
      result=PZeroCross(*ims,*imd,atoi(parv[0]), (Img2dsf::ValueType)atof(parv[1]));
      goto end;
   }
   if (objs[0]->Type() == Po_Img3duc) {
      Img3duc* const ims=(Img3duc*)objs[0];
      objd[0]=new Img3duc(ims->Size());
      Img3duc* const imd=(Img3duc*)objd[0];
      
      result=PZeroCross(*ims,*imd,atoi(parv[0]), (Img3duc::ValueType)atof(parv[1]));
      goto end;
   }
   if (objs[0]->Type() == Po_Img3dsl) {
      Img3dsl* const ims=(Img3dsl*)objs[0];
      objd[0]=new Img3duc(ims->Size());
      Img3duc* const imd=(Img3duc*)objd[0];
      
      result=PZeroCross(*ims,*imd,atoi(parv[0]), (Img3dsl::ValueType)atof(parv[1]));
      goto end;
   }
   if (objs[0]->Type() == Po_Img3dsf) {
      Img3dsf* const ims=(Img3dsf*)objs[0];
      objd[0]=new Img3duc(ims->Size());
      Img3duc* const imd=(Img3duc*)objd[0];
      
      result=PZeroCross(*ims,*imd,atoi(parv[0]), (Img3dsf::ValueType)atof(parv[1]));
      goto end;
   }
   if (objs[0]->Type() == Po_Graph2d) {
      Graph2d* const grs=(Graph2d*)objs[0];
      objd[0]=new Graph2d(grs->Size());
      Graph2d* const grd=(Graph2d*)objd[0];
      
      result=PZeroCross(*grs,*grd,atoi(parv[0]),(Graph2d::ValueType)atof(parv[1]));
      goto end;
   }
   if (objs[0]->Type() == Po_Graph3d) {
      Graph3d* const grs=(Graph3d*)objs[0];
      objd[0]=new Graph3d(grs->Size());
      Graph3d* const grd=(Graph3d*)objd[0];
      
      result=PZeroCross(*grs,*grd,atoi(parv[0]),(Graph3d::ValueType)atof(parv[1]));
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
