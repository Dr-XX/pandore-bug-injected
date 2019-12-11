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
using namespace pandore;

Errc PSeedPlacement2D( Reg2d &rgd, int dx, int dy ) {
   int w = rgd.Width();
   int h = rgd.Height();
   int x = 0;
   int y = 0;

   // init rgd 
   rgd = 0;

   if (dy <= 0) {
      // si dy==0, alors on place (approximativement) 'dx' seeds
      // sur une grille reguliere
      int K = dx;

      int superpixelsize = (int)(0.5 + double(w * h) / double(K));
      int STEP = (int)(sqrt(double(superpixelsize)) + 0.5);

      int xstrips =  (int)(0.5 + double(w) / double(STEP));
      int ystrips =  (int)(0.5 + double(h) / double(STEP));

      int xerr =  w - STEP*xstrips;
      if (xerr < 0) {
	 xstrips--;
	 xerr =  w - STEP * xstrips;
      }
      int yerr =  h - STEP * ystrips;
      if (yerr < 0) {
	 ystrips--;
	 yerr =  h - STEP * ystrips;
      }

      double xerrperstrip =  double(xerr) / double(xstrips);
      double yerrperstrip =  double(yerr) / double(ystrips);

      int xoff =  STEP / 2;
      int yoff =  STEP / 2;
      
      int N = 1;
      for ( int y =  0; y < ystrips; y++ ) {
	 int ye =  (int)(y * yerrperstrip);
	 for  ( int x =  0; x < xstrips; x++ ) {
	    int xe =  (int)(x * xerrperstrip);
	    rgd(y * STEP + yoff+ye, x * STEP + xoff+xe) = N++;
	 }
      }
      rgd.Labels(N - 1);

   } else {
      // si dy>0, alors on place les seeds avec les offsets dx et dy
      int hdx = dx / 2;
      int hdy = dy / 2;

      unsigned long N = 1;
      for (y = hdy; y < h; y += dy) {
	 for (x = hdx; x < w; x += dx) {
	    rgd(y, x) = N++;
	 }
      }
      rgd.Labels(N - 1);
   }


   return SUCCESS;
}

Errc PSeedPlacement3D( Reg3d &rgd, int dx, int dy, int dz ) {
   int w = rgd.Width();
   int h = rgd.Height();
   int d = rgd.Depth();
   int x = 0;
   int y = 0;
   int z = 0;

   // init rgd 
   rgd = 0;

   if (dz == 0) { 
      if (dy == 0) {
	 // si dz = = 0 ET dy = = 0, alors on place (approximativement)
	 // 'dx' seeds sur une grille reguliere
	 int K = dx;

	 int superpixelsize = (int)(0.5 + double(w * h * d) / double(K));
	 int STEP = (int)(sqrt(double(superpixelsize)) + 0.5);
	 STEP = (int)(pow(double(superpixelsize), 1.0 / 3));

	 int xstrips = (int)(0.5 + double(w) / double(STEP));
	 int ystrips = (int)(0.5 + double(h) / double(STEP));
	 int zstrips = (int)(0.5 + double(d) / double(STEP));

	 int xerr =  w - STEP * xstrips;
	 if (xerr < 0) {
	    xstrips--;
	    xerr =  w - STEP * xstrips;
	 }
	 int yerr =  h - STEP * ystrips;
	 if (yerr < 0) {
	    ystrips--;
	    yerr =  h - STEP * ystrips;
	 }
	 int zerr =  d - STEP * zstrips;
	 if (zerr < 0) {
	    zstrips--;
	    zerr =  d - STEP * zstrips;
	 }

	 double xerrperstrip =  double(xerr) / double(xstrips);
	 double yerrperstrip =  double(yerr) / double(ystrips);
	 double zerrperstrip =  double(zerr) / double(zstrips);

	 int xoff =  STEP / 2;
	 int yoff =  STEP / 2;
	 int zoff =  STEP / 2;

	 if (STEP > w / 2.) {
	    xoff = (int)(w / 2.);
	 }
	 if (STEP > h / 2.) {
	    yoff = (int)(h / 2.);
	 }
	 if (STEP > d / 2.) {
	    zoff = (int)(d / 2.);
	 }

	 unsigned long N = 1;
	 for (int z =  0; z < zstrips; z++ ) {
	    int ze =  (int)(z * zerrperstrip);
	    for (int y =  0; y < ystrips; y++ ) {
	       int ye =  (int)(y * yerrperstrip);
	       for (int x =  0; x < xstrips; x++ ) {
		  int xe =  (int)(x * xerrperstrip);
		  rgd(z * STEP + zoff+ze, y * STEP + yoff + ye, x * STEP + xoff + xe) = N++;
	       }
	    }
	 }
	 rgd.Labels(N-1);
      } else {
	 // si dz == 0 ET dy>0, alors on place les seeds avec les
	 // offsets dx et dy sur CHAQUE plan
	 int hdx = dx / 2;
	 int hdy = dy / 2;
	 int N = 1;
	 for (z = 0; z < d; z++) {
	    for (y = hdy; y < h; y += dy) {
	       for (x = hdx;x<w; x += dx) {
		  rgd(z, y, x) = N++;
	       }
	    }
	 }
	 rgd.Labels(N-1);
      }
   } else {
      // si dx>0, dy>0 et dz>0, alors on place les seeds avec les
      // offsets dx, dy et dz
	 int hdx = dx / 2;
	 int hdy = dy / 2;
	 int hdz = dz / 2;
	 int N = 1;
	 for (z = hdz; z < d; z += dz) {
	    for (y = hdy; y < h; y += dy) {
	       for (x = hdx; x < w; x += dx) {
		  rgd(z, y, x) = N++;
	       }
	    }
	 }
	 rgd.Labels(N-1);
   }
   
   return SUCCESS;
}

