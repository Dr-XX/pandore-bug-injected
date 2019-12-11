/* -*- c-basic-offset: 3 -*-
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

#include <math.h>
#include <pandore.h>
using namespace pandore;

/**
 * @file psusan.cpp
 * Corner detection using susan algorithm.
 *
 * Reference : Smith, S.M. and Brady, J.M., "Susan, A New Approach to Low Level
 * Image Processing", Int. Journal of Computer Vision", 45--78, 23(1), 1997.
 */

#define MAX_CORNERS   32000  /* max corners per frame */

struct corner {
   Point2d coord;
   int info;
};

/*
 * The Susan brightness function is implemented as a LUT for speed.
 * This mallocs enough space for *bp and then repositions the pointer
 * to the centre of the malloced space.
 * The SUSAN function e^-(x^6) or e^-(x^2) is calculated and converted to
 * a Uchar in the range 0-100, for all possible image brightness differences
 * (including negative ones). Thus bp[23] is the output for a brightness difference
 * of 23 greylevels.
 */
void setup_brightness_lut( Uchar **bp, int bt, int form ) {
  float temp;
  
  *bp = (Uchar*)malloc(516);
  *bp = *bp + 258;
  
  for (int k = -256; k < 257; k++) {
    temp = ((float)k) / ((float)bt);
    temp = temp * temp;
    if (form == 6) {
      temp = temp * temp * temp;
    }
    temp = 100.0F * exp(-temp);
    *(*bp + k) = (Uchar)temp;
  }
}

/**
 * Susan algorithm.
 * 1. Place a circular mask around the pixel in question (the nucleus).
 * 2. Using Brightness_lut calculate the number of pixels within the circular
 *    mask which have similar brightness to the nucleus. (These pixels define the USAN.)
 * 3. Threshold USAN to produce a corner strength image.
 * 4. Use non-maximum suppression to find corners.
 */
