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
 * @author Regis Clouard - 1999-10-26
 * @author Regis Clouard - 2007-05-11 (add with color conversions)
 * @author Regis Clouard - 2008-05-27 (added decompression mode BITFIELDS + V4 + v5)
 */

#include <string.h>
#include <pandore.h>
using namespace pandore;

//#undef NDEBUG

/**
 * @file pbmp2pan.cpp
 * Convert BMP image to Pandore file.
 */

#ifdef _MSC_VER
#undef BI_RGB
#undef BI_RLE4
#undef BI_RLE8
#undef BI_BITFIELDS
#endif

/* BMP format constants. */
#define BI_RGB		0
#define BI_RLE8		1
#define BI_RLE4		2
#define BI_BITFIELDS    3             /* RGB bitmap with RGB masks */

#define WIN_OS2_OLD	12
#define WIN_V3		40
#define OS2_NEW		64
#define WIN_V4		108
#define WIN_V5		124

/**
 * Reads a character.
 */
#define	readc(fp) getc(fp)

/**
 * Reads a short.
 */
static unsigned int reads( FILE *fp ) {
   int b0, b1;
  
   b0 = getc(fp); 
   b1 = getc(fp);
   return ((unsigned int) b0) + (((unsigned int) b1) << 8);
}

/**
 * Reads an integer.
 */
static int readi( FILE *fp ) {
   int b0, b1, b2, b3;
  
   b0 = getc(fp); 
   b1 = getc(fp);
   b2 = getc(fp);
   b3 = getc(fp); 
   return b0 + (b1 << 8) + (b2 << 16) + (b3 << 24);
}

/**
 * Finds the rank of the first bit set of the value v.
 * @param v an integer.
 * @return the rank of the first it set.
 */
static int first_bit_set (int v) {
   unsigned int i;
   for (i = 0; i<sizeof (int) * 8;i++) {
      if (v>>i & 0x01) {
	 return i;
      }
   }
   return 0;
}

/**
 * Reads image data for black and white images.
 * @param fp the input file.
 * @param imd the output Pandore image.
 */
Errc ReadBlackWhiteData( FILE *fp, Img2duc &imd, bool bottom_up ) {
   int	i, j, k;
   int	ob;
   int	b0;
   Uchar *p;
   
#ifndef NDEBUG
   printf("-> Black and White image\n");
#endif

   b0 = 0;
   ob = ((imd.Width() + 31)/32) * 32;  /* offset for 32 bits length lines */
   const int start =  (bottom_up)? imd.Height()-1: 0;
   const int end =  (bottom_up)? 0: imd.Height()-1;
   const int step  =  (bottom_up)? -1: +1;
   for (i=start; i!=end; i+=step) {
//    for (i=imd.Height()-1; i>=0; i--) {
      p = &imd[i][0];
      for (j=k=0; j<ob; j++, k++) {
	 if ((k&7) == 0) { /* next byte */
	    b0 = readc(fp);
	    k = 0;
	 }
	 if (j<imd.Width()) {
	    *p++ = (b0 & 0x80) ? 255 : 0;
	    b0 <<= 1;
	 }
      }
   }

   return SUCCESS;
}

/**
 * Reads image data for grayscale images.
 * @param fp the input file.
 * @param imd the output Pandore image.
 */
