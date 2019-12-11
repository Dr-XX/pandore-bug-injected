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
 * @author Abderrahim Elmoataz - 1999-02-19
 * @author Régis Clouard - 2002-05-24
 */

#include <pandore.h>
using namespace pandore;

/**
 * @file pdilationreconstruction.cpp
 * Reconstruction by dilation.
 * ims1 : markers image
 * ims2 : mask image
 * Need ims1 <= ims2.
 */




Errc PDilationReconstruction( const Img1duc &ims1, const Img1duc &ims2, Img1duc &imd, int connexity ) {
   Point1d p, q;
   int v;
   Img1duc::ValueType val, maxv;
   Fifo<Point1d> f;

   if (connexity != 2 && connexity != 4 && connexity != 8 && connexity != 6 && connexity != 26) {
      std::cerr << "Error pdilationreconstruction: bad connexity value:" << connexity << std::endl;
      return FAILURE;
   }
   if ( ims1.Size() != ims2.Size()) {
      std::cerr << "Error pdilationreconstruction: Incompatible image size." << std::endl;
      return FAILURE;
   }
   if ( ims1.Bands() != ims2.Bands() ) {
      std::cerr << "Error pdilationreconstruction: Incompatible number of bands." << std::endl;
      return FAILURE;
   }

   // Build the MIN(ims1, ims2)
   imd = ims1;
   for (p.x = 0; p.x < imd.Width(); p.x++)
   {
      maxv = imd[p];
      for (v = 0; v < connexity / 2; v++) {
	 if (imd.Hold(p + reinterpret_cast<const Point1d*>(vc[connexity])[v])) {
	    val = imd[p + reinterpret_cast<const Point1d*>(vc[connexity])[v]];
	    if (val > maxv) {
	       maxv = val;
	    }
	 }
      }
      imd[p] = MIN(maxv, ims2[p]);
   }
   // Backward scanning
   for (p.x = imd.Width() - 1; p.x >= 0; p.x--)
   {
      maxv = imd[p];
      for (v = connexity / 2; v < connexity; v++) {
	 if (imd.Hold(p + reinterpret_cast<const Point1d*>(vc[connexity])[v])) {
	    val = imd[p + reinterpret_cast<const Point1d*>(vc[connexity])[v]];
	    if (val > maxv) {
	       maxv = val;
	    }
	 }
      }
      imd[p] = MIN(maxv, ims2[p]);
      for (v = connexity / 2;  v < connexity; v++) {
	 q = p + reinterpret_cast<const Point1d*>(vc[connexity])[v];
	 if (imd.Hold(q) && (imd[q] < imd[p]) && (imd[q] < ims2[q])) { 
	    f.Push(p);
	    break;
	 }
      }
   }
   
   // Propagation
   while (!f.Empty()) {
      p = f.Pop();
      for (v = 0; v < connexity; v++) {
	 q = p + reinterpret_cast<const Point1d*>(vc[connexity])[v];
	 if (imd.Hold(q) && (imd[q] < imd[p]) && (ims2[q] != imd[q])) {
	    f.Push(q);
	    imd[q] = MIN(imd[p], ims2[q]);
	 }

      }
   }

   return SUCCESS;
}

Errc PDilationReconstruction( const Img1dsl &ims1, const Img1dsl &ims2, Img1dsl &imd, int connexity ) {
   Point1d p, q;
   int v;
   Img1dsl::ValueType val, maxv;
   Fifo<Point1d> f;

   if (connexity != 2 && connexity != 4 && connexity != 8 && connexity != 6 && connexity != 26) {
      std::cerr << "Error pdilationreconstruction: bad connexity value:" << connexity << std::endl;
      return FAILURE;
   }
   if ( ims1.Size() != ims2.Size()) {
      std::cerr << "Error pdilationreconstruction: Incompatible image size." << std::endl;
      return FAILURE;
   }
   if ( ims1.Bands() != ims2.Bands() ) {
      std::cerr << "Error pdilationreconstruction: Incompatible number of bands." << std::endl;
      return FAILURE;
   }

   // Build the MIN(ims1, ims2)
   imd = ims1;
   for (p.x = 0; p.x < imd.Width(); p.x++)
   {
      maxv = imd[p];
      for (v = 0; v < connexity / 2; v++) {
	 if (imd.Hold(p + reinterpret_cast<const Point1d*>(vc[connexity])[v])) {
	    val = imd[p + reinterpret_cast<const Point1d*>(vc[connexity])[v]];
	    if (val > maxv) {
	       maxv = val;
	    }
	 }
      }
      imd[p] = MIN(maxv, ims2[p]);
   }
   // Backward scanning
   for (p.x = imd.Width() - 1; p.x >= 0; p.x--)
   {
      maxv = imd[p];
      for (v = connexity / 2; v < connexity; v++) {
	 if (imd.Hold(p + reinterpret_cast<const Point1d*>(vc[connexity])[v])) {
	    val = imd[p + reinterpret_cast<const Point1d*>(vc[connexity])[v]];
	    if (val > maxv) {
	       maxv = val;
	    }
	 }
      }
      imd[p] = MIN(maxv, ims2[p]);
      for (v = connexity / 2;  v < connexity; v++) {
	 q = p + reinterpret_cast<const Point1d*>(vc[connexity])[v];
	 if (imd.Hold(q) && (imd[q] < imd[p]) && (imd[q] < ims2[q])) { 
	    f.Push(p);
	    break;
	 }
      }
   }
   
   // Propagation
   while (!f.Empty()) {
      p = f.Pop();
      for (v = 0; v < connexity; v++) {
	 q = p + reinterpret_cast<const Point1d*>(vc[connexity])[v];
	 if (imd.Hold(q) && (imd[q] < imd[p]) && (ims2[q] != imd[q])) {
	    f.Push(q);
	    imd[q] = MIN(imd[p], ims2[q]);
	 }

      }
   }

   return SUCCESS;
}

Errc PDilationReconstruction( const Img1dsf &ims1, const Img1dsf &ims2, Img1dsf &imd, int connexity ) {
   Point1d p, q;
   int v;
   Img1dsf::ValueType val, maxv;
   Fifo<Point1d> f;

   if (connexity != 2 && connexity != 4 && connexity != 8 && connexity != 6 && connexity != 26) {
      std::cerr << "Error pdilationreconstruction: bad connexity value:" << connexity << std::endl;
      return FAILURE;
   }
   if ( ims1.Size() != ims2.Size()) {
      std::cerr << "Error pdilationreconstruction: Incompatible image size." << std::endl;
      return FAILURE;
   }
   if ( ims1.Bands() != ims2.Bands() ) {
      std::cerr << "Error pdilationreconstruction: Incompatible number of bands." << std::endl;
      return FAILURE;
   }

   // Build the MIN(ims1, ims2)
   imd = ims1;
   for (p.x = 0; p.x < imd.Width(); p.x++)
   {
      maxv = imd[p];
      for (v = 0; v < connexity / 2; v++) {
	 if (imd.Hold(p + reinterpret_cast<const Point1d*>(vc[connexity])[v])) {
	    val = imd[p + reinterpret_cast<const Point1d*>(vc[connexity])[v]];
	    if (val > maxv) {
	       maxv = val;
	    }
	 }
      }
      imd[p] = MIN(maxv, ims2[p]);
   }
   // Backward scanning
   for (p.x = imd.Width() - 1; p.x >= 0; p.x--)
   {
      maxv = imd[p];
      for (v = connexity / 2; v < connexity; v++) {
	 if (imd.Hold(p + reinterpret_cast<const Point1d*>(vc[connexity])[v])) {
	    val = imd[p + reinterpret_cast<const Point1d*>(vc[connexity])[v]];
	    if (val > maxv) {
	       maxv = val;
	    }
	 }
      }
      imd[p] = MIN(maxv, ims2[p]);
      for (v = connexity / 2;  v < connexity; v++) {
	 q = p + reinterpret_cast<const Point1d*>(vc[connexity])[v];
	 if (imd.Hold(q) && (imd[q] < imd[p]) && (imd[q] < ims2[q])) { 
	    f.Push(p);
	    break;
	 }
      }
   }
   
   // Propagation
   while (!f.Empty()) {
      p = f.Pop();
      for (v = 0; v < connexity; v++) {
	 q = p + reinterpret_cast<const Point1d*>(vc[connexity])[v];
	 if (imd.Hold(q) && (imd[q] < imd[p]) && (ims2[q] != imd[q])) {
	    f.Push(q);
	    imd[q] = MIN(imd[p], ims2[q]);
	 }

      }
   }

   return SUCCESS;
}

Errc PDilationReconstruction( const Img2duc &ims1, const Img2duc &ims2, Img2duc &imd, int connexity ) {
   Point2d p, q;
   int v;
   Img2duc::ValueType val, maxv;
   Fifo<Point2d> f;

   if (connexity != 2 && connexity != 4 && connexity != 8 && connexity != 6 && connexity != 26) {
      std::cerr << "Error pdilationreconstruction: bad connexity value:" << connexity << std::endl;
      return FAILURE;
   }
   if ( ims1.Size() != ims2.Size()) {
      std::cerr << "Error pdilationreconstruction: Incompatible image size." << std::endl;
      return FAILURE;
   }
   if ( ims1.Bands() != ims2.Bands() ) {
      std::cerr << "Error pdilationreconstruction: Incompatible number of bands." << std::endl;
      return FAILURE;
   }

   // Build the MIN(ims1, ims2)
   imd = ims1;
   for (p.y = 0; p.y < imd.Height(); p.y++)
   for (p.x = 0; p.x < imd.Width(); p.x++)
   {
      maxv = imd[p];
      for (v = 0; v < connexity / 2; v++) {
	 if (imd.Hold(p + reinterpret_cast<const Point2d*>(vc[connexity])[v])) {
	    val = imd[p + reinterpret_cast<const Point2d*>(vc[connexity])[v]];
	    if (val > maxv) {
	       maxv = val;
	    }
	 }
      }
      imd[p] = MIN(maxv, ims2[p]);
   }
   // Backward scanning
   for (p.y = imd.Height() - 1; p.y >= 0; p.y--)
   for (p.x = imd.Width() - 1; p.x >= 0; p.x--)
   {
      maxv = imd[p];
      for (v = connexity / 2; v < connexity; v++) {
	 if (imd.Hold(p + reinterpret_cast<const Point2d*>(vc[connexity])[v])) {
	    val = imd[p + reinterpret_cast<const Point2d*>(vc[connexity])[v]];
	    if (val > maxv) {
	       maxv = val;
	    }
	 }
      }
      imd[p] = MIN(maxv, ims2[p]);
      for (v = connexity / 2;  v < connexity; v++) {
	 q = p + reinterpret_cast<const Point2d*>(vc[connexity])[v];
	 if (imd.Hold(q) && (imd[q] < imd[p]) && (imd[q] < ims2[q])) { 
	    f.Push(p);
	    break;
	 }
      }
   }
   
   // Propagation
   while (!f.Empty()) {
      p = f.Pop();
      for (v = 0; v < connexity; v++) {
	 q = p + reinterpret_cast<const Point2d*>(vc[connexity])[v];
	 if (imd.Hold(q) && (imd[q] < imd[p]) && (ims2[q] != imd[q])) {
	    f.Push(q);
	    imd[q] = MIN(imd[p], ims2[q]);
	 }

      }
   }

   return SUCCESS;
}

