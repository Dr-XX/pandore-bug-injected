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

#if (defined _MSC_VER) && (!defined snprintf)
#define snprintf _snprintf
#endif

#include <string.h>
#include <pandore.h>
using namespace pandore;

/**
 * @file ppan2tiff.cpp
 * Convert Pandore images to TIFF images.
 * 3D Pandore images generates ndep TIFF files.
 */

typedef struct _TiffHeader {
   short identifier;	// Byte order identifier
   short version;	// TIFF version number
   Long ifdoffset;	// Offset of the first Image File Directory
} TIFHEAD;


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
#define STRIPOFFSETS 273
#define SAMPLESPERPIXEL 277
#define ROWSPERSTRIP 278
#define STRIPBYTECOUNTS 279
#define XRESOLUTION 282
#define YRESOLUTION 283
#define PLANARCONFIGURATION 284
#define RESOLUTIONUNIT 296
#define SOFTWARE 305

typedef struct _TifTag {
   short tagid;		// Tag identifier
   short datatype;	// The scalar type of the data items 1 byte, 2 ascii, 3 short, 4 long...
   Long datacount;	// The number of items in the tag data
   Long dataoffset;	// The byte offset of the data items
} TIFTAG;

typedef struct _TagPos {
   short tagid;
   Long filepos;
}TAGPOS;

#define MAXTAG 30

typedef struct _Tiff {
   FILE* fp;
   short numdirentries;
   Long  numdirentriespointer; // Used to mark the file-position of NumDirEntries tag.
   TAGPOS tagpos[MAXTAG];
   Long nextifdoffset;
} TIFF;

/*
 * Determines the byte order, 
 * and sets the endian global variable.
 */
#define _BIG_ENDIAN_      0 // most to least significant byte order
#define _LITTLE_ENDIAN_   1 // least to most significant byte order
int determineByteOrder(void) {
   short word = 0x0001;
   char *byte = (char *) &word;

   return (byte[0] ? _LITTLE_ENDIAN_ : _BIG_ENDIAN_);
}


/*
 * Writes a short in file, 
 * but reverses high and low bytes, 
 * in case of _BIG_ENDIAN_
 */
void writeBytes( FILE *fp, short s ) {
   static int endian = determineByteOrder();
   union {
      short s;
      char c[2];
   } u, v;
   
   u.s = s;
   v.c[0] = u.c[1];
   v.c[1] = u.c[0];
   
   if (endian == _BIG_ENDIAN_) {
      fwrite(&v.s, sizeof(short), 1, fp);
   } else {
      fwrite(&s, sizeof(short), 1, fp);
   }
}

/*
 * Writes a long in file, 
 * but reverses high and low bytes, 
 * in case of _BIG_ENDIAN_
 */
void writeBytes( FILE* fp, Long l ) {
   static int endian = determineByteOrder();
   union {
      Long l;
      char c[4];
   } u, v;
  
   u.l = l;
   v.c[0] = u.c[3];
   v.c[1] = u.c[2];
   v.c[2] = u.c[1];
   v.c[3] = u.c[0];

   if (endian == _BIG_ENDIAN_) {
      fwrite(&v.l, sizeof(Long), 1, fp);
   } else {
      fwrite(&l, sizeof(Long), 1, fp);
   }
}

/*
 * Rewrites the DataOffset value of a tag
 * when knowing the right Offset.
 */
void TIFFResetTagdataoffsetValue( TIFF *tif, short tagid ) {
   int i;
   Long currentpos;
   Long filepos;

   filepos = currentpos = ftell(tif->fp);

   for (i = 0; i< tif->numdirentries; i++) {
      if (tif->tagpos[i].tagid == tagid) {
	 fseek(tif->fp, tif->tagpos[i].filepos + sizeof(TIFTAG)-sizeof(Long), SEEK_SET);
	 writeBytes(tif->fp, filepos);
      }
      fseek(tif->fp, currentpos, SEEK_SET);    
   }
}

/*
 * Writes a TAG in the file.
 * Rewrites the number of DirEntries in the file.
 */
