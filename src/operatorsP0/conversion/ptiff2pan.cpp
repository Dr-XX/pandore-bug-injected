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
 * @author Regis Clouard - 2001-02-23
 * @author Regis Clouard - 2005-05-04 (fix bug).
 */

#include <string.h>
#include <pandore.h>
using namespace pandore;

/**
 * @file ptiff2pan.cpp
 * Convert TIFF images to Pandore images.
 */ 

// Data type
#define BYTE 1
#define ASCII 2
#define SHORT 3
#define LONG 4
#define RATIONAL 5

// Tag List
#define IMAGEWIDTH 256
#define IMAGELENGTH 257
#define BITSPERSAMPLE 258
#define COMPRESSION 259
#define PHOTOMETRICINTERPRETATION 262
#define FILLORDER 266
#define STRIPOFFSETS 273
#define ORIENTATION 274
#define SAMPLESPERPIXEL 277
#define ROWSPERSTRIP 278
#define STRIPBYTECOUNTS 279
#define XRESOLUTION 282
#define YRESOLUTION 283
#define PLANARCONFIGURATION 284
#define RESOLUTIONUNIT 296
#define SOFTWARE 305
#define COLORMAP 320
#define EXTRASAMPLES 338

struct  TIFHEAD {
   short identifier;	// Byte order identifier
   short version;	// TIFF version number
   Long ifdoffset;	// Offset of the first Image File Directory
};

struct TIFTAG {
   short tagid;		// Tag identifier
   short datatype;	// The scalar type of data 1:byte, 2:ascii, 3:short, 4:long...
   Long datacount;	// The number of items in the tag data
   Long dataoffset;	// The byte offset of the data items
};

struct TAGPOS {
   short tagid;
   Long filepos;
};

#define MAXTAG 30

struct TIFF {
      FILE* fp;
      Long ifdoffset;
      Long fillorder;
      Long photometricinterpretation;
      Long nextifdoffset;
      Long ImageWidth;
      Long ImageHeight;
      Long planarconfiguration;
      Long bitspersample;
      Long RowsPerStrip;
      Long stripoffsets;
      Long stripbytecounts;
      Long StripsCount;
      Long orientation;
      Long SamplesPerPixel;
      Long colormapoffset;
      Long colormapcount;
      Long extrasamples;
      unsigned char *red;
      unsigned char *green;
      unsigned char *blue;
      TIFF(): fp(0), red(0), green(0), blue(0) {}
      ~TIFF() {
 	 if (red) { delete[] red; red=0; }
 	 if (green) {delete[] green; green=0; }
 	 if (blue) { delete[] blue; blue=0;}
      }
};

int mode; // mode=1, Invert Bytes Order after reading.

#define _BIG_ENDIAN_      0 // most to least significant byte order
#define _LITTLE_ENDIAN_   1 // least to most significant byte order

/*
 * Determine the byte order, 
 * and set the endian global variable.
 */
int determineByteOrder(void) {
   short int word = 0x0001;
   char *byte = (char *) &word;

   return (byte[0] ? _LITTLE_ENDIAN_ : _BIG_ENDIAN_);
}

/*
 * reads a short in file, 
 * but reverse high and low bytes, 
 * in case of _BIG_ENDIAN_
 */
void readBytes( FILE *fp, short *s ) {
   union {
      short s;
      char c[2];
   } u, v;
   
   if (fread(&u.s, sizeof(short), 1, fp)<1) {
      u.s=0;
   }
   if (mode) {
      v.c[0]=u.c[1];
      v.c[1]=u.c[0];
      *s= v.s;
   } else {
      *s = u.s;
   }
}

/*
 * Reads a long in file, 
 * but reverse high and low bytes, 
 * in case of _BIG_ENDIAN_
 */
void readBytes( FILE *fp, Long *l ) {
   union {
      Long l;
      char c[4];
   } u, v;
  
   if (fread(&u.l, sizeof(Long), 1, fp) < 1) {
      u.l=0;
   }
   if (mode) {
      v.c[0]=u.c[3];
      v.c[1]=u.c[2];
      v.c[2]=u.c[1];
      v.c[3]=u.c[0];
      *l= v.l;
   } else {
      *l= u.l;
   }
}

/*
 * Read and return the next byte of
 * input stream.
 * If fillorder=2  reverse byte using the
 * following table.
 */