Errc PDilationReconstruction( const Img2dsl &ims1, const Img2dsl &ims2, Img2dsl &imd, int connexity ) {
   Point2d p, q;
   int v;
   Img2dsl::ValueType val, maxv;
   Fifo<Point2d> f;

   if (connexity != 2 && connexity != 4 && connexity != 8 && connexity != 6 && connexity != 26) {
      std::cerr << "Error pdilationreconstruction: bad connexity value:" << connexity << std::endl;
      return FAILURE;
   }
   if ( ims1.Size() != ims2.Size()) {
      std::cerr << "Error pdilationreconstruction: Incompatible image size." << std::endl;
      return FAILURE;
   }
   if ( ims1.Bands() != ims2.Bands() ) {
      std::cerr << "Error pdilationreconstruction: Incompatible number of bands." << std::endl;
      return FAILURE;
   }

   // Build the MIN(ims1, ims2)
   imd = ims1;
   for (p.y = 0; p.y < imd.Height(); p.y++)
   for (p.x = 0; p.x < imd.Width(); p.x++)
   {
      maxv = imd[p];
      for (v = 0; v < connexity / 2; v++) {
	 if (imd.Hold(p + reinterpret_cast<const Point2d*>(vc[connexity])[v])) {
	    val = imd[p + reinterpret_cast<const Point2d*>(vc[connexity])[v]];
	    if (val > maxv) {
	       maxv = val;
	    }
	 }
      }
      imd[p] = MIN(maxv, ims2[p]);
   }
   // Backward scanning
   for (p.y = imd.Height() - 1; p.y >= 0; p.y--)
   for (p.x = imd.Width() - 1; p.x >= 0; p.x--)
   {
      maxv = imd[p];
      for (v = connexity / 2; v < connexity; v++) {
	 if (imd.Hold(p + reinterpret_cast<const Point2d*>(vc[connexity])[v])) {
	    val = imd[p + reinterpret_cast<const Point2d*>(vc[connexity])[v]];
	    if (val > maxv) {
	       maxv = val;
	    }
	 }
      }
      imd[p] = MIN(maxv, ims2[p]);
      for (v = connexity / 2;  v < connexity; v++) {
	 q = p + reinterpret_cast<const Point2d*>(vc[connexity])[v];
	 if (imd.Hold(q) && (imd[q] < imd[p]) && (imd[q] < ims2[q])) { 
	    f.Push(p);
	    break;
	 }
      }
   }
   
   // Propagation
   while (!f.Empty()) {
      p = f.Pop();
      for (v = 0; v < connexity; v++) {
	 q = p + reinterpret_cast<const Point2d*>(vc[connexity])[v];
	 if (imd.Hold(q) && (imd[q] < imd[p]) && (ims2[q] != imd[q])) {
	    f.Push(q);
	    imd[q] = MIN(imd[p], ims2[q]);
	 }

      }
   }

   return SUCCESS;
}

Errc PDilationReconstruction( const Img2dsf &ims1, const Img2dsf &ims2, Img2dsf &imd, int connexity ) {
   Point2d p, q;
   int v;
   Img2dsf::ValueType val, maxv;
   Fifo<Point2d> f;

   if (connexity != 2 && connexity != 4 && connexity != 8 && connexity != 6 && connexity != 26) {
      std::cerr << "Error pdilationreconstruction: bad connexity value:" << connexity << std::endl;
      return FAILURE;
   }
   if ( ims1.Size() != ims2.Size()) {
      std::cerr << "Error pdilationreconstruction: Incompatible image size." << std::endl;
      return FAILURE;
   }
   if ( ims1.Bands() != ims2.Bands() ) {
      std::cerr << "Error pdilationreconstruction: Incompatible number of bands." << std::endl;
      return FAILURE;
   }

   // Build the MIN(ims1, ims2)
   imd = ims1;
   for (p.y = 0; p.y < imd.Height(); p.y++)
   for (p.x = 0; p.x < imd.Width(); p.x++)
   {
      maxv = imd[p];
      for (v = 0; v < connexity / 2; v++) {
	 if (imd.Hold(p + reinterpret_cast<const Point2d*>(vc[connexity])[v])) {
	    val = imd[p + reinterpret_cast<const Point2d*>(vc[connexity])[v]];
	    if (val > maxv) {
	       maxv = val;
	    }
	 }
      }
      imd[p] = MIN(maxv, ims2[p]);
   }
   // Backward scanning
   for (p.y = imd.Height() - 1; p.y >= 0; p.y--)
   for (p.x = imd.Width() - 1; p.x >= 0; p.x--)
   {
      maxv = imd[p];
      for (v = connexity / 2; v < connexity; v++) {
	 if (imd.Hold(p + reinterpret_cast<const Point2d*>(vc[connexity])[v])) {
	    val = imd[p + reinterpret_cast<const Point2d*>(vc[connexity])[v]];
	    if (val > maxv) {
	       maxv = val;
	    }
	 }
      }
      imd[p] = MIN(maxv, ims2[p]);
      for (v = connexity / 2;  v < connexity; v++) {
	 q = p + reinterpret_cast<const Point2d*>(vc[connexity])[v];
	 if (imd.Hold(q) && (imd[q] < imd[p]) && (imd[q] < ims2[q])) { 
	    f.Push(p);
	    break;
	 }
      }
   }
   
   // Propagation
   while (!f.Empty()) {
      p = f.Pop();
      for (v = 0; v < connexity; v++) {
	 q = p + reinterpret_cast<const Point2d*>(vc[connexity])[v];
	 if (imd.Hold(q) && (imd[q] < imd[p]) && (ims2[q] != imd[q])) {
	    f.Push(q);
	    imd[q] = MIN(imd[p], ims2[q]);
	 }

      }
   }

   return SUCCESS;
}

Errc PDilationReconstruction( const Img3duc &ims1, const Img3duc &ims2, Img3duc &imd, int connexity ) {
   Point3d p, q;
   int v;
   Img3duc::ValueType val, maxv;
   Fifo<Point3d> f;

   if (connexity != 2 && connexity != 4 && connexity != 8 && connexity != 6 && connexity != 26) {
      std::cerr << "Error pdilationreconstruction: bad connexity value:" << connexity << std::endl;
      return FAILURE;
   }
   if ( ims1.Size() != ims2.Size()) {
      std::cerr << "Error pdilationreconstruction: Incompatible image size." << std::endl;
      return FAILURE;
   }
   if ( ims1.Bands() != ims2.Bands() ) {
      std::cerr << "Error pdilationreconstruction: Incompatible number of bands." << std::endl;
      return FAILURE;
   }

   // Build the MIN(ims1, ims2)
   imd = ims1;
   for (p.z = 0; p.z < imd.Depth(); p.z++)
   for (p.y = 0; p.y < imd.Height(); p.y++)
   for (p.x = 0; p.x < imd.Width(); p.x++)
   {
      maxv = imd[p];
      for (v = 0; v < connexity / 2; v++) {
	 if (imd.Hold(p + reinterpret_cast<const Point3d*>(vc[connexity])[v])) {
	    val = imd[p + reinterpret_cast<const Point3d*>(vc[connexity])[v]];
	    if (val > maxv) {
	       maxv = val;
	    }
	 }
      }
      imd[p] = MIN(maxv, ims2[p]);
   }
   // Backward scanning
   for (p.z = imd.Depth() - 1; p.z >= 0; p.z--)
   for (p.y = imd.Height() - 1; p.y >= 0; p.y--)
   for (p.x = imd.Width() - 1; p.x >= 0; p.x--)
   {
      maxv = imd[p];
      for (v = connexity / 2; v < connexity; v++) {
	 if (imd.Hold(p + reinterpret_cast<const Point3d*>(vc[connexity])[v])) {
	    val = imd[p + reinterpret_cast<const Point3d*>(vc[connexity])[v]];
	    if (val > maxv) {
	       maxv = val;
	    }
	 }
      }
      imd[p] = MIN(maxv, ims2[p]);
      for (v = connexity / 2;  v < connexity; v++) {
	 q = p + reinterpret_cast<const Point3d*>(vc[connexity])[v];
	 if (imd.Hold(q) && (imd[q] < imd[p]) && (imd[q] < ims2[q])) { 
	    f.Push(p);
	    break;
	 }
      }
   }
   
   // Propagation
   while (!f.Empty()) {
      p = f.Pop();
      for (v = 0; v < connexity; v++) {
	 q = p + reinterpret_cast<const Point3d*>(vc[connexity])[v];
	 if (imd.Hold(q) && (imd[q] < imd[p]) && (ims2[q] != imd[q])) {
	    f.Push(q);
	    imd[q] = MIN(imd[p], ims2[q]);
	 }

      }
   }

   return SUCCESS;
}

