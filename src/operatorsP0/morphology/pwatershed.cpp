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
 * @author Régis Clouard - 1996-02-19
 * @author Francois Angot - 1996-08-05
 * @author Olivier Lezoray - 1996-09-30
 * @author Abder Elmoataz - 2002-12-13 (New algorithm).
 * @author Clouard Régis - 2003-06-17 (+ color according to F. Meyer).
 * @author Clouard Régis - 2007-03-02 (fix bug on cle=(float)impot[p1] -> cle=(float)impot[p])
 */

#include <pandore.h>
using namespace pandore;

// TIP: To limit the memory we replace the Point2d and Point3d
// to use Ushort rather than Ulong for storing the coordinates.
// 2 bytes gained.
class point2d {
public:
   Ushort x;
   Ushort y;
   point2d() {}
   point2d( const Point2d &p ) : x(p.x), y(p.y) { }
   point2d &operator=( const point2d & p ) {
      x = p.x;
      y = p.y;
      return *this;
   }
   Point2d toPoint() const {
      return Point2d(y, x);
   }
};

class point3d {
public:
   Ushort x;
   Ushort y;
   Ushort z;
   point3d() {}
   point3d( const Point3d &p ) : x(p.x), y(p.y), z(p.z) {}
   point3d& operator=( const point3d & p ) {
      x = p.x;
      y = p.y;
      z = p.z;
      return *this;
   }
   Point3d toPoint() const {
      return Point3d(z, y, x);
   }
};

/**
 * @file pwatershed.cpp
 *
 * Watershed implemented from a heap.
 * @param ims1 makers image (region map)
 * @param ims2 potential image (intensity image).
 */


Errc PWatershed( const Reg2d &imlabel, const Img2duc &impot, Reg2d &imd ) {
   if (imlabel.Size() != impot.Size()) {
      std::cerr << "Error pwatershed: incompatible image sizes." << std::endl;
      return FAILURE;
   }

   // Estimation of the heap size...
   Heap<point2d, float, Ulong> tas(imlabel.VectorSize() / 2);

   // 1. Initialization.
   // Push boundary seed points into the heap.
   Point2d p;
   for (p.y = 0; p.y < imlabel.Height(); p.y++)
   for (p.x = 0; p.x < imlabel.Width(); p.x++)
   {
      imd[p] = imlabel[p];
      if (imlabel[p] != 0) {
	 // Pop boundary points.
	 for (int v = 0; v < 8; v++) {
	    Point2d p1 = p + v8[v];
	    if (imlabel.Hold(p1) && imlabel[p1] == 0) {
	       tas.Push(point2d(p), (float)impot[p]);
	       break;
	    }
	 }
      }
   }

   // 2. Priority growing.
   Ulong it = 0;
   while (!tas.Empty()) {
      it++;
      p = (tas.Pop()).toPoint();
      for (int v = 0; v < 8; v++) {
	 Point2d p1 = p + v8[v];
	 if (imd.Hold(p1) && imd[p1] ==0) {
	    // Push this new point in the heap.
	    tas.Push(point2d(p1), (float)impot[p1], it);
	    imd[p1] = imd[p];
	 }
      }
   }
   
   imd.Labels(imlabel.Labels());
   return SUCCESS;
}

Errc PWatershed( const Reg2d &imlabel, const Img2dsl &impot, Reg2d &imd ) {
   if (imlabel.Size() != impot.Size()) {
      std::cerr << "Error pwatershed: incompatible image sizes." << std::endl;
      return FAILURE;
   }

   // Estimation of the heap size...
   Heap<point2d, float, Ulong> tas(imlabel.VectorSize() / 2);

   // 1. Initialization.
   // Push boundary seed points into the heap.
   Point2d p;
   for (p.y = 0; p.y < imlabel.Height(); p.y++)
   for (p.x = 0; p.x < imlabel.Width(); p.x++)
   {
      imd[p] = imlabel[p];
      if (imlabel[p] != 0) {
	 // Pop boundary points.
	 for (int v = 0; v < 8; v++) {
	    Point2d p1 = p + v8[v];
	    if (imlabel.Hold(p1) && imlabel[p1] == 0) {
	       tas.Push(point2d(p), (float)impot[p]);
	       break;
	    }
	 }
      }
   }

   // 2. Priority growing.
   Ulong it = 0;
   while (!tas.Empty()) {
      it++;
      p = (tas.Pop()).toPoint();
      for (int v = 0; v < 8; v++) {
	 Point2d p1 = p + v8[v];
	 if (imd.Hold(p1) && imd[p1] ==0) {
	    // Push this new point in the heap.
	    tas.Push(point2d(p1), (float)impot[p1], it);
	    imd[p1] = imd[p];
	 }
      }
   }
   
   imd.Labels(imlabel.Labels());
   return SUCCESS;
}