#ifdef MAIN
#define USAGE   "usage: %s dx dy dz [im_in|-] [rg_out|-]"
#define PARC    3
#define FINC    1
#define FOUTC   1
#define MASK    0

int main( int argc, char* argv[] ) {
   Errc result;			// The result code of the execution.
   Pobject *mask;		// The mask.
   Pobject *objin[FINC + 1];	// The input objects;
   Pobject *objs[FINC + 1];	// The source objects masked by mask.
   Pobject *objout[FOUTC + 1];	// The output object unmasked by mask.
   Pobject *objd[FOUTC + 1];	// The result object.
   char *parv[PARC + 1];        // The input parameters.
  
   ReadArgs(argc, argv, PARC, FINC, FOUTC, &mask, objin, objs, objout, objd, parv, USAGE, MASK);

   int dx = atoi(parv[0]);
   int dy = atoi(parv[1]);
   int dz = atoi(parv[2]);

   if ((objs[0]->Type() == Po_Img2duc)
      || (objs[0]->Type() == Po_Img2dsl)
      || (objs[0]->Type() == Po_Img2dsf)
      || (objs[0]->Type() == Po_Imc2duc)
      || (objs[0]->Type() == Po_Imc2dsl)
      || (objs[0]->Type() == Po_Imc2dsf)
      || (objs[0]->Type() == Po_Imx2duc)
      || (objs[0]->Type() == Po_Imx2dsl)
      || (objs[0]->Type() == Po_Imx2dsf) ) {
      Imx2duc* const ims = (Imx2duc*)objs[0];
      objd[0] = new Reg2d(ims->Props());
      Reg2d* const rgd = (Reg2d*)objd[0];
      result = PSeedPlacement2D(*rgd, dx, dy);
   }
   
   if ((objs[0]->Type() == Po_Img3duc)
       || (objs[0]->Type() == Po_Img3dsl)
       || (objs[0]->Type() == Po_Img3dsf)
       || (objs[0]->Type() == Po_Imc3duc)
       || (objs[0]->Type() == Po_Imc3dsl)
       || (objs[0]->Type() == Po_Imc3dsf)
       || (objs[0]->Type() == Po_Imx3duc)
       || (objs[0]->Type() == Po_Imx3dsl)
       || (objs[0]->Type() == Po_Imx3dsf) ) {
      Imx3duc* const ims = (Imx3duc*)objs[0];
      objd[0] = new Reg3d(ims->Props());
      Reg3d* const rgd = (Reg3d*)objd[0];
      result = PSeedPlacement3D(*rgd, dx, dy, dz);
   }

   if (result) {
      WriteArgs(argc, argv, PARC, FINC, FOUTC, &mask, objin, objs, objout, objd, MASK);
   }
   Exit(result);
   return 0;
}

#endif