Errc PDilationReconstruction( const Img3dsl &ims1, const Img3dsl &ims2, Img3dsl &imd, int connexity ) {
   Point3d p, q;
   int v;
   Img3dsl::ValueType val, maxv;
   Fifo<Point3d> f;

   if (connexity != 2 && connexity != 4 && connexity != 8 && connexity != 6 && connexity != 26) {
      std::cerr << "Error pdilationreconstruction: bad connexity value:" << connexity << std::endl;
      return FAILURE;
   }
   if ( ims1.Size() != ims2.Size()) {
      std::cerr << "Error pdilationreconstruction: Incompatible image size." << std::endl;
      return FAILURE;
   }
   if ( ims1.Bands() != ims2.Bands() ) {
      std::cerr << "Error pdilationreconstruction: Incompatible number of bands." << std::endl;
      return FAILURE;
   }

   // Build the MIN(ims1, ims2)
   imd = ims1;
   for (p.z = 0; p.z < imd.Depth(); p.z++)
   for (p.y = 0; p.y < imd.Height(); p.y++)
   for (p.x = 0; p.x < imd.Width(); p.x++)
   {
      maxv = imd[p];
      for (v = 0; v < connexity / 2; v++) {
	 if (imd.Hold(p + reinterpret_cast<const Point3d*>(vc[connexity])[v])) {
	    val = imd[p + reinterpret_cast<const Point3d*>(vc[connexity])[v]];
	    if (val > maxv) {
	       maxv = val;
	    }
	 }
      }
      imd[p] = MIN(maxv, ims2[p]);
   }
   // Backward scanning
   for (p.z = imd.Depth() - 1; p.z >= 0; p.z--)
   for (p.y = imd.Height() - 1; p.y >= 0; p.y--)
   for (p.x = imd.Width() - 1; p.x >= 0; p.x--)
   {
      maxv = imd[p];
      for (v = connexity / 2; v < connexity; v++) {
	 if (imd.Hold(p + reinterpret_cast<const Point3d*>(vc[connexity])[v])) {
	    val = imd[p + reinterpret_cast<const Point3d*>(vc[connexity])[v]];
	    if (val > maxv) {
	       maxv = val;
	    }
	 }
      }
      imd[p] = MIN(maxv, ims2[p]);
      for (v = connexity / 2;  v < connexity; v++) {
	 q = p + reinterpret_cast<const Point3d*>(vc[connexity])[v];
	 if (imd.Hold(q) && (imd[q] < imd[p]) && (imd[q] < ims2[q])) { 
	    f.Push(p);
	    break;
	 }
      }
   }
   
   // Propagation
   while (!f.Empty()) {
      p = f.Pop();
      for (v = 0; v < connexity; v++) {
	 q = p + reinterpret_cast<const Point3d*>(vc[connexity])[v];
	 if (imd.Hold(q) && (imd[q] < imd[p]) && (ims2[q] != imd[q])) {
	    f.Push(q);
	    imd[q] = MIN(imd[p], ims2[q]);
	 }

      }
   }

   return SUCCESS;
}

Errc PDilationReconstruction( const Img3dsf &ims1, const Img3dsf &ims2, Img3dsf &imd, int connexity ) {
   Point3d p, q;
   int v;
   Img3dsf::ValueType val, maxv;
   Fifo<Point3d> f;

   if (connexity != 2 && connexity != 4 && connexity != 8 && connexity != 6 && connexity != 26) {
      std::cerr << "Error pdilationreconstruction: bad connexity value:" << connexity << std::endl;
      return FAILURE;
   }
   if ( ims1.Size() != ims2.Size()) {
      std::cerr << "Error pdilationreconstruction: Incompatible image size." << std::endl;
      return FAILURE;
   }
   if ( ims1.Bands() != ims2.Bands() ) {
      std::cerr << "Error pdilationreconstruction: Incompatible number of bands." << std::endl;
      return FAILURE;
   }

   // Build the MIN(ims1, ims2)
   imd = ims1;
   for (p.z = 0; p.z < imd.Depth(); p.z++)
   for (p.y = 0; p.y < imd.Height(); p.y++)
   for (p.x = 0; p.x < imd.Width(); p.x++)
   {
      maxv = imd[p];
      for (v = 0; v < connexity / 2; v++) {
	 if (imd.Hold(p + reinterpret_cast<const Point3d*>(vc[connexity])[v])) {
	    val = imd[p + reinterpret_cast<const Point3d*>(vc[connexity])[v]];
	    if (val > maxv) {
	       maxv = val;
	    }
	 }
      }
      imd[p] = MIN(maxv, ims2[p]);
   }
   // Backward scanning
   for (p.z = imd.Depth() - 1; p.z >= 0; p.z--)
   for (p.y = imd.Height() - 1; p.y >= 0; p.y--)
   for (p.x = imd.Width() - 1; p.x >= 0; p.x--)
   {
      maxv = imd[p];
      for (v = connexity / 2; v < connexity; v++) {
	 if (imd.Hold(p + reinterpret_cast<const Point3d*>(vc[connexity])[v])) {
	    val = imd[p + reinterpret_cast<const Point3d*>(vc[connexity])[v]];
	    if (val > maxv) {
	       maxv = val;
	    }
	 }
      }
      imd[p] = MIN(maxv, ims2[p]);
      for (v = connexity / 2;  v < connexity; v++) {
	 q = p + reinterpret_cast<const Point3d*>(vc[connexity])[v];
	 if (imd.Hold(q) && (imd[q] < imd[p]) && (imd[q] < ims2[q])) { 
	    f.Push(p);
	    break;
	 }
      }
   }
   
   // Propagation
   while (!f.Empty()) {
      p = f.Pop();
      for (v = 0; v < connexity; v++) {
	 q = p + reinterpret_cast<const Point3d*>(vc[connexity])[v];
	 if (imd.Hold(q) && (imd[q] < imd[p]) && (ims2[q] != imd[q])) {
	    f.Push(q);
	    imd[q] = MIN(imd[p], ims2[q]);
	 }

      }
   }

   return SUCCESS;
}

Errc PDilationReconstruction( const Imc2duc &ims1, const Imc2duc &ims2, Imc2duc &imd, int connexity ) {
   Point2d p; 
   int v;
   Imc2duc::ValueType val, maxv;
   Fifo<Point2d> fX, fY, fZ;

   if (connexity != 4 && connexity != 8 && connexity != 6 && connexity != 26) {
      std::cerr << "Error pdilationreconstruction: bad connexity value:" << connexity << std::endl;
      return FAILURE;
   }
   if ( ims1.Size() != ims2.Size()) {
      std::cerr << "Error pdilationreconstruction: Incompatible image size." << std::endl;
      return FAILURE;
   }
   if ( ims1.Bands() != ims2.Bands() ) {
      std::cerr << "Error pdilationreconstruction: Incompatible number of bands." << std::endl;
      return FAILURE;
   }
   
   imd = ims1;
   
   // Forward scanning
   for (p.y = 0; p.y < imd.Height(); p.y++)
   for (p.x = 0; p.x < imd.Width(); p.x++)
   {
      maxv = imd.X[p];
      for (v = 0; v < connexity / 2; v++) {
	 if (imd.Hold(p + reinterpret_cast<const Point2d*>(vc[connexity])[v])) {
	    val = imd.X[p + reinterpret_cast<const Point2d*>(vc[connexity])[v]];
	    if (val > maxv) {
	       maxv = val;
	    }
	 }
      }
      imd.X[p] = MIN(maxv, ims2.X[p]);

      maxv = imd.Y[p];
      for (v = 0; v < connexity / 2; v++) {
	 if (imd.Hold(p + reinterpret_cast<const Point2d*>(vc[connexity])[v])) {
	    val = imd.Y[p + reinterpret_cast<const Point2d*>(vc[connexity])[v]];
	    if (val > maxv) {
	       maxv = val;
	    }
	 }
      }
      imd.Y[p] = MIN(maxv, ims2.Y[p]);

      maxv = imd.Z[p];
      for (v = 0; v < connexity / 2; v++) {
	 if (imd.Hold(p + reinterpret_cast<const Point2d*>(vc[connexity])[v])) {
	    val = imd.Z[p + reinterpret_cast<const Point2d*>(vc[connexity])[v]];
	    if (val > maxv) {
	       maxv = val;
	    }
	 }
      }
      imd.Z[p] = MIN(maxv, ims2.Z[p]);
   }
   
   // Backward scanning
   for (p.y = imd.Height() - 1; p.y >= 0; p.y--)
   for (p.x = imd.Width() - 1; p.x >= 0; p.x--)
   {
      maxv = imd.X[p];
      for (v = connexity / 2; v < connexity; v++) {
	 if (imd.Hold(p + reinterpret_cast<const Point2d*>(vc[connexity])[v])) {
	    val = imd.X[p + reinterpret_cast<const Point2d*>(vc[connexity])[v]];
	    if (val > maxv) {
	       maxv = val;
	    }
	 }
      }
      imd.X[p] = MIN(maxv, ims2.X[p]);
      for (v = connexity / 2; v < connexity; v++) {
	 if (imd.Hold(p + reinterpret_cast<const Point2d*>(vc[connexity])[v])
	     && (imd.X[p + reinterpret_cast<const Point2d*>(vc[connexity])[v]] < imd.X[p])
	     && (imd.X[p + reinterpret_cast<const Point2d*>(vc[connexity])[v]] < ims2.X[p + reinterpret_cast<const Point2d*>(vc[connexity])[v]])) { 
	    fX.Push(p);
	    break;
	 }
      }
 
      maxv = imd.Y[p];
      for (v = connexity / 2; v < connexity; v++) {
	 if (imd.Hold(p + reinterpret_cast<const Point2d*>(vc[connexity])[v])) {
	    val = imd.Y[p + reinterpret_cast<const Point2d*>(vc[connexity])[v]];
	    if (val > maxv) {
	       maxv = val;
	    }
	 }
      }
      imd.Y[p] = MIN(maxv, ims2.Y[p]);
      for (v = connexity / 2; v < connexity; v++) {
	 if (imd.Hold(p + reinterpret_cast<const Point2d*>(vc[connexity])[v])
	     && (imd.Y[p + reinterpret_cast<const Point2d*>(vc[connexity])[v]] < imd.Y[p])
	     && (imd.Y[p + reinterpret_cast<const Point2d*>(vc[connexity])[v]] < ims2.Y[p + reinterpret_cast<const Point2d*>(vc[connexity])[v]])) {  
	    fY.Push(p);
	    break;
	 }
      }
      maxv = imd.Z[p];
      for (v = connexity / 2; v < connexity; v++) {
	 if (imd.Hold(p + reinterpret_cast<const Point2d*>(vc[connexity])[v])) { 
	    val = imd.Z[p + reinterpret_cast<const Point2d*>(vc[connexity])[v]];
	    if (val > maxv) {
	       maxv = val;
	    }
	 }
      }
      imd.Z[p] = MIN(maxv, ims2.Z[p]);
      for (v = connexity / 2; v < connexity; v++) {
	 if (imd.Hold(p + reinterpret_cast<const Point2d*>(vc[connexity])[v])
	     && (imd.Z[p + reinterpret_cast<const Point2d*>(vc[connexity])[v]] < imd.Z[p])
	     && (imd.Z[p + reinterpret_cast<const Point2d*>(vc[connexity])[v]] < ims2.Z[p+reinterpret_cast<const Point2d*>(vc[connexity])[v]])) { 
	    fZ.Push(p);
	    break;
	 }
      }
   }
   
   // Propagation
   Point2d q;
   while (!fX.Empty()) {
      p = fX.Pop();
      for (v = 0; v < connexity; v++) {
	 q = p + reinterpret_cast<const Point2d*>(vc[connexity])[v];
	 if (imd.Hold(q) && (imd.X[q] < imd.X[p]) && (ims2.X[q] != imd.X[q])) {
	    fX.Push(q);
	    imd.X[q] = MIN(imd.X[p], ims2.X[q]);
	 }
      }
   }
   

   while (!fY.Empty()) {
      p = fY.Pop();
      for (v = 0; v < connexity; v++) {
	 q = p + reinterpret_cast<const Point2d*>(vc[connexity])[v];
	 if (imd.Hold(q) && (imd.Y[q] < imd.Y[p]) && (ims2.Y[q] != imd.Y[q])) {
	    fY.Push(q);
	    imd.Y[q] = MIN(imd.Y[p], ims2.Y[q]);
	 }

      }
   }
   
   while (!fZ.Empty()) {
      p = fZ.Pop();
      for (v = 0; v < connexity; v++) {
	 q = p + reinterpret_cast<const Point2d*>(vc[connexity])[v];
	 if (imd.Hold(q) && (imd.Z[q] < imd.Z[p]) && (ims2.Z[q] != imd.Z[q])) {
	    fZ.Push(q);
	    imd.Z[q]  =MIN(imd.Z[p], ims2.Z[q]);
	 }
      }
   }
   
   return SUCCESS;
}

