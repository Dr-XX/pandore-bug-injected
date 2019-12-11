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
 * @author Regis Clouard - 2001-04-23
 */

#if (defined _MSC_VER) && (!defined snprintf)
#define snprintf _snprintf
#endif

#include <string.h>
#include <pandore.h>
using namespace pandore;

/**
 * @file pgif2pan.cpp
 * Convert GIF file to Pandore file.
 * Mainly inspired from XV: John Bradley  (bradley@cis.upenn.edu)
 */
#define NEXTBYTE (*dataptr++)
#define EXTENSION     0x21
#define IMAGESEP      0x2c
#define TRAILER       0x3b
#define INTERLACEMASK 0x40
#define COLORMAPMASK  0x80

int temp, 			/* Unused variable */
   BitOffset = 0, 		/* Bit Offset  */
   XC = 0, YC = 0, 		/* Output X and Y coords of current pixel */
   Pass = 0, 			/* Used by output routine if interlaced pic */
   OutCount = 0, 		/* Decompressor output 'stack count' */
   Sncol, Snrow, 		/* screen dimensions */
   ncol, nrow, 			/* image dimensions */
   LeftOfs, TopOfs, 		/* image offset */
   BitsPerPixel, 		/* Bits per pixel, read from GIF header */
   BytesPerScanline, 		/* bytes per scanline in output raster */
   ColorMapSize, 		/* number of colors */
   Background, 			/* background color */
   CodeSize, 			/* Code size, read from GIF header */
   InitCodeSize, 		/* Starting code size, used during Clear */
   ReturnCode, 			/* Value returned by ReadCode */
   MaxCode, 			/* limiting value for current code size */
   ClearCode, 			/* GIF clear code */
   EOFCode, 			/* GIF end-of-information code */
   CurCode, OldCode, InCode, 	/* Decompressor variables */
   FirstFree, 			/* First free code, generated per GIF spec */
   FreeCode, 			/* Decompressor, next free slot in hash table */
   FinChar, 			/* Decompressor variable */
   BitMask, 			/* AND mask for data size */
   ReadMask, 			/* Code AND mask for current code size */
   Misc;                        /* miscellaneous bits (interlace, local cmap)*/

float normaspect;
bool Interlace, HasColormap;

Uchar *RawGIF;			/* The heap array to hold it, raw */
Uchar *Raster;			/* The raster data stream, unblocked */
Uchar *pic8;

/* The hash table used by the decompressor */
int Prefix[4096];
int Suffix[4096];

/* An output array used by the decompressor */
int OutCode[4097];

int   gif89 = 0;
#if defined _WIN32 || defined WIN32
char *id87 = _strdup("GIF87a");
char *id89 = _strdup("GIF89a");
#else
char *id87 = strdup("GIF87a");
char *id89 = strdup("GIF89a");
#endif
int   filesize;

Uchar *dataptr;

/* Fetch the next code from the data.  The codes can be
 * any length from 3 to 12 bits, packed into 8-bit bytes, so we have to
 * maintain our location in the Raster array as a BIT Offset.  We compute
 * the byte Offset into the raster array by dividing this by 8, pick up
 * three bytes, compute the bit Offset into our 24-bit chunk, shift to
 * bring the desired code to the bottom, then mask it off and return it. 
 */
static int readCode() {
   int RawCode, ByteOffset;
   
   ByteOffset = BitOffset / 8;
   RawCode = Raster[ByteOffset] + (Raster[ByteOffset + 1] << 8);
   if (CodeSize >= 8) {
      RawCode += ( ((int) Raster[ByteOffset + 2]) << 16);
   }
   RawCode >>= (BitOffset % 8);
   BitOffset += CodeSize;
   
   return RawCode & ReadMask;
}

