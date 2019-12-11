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
 * @author Régis Clouard - 1996-02-19
 * @author Régis Clouard - 2003-05-23 (new algorithm)
 * @author Régis Clouard - Jun 19, 2011 (new algorithm)
 */

#define _USE_MATH_DEFINES
#include <math.h>
#include <pandore.h>
using namespace pandore;
#include "morphology/structelt.h"

/**
 * @file pdilatationgeodesique.cpp
 * Geodesic dilatation:: dilatation controled by a mask image.
 * if iteration=-1, dilatation is done until idempotence
 * wich corresponds to a reconstruction by dilatation.
 */



#ifndef __GREATER_H__
#define __GREATER_H__
/*
 * Lexicographic order between 2 colors (cx, cy, cz) and (cx1, cy1, cz1)
 */
template<typename T>
bool Greater(T cx, T cy, T cz, T cx1, T cy1, T cz1) {
   if (cx1 < cx) {
      return true;
   } else if (cx1 > cx) {
      return false;
   }
   if (cy1 < cy) {
      return true;
   } else if (cy1 > cy) {
      return false;
   }
   if (cz1 < cz) {
      return true;
   } else if (cz1 > cz) {
      return false;
   } else {
      return false;
   }
}

#endif



Errc PGeodesicDilation( const Img2duc &ims, const Img2duc &imq, Img2duc &imd, int numSE, int halfsize, int iteration ) {
   if (numSE > 8 || numSE < 0) {
      std::cerr << "Error pgeodesicdilatation: bad structuring element type: " << numSE << std::endl;
      return FAILURE;
   }
   if (halfsize < 1) {
      std::cerr << "Error pgeodesicdilatation: bad structuring element size" << halfsize << std::endl;
      return FAILURE;
   }
   if ( ims.Size() != imq.Size()) {
      std::cerr << "Error pdilatationreconstruction: Incompatible image size." << std::endl;
      return FAILURE;
   }

   StructuringElement2d *se = new StructuringElement2d(numSE, halfsize);

   Point2d p, p1;
   Fifo<Point2d> toBeVisited[2];

   imd = ims;
   for (p.y = 0; p.y < ims.Height(); p.y++) {
      for (p.x = 0; p.x < ims.Width(); p.x++) {
	 if ((imq[p])) {
	    Img2duc::ValueType max = Limits<Img2duc::ValueType>::min(), val;
	    for (int i = 0; i < se->length; i++) {
	       p1 = p;
	       se->Move(i, p1);
	       if (ims.Hold(p1) && ((val = ims[p1]) > max) && (imq[p1] == imq[p])) {
		  max = val;
	       }
	    }
	    if (ims[p] < max) {
	       imd[p] = max;
	       toBeVisited[0].Push(p);
	    }
	 }
      }
   }
   int fifo = 0;
   if (iteration < 0) {
      iteration = Limits<Long>::max();
   }
   iteration--;
   while (iteration > 0 && !toBeVisited[fifo].Empty()) {
      iteration--;
      while (!toBeVisited[fifo].Empty()) {
	 Point2d p3 = toBeVisited[fifo].Pop();
	 for (int v = 0; v < 8; v++) {
	    p = p3 + v8[v];
	    if (imq.Hold(p) && (imq[p])) {
	       Img2duc::ValueType max = imd[p], val;
	       for (int i = 0; i < se->length; i++) {
		  p1 = p;
		  se->Move(i, p1);
		  if (imd.Hold(p1) && ((val = imd[p1]) > max) && (imq[p1] == imq[p])) {
		     max = val;
		  }
	       }
	       if (imd[p] < max) {
		  imd[p] = max;
		  toBeVisited[1 - fifo].Push(p);
	       }
	    }
	 }
      }
      fifo = 1 - fifo;
   }

   delete se;
   return SUCCESS;
}

Errc PGeodesicDilation( const Img2dsl &ims, const Img2duc &imq, Img2dsl &imd, int numSE, int halfsize, int iteration ) {
   if (numSE > 8 || numSE < 0) {
      std::cerr << "Error pgeodesicdilatation: bad structuring element type: " << numSE << std::endl;
      return FAILURE;
   }
   if (halfsize < 1) {
      std::cerr << "Error pgeodesicdilatation: bad structuring element size" << halfsize << std::endl;
      return FAILURE;
   }
   if ( ims.Size() != imq.Size()) {
      std::cerr << "Error pdilatationreconstruction: Incompatible image size." << std::endl;
      return FAILURE;
   }

   StructuringElement2d *se = new StructuringElement2d(numSE, halfsize);

   Point2d p, p1;
   Fifo<Point2d> toBeVisited[2];

   imd = ims;
   for (p.y = 0; p.y < ims.Height(); p.y++) {
      for (p.x = 0; p.x < ims.Width(); p.x++) {
	 if ((imq[p])) {
	    Img2dsl::ValueType max = Limits<Img2dsl::ValueType>::min(), val;
	    for (int i = 0; i < se->length; i++) {
	       p1 = p;
	       se->Move(i, p1);
	       if (ims.Hold(p1) && ((val = ims[p1]) > max) && (imq[p1] == imq[p])) {
		  max = val;
	       }
	    }
	    if (ims[p] < max) {
	       imd[p] = max;
	       toBeVisited[0].Push(p);
	    }
	 }
      }
   }
   int fifo = 0;
   if (iteration < 0) {
      iteration = Limits<Long>::max();
   }
   iteration--;
   while (iteration > 0 && !toBeVisited[fifo].Empty()) {
      iteration--;
      while (!toBeVisited[fifo].Empty()) {
	 Point2d p3 = toBeVisited[fifo].Pop();
	 for (int v = 0; v < 8; v++) {
	    p = p3 + v8[v];
	    if (imq.Hold(p) && (imq[p])) {
	       Img2dsl::ValueType max = imd[p], val;
	       for (int i = 0; i < se->length; i++) {
		  p1 = p;
		  se->Move(i, p1);
		  if (imd.Hold(p1) && ((val = imd[p1]) > max) && (imq[p1] == imq[p])) {
		     max = val;
		  }
	       }
	       if (imd[p] < max) {
		  imd[p] = max;
		  toBeVisited[1 - fifo].Push(p);
	       }
	    }
	 }
      }
      fifo = 1 - fifo;
   }

   delete se;
   return SUCCESS;
}

Errc PGeodesicDilation( const Img2dsf &ims, const Img2duc &imq, Img2dsf &imd, int numSE, int halfsize, int iteration ) {
   if (numSE > 8 || numSE < 0) {
      std::cerr << "Error pgeodesicdilatation: bad structuring element type: " << numSE << std::endl;
      return FAILURE;
   }
   if (halfsize < 1) {
      std::cerr << "Error pgeodesicdilatation: bad structuring element size" << halfsize << std::endl;
      return FAILURE;
   }
   if ( ims.Size() != imq.Size()) {
      std::cerr << "Error pdilatationreconstruction: Incompatible image size." << std::endl;
      return FAILURE;
   }

   StructuringElement2d *se = new StructuringElement2d(numSE, halfsize);

   Point2d p, p1;
   Fifo<Point2d> toBeVisited[2];

   imd = ims;
   for (p.y = 0; p.y < ims.Height(); p.y++) {
      for (p.x = 0; p.x < ims.Width(); p.x++) {
	 if ((imq[p])) {
	    Img2dsf::ValueType max = Limits<Img2dsf::ValueType>::min(), val;
	    for (int i = 0; i < se->length; i++) {
	       p1 = p;
	       se->Move(i, p1);
	       if (ims.Hold(p1) && ((val = ims[p1]) > max) && (imq[p1] == imq[p])) {
		  max = val;
	       }
	    }
	    if (ims[p] < max) {
	       imd[p] = max;
	       toBeVisited[0].Push(p);
	    }
	 }
      }
   }
   int fifo = 0;
   if (iteration < 0) {
      iteration = Limits<Long>::max();
   }
   iteration--;
   while (iteration > 0 && !toBeVisited[fifo].Empty()) {
      iteration--;
      while (!toBeVisited[fifo].Empty()) {
	 Point2d p3 = toBeVisited[fifo].Pop();
	 for (int v = 0; v < 8; v++) {
	    p = p3 + v8[v];
	    if (imq.Hold(p) && (imq[p])) {
	       Img2dsf::ValueType max = imd[p], val;
	       for (int i = 0; i < se->length; i++) {
		  p1 = p;
		  se->Move(i, p1);
		  if (imd.Hold(p1) && ((val = imd[p1]) > max) && (imq[p1] == imq[p])) {
		     max = val;
		  }
	       }
	       if (imd[p] < max) {
		  imd[p] = max;
		  toBeVisited[1 - fifo].Push(p);
	       }
	    }
	 }
      }
      fifo = 1 - fifo;
   }

   delete se;
   return SUCCESS;
}

Errc PGeodesicDilation( const Img2duc &ims, const Reg2d &imq, Img2duc &imd, int numSE, int halfsize, int iteration ) {
   if (numSE > 8 || numSE < 0) {
      std::cerr << "Error pgeodesicdilatation: bad structuring element type: " << numSE << std::endl;
      return FAILURE;
   }
   if (halfsize < 1) {
      std::cerr << "Error pgeodesicdilatation: bad structuring element size" << halfsize << std::endl;
      return FAILURE;
   }
   if ( ims.Size() != imq.Size()) {
      std::cerr << "Error pdilatationreconstruction: Incompatible image size." << std::endl;
      return FAILURE;
   }

   StructuringElement2d *se = new StructuringElement2d(numSE, halfsize);

   Point2d p, p1;
   Fifo<Point2d> toBeVisited[2];

   imd = ims;
   for (p.y = 0; p.y < ims.Height(); p.y++) {
      for (p.x = 0; p.x < ims.Width(); p.x++) {
	 if ((imq[p])) {
	    Img2duc::ValueType max = Limits<Img2duc::ValueType>::min(), val;
	    for (int i = 0; i < se->length; i++) {
	       p1 = p;
	       se->Move(i, p1);
	       if (ims.Hold(p1) && ((val = ims[p1]) > max) && (imq[p1] == imq[p])) {
		  max = val;
	       }
	    }
	    if (ims[p] < max) {
	       imd[p] = max;
	       toBeVisited[0].Push(p);
	    }
	 }
      }
   }
   int fifo = 0;
   if (iteration < 0) {
      iteration = Limits<Long>::max();
   }
   iteration--;
   while (iteration > 0 && !toBeVisited[fifo].Empty()) {
      iteration--;
      while (!toBeVisited[fifo].Empty()) {
	 Point2d p3 = toBeVisited[fifo].Pop();
	 for (int v = 0; v < 8; v++) {
	    p = p3 + v8[v];
	    if (imq.Hold(p) && (imq[p])) {
	       Img2duc::ValueType max = imd[p], val;
	       for (int i = 0; i < se->length; i++) {
		  p1 = p;
		  se->Move(i, p1);
		  if (imd.Hold(p1) && ((val = imd[p1]) > max) && (imq[p1] == imq[p])) {
		     max = val;
		  }
	       }
	       if (imd[p] < max) {
		  imd[p] = max;
		  toBeVisited[1 - fifo].Push(p);
	       }
	    }
	 }
      }
      fifo = 1 - fifo;
   }

   delete se;
   return SUCCESS;
}

Errc PGeodesicDilation( const Img2dsl &ims, const Reg2d &imq, Img2dsl &imd, int numSE, int halfsize, int iteration ) {
   if (numSE > 8 || numSE < 0) {
      std::cerr << "Error pgeodesicdilatation: bad structuring element type: " << numSE << std::endl;
      return FAILURE;
   }
   if (halfsize < 1) {
      std::cerr << "Error pgeodesicdilatation: bad structuring element size" << halfsize << std::endl;
      return FAILURE;
   }
   if ( ims.Size() != imq.Size()) {
      std::cerr << "Error pdilatationreconstruction: Incompatible image size." << std::endl;
      return FAILURE;
   }

   StructuringElement2d *se = new StructuringElement2d(numSE, halfsize);

   Point2d p, p1;
   Fifo<Point2d> toBeVisited[2];

   imd = ims;
   for (p.y = 0; p.y < ims.Height(); p.y++) {
      for (p.x = 0; p.x < ims.Width(); p.x++) {
	 if ((imq[p])) {
	    Img2dsl::ValueType max = Limits<Img2dsl::ValueType>::min(), val;
	    for (int i = 0; i < se->length; i++) {
	       p1 = p;
	       se->Move(i, p1);
	       if (ims.Hold(p1) && ((val = ims[p1]) > max) && (imq[p1] == imq[p])) {
		  max = val;
	       }
	    }
	    if (ims[p] < max) {
	       imd[p] = max;
	       toBeVisited[0].Push(p);
	    }
	 }
      }
   }
   int fifo = 0;
   if (iteration < 0) {
      iteration = Limits<Long>::max();
   }
   iteration--;
   while (iteration > 0 && !toBeVisited[fifo].Empty()) {
      iteration--;
      while (!toBeVisited[fifo].Empty()) {
	 Point2d p3 = toBeVisited[fifo].Pop();
	 for (int v = 0; v < 8; v++) {
	    p = p3 + v8[v];
	    if (imq.Hold(p) && (imq[p])) {
	       Img2dsl::ValueType max = imd[p], val;
	       for (int i = 0; i < se->length; i++) {
		  p1 = p;
		  se->Move(i, p1);
		  if (imd.Hold(p1) && ((val = imd[p1]) > max) && (imq[p1] == imq[p])) {
		     max = val;
		  }
	       }
	       if (imd[p] < max) {
		  imd[p] = max;
		  toBeVisited[1 - fifo].Push(p);
	       }
	    }
	 }
      }
      fifo = 1 - fifo;
   }

   delete se;
   return SUCCESS;
}

