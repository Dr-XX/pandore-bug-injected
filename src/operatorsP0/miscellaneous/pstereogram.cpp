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

/**
 * @author Régis Clouard (Inspired from W.A. Sterer) - 1999-11-19
 */

#include <pandore.h>
using namespace pandore;

/**
 * @file pstereogram.cpp
 * Build a stereogram from tile and depth images.
 */

#define INCH 2.54
#define DIST 55 *2.54
#define EYE 6

typedef struct {
   int R;
   int V;
   int B;
} COLOUR;

/**
 * Inputs: depth image
 *	 : pattern image
 * Output: stereogram image
 */
Errc PStereogram( const Img2duc &imdepth, Imc2duc &impattern, Imc2duc &imstereo ) {
   // Observation condition
   int xdpi = 75; int ydpi = 75;		// Resolution of typical monitor
   int eyeSep = (int)(xdpi * 6 / INCH);	// Supposed 6 cm from screen
   int obsDist = (int)(xdpi * 55 / INCH);	// Supposed 55 cm from screen.

   // Deduce maxdepth from pattern size.
   int maxdepth = (int)((impattern.Width() * obsDist) / (eyeSep - impattern.Width()));

   const int maxwidth = imdepth.Width() * 6;  // allow space for up to 6 times oversampling

   int yShift = ydpi / 16;

   int *lookL = new int[maxwidth];
   int *lookR = new int[maxwidth];
   COLOUR *colour = new COLOUR[maxwidth];
   COLOUR col;
   int lastlinked; int i;

   int oversam = 4;			// oversampling ratio  
   int vwidth = imdepth.Width() * oversam;	//the new 'virtual' pixels.

   int veyeSep = eyeSep * oversam;		// eye separation in 'virtual' pixels.

   // pattern must be at least this wide
   int maxsep = (int)(((long)eyeSep * maxdepth) / (maxdepth + obsDist));
   int vmaxsep = oversam * maxsep; // maximum stereo separation in 'virtual' pixels.
    
   int s = vwidth / 2 - vmaxsep / 2;
   int poffset = vmaxsep-(s % vmaxsep);
  
   int featureZ, sep = 0;
   int x, y, left, right;
   bool vis;

   if (maxsep > impattern.Width()) {
      return FAILURE;
   }

   // Working line by line.
   for (y = 0; y < imdepth.Height(); y++) {

      // Init : Each point is linked to itself, purely to indicate that
      // is not linked to any other.
      for (x = 0; x < vwidth; x++) {
	 lookL[x] = lookR[x] = x;
      }

      // Identify the associated pair of screen point from the depth image.
      // Resolution enhancement. Use virtual pixel: (oversam pixels between two real pixels).
      for (x = 0; x < vwidth; x++) {
	 if ((x % oversam) == 0) { // SPEEDUP for oversampled pictures
	    featureZ = maxdepth - (int)imdepth[y][(x / oversam)];
	    sep = (int)(((long)veyeSep * featureZ) / (featureZ + obsDist));
	 }

	 // Resulting left and right links from depth at point x, y.
	 left = x - sep / 2; right = left + sep;
	 vis = true;

	 if ((left >= 0) && (right < vwidth)) {
	    if (lookL[right] != right) {		// right point already linked
	       if (lookL[right] < left) {		// deeper than current (so hide previous point)
		  lookR[lookL[right]] = lookL[right];	// break old links.
		  lookL[right] = right;			// unset: lookL[x]=x;
	       } else {
		  vis = false;				// Visible.
	       }
	    }
	
	    if (lookR[left] != left) {			// left point already linked
	       if (lookR[left] > right) {		// deeper than current (so hide previous point)
		  lookL[lookR[left]] = lookR[left];	// break old links.
		  lookR[left] = left;			// unset: lookR[x]=x;
	       } else {
		  vis = false;
	       }
	    }

	    if (vis) {				// make link.
	       lookL[right] = left;
	       lookR[left] = right;
	    }
	 }
      }
 
      // Plotting processes using bitmapped pattern.

      // Left to right
      lastlinked = -10; // dummy initial value
      for (x = s; x < vwidth; x++) {
	 // If the pixel is unlinked or link to a pixel out of the image.
	 if ((lookL[x] == x) || (lookL[x] < s)) {
	    // Take the previous color
	    if (lastlinked == (x - 1)) {
	       colour[x] = colour[x - 1];
	       // or take the original colour of the pattern.
	    } else {
	       colour[x].R = impattern.X[y % impattern.Height()][((x + poffset) % vmaxsep) / oversam];
	       colour[x].V = impattern.Y[y % impattern.Height()][((x + poffset) % vmaxsep) / oversam];
	       colour[x].B = impattern.Z[y % impattern.Height()][((x + poffset) % vmaxsep) / oversam];
     
	    }
	 } else {
	    colour[x] = colour[lookL[x]];	
	    lastlinked = x; // keep track of the last pixel to be constrained
	 }
      }
    
      // Right to left
      lastlinked = -10;	// dummy initial value
      for (x = s - 1; x >= 0; x--) {
	 if (lookR[x] == x) {
	    if (lastlinked == (x + 1)) {
	       colour[x] = colour[x + 1];
	    } else {
	       colour[x].R = impattern.X[(y + ((s - x) / vmaxsep + 1) * yShift) % impattern.Height()][((x  +poffset) % vmaxsep) / oversam];
	       colour[x].V = impattern.Y[(y + ((s - x) / vmaxsep + 1) * yShift) % impattern.Height()][((x + poffset) % vmaxsep) / oversam];
	       colour[x].B = impattern.Z[(y + ((s - x) / vmaxsep + 1) * yShift) % impattern.Height()][((x + poffset) % vmaxsep) / oversam];
	    }
	 } else {
	    colour[x] = colour[lookR[x]];
	
	    lastlinked = x; // keep track of the last pixel to be constrained
	 }
      }
    
      // Plot the resulting pattern in the output image.
      int red, green, blue;
      for (x = 0; x < vwidth; x += oversam) {
	 red = 0; green = 0; blue = 0;
	
	 // use average colour of virtual pixels for screen pixel
	 for (i = x; i < (x + oversam); i++) {
	    col = colour[i];
	    red += col.R;
	    green += col.V;
	    blue += col.B;
	 }
	 imstereo.X[y][x / oversam] = red / oversam;
	 imstereo.Y[y][x / oversam] = green / oversam;
	 imstereo.Z[y][x / oversam] = blue / oversam;
      }
   }
   delete [] lookL;
   delete [] lookR; 
   delete [] colour;
   return SUCCESS;
}

