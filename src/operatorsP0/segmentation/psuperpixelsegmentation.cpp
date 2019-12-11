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
 * @author Pierre Buyssens - Nov 25, 2013
 */

#include <stdio.h>
#include <math.h>
#include <pandore.h>
#include <iostream>
#include <limits>
using namespace pandore;

template<typename T>
float computeVarianceNeighborhood2d( Imx2d<T> &ims, int x, int y ) {
   int W = ims.Width();
   int H = ims.Height();
   int S = ims.Bands();
   int v5x[] = {0, -1, 0, 1,  0};
   int v5y[] = {0,  0, 1, 0, -1};
   float* mu = new float[S];
   for (int c = 0; c < S; c++) {
      mu[c] = 0;
   }
   int N = 0;

   // compute mu
   for (int k = 0; k < 5; k++) {
      int xx = x + v5x[k];
      int yy = y + v5y[k];
      if ( (xx >= 0) && (xx < W) && (yy >= 0) && (yy < H) ) {
	 N++;
	 for (int c = 0; c < S; c++) {
	    mu[c] += (float)ims(c, yy, xx);
	 }
      }
   }
   for (int c = 0; c < S; c++) {
      mu[c] /= (float)N;
   }

   // compute 'variance'
   float var = 0;
   for (int k = 0; k < 5; k++) {
      int xx = x + v5x[k];
      int yy = y + v5y[k];
      if ( (xx >= 0) && (xx < W)
	   && (yy >= 0) && (yy < H) ) {
	 for (int c = 0; c < S; c++) {
	    float t = (float)ims(c, yy, xx) - mu[c];
	    var += t * t;
	 }
      }
   }
   var /= (float)N;

   delete mu;

   return var;
}

template<typename T>
float computeVarianceNeighborhood3d( Imx3d<T> &ims, int x, int y, int z ) {
   int W = ims.Width();
   int H = ims.Height();
   int D = ims.Depth();
   int S = ims.Bands();
   int v7x[] = {0, -1, 0, 1,  0, 0,  0};
   int v7y[] = {0,  0, 1, 0, -1, 0,  0};
   int v7z[] = {0,  0, 0, 0,  0, 1, -1};
   float* mu = new float[S];
   for (int c = 0; c < S; c++) {
      mu[c] = 0;
   }
   int N = 0;
   // compute mu
   for (int k = 0; k < 7; k++) {
      int xx = x + v7x[k];
      int yy = y + v7y[k];
      int zz = z + v7z[k];
      if ( (xx >= 0) && (xx < W)
	   && (yy >= 0) && (yy < H)
	   && (zz >= 0) && (zz < D) ) {
	 N++;
	 for (int c = 0; c < S; c++)
	    mu[c] += (float)ims(c, zz, yy, xx);
      }
   }
   for (int c = 0; c < S; c++) {
      mu[c] /= (float)N;
   }

   // compute 'variance'
   float var = 0;
   for (int k = 0; k < 7; k++) {
      int xx = x + v7x[k];
      int yy = y + v7y[k];
      int zz = z + v7z[k];
      if ( (xx >= 0) && (xx < W)
	   && (yy >= 0) && (yy < H)
	   && (zz >= 0) && (zz < D) ) {
	 for (int c = 0; c < S; c++) {
	    float t = (float)ims(c, zz, yy, xx) - mu[c];
	    var += t*t;
	 }
      }
   }
   var /= (float) N;

   delete mu;

   return var;
}



// update means and count values (for label lab) with pixel of ims at (x,y)
// assuming that ims contains pixel (x,y)
template<typename T>
void updateSuperpixels2d( Imx2d<T> &ims, Img2dsf &means, Img2dsl &count, int lab, int x, int y ) {
   for (int c = 0; c < ims.Bands(); c++) {
      means(c, lab) = means(c, lab) * count(0, lab) + (float)ims(c, y, x);
   }
   count(0, lab) = count(0, lab) + 1;
   for (int c = 0; c < ims.Bands(); c++) {
      means(c, lab) = means(c, lab) / count(0, lab);
   }
}