Errc ReadGrayscaleData(FILE *fp, Img2duc &imd, int nbit, Uchar lut[][3], int compression, bool bottom_up ) {
   int	i, j, k, x, y;
   int	ob;
   int	b0, b1;
   Uchar *p;

#ifndef NDEBUG
   printf("-> Grayscale image\n");
#endif
  
   b0 = 0;
   if (nbit == 4) {	/* 4 bits */
      b0 = b1 = 0;
      if (compression == BI_RGB) {   /* no compression */
	 ob = ((imd.Width() + 7)/8) * 8; /* complement to 32 bits */
	 
   const int start =  (bottom_up)? imd.Height()-1: 0;
   const int end =  (bottom_up)? 0: imd.Height()-1;
   const int step  =  (bottom_up)? -1: +1;
   for (i=start; i!=end; i+=step) {
// 	 for (i=imd.Height()-1; i>=0; i--) {
	    p = &imd[i][0];
	    for (j=k=0; j<ob; j++, k++) {
	       if ((k & 1) == 0) { /* next byte */
		  b0 = readc(fp);
		  k = 0;
	       }
	       if (j<imd.Width()) {
		  *p++ = (b0 & 0xf0) >> 4;
		  b0 <<= 4;
	       }
	    }
	 }
      } else if (compression == BI_RLE4) {  /* RLE compression format */
	 x = y = 0;
	 p = &imd[imd.Height()-y-1][x];    
	 while (y<imd.Height()) {
	    b0 = readc(fp);
	    if (b0) {
	       b1 = readc(fp);
	       for (i=0; i<b0; i++, x++, p++) 
		  *p = (i&1) ? (b1 & 0x0f) : ((b1>>4) & 0x0f);
	    } else {	/* b0==0  :  escape codes */
	       b0 = readc(fp);
	       if (b0 == 0x00) {	/* end of line */
		  x=0;  y++; 
		  p = &imd[imd.Height()-y-1][x];
	       } else if (b0 == 0x01) {
		  break;		/* end of data */	
	       } else if (b0 == 0x02) {	/* delta */
		  b0 = readc(fp); 
		  x += b0;
		  b0 = readc(fp); 
		  y += b0;
		  p = &imd[imd.Height()-y-1][x];
	       } else {		/* absolute mode */
		  for (i=0; i<b0; i++, x++, p++) {
		     if ((i&1) == 0)
			b1 = readc(fp);
		     *p = (i&1) ? (b1 & 0x0f) : ((b1>>4) & 0x0f);
		  }
		  if (((b0&3)==1) || ((b0&3)==2))
		     readc(fp);  /* Skip end of line*/
	       }
	    }
	 }
      } else {
	 fprintf(stderr, "Error pbmp2pan: Bad input file: Unknow compression format\n");
	 return FAILURE;
      }
   } else if (nbit == 8) {	/* 8 bits */
      if (compression == BI_RGB) {   /* no compression */
	 ob = ((imd.Width() + 3)/4) * 4; /* complement to 32 bits */
	 const int start =  (bottom_up)? imd.Height()-1: 0;
	 const int end =  (bottom_up)? 0: imd.Height()-1;
	 const int step  =  (bottom_up)? -1: +1;
	 for (i=start; i!=end; i+=step) {
// 	 for (i=imd.Height()-1; i>=0; i--) {
	    for (j=0; j<ob; j++) {
	       b0 = readc(fp);
	       if (j<imd.Width()) {
		  imd[i][j] = b0;
	       }
	    }
	 }
      } else if (compression == BI_RLE8) {  /* RLE compression format */
	 x = y = 0;  
	 p = &imd[imd.Height()-y-1][x];
	 while (y<imd.Height()) {
	    b0 = readc(fp);
	    if ((b0)) {	/* encoded mode */
	       b1 = readc(fp);
	       for (i=0; i<b0; i++, x++, p++) {
		  if (x<imd.Width()) {
		     *p = b1;
		  }
	       }
	    } else {    /* b0==0  :  escape codes */
	       b0 = readc(fp);
	       if (b0 == 0x00) {	/* end of line */
		  x=0;  y++;  p = &imd[imd.Height()-y-1][0];
	       } else if (b0 == 0x01) {/* end of bitmap */
		  break;
	       } else if (b0 == 0x02) {	/* delta */
		  b0 = readc(fp);  x += b0;
		  b0 = readc(fp);  y += b0;
		  p = &imd[imd.Height()-y-1][x];
	       }else {
		  for (i=0; i<b0; i++, x++, p++) {
		     b1 = readc(fp);
		     *p = b1;
		  }
		  if (b0 & 1)
		     readc(fp); 
	       }
	    } 
	 }
      }else {
	 fprintf(stderr, "Error pbmp2pan: Bad input file: Unknown format\n");
	 return FAILURE;
      }
   }

   // applies the lut transform.
   for (i = 0; i<imd.Height(); i++) {
      for (j = 0; j<imd.Width(); j++) {
	 imd[i][j] = lut[imd[i][j]][0];
      }
   }
   return SUCCESS;
}

