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
 * @author Régis Clouard - 1999-02-19
 */

#include <pandore.h>
using namespace pandore;

/**
 * @file plocalextrema.cpp
 * Localisation des extremas des regions.
 * Un point extrema est un point dont
 * l'un de ses voisins a une valeur inferieure.
 */

Errc PLocalExtrema( const Img1duc &ims, Img1duc &imd, int connexite ) {
   int v;
   Point1d p;
      
   if (connexite == 2) {
      int demiv=2/2;
   for (p.x = 0; p.x < ims.Width(); p.x++)
      {
	 imd[p]=0;
	 for (v=0; v<demiv; v++) {
	    if (ims.Hold(p+v2[v]) && ims.Hold(p+v2[v+demiv]) &&
		(((ims[p] >= ims[p+v2[v]]) && (ims[p] > ims[p+v2[v+demiv]])) ||
		 ((ims[p] > ims[p+v2[v]]) && (ims[p] >= ims[p+v2[v+demiv]])))) {
	       imd[p] = ims[p];
	    }
	    break;
	 }
      } 
   } else {
      int demiv = 2 / 2;
   for (p.x = 0; p.x < ims.Width(); p.x++)
      {
	 imd[p] = 0;
	 for (v = 0; v < demiv; v++) {
	    if (ims.Hold(p+v2[v]) && ims.Hold(p+v2[v+demiv]) &&
		(((ims[p] >= ims[p+v2[v]]) && (ims[p] > ims[p+v2[v+demiv]])) ||
		 ((ims[p] > ims[p+v2[v]]) && (ims[p] >= ims[p+v2[v+demiv]])))) {
	       imd[p] = ims[p];
	    }
	    break;
	 }
      }
   }
   
   return SUCCESS;
}
Errc PLocalExtrema( const Img1dsl &ims, Img1dsl &imd, int connexite ) {
   int v;
   Point1d p;
      
   if (connexite == 2) {
      int demiv=2/2;
   for (p.x = 0; p.x < ims.Width(); p.x++)
      {
	 imd[p]=0;
	 for (v=0; v<demiv; v++) {
	    if (ims.Hold(p+v2[v]) && ims.Hold(p+v2[v+demiv]) &&
		(((ims[p] >= ims[p+v2[v]]) && (ims[p] > ims[p+v2[v+demiv]])) ||
		 ((ims[p] > ims[p+v2[v]]) && (ims[p] >= ims[p+v2[v+demiv]])))) {
	       imd[p] = ims[p];
	    }
	    break;
	 }
      } 
   } else {
      int demiv = 2 / 2;
   for (p.x = 0; p.x < ims.Width(); p.x++)
      {
	 imd[p] = 0;
	 for (v = 0; v < demiv; v++) {
	    if (ims.Hold(p+v2[v]) && ims.Hold(p+v2[v+demiv]) &&
		(((ims[p] >= ims[p+v2[v]]) && (ims[p] > ims[p+v2[v+demiv]])) ||
		 ((ims[p] > ims[p+v2[v]]) && (ims[p] >= ims[p+v2[v+demiv]])))) {
	       imd[p] = ims[p];
	    }
	    break;
	 }
      }
   }
   
   return SUCCESS;
}
Errc PLocalExtrema( const Img1dsf &ims, Img1dsf &imd, int connexite ) {
   int v;
   Point1d p;
      
   if (connexite == 2) {
      int demiv=2/2;
   for (p.x = 0; p.x < ims.Width(); p.x++)
      {
	 imd[p]=0;
	 for (v=0; v<demiv; v++) {
	    if (ims.Hold(p+v2[v]) && ims.Hold(p+v2[v+demiv]) &&
		(((ims[p] >= ims[p+v2[v]]) && (ims[p] > ims[p+v2[v+demiv]])) ||
		 ((ims[p] > ims[p+v2[v]]) && (ims[p] >= ims[p+v2[v+demiv]])))) {
	       imd[p] = ims[p];
	    }
	    break;
	 }
      } 
   } else {
      int demiv = 2 / 2;
   for (p.x = 0; p.x < ims.Width(); p.x++)
      {
	 imd[p] = 0;
	 for (v = 0; v < demiv; v++) {
	    if (ims.Hold(p+v2[v]) && ims.Hold(p+v2[v+demiv]) &&
		(((ims[p] >= ims[p+v2[v]]) && (ims[p] > ims[p+v2[v+demiv]])) ||
		 ((ims[p] > ims[p+v2[v]]) && (ims[p] >= ims[p+v2[v+demiv]])))) {
	       imd[p] = ims[p];
	    }
	    break;
	 }
      }
   }
   
   return SUCCESS;
}
Errc PLocalExtrema( const Img2duc &ims, Img2duc &imd, int connexite ) {
   int v;
   Point2d p;
      
   if (connexite == 4) {
      int demiv=4/2;
   for (p.y = 0; p.y < ims.Height(); p.y++)
   for (p.x = 0; p.x < ims.Width(); p.x++)
      {
	 imd[p]=0;
	 for (v=0; v<demiv; v++) {
	    if (ims.Hold(p+v4[v]) && ims.Hold(p+v4[v+demiv]) &&
		(((ims[p] >= ims[p+v4[v]]) && (ims[p] > ims[p+v4[v+demiv]])) ||
		 ((ims[p] > ims[p+v4[v]]) && (ims[p] >= ims[p+v4[v+demiv]])))) {
	       imd[p] = ims[p];
	    }
	    break;
	 }
      } 
   } else {
      int demiv = 8 / 2;
   for (p.y = 0; p.y < ims.Height(); p.y++)
   for (p.x = 0; p.x < ims.Width(); p.x++)
      {
	 imd[p] = 0;
	 for (v = 0; v < demiv; v++) {
	    if (ims.Hold(p+v8[v]) && ims.Hold(p+v8[v+demiv]) &&
		(((ims[p] >= ims[p+v8[v]]) && (ims[p] > ims[p+v8[v+demiv]])) ||
		 ((ims[p] > ims[p+v8[v]]) && (ims[p] >= ims[p+v8[v+demiv]])))) {
	       imd[p] = ims[p];
	    }
	    break;
	 }
      }
   }
   
   return SUCCESS;
}
Errc PLocalExtrema( const Img2dsl &ims, Img2dsl &imd, int connexite ) {
   int v;
   Point2d p;
      
   if (connexite == 4) {
      int demiv=4/2;
   for (p.y = 0; p.y < ims.Height(); p.y++)
   for (p.x = 0; p.x < ims.Width(); p.x++)
      {
	 imd[p]=0;
	 for (v=0; v<demiv; v++) {
	    if (ims.Hold(p+v4[v]) && ims.Hold(p+v4[v+demiv]) &&
		(((ims[p] >= ims[p+v4[v]]) && (ims[p] > ims[p+v4[v+demiv]])) ||
		 ((ims[p] > ims[p+v4[v]]) && (ims[p] >= ims[p+v4[v+demiv]])))) {
	       imd[p] = ims[p];
	    }
	    break;
	 }
      } 
   } else {
      int demiv = 8 / 2;
   for (p.y = 0; p.y < ims.Height(); p.y++)
   for (p.x = 0; p.x < ims.Width(); p.x++)
      {
	 imd[p] = 0;
	 for (v = 0; v < demiv; v++) {
	    if (ims.Hold(p+v8[v]) && ims.Hold(p+v8[v+demiv]) &&
		(((ims[p] >= ims[p+v8[v]]) && (ims[p] > ims[p+v8[v+demiv]])) ||
		 ((ims[p] > ims[p+v8[v]]) && (ims[p] >= ims[p+v8[v+demiv]])))) {
	       imd[p] = ims[p];
	    }
	    break;
	 }
      }
   }
   
   return SUCCESS;
}
Errc PLocalExtrema( const Img2dsf &ims, Img2dsf &imd, int connexite ) {
   int v;
   Point2d p;
      
   if (connexite == 4) {
      int demiv=4/2;
   for (p.y = 0; p.y < ims.Height(); p.y++)
   for (p.x = 0; p.x < ims.Width(); p.x++)
      {
	 imd[p]=0;
	 for (v=0; v<demiv; v++) {
	    if (ims.Hold(p+v4[v]) && ims.Hold(p+v4[v+demiv]) &&
		(((ims[p] >= ims[p+v4[v]]) && (ims[p] > ims[p+v4[v+demiv]])) ||
		 ((ims[p] > ims[p+v4[v]]) && (ims[p] >= ims[p+v4[v+demiv]])))) {
	       imd[p] = ims[p];
	    }
	    break;
	 }
      } 
   } else {
      int demiv = 8 / 2;
   for (p.y = 0; p.y < ims.Height(); p.y++)
   for (p.x = 0; p.x < ims.Width(); p.x++)
      {
	 imd[p] = 0;
	 for (v = 0; v < demiv; v++) {
	    if (ims.Hold(p+v8[v]) && ims.Hold(p+v8[v+demiv]) &&
		(((ims[p] >= ims[p+v8[v]]) && (ims[p] > ims[p+v8[v+demiv]])) ||
		 ((ims[p] > ims[p+v8[v]]) && (ims[p] >= ims[p+v8[v+demiv]])))) {
	       imd[p] = ims[p];
	    }
	    break;
	 }
      }
   }
   
   return SUCCESS;
}
Errc PLocalExtrema( const Img3duc &ims, Img3duc &imd, int connexite ) {
   int v;
   Point3d p;
      
   if (connexite == 6) {
      int demiv=6/2;
   for (p.z = 0; p.z < ims.Depth(); p.z++)
   for (p.y = 0; p.y < ims.Height(); p.y++)
   for (p.x = 0; p.x < ims.Width(); p.x++)
      {
	 imd[p]=0;
	 for (v=0; v<demiv; v++) {
	    if (ims.Hold(p+v6[v]) && ims.Hold(p+v6[v+demiv]) &&
		(((ims[p] >= ims[p+v6[v]]) && (ims[p] > ims[p+v6[v+demiv]])) ||
		 ((ims[p] > ims[p+v6[v]]) && (ims[p] >= ims[p+v6[v+demiv]])))) {
	       imd[p] = ims[p];
	    }
	    break;
	 }
      } 
   } else {
      int demiv = 26 / 2;
   for (p.z = 0; p.z < ims.Depth(); p.z++)
   for (p.y = 0; p.y < ims.Height(); p.y++)
   for (p.x = 0; p.x < ims.Width(); p.x++)
      {
	 imd[p] = 0;
	 for (v = 0; v < demiv; v++) {
	    if (ims.Hold(p+v26[v]) && ims.Hold(p+v26[v+demiv]) &&
		(((ims[p] >= ims[p+v26[v]]) && (ims[p] > ims[p+v26[v+demiv]])) ||
		 ((ims[p] > ims[p+v26[v]]) && (ims[p] >= ims[p+v26[v+demiv]])))) {
	       imd[p] = ims[p];
	    }
	    break;
	 }
      }
   }
   
   return SUCCESS;
}
Errc PLocalExtrema( const Img3dsl &ims, Img3dsl &imd, int connexite ) {
   int v;
   Point3d p;
      
   if (connexite == 6) {
      int demiv=6/2;
   for (p.z = 0; p.z < ims.Depth(); p.z++)
   for (p.y = 0; p.y < ims.Height(); p.y++)
   for (p.x = 0; p.x < ims.Width(); p.x++)
      {
	 imd[p]=0;
	 for (v=0; v<demiv; v++) {
	    if (ims.Hold(p+v6[v]) && ims.Hold(p+v6[v+demiv]) &&
		(((ims[p] >= ims[p+v6[v]]) && (ims[p] > ims[p+v6[v+demiv]])) ||
		 ((ims[p] > ims[p+v6[v]]) && (ims[p] >= ims[p+v6[v+demiv]])))) {
	       imd[p] = ims[p];
	    }
	    break;
	 }
      } 
   } else {
      int demiv = 26 / 2;
   for (p.z = 0; p.z < ims.Depth(); p.z++)
   for (p.y = 0; p.y < ims.Height(); p.y++)
   for (p.x = 0; p.x < ims.Width(); p.x++)
      {
	 imd[p] = 0;
	 for (v = 0; v < demiv; v++) {
	    if (ims.Hold(p+v26[v]) && ims.Hold(p+v26[v+demiv]) &&
		(((ims[p] >= ims[p+v26[v]]) && (ims[p] > ims[p+v26[v+demiv]])) ||
		 ((ims[p] > ims[p+v26[v]]) && (ims[p] >= ims[p+v26[v+demiv]])))) {
	       imd[p] = ims[p];
	    }
	    break;
	 }
      }
   }
   
   return SUCCESS;
}
Errc PLocalExtrema( const Img3dsf &ims, Img3dsf &imd, int connexite ) {
   int v;
   Point3d p;
      
   if (connexite == 6) {
      int demiv=6/2;
   for (p.z = 0; p.z < ims.Depth(); p.z++)
   for (p.y = 0; p.y < ims.Height(); p.y++)
   for (p.x = 0; p.x < ims.Width(); p.x++)
      {
	 imd[p]=0;
	 for (v=0; v<demiv; v++) {
	    if (ims.Hold(p+v6[v]) && ims.Hold(p+v6[v+demiv]) &&
		(((ims[p] >= ims[p+v6[v]]) && (ims[p] > ims[p+v6[v+demiv]])) ||
		 ((ims[p] > ims[p+v6[v]]) && (ims[p] >= ims[p+v6[v+demiv]])))) {
	       imd[p] = ims[p];
	    }
	    break;
	 }
      } 
   } else {
      int demiv = 26 / 2;
   for (p.z = 0; p.z < ims.Depth(); p.z++)
   for (p.y = 0; p.y < ims.Height(); p.y++)
   for (p.x = 0; p.x < ims.Width(); p.x++)
      {
	 imd[p] = 0;
	 for (v = 0; v < demiv; v++) {
	    if (ims.Hold(p+v26[v]) && ims.Hold(p+v26[v+demiv]) &&
		(((ims[p] >= ims[p+v26[v]]) && (ims[p] > ims[p+v26[v+demiv]])) ||
		 ((ims[p] > ims[p+v26[v]]) && (ims[p] >= ims[p+v26[v+demiv]])))) {
	       imd[p] = ims[p];
	    }
	    break;
	 }
      }
   }
   
   return SUCCESS;
}

