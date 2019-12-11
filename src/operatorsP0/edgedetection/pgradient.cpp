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
 * @author Regis Clouard - 1997-02-24
 * @author Francois Angot - 1997-10-29
 * @author Olivier Lezoray - 1997-07-09 (add color gradient)
 * @author Regis Clouard - 2003-05-16 (Refine magnitude: MAX(dx, dy, 3/4(dz+dy)))
 * @author Regis Clouard - 2008-11-05 (Add borders in 2D)
 */

#include <math.h>
#include <pandore.h>
using namespace pandore;



/**
 * @file pgradient.cpp
 * Gradient is approximated with 1st order derivation mask : -1 0 1.
 * Magnitude is MAX(dx, dy, 3/4(dz+dy)) in 2D, MAX(dx, dy, dz, 4/7(dx+dy+dz))
 * Direction is arctan(Dy / Dx) and then discretized with Freeman code.
 */

template<typename T>
Errc PGradient( const Img3d<T> &ims, Img3d<T> &ima, Img3duc &imd, const int halfsize ) {
   double gx, gy, gz;
   double dx, dy, dz;
   Long sx, sy, sz;
   Errc PGradient( const Img2d<T> &ims, Img2d<T> &ima, Img2duc &imd, const int halfsize );
   
   if (halfsize < 1) {
      std::cerr << "Error pgradient: bad size value: "<<halfsize<< std::endl;
      return FAILURE;
   }

   Border3DIterator *it = ims.getBorderIterator(halfsize, halfsize, halfsize, halfsize, halfsize, halfsize);
   while( it->hasNext()) {
      Point3d p = it->next();
      // Convolution with mask -1, 0, 1 in each direction.
      gz = gx = gy = 0.0;
      for (int k = 1; k <= halfsize; k++) {
	 int i1, i2;
	 if (p.x - k < 0) {
	    i1 = 0;
	 } else {
	    i1 = p.x-k;
	 }
	 if (p.x + k >= ims.Width()) {
	    i2 = ims.Width() - 1;
	 } else {
	    i2 = p.x + k;
	 }
	 gx += (float)ims(p.z, p.y, i1) - ims(p.z, p.y, i2);

	 if (p.y-k < 0) {
	    i1 = 0;
	 } else {
	    i1 = p.y - k;
	 }
	 if (p.y + k >= ims.Height()) {
	    i2 = ims.Height() - 1;
	 } else {
	    i2 = p.y + k;
	 }
	 gy += (float)ims(p.z, i1, p.x) - ims(p.z, i2, p.x);

	 if (p.z - k < 0) {
	    i1 = 0;
	 } else {
	    i1 = p.z - k;
	 }
	 if (p.z + k >= ims.Depth()) {
	    i2 = ims.Depth() - 1;
	 } else {
	    i2 = p.z + k;
	 }
	 gz += (float)ims(i1, p.y, p.x) - ims(i2, p.y, p.x);
      }
      
      dx = fabs(gx);
      dy = fabs(gy);
      dz = fabs(gz);
      
      ima(p) = (T)((dx + dy + dz) / (3.0 * halfsize));
      
      // Discretization with Freeman code -> use sign
      // to determine the related quadran.
      
      // Determination of the sign of each derivation component.
      sx  = (gx > 0) ? -1 : ((gx == 0)? 0 : +1);
      sy  = (gy > 0) ? -1 : ((gy == 0)? 0 : +1);
      sz  = (gz > 0) ? -1 : ((gz == 0)? 0 : +1);
      // Discretization with Freeman code.
      imd(p) = 0;
      for (int k = 0; k < 26; k++) {
	 if ((v26z[k] == sz) && (v26y[k] == sy) && (v26x[k] == sx)) {
	    imd(p) = k;
	 }
      }
   }
   
   for (int z=halfsize; z<ims.Depth()-halfsize; z++) {
      for (int y=halfsize; y<ims.Height()-halfsize; y++) {
	 for (int x=halfsize; x<ims.Width()-halfsize; x++) {
	    // Convolution with mask -1, 0, 1 in each direction.
	    gz = gx = gy = 0.0;
	    for (int k = 1; k <= halfsize; k++) {
	       gx += (float)ims(z, y, x+k) - ims(z, y, x-k);
	       gy += (float)ims(z, y+k, x) - ims(z, y-k, x);
	       gz += (float)ims(z+k, y, x) - ims(z-k, y, x);
	    }
	    
	    dx = fabs(gx);
	    dy = fabs(gy);
	    dz = fabs(gz);

	    ima(z, y, x) = (T)((dx + dy + dz) / (3.0 * halfsize));
	    
	    // Discretization with Freeman code -> use sign
	    // to determine the related quadran.
	    
	    // Determination of the sign of each derivation component.
	    sx  = (gx > 0)? -1 : ((gx == 0)? 0 : +1);
	    sy  = (gy > 0)? -1 : ((gy == 0)? 0 : +1);
	    sz  = (gz > 0)? -1 : ((gz == 0)? 0 : +1);
	    // Discretization with Freeman code.
	    imd(z, y, x) = 0;
	    for (int k = 0; k < 26; k++) {
	       if ((v26z[k]== sz) && (v26y[k] == sy) && (v26x[k] == sx)) {
		  imd(z, y, x) = k;
	       }
	    }
	 }
      }
   }

   delete it;

   return SUCCESS;
}


/**
 * Di Zenzo algorithm.
 * Looking of extremal values of p*(dx*dx)+2*tdx*dy+ q*dyy*dy
 * is the same than looking for the eigenvalues matrix:
 * | p t |
 * | t q |
 * lambda1=1/2 * (p+q + sqrt((p-q)*(p-q)-4*t*t)) 
 * lambda2=1/2 * (p+q - sqrt((p-q)*(p-q)-4*t*t)) 
 * Strenght of edge=Shapiro recommends sqrt(lambda1 + lambda2)
 * orientation = 1/2 *arctan (2*t / (p-q))
 */

