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
 * For more information, refer to:
 * https://clouard.users.greyc.fr/Pandore/
 */

/**
 * @author Régis Clouard - 2003-05-22
 * @author Régis Clouard - 2007-02-08 (invert sphere and cube numSE).
 */

/**
 * @file structelt.h
 *
 * Builds a structuring element.
 */

#ifndef __STRUCT_H__
#define __STRUCT_H__
#include <math.h>

class StructuringElement1d {
public:
   StructuringElement1d( int maxLength ): length(0) {
      _se = new Point1d[maxLength];
   }
   ~StructuringElement1d() {
      delete[] _se;
   }
   void Add( Point1d p ) {
      _se[length++] = p;
   }
   void Move( int i, Point1d &p ) const {
      p.x += _se[i].x;
   }
   int length;

   StructuringElement1d( int , int halfsize ): length(0) {
      Point1d p;
      _se = new Point1d[(halfsize*2+1)];
      for (p.x = 0; p.x < 2 * halfsize + 1; p.x++) {
	 Add(Point1d(p.x - halfsize));
      }
   }
      
private:
   Point1d *_se;
};

class StructuringElement2d {
public:
   StructuringElement2d( int maxLength ): length(0) {
      _se = new Point2d[maxLength];
   }
   ~StructuringElement2d() {
      delete[] _se;
   }
   void Add( Point2d p ) {
      _se[length++] = p;
   }
   void Move( int i, Point2d &p ) const {
      p.x += _se[i].x;
      p.y += _se[i].y;
   }
   int length;

   StructuringElement2d( int numSE, int halfsize ): length(0) {
      _se = new Point2d[(halfsize * 2 + 1) * (halfsize * 2 + 1)];
      Point2d p;

      switch(numSE) {
	  case 0 : // Diamond
	     for (p.y = 0; p.y <= halfsize; p.y++) {
		for (p.x = halfsize - p.y; p.x <= halfsize + p.y; p.x++) {
		   Add(Point2d(p.y  -halfsize, p.x - halfsize));
		   Add(Point2d(halfsize - p.y, p.x - halfsize));
		}
	     }
	     break;
	  case 1:  // Square
	     for (p.y = 0; p.y < 2 * halfsize + 1; p.y++) {
		for (p.x = 0; p.x < 2 * halfsize + 1; p.x++) {
		   Add(Point2d(p.y - halfsize, p.x - halfsize));
		}
	     }
	     break;
	  case 2: // Disc
	     for (p.x = 0; p.x <= halfsize; p.x++) {
		for (p.y = 0;p.y <= (Long)sqrt(((double)halfsize * halfsize) - ((double)p.x * p.x)); p.y++) {
		   Add(Point2d(p.y, p.x));
		   if (p.x > 0) {
		      Add(Point2d(p.y, -p.x));
		   }
		   if (p.y > 0) {
		      Add(Point2d(-p.y, p.x));
		   }
		   if (p.x > 0 && p.y > 0) {
		      Add(Point2d(-p.y, -p.x));
		   }
		}
	     }
	     break;
	  case 3:  // Horizontal line.
	     for (p.x = -halfsize; p.x <= halfsize; p.x++) {
		Add(Point2d(0, p.x));
	     }
	     break;
	  case 4:  // Backslash
	     for (p.y = -halfsize; p.y <= halfsize; p.y++) {
		Add(Point2d(p.y, p.y));
	     }
	     break;
	  case 5:  // Vertical line
	     for (p.y = -halfsize; p.y <= halfsize; p.y++) {
		Add(Point2d(p.y, 0));
	     }
	     break;
	  case 6:  // Slash
	     for (p.y = -halfsize; p.y <= halfsize; p.y++) {
		Add(Point2d(-p.y, p.y));
	     }
	     break;
	  case 7: // Cross
	     for (p.x = -halfsize; p.x <= halfsize; p.x++) {
		Add(Point2d(0,p.x));
		if (p.x != 0) {
		   Add(Point2d(p.x, 0));
		}
	     }
	     break;
	  case 8: // X
	     for (p.x = -halfsize; p.x <= halfsize; p.x++) {
		Add(Point2d(p.x, p.x));
		if (p.x != 0) {
		   Add(Point2d(p.x, -p.x));
		}
	     }
	     break;
	  case 9: // Circle
	     for (p.x = 0; p.x <= halfsize; p.x++) {
		p.y = (Long)sqrt(((double)halfsize * halfsize) - ((double)p.x * p.x));
		Add(Point2d(p.y, p.x));
		if (p.y > 0) {
		   Add(Point2d(-p.y, p.x));
		}
		if (p.x > 0) {
		   Add(Point2d(p.y, -p.x));
		}
		if (p.x > 0 && p.y > 0) {
		   Add(Point2d(-p.y, -p.x));
		}
	     }
	     break;
      }  
   }