Errc PGeodesicDilation( const Img2dsf &ims, const Reg2d &imq, Img2dsf &imd, int numSE, int halfsize, int iteration ) {
   if (numSE > 8 || numSE < 0) {
      std::cerr << "Error pgeodesicdilatation: bad structuring element type: " << numSE << std::endl;
      return FAILURE;
   }
   if (halfsize < 1) {
      std::cerr << "Error pgeodesicdilatation: bad structuring element size" << halfsize << std::endl;
      return FAILURE;
   }
   if ( ims.Size() != imq.Size()) {
      std::cerr << "Error pdilatationreconstruction: Incompatible image size." << std::endl;
      return FAILURE;
   }

   StructuringElement2d *se = new StructuringElement2d(numSE, halfsize);

   Point2d p, p1;
   Fifo<Point2d> toBeVisited[2];

   imd = ims;
   for (p.y = 0; p.y < ims.Height(); p.y++) {
      for (p.x = 0; p.x < ims.Width(); p.x++) {
	 if ((imq[p])) {
	    Img2dsf::ValueType max = Limits<Img2dsf::ValueType>::min(), val;
	    for (int i = 0; i < se->length; i++) {
	       p1 = p;
	       se->Move(i, p1);
	       if (ims.Hold(p1) && ((val = ims[p1]) > max) && (imq[p1] == imq[p])) {
		  max = val;
	       }
	    }
	    if (ims[p] < max) {
	       imd[p] = max;
	       toBeVisited[0].Push(p);
	    }
	 }
      }
   }
   int fifo = 0;
   if (iteration < 0) {
      iteration = Limits<Long>::max();
   }
   iteration--;
   while (iteration > 0 && !toBeVisited[fifo].Empty()) {
      iteration--;
      while (!toBeVisited[fifo].Empty()) {
	 Point2d p3 = toBeVisited[fifo].Pop();
	 for (int v = 0; v < 8; v++) {
	    p = p3 + v8[v];
	    if (imq.Hold(p) && (imq[p])) {
	       Img2dsf::ValueType max = imd[p], val;
	       for (int i = 0; i < se->length; i++) {
		  p1 = p;
		  se->Move(i, p1);
		  if (imd.Hold(p1) && ((val = imd[p1]) > max) && (imq[p1] == imq[p])) {
		     max = val;
		  }
	       }
	       if (imd[p] < max) {
		  imd[p] = max;
		  toBeVisited[1 - fifo].Push(p);
	       }
	    }
	 }
      }
      fifo = 1 - fifo;
   }

   delete se;
   return SUCCESS;
}

Errc PGeodesicDilation( const Reg2d &ims, const Reg2d &imq, Reg2d &imd, int numSE, int halfsize, int iteration ) {
   if (numSE > 8 || numSE < 0) {
      std::cerr << "Error pgeodesicdilatation: bad structuring element type: " << numSE << std::endl;
      return FAILURE;
   }
   if (halfsize < 1) {
      std::cerr << "Error pgeodesicdilatation: bad structuring element size" << halfsize << std::endl;
      return FAILURE;
   }
   if ( ims.Size() != imq.Size()) {
      std::cerr << "Error pdilatationreconstruction: Incompatible image size." << std::endl;
      return FAILURE;
   }

   StructuringElement2d *se = new StructuringElement2d(numSE, halfsize);

   Point2d p, p1;
   Fifo<Point2d> toBeVisited[2];

   imd = ims;
   for (p.y = 0; p.y < ims.Height(); p.y++) {
      for (p.x = 0; p.x < ims.Width(); p.x++) {
	 if ((imq[p])) {
	    Reg2d::ValueType max = Limits<Reg2d::ValueType>::min(), val;
	    for (int i = 0; i < se->length; i++) {
	       p1 = p;
	       se->Move(i, p1);
	       if (ims.Hold(p1) && ((val = ims[p1]) > max) && (imq[p1] == imq[p])) {
		  max = val;
	       }
	    }
	    if (ims[p] < max) {
	       imd[p] = max;
	       toBeVisited[0].Push(p);
	    }
	 }
      }
   }
   int fifo = 0;
   if (iteration < 0) {
      iteration = Limits<Long>::max();
   }
   iteration--;
   while (iteration > 0 && !toBeVisited[fifo].Empty()) {
      iteration--;
      while (!toBeVisited[fifo].Empty()) {
	 Point2d p3 = toBeVisited[fifo].Pop();
	 for (int v = 0; v < 8; v++) {
	    p = p3 + v8[v];
	    if (imq.Hold(p) && (imq[p])) {
	       Reg2d::ValueType max = imd[p], val;
	       for (int i = 0; i < se->length; i++) {
		  p1 = p;
		  se->Move(i, p1);
		  if (imd.Hold(p1) && ((val = imd[p1]) > max) && (imq[p1] == imq[p])) {
		     max = val;
		  }
	       }
	       if (imd[p] < max) {
		  imd[p] = max;
		  toBeVisited[1 - fifo].Push(p);
	       }
	    }
	 }
      }
      fifo = 1 - fifo;
   }

   delete se;
   return SUCCESS;
}

Errc PGeodesicDilation( const Reg2d &ims, const Img2duc &imq, Reg2d &imd, int numSE, int halfsize, int iteration ) {
   if (numSE > 8 || numSE < 0) {
      std::cerr << "Error pgeodesicdilatation: bad structuring element type: " << numSE << std::endl;
      return FAILURE;
   }
   if (halfsize < 1) {
      std::cerr << "Error pgeodesicdilatation: bad structuring element size" << halfsize << std::endl;
      return FAILURE;
   }
   if ( ims.Size() != imq.Size()) {
      std::cerr << "Error pdilatationreconstruction: Incompatible image size." << std::endl;
      return FAILURE;
   }

   StructuringElement2d *se = new StructuringElement2d(numSE, halfsize);

   Point2d p, p1;
   Fifo<Point2d> toBeVisited[2];

   imd = ims;
   for (p.y = 0; p.y < ims.Height(); p.y++) {
      for (p.x = 0; p.x < ims.Width(); p.x++) {
	 if ((imq[p])) {
	    Reg2d::ValueType max = Limits<Reg2d::ValueType>::min(), val;
	    for (int i = 0; i < se->length; i++) {
	       p1 = p;
	       se->Move(i, p1);
	       if (ims.Hold(p1) && ((val = ims[p1]) > max) && (imq[p1] == imq[p])) {
		  max = val;
	       }
	    }
	    if (ims[p] < max) {
	       imd[p] = max;
	       toBeVisited[0].Push(p);
	    }
	 }
      }
   }
   int fifo = 0;
   if (iteration < 0) {
      iteration = Limits<Long>::max();
   }
   iteration--;
   while (iteration > 0 && !toBeVisited[fifo].Empty()) {
      iteration--;
      while (!toBeVisited[fifo].Empty()) {
	 Point2d p3 = toBeVisited[fifo].Pop();
	 for (int v = 0; v < 8; v++) {
	    p = p3 + v8[v];
	    if (imq.Hold(p) && (imq[p])) {
	       Reg2d::ValueType max = imd[p], val;
	       for (int i = 0; i < se->length; i++) {
		  p1 = p;
		  se->Move(i, p1);
		  if (imd.Hold(p1) && ((val = imd[p1]) > max) && (imq[p1] == imq[p])) {
		     max = val;
		  }
	       }
	       if (imd[p] < max) {
		  imd[p] = max;
		  toBeVisited[1 - fifo].Push(p);
	       }
	    }
	 }
      }
      fifo = 1 - fifo;
   }

   delete se;
   return SUCCESS;
}

Errc PGeodesicDilation( const Reg2d &ims, const Img2dsl &imq, Reg2d &imd, int numSE, int halfsize, int iteration ) {
   if (numSE > 8 || numSE < 0) {
      std::cerr << "Error pgeodesicdilatation: bad structuring element type: " << numSE << std::endl;
      return FAILURE;
   }
   if (halfsize < 1) {
      std::cerr << "Error pgeodesicdilatation: bad structuring element size" << halfsize << std::endl;
      return FAILURE;
   }
   if ( ims.Size() != imq.Size()) {
      std::cerr << "Error pdilatationreconstruction: Incompatible image size." << std::endl;
      return FAILURE;
   }

   StructuringElement2d *se = new StructuringElement2d(numSE, halfsize);

   Point2d p, p1;
   Fifo<Point2d> toBeVisited[2];

   imd = ims;
   for (p.y = 0; p.y < ims.Height(); p.y++) {
      for (p.x = 0; p.x < ims.Width(); p.x++) {
	 if ((imq[p])) {
	    Reg2d::ValueType max = Limits<Reg2d::ValueType>::min(), val;
	    for (int i = 0; i < se->length; i++) {
	       p1 = p;
	       se->Move(i, p1);
	       if (ims.Hold(p1) && ((val = ims[p1]) > max) && (imq[p1] == imq[p])) {
		  max = val;
	       }
	    }
	    if (ims[p] < max) {
	       imd[p] = max;
	       toBeVisited[0].Push(p);
	    }
	 }
      }
   }
   int fifo = 0;
   if (iteration < 0) {
      iteration = Limits<Long>::max();
   }
   iteration--;
   while (iteration > 0 && !toBeVisited[fifo].Empty()) {
      iteration--;
      while (!toBeVisited[fifo].Empty()) {
	 Point2d p3 = toBeVisited[fifo].Pop();
	 for (int v = 0; v < 8; v++) {
	    p = p3 + v8[v];
	    if (imq.Hold(p) && (imq[p])) {
	       Reg2d::ValueType max = imd[p], val;
	       for (int i = 0; i < se->length; i++) {
		  p1 = p;
		  se->Move(i, p1);
		  if (imd.Hold(p1) && ((val = imd[p1]) > max) && (imq[p1] == imq[p])) {
		     max = val;
		  }
	       }
	       if (imd[p] < max) {
		  imd[p] = max;
		  toBeVisited[1 - fifo].Push(p);
	       }
	    }
	 }
      }
      fifo = 1 - fifo;
   }

   delete se;
   return SUCCESS;
}

Errc PGeodesicDilation( const Reg2d &ims, const Img2dsf &imq, Reg2d &imd, int numSE, int halfsize, int iteration ) {
   if (numSE > 8 || numSE < 0) {
      std::cerr << "Error pgeodesicdilatation: bad structuring element type: " << numSE << std::endl;
      return FAILURE;
   }
   if (halfsize < 1) {
      std::cerr << "Error pgeodesicdilatation: bad structuring element size" << halfsize << std::endl;
      return FAILURE;
   }
   if ( ims.Size() != imq.Size()) {
      std::cerr << "Error pdilatationreconstruction: Incompatible image size." << std::endl;
      return FAILURE;
   }

   StructuringElement2d *se = new StructuringElement2d(numSE, halfsize);

   Point2d p, p1;
   Fifo<Point2d> toBeVisited[2];

   imd = ims;
   for (p.y = 0; p.y < ims.Height(); p.y++) {
      for (p.x = 0; p.x < ims.Width(); p.x++) {
	 if ((imq[p])) {
	    Reg2d::ValueType max = Limits<Reg2d::ValueType>::min(), val;
	    for (int i = 0; i < se->length; i++) {
	       p1 = p;
	       se->Move(i, p1);
	       if (ims.Hold(p1) && ((val = ims[p1]) > max) && (imq[p1] == imq[p])) {
		  max = val;
	       }
	    }
	    if (ims[p] < max) {
	       imd[p] = max;
	       toBeVisited[0].Push(p);
	    }
	 }
      }
   }
   int fifo = 0;
   if (iteration < 0) {
      iteration = Limits<Long>::max();
   }
   iteration--;
   while (iteration > 0 && !toBeVisited[fifo].Empty()) {
      iteration--;
      while (!toBeVisited[fifo].Empty()) {
	 Point2d p3 = toBeVisited[fifo].Pop();
	 for (int v = 0; v < 8; v++) {
	    p = p3 + v8[v];
	    if (imq.Hold(p) && (imq[p])) {
	       Reg2d::ValueType max = imd[p], val;
	       for (int i = 0; i < se->length; i++) {
		  p1 = p;
		  se->Move(i, p1);
		  if (imd.Hold(p1) && ((val = imd[p1]) > max) && (imq[p1] == imq[p])) {
		     max = val;
		  }
	       }
	       if (imd[p] < max) {
		  imd[p] = max;
		  toBeVisited[1 - fifo].Push(p);
	       }
	    }
	 }
      }
      fifo = 1 - fifo;
   }

   delete se;
   return SUCCESS;
}


Errc PGeodesicDilation( const Img3duc &ims, const Img3duc &imq, Img3duc &imd, int numSE, int halfsize, int iteration ) {
   if (numSE > 12 || numSE < 0) {
      std::cerr << "Error pgeodesicdilatation: bad structuring element type: " << numSE << std::endl;
      return FAILURE;
   }
   if (halfsize < 1) {
      std::cerr << "Error pgeodesicdilatation: bad structuring element size: " << halfsize << std::endl;
      return FAILURE;
   }
   if ( ims.Size() != imq.Size()) {
      std::cerr << "Error pdilatationreconstruction: Incompatible image size." << std::endl;
      return FAILURE;
   }

   Point3d p, p1;
   Fifo<Point3d> toBeVisited[2];

   StructuringElement3d *se = new StructuringElement3d(numSE, halfsize);

   imd = ims;
   for (p.z = 0; p.z < ims.Depth(); p.z++) {
      for (p.y = 0; p.y < ims.Height(); p.y++) {
	 for (p.x = 0; p.x < ims.Width(); p.x++) {
	    if ((imq[p])) {
	       Img3duc::ValueType max = Limits<Img3duc::ValueType>::min(), val;
	       for (int i = 0; i < se->length; i++) {
		  p1 = p;
		  se->Move(i, p1);
		  if (ims.Hold(p1) && ((val = ims[p1]) > max) && (imq[p1] == imq[p])) {
		     max = val;
		  }
	       }
	       if (ims[p] < max) {
		  imd[p] = max;
		  toBeVisited[0].Push(p);
	       }
	    }
	 }
      }
   }
   
   if (iteration < 0) {
      iteration = Limits<Long>::max();
   }
   iteration--;
   int fifo = 0;
   while (iteration > 0 && !toBeVisited[fifo].Empty()) {
      iteration--;
      while(!toBeVisited[fifo].Empty()) {
	 Point3d p3 = toBeVisited[fifo].Pop();
	 for (int v = 0; v < 26; v++) {
	    p = p3 + v26[v];
	    if (imq.Hold(p) && (imq[p])) {
	       Img3duc::ValueType max = Limits<Img3duc::ValueType>::min(), val;
	       for (int i = 0; i < se->length; i++) {
		  p1 = p;
		  se->Move(i, p1);
		  if (imd.Hold(p1)&& ((val = imd[p1]) > max) && (imq[p1] == imq[p])) {
		     max = val;
		  }
	       }
	       if (imd[p] < max) {
		  imd[p] = max;
		  toBeVisited[1 - fifo].Push(p);
	       }
	    }
	 }
      }
      fifo = 1 - fifo;
   }
   
   delete se;
   return SUCCESS;
}