template<typename T>
void updateSuperpixels3d( Imx3d<T> &ims, Img2dsf &means, Img2dsl &count, int lab, int x, int y, int z ) {
   for (int c = 0; c < ims.Bands(); c++) {
      means(c, lab) = means(c, lab) * count(0, lab) + (float)ims(c, z, y, x);
   }
   count(0,lab) = count(0,lab) + 1;
   for (int c = 0; c < ims.Bands(); c++) {
      means(c, lab) = means(c, lab) / count(0, lab);
   }
}

template<typename T>
Errc PSuperPixelSegentation2D( Imx2d<T> &ims, Reg2d &rgs, Reg2d &rgd, Img2dsf &D, int compactness, int perturbation ) {
   int W = ims.Width();
   int H = ims.Height();
   int C = ims.Bands();
   float INF = std::numeric_limits<float>::max();
   long x, y, xx, yy, lab;
   int k, c;
   float P, a1, a2, A1, delta;
   int v4x[] = {-1,0,1, 0};
   int v4y[] = { 0,1,0,-1};
   int v8x [] = {-1, 0, 1, 0, -1, -1, 1, 1};
   int v8y [] = {0 ,-1, 0, 1, -1, 1, 1, -1};

   Img2dsl S(rgs.Height(), rgs.Width());
   Heap<Point2d*, float, Ulong> tas(10000000);
   S = 1;
   D = INF;
   rgd = 0;
   int N = 0;
   if (perturbation) {
      // perturb initial seeds and initialize rgd, S, D images and Heap
      for (y = 0; y < H; y++) {
	 for (x = 0; x < W; x++) {
	    if (rgs(y, x)) {
	       N++;
	       long xmin = x;
	       long ymin = y;
	       float varmin = computeVarianceNeighborhood2d(ims, x, y);
	       for (k = 0; k < 8; k++) {
		  xx = x + v8x[k];
		  yy = y + v8y[k];
		  if ( (xx >= 0) && (xx < W)
		       && (yy >= 0) && (yy < H)) {
		     float v = computeVarianceNeighborhood2d(ims, xx, yy);
		     if (v < varmin) {
			xmin = xx;
			ymin = yy;
			varmin = v;
		     }
		  }
	       }
	       rgd(ymin, xmin) = rgs(y, x);
	       D(ymin, xmin) = 0;
	       S(ymin, xmin) = 0;
	       Point2d *pt1 = new Point2d(ymin, xmin);
	       tas.Push(pt1, 0, 0);
	    }
	 }
      }
      rgd.Labels(N);
   } else {
      // initialize rgd, S, D images and Heap
      for (y = 0; y < H; y++) {
	 for (x = 0; x < W; x++) {
	    if (rgs(y, x)) {
	       N++;
	       rgd(y, x) = rgs(y, x);
	       D(y, x) = 0;
	       S(y, x) = 0;
	       Point2d *pt1 = new Point2d(y, x);
	       tas.Push(pt1, 0, 0);
	    }
	 }
      }
      rgd.Labels(N);
   }


   float Sz = (float)W * H / (float)N;
   float Sz2 = Sz * Sz;
   float m2 = (float)compactness * compactness;
   float vv = m2 / Sz2;

   // initialize superpixels
   Img2dsf means(C,N);
   means = 0;
   Img2dsl count(1,N);
   count = 0;
   Img2dsf coords(2,N);
   coords = -1; // seeds coordinates
   for (y = 0; y < H; y++) {
      for (x = 0; x < W; x++) {
	 if (rgd(y, x) > 0) {
	    lab = rgd(y, x) - 1;
	    coords(0, lab) = (float)x;
	    coords(1, lab) = (float)y; // save seeds coordinates
	    for (c = 0; c < C; c++) {
	       means(c, lab) = (float)ims(c, y, x);
	    }
	    count(0, lab) = 1;
	    for (k = 0; k < 4; k++) {
	       xx = x + v4x[k];
	       yy = y + v4y[k];
	       if ( (xx >= 0) && (xx < W)
		    && (yy >= 0) && (yy < H)) {
		  updateSuperpixels2d(ims, means, count, lab, xx, yy);
	       }
	    }
	 }
      }
   }

   // let's go
   bool ok = false;
   Point2d *pt;
   long iter = 0;
   while (!ok) {
      iter++;
      // current point2d
      pt = tas.Pop();
      x = pt->x;
      y = pt->y;
      delete pt;

      if (S(y, x) != -1) { // consider only non fixed point2d
	 S(y, x) = -1; // fix it !
	 
	 // update the mean color of the superpixel
	 lab = rgd(y, x) - 1;
	 updateSuperpixels2d(ims, means, count, lab, x, y);
	 
	 // go for the neighborhood investigation
	 for (k = 0; k < 4; k++) {
	    xx = x + v4x[k];
	    yy = y + v4y[k];

	    if ((xx < W) && (xx >= 0)
		&& (yy >= 0) && (yy < H)) {
	       P = 0;
	       for (c = 0; c < C; c++) {
		  float t = means(c, lab) - (float)ims(c, yy, xx);
		  P += t * t;
	       }
	       
	       if (compactness > 0) {
		  float d = 0;
		  float t = coords(0, lab) - (float)xx;
		  d += t * t;
		  t = coords(1, lab) - (float)yy;
		  d += t * t;
		  d *= d;
		  P *= P;
		  P = sqrt(P + d * vv);
	       }
	       
	       // compute its neighboring values
	       a1 = INF;
	       if (xx < W - 1)
		  a1 = D(yy, xx + 1);
	       if (xx > 0) {
		  a1 = (a1<D(yy,xx-1))?a1:D(yy,xx-1);
	       }
		  
	       a2 = INF;
	       if (yy < H - 1) {
		  a2 = D(yy + 1, xx);
	       }
	       if (yy > 0) {
		  a2 = (a2 < D(yy - 1, xx)) ? a2 : D(yy - 1, xx);
	       }
	       
	       if (a2 < a1) { // swap such that a1 < = a2
		  float t = a1;
		  a1 = a2;
		  a2 = t;
	       }

	       // update its distance
	       // now the equation is   (a-a1)^2+(a-a2)^2 = P, with a > = a2 > = a1.
	       A1 = 0;
	       if (P * P > (a2 - a1) * (a2 - a1) ) {
		  delta = 2 * P * P - (a2 - a1)*(a2 - a1);
		  A1 = (float)((a1 + a2 + sqrt(delta)) / 2.0);
	       } else {
		  A1 = a1 + P;
	       }
	       
	       if (S(yy, xx) == 0) {
		  if (A1 < D(yy, xx)) {
		     // update distance
		     D(yy, xx) = A1;
		     rgd(yy, xx) = lab + 1;
		     Point2d* pt1 = new Point2d(yy, xx);
		     tas.Push(pt1, A1, iter);
		  }
	       } else {
		  if (S(yy, xx) == 1) {
		     // add new point2d
		     S(yy, xx) = 0;
		     D(yy, xx) = A1;
		     rgd(yy, xx) = lab + 1;
		     Point2d* pt1 = new Point2d(yy, xx);
		     tas.Push(pt1, A1, iter);
		  }
	       }
	    }
	 }
      }
      if (tas.Empty()) {
	 ok = true;
      }
   }
   return  SUCCESS;
}

