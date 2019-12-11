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
 */

#include <string.h>
#include <pandore.h>
using namespace pandore;

#include <regionlut.h>

/**
 * @file ppan2gif.cpp
 * Convert a Pandore file to GIF file.
 * Widely inspired from XV: John Bradley  (bradley@cis.upenn.edu)
 */

#define MONO(rd,gn,bl) ( ((int)(rd)*11 + (int)(gn)*16 + (int)(bl)*5) >> 5)

/*
 * Use (RRRGGGBB) for a pixel encoding.
 */
#define RMASK      0xe0
#define GMASK      0xe0
#define BMASK      0xc0 

static int  Width, Height;
static int  curx, cury;
static Long CountDown;
static int  Interlace;

static void cl_block    (void);
static void cl_hash     (Long);
static void char_init   (void);
static void char_out    (int);
static void flush_char  (void);

static Uchar pc2nc[256],r1[256],g1[256],b1[256];


static void putword(int w, FILE *fp) {
   /* writes a 16-bit integer in GIF order (LSB first) */
   fputc(w & 0xff, fp);
   fputc((w>>8)&0xff, fp);
}

static Ulong cur_accum = 0;
static int           cur_bits = 0;

#define XV_BITS	12    /* BITS was already defined on some systems */
#define MSDOS	1

#define HSIZE  5003            /* 80% occupancy */

typedef Uchar   char_type;

static int n_bits;                    /* number of bits/code */
static int maxbits = XV_BITS;         /* user settable max # bits/code */
static int maxcode;                   /* maximum code, given n_bits */
static int maxmaxcode = 1 << XV_BITS; /* NEVER generate this */

#define MAXCODE(n_bits)     ( (1 << (n_bits)) - 1)

static  Long htab[HSIZE];
static  Ushort codetab[HSIZE];
#define HashTabOf(i)   htab[i]
#define CodeTabOf(i)   codetab[i]

static int hsize = HSIZE;            /* for dynamic table sizing */

/**
 * To save much memory, we overlay the table used by compress() with those
 * used by decompress().  The tab_prefix table is the same size and type
 * as the codetab.  The tab_suffix table needs 2**BITS characters.  We
 * get this from the beginning of htab.  The output stack uses the rest
 * of htab, and contains characters.  There is plenty of room for any
 * possible stack (stack used to be 8000 characters).
 */

#define tab_prefixof(i) CodeTabOf(i)
#define tab_suffixof(i) ((char_type *)(htab))[i]
#define de_stack        ((char_type *)&tab_suffixof(1<<XV_BITS))

static int free_ent = 0;                  /* first unused entry */

/*
 * Block compression parameters -- after all codes are used up,
 * and compression rate changes, start over.
 */
static int clear_flg = 0;

static Long in_count = 1;            /* length of input */
static Long out_count = 0;           /* # of codes output (for debugging) */

static int g_init_bits;
static FILE *g_outfile;

static int ClearCode;
static int EOFCode;

/**
 * TAG( output )
 *
 * Output the given code.
 * Inputs:
 *      code:   A n_bits-bit integer.  If == -1, then EOF.  This assumes
 *              that n_bits =< (long)wordsize - 1.
 * Outputs:
 *      Outputs code to the file.
 * Assumptions:
 *      Chars are 8 bits long.
 * Algorithm:
 *      Maintain a BITS character long buffer (so that 8 codes will
 * fit in it exactly).  Use the VAX insv instruction to insert each
 * code in turn.  When the buffer fills up empty it and start over.
 */

static Ulong masks[] = { 0x0000, 0x0001, 0x0003, 0x0007, 0x000F,
			  0x001F, 0x003F, 0x007F, 0x00FF,
			  0x01FF, 0x03FF, 0x07FF, 0x0FFF,
			  0x1FFF, 0x3FFF, 0x7FFF, 0xFFFF };