Errc PGeodesicDilation( const Img3dsl &ims, const Img3duc &imq, Img3dsl &imd, int numSE, int halfsize, int iteration ) {
   if (numSE > 12 || numSE < 0) {
      std::cerr << "Error pgeodesicdilatation: bad structuring element type: " << numSE << std::endl;
      return FAILURE;
   }
   if (halfsize < 1) {
      std::cerr << "Error pgeodesicdilatation: bad structuring element size: " << halfsize << std::endl;
      return FAILURE;
   }
   if ( ims.Size() != imq.Size()) {
      std::cerr << "Error pdilatationreconstruction: Incompatible image size." << std::endl;
      return FAILURE;
   }

   Point3d p, p1;
   Fifo<Point3d> toBeVisited[2];

   StructuringElement3d *se = new StructuringElement3d(numSE, halfsize);

   imd = ims;
   for (p.z = 0; p.z < ims.Depth(); p.z++) {
      for (p.y = 0; p.y < ims.Height(); p.y++) {
	 for (p.x = 0; p.x < ims.Width(); p.x++) {
	    if ((imq[p])) {
	       Img3dsl::ValueType max = Limits<Img3dsl::ValueType>::min(), val;
	       for (int i = 0; i < se->length; i++) {
		  p1 = p;
		  se->Move(i, p1);
		  if (ims.Hold(p1) && ((val = ims[p1]) > max) && (imq[p1] == imq[p])) {
		     max = val;
		  }
	       }
	       if (ims[p] < max) {
		  imd[p] = max;
		  toBeVisited[0].Push(p);
	       }
	    }
	 }
      }
   }
   
   if (iteration < 0) {
      iteration = Limits<Long>::max();
   }
   iteration--;
   int fifo = 0;
   while (iteration > 0 && !toBeVisited[fifo].Empty()) {
      iteration--;
      while(!toBeVisited[fifo].Empty()) {
	 Point3d p3 = toBeVisited[fifo].Pop();
	 for (int v = 0; v < 26; v++) {
	    p = p3 + v26[v];
	    if (imq.Hold(p) && (imq[p])) {
	       Img3dsl::ValueType max = Limits<Img3dsl::ValueType>::min(), val;
	       for (int i = 0; i < se->length; i++) {
		  p1 = p;
		  se->Move(i, p1);
		  if (imd.Hold(p1)&& ((val = imd[p1]) > max) && (imq[p1] == imq[p])) {
		     max = val;
		  }
	       }
	       if (imd[p] < max) {
		  imd[p] = max;
		  toBeVisited[1 - fifo].Push(p);
	       }
	    }
	 }
      }
      fifo = 1 - fifo;
   }
   
   delete se;
   return SUCCESS;
}

Errc PGeodesicDilation( const Img3dsf &ims, const Img3duc &imq, Img3dsf &imd, int numSE, int halfsize, int iteration ) {
   if (numSE > 12 || numSE < 0) {
      std::cerr << "Error pgeodesicdilatation: bad structuring element type: " << numSE << std::endl;
      return FAILURE;
   }
   if (halfsize < 1) {
      std::cerr << "Error pgeodesicdilatation: bad structuring element size: " << halfsize << std::endl;
      return FAILURE;
   }
   if ( ims.Size() != imq.Size()) {
      std::cerr << "Error pdilatationreconstruction: Incompatible image size." << std::endl;
      return FAILURE;
   }

   Point3d p, p1;
   Fifo<Point3d> toBeVisited[2];

   StructuringElement3d *se = new StructuringElement3d(numSE, halfsize);

   imd = ims;
   for (p.z = 0; p.z < ims.Depth(); p.z++) {
      for (p.y = 0; p.y < ims.Height(); p.y++) {
	 for (p.x = 0; p.x < ims.Width(); p.x++) {
	    if ((imq[p])) {
	       Img3dsf::ValueType max = Limits<Img3dsf::ValueType>::min(), val;
	       for (int i = 0; i < se->length; i++) {
		  p1 = p;
		  se->Move(i, p1);
		  if (ims.Hold(p1) && ((val = ims[p1]) > max) && (imq[p1] == imq[p])) {
		     max = val;
		  }
	       }
	       if (ims[p] < max) {
		  imd[p] = max;
		  toBeVisited[0].Push(p);
	       }
	    }
	 }
      }
   }
   
   if (iteration < 0) {
      iteration = Limits<Long>::max();
   }
   iteration--;
   int fifo = 0;
   while (iteration > 0 && !toBeVisited[fifo].Empty()) {
      iteration--;
      while(!toBeVisited[fifo].Empty()) {
	 Point3d p3 = toBeVisited[fifo].Pop();
	 for (int v = 0; v < 26; v++) {
	    p = p3 + v26[v];
	    if (imq.Hold(p) && (imq[p])) {
	       Img3dsf::ValueType max = Limits<Img3dsf::ValueType>::min(), val;
	       for (int i = 0; i < se->length; i++) {
		  p1 = p;
		  se->Move(i, p1);
		  if (imd.Hold(p1)&& ((val = imd[p1]) > max) && (imq[p1] == imq[p])) {
		     max = val;
		  }
	       }
	       if (imd[p] < max) {
		  imd[p] = max;
		  toBeVisited[1 - fifo].Push(p);
	       }
	    }
	 }
      }
      fifo = 1 - fifo;
   }
   
   delete se;
   return SUCCESS;
}

Errc PGeodesicDilation( const Img3duc &ims, const Reg3d &imq, Img3duc &imd, int numSE, int halfsize, int iteration ) {
   if (numSE > 12 || numSE < 0) {
      std::cerr << "Error pgeodesicdilatation: bad structuring element type: " << numSE << std::endl;
      return FAILURE;
   }
   if (halfsize < 1) {
      std::cerr << "Error pgeodesicdilatation: bad structuring element size: " << halfsize << std::endl;
      return FAILURE;
   }
   if ( ims.Size() != imq.Size()) {
      std::cerr << "Error pdilatationreconstruction: Incompatible image size." << std::endl;
      return FAILURE;
   }

   Point3d p, p1;
   Fifo<Point3d> toBeVisited[2];

   StructuringElement3d *se = new StructuringElement3d(numSE, halfsize);

   imd = ims;
   for (p.z = 0; p.z < ims.Depth(); p.z++) {
      for (p.y = 0; p.y < ims.Height(); p.y++) {
	 for (p.x = 0; p.x < ims.Width(); p.x++) {
	    if ((imq[p])) {
	       Img3duc::ValueType max = Limits<Img3duc::ValueType>::min(), val;
	       for (int i = 0; i < se->length; i++) {
		  p1 = p;
		  se->Move(i, p1);
		  if (ims.Hold(p1) && ((val = ims[p1]) > max) && (imq[p1] == imq[p])) {
		     max = val;
		  }
	       }
	       if (ims[p] < max) {
		  imd[p] = max;
		  toBeVisited[0].Push(p);
	       }
	    }
	 }
      }
   }
   
   if (iteration < 0) {
      iteration = Limits<Long>::max();
   }
   iteration--;
   int fifo = 0;
   while (iteration > 0 && !toBeVisited[fifo].Empty()) {
      iteration--;
      while(!toBeVisited[fifo].Empty()) {
	 Point3d p3 = toBeVisited[fifo].Pop();
	 for (int v = 0; v < 26; v++) {
	    p = p3 + v26[v];
	    if (imq.Hold(p) && (imq[p])) {
	       Img3duc::ValueType max = Limits<Img3duc::ValueType>::min(), val;
	       for (int i = 0; i < se->length; i++) {
		  p1 = p;
		  se->Move(i, p1);
		  if (imd.Hold(p1)&& ((val = imd[p1]) > max) && (imq[p1] == imq[p])) {
		     max = val;
		  }
	       }
	       if (imd[p] < max) {
		  imd[p] = max;
		  toBeVisited[1 - fifo].Push(p);
	       }
	    }
	 }
      }
      fifo = 1 - fifo;
   }
   
   delete se;
   return SUCCESS;
}

Errc PGeodesicDilation( const Img3dsl &ims, const Reg3d &imq, Img3dsl &imd, int numSE, int halfsize, int iteration ) {
   if (numSE > 12 || numSE < 0) {
      std::cerr << "Error pgeodesicdilatation: bad structuring element type: " << numSE << std::endl;
      return FAILURE;
   }
   if (halfsize < 1) {
      std::cerr << "Error pgeodesicdilatation: bad structuring element size: " << halfsize << std::endl;
      return FAILURE;
   }
   if ( ims.Size() != imq.Size()) {
      std::cerr << "Error pdilatationreconstruction: Incompatible image size." << std::endl;
      return FAILURE;
   }

   Point3d p, p1;
   Fifo<Point3d> toBeVisited[2];

   StructuringElement3d *se = new StructuringElement3d(numSE, halfsize);

   imd = ims;
   for (p.z = 0; p.z < ims.Depth(); p.z++) {
      for (p.y = 0; p.y < ims.Height(); p.y++) {
	 for (p.x = 0; p.x < ims.Width(); p.x++) {
	    if ((imq[p])) {
	       Img3dsl::ValueType max = Limits<Img3dsl::ValueType>::min(), val;
	       for (int i = 0; i < se->length; i++) {
		  p1 = p;
		  se->Move(i, p1);
		  if (ims.Hold(p1) && ((val = ims[p1]) > max) && (imq[p1] == imq[p])) {
		     max = val;
		  }
	       }
	       if (ims[p] < max) {
		  imd[p] = max;
		  toBeVisited[0].Push(p);
	       }
	    }
	 }
      }
   }
   
   if (iteration < 0) {
      iteration = Limits<Long>::max();
   }
   iteration--;
   int fifo = 0;
   while (iteration > 0 && !toBeVisited[fifo].Empty()) {
      iteration--;
      while(!toBeVisited[fifo].Empty()) {
	 Point3d p3 = toBeVisited[fifo].Pop();
	 for (int v = 0; v < 26; v++) {
	    p = p3 + v26[v];
	    if (imq.Hold(p) && (imq[p])) {
	       Img3dsl::ValueType max = Limits<Img3dsl::ValueType>::min(), val;
	       for (int i = 0; i < se->length; i++) {
		  p1 = p;
		  se->Move(i, p1);
		  if (imd.Hold(p1)&& ((val = imd[p1]) > max) && (imq[p1] == imq[p])) {
		     max = val;
		  }
	       }
	       if (imd[p] < max) {
		  imd[p] = max;
		  toBeVisited[1 - fifo].Push(p);
	       }
	    }
	 }
      }
      fifo = 1 - fifo;
   }
   
   delete se;
   return SUCCESS;
}

Errc PGeodesicDilation( const Img3dsf &ims, const Reg3d &imq, Img3dsf &imd, int numSE, int halfsize, int iteration ) {
   if (numSE > 12 || numSE < 0) {
      std::cerr << "Error pgeodesicdilatation: bad structuring element type: " << numSE << std::endl;
      return FAILURE;
   }
   if (halfsize < 1) {
      std::cerr << "Error pgeodesicdilatation: bad structuring element size: " << halfsize << std::endl;
      return FAILURE;
   }
   if ( ims.Size() != imq.Size()) {
      std::cerr << "Error pdilatationreconstruction: Incompatible image size." << std::endl;
      return FAILURE;
   }

   Point3d p, p1;
   Fifo<Point3d> toBeVisited[2];

   StructuringElement3d *se = new StructuringElement3d(numSE, halfsize);

   imd = ims;
   for (p.z = 0; p.z < ims.Depth(); p.z++) {
      for (p.y = 0; p.y < ims.Height(); p.y++) {
	 for (p.x = 0; p.x < ims.Width(); p.x++) {
	    if ((imq[p])) {
	       Img3dsf::ValueType max = Limits<Img3dsf::ValueType>::min(), val;
	       for (int i = 0; i < se->length; i++) {
		  p1 = p;
		  se->Move(i, p1);
		  if (ims.Hold(p1) && ((val = ims[p1]) > max) && (imq[p1] == imq[p])) {
		     max = val;
		  }
	       }
	       if (ims[p] < max) {
		  imd[p] = max;
		  toBeVisited[0].Push(p);
	       }
	    }
	 }
      }
   }
   
   if (iteration < 0) {
      iteration = Limits<Long>::max();
   }
   iteration--;
   int fifo = 0;
   while (iteration > 0 && !toBeVisited[fifo].Empty()) {
      iteration--;
      while(!toBeVisited[fifo].Empty()) {
	 Point3d p3 = toBeVisited[fifo].Pop();
	 for (int v = 0; v < 26; v++) {
	    p = p3 + v26[v];
	    if (imq.Hold(p) && (imq[p])) {
	       Img3dsf::ValueType max = Limits<Img3dsf::ValueType>::min(), val;
	       for (int i = 0; i < se->length; i++) {
		  p1 = p;
		  se->Move(i, p1);
		  if (imd.Hold(p1)&& ((val = imd[p1]) > max) && (imq[p1] == imq[p])) {
		     max = val;
		  }
	       }
	       if (imd[p] < max) {
		  imd[p] = max;
		  toBeVisited[1 - fifo].Push(p);
	       }
	    }
	 }
      }
      fifo = 1 - fifo;
   }
   
   delete se;
   return SUCCESS;
}