/**
 * Reads image data for grayscale images.
 * @param fp the input file.
 * @param imd the output Pandore image.
 */
Errc ReadColorData(FILE *fp, Imc2duc &imd, int nbit, Uchar lut[][3], int compression, bool bottom_up ) {
   int	i, j, k, x, y;
   int	ob;
   int	b0, b1;
   Uchar *p;
  
   b0 = 0;
   if (nbit == 4) {	/* 4 bits */
#ifndef NDEBUG
   printf("-> Color image 4 bits\n");
#endif
      b0 = b1 = 0;
      
      if (compression == BI_RGB) {   /* no compression */
	 ob = ((imd.Width() + 7)/8) * 8; /* complement to 32 bits */
	 
	 const int start =  (bottom_up)? imd.Height()-1: 0;
	 const int end =  (bottom_up)? 0: imd.Height()-1;
	 const int step  =  (bottom_up)? -1: +1;
	 for (i=start; i!=end; i+=step) {
// 	 for (i=imd.Height()-1; i>=0; i--) {
	    p = &imd.X[i][0];
	    for (j=k=0; j<ob; j++, k++) {
	       if ((k & 1) == 0) { /* next byte */
		  b0 = readc(fp);
		  k = 0;
	       }
	       if (j<imd.Width()) {
		  *p++ = (b0 & 0xf0) >> 4;
		  b0 <<= 4;
	       }
	    }
	 }
      } else if (compression == BI_RLE4) {  /* RLE4 compression format */
#ifndef NDEBUG
	 printf("    with RLE encoding\n");
#endif
	 x = y = 0;
	 p = &imd.X[imd.Height()-y-1][x];    
	 while (y<imd.Height()) {
	    b0 = readc(fp);
	    if (b0) {
	       b1 = readc(fp);
	       for (i=0; i<b0; i++, x++, p++) 
		  *p = (i&1) ? (b1 & 0x0f) : ((b1>>4) & 0x0f);
	    } else {	/* b0==0  :  escape codes */
	       b0 = readc(fp);
	       if (b0 == 0x00) {	/* end of line */
		  x=0;  y++; 
		  p = &imd.X[imd.Height()-y-1][x];
	       } else if (b0 == 0x01) {
		  break;		/* end of data */	
	       } else if (b0 == 0x02) {	/* delta */
		  b0 = readc(fp); 
		  x += b0;
		  b0 = readc(fp); 
		  y += b0;
		  p = &imd.X[imd.Height()-y-1][x];
	       } else {		/* absolute mode */
		  for (i=0; i<b0; i++, x++, p++) {
		     if ((i&1) == 0)
			b1 = readc(fp);
		     *p = (i&1) ? (b1 & 0x0f) : ((b1>>4) & 0x0f);
		  }
		  if (((b0&3)==1) || ((b0&3)==2))
		     readc(fp);  /* skip end of line */
	       }
	    }
	 }
      } else {
	 fprintf(stderr, "Error pbmp2pan: Bad input file: Unknow compression format\n");
	 return FAILURE;
      }
      // Applies the lut transform.
      for (int i = 0; i<imd.Height(); i++) {
	 for (int j = 0; j<imd.Width(); j++) {
	    Uchar c=imd.X[i][j];
	    imd.Z[i][j] = lut[c][0];
	    imd.Y[i][j] = lut[c][1];
	    imd.X[i][j] = lut[c][2];
	 }
      }
   } else if (nbit == 8) {	/* 8 Bits */
#ifndef NDEBUG
      printf("-> Color image 8 bits\n");
#endif
      if (compression == BI_RGB) {   /* no compression */
	 ob = ((imd.Width() + 3)/4) * 4; /* complement to 32 bits */
	 const int start =  (bottom_up)? imd.Height()-1: 0;
	 const int end =  (bottom_up)? 0: imd.Height()-1;
	 const int step  =  (bottom_up)? -1: +1;
	 for (i=start; i!=end; i+=step) {
// 	 for (i=imd.Height()-1; i>=0; i--) {
	    for (j=0; j<ob; j++) {
	       b0 = readc(fp);
	       if (j<imd.Width()) {
		  imd.X[i][j] = lut[b0][2];
  		  imd.Y[i][j] = lut[b0][1];
 		  imd.Z[i][j] = lut[b0][0];
	       }
	    }
	 }
      } else if (compression == BI_RLE8) {  /* RLE compression format */
#ifndef NDEBUG
	 printf("    with RLE encoding\n");
#endif
	 x = y = 0;  
	 p = &imd.X[imd.Height()-y-1][x];
	 while (y<imd.Height()) {
	    b0 = readc(fp);
	    if (b0) {	/* encoded mode */
	       b1 = readc(fp);
	       for (i=0; i<b0; i++, x++, p++)
		  *p = b1;
	    } else {    /* b0==0  :  escape codes */
	       b0 = readc(fp);
	       if (b0 == 0x00) {	/* end of line */
		  x=0;  y++;  p = &imd.X[imd.Height()-y-1][x];
	       } else if (b0 == 0x01)
		  break;	/* end of data */
	       else if (b0 == 0x02) {	/* delta */
		  b0 = readc(fp);  x += b0;
		  b0 = readc(fp);  y += b0;
		  p = &imd.X[imd.Height()-y-1][x];
	       } else {
		  for (i=0; i<b0; i++, x++, p++) {
		     b1 = readc(fp);
		     *p = b1;
		  }
		  if (b0 & 1)
		     readc(fp); 
	       }
	    } 
	 }
	 // Applies the lut transform.
	 for (int i = 0; i<imd.Height(); i++) {
	    for (int j = 0; j<imd.Width(); j++) {
	       Uchar c=imd.X[i][j];
	       imd.Z[i][j] = lut[c][0];
	       imd.Y[i][j] = lut[c][1];
	       imd.X[i][j] = lut[c][2];
	    }
	 }
      } else {
	 fprintf(stderr, "Error pbmp2pan: Bad input file: Unknown format\n");
	 return FAILURE;
      }
   }
   
   return SUCCESS;
}