void TIFFSetTag( TIFF *tif, short tagid, short datatype, Long datacount, Long dataoffset ) {
   TIFTAG tiftag;

   // Mark the file-position of NumDirEntries tag., 
   // in order to rewrite when knowing the exact number.
   if (tif->numdirentries == 0) {
      short nbtags;
      tif->numdirentriespointer = ftell(tif->fp);
      writeBytes(tif->fp, nbtags = 0);
   }

   // Mark the file-position of the tag.
   tif->tagpos[tif->numdirentries].tagid = tagid;
   tif->tagpos[tif->numdirentries].filepos = ftell(tif->fp);

   tif->numdirentries++;
  
   // Add the tag to file.
   writeBytes(tif->fp, tiftag.tagid = tagid);
   writeBytes(tif->fp, tiftag.datatype = datatype);
   writeBytes(tif->fp, tiftag.datacount = datacount);
   writeBytes(tif->fp, tiftag.dataoffset = dataoffset);
}

/*
 * Creates a new TIFF image.
 * Inserts the general header.
 */
TIFF* TIFFOpen( char *filename ) {
   TIFF *tif;
   TIFHEAD tifhead;
   union{ short s; char c[2]; }identifier;

   tif = new TIFF;
   tif->numdirentries = 0;
   tif->numdirentriespointer = 0; // Used to mark the file-position of NumDirEntries tag

   if ((tif->fp = fopen(filename, "wb+")) == NULL) {
      fprintf(stderr, "Error ppan2tiff: Cannot create %s file.\n", filename);
      return NULL;
   }

   // Write the "magic_number"
   identifier.c[0] = identifier.c[1] = 'I';
   tifhead.identifier = identifier.s;
   tifhead.version = 42;
   tifhead.ifdoffset = sizeof(TIFHEAD);
   
   writeBytes(tif->fp, tifhead.identifier);
   writeBytes(tif->fp, tifhead.version);
   writeBytes(tif->fp, tifhead.ifdoffset);
  
   return tif;
}

/*
 * Closes the specified tiff file.
 */
void TIFFClose( TIFF* tif ) {  
   fclose(tif->fp);
   delete tif;
}

/*
 * Configuration for Grayscale images: 8 tags.
 * WARNING: Tags must be in ascending order.
 */
void TIFFGrayscale( TIFF *tif, int nrow, int ncol, int nbytes ) {
   TIFFSetTag(tif, IMAGEWIDTH, LONG,  1, ncol);
   TIFFSetTag(tif, IMAGELENGTH, LONG,  1, nrow);
   TIFFSetTag(tif, BITSPERSAMPLE, SHORT,  1, nbytes * 8);
   TIFFSetTag(tif, COMPRESSION, SHORT,  1, 1);  // No compression.
   TIFFSetTag(tif, PHOTOMETRICINTERPRETATION, SHORT,  1, 1);  // Black is 0.
   TIFFSetTag(tif, STRIPOFFSETS, LONG,  1, 0);
   TIFFSetTag(tif, ROWSPERSTRIP, LONG,  1, nrow); // RowsPerStrip is 2**32 by default.
   TIFFSetTag(tif, STRIPBYTECOUNTS, LONG,  1, nrow * ncol * nbytes);
}

/*
 * Configuration for colored images: 6 tags.
 * WARNING: Tags must be in ascending order.
 */
void TIFFColor( TIFF *tif, int nrow, int ncol ) {
   TIFFSetTag(tif, IMAGEWIDTH, SHORT,  1, ncol);
   TIFFSetTag(tif, IMAGELENGTH, SHORT,  1, nrow);
   TIFFSetTag(tif, BITSPERSAMPLE, SHORT, 3, 1); // 8, 8, 8
   TIFFSetTag(tif, COMPRESSION, SHORT,  1, 1);  // No compression.
   TIFFSetTag(tif, PHOTOMETRICINTERPRETATION, SHORT,  1, 2);  // Black is 0.
   TIFFSetTag(tif, STRIPOFFSETS, LONG,  1, 0);
   TIFFSetTag(tif, SAMPLESPERPIXEL, SHORT, 1, 3); // RGB
   TIFFSetTag(tif, ROWSPERSTRIP, LONG,  1, nrow); // RowsPerStrip is 2**32 by default.
   TIFFSetTag(tif, STRIPBYTECOUNTS, LONG,  1, nrow*ncol*3);
   TIFFSetTag(tif, PLANARCONFIGURATION, SHORT,  1, 1);
   TIFFSetTag(tif, RESOLUTIONUNIT, SHORT,  1, 1);
}