static void output(int code) {
   cur_accum &= masks[cur_bits];

   if (cur_bits > 0) {
      cur_accum |= ((Long)code << cur_bits);
   } else {
      cur_accum = code;
   }
	
   cur_bits += n_bits;

   while (cur_bits >= 8) {
      char_out( (int) (cur_accum & 0xff) );
      cur_accum >>= 8;
      cur_bits -= 8;
   }

   /*
    * If the next entry is going to be too big for the code size,
    * then increase it, if possible.
    */

   if (free_ent > maxcode || clear_flg) {

      if (clear_flg) {
	 maxcode = MAXCODE (n_bits = g_init_bits);
	 clear_flg = 0;
      } else {
	 n_bits++;
	 if (n_bits == maxbits) {
	    maxcode = maxmaxcode;
	 } else {
	    maxcode = MAXCODE(n_bits);
	 }
      }
   }

   if (code == EOFCode) {
      /* At EOF, write the rest of the buffer */
      while (cur_bits > 0) {
	 char_out( (int)(cur_accum & 0xff) );
	 cur_accum >>= 8;
	 cur_bits -= 8;
      }

      flush_char();
	
      fflush( g_outfile );
   }
}



/*
 * table clear for block compress.
 */
static void cl_block () {
   /* Clear out the hash table */

   cl_hash ( (Long) hsize );
   free_ent = ClearCode + 2;
   clear_flg = 1;

   output(ClearCode);
}

/*
 * reset code table
 */
static void cl_hash(Long hsize) {
   Long *htab_p = htab+hsize;
   Long i;
   Long m1 = -1;

   i = hsize - 16;
   do {                            /* might use Sys V memset(3) here */
      *(htab_p-16) = m1;
      *(htab_p-15) = m1;
      *(htab_p-14) = m1;
      *(htab_p-13) = m1;
      *(htab_p-12) = m1;
      *(htab_p-11) = m1;
      *(htab_p-10) = m1;
      *(htab_p-9) = m1;
      *(htab_p-8) = m1;
      *(htab_p-7) = m1;
      *(htab_p-6) = m1;
      *(htab_p-5) = m1;
      *(htab_p-4) = m1;
      *(htab_p-3) = m1;
      *(htab_p-2) = m1;
      *(htab_p-1) = m1;
      htab_p -= 16;
   } while ((i -= 16) >= 0);

   for ( i += 16; i > 0; i-- )
      *--htab_p = m1;
}

/*
 * Number of characters so far in this 'packet'
 */
static int a_count;

/*
 * Set up the 'Uchar output' routine
 */
static void char_init() {
   a_count = 0;
}

/*
 * Define the storage for the packet accumulator
 */
static char accum[ 256 ];

/*
 * Add a character to the end of the current packet, and if it is 254
 * characters, flush the packet to disk.
 */
static void char_out(int c) {
   accum[ a_count++ ] = c;
   if ( a_count >= 254) {
      flush_char();
   }
}

/*
 * Flush the packet to disk, and reset the accumulator
 */
static void flush_char() {
   if (a_count > 0) {
      fputc(a_count, g_outfile );
      fwrite(accum, (size_t) 1, (size_t) a_count, g_outfile );
      a_count = 0;
   }
}	

/*
 * compress stdin to stdout
 *
 * Algorithm:  use open addressing double hashing (no chaining) on the 
 * prefix code / next character combination.  We do a variant of Knuth's
 * algorithm D (vol. 3, sec. 6.4) along with G. Knott's relatively-prime
 * secondary probe.  Here, the modular division first probe is gives way
 * to a faster exclusive-or manipulation.  Also do block compression with
 * an adaptive reset, whereby the code table is cleared when the compression
 * ratio decreases, but after the table fills.  The variable-length output
 * codes are re-sized at this point, and a special CLEAR code is generated
 * for the decompressor.  Late addition:  construct the table according to
 * file size for noticeable speed improvement on small files.  Please direct
 * questions about this implementation to ames!jaw.
 */