unsigned char BYTEREVTABLE[256] = {
   0x00, 0x80, 0x40, 0xc0, 0x20, 0xa0, 0x60, 0xe0, 
   0x10, 0x90, 0x50, 0xd0, 0x30, 0xb0, 0x70, 0xf0, 
   0x08, 0x88, 0x48, 0xc8, 0x28, 0xa8, 0x68, 0xe8, 
   0x18, 0x98, 0x58, 0xd8, 0x38, 0xb8, 0x78, 0xf8, 
   0x04, 0x84, 0x44, 0xc4, 0x24, 0xa4, 0x64, 0xe4, 
   0x14, 0x94, 0x54, 0xd4, 0x34, 0xb4, 0x74, 0xf4, 
   0x0c, 0x8c, 0x4c, 0xcc, 0x2c, 0xac, 0x6c, 0xec, 
   0x1c, 0x9c, 0x5c, 0xdc, 0x3c, 0xbc, 0x7c, 0xfc, 
   0x02, 0x82, 0x42, 0xc2, 0x22, 0xa2, 0x62, 0xe2, 
   0x12, 0x92, 0x52, 0xd2, 0x32, 0xb2, 0x72, 0xf2, 
   0x0a, 0x8a, 0x4a, 0xca, 0x2a, 0xaa, 0x6a, 0xea, 
   0x1a, 0x9a, 0x5a, 0xda, 0x3a, 0xba, 0x7a, 0xfa, 
   0x06, 0x86, 0x46, 0xc6, 0x26, 0xa6, 0x66, 0xe6, 
   0x16, 0x96, 0x56, 0xd6, 0x36, 0xb6, 0x76, 0xf6, 
   0x0e, 0x8e, 0x4e, 0xce, 0x2e, 0xae, 0x6e, 0xee, 
   0x1e, 0x9e, 0x5e, 0xde, 0x3e, 0xbe, 0x7e, 0xfe, 
   0x01, 0x81, 0x41, 0xc1, 0x21, 0xa1, 0x61, 0xe1, 
   0x11, 0x91, 0x51, 0xd1, 0x31, 0xb1, 0x71, 0xf1, 
   0x09, 0x89, 0x49, 0xc9, 0x29, 0xa9, 0x69, 0xe9, 
   0x19, 0x99, 0x59, 0xd9, 0x39, 0xb9, 0x79, 0xf9, 
   0x05, 0x85, 0x45, 0xc5, 0x25, 0xa5, 0x65, 0xe5, 
   0x15, 0x95, 0x55, 0xd5, 0x35, 0xb5, 0x75, 0xf5, 
   0x0d, 0x8d, 0x4d, 0xcd, 0x2d, 0xad, 0x6d, 0xed, 
   0x1d, 0x9d, 0x5d, 0xdd, 0x3d, 0xbd, 0x7d, 0xfd, 
   0x03, 0x83, 0x43, 0xc3, 0x23, 0xa3, 0x63, 0xe3, 
   0x13, 0x93, 0x53, 0xd3, 0x33, 0xb3, 0x73, 0xf3, 
   0x0b, 0x8b, 0x4b, 0xcb, 0x2b, 0xab, 0x6b, 0xeb, 
   0x1b, 0x9b, 0x5b, 0xdb, 0x3b, 0xbb, 0x7b, 0xfb, 
   0x07, 0x87, 0x47, 0xc7, 0x27, 0xa7, 0x67, 0xe7, 
   0x17, 0x97, 0x57, 0xd7, 0x37, 0xb7, 0x77, 0xf7, 
   0x0f, 0x8f, 0x4f, 0xcf, 0x2f, 0xaf, 0x6f, 0xef, 
   0x1f, 0x9f, 0x5f, 0xdf, 0x3f, 0xbf, 0x7f, 0xff
};

unsigned int readPixel( TIFF *tif ) {
   unsigned char byte=0;

   if (fread(&byte, sizeof(unsigned char), 1, tif->fp)<1) {
      byte=0;
   }
   if (tif->fillorder == 2)  {
      byte = BYTEREVTABLE[byte];
   }
   return byte;
}

/*
 * Return the value from a tag data.
 * From the tag type return either
 * the long or the short value.
 */
