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
 * @ file pquadrangle2rectangle.cpp
 *
 * Transforming a quadrangle into a rectangle.
 * The quadrangle is given as a list of four points.
 *
 * @author Florian LEGRAND & Damien LANNOY - 2006-12-11
 * Reference : Z. Zhang, and L. He, "Whiteboard Scanning and Image Enhancement", 
 *             Digital Signal Processing, Vol.17, No.2, pages 414-432, 2007.
 */

#include <cmath>
#include <vector>
#include <pandore.h>
using namespace pandore; 

//#define NDEBUG

class Coord3d{
   public:
      /**   The x coordinate -abscissa. */
      double x;
      /**   The y coordinate -ordinate. */
      double y;
      /**   The y coordinate -depth. */
      double z;
      
      /**
       * Constructs and initializes a point at the origin (0, 0, 0)
       * of the coordinate space.
       */
      Coord3d(): x(0), y(0), z(0) {}
      
      /**
       * Constructs and initializes a point at the specified (i, i, i)
       * location in the coordinate space.
       * @param i	the value for x, y and z coordinates.
       */
      Coord3d( double i ): x(i), y(i), z(i) {}

      /**
       * Constructs and initializes a point at the specified (z, y, x)
       * location in the coordinate space.
       * @param x	the x coordinate.
       * @param y	the y coordinate.
       * @param z	the z coordinate.
       */
      Coord3d( double x, double y, double z): x(x), y(y), z(z) {}

      Coord3d( double x, double y): x(x), y(y), z(1) {}

      /**
       * Constructs and initializes a point with the same location
       * as the reference point.
       * @param p	a point.
       */
      Coord3d( const Point2d& p ):  x((double)p.x), y((double)p.y), z(1.0) { }

      /**
       * Sets the coordinates with the coordinates of specified point.
       * @param pt	a point.
       * @return	the point itself.
       */
      Coord3d operator=( Coord3d pt ) { x = pt.x; y = pt.y; z = pt.z; return *this; }

     /**
       * Returns a new point from the addition
       * of the coordinates of the object with the coordinates of the specified 
       * point.
       * @param pt	the specified point.
       * @return	a new point.
       */
      Coord3d operator+( const Coord3d& pt ) const { return Coord3d(x+pt.x, y+pt.y, z+pt.z); }

      /**
       * Returns a new point from the subtraction
       * of the coordinates of the object with the coordinates of the specified 
       * point.
       * @param pt	the specified point.
       * @return	a new point.
       */
      Coord3d operator-( const Coord3d& pt ) const { return Coord3d(x-pt.x, y-pt.y, z-pt.z); }
      
      /**
       * Returns a new point from the dot product
       * of the object with the coordinates of the specified 
       * point.
       * @param pt	the specified point.
       * @return	a new point.
       */
      double operator*( const Coord3d& pt ) const { return x * pt.x + y * pt.y + z * pt.z; }  
      
      /**
       * Returns a new point from the multiplication
       * of the coordinates of the object with a double
       * @param d the specified double
       * @return	a new point.
       */
      Coord3d operator*( double d ) const { return Coord3d(x * d, y * d, z * d); }
      
      /**
       * Returns a new point from the division
       * of the coordinates of the object with a double
       * @param d the specified double
       * @return	a new point.
       */
      Coord3d operator/( double d ) const { return Coord3d(x / d, y / d, z / d); }
      
      /**
       * Returns the specified coordinates of the object
       */
      double &operator[]( int i ) {
	 switch (i) {
	    case 0: return this->x;
	    case 1: return this->y;
	    case 2: return this->z;
	 }
	 return x;
      }
      
      /**
       * @returns the vectoriel product of the coord3d by c.
       */
      Coord3d vectorProduct( const Coord3d &c ) const {
	 return Coord3d(y * c.z - z * c.y, z * c.x - x * c.z, x * c.y - y * c.x);
      }
      
      /**
       * Returns the square of the coord3d norm
       */
      double squaredNorm() {
	 return (*this)* (*this);
      }
      
      /**
       * Creates and returns a distinct copy of this object.
       */
      Coord3d* Clone() const { return new Coord3d(x, y, z); }
      