struct BitfieldMask {
      unsigned int red_mask;
      unsigned int green_mask;
      unsigned int blue_mask;
};

/*
 * Reads data for color images.
 * @param fp the input file.
 * @param imd the output Pandore image.
 */
Errc ReadTrueColorsData( FILE *fp, Imc2duc &imd, int nbit, struct BitfieldMask masks, bool bottom_up ) {
   int i, j;

#ifndef NDEBUG
   printf("-> True colors image with %d bits\n", nbit);
#endif

   imd.ColorSpace(RGB);
   switch (nbit) {
   case 16: {
      const int r_shift = first_bit_set (masks.red_mask);
      const int g_shift = first_bit_set (masks.green_mask);
      const int b_shift = first_bit_set (masks.blue_mask);
#ifndef NDEBUG
      printf("  --> masks %x %x %x\n", masks.red_mask, masks.green_mask, masks.blue_mask);
      printf("  --> shitf %d %d %d\n", r_shift, g_shift, b_shift);
#endif

      int bf = (4 - ((imd.Width()*2) % 4)) & 0x03;  /* complement to 32 bits of needed. */
      const int start =  (bottom_up)? imd.Height()-1: 0;
      const int end =  (bottom_up)? 0: imd.Height()-1;
      const int step  =  (bottom_up)? -1: +1;
      for (i=start; i!=end; i+=step) {
//       for (i=imd.Height()-1; i>=0; i--) {
	 // Lit les couleurs.
	 for (j=0; j<imd.Width(); j++) {
	    unsigned int a = reads(fp);
	    imd.X[i][j] = (Uchar) (((a & masks.red_mask)   >> r_shift) <<3); /* red */
	    imd.Y[i][j] = (Uchar) (((a & masks.green_mask) >> g_shift) <<3); /* green */
	    imd.Z[i][j] = (Uchar) (((a & masks.blue_mask)  >> b_shift) <<3); /* blue */
	 }
	 // Skip end of lines (complement to 32 bits).
	 for (j=0; j<bf; j++)
	    readc(fp);
      }
   }
      break;

   case 24: {
      int bf = (4 - ((imd.Width()*3) % 4)) & 0x03;  /* complement to 32 bits of needed. */
      const int start =  (bottom_up)? imd.Height()-1: 0;
      const int end =  (bottom_up)? 0: imd.Height()-1;
      const int step  =  (bottom_up)? -1: +1;
      for (i=start; i!=end; i+=step) {
//       for (i=imd.Height()-1; i>=0; i--) {
	 // Lit les couleurs.
	 for (j=0; j<imd.Width(); j++) {
	    imd.Z[i][j] = (Uchar)readc(fp);   /* blue */
	    imd.Y[i][j] = (Uchar)readc(fp);   /* green */
	    imd.X[i][j] = (Uchar)readc(fp);   /* red */
	 }	
	 // Skip end of lines (complement to 32 bits).
	 for (j=0; j<bf; j++)
	    readc(fp);
      }
   }
      break;
   case 32: 
      const int r_shift = first_bit_set (masks.red_mask);
      const int g_shift = first_bit_set (masks.green_mask);
      const int b_shift = first_bit_set (masks.blue_mask);
#ifndef NDEBUG
      printf("  --> masks %x %x %x\n", masks.red_mask, masks.green_mask, masks.blue_mask);
      printf("  --> shitf %d %d %d\n", r_shift, g_shift, b_shift);
#endif
      const int start =  (bottom_up)? imd.Height()-1: 0;
      const int end =  (bottom_up)? 0: imd.Height()-1;
      const int step  =  (bottom_up)? -1: +1;
      for (i=start; i!=end; i+=step) {
//       for (i=imd.Height()-1; i>=0; i--) {
	 // Lit les couleurs.
	 for (j=0; j<imd.Width(); j++) {
	    unsigned int a = readi(fp);
	    imd.X[i][j] = (Uchar) ((a & masks.red_mask)   >> r_shift); /* red */
	    imd.Y[i][j] = (Uchar) ((a & masks.green_mask) >> g_shift); /* green */
	    imd.Z[i][j] = (Uchar) ((a & masks.blue_mask)  >> b_shift); /* blue */
	 } 
      }
      break;
   }
   return SUCCESS;
}