/*
 * End the current ImageFileDirectory:
 * -Adds the comment.
 * -Adds the Offset of the next IFD ( = 0).
 * -Sets the right StripOffsets tag value
 * -Sets the right number of tags.
 */
void TIFFEndIFD( TIFF *tif ) {
   int dataoffsetvalue;
   char buf[256];
   Long next;
  
   // Close the IDF part. 
   next = 0;
   writeBytes(tif->fp, next);

   // Set the right number of Tags.
   dataoffsetvalue = ftell(tif->fp); // Determine the current size of the stream.
   short nbtags;
   fseek(tif->fp, tif->numdirentriespointer, SEEK_SET);
   nbtags = tif->numdirentries;
   writeBytes(tif->fp, nbtags);
   fseek(tif->fp, dataoffsetvalue, SEEK_SET);   // Return to the Data offset.

   // The software data.
#define COMMENT "Pandore@GREYC-IMAGE(fr)\n"
   TIFFSetTag(tif, SOFTWARE, ASCII, strlen(COMMENT), 0); // Add a comment (the name of the software).

   TIFFResetTagdataoffsetValue(tif, SOFTWARE);
   strncpy(buf, COMMENT, strlen(COMMENT));
   fwrite(buf, sizeof(char), strlen(COMMENT), tif->fp);
}

/*
 * Writes data at the specified offset.
 * First rewrites the StripOffsets value.
 */
void TIFFWriteStrips( TIFF *tif, const Img3duc &ims, int z ) {
   TIFFEndIFD(tif);
   TIFFResetTagdataoffsetValue(tif, STRIPOFFSETS);

   for (int y = 0; y < ims.Height(); y++) {
      fwrite(&ims(z, y, 0), sizeof(char), ims.Width(), tif->fp);
   }
} 

/*
 * Writes data at the specified offset.
 * First rewrites the StripOffsets value.
 */
void TIFFWriteStrips( TIFF *tif, const Img3dsl &ims, int z ) {
   int y, x;

   TIFFEndIFD(tif);
   TIFFResetTagdataoffsetValue(tif, STRIPOFFSETS);

   for (y = 0; y < ims.Height(); y++) {
      for (x = 0;  x< ims.Width(); x++) {
	 if (ims(z, y, x) < 0) {
	    writeBytes(tif->fp, (short)0);
	 } else if (ims(z, y, x) <= MAXUSHORT) {
	    writeBytes(tif->fp, (short)ims(z, y, x));
	 } else {
	    writeBytes(tif->fp, (short)MAXUSHORT);
	 }
      }
   }
}

/*
 * Writes data at the specified offset.
 */
void TIFFWriteStrips( TIFF *tif, const Img2duc &ims ) {
   TIFFEndIFD(tif);
   TIFFResetTagdataoffsetValue(tif, STRIPOFFSETS);
 
   for (int y = 0; y < ims.Height(); y++) {
      fwrite(&ims(y, 0), sizeof(char), ims.Width(), tif->fp);
   }
}

/*
 * Writes data at the specified offset.
 * Encodes gray levels with 2 bytes.
 */
void TIFFWriteStrips( TIFF *tif, const Img2dsl &ims ) {
   int x, y;
   
   TIFFEndIFD(tif); 
   TIFFResetTagdataoffsetValue(tif, STRIPOFFSETS);

   for (y = 0; y < ims.Height(); y++) {
      for (x = 0; x < ims.Width(); x++) {
	 if (ims(y, x)< 0 ) {
	    writeBytes(tif->fp, (short)0);
	 } else if (ims(y, x)>MAXUSHORT) {
	    writeBytes(tif->fp, (short)MAXUSHORT);
	 } else {
	    writeBytes(tif->fp, (short)ims(y, x));
	 }
      }
   }
}

/*
 * Writes data at the specified offset.
 */