/**
 * Inputs: depth image
 *	 : pattern image
 * Output: stereogram image
 */
Errc PStereogram( const Img2dsl &imdepth, Imc2duc &impattern, Imc2duc &imstereo ) {
   // Observation condition
   int xdpi = 75; int ydpi = 75;		// Resolution of typical monitor
   int eyeSep = (int)(xdpi * 6 / INCH);	// Supposed 6 cm from screen
   int obsDist = (int)(xdpi * 55 / INCH);	// Supposed 55 cm from screen.

   // Deduce maxdepth from pattern size.
   int maxdepth = (int)((impattern.Width() * obsDist) / (eyeSep - impattern.Width()));

   const int maxwidth = imdepth.Width() * 6;  // allow space for up to 6 times oversampling

   int yShift = ydpi / 16;

   int *lookL= new int[maxwidth];
   int *lookR= new int[maxwidth];
   COLOUR *colour = new COLOUR[maxwidth];
   COLOUR col;
   int lastlinked;
   int i;
   
   int oversam = 4;			// oversampling ratio  
   int vwidth = imdepth.Width() * oversam;	//the new 'virtual' pixels.

   int veyeSep = eyeSep * oversam;		// eye separation in 'virtual' pixels.

   // pattern must be at least this wide
   int maxsep = (int)(((long)eyeSep  *maxdepth) / (maxdepth + obsDist));
   int vmaxsep = oversam * maxsep; // maximum stereo separation in 'virtual' pixels.
    
   int s = vwidth / 2 - vmaxsep / 2;
   int poffset = vmaxsep - (s % vmaxsep);
  
   int featureZ, sep = 0;
   int x, y, left, right;
   bool vis;

   if (maxsep > impattern.Width()) {
      return FAILURE;
   }
   // Working line by line.
   for (y = 0; y < imdepth.Height(); y++) {

      // Init : Each point is linked to itself, purely to indicate that
      // is not linked to any other.
      for (x = 0; x < vwidth; x++) {
	 lookL[x] = lookR[x] = x;
      }

      // Identify the associated pair of screen point from the depth image.
      // Resolution enhancement. Use virtual pixel: (oversam pixels between two real pixels).
      for (x = 0; x < vwidth; x++) {
	 if ((x % oversam) == 0) { // SPEEDUP for oversampled pictures
	    featureZ = maxdepth - (int)imdepth[y][(x / oversam)];
	    sep = (int)(((long)veyeSep * featureZ) / (featureZ + obsDist));
	 }

	 // Resulting left and right links from depth at point x, y.
	 left = x - sep / 2; right = left + sep;
	 vis = true;

	 if ((left >= 0) && (right < vwidth)) {
	    if (lookL[right] != right) {		// right point already linked
	       if (lookL[right] < left) {		// deeper than current (so hide previous point)
		  lookR[lookL[right]] = lookL[right];	// break old links.
		  lookL[right] = right;			// unset: lookL[x]=x;
	       } else {
		  vis = false;				// Visible.
	       }
	    }
	
	    if (lookR[left] != left) {			// left point already linked
	       if (lookR[left] > right) {		// deeper than current (so hide previous point)
		  lookL[lookR[left]] = lookR[left];	// break old links.
		  lookR[left] = left;			// unset: lookR[x]=x;
	       } else {
		  vis = false;
	       }
	    }

	    if (vis) {				// make link.
	       lookL[right] = left;
	       lookR[left] = right;
	    }
	 }
      }
 
      // Plotting processes using bitmapped pattern.

      // Left to right
      lastlinked = -10; // dummy initial value
      for (x = s; x < vwidth; x++) {
	 // If the pixel is unlinked or link to a pixel out of the image.
	 if ((lookL[x] == x) || (lookL[x] < s)) {
	    // Take the previous color
	    if (lastlinked == (x - 1)) {
	       colour[x] = colour[x - 1];
	       // or take the original colour of the pattern.
	    } else {
	       colour[x].R = impattern.X[y % impattern.Height()][((x + poffset) % vmaxsep) / oversam];
	       colour[x].V = impattern.Y[y % impattern.Height()][((x + poffset) % vmaxsep) / oversam];
	       colour[x].B = impattern.Z[y % impattern.Height()][((x + poffset) % vmaxsep) / oversam];
     
	    }
	 } else {
	    colour[x] = colour[lookL[x]];	
	    lastlinked = x; // keep track of the last pixel to be constrained
	 }
      }
    
      // Right to left
      lastlinked = -10;	// dummy initial value
      for (x = s - 1; x >= 0; x--) {
	 if (lookR[x] == x) {
	    if (lastlinked == (x + 1)) {
	       colour[x] = colour[x + 1];
	    } else {
	       colour[x].R = impattern.X[(y + ((s - x) / vmaxsep + 1) * yShift) % impattern.Height()][((x + poffset) % vmaxsep) / oversam];
	       colour[x].V = impattern.Y[(y + ((s - x) / vmaxsep + 1)  *yShift) % impattern.Height()][((x + poffset) % vmaxsep) / oversam];
	       colour[x].B = impattern.Z[(y+  ((s-  x) / vmaxsep + 1) * yShift) % impattern.Height()][((x + poffset) % vmaxsep) / oversam];
	    }
	 } else {
	    colour[x] = colour[lookR[x]];
	
	    lastlinked = x; // keep track of the last pixel to be constrained
	 }
      }
    
      // Plot the resulting pattern in the output image.
      int red, green, blue;
      for (x = 0; x < vwidth; x += oversam) {
	 red = 0; green = 0; blue = 0;
	
	 // use average colour of virtual pixels for screen pixel
	 for (i = x; i < (x + oversam); i++) {
	    col = colour[i];
	    red += col.R;
	    green += col.V;
	    blue += col.B;
	 }
	 imstereo.X[y][x / oversam] = red / oversam;
	 imstereo.Y[y][x / oversam] = green / oversam;
	 imstereo.Z[y][x / oversam] = blue / oversam;
      }
   }
   delete [] lookL;
   delete [] lookR; 
   delete [] colour;
   return SUCCESS;
}