Errc PWatershed( const Reg2d &imlabel, const Img2dsf &impot, Reg2d &imd ) {
   if (imlabel.Size() != impot.Size()) {
      std::cerr << "Error pwatershed: incompatible image sizes." << std::endl;
      return FAILURE;
   }

   // Estimation of the heap size...
   Heap<point2d, float, Ulong> tas(imlabel.VectorSize() / 2);

   // 1. Initialization.
   // Push boundary seed points into the heap.
   Point2d p;
   for (p.y = 0; p.y < imlabel.Height(); p.y++)
   for (p.x = 0; p.x < imlabel.Width(); p.x++)
   {
      imd[p] = imlabel[p];
      if (imlabel[p] != 0) {
	 // Pop boundary points.
	 for (int v = 0; v < 8; v++) {
	    Point2d p1 = p + v8[v];
	    if (imlabel.Hold(p1) && imlabel[p1] == 0) {
	       tas.Push(point2d(p), (float)impot[p]);
	       break;
	    }
	 }
      }
   }

   // 2. Priority growing.
   Ulong it = 0;
   while (!tas.Empty()) {
      it++;
      p = (tas.Pop()).toPoint();
      for (int v = 0; v < 8; v++) {
	 Point2d p1 = p + v8[v];
	 if (imd.Hold(p1) && imd[p1] ==0) {
	    // Push this new point in the heap.
	    tas.Push(point2d(p1), (float)impot[p1], it);
	    imd[p1] = imd[p];
	 }
      }
   }
   
   imd.Labels(imlabel.Labels());
   return SUCCESS;
}

Errc PWatershed( const Reg3d &imlabel, const Img3duc &impot, Reg3d &imd ) {
   if (imlabel.Size() != impot.Size()) {
      std::cerr << "Error pwatershed: incompatible image sizes." << std::endl;
      return FAILURE;
   }

   // Estimation of the heap size...
   Heap<point3d, float, Ulong> tas(imlabel.VectorSize() / 2);

   // 1. Initialization.
   // Push boundary seed points into the heap.
   Point3d p;
   for (p.z = 0; p.z < imlabel.Depth(); p.z++)
   for (p.y = 0; p.y < imlabel.Height(); p.y++)
   for (p.x = 0; p.x < imlabel.Width(); p.x++)
   {
      imd[p] = imlabel[p];
      if (imlabel[p] != 0) {
	 // Pop boundary points.
	 for (int v = 0; v < 26; v++) {
	    Point3d p1 = p + v26[v];
	    if (imlabel.Hold(p1) && imlabel[p1] == 0) {
	       tas.Push(point3d(p), (float)impot[p]);
	       break;
	    }
	 }
      }
   }

   // 2. Priority growing.
   Ulong it = 0;
   while (!tas.Empty()) {
      it++;
      p = (tas.Pop()).toPoint();
      for (int v = 0; v < 26; v++) {
	 Point3d p1 = p + v26[v];
	 if (imd.Hold(p1) && imd[p1] ==0) {
	    // Push this new point in the heap.
	    tas.Push(point3d(p1), (float)impot[p1], it);
	    imd[p1] = imd[p];
	 }
      }
   }
   
   imd.Labels(imlabel.Labels());
   return SUCCESS;
}

Errc PWatershed( const Reg3d &imlabel, const Img3dsl &impot, Reg3d &imd ) {
   if (imlabel.Size() != impot.Size()) {
      std::cerr << "Error pwatershed: incompatible image sizes." << std::endl;
      return FAILURE;
   }

   // Estimation of the heap size...
   Heap<point3d, float, Ulong> tas(imlabel.VectorSize() / 2);

   // 1. Initialization.
   // Push boundary seed points into the heap.
   Point3d p;
   for (p.z = 0; p.z < imlabel.Depth(); p.z++)
   for (p.y = 0; p.y < imlabel.Height(); p.y++)
   for (p.x = 0; p.x < imlabel.Width(); p.x++)
   {
      imd[p] = imlabel[p];
      if (imlabel[p] != 0) {
	 // Pop boundary points.
	 for (int v = 0; v < 26; v++) {
	    Point3d p1 = p + v26[v];
	    if (imlabel.Hold(p1) && imlabel[p1] == 0) {
	       tas.Push(point3d(p), (float)impot[p]);
	       break;
	    }
	 }
      }
   }

   // 2. Priority growing.
   Ulong it = 0;
   while (!tas.Empty()) {
      it++;
      p = (tas.Pop()).toPoint();
      for (int v = 0; v < 26; v++) {
	 Point3d p1 = p + v26[v];
	 if (imd.Hold(p1) && imd[p1] ==0) {
	    // Push this new point in the heap.
	    tas.Push(point3d(p1), (float)impot[p1], it);
	    imd[p1] = imd[p];
	 }
      }
   }
   
   imd.Labels(imlabel.Labels());
   return SUCCESS;
}

