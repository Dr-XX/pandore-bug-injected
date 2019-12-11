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
 * @author David Tschumperlé - 2005-08-30
 */

#define cimg_OS 0
#define cimg_display_type 0
#include "CImg1-16.h"
using namespace cimg_library1_16;
#include <pandore.h>
using namespace pandore;

/**
 * @file panalyze2pan
 * @brief Convert an ANALYZE 7.5 file to Pandore file.
 *
 * Converts an ANALYZE 7.5 image to a suitable Pandore image.
 * @param nomfichier	the filename of the Analyze file.
 * @param objout	the resulting Pandore image.
 */
Errc PAnalyze2Pan( char *nomfichier, Pobject **objout ) {
   CImg<float> img = CImg<float>::get_load_analyze(nomfichier);

   switch (img.dimv()) {
   case 1: {
      *objout = new Img3dsf(img.dimz(), img.dimy(), img.dimx());
      Img3dsf &dest = **((Img3dsf**)objout);
      cimg_mapXYZ(img, x, y, z) dest[z][y][x] = img(x, y, z);
      } break;
   case 3: {
      *objout = new Imc3dsf(img.dimz(), img.dimy(), img.dimx());
      Imc3dsf &dest = **((Imc3dsf**)objout);
      cimg_mapXYZV(img, x, y, z, v) dest[v][z][y][x] = img(x, y, z, v);
      } break;
   default: {
      *objout = new Imx3dsf(img.dimv(), img.dimz(), img.dimy(), img.dimx());
      Imx3dsf &dest = **((Imx3dsf**)objout);
      cimg_mapXYZV(img, x, y, z, v) dest[v][z][y][x] = img(x, y, z, v);
      } break;
   }
   return SUCCESS;
}

#ifdef MAIN

/*
 * Modify only the following constants, and the function calls.
 */
#define	USAGE	"usage: %s [im_in0] im_in [im_out|-]"
#define	PARC	0
#define	FINC	0
#define	FOUTC	1
#define	MASK	0

int main( int argc, char *argv[] ) {
   Pobject* mask;            // The region mask.
   Pobject* objin[FINC+1];   // The input objects.
   Pobject* objs[FINC+1];    // The source objects masked by the mask.
   Pobject* objout[FOUTC+1]; // The output objects.
   Pobject* objd[FOUTC+1];   // The result objects of the execution.
   char* parv[PARC+1];       // The input parameters.
   
   if (argc<2) {
      fprintf(stderr, USAGE, argv[0]);
      Exit((Errc)FAILURE);
   }
   
   if ((argc==2) && (!strcmp(argv[1], "-p"))) {
      fprintf(stdout, "%s 0 1 1\n", argv[0]);
      exit(0);
   }

   if ((argc==2) && (!strcmp(argv[1], "-s"))) {
      fprintf(stdout, "\n");
      exit(0);
   }
 
   ReadArgs(argc, argv, PARC, FINC, FOUTC, &mask, objin, objs, objout, objd, parv, USAGE, MASK);
   
   if (PAnalyze2Pan(((argc>1)? argv[1]:NULL), &objd[0])) {
      WriteArgs(argc, argv, PARC, FINC+1, FOUTC, &mask, objin, objs, objout, objd, MASK);
      Exit(SUCCESS);
   } else {
      Exit(FAILURE);
   }
   return 0;
}

#endif