#ifdef MAIN

/*
 * Modify only the following constants, and the operateur switches.
 */
#define	USAGE	"usage: %s [im_depth|-] [im_tile|-] [im_out|-]"
#define	PARC	0
#define	FINC	2
#define	FOUTC	1
#define	MASK	0

int main( int argc, char *argv[] ) {
   Errc result;		     // The result code of the execution.
   Pobject* stencil;	     // The region stencil.
   Pobject* objin[FINC+1];   // The input objects.
   Pobject* objs[FINC+1];    // The source objects masked by the stencil.
   Pobject* objout[FOUTC+1]; // The output object.
   Pobject* objd[FOUTC+1];   // The result object of the execution.
   char* parv[PARC+1];	     // The input parameters.

   ReadArgs(argc, argv, PARC, FINC, FOUTC, &stencil, objin, objs, objout, objd, parv, USAGE, MASK);

   switch(objs[0]->Type()) {
   case Po_Img2duc :{ // Image couleur
      Img2duc* const ims1 = (Img2duc*)objs[0];
      Imc2duc* const ims2 = (Imc2duc*)objs[1];
      objd[0] = new Imc2duc(ims1->Size());

      Imc2duc* const imd = (Imc2duc*)objd[0];

      result = PStereogram(*ims1, *ims2, *imd);
   }break;
   case Po_Img2dsl :{ // Image couleur
      Img2dsl* const ims1 = (Img2dsl*)objs[0];
      Imc2duc* const ims2 = (Imc2duc*)objs[1];
      objd[0] = new Imc2duc(ims1->Size());

      Imc2duc* const imd = (Imc2duc*)objd[0];

      result = PStereogram(*ims1, *ims2, *imd);
   }break;
   default:
      PrintErrorFormat(objin, FINC);
      result = FAILURE;
   }

   if (result) {
      WriteArgs(argc, argv, PARC, FINC, FOUTC, &stencil, objin, objs, objout, objd, MASK);
   }

   Exit(result);
   return 0;
}

#endif
