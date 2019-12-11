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
 *   * Neither the name of Research In Motion, nor the name of Nokia
 *     Corporation and its Subsidiary(-ies), nor the names of its
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
 */

#include <string.h>
#include <pandore.h> 

using namespace pandore;

/**
 * @file praw2pan.cpp
 *
 * Convert a raw file to a Pandore file.
 */

static bool change_endian = false;

/**
 * Reads a pixel according to the its type
 * and the current endian.
 */
size_t readPixel( Ushort *ptr, FILE *stream ) {
   size_t result = fread(ptr, sizeof(*ptr), 1, stream);
   // Exchange high byte and low byte
   if (change_endian) {
      union{
	 Ushort s;
	 char c[2];
      } u, v;
      
      u.s = *ptr;
      v.c[0] = u.c[1];
      v.c[1] = u.c[0];
      *ptr = v.s;
   }
   return result;
}

size_t readPixel( Long *ptr, FILE *stream ) {
   size_t result = fread(ptr, sizeof(*ptr), 1, stream);
   // Exchange high bytes and low bytes
   if (change_endian) {
      union {
	 Long l;
	 char c[4];
      } u, v;
      
      u.l = *ptr;
      v.c[0] = u.c[3];
      v.c[1] = u.c[2];
      v.c[2] = u.c[1];
      v.c[3] = u.c[0];
      *ptr = v.l;
   }
   return result;
}

size_t readPixel( Float *ptr, FILE *stream ) {
   size_t result = fread(ptr, sizeof(*ptr), 1, stream);
   // Exchange high byte and low byte
   if (change_endian) {
      union {
	 float l;
	 char c[4];
      } u, v;
      
      u.l = *ptr;
      v.c[0] = u.c[3];
      v.c[1] = u.c[2];
      v.c[2] = u.c[1];
      v.c[3] = u.c[0];
      *ptr = v.l;
   }
   return result;
}

size_t readPixel( Double *ptr, FILE *stream ) {
   size_t result = fread(ptr, sizeof(*ptr), 1, stream);
   // Exchange high byte and low byte
   if (change_endian) {
      union {
	 double l;
	 char c[8];
      } u, v;
      
      u.l = *ptr;
      v.c[0] = u.c[7];
      v.c[1] = u.c[6];
      v.c[2] = u.c[5];
      v.c[3] = u.c[4];
      v.c[4] = u.c[3];
      v.c[5] = u.c[2];
      v.c[6] = u.c[1];
      v.c[7] = u.c[0];
      *ptr = v.l;
   }
   return result;
}

/**
 * Does it work with MSDOS? (fread)
 * See also pan2raw.
 */
Errc PRaw2Pan( char *filename, Img1duc &imd, int pixelSize ) {
   FILE *fp = stdin;
   
   if (filename != NULL && !(fp = fopen(filename, "rb"))) {
      return FAILURE;
   }

   // Skip possible heading data.
   int skip = -pixelSize*imd.VectorSize();
   fseek(fp, skip, SEEK_END);

   if (fread(&(imd[0]), sizeof(Img1duc::ValueType), imd.Width(), fp)<(size_t)imd.Width()) {
      return FAILURE;
   }
   
   fclose(fp);
   
   return SUCCESS;
}

Errc PRaw2Pan( char *filename, Img1dsl &imd, int pixelSize ) {
   FILE *fp = stdin;
   
   if ((filename != NULL) && (!(fp = fopen(filename, "rb")))) {
      return FAILURE;   
   }

   // Skip possible heading data.
   int skip = -pixelSize*imd.VectorSize();
   fseek(fp, skip, SEEK_END);

   for (int x = 0; x<imd.Width(); x++) {
      Long val;
      if (readPixel(&val, fp) < 1) {
	 return FAILURE;
      }
      imd[x] = (Long)val;
   }
   fclose(fp);
   
   return SUCCESS;
}