void TIFFWriteStrips( TIFF *tif, const Reg2d &ims ) {
   int i, j;
   short x;
#define MAXCOLS 16
   char rmap[MAXCOLS], gmap[MAXCOLS], bmap[MAXCOLS];
   char zero = 0;

   for (i = 0; i < MAXCOLS; i++) {
      rmap[i] = rand() & 0x00ff;
      gmap[i] = rand() & 0x00ff;
      bmap[i] = rand() & 0x00ff;
      // Cas peu probable du noir total -> on retire
      // puisqu'il n'y a que le fond qui doit etre noir.
      if (rmap[i] == 0 && gmap[i] == 0 && bmap[i] == 0) {
	 rmap[i] = rand() & 0x00ff;
	 gmap[i] = rand() & 0x00ff;
	 bmap[i] = rand() & 0x00ff;
      }
   }
   
   TIFFEndIFD(tif);
   TIFFResetTagdataoffsetValue(tif, BITSPERSAMPLE);

   x = 8;
   writeBytes(tif->fp, x);
   writeBytes(tif->fp, x);
   writeBytes(tif->fp, x);
   
   TIFFResetTagdataoffsetValue(tif, STRIPOFFSETS);
   
   for (i = 0; i < ims.Height(); i++) {
      for (j = 0; j < ims.Width(); j++) {
	 if (ims[i][j] == 0) {
	    fwrite(&zero, sizeof(char), 1, tif->fp);
	    fwrite(&zero, sizeof(char), 1, tif->fp);
	    fwrite(&zero, sizeof(char), 1, tif->fp);
	 } else {
	    fwrite(&rmap[ims[i][j] % MAXCOLS], sizeof(char), 1, tif->fp);
	    fwrite(&gmap[ims[i][j] % MAXCOLS], sizeof(char), 1, tif->fp);
	    fwrite(&bmap[ims[i][j] % MAXCOLS], sizeof(char), 1, tif->fp);
	 }
      }
   }
} 

/*
 * Writes data at the specified offset.
 */
void TIFFWriteStrips( TIFF *tif, const Imc2duc &ims ) {
   int y, x;
   short bits;

   TIFFEndIFD(tif);
   TIFFResetTagdataoffsetValue(tif, BITSPERSAMPLE);

   bits = 8;
   writeBytes(tif->fp, bits);
   writeBytes(tif->fp, bits);
   writeBytes(tif->fp, bits);
  
   TIFFResetTagdataoffsetValue(tif, STRIPOFFSETS);
   for (y = 0; y<ims.Height(); y++) {
      for (x = 0; x<ims.Width(); x++) {
	 fwrite(&ims(0, y, x), sizeof(char), 1, tif->fp);
	 fwrite(&ims(1, y, x), sizeof(char), 1, tif->fp);
	 fwrite(&ims(2, y, x), sizeof(char), 1, tif->fp);
      }
   }
} 

/*
 * Generates a series of TIFF files named name##.tif 
 * from the planes of the 3D images.
 */
Errc PPan2Tiff( const Img3duc &ims, char *nom ) {
   int i, j;
   short nb_dieses;
   char format_nom[256], nom_tiff[256];
   TIFF* tif;

   // Construit le nom des fichiers TIFF (nom##.tif)
   for (i = 0; nom[i]&&(nom[i] != '.'); i++) ;
   j = i; // Fin du nom.
   nom[j] = '\0';
   nb_dieses = (ims.Depth()>1000)? 4 : 3;
   snprintf(format_nom, sizeof(format_nom), "%s%%.%dd.tif", nom, nb_dieses);
   format_nom[sizeof(format_nom) - 1] = 0;

   // Construit une image Tiff pour chaque plan.
   for (i = 0; i < ims.Depth(); i++) {
      snprintf(nom_tiff, sizeof(nom_tiff), format_nom, i);
      nom_tiff[sizeof(nom_tiff) - 1] = 0;

      if ((tif = TIFFOpen(nom_tiff))) {
	 TIFFGrayscale(tif, ims.Height(), ims.Width(), 1);
	 TIFFWriteStrips(tif, ims, i);
	 TIFFClose(tif);
      } else {
	 return FAILURE;
      }
   }
   return SUCCESS;
}

/*
 * Generates a series of TIFF files named name##.tif 
 * from the planes of the 3D images.
 */
Errc PPan2Tiff( const Img3dsl &ims, char *nom ) {
   int i, j;
   short nb_dieses;
   char format_nom[256], nom_tiff[256];
   TIFF* tif;

   
   // Construit le nom des fichiers TIFF (nom##.tif)
   for (i = 0 ; nom[i] && (nom[i] != '.') ; i++) ;
   j = i; // Fin du nom.
   nom[j] = '\0';
   nb_dieses = (ims.Depth()>1000) ? 4 : 3;
   snprintf(format_nom, sizeof(format_nom), "%s%%.%dd.tif", nom, nb_dieses);
   format_nom[sizeof(format_nom) - 1] = 0;

   // Construit une image Tiff pour chaque plan.
   for (i = 0; i < ims.Depth(); i++) {
      snprintf(nom_tiff, sizeof(nom_tiff), format_nom, i);
      nom_tiff[sizeof(nom_tiff) - 1] = 0;
      if ((tif = TIFFOpen(nom_tiff))) {
	 TIFFGrayscale(tif, ims.Height(), ims.Width(), 2);
	 TIFFWriteStrips(tif, ims, i);
	 TIFFClose(tif);
      } else {
	 return FAILURE;
      }
   }
   return SUCCESS;
}