static void compress(int   init_bits, FILE *outfile, Uchar *data, int   len) {
   Long fcode;
   int i = 0;
   int c;
   int ent;
   int disp;
   int hsize_reg;
   int hshift;

   /*
    * Set up the globals:  g_init_bits - initial number of bits
    *                      g_outfile   - pointer to output file
    */
   g_init_bits = init_bits;
   g_outfile   = outfile;

   /* initialize 'compress' globals */
   maxbits = XV_BITS;
   maxmaxcode = 1<<XV_BITS;
   memset((char *) htab, 0, sizeof(htab));
   memset((char *) codetab,0,  sizeof(codetab));
   hsize = HSIZE;
   free_ent = 0;
   clear_flg = 0;
   in_count = 1;
   out_count = 0;
   cur_accum = 0;
   cur_bits = 0;

   /*
    * Set up the necessary values
    */
   out_count = 0;
   clear_flg = 0;
   in_count = 1;
   maxcode = MAXCODE(n_bits = g_init_bits);

   ClearCode = (1 << (init_bits - 1));
   EOFCode = ClearCode + 1;
   free_ent = ClearCode + 2;

   char_init();
   ent = pc2nc[*data++];  len--;

   hshift = 0;
   for ( fcode = (Long) hsize;  fcode < 65536L; fcode *= 2L )
      hshift++;
   hshift = 8 - hshift;                /* set hash code range bound */

   hsize_reg = hsize;
   cl_hash( (Long) hsize_reg);            /* clear hash table */

   output(ClearCode);
    
   while (len) {
      c = pc2nc[*data++];  len--;
      in_count++;

      fcode = (Long) ( ( (Long) c << maxbits) + ent);
      i = (((int) c << hshift) ^ ent);    /* xor hashing */

      if ( HashTabOf (i) == fcode ) {
	 ent = CodeTabOf (i);
	 continue;
      } else if ( (Long)HashTabOf (i) < 0 ) {      /* empty slot */
	 goto nomatch;
      }
      disp = hsize_reg - i;           /* secondary hash (after G. Knott) */
      if ( i == 0 )
	 disp = 1;

   probe:
      if ( (i -= disp) < 0 )
	 i += hsize_reg;

      if ( HashTabOf (i) == fcode ) {
	 ent = CodeTabOf (i);
	 continue;
      }

      if ( (Long)HashTabOf (i) >= 0 ) 
	 goto probe;

   nomatch:
      output(ent);
      out_count++;
      ent = c;

      if ( free_ent < maxmaxcode ) {
	 CodeTabOf (i) = free_ent++; /* code -> hashtable */
	 HashTabOf (i) = fcode;
      } else {
	 cl_block();
      }
   }

   /* Put out the final code */
   output(ent);
   out_count++;
   output(EOFCode);
}



Errc WriteGIF(FILE* fp, Uchar* pic8, int ptype, int w, int h, const Uchar* rmap, const Uchar* gmap, const Uchar *bmap, int numcols, int grayscale) {
   int RWidth, RHeight;
   int LeftOfs, TopOfs;
   int ColorMapSize, InitCodeSize, Background, BitsPerPixel;
   int i,j,nc;

   Interlace = 0;
   Background = 0;
  
   for (i=0; i<256; i++) {
      pc2nc[i]=r1[i]=g1[i]=b1[i]=0;
   }
  
   /* Total different colors. */
   nc = 0;
  
   for (i=0; i<numcols; i++) {
      /* see if color #i is already used */
      for (j=0; j<i; j++) {
	 if (rmap[i] == rmap[j] && gmap[i] == gmap[j] && bmap[i] == bmap[j]) {
	    break;
	 }
      }
      if (j==i) {  /* wasn't found */
	 pc2nc[i] = nc;
	 r1[nc] = rmap[i];
	 g1[nc] = gmap[i];
	 b1[nc] = bmap[i];
	 nc++;
      } else {
	 pc2nc[i] = pc2nc[j];
      }
   }

   /* figure out 'BitsPerPixel' */
   for (i=1; i<8; i++) {
      if ( (1<<i) >= nc) {
	 break;
      }
   }
  
   BitsPerPixel = i;

   ColorMapSize = 1 << BitsPerPixel;
	
   RWidth  = Width  = w;
   RHeight = Height = h;
   LeftOfs = TopOfs = 0;
	
   CountDown = w * h;    /* # of pixels we'll be doing */

   if (BitsPerPixel <= 1) { 
      InitCodeSize = 2;
   } else {
      InitCodeSize = BitsPerPixel;
   }

   curx = cury = 0;

   if (!fp) {
      fprintf(stderr, "Error ppan2gif: WriteGIF: file not open for writing\n" );
      if (ptype == 24) free(pic8);
      return FAILURE;
   }

   fwrite("GIF87a", (size_t) 1, (size_t) 6, fp);    /* the GIF magic number */

   putword(RWidth, fp);           /* screen descriptor */
   putword(RHeight, fp);

   i = 0x80;	                 /* Yes, there is a color map */
   i |= (8-1)<<4;                 /* OR in the color resolution (hardwired 8) */
   i |= (BitsPerPixel - 1);       /* OR in the # of bits per pixel */
   fputc(i,fp);          

   fputc(Background, fp);         /* background color */

   fputc(0, fp);                  /* future expansion Uchar */

   if (grayscale) {         /* greyscale */
      for (i=0; i<ColorMapSize; i++) {
	 j = MONO(r1[i], g1[i], b1[i]);
	 fputc(j, fp);
	 fputc(j, fp);
	 fputc(j, fp);
      }
   } else {
      for (i=0; i<ColorMapSize; i++) {       /* write out Global colormap */
	 fputc(r1[i], fp);
	 fputc(g1[i], fp);
	 fputc(b1[i], fp);
      }
   }
  
   fputc( ',', fp );              /* image separator */

   /* Write the Image header */
   putword(LeftOfs, fp);
   putword(TopOfs,  fp);
   putword(Width,   fp);
   putword(Height,  fp);
   if (Interlace) {
      fputc(0x40, fp);   /* Use Global Colormap, maybe Interlace */
   } else {
      fputc(0x00, fp);
   }

   fputc(InitCodeSize, fp);
   compress(InitCodeSize+1, fp, pic8, w*h);

   fputc(0,fp);                      /* Write out a Zero-length packet (EOF) */
   fputc(';',fp);                    /* Write GIF file terminator */

   if (ptype == 24) free(pic8);

   if (ferror(fp)) return FAILURE;
   return SUCCESS;
}