   StructuringElement2d( const Img2duc &imse ): length(0) {
      Point2d p;
      const Point2d shift(imse.Size() / 2);
      _se = new Point2d[imse.VectorSize()];
      for (p.y = 0; p.y < imse.Height(); p.y++) {
	 for (p.x = 0; p.x < imse.Width(); p.x++) {
	    if ((imse[p])) {
	       Add(Point2d(p - shift));
	    }
	 }
      }
   }

   StructuringElement2d( int theta, int, int halfsize ): length(0) {
      _se = new Point2d[(halfsize * 2 + 1)];
      Point2d p1, p2(-10000, -10000);
      double costheta = cos(theta * M_PI / 180.0);
      double sintheta = sin(theta * M_PI / 180.0);

      for (int r = -halfsize; r <= halfsize; r++ ) {
	 double y = r * sintheta;
	 double x = r * costheta;
	 if (y >= 0) {
	    y += 0.5;
	 } else {
	    y -= 0.5;
	 }
	 if (x >= 0) {
	    x += 0.5;
	 } else {
	    x -= 0.5;
	 }
	 p1 = Point2d(-(int)y, (int)x);
	 if (p1 != p2) {
	    Add(p1);
	 }
	 p2 = p1;
      }
   }

private:
   Point2d *_se;
};

class StructuringElement3d {
public:
   StructuringElement3d( int maxLength ): length(0) {
      _se = new Point3d[maxLength];
   }
   ~StructuringElement3d() {
      delete[] _se;
   }
   void Add( Point3d p ) {
      _se[length++] = p;
   }
   void Move( int i, Point3d &p ) const {
      p.x += _se[i].x;
      p.y += _se[i].y;
      p.z += _se[i].z;
   }
   int length;
      