int getValue( TIFTAG tiftag ) {
   union{ Ulong l; unsigned short s[2];} v;

   v.l=tiftag.dataoffset;
   
   if (tiftag.datatype == SHORT) {
      if (mode) {
	 v.l = v.s[1];
      } else {
	 v.l = v.s[0];
      }
   }

   return v.l;
}


/*
 * Return the value from a tag data.
 * From the tag type return either
 * the long or the short value.
 */
int getOffset( TIFTAG tiftag ) {
   return tiftag.dataoffset;
}

/*
 * Read the TIFF header. 
 */
TIFHEAD TIFFGetHeader( TIFF *tif ) {
   TIFHEAD tifhead;
  
   readBytes(tif->fp, &tifhead.identifier);
   readBytes(tif->fp, &tifhead.version);
   readBytes(tif->fp, &tifhead.ifdoffset);

   return tifhead;
}

/*
 * Read a tag in the TIFF file.
 * Convert as needed bytes order.
 */
TIFTAG TIFFGetTag( TIFF *tif ) {
   TIFTAG tiftag;

   readBytes(tif->fp, &tiftag.tagid);
   readBytes(tif->fp, &tiftag.datatype);
   readBytes(tif->fp, &tiftag.datacount);
   readBytes(tif->fp, &tiftag.dataoffset);
  
   return tiftag;
}

bool isTIFF( FILE * fd) {
   int a = fgetc(fd);
   int b = fgetc(fd);

   return ((a=='I' && b=='I') || (a=='M' && b=='M'));
}

/*
 * Opens a TIFF file and
 * checks file type.
 */
TIFF* TIFFOpen( FILE * fp) {
   union{ short s; char c[2]; } identifier;

   TIFF *tif = new TIFF();
   tif ->fp=fp;
   int endian=determineByteOrder();


   // Default values.
   tif->SamplesPerPixel=1;
   tif->bitspersample=8;
   tif->RowsPerStrip=MAXLONG;
   tif->fillorder=1;
   tif->orientation=1;
   
   // Check TIFF consistency.
   if ( (fread(&identifier, sizeof(short), 1, tif->fp)<1)
	|| (identifier.c[0] != identifier.c[1])) {
      delete tif;
      return 0;
   }
   // Mode =little_endian or big_endian?
   if (identifier.c[0]=='M') {
      if (endian==_LITTLE_ENDIAN_)
	 mode=1;
      else
	 mode=0;
   } else {
      if (identifier.c[0]=='I') {
	 if (endian==_LITTLE_ENDIAN_) {
	    mode=0;
	 } else {
	    mode=1;
	 }
      }
   }

   // Rewind.
   fseek(tif->fp, 0, SEEK_SET);
   TIFHEAD tifhead=TIFFGetHeader(tif);
   
   if (tifhead.version != 42) {
      return 0;
   }
   
   tif->ifdoffset=tifhead.ifdoffset;
   
   return tif;
}

/*
 * Read the palette-color.
 */
void TIFFGetPalette( TIFF *tif ) {
   unsigned short color;
   Long cur_offset;
   Long count=tif->colormapcount;
   int c;

#define	CVT(x) (unsigned char)((((int) x) * 255) / ((1L<<16)-1))

   cur_offset=ftell(tif->fp);
   fseek(tif->fp, tif->colormapoffset, SEEK_SET);

   tif->red = new unsigned char[tif->colormapcount];
   tif->green = new unsigned char[tif->colormapcount];
   tif->blue = new unsigned char[tif->colormapcount];

   for (c=0; c<count/3; c++) {
      readBytes(tif->fp, (short*)&color);
      tif->red[c]= CVT(color);
   }
   for (c=0; c<count/3; c++) {
      readBytes(tif->fp, (short*)&color);
      tif->green[c]= CVT(color);
   }  	
   for (c=0; c<count/3; c++) {
      readBytes(tif->fp, (short*)&color);
      tif->blue[c]= CVT(color);	
   }

   fseek(tif->fp, cur_offset, SEEK_SET);
}

/*
 * Reads an Image File Directory (i.e., list of TAGs)
 * and gathered properties in order to create
 * the relevant Pandore image.
 */