Errc PWatershed( const Reg3d &imlabel, const Img3dsf &impot, Reg3d &imd ) {
   if (imlabel.Size() != impot.Size()) {
      std::cerr << "Error pwatershed: incompatible image sizes." << std::endl;
      return FAILURE;
   }

   // Estimation of the heap size...
   Heap<point3d, float, Ulong> tas(imlabel.VectorSize() / 2);

   // 1. Initialization.
   // Push boundary seed points into the heap.
   Point3d p;
   for (p.z = 0; p.z < imlabel.Depth(); p.z++)
   for (p.y = 0; p.y < imlabel.Height(); p.y++)
   for (p.x = 0; p.x < imlabel.Width(); p.x++)
   {
      imd[p] = imlabel[p];
      if (imlabel[p] != 0) {
	 // Pop boundary points.
	 for (int v = 0; v < 26; v++) {
	    Point3d p1 = p + v26[v];
	    if (imlabel.Hold(p1) && imlabel[p1] == 0) {
	       tas.Push(point3d(p), (float)impot[p]);
	       break;
	    }
	 }
      }
   }

   // 2. Priority growing.
   Ulong it = 0;
   while (!tas.Empty()) {
      it++;
      p = (tas.Pop()).toPoint();
      for (int v = 0; v < 26; v++) {
	 Point3d p1 = p + v26[v];
	 if (imd.Hold(p1) && imd[p1] ==0) {
	    // Push this new point in the heap.
	    tas.Push(point3d(p1), (float)impot[p1], it);
	    imd[p1] = imd[p];
	 }
      }
   }
   
   imd.Labels(imlabel.Labels());
   return SUCCESS;
}

Errc PWatershed( const Reg2d &imlabel, const Imc2duc &impot, Reg2d &imd ) {
   int r;
   Point2d p, p1;

   if (imlabel.Size() != impot.Size()) {
      std::cerr << "Error pwatershed: incomptible image sizes." << std::endl;
      return FAILURE;
   }
   
   Heap<point2d, double, Ulong> tas(imlabel.VectorSize() / 3);

   double* means[3];
   means[0] = (double*)calloc(imlabel.Labels() + 1, sizeof(double));    // Red
   means[1] = (double*)calloc(imlabel.Labels() + 1, sizeof(double));    // Green
   means[2] = (double*)calloc(imlabel.Labels() + 1, sizeof(double));    // Blue
   Ulong *counts = (Ulong*)calloc(imlabel.Labels() + 1, sizeof(Ulong)); // count

   // Calcul de la moyenne de chaque region
   for (p.y = 0; p.y < imlabel.Height(); p.y++)
   for (p.x = 0; p.x < imlabel.Width(); p.x++)
   {
      if ((r = imlabel[p])) {
	 means[0][r] += (double)impot.X[p];
	 means[1][r] += (double)impot.Y[p];
	 means[2][r] += (double)impot.Z[p];
	 counts[r]++;
      }
   }
   
   for (r = 1; r < (int)imlabel.Labels() + 1; r++) {
      means[0][r] /= (double)counts[r];
      means[1][r] /= (double)counts[r];
      means[2][r] /= (double)counts[r];
   }

   // Initialisation des frontieres marqueurs
   for (p.y = 0; p.y < imlabel.Height(); p.y++)
   for (p.x = 0; p.x < imlabel.Width(); p.x++)
   {
      if ((r = imd[p] = imlabel[p]) == 0) {
	 continue;
      }
      
      for (int v = 0; v < 8; v++) {
	 p1 = p + v8[v];
	 if (imlabel.Hold(p1) && imlabel[p1] == 0) { // Point frontiere
	    double key = (double)(SQR(means[0][r] - impot.X[p]) +
				  SQR(means[1][r] - impot.Y[p]) +
				  SQR(means[2][r] - impot.Z[p]));
	    tas.Push(point2d(p), key);
	    break;
	 }
      }
   }
   
   // Croissance prioritaire.
   Ulong it = 0;
   while (!tas.Empty()) {
      it++;
      p = (tas.Pop()).toPoint();
      r = imd[p];
      for (int v = 0; v < 8; v++) {
	 p1 = p + v8[v];
	 if (imd.Hold(p1) && imd[p1] == 0) {
	    // On empile ce nouveau point avec une clé secondaire
	    // correspondant à l'itération courante.
	    double key = (double)(SQR(means[0][r] - impot.X[p1]) +
				  SQR(means[1][r] - impot.Y[p1]) +
				  SQR(means[2][r] - impot.Z[p1]));
	    tas.Push(point2d(p1), key, it);
	    imd[p1] = r;
	 }
      }
   }
   imd.Labels(imlabel.Labels());
   free(means[0]);
   free(means[1]);
   free(means[2]);
   free(counts);
   
   return SUCCESS;
}

