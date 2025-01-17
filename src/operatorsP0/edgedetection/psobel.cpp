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
 * @author Regis Clouard - 1997-07-16
 */

#include <cmath>
#include <pandore.h>
using namespace pandore;

/**
 * @file psobel.cpp
 *
 * Approximation of the gradient The Sobel's method.
 */

static short sobel2d[2][3][3] = {
   {{+1,   0,  -1}, 
    {+2,   0,  -2}, 
    {+1,   0,  -1}}, 
   
   {{+1,  +2,  +1}, 
    { 0,   0,   0}, 
    {-1,  -2,  -1}}};

template <typename T>
Errc PSobel( const Img2d<T> &ims,  Img2d<T> &imd ) {
   // Border
   Border2DIterator *it = ims.getBorderIterator(1, 1, 1, 1);
   double g[2];
   while (it->hasNext()) {
      Point2d p = it->next();
      // Convolution
      g[0] = 0;
      if (p.x > 0 && p.x < ims.Width() - 1) {
	 for (int k = -1; k <= 1; k++) {
	    for (int l = -1; l <= 1; l++) {
	       if (ims.Hold(p.y + k, p.x + l)) {
		  g[0] += ims(p.y + k, p.x + l) * (double)sobel2d[0][k + 1][l + 1];
	       }
	    }
	 }
      }
      g[1] = 0;
      if (p.y > 0 &&  p.y < ims.Height() - 1) {
	 for (int k = -1; k <= 1; k++) {
	    for (int l = -1; l <= 1; l++) {
	       if (ims.Hold(p.y + k, p.x + l)) {
		  g[1] += ims(p.y + k, p.x + l) * (double)sobel2d[1][k + 1][l + 1];
	       }
	    }
	 }
      }
      
      imd[p] = (T)((fabs(g[0]) + fabs(g[1])) / (2.0 * 4.0));
   }

   // General 
   Point2d p;
   for (p.y = 1; p.y < ims.Height() - 1; p.y++) {
      for (p.x = 1; p.x < ims.Width() - 1; p.x++) {
	 double amplitude = 0.0;
	 for (int i = 0; i < 2; i++) {
	    // Convolution
	    g[i] = 0;
	    for (int k = -1; k <= 1; k++) {
	       for (int l = -1; l <= 1; l++) {
		  g[i] += ims(p.y + k, p.x + l) * (double)sobel2d[i][k + 1][l + 1];
	       }
	    }
	    g[i] = fabs(g[i]);
	 }
	 amplitude = (g[0] + g[1]) / (2.0 * 4.0);
	 imd[p] = (T)amplitude;
      }
   }
   delete it;

   return SUCCESS;
}

static short sobel3d[3][3][3][3] = {
   {{{+1,   0,  -1},  // Gx
     {+2,   0,  -2}, 
     {+1,   0,  -1}}, 
    {{+2,   0,  -2}, 
     {+4,   0,  -4}, 
     {+2,   0,  -2}}, 
    {{+1,   0,  -1}, 
     {+2,   0,  -2}, 
     {+1,   0,  -1}}}, 

   {{{+1,   2,  +1},  // Gy
     {+0,   0,   0}, 
     {-1,  -2,  -1}}, 
    {{+2,   4,   2}, 
     { 0,   0,   0}, 
     {-2,  -4,  -2}}, 
    {{+1,   2,   1}, 
     { 0,   0,   0}, 
     {-1,  -2,  -1}}}, 
   
   {{{+1,   2,   1},  // Gz
     {+2,   4,   2}, 
     {+1,   2,   1}}, 
    {{ 0,   0,   0}, 
     { 0,   0,   0}, 
     { 0,   0,   0}}, 
    {{-1,  -2,  -1}, 
     {-2,  -4,  -2}, 
     {-1,  -2,  -1}}} };