Errc PPan2GIF( const Img2duc &ims, char *fich ) {
   FILE* dfd;
   int i,j;
   Uchar *pic, *pic8;
   Uchar rmap[256],gmap[256],bmap[256];
  
   if ((fich==NULL) || (!strcmp(fich, "-"))) {
      dfd = stdout;
   } else if (!(dfd = fopen(fich, "wb"))) {
      fprintf(stderr, "Error ppan2gif: Cannot create GIF file %s\n", fich);
      Exit(FAILURE);
   }
   
   pic = pic8=(Uchar *)calloc(ims.Width()*ims.Height(), sizeof(Char));
   
   for (i=0; i<ims.Height();i++) {
      for (j=0; j<ims.Width();j++) {
	 *(pic++) = ims[i][j];
      }
   }
   for (i=0; i< 256; i++) {
      rmap[i]=gmap[i]=bmap[i]=i;
   }
   
   WriteGIF(dfd,pic8,8,ims.Width(),ims.Height(),rmap,gmap,bmap,256, 1 );
   
   free(pic8);
   
   return SUCCESS; 
} 

/* RANGE forces a to be in the range b..c (inclusive) */
#define ECRETE(a,b,c) { if (a < b) a = b;  if (a > c) a = c;}

int Suffisant( const Imc2duc &imi, Uchar *pic8, Uchar* rmap, Uchar* gmap, Uchar* bmap, int *numcols ) {
   /* scans picture until it finds more than 'maxcol' different colors.
      If it finds more than 'maxcol' colors, it returns '0'.  If it
      DOESN'T, it does the 24-to-8 conversion by simply sticking the
      colors it found into a colormap, and changing instances of a
      color in pic24 into colormap indicies (in pic8) */

   Ulong colors[257],col;
   int i, j, k, nc, low, high, mid;

   /* put the first color in the table by hand */
   nc = 0; mid = 0;
   for (i=0; i<imi.Height(); i++) {
      for (j=0; j<imi.Width(); j++) {
	 col  = (Ulong)(imi.X[i][j])<<16;
	 col += (Ulong)(imi.Y[i][j])<<8;
	 col += (Ulong)(imi.Z[i][j]);
	 /* Recherche de la couleur si elle existe deja */
	 low = 0; high = nc-1;
	 while (low <= high) {
	    mid = (low+high)/2;
	    if (col < colors[mid]) {
	       high = mid - 1;
	    } else {
	       if (col > colors[mid]) {
		  low = mid + 1;
	       } else {
		  break;
	       }
	    }
	 }
	 if (high < low) { /* POas trouvee, alors l'ajouter. */
	    if (nc>=256) {
	       return 0;
	    }
	    for (k=nc; k>=low; k--) {
	       colors[k+1]=colors[k];
	    }
	    colors[low] = col;
	    nc++;
	 }
      }
   }
   /* run through the data a  second time, this time mapping pixel values in pic24 into colormap offsets into 'colors' */
   for (i=0; i<imi.Height(); i++) {
      for (j=0; j<imi.Width(); j++) {
	 col =(Ulong)(imi.X[i][j]) << 16;
	 col += (Ulong)(imi.Y[i][j]) << 8;
	 col += (Ulong)(imi.Z[i][j]); 
	 /* binary search the 'colors' array.  It *IS* in there */
	 low = 0; high = nc-1;
	 while (low <= high) {
	    mid = (low+high)/2; 
	    if (col < colors[mid]) {
	       high = mid - 1; 
	    } else {
	       if (col > colors[mid]) {
		  low = mid + 1; 
	       } else {
		  break; 
	       }
	    }
	 } 
	 if (high < low) {
	    fprintf(stderr,"Error ppan2gif: quick_check: impossible situation!\n"); 
	    exit(1);
	 }
	 *(pic8++) = (Uchar)mid;
      }
   }
   /* and load up the 'desired colormap' */ 
   for (i=0; i<nc; i++) {
      rmap[i] = (Uchar)(colors[i]>>16);
      gmap[i] = (Uchar)((colors[i]>>8) & 0xff);
      bmap[i] = (Uchar)(colors[i] & 0xff);
   }
  
   *numcols=nc; 
   return 1;
}