   StructuringElement3d( int numSE, int halfsize ): length(0) {
      Point3d p;
      const int length = 2 * halfsize + 1;
      _se = new Point3d[length * length * length];

      switch(numSE) {
	  case 0 : {// Bipyramid
	     Img3duc temp(length, length, length);
	     temp = 0;
	     for (p.z =0 ; p.z <= halfsize; p.z++) {
		for (p.y = 0; p.y <= p.z; p.y++) {
		   for (p.x = 0; p.x <= p.y; p.x++) {
		      temp[p.z][halfsize - p.z + p.y][halfsize - p.x] = 1;
		      temp[p.z][halfsize - p.z + p.y][halfsize + p.x] = 1;
		      temp[p.z][halfsize + p.z - p.y][halfsize + p.x] = 1;
		      temp[p.z][halfsize + p.z - p.y][halfsize - p.x] = 1;
		      temp[2 * halfsize - p.z][halfsize - p.z + p.y][halfsize - p.x] = 1;
		      temp[2 * halfsize - p.z][halfsize - p.z + p.y][halfsize + p.x] = 1;
		      temp[2 * halfsize  -p.z][halfsize + p.z - p.y][halfsize + p.x] = 1;
		      temp[2 * halfsize - p.z][halfsize + p.z - p.y][halfsize - p.x] = 1;
		   }
		}
	     }
	     for (p.z = 0; p.z < 2 * halfsize + 1; p.z++) {
		for (p.y = 0; p.y < 2 * halfsize + 1; p.y++) {
		   for (p.x = 0; p.x < 2 * halfsize + 1; p.x++) {
		      if ((temp[p])) {
			 Add(Point3d(p.z - halfsize, p.y - halfsize, p.x - halfsize));
		      }
		   }
		}
	     }
	  }
	     break;
	  case 1: { // Cube
	     Img3duc temp(length, length, length);
	     temp = MAXUCHAR;
	     for (p.z = 0; p.z < 2 * halfsize + 1; p.z++) {
		for (p.y  =0; p.y < 2 * halfsize + 1; p.y++) {
		   for (p.x = 0; p.x < 2 * halfsize + 1; p.x++) {
		      if ((temp[p])) {
			 Add(Point3d(p.z - halfsize, p.y - halfsize, p.x - halfsize));
		      }
		   }
		}
	     }
	  }
	     break;
	  case 2: {// Sphere
	     Img3duc temp(length, length, length);
	     temp = 0;
	     for (p.z = 0; p.z <= halfsize; p.z++) {
		for (p.x = 0; p.x <= p.z; p.x++) {
		   for (p.y = 0; p.y <= (Long)sqrt(((double)p.z * p.z) - ((double)p.x * p.x)); p.y++) {
		      temp[p.z][halfsize  +p.y][halfsize + p.x] = 1;
		      temp[p.z][halfsize + p.y][halfsize - p.x] = 1;
		      temp[p.z][halfsize - p.y][halfsize + p.x] = 1;
		      temp[p.z][halfsize  -p.y][halfsize - p.x] = 1;
		      temp[2 * halfsize  -p.z][halfsize + p.y][halfsize + p.x] = 1;
		      temp[2 * halfsize  -p.z][halfsize + p.y][halfsize - p.x] = 1;
		      temp[2 * halfsize - p.z][halfsize - p.y][halfsize + p.x] = 1;
		      temp[2 * halfsize - p.z][halfsize - p.y][halfsize-  p.x] = 1;
		   }
		}
	     }
	     for (p.z = 0; p.z < 2 * halfsize + 1; p.z++) {
		for (p.y = 0; p.y < 2 * halfsize + 1; p.y++) {
		   for (p.x = 0; p.x < 2 * halfsize + 1; p.x++) {
		      if ((temp[p])) {
			 Add(Point3d(p.z - halfsize, p.y - halfsize, p.x - halfsize));
		      }
		   }
		}
	     }
	  }
	     break;
	  case 3:  // Horizontal line x-coordinate.
	     for (p.x = -halfsize; p.x <= halfsize; p.x++) {
		Add(Point3d(0, 0, p.x));
	     }
	     break;
	  case 4:  // Horizontal line y-coordinate.
	     for (p.x = -halfsize; p.x <= halfsize; p.x++) {
		Add(Point3d(0, p.x, 0));
	     }
	     break;
	  case 5:  // Horizontal line z-coordinate.
	     for (p.x = -halfsize; p.x <= halfsize; p.x++) {
		Add(Point3d(p.x, 0, 0));
	     }
	     break;
	  case 6:  // Backslash on x-y
	     for (p.y = -halfsize; p.y <= halfsize; p.y++) {
		Add(Point3d(0, p.y, p.y));
	     }
	     break;
	  case 7:  // Backslash on x-z
	     for (p.y = -halfsize; p.y <= halfsize; p.y++) {
		Add(Point3d(p.y, 0, p.y));
	     }
	     break;
	  case 8:  // Backslash on y-z
	     for (p.y = -halfsize; p.y <= halfsize; p.y++) {
		Add(Point3d(p.y, p.y, 0));
	     }
	     break;
	  case 9:  // Slash on x-y
	     for (p.y = -halfsize; p.y <= halfsize; p.y++) {
		Add(Point3d(0, -p.y, p.y));
	     }
	     break;
	  case 10:  // Slash on x-z
	     for (p.y = -halfsize; p.y <= halfsize; p.y++) {
		Add(Point3d(-p.y, 0, p.y));
	     }
	     break;
	  case 11:  // Slash on y-z
	     for (p.y = -halfsize; p.y <= halfsize; p.y++) {
		Add(Point3d(-p.y, p.y, 0));
	     }
	     break;
	  case 12: // Cross
	     for (p.x = -halfsize; p.x <= halfsize; p.x++) {
		Add(Point3d(0, 0, p.x));
		if (p.x != 0) {
		   Add(Point3d(0, p.x, 0));
		}
		if (p.x != 0) {
		   Add(Point3d(p.x, 0, 0));
		}
	     }
	     break;
      }
   }
   
   StructuringElement3d( const Img3duc &imse ): length(0) {
      Point3d p;
      const Point3d shift(imse.Size() / 2);
      _se = new Point3d[imse.VectorSize()];
      for (p.z = 0; p.z < imse.Depth(); p.z++) {
	 for (p.y = 0; p.y < imse.Height(); p.y++) {
	    for (p.x = 0; p.x < imse.Width(); p.x++) {
	       if ((imse[p])) {
		  Add(Point3d(p - shift));
	       }
	    }
	 }
      }
   }

   StructuringElement3d( int theta, int phi, int halfsize ): length(0) {
      _se = new Point3d[(halfsize * 2 + 1)];
      Point3d p1, p2(-1000, -10000, -1000);
      double costheta = cos(theta * M_PI / 180.0);
      double sintheta = sin(theta * M_PI / 180.0);
      double cosphi = cos(phi * M_PI / 180.0);
      double sinphi = sin(phi * M_PI / 180.0);

      for ( int r = -halfsize; r <= halfsize; r++ ) {
	 double x = r  *costheta * cosphi;
	 double y = r * sintheta * cosphi;
	 double z = r * sinphi;
	 if (z >= 0) {
	    z += 0.5;
	 } else {
	    z -= 0.5;
	 }
	 if (y >= 0) {
	    y += 0.5;
	 } else {
	    y -= 0.5;
	 }
	 if (x >= 0) {
	    x += 0.5;
	 } else {
	    x -= 0.5;
	 }
	 p1 = Point3d((int)z, -(int)y, (int)x);
	 if (p1 != p2) {
	    Add(p1);
	 }
	 p2 = p1;
      }
   }

private:
   Point3d *_se;
};

#endif