Errc PDilationReconstruction( const Imc2dsl &ims1, const Imc2dsl &ims2, Imc2dsl &imd, int connexity ) {
   Point2d p; 
   int v;
   Imc2dsl::ValueType val, maxv;
   Fifo<Point2d> fX, fY, fZ;

   if (connexity != 4 && connexity != 8 && connexity != 6 && connexity != 26) {
      std::cerr << "Error pdilationreconstruction: bad connexity value:" << connexity << std::endl;
      return FAILURE;
   }
   if ( ims1.Size() != ims2.Size()) {
      std::cerr << "Error pdilationreconstruction: Incompatible image size." << std::endl;
      return FAILURE;
   }
   if ( ims1.Bands() != ims2.Bands() ) {
      std::cerr << "Error pdilationreconstruction: Incompatible number of bands." << std::endl;
      return FAILURE;
   }
   
   imd = ims1;
   
   // Forward scanning
   for (p.y = 0; p.y < imd.Height(); p.y++)
   for (p.x = 0; p.x < imd.Width(); p.x++)
   {
      maxv = imd.X[p];
      for (v = 0; v < connexity / 2; v++) {
	 if (imd.Hold(p + reinterpret_cast<const Point2d*>(vc[connexity])[v])) {
	    val = imd.X[p + reinterpret_cast<const Point2d*>(vc[connexity])[v]];
	    if (val > maxv) {
	       maxv = val;
	    }
	 }
      }
      imd.X[p] = MIN(maxv, ims2.X[p]);

      maxv = imd.Y[p];
      for (v = 0; v < connexity / 2; v++) {
	 if (imd.Hold(p + reinterpret_cast<const Point2d*>(vc[connexity])[v])) {
	    val = imd.Y[p + reinterpret_cast<const Point2d*>(vc[connexity])[v]];
	    if (val > maxv) {
	       maxv = val;
	    }
	 }
      }
      imd.Y[p] = MIN(maxv, ims2.Y[p]);

      maxv = imd.Z[p];
      for (v = 0; v < connexity / 2; v++) {
	 if (imd.Hold(p + reinterpret_cast<const Point2d*>(vc[connexity])[v])) {
	    val = imd.Z[p + reinterpret_cast<const Point2d*>(vc[connexity])[v]];
	    if (val > maxv) {
	       maxv = val;
	    }
	 }
      }
      imd.Z[p] = MIN(maxv, ims2.Z[p]);
   }
   
   // Backward scanning
   for (p.y = imd.Height() - 1; p.y >= 0; p.y--)
   for (p.x = imd.Width() - 1; p.x >= 0; p.x--)
   {
      maxv = imd.X[p];
      for (v = connexity / 2; v < connexity; v++) {
	 if (imd.Hold(p + reinterpret_cast<const Point2d*>(vc[connexity])[v])) {
	    val = imd.X[p + reinterpret_cast<const Point2d*>(vc[connexity])[v]];
	    if (val > maxv) {
	       maxv = val;
	    }
	 }
      }
      imd.X[p] = MIN(maxv, ims2.X[p]);
      for (v = connexity / 2; v < connexity; v++) {
	 if (imd.Hold(p + reinterpret_cast<const Point2d*>(vc[connexity])[v])
	     && (imd.X[p + reinterpret_cast<const Point2d*>(vc[connexity])[v]] < imd.X[p])
	     && (imd.X[p + reinterpret_cast<const Point2d*>(vc[connexity])[v]] < ims2.X[p + reinterpret_cast<const Point2d*>(vc[connexity])[v]])) { 
	    fX.Push(p);
	    break;
	 }
      }
 
      maxv = imd.Y[p];
      for (v = connexity / 2; v < connexity; v++) {
	 if (imd.Hold(p + reinterpret_cast<const Point2d*>(vc[connexity])[v])) {
	    val = imd.Y[p + reinterpret_cast<const Point2d*>(vc[connexity])[v]];
	    if (val > maxv) {
	       maxv = val;
	    }
	 }
      }
      imd.Y[p] = MIN(maxv, ims2.Y[p]);
      for (v = connexity / 2; v < connexity; v++) {
	 if (imd.Hold(p + reinterpret_cast<const Point2d*>(vc[connexity])[v])
	     && (imd.Y[p + reinterpret_cast<const Point2d*>(vc[connexity])[v]] < imd.Y[p])
	     && (imd.Y[p + reinterpret_cast<const Point2d*>(vc[connexity])[v]] < ims2.Y[p + reinterpret_cast<const Point2d*>(vc[connexity])[v]])) {  
	    fY.Push(p);
	    break;
	 }
      }
      maxv = imd.Z[p];
      for (v = connexity / 2; v < connexity; v++) {
	 if (imd.Hold(p + reinterpret_cast<const Point2d*>(vc[connexity])[v])) { 
	    val = imd.Z[p + reinterpret_cast<const Point2d*>(vc[connexity])[v]];
	    if (val > maxv) {
	       maxv = val;
	    }
	 }
      }
      imd.Z[p] = MIN(maxv, ims2.Z[p]);
      for (v = connexity / 2; v < connexity; v++) {
	 if (imd.Hold(p + reinterpret_cast<const Point2d*>(vc[connexity])[v])
	     && (imd.Z[p + reinterpret_cast<const Point2d*>(vc[connexity])[v]] < imd.Z[p])
	     && (imd.Z[p + reinterpret_cast<const Point2d*>(vc[connexity])[v]] < ims2.Z[p+reinterpret_cast<const Point2d*>(vc[connexity])[v]])) { 
	    fZ.Push(p);
	    break;
	 }
      }
   }
   
   // Propagation
   Point2d q;
   while (!fX.Empty()) {
      p = fX.Pop();
      for (v = 0; v < connexity; v++) {
	 q = p + reinterpret_cast<const Point2d*>(vc[connexity])[v];
	 if (imd.Hold(q) && (imd.X[q] < imd.X[p]) && (ims2.X[q] != imd.X[q])) {
	    fX.Push(q);
	    imd.X[q] = MIN(imd.X[p], ims2.X[q]);
	 }
      }
   }
   

   while (!fY.Empty()) {
      p = fY.Pop();
      for (v = 0; v < connexity; v++) {
	 q = p + reinterpret_cast<const Point2d*>(vc[connexity])[v];
	 if (imd.Hold(q) && (imd.Y[q] < imd.Y[p]) && (ims2.Y[q] != imd.Y[q])) {
	    fY.Push(q);
	    imd.Y[q] = MIN(imd.Y[p], ims2.Y[q]);
	 }

      }
   }
   
   while (!fZ.Empty()) {
      p = fZ.Pop();
      for (v = 0; v < connexity; v++) {
	 q = p + reinterpret_cast<const Point2d*>(vc[connexity])[v];
	 if (imd.Hold(q) && (imd.Z[q] < imd.Z[p]) && (ims2.Z[q] != imd.Z[q])) {
	    fZ.Push(q);
	    imd.Z[q]  =MIN(imd.Z[p], ims2.Z[q]);
	 }
      }
   }
   
   return SUCCESS;
}