static void doInterlace(int Index) {
   static Uchar *ptr = NULL;
   static int oldYC = -1;
   
   if (oldYC != YC) {
      ptr = pic8 + YC * ncol;
      oldYC = YC;
   }
   
   if (YC < nrow) {
      *ptr++ = Index;
   }
   // Update the X-coordinate, and if it overflows, update the Y-coordinate.
   if (++XC == ncol) {
      // deal with the interlace as described in the GIF
      // spec.  Put the decoded scan line out to the screen if we haven't gone
      // past the bottom of it
      
      XC = 0;
      
      switch (Pass) {
      case 0:
	 YC += 8;
	 if (YC >= nrow) {
	    Pass++;
	    YC = 4;
	 }
	 break;
	 
      case 1:
	 YC += 8;
	 if (YC >= nrow) {
	    Pass++;
	    YC = 2;
	 }
	 break;
	 
      case 2:
	 YC += 4;
	 if (YC >= nrow) {
	    Pass++;
	    YC = 1;
	 }
	 break;
	 
      case 3:
	 YC += 2;
	 break;
	 
      default:
	 break;
      }
   }
}

/*
 * Read data (with lut).
 */
static Errc ReadData( Uchar lut[][3] ) {
   register Uchar ch, ch1, *ptr1, *picptr;
   int i, npixels, maxpixels;
   
   npixels = maxpixels = 0;
   
   // Read in values from the image descriptor.
   
   ch = NEXTBYTE;
   LeftOfs = ch + 0x100 * NEXTBYTE;
   ch = NEXTBYTE;
   TopOfs  = ch + 0x100 * NEXTBYTE;
   ch = NEXTBYTE;
   ncol   = ch + 0x100 * NEXTBYTE;
   ch = NEXTBYTE;
   nrow  = ch + 0x100 * NEXTBYTE;
   
   Misc = NEXTBYTE;
   Interlace = ((Misc & INTERLACEMASK) ? 1 : 0);
   
   if (Misc & 0x80) {
      for (i=0; i< 1 << ((Misc&7)+1); i++) {
	 lut[i][0] = NEXTBYTE;
	 lut[i][1] = NEXTBYTE;
	 lut[i][2] = NEXTBYTE;
      }
   }
   
   // Start reading the raster data. First we get the intial code size
   // and compute decompressor constant values, based on this code size.
   CodeSize = NEXTBYTE;
   
   ClearCode = (1 << CodeSize);
   EOFCode = ClearCode + 1;
   FreeCode = FirstFree = ClearCode + 2;
  
   // The GIF spec has it that the code size is the code size used to
   // compute the above values is the code size given in the file, but the
   // code size used in compression/decompression is the code size given in
   //the file plus one. (thus the ++).
   CodeSize++;
   InitCodeSize = CodeSize;
   MaxCode = (1 << CodeSize);
   ReadMask = MaxCode - 1;

   // UNBLOCK:
   // Read the raster data.  Here we just transpose it from the GIF array
   // to the Raster array, turning it from a series of blocks into one long
   // data stream, which makes life much easier for readCode().
   ptr1 = Raster;
   do {
      ch = ch1 = NEXTBYTE;
      while (ch--) {
	 *ptr1 = NEXTBYTE;
	 ptr1++;
      }
      if ((dataptr - RawGIF) > filesize) {
	 break;
      }
   } while(ch1);


   // Allocate the 'pic'.
   maxpixels = ncol*nrow;
   picptr = pic8 = (Uchar *) malloc((size_t) maxpixels);
   if (!pic8) {
      return FAILURE;
   }
  
   // Decompress the file, continuing until you see the GIF EOF code.
   // One obvious enhancement is to add checking for corrupt files here.
   ReturnCode = readCode();
   while (ReturnCode != EOFCode) {
      // Clear code sets everything back to its initial value, then reads the
      // immediately subsequent code as uncompressed data.
      if (ReturnCode == ClearCode) {
	 CodeSize = InitCodeSize;
	 MaxCode = (1 << CodeSize);
	 ReadMask = MaxCode - 1;
	 FreeCode = FirstFree;
	 ReturnCode = readCode();
	 CurCode = OldCode = ReturnCode;
	 FinChar = CurCode & BitMask;
	 if (!Interlace) {
	    *picptr++ = FinChar;
	 } else {
	    doInterlace(FinChar);
	 }
	 npixels++;
      } else {
	 // If not a clear code, must be data: save same as CurCode and InCode
	 
	 // if we're at maxcode and didn't get a clear, stop loading
	 if (FreeCode >= 4096) {
	    break;
	 }
	 
	 CurCode = InCode = ReturnCode;
	 
	 // If greater or equal to FreeCode, not in the hash table yet;
	 // repeat the last character decoded
	 if (CurCode >= FreeCode) {
	    CurCode = OldCode;
	    if (OutCount > 4096) {
	       break;
	    }
	    OutCode[OutCount++] = FinChar;
	 }
	 
	 // Unless this code is raw data, pursue the chain pointed to by CurCode
	 // through the hash table to its end; each code in the chain puts its
	 // associated output code on the output queue.
      
	 while (CurCode > BitMask) {
	    if (OutCount > 4096) {
	       break;   // corrupt file
	    }
	    OutCode[OutCount++] = Suffix[CurCode];
	    CurCode = Prefix[CurCode];
	 }
      
	 if (OutCount > 4096) {
	    break;
	 }
      
	 // The last code in the chain is treated as raw data.
      
	 FinChar = CurCode & BitMask;
	 OutCode[OutCount++] = FinChar;
      
	 // Now we put the data out to the Output routine.
	 // It's been stacked LIFO, so deal with it that way...

	 // safety thing:  prevent exceeding range of 'pic8'.
	 if (npixels + OutCount > maxpixels) {
	    OutCount = maxpixels-npixels;
	 }
	
	 npixels += OutCount;
	 if (!Interlace) {
	    for (i = OutCount - 1; i >= 0; i--) {
	       *picptr++ = OutCode[i];
	    }
	 } else {
	    for (i = OutCount - 1; i >= 0; i--) {
	       doInterlace(OutCode[i]);
	    }
	 }
	 OutCount = 0;

	 // Build the hash table on-the-fly. No table is stored in the file.
      
	 Prefix[FreeCode] = OldCode;
	 Suffix[FreeCode] = FinChar;
	 OldCode = InCode;
      
	 // Point to the next slot in the table.  If we exceed the current
	 // MaxCode value, increment the code size unless it's already 12.  If it
	 // is, do nothing: the next code decompressed better be CLEAR
	 FreeCode++;
	 if (FreeCode >= MaxCode) {
	    if (CodeSize < 12) {
	       CodeSize++;
	       MaxCode *= 2;
	       ReadMask = (1 << CodeSize) - 1;
	    }
	 }
      }
      ReturnCode = readCode();
      if (npixels >= maxpixels) {
	 break;
      }
   }
   if (npixels != maxpixels) {
      if (!Interlace) {
	 memset((char *) pic8+npixels, 0, (size_t) (maxpixels-npixels));
      }
   }

   return SUCCESS;
}