Errc PGradient( const Img1duc &ims, Img1duc &ima, Img1duc &imd, const int halfsize ) {
   float gx;
   register int x, k;
   
   if (halfsize < 1) {
      std::cerr << "Error pgradient: bad size value: "<<halfsize<< std::endl;
      return FAILURE;
   }

   // Upper left Corner
   for (x = 0; x < ims.Width(); x++) {
      gx = 0.0;
      for  (k = 0; k <= halfsize; k++) {
	 if (x - k < 0) {
	    gx -= Float(ims(x));
	 } else {
	    gx -= Float(ims(x - k));
	 }
	 if (x + k >= ims.Width()) {
	    gx += Float(ims(x));
	 } else {
	    gx += Float(ims(x + k));
	 }
      }
      ima(x) = (Img1duc::ValueType)fabs(gx);
      imd(x) = (gx <= 0)? 0 : 1;
   }

   return SUCCESS;
}

Errc PGradient( const Img1dsl &ims, Img1dsl &ima, Img1duc &imd, const int halfsize ) {
   float gx;
   register int x, k;
   
   if (halfsize < 1) {
      std::cerr << "Error pgradient: bad size value: "<<halfsize<< std::endl;
      return FAILURE;
   }

   // Upper left Corner
   for (x = 0; x < ims.Width(); x++) {
      gx = 0.0;
      for  (k = 0; k <= halfsize; k++) {
	 if (x - k < 0) {
	    gx -= Float(ims(x));
	 } else {
	    gx -= Float(ims(x - k));
	 }
	 if (x + k >= ims.Width()) {
	    gx += Float(ims(x));
	 } else {
	    gx += Float(ims(x + k));
	 }
      }
      ima(x) = (Img1dsl::ValueType)fabs(gx);
      imd(x) = (gx <= 0)? 0 : 1;
   }

   return SUCCESS;
}

Errc PGradient( const Img1dsf &ims, Img1dsf &ima, Img1duc &imd, const int halfsize ) {
   float gx;
   register int x, k;
   
   if (halfsize < 1) {
      std::cerr << "Error pgradient: bad size value: "<<halfsize<< std::endl;
      return FAILURE;
   }

   // Upper left Corner
   for (x = 0; x < ims.Width(); x++) {
      gx = 0.0;
      for  (k = 0; k <= halfsize; k++) {
	 if (x - k < 0) {
	    gx -= Float(ims(x));
	 } else {
	    gx -= Float(ims(x - k));
	 }
	 if (x + k >= ims.Width()) {
	    gx += Float(ims(x));
	 } else {
	    gx += Float(ims(x + k));
	 }
      }
      ima(x) = (Img1dsf::ValueType)fabs(gx);
      imd(x) = (gx <= 0)? 0 : 1;
   }

   return SUCCESS;
}

static void _Gradient1( double gx, double gy, int x, int y, Img2duc &ima, Img2duc &imd, const int halfsize ) {
   double dx = fabs(gx);
   double dy = fabs(gy);
   
   ima(y, x) = (Img2duc::ValueType)( (dx + dy) / (2.0 * halfsize));
   
   // Discretization with Freeman code -> use sign
   // to determine the related quadran.
   // Determination of the sign of each derivation component.
   double sx  = (gx > 0)? -1 : ((gx==0)? 0 : +1);
   double sy  = (gy > 0)? -1 : ((gy==0)? 0 : +1);
   imd(y, x) = 0;
   for (int k = 0; k < 8; k++) {
      if ((v8y[k] == sy) && (v8x[k] == sx)) {
	 imd(y, x) = k;
      }
   }
}