Errc PDilationReconstruction( const Imc2dsf &ims1, const Imc2dsf &ims2, Imc2dsf &imd, int connexity ) {
   Point2d p; 
   int v;
   Imc2dsf::ValueType val, maxv;
   Fifo<Point2d> fX, fY, fZ;

   if (connexity != 4 && connexity != 8 && connexity != 6 && connexity != 26) {
      std::cerr << "Error pdilationreconstruction: bad connexity value:" << connexity << std::endl;
      return FAILURE;
   }
   if ( ims1.Size() != ims2.Size()) {
      std::cerr << "Error pdilationreconstruction: Incompatible image size." << std::endl;
      return FAILURE;
   }
   if ( ims1.Bands() != ims2.Bands() ) {
      std::cerr << "Error pdilationreconstruction: Incompatible number of bands." << std::endl;
      return FAILURE;
   }
   
   imd = ims1;
   
   // Forward scanning
   for (p.y = 0; p.y < imd.Height(); p.y++)
   for (p.x = 0; p.x < imd.Width(); p.x++)
   {
      maxv = imd.X[p];
      for (v = 0; v < connexity / 2; v++) {
	 if (imd.Hold(p + reinterpret_cast<const Point2d*>(vc[connexity])[v])) {
	    val = imd.X[p + reinterpret_cast<const Point2d*>(vc[connexity])[v]];
	    if (val > maxv) {
	       maxv = val;
	    }
	 }
      }
      imd.X[p] = MIN(maxv, ims2.X[p]);

      maxv = imd.Y[p];
      for (v = 0; v < connexity / 2; v++) {
	 if (imd.Hold(p + reinterpret_cast<const Point2d*>(vc[connexity])[v])) {
	    val = imd.Y[p + reinterpret_cast<const Point2d*>(vc[connexity])[v]];
	    if (val > maxv) {
	       maxv = val;
	    }
	 }
      }
      imd.Y[p] = MIN(maxv, ims2.Y[p]);

      maxv = imd.Z[p];
      for (v = 0; v < connexity / 2; v++) {
	 if (imd.Hold(p + reinterpret_cast<const Point2d*>(vc[connexity])[v])) {
	    val = imd.Z[p + reinterpret_cast<const Point2d*>(vc[connexity])[v]];
	    if (val > maxv) {
	       maxv = val;
	    }
	 }
      }
      imd.Z[p] = MIN(maxv, ims2.Z[p]);
   }
   
   // Backward scanning
   for (p.y = imd.Height() - 1; p.y >= 0; p.y--)
   for (p.x = imd.Width() - 1; p.x >= 0; p.x--)
   {
      maxv = imd.X[p];
      for (v = connexity / 2; v < connexity; v++) {
	 if (imd.Hold(p + reinterpret_cast<const Point2d*>(vc[connexity])[v])) {
	    val = imd.X[p + reinterpret_cast<const Point2d*>(vc[connexity])[v]];
	    if (val > maxv) {
	       maxv = val;
	    }
	 }
      }
      imd.X[p] = MIN(maxv, ims2.X[p]);
      for (v = connexity / 2; v < connexity; v++) {
	 if (imd.Hold(p + reinterpret_cast<const Point2d*>(vc[connexity])[v])
	     && (imd.X[p + reinterpret_cast<const Point2d*>(vc[connexity])[v]] < imd.X[p])
	     && (imd.X[p + reinterpret_cast<const Point2d*>(vc[connexity])[v]] < ims2.X[p + reinterpret_cast<const Point2d*>(vc[connexity])[v]])) { 
	    fX.Push(p);
	    break;
	 }
      }
 
      maxv = imd.Y[p];
      for (v = connexity / 2; v < connexity; v++) {
	 if (imd.Hold(p + reinterpret_cast<const Point2d*>(vc[connexity])[v])) {
	    val = imd.Y[p + reinterpret_cast<const Point2d*>(vc[connexity])[v]];
	    if (val > maxv) {
	       maxv = val;
	    }
	 }
      }
      imd.Y[p] = MIN(maxv, ims2.Y[p]);
      for (v = connexity / 2; v < connexity; v++) {
	 if (imd.Hold(p + reinterpret_cast<const Point2d*>(vc[connexity])[v])
	     && (imd.Y[p + reinterpret_cast<const Point2d*>(vc[connexity])[v]] < imd.Y[p])
	     && (imd.Y[p + reinterpret_cast<const Point2d*>(vc[connexity])[v]] < ims2.Y[p + reinterpret_cast<const Point2d*>(vc[connexity])[v]])) {  
	    fY.Push(p);
	    break;
	 }
      }
      maxv = imd.Z[p];
      for (v = connexity / 2; v < connexity; v++) {
	 if (imd.Hold(p + reinterpret_cast<const Point2d*>(vc[connexity])[v])) { 
	    val = imd.Z[p + reinterpret_cast<const Point2d*>(vc[connexity])[v]];
	    if (val > maxv) {
	       maxv = val;
	    }
	 }
      }
      imd.Z[p] = MIN(maxv, ims2.Z[p]);
      for (v = connexity / 2; v < connexity; v++) {
	 if (imd.Hold(p + reinterpret_cast<const Point2d*>(vc[connexity])[v])
	     && (imd.Z[p + reinterpret_cast<const Point2d*>(vc[connexity])[v]] < imd.Z[p])
	     && (imd.Z[p + reinterpret_cast<const Point2d*>(vc[connexity])[v]] < ims2.Z[p+reinterpret_cast<const Point2d*>(vc[connexity])[v]])) { 
	    fZ.Push(p);
	    break;
	 }
      }
   }
   
   // Propagation
   Point2d q;
   while (!fX.Empty()) {
      p = fX.Pop();
      for (v = 0; v < connexity; v++) {
	 q = p + reinterpret_cast<const Point2d*>(vc[connexity])[v];
	 if (imd.Hold(q) && (imd.X[q] < imd.X[p]) && (ims2.X[q] != imd.X[q])) {
	    fX.Push(q);
	    imd.X[q] = MIN(imd.X[p], ims2.X[q]);
	 }
      }
   }
   

   while (!fY.Empty()) {
      p = fY.Pop();
      for (v = 0; v < connexity; v++) {
	 q = p + reinterpret_cast<const Point2d*>(vc[connexity])[v];
	 if (imd.Hold(q) && (imd.Y[q] < imd.Y[p]) && (ims2.Y[q] != imd.Y[q])) {
	    fY.Push(q);
	    imd.Y[q] = MIN(imd.Y[p], ims2.Y[q]);
	 }

      }
   }
   
   while (!fZ.Empty()) {
      p = fZ.Pop();
      for (v = 0; v < connexity; v++) {
	 q = p + reinterpret_cast<const Point2d*>(vc[connexity])[v];
	 if (imd.Hold(q) && (imd.Z[q] < imd.Z[p]) && (ims2.Z[q] != imd.Z[q])) {
	    fZ.Push(q);
	    imd.Z[q]  =MIN(imd.Z[p], ims2.Z[q]);
	 }
      }
   }
   
   return SUCCESS;
}

Errc PDilationReconstruction( const Imc3duc &ims1, const Imc3duc &ims2, Imc3duc &imd, int connexity ) {
   Point3d p; 
   int v;
   Imc3duc::ValueType val, maxv;
   Fifo<Point3d> fX, fY, fZ;

   if (connexity != 4 && connexity != 8 && connexity != 6 && connexity != 26) {
      std::cerr << "Error pdilationreconstruction: bad connexity value:" << connexity << std::endl;
      return FAILURE;
   }
   if ( ims1.Size() != ims2.Size()) {
      std::cerr << "Error pdilationreconstruction: Incompatible image size." << std::endl;
      return FAILURE;
   }
   if ( ims1.Bands() != ims2.Bands() ) {
      std::cerr << "Error pdilationreconstruction: Incompatible number of bands." << std::endl;
      return FAILURE;
   }
   
   imd = ims1;
   
   // Forward scanning
   for (p.z = 0; p.z < imd.Depth(); p.z++)
   for (p.y = 0; p.y < imd.Height(); p.y++)
   for (p.x = 0; p.x < imd.Width(); p.x++)
   {
      maxv = imd.X[p];
      for (v = 0; v < connexity / 2; v++) {
	 if (imd.Hold(p + reinterpret_cast<const Point3d*>(vc[connexity])[v])) {
	    val = imd.X[p + reinterpret_cast<const Point3d*>(vc[connexity])[v]];
	    if (val > maxv) {
	       maxv = val;
	    }
	 }
      }
      imd.X[p] = MIN(maxv, ims2.X[p]);

      maxv = imd.Y[p];
      for (v = 0; v < connexity / 2; v++) {
	 if (imd.Hold(p + reinterpret_cast<const Point3d*>(vc[connexity])[v])) {
	    val = imd.Y[p + reinterpret_cast<const Point3d*>(vc[connexity])[v]];
	    if (val > maxv) {
	       maxv = val;
	    }
	 }
      }
      imd.Y[p] = MIN(maxv, ims2.Y[p]);

      maxv = imd.Z[p];
      for (v = 0; v < connexity / 2; v++) {
	 if (imd.Hold(p + reinterpret_cast<const Point3d*>(vc[connexity])[v])) {
	    val = imd.Z[p + reinterpret_cast<const Point3d*>(vc[connexity])[v]];
	    if (val > maxv) {
	       maxv = val;
	    }
	 }
      }
      imd.Z[p] = MIN(maxv, ims2.Z[p]);
   }
   
   // Backward scanning
   for (p.z = imd.Depth() - 1; p.z >= 0; p.z--)
   for (p.y = imd.Height() - 1; p.y >= 0; p.y--)
   for (p.x = imd.Width() - 1; p.x >= 0; p.x--)
   {
      maxv = imd.X[p];
      for (v = connexity / 2; v < connexity; v++) {
	 if (imd.Hold(p + reinterpret_cast<const Point3d*>(vc[connexity])[v])) {
	    val = imd.X[p + reinterpret_cast<const Point3d*>(vc[connexity])[v]];
	    if (val > maxv) {
	       maxv = val;
	    }
	 }
      }
      imd.X[p] = MIN(maxv, ims2.X[p]);
      for (v = connexity / 2; v < connexity; v++) {
	 if (imd.Hold(p + reinterpret_cast<const Point3d*>(vc[connexity])[v])
	     && (imd.X[p + reinterpret_cast<const Point3d*>(vc[connexity])[v]] < imd.X[p])
	     && (imd.X[p + reinterpret_cast<const Point3d*>(vc[connexity])[v]] < ims2.X[p + reinterpret_cast<const Point3d*>(vc[connexity])[v]])) { 
	    fX.Push(p);
	    break;
	 }
      }
 
      maxv = imd.Y[p];
      for (v = connexity / 2; v < connexity; v++) {
	 if (imd.Hold(p + reinterpret_cast<const Point3d*>(vc[connexity])[v])) {
	    val = imd.Y[p + reinterpret_cast<const Point3d*>(vc[connexity])[v]];
	    if (val > maxv) {
	       maxv = val;
	    }
	 }
      }
      imd.Y[p] = MIN(maxv, ims2.Y[p]);
      for (v = connexity / 2; v < connexity; v++) {
	 if (imd.Hold(p + reinterpret_cast<const Point3d*>(vc[connexity])[v])
	     && (imd.Y[p + reinterpret_cast<const Point3d*>(vc[connexity])[v]] < imd.Y[p])
	     && (imd.Y[p + reinterpret_cast<const Point3d*>(vc[connexity])[v]] < ims2.Y[p + reinterpret_cast<const Point3d*>(vc[connexity])[v]])) {  
	    fY.Push(p);
	    break;
	 }
      }
      maxv = imd.Z[p];
      for (v = connexity / 2; v < connexity; v++) {
	 if (imd.Hold(p + reinterpret_cast<const Point3d*>(vc[connexity])[v])) { 
	    val = imd.Z[p + reinterpret_cast<const Point3d*>(vc[connexity])[v]];
	    if (val > maxv) {
	       maxv = val;
	    }
	 }
      }
      imd.Z[p] = MIN(maxv, ims2.Z[p]);
      for (v = connexity / 2; v < connexity; v++) {
	 if (imd.Hold(p + reinterpret_cast<const Point3d*>(vc[connexity])[v])
	     && (imd.Z[p + reinterpret_cast<const Point3d*>(vc[connexity])[v]] < imd.Z[p])
	     && (imd.Z[p + reinterpret_cast<const Point3d*>(vc[connexity])[v]] < ims2.Z[p+reinterpret_cast<const Point3d*>(vc[connexity])[v]])) { 
	    fZ.Push(p);
	    break;
	 }
      }
   }
   
   // Propagation
   Point3d q;
   while (!fX.Empty()) {
      p = fX.Pop();
      for (v = 0; v < connexity; v++) {
	 q = p + reinterpret_cast<const Point3d*>(vc[connexity])[v];
	 if (imd.Hold(q) && (imd.X[q] < imd.X[p]) && (ims2.X[q] != imd.X[q])) {
	    fX.Push(q);
	    imd.X[q] = MIN(imd.X[p], ims2.X[q]);
	 }
      }
   }
   

   while (!fY.Empty()) {
      p = fY.Pop();
      for (v = 0; v < connexity; v++) {
	 q = p + reinterpret_cast<const Point3d*>(vc[connexity])[v];
	 if (imd.Hold(q) && (imd.Y[q] < imd.Y[p]) && (ims2.Y[q] != imd.Y[q])) {
	    fY.Push(q);
	    imd.Y[q] = MIN(imd.Y[p], ims2.Y[q]);
	 }

      }
   }
   
   while (!fZ.Empty()) {
      p = fZ.Pop();
      for (v = 0; v < connexity; v++) {
	 q = p + reinterpret_cast<const Point3d*>(vc[connexity])[v];
	 if (imd.Hold(q) && (imd.Z[q] < imd.Z[p]) && (ims2.Z[q] != imd.Z[q])) {
	    fZ.Push(q);
	    imd.Z[q]  =MIN(imd.Z[p], ims2.Z[q]);
	 }
      }
   }
   
   return SUCCESS;
}