Errc PWatershed( const Reg2d &imlabel, const Imc2dsl &impot, Reg2d &imd ) {
   int r;
   Point2d p, p1;

   if (imlabel.Size() != impot.Size()) {
      std::cerr << "Error pwatershed: incomptible image sizes." << std::endl;
      return FAILURE;
   }
   
   Heap<point2d, double, Ulong> tas(imlabel.VectorSize() / 3);

   double* means[3];
   means[0] = (double*)calloc(imlabel.Labels() + 1, sizeof(double));    // Red
   means[1] = (double*)calloc(imlabel.Labels() + 1, sizeof(double));    // Green
   means[2] = (double*)calloc(imlabel.Labels() + 1, sizeof(double));    // Blue
   Ulong *counts = (Ulong*)calloc(imlabel.Labels() + 1, sizeof(Ulong)); // count

   // Calcul de la moyenne de chaque region
   for (p.y = 0; p.y < imlabel.Height(); p.y++)
   for (p.x = 0; p.x < imlabel.Width(); p.x++)
   {
      if ((r = imlabel[p])) {
	 means[0][r] += (double)impot.X[p];
	 means[1][r] += (double)impot.Y[p];
	 means[2][r] += (double)impot.Z[p];
	 counts[r]++;
      }
   }
   
   for (r = 1; r < (int)imlabel.Labels() + 1; r++) {
      means[0][r] /= (double)counts[r];
      means[1][r] /= (double)counts[r];
      means[2][r] /= (double)counts[r];
   }

   // Initialisation des frontieres marqueurs
   for (p.y = 0; p.y < imlabel.Height(); p.y++)
   for (p.x = 0; p.x < imlabel.Width(); p.x++)
   {
      if ((r = imd[p] = imlabel[p]) == 0) {
	 continue;
      }
      
      for (int v = 0; v < 8; v++) {
	 p1 = p + v8[v];
	 if (imlabel.Hold(p1) && imlabel[p1] == 0) { // Point frontiere
	    double key = (double)(SQR(means[0][r] - impot.X[p]) +
				  SQR(means[1][r] - impot.Y[p]) +
				  SQR(means[2][r] - impot.Z[p]));
	    tas.Push(point2d(p), key);
	    break;
	 }
      }
   }
   
   // Croissance prioritaire.
   Ulong it = 0;
   while (!tas.Empty()) {
      it++;
      p = (tas.Pop()).toPoint();
      r = imd[p];
      for (int v = 0; v < 8; v++) {
	 p1 = p + v8[v];
	 if (imd.Hold(p1) && imd[p1] == 0) {
	    // On empile ce nouveau point avec une clé secondaire
	    // correspondant à l'itération courante.
	    double key = (double)(SQR(means[0][r] - impot.X[p1]) +
				  SQR(means[1][r] - impot.Y[p1]) +
				  SQR(means[2][r] - impot.Z[p1]));
	    tas.Push(point2d(p1), key, it);
	    imd[p1] = r;
	 }
      }
   }
   imd.Labels(imlabel.Labels());
   free(means[0]);
   free(means[1]);
   free(means[2]);
   free(counts);
   
   return SUCCESS;
}