bool isGIF( FILE *fp ) {
   int a, b, c;
   a = fgetc(fp);
   b = fgetc(fp);
   c = fgetc(fp);
   return (a == 'G' && b == 'I' && c == 'F');
}

Uchar* LoadGIF( FILE *fp, Uchar lut[256][3] ) {
   register Uchar  ch, *origptr;
   register int   i, block;
   int aspect, gotimage;

   // initialize variables
   BitOffset = XC = YC = Pass = OutCount = gotimage = 0;
   RawGIF = Raster = pic8 = NULL;
   gif89 = 0;

   // find the size of the file
   fseek(fp, 0L, 2);
   filesize = ftell(fp);
   fseek(fp, 0L, 0);
  
   // the +256's are so we can read truncated GIF files
   // without fear of segmentation violation.
   if (!(dataptr = RawGIF = (Uchar *) calloc((size_t) filesize+256, (size_t) 1))) {
      return NULL;
   }
   if (!(Raster = (Uchar *) calloc((size_t) filesize+256, (size_t) 1))) {
      return NULL;
   }
   if (fread(dataptr, (size_t) filesize, (size_t) 1, fp) != 1) {
      return NULL;
   }
   origptr = dataptr;

   if (strncmp((char *) dataptr, id87, (size_t) 6) == 0) {
      gif89 = 0;
   } else if (strncmp((char *) dataptr, id89, (size_t) 6) == 0) {
      gif89 = 1;
   } else {
      return NULL;
   }
  
   dataptr += 6;
  
   // Get variables from the GIF screen descriptor.
  
   ch = NEXTBYTE;
   Sncol = ch + 0x100 * NEXTBYTE;	// screen dimensions... not used.
   ch = NEXTBYTE;
   Snrow = ch + 0x100 * NEXTBYTE;
  
   ch = NEXTBYTE;
   HasColormap = ((ch & COLORMAPMASK) ? 1 : 0);
  
   BitsPerPixel = (ch & 7) + 1;
   ColorMapSize = 1 << BitsPerPixel;
   BitMask = ColorMapSize - 1;
  
   Background = NEXTBYTE;		// background color... not used.
  
   aspect = NEXTBYTE;
   if (aspect) {
      if (!gif89) {
	 return NULL;
      } else {
	 normaspect = (float) (aspect + 15) / 64.0F;   // gif89 aspect ratio
      }
   }

   // Read in global colormap.
   if (HasColormap) {
      for (i = 0; i < ColorMapSize; i++) {
	 lut[i][0] = NEXTBYTE;
	 lut[i][1] = NEXTBYTE;
	 lut[i][2] = NEXTBYTE;
      }
   }

   // possible things at this point are:
   //   an application extension block
   //   a comment extension block
   //   an (optional) graphic control extension block
   //       followed by either an image
   //	   or a plaintext extension
  
   while (1) {
      block = NEXTBYTE;

      if (block == EXTENSION) {  // parse extension blocks.
	 int i, fn, blocksize, aspnum, aspden;

	 // read extension block
	 fn = NEXTBYTE;

	 if (fn == 'R') {                  // GIF87 aspect extension.
	    int sbsize;

	    blocksize = NEXTBYTE;
	    if (blocksize == 2) {
	       aspnum = NEXTBYTE;
	       aspden = NEXTBYTE;
	       if (aspden>0 && aspnum > 0) {
		  normaspect = (float) aspnum / (float) aspden;
	       } else {
		  normaspect = 1.0;
		  aspnum = aspden = 1;
	       }
	    } else {
	       for (i = 0; i < blocksize; i++) {
		  temp = NEXTBYTE;
	       }
	    }

	    while ((sbsize = NEXTBYTE) > 0) {  // eat any following data subblocks
	       for (i = 0; i < sbsize; i++) {
		  temp = NEXTBYTE;
	       }
	    }
	 } else if (fn == 0xFE) {  // Comment Extension
	    int   j, sbsize, cmtlen;
	    Uchar *ptr1, *cmt, *sp;

	    cmtlen = 0;
	    ptr1 = dataptr;      // remember start of comments

	    // figure out length of comment
	    do {
	       sbsize = NEXTBYTE;
	       cmtlen += sbsize;
	       for (j = 0; j < sbsize; j++) { 
		  (void)NEXTBYTE;
	       }
	    } while (sbsize);

	    if (cmtlen > 0) {   // build into one un-blocked comment
	       cmt = (Uchar *) malloc((size_t) (cmtlen + 1));
	       if (cmt) {
		  sp = cmt;
		  do {
		     sbsize = (*ptr1++);
		     for (j = 0; j < sbsize; j++, sp++, ptr1++) {
			*sp = *ptr1;
		     }
		  } while (sbsize);
		  *sp = '\0';
		  
	       }
	    }
	 } else if (fn == 0x01) {  // PlainText Extension
	    int j, sbsize ;
	    int tgLeft, tgTop, tgncol, tgnrow, cncol, cnrow, fg, bg;
	    
	    sbsize  = NEXTBYTE;
	    tgLeft  = NEXTBYTE;  tgLeft += (NEXTBYTE)<<8;
	    tgTop   = NEXTBYTE;  tgTop  += (NEXTBYTE)<<8;
	    tgncol  = NEXTBYTE;  tgncol += (NEXTBYTE)<<8;
	    tgnrow  = NEXTBYTE;  tgnrow += (NEXTBYTE)<<8;
	    cncol   = NEXTBYTE;
	    cnrow   = NEXTBYTE;
	    fg      = NEXTBYTE;
	    bg      = NEXTBYTE;

	    for (i = 12 ; i < sbsize; i++) {  // read rest of first subblock
	       temp=NEXTBYTE;
	    }
	    { // Suppress warnings...
	       (void)cncol;
	       (void)cnrow;
	       (void)fg;
	       (void)bg;
	    }
	    // read (and ignore) data sub-blocks
	    do {
	       j = 0;
	       sbsize = NEXTBYTE;
	       while (j < sbsize) {
		  (void)NEXTBYTE;
		  j++;
	       }
	    } while (sbsize);
	 } else if (fn == 0xF9) {  // Graphic Control Extension
	    int j, sbsize;
	    
	    // read (and ignore) data sub-blocks.
	    do {
	       j = 0; sbsize = NEXTBYTE;
	       while (j < sbsize) {
		  temp = NEXTBYTE; 
		  j++;
	       }
	    } while (sbsize);
	 } else if (fn == 0xFF) {  // Application Extension
	    int j, sbsize;

	    // read (and ignore) data sub-blocks
	    do {
	       j = 0;
	       sbsize = NEXTBYTE;
	       while (j  <sbsize) {
		  temp = NEXTBYTE; 
		  j++;
	       }
	    } while (sbsize);
	 } else { // unknown extension
	    int j, sbsize;
	    
	    // read (and ignore) data sub-blocks
	    do {
	       j = 0;
	       sbsize = NEXTBYTE;
	       while (j < sbsize) {
		  temp = NEXTBYTE;
		  j++;
	       }
	    } while (sbsize);
	 }
      } else if (block == IMAGESEP) {
	 if (gotimage) {   // just skip over remaining images
	    int i, misc, ch, ch1;
	    
	    // skip image header
	    temp=NEXTBYTE;  temp=NEXTBYTE;  // left position
	    temp=NEXTBYTE;  temp=NEXTBYTE;  // top position
	    temp=NEXTBYTE;  temp=NEXTBYTE;  // ncol
	    temp=NEXTBYTE;  temp=NEXTBYTE;  // nrow
	    misc = NEXTBYTE;      // misc. bits
	    
	    if (misc & 0x80) {    // image has local colormap.  skip it
	       for (i  =0; i< (1 << ((misc&7)+1));  i++) {
		  temp = NEXTBYTE;
		  temp = NEXTBYTE;
		  temp = NEXTBYTE;
	       }
	    }

	    temp = NEXTBYTE;       // minimum code size

	    // skip image data sub-blocks
	    do {
	       ch = ch1 = NEXTBYTE;
	       while (ch--) {
		  temp = NEXTBYTE;
	       }
	       if ((dataptr - RawGIF) > filesize) {
		  break;      // EOF
	       }
	    } while(ch1);
	 } else if (ReadData(lut)) {
	    gotimage = 1;
	 }
      } else if (block == TRAILER) {      // stop reading blocks.
	 break;
      } else {      // unknown block type
	 char str[128];
	 // don't mention bad block if file was trunc'd, as it's all bogus.
	 if ((dataptr - origptr) < filesize) {
	    snprintf(str, sizeof(str), "Unknown block type (0x%02x) at offset 0x%x", block, (unsigned int)(dataptr - origptr) - 1);
	    str[sizeof(str) - 1] = 0;
	    if (!gotimage) {
	       return NULL;
	    }
	 }
	 break;
      }
   }

   free(RawGIF);
   RawGIF = NULL;
   free(Raster);
   Raster = NULL;

   if (!gotimage) { 
      return NULL;
   }
   return pic8;
}