Errc TIFFGetIDF( TIFF * tif ) {
   short rep_suiv;
   TIFTAG tiftag;
   short count;

   fseek(tif->fp, tif->ifdoffset, SEEK_SET);

   // Number of tags
   readBytes(tif->fp, &count);

   for (int j=0; j<count; j++) {
      tiftag=TIFFGetTag(tif);
      switch(tiftag.tagid) {
      case IMAGEWIDTH:
	 tif->ImageWidth=getValue(tiftag);
	 break;
      case IMAGELENGTH:
	 tif->ImageHeight=getValue(tiftag);
	 break;
      case BITSPERSAMPLE:
	 tif->bitspersample=getValue(tiftag);
	 break;
      case COMPRESSION:
	 if (getValue(tiftag)!=1) {
	    fprintf(stderr, "Error ptiff2pan: Cannot read compressed TIFF file.\n");
	    Exit(FAILURE);
	    return FAILURE;
	 }
	 break;
      case PHOTOMETRICINTERPRETATION:
	 tif->photometricinterpretation=getValue(tiftag);
	 break;
      case FILLORDER:
	 tif->fillorder=getValue(tiftag);
	 break;
      case STRIPOFFSETS:
	 tif->stripoffsets=getOffset(tiftag);
	 tif->StripsCount=tiftag.datacount;
	 break;
      case SAMPLESPERPIXEL:
	 tif->SamplesPerPixel=getValue(tiftag);
	 break;
      case ORIENTATION:
	 tif->orientation=getValue(tiftag);
	 break;
      case ROWSPERSTRIP:
	 tif->RowsPerStrip=getValue(tiftag);
	 break;
      case STRIPBYTECOUNTS:
	 tif->stripbytecounts=getValue(tiftag);
	 break;
      case XRESOLUTION: break;
      case YRESOLUTION: break;
      case PLANARCONFIGURATION:
	 tif->planarconfiguration=getValue(tiftag);
	 break;
      case RESOLUTIONUNIT: break;
      case EXTRASAMPLES: 
	 tif->extrasamples=getValue(tiftag);
	 break;
      case COLORMAP:
	 tif->colormapcount=tiftag.datacount;
	 tif->colormapoffset=getOffset(tiftag);
	 TIFFGetPalette(tif);
	 break;
      default : break;
      }
   }
   rep_suiv=0;
   // lecture de l'adresse du prochain IFD
   readBytes(tif->fp, &rep_suiv);      
   tif->ifdoffset=rep_suiv;

   return SUCCESS;
}

/*
 * Create a grayscale image.
 */
Pobject* CreateImc2duc( TIFF *tif ) {
   Imc2duc* imd=new Imc2duc(3, tif->ImageHeight, tif->ImageWidth);
   Long *stripOffset = new Long[tif->StripsCount+1];
   Long *stripByteCount = new Long[tif->StripsCount+1];

   // Get offset of each strips.
   fseek(tif->fp, tif->stripoffsets, 0);

   if (tif->StripsCount!=1) {
      for (int w=0;w<(int)tif->StripsCount;w++) {
	 readBytes(tif->fp, &stripOffset[w]);
      }
   } else {	
      stripOffset[0] = tif->stripoffsets;
   }
   
   // Get the number of bytes in a strip.
   fseek(tif->fp, tif->stripbytecounts, 0);
   if (tif->StripsCount!=1) {
      for (int u=0;u<(int)tif->StripsCount;u++) {
	 readBytes(tif->fp, &stripByteCount[u]);
      }
   } else {
      stripByteCount[0] = tif->stripbytecounts;
   }
   
   unsigned char byte;
   int k = 0;
   int l = 0;

   /* 
    * Palette color
    */
   if (tif->photometricinterpretation==3) {

      for (int i=0;i<(int)tif->StripsCount;i++) {
	 fseek(tif->fp, stripOffset[i], SEEK_SET);

	 for (int j=0;j<stripByteCount[i];j++) {
	    byte=readPixel(tif);
	    if (tif->bitspersample==4) { // 4 gray.
	       for (int b=0; b<2; b++) {
		  unsigned char p=byte & 0xF0;
		  byte<<=4;
		  
		  (*imd)(0, k, l)=tif->red[p];
		  (*imd)(1, k, l)=tif->green[p];
		  (*imd)(2, k, l)=tif->blue[p];
		  l++;
		  if (l == tif->ImageWidth) {l=0;k++;}
	       }
	    }else{ // 8 gray.
	       (*imd)(0, k, l)=tif->red[byte];
	       (*imd)(1, k, l)=tif->green[byte];
	       (*imd)(2, k, l)=tif->blue[byte];
	       l++;
	       if (l == tif->ImageWidth) {l=0;k++;}
	    }
	 }
      }
   } else {

      /*
       * Full RGB colors.
       */
      if (tif->photometricinterpretation==2) {
	 if (tif->SamplesPerPixel==3) { // Full RGB colors.
	    for (int i=0;i<(int)tif->StripsCount;i++) {
	       fseek(tif->fp, stripOffset[i], 0);

	       for (int j=0;j<stripByteCount[i];j+=3) {
		  (*imd)(0, k, l)=readPixel(tif);
		  (*imd)(1, k, l)=readPixel(tif);
		  (*imd)(2, k, l)=readPixel(tif);
		  l++;
		  if (l == tif->ImageWidth) {l=0;k++;}
	       }
	    }
	 } else {
	    fprintf(stderr, "Error ptiff2pan: Don't know how to convert such TIFF file\n");
	 }
      }
   }
   delete[] stripOffset;
   delete[] stripByteCount;
   return imd;
}

