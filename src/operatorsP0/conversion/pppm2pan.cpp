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
 * THIS SOFTWARE IS PqROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
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

#include <string.h>
#include <pandore.h>
using namespace pandore;

/**
 * Skip comments.
 * A comment is started with a # and is ended with an end of line.
 * @param f the file.
 */
void SkipComment( FILE *f ) {
   int c;
   
   do {
      c = fgetc(f);
      if (c == '#') {
	 do {
	    c = fgetc(f);
	 } while (c != '\n');
      }
   } while ((c == '#') || (c == '\r') || (c == '\t') || (c == '\n') || (c == ' '));
   ungetc(c, f);
}

bool isPPM( FILE *fp ) {
   int a, b;
   a = fgetc(fp);
   b = fgetc(fp);
   return a == 'P' && ( b == '1' || b == '2' || b == '3' || b == '4' || b == '5' || b == '6');
}

/**
 * Converts a PPM image (Portable Pixmap) or PGM (Portable GrayMap)
 * to a suitable Pandore image.
 *
 * @param nomfichier	the filename of the ppm file.
 * @param objout	the result Pandore image.
 */
Errc PPPM2Pan( FILE *f, Pobject **objout ) {
   Long x, y, z;
   char imagecode[2];
   Uchar *data;
   int type;
   int nbpix, nlig, ncol, ndep = 0;
   
   // HEADER
   // Read the magic number.
   if (fscanf(f, "%c", &imagecode[0]) < 1 || fscanf(f, "%c", &imagecode[1]) < 1) {
      return FAILURE;
   }
   if (imagecode[0] != 'P') {
      return FAILURE;
   }
   if (imagecode[1] == '2') { // Gray ascii.
      type = 2;
   } else if (imagecode[1] == '5') { // RAW Gray scale
      type = 5;
   } else if (imagecode[1] == '3') { // ASCII Color.
      type = 3;
   } else if (imagecode[1] == '6') { // RAW Color.
      type = 6;
   } else if (imagecode[1] == '1') { // ASCII Binary
      type = 1;
   } else if (imagecode[1] == '4') { // RAW Binary
      type = 4;
   } else {
      return FAILURE;
   }

#ifndef NDEBUG
   printf("Type = %d\n", type);
#endif

   SkipComment(f);
   if (fscanf(f, "%d", &ncol) < 1) {
      return FAILURE;
   }
   if (fscanf(f, "%d", &nlig) < 1) {
      return FAILURE;
   }

   char buf[255];
   if (fgets(buf, 254, f) == NULL) {
      return FAILURE;
   }
   sscanf(buf, "%d", &ndep);
   SkipComment(f);

#ifndef NDEBUG
   printf("Image size = %d cols x %d rows x %d planes\n", ncol, nlig, ndep);
#endif

   // DATA
   if (type == 2) { // P2: Gray ascii
      if (fscanf(f, "%d", &nbpix) < 1) {
	 return FAILURE;
      }
      nbpix = (nbpix / 256) + 1;
      SkipComment(f);
#ifndef NDEBUG
      printf("Pixel size = %d\n", nbpix);
#endif

      if (ndep == 0) {
	 if (nbpix == 1) {
	    *objout = new Img2duc(nlig, ncol);
	    Img2duc *imd = (Img2duc*)*objout;
	    for (y = 0; y < nlig; y++) {
	       for (x = 0 ; x < ncol; x++) {
		  int val;
		  if (fscanf(f, "%d", &val) < 1) {
		     return FAILURE;
		  }
		  (*imd)(y, x) = val;
	       }
	    }
	 } else {
	    *objout = new Img2dsl(nlig, ncol);
	    Img2dsl *imd = (Img2dsl*)*objout;
	    for (y = 0; y < nlig; y++) {
	       for (x = 0; x < ncol; x++) {
		  int val;
		  if (fscanf(f, "%d", &val) < 1) {
		     return FAILURE;
		  }
		  (*imd)(y, x) = val;
	       }
	    }
	 }
      } else { //3D
	 if (nbpix == 1) {
	    *objout = new Img3duc(ndep, nlig, ncol);
	    Img3duc *imd = (Img3duc*)*objout;
	    for (z = 0; z < ndep; z++) {
	       for (y = 0; y <  nlig; y++) {
		  for (x = 0 ; x < ncol; x++) {
		     int val;
		     if (fscanf(f, "%d", &val) < 1) {
			return FAILURE;
		     }
		     (*imd)(z, y, x) = val;
		  }
	       }
	    }
	 } else {
	    *objout = new Img3dsl(ndep, nlig, ncol);
	    Img3dsl *imd = (Img3dsl*)*objout;
	    for (z = 0; z < ndep; z++) {
	       for (y = 0; y < nlig; y++) {
		  for (x=  0; x < ncol; x++) {
		     int val;
		     if (fscanf(f, "%d", &val) < 1) {
			return FAILURE;
		     }
		     (*imd)(z, y, x) = val;
		  }
	       }
	    }
	 }
      }
   } else if (type == 3) { // P3: Color ascii
      if (fscanf(f, "%d", &nbpix) < 1) {
	 return FAILURE;
      }
      nbpix = (nbpix / 256) + 1;
      SkipComment(f);
#ifndef NDEBUG
      printf("Pixel size = %d\n", nbpix);
#endif

      if (nbpix == 1) {
	 *objout = new Imc2duc(nlig, ncol);
	 Imc2duc *imd = (Imc2duc*)*objout;
	 for (y = 0; y  < nlig; y++) {
	    for (x = 0 ; x < ncol; x++) {
	       int val;
	       if (fscanf(f, "%d", &val) < 1) {
		  return FAILURE;
	       }
	       (*imd)(0, y, x) = val;
	       if (fscanf(f, "%d", &val) < 1) {
		  return FAILURE;
	       }
	       (*imd)(1, y, x) = val;
	       if (fscanf(f, "%d", &val) < 1) {
		  return FAILURE;
	       }
	       (*imd)(2, y, x) = val;
	    }
	 }
      } else {
	 *objout = new Imc2dsl(nlig, ncol);
	 Imc2dsl *imd = (Imc2dsl*)*objout;
	 for (y = 0; y < nlig; y++) {
	    for (x = 0; x <  ncol; x++) {
	       int val;
	       if (fscanf(f, "%d", &val) < 1) {
		  return FAILURE;}
	       (*imd)(0, y, x) = val;
	       if (fscanf(f, "%d", &val) < 1) {
		  return FAILURE;
	       }
	       (*imd)(1, y , x) = val;
	       if (fscanf(f, "%d", &val) < 1) {
		  return FAILURE;
	       }
	       (*imd)(2, y, x) = val;
	    }
	 }
      }
   } else if (type == 5) { // P5: RAW Gray
      if (fscanf(f, "%d", &nbpix) < 1) {
	 return FAILURE;
      }
      nbpix = (nbpix / 256) + 1;
      SkipComment(f);
#ifndef NDEBUG
      printf("Pixel size = %d\n", nbpix);
#endif
      if (ndep == 0) { // 2D
	 if ((data = (Uchar*)malloc(sizeof(Uchar)*nbpix * nlig * ncol)) == NULL) {
	    return FAILURE;
	 }
	 if (fread((Uchar*)data, sizeof(Uchar), nbpix * ncol * nlig, f) < 1) {
	    return FAILURE;
	 }
	 if (nbpix == 1) {
	    Uchar *p;
	    *objout = new Img2duc(nlig, ncol);
	    Img2duc *imd = (Img2duc*)*objout;
	    p = data;
	    for (y = 0; y < nlig; y++) {
	       for (x = 0; x < ncol; x++, p++) {
		  (*imd)(y, x) = *p;
	       }
	    }
	 } else {
	    *objout = new Img2dsl(nlig, ncol);
	    Img2dsl *imd = (Img2dsl*)*objout;
	    unsigned short* p = (unsigned short *)data;
	    for (y = 0; y < nlig; y++) {
	       for (x = 0; x < ncol; x++, p++) {
		  (*imd)(y, x) = (long)*p;
	       }
	    }
	 }
	 free((char*)data);   
      } else { // 3D
	 if ((data = (Uchar*)malloc(sizeof(Uchar) * nbpix * nlig * ncol * ndep)) == NULL) {
	    return FAILURE;
	 }
	 if (fread((Uchar*)data, sizeof(Uchar), nbpix * ncol * nlig * ndep, f) < 1) {
	    return FAILURE;
	 }
	 if (nbpix == 1) {
	    Uchar *p;
	    *objout = new Img3duc(ndep, nlig, ncol);
	    Img3duc *imd = (Img3duc*)*objout;
	    p = data;
	    for (z = 0; z < ndep; z++) {
	       for (y = 0; y < nlig; y++) {
		  for (x = 0 ; x < ncol; x++, p++) {
		     (*imd)(z, y , x) = *p;
		  }
	       }
	    }
	 } else {
	    *objout = new Img3dsl(ndep, nlig, ncol);
	    Img3dsl *imd = (Img3dsl*)*objout;
	    unsigned short* p = (unsigned short *)data;
	    for ( z = 0; z < ndep; z++) {
	       for (y = 0 ; y < nlig; y++) {
		  for (x = 0; x <  ncol; x++, p++) {
		     (*imd)(z, y, x) = (long)*p;
		  }
	       }
	    }
	 }
	 free((char*)data);
      }
   } else if (type == 6) { // P6: RAW Color
      if (fscanf(f, "%d", &nbpix) < 1) {
	 return FAILURE;
      }
      nbpix = (nbpix / 256) + 1;
      SkipComment(f);
#ifndef NDEBUG
      printf("Pixel size = %d\n", nbpix);
#endif
      if ((data = (Uchar*)malloc(sizeof(Uchar)*nbpix * 3 * nlig * ncol)) == NULL) {
	 return FAILURE;
      }
      if (fread((char*)data, sizeof(Uchar), nbpix * 3 * ncol * nlig, f) < 1) {
	 return FAILURE;
      }

      if (nbpix == 1) {
	 Uchar *p;
	 *objout = new Imc2duc(nlig, ncol);
	 Imc2duc *imd = (Imc2duc*)*objout;
	 p = data;

	 for (y = 0; y < nlig; y++) {
	    for (x = 0; x < ncol; x++) {
	       (*imd)(0, y, x) = *(p++);
	       (*imd)(1, y, x) = *(p++);
	       (*imd)(2, y, x) = *(p++);
	    }
	 }
      } else {
	 *objout = new Imc2dsl(nlig, ncol);
	 Imc2dsl *imd = (Imc2dsl*)*objout;
	 unsigned short * p = (unsigned short *)data;

	 for (y = 0; y < nlig; y++) {
	    for (x = 0 ; x < ncol; x++) {
	       (*imd)(0, y, x) = *(p++);
	       (*imd)(1, y, x) = *(p++);
	       (*imd)(2, y, x) = *(p++);
	    }
	 }
      }
      
      free((char*)data);
   }

   else if (type == 1) { // P2: ascii RAW
      SkipComment(f);
      *objout = new Img2duc(nlig, ncol);
      Img2duc *imd = (Img2duc*)*objout;
      for (y  =0; y < nlig; y++) {
	 for (x = 0; x < ncol; x++) {
	    int val;
	    if (fscanf(f, "%d", &val) < 1) {
	       return FAILURE;
	    }
	    (*imd)(y, x) = (char)((!val) * 255);
 	 }
      }
   } else if (type == 4) { // P4: ascii RAW
      SkipComment(f);
      *objout = new Img2duc(nlig, ncol);
      Img2duc *imd = (Img2duc*)*objout;
      for (y = 0; y < nlig; y++) {
	 for (x = 0; x < ncol; x += 8) {
	    char val;
	    if (fscanf(f, "%c", &val) < 1) {
	       return FAILURE;
	    }
	    for (int i = 0; i < 8; i++) {
	       if (x + (7 - i) >= ncol) {
		  break;
	       }
	       (*imd)(y, x + (7 - i)) = 255 * (char)!((val >> i) & 0x01);
	    }
	 }
      }
   }
   return SUCCESS;
}