Errc Reduit24en8( const Imc2duc &imi, Uchar *pic8, Uchar* rmap, Uchar *gmap, Uchar *bmap, int *numcols ) {
   int  r1, g1, b1;
   int  *line1, *line2, *l1, *l2;
   int  i, j, val;

   /* Intialisation of the colormap */
   for (i=0; i<256; i++) {
      rmap[i] = (( i     & RMASK)*255 + RMASK/2) / RMASK;
      gmap[i] = (((i<<3) & GMASK)*255 + GMASK/2) / GMASK;
      bmap[i] = (((i<<6) & BMASK)*255 + BMASK/2) / BMASK;
   }

   if ((!(line1=(int *) malloc(imi.Width()*3 * sizeof(int)))) ||
       (!(line2=(int *) malloc(imi.Width()*3 * sizeof(int)))))
      return FAILURE;
  
   /* get first line of image */
   for (j=0; j<imi.Width();j++) {
      line2[3*j] = (int)imi.X[0][j];
      line2[(3*j)+1] = (int)imi.Y[0][j];
      line2[(3*j)+2] = (int)imi.Z[0][j];
   }
   for (i=0; i<imi.Height(); i++) {
      l1 = line1; line1 = line2; line2 = l1;
      if (i<imi.Height()-1) {  
	 /* get next line */
	 for (j=0; j< imi.Width(); j++) {
	    line2[3*j]     = (int)imi.X[i+1][j];
	    line2[(3*j)+1] = (int)imi.Y[i+1][j];
	    line2[(3*j)+2] = (int)imi.Z[i+1][j];
	 }
      }
      for (j=0, l1=line1, l2=line2; j<imi.Width();j++, pic8++) {
	 r1 = *l1++;g1 = *l1++; b1 = *l1++;
	 ECRETE(r1,0,255); ECRETE(g1,0,255); ECRETE(b1,0,255);
	 /* choose actual pixel value */
	 val = ((r1&RMASK)|((g1&GMASK)>>3)|((b1&BMASK)>>6));
	 *pic8 = (Uchar)val;
      
	 /* compute color errors */
	 r1 -= rmap[val];
	 g1 -= gmap[val];
	 b1 -= bmap[val];
	 /* Add fractions of errors to adjacent pixels */
	 if (j<imi.Width()-1) {  /* adjust RIGHT pixel */
	    l1[0] += (r1*7) / 16;
	    l1[1] += (g1*7) / 16;
	    l1[2] += (b1*7) / 16;
	 }
      
	 if (i<imi.Height()-1) {	/* do BOTTOM pixel */
	    l2[0] += (r1*5) / 16;
	    l2[1] += (g1*5) / 16;
	    l2[2] += (b1*5) / 16;
	
	    if (j>0) {  /* do BOTTOM LEFT pixel */
	       l2[-3] += (r1*3) / 16;
	       l2[-2] += (g1*3) / 16;
	       l2[-1] += (b1*3) / 16;
	    }
	
	    if (j<imi.Width()-1) {  /* do BOTTOM RIGHT pixel */
	       l2[3] += (r1)/16;
	       l2[4] += (g1)/16;
	       l2[5] += (b1)/16;
	    }
	    l2 += 3;
	 }
      }
   }
  
   free(line1);
   free(line2);
   *numcols = 256;
   return SUCCESS;
}

