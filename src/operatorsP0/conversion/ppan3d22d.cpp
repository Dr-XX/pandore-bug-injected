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
 * @author François Angot - 1996-11-19
 */

#if (defined _MSC_VER) && (!defined snprintf)
#define snprintf _snprintf
#endif

#include <string.h>
#include <math.h>
#include <pandore.h>
using namespace pandore;

/**
 * @file ppan3d22d.cpp
 * Convert 3D image to a series of 2D images.
 */
template <typename T>
Errc PPan3d22d( const Img3d<T> &ims, char *basename, char *suffixe ) {
   if (ims.Depth() == 0) {
      std::cout << "Error ppan3d22d: Bad input image size: depth=0" << std::endl;
      return FAILURE;
   }

   const int fieldsize = (int)ceil(log10((double)ims.Depth()));
   char filename[256];

   Img2d<T> imd(ims.Props());
   for (int d = 0; d < ims.Depth(); d++) {
      for (int y = 0; y < ims.Height(); y++) {
	 for (int x = 0; x < ims.Width(); x++) {
	    imd[y][x] = ims[d][y][x];
	 }
      }
      snprintf(filename, sizeof(filename), "%s%0*d%s", basename, fieldsize, d, suffixe);
      filename[sizeof(filename) - 1] = 0;
      imd.SaveFile(filename);
   }
   
   return SUCCESS;
}

template <typename T>
Errc PPan3d22d( const Imx3d<T> &ims, char *basename, char *suffixe ) {
   if (ims.Depth() == 0) {
      std::cout << "Error ppan3d22d: Bad input image size: depth=0"  << std::endl;
      return FAILURE;
   }

   char filename[256];
   const int fieldsize = (int)ceil(log10((double)ims.Depth()));

   Imx2d<T> imd(ims.Props());
   for (int d = 0; d < ims.Depth(); d++) {
      for (int b = 0; b < ims.Bands(); b++) {
	 for (int y = 0; y < ims.Height(); y++) {
	    for (int x = 0; x < ims.Width(); x++) {
	       imd[b][y][x] = ims[b][d][y][x];
	       imd[b][y][x] = ims[b][d][y][x];
	       imd[b][y][x] = ims[b][d][y][x];
	    }
	 }
      }
      snprintf(filename, sizeof(filename), "%s%0*d%s", basename, fieldsize, d, suffixe);
      filename[sizeof(filename) - 1] = 0;
      imd.SaveFile(filename);
   }
   
   return SUCCESS;
}

#ifdef MAIN

/*
 * Modify only the following constants, and the function call.
 */
#define	USAGE	"usage: %s [-m mask] [im_in|-] im_out"
#define	PARC	0
#define	FINC	1
#define	FOUTC	0
#define	MASK	2

int main( int argc, char *argv[] ) {
   Errc result;                // The result code of the execution.
   Pobject* mask;              // The region mask.
   Pobject* objin[FINC + 1];   // The input objects.
   Pobject* objs[FINC + 1];    // The source objects masked by the mask.
   Pobject* objout[FOUTC + 1]; // The output objects.
   Pobject* objd[FOUTC + 1];   // The result objects of the execution.
   char* parv[PARC + 1];       // The input parameters.
   
   char basename[256], suffixe[256];
   
   if (argc < 2) {
      fprintf(stderr, USAGE"\n", argv[0]);
      Exit(FAILURE);
   }
   
   if ((argc == 2) && (!strcmp(argv[1], "-p"))) {
      fprintf(stdout, "%s 0 1 1\n", argv[0]);
      exit(0);
   }
 
   ReadArgs(argc, argv, PARC, FINC, FOUTC, &mask, objin, objs, objout, objd, parv, USAGE, MASK);
   
   if (argc != 3) {
      fprintf(stderr, USAGE"\n", argv[0]);
      Exit(FAILURE);
   }
   
   int i = strlen(argv[2]) - 1;
   while ((i >= 0) && (argv[2][i] != '.')) {
      i--;
   }
   if (i > 0) {
      strncpy(basename, argv[2], i);
      basename[i] = '\0';
      strncpy(suffixe, &argv[2][i], sizeof(suffixe));
   } else {
      strncpy(basename, argv[2], sizeof(basename));
      strncpy(suffixe, "", sizeof(suffixe));
   }
   
   switch(objs[0]->Type()) {
   case Po_Img3duc: {
      Img3duc* const ims = (Img3duc*)objs[0];
      
      result = PPan3d22d(*ims, basename, suffixe);
   } break;
   case Po_Img3dsl: {
      Img3dsl* const ims = (Img3dsl*)objs[0];
      
      result = PPan3d22d(*ims, basename, suffixe);
   } break;
   case Po_Img3dsf: {
      Img3dsf* const ims = (Img3dsf*)objs[0];
      
      result = PPan3d22d(*ims, basename, suffixe);
   } break;
   case Po_Imc3duc: {
      Imc3duc* const ims = (Imc3duc*)objs[0];
      
      result = PPan3d22d(*ims, basename, suffixe);
   } break;
   case Po_Imc3dsl: {
      Imc3dsl* const ims = (Imc3dsl*)objs[0];
      
      result = PPan3d22d(*ims, basename, suffixe);
   } break;
   case Po_Imc3dsf: {
      Imc3dsf* const ims = (Imc3dsf*)objs[0];
      
      result = PPan3d22d(*ims, basename, suffixe);
   } break;
   case Po_Imx3duc: {
      Imx3duc* const ims = (Imx3duc*)objs[0];
      
      result = PPan3d22d(*ims, basename, suffixe);
   } break;
   case Po_Imx3dsl: {
      Imx3dsl* const ims = (Imx3dsl*)objs[0];
      
      result = PPan3d22d(*ims, basename, suffixe);
   } break;
   case Po_Imx3dsf: {
      Imx3dsf* const ims = (Imx3dsf*)objs[0];
      
      result = PPan3d22d(*ims, basename, suffixe);
   } break;
   default:
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