Errc PWatershed( const Reg2d &imlabel, const Imc2dsf &impot, Reg2d &imd ) {
   int r;
   Point2d p, p1;

   if (imlabel.Size() != impot.Size()) {
      std::cerr << "Error pwatershed: incomptible image sizes." << std::endl;
      return FAILURE;
   }
   
   Heap<point2d, double, Ulong> tas(imlabel.VectorSize() / 3);

   double* means[3];
   means[0] = (double*)calloc(imlabel.Labels() + 1, sizeof(double));    // Red
   means[1] = (double*)calloc(imlabel.Labels() + 1, sizeof(double));    // Green
   means[2] = (double*)calloc(imlabel.Labels() + 1, sizeof(double));    // Blue
   Ulong *counts = (Ulong*)calloc(imlabel.Labels() + 1, sizeof(Ulong)); // count

   // Calcul de la moyenne de chaque region
   for (p.y = 0; p.y < imlabel.Height(); p.y++)
   for (p.x = 0; p.x < imlabel.Width(); p.x++)
   {
      if ((r = imlabel[p])) {
	 means[0][r] += (double)impot.X[p];
	 means[1][r] += (double)impot.Y[p];
	 means[2][r] += (double)impot.Z[p];
	 counts[r]++;
      }
   }
   
   for (r = 1; r < (int)imlabel.Labels() + 1; r++) {
      means[0][r] /= (double)counts[r];
      means[1][r] /= (double)counts[r];
      means[2][r] /= (double)counts[r];
   }

   // Initialisation des frontieres marqueurs
   for (p.y = 0; p.y < imlabel.Height(); p.y++)
   for (p.x = 0; p.x < imlabel.Width(); p.x++)
   {
      if ((r = imd[p] = imlabel[p]) == 0) {
	 continue;
      }
      
      for (int v = 0; v < 8; v++) {
	 p1 = p + v8[v];
	 if (imlabel.Hold(p1) && imlabel[p1] == 0) { // Point frontiere
	    double key = (double)(SQR(means[0][r] - impot.X[p]) +
				  SQR(means[1][r] - impot.Y[p]) +
				  SQR(means[2][r] - impot.Z[p]));
	    tas.Push(point2d(p), key);
	    break;
	 }
      }
   }
   
   // Croissance prioritaire.
   Ulong it = 0;
   while (!tas.Empty()) {
      it++;
      p = (tas.Pop()).toPoint();
      r = imd[p];
      for (int v = 0; v < 8; v++) {
	 p1 = p + v8[v];
	 if (imd.Hold(p1) && imd[p1] == 0) {
	    // On empile ce nouveau point avec une clé secondaire
	    // correspondant à l'itération courante.
	    double key = (double)(SQR(means[0][r] - impot.X[p1]) +
				  SQR(means[1][r] - impot.Y[p1]) +
				  SQR(means[2][r] - impot.Z[p1]));
	    tas.Push(point2d(p1), key, it);
	    imd[p1] = r;
	 }
      }
   }
   imd.Labels(imlabel.Labels());
   free(means[0]);
   free(means[1]);
   free(means[2]);
   free(counts);
   
   return SUCCESS;
}

Errc PWatershed( const Reg3d &imlabel, const Imc3duc &impot, Reg3d &imd ) {
   int r;
   Point3d p, p1;

   if (imlabel.Size() != impot.Size()) {
      std::cerr << "Error pwatershed: incomptible image sizes." << std::endl;
      return FAILURE;
   }
   
   Heap<point3d, double, Ulong> tas(imlabel.VectorSize() / 3);

   double* means[3];
   means[0] = (double*)calloc(imlabel.Labels() + 1, sizeof(double));    // Red
   means[1] = (double*)calloc(imlabel.Labels() + 1, sizeof(double));    // Green
   means[2] = (double*)calloc(imlabel.Labels() + 1, sizeof(double));    // Blue
   Ulong *counts = (Ulong*)calloc(imlabel.Labels() + 1, sizeof(Ulong)); // count

   // Calcul de la moyenne de chaque region
   for (p.z = 0; p.z < imlabel.Depth(); p.z++)
   for (p.y = 0; p.y < imlabel.Height(); p.y++)
   for (p.x = 0; p.x < imlabel.Width(); p.x++)
   {
      if ((r = imlabel[p])) {
	 means[0][r] += (double)impot.X[p];
	 means[1][r] += (double)impot.Y[p];
	 means[2][r] += (double)impot.Z[p];
	 counts[r]++;
      }
   }
   
   for (r = 1; r < (int)imlabel.Labels() + 1; r++) {
      means[0][r] /= (double)counts[r];
      means[1][r] /= (double)counts[r];
      means[2][r] /= (double)counts[r];
   }

   // Initialisation des frontieres marqueurs
   for (p.z = 0; p.z < imlabel.Depth(); p.z++)
   for (p.y = 0; p.y < imlabel.Height(); p.y++)
   for (p.x = 0; p.x < imlabel.Width(); p.x++)
   {
      if ((r = imd[p] = imlabel[p]) == 0) {
	 continue;
      }
      
      for (int v = 0; v < 26; v++) {
	 p1 = p + v26[v];
	 if (imlabel.Hold(p1) && imlabel[p1] == 0) { // Point frontiere
	    double key = (double)(SQR(means[0][r] - impot.X[p]) +
				  SQR(means[1][r] - impot.Y[p]) +
				  SQR(means[2][r] - impot.Z[p]));
	    tas.Push(point3d(p), key);
	    break;
	 }
      }
   }
   
   // Croissance prioritaire.
   Ulong it = 0;
   while (!tas.Empty()) {
      it++;
      p = (tas.Pop()).toPoint();
      r = imd[p];
      for (int v = 0; v < 26; v++) {
	 p1 = p + v26[v];
	 if (imd.Hold(p1) && imd[p1] == 0) {
	    // On empile ce nouveau point avec une clé secondaire
	    // correspondant à l'itération courante.
	    double key = (double)(SQR(means[0][r] - impot.X[p1]) +
				  SQR(means[1][r] - impot.Y[p1]) +
				  SQR(means[2][r] - impot.Z[p1]));
	    tas.Push(point3d(p1), key, it);
	    imd[p1] = r;
	 }
      }
   }
   imd.Labels(imlabel.Labels());
   free(means[0]);
   free(means[1]);
   free(means[2]);
   free(counts);
   
   return SUCCESS;
}