      //print the coordinates of the coord3d
      void print() const{
	 printf("%g  %g  %g\n", x, y, z);
      }
};

class Matrice3x3 {
   private:
      Coord3d vect0;
      Coord3d vect1;
      Coord3d vect2;
      
   public:
      
      Matrice3x3() {}
      
      // Fonction de contruction à partir d'un double d, tous les élément de la matrice valent d
      Matrice3x3( double d ) {
	 for (int i = 0; i < 3; i++) {
	    vect0=d;
	    vect1=d;
	    vect2=d;
	 }
      }
      
      //Fonction de construction à partir des 3 vecteurs colonnes
      Matrice3x3( const Coord3d v0, const Coord3d v1, const Coord3d v2 ) {
	 vect0 = v0;
	 vect1 = v1;
	 vect2 = v2;
      }
      
      //Fonction de construction à partir des 9 éléments de la matrice
      Matrice3x3( double d00, double d01, double d02,  double d10,  double d11,  double d12, double d20, double d21, double d22 ) {
	 vect0.x = d00; vect1.x = d01; vect2.x = d02;
	 vect0.y = d10; vect1.y = d11; vect2.y = d12;
	 vect0.z = d20; vect1.z = d21; vect2.z = d22;
      }
      
      //Fonction de constrution à partir d'une autre matrice
      Matrice3x3( const Matrice3x3 &m ) {
	 vect0 = m.colonne(0);
	 vect1 = m.colonne(1);
	 vect2 = m.colonne(2);
      }
      
      //surcharge de l'opérateur égal
      Matrice3x3 operator=( Matrice3x3& m ) {
	 vect0 = m[0];
	 vect1 = m[1];
	 vect2 = m[2];
	 return *this;
      }
      
      //surcharge de l'opérateur plus
      Matrice3x3 operator+( Matrice3x3 m ) {
	 Matrice3x3 ret;
	 ret[0] = vect0 + m[0];
	 ret[1] = vect1 + m[1];
	 ret[2] = vect2 + m[2];
	 return ret;
      }
      
      //surcharge de l'opérateur moins
      Matrice3x3 operator-( Matrice3x3 m ) {
	 Matrice3x3 ret;
	 ret[0] = vect0 - m[0];
	 ret[1] = vect1 - m[1];
	 ret[2] = vect2 - m[2];
	 return ret;
      }
      
      //surcharge de l'opérateur fois pour le produit de 2 matrices
      Matrice3x3 operator*( Matrice3x3 m ) {
	 Matrice3x3 ret(0);
	 for (int i = 0; i < 3; i++) {
	    for (int j = 0; j < 3; j++) {
	       for (int k = 0; k < 3; k++) {
		  ret[j][i] += (*this)[k][i] * m[j][k];
	       }
	    }
	 }
	 return ret;
      }
      
      //surcharge de l'opérateur fois pour la multiplication par un double
      Matrice3x3 operator*( double d ) {
	 Matrice3x3 ret;
	 for (int i = 0; i < 3; i++) {
	    ret[i] = (*this)[i]*d;
	 }
	 
	 return Matrice3x3(ret);
      } 
      
      //Surcharge de l'opérateur fois pour la multiplication par un Coord3d à droite
      Coord3d operator*( Coord3d c ) {
	 Coord3d ret(0);
	 for (int k = 0; k < 3; k++) {
	    ret.x += (*this)[k][0] * c[k];
	    ret.y += (*this)[k][1] * c[k];
	    ret.z += (*this)[k][2] * c[k];
	 }
	 
	 return Coord3d(ret);
      }
      
      
      //Surcharge de l'opérateur diviser pour la division par un double
      Matrice3x3 operator/( double d ) {
	 Matrice3x3 ret;
	 for (int i = 0; i < 3; i++) {
	    ret[i] = (*this)[i] / d;
	 }
	 
	 return ret;
      } 
      
      //Surcharge de l'opérateur [] qui retourne les vecteurs colonnes
      Coord3d &operator[]( int i ) {
	 switch (i) {
	    case 0: return vect0;
	    case 1: return vect1;
	    case 2: return vect2;
	 }
	 return vect0;
      }
      