#ifdef MAIN

/*
 * Modify only the following constants, and the function calls.
 */
#define	USAGE	"usage: %s im_in [im_out|-]"
#define	PARC	0
#define	FINC	0
#define	FOUTC	1
#define	MASK	0

int main( int argc, char *argv[] ) {
   Pobject* mask;             // The region mask.
   Pobject* objin[FINC + 1];   // The input objects.
   Pobject* objs[FINC + 1];    // The source objects masked by the mask.
   Pobject* objout[FOUTC + 1]; // The output objects.
   Pobject* objd[FOUTC + 1];   // The result objects of the execution.
   char* parv[PARC + 1];       // The input parameters.
   
   if (argc < 2) {
      fprintf(stderr, USAGE"\n", argv[0]);
      Exit((Errc)FAILURE);
   }
   
   if ((argc == 2) && (!strcmp(argv[1], "-p"))) {
      fprintf(stdout, "%s 0 1 1\n", argv[0]);
      exit(0);
   }
 
   ReadArgs(argc, argv, PARC, FINC, FOUTC, &mask, objin, objs, objout, objd, parv, USAGE, MASK);
   // Check inputs.
   char* nomfichier = (argc > 1) ? argv[1] : NULL;
   FILE*  fdin = stdin;
   if ((nomfichier != NULL) && (strcmp(nomfichier, "-"))) {
      if ((fdin = fopen(nomfichier, "rb")) == NULL) {
	 fprintf(stderr, "Error pppm2pan: Cannot open file %s\n", nomfichier);
	 Exit(FAILURE);
      }
   }
   
   if (PPPM2Pan(fdin, &objd[0])) {
      WriteArgs(argc, argv, PARC, FINC + 1, FOUTC, &mask, objin, objs, objout, objd, MASK);
      Exit(SUCCESS);
   } else {
      fprintf(stderr, "Error pppm2pan: Bad PPM file: %s.\n", nomfichier);
      Exit(FAILURE);
   }
   return 0;
}

#endif