Errc PDilationReconstruction( const Imc3dsl &ims1, const Imc3dsl &ims2, Imc3dsl &imd, int connexity ) {
   Point3d p; 
   int v;
   Imc3dsl::ValueType val, maxv;
   Fifo<Point3d> fX, fY, fZ;

   if (connexity != 4 && connexity != 8 && connexity != 6 && connexity != 26) {
      std::cerr << "Error pdilationreconstruction: bad connexity value:" << connexity << std::endl;
      return FAILURE;
   }
   if ( ims1.Size() != ims2.Size()) {
      std::cerr << "Error pdilationreconstruction: Incompatible image size." << std::endl;
      return FAILURE;
   }
   if ( ims1.Bands() != ims2.Bands() ) {
      std::cerr << "Error pdilationreconstruction: Incompatible number of bands." << std::endl;
      return FAILURE;
   }
   
   imd = ims1;
   
   // Forward scanning
   for (p.z = 0; p.z < imd.Depth(); p.z++)
   for (p.y = 0; p.y < imd.Height(); p.y++)
   for (p.x = 0; p.x < imd.Width(); p.x++)
   {
      maxv = imd.X[p];
      for (v = 0; v < connexity / 2; v++) {
	 if (imd.Hold(p + reinterpret_cast<const Point3d*>(vc[connexity])[v])) {
	    val = imd.X[p + reinterpret_cast<const Point3d*>(vc[connexity])[v]];
	    if (val > maxv) {
	       maxv = val;
	    }
	 }
      }
      imd.X[p] = MIN(maxv, ims2.X[p]);

      maxv = imd.Y[p];
      for (v = 0; v < connexity / 2; v++) {
	 if (imd.Hold(p + reinterpret_cast<const Point3d*>(vc[connexity])[v])) {
	    val = imd.Y[p + reinterpret_cast<const Point3d*>(vc[connexity])[v]];
	    if (val > maxv) {
	       maxv = val;
	    }
	 }
      }
      imd.Y[p] = MIN(maxv, ims2.Y[p]);

      maxv = imd.Z[p];
      for (v = 0; v < connexity / 2; v++) {
	 if (imd.Hold(p + reinterpret_cast<const Point3d*>(vc[connexity])[v])) {
	    val = imd.Z[p + reinterpret_cast<const Point3d*>(vc[connexity])[v]];
	    if (val > maxv) {
	       maxv = val;
	    }
	 }
      }
      imd.Z[p] = MIN(maxv, ims2.Z[p]);
   }
   
   // Backward scanning
   for (p.z = imd.Depth() - 1; p.z >= 0; p.z--)
   for (p.y = imd.Height() - 1; p.y >= 0; p.y--)
   for (p.x = imd.Width() - 1; p.x >= 0; p.x--)
   {
      maxv = imd.X[p];
      for (v = connexity / 2; v < connexity; v++) {
	 if (imd.Hold(p + reinterpret_cast<const Point3d*>(vc[connexity])[v])) {
	    val = imd.X[p + reinterpret_cast<const Point3d*>(vc[connexity])[v]];
	    if (val > maxv) {
	       maxv = val;
	    }
	 }
      }
      imd.X[p] = MIN(maxv, ims2.X[p]);
      for (v = connexity / 2; v < connexity; v++) {
	 if (imd.Hold(p + reinterpret_cast<const Point3d*>(vc[connexity])[v])
	     && (imd.X[p + reinterpret_cast<const Point3d*>(vc[connexity])[v]] < imd.X[p])
	     && (imd.X[p + reinterpret_cast<const Point3d*>(vc[connexity])[v]] < ims2.X[p + reinterpret_cast<const Point3d*>(vc[connexity])[v]])) { 
	    fX.Push(p);
	    break;
	 }
      }
 
      maxv = imd.Y[p];
      for (v = connexity / 2; v < connexity; v++) {
	 if (imd.Hold(p + reinterpret_cast<const Point3d*>(vc[connexity])[v])) {
	    val = imd.Y[p + reinterpret_cast<const Point3d*>(vc[connexity])[v]];
	    if (val > maxv) {
	       maxv = val;
	    }
	 }
      }
      imd.Y[p] = MIN(maxv, ims2.Y[p]);
      for (v = connexity / 2; v < connexity; v++) {
	 if (imd.Hold(p + reinterpret_cast<const Point3d*>(vc[connexity])[v])
	     && (imd.Y[p + reinterpret_cast<const Point3d*>(vc[connexity])[v]] < imd.Y[p])
	     && (imd.Y[p + reinterpret_cast<const Point3d*>(vc[connexity])[v]] < ims2.Y[p + reinterpret_cast<const Point3d*>(vc[connexity])[v]])) {  
	    fY.Push(p);
	    break;
	 }
      }
      maxv = imd.Z[p];
      for (v = connexity / 2; v < connexity; v++) {
	 if (imd.Hold(p + reinterpret_cast<const Point3d*>(vc[connexity])[v])) { 
	    val = imd.Z[p + reinterpret_cast<const Point3d*>(vc[connexity])[v]];
	    if (val > maxv) {
	       maxv = val;
	    }
	 }
      }
      imd.Z[p] = MIN(maxv, ims2.Z[p]);
      for (v = connexity / 2; v < connexity; v++) {
	 if (imd.Hold(p + reinterpret_cast<const Point3d*>(vc[connexity])[v])
	     && (imd.Z[p + reinterpret_cast<const Point3d*>(vc[connexity])[v]] < imd.Z[p])
	     && (imd.Z[p + reinterpret_cast<const Point3d*>(vc[connexity])[v]] < ims2.Z[p+reinterpret_cast<const Point3d*>(vc[connexity])[v]])) { 
	    fZ.Push(p);
	    break;
	 }
      }
   }
   
   // Propagation
   Point3d q;
   while (!fX.Empty()) {
      p = fX.Pop();
      for (v = 0; v < connexity; v++) {
	 q = p + reinterpret_cast<const Point3d*>(vc[connexity])[v];
	 if (imd.Hold(q) && (imd.X[q] < imd.X[p]) && (ims2.X[q] != imd.X[q])) {
	    fX.Push(q);
	    imd.X[q] = MIN(imd.X[p], ims2.X[q]);
	 }
      }
   }
   

   while (!fY.Empty()) {
      p = fY.Pop();
      for (v = 0; v < connexity; v++) {
	 q = p + reinterpret_cast<const Point3d*>(vc[connexity])[v];
	 if (imd.Hold(q) && (imd.Y[q] < imd.Y[p]) && (ims2.Y[q] != imd.Y[q])) {
	    fY.Push(q);
	    imd.Y[q] = MIN(imd.Y[p], ims2.Y[q]);
	 }

      }
   }
   
   while (!fZ.Empty()) {
      p = fZ.Pop();
      for (v = 0; v < connexity; v++) {
	 q = p + reinterpret_cast<const Point3d*>(vc[connexity])[v];
	 if (imd.Hold(q) && (imd.Z[q] < imd.Z[p]) && (ims2.Z[q] != imd.Z[q])) {
	    fZ.Push(q);
	    imd.Z[q]  =MIN(imd.Z[p], ims2.Z[q]);
	 }
      }
   }
   
   return SUCCESS;
}