      //Fonction colonne permettant de retourner les vecteurs colonnes, non modifiable
      Coord3d colonne( int i ) const{
	 switch (i) {
	    case 0: return vect0;
	    case 1: return vect1;
	    case 2: return vect2;
	 }
	 return vect0;
      }
      
      //Fonction qui effectue un affichage des éléments de la matrice
      void print() {
	 printf("%f  %f  %f\n%f  %f  %f\n%f  %f  %f\n", vect0[0], vect1[0], vect2[0], vect0[1], vect1[1], vect2[1], vect0[2], vect1[2], vect2[2]);
      }  
};


/**
 * Calculates the square euclidian distance between two points.
 * @param p1 the fisrt point.
 * @param p2 thesecond point.
 */
double distance_squared( const Point2d p1, const Point2d p2 ) {
   return (p1.x - p2.x) * (p1.x - p2.x) + (p1.y - p2.y) * (p1.y - p2.y);
}

/**
 * Determines the output image size form input image size and 
 * the output rectangle ratio.
 */
void initFrame( Imc2duc &imd, const Point2d *corners, double ratio ) {
    double width  = sqrt(MAX(distance_squared(corners[0], corners[1]), distance_squared(corners[2], corners[3])));
    double height = sqrt(MAX(distance_squared(corners[0], corners[2]), distance_squared(corners[1], corners[3])));

   if (ratio!=0) {
      if ((width / height) >= ratio) {
	 width = ratio * height;
      } else {
	 height = width / ratio;
      }
   }

#ifndef NDEBUG
   printf("Output image size = (%d, %d)\n", (int)width, (int)height);
#endif
   imd.New((int)height, (int)width);
}

/**
 * Estimates the output rectangle aspect ratio.
 * @param imd the output image.
 * @param corners the list of the 4 quadrangle corners.
 */
void rectangleAspectRatio( Imc2duc &imd, const Point2d *corners, int width, int height ) {
   const double u0 = width/2.0;
   const double v0 = height/2.0;

   double m1x = corners[0].x - u0;
   double m1y = corners[0].y - v0;
   double m2x = corners[1].x - u0;
   double m2y = corners[1].y - v0;
   double m3x = corners[2].x - u0;
   double m3y = corners[2].y - v0;
   double m4x = corners[3].x - u0;
   double m4y = corners[3].y - v0;

   // temporary variables k2, k3
   double k2 = ( ( (m1y - m4y) * m3x - (m1x - m4x) * m3y + m1x * m4y - m1y * m4x) /
		  ((m2y - m4y) * m3x - (m2x - m4x) * m3y + m2x * m4y - m2y * m4x));
   
   double k3 = ( ((m1y - m4y) * m2x - (m1x - m4x) * m2y + m1x * m4y - m1y * m4x) / 
		 ((m3y - m4y) * m2x - (m3x - m4x) * m2y + m3x * m4y - m3y * m4x));

   double f_squared =  - (((k3 * m3y - m1y) * (k2 * m2y - m1y) + (k3 * m3x - m1x) * (k2 * m2x - m1x)) / 
			  ((k3 - 1) * (k2 - 1)));
   
   double whRatio;
   if (!(k2=1 || k3==1)) {
      //The width/height ratio of the original rectangle
      whRatio = sqrt( (SQR(k2 - 1) + SQR(k2 * m2y - m1y) / f_squared + SQR(k2 * m2x - m1x) / f_squared) /
		      (SQR(k3 - 1) + SQR(k3 * m3y - m1y) / f_squared + SQR(k3 * m3x - m1x) / f_squared) ) ;
   } else {
      // if k2==1 AND k3==1, then the focal length equation is not solvable 
      // but the focal length is not needed to calculate the ratio.
      // I am still trying to figure out under which circumstances k2 and k3 become 1
      // but it seems to be when the rectangle is not distorted by perspective, 
      // i.e. viewed straight on. Then the equation is obvious:
      whRatio = sqrt( (SQR(m2y - m1y) + SQR(m2x - m1x)) / (SQR(m3y - m1y) + SQR(m3x - m1x)));
   }
   
#ifndef NDEBUG
   printf("   ratio width/height = %lf\n", whRatio);
#endif
   initFrame(imd, corners, whRatio);
}