/*
 * Generates a TIFF file from the Pandore 2D image.
 */
Errc PPan2Tiff( const Img2duc &ims, char *nom ) {
   TIFF* tif;
   if ((tif = TIFFOpen(nom))) {
      TIFFGrayscale(tif, ims.Height(), ims.Width(), 1);
      TIFFWriteStrips(tif, ims);
      TIFFClose(tif);
      return SUCCESS;
   } else {
      return FAILURE;
   }
}

Errc PPan2Tiff( const Img2dsl &ims, char *nom ) {
   TIFF* tif;

   if ((tif = TIFFOpen(nom))) {
      TIFFGrayscale(tif, ims.Height(), ims.Width(), 2);
      TIFFWriteStrips(tif, ims);
      TIFFClose(tif);
      return SUCCESS;
   } else {
      return FAILURE;
   }
}


/*
 * Reg2d : color with random colors.
 */
Errc PPan2Tiff( const Reg2d &rgs, char *nom ) {
   TIFF* tif;

   if ((tif = TIFFOpen(nom))) {
      TIFFColor(tif, rgs.Height(), rgs.Width());
      TIFFWriteStrips(tif, rgs);
      TIFFClose(tif);
      return SUCCESS;
   } else {
      return FAILURE;
   }
}

/*
 * 2D color.
 */
Errc PPan2Tiff( const Imc2duc &ims, char *nom ) {
   TIFF* tif;

   if ((tif = TIFFOpen(nom))) {
      TIFFColor(tif, ims.Height(), ims.Width());
      TIFFWriteStrips(tif, ims);
      TIFFClose(tif);
      return SUCCESS;
   } else {
      return FAILURE;
   }
}

#ifdef MAIN
	
/*
 * Modify only the following constants, and the function call.
 */
#define	USAGE	"usage: %s [im_in|-] [tiff_out]"
#define	PARC	0
#define	FINC	1
#define	FOUTC	1
#define	MASK	0

int main( int argc, char *argv[] ) {
   Errc result;                // The result code of the execution.
   Pobject* mask;              // The region mask.
   Pobject* objin[FINC + 1];   // The input objects.
   Pobject* objs[FINC + 1];    // The source objects masked by the mask.
   Pobject* objout[FOUTC  +1]; // The output objects.
   Pobject* objd[FOUTC + 1];   // The result objects of the execution.
   char* parv[PARC + 1];       // The input parameters.

   ReadArgs(argc, argv, PARC, FINC, FOUTC, &mask, objin, objs, objout, objd, parv, USAGE, MASK);

   if (argc < 3) {
      fprintf(stderr, USAGE"\n", argv[0]);
      Exit(FAILURE);
   }

   switch(objs[0]->Type()) {
      case Po_Img2duc:{
	 Img2duc* const ims = (Img2duc*)objs[0];

	 result = PPan2Tiff(*ims, argv[2]);
      }break;

      case Po_Img2dsl:{
	 Img2dsl* const ims = (Img2dsl*)objs[0];

	 result = PPan2Tiff(*ims, argv[2]);
      }break;

      case Po_Reg2d:{
	 Reg2d* const ims = (Reg2d*)objs[0];

	 result = PPan2Tiff(*ims, argv[2]);
      }break;


      case Po_Imc2duc:{
	 Imc2duc* const ims = (Imc2duc*)objs[0];

	 result = PPan2Tiff(*ims, argv[2]);
      }break;

      case Po_Img3duc:{
	 Img3duc* const ims = (Img3duc*)objs[0];

	 result = PPan2Tiff(*ims, argv[2]);
      }break;

      case Po_Img3dsl:{
	 Img3dsl* const ims = (Img3dsl*)objs[0];

	 result = PPan2Tiff(*ims, argv[2]);
      }break;

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