Errc PGeodesicDilation( const Reg3d &ims, const Reg3d &imq, Reg3d &imd, int numSE, int halfsize, int iteration ) {
   if (numSE > 12 || numSE < 0) {
      std::cerr << "Error pgeodesicdilatation: bad structuring element type: " << numSE << std::endl;
      return FAILURE;
   }
   if (halfsize < 1) {
      std::cerr << "Error pgeodesicdilatation: bad structuring element size: " << halfsize << std::endl;
      return FAILURE;
   }
   if ( ims.Size() != imq.Size()) {
      std::cerr << "Error pdilatationreconstruction: Incompatible image size." << std::endl;
      return FAILURE;
   }

   Point3d p, p1;
   Fifo<Point3d> toBeVisited[2];

   StructuringElement3d *se = new StructuringElement3d(numSE, halfsize);

   imd = ims;
   for (p.z = 0; p.z < ims.Depth(); p.z++) {
      for (p.y = 0; p.y < ims.Height(); p.y++) {
	 for (p.x = 0; p.x < ims.Width(); p.x++) {
	    if ((imq[p])) {
	       Reg3d::ValueType max = Limits<Reg3d::ValueType>::min(), val;
	       for (int i = 0; i < se->length; i++) {
		  p1 = p;
		  se->Move(i, p1);
		  if (ims.Hold(p1) && ((val = ims[p1]) > max) && (imq[p1] == imq[p])) {
		     max = val;
		  }
	       }
	       if (ims[p] < max) {
		  imd[p] = max;
		  toBeVisited[0].Push(p);
	       }
	    }
	 }
      }
   }
   
   if (iteration < 0) {
      iteration = Limits<Long>::max();
   }
   iteration--;
   int fifo = 0;
   while (iteration > 0 && !toBeVisited[fifo].Empty()) {
      iteration--;
      while(!toBeVisited[fifo].Empty()) {
	 Point3d p3 = toBeVisited[fifo].Pop();
	 for (int v = 0; v < 26; v++) {
	    p = p3 + v26[v];
	    if (imq.Hold(p) && (imq[p])) {
	       Reg3d::ValueType max = Limits<Reg3d::ValueType>::min(), val;
	       for (int i = 0; i < se->length; i++) {
		  p1 = p;
		  se->Move(i, p1);
		  if (imd.Hold(p1)&& ((val = imd[p1]) > max) && (imq[p1] == imq[p])) {
		     max = val;
		  }
	       }
	       if (imd[p] < max) {
		  imd[p] = max;
		  toBeVisited[1 - fifo].Push(p);
	       }
	    }
	 }
      }
      fifo = 1 - fifo;
   }
   
   delete se;
   return SUCCESS;
}

Errc PGeodesicDilation( const Reg3d &ims, const Img3duc &imq, Reg3d &imd, int numSE, int halfsize, int iteration ) {
   if (numSE > 12 || numSE < 0) {
      std::cerr << "Error pgeodesicdilatation: bad structuring element type: " << numSE << std::endl;
      return FAILURE;
   }
   if (halfsize < 1) {
      std::cerr << "Error pgeodesicdilatation: bad structuring element size: " << halfsize << std::endl;
      return FAILURE;
   }
   if ( ims.Size() != imq.Size()) {
      std::cerr << "Error pdilatationreconstruction: Incompatible image size." << std::endl;
      return FAILURE;
   }

   Point3d p, p1;
   Fifo<Point3d> toBeVisited[2];

   StructuringElement3d *se = new StructuringElement3d(numSE, halfsize);

   imd = ims;
   for (p.z = 0; p.z < ims.Depth(); p.z++) {
      for (p.y = 0; p.y < ims.Height(); p.y++) {
	 for (p.x = 0; p.x < ims.Width(); p.x++) {
	    if ((imq[p])) {
	       Reg3d::ValueType max = Limits<Reg3d::ValueType>::min(), val;
	       for (int i = 0; i < se->length; i++) {
		  p1 = p;
		  se->Move(i, p1);
		  if (ims.Hold(p1) && ((val = ims[p1]) > max) && (imq[p1] == imq[p])) {
		     max = val;
		  }
	       }
	       if (ims[p] < max) {
		  imd[p] = max;
		  toBeVisited[0].Push(p);
	       }
	    }
	 }
      }
   }
   
   if (iteration < 0) {
      iteration = Limits<Long>::max();
   }
   iteration--;
   int fifo = 0;
   while (iteration > 0 && !toBeVisited[fifo].Empty()) {
      iteration--;
      while(!toBeVisited[fifo].Empty()) {
	 Point3d p3 = toBeVisited[fifo].Pop();
	 for (int v = 0; v < 26; v++) {
	    p = p3 + v26[v];
	    if (imq.Hold(p) && (imq[p])) {
	       Reg3d::ValueType max = Limits<Reg3d::ValueType>::min(), val;
	       for (int i = 0; i < se->length; i++) {
		  p1 = p;
		  se->Move(i, p1);
		  if (imd.Hold(p1)&& ((val = imd[p1]) > max) && (imq[p1] == imq[p])) {
		     max = val;
		  }
	       }
	       if (imd[p] < max) {
		  imd[p] = max;
		  toBeVisited[1 - fifo].Push(p);
	       }
	    }
	 }
      }
      fifo = 1 - fifo;
   }
   
   delete se;
   return SUCCESS;
}

Errc PGeodesicDilation( const Reg3d &ims, const Img3dsl &imq, Reg3d &imd, int numSE, int halfsize, int iteration ) {
   if (numSE > 12 || numSE < 0) {
      std::cerr << "Error pgeodesicdilatation: bad structuring element type: " << numSE << std::endl;
      return FAILURE;
   }
   if (halfsize < 1) {
      std::cerr << "Error pgeodesicdilatation: bad structuring element size: " << halfsize << std::endl;
      return FAILURE;
   }
   if ( ims.Size() != imq.Size()) {
      std::cerr << "Error pdilatationreconstruction: Incompatible image size." << std::endl;
      return FAILURE;
   }

   Point3d p, p1;
   Fifo<Point3d> toBeVisited[2];

   StructuringElement3d *se = new StructuringElement3d(numSE, halfsize);

   imd = ims;
   for (p.z = 0; p.z < ims.Depth(); p.z++) {
      for (p.y = 0; p.y < ims.Height(); p.y++) {
	 for (p.x = 0; p.x < ims.Width(); p.x++) {
	    if ((imq[p])) {
	       Reg3d::ValueType max = Limits<Reg3d::ValueType>::min(), val;
	       for (int i = 0; i < se->length; i++) {
		  p1 = p;
		  se->Move(i, p1);
		  if (ims.Hold(p1) && ((val = ims[p1]) > max) && (imq[p1] == imq[p])) {
		     max = val;
		  }
	       }
	       if (ims[p] < max) {
		  imd[p] = max;
		  toBeVisited[0].Push(p);
	       }
	    }
	 }
      }
   }
   
   if (iteration < 0) {
      iteration = Limits<Long>::max();
   }
   iteration--;
   int fifo = 0;
   while (iteration > 0 && !toBeVisited[fifo].Empty()) {
      iteration--;
      while(!toBeVisited[fifo].Empty()) {
	 Point3d p3 = toBeVisited[fifo].Pop();
	 for (int v = 0; v < 26; v++) {
	    p = p3 + v26[v];
	    if (imq.Hold(p) && (imq[p])) {
	       Reg3d::ValueType max = Limits<Reg3d::ValueType>::min(), val;
	       for (int i = 0; i < se->length; i++) {
		  p1 = p;
		  se->Move(i, p1);
		  if (imd.Hold(p1)&& ((val = imd[p1]) > max) && (imq[p1] == imq[p])) {
		     max = val;
		  }
	       }
	       if (imd[p] < max) {
		  imd[p] = max;
		  toBeVisited[1 - fifo].Push(p);
	       }
	    }
	 }
      }
      fifo = 1 - fifo;
   }
   
   delete se;
   return SUCCESS;
}

Errc PGeodesicDilation( const Reg3d &ims, const Img3dsf &imq, Reg3d &imd, int numSE, int halfsize, int iteration ) {
   if (numSE > 12 || numSE < 0) {
      std::cerr << "Error pgeodesicdilatation: bad structuring element type: " << numSE << std::endl;
      return FAILURE;
   }
   if (halfsize < 1) {
      std::cerr << "Error pgeodesicdilatation: bad structuring element size: " << halfsize << std::endl;
      return FAILURE;
   }
   if ( ims.Size() != imq.Size()) {
      std::cerr << "Error pdilatationreconstruction: Incompatible image size." << std::endl;
      return FAILURE;
   }

   Point3d p, p1;
   Fifo<Point3d> toBeVisited[2];

   StructuringElement3d *se = new StructuringElement3d(numSE, halfsize);

   imd = ims;
   for (p.z = 0; p.z < ims.Depth(); p.z++) {
      for (p.y = 0; p.y < ims.Height(); p.y++) {
	 for (p.x = 0; p.x < ims.Width(); p.x++) {
	    if ((imq[p])) {
	       Reg3d::ValueType max = Limits<Reg3d::ValueType>::min(), val;
	       for (int i = 0; i < se->length; i++) {
		  p1 = p;
		  se->Move(i, p1);
		  if (ims.Hold(p1) && ((val = ims[p1]) > max) && (imq[p1] == imq[p])) {
		     max = val;
		  }
	       }
	       if (ims[p] < max) {
		  imd[p] = max;
		  toBeVisited[0].Push(p);
	       }
	    }
	 }
      }
   }
   
   if (iteration < 0) {
      iteration = Limits<Long>::max();
   }
   iteration--;
   int fifo = 0;
   while (iteration > 0 && !toBeVisited[fifo].Empty()) {
      iteration--;
      while(!toBeVisited[fifo].Empty()) {
	 Point3d p3 = toBeVisited[fifo].Pop();
	 for (int v = 0; v < 26; v++) {
	    p = p3 + v26[v];
	    if (imq.Hold(p) && (imq[p])) {
	       Reg3d::ValueType max = Limits<Reg3d::ValueType>::min(), val;
	       for (int i = 0; i < se->length; i++) {
		  p1 = p;
		  se->Move(i, p1);
		  if (imd.Hold(p1)&& ((val = imd[p1]) > max) && (imq[p1] == imq[p])) {
		     max = val;
		  }
	       }
	       if (imd[p] < max) {
		  imd[p] = max;
		  toBeVisited[1 - fifo].Push(p);
	       }
	    }
	 }
      }
      fifo = 1 - fifo;
   }
   
   delete se;
   return SUCCESS;
}


Errc PGeodesicDilation( const Imc2duc &ims, Img2duc &imq, Imc2duc &imd, int numSE, int halfsize, int iteration ) {
   if (numSE > 8 || numSE < 0) {
      std::cerr << "Error pgeodesicdilatation: bad structuring element type: " << numSE << std::endl;
      return FAILURE;
   }
   if (halfsize < 1) {
      std::cerr << "Error pgeodesicdilatation: bad structuring element size: " << halfsize << std::endl;
      return FAILURE;
   }
   if ( ims.Size() != imq.Size()) {
      std::cerr << "Error pdilatationreconstruction: Incompatible image size." << std::endl;
      return FAILURE;
   }

   Point2d p, p1;
   Fifo<Point2d> toBeVisited[2];

   StructuringElement2d *se = new StructuringElement2d(numSE, halfsize);

   imd = ims;
   for (p.y = 0; p.y < ims.Height(); p.y++) {
      for (p.x = 0; p.x < ims.Width(); p.x++) {
	 if ((imq[p])) {
	    Imc2duc::ValueType maxX = Limits<Imc2duc::ValueType>::min();
	    Imc2duc::ValueType maxY = Limits<Imc2duc::ValueType>::min();
	    Imc2duc::ValueType maxZ = Limits<Imc2duc::ValueType>::min();
	    for (int i = 0; i < se->length; i++) {
	       p1 = p;
	       se->Move(i, p1);
	       if (ims.Hold(p1)
		   && Greater(ims.X[p1], ims.Y[p1], ims.Z[p1], maxX, maxY, maxZ)
		   && (imq[p1] == imq[p])) {
		  maxX = ims.X[p1];
		  maxY = ims.Y[p1];
		  maxZ = ims.Z[p1];
	       }
	    }
	    if (ims.X[p] != maxX|| ims.Y[p] != maxY || ims.Z[p] != maxZ ) {
	       imd.X[p] = maxX;
	       imd.Y[p] = maxY;
	       imd.Z[p] = maxZ;
	       toBeVisited[0].Push(p);
	    }
	 }
      }
   }

   if (iteration < 0) {
      iteration = Limits<Long>::max();
   }
   iteration--;
   int fifo = 0;
   while (iteration > 0 && !toBeVisited[fifo].Empty()) {
      iteration--;
      while (!toBeVisited[fifo].Empty()) {
	 Point2d p3 = toBeVisited[fifo].Pop();
	 for (int v = 0; v < 8; v++) {
	    p = p3 + v8[v];
	    if (imq.Hold(p) && (imq[p])) {
	       Imc2duc::ValueType maxX = Limits<Imc2duc::ValueType>::min();
	       Imc2duc::ValueType maxY = Limits<Imc2duc::ValueType>::min();
	       Imc2duc::ValueType maxZ = Limits<Imc2duc::ValueType>::min();
	       for (int i = 0; i < se->length; i++) {
		  p1 = p;
		  se->Move(i, p1);
		  if (imd.Hold(p1)
		      && Greater(imd.X[p1], imd.Y[p1], imd.Z[p1], maxX, maxY, maxZ)
		      && (imq[p1] == imq[p])) {
		     maxX = imd.X[p1];
		     maxY = imd.Y[p1];
		     maxZ = imd.Z[p1];
		  }
	       }
	       if (imd.X[p] != maxX || imd.Y[p] != maxY || imd.Z[p] != maxZ) {
		  imd.X[p] = maxX;
		  imd.Y[p] = maxY;
		  imd.Z[p] = maxZ;
		  toBeVisited[1 - fifo].Push(p);
	       }
	    }
	 }
      }
      fifo = 1 - fifo;
   }

   delete se;
   return SUCCESS;
}