Errc PGradient( const Img2duc &ims, Img2duc &ima, Img2duc &imd, const int halfsize ) {
   double gx, gy;
   register int x, y, k;

   if (halfsize < 1) {
      std::cerr << "Error pgradient: bad size value: "<<halfsize<< std::endl;
      return FAILURE;
   }

   // Upper left Corner
   for (y = 0; y < halfsize; y++) {
      for (x = 0; x < halfsize; x++ ) {
	 gx = gy = 0.0;
	 for (k  =1; k <= halfsize; k++) {
	    if (x - k < 0) {
	       gx -= Float(ims(y, 0));
	    } else {
	       gx -= Float(ims(y, x - k));
	    }
	    gx += Float(ims(y, x + k));
	    if (y - k < 0) {
	       gy -= Float(ims(0, x));
	    } else {
	       gy -= Float(ims(y - k, x));
	    }
	    gy += Float(ims(y + k, x));
	 }
	 _Gradient1(gx, gy, x, y, ima, imd, halfsize);	 
      }
   }

   // Upper right Corner
   for (y = 0; y < halfsize; y++) {
      for (x = ims.Width() - 1; x > ims.Width() - 1 - halfsize; x--) {
	 gx = gy = 0.0;
	 for (k = 1; k <= halfsize; k++) {
	    if (x+k>ims.Width() - 1) {
	       gx += Float(ims(y, ims.Width() - 1));
	    } else {
	       gx += Float(ims(y, x + k));
	    }
	    gx -= Float(ims(y, x - k));
	    if (y - k < 0) {
	       gy -= Float(ims(0, x));
	    } else {
	       gy -= Float(ims(y - k, x));
	    }
	    gy += Float(ims(y + k, x));
	 }
	 _Gradient1(gx, gy, x, y, ima, imd, halfsize);	 
      }
   }

   // Lower left Corner
   for (y = ims.Height() - 1; y > ims.Height() - 1 - halfsize; y--) {
      for (x = 0; x < halfsize; x++ ) {
	 gx = gy = 0.0;
	 for (k = 1; k <= halfsize; k++) {
	    if (x - k < 0) {
	       gx -= Float(ims(y, 0));
	    } else {
	       gx -= Float(ims(y, x - k));
	    }
	    gx += Float(ims(y, x + k));
	    if (y + k > ims.Height() - 1) {
	       gy += Float(ims(ims.Height() - 1, x));
	    } else {
	       gy += Float(ims(y + k, x));
	    }
	    gy -= Float(ims(y - k, x));
	 }
	 _Gradient1(gx, gy, x, y, ima, imd, halfsize);	 
      }
   }

   // Lower right Corner
   for (y = ims.Height() - 1; y > ims.Height() - 1 - halfsize; y--) {
      for (x = ims.Width() - 1; x > ims.Width() - 1 - halfsize; x--) {
	 gx = gy = 0.0;
	 for (k = 1; k <= halfsize; k++) {
	    if (x + k > ims.Width() - 1) {
	       gx += Float(ims(y, ims.Width() - 1));
	    } else {
	       gx += Float(ims(y, x + k));
	    }
	    gx -= Float(ims(y, x - k));
	    if (y + k > ims.Height() - 1) {
	       gy += Float(ims(0, x));
	    } else {
	       gy += Float(ims(y + k, x));
	    }
	    gy -= Float(ims(y - k, x));
	 }
	 _Gradient1(gx, gy, x, y, ima, imd, halfsize);	 
      }
   }

   // Upper borders.
   for (y = 0; y < halfsize; y++) {
      for (x = halfsize; x < ims.Width() - halfsize; x++) {
	 // Convolution with mask -1, 0, 1.
	 gx = gy = 0.0;
	 for (k = 1; k <= halfsize; k++) {
	    gx += Float(ims(y, x + k)) - Float(ims(y, x - k));
	    gy += Float(ims(y + k, x));
	 }
	 for (k = 0; k < y; k++) {
	    gy -= Float(ims(y - k - 1, x));
	 }
	 gy -= (halfsize - y)*ims(0, x);
	 _Gradient1(gx, gy, x, y, ima, imd, halfsize);
      }
   }
   // Lower borders
   for (y = 0; y < halfsize; y++) {
      for (x = halfsize; x < ims.Width() - halfsize; x++) {
	 // Convolution with mask -1, 0, 1.
	 gx = gy = 0.0;
	 int bottom = ims.Height() - 1;
	 for (k = 1; k <= halfsize; k++) {
	    gx += Float(ims(bottom - y, x + k)) - Float(ims(bottom - y, x - k));
	    gy -= Float(ims(bottom - y - k, x));
	 }
 	 for (k=0; k<y; k++) {
	    gy += Float(ims(bottom - y + k + 1, x));
 	 }
	 gy += (halfsize - y)*ims(bottom, x);
	 _Gradient1(gx, gy, x, bottom - y, ima, imd, halfsize);
      }
   }

   // Left borders.
   for (y = halfsize; y < ims.Height() - halfsize; y++) {
      for (x = 0; x < halfsize; x++) {
	 // Convolution with mask -1, 0, 1.
	 gx = gy = 0.0;
	 for (k = 1; k <= halfsize; k++) {
	    gy += Float(ims(y + k, x)) - Float(ims(y - k, x));
	    gx += Float(ims(y, x + k));
	 }
	 for (k = 0; k < x; k++) {
	    gx -= Float(ims(y, x - k - 1));
	 }
	 gx -= (halfsize - x) * ims(y, 0);
	 _Gradient1(gx, gy, x, y, ima, imd, halfsize);
      }
   }
   // Right borders
   for (y = halfsize; y < ims.Height() - halfsize; y++) {
      for (x = 0; x  <halfsize; x++) {
	 // Convolution with mask -1, 0, 1.
	 gx = gy = 0.0;
	 int right = ims.Width() - 1;
	 for (k = 1; k <= halfsize; k++) {
	    gy += Float(ims(y + k, right - x)) - Float(ims(y - k, right - x));
	    gx -= Float(ims(y, right - x - k));
	 }
 	 for (k = 0; k < x; k++) {
	    gx += Float(ims(y, right - x + k + 1));
 	 }
	 gx += (halfsize - x) * ims(y, right);
	 _Gradient1(gx, gy, right - x, y, ima, imd, halfsize);
      }
   }

   // General case.
   for (y = halfsize; y < ims.Height() - halfsize; y++) {
      for (x = halfsize; x < ims.Width() - halfsize; x++) {
	 // Convolution with mask -1, 0, 1.
	 gx = gy = 0.0;
	 for (k = 1; k <= halfsize; k++) {
	    gx += Float(ims(y, x + k)) - Float(ims(y, x - k));
	    gy += Float(ims(y + k, x)) - Float(ims(y - k, x));
	 }
	 _Gradient1(gx, gy, x, y, ima, imd, halfsize);
      }
   }
   return SUCCESS;
}

static void _Gradient1( double gx, double gy, int x, int y, Img2dsl &ima, Img2duc &imd, const int halfsize ) {
   double dx = fabs(gx);
   double dy = fabs(gy);
   
   ima(y, x) = (Img2dsl::ValueType)( (dx + dy) / (2.0 * halfsize));
   
   // Discretization with Freeman code -> use sign
   // to determine the related quadran.
   // Determination of the sign of each derivation component.
   double sx  = (gx > 0)? -1 : ((gx==0)? 0 : +1);
   double sy  = (gy > 0)? -1 : ((gy==0)? 0 : +1);
   imd(y, x) = 0;
   for (int k = 0; k < 8; k++) {
      if ((v8y[k] == sy) && (v8x[k] == sx)) {
	 imd(y, x) = k;
      }
   }
}

