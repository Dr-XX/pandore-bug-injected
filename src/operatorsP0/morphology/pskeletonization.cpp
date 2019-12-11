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
 * @author Sebastien Fourey - March 2013
 */

/**
 * @file pskeletonization.cc
 * Computes a homotopic skeleton for a 2D image.
 * The skeleton will be either made of 4-connected or 8-connected
 * components.
 * A Euclidean-distance-ordered removal process is used
 * that will repeatedly remove simple points, but will not remove 
 * points that belong to the medial axis obtained
 * from the euclidean distance transform.
 */

#include <cstring>
#include <cstdlib>
#include <set>
#include <queue>

#ifndef _WIN32
#include <unistd.h>
#endif
#include <pandore.h>
using namespace pandore;

#ifdef MAIN
#undef MAIN
#include "contour/pdistance.cpp"
#define MAIN
#else
#include "contour/pdistance.cpp"
#endif

const char Simple8[256] = { 
   0,1,1,1,1,1,1,1,1,0,0,0,1,1,1,1,1,0,0,0,1,1,1,1,1,0,0,0,1,1,1,1,1,
   0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,1,1,1,1,1,0,0,0,1,1,1,1,1,1,
   0,1,0,1,0,1,0,0,0,0,0,1,0,1,1,1,0,1,1,0,1,0,1,1,0,1,1,0,1,0,1,1,0,
   1,0,1,0,1,0,0,0,0,0,1,0,1,1,1,0,1,1,0,1,0,1,1,0,1,1,0,1,0,1,1,0,1,
   0,1,0,1,0,0,0,0,0,1,0,1,0,0,0,0,0,1,0,1,0,0,0,0,0,1,0,1,0,0,0,0,0,
   0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,1,0,0,0,0,0,1,0,1,1,1,0,1,0,1,
   0,1,0,0,0,0,0,1,0,1,1,1,0,1,1,0,1,0,1,1,0,1,1,0,1,0,1,1,0,1,0,1,0,
   1,0,0,0,0,0,1,0,1,1,1,0,1,1,0,1,0,1,1,0,1,1,0,1,0
};

const char Simple4[256] = { 
   0,1,0,1,1,0,1,1,0,1,0,1,1,0,1,1,1,0,1,0,0,0,0,0,1,0,1,0,1,0,1,1,0,1,
   0,1,1,0,1,1,0,1,0,1,1,0,1,1,1,0,1,0,0,0,0,0,1,0,1,0,1,0,1,1,1,0,1,0,
   0,0,0,0,1,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,1,0,0,0,
   0,0,1,0,1,0,0,0,0,0,1,0,1,0,0,0,0,0,1,0,1,0,1,0,1,1,0,1,0,1,1,0,1,1,
   0,1,0,1,1,0,1,1,1,0,1,0,0,0,0,0,1,0,1,0,1,0,1,1,0,1,0,1,1,0,1,1,0,1,
   0,1,1,0,1,1,1,0,1,0,0,0,0,0,1,0,1,0,1,0,1,1,1,1,1,1,0,0,0,1,1,1,1,1,
   0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,0,0,0,1,1,1,1,1,0,0,
   0,1,1,1,1,1,0,0,0,1,1,1,1,1,1,1,1,0
};

const char Border8[256] = {
   1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
   1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
   1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,1,0,1,1,1,1,1,0,1,0,1,1,1,1,1,1,
   1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,1,0,1,1,1,1,1,0,1,0,1,1,1,1,1,1,1,1,
   1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
   1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
   1,1,1,1,1,1,1,1,1,0,1,0,1,1,1,1,1,0,1,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
   1,1,1,1,1,1,1,0,1,0,1,1,1,1,1,0,1,0
};

const char Border4[256] = {
   1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
   1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
   1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
   1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
   1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
   1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
   1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
   1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0
};

/*
 * Convention used in the numbering of the neighboring pixels
 *  32 16  8
 *  64     4
 * 128  1  2
 */

const int NorthBit = 16;
const int SouthBit = 1;
const int WestBit = 64;
const int EastBit = 4;

const int Neighbors_dx4[] = { -1, 0, +1, 0 };
const int Neighbors_dy4[] = { 0, -1, 0, +1 };
const int Neighbors_dx8[] = { -1, 0,  +1, -1, +1, -1,  0, +1 };
const int Neighbors_dy8[] = { -1, -1, -1,  0,  0, +1, +1, +1 };


inline bool extremity4( const int & cnf ) {
   int res = 0;
   res += (cnf & 1) ? 1 : 0;
   res += (cnf & 4) ? 1 : 0;
   res += (cnf & 16) ? 1 : 0;
   res += (cnf & 64) ? 1 : 0;
   return res == 1;
}

inline bool extremity8( const int & cnf ) {
   int res = 0;
   int mask = 1;
   while (mask < 256) {
      res += (cnf & mask) ? 1 : 0;   
      mask <<= 1;
   }
   return res == 1;
}