#ifdef MAIN
#define	USAGE	"usage: %s connectivity [-m mask] [im_in|-] [im_out|-]"
#define	PARC	1
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
   if (objs[0]->Type() == Po_Img1duc) {
      Img1duc* const ims=(Img1duc*)objs[0];
      objd[0]=new Img1duc(ims->Props());
      Img1duc* const imd=(Img1duc*)objd[0];
      
      result=PLocalExtrema(*ims,*imd,atoi(parv[0]));
      goto end;
   }
   if (objs[0]->Type() == Po_Img1dsl) {
      Img1dsl* const ims=(Img1dsl*)objs[0];
      objd[0]=new Img1dsl(ims->Props());
      Img1dsl* const imd=(Img1dsl*)objd[0];
      
      result=PLocalExtrema(*ims,*imd,atoi(parv[0]));
      goto end;
   }
   if (objs[0]->Type() == Po_Img1dsf) {
      Img1dsf* const ims=(Img1dsf*)objs[0];
      objd[0]=new Img1dsf(ims->Props());
      Img1dsf* const imd=(Img1dsf*)objd[0];
      
      result=PLocalExtrema(*ims,*imd,atoi(parv[0]));
      goto end;
   }
   if (objs[0]->Type() == Po_Img2duc) {
      Img2duc* const ims=(Img2duc*)objs[0];
      objd[0]=new Img2duc(ims->Props());
      Img2duc* const imd=(Img2duc*)objd[0];
      
      result=PLocalExtrema(*ims,*imd,atoi(parv[0]));
      goto end;
   }
   if (objs[0]->Type() == Po_Img2dsl) {
      Img2dsl* const ims=(Img2dsl*)objs[0];
      objd[0]=new Img2dsl(ims->Props());
      Img2dsl* const imd=(Img2dsl*)objd[0];
      
      result=PLocalExtrema(*ims,*imd,atoi(parv[0]));
      goto end;
   }
   if (objs[0]->Type() == Po_Img2dsf) {
      Img2dsf* const ims=(Img2dsf*)objs[0];
      objd[0]=new Img2dsf(ims->Props());
      Img2dsf* const imd=(Img2dsf*)objd[0];
      
      result=PLocalExtrema(*ims,*imd,atoi(parv[0]));
      goto end;
   }
   if (objs[0]->Type() == Po_Img3duc) {
      Img3duc* const ims=(Img3duc*)objs[0];
      objd[0]=new Img3duc(ims->Props());
      Img3duc* const imd=(Img3duc*)objd[0];
      
      result=PLocalExtrema(*ims,*imd,atoi(parv[0]));
      goto end;
   }
   if (objs[0]->Type() == Po_Img3dsl) {
      Img3dsl* const ims=(Img3dsl*)objs[0];
      objd[0]=new Img3dsl(ims->Props());
      Img3dsl* const imd=(Img3dsl*)objd[0];
      
      result=PLocalExtrema(*ims,*imd,atoi(parv[0]));
      goto end;
   }
   if (objs[0]->Type() == Po_Img3dsf) {
      Img3dsf* const ims=(Img3dsf*)objs[0];
      objd[0]=new Img3dsf(ims->Props());
      Img3dsf* const imd=(Img3dsf*)objd[0];
      
      result=PLocalExtrema(*ims,*imd,atoi(parv[0]));
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