/*
 * Creates a grayscale image (4 or 8 bits).
 */
Pobject* CreateImg2duc( TIFF *tif ) {
   Img2duc* imd = new Img2duc(tif->ImageHeight, tif->ImageWidth);
   Long* stripOffset = new Long[tif->StripsCount+1];
   Long* stripByteCount = new Long[tif->StripsCount+1];

   // Get offset of each strips.
   fseek(tif->fp, tif->stripoffsets, 0);
   
   if (tif->StripsCount!=1) {
      for ( int w=0; w<(int)tif->StripsCount; w++ ) {
	 readBytes(tif->fp, &stripOffset[w]);
      }
   } else {
      stripOffset[0] = tif->stripoffsets;
   }
   
   // Get the number of bytes in a strip.
   fseek(tif->fp, tif->stripbytecounts, 0);
   if (tif->StripsCount!=1) {
      for (int u=0; u<(int)tif->StripsCount; u++) {
	 readBytes(tif->fp, &stripByteCount[u]);
      }
   } else {
      stripByteCount[0] = tif->stripbytecounts;
   }
   
   unsigned char byte;
   Point2d p(0, 0);

//    printf("strip count     = %d\n", tif->StripsCount);
//    printf("rowsperstrip    = %d\n", tif->RowsPerStrip);
//    printf("bitspersample   = %d\n", tif->bitspersample);
//    printf("sampleperpixel  = %d\n", tif->SamplesPerPixel);
//    printf("StripOffsets    = %d\n", tif->stripoffsets);
//    printf("stripbytecounts = %d\n", tif->stripbytecounts);

   for (int i = 0; i < (int)tif->StripsCount; i++) {
      fseek(tif->fp, stripOffset[i], 0);
      for (int j = 0; j < stripByteCount[i]; j ++) {
	 byte=readPixel(tif);
         // Case of alpha channel .. discard it
	 for (int k = 1; k < tif->SamplesPerPixel; k++) {
	    readPixel(tif); // Skip the extra components I don't know what do with them
	 }
	 if (tif->bitspersample == 1) {  //bilevel.
 	    for (int b = 0; b < 8; b++) {
	       Uchar value = (byte & 0x80)/0x80; 
	       (*imd)[p] = (tif->photometricinterpretation==0)? (1-value)*255 : value*255;
 	       byte<<=1;
 	       p.x++;
 	       if (p.x == tif->ImageWidth) { p.x=0; p.y++; break; }
	    }
	 } else {
	    if (tif->bitspersample==4) { // 4 gray.
	       for (int b = 0; b < 2; b++) {
		  (*imd)[p]=(byte & 0xF0);
		  byte<<=4;
		  p.x++;
		  if (p.x == tif->ImageWidth) { p.x=0; p.y++; break; }
	       }
	    } else { // 8 gray.
	       (*imd)[p] = byte;
	       p.x++;
	       if (p.x == tif->ImageWidth) { p.x=0; p.y++; }
	    }
	 }
      }
   }

   delete[] stripOffset;
   delete[] stripByteCount;
   return imd;
}

/*
 * Creates a grayscale image (16 bits).
 */