/*
 * Color images : converts 24 bits to 8 bits.
 */
Errc Convertie24en8( const Imc2duc &imi, Uchar *pic8, Uchar* rmap, Uchar* gmap, Uchar* bmap, int *numcols ) {
   if (!Suffisant(imi,pic8,rmap, gmap, bmap, numcols)) {
      /* No color compression was necessary */
      Reduit24en8(imi,pic8,rmap,gmap,bmap, numcols);
   }
   return SUCCESS;
}

Errc PPan2GIF( const Imc2duc &ims, char *fich ) {
   FILE* dfd;
   Uchar *pic8;
   Uchar rmap[256],gmap[256],bmap[256];
   int numcols;

   if ((fich == NULL) || (!strcmp(fich, "-"))) {
      dfd = stdout;
   } else if (!(dfd = fopen(fich,"wb"))) {
      fprintf(stderr, "Error ppan2gif: Cannot create GIF file %s\n", fich);
      Exit(FAILURE);
   }
  
   pic8=(Uchar *)calloc(ims.Width()*ims.Height(), sizeof(Char));
   Convertie24en8(ims, pic8, rmap, gmap, bmap, &numcols);

   WriteGIF(dfd,pic8,8,ims.Width(),ims.Height(),rmap,gmap,bmap,numcols,0);
  
   free(pic8);
  
   return SUCCESS;
} 

/*
 * Reg2d -> Imc2uc with random colors.
 */
Errc PPan2GIF( const Reg2d &rgs, char *fich ) {
   FILE* dfd;

   if ((fich == NULL) || (!strcmp(fich, "-"))) {
      dfd = stdout;
   } else if (!(dfd = fopen(fich, "wb"))) {
      fprintf(stderr, "Error ppan2gif: Cannot create GIF file %s\n",fich);
      Exit(FAILURE);
   }
   Imc2duc imi(rgs.Props());
   
   Uchar lut[3][MAXCOLORS];

   buildRegionLut(lut);

   const Ulong *datasrc = rgs.Vector();
   unsigned long length  = rgs.VectorSize();
   Uchar *data = imi.Vector();
   for (Ulong i = 0; i< length; i++ ) {
      Ulong  pixel = datasrc[i]%MAXCOLORS;
      data[i] = lut[0][pixel];
      data[length+i] = lut[1][pixel];
      data[2*length+i] = lut[2][pixel];
   }
   
   PPan2GIF( imi, fich);

   return SUCCESS;
}

#ifdef MAIN

/*
 * Modify only the following constants, and the function call.
 */

#define	USAGE	"usage: %s [im_in|-] [im_out|-]"
#define	PARC	0
#define	FINC	1
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

   ReadArgs(argc,argv,PARC,FINC,FOUTC,&mask,objin,objs,objout,objd,parv,USAGE,MASK);

   switch(objs[0]->Type()) {
   case Po_Img2duc :{
      Img2duc* const ims=(Img2duc*)objs[0];

      result=PPan2GIF(*ims,(argc>2)? argv[2] : NULL);
   }break;
   case Po_Reg2d :{
      Reg2d* const ims=(Reg2d*)objs[0];

      result=PPan2GIF(*ims,(argc>2)? argv[2] : NULL);
   }break;

   case Po_Imc2duc :{
      Imc2duc* const ims=(Imc2duc*)objs[0];

      result=PPan2GIF(*ims,(argc>2)? argv[2] : NULL);
   }break;
   
   default :
      PrintErrorFormat(objin,FINC);
      result=FAILURE;
   }
   if (result)
      WriteArgs(argc,argv,PARC,FINC,FOUTC,&mask,objin,objs,objout,objd,MASK);

   Exit(result);
   return(0);
}

#endif