bool isBMP( FILE *fp ) {
   int b, m;
   b = readc(fp);  m = readc(fp);
   if (!
       (b=='B' && m=='M') ||	// Windows Bitmap
       (b=='B' && m=='A') ||	// OS/2 Bitmap
       (b=='C' && m=='I') ||	// OS/2 color icon
       (b=='C' && m=='P') ||	// OS/2 icon
       (b=='I' && m=='C') ||	// OS/2 Pointer 
       (b=='P' && m=='T') ) {
      return false;
   }
   return true;
}

/*
 * Converts the input BMP file
 * into the output Pandore image.
 * @param fp the input file.
 * @param objout the output Pandore image.
 * @return SUCCESS or FAILURE.
 */
Errc PBmp2Pan( FILE *fp, Pobject **objout ) {
   int offset, filesize;
   Errc result=SUCCESS;
   unsigned long	nlig, ncol, nbit;
   Uchar lut[256][3];
   unsigned int	i;
   int	biHeight;
   unsigned int	bfSize, bfOffBits, biSize, biWidth, biPlanes;
   unsigned int	biBitCount, biCompression, biSizeImage, biXPelsPerMeter;
   unsigned int	biYPelsPerMeter, biClrUsed, biClrImportant;
   struct BitfieldMask masks;
   bool bottom_up=true;

   fseek(fp, 0L, SEEK_END);
   filesize = ftell(fp);
   fseek(fp, 0L, SEEK_SET);
  
   // 1. FILE HEADER.
   if (!isBMP(fp)) { return FAILURE; }

   bfSize = readi(fp);		// file size.
   readi(fp);			// unused
  
   bfOffBits = readi(fp);	// data offset.

   // 2. INFORMATION HEADER
   biSize = readi(fp);

#ifndef NDEBUG
   switch(biSize) {
   case WIN_V3: puts("type=Windows 3");break;
   case WIN_V4: puts("type=Windows 4");break;
   case WIN_V5: puts("type=Windows 5");break;
   case OS2_NEW: puts("type=OS2");break;
   default: puts("Windows 2");break;
   }
#endif
   if (biSize == WIN_V3 || biSize == OS2_NEW || biSize == WIN_V4 || biSize == WIN_V5) {
      biWidth         = readi(fp);
      biHeight        = readi(fp);
      if (biHeight<0) { bottom_up=false; biHeight=-biHeight; } // image is a top-down bitmap
      biPlanes        = reads(fp);
      biBitCount      = reads(fp);
      biCompression   = readi(fp);
      biSizeImage     = readi(fp);
      biXPelsPerMeter = readi(fp);
      biYPelsPerMeter = readi(fp);
      biClrUsed       = readi(fp);
      biClrImportant  = readi(fp);
   } else if (biSize == WIN_OS2_OLD) {    // Old format.
      biWidth         = reads(fp);
      biHeight        = reads(fp);
      if (biHeight<0) { bottom_up=false; biHeight=-biHeight; } // image is a top-down bitmap
      biPlanes        = reads(fp);
      biBitCount      = reads(fp);
      biSizeImage     = (((biPlanes * biBitCount*biWidth)+31)/32)*4*biHeight;
      biCompression   = BI_RGB; 
      biXPelsPerMeter = biYPelsPerMeter = 0;
      biClrUsed       = biClrImportant  = 0;
   } else {
      fprintf(stderr, "Error pbmp2pan: Unrecognized BMP format\n");
      return FAILURE;
   }
  
#ifndef NDEBUG
   printf("Width=%d\n", biWidth);
   printf("height=%d\n", biHeight);
   printf("planes=%d\n", biPlanes);
   printf("bit count=%d\n", biBitCount);
   printf("compression =%d\n", biCompression);
   printf("image size=%d\n", biSizeImage);
   printf("x pixel per meter=%d\n", biXPelsPerMeter);
   printf("y pixel per meter=%d\n", biYPelsPerMeter);
   printf("color number=%d\n", biClrUsed);
   printf("important colors =%d\n", biClrImportant);
#else
   // Suppress warnings
   (void)biSizeImage;
   (void)biXPelsPerMeter;
#endif

   offset = 0;

   // skip to colors.
   if (biSize != WIN_OS2_OLD) {
      for (i=0; i<(biSize-40); i++) {
	 readc(fp);
      }
      // BMP error : bad file size -> skip to the end of file.
      offset = bfOffBits - (biSize + 14);
   }

   if (biBitCount==16) {
      masks.red_mask   = 0x001F<<10;
      masks.green_mask = 0x001F<<5;
      masks.blue_mask  = 0x001F; 
   }
   if (biBitCount==32) {
      masks.red_mask   = 0xFF<<16;
      masks.green_mask = 0xFF<<8;
      masks.blue_mask  = 0xFF;
   }
   
   // load colors.
   bool colorImage = false; // flag true if it is a color image, false if is a grayscale image.
   if (biBitCount<16) {
      ncol = (biClrUsed) ? biClrUsed : 1 << biBitCount;
      for (i=0; i<ncol; i++) {
	 lut[i][0] = readc(fp);
	 lut[i][1] = readc(fp);
	 lut[i][2] = readc(fp);
	 // determines if it is grayscale image
	 // or a color image.
	 if (lut[i][0] != lut[i][1] || lut[i][0] != lut[i][2] || lut[i][1] != lut[i][2]) {
	    colorImage=true;
	 }
	 if (biSize != WIN_OS2_OLD) {
	    readc(fp);
	    offset -= 4;
	 }
      }
   } else if (biSize == WIN_V4) {	// BMP version 4
//        masks.red_mask = readi(fp);
//        masks.green_mask = readi(fp);
//        masks.blue_mask = readi(fp);
//        offset -=12;
#ifndef NDEBUG
      puts("  # version 4");
      printf("  # masks %x %x %x\n", masks.red_mask, masks.green_mask, masks.blue_mask);
#endif
   } else if (biCompression == BI_BITFIELDS) {	// skip colors.
      masks.red_mask = readi(fp);
      masks.green_mask = readi(fp);
      masks.blue_mask = readi(fp);
      offset -=12;
#ifndef NDEBUG
      puts("  # bitfields");
      printf("  # masks %x %x %x\n", masks.red_mask, masks.green_mask, masks.blue_mask);
#endif
   } else {	// skip colors.
      offset = bfSize - filesize - offset;
   }
   // skip to data.
   if (biSize != WIN_OS2_OLD) {
      while (offset > 0) {
	 (void) readc(fp);
	 offset--;
      }
   }
  
   ncol = biWidth;
   nlig = biHeight;
   nbit = biBitCount;
  
   if (nbit == 1) {
      *objout = new Img2duc(nlig, ncol);
      result=ReadBlackWhiteData(fp, (*(Img2duc*)*objout), bottom_up);
   } else if (nbit == 4 || nbit==8) {
      if (colorImage) {
 	 *objout = new Imc2duc(nlig, ncol);
 	 result=ReadColorData(fp, (*(Imc2duc*)*objout), nbit, lut, biCompression, bottom_up);
      } else {
	 *objout = new Img2duc(nlig, ncol);
	 result=ReadGrayscaleData(fp, (*(Img2duc*)*objout), nbit, lut, biCompression, bottom_up);
      }
   } else if (nbit == 16 || nbit == 24 || nbit == 32) { 
      *objout = new Imc2duc(nlig, ncol);
      result=ReadTrueColorsData(fp, (*(Imc2duc*)*objout), nbit, masks, bottom_up);
   } else {
       return FAILURE;
   }

   return result;
}