Errc PRaw2Pan( char *filename, Img1dsf &imd, int pixelSize ) {
   FILE *fp = stdin;
   
   if ((filename != NULL) && (!(fp = fopen(filename, "rb")))) {
      return FAILURE;
   }

   // Skip possible heading data.
   int skip = -pixelSize*imd.VectorSize();
   fseek(fp, skip, SEEK_END);
   
   if (pixelSize == 6) {
      for (int x = 0; x<imd.Width(); x++) {
	 Float val;
	 readPixel(&val, fp);
	 imd[x] = (float)val;
      }
   } else {
      for (int x = 0; x<imd.Width(); x++) {
	 Double val;
	 readPixel(&val, fp);
	 imd[x] = (float)val;
      }
   }
   fclose(fp);
   
   return SUCCESS;
}

Errc PRaw2Pan( char *filename, Img2duc &imd, int pixelSize ) {
   FILE *fp = stdin;
   
   if ((filename != NULL) && (!(fp = fopen(filename, "rb")))) {
      return FAILURE;
   }

   // Skip possible heading data.
   int skip = -pixelSize*imd.VectorSize();
   fseek(fp, skip, SEEK_END);

   if (fread(imd.Vector(), sizeof(Img2duc::ValueType), imd.VectorSize(), fp)<imd.VectorSize()) {
      return FAILURE;
   }

   fclose(fp);
   
   return SUCCESS;
}

Errc PRaw2Pan( char *filename, Img2dsl &imd, int pixelSize ) {
   FILE *fp = stdin;
   
   if ((filename != NULL) && (!(fp = fopen(filename, "rb")))) {
      return FAILURE;
   }
   
   // Skip possible heading data.
   int skip = -pixelSize*imd.VectorSize();
   fseek(fp, skip, SEEK_END);

   if (pixelSize == 2) {
      for (int y = 0; y<imd.Height(); y++) {
	 for (int x = 0; x<imd.Width(); x++) {
	    Ushort val;
	    if (readPixel(&val, fp)<1) {
	       return FAILURE;
	    }
	    imd(y, x) = (Long)val;
	 }
      }
   } else {
      for (int y = 0;y<imd.Height();y++) {
	 for (int x = 0;x<imd.Width();x++) {
	    Long val;
	    if (readPixel(&val, fp)<1) {
	       return FAILURE;
	    }
	    imd(y, x) = (Long)val;
	 }
      }
   }

   
   fclose(fp);
   
   return SUCCESS;
}

Errc PRaw2Pan(char *filename, Img2dsf &imd, int pixelSize ) {
   FILE *fp;
  
   if ((filename != NULL) || (!strcmp(filename, "-"))) {
      if ((fp = fopen(filename, "rb")) == NULL) {
	 return FAILURE;
      }
   } else {
      fp = stdin;
   }

   // Skip possible heading data.
   int skip = -pixelSize*imd.VectorSize();
   fseek(fp, skip, SEEK_END);

   if (pixelSize == 6) {
      for (int y = 0; y<imd.Height(); y++) {
	 for (int x = 0; x<imd.Width(); x++) {
	    Float val;
	    readPixel(&val, fp);
	    imd(y, x) = (float)val;
	 }
      }
   } else {
      for (int y = 0; y<imd.Height(); y++) {
	 for (int x = 0; x<imd.Width(); x++) {
	    Double val;
	    readPixel(&val, fp);
	    imd(y, x) = (float)val;
	 }
      }
   }
  
   fclose(fp);
  
   return SUCCESS;
}

Errc PRaw2Pan( char *filename, Imc2duc &imd, int pixelSize, int color ) {
   FILE *fp = stdin;
   
   if ((filename != NULL) && (!(fp = fopen(filename, "rb")))) {
      return FAILURE;
   }

   // Skip possible heading data.
   int skip = -3*pixelSize*imd.VectorSize();
   fseek(fp, skip, SEEK_END);

   if (color == 1) {
      if ((fread(imd.Vector(0), pixelSize, imd.VectorSize(), fp) < imd.VectorSize())
	  || (fread(imd.Vector(1), pixelSize, imd.VectorSize(), fp) < imd.VectorSize())
	  || (fread(imd.Vector(2), pixelSize, imd.VectorSize(), fp) < imd.VectorSize())) {
	 return FAILURE;
      }
   } else {
      char buff[3];
      for (int y = 0; y<imd.Height(); y++) {
	 for (int x = 0; x<imd.Width(); x++) {
	    if (fread(buff, 1, 3, fp) < 3) {
	       return FAILURE;
	    }
	    imd(0, y, x) = buff[0];
	    imd(1, y, x) = buff[1];
	    imd(2, y, x) = buff[2];
	 }
      }
   }

   fclose(fp);
   return SUCCESS;
}