/**
 * Calculates the homography that transformes the input
 * quadrangle into the output rectangle.
 * @param  corners the list of input quadrangle corners.
 * @param w the width of the output image.
 * @param h the height of the output image.
 * @return the homography matrix.
 */
Matrice3x3 getHomographyMatrix( const Point2d *corners, int w, int h ) {
   double r2 = (double)(corners[1].x * (corners[2].y - corners[0].y) + corners[2].x * (corners[0].y - corners[1].y) + corners[0].x * (corners[1].y - corners[2].y)) / (double)(corners[3].x * (corners[2].y - corners[0].y) + corners[2].x * (corners[0].y  - corners[3].y) + corners[0].x* (corners[3].y - corners[2].y));
   
   double r1 = (double)(corners[1].y - corners[0].y - r2 * (corners[3].y- corners[0].y)) / (corners[0].y - corners[2].y);
   
   double r0 = 1 + r1 - r2;

   return Matrice3x3((r2 * corners[3].x - r1 * corners[2].x) / w, (r1 * corners[2].x - r0 * corners[0].x) / h, r0 * corners[0].x, (r2 * corners[3].y - r1 * corners[2].y) / w, (r1 * corners[2].y - r0 * corners[0].y) / h, r0 * corners[0].y, (r2 - r1) / w, (r1 - r0) / h, r0);
}

/**
 * Tranforms the input image into the output image
 * from the homography matrix and the bilinear interpolation.
 * @param ims the input image.
 * @param imd the output image.
 * @param H the homography matrix.
 */
Errc transformation( const Imc2duc &ims, Imc2duc &imd, Matrice3x3 H ) {
   Coord3d tmp;
   
   for (int x = 0; x < imd.Width(); x++) {
      for (int y = 0; y < imd.Height(); y++) {
	 tmp = H * Coord3d(x, y);
	 tmp = tmp / (tmp[2]);
	 
	 if ((int)tmp.x >= ims.Width() || (int)tmp.x < 0 || (int)tmp.y >= ims.Height() || (int)tmp.y < 0) {
	    for (int b = 0; b < 3; b++) {
	       imd(b, y, x) = 0;
	    }
	 } else {
	    double rx = (double)((int)tmp.x + 1) - tmp.x;
	    double ry = (double)((int)tmp.y + 1) - tmp.y;
	    for (int b = 0; b < 3; b++) {
	       imd(b, y, x) = (int)(rx * (ry * ims(b, (int)tmp.y, (int)tmp.x) + (1 - ry) * ims(b, (int)tmp.y + 1, (int)tmp.x)) + (1 - rx) * (ry * ims(b, (int)tmp.y, (int)tmp.x + 1) + (1 - ry)*ims(b, (int)tmp.y + 1, (int)tmp.x + 1)));
	    }
	 }
      }
   }
   return SUCCESS;
}

/**
 * Sorts the corner in order to have corner[0] in the upper left, 
 * corner[1] in lower left, corner[2] in the lower right and corner[3] in the upper right.
 * @param corners the 4 input corners.
 */