inline bool border4( const int & cnf ) {
   return cnf != 255;
}

inline bool border8( const int & cnf ) {
   return (cnf | (1|4|16|64)) != cnf ;
}

struct PointDistance {
   int x;
   int y;
   Float distance;
   PointDistance( int x = 0, int y = 0, Float distance = 0.0f ): x(x), y(y), distance(distance) {
   }
};
struct PointDistanceLess { 
   bool operator()( const PointDistance & p1, const PointDistance & p2 ) {
      if (p1.distance < p2.distance) {
	 return true;
      }
      if (p1.distance == p2.distance) { 
	 if (p1.y < p2.y) {
	    return true;
	 }
	 if (p1.y == p2.y) {
	    if (p1.x < p2.x)  {
	       return true;
	    }
	 }
      }
      return false;
   }
};
struct PointDistanceGreater { 
   bool operator()( const PointDistance & p1, const PointDistance & p2 ) {
      if (p1.distance > p2.distance) {
	 return true;
      }
      if (p1.distance == p2.distance) { 
	 if (p1.y > p2.y)  {
	    return true;
	 }
	 if (p1.y == p2.y) {
	    if (p1.x > p2.x) {
	       return true;
	    }
	 }
      }
      return false;
   }
};

template<typename T>
inline int config( const Img2d<T> & im, int x, int y, const int & width, const int & height ) {
   int cnf = 0;
   if (y < height - 1 && im[y + 1][x]) {
      cnf |= 1;
   }
   if (x < width - 1 && y < height - 1 && im[y + 1][x + 1]) {
      cnf |= 2;
   }
   if (x < width - 1 && im[y][x + 1]) {
      cnf |= 4;
   }
   if (x < width-1 && y>0 && im[y - 1][x + 1]) {
      cnf |= 8;
   }
   if (y > 0 && im[y - 1][x]) {
      cnf |= 16;
   }
   if (x > 0 && y > 0 && im[y - 1][x - 1]) {
      cnf |= 32;
   }
   if (x > 0 && im[y][x - 1]) { 
      cnf |= 64;
   }
   if (x > 0 && y < height - 1 && im[y + 1][x - 1]) {
      cnf |= 128;
   }
   return cnf;
}