/*
 * Parse gray data fron the pic8 vector.
 */
Errc FiltreDonnees( Img2duc &ims, Uchar *pic8 ) {
   for (int i = 0; i < ims.Height(); i++) {
      for (int j = 0; j < ims.Width(); j++) {
	 ims(i, j) = *(pic8++);
      }
   }
   return SUCCESS;
}

/*
 * Parse color data form the pic8 and lut vectors.
 */
Errc FiltreDonnees( Imc2duc &ims, Uchar *pic8, Uchar lut[256][3] ) {
   for (int i = 0; i < ims.Height(); i++) {
      for (int j = 0; j < ims.Width(); j++, pic8++) {
	 ims(0, i, j) = lut[*pic8][0]; 
	 ims(1, i, j) = lut[*pic8][1];
	 ims(2, i, j) = lut[*pic8][2];
      }
   }
   return SUCCESS;
}

/*
 * Select the output object format (gray or color image)
 * from the existence or not of a lut inside the
 * Gif file.
 */
Errc PGif2Pan( FILE *fp, Pobject **objout ) {
   Uchar lut[256][3];
   
   if ((pic8 = LoadGIF(fp, lut)) == NULL) {
      return FAILURE;
   }
   
   if (!HasColormap) { 
      *objout = new Img2duc(nrow, ncol);
      FiltreDonnees((*(Img2duc*)*objout), pic8);
   } else {
      *objout = new Imc2duc(nrow, ncol);
      FiltreDonnees((*(Imc2duc*)*objout), pic8, lut);
   }
   return SUCCESS;
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
   Errc	 result;               // The result code of the execution.
   Pobject* mask;              // The mask of input objects.
   Pobject* objin[FINC + 1];   // The input objects.
   Pobject* objs[FINC + 1];    // The source objects masked.
   Pobject* objout[FOUTC + 1]; // The output object.
   Pobject* objd[FOUTC + 1];   // The result object of the execution.
   char* parv[PARC + 1];	     // The input parameters.
   
   if ((argc == 2) && (!strcmp(argv[1], "-p"))) {
      fprintf(stdout, "%s 0 1 1\n", argv[0]);
      exit(0);
   }
   if ((argc == 2) && (!strcmp(argv[1], "-s"))) {
      fprintf(stdout, "\n");
      exit(0);
   }
   
   ReadArgs(argc, argv, PARC, FINC, FOUTC, &mask, objin, objs, objout, objd, parv, USAGE, MASK);
   
   // Check inputs.
   char* nomfichier = (argc>1)? argv[1] : NULL;
   FILE*  fdin;
   if ((nomfichier != NULL) && (strcmp(nomfichier, "-"))) {
      if ((fdin = fopen(nomfichier, "rb")) == NULL) {
	 fprintf(stderr, "Error pgif2pan: Cannot open file %s\n", nomfichier);
	 Exit(FAILURE);
      }
   } else {
      fdin = stdin;
   }
   
   if ((result = PGif2Pan(fdin, &objd[0])) == SUCCESS) {
      WriteArgs(argc, argv, PARC, FINC, FOUTC, &mask, objin, objs, objout, objd, MASK);
   } else {
      fprintf(stderr, "Error pgif2pan: Bad GIF file: %s.\n", nomfichier);
   }
   
   if (fdin != stdin) {
      fclose(fdin);
   }
   Exit(result);
   return 0;
}

#endif