Errc PGradient( const Img2dsl &ims, Img2dsl &ima, Img2duc &imd, const int halfsize ) {
   double gx, gy;
   register int x, y, k;

   if (halfsize < 1) {
      std::cerr << "Error pgradient: bad size value: "<<halfsize<< std::endl;
      return FAILURE;
   }

   // Upper left Corner
   for (y = 0; y < halfsize; y++) {
      for (x = 0; x < halfsize; x++ ) {
	 gx = gy = 0.0;
	 for (k  =1; k <= halfsize; k++) {
	    if (x - k < 0) {
	       gx -= Float(ims(y, 0));
	    } else {
	       gx -= Float(ims(y, x - k));
	    }
	    gx += Float(ims(y, x + k));
	    if (y - k < 0) {
	       gy -= Float(ims(0, x));
	    } else {
	       gy -= Float(ims(y - k, x));
	    }
	    gy += Float(ims(y + k, x));
	 }
	 _Gradient1(gx, gy, x, y, ima, imd, halfsize);	 
      }
   }

   // Upper right Corner
   for (y = 0; y < halfsize; y++) {
      for (x = ims.Width() - 1; x > ims.Width() - 1 - halfsize; x--) {
	 gx = gy = 0.0;
	 for (k = 1; k <= halfsize; k++) {
	    if (x+k>ims.Width() - 1) {
	       gx += Float(ims(y, ims.Width() - 1));
	    } else {
	       gx += Float(ims(y, x + k));
	    }
	    gx -= Float(ims(y, x - k));
	    if (y - k < 0) {
	       gy -= Float(ims(0, x));
	    } else {
	       gy -= Float(ims(y - k, x));
	    }
	    gy += Float(ims(y + k, x));
	 }
	 _Gradient1(gx, gy, x, y, ima, imd, halfsize);	 
      }
   }

   // Lower left Corner
   for (y = ims.Height() - 1; y > ims.Height() - 1 - halfsize; y--) {
      for (x = 0; x < halfsize; x++ ) {
	 gx = gy = 0.0;
	 for (k = 1; k <= halfsize; k++) {
	    if (x - k < 0) {
	       gx -= Float(ims(y, 0));
	    } else {
	       gx -= Float(ims(y, x - k));
	    }
	    gx += Float(ims(y, x + k));
	    if (y + k > ims.Height() - 1) {
	       gy += Float(ims(ims.Height() - 1, x));
	    } else {
	       gy += Float(ims(y + k, x));
	    }
	    gy -= Float(ims(y - k, x));
	 }
	 _Gradient1(gx, gy, x, y, ima, imd, halfsize);	 
      }
   }

   // Lower right Corner
   for (y = ims.Height() - 1; y > ims.Height() - 1 - halfsize; y--) {
      for (x = ims.Width() - 1; x > ims.Width() - 1 - halfsize; x--) {
	 gx = gy = 0.0;
	 for (k = 1; k <= halfsize; k++) {
	    if (x + k > ims.Width() - 1) {
	       gx += Float(ims(y, ims.Width() - 1));
	    } else {
	       gx += Float(ims(y, x + k));
	    }
	    gx -= Float(ims(y, x - k));
	    if (y + k > ims.Height() - 1) {
	       gy += Float(ims(0, x));
	    } else {
	       gy += Float(ims(y + k, x));
	    }
	    gy -= Float(ims(y - k, x));
	 }
	 _Gradient1(gx, gy, x, y, ima, imd, halfsize);	 
      }
   }

   // Upper borders.
   for (y = 0; y < halfsize; y++) {
      for (x = halfsize; x < ims.Width() - halfsize; x++) {
	 // Convolution with mask -1, 0, 1.
	 gx = gy = 0.0;
	 for (k = 1; k <= halfsize; k++) {
	    gx += Float(ims(y, x + k)) - Float(ims(y, x - k));
	    gy += Float(ims(y + k, x));
	 }
	 for (k = 0; k < y; k++) {
	    gy -= Float(ims(y - k - 1, x));
	 }
	 gy -= (halfsize - y)*ims(0, x);
	 _Gradient1(gx, gy, x, y, ima, imd, halfsize);
      }
   }
   // Lower borders
   for (y = 0; y < halfsize; y++) {
      for (x = halfsize; x < ims.Width() - halfsize; x++) {
	 // Convolution with mask -1, 0, 1.
	 gx = gy = 0.0;
	 int bottom = ims.Height() - 1;
	 for (k = 1; k <= halfsize; k++) {
	    gx += Float(ims(bottom - y, x + k)) - Float(ims(bottom - y, x - k));
	    gy -= Float(ims(bottom - y - k, x));
	 }
 	 for (k=0; k<y; k++) {
	    gy += Float(ims(bottom - y + k + 1, x));
 	 }
	 gy += (halfsize - y)*ims(bottom, x);
	 _Gradient1(gx, gy, x, bottom - y, ima, imd, halfsize);
      }
   }

   // Left borders.
   for (y = halfsize; y < ims.Height() - halfsize; y++) {
      for (x = 0; x < halfsize; x++) {
	 // Convolution with mask -1, 0, 1.
	 gx = gy = 0.0;
	 for (k = 1; k <= halfsize; k++) {
	    gy += Float(ims(y + k, x)) - Float(ims(y - k, x));
	    gx += Float(ims(y, x + k));
	 }
	 for (k = 0; k < x; k++) {
	    gx -= Float(ims(y, x - k - 1));
	 }
	 gx -= (halfsize - x) * ims(y, 0);
	 _Gradient1(gx, gy, x, y, ima, imd, halfsize);
      }
   }
   // Right borders
   for (y = halfsize; y < ims.Height() - halfsize; y++) {
      for (x = 0; x  <halfsize; x++) {
	 // Convolution with mask -1, 0, 1.
	 gx = gy = 0.0;
	 int right = ims.Width() - 1;
	 for (k = 1; k <= halfsize; k++) {
	    gy += Float(ims(y + k, right - x)) - Float(ims(y - k, right - x));
	    gx -= Float(ims(y, right - x - k));
	 }
 	 for (k = 0; k < x; k++) {
	    gx += Float(ims(y, right - x + k + 1));
 	 }
	 gx += (halfsize - x) * ims(y, right);
	 _Gradient1(gx, gy, right - x, y, ima, imd, halfsize);
      }
   }

   // General case.
   for (y = halfsize; y < ims.Height() - halfsize; y++) {
      for (x = halfsize; x < ims.Width() - halfsize; x++) {
	 // Convolution with mask -1, 0, 1.
	 gx = gy = 0.0;
	 for (k = 1; k <= halfsize; k++) {
	    gx += Float(ims(y, x + k)) - Float(ims(y, x - k));
	    gy += Float(ims(y + k, x)) - Float(ims(y - k, x));
	 }
	 _Gradient1(gx, gy, x, y, ima, imd, halfsize);
      }
   }
   return SUCCESS;
}