Errc PRaw2Pan( char *filename, Img3duc &imd, int pixelSize ) {
   FILE *fp = stdin;
   
   if ((filename != NULL) && (!(fp = fopen(filename, "rb")))) {
      return FAILURE;
   }

   // Skip possible heading data.
   int skip = -pixelSize*imd.VectorSize();
   fseek(fp, skip, SEEK_END);

   if (fread(imd.Vector(), pixelSize, imd.VectorSize(), fp) < imd.VectorSize()) {
      return FAILURE;
   }
   
   fclose(fp);
   
   return SUCCESS;
}

Errc PRaw2Pan( char *filename, Img3dsl &imd, int pixelSize ) {
   FILE *fp = stdin;
   
   if ((filename != NULL) && (!(fp = fopen(filename, "rb")))) {
      return FAILURE;
   }

   // Skip possible heading data.
   int skip = -pixelSize*imd.VectorSize();
   fseek(fp, skip, SEEK_END);

   if (pixelSize == 2) { // Ushort
      for (int z = 0;z<imd.Depth();z++) {
	 for (int y = 0;y<imd.Height();y++) {
	    for (int x = 0;x<imd.Width();x++) {
	       Ushort val;
	       if (readPixel(&val, fp) < 1) {
		  return FAILURE;
	       }
	       imd(z, y, x) = (Long)val;
	    }
	 }
      }
   } else {	// Ulong, Long
      for (int z = 0;z<imd.Depth();z++) {
	 for (int y = 0;y<imd.Height();y++) {
	    for (int x = 0;x<imd.Width();x++) {
	       Long val = 0;
	       if (readPixel(&val, fp) < 1) {
		  return FAILURE;
	       }
	       imd(z, y, x) = (Long)val;
	    }
	 }
      }
   }
   fclose(fp);
   
   return SUCCESS;
}

Errc PRaw2Pan( char *filename, Img3dsf &imd, int pixelSize ) {
   FILE *fp = stdin;

   // Skip possible heading data.
   int skip = -pixelSize*imd.VectorSize();
   fseek(fp, skip, SEEK_END);
   
   if ((filename != NULL) && (!(fp = fopen(filename, "rb")))) {
      return FAILURE;
   }

   if (pixelSize == 6) {
      for (int z = 0; z<imd.Depth(); z++) {
	 for (int y = 0; y<imd.Height(); y++) {
	    for (int x = 0; x<imd.Width(); x++) {
	       Float val;
	       readPixel(&val, fp);
	       imd(z, y, x) = val;
	    }
	 }
      }
   } else {
      for (int z = 0; z<imd.Depth(); z++) {
	 for (int y = 0; y<imd.Height(); y++) {
	    for (int x = 0; x<imd.Width(); x++) {
	       Double val;
	       readPixel(&val, fp);
	       imd(z, y, x) = (float)val;
	    }
	 }
      }
   }
   
   fclose(fp);
   
   return SUCCESS;
}

Errc PRaw2Pan( char *filename, Imc3duc &imd, int pixelSize, int color ) {
   FILE *fp = stdin;
   
   if ((filename != NULL) && (!(fp = fopen(filename, "rb")))) {
      return FAILURE;
   }

   // Skip possible heading data.
   int skip = -3*pixelSize*imd.VectorSize();
   fseek(fp, skip, SEEK_END);

   if (color == 1) {
      if ((fread(imd.Vector(0), pixelSize, imd.VectorSize(), fp) < imd.VectorSize())
	  || (fread(imd.Vector(1), pixelSize, imd.VectorSize(), fp) < imd.VectorSize())
	  || (fread(imd.Vector(2), pixelSize, imd.VectorSize(), fp) < imd.VectorSize())) {
	 return FAILURE;
      }
   } else {
      unsigned char *line = new unsigned char[imd.Width()*3];
      for (int z = 0; z<imd.Depth(); z++) {
	 for (int y = 0; y<imd.Height(); y++) {
	    if (fread(line, sizeof(unsigned char), 3*imd.Width(), fp) < 3) {
	       return FAILURE;
	    }
	    for (int x = 0; x<imd.Width(); x++) {
	       imd(0, z, y, x) = line[x*3];
	       imd(1, z, y, x) = line[x*3+1];
	       imd(2, z, y, x) = line[x*3+2];
	    }
	 }
      }

      delete[] line;
   }
   fclose(fp);
   
   return SUCCESS;
}