Errc PSusan( const Img2duc &ims, Img2dsl &imd, int bt ) {
   Uchar *bp;
   int max_no_corners = 1850;
   int x, y, v, n;
   struct corner corner_list[MAX_CORNERS];
   
   Img2dsl r(ims.Size());
   
   setup_brightness_lut(&bp, bt, 6);

   r.Frame(0, 3, 3);
   for (y = 3; y < ims.Height() - 3; y++) {
      for (x = 3; x < ims.Width() - 3; x++) {
	 n = 100;
	 
	 n += bp[ims[y][x] - ims[y - 3][x - 1]];
	 n += bp[ims[y][x] - ims[y - 3][x]];
	 n += bp[ims[y][x] - ims[y - 3][x + 1]];      
	 
	 n += bp[ims[y][x] - ims[y - 2][x - 2]];
	 n += bp[ims[y][x] - ims[y - 2][x - 1]];
	 n += bp[ims[y][x] - ims[y - 2][x]];
	 n += bp[ims[y][x] - ims[y - 2][x + 1]];
	 n += bp[ims[y][x] - ims[y - 2][x + 2]];
	 
	 n += bp[ims[y][x] - ims[y - 1][x - 3]];
	 n += bp[ims[y][x] - ims[y - 1][x - 2]];
	 n += bp[ims[y][x] - ims[y - 1][x - 1]];
	 n += bp[ims[y][x] - ims[y - 1][x]];
	 n += bp[ims[y][x] - ims[y - 1][x + 1]];
	 n += bp[ims[y][x] - ims[y - 1][x + 2]];
	 n += bp[ims[y][x] - ims[y - 1][x + 3]];
	 
	 n += bp[ims[y][x] - ims[y][x - 3]];
	 n += bp[ims[y][x] - ims[y][x - 2]];
	 n += bp[ims[y][x] - ims[y][x - 1]];
	 n += bp[ims[y][x] - ims[y][x]];
	 n += bp[ims[y][x] - ims[y][x + 1]];
	 n += bp[ims[y][x] - ims[y][x + 2]];
	 n += bp[ims[y][x] - ims[y][x + 3]];
	 
	 n += bp[ims[y][x] - ims[y + 1][x - 3]];
	 n += bp[ims[y][x] - ims[y + 1][x - 2]];
	 n += bp[ims[y][x] - ims[y + 1][x - 1]];
	 n += bp[ims[y][x] - ims[y + 1][x]];
	 n += bp[ims[y][x] - ims[y + 1][x + 1]];
	 n += bp[ims[y][x] - ims[y + 1][x + 2]];
	 n += bp[ims[y][x] - ims[y + 1][x + 3]];
	 
	 n += bp[ims[y][x] - ims[y + 2][x - 2]];
	 n += bp[ims[y][x] - ims[y + 2][x - 1]];
	 n += bp[ims[y][x] - ims[y + 2][x]];
	 n += bp[ims[y][x] - ims[y + 2][x + 1]];
	 n += bp[ims[y][x] - ims[y + 2][x + 2]];
	 
	 n += bp[ims[y][x] - ims[y + 3][x - 1]];
	 n += bp[ims[y][x] - ims[y + 3][x]];
	 n += bp[ims[y][x] - ims[y + 3][x + 1]];      
	 
	 if (n < max_no_corners) {
	    r[y][x] = max_no_corners - n;
	 } else {
	    r[y][x] = 0;
	 }
      }
   }
   
   /* to locate the local maxima */
   n = 0;
   for (y = 3; y < ims.Height() - 3; y++) {
      for (x = 3; x <ims.Width() - 3; x++) {
	 v = r[y][x];
	 if ((v>0) && (v>r[y - 3][x - 3]) && (v>r[y - 3][x - 2]) && (v>r[y - 3][x - 1]) && (v>r[y - 3][x]) &&
	     (v>r[y - 3][x + 1]) && (v>r[y - 3][x + 2]) && (v>r[y - 3][x + 3]) &&
	     
	     (v>r[y - 2][x - 3]) && (v>r[y - 2][x - 2]) && (v>r[y - 2][x - 1]) && (v>r[y - 2][x  ]) &&
	     (v>r[y - 2][x + 1]) && (v>r[y - 2][x + 2]) && (v>r[y - 2][x + 3]) &&
	     
	     (v>r[y - 1][x - 3]) && (v>r[y - 1][x - 2]) && (v>r[y - 1][x - 1]) && (v>r[y - 1][x  ]) &&
	     (v>r[y - 1][x + 1]) && (v>r[y - 1][x + 2]) && (v>r[y - 1][x + 3]) &&
	     
	     (v>r[y][x - 3]) && (v>r[y][x - 2]) && (v>r[y][x - 1]) &&
	     (v >= r[y][x + 1]) && (v >= r[y][x + 2]) && (v >= r[y][x + 3]) &&
	     
	     (v >= r[y+1][x - 3]) && (v >= r[y+1][x - 2]) && (v >= r[y+1][x - 1]) && (v >= r[y+1][x  ]) &&
	     (v >= r[y+1][x + 1]) && (v >= r[y+1][x + 2]) && (v >= r[y+1][x + 3]) &&
	     
	     (v >= r[y+2][x - 3]) && (v >= r[y+2][x - 2]) && (v >= r[y+2][x - 1]) && (v >= r[y+2][x  ]) &&
	     (v >= r[y+2][x + 1]) && (v >= r[y+2][x + 2]) && (v >= r[y+2][x + 3]) &&
	     
	     (v >= r[y + 3][x - 3]) && (v >= r[y + 3][x - 2]) && (v >= r[y + 3][x - 1]) &&
	     (v >= r[y + 3][x  ]) && (v >= r[y + 3][x + 1]) && (v >= r[y + 3][x + 2]) && (v >= r[y + 3][x + 3]) ) {
	    corner_list[n].info = 0;
	    corner_list[n].coord.x = x;
	    corner_list[n].coord.y = y;
	    n++;
	    if (n == MAX_CORNERS) {
	       std::cerr << "Error psusan: Too many corners" << std::endl;
	       return FAILURE;
	    }
	 }
      }
   }
   corner_list[n].info = 7; // This is the end.
   
   imd = 0;
   for (n = 0; corner_list[n].info != 7; n++) {
      imd[corner_list[n].coord] = r[corner_list[n].coord];
   }
   
   return SUCCESS;
}

#ifdef MAIN
#define	USAGE	"usage: %s brightness [-m mask] [im_in|-] [im_out|-]"
#define	PARC	1
#define	FINC	1
#define	FOUTC	1
#define	MASK	1

int main( int argc, char *argv[] ) {
   Errc result;                // The result code of the execution.
   Pobject* mask;              // The region map.
   Pobject* objin[FINC  +1];   // The input objects.
   Pobject* objs[FINC + 1];    // The source objects masked.
   Pobject* objout[FOUTC + 1]; // The output object.
   Pobject* objd[FOUTC + 1];   // The result object of the execution.
   char* parv[PARC + 1];       // The input parameters.

   ReadArgs(argc, argv, PARC, FINC, FOUTC, &mask, objin, objs, objout, objd, parv, USAGE, MASK);
   if (objs[0]->Type() == Po_Img2duc) {
     Img2duc* const ims = (Img2duc*)objs[0];
     objd[0] = new Img2dsl(ims->Size());
     Img2dsl * const imd = (Img2dsl*)objd[0];
     
     result = PSusan(*ims, *imd, atoi(parv[0]));
     goto end;
   }
   {
     PrintErrorFormat(objin, FINC);
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