Errc PGeodesicDilation( const Imc2dsl &ims, Img2duc &imq, Imc2dsl &imd, int numSE, int halfsize, int iteration ) {
   if (numSE > 8 || numSE < 0) {
      std::cerr << "Error pgeodesicdilatation: bad structuring element type: " << numSE << std::endl;
      return FAILURE;
   }
   if (halfsize < 1) {
      std::cerr << "Error pgeodesicdilatation: bad structuring element size: " << halfsize << std::endl;
      return FAILURE;
   }
   if ( ims.Size() != imq.Size()) {
      std::cerr << "Error pdilatationreconstruction: Incompatible image size." << std::endl;
      return FAILURE;
   }

   Point2d p, p1;
   Fifo<Point2d> toBeVisited[2];

   StructuringElement2d *se = new StructuringElement2d(numSE, halfsize);

   imd = ims;
   for (p.y = 0; p.y < ims.Height(); p.y++) {
      for (p.x = 0; p.x < ims.Width(); p.x++) {
	 if ((imq[p])) {
	    Imc2dsl::ValueType maxX = Limits<Imc2dsl::ValueType>::min();
	    Imc2dsl::ValueType maxY = Limits<Imc2dsl::ValueType>::min();
	    Imc2dsl::ValueType maxZ = Limits<Imc2dsl::ValueType>::min();
	    for (int i = 0; i < se->length; i++) {
	       p1 = p;
	       se->Move(i, p1);
	       if (ims.Hold(p1)
		   && Greater(ims.X[p1], ims.Y[p1], ims.Z[p1], maxX, maxY, maxZ)
		   && (imq[p1] == imq[p])) {
		  maxX = ims.X[p1];
		  maxY = ims.Y[p1];
		  maxZ = ims.Z[p1];
	       }
	    }
	    if (ims.X[p] != maxX|| ims.Y[p] != maxY || ims.Z[p] != maxZ ) {
	       imd.X[p] = maxX;
	       imd.Y[p] = maxY;
	       imd.Z[p] = maxZ;
	       toBeVisited[0].Push(p);
	    }
	 }
      }
   }

   if (iteration < 0) {
      iteration = Limits<Long>::max();
   }
   iteration--;
   int fifo = 0;
   while (iteration > 0 && !toBeVisited[fifo].Empty()) {
      iteration--;
      while (!toBeVisited[fifo].Empty()) {
	 Point2d p3 = toBeVisited[fifo].Pop();
	 for (int v = 0; v < 8; v++) {
	    p = p3 + v8[v];
	    if (imq.Hold(p) && (imq[p])) {
	       Imc2dsl::ValueType maxX = Limits<Imc2dsl::ValueType>::min();
	       Imc2dsl::ValueType maxY = Limits<Imc2dsl::ValueType>::min();
	       Imc2dsl::ValueType maxZ = Limits<Imc2dsl::ValueType>::min();
	       for (int i = 0; i < se->length; i++) {
		  p1 = p;
		  se->Move(i, p1);
		  if (imd.Hold(p1)
		      && Greater(imd.X[p1], imd.Y[p1], imd.Z[p1], maxX, maxY, maxZ)
		      && (imq[p1] == imq[p])) {
		     maxX = imd.X[p1];
		     maxY = imd.Y[p1];
		     maxZ = imd.Z[p1];
		  }
	       }
	       if (imd.X[p] != maxX || imd.Y[p] != maxY || imd.Z[p] != maxZ) {
		  imd.X[p] = maxX;
		  imd.Y[p] = maxY;
		  imd.Z[p] = maxZ;
		  toBeVisited[1 - fifo].Push(p);
	       }
	    }
	 }
      }
      fifo = 1 - fifo;
   }

   delete se;
   return SUCCESS;
}

Errc PGeodesicDilation( const Imc2dsf &ims, Img2duc &imq, Imc2dsf &imd, int numSE, int halfsize, int iteration ) {
   if (numSE > 8 || numSE < 0) {
      std::cerr << "Error pgeodesicdilatation: bad structuring element type: " << numSE << std::endl;
      return FAILURE;
   }
   if (halfsize < 1) {
      std::cerr << "Error pgeodesicdilatation: bad structuring element size: " << halfsize << std::endl;
      return FAILURE;
   }
   if ( ims.Size() != imq.Size()) {
      std::cerr << "Error pdilatationreconstruction: Incompatible image size." << std::endl;
      return FAILURE;
   }

   Point2d p, p1;
   Fifo<Point2d> toBeVisited[2];

   StructuringElement2d *se = new StructuringElement2d(numSE, halfsize);

   imd = ims;
   for (p.y = 0; p.y < ims.Height(); p.y++) {
      for (p.x = 0; p.x < ims.Width(); p.x++) {
	 if ((imq[p])) {
	    Imc2dsf::ValueType maxX = Limits<Imc2dsf::ValueType>::min();
	    Imc2dsf::ValueType maxY = Limits<Imc2dsf::ValueType>::min();
	    Imc2dsf::ValueType maxZ = Limits<Imc2dsf::ValueType>::min();
	    for (int i = 0; i < se->length; i++) {
	       p1 = p;
	       se->Move(i, p1);
	       if (ims.Hold(p1)
		   && Greater(ims.X[p1], ims.Y[p1], ims.Z[p1], maxX, maxY, maxZ)
		   && (imq[p1] == imq[p])) {
		  maxX = ims.X[p1];
		  maxY = ims.Y[p1];
		  maxZ = ims.Z[p1];
	       }
	    }
	    if (ims.X[p] != maxX|| ims.Y[p] != maxY || ims.Z[p] != maxZ ) {
	       imd.X[p] = maxX;
	       imd.Y[p] = maxY;
	       imd.Z[p] = maxZ;
	       toBeVisited[0].Push(p);
	    }
	 }
      }
   }

   if (iteration < 0) {
      iteration = Limits<Long>::max();
   }
   iteration--;
   int fifo = 0;
   while (iteration > 0 && !toBeVisited[fifo].Empty()) {
      iteration--;
      while (!toBeVisited[fifo].Empty()) {
	 Point2d p3 = toBeVisited[fifo].Pop();
	 for (int v = 0; v < 8; v++) {
	    p = p3 + v8[v];
	    if (imq.Hold(p) && (imq[p])) {
	       Imc2dsf::ValueType maxX = Limits<Imc2dsf::ValueType>::min();
	       Imc2dsf::ValueType maxY = Limits<Imc2dsf::ValueType>::min();
	       Imc2dsf::ValueType maxZ = Limits<Imc2dsf::ValueType>::min();
	       for (int i = 0; i < se->length; i++) {
		  p1 = p;
		  se->Move(i, p1);
		  if (imd.Hold(p1)
		      && Greater(imd.X[p1], imd.Y[p1], imd.Z[p1], maxX, maxY, maxZ)
		      && (imq[p1] == imq[p])) {
		     maxX = imd.X[p1];
		     maxY = imd.Y[p1];
		     maxZ = imd.Z[p1];
		  }
	       }
	       if (imd.X[p] != maxX || imd.Y[p] != maxY || imd.Z[p] != maxZ) {
		  imd.X[p] = maxX;
		  imd.Y[p] = maxY;
		  imd.Z[p] = maxZ;
		  toBeVisited[1 - fifo].Push(p);
	       }
	    }
	 }
      }
      fifo = 1 - fifo;
   }

   delete se;
   return SUCCESS;
}

Errc PGeodesicDilation( const Imc2duc &ims, Reg2d &imq, Imc2duc &imd, int numSE, int halfsize, int iteration ) {
   if (numSE > 8 || numSE < 0) {
      std::cerr << "Error pgeodesicdilatation: bad structuring element type: " << numSE << std::endl;
      return FAILURE;
   }
   if (halfsize < 1) {
      std::cerr << "Error pgeodesicdilatation: bad structuring element size: " << halfsize << std::endl;
      return FAILURE;
   }
   if ( ims.Size() != imq.Size()) {
      std::cerr << "Error pdilatationreconstruction: Incompatible image size." << std::endl;
      return FAILURE;
   }

   Point2d p, p1;
   Fifo<Point2d> toBeVisited[2];

   StructuringElement2d *se = new StructuringElement2d(numSE, halfsize);

   imd = ims;
   for (p.y = 0; p.y < ims.Height(); p.y++) {
      for (p.x = 0; p.x < ims.Width(); p.x++) {
	 if ((imq[p])) {
	    Imc2duc::ValueType maxX = Limits<Imc2duc::ValueType>::min();
	    Imc2duc::ValueType maxY = Limits<Imc2duc::ValueType>::min();
	    Imc2duc::ValueType maxZ = Limits<Imc2duc::ValueType>::min();
	    for (int i = 0; i < se->length; i++) {
	       p1 = p;
	       se->Move(i, p1);
	       if (ims.Hold(p1)
		   && Greater(ims.X[p1], ims.Y[p1], ims.Z[p1], maxX, maxY, maxZ)
		   && (imq[p1] == imq[p])) {
		  maxX = ims.X[p1];
		  maxY = ims.Y[p1];
		  maxZ = ims.Z[p1];
	       }
	    }
	    if (ims.X[p] != maxX|| ims.Y[p] != maxY || ims.Z[p] != maxZ ) {
	       imd.X[p] = maxX;
	       imd.Y[p] = maxY;
	       imd.Z[p] = maxZ;
	       toBeVisited[0].Push(p);
	    }
	 }
      }
   }

   if (iteration < 0) {
      iteration = Limits<Long>::max();
   }
   iteration--;
   int fifo = 0;
   while (iteration > 0 && !toBeVisited[fifo].Empty()) {
      iteration--;
      while (!toBeVisited[fifo].Empty()) {
	 Point2d p3 = toBeVisited[fifo].Pop();
	 for (int v = 0; v < 8; v++) {
	    p = p3 + v8[v];
	    if (imq.Hold(p) && (imq[p])) {
	       Imc2duc::ValueType maxX = Limits<Imc2duc::ValueType>::min();
	       Imc2duc::ValueType maxY = Limits<Imc2duc::ValueType>::min();
	       Imc2duc::ValueType maxZ = Limits<Imc2duc::ValueType>::min();
	       for (int i = 0; i < se->length; i++) {
		  p1 = p;
		  se->Move(i, p1);
		  if (imd.Hold(p1)
		      && Greater(imd.X[p1], imd.Y[p1], imd.Z[p1], maxX, maxY, maxZ)
		      && (imq[p1] == imq[p])) {
		     maxX = imd.X[p1];
		     maxY = imd.Y[p1];
		     maxZ = imd.Z[p1];
		  }
	       }
	       if (imd.X[p] != maxX || imd.Y[p] != maxY || imd.Z[p] != maxZ) {
		  imd.X[p] = maxX;
		  imd.Y[p] = maxY;
		  imd.Z[p] = maxZ;
		  toBeVisited[1 - fifo].Push(p);
	       }
	    }
	 }
      }
      fifo = 1 - fifo;
   }

   delete se;
   return SUCCESS;
}

Errc PGeodesicDilation( const Imc2dsl &ims, Reg2d &imq, Imc2dsl &imd, int numSE, int halfsize, int iteration ) {
   if (numSE > 8 || numSE < 0) {
      std::cerr << "Error pgeodesicdilatation: bad structuring element type: " << numSE << std::endl;
      return FAILURE;
   }
   if (halfsize < 1) {
      std::cerr << "Error pgeodesicdilatation: bad structuring element size: " << halfsize << std::endl;
      return FAILURE;
   }
   if ( ims.Size() != imq.Size()) {
      std::cerr << "Error pdilatationreconstruction: Incompatible image size." << std::endl;
      return FAILURE;
   }

   Point2d p, p1;
   Fifo<Point2d> toBeVisited[2];

   StructuringElement2d *se = new StructuringElement2d(numSE, halfsize);

   imd = ims;
   for (p.y = 0; p.y < ims.Height(); p.y++) {
      for (p.x = 0; p.x < ims.Width(); p.x++) {
	 if ((imq[p])) {
	    Imc2dsl::ValueType maxX = Limits<Imc2dsl::ValueType>::min();
	    Imc2dsl::ValueType maxY = Limits<Imc2dsl::ValueType>::min();
	    Imc2dsl::ValueType maxZ = Limits<Imc2dsl::ValueType>::min();
	    for (int i = 0; i < se->length; i++) {
	       p1 = p;
	       se->Move(i, p1);
	       if (ims.Hold(p1)
		   && Greater(ims.X[p1], ims.Y[p1], ims.Z[p1], maxX, maxY, maxZ)
		   && (imq[p1] == imq[p])) {
		  maxX = ims.X[p1];
		  maxY = ims.Y[p1];
		  maxZ = ims.Z[p1];
	       }
	    }
	    if (ims.X[p] != maxX|| ims.Y[p] != maxY || ims.Z[p] != maxZ ) {
	       imd.X[p] = maxX;
	       imd.Y[p] = maxY;
	       imd.Z[p] = maxZ;
	       toBeVisited[0].Push(p);
	    }
	 }
      }
   }

   if (iteration < 0) {
      iteration = Limits<Long>::max();
   }
   iteration--;
   int fifo = 0;
   while (iteration > 0 && !toBeVisited[fifo].Empty()) {
      iteration--;
      while (!toBeVisited[fifo].Empty()) {
	 Point2d p3 = toBeVisited[fifo].Pop();
	 for (int v = 0; v < 8; v++) {
	    p = p3 + v8[v];
	    if (imq.Hold(p) && (imq[p])) {
	       Imc2dsl::ValueType maxX = Limits<Imc2dsl::ValueType>::min();
	       Imc2dsl::ValueType maxY = Limits<Imc2dsl::ValueType>::min();
	       Imc2dsl::ValueType maxZ = Limits<Imc2dsl::ValueType>::min();
	       for (int i = 0; i < se->length; i++) {
		  p1 = p;
		  se->Move(i, p1);
		  if (imd.Hold(p1)
		      && Greater(imd.X[p1], imd.Y[p1], imd.Z[p1], maxX, maxY, maxZ)
		      && (imq[p1] == imq[p])) {
		     maxX = imd.X[p1];
		     maxY = imd.Y[p1];
		     maxZ = imd.Z[p1];
		  }
	       }
	       if (imd.X[p] != maxX || imd.Y[p] != maxY || imd.Z[p] != maxZ) {
		  imd.X[p] = maxX;
		  imd.Y[p] = maxY;
		  imd.Z[p] = maxZ;
		  toBeVisited[1 - fifo].Push(p);
	       }
	    }
	 }
      }
      fifo = 1 - fifo;
   }

   delete se;
   return SUCCESS;
}