template<typename T>
Errc PSuperPixelSegmentation3D( Imx3d<T> &ims, Reg3d &rgs, Reg3d &rgd, Img3dsf &D, int compactness, int perturbation ) {
   int W = ims.Width();
   int H = ims.Height();
   int De = ims.Depth();
   int C = ims.Bands();

   float INF = std::numeric_limits<float>::max();
   long x, y, z, xx, yy, zz, lab;
   int k, c;
   float P, a1, a2, a3, A1, delta;
   int v6x[] = {-1, 0, 1,  0,  0, 0};
   int v6y[] = { 0, 1, 0, -1,  0, 0};
   int v6z[] = { 0, 0, 0,  0,- 1, 1};

   Img3dsl S(rgs.Depth(), rgs.Height(), rgs.Width());
   Heap<Point3d*, float, Ulong> tas(10000000);
   S = 1;
   D = INF;
   rgd = 0;
   int N = 0;
   if (perturbation) {
      // perturb initial seeds and initialize rgd, S, D images and Heap
      for (z = 0; z < De; z++) {
	 for (y = 0; y < H; y++) {
	    for (x = 0; x < W; x++) {
	       if (rgs(z, y, x)) {
		  N++;
		  long xmin = x;
		  long ymin = y;
		  long zmin = z;
		  float varmin = computeVarianceNeighborhood3d(ims, x, y, z);
		  for (k = 0; k < 6; k++) {
		     xx = x + v6x[k];
		     yy = y + v6y[k];
		     zz = z + v6z[k];
		     if ( (xx >= 0) && (xx < W)
			  && (yy >= 0) && (yy < H)
			  && (zz >= 0) && (zz < De)) {
			float v = computeVarianceNeighborhood3d(ims, xx, yy, zz);
			if (v < varmin) {
			   xmin = xx;
			   ymin = yy;
			   zmin = zz;
			   varmin = v;
			}
		     }
		  }
		  rgd(zmin, ymin, xmin) = rgs(z, y, x);
		  D(zmin, ymin, xmin) = 0;
		  S(zmin, ymin, xmin) = 0;
		  Point3d *pt1 = new Point3d(zmin, ymin, xmin);
		  tas.Push(pt1, 0, 0);
	       }
	    }
	 }
      }
      rgd.Labels(N);
   } else {
      // initialize rgd, S, D images and Heap
      for (z = 0; z < De; z++) {
	 for (y = 0; y < H; y++) {
	    for (x = 0; x < W; x++) {
	       if (rgs(z, y, x)) {
		  N++;
		  rgd(z, y, x) = rgs(z, y, x);
		  D(z, y, x) = 0;
		  S(z, y, x) = 0;
		  Point3d *pt1 = new Point3d(z, y, x);
		  tas.Push(pt1, 0, 0);
	       }
	    }
	 }
      }
      rgd.Labels(N);
   }
   
   float Sz = (float)W * H * De / (float)N;
   float Sz3 = Sz * Sz * Sz;
   float m3 = (float)compactness * compactness * compactness;
   float vv = m3 / Sz3;
   
   // initialize superpixels
   Img2dsf means(C, N);
   means = 0;
   Img2dsl count(1, N);
   count = 0;
   Img2dsf coords(3, N);
   coords = -1; // seeds coordinates
   for (z = 0; z < De; z++) {
      for (y = 0;y < H; y++) {
	 for (x = 0; x < W; x++) {
	    if (rgd(z, y, x) > 0) {
	       lab = rgd(z, y, x) - 1;
	       coords(0, lab) = (float)x;
	       coords(1, lab) = (float)y;
	       coords(2, lab) = (float)z; // save seeds coordinates
	       for (c = 0; c < C; c++) {
		  means(c,lab) = (float)ims(c, z, y ,x);
	       }
	       count(0, lab) = 1;
	       for (k = 0; k < 6; k++) {
		  xx = x + v6x[k];
		  yy = y + v6y[k];
		  zz = z + v6z[k];
		  if ( (xx >= 0) && (xx < W)
		       && (yy >= 0) && (yy < H)
		       && (zz >= 0) && (zz < De)) {
		     updateSuperpixels3d(ims, means, count, lab, xx, yy, zz);
		  }
	       }
	    }
	 }
      }
   }

   // let's go
   bool ok = false;
   Point3d *pt;
   long iter = 0;
   while (!ok) {
      iter++;
      // current point2d
      pt = tas.Pop();
      x = pt->x;
      y = pt->y;
      z = pt->z;
      delete pt;

      if (S(z, y, x) != -1) { // consider only non fixed point2d
	 S(z, y, x) = -1; // fix it !
	 
	 // update the mean color of the superpixel
	 lab = rgd(z, y, x) - 1;
	 updateSuperpixels3d(ims, means, count, lab, x, y, z);
	 
	 // go for the neighborhood investigation
	 for (k = 0; k < 6; k++) {
	    xx = x + v4x[k];
	    yy = y + v4y[k];
	    zz = z + v6z[k];

	    if ((xx < W) && (xx >= 0)
		&& (yy >= 0) && (yy < H)
		&& (zz >= 0) && (zz < De)) {
	       P = 0;
	       for (c = 0; c < C; c++) {
		  float t = means(c, lab) - (float)ims(c, zz, yy, xx);
		  P += t * t;
	       }
	       
	       if (compactness > 0) {
		  float d = 0;
		  float t = coords(0, lab) - (float)xx;
		  d += t * t;
		  t = coords(1, lab) - (float)yy;
		  d += t * t;
		  t = coords(2, lab) - (float)zz;
		  d += t * t;
		  d *= d;
		  P *= P;
		  P = sqrt(P + d * vv);
	       }
	       
	       // compute its neighboring values
	       a1 = INF;
	       if (xx < W - 1) {
		  a1 = D(yy, xx + 1);
	       }
	       if (xx > 0) {
		  a1 = (a1<D(yy, xx - 1)) ? a1 : D(yy, xx - 1);
	       }
		  
	       a2 = INF;
	       if (yy < H - 1) {
		  a2 = D(yy + 1, xx);
	       }
	       if (yy > 0) {
		  a2 = (a2 < D(yy - 1, xx)) ? a2 : D(yy - 1, xx);
	       }
	       
	       a3 = INF;
	       if (zz < De - 1) {
		  a3 = D(xx, yy, zz + 1);
	       }
	       if (zz > 0) {
		  a3 = (a3 < D(xx, yy, zz - 1)) ? a3 : D(xx, yy, zz - 1);
	       }

	       // swap such that a1 < = a2 < = a3
	       if (a3 < a2) {
		  float t = a2;
		  a2 = a3;
		  a3 = t;
	       }
	       if (a2 < a1) {
		  float t = a1;
		  a1 = a2;
		  a2 = t;
	       }
	       if (a3 < a2) {
		  float t = a2;
		  a2 = a3;
		  a3 = t;
	       }

	       // update its distance
	       // now the equation is
	       // (a-a1)^2+(a-a2)^2+(a-a3)^2 - P^2 = 0
	       // with a > = a3 > = a2 > = a1.
	       // = >    3*a^2 - 2*(a2+a1+a3)*a - P^2 + a1^2 + a3^2 + a2^2
	       // = > delta = (a2+a1+a3)^2 - 3*(a1^2 + a3^2 + a2^2 - P^2)
	       A1 = 0;
	       delta = (a2 + a1 + a3) * (a2 + a1 + a3) -3 * (a1 * a1 + a2 * a2 + a3 * a3 - P * P);
	       A1 = 0;
	       if (delta >= 0 ) {
		  A1 = (float)((a2 + a1 + a3 + sqrt(delta)) / 3.0);
	       }
	       if (A1 <= a3 ) {
		  // at least a3 is too large, so we have
		  // a > = a2 > = a1  and  a<a3 so the equation is 
		  //		(a-a1)^2+(a-a2)^2 - P^2 = 0
		  // = > 2*a^2 - 2*(a1+a2)*a + a1^2+a2^2-P^2
		  // delta = (a2+a1)^2 - 2*(a1^2 + a2^2 - P^2)
		  delta = (a2 + a1) * (a2 + a1) - 2 * (a1 * a1 + a2 * a2 - P * P);
		  A1 = 0;
		  if (delta >= 0 ) {
		     A1 = (float)(0.5 * (a2 + a1 + sqrt(delta)));
		  }
		  if (A1 <= a2 ) {
		     A1 = a1 + P;
		  }
	       }
	       
	       if (S(zz, yy, xx) == 0) {
		  if (A1 < D(zz, yy, xx)) {
		     // update distance
		     D(zz, yy, xx) = A1;
		     rgd(zz, yy, xx) = lab + 1;
		     Point3d* pt1 = new Point3d(zz, yy, xx);
		     tas.Push(pt1, A1, iter);
		  }
	       } else {
		  if (S(zz, yy, xx) == 1) {
		     // add new point3d
		     S(zz, yy, xx) = 0;
		     D(zz, yy, xx) = A1;
		     rgd(zz, yy, xx) = lab + 1;
		     Point3d* pt1 = new Point3d(zz, yy, xx);
		     tas.Push(pt1, A1, iter);
		  }
	       }
	    }
	 }
      }
      if (tas.Empty()) {
	 ok = true;
      }
   }

   return SUCCESS;
}