void sortCorners( const std::vector<Point2d> &points, Point2d *corners, int width, int height ) {
   corners[0] = points[0];
   corners[1] = points[0];
   corners[2] = points[0];
   corners[3] = points[0];

#ifndef NDEBUG
   for (std::vector<Point2d>::const_iterator it = points.begin(); it != points.end(); ++it) {
      printf("Before sorting: (%d, %d)\n", (*it).x, (*it).y);
   }   
#endif

   // Corner 0: upper letf.
   for (std::vector<Point2d>::const_iterator it = points.begin(); it != points.end(); ++it) {
      if ( SQR((*it).x) + SQR((*it).y) < SQR(corners[0].x) + SQR(corners[0].y) ) {
 	 corners[0] = *it;
      }
   }
   
   // Corner 1: upper right.
   for (std::vector<Point2d>::const_iterator it = points.begin(); it != points.end(); ++it) {
      if ( SQR(width - (*it).x) + SQR((*it).y) < SQR(width - corners[1].x) + SQR(corners[1].y) ) {
 	 corners[1] = *it;
      }
   }
   
   // Corner 2: lower left.
   for (std::vector<Point2d>::const_iterator it = points.begin(); it != points.end(); ++it) {
      if ( SQR((*it).x) + SQR(height - (*it).y) < SQR(corners[2].x) + SQR(height - corners[2].y) ) {
 	 corners[2] = *it;
      }
   }
   
   // Corner 3: lower right.
   for (std::vector<Point2d>::const_iterator it = points.begin(); it != points.end(); ++it) {
      if ( SQR(width - (*it).x) + SQR(height - (*it).y) < SQR(width  - corners[3].x) + SQR(height - corners[3].y) ) {
 	 corners[3] = *it;
      }
   }
   
#ifndef NDEBUG
   puts("");
   for (int i = 0; i < 4; i++) {
      printf("After sorting %d : (%d, %d)\n", i, corners[i].x, corners[i].y);
   }
#endif
}

bool getCorners( const Img2duc &imc, std::vector<Point2d> &points ) {
   Point2d p;
   for (p.y = 0; p.y < imc.Height(); p.y++) {
      for (p.x = 0; p.x < imc.Width(); p.x++) {
	 if (imc[p] > 0) {
	    points.push_back(p);
	 }
      }
   }
   return (points.size() >= 4);
}

Errc PQuadrangle2Rectangle( const Imc2duc &ims, const Img2duc &imc, Imc2duc &imd ) {
   if (ims.Size() != imc.Size() ) {
      std::cerr<<"Error pquadrangle2rectangle: incompatible image sizes."<<std::endl;
      return FAILURE;
   }
   std::vector<Point2d> *points = new std::vector<Point2d>();
   Point2d *corners = new Point2d[4];
   Errc result = FAILURE;

   if (getCorners(imc, *points)) {
      sortCorners(*points, corners, ims.Width(), ims.Height());
      rectangleAspectRatio(imd, corners, ims.Width(), ims.Height());
      Matrice3x3 homographyMatrix = getHomographyMatrix(corners, imd.Width(), imd.Height());
      transformation(ims, imd, homographyMatrix);
      result = SUCCESS;
   }
   delete[] corners;
   delete points;
   
   return result;
}

#ifdef MAIN

/*
 * Modify only the following constants, and the operator switches.
 */

#define	USAGE	"usage: %s [-m mask] [im1_in|-] [im2_in|-] [im_out|-]"
#define	PARC	0  // Number of parameters
#define	FINC	2  // Number of input images
#define	FOUTC	1  // Number of output images
#define	MASK	1  // Level of masking

int main( int argc, char *argv[] ) {
   Errc result;                // The result code of the execution.
   Pobject* mask;              // The region map.
   Pobject* objin[FINC + 1];   // The input objects;
   Pobject* objs[FINC + 1];    // The source objects masked.
   Pobject* objout[FOUTC + 1]; // The output object.
   Pobject* objd[FOUTC + 1];   // The result object of the execution.
   char* parv[PARC + 1];       // The input parameters.
   
   ReadArgs(argc, argv, PARC, FINC, FOUTC, &mask, objin, objs, objout, objd, parv, USAGE, MASK);
   if (objs[0]->Type() == Po_Imc2duc && objs[1]->Type() == Po_Img2duc) {
      Imc2duc* const ims = (Imc2duc*)objs[0];     
      Img2duc* const imc = (Img2duc*)objs[1];     
      objd[0] = new Imc2duc;
      Imc2duc* imd = (Imc2duc*)objd[0];

      result = PQuadrangle2Rectangle(*ims, *imc, *imd);
   } else {
      PrintErrorFormat(objin, FINC);
      result = FAILURE;
   }	
   
   if (result) {
      WriteArgs(argc, argv, PARC, FINC, FOUTC, &mask, objin, objs, objout, objd, MASK);
   }
   Exit(result);
   return 0;
}

#endif