template <typename T>
Errc PSobel( const Img3d<T> &ims,  Img3d<T> &imd ) {
  // Border
   Border3DIterator *it = ims.getBorderIterator(1, 1, 1, 1, 1, 1);
   double g[3];

   while( it->hasNext()) {
      Point3d p = it->next();
      // Convolution
      for (int i = 0; i < 3; i++) {
	 // Convolution
	 g[i] = 0;
	 Point3d q;
	 for (int k = -1; k <= 1; k++) {
	    if (p.z + k < 0 || p.z + k > ims.Depth() - 1) {
	       q.z = p.z;
	    } else {
	       q.z = p.z + k;
	    }
	    for (int l = -1; l <= 1; l++) {
	       if (p.y + l < 0 || p.y + l > ims.Height() - 1) {
		  q.y = p.y; 
	       } else {
		  q.y = p.y + l;
	       }
	       for (int m =-1; m <= 1; m++) {
		  if (p.x + m < 0 || p.x + m > ims.Width() - 1) {
		     q.x = p.x;
		  } else {
		     q.x = p.x + m;
		  }
		  g[i] += ims(q.z, q.y, q.x) * (double)sobel3d[i][k + 1][l + 1][m + 1];
	       }
	    }
	 }
      }
      imd[p] = (T)((fabs(g[0]) + fabs(g[1]) + fabs(g[2])) / (3.0 * 12.0));
   }
   
   Point3d p;
   for (p.z = 1; p.z < ims.Depth() - 1; p.z++) {
      for (p.y = 1; p.y < ims.Height() - 1; p.y++) {
	 for (p.x = 1; p.x < ims.Width() - 1; p.x++) {
	    for (int i = 0; i < 3; i++) {
	       // Convolution
	       g[i] = 0;
	       for (int k = -1; k <= 1; k++) {
		  for (int l = -1; l <= 1; l++) {
		     for (int m = -1; m <= 1; m++) {
			g[i] += ims(p.z + k, p.y + l, p.x + m) * (double)sobel3d[i][k + 1][l + 1][m + 1];
		     }
		  }
	       }
	       g[i] = fabs(g[i]);
	    }
	    imd[p] = (T)((g[0] + g[1] + g[2]) / (3.0  *12.0));
	 }
      }
   }
   delete it;

   return SUCCESS;
}



#ifdef MAIN
#define	USAGE	"usage: %s [-m mask] [im_src|-] [im_out|-]"
#define	PARC	0
#define	FINC	1
#define	FOUTC	1
#define MASK	2

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
      
      result = PSobel(*ims, *imd);
      goto end;
   }

   if (objs[0]->Type() == Po_Img2dsl) {
      Img2dsl* const ims = (Img2dsl*)objs[0];
      objd[0] = new Img2dsl(ims->Size());		  
      Img2dsl* const imd = (Img2dsl*)objd[0];
      
      result = PSobel(*ims, *imd);
      goto end;
   }

   if (objs[0]->Type() == Po_Img2dsf) {
      Img2dsf* const ims = (Img2dsf*)objs[0];
      objd[0] = new Img2dsf(ims->Size());		  
      Img2dsf* const imd = (Img2dsf*)objd[0];
      
      result = PSobel(*ims, *imd);
      goto end;
   }

   if (objs[0]->Type() == Po_Img3duc) {
      Img3duc* const ims = (Img3duc*)objs[0];
      objd[0] = new Img3duc(ims->Size());		  
      Img3duc* const imd = (Img3duc*)objd[0];
      
      result = PSobel(*ims, *imd);
      goto end;
   }

   if (objs[0]->Type() == Po_Img3dsl) {
      Img3dsl* const ims = (Img3dsl*)objs[0];
      objd[0] = new Img3dsl(ims->Size());		  
      Img3dsl* const imd = (Img3dsl*)objd[0];
      
      result = PSobel(*ims, *imd);
      goto end;
   }

   if (objs[0]->Type() == Po_Img3dsf) {
      Img3dsf* const ims = (Img3dsf*)objs[0];
      objd[0] = new Img3dsf(ims->Size());		  
      Img3dsf* const imd = (Img3dsf*)objd[0];
      
      result = PSobel(*ims, *imd);
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