#ifdef MAIN

/*
 * Modify only the following constants, and the function calls.
 */
#define	USAGE	"usage: %s bytes ncol nrow ndep color im_in [im_out|-]"
#define	PARC	5
#define	FINC	0
#define	FOUTC	1
#define	MASK	0

int main( int argc, char *argv[] ) {
   Errc result;              // The result code of the execution.
   Pobject* mask;            // The region map.
   Pobject* objin[FINC+1];   // The input objects.
   Pobject* objs[FINC+1];    // The source objects masked.
   Pobject* objout[FOUTC+1]; // The output object.
   Pobject* objd[FOUTC+1];   // The result object of the execution.
   char* parv[PARC+1];       // The input parameters.
   
   if (argc<2) {
      fprintf(stderr, USAGE"\n", argv[0]);
      Exit((Errc)FAILURE);
   }
   
   if ((argc == 2) && (!strcmp(argv[1], "-p"))) {
      fprintf(stdout, "%s 4 1 1\n", argv[0]);
      exit(0);
   }
   
   ReadArgs(argc, argv, PARC, FINC, FOUTC, &mask, objin, objs, objout, objd, parv, USAGE, MASK);

   // Look for raw filename.
   char *filename = NULL;
   if ((argc>6) && (strcmp(argv[6], "-")))
      filename = argv[6];
   
   int pixel = atoi(parv[0]);
   int ncol = atoi(parv[1]);
   int nrow = atoi(parv[2]);
   int ndep = atoi(parv[3]);
   int color = atoi(parv[4]);

   if (pixel < 0) {
      change_endian = true;
      pixel = -pixel;
   }

   if ((ndep == 0) && (nrow == 0) && color == 0) { // 1d grayscale
      switch(pixel) {
      case 1: {
	 objd[0] = new Img1duc(ncol);
	 Img1duc* const imd = (Img1duc*)objd[0];
	 
	 result = PRaw2Pan(filename, *imd, pixel);
      } break;
      case 2 :
      case 3 :
      case 4 : {
	 objd[0] = new Img1dsl(ncol);
	 Img1dsl* const imd = (Img1dsl*)objd[0];
	 
	 result = PRaw2Pan(filename, *imd, pixel);
      } break;
      case 6 :
      case 8 : {
	 objd[0] = new Img1dsf(ncol);
	 Img1dsf* const imd = (Img1dsf*)objd[0];
	 
	 result = PRaw2Pan(filename, *imd, pixel);
      } break;
      default :
	 fprintf(stderr, "Error praw2pan: Invalid pixel size\n");
	 result = FAILURE;
      }
//    } else if ((ndep == 0) && (nrow == 0) && color>0) { // 1d color
//       switch(pixel) {
//       case 1: {
// 	 objd[0] = new Imc1duc(ncol);
// 	 Imc1duc* const imd = (Imc1duc*)objd[0];
	 
// 	 result = PRaw2Pan(filename, *imd, pixel);
//       } break;
//       case 2 :
//       case 3 :
//       case 4 : {
// 	 objd[0] = new Imc1dsl(ncol);
// 	 Imc1dsl* const imd = (Imc1dsl*)objd[0];
	 
// 	 result = PRaw2Pan(filename, *imd, pixel);
//       } break;
//       case 6 :
//       case 8 : {
// 	 objd[0] = new Imc1dsf(ncol);
// 	 Imc1dsf* const imd = (Imc1dsf*)objd[0];
	 
// 	 result = PRaw2Pan(filename, *imd, pixel);
//       } break;
//       default :
// 	 fprintf(stderr, "Error praw2pan: Invalid pixel size\n");
// 	 result = FAILURE;
//       }
   } else if ((ndep == 0) && (nrow>0) && (ncol>0) && color == 0) { // 2d grayscale
      switch(pixel) {
      case 1: {
	 objd[0] = new Img2duc(nrow, ncol);
	 Img2duc* const imd = (Img2duc*)objd[0];
	    
	 result = PRaw2Pan(filename, *imd, pixel);
      } break;
      case 2 :
      case 3 :
      case 4 : {
	 objd[0] = new Img2dsl(nrow, ncol);
	 Img2dsl* const imd = (Img2dsl*)objd[0];
	    
	 result = PRaw2Pan(filename, *imd, pixel);
      } break;
      case 6 :
      case 8 : {
	 objd[0] = new Img2dsf(nrow, ncol);
	 Img2dsf* const imd = (Img2dsf*)objd[0];
	    
	 result = PRaw2Pan(filename, *imd, pixel);
      } break;
      default :
	 fprintf(stderr, "Error praw2pan: Invalid pixel size\n");
	 result = FAILURE;
      }
   } else if ((ndep == 0) && (nrow>0) && (ncol>0) && color>0) { // 2d color
      switch(pixel) {
	 case 1: {
	    objd[0] = new Imc2duc(nrow, ncol);
	    Imc2duc* const imd = (Imc2duc*)objd[0];
	    result = PRaw2Pan(filename, *imd, pixel, color);
	 } break;
//       case 2 :
//       case 3 :
//       case 4 : {
// 	 objd[0] = new Img2dsl(nrow, ncol);
// 	 Img2dsl* const imd = (Img2dsl*)objd[0];

// 	 result = PRaw2Pan(filename, *imd, pixel, color);
//       } break;
//       case 6 :
//       case 8 : {
// 	 objd[0] = new Img2dsf(nrow, ncol);
// 	 Img2dsf* const imd = (Img2dsf*)objd[0];
	    
// 	 result = PRaw2Pan(filename, *imd, pixel, color);
//       } break;
      default :
	 fprintf(stderr, "Error praw2pan: Invalid pixel size\n");
	 result = FAILURE;
      }
   } else if ((ndep>0) && (nrow>0) && (ncol>0) && color == 0) { // 3d grayscale
      switch(pixel) {
	 case 1: {
	    objd[0] = new Img3duc(ndep, nrow, ncol);
	    Img3duc* const imd = (Img3duc*)objd[0];
	    
	    result = PRaw2Pan(filename, *imd, pixel);
	 } break;
	 case 2 :
	 case 3 :
	 case 4 : {
	    objd[0] = new Img3dsl(ndep, nrow, ncol);
	    Img3dsl* const imd = (Img3dsl*)objd[0];
	    
	    result = PRaw2Pan(filename, *imd, pixel);
	 } break;
	 case 6 :
	 case 8 : {
	    objd[0] = new Img3dsf(ndep, nrow, ncol);
	    Img3dsf* const imd = (Img3dsf*)objd[0];
	    
	    result = PRaw2Pan(filename, *imd, pixel);
	 } break;
	 default :
	    fprintf(stderr, "Error praw2pan: Invalid pixel size\n");
	    result = FAILURE;
      }
   } else if ((ndep>0) && (nrow>0) && (ncol>0) && color>0) { // 3d color
      switch(pixel) {
	 case 1: {
	    objd[0] = new Imc3duc(ndep, nrow, ncol);
	    Imc3duc* const imd = (Imc3duc*)objd[0];
	    
	    result = PRaw2Pan(filename, *imd, pixel, color);
	 } break;
//       case 2 :
//       case 3 :
//       case 4 : {
// 	 objd[0] = new Imc3dsl(ndep, nrow, ncol);
// 	 Imc3dsl* const imd = (Imc3dsl*)objd[0];
	       
// 	 result = PRaw2Pan(filename, *imd, pixel);
//       } break;
//       case 6 :
//       case 8 : {
// 	 objd[0] = new Imc3dsf(ndep, nrow, ncol);
// 	 Imc3dsf* const imd = (Imc3dsf*)objd[0];
	       
// 	 result = PRaw2Pan(filename, *imd, pixel);
//       } break;
	 default :
	    fprintf(stderr, "Error praw2pan: Invalid pixel size\n");
	    result = FAILURE;
      }
   }
   if (result) {
      WriteArgs(argc, argv, PARC, FINC+1, FOUTC, &mask, objin, objs, objout, objd, MASK);
   }
   Exit(result);
   return 0;
}

#endif