static void _Gradient1( double gx, double gy, int x, int y, Img2dsf &ima, Img2duc &imd, const int halfsize ) {
   double dx = fabs(gx);
   double dy = fabs(gy);
   
   ima(y, x) = (Img2dsf::ValueType)( (dx + dy) / (2.0 * halfsize));
   
   // Discretization with Freeman code -> use sign
   // to determine the related quadran.
   // Determination of the sign of each derivation component.
   double sx  = (gx > 0)? -1 : ((gx==0)? 0 : +1);
   double sy  = (gy > 0)? -1 : ((gy==0)? 0 : +1);
   imd(y, x) = 0;
   for (int k = 0; k < 8; k++) {
      if ((v8y[k] == sy) && (v8x[k] == sx)) {
	 imd(y, x) = k;
      }
   }
}

Errc PGradient( const Img2dsf &ims, Img2dsf &ima, Img2duc &imd, const int halfsize ) {
   double gx, gy;
   register int x, y, k;

   if (halfsize < 1) {
      std::cerr << "Error pgradient: bad size value: "<<halfsize<< std::endl;
      return FAILURE;
   }

   // Upper left Corner
   for (y = 0; y < halfsize; y++) {
      for (x = 0; x < halfsize; x++ ) {
	 gx = gy = 0.0;
	 for (k  =1; k <= halfsize; k++) {
	    if (x - k < 0) {
	       gx -= Float(ims(y, 0));
	    } else {
	       gx -= Float(ims(y, x - k));
	    }
	    gx += Float(ims(y, x + k));
	    if (y - k < 0) {
	       gy -= Float(ims(0, x));
	    } else {
	       gy -= Float(ims(y - k, x));
	    }
	    gy += Float(ims(y + k, x));
	 }
	 _Gradient1(gx, gy, x, y, ima, imd, halfsize);	 
      }
   }

   // Upper right Corner
   for (y = 0; y < halfsize; y++) {
      for (x = ims.Width() - 1; x > ims.Width() - 1 - halfsize; x--) {
	 gx = gy = 0.0;
	 for (k = 1; k <= halfsize; k++) {
	    if (x+k>ims.Width() - 1) {
	       gx += Float(ims(y, ims.Width() - 1));
	    } else {
	       gx += Float(ims(y, x + k));
	    }
	    gx -= Float(ims(y, x - k));
	    if (y - k < 0) {
	       gy -= Float(ims(0, x));
	    } else {
	       gy -= Float(ims(y - k, x));
	    }
	    gy += Float(ims(y + k, x));
	 }
	 _Gradient1(gx, gy, x, y, ima, imd, halfsize);	 
      }
   }

   // Lower left Corner
   for (y = ims.Height() - 1; y > ims.Height() - 1 - halfsize; y--) {
      for (x = 0; x < halfsize; x++ ) {
	 gx = gy = 0.0;
	 for (k = 1; k <= halfsize; k++) {
	    if (x - k < 0) {
	       gx -= Float(ims(y, 0));
	    } else {
	       gx -= Float(ims(y, x - k));
	    }
	    gx += Float(ims(y, x + k));
	    if (y + k > ims.Height() - 1) {
	       gy += Float(ims(ims.Height() - 1, x));
	    } else {
	       gy += Float(ims(y + k, x));
	    }
	    gy -= Float(ims(y - k, x));
	 }
	 _Gradient1(gx, gy, x, y, ima, imd, halfsize);	 
      }
   }

   // Lower right Corner
   for (y = ims.Height() - 1; y > ims.Height() - 1 - halfsize; y--) {
      for (x = ims.Width() - 1; x > ims.Width() - 1 - halfsize; x--) {
	 gx = gy = 0.0;
	 for (k = 1; k <= halfsize; k++) {
	    if (x + k > ims.Width() - 1) {
	       gx += Float(ims(y, ims.Width() - 1));
	    } else {
	       gx += Float(ims(y, x + k));
	    }
	    gx -= Float(ims(y, x - k));
	    if (y + k > ims.Height() - 1) {
	       gy += Float(ims(0, x));
	    } else {
	       gy += Float(ims(y + k, x));
	    }
	    gy -= Float(ims(y - k, x));
	 }
	 _Gradient1(gx, gy, x, y, ima, imd, halfsize);	 
      }
   }

   // Upper borders.
   for (y = 0; y < halfsize; y++) {
      for (x = halfsize; x < ims.Width() - halfsize; x++) {
	 // Convolution with mask -1, 0, 1.
	 gx = gy = 0.0;
	 for (k = 1; k <= halfsize; k++) {
	    gx += Float(ims(y, x + k)) - Float(ims(y, x - k));
	    gy += Float(ims(y + k, x));
	 }
	 for (k = 0; k < y; k++) {
	    gy -= Float(ims(y - k - 1, x));
	 }
	 gy -= (halfsize - y)*ims(0, x);
	 _Gradient1(gx, gy, x, y, ima, imd, halfsize);
      }
   }
   // Lower borders
   for (y = 0; y < halfsize; y++) {
      for (x = halfsize; x < ims.Width() - halfsize; x++) {
	 // Convolution with mask -1, 0, 1.
	 gx = gy = 0.0;
	 int bottom = ims.Height() - 1;
	 for (k = 1; k <= halfsize; k++) {
	    gx += Float(ims(bottom - y, x + k)) - Float(ims(bottom - y, x - k));
	    gy -= Float(ims(bottom - y - k, x));
	 }
 	 for (k=0; k<y; k++) {
	    gy += Float(ims(bottom - y + k + 1, x));
 	 }
	 gy += (halfsize - y)*ims(bottom, x);
	 _Gradient1(gx, gy, x, bottom - y, ima, imd, halfsize);
      }
   }

   // Left borders.
   for (y = halfsize; y < ims.Height() - halfsize; y++) {
      for (x = 0; x < halfsize; x++) {
	 // Convolution with mask -1, 0, 1.
	 gx = gy = 0.0;
	 for (k = 1; k <= halfsize; k++) {
	    gy += Float(ims(y + k, x)) - Float(ims(y - k, x));
	    gx += Float(ims(y, x + k));
	 }
	 for (k = 0; k < x; k++) {
	    gx -= Float(ims(y, x - k - 1));
	 }
	 gx -= (halfsize - x) * ims(y, 0);
	 _Gradient1(gx, gy, x, y, ima, imd, halfsize);
      }
   }
   // Right borders
   for (y = halfsize; y < ims.Height() - halfsize; y++) {
      for (x = 0; x  <halfsize; x++) {
	 // Convolution with mask -1, 0, 1.
	 gx = gy = 0.0;
	 int right = ims.Width() - 1;
	 for (k = 1; k <= halfsize; k++) {
	    gy += Float(ims(y + k, right - x)) - Float(ims(y - k, right - x));
	    gx -= Float(ims(y, right - x - k));
	 }
 	 for (k = 0; k < x; k++) {
	    gx += Float(ims(y, right - x + k + 1));
 	 }
	 gx += (halfsize - x) * ims(y, right);
	 _Gradient1(gx, gy, right - x, y, ima, imd, halfsize);
      }
   }

   // General case.
   for (y = halfsize; y < ims.Height() - halfsize; y++) {
      for (x = halfsize; x < ims.Width() - halfsize; x++) {
	 // Convolution with mask -1, 0, 1.
	 gx = gy = 0.0;
	 for (k = 1; k <= halfsize; k++) {
	    gx += Float(ims(y, x + k)) - Float(ims(y, x - k));
	    gy += Float(ims(y + k, x)) - Float(ims(y - k, x));
	 }
	 _Gradient1(gx, gy, x, y, ima, imd, halfsize);
      }
   }
   return SUCCESS;
}