Errc PGeodesicDilation( const Imc2dsf &ims, Reg2d &imq, Imc2dsf &imd, int numSE, int halfsize, int iteration ) {
   if (numSE > 8 || numSE < 0) {
      std::cerr << "Error pgeodesicdilatation: bad structuring element type: " << numSE << std::endl;
      return FAILURE;
   }
   if (halfsize < 1) {
      std::cerr << "Error pgeodesicdilatation: bad structuring element size: " << halfsize << std::endl;
      return FAILURE;
   }
   if ( ims.Size() != imq.Size()) {
      std::cerr << "Error pdilatationreconstruction: Incompatible image size." << std::endl;
      return FAILURE;
   }

   Point2d p, p1;
   Fifo<Point2d> toBeVisited[2];

   StructuringElement2d *se = new StructuringElement2d(numSE, halfsize);

   imd = ims;
   for (p.y = 0; p.y < ims.Height(); p.y++) {
      for (p.x = 0; p.x < ims.Width(); p.x++) {
	 if ((imq[p])) {
	    Imc2dsf::ValueType maxX = Limits<Imc2dsf::ValueType>::min();
	    Imc2dsf::ValueType maxY = Limits<Imc2dsf::ValueType>::min();
	    Imc2dsf::ValueType maxZ = Limits<Imc2dsf::ValueType>::min();
	    for (int i = 0; i < se->length; i++) {
	       p1 = p;
	       se->Move(i, p1);
	       if (ims.Hold(p1)
		   && Greater(ims.X[p1], ims.Y[p1], ims.Z[p1], maxX, maxY, maxZ)
		   && (imq[p1] == imq[p])) {
		  maxX = ims.X[p1];
		  maxY = ims.Y[p1];
		  maxZ = ims.Z[p1];
	       }
	    }
	    if (ims.X[p] != maxX|| ims.Y[p] != maxY || ims.Z[p] != maxZ ) {
	       imd.X[p] = maxX;
	       imd.Y[p] = maxY;
	       imd.Z[p] = maxZ;
	       toBeVisited[0].Push(p);
	    }
	 }
      }
   }

   if (iteration < 0) {
      iteration = Limits<Long>::max();
   }
   iteration--;
   int fifo = 0;
   while (iteration > 0 && !toBeVisited[fifo].Empty()) {
      iteration--;
      while (!toBeVisited[fifo].Empty()) {
	 Point2d p3 = toBeVisited[fifo].Pop();
	 for (int v = 0; v < 8; v++) {
	    p = p3 + v8[v];
	    if (imq.Hold(p) && (imq[p])) {
	       Imc2dsf::ValueType maxX = Limits<Imc2dsf::ValueType>::min();
	       Imc2dsf::ValueType maxY = Limits<Imc2dsf::ValueType>::min();
	       Imc2dsf::ValueType maxZ = Limits<Imc2dsf::ValueType>::min();
	       for (int i = 0; i < se->length; i++) {
		  p1 = p;
		  se->Move(i, p1);
		  if (imd.Hold(p1)
		      && Greater(imd.X[p1], imd.Y[p1], imd.Z[p1], maxX, maxY, maxZ)
		      && (imq[p1] == imq[p])) {
		     maxX = imd.X[p1];
		     maxY = imd.Y[p1];
		     maxZ = imd.Z[p1];
		  }
	       }
	       if (imd.X[p] != maxX || imd.Y[p] != maxY || imd.Z[p] != maxZ) {
		  imd.X[p] = maxX;
		  imd.Y[p] = maxY;
		  imd.Z[p] = maxZ;
		  toBeVisited[1 - fifo].Push(p);
	       }
	    }
	 }
      }
      fifo = 1 - fifo;
   }

   delete se;
   return SUCCESS;
}


Errc PGeodesicDilation( const Imc3duc &ims, const Img3duc &imq, Imc3duc &imd, int numSE, int halfsize, int iteration ) {
   if (numSE > 12 || numSE < 0) {
      std::cerr << "Error pgeodesicdilatation: bad structuring element type: " << numSE << std::endl;
      return FAILURE;
   }
   if (halfsize < 1) {
      std::cerr << "Error pgeodesicdilatation: bad structuring element size: " << halfsize << std::endl;
      return FAILURE;
   }
   if (ims.Size() != imq.Size()) {
      std::cerr << "Error pdilatationreconstruction: Incompatible image size." << std::endl;
      return FAILURE;
   }

   Point3d p, p1;
   Fifo<Point3d> toBeVisited[2];

   StructuringElement3d *se = new StructuringElement3d(numSE, halfsize);

   imd = ims;
   for (p.z = 0; p.z < ims.Depth(); p.z++) {
      for (p.y = 0; p.y < ims.Height(); p.y++) {
	 for (p.x = 0; p.x < ims.Width(); p.x++) {
	    if ((imq[p])) {
	       Imc3duc::ValueType maxX = ims.X[p];
	       Imc3duc::ValueType maxY = ims.Y[p];
	       Imc3duc::ValueType maxZ = ims.Z[p];
	       for (int i = 0; i < se->length; i++) {
		  p1 = p;
		  se->Move(i, p1);
		  if (ims.Hold(p1)
		      && Greater(ims.X[p1], ims.Y[p1], ims.Z[p1], maxX, maxY, maxZ)
		      && (imq[p1] == imq[p])) {
		     maxX = ims.X[p1];
		     maxY = ims.Y[p1];
		     maxZ = ims.Z[p1];
		  }
	       }
	       if (ims.X[p] != maxX|| ims.Y[p] != maxY || ims.Z[p] != maxZ ) {
		  imd.X[p] = maxX;
		  imd.Y[p] = maxY;
		  imd.Z[p] = maxZ;
		  toBeVisited[0].Push(p);
	       }
	    }
	 }
      }
   }

   if (iteration < 0) {
      iteration = Limits<Long>::max();
   }
   iteration--;
   int fifo  =0;
   while (iteration > 0 && !toBeVisited[fifo].Empty()) {
      iteration--;
      while (!toBeVisited[fifo].Empty()) {
	 Point3d p3 = toBeVisited[fifo].Pop();
	 for (int v = 0; v < 26; v++) {
	    p = p3 + v26[v];
	    
	    if (imq.Hold(p) && (imq[p])) {
	       Imc3duc::ValueType maxX = imd.X[p];
	       Imc3duc::ValueType maxY = imd.Y[p];
	       Imc3duc::ValueType maxZ = imd.Z[p];
	       for (int i = 0; i < se->length; i++) {
		  p1 = p;
		  se->Move(i, p1);
		  if (imd.Hold(p1)
		      && Greater(imd.X[p1], imd.Y[p1], imd.Z[p1], maxX, maxY, maxZ)
		      && (imq[p1] == imq[p])) {
		     maxX = imd.X[p1];
		     maxY = imd.Y[p1];
		     maxZ = imd.Z[p1];
		  }
	       }
	       if (imd.X[p] != maxX || imd.Y[p] != maxY || imd.Z[p] != maxZ) {
		  imd.X[p] = maxX;
		  imd.Y[p] = maxY;
		  imd.Z[p] = maxZ;
		  toBeVisited[1 - fifo].Push(p);
	       }
	    }
	 }
      }
      fifo = 1 - fifo;
   }
   
   delete se;
   return SUCCESS;
}

Errc PGeodesicDilation( const Imc3dsl &ims, const Img3duc &imq, Imc3dsl &imd, int numSE, int halfsize, int iteration ) {
   if (numSE > 12 || numSE < 0) {
      std::cerr << "Error pgeodesicdilatation: bad structuring element type: " << numSE << std::endl;
      return FAILURE;
   }
   if (halfsize < 1) {
      std::cerr << "Error pgeodesicdilatation: bad structuring element size: " << halfsize << std::endl;
      return FAILURE;
   }
   if (ims.Size() != imq.Size()) {
      std::cerr << "Error pdilatationreconstruction: Incompatible image size." << std::endl;
      return FAILURE;
   }

   Point3d p, p1;
   Fifo<Point3d> toBeVisited[2];

   StructuringElement3d *se = new StructuringElement3d(numSE, halfsize);

   imd = ims;
   for (p.z = 0; p.z < ims.Depth(); p.z++) {
      for (p.y = 0; p.y < ims.Height(); p.y++) {
	 for (p.x = 0; p.x < ims.Width(); p.x++) {
	    if ((imq[p])) {
	       Imc3dsl::ValueType maxX = ims.X[p];
	       Imc3dsl::ValueType maxY = ims.Y[p];
	       Imc3dsl::ValueType maxZ = ims.Z[p];
	       for (int i = 0; i < se->length; i++) {
		  p1 = p;
		  se->Move(i, p1);
		  if (ims.Hold(p1)
		      && Greater(ims.X[p1], ims.Y[p1], ims.Z[p1], maxX, maxY, maxZ)
		      && (imq[p1] == imq[p])) {
		     maxX = ims.X[p1];
		     maxY = ims.Y[p1];
		     maxZ = ims.Z[p1];
		  }
	       }
	       if (ims.X[p] != maxX|| ims.Y[p] != maxY || ims.Z[p] != maxZ ) {
		  imd.X[p] = maxX;
		  imd.Y[p] = maxY;
		  imd.Z[p] = maxZ;
		  toBeVisited[0].Push(p);
	       }
	    }
	 }
      }
   }

   if (iteration < 0) {
      iteration = Limits<Long>::max();
   }
   iteration--;
   int fifo  =0;
   while (iteration > 0 && !toBeVisited[fifo].Empty()) {
      iteration--;
      while (!toBeVisited[fifo].Empty()) {
	 Point3d p3 = toBeVisited[fifo].Pop();
	 for (int v = 0; v < 26; v++) {
	    p = p3 + v26[v];
	    
	    if (imq.Hold(p) && (imq[p])) {
	       Imc3dsl::ValueType maxX = imd.X[p];
	       Imc3dsl::ValueType maxY = imd.Y[p];
	       Imc3dsl::ValueType maxZ = imd.Z[p];
	       for (int i = 0; i < se->length; i++) {
		  p1 = p;
		  se->Move(i, p1);
		  if (imd.Hold(p1)
		      && Greater(imd.X[p1], imd.Y[p1], imd.Z[p1], maxX, maxY, maxZ)
		      && (imq[p1] == imq[p])) {
		     maxX = imd.X[p1];
		     maxY = imd.Y[p1];
		     maxZ = imd.Z[p1];
		  }
	       }
	       if (imd.X[p] != maxX || imd.Y[p] != maxY || imd.Z[p] != maxZ) {
		  imd.X[p] = maxX;
		  imd.Y[p] = maxY;
		  imd.Z[p] = maxZ;
		  toBeVisited[1 - fifo].Push(p);
	       }
	    }
	 }
      }
      fifo = 1 - fifo;
   }
   
   delete se;
   return SUCCESS;
}

Errc PGeodesicDilation( const Imc3dsf &ims, const Img3duc &imq, Imc3dsf &imd, int numSE, int halfsize, int iteration ) {
   if (numSE > 12 || numSE < 0) {
      std::cerr << "Error pgeodesicdilatation: bad structuring element type: " << numSE << std::endl;
      return FAILURE;
   }
   if (halfsize < 1) {
      std::cerr << "Error pgeodesicdilatation: bad structuring element size: " << halfsize << std::endl;
      return FAILURE;
   }
   if (ims.Size() != imq.Size()) {
      std::cerr << "Error pdilatationreconstruction: Incompatible image size." << std::endl;
      return FAILURE;
   }

   Point3d p, p1;
   Fifo<Point3d> toBeVisited[2];

   StructuringElement3d *se = new StructuringElement3d(numSE, halfsize);

   imd = ims;
   for (p.z = 0; p.z < ims.Depth(); p.z++) {
      for (p.y = 0; p.y < ims.Height(); p.y++) {
	 for (p.x = 0; p.x < ims.Width(); p.x++) {
	    if ((imq[p])) {
	       Imc3dsf::ValueType maxX = ims.X[p];
	       Imc3dsf::ValueType maxY = ims.Y[p];
	       Imc3dsf::ValueType maxZ = ims.Z[p];
	       for (int i = 0; i < se->length; i++) {
		  p1 = p;
		  se->Move(i, p1);
		  if (ims.Hold(p1)
		      && Greater(ims.X[p1], ims.Y[p1], ims.Z[p1], maxX, maxY, maxZ)
		      && (imq[p1] == imq[p])) {
		     maxX = ims.X[p1];
		     maxY = ims.Y[p1];
		     maxZ = ims.Z[p1];
		  }
	       }
	       if (ims.X[p] != maxX|| ims.Y[p] != maxY || ims.Z[p] != maxZ ) {
		  imd.X[p] = maxX;
		  imd.Y[p] = maxY;
		  imd.Z[p] = maxZ;
		  toBeVisited[0].Push(p);
	       }
	    }
	 }
      }
   }

   if (iteration < 0) {
      iteration = Limits<Long>::max();
   }
   iteration--;
   int fifo  =0;
   while (iteration > 0 && !toBeVisited[fifo].Empty()) {
      iteration--;
      while (!toBeVisited[fifo].Empty()) {
	 Point3d p3 = toBeVisited[fifo].Pop();
	 for (int v = 0; v < 26; v++) {
	    p = p3 + v26[v];
	    
	    if (imq.Hold(p) && (imq[p])) {
	       Imc3dsf::ValueType maxX = imd.X[p];
	       Imc3dsf::ValueType maxY = imd.Y[p];
	       Imc3dsf::ValueType maxZ = imd.Z[p];
	       for (int i = 0; i < se->length; i++) {
		  p1 = p;
		  se->Move(i, p1);
		  if (imd.Hold(p1)
		      && Greater(imd.X[p1], imd.Y[p1], imd.Z[p1], maxX, maxY, maxZ)
		      && (imq[p1] == imq[p])) {
		     maxX = imd.X[p1];
		     maxY = imd.Y[p1];
		     maxZ = imd.Z[p1];
		  }
	       }
	       if (imd.X[p] != maxX || imd.Y[p] != maxY || imd.Z[p] != maxZ) {
		  imd.X[p] = maxX;
		  imd.Y[p] = maxY;
		  imd.Z[p] = maxZ;
		  toBeVisited[1 - fifo].Push(p);
	       }
	    }
	 }
      }
      fifo = 1 - fifo;
   }
   
   delete se;
   return SUCCESS;
}