Errc PWatershed( const Reg3d &imlabel, const Imc3dsl &impot, Reg3d &imd ) {
   int r;
   Point3d p, p1;

   if (imlabel.Size() != impot.Size()) {
      std::cerr << "Error pwatershed: incomptible image sizes." << std::endl;
      return FAILURE;
   }
   
   Heap<point3d, double, Ulong> tas(imlabel.VectorSize() / 3);

   double* means[3];
   means[0] = (double*)calloc(imlabel.Labels() + 1, sizeof(double));    // Red
   means[1] = (double*)calloc(imlabel.Labels() + 1, sizeof(double));    // Green
   means[2] = (double*)calloc(imlabel.Labels() + 1, sizeof(double));    // Blue
   Ulong *counts = (Ulong*)calloc(imlabel.Labels() + 1, sizeof(Ulong)); // count

   // Calcul de la moyenne de chaque region
   for (p.z = 0; p.z < imlabel.Depth(); p.z++)
   for (p.y = 0; p.y < imlabel.Height(); p.y++)
   for (p.x = 0; p.x < imlabel.Width(); p.x++)
   {
      if ((r = imlabel[p])) {
	 means[0][r] += (double)impot.X[p];
	 means[1][r] += (double)impot.Y[p];
	 means[2][r] += (double)impot.Z[p];
	 counts[r]++;
      }
   }
   
   for (r = 1; r < (int)imlabel.Labels() + 1; r++) {
      means[0][r] /= (double)counts[r];
      means[1][r] /= (double)counts[r];
      means[2][r] /= (double)counts[r];
   }

   // Initialisation des frontieres marqueurs
   for (p.z = 0; p.z < imlabel.Depth(); p.z++)
   for (p.y = 0; p.y < imlabel.Height(); p.y++)
   for (p.x = 0; p.x < imlabel.Width(); p.x++)
   {
      if ((r = imd[p] = imlabel[p]) == 0) {
	 continue;
      }
      
      for (int v = 0; v < 26; v++) {
	 p1 = p + v26[v];
	 if (imlabel.Hold(p1) && imlabel[p1] == 0) { // Point frontiere
	    double key = (double)(SQR(means[0][r] - impot.X[p]) +
				  SQR(means[1][r] - impot.Y[p]) +
				  SQR(means[2][r] - impot.Z[p]));
	    tas.Push(point3d(p), key);
	    break;
	 }
      }
   }
   
   // Croissance prioritaire.
   Ulong it = 0;
   while (!tas.Empty()) {
      it++;
      p = (tas.Pop()).toPoint();
      r = imd[p];
      for (int v = 0; v < 26; v++) {
	 p1 = p + v26[v];
	 if (imd.Hold(p1) && imd[p1] == 0) {
	    // On empile ce nouveau point avec une clé secondaire
	    // correspondant à l'itération courante.
	    double key = (double)(SQR(means[0][r] - impot.X[p1]) +
				  SQR(means[1][r] - impot.Y[p1]) +
				  SQR(means[2][r] - impot.Z[p1]));
	    tas.Push(point3d(p1), key, it);
	    imd[p1] = r;
	 }
      }
   }
   imd.Labels(imlabel.Labels());
   free(means[0]);
   free(means[1]);
   free(means[2]);
   free(counts);
   
   return SUCCESS;
}