#ifdef MAIN
#define USAGE   "usage: %s compactness perturbation [rg_in|-] [im_in|-] [rg_out|-] [im_out|-]"
#define PARC    2
#define FINC    2
#define FOUTC   2
#define MASK    0

int main( int argc,char* argv[] ) {
   Errc result;			// The result code of the execution.
   Pobject *mask;		// The mask.
   Pobject *objin[FINC+1];	// The input objects;
   Pobject *objs[FINC+1];	// The source objects masked by mask.
   Pobject *objout[FOUTC+1];	// The output object unmasked by mask.
   Pobject *objd[FOUTC+1];	// The result object.
   char *parv[PARC+1];		// The input parameters.
  
   ReadArgs(argc, argv, PARC, FINC, FOUTC, &mask, objin, objs, objout, objd, parv, USAGE, MASK);

   int compactness = atoi(parv[0]);
   int perturbation = atoi(parv[1]);
   
   if ((objs[1]->Type() == Po_Img2duc)) {
      // inputs
      Reg2d* rgs = (Reg2d*)objs[0];
      Img2duc* ims = (Img2duc*)objs[1];

      // outputs
      objd[0] = new Reg2d(rgs->Size());
      Reg2d* rgd = (Reg2d*)objd[0];
      objd[1] = new Img2dsf(rgs->Size());
      Img2dsf* imd = (Img2dsf*)objd[1];

      result = PSuperPixelSegentation2D(*ims, *rgs, *rgd, *imd, compactness, perturbation);
   }
   if ((objs[1]->Type() == Po_Img2dsl)) {
      // inputs
      Reg2d* rgs = (Reg2d*)objs[0];
      Img2dsl* ims = (Img2dsl*)objs[1];

      // outputs
      objd[0] = new Reg2d(rgs->Size());
      Reg2d* rgd = (Reg2d*)objd[0];
      objd[1] = new Img2dsf(rgs->Size());
      Img2dsf* imd = (Img2dsf*)objd[1];

      result = PSuperPixelSegentation2D(*ims, *rgs, *rgd, *imd, compactness, perturbation);
   }
   if ((objs[1]->Type() == Po_Img2dsf)) {
      // inputs
      Reg2d* rgs = (Reg2d*)objs[0];
      Img2dsf* ims = (Img2dsf*)objs[1];

      // outputs
      objd[0] = new Reg2d(rgs->Size());
      Reg2d* rgd = (Reg2d*)objd[0];
      objd[1] = new Img2dsf(rgs->Size());
      Img2dsf* imd = (Img2dsf*)objd[1];

      result = PSuperPixelSegentation2D(*ims, *rgs, *rgd, *imd, compactness, perturbation);
   }
   if ((objs[1]->Type() == Po_Imc2duc)) {
      // inputs
      Reg2d* rgs = (Reg2d*)objs[0];
      Imc2duc* ims = (Imc2duc*)objs[1];

      // outputs
      objd[0] = new Reg2d(rgs->Size());
      Reg2d* rgd = (Reg2d*)objd[0];
      objd[1] = new Img2dsf(rgs->Size());
      Img2dsf* imd = (Img2dsf*)objd[1];

      result = PSuperPixelSegentation2D(*ims, *rgs, *rgd, *imd, compactness, perturbation);
   }
   if ((objs[1]->Type() == Po_Imc2dsl)) {
      // inputs
      Reg2d* rgs = (Reg2d*)objs[0];
      Imc2dsl* ims = (Imc2dsl*)objs[1];

      // outputs
      objd[0] = new Reg2d(rgs->Size());
      Reg2d* rgd = (Reg2d*)objd[0];
      objd[1] = new Img2dsf(rgs->Size());
      Img2dsf* imd = (Img2dsf*)objd[1];

      result = PSuperPixelSegentation2D(*ims, *rgs, *rgd, *imd, compactness, perturbation);
   }
   if ((objs[1]->Type() == Po_Imc2dsf)) {
      // inputs
      Reg2d* rgs = (Reg2d*)objs[0];
      Imc2dsf* ims = (Imc2dsf*)objs[1];

      // outputs
      objd[0] = new Reg2d(rgs->Size());
      Reg2d* rgd = (Reg2d*)objd[0];
      objd[1] = new Img2dsf(rgs->Size());
      Img2dsf* imd = (Img2dsf*)objd[1];

      result = PSuperPixelSegentation2D(*ims, *rgs, *rgd, *imd, compactness, perturbation);
   }
   if ((objs[1]->Type() == Po_Imx2duc)) {
      // inputs
      Reg2d* rgs = (Reg2d*)objs[0];
      Imx2duc* ims = (Imx2duc*)objs[1];

      // outputs
      objd[0] = new Reg2d(rgs->Size());
      Reg2d* rgd = (Reg2d*)objd[0];
      objd[1] = new Img2dsf(rgs->Size());
      Img2dsf* imd = (Img2dsf*)objd[1];

      result = PSuperPixelSegentation2D(*ims, *rgs, *rgd, *imd, compactness, perturbation);
   }
   if ((objs[1]->Type() == Po_Imx2dsl)) {
      // inputs
      Reg2d* rgs = (Reg2d*)objs[0];
      Imx2dsl* ims = (Imx2dsl*)objs[1];

      // outputs
      objd[0] = new Reg2d(rgs->Size());
      Reg2d* rgd = (Reg2d*)objd[0];
      objd[1] = new Img2dsf(rgs->Size());
      Img2dsf* imd = (Img2dsf*)objd[1];

      result = PSuperPixelSegentation2D(*ims, *rgs, *rgd, *imd, compactness, perturbation);
   }
   
   if ((objs[1]->Type() == Po_Imx2dsf)) {
      // inputs
      Reg2d* rgs = (Reg2d*)objs[0];
      Imx2dsf* ims = (Imx2dsf*)objs[1];

      // outputs
      objd[0] = new Reg2d(rgs->Size());
      Reg2d* rgd = (Reg2d*)objd[0];
      objd[1] = new Img2dsf(rgs->Size());
      Img2dsf* imd = (Img2dsf*)objd[1];

      result = PSuperPixelSegentation2D(*ims, *rgs, *rgd, *imd, compactness, perturbation);
   }

   // 3D
   if ((objs[1]->Type() == Po_Img3duc)) {
      // inputs
      Reg3d* rgs = (Reg3d*)objs[0];
      Img3duc* ims = (Img3duc*)objs[1];

      // outputs
      objd[0] = new Reg3d(rgs->Size());
      Reg3d* rgd = (Reg3d*)objd[0];
      objd[1] = new Img3dsf(rgs->Size());
      Img3dsf* imd = (Img3dsf*)objd[1];

      result = PSuperPixelSegmentation3D(*ims, *rgs, *rgd, *imd, compactness, perturbation);
   }
   if ((objs[1]->Type() == Po_Img3dsl)) {
      // inputs
      Reg3d* rgs = (Reg3d*)objs[0];
      Img3dsl* ims = (Img3dsl*)objs[1];

      // outputs
      objd[0] = new Reg3d(rgs->Size());
      Reg3d* rgd = (Reg3d*)objd[0];
      objd[1] = new Img3dsf(rgs->Size());
      Img3dsf* imd = (Img3dsf*)objd[1];

      result = PSuperPixelSegmentation3D(*ims, *rgs, *rgd, *imd, compactness, perturbation);
   }
   if ((objs[1]->Type() == Po_Img3dsf)) {
      // inputs
      Reg3d* rgs = (Reg3d*)objs[0];
      Img3dsf* ims = (Img3dsf*)objs[1];

      // outputs
      objd[0] = new Reg3d(rgs->Size());
      Reg3d* rgd = (Reg3d*)objd[0];
      objd[1] = new Img3dsf(rgs->Size());
      Img3dsf* imd = (Img3dsf*)objd[1];

      result = PSuperPixelSegmentation3D(*ims, *rgs, *rgd, *imd, compactness, perturbation);
   }
   if ((objs[1]->Type() == Po_Imc3duc)) {
      // inputs
      Reg3d* rgs = (Reg3d*)objs[0];
      Imc3duc* ims = (Imc3duc*)objs[1];

      // outputs
      objd[0] = new Reg3d(rgs->Size());
      Reg3d* rgd = (Reg3d*)objd[0];
      objd[1] = new Img3dsf(rgs->Size());
      Img3dsf* imd = (Img3dsf*)objd[1];

      result = PSuperPixelSegmentation3D(*ims, *rgs, *rgd, *imd, compactness, perturbation);
   }
   if ((objs[1]->Type() == Po_Imc3dsl)) {
      // inputs
      Reg3d* rgs = (Reg3d*)objs[0];
      Imc3dsl* ims = (Imc3dsl*)objs[1];

      // outputs
      objd[0] = new Reg3d(rgs->Size());
      Reg3d* rgd = (Reg3d*)objd[0];
      objd[1] = new Img3dsf(rgs->Size());
      Img3dsf* imd = (Img3dsf*)objd[1];

      result = PSuperPixelSegmentation3D(*ims, *rgs, *rgd, *imd, compactness, perturbation);
   }
   if ((objs[1]->Type() == Po_Imc3dsf)) {
      // inputs
      Reg3d* rgs = (Reg3d*)objs[0];
      Imc3dsf* ims = (Imc3dsf*)objs[1];

      // outputs
      objd[0] = new Reg3d(rgs->Size());
      Reg3d* rgd = (Reg3d*)objd[0];
      objd[1] = new Img3dsf(rgs->Size());
      Img3dsf* imd = (Img3dsf*)objd[1];

      result = PSuperPixelSegmentation3D(*ims, *rgs, *rgd, *imd, compactness, perturbation);
   }
   if ((objs[1]->Type() == Po_Imx3duc)) {
      // inputs
      Reg3d* rgs = (Reg3d*)objs[0];
      Imx3duc* ims = (Imx3duc*)objs[1];

      // outputs
      objd[0] = new Reg3d(rgs->Size());
      Reg3d* rgd = (Reg3d*)objd[0];
      objd[1] = new Img3dsf(rgs->Size());
      Img3dsf* imd = (Img3dsf*)objd[1];

      result = PSuperPixelSegmentation3D(*ims, *rgs, *rgd, *imd, compactness, perturbation);
   }
   if ((objs[1]->Type() == Po_Imx3dsl)) {
      // inputs
      Reg3d* rgs = (Reg3d*)objs[0];
      Imx3dsl* ims = (Imx3dsl*)objs[1];

      // outputs
      objd[0] = new Reg3d(rgs->Size());
      Reg3d* rgd = (Reg3d*)objd[0];
      objd[1] = new Img3dsf(rgs->Size());
      Img3dsf* imd = (Img3dsf*)objd[1];

      result = PSuperPixelSegmentation3D(*ims, *rgs, *rgd, *imd, compactness, perturbation);
   }
   if ((objs[1]->Type() == Po_Imx3dsf)) {
      // inputs
      Reg3d* rgs = (Reg3d*)objs[0];
      Imx3dsf* ims = (Imx3dsf*)objs[1];

      // outputs
      objd[0] = new Reg3d(rgs->Size());
      Reg3d* rgd = (Reg3d*)objd[0];
      objd[1] = new Img3dsf(rgs->Size());
      Img3dsf* imd = (Img3dsf*)objd[1];

      result = PSuperPixelSegmentation3D(*ims, *rgs, *rgd, *imd, compactness, perturbation);
   }

   if (result) {
      WriteArgs(argc, argv, PARC, FINC, FOUTC, &mask, objin, objs, objout, objd, MASK);
   }
   Exit(result);
   return 0;
}

#endif