Errc PGeodesicDilation( const Imc3duc &ims, const Reg3d &imq, Imc3duc &imd, int numSE, int halfsize, int iteration ) {
   if (numSE > 12 || numSE < 0) {
      std::cerr << "Error pgeodesicdilatation: bad structuring element type: " << numSE << std::endl;
      return FAILURE;
   }
   if (halfsize < 1) {
      std::cerr << "Error pgeodesicdilatation: bad structuring element size: " << halfsize << std::endl;
      return FAILURE;
   }
   if (ims.Size() != imq.Size()) {
      std::cerr << "Error pdilatationreconstruction: Incompatible image size." << std::endl;
      return FAILURE;
   }

   Point3d p, p1;
   Fifo<Point3d> toBeVisited[2];

   StructuringElement3d *se = new StructuringElement3d(numSE, halfsize);

   imd = ims;
   for (p.z = 0; p.z < ims.Depth(); p.z++) {
      for (p.y = 0; p.y < ims.Height(); p.y++) {
	 for (p.x = 0; p.x < ims.Width(); p.x++) {
	    if ((imq[p])) {
	       Imc3duc::ValueType maxX = ims.X[p];
	       Imc3duc::ValueType maxY = ims.Y[p];
	       Imc3duc::ValueType maxZ = ims.Z[p];
	       for (int i = 0; i < se->length; i++) {
		  p1 = p;
		  se->Move(i, p1);
		  if (ims.Hold(p1)
		      && Greater(ims.X[p1], ims.Y[p1], ims.Z[p1], maxX, maxY, maxZ)
		      && (imq[p1] == imq[p])) {
		     maxX = ims.X[p1];
		     maxY = ims.Y[p1];
		     maxZ = ims.Z[p1];
		  }
	       }
	       if (ims.X[p] != maxX|| ims.Y[p] != maxY || ims.Z[p] != maxZ ) {
		  imd.X[p] = maxX;
		  imd.Y[p] = maxY;
		  imd.Z[p] = maxZ;
		  toBeVisited[0].Push(p);
	       }
	    }
	 }
      }
   }

   if (iteration < 0) {
      iteration = Limits<Long>::max();
   }
   iteration--;
   int fifo  =0;
   while (iteration > 0 && !toBeVisited[fifo].Empty()) {
      iteration--;
      while (!toBeVisited[fifo].Empty()) {
	 Point3d p3 = toBeVisited[fifo].Pop();
	 for (int v = 0; v < 26; v++) {
	    p = p3 + v26[v];
	    
	    if (imq.Hold(p) && (imq[p])) {
	       Imc3duc::ValueType maxX = imd.X[p];
	       Imc3duc::ValueType maxY = imd.Y[p];
	       Imc3duc::ValueType maxZ = imd.Z[p];
	       for (int i = 0; i < se->length; i++) {
		  p1 = p;
		  se->Move(i, p1);
		  if (imd.Hold(p1)
		      && Greater(imd.X[p1], imd.Y[p1], imd.Z[p1], maxX, maxY, maxZ)
		      && (imq[p1] == imq[p])) {
		     maxX = imd.X[p1];
		     maxY = imd.Y[p1];
		     maxZ = imd.Z[p1];
		  }
	       }
	       if (imd.X[p] != maxX || imd.Y[p] != maxY || imd.Z[p] != maxZ) {
		  imd.X[p] = maxX;
		  imd.Y[p] = maxY;
		  imd.Z[p] = maxZ;
		  toBeVisited[1 - fifo].Push(p);
	       }
	    }
	 }
      }
      fifo = 1 - fifo;
   }
   
   delete se;
   return SUCCESS;
}

Errc PGeodesicDilation( const Imc3dsl &ims, const Reg3d &imq, Imc3dsl &imd, int numSE, int halfsize, int iteration ) {
   if (numSE > 12 || numSE < 0) {
      std::cerr << "Error pgeodesicdilatation: bad structuring element type: " << numSE << std::endl;
      return FAILURE;
   }
   if (halfsize < 1) {
      std::cerr << "Error pgeodesicdilatation: bad structuring element size: " << halfsize << std::endl;
      return FAILURE;
   }
   if (ims.Size() != imq.Size()) {
      std::cerr << "Error pdilatationreconstruction: Incompatible image size." << std::endl;
      return FAILURE;
   }

   Point3d p, p1;
   Fifo<Point3d> toBeVisited[2];

   StructuringElement3d *se = new StructuringElement3d(numSE, halfsize);

   imd = ims;
   for (p.z = 0; p.z < ims.Depth(); p.z++) {
      for (p.y = 0; p.y < ims.Height(); p.y++) {
	 for (p.x = 0; p.x < ims.Width(); p.x++) {
	    if ((imq[p])) {
	       Imc3dsl::ValueType maxX = ims.X[p];
	       Imc3dsl::ValueType maxY = ims.Y[p];
	       Imc3dsl::ValueType maxZ = ims.Z[p];
	       for (int i = 0; i < se->length; i++) {
		  p1 = p;
		  se->Move(i, p1);
		  if (ims.Hold(p1)
		      && Greater(ims.X[p1], ims.Y[p1], ims.Z[p1], maxX, maxY, maxZ)
		      && (imq[p1] == imq[p])) {
		     maxX = ims.X[p1];
		     maxY = ims.Y[p1];
		     maxZ = ims.Z[p1];
		  }
	       }
	       if (ims.X[p] != maxX|| ims.Y[p] != maxY || ims.Z[p] != maxZ ) {
		  imd.X[p] = maxX;
		  imd.Y[p] = maxY;
		  imd.Z[p] = maxZ;
		  toBeVisited[0].Push(p);
	       }
	    }
	 }
      }
   }

   if (iteration < 0) {
      iteration = Limits<Long>::max();
   }
   iteration--;
   int fifo  =0;
   while (iteration > 0 && !toBeVisited[fifo].Empty()) {
      iteration--;
      while (!toBeVisited[fifo].Empty()) {
	 Point3d p3 = toBeVisited[fifo].Pop();
	 for (int v = 0; v < 26; v++) {
	    p = p3 + v26[v];
	    
	    if (imq.Hold(p) && (imq[p])) {
	       Imc3dsl::ValueType maxX = imd.X[p];
	       Imc3dsl::ValueType maxY = imd.Y[p];
	       Imc3dsl::ValueType maxZ = imd.Z[p];
	       for (int i = 0; i < se->length; i++) {
		  p1 = p;
		  se->Move(i, p1);
		  if (imd.Hold(p1)
		      && Greater(imd.X[p1], imd.Y[p1], imd.Z[p1], maxX, maxY, maxZ)
		      && (imq[p1] == imq[p])) {
		     maxX = imd.X[p1];
		     maxY = imd.Y[p1];
		     maxZ = imd.Z[p1];
		  }
	       }
	       if (imd.X[p] != maxX || imd.Y[p] != maxY || imd.Z[p] != maxZ) {
		  imd.X[p] = maxX;
		  imd.Y[p] = maxY;
		  imd.Z[p] = maxZ;
		  toBeVisited[1 - fifo].Push(p);
	       }
	    }
	 }
      }
      fifo = 1 - fifo;
   }
   
   delete se;
   return SUCCESS;
}

Errc PGeodesicDilation( const Imc3dsf &ims, const Reg3d &imq, Imc3dsf &imd, int numSE, int halfsize, int iteration ) {
   if (numSE > 12 || numSE < 0) {
      std::cerr << "Error pgeodesicdilatation: bad structuring element type: " << numSE << std::endl;
      return FAILURE;
   }
   if (halfsize < 1) {
      std::cerr << "Error pgeodesicdilatation: bad structuring element size: " << halfsize << std::endl;
      return FAILURE;
   }
   if (ims.Size() != imq.Size()) {
      std::cerr << "Error pdilatationreconstruction: Incompatible image size." << std::endl;
      return FAILURE;
   }

   Point3d p, p1;
   Fifo<Point3d> toBeVisited[2];

   StructuringElement3d *se = new StructuringElement3d(numSE, halfsize);

   imd = ims;
   for (p.z = 0; p.z < ims.Depth(); p.z++) {
      for (p.y = 0; p.y < ims.Height(); p.y++) {
	 for (p.x = 0; p.x < ims.Width(); p.x++) {
	    if ((imq[p])) {
	       Imc3dsf::ValueType maxX = ims.X[p];
	       Imc3dsf::ValueType maxY = ims.Y[p];
	       Imc3dsf::ValueType maxZ = ims.Z[p];
	       for (int i = 0; i < se->length; i++) {
		  p1 = p;
		  se->Move(i, p1);
		  if (ims.Hold(p1)
		      && Greater(ims.X[p1], ims.Y[p1], ims.Z[p1], maxX, maxY, maxZ)
		      && (imq[p1] == imq[p])) {
		     maxX = ims.X[p1];
		     maxY = ims.Y[p1];
		     maxZ = ims.Z[p1];
		  }
	       }
	       if (ims.X[p] != maxX|| ims.Y[p] != maxY || ims.Z[p] != maxZ ) {
		  imd.X[p] = maxX;
		  imd.Y[p] = maxY;
		  imd.Z[p] = maxZ;
		  toBeVisited[0].Push(p);
	       }
	    }
	 }
      }
   }

   if (iteration < 0) {
      iteration = Limits<Long>::max();
   }
   iteration--;
   int fifo  =0;
   while (iteration > 0 && !toBeVisited[fifo].Empty()) {
      iteration--;
      while (!toBeVisited[fifo].Empty()) {
	 Point3d p3 = toBeVisited[fifo].Pop();
	 for (int v = 0; v < 26; v++) {
	    p = p3 + v26[v];
	    
	    if (imq.Hold(p) && (imq[p])) {
	       Imc3dsf::ValueType maxX = imd.X[p];
	       Imc3dsf::ValueType maxY = imd.Y[p];
	       Imc3dsf::ValueType maxZ = imd.Z[p];
	       for (int i = 0; i < se->length; i++) {
		  p1 = p;
		  se->Move(i, p1);
		  if (imd.Hold(p1)
		      && Greater(imd.X[p1], imd.Y[p1], imd.Z[p1], maxX, maxY, maxZ)
		      && (imq[p1] == imq[p])) {
		     maxX = imd.X[p1];
		     maxY = imd.Y[p1];
		     maxZ = imd.Z[p1];
		  }
	       }
	       if (imd.X[p] != maxX || imd.Y[p] != maxY || imd.Z[p] != maxZ) {
		  imd.X[p] = maxX;
		  imd.Y[p] = maxY;
		  imd.Z[p] = maxZ;
		  toBeVisited[1 - fifo].Push(p);
	       }
	    }
	 }
      }
      fifo = 1 - fifo;
   }
   
   delete se;
   return SUCCESS;
}