Errc PDilationReconstruction( const Imc3dsf &ims1, const Imc3dsf &ims2, Imc3dsf &imd, int connexity ) {
   Point3d p; 
   int v;
   Imc3dsf::ValueType val, maxv;
   Fifo<Point3d> fX, fY, fZ;

   if (connexity != 4 && connexity != 8 && connexity != 6 && connexity != 26) {
      std::cerr << "Error pdilationreconstruction: bad connexity value:" << connexity << std::endl;
      return FAILURE;
   }
   if ( ims1.Size() != ims2.Size()) {
      std::cerr << "Error pdilationreconstruction: Incompatible image size." << std::endl;
      return FAILURE;
   }
   if ( ims1.Bands() != ims2.Bands() ) {
      std::cerr << "Error pdilationreconstruction: Incompatible number of bands." << std::endl;
      return FAILURE;
   }
   
   imd = ims1;
   
   // Forward scanning
   for (p.z = 0; p.z < imd.Depth(); p.z++)
   for (p.y = 0; p.y < imd.Height(); p.y++)
   for (p.x = 0; p.x < imd.Width(); p.x++)
   {
      maxv = imd.X[p];
      for (v = 0; v < connexity / 2; v++) {
	 if (imd.Hold(p + reinterpret_cast<const Point3d*>(vc[connexity])[v])) {
	    val = imd.X[p + reinterpret_cast<const Point3d*>(vc[connexity])[v]];
	    if (val > maxv) {
	       maxv = val;
	    }
	 }
      }
      imd.X[p] = MIN(maxv, ims2.X[p]);

      maxv = imd.Y[p];
      for (v = 0; v < connexity / 2; v++) {
	 if (imd.Hold(p + reinterpret_cast<const Point3d*>(vc[connexity])[v])) {
	    val = imd.Y[p + reinterpret_cast<const Point3d*>(vc[connexity])[v]];
	    if (val > maxv) {
	       maxv = val;
	    }
	 }
      }
      imd.Y[p] = MIN(maxv, ims2.Y[p]);

      maxv = imd.Z[p];
      for (v = 0; v < connexity / 2; v++) {
	 if (imd.Hold(p + reinterpret_cast<const Point3d*>(vc[connexity])[v])) {
	    val = imd.Z[p + reinterpret_cast<const Point3d*>(vc[connexity])[v]];
	    if (val > maxv) {
	       maxv = val;
	    }
	 }
      }
      imd.Z[p] = MIN(maxv, ims2.Z[p]);
   }
   
   // Backward scanning
   for (p.z = imd.Depth() - 1; p.z >= 0; p.z--)
   for (p.y = imd.Height() - 1; p.y >= 0; p.y--)
   for (p.x = imd.Width() - 1; p.x >= 0; p.x--)
   {
      maxv = imd.X[p];
      for (v = connexity / 2; v < connexity; v++) {
	 if (imd.Hold(p + reinterpret_cast<const Point3d*>(vc[connexity])[v])) {
	    val = imd.X[p + reinterpret_cast<const Point3d*>(vc[connexity])[v]];
	    if (val > maxv) {
	       maxv = val;
	    }
	 }
      }
      imd.X[p] = MIN(maxv, ims2.X[p]);
      for (v = connexity / 2; v < connexity; v++) {
	 if (imd.Hold(p + reinterpret_cast<const Point3d*>(vc[connexity])[v])
	     && (imd.X[p + reinterpret_cast<const Point3d*>(vc[connexity])[v]] < imd.X[p])
	     && (imd.X[p + reinterpret_cast<const Point3d*>(vc[connexity])[v]] < ims2.X[p + reinterpret_cast<const Point3d*>(vc[connexity])[v]])) { 
	    fX.Push(p);
	    break;
	 }
      }
 
      maxv = imd.Y[p];
      for (v = connexity / 2; v < connexity; v++) {
	 if (imd.Hold(p + reinterpret_cast<const Point3d*>(vc[connexity])[v])) {
	    val = imd.Y[p + reinterpret_cast<const Point3d*>(vc[connexity])[v]];
	    if (val > maxv) {
	       maxv = val;
	    }
	 }
      }
      imd.Y[p] = MIN(maxv, ims2.Y[p]);
      for (v = connexity / 2; v < connexity; v++) {
	 if (imd.Hold(p + reinterpret_cast<const Point3d*>(vc[connexity])[v])
	     && (imd.Y[p + reinterpret_cast<const Point3d*>(vc[connexity])[v]] < imd.Y[p])
	     && (imd.Y[p + reinterpret_cast<const Point3d*>(vc[connexity])[v]] < ims2.Y[p + reinterpret_cast<const Point3d*>(vc[connexity])[v]])) {  
	    fY.Push(p);
	    break;
	 }
      }
      maxv = imd.Z[p];
      for (v = connexity / 2; v < connexity; v++) {
	 if (imd.Hold(p + reinterpret_cast<const Point3d*>(vc[connexity])[v])) { 
	    val = imd.Z[p + reinterpret_cast<const Point3d*>(vc[connexity])[v]];
	    if (val > maxv) {
	       maxv = val;
	    }
	 }
      }
      imd.Z[p] = MIN(maxv, ims2.Z[p]);
      for (v = connexity / 2; v < connexity; v++) {
	 if (imd.Hold(p + reinterpret_cast<const Point3d*>(vc[connexity])[v])
	     && (imd.Z[p + reinterpret_cast<const Point3d*>(vc[connexity])[v]] < imd.Z[p])
	     && (imd.Z[p + reinterpret_cast<const Point3d*>(vc[connexity])[v]] < ims2.Z[p+reinterpret_cast<const Point3d*>(vc[connexity])[v]])) { 
	    fZ.Push(p);
	    break;
	 }
      }
   }
   
   // Propagation
   Point3d q;
   while (!fX.Empty()) {
      p = fX.Pop();
      for (v = 0; v < connexity; v++) {
	 q = p + reinterpret_cast<const Point3d*>(vc[connexity])[v];
	 if (imd.Hold(q) && (imd.X[q] < imd.X[p]) && (ims2.X[q] != imd.X[q])) {
	    fX.Push(q);
	    imd.X[q] = MIN(imd.X[p], ims2.X[q]);
	 }
      }
   }
   

   while (!fY.Empty()) {
      p = fY.Pop();
      for (v = 0; v < connexity; v++) {
	 q = p + reinterpret_cast<const Point3d*>(vc[connexity])[v];
	 if (imd.Hold(q) && (imd.Y[q] < imd.Y[p]) && (ims2.Y[q] != imd.Y[q])) {
	    fY.Push(q);
	    imd.Y[q] = MIN(imd.Y[p], ims2.Y[q]);
	 }

      }
   }
   
   while (!fZ.Empty()) {
      p = fZ.Pop();
      for (v = 0; v < connexity; v++) {
	 q = p + reinterpret_cast<const Point3d*>(vc[connexity])[v];
	 if (imd.Hold(q) && (imd.Z[q] < imd.Z[p]) && (ims2.Z[q] != imd.Z[q])) {
	    fZ.Push(q);
	    imd.Z[q]  =MIN(imd.Z[p], ims2.Z[q]);
	 }
      }
   }
   
   return SUCCESS;
}

Errc PDilationReconstruction( const Graph2d &grs1, const Graph2d &grs2, Graph2d &grd ) {
   int i, j;		/*Gm=gs1=masque=numerique=I*/
   Graph2d::ValueType u, v;	/*G=gs2=marqueurs=J*/
   bool propage;	/*G->Gmax=gd=maxima regionaux de G*/
   GEdge *ptr;
   Fifo<Point1d> f;
   
   grd = grs2;
   
   for (i = 0; i < grd.Size(); i++) {
      if ((grd[i])) {
	 grd[i]->value = 255.0;
      }
   }
   for (i = 0; i < grd.Size(); i++) {
      if (!grd[i] || grd[i]->value != 255.0) {
	 continue;
      }
      propage = false;
      for (ptr = grd[i]->Neighbours(); ptr != NULL; ptr = ptr->Next()) {
	 if (grs2[i]->value < grs2[ptr->Node()]->value) {
	    propage = true;
	 }
      }
      if (propage) {
	 f.Push(Point1d(i));
	 grd[i]->value = 0;
	 while (!f.Empty()) {
	    j = f.Pop().x;
	    for (ptr = grd[j]->Neighbours(); ptr != NULL; ptr = ptr->Next()) {
	       if ((grd[ptr->Node()]->value == 255.0)
		   && (grs2[j]->value >= grd[ptr->Node()]->value)) {
		  f.Push(Point1d(ptr->Node()));
		  grd[ptr->Node()]->value = 0.0;
	       }
	    }
	 }
      }
   }
   
   for (i = 0; i < grd.Size(); i++) {
      if (grd[i] && grd[i]->value) {
	 grd[i]->value = grs2[i]->value;
      }
   }
   for (i = 0; i < grd.Size(); i++) {
      if (!grd[i] || grd[i]->value == 0) {
	 continue;
      }
      propage = false;
      for (ptr = grd[i]->Neighbours(); ptr != NULL; ptr = ptr->Next()) {
	 if (grd[ptr->Node()]->value == 0) {
	    propage = true;
	 }
      }
      if (propage) {
	 f.Push(Point1d(i));
      }
   }
   
   while (!f.Empty()) {
      j = f.Pop().x;
      for (ptr = grd[j]->Neighbours(); ptr != NULL; ptr = ptr->Next()) {
	 if ((grd[ptr->Node()]->value < grd[j]->value)
	     && (grd[ptr->Node()]->value != grs1[ptr->Node()]->value)) {
	    u = grd[j]->value;
	    v = grs1[ptr->Node()]->value;
	    grd[ptr->Node()]->value = (u < v) ? u : v;
	    f.Push(Point1d(ptr->Node()));
	 }
      }
   }
   
   return SUCCESS;
}

