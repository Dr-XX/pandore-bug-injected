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
 * @author Regis Clouard - May 18, 2012
 */

#if (defined _MSC_VER) && (!defined snprintf)
#define snprintf _snprintf
#endif

#include <string.h>
#include <pandore.h>
using namespace pandore;
#ifdef _MSC_VER
#include <io.h>
#endif

typedef enum {
   FMT_BMP, 
   FMT_GIF, 
   FMT_JPEG, 
   FMT_PPM, 
   FMT_TIFF, 
   FMT_PNG, 
   FMT_PANDORE, 
   FMT_ERROR
} IMAGE_FORMAT;

#ifdef MAIN
#undef MAIN
#define __MAIN_REQUIRED
#endif

#include "pjpeg2pan.cpp"
namespace PNG {
#include "ppng2pan.cpp"
}

#include "pbmp2pan.cpp"
#include "pppm2pan.cpp"
#include "ptiff2pan.cpp"
#include "pgif2pan.cpp"

bool isPANDORE( FILE *fd ) {
   int a, b, c, d, e, f;
   a = fgetc(fd);
   b = fgetc(fd);
   c = fgetc(fd);
   d = fgetc(fd);
   e = fgetc(fd);
   f = fgetc(fd);

   return (a == 'P' && b == 'A' && c == 'N' && d == 'D' && e == 'O' && f == 'R');
}

IMAGE_FORMAT getFormat( FILE * infile ) {

   if (isBMP(infile)) {
      fseek(infile, 0, SEEK_SET);
      return FMT_BMP;
   }

   fseek(infile, 0, SEEK_SET);
   if (isGIF(infile)) {
      fseek(infile, 0, SEEK_SET);
      return FMT_GIF;
   } 

   fseek(infile, 0, SEEK_SET);
   if (isJPEG(infile)) {
      fseek(infile, 0, SEEK_SET);
      return FMT_JPEG;
   } 

   fseek(infile, 0, SEEK_SET);
   if (isPPM(infile)) {
      fseek(infile, 0, SEEK_SET);
      return FMT_PPM;
   }

   fseek(infile, 0, SEEK_SET);
   if (isTIFF(infile)) {
      fseek(infile, 0, SEEK_SET);
      return FMT_TIFF;
   } 

   fseek(infile, 0, SEEK_SET);
   if (PNG::isPNG(infile)) {
      fseek(infile, 0, SEEK_SET);
      return FMT_PNG;
   } 

   fseek(infile, 0, SEEK_SET);
   if (isPANDORE(infile)) {
      fseek(infile, 0, SEEK_SET);
      return FMT_PANDORE;
   } 
   return FMT_ERROR;
}


Errc PAny2Pan( FILE *fdin, char * nomfichier, Pobject **objd ) {
   Errc result;
   IMAGE_FORMAT format = getFormat(fdin);

   switch(format) {
   case FMT_BMP: result = PBmp2Pan(fdin, objd); break;
   case FMT_GIF: result = PGif2Pan(fdin, objd); break;
   case FMT_JPEG: result = PJpeg2Pan(fdin, objd); break;
   case FMT_PPM: result = PPPM2Pan(fdin, objd); break;
   case FMT_TIFF: result = PTiff2Pan(fdin, objd); break;
   case FMT_PNG: result = PNG::PPng2Pan(fdin, objd); break;
   case FMT_PANDORE: *objd = LoadFile(nomfichier); break;
   case FMT_ERROR:
      fprintf(stderr, "Error pany2pan: unknown image format: %s.\n", nomfichier);
      return FAILURE;
   }
   return result;
}

#ifdef __MAIN_REQUIRED
#define MAIN
#endif

/*
 * Modify only the following constants, and the function call.
 */
#ifdef MAIN
#define	USAGE	"usage: %s im_in [im_out|-]"
#define	PARC	1
#define	FINC	0
#define	FOUTC	1
#undef MASK
#define	MASK	0

int main( int argc, char *argv[] ) {
   Errc result;              // The result code of the execution.
   Pobject* mask;            // The region mask.
   Pobject* objin[FINC+1];   // The input objects.
   Pobject* objs[FINC+1];    // The source objects masked by the mask.
   Pobject* objout[FOUTC+1]; // The output objects.
   Pobject* objd[FOUTC+1];   // The result objects of the execution.
   char* parv[PARC+1];       // The input parameters.
   char* nomfichier;
   FILE* fdin = NULL;

   if ((argc == 2) && (!strcmp(argv[1], "-p"))) {
      fprintf(stdout, "%s 0 1 1\n", argv[0]);
      exit(0);
   }
   
   if ((argc == 2) && (!strcmp(argv[1], "-s"))) {
      fprintf(stdout, "\n");
      exit(0);
   }
   
   ReadArgs(argc, argv, PARC, FINC, FOUTC, &mask, objin, objs, objout, objd, parv, USAGE, MASK);
   // look for input filename.
   nomfichier = (argc > 1)? argv[1] : NULL;
   if ((nomfichier) && (strcmp(nomfichier, "-"))) {
      if ((fdin = fopen(nomfichier, "rb")) == NULL) {
	 fprintf(stderr, "Error pany2pan: Cannot open file %s\n", nomfichier);
	 Exit(FAILURE);
      }
   }
   if (fdin == NULL) {
      fprintf(stderr, "Error pany2pan: Cannot use standard input as an image file.\n");
      Exit(FAILURE);
   }

   result = PAny2Pan(fdin, nomfichier, &objd[0]);

   if (result == SUCCESS) {
      WriteArgs(argc, argv, PARC, FINC, FOUTC, &mask, objin, objs, objout, objd, MASK);
   } else {
      fprintf(stderr, "Error pany2pan: Bad image file: %s.\n", nomfichier);
   }   

   fclose(fdin);
   
   Exit(result);
   return 0;
}

#endif
