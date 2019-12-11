/*
Copyright (C) 2006 Pedro Felzenszwalb

This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

Efficient Graph-Based Image Segmentation
Pedro F. Felzenszwalb and Daniel P. Huttenlocher
International Journal of Computer Vision, 59(2) September 2004.
*/

#include <cstdio>
#include <cstdlib>
#include "graphbasedsegmentation/image.h"
#include "graphbasedsegmentation/misc.h"
#include "graphbasedsegmentation/segment-image.h"

#ifdef MAIN
#undef MAIN
#include "segmentation/plabeling.cpp"
#define MAIN
#else
#include "segmentation/plabeling.cpp"
#endif

#include <pandore.h>
using namespace pandore;

image<rgb> * convertPandoreToRGB( const Imc2duc &ims ) {
   image<rgb> *im = new image<rgb>(ims.Height(), ims.Width(), false);
  
   for (int y = 0; y < ims.Height(); y++) {
      for (int x = 0; x < ims.Width(); x++) {
	 imRef(im, y, x).r = ims(0, y, x);
	 imRef(im, y, x).g = ims(1, y, x);
	 imRef(im, y, x).b = ims(2, y, x);
      }
   }
   
   return im;
}

void
convertLabelToPandore( image<rgb> *seg, Imc2duc &imd ) {
   for (int y = 0; y < imd.Height(); y++) {
      for (int x = 0; x < imd.Width(); x++) {
	 imd(0, y, x) = imRef(seg, y, x).r;
	 imd(1, y, x) = imRef(seg, y, x).g;
	 imd(2, y, x) = imRef(seg, y, x).b;
      }
   }
}

Errc
PGraphBasedSegmentation( const Imc2duc &ims, Reg2d &rgd, float sigma, float threshold, int minimumRegionArea ) {
   image<rgb> *input = convertPandoreToRGB(ims);
   
   Imc2duc imi(ims.Props());
   int num_ccs;

   image<rgb> *seg = segment_image(input, sigma, threshold, minimumRegionArea, &num_ccs); 
   
   convertLabelToPandore(seg, imi);
   Errc result = PLabeling(imi, rgd, 8);
   
   return result;
}

#ifdef MAIN
#define	USAGE	"usage: %s sigma k minimum-region-area [-m mask] [im_in|-] [rg_out|-]"
#define	PARC	3
#define	FINC	1
#define	FOUTC	1
#define	MASK	3

int main( int argc, char *argv[] ) {
   Errc result;              // The result code of the execution.
   Pobject* mask;            // The region map.
   Pobject* objin[FINC+1];   // The input objects.
   Pobject* objs[FINC+1];    // The source objects masked.
   Pobject* objout[FOUTC+1]; // The output object.
   Pobject* objd[FOUTC+1];   // The result object of the execution.
   char* parv[PARC+1];       // The input parameters.
   
   ReadArgs(argc, argv, PARC, FINC, FOUTC, &mask, objin, objs, objout, objd, parv, USAGE, MASK);

   if (objs[0]->Type() == Po_Imc2duc) {
      Imc2duc* const ims = (Imc2duc*)objs[0];
      objd[0] = new Reg2d(ims->Props());
      Reg2d* rgd = (Reg2d*)objd[0];

      result = PGraphBasedSegmentation(*ims, *rgd, (float)atof(parv[0]), (float)atof(parv[1]), atoi(parv[2]));
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