Errc PDilationReconstruction( const Graph3d &grs1, const Graph3d &grs2, Graph3d &grd ) {
   int i, j;		/*Gm=gs1=masque=numerique=I*/
   Graph3d::ValueType u, v;	/*G=gs2=marqueurs=J*/
   bool propage;	/*G->Gmax=gd=maxima regionaux de G*/
   GEdge *ptr;
   Fifo<Point1d> f;
   
   grd = grs2;
   
   for (i = 0; i < grd.Size(); i++) {
      if ((grd[i])) {
	 grd[i]->value = 255.0;
      }
   }
   for (i = 0; i < grd.Size(); i++) {
      if (!grd[i] || grd[i]->value != 255.0) {
	 continue;
      }
      propage = false;
      for (ptr = grd[i]->Neighbours(); ptr != NULL; ptr = ptr->Next()) {
	 if (grs2[i]->value < grs2[ptr->Node()]->value) {
	    propage = true;
	 }
      }
      if (propage) {
	 f.Push(Point1d(i));
	 grd[i]->value = 0;
	 while (!f.Empty()) {
	    j = f.Pop().x;
	    for (ptr = grd[j]->Neighbours(); ptr != NULL; ptr = ptr->Next()) {
	       if ((grd[ptr->Node()]->value == 255.0)
		   && (grs2[j]->value >= grd[ptr->Node()]->value)) {
		  f.Push(Point1d(ptr->Node()));
		  grd[ptr->Node()]->value = 0.0;
	       }
	    }
	 }
      }
   }
   
   for (i = 0; i < grd.Size(); i++) {
      if (grd[i] && grd[i]->value) {
	 grd[i]->value = grs2[i]->value;
      }
   }
   for (i = 0; i < grd.Size(); i++) {
      if (!grd[i] || grd[i]->value == 0) {
	 continue;
      }
      propage = false;
      for (ptr = grd[i]->Neighbours(); ptr != NULL; ptr = ptr->Next()) {
	 if (grd[ptr->Node()]->value == 0) {
	    propage = true;
	 }
      }
      if (propage) {
	 f.Push(Point1d(i));
      }
   }
   
   while (!f.Empty()) {
      j = f.Pop().x;
      for (ptr = grd[j]->Neighbours(); ptr != NULL; ptr = ptr->Next()) {
	 if ((grd[ptr->Node()]->value < grd[j]->value)
	     && (grd[ptr->Node()]->value != grs1[ptr->Node()]->value)) {
	    u = grd[j]->value;
	    v = grs1[ptr->Node()]->value;
	    grd[ptr->Node()]->value = (u < v) ? u : v;
	    f.Push(Point1d(ptr->Node()));
	 }
      }
   }
   
   return SUCCESS;
}


#ifdef MAIN
#define	USAGE	"usage: %s connexity [-m mask] [im_in1|-] [im_in2|-] [im_out|-]"
#define	PARC	1
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
   if ((objs[0]->Type() == Po_Img1duc) && (objs[1]->Type() == Po_Img1duc)) {
     Img1duc* const ims1 = (Img1duc*)objs[0];
     Img1duc* const ims2 = (Img1duc*)objs[1];
     objd[0] = new Img1duc(ims1->Props());
     Img1duc* const imd = (Img1duc*)objd[0];
     
     result = PDilationReconstruction(*ims1, *ims2, *imd, atoi(parv[0]));
     goto end;
   }
   if ((objs[0]->Type() == Po_Img1dsl) && (objs[1]->Type() == Po_Img1dsl)) {
     Img1dsl* const ims1 = (Img1dsl*)objs[0];
     Img1dsl* const ims2 = (Img1dsl*)objs[1];
     objd[0] = new Img1dsl(ims1->Props());
     Img1dsl* const imd = (Img1dsl*)objd[0];
     
     result = PDilationReconstruction(*ims1, *ims2, *imd, atoi(parv[0]));
     goto end;
   }
   if ((objs[0]->Type() == Po_Img1dsf) && (objs[1]->Type() == Po_Img1dsf)) {
     Img1dsf* const ims1 = (Img1dsf*)objs[0];
     Img1dsf* const ims2 = (Img1dsf*)objs[1];
     objd[0] = new Img1dsf(ims1->Props());
     Img1dsf* const imd = (Img1dsf*)objd[0];
     
     result = PDilationReconstruction(*ims1, *ims2, *imd, atoi(parv[0]));
     goto end;
   }
   if ((objs[0]->Type() == Po_Img2duc) && (objs[1]->Type() == Po_Img2duc)) {
     Img2duc* const ims1 = (Img2duc*)objs[0];
     Img2duc* const ims2 = (Img2duc*)objs[1];
     objd[0] = new Img2duc(ims1->Props());
     Img2duc* const imd = (Img2duc*)objd[0];
     
     result = PDilationReconstruction(*ims1, *ims2, *imd, atoi(parv[0]));
     goto end;
   }
   if ((objs[0]->Type() == Po_Img2dsl) && (objs[1]->Type() == Po_Img2dsl)) {
     Img2dsl* const ims1 = (Img2dsl*)objs[0];
     Img2dsl* const ims2 = (Img2dsl*)objs[1];
     objd[0] = new Img2dsl(ims1->Props());
     Img2dsl* const imd = (Img2dsl*)objd[0];
     
     result = PDilationReconstruction(*ims1, *ims2, *imd, atoi(parv[0]));
     goto end;
   }
   if ((objs[0]->Type() == Po_Img2dsf) && (objs[1]->Type() == Po_Img2dsf)) {
     Img2dsf* const ims1 = (Img2dsf*)objs[0];
     Img2dsf* const ims2 = (Img2dsf*)objs[1];
     objd[0] = new Img2dsf(ims1->Props());
     Img2dsf* const imd = (Img2dsf*)objd[0];
     
     result = PDilationReconstruction(*ims1, *ims2, *imd, atoi(parv[0]));
     goto end;
   }
   if ((objs[0]->Type() == Po_Img3duc) && (objs[1]->Type() == Po_Img3duc)) {
     Img3duc* const ims1 = (Img3duc*)objs[0];
     Img3duc* const ims2 = (Img3duc*)objs[1];
     objd[0] = new Img3duc(ims1->Props());
     Img3duc* const imd = (Img3duc*)objd[0];
     
     result = PDilationReconstruction(*ims1, *ims2, *imd, atoi(parv[0]));
     goto end;
   }
   if ((objs[0]->Type() == Po_Img3dsl) && (objs[1]->Type() == Po_Img3dsl)) {
     Img3dsl* const ims1 = (Img3dsl*)objs[0];
     Img3dsl* const ims2 = (Img3dsl*)objs[1];
     objd[0] = new Img3dsl(ims1->Props());
     Img3dsl* const imd = (Img3dsl*)objd[0];
     
     result = PDilationReconstruction(*ims1, *ims2, *imd, atoi(parv[0]));
     goto end;
   }
   if ((objs[0]->Type() == Po_Img3dsf) && (objs[1]->Type() == Po_Img3dsf)) {
     Img3dsf* const ims1 = (Img3dsf*)objs[0];
     Img3dsf* const ims2 = (Img3dsf*)objs[1];
     objd[0] = new Img3dsf(ims1->Props());
     Img3dsf* const imd = (Img3dsf*)objd[0];
     
     result = PDilationReconstruction(*ims1, *ims2, *imd, atoi(parv[0]));
     goto end;
   }
   if ((objs[0]->Type() == Po_Imc2duc) && (objs[1]->Type() == Po_Imc2duc)) {
     Imc2duc* const ims1 = (Imc2duc*)objs[0];
     Imc2duc* const ims2 = (Imc2duc*)objs[1];
     objd[0] = new Imc2duc(ims1->Props());
     Imc2duc* const imd = (Imc2duc*)objd[0];
     
     result = PDilationReconstruction(*ims1, *ims2, *imd, atoi(parv[0]));
     goto end;
   }
   if ((objs[0]->Type() == Po_Imc2dsl) && (objs[1]->Type() == Po_Imc2dsl)) {
     Imc2dsl* const ims1 = (Imc2dsl*)objs[0];
     Imc2dsl* const ims2 = (Imc2dsl*)objs[1];
     objd[0] = new Imc2dsl(ims1->Props());
     Imc2dsl* const imd = (Imc2dsl*)objd[0];
     
     result = PDilationReconstruction(*ims1, *ims2, *imd, atoi(parv[0]));
     goto end;
   }
   if ((objs[0]->Type() == Po_Imc2dsf) && (objs[1]->Type() == Po_Imc2dsf)) {
     Imc2dsf* const ims1 = (Imc2dsf*)objs[0];
     Imc2dsf* const ims2 = (Imc2dsf*)objs[1];
     objd[0] = new Imc2dsf(ims1->Props());
     Imc2dsf* const imd = (Imc2dsf*)objd[0];
     
     result = PDilationReconstruction(*ims1, *ims2, *imd, atoi(parv[0]));
     goto end;
   }
   if ((objs[0]->Type() == Po_Imc3duc) && (objs[1]->Type() == Po_Imc3duc)) {
     Imc3duc* const ims1 = (Imc3duc*)objs[0];
     Imc3duc* const ims2 = (Imc3duc*)objs[1];
     objd[0] = new Imc3duc(ims1->Props());
     Imc3duc* const imd = (Imc3duc*)objd[0];
     
     result = PDilationReconstruction(*ims1, *ims2, *imd, atoi(parv[0]));
     goto end;
   }
   if ((objs[0]->Type() == Po_Imc3dsl) && (objs[1]->Type() == Po_Imc3dsl)) {
     Imc3dsl* const ims1 = (Imc3dsl*)objs[0];
     Imc3dsl* const ims2 = (Imc3dsl*)objs[1];
     objd[0] = new Imc3dsl(ims1->Props());
     Imc3dsl* const imd = (Imc3dsl*)objd[0];
     
     result = PDilationReconstruction(*ims1, *ims2, *imd, atoi(parv[0]));
     goto end;
   }
   if ((objs[0]->Type() == Po_Imc3dsf) && (objs[1]->Type() == Po_Imc3dsf)) {
     Imc3dsf* const ims1 = (Imc3dsf*)objs[0];
     Imc3dsf* const ims2 = (Imc3dsf*)objs[1];
     objd[0] = new Imc3dsf(ims1->Props());
     Imc3dsf* const imd = (Imc3dsf*)objd[0];
     
     result = PDilationReconstruction(*ims1, *ims2, *imd, atoi(parv[0]));
     goto end;
   }
   if ((objs[0]->Type() == Po_Graph2d) && (objs[1]->Type() == Po_Graph2d)) {
     Graph2d* const grs1 = (Graph2d*)objs[0];
     Graph2d* const grs2 = (Graph2d*)objs[1];
     objd[0] = new Graph2d(grs1->Props());
     Graph2d* const grd = (Graph2d*)objd[0];
     
     result = PDilationReconstruction(*grs1, *grs2, *grd);
     goto end;
   }
   if ((objs[0]->Type() == Po_Graph3d) && (objs[1]->Type() == Po_Graph3d)) {
     Graph3d* const grs1 = (Graph3d*)objs[0];
     Graph3d* const grs2 = (Graph3d*)objs[1];
     objd[0] = new Graph3d(grs1->Props());
     Graph3d* const grd = (Graph3d*)objd[0];
     
     result = PDilationReconstruction(*grs1, *grs2, *grd);
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