template<typename T>
Errc PSkeletonization( int adjacency, Img2d<T> &ims, Img2d<T> &imd ) {
   if (adjacency != 4 && adjacency != 8) {
      std::cerr << "pkeletonization (2d): error: adjacency must be either 4 or 8\n";
      exit(1);
   }
   
   // Build the Euclidean distance map
   Img2dsf dmap;
   T * p = ims.Vector();
   T * p2 = imd.Vector();
   T * end = p + ims.VectorSize();
   while (p != end) {
      (*p) = (*p) ? ((*p2 = 128), 0) : ((*p2 = 0), 128);
      ++p;
      ++p2;
   }
   dmap.New(ims.Size());
   PDistance(ims, dmap);

   // Compute a thick medial axis (terminal pixels for the next step)
   Img2duc terminal;
   terminal.New(ims.Size());
   std::memset(terminal.Vector(),0,sizeof(Uchar)*terminal.VectorSize());
   const Long width = ims.Width();
   const Long height = ims.Height();
   for ( Long y = 0; y < height; ++y ) 
      for ( Long x = 0; x < width; ++x ) {
	 const Float value = dmap[y][x];
	 if ( value ) {
	    // tl tt tr
	    // ll    rr
	    // bl bb br
	    const Float tl = (x>0 && y>0)?dmap[y-1][x-1]:Infinity;
	    const Float tt = (y>0)?dmap[y-1][x]:Infinity;
	    const Float tr = (x<(width-1) && y>0)?dmap[y-1][x+1]:Infinity;
	    const Float ll = (x>0)?dmap[y][x-1]:Infinity;
	    const Float rr = (x<width-1)?dmap[y][x+1]:Infinity;
	    const Float bl = (x>0 && y<(height-1))?dmap[y+1][x-1]:Infinity;
	    const Float bb = (y<(height-1))?dmap[y+1][x]:Infinity;
	    const Float br = (x<(width-1) && y<(height-1))?dmap[y+1][x+1]:Infinity;
	    if ( ( value > tt && value > bb ) ||
		 ( value > ll && value > rr ) ||
		 ( value > bl && value > tr ) ||
		 ( value > tl && value > br ) ) {
	       terminal[y][x] = 255;
	    }
	 }
      }

   // Thinning the medial axis
   const int directions[] = { NorthBit, SouthBit, WestBit, EastBit };
   bool stable = false;
   int cnf; 
   // First, a directionnal thinning using (4,8)
   while (!stable) {
      for ( int i = 0; (stable = true) && i < 4; ++i ) {
	 int direction = directions[i];      
	 for ( Long y = 0; y < height; ++y ) {
	    for ( Long x = 0; x < width; ++x ) {
	       if ( terminal[y][x] ) { 
		  cnf = config(terminal, x, y, width, height);
		  if ( ( (cnf|direction) != cnf) &&
		       !extremity4(cnf)  &&
		       !extremity8(cnf)  &&
		       Simple4[cnf] ) {
		     terminal[y][x] = 0;
		     stable = false;
		  }
	       }
	    }
	 }
      }
   }
   // Second, a directionnal thinning using (8,4) if needed
   if ( adjacency == 8 ) {
      stable = false;
   }
   while (!stable)
      for ( int i = 0; (stable = true) && i < 4; ++i ) {
	 int direction = directions[i];      
	 for ( Long y = 0; y < height; ++y ) 
	    for ( Long x = 0; x < width; ++x )
	       if ( terminal[y][x] ) { 
		  cnf = config(terminal, x, y, width, height);
		  if ( ( (cnf|direction) != cnf) &&
		       !extremity4(cnf)  &&
		       !extremity8(cnf)  &&
		       Simple8[cnf] ) {
		     terminal[y][x] = 0;
		     stable = false;
		  }
	       }
      }
   
   //
   // Eventually, a distance driven thinning with medial axis as terminal points
   // 

   // Build the set of border points
   // Pixels that are in the border set have the bit 6(64) set.
   typedef std::priority_queue<PointDistance, std::vector<PointDistance>, PointDistanceGreater> PointDistanceHeap;
   PointDistanceHeap border;
   PointDistanceHeap newBorder;
   const char * simpleTest = (adjacency==8)?Simple8:Simple4;
   const char * borderTest = (adjacency==8)?Border8:Border4;
   const int * neighbors_dx = (adjacency==8)?Neighbors_dx4:Neighbors_dx8;
   const int * neighbors_dy = (adjacency==8)?Neighbors_dy4:Neighbors_dy8;
   const int neighbors_count = (adjacency==8)?4:8;
   for ( Long y = 0; y < height; ++y ) 
      for ( Long x = 0; x < width; ++x ) {
	 if ( imd[y][x] && borderTest[cnf=config(imd,x,y,width,height)] ) {
	    border.push(PointDistance(x,y,dmap[y][x]));
	    imd[y][x] |= 64;
	 }
      }


   stable = false;
   while (!stable) {
      stable = true;
      PointDistance borderPixel;
      while (! border.empty()) {
	 borderPixel = border.top();
	 border.pop();
	 const int & x = borderPixel.x;
	 const int & y = borderPixel.y;
	 cnf=config(imd, x, y, width, height);
	 if ( !terminal[y][x] && simpleTest[cnf] ) {
	    imd[y][x] = 0;
	    stable = false;
	    // Insert new border pixels
	    for ( int i = 0; i < neighbors_count; ++i ) {
	       const int nx = x + neighbors_dx[i];
	       const int ny = y + neighbors_dy[i];
	       if ( imd.Hold( ny, nx ) &&
		    (imd[ny][nx] & 192) == 128 &&
		    borderTest[config(imd, x, y, width, height)] ) {
		  // This neighbor is a new border pixel that is not already in the heap.
		  border.push(PointDistance(nx, ny, dmap[ny][nx]));
		  imd[ny][nx] |= 64;
	       }
	    }
	 } else {
	    newBorder.push(borderPixel);
	 }
      }
      while (! newBorder.empty())  {
	 border.push(newBorder.top());
	 newBorder.pop();
      }
   }
   return SUCCESS;
}

#ifdef MAIN

/*
 * Modify only the following constants, and the function call.
 */
#define	USAGE	"usage: %s connexity [-m mask] [im_in|-] [im_out|-]"
#define	PARC	1
#define	FINC	1
#define	FOUTC	1
#define	MASK	1

int main( int argc, char *argv[] ) {
   Errc result;                // The result code of the execution.
   Pobject* mask;              // The region mask.
   Pobject* objin[FINC + 1];   // The input objects.
   Pobject* objs[FINC + 1];    // The source objects masked by the mask.
   Pobject* objout[FOUTC + 1]; // The output object.
   Pobject* objd[FOUTC + 1];   // The result object of the execution.
   char* parv[PARC + 1];       // The input parameters.
  
   ReadArgs(argc, argv, PARC, FINC, FOUTC, &mask, objin, objs, objout, objd, parv, USAGE, MASK); 
  
   switch(objs[0]->Type()) {
   case Po_Img2duc :{
      Img2duc* const ims = (Img2duc*)objs[0];
      objd[0] = new Img2duc(ims->Size());
      Img2duc* const imd = (Img2duc*)objd[0];
    
      result = PSkeletonization(atoi(parv[0]),*ims,*imd);
   }break;
   case Po_Img2dsl :{
      Img2dsl* const ims = (Img2dsl*)objs[0];
      objd[0] = new Img2dsl(ims->Size());
      Img2dsl* const imd = (Img2dsl*)objd[0];
    
      result = PSkeletonization(atoi(parv[0]), *ims, *imd);
   }break;
   default :
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
