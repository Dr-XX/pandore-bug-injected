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
 * @file ppan2ppm.cpp
 * @brief Convert ppm file to Pandore.
 */ 

#include <string.h>
#include <pandore.h>
using namespace pandore;

/*
 * Format a PPM image (Portable Pixmap) or PGM image (Portable GrayMap)
 * from a Pandore image.
 */
Errc PPan2PPM( const Img2duc &ims, char *filename ) {
   FILE *f;

   if ((filename == NULL) || (!strcmp(filename, "-"))) {
      f = stdout;
   } else if ((f = fopen(filename, "w")) == NULL) {
      fprintf(stderr, "Error ppan2ppm: Cannot create PPM file %s\n", filename);
      return FAILURE;
   }
   
   // HEADER
   fputs("P2\n", f);
   fputs("# CREATOR: Pandore @ Greyc-Image\n", f);
   fprintf(f, "%d %d\n", (int)ims.Width(), (int)ims.Height());
   fputs("255\n", f);
   
   int npix = 0;
 
   for (int y = 0; y < ims.Height(); y++) {
      for (int x = 0; x < ims.Width(); x++) {
	 fprintf(f, "%3d ", (int)ims(y, x));
	 if (++npix == 15) {
	    npix = 0;
	    fputs("\n", f);
	 }
      }
   }
   fclose(f);
   
   return SUCCESS;
}

Errc PPan2PPM( const Img3duc &ims, char *filename ) {
   FILE *f;
   
   if ((filename == NULL) || (!strcmp(filename, "-"))) {
      f = stdout;
   } else if ((f = fopen(filename, "w")) == NULL) {
      fprintf(stderr, "Error ppan2ppm: Cannot create PPM file %s\n", filename);
      return FAILURE;
   }
   
   // HEADER
   fputs("P2\n", f);
   fputs("# CREATOR: Pandore @ Greyc-Image\n", f);
   fprintf(f, "%d %d %d\n", (int)ims.Width(), (int)ims.Height(), (int)ims.Depth());
   fputs("255\n", f);
   
   int npix = 0;
 
   for (int z = 0; z < ims.Depth(); z++) {
      for (int y = 0; y < ims.Height(); y++) {
	 for (int x = 0; x < ims.Width(); x++) {
	    fprintf(f, "%3d ", (int)ims(z, y ,x));
	    if (++npix == 15) {
	       npix = 0;
	       fputs("\n", f);
	    }
	 }
      }
   }
   fclose(f);
   
   return SUCCESS;
}

Errc PPan2PPM( const Imc2duc &ims, char *filename ) {
   FILE *f;
   
   if ((filename == NULL) || (!strcmp(filename, "-"))) {
      f = stdout;
   } else if ((f = fopen(filename, "w")) == NULL) {
      fprintf(stderr, "Error ppan2ppm: Cannot create PPM file %s\n", filename);
      return FAILURE;
   }
   
   // HEADER
   fputs("P3\n", f);
   fputs("# CREATOR: Pandore @ Greyc-Image\n", f);
   fprintf(f, "%d %d\n", (int)ims.Width(), (int)ims.Height());
   fputs("255\n", f);
   
   int npix = 0;
 
   for (int y = 0; y < ims.Height(); y++) {
      for (int x = 0; x < ims.Width(); x++) {
	 fprintf(f, "%3d ", (int)ims(0, y, x));
	 fprintf(f, "%3d ", (int)ims(1, y, x));
	 fprintf(f, "%3d ", (int)ims(2, y, x));
	 if (++npix == 5) {
	    npix = 0;
	    fputs("\n", f);
	 }
      }
   }
   fclose(f);
   return SUCCESS;
}

Errc PPan2PPM( const Reg2d &ims, char *filename ) {
   FILE *f;
   
   if ((filename == NULL) || (!strcmp(filename, "-"))) {
      f = stdout;
   } else if ((f = fopen(filename, "w")) == NULL) {
      fprintf(stderr, "Error ppan2ppm: Cannot create PPM file %s\n", filename);
      return FAILURE;
   }
   
   // HEADER
   fputs("P3\n", f);
   fputs("# CREATOR: Pandore @ Greyc-Image\n", f);
   fprintf(f, "%d %d\n", (int)ims.Width(), (int)ims.Height());
   fputs("255\n", f);
   
   int npix = 0;
 
   for (int y = 0; y < ims.Height(); y++) {
      for (int x = 0; x < ims.Width(); x++) {
	 fprintf(f, "%3d ", (int)((ims(y, x) & 0x0000ff)));
	 fprintf(f, "%3d ", (int)((ims(y, x) & 0x00ff00) >> 8));
	 fprintf(f, "%3d ", (int)((ims(y, x) & 0xff0000) >> 16));
	 if (++npix == 5) {
	    npix = 0;
	    fputs("\n", f);
	 }
      }
   }
   fclose(f);   
   return SUCCESS;
}

#ifdef MAIN


/*
 * Modify only the following constants, and the function calls.
 */
#define	USAGE	"usage: %s [im_in|-] [im_out|-]"
#define	PARC	0
#define	FINC	1
#define	FOUTC	1
#define	MASK	0

int main( int argc, char *argv[] ) {
   Pobject* mask;              // The region mask.
   Pobject* objin[FINC + 1];   // The input objects.
   Pobject* objs[FINC + 1];    // The source objects masked by the mask.
   Pobject* objout[FOUTC + 1]; // The output objects.
   Pobject* objd[FOUTC + 1];   // The result objects of the execution.
   char* parv[PARC + 1];       // The input parameters.
   Errc result;

   ReadArgs(argc, argv, PARC, FINC, FOUTC, &mask, objin, objs, objout, objd, parv, USAGE, MASK);
   
   switch(objs[0]->Type()) {
   case Po_Img2duc: {
      Img2duc* const ims = (Img2duc*)objs[0];
      
      result = PPan2PPM(*ims, (argc > 2)? argv[2] : NULL);
   } break;
   case Po_Img3duc: {
      Img3duc* const ims = (Img3duc*)objs[0];
      
      result = PPan2PPM(*ims, (argc > 2)? argv[2] : NULL);
   } break;
   case Po_Reg2d: {
      Reg2d* const ims = (Reg2d*)objs[0];

      result = PPan2PPM(*ims, (argc > 2)? argv[2] : NULL);
   } break;

   case Po_Imc2duc: {
      Imc2duc* const ims = (Imc2duc*)objs[0];

      result = PPan2PPM(*ims, (argc > 2)? argv[2] : NULL);
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