Errc PGradient( const Imc2duc &ims, Img2duc &ima, Img2duc &imd, const int halfsize) {
   int x, y, k;
   float dxX, dyX, dxY, dyY, dxZ, dyZ;
   float p, q, t;
   double lambda1, lambda2;
   Long sx, sy;

   for (y = halfsize; y < ims.Height() - halfsize; y++) {
      for (x = halfsize; x < ims.Width() - halfsize; x++) {
	 
	 dxX = dyX = dxY = dyY = dxZ = dyZ = 0.0F;
	 for (k=1; k<=halfsize; k++) {
	    dxX += Float(ims(0, y, x+k)) - Float(ims(0, y, x-k));
	    dyX += Float(ims(0, y+k, x)) - Float(ims(0, y-k, x));
	    
	    dxY += Float(ims(1, y, x+k)) - Float(ims(1, y, x-k));
	    dyY += Float(ims(1, y+k, x)) - Float(ims(1, y-k, x));
	    
	    dxZ += Float(ims(2, y, x+k)) - Float(ims(2, y, x-k));
	    dyZ += Float(ims(2, y+k, x)) - Float(ims(2, y-k, x));
	 }
	 // Normalization
	 dxX /= halfsize;
	 dyX /= halfsize;
	 dxY /= halfsize;
	 dyY /= halfsize;
	 dxZ /= halfsize;
	 dyZ /= halfsize;
	 
	 p = dxX * dxX + dxY * dxY + dxZ * dxZ;
	 q = dyX * dyX + dyY * dyY + dyZ * dyZ;
	 t = dxX * dyX + dxY * dyY + dxZ * dyZ;
	 
	 lambda1 = (p + q + sqrt(((p - q) * (p - q) + 4 * t * t))) / 2.0;
	 lambda2 = (p + q - sqrt(((p - q) * (p - q) + 4 * t * t))) / 2.0;
	 double amp = sqrt(lambda1 + lambda2);

	 if (amp > Limits<Img2duc::ValueType>::max()) {
	    amp = Limits<Img2duc::ValueType>::max();
	 }
	 ima(y, x) = (Img2duc::ValueType)amp;
	 
	 // Discretization with Freeman code -> use sign
	 // to determine the related quadran.
	 
	 // Determination of the sign of each derivation component.
	 float v = p - q;
	 sx  = (v > 0)? -1 : ((v == 0)? 0 : +1);
	 sy  = (t > 0)? -1 : ((t == 0)? 0 : +1);
	 // Discretization with Freeman code.
	 imd(y, x)=0;
	 for (k = 0; k < 8; k++) {
	    if ((v8y[k] == sy) && (v8x[k] == sx)) {
	       imd(y, x) = k / 2;
	    }
	 }
      }
   }

   return SUCCESS;
}