Errc PWatershed( const Reg3d &imlabel, const Imc3dsf &impot, Reg3d &imd ) {
   int r;
   Point3d p, p1;

   if (imlabel.Size() != impot.Size()) {
      std::cerr << "Error pwatershed: incomptible image sizes." << std::endl;
      return FAILURE;
   }
   
   Heap<point3d, double, Ulong> tas(imlabel.VectorSize() / 3);

   double* means[3];
   means[0] = (double*)calloc(imlabel.Labels() + 1, sizeof(double));    // Red
   means[1] = (double*)calloc(imlabel.Labels() + 1, sizeof(double));    // Green
   means[2] = (double*)calloc(imlabel.Labels() + 1, sizeof(double));    // Blue
   Ulong *counts = (Ulong*)calloc(imlabel.Labels() + 1, sizeof(Ulong)); // count

   // Calcul de la moyenne de chaque region
   for (p.z = 0; p.z < imlabel.Depth(); p.z++)
   for (p.y = 0; p.y < imlabel.Height(); p.y++)
   for (p.x = 0; p.x < imlabel.Width(); p.x++)
   {
      if ((r = imlabel[p])) {
	 means[0][r] += (double)impot.X[p];
	 means[1][r] += (double)impot.Y[p];
	 means[2][r] += (double)impot.Z[p];
	 counts[r]++;
      }
   }
   
   for (r = 1; r < (int)imlabel.Labels() + 1; r++) {
      means[0][r] /= (double)counts[r];
      means[1][r] /= (double)counts[r];
      means[2][r] /= (double)counts[r];
   }

   // Initialisation des frontieres marqueurs
   for (p.z = 0; p.z < imlabel.Depth(); p.z++)
   for (p.y = 0; p.y < imlabel.Height(); p.y++)
   for (p.x = 0; p.x < imlabel.Width(); p.x++)
   {
      if ((r = imd[p] = imlabel[p]) == 0) {
	 continue;
      }
      
      for (int v = 0; v < 26; v++) {
	 p1 = p + v26[v];
	 if (imlabel.Hold(p1) && imlabel[p1] == 0) { // Point frontiere
	    double key = (double)(SQR(means[0][r] - impot.X[p]) +
				  SQR(means[1][r] - impot.Y[p]) +
				  SQR(means[2][r] - impot.Z[p]));
	    tas.Push(point3d(p), key);
	    break;
	 }
      }
   }
   
   // Croissance prioritaire.
   Ulong it = 0;
   while (!tas.Empty()) {
      it++;
      p = (tas.Pop()).toPoint();
      r = imd[p];
      for (int v = 0; v < 26; v++) {
	 p1 = p + v26[v];
	 if (imd.Hold(p1) && imd[p1] == 0) {
	    // On empile ce nouveau point avec une clé secondaire
	    // correspondant à l'itération courante.
	    double key = (double)(SQR(means[0][r] - impot.X[p1]) +
				  SQR(means[1][r] - impot.Y[p1]) +
				  SQR(means[2][r] - impot.Z[p1]));
	    tas.Push(point3d(p1), key, it);
	    imd[p1] = r;
	 }
      }
   }
   imd.Labels(imlabel.Labels());
   free(means[0]);
   free(means[1]);
   free(means[2]);
   free(counts);
   
   return SUCCESS;
}


#ifdef MAIN
#define	USAGE	"usage: %s [-m mask] [im_in1|-] [im_in2|-] [im_out|-]"
#define	PARC	0
#define	FINC	2
#define	FOUTC	1
#define	MASK	1

