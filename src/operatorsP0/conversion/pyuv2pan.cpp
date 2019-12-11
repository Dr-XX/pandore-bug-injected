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

//--------------------------------------------------------------------------
// IMPORTANT NOTICE :
//--------------------
// The source code of this Pandore operator is governed by a specific
// Free-Software License (the CeCiLL License), also applying to the
// CImg Library. Please read it carefully, if you want to use this module
// in your own project (file CImg.h).
// IN PARTICULAR, YOU ARE NOT ALLOWED TO USE THIS PANDORE MODULE IN A
// CLOSED-SOURCE PROPRIETARY PROJECT WITHOUT ASKING AN AUTHORIZATION
// TO THE CIMG LIBRARY AUTHOR ( http://www.greyc.ensicaen.fr/~dtschump/ )
//--------------------------------------------------------------------------

/**
 * @author David Tschumperlé - 2005-08-30
 */

#define cimg_display_type 0
#define cimg_OS 0
#include "CImg1-16.h"
using namespace cimg_library1_16;
#include <pandore.h>
using namespace pandore;

/**
 * @file pyuv2pan
 * @brief Convert an yuv file to Pandore file.
 *
 * Converts a YUV image to a suitable Pandore image.
 * @param nomfichier	the filename of the YUV file.
 * @param objout	the resulting Pandore image.
 */
Errc PYUV2Pan( char* nomfichier, Pobject** objout,
	       const unsigned int width, const unsigned int height,
	       const unsigned int first_frame = 0, const int last_frame = -1,
	       const bool to_rgb = false ) {
   const CImg<unsigned char>
      img = CImg<unsigned char>::get_load_yuv(nomfichier ? nomfichier : "-", width, height, first_frame, last_frame, to_rgb);   
   if (img.dimz() == 1) {
      *objout = new Imc2duc(img.dimy(), img.dimx());
      Imc2duc &dest = **((Imc2duc**)objout);
      cimg_mapXYV(img, x, y, v) dest[v][y][x] = img(x, y, 0, v);
      if (to_rgb) {
	 dest.ColorSpace(RGB);
      } else {
	 dest.ColorSpace(YUV);
      }
   } else {
      *objout = new Imc3duc(img.dimz(), img.dimy(), img.dimx());
      Imc3duc &dest = **((Imc3duc**)objout);
      cimg_mapXYZV(img, x, y, z, v) dest[v][z][y][x] = img(x, y, z, v);
      if (to_rgb) {
	 dest.ColorSpace(RGB);
      } else {
	 dest.ColorSpace(YUV);
      }
   }
   return SUCCESS;
}

#ifdef MAIN

/*
 * Modify only the following constants, and the function calls.
 */
#define	USAGE	"usage : %s width height first_frame last_frame to_rgb im_in [im_out|-]"
#define	PARC	5
#define	FINC	0
#define	FOUTC	1
#define	MASK	0

int main( int argc, char *argv[] ) {
   Errc result;              // The result code of the execution.
   Pobject* mask;            // The region mask.
   Pobject* objin[FINC+1];   // The input objects.
   Pobject* objs[FINC+1];    // The source objects masked by the mask.
   Pobject* objout[FOUTC+1]; // The output objects.
   Pobject* objd[FOUTC+1];   // The result objects of the execution.
   char* parv[PARC+1];       // The input parameters.
   
   if (argc < 2) {
      fprintf(stderr, USAGE"\n", argv[0]);
      Exit(FAILURE);
   }
   
   if ((argc == 2) && (!strcmp(argv[1], "-p"))) {
      fprintf(stdout, "%s 5 1 1\n", argv[0]);
      exit(0);
   }
   
   ReadArgs(argc, argv, PARC, FINC, FOUTC, &mask, objin, objs, objout, objd, parv, USAGE, MASK);
   
   result = PYUV2Pan(((argc > 5) ? argv[6] : NULL),
		     &objd[0],
		     (unsigned int)atoi(parv[0]),
		     (unsigned int)atoi(parv[1]),
		     (unsigned int)atoi(parv[2]),
		     atoi(parv[3]),
		     (bool)(atoi(parv[4]) != 0));
   if (result) {
      WriteArgs(argc, argv, PARC, FINC + 1, FOUTC, &mask, objin, objs, objout, objd, MASK);
   } else {
      PrintErrorFormat(objin, FINC);
      result = FAILURE;
   }
   
   Exit(result);
   return(0);
}

#endif