#ifdef MAIN
#define	USAGE	"usage: %s num_es halfsize iteration [-m mask] [im_in|-] [im_mq|-] [im_out|-]"
#define	PARC	3
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
   if (objs[0]->Type() == Po_Img2duc && objs[1]->Type() == Po_Img2duc) {
      Img2duc* const ims = (Img2duc*)objs[0];
      Img2duc* const imq = (Img2duc*)objs[1];
      objd[0] = new Img2duc(ims->Props());
      Img2duc* const imd = (Img2duc*)objd[0];
      
      result = PGeodesicDilation(*ims, *imq, *imd, atoi(parv[0]), atoi(parv[1]), atoi(parv[2]));
      goto end;
   }
   if (objs[0]->Type() == Po_Img2dsl && objs[1]->Type() == Po_Img2duc) {
      Img2dsl* const ims = (Img2dsl*)objs[0];
      Img2duc* const imq = (Img2duc*)objs[1];
      objd[0] = new Img2dsl(ims->Props());
      Img2dsl* const imd = (Img2dsl*)objd[0];
      
      result = PGeodesicDilation(*ims, *imq, *imd, atoi(parv[0]), atoi(parv[1]), atoi(parv[2]));
      goto end;
   }
   if (objs[0]->Type() == Po_Img2dsf && objs[1]->Type() == Po_Img2duc) {
      Img2dsf* const ims = (Img2dsf*)objs[0];
      Img2duc* const imq = (Img2duc*)objs[1];
      objd[0] = new Img2dsf(ims->Props());
      Img2dsf* const imd = (Img2dsf*)objd[0];
      
      result = PGeodesicDilation(*ims, *imq, *imd, atoi(parv[0]), atoi(parv[1]), atoi(parv[2]));
      goto end;
   }
   if (objs[0]->Type() == Po_Img2duc && objs[1]->Type() == Po_Reg2d) {
      Img2duc* const ims = (Img2duc*)objs[0];
      Reg2d* const imq = (Reg2d*)objs[1];
      objd[0] = new Img2duc(ims->Props());
      Img2duc* const imd = (Img2duc*)objd[0];
      
      result = PGeodesicDilation(*ims, *imq, *imd, atoi(parv[0]), atoi(parv[1]), atoi(parv[2]));
      goto end;
   }
   if (objs[0]->Type() == Po_Img2dsl && objs[1]->Type() == Po_Reg2d) {
      Img2dsl* const ims = (Img2dsl*)objs[0];
      Reg2d* const imq = (Reg2d*)objs[1];
      objd[0] = new Img2dsl(ims->Props());
      Img2dsl* const imd = (Img2dsl*)objd[0];
      
      result = PGeodesicDilation(*ims, *imq, *imd, atoi(parv[0]), atoi(parv[1]), atoi(parv[2]));
      goto end;
   }
   if (objs[0]->Type() == Po_Img2dsf && objs[1]->Type() == Po_Reg2d) {
      Img2dsf* const ims = (Img2dsf*)objs[0];
      Reg2d* const imq = (Reg2d*)objs[1];
      objd[0] = new Img2dsf(ims->Props());
      Img2dsf* const imd = (Img2dsf*)objd[0];
      
      result = PGeodesicDilation(*ims, *imq, *imd, atoi(parv[0]), atoi(parv[1]), atoi(parv[2]));
      goto end;
   }
   if (objs[0]->Type() == Po_Reg2d && objs[1]->Type() == Po_Reg2d) {
      Reg2d* const ims = (Reg2d*)objs[0];
      Reg2d* const imq = (Reg2d*)objs[1];
      objd[0] = new Reg2d(ims->Props());
      Reg2d* const imd = (Reg2d*)objd[0];
      
      result = PGeodesicDilation(*ims, *imq, *imd, atoi(parv[0]), atoi(parv[1]), atoi(parv[2]));
      goto end;
   }
   if (objs[0]->Type() == Po_Reg2d && objs[1]->Type() == Po_Img2duc) {
      Reg2d* const ims = (Reg2d*)objs[0];
      Img2duc* const imq = (Img2duc*)objs[1];
      objd[0] = new Reg2d(ims->Props());
      Reg2d* const imd = (Reg2d*)objd[0];
      
      result = PGeodesicDilation(*ims, *imq, *imd, atoi(parv[0]), atoi(parv[1]), atoi(parv[2]));
      goto end;
   }
   if (objs[0]->Type() == Po_Reg2d && objs[1]->Type() == Po_Img2dsl) {
      Reg2d* const ims = (Reg2d*)objs[0];
      Img2dsl* const imq = (Img2dsl*)objs[1];
      objd[0] = new Reg2d(ims->Props());
      Reg2d* const imd = (Reg2d*)objd[0];
      
      result = PGeodesicDilation(*ims, *imq, *imd, atoi(parv[0]), atoi(parv[1]), atoi(parv[2]));
      goto end;
   }
   if (objs[0]->Type() == Po_Reg2d && objs[1]->Type() == Po_Img2dsf) {
      Reg2d* const ims = (Reg2d*)objs[0];
      Img2dsf* const imq = (Img2dsf*)objs[1];
      objd[0] = new Reg2d(ims->Props());
      Reg2d* const imd = (Reg2d*)objd[0];
      
      result = PGeodesicDilation(*ims, *imq, *imd, atoi(parv[0]), atoi(parv[1]), atoi(parv[2]));
      goto end;
   }
   if (objs[0]->Type() == Po_Img3duc && objs[1]->Type() == Po_Img3duc) {
      Img3duc* const ims = (Img3duc*)objs[0];
      Img3duc* const imq = (Img3duc*)objs[1];
      objd[0] = new Img3duc(ims->Props());
      Img3duc* const imd = (Img3duc*)objd[0];

      result = PGeodesicDilation(*ims, *imq, *imd, atoi(parv[0]), atoi(parv[1]), atoi(parv[2]));
      goto end;
   }
   if (objs[0]->Type() == Po_Img3dsl && objs[1]->Type() == Po_Img3duc) {
      Img3dsl* const ims = (Img3dsl*)objs[0];
      Img3duc* const imq = (Img3duc*)objs[1];
      objd[0] = new Img3dsl(ims->Props());
      Img3dsl* const imd = (Img3dsl*)objd[0];

      result = PGeodesicDilation(*ims, *imq, *imd, atoi(parv[0]), atoi(parv[1]), atoi(parv[2]));
      goto end;
   }
   if (objs[0]->Type() == Po_Img3dsf && objs[1]->Type() == Po_Img3duc) {
      Img3dsf* const ims = (Img3dsf*)objs[0];
      Img3duc* const imq = (Img3duc*)objs[1];
      objd[0] = new Img3dsf(ims->Props());
      Img3dsf* const imd = (Img3dsf*)objd[0];

      result = PGeodesicDilation(*ims, *imq, *imd, atoi(parv[0]), atoi(parv[1]), atoi(parv[2]));
      goto end;
   }
   if (objs[0]->Type() == Po_Img3duc && objs[1]->Type() == Po_Reg3d) {
      Img3duc* const ims = (Img3duc*)objs[0];
      Reg3d* const imq = (Reg3d*)objs[1];
      objd[0] = new Img3duc(ims->Props());
      Img3duc* const imd = (Img3duc*)objd[0];

      result = PGeodesicDilation(*ims, *imq, *imd, atoi(parv[0]), atoi(parv[1]), atoi(parv[2]));
      goto end;
   }
   if (objs[0]->Type() == Po_Img3dsl && objs[1]->Type() == Po_Reg3d) {
      Img3dsl* const ims = (Img3dsl*)objs[0];
      Reg3d* const imq = (Reg3d*)objs[1];
      objd[0] = new Img3dsl(ims->Props());
      Img3dsl* const imd = (Img3dsl*)objd[0];

      result = PGeodesicDilation(*ims, *imq, *imd, atoi(parv[0]), atoi(parv[1]), atoi(parv[2]));
      goto end;
   }
   if (objs[0]->Type() == Po_Img3dsf && objs[1]->Type() == Po_Reg3d) {
      Img3dsf* const ims = (Img3dsf*)objs[0];
      Reg3d* const imq = (Reg3d*)objs[1];
      objd[0] = new Img3dsf(ims->Props());
      Img3dsf* const imd = (Img3dsf*)objd[0];

      result = PGeodesicDilation(*ims, *imq, *imd, atoi(parv[0]), atoi(parv[1]), atoi(parv[2]));
      goto end;
   }
   if (objs[0]->Type() == Po_Reg3d && objs[1]->Type() == Po_Reg3d) {
      Reg3d* const ims = (Reg3d*)objs[0];
      Reg3d* const imq = (Reg3d*)objs[1];
      objd[0] = new Reg3d(ims->Props());
      Reg3d* const imd = (Reg3d*)objd[0];

      result = PGeodesicDilation(*ims, *imq, *imd, atoi(parv[0]), atoi(parv[1]), atoi(parv[2]));
      goto end;
   }
   if (objs[0]->Type() == Po_Reg3d && objs[1]->Type() == Po_Img3duc) {
      Reg3d* const ims = (Reg3d*)objs[0];
      Img3duc* const imq = (Img3duc*)objs[1];
      objd[0] = new Reg3d(ims->Props());
      Reg3d* const imd = (Reg3d*)objd[0];

      result = PGeodesicDilation(*ims, *imq, *imd, atoi(parv[0]), atoi(parv[1]), atoi(parv[2]));
      goto end;
   }
   if (objs[0]->Type() == Po_Reg3d && objs[1]->Type() == Po_Img3dsl) {
      Reg3d* const ims = (Reg3d*)objs[0];
      Img3dsl* const imq = (Img3dsl*)objs[1];
      objd[0] = new Reg3d(ims->Props());
      Reg3d* const imd = (Reg3d*)objd[0];

      result = PGeodesicDilation(*ims, *imq, *imd, atoi(parv[0]), atoi(parv[1]), atoi(parv[2]));
      goto end;
   }
   if (objs[0]->Type() == Po_Reg3d && objs[1]->Type() == Po_Img3dsf) {
      Reg3d* const ims = (Reg3d*)objs[0];
      Img3dsf* const imq = (Img3dsf*)objs[1];
      objd[0] = new Reg3d(ims->Props());
      Reg3d* const imd = (Reg3d*)objd[0];

      result = PGeodesicDilation(*ims, *imq, *imd, atoi(parv[0]), atoi(parv[1]), atoi(parv[2]));
      goto end;
   }
   if (objs[0]->Type() == Po_Imc2duc && objs[1]->Type() == Po_Img2duc) {
      Imc2duc* const ims = (Imc2duc*)objs[0];
      Img2duc* const imq = (Img2duc*)objs[1];
      objd[0] = new Imc2duc(ims->Props());
      Imc2duc* const imd = (Imc2duc*)objd[0];
      
      result = PGeodesicDilation(*ims, *imq, *imd, atoi(parv[0]), atoi(parv[1]), atoi(parv[2]));
      goto end;
   }
   if (objs[0]->Type() == Po_Imc2dsl && objs[1]->Type() == Po_Img2duc) {
      Imc2dsl* const ims = (Imc2dsl*)objs[0];
      Img2duc* const imq = (Img2duc*)objs[1];
      objd[0] = new Imc2dsl(ims->Props());
      Imc2dsl* const imd = (Imc2dsl*)objd[0];
      
      result = PGeodesicDilation(*ims, *imq, *imd, atoi(parv[0]), atoi(parv[1]), atoi(parv[2]));
      goto end;
   }
   if (objs[0]->Type() == Po_Imc2dsf && objs[1]->Type() == Po_Img2duc) {
      Imc2dsf* const ims = (Imc2dsf*)objs[0];
      Img2duc* const imq = (Img2duc*)objs[1];
      objd[0] = new Imc2dsf(ims->Props());
      Imc2dsf* const imd = (Imc2dsf*)objd[0];
      
      result = PGeodesicDilation(*ims, *imq, *imd, atoi(parv[0]), atoi(parv[1]), atoi(parv[2]));
      goto end;
   }
   if (objs[0]->Type() == Po_Imc2duc && objs[1]->Type() == Po_Reg2d) {
      Imc2duc* const ims = (Imc2duc*)objs[0];
      Reg2d* const imq = (Reg2d*)objs[1];
      objd[0] = new Imc2duc(ims->Props());
      Imc2duc* const imd = (Imc2duc*)objd[0];
      
      result = PGeodesicDilation(*ims, *imq, *imd, atoi(parv[0]), atoi(parv[1]), atoi(parv[2]));
      goto end;
   }
   if (objs[0]->Type() == Po_Imc2dsl && objs[1]->Type() == Po_Reg2d) {
      Imc2dsl* const ims = (Imc2dsl*)objs[0];
      Reg2d* const imq = (Reg2d*)objs[1];
      objd[0] = new Imc2dsl(ims->Props());
      Imc2dsl* const imd = (Imc2dsl*)objd[0];
      
      result = PGeodesicDilation(*ims, *imq, *imd, atoi(parv[0]), atoi(parv[1]), atoi(parv[2]));
      goto end;
   }
   if (objs[0]->Type() == Po_Imc2dsf && objs[1]->Type() == Po_Reg2d) {
      Imc2dsf* const ims = (Imc2dsf*)objs[0];
      Reg2d* const imq = (Reg2d*)objs[1];
      objd[0] = new Imc2dsf(ims->Props());
      Imc2dsf* const imd = (Imc2dsf*)objd[0];
      
      result = PGeodesicDilation(*ims, *imq, *imd, atoi(parv[0]), atoi(parv[1]), atoi(parv[2]));
      goto end;
   }
   if (objs[0]->Type() == Po_Imc3duc && objs[1]->Type() == Po_Img3duc) {
      Imc3duc* const ims = (Imc3duc*)objs[0];
      Img3duc* const imq = (Img3duc*)objs[1];
      objd[0] = new Imc3duc(ims->Props());
      Imc3duc* const imd = (Imc3duc*)objd[0];
      
      result = PGeodesicDilation(*ims, *imq, *imd, atoi(parv[0]), atoi(parv[1]), atoi(parv[2]));
      goto end;
   }
   if (objs[0]->Type() == Po_Imc3dsl && objs[1]->Type() == Po_Img3duc) {
      Imc3dsl* const ims = (Imc3dsl*)objs[0];
      Img3duc* const imq = (Img3duc*)objs[1];
      objd[0] = new Imc3dsl(ims->Props());
      Imc3dsl* const imd = (Imc3dsl*)objd[0];
      
      result = PGeodesicDilation(*ims, *imq, *imd, atoi(parv[0]), atoi(parv[1]), atoi(parv[2]));
      goto end;
   }
   if (objs[0]->Type() == Po_Imc3dsf && objs[1]->Type() == Po_Img3duc) {
      Imc3dsf* const ims = (Imc3dsf*)objs[0];
      Img3duc* const imq = (Img3duc*)objs[1];
      objd[0] = new Imc3dsf(ims->Props());
      Imc3dsf* const imd = (Imc3dsf*)objd[0];
      
      result = PGeodesicDilation(*ims, *imq, *imd, atoi(parv[0]), atoi(parv[1]), atoi(parv[2]));
      goto end;
   }
   if (objs[0]->Type() == Po_Imc3duc && objs[1]->Type() == Po_Reg3d) {
      Imc3duc* const ims = (Imc3duc*)objs[0];
      Reg3d* const imq = (Reg3d*)objs[1];
      objd[0] = new Imc3duc(ims->Props());
      Imc3duc* const imd = (Imc3duc*)objd[0];
      
      result = PGeodesicDilation(*ims, *imq, *imd, atoi(parv[0]), atoi(parv[1]), atoi(parv[2]));
      goto end;
   }
   if (objs[0]->Type() == Po_Imc3dsl && objs[1]->Type() == Po_Reg3d) {
      Imc3dsl* const ims = (Imc3dsl*)objs[0];
      Reg3d* const imq = (Reg3d*)objs[1];
      objd[0] = new Imc3dsl(ims->Props());
      Imc3dsl* const imd = (Imc3dsl*)objd[0];
      
      result = PGeodesicDilation(*ims, *imq, *imd, atoi(parv[0]), atoi(parv[1]), atoi(parv[2]));
      goto end;
   }
   if (objs[0]->Type() == Po_Imc3dsf && objs[1]->Type() == Po_Reg3d) {
      Imc3dsf* const ims = (Imc3dsf*)objs[0];
      Reg3d* const imq = (Reg3d*)objs[1];
      objd[0] = new Imc3dsf(ims->Props());
      Imc3dsf* const imd = (Imc3dsf*)objd[0];
      
      result = PGeodesicDilation(*ims, *imq, *imd, atoi(parv[0]), atoi(parv[1]), atoi(parv[2]));
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