Pobject* CreateImg2dsl( TIFF *tif ) {
   Long* stripOffset= new Long[tif->StripsCount+1];
   Long* stripByteCount= new Long[tif->StripsCount+1];
   Img2dsl* imd=new Img2dsl(tif->ImageHeight, tif->ImageWidth);

   // Get offset of each strips.
   fseek(tif->fp, tif->stripoffsets, 0);

   if (tif->StripsCount!=1) {
      for (int w=0;w<(int)tif->StripsCount;w++) {
	 readBytes(tif->fp, &stripOffset[w]);
      }
   } else {
      stripOffset[0] = tif->stripoffsets;
   }
   
   // Get the number of bytes in a strip.
   fseek(tif->fp, tif->stripbytecounts, 0);
   if (tif->StripsCount!=1) {
      for (int u=0;u<(int)tif->StripsCount;u++) {
	 readBytes(tif->fp, &stripByteCount[u]);
      }
   } else {
      stripByteCount[0] = tif->stripbytecounts;
   }
   
   unsigned char byte;
   int k = 0;
   int l = 0;
   
   for (int i=0;i<(int)tif->StripsCount;i++) {
      fseek(tif->fp, stripOffset[i], 0);
      
      for (int j=0;j<stripByteCount[i];) {// Read 2 bits for 1 pixel
	 if (tif->bitspersample==16) { // 16 bits
	    byte=readPixel(tif); j++;
	    (*imd)(k, l)|=(Long)byte;
	    byte=readPixel(tif); j++;
	    (*imd)(k, l)=(Long)((unsigned short)byte)<<8;
	    l++;
	    if (l == tif->ImageWidth) {l=0;k++;}
	 }
      }
   }
   delete[] stripOffset;
   delete[] stripByteCount;
   return imd;
}

/*
 * Creates a Pandore form a tiff file.
 * The type of the Pandore (img2duc, Imc2uc)
 * is determine from the tiff properties.
 */
Errc PTiff2Pan( FILE *fp, Pobject **objout ) {
   TIFF *tif;

   if ((tif = TIFFOpen(fp)) == 0) {
      return FAILURE;
   }

   if (!TIFFGetIDF(tif)) {
      delete tif;
      return FAILURE;
   }
   
   if ((tif->photometricinterpretation==1) || (tif->photometricinterpretation==0)) {     // Gray images
      if (tif->bitspersample <= 8) {
	 *objout=CreateImg2duc(tif);
      } else if (tif->bitspersample <= 16) {
	 *objout=CreateImg2dsl(tif);
      } else {
	 fprintf(stderr, "Error ptiff2pan: Cannot convert that type of TIFF image\n");  
      }
   } else {										// Color images.
      if ((tif->photometricinterpretation==3) || (tif->photometricinterpretation==2)) {
	 *objout=CreateImc2duc(tif);
      } else {
	 fprintf(stderr, "Error ptiff2pan: Cannot convert that type of TIFF image\n");
      }
   }
   delete tif;
   return SUCCESS;
}

#ifdef MAIN

/*
 * Modify only the following constants, and the function call.
 */
#define	USAGE	"usage: %s tiff_in [im_out|-]"
#define	PARC	1
#define	FINC	0
#define	FOUTC	1
#define	MASK	0

int main( int argc, char *argv[] ) {
   Errc result;		     // The result code of the execution.
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
      fprintf(stdout, "%s 0 1 1\n", argv[0]);
      exit(0);
   }

   if ((argc == 2) && (!strcmp(argv[1], "-s"))) {
      fprintf(stdout, "\n");
      exit(0);
   }

   ReadArgs(argc, argv, PARC, FINC, FOUTC, &mask, objin, objs, objout, objd, parv, USAGE, MASK);

   // look for input filename.
   FILE* fdin = stdin;
   char* nomfichier = (argc > 1)? argv[1] : NULL;
   if (nomfichier && (strcmp(nomfichier, "-"))) {
      if ((fdin = fopen(nomfichier, "rb")) == NULL) {
	 fprintf(stderr, "Error ptiff2pan: Cannot open file %s\n", nomfichier);
	 Exit(FAILURE);
      }
   }      

   if ((result = PTiff2Pan(fdin, &objd[0])) == SUCCESS) {
      WriteArgs(argc, argv, PARC, FINC, FOUTC, &mask, objin, objs, objout, objd, MASK);
   } else {
      fprintf(stderr, "Error ptiff2pan: Bad TIFF file: %s.\n", nomfichier);
   }

   fclose(fdin);

   Exit(result);
   return 0;
}

#endif