Errc PGradient( const Imc2dsl &ims, Img2dsl &ima, Img2duc &imd, const int halfsize) {
   int x, y, k;
   float dxX, dyX, dxY, dyY, dxZ, dyZ;
   float p, q, t;
   double lambda1, lambda2;
   Long sx, sy;

   for (y = halfsize; y < ims.Height() - halfsize; y++) {
      for (x = halfsize; x < ims.Width() - halfsize; x++) {
	 
	 dxX = dyX = dxY = dyY = dxZ = dyZ = 0.0F;
	 for (k=1; k<=halfsize; k++) {
	    dxX += Float(ims(0, y, x+k)) - Float(ims(0, y, x-k));
	    dyX += Float(ims(0, y+k, x)) - Float(ims(0, y-k, x));
	    
	    dxY += Float(ims(1, y, x+k)) - Float(ims(1, y, x-k));
	    dyY += Float(ims(1, y+k, x)) - Float(ims(1, y-k, x));
	    
	    dxZ += Float(ims(2, y, x+k)) - Float(ims(2, y, x-k));
	    dyZ += Float(ims(2, y+k, x)) - Float(ims(2, y-k, x));
	 }
	 // Normalization
	 dxX /= halfsize;
	 dyX /= halfsize;
	 dxY /= halfsize;
	 dyY /= halfsize;
	 dxZ /= halfsize;
	 dyZ /= halfsize;
	 
	 p = dxX * dxX + dxY * dxY + dxZ * dxZ;
	 q = dyX * dyX + dyY * dyY + dyZ * dyZ;
	 t = dxX * dyX + dxY * dyY + dxZ * dyZ;
	 
	 lambda1 = (p + q + sqrt(((p - q) * (p - q) + 4 * t * t))) / 2.0;
	 lambda2 = (p + q - sqrt(((p - q) * (p - q) + 4 * t * t))) / 2.0;
	 double amp = sqrt(lambda1 + lambda2);

	 if (amp > Limits<Img2dsl::ValueType>::max()) {
	    amp = Limits<Img2dsl::ValueType>::max();
	 }
	 ima(y, x) = (Img2dsl::ValueType)amp;
	 
	 // Discretization with Freeman code -> use sign
	 // to determine the related quadran.
	 
	 // Determination of the sign of each derivation component.
	 float v = p - q;
	 sx  = (v > 0)? -1 : ((v == 0)? 0 : +1);
	 sy  = (t > 0)? -1 : ((t == 0)? 0 : +1);
	 // Discretization with Freeman code.
	 imd(y, x)=0;
	 for (k = 0; k < 8; k++) {
	    if ((v8y[k] == sy) && (v8x[k] == sx)) {
	       imd(y, x) = k / 2;
	    }
	 }
      }
   }

   return SUCCESS;
}

Errc PGradient( const Imc2dsf &ims, Img2dsf &ima, Img2duc &imd, const int halfsize) {
   int x, y, k;
   float dxX, dyX, dxY, dyY, dxZ, dyZ;
   float p, q, t;
   double lambda1, lambda2;
   Long sx, sy;

   for (y = halfsize; y < ims.Height() - halfsize; y++) {
      for (x = halfsize; x < ims.Width() - halfsize; x++) {
	 
	 dxX = dyX = dxY = dyY = dxZ = dyZ = 0.0F;
	 for (k=1; k<=halfsize; k++) {
	    dxX += Float(ims(0, y, x+k)) - Float(ims(0, y, x-k));
	    dyX += Float(ims(0, y+k, x)) - Float(ims(0, y-k, x));
	    
	    dxY += Float(ims(1, y, x+k)) - Float(ims(1, y, x-k));
	    dyY += Float(ims(1, y+k, x)) - Float(ims(1, y-k, x));
	    
	    dxZ += Float(ims(2, y, x+k)) - Float(ims(2, y, x-k));
	    dyZ += Float(ims(2, y+k, x)) - Float(ims(2, y-k, x));
	 }
	 // Normalization
	 dxX /= halfsize;
	 dyX /= halfsize;
	 dxY /= halfsize;
	 dyY /= halfsize;
	 dxZ /= halfsize;
	 dyZ /= halfsize;
	 
	 p = dxX * dxX + dxY * dxY + dxZ * dxZ;
	 q = dyX * dyX + dyY * dyY + dyZ * dyZ;
	 t = dxX * dyX + dxY * dyY + dxZ * dyZ;
	 
	 lambda1 = (p + q + sqrt(((p - q) * (p - q) + 4 * t * t))) / 2.0;
	 lambda2 = (p + q - sqrt(((p - q) * (p - q) + 4 * t * t))) / 2.0;
	 double amp = sqrt(lambda1 + lambda2);

	 if (amp > Limits<Img2dsf::ValueType>::max()) {
	    amp = Limits<Img2dsf::ValueType>::max();
	 }
	 ima(y, x) = (Img2dsf::ValueType)amp;
	 
	 // Discretization with Freeman code -> use sign
	 // to determine the related quadran.
	 
	 // Determination of the sign of each derivation component.
	 float v = p - q;
	 sx  = (v > 0)? -1 : ((v == 0)? 0 : +1);
	 sy  = (t > 0)? -1 : ((t == 0)? 0 : +1);
	 // Discretization with Freeman code.
	 imd(y, x)=0;
	 for (k = 0; k < 8; k++) {
	    if ((v8y[k] == sy) && (v8x[k] == sx)) {
	       imd(y, x) = k / 2;
	    }
	 }
      }
   }

   return SUCCESS;
}


#ifdef MAIN

/*
 * Modify only the following constants, and the function call.
 */