int main( int argc, char *argv[] ) {
   Errc result;                // The result code of the execution.
   Pobject* mask;              // The region map.
   Pobject* objin[FINC + 1];   // The input objects.
   Pobject* objs[FINC + 1];    // The source objects masked.
   Pobject* objout[FOUTC + 1]; // The output object.
   Pobject* objd[FOUTC + 1];   // The result object of the execution.
   char* parv[PARC + 1];       // The input parameters.

   ReadArgs(argc, argv, PARC, FINC, FOUTC, &mask, objin, objs, objout, objd, parv, USAGE, MASK); 
   if ((objs[0]->Type() == Po_Reg2d ) && (objs[1]->Type() == Po_Img2duc )) {
      Reg2d* const rgs1 = (Reg2d*)objs[0];
      Img2duc* const ims2 = (Img2duc*)objs[1];
      objd[0] = new Reg2d(rgs1->Size());
      Reg2d* const rgd = (Reg2d*)objd[0];
      result = PWatershed(*rgs1, *ims2, *rgd);
      
      goto end;
   }
   if ((objs[0]->Type() == Po_Reg2d ) && (objs[1]->Type() == Po_Img2dsl )) {
      Reg2d* const rgs1 = (Reg2d*)objs[0];
      Img2dsl* const ims2 = (Img2dsl*)objs[1];
      objd[0] = new Reg2d(rgs1->Size());
      Reg2d* const rgd = (Reg2d*)objd[0];
      result = PWatershed(*rgs1, *ims2, *rgd);
      
      goto end;
   }
   if ((objs[0]->Type() == Po_Reg2d ) && (objs[1]->Type() == Po_Img2dsf )) {
      Reg2d* const rgs1 = (Reg2d*)objs[0];
      Img2dsf* const ims2 = (Img2dsf*)objs[1];
      objd[0] = new Reg2d(rgs1->Size());
      Reg2d* const rgd = (Reg2d*)objd[0];
      result = PWatershed(*rgs1, *ims2, *rgd);
      
      goto end;
   }
   if ((objs[0]->Type() == Po_Reg3d ) && (objs[1]->Type() == Po_Img3duc )) {
      Reg3d* const rgs1 = (Reg3d*)objs[0];
      Img3duc* const ims2 = (Img3duc*)objs[1];
      objd[0] = new Reg3d(rgs1->Size());
      Reg3d* const rgd = (Reg3d*)objd[0];
      result = PWatershed(*rgs1, *ims2, *rgd);
      
      goto end;
   }
   if ((objs[0]->Type() == Po_Reg3d ) && (objs[1]->Type() == Po_Img3dsl )) {
      Reg3d* const rgs1 = (Reg3d*)objs[0];
      Img3dsl* const ims2 = (Img3dsl*)objs[1];
      objd[0] = new Reg3d(rgs1->Size());
      Reg3d* const rgd = (Reg3d*)objd[0];
      result = PWatershed(*rgs1, *ims2, *rgd);
      
      goto end;
   }
   if ((objs[0]->Type() == Po_Reg3d ) && (objs[1]->Type() == Po_Img3dsf )) {
      Reg3d* const rgs1 = (Reg3d*)objs[0];
      Img3dsf* const ims2 = (Img3dsf*)objs[1];
      objd[0] = new Reg3d(rgs1->Size());
      Reg3d* const rgd = (Reg3d*)objd[0];
      result = PWatershed(*rgs1, *ims2, *rgd);
      
      goto end;
   }
   if ( (objs[0]->Type() == Po_Reg2d ) && (objs[1]->Type() == Po_Imc2duc ) ) {
      Reg2d* const rgs1 = (Reg2d*)objs[0];
      Imc2duc* const ims2 = (Imc2duc*)objs[1];
      objd[0] = new Reg2d(rgs1->Size());
      Reg2d* const rgd = (Reg2d*)objd[0];
      result = PWatershed(*rgs1, *ims2, *rgd);
      
      goto end;
   }
   if ( (objs[0]->Type() == Po_Reg2d ) && (objs[1]->Type() == Po_Imc2dsl ) ) {
      Reg2d* const rgs1 = (Reg2d*)objs[0];
      Imc2dsl* const ims2 = (Imc2dsl*)objs[1];
      objd[0] = new Reg2d(rgs1->Size());
      Reg2d* const rgd = (Reg2d*)objd[0];
      result = PWatershed(*rgs1, *ims2, *rgd);
      
      goto end;
   }
   if ( (objs[0]->Type() == Po_Reg2d ) && (objs[1]->Type() == Po_Imc2dsf ) ) {
      Reg2d* const rgs1 = (Reg2d*)objs[0];
      Imc2dsf* const ims2 = (Imc2dsf*)objs[1];
      objd[0] = new Reg2d(rgs1->Size());
      Reg2d* const rgd = (Reg2d*)objd[0];
      result = PWatershed(*rgs1, *ims2, *rgd);
      
      goto end;
   }
   if ( (objs[0]->Type() == Po_Reg3d ) && (objs[1]->Type() == Po_Imc3duc ) ) {
      Reg3d* const rgs1 = (Reg3d*)objs[0];
      Imc3duc* const ims2 = (Imc3duc*)objs[1];
      objd[0] = new Reg3d(rgs1->Size());
      Reg3d* const rgd = (Reg3d*)objd[0];
      result = PWatershed(*rgs1, *ims2, *rgd);
      
      goto end;
   }
   if ( (objs[0]->Type() == Po_Reg3d ) && (objs[1]->Type() == Po_Imc3dsl ) ) {
      Reg3d* const rgs1 = (Reg3d*)objs[0];
      Imc3dsl* const ims2 = (Imc3dsl*)objs[1];
      objd[0] = new Reg3d(rgs1->Size());
      Reg3d* const rgd = (Reg3d*)objd[0];
      result = PWatershed(*rgs1, *ims2, *rgd);
      
      goto end;
   }
   if ( (objs[0]->Type() == Po_Reg3d ) && (objs[1]->Type() == Po_Imc3dsf ) ) {
      Reg3d* const rgs1 = (Reg3d*)objs[0];
      Imc3dsf* const ims2 = (Imc3dsf*)objs[1];
      objd[0] = new Reg3d(rgs1->Size());
      Reg3d* const rgd = (Reg3d*)objd[0];
      result = PWatershed(*rgs1, *ims2, *rgd);
      
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
