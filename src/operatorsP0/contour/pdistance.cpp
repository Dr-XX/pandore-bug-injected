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
 * @author Abderrahim Elmoataz - 2002-12-31
 * @author Jean-Marie Janik - 2002-12-31
 * @author Sophie Schupp - 2002-12-31
 * @author Jean-Marie Janik - 2005-04-26 (new algorithm for 2D images)
 * @author Sebastien Fourey - 2013-03-00 (Single function for both 2D and 3D images
 *                                        using Meijster's. See credits below.)
 */

#include <pandore.h>
#include <vector>
#include <limits>
#include <cmath>

using namespace pandore;

/**
 * @file pdistance.cc
 * Computes the distance in each point to the nearest contour point.
 * This operator is the implementation of the Meijster's
 * algorithm for computing the euclidean distance transform in linear time.
 *
 * Credits: This implementation is almost a verbatim copy of the code written
 * by Meijster himself for CImg (http://cimg.sourceforge.net). Handling of infinity
 * is an idea of David Coeurjolly in his implementation of the 3D squared
 * Euclidean distance transform (see http://www.tc18.org/code_data_set/code.html).
 *
 * Source : A. Meijster, J.B.T.M. Roerdink, and W.H. Hesselink: A general
 * algorithm for computing distance transforms in linear time.
 * In: Mathematical Morphology and its Applications to Image and
 * Signal Processing, Kluwer, 2000, pp. 331-340.
 */

namespace {
   const Long Infinity = std::numeric_limits<Long>::max();

   inline Long sum( const Long & a, const Long & b ) {
      return ((a == Infinity) || (b == Infinity)) ? Infinity : (a + b);
   }

   inline Long product( const Long & a, const Long & b ) {
      return ((a == Infinity) || (b == Infinity)) ? Infinity : (a * b);
   }

   inline Long opposite( const Long & a ) {
      return (a == Infinity) ? Infinity : -a;
   }

   inline Long quotient( const Long & dividend, const Long & divisor ) {
      return ((divisor == 0) || (dividend == Infinity)) ? Infinity : (dividend / divisor);
   }

   static long sep_dt( const Long & i, const Long & u, const std::vector<Long> & g ) {
      return quotient(sum(sum(u * u - i * i, g[u]), opposite(g[i])), 2 * (u - i));
   }

   static long f_dt( const Long & x, const Long & i, const std::vector<Long> & g ) {
      return sum((x - i) * (x - i), g[i]);
   }
   
   inline void distance_scan( const Long & len,
			      const std::vector<Long> & g,
			      std::vector<Long> & s,
			      std::vector<Long> & t,
			      std::vector<Long> & dt ) {
      Long q = s[0] = t[0] = 0;
      // Forward scan
      for (Long u = 1; u < len; ++u) {
	    while ((q >= 0) && (f_dt(t[q], s[q], g) > f_dt(t[q], u, g))) {
	       --q;
	    }
	    if (q < 0) { 
	       q = 0;
	       s[0] = u; 
	    } else {
	       const Long w = sum(1, sep_dt(s[q], u, g));
	       if (w < len) {
		  ++q;
		  s[q] = u;
		  t[q] = w; 
	       }
	    }
      }
      // Backward scan
      for (Long u = len - 1; u >= 0; --u) { 
	 dt[u] = f_dt(u, s[q], g); 
	 if (u == t[q]) {
	    --q;
	 }
      } 
    }
  
   /**
    * Squared Euclidean distance transform
    *
    * @param src Imput image
    * @param out Output image
    */
   void squared_edt( Img3dsl & image, Float * out ) {
      const Long width = image.Width();
      const Long height = image.Height();
      const Long depth = image.Depth();
      const Long wh = width * height;
      Long * imagePixels = image.Vector();
      std::vector<Long> g(width), dt(width), s(width), t(width);

      for (Long y = 0; y < height; ++y) {
	 for (Long z = 0; z < depth; ++z) { // X-direction.
	    memcpy(&g[0], imagePixels + z * wh + y * width, width * sizeof(Long));
	    distance_scan(width, g, s, t, dt);
	    memcpy(imagePixels + z * wh + y * width, &dt[0], width * sizeof(Long));
	 }
      }

      g.assign(height, 0);
      dt.assign(height, 0);
      s.assign(height, 0);
      t.assign(height, 0);

      for (int x = 0; x < width; ++x) {
	 for (int z = 0; z < depth; ++z) { // Y-direction.
	    for (int y = 0; y < height; ++y) {
	       g[y] = imagePixels[z * wh + y * width + x];
	    }
	    distance_scan(height, g, s, t, dt);
	    for (int y = 0; y < height; ++y) {
	       imagePixels[z * wh + y * width + x] = dt[y];
	    }
	 }
      }

      if (depth > 1) {
	 g.assign(depth, 0);
	 dt.assign(depth, 0);
	 s.assign(depth, 0);
	 t.assign(depth, 0);

	 for (int x= 0; x < width; ++x ) {
	    for (int y = 0; y < height; ++y ) { // Z-direction.
	       for (int z = 0; z < depth; ++z ) {
		  g[z] = imagePixels[z * wh + y * width + x];
	       }
	       distance_scan(depth, g, s, t, dt);
	       for (int z = 0; z < depth; ++z ) {
		  imagePixels[z * wh + y * width + x] = dt[z];
	       }
	    }
	 }
      }
      // Squared EDT (image) to EDT (out)
      Long * pixel = imagePixels;
      Long * end = imagePixels + image.VectorSize();
      Float * distance = out;
      while (pixel != end) {
	 *distance++ = std::sqrt((float)*pixel++);
      }
   }
}

template<typename T>
Errc PDistance( const Img3d<T> & ims, Img3dsf & imd ) {
   T * srcPixel = ims.Vector();
   T * srcPixelEnd = srcPixel + ims.VectorSize();
   Img3dsl squared(ims.Size());
   Long * dPixel = squared.Vector();
   while (srcPixel != srcPixelEnd) {
      *dPixel++ = ( *srcPixel++ ) ? 0 : Infinity; 
   }
   squared_edt(squared, imd.Vector());
   return SUCCESS;
}

template<typename T>
Errc PDistance( const Img2d<T> & ims, Img2dsf & imd ) {
   T * srcPixel = ims.Vector();
   T * srcPixelEnd = srcPixel + ims.VectorSize();
   Img3dsl squared(1, ims.Height(), ims.Width());
   Long * dPixel = squared.Vector();
   while (srcPixel != srcPixelEnd ) {
      *dPixel++ = ( *srcPixel++ ) ? 0 : Infinity; 
   }
   squared_edt(squared, imd.Vector());
   return SUCCESS;
}

Errc PDistance( const Graph2d &grs, Graph2d &grd ) {
   int d;
   bool stop;
   Fifo<Point2d> f;
   Point2d pt;
     
   grd = grs;
  
   d = 1;
   for (int i = 1; i < grs.Size(); i++) {
      grd[i]->value = 0;
      if (!grs[i]->value) {
	 for (GEdge *ptr = grs[i]->Neighbours(); ptr != NULL; ptr = ptr->Next()) {
	    if (grs[ptr->Node()]->value) {
	       f.Push(Point2d(i, i));
	       grd[i]->value = (Graph2d::ValueType)d;
	       break;
	    }
	 }
      }
   }
  
   f.Push(Point2d(0, 0));
   stop = false;
   while (!stop) {
      pt = f.Pop();
      if (!pt.x) {
	 if (f.Empty()) {
	    stop = true;
	 } else {
	    d++;
	    f.Push(Point2d(0, 0));
	 }
      } else {
	 for (GEdge *ptr = grs[pt.x]->Neighbours(); ptr != NULL; ptr = ptr->Next()) {
	    if ( !grs[ptr->Node()]->value && !grd[ptr->Node()]->value) {
	       grd[ptr->Node()]->value = (Graph2d::ValueType)d + 1.0f;
	       f.Push(Point2d(ptr->Node(),ptr->Node()));
	    }
	 }
      }
   }
   return SUCCESS;
}

#ifdef MAIN
#define USAGE	"usage: %s [-m mask] [im_in|-] [im_out|-]"
#define PARC	0
#define FINC	1
#define FOUTC	1
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
      objd[0] = new Img2dsf(ims->Props());
      Img2dsf* const imd = (Img2dsf*)objd[0];
      
      result = PDistance(*ims, *imd);
      goto end;
   }
   if (objs[0]->Type() == Po_Img2dsl) {
      Img2dsl* const ims = (Img2dsl*)objs[0];
      objd[0] = new Img2dsf(ims->Props());
      Img2dsf* const imd = (Img2dsf*)objd[0];
      
      result = PDistance(*ims, *imd);
      goto end;
   }
   if (objs[0]->Type() == Po_Img2dsf) {
      Img2dsf* const ims = (Img2dsf*)objs[0];
      objd[0] = new Img2dsf(ims->Props());
      Img2dsf* const imd = (Img2dsf*)objd[0];
      
      result = PDistance(*ims, *imd);
      goto end;
   }
   if (objs[0]->Type() == Po_Img3duc) {
      Img3duc* const ims = (Img3duc*)objs[0];
      objd[0] = new Img3dsf(ims->Props());
      Img3dsf* const imd = (Img3dsf*)objd[0];
      
      result = PDistance(*ims, *imd);
      goto end;
   }
   if (objs[0]->Type() == Po_Img3dsl) {
      Img3dsl* const ims = (Img3dsl*)objs[0];
      objd[0] = new Img3dsf(ims->Props());
      Img3dsf* const imd = (Img3dsf*)objd[0];
      
      result = PDistance(*ims, *imd);
      goto end;
   }
   if (objs[0]->Type() == Po_Img3dsf) {
      Img3dsf* const ims = (Img3dsf*)objs[0];
      objd[0] = new Img3dsf(ims->Props());
      Img3dsf* const imd = (Img3dsf*)objd[0];
      
      result = PDistance(*ims, *imd);
      goto end;
   }
   if (objs[0]->Type() == Po_Graph2d) {
      Graph2d* const grs = (Graph2d*)objs[0];
      objd[0] = new Graph2d(grs->Size());
      Graph2d* const grd = (Graph2d*)objd[0];
      result = PDistance(*grs, *grd);
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