#ifdef MAIN
	
/*
 * Modify only the following constants, and the function call.
 */
#define	USAGE	"usage: %s im_in [im_out|-]"
#define	PARC	1
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

   if ((argc==2) && (!strcmp(argv[1], "-p"))) {
      fprintf(stdout, "%s 0 1 1\n", argv[0]);
      exit(0);
   }

   if ((argc==2) && (!strcmp(argv[1], "-s"))) {
      fprintf(stdout, "\n");
      exit(0);
   }
   	
   ReadArgs(argc, argv, PARC, FINC, FOUTC, &mask, objin, objs, objout, objd, parv, USAGE, MASK);

   // look for input filename.
   FILE* fdin = stdin;
   char* nomfichier = (argc > 1)? argv[1] : NULL;
   if ((nomfichier) && (strcmp(nomfichier, "-"))) {
      if ((fdin = fopen(nomfichier, "rb")) == NULL) {
	 fprintf(stderr, "Error pbmp2pan: Cannot open file %s\n", nomfichier);
	 Exit(FAILURE);
      }
   }      

   if ((result = PBmp2Pan(fdin, &objd[0])) == SUCCESS) {
      WriteArgs(argc, argv, PARC, FINC, FOUTC, &mask, objin, objs, objout, objd, MASK);
   } else {
      fprintf(stderr, "Error pbmp2pan: Bad BMP file: %s.\n", nomfichier);
   }
   
   fclose(fdin);   
   Exit(result);
   return 0;
}

#endif