#define	USAGE	"usage: %s halfsize [-m mask] [im_in|-] [im_out|-] [im_out|-]"
#define	PARC	1
#define	FINC	1
#define	FOUTC	2
#define	MASK	2 // Only masking operation.

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
      Img1duc* const ims = (Img1duc*)objs[0];
      objd[0] = new Img1duc(ims->Size());
      objd[1] = new Img1duc(ims->Size());
      Img1duc* const ima = (Img1duc*)objd[0];
      Img1duc* const imd = (Img1duc*)objd[1];
      
      result = PGradient(*ims, *ima, *imd, atoi(parv[0]));
      goto end;
   }

   if (objs[0]->Type() == Po_Img1dsl) {
      Img1dsl* const ims = (Img1dsl*)objs[0];
      objd[0] = new Img1dsl(ims->Size());
      objd[1] = new Img1duc(ims->Size());
      Img1dsl* const ima = (Img1dsl*)objd[0];
      Img1duc* const imd = (Img1duc*)objd[1];
      
      result = PGradient(*ims, *ima, *imd, atoi(parv[0]));
      goto end;
   }

   if (objs[0]->Type() == Po_Img1dsf) {
      Img1dsf* const ims = (Img1dsf*)objs[0];
      objd[0] = new Img1dsf(ims->Size());
      objd[1] = new Img1duc(ims->Size());
      Img1dsf* const ima = (Img1dsf*)objd[0];
      Img1duc* const imd = (Img1duc*)objd[1];
      
      result = PGradient(*ims, *ima, *imd, atoi(parv[0]));
      goto end;
   }

   if (objs[0]->Type() == Po_Img2duc) {
      Img2duc* const ims = (Img2duc*)objs[0];
      objd[0] = new Img2duc(ims->Size());
      objd[1] = new Img2duc(ims->Size());
      Img2duc* const ima = (Img2duc*)objd[0];
      Img2duc* const imd = (Img2duc*)objd[1];
      
      result = PGradient(*ims, *ima, *imd, atoi(parv[0]));
      goto end;
   }

   if (objs[0]->Type() == Po_Img2dsl) {
      Img2dsl* const ims = (Img2dsl*)objs[0];
      objd[0] = new Img2dsl(ims->Size());
      objd[1] = new Img2duc(ims->Size());
      Img2dsl* const ima = (Img2dsl*)objd[0];
      Img2duc* const imd = (Img2duc*)objd[1];
      
      result = PGradient(*ims, *ima, *imd, atoi(parv[0]));
      goto end;
   }

   if (objs[0]->Type() == Po_Img2dsf) {
      Img2dsf* const ims = (Img2dsf*)objs[0];
      objd[0] = new Img2dsf(ims->Size());
      objd[1] = new Img2duc(ims->Size());
      Img2dsf* const ima = (Img2dsf*)objd[0];
      Img2duc* const imd = (Img2duc*)objd[1];
      
      result = PGradient(*ims, *ima, *imd, atoi(parv[0]));
      goto end;
   }

   if (objs[0]->Type() == Po_Img3duc) {
      Img3duc* const ims = (Img3duc*)objs[0];
      objd[0] = new Img3duc(ims->Size());
      objd[1] = new Img3duc(ims->Size());
      Img3duc* const ima = (Img3duc*)objd[0];
      Img3duc* const imd = (Img3duc*)objd[1];
      
      result = PGradient(*ims, *ima, *imd, atoi(parv[0]));
      goto end;
   }

   if (objs[0]->Type() == Po_Img3dsl) {
      Img3dsl* const ims = (Img3dsl*)objs[0];
      objd[0] = new Img3dsl(ims->Size());
      objd[1] = new Img3duc(ims->Size());
      Img3dsl* const ima = (Img3dsl*)objd[0];
      Img3duc* const imd = (Img3duc*)objd[1];
      
      result = PGradient(*ims, *ima, *imd, atoi(parv[0]));
      goto end;
   }

   if (objs[0]->Type() == Po_Img3dsf) {
      Img3dsf* const ims = (Img3dsf*)objs[0];
      objd[0] = new Img3dsf(ims->Size());
      objd[1] = new Img3duc(ims->Size());
      Img3dsf* const ima = (Img3dsf*)objd[0];
      Img3duc* const imd = (Img3duc*)objd[1];
      
      result = PGradient(*ims, *ima, *imd, atoi(parv[0]));
      goto end;
   }

   if (objs[0]->Type() == Po_Imc2duc) {
      Imc2duc* const ims = (Imc2duc*)objs[0];
      objd[0] = new Img2duc(ims->Size());
      Img2duc* const ima = (Img2duc*)objd[0];
      objd[1] = new Img2duc(ims->Size());
      Img2duc* const imd = (Img2duc*)objd[1];
			
      result = PGradient(*ims, *ima, *imd, atoi(parv[0]));
      goto end;
   }

   if (objs[0]->Type() == Po_Imc2dsl) {
      Imc2dsl* const ims = (Imc2dsl*)objs[0];
      objd[0] = new Img2dsl(ims->Size());
      Img2dsl* const ima = (Img2dsl*)objd[0];
      objd[1] = new Img2duc(ims->Size());
      Img2duc* const imd = (Img2duc*)objd[1];
			
      result = PGradient(*ims, *ima, *imd, atoi(parv[0]));
      goto end;
   }

   if (objs[0]->Type() == Po_Imc2dsf) {
      Imc2dsf* const ims = (Imc2dsf*)objs[0];
      objd[0] = new Img2dsf(ims->Size());
      Img2dsf* const ima = (Img2dsf*)objd[0];
      objd[1] = new Img2duc(ims->Size());
      Img2duc* const imd = (Img2duc*)objd[1];
			
      result = PGradient(*ims, *ima, *imd, atoi(parv[0]));
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
