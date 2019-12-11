/* -*- mode: c++; c-basic-offset: 3 -*-
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
 * For more information, refer to:
 * https://clouard.users.greyc.fr/Pandore/
 */

/**
 * @author Jean-Luc Starck (CEA 1996).
 */

#ifndef _FITSIO_H
#define _FITSIO_H

//#include "_fitsio.h"

#define INT32BIT int  /* 32-bit integer datatype.  Currently this       */
                      /* datatype is an 'int' on all useful platforms   */
                      /* however, it is possible that that are cases    */
                      /* where 'int' is a 2-byte integer, in which case */
                      /* FITSINT would need to be defined as 'long'.    */

/*
 * A few definitions to read FITS parameters
 */

#define FBSIZE  2880L   /* size (in bytes) of one FITS block */

#define FITSTOF(k, def) \
                        ((point = fitsnfind(buf, k, n))? \
                                 atof(strncpy(st, &point[10], 70)) \
                                :(def))

#define FITSTOI(k, def) \
                        ((point = fitsnfind(buf, k, n))? \
                                 atoi(strncpy(st, &point[10], 70)) \
                                :(def))
#define FITSTOS(k, str, def) \
                        { point = fitsnfind(buf, k, n); \
                          if (point != NULL) \
                                { \
                                for (i=0,point+=11; (*point)!='\'' && i < 69;) \
                                        (str)[i++] = *(point++); \
                                (str)[i] = '\0'; \
                                } \
                          else\
                                strcpy(str, def); \
                        }
#define QFREAD(ptr, size, file, fname) \
                if (fread(ptr, (size_t)(size), (size_t)1, file)!=1) \
                  error(EXIT_FAILURE, "*Error* while reading ", fname)

#define QFWRITE(ptr, size, file, fname) \
                if (fwrite(ptr, (size_t)(size), (size_t)1, file)!=1) \
                  error(EXIT_FAILURE, "*Error* while writing ", fname)

#define QFSEEK(file, offset, pos, fname) \
                if (fseek(file, (offset), pos)) \
                  error(EXIT_FAILURE,"*Error*: file positioning failed in ", \
                        fname)
#define QFTELL(pos, file, fname) \
                if ((pos=ftell(file))==-1) \
                  error(EXIT_FAILURE,"*Error*: file position unknown in ", \
                        fname)
#define	FITSABS(x) (((x)>0)? (x) : (-1*(x)))

/* int     t_size[] = {1, 2, 4, 4, 8}; */
typedef enum {H_INT, H_FLOAT, H_EXPO, H_BOOL, H_STRING, H_COMMENT,
	      H_KEY}  h_type;         /* type of FITS-header data */


enum type_data {T_BYTE, T_SHORT, T_INT, T_FLOAT, T_DOUBLE,
		T_COMPLEX_F, T_COMPLEX_D, UN_KNOWN};

typedef unsigned char byte;
typedef unsigned long u_long;

/*
 * FITS BitPix coding
 */

#define BP_BYTE         8
#define BP_SHORT        16
#define BP_INT          32
#define BP_FLOAT        (-32)
#define BP_DOUBLE       (-64)

#define MAX_NBR_AXIS 	3 /* Maximum number of axes */
#define FLOAT_EPSILON 5.96047e-08

#define DBUFFSIZE 28800 /* size of data buffer in bytes */

#define NIOBUF 25        /* number of IO buffers to create */
#define IOBUFLEN 2880    /* size in bytes of each IO buffer */
#define MINDIRECT 8640   /* minimum size for direct reads and writes */
                         /* MINDIRECT must have a value >= 8640 */

#define NATIVE             0 /* a generic machine that uses IEEE formats */
#define ULTRIX             1
#define ALPHA_OSF          2
#define VAXVMS             3
#define ALPHAVMS           4
#define IBMPC              5
#define CRAY               6

#define GFLOAT             1
#define IEEEFLOAT          2

/* the following are used to determine what type machine we are running on */

/* the following block determines the size of longs on SGI IRIX machines */
#if defined(_MIPS_SZLONG)
#  if _MIPS_SZLONG == 32
#    define LONGSIZE 32
#  elif _MIPS_SZLONG == 64
#    define LONGSIZE 64
#  else
#    error "can't handle long size given by _MIPS_SZLONG"
#  endif
#endif

#if defined(vax) && defined(VMS)
 
#define MACHINE VAXVMS
#define BYTESWAPPED 1
 
#elif defined(__alpha) && defined(__VMS) &&  (__D_FLOAT == 1)

/* this float option is the same as for VAX/VMS machines. */
#define MACHINE VAXVMS
#define BYTESWAPPED 1
 
#elif defined(__alpha) && defined(__VMS) &&  (__G_FLOAT == 1)
 
/*  G_FLOAT is the default for ALPHA VMS systems */
#define MACHINE ALPHAVMS
#define BYTESWAPPED 1
#define FLOATTYPE GFLOAT
 
#elif defined(__alpha) && defined(__VMS) &&  (__IEEE_FLOAT == 1)
 
#define MACHINE ALPHAVMS
#define BYTESWAPPED 1
#define FLOATTYPE IEEEFLOAT

#elif defined(__alpha) && defined(__unix__)
 
#define MACHINE ALPHA_OSF
#define BYTESWAPPED 1
#define LONGSIZE 64

#elif defined(ultrix) && defined(unix)
 
#define MACHINE ULTRIX
#define BYTESWAPPED 1
 
#elif defined(__i386) || defined(__i386__) || defined(__i486__) || defined(__i586__) 

/*  IBM PC */
#define MACHINE IBMPC
#define BYTESWAPPED 1

#elif defined(_MSC_VER) || defined(__BORLANDC__) || defined(__TURBOC__)

/*  IBM PC running DOS or Windows */
#define MACHINE IBMPC       
#define BYTESWAPPED 1

#elif defined(_NI_mswin_)

/*  LabWindows/CVI with Windows 3.x, 95, or NT  */
#define MACHINE IBMPC       
#define BYTESWAPPED 1

#elif  defined(__EMX__)

/*  IBM PC running OS/2 */
#define MACHINE IBMPC
#define BYTESWAPPED 1

#else

/*  assume machine uses the same IEEE formats as used in FITS files */
#define MACHINE NATIVE
#define BYTESWAPPED 0
 
#endif

/*  assume longs are 4 bytes long, unless previously set otherwise */
#ifndef LONGSIZE
#define LONGSIZE 32
#endif
  
#define IGNORE_EOF 1
#define REPORT_EOF 0
#define DATA_UNDEFINED -1
#define NULL_UNDEFINED 1234554321
#define ASCII_NULL_UNDEFINED 1   /* indicate no defined null value */
 
#define maxvalue(A,B) ((A) > (B) ? (A) : (B))
#define minvalue(A,B) ((A) < (B) ? (A) : (B))
 
#define FSTRCMP(a,b)     ((a)[0]<(b)[0]? -1:(a)[0]>(b)[0]?1:strcmp((a),(b)))
#define FSTRNCMP(a,b,n)  ((a)[0]<(b)[0]?-1:(a)[0]>(b)[0]?1:strncmp((a),(b),(n)))

#if MACHINE == ALPHAVMS
 
#define FNANMASK   0x7F00 /* mask bits 1 - 7; all set on NaNs */
/* all 0 on underflow  or 0. */
 
#define DNANMASK   0x7FE0 /* mask bits 1 - 10; all set on NaNs */
/* all 0 on underflow  or 0. */
 
#else
 
#define FNANMASK   0x7F80 /* mask bits 1 - 8; all set on NaNs */
/* all 0 on underflow  or 0. */
 
#define DNANMASK   0x7FF0 /* mask bits 1 - 11; all set on NaNs */
/* all 0 on underflow  or 0. */
 
#endif

#if MACHINE == CRAY
/*
  Cray machines:   the large negative integer corresponds
  to the 3 most sig digits set to 1.   If these
  3 bits are set in a floating point number (64 bits), then it represents
  a reserved value (i.e., a NaN)
*/
#define fnan(L) ( (L) >= 0xE000000000000000 ? 1 : 0)


#else
/* these functions work for both big and little endian machines */
/* that use the IEEE floating point format for internal numbers */
 
/* These functions tests whether the float value is a reserved IEEE     */
/* value such as a Not-a-Number (NaN), or underflow, overflow, or       */
/* infinity.   The functions returns 1 if the value is a NaN, overflow  */
/* or infinity; it returns 2 if the value is an denormalized underflow  */
/* value; otherwise it returns 0. fnan tests floats, dnan tests doubles */
 
#define fnan(L) \
      ( (L & FNANMASK) == FNANMASK ?  1 : (L & FNANMASK) == 0 ? 2 : 0)
 
#define dnan(L) \
      ( (L & DNANMASK) == DNANMASK ?  1 : (L & DNANMASK) == 0 ? 2 : 0)
 
#endif

#define DUCHAR_MAX  255.49 /* max double value that fits in an unsigned char */
#define DUCHAR_MIN -0.49   /* min double value that fits in an unsigned char */
#define DUSHRT_MAX  65535.49 /* max double value that fits in a unsigned short*/
#define DUSHRT_MIN -0.49   /* min double value that fits in an unsigned short */
#define DSHRT_MAX  32767.49 /* max double value that fits in a short */
#define DSHRT_MIN -32768.49 /* min double value that fits in a short */
#define DULONG_MAX 4294967295.49 /* max double that fits in a unsigned long */
#define DULONG_MIN -0.49   /* min double value that fits in an unsigned long */
#define DLONG_MAX  2147483647.49 /* max double value that fits in a long */
#define DLONG_MIN -2147483648.49 /* min double value that fits in a long */
#define DUINT_MAX 4294967295.49 /* max dbl that fits in a unsigned 4-byte int */
#define DUINT_MIN -0.49   /* min dbl that fits in an unsigned 4-byte int */
#define DINT_MAX  2147483647.49 /* max double value that fits in a 4-byte int */
#define DINT_MIN -2147483648.49 /* min double value that fits in a 4-byte int */

#ifndef UINT32_MAX
#define UINT32_MAX 4294967295U /* max unsigned 32-bit integer */
#endif
#ifndef INT32_MAX
#define INT32_MAX  2147483647 /* max 32-bit integer */
#endif
#ifndef INT32_MIN
#define INT32_MIN -2147483647 /* min 32-bit integer */
#endif

/* ==================== SHARED MEMORY DRIVER SECTION ======================= */

#ifdef HAVE_SHMEM_SERVICES
#include "drvrsmem.h"
#endif

/* ==================== END OF SHARED MEMORY DRIVER SECTION ================ */

#if defined(vms) || defined(__vms) || defined(__WIN32__) || defined (WIN32) || defined (_WIN32) || defined(macintosh)

/* ================================================================== */
/* A hack for nonunix machines, which lack strcasecmp and strncasecmp */
/* ================================================================== */

int strcasecmp (char *s1, char *s2       );
int strncasecmp(char *s1, char *s2, int n);

#endif

/*
 * Fits image parameters
 */

class fitsstruct {
   void fitsinit() {
      file=NULL;	
      fitsheadsize= 2880;
      bitpix = 0;
      bytepix = 0;
      width = 0;
      height = 0;
      npix = 0;
      bscale = 1.;
      bzero = 0.;
      crpixx = 0.;
      crpixy = 0.;
      crvalx = 0.;
      crvaly = 0.;
      cdeltx= 0.;
      cdelty = 0.;
      ngamma=0.;
      pixscale=0.;
      nlevels = 0;
      pixmin  = 0.;
      pixmax  = 0.;
      epoch = 0.;
      crotax = 0.;
      crotay = 0.;
      fitshead = NULL;
      origin = "";
      strcpy(ctypex, "");
      strcpy(ctypey, "");
      strcpy(rident, "");
    
      /* HISTORY & COMMENT fields */
      history = NULL;
      hist_size = 0;
      comment = NULL;
      com_size = 0;
    
      naxis=0;
      for (int i=0; i < MAX_NBR_AXIS;i++) {
	 TabAxis[i]=0;
	 TabStep[i]=0.;
	 TabRef[i]=0.;
	 TabValRef[i]=0.;
      }
      filename = NULL;
      origin = NULL;
      fitshead = NULL;
   }
 public:
  
   fitsstruct ()  { 
      fitsinit();
   }
  
   /*void hd_fltarray(fltarray &Mat, char *History=NULL) {
     char *creafitsheader();
    
     fitshead = creafitsheader();
     bitpix = -32;
     width =  Mat.nx();
     height = Mat.ny();
     naxis = Mat.naxis();
     npix = Mat.n_elem();
     for (int i=0; i < naxis; i++) TabAxis[i] = Mat.axis(i+1);
     if (History != NULL) origin = History;
     }*/
   ~fitsstruct() { 
      if (filename != NULL) free (filename);
      // if (fitshead != NULL) free ((char *) fitshead);
      if (history != NULL)  free (history);
      if (comment != NULL)  free (comment);
      fitsinit();
   }
  
   char *filename;		/* pointer to the image filename */
   const char *origin;	       	/* pointer to the origin */
   char ident[512];		/* field identifier (read from FITS)*/
   char rident[512];	        /* field identifier (relative) */
   FILE *file;			/* pointer the image file structure */
   char *fitshead;		/* pointer to the FITS header */
   int fitsheadsize;		/* FITS header size */
   /* ---- main image parameters */
   int bitpix, bytepix;		/* nb of bits and bytes per pixel */
   int width, height;		/* x,y size of the field */
   int npix;			/* total number of pixels */
   double bscale, bzero;		/* FITS scale and offset */
   double ngamma;		/* normalized photo gamma */
   int nlevels;			/* nb of quantification levels */
   float pixmin, pixmax;		/* min and max values in frame */
   /* ---- basic astrometric parameters */
   double epoch;			/* epoch for coordinates */
   double pixscale;		/* pixel size in arcsec.pix-1 */
   /* */
   /* ---- astrometric parameters */
   double crpixx,crpixy;		/* FITS CRPIXn */
   double crvalx,crvaly;		/* FITS CRVALn */
   double cdeltx,cdelty;		/* FITS CDELTn */
   double crotax,crotay;		/* FITS CROTAn */
   char ctypex[256];		/* FITS CTYPE1 */
   char ctypey[256];		/* FITS CTYPE2 */
   char CoordType[256];         
  
   /* ---- HISTORY & COMMENT parameters --- */
   char *history;
   int hist_size;
   char *comment;
   int com_size;
  
   /* ---- for non image use */
   int naxis;
   int TabAxis[MAX_NBR_AXIS];
   double TabStep[MAX_NBR_AXIS];
   double TabRef[MAX_NBR_AXIS];
   double TabValRef[MAX_NBR_AXIS];
};
 
/* FITS read-write utilities. */
  
enum Bool {False = 0,True = 1};
int DEBUH = 0;

/*
 *
 */
void error(int num, const char *msg1, const char *msg2) {
   fprintf(stderr, "\n> %s%s\n\n",msg1,msg2);
   exit(num);
}

/*
 * swapbytes
 */

void swapbytes(void *ptr, int nb, int n) {
   
   /*
    * Swap bytes for doubles, longs and shorts
    * (for DEC machines or PC for inst.).
    */
   int	j;   
   char *cp = (char *)ptr;
   
   if (nb&4) {
      for (j=0; j<n; j++) {
	 cp[0]^=cp[3]; cp[3]^=cp[0]; cp[0]^=cp[3];
	 cp[1]^=cp[2]; cp[2]^=cp[1]; cp[1]^=cp[2];
	 cp +=4;
      }
      return;
   }
   if (nb&2) {
      for (j=0; j<n; j++) {
	 cp[0]^=cp[1]; cp[1]^=cp[0]; cp[0]^=cp[1];
	 cp += 2;
      }
      return;
   }
   if (nb&1)
      return;
  
   if (nb&8) {
      for (j=0; j<n; j++) {
	 cp[0]^=cp[7]; cp[7]^=cp[0]; cp[0]^=cp[7];
	 cp[1]^=cp[6]; cp[6]^=cp[1]; cp[1]^=cp[6];
	 cp[2]^=cp[5]; cp[5]^=cp[2]; cp[2]^=cp[5];
	 cp[3]^=cp[4]; cp[4]^=cp[3]; cp[3]^=cp[4];
	 cp += 8;
      }
      return;
   }
  
   error(EXIT_FAILURE, "*Internal Error*: Unknown size in ", "swapbytes()");
  
   return;
}

/*--------------------------------------------------------------------------*/
void ffswap2(short *svalues,  /* IO - pointer to shorts to be swapped       */
             long nvals) {      /* I  - number of shorts to be swapped        */
   /*
     swap the bytes in the input short integers: ( 0 1 -> 1 0 )
   */
   register char *cvalues;
   register long ii;
  
   union u_tag {
      char cvals[2];   /* equivalence an array of 4 bytes with */
      short sval;      /* a short */
   } u;
  
   cvalues = (char *) svalues;      /* copy the initial pointer value */
  
   for (ii = 0; ii < nvals;) {
      u.sval = svalues[ii++];  /* copy next short to temporary buffer */
    
      *cvalues++ = u.cvals[1]; /* copy the 2 bytes to output in turn */
      *cvalues++ = u.cvals[0];
   }
   return;
}

/*--------------------------------------------------------------------------*/
void ffswap4(INT32BIT *ivalues,  /* IO - pointer to floats to be swapped    */
	     long nvals) {    /* I  - number of floats to be swapped     */
   /*
     swap the bytes in the input 4-byte integer: ( 0 1 2 3 -> 3 2 1 0 )
   */
   register char *cvalues;
   register long ii;
  
   union u_tag {
      char cvals[4];      /* equivalence an array of 4 bytes with */
      INT32BIT ival;      /* a float */
   } u;
  
   cvalues = (char *) ivalues;   /* copy the initial pointer value */
  
   for (ii = 0; ii < nvals;) {
      u.ival = ivalues[ii++];  /* copy next float to buffer */
    
      *cvalues++ = u.cvals[3]; /* copy the 4 bytes in turn */
      *cvalues++ = u.cvals[2];
      *cvalues++ = u.cvals[1];
      *cvalues++ = u.cvals[0]; 
   }
   return;
}

/*
 *  swap the bytes in the input doubles: ( 01234567  -> 76543210 )
 */
void ffswap8(double *dvalues,  /* IO - pointer to doubles to be swapped     */
             long nvals) {      /* I  - number of doubles to be swapped      */
   register char *cvalues;
   register long ii;
  
   union u_tag {
      char cvals[8];   /* equivalence an array of 8 bytes with */
      double dval;      /* a double */
   } u;
  
   cvalues = (char *) dvalues;      /* copy the pointer value */
  
   for (ii = 0; ii < nvals;) {
      u.dval = dvalues[ii++];  /* copy next double to buffer */
    
      *cvalues++ = u.cvals[7]; /* copy the 8 bytes in turn */
      *cvalues++ = u.cvals[6];
      *cvalues++ = u.cvals[5];
      *cvalues++ = u.cvals[4];
      *cvalues++ = u.cvals[3];
      *cvalues++ = u.cvals[2];
      *cvalues++ = u.cvals[1];
      *cvalues++ = u.cvals[0];
   }
   return;
}

char *fitsname(char * NameStep) {
   char Sreturn[256];
   char Name[256];
   char *ValRet;
  
   strcpy(Name, NameStep);
   if (strstr(Name, ".fit") != NULL)  strcpy(Sreturn, Name);
   else sprintf(Sreturn, "%s.%s", Name, "fits");
#if defined _WIN32 || defined WIN32
   ValRet = _strdup(Sreturn);
#else
   ValRet = strdup(Sreturn);
#endif

   return (ValRet);
}

Bool std_inout(char *) { return False; }

FILE *fits_file_des_in(char *fname) {
   FILE *fp;
  
   if (std_inout(fname) == True)  fp = stdin;
   else {
      fp = fopen(fitsname(fname), "rb");
      if (!fp) {
	 std::cerr << "Unable to open file: " <<  fname  << std::endl;
	 exit(-1);
      }
   }
   return fp;
}

FILE *fits_file_des_out(char *fname) {
   FILE *fp;
  
   if (std_inout(fname) == True) fp = stdout;
   else {
      fp = fopen(fitsname(fname), "wb");
      if (!fp) {
	 std::cerr << "Unable to open file: " <<  fname  << std::endl;
	 exit(-1);
      }
   }
   return fp;
} 


/* Search for a FITS keyword in a FITS header. 
   return the first line containing the keyword: keyword */
int fitsfind( const char *fitsbuf, const char *keyword )  {
   char	*ptr;
   int	i=0, len;
   
   len = strlen(keyword);
  
   ptr = (char *) &(fitsbuf[80*i]);
   // RG printf(" %80s\n", ptr);
   for (i=0; strncmp(ptr, "END     ", 8); i++) {
      if (!strncmp(ptr, keyword, len)) {
	 // printf(" we got it %d\n",i);
	 return i;
      }
      ptr= (char *) &(fitsbuf[80*(i+1)]);
   }
   if (strncmp(keyword, "END     ", 8)) {
      return -1;
   } else {
      return i;
   }
}

char *creafitsheader() {
   char *Buf=NULL;
   int i,k;
   static char	bufmodel[4][80] = {
      "SIMPLE  =                    T / THIS IS REGULAR FITS",
      "BITPIX  =                    0 /",
      "NAXIS   =                    0 /",
      "END"};
  
   if (!(Buf = (char *) malloc ((size_t)(FBSIZE*sizeof(char)))))
      error(EXIT_FAILURE, "*Error*: Not enough memory in ", "creafitsheader()");
  
   for (k = 0; k < 4; k++) {
      for (i = 0; i < (int) strlen (bufmodel[k]); i++) Buf[k*80+i] = bufmodel[k][i];
      for (i = strlen (bufmodel[k]); i < 80; i++) Buf[k*80+i] = ' ';
    
   }
   for (k = 4; k < 36; k++) for (i = 0; i < 80; i++) Buf[k*80+i] = ' ';
   return (Buf);
}

/* 
 * PURPOSE: Add a comment in the field 'history' or 'comment' of the structure 
 *    FITSSTRUCT. It begins at new line of 72 characters.
 *
 * INPUT:  comment --- STRING, value of HISTORY or COMMENT field;
 *         type_com --- STRING, "HISTORY" or "COMMENT" to specify in which field the comment
 *                      must be appended.
 * OUTPUT: pfitsbuf --- FITSSTRUCT *, address of the structure FITSSTRUCT holding the FITS Header;
 * RETURN: INT, the length of the added comment.
 *
 *  16-MAR-1998 
 */
int fitsaddhist_com(fitsstruct *pfitsbuf, const char *comment, const char *type_com) {
   int add_len = 0;  /* Length of the comment or history to be added */
   int nb_chars = 0; /* number of characters stored in out */
   int out_nblines = 0;  /* number of lines of the output */
   int add_nblines = 0;  /* number of lines to be added */
   char *out_ptr=NULL; /* pointer to the output */
   int out_size=0;   /* the allocated size, not to be confused with the length! */
   int out_len ;  /* the actual size */
   int status = 0;
   char *Newptr;
   int i;
   
   /* Length of the comment to be added */
   if (comment != NULL) add_len = strlen(comment); 
   else return(0); /* it is finished nothing to add !!! */

   /*--- TYPE of the comment ---*/
   if (strcmp(type_com,"HISTORY") == 0) {
	 out_ptr = pfitsbuf->history;
	 out_size = pfitsbuf->hist_size; 
	 status = 1;
      }

   if (strcmp(type_com,"COMMENT") == 0) {
	 out_ptr = pfitsbuf->comment;
	 out_size = pfitsbuf->com_size; 
	 status = 1;
      }
   
   if (status == 0) {
      printf(" ERROR in fitsaddhist_com: the type of the comment is  %s!\n\n", type_com);
      exit(-1);
   }

   /* find out the length of the field */
   if (out_ptr == NULL) out_len = 0; else out_len = strlen(out_ptr);
  
   //  nb_chars = out_len + add_len;
   out_nblines = out_len / 72;
   if( (out_len % 72) > 0) out_nblines ++;
   nb_chars = (out_len % 72) ;
   add_nblines = add_len / 72;
   if( (add_len % 72) > 0) add_nblines ++;
   out_nblines += add_nblines;

   /* Memory reallocation if there is not enough memory for adding the comment */
   if (out_nblines*72 +1  > out_size) {
	 //  Newptr = new char [out_nblines*72 +1  ];
	 Newptr = (char*) malloc( (out_nblines*72 +1  ) *sizeof(char));
	 if (Newptr == NULL) {
	       std::cerr << "Error: unable to allocate memory : "  << std::endl;
	       exit(-1);
	    }
	 memset(Newptr,'\0', (out_nblines*72 +1));
	 strncpy(Newptr, out_ptr, out_size);
	 // Newptr[out_size] = '\0';
	 free ((char *) out_ptr);
	 out_ptr = Newptr;
	 out_size = out_nblines*72 +1;
	 if (DEBUH) std::cout << "Memory reallocation = " << out_size << std::endl;
	 if (DEBUH) std::cout << " out_ptr = " << out_ptr <<"X" << std::endl;
      }
   if (DEBUH) std::cout << " nb_chars="  << nb_chars << std::endl;
   if (nb_chars > 0) {
      for(i = nb_chars; i < 72; i++) {
	 strcat(out_ptr, " ");
      }
   }
   /* Now add the comment */
   strcat(out_ptr, comment);
  

   if (DEBUH) std::cout << " out_ptr = " << out_ptr  << std::endl;

   /* update of the input structure */
   if ( strcmp(type_com,"COMMENT") == 0 ) 
      {
	 pfitsbuf->comment = out_ptr ;
	 pfitsbuf->com_size = out_size; 
      }
   else
      {
	 pfitsbuf->history = out_ptr ;
	 pfitsbuf->hist_size = out_size; 
      }
   return add_len;

}

/* 
 * PURPOSE: add in the field 'fitshead' the value contained in the fields 'history' & 'comment'.
 *          The header size referred by 'pfitsbuf->fitsheadsize' is updated.
 *
 * INPUTS: pfitsbuf --- POINTER to FITSSTRUCT, the fits header to update
 *
 *
 * RETURN: INT, the size of 'fitshead'.
 *
 */ 
int fitswritehist(fitsstruct *pfitsbuf) {
   char    *outptr, *ptr, *curptr;
   int     len, lenhist, lencom,size,i;

	

   size = pfitsbuf->fitsheadsize;
   outptr = (char *) calloc(size+1 ,sizeof(char)); // v3.4

   /* copy all the fields, except HISTORY & COMMENT fields, of the fits header in the new string */
   ptr = pfitsbuf->fitshead;
   curptr = outptr;
   while (strncmp(ptr,"END",3) != 0) {
      if ((strncmp(ptr,"HISTORY",7) != 0) && (strncmp(ptr,"COMMENT",7) != 0)){
	 strncpy(curptr,ptr,80);
	 curptr +=80;
      }
      ptr +=80;
   }
   *curptr='\0';


   /* is there enough memory for adding 'history' & 'comment' fields of the FITSSTRUCT structure? */
   lenhist = (pfitsbuf->history != NULL?strlen(pfitsbuf->history):0);
   lencom = (pfitsbuf->comment != NULL? strlen(pfitsbuf->comment) :0);
   len =  lenhist + ((lenhist / 72)+1)*8 + lencom + ((lencom / 72)+1)*8 + strlen(outptr) + 80*sizeof(char);
   if (len >= FBSIZE) {
      size = FBSIZE*(len/FBSIZE + 1);
      outptr = (char *) realloc( (void *) outptr, (size_t) sizeof(char)*(size+1) );
      curptr = outptr + strlen(outptr);
   }
	

   /* Add in 'fitshead' field the 'history' & 'comment' fields of the FITSSTRUCT structure */
   len = (pfitsbuf->history != NULL?strlen(pfitsbuf->history):0);
   ptr = pfitsbuf->history;
   i=0;
   while(i<len){
      if( ptr + 72 > pfitsbuf->history + pfitsbuf->hist_size ) 
	 printf(" warning fitswritehist hist_size=%d \n",pfitsbuf->hist_size );
      sprintf(curptr,"%-8.8s%-72.72s","HISTORY ",ptr);
      i += 72;
      ptr += 72;
      curptr += 80;
   }


   len = (pfitsbuf->comment != NULL? strlen(pfitsbuf->comment) :0);
   ptr = pfitsbuf->comment;
   i=0;
   // printf(" pfitsbuf->com_size=%d len=%d\n", pfitsbuf->com_size, len);
   while(i<len){
      // printf(" strlen=%d \n", strlen(ptr));
      if( ptr +72 > pfitsbuf->comment + pfitsbuf->com_size ) 
	 printf(" warning fitswritehist com_size=%d \n",pfitsbuf->com_size );
      sprintf(curptr,"%-8.8s%-72.72s","COMMENT ",ptr);
      // left centered, 8 is the minimum number of caracters
      //  .8 is the maximum number of carac.
	  
      i += 72;
      ptr += 72;
      curptr += 80;
   }

   /* Add the 'END' keyword */
   sprintf(curptr,"%-80s","END");
   len = (size - strlen(outptr))/80;
	
   for (i=0, curptr += 80;i<len;i++,curptr += 80) 
      sprintf(curptr,"%-80s","  ");



   /* Update the  header and its size */
   pfitsbuf->fitshead = outptr;
   pfitsbuf->fitsheadsize = size;
	
			
   return size; 
  
}

/* 
 * PURPOSE: If the keyword 'keyword' doesn't exist, a fits line containing this keyword is created & added to
 *          the STRING referred by 'add_fitsbuf'. The header size referred by pfitsheadsize
 *
 * INPUTS: keyword --- STRING, the name of the keyword to update or to add
 *         comment --- STRING, value to put in the field 'keyword'
 *
 * OUTPUTS: add_fitsbuf   --- STRING POINTER, the address of the fits header string to update
 *          pfitsheadsize --- POINTER, address of the header size
 *
 * RETURN: INT, the line number in the fits header where the keyword has been added.
 *
 * CAUTION: doesn't work for creating multiple fits lines field like HISTORY or COMMENT fields.
 *
 */
int fitsadd(char **add_fitsbuf, const char *keyword, char *comment, int *pfitsheadsize) {
   char    *key_ptr;
   int     headpos, nkey;
   
   if ((headpos = fitsfind(*add_fitsbuf, keyword)) < 0) {
      headpos = fitsfind(*add_fitsbuf, "END     ");
      nkey = *pfitsheadsize / 80;
      if (nkey - headpos < 4) {
	 *pfitsheadsize += FBSIZE;
	 *add_fitsbuf = (char *) realloc((char *) *add_fitsbuf, (size_t) *pfitsheadsize );
	 memset(*add_fitsbuf+*pfitsheadsize-FBSIZE,'\0', FBSIZE);  // v3.4
      }
      
      // Go to the 'END' line and copy it to the next line
      key_ptr = *add_fitsbuf + 80*headpos;
      sprintf(key_ptr, "%-8.8s=                      / %-47.47s", keyword, comment?comment:" ");
      sprintf(key_ptr + 80, "%-80s","END");
   }

   return headpos;
}

/* 
 * PURPOSE: Update the value (enclosed in 'ptr') of the keyword 'keyword' in the STRING referred by 'fitsbuf'.
 *          If the keyword 'keyword' doesn't exist, a fits line containing this keyword is created & added to
 *          the STRING referred by 'fitsbuf'. The header size referred by pfitsheadsize
 *
 * INPUTS: keyword --- STRING, the name of the keyword to update or to add
 *         ptr     --- POINTER, address of the value to put in the field 'keyword'
 *         type    --- H_TYPE, type of the value referred by 'ptr'
 *         t_type  --- TYPE_DATA, secondary type of the value referred by 'ptr'
 *
 * OUTPUTS: fitsbuf --- STRING, the fits header to update
 *          pfitsheadsize --- POINTER, address of the header size
 *
 * RETURN: INT, SUCCESS if no problems, FAILURE if an error occures.
 *
 * CAUTION: doesn't work for updating or creating multiple fits lines field like HISTORY or COMMENT fields.
 *
 */ 
int fitswrite(char *fitsbuf, const char *keyword, void *ptr, h_type type, type_data t_type, int *pfitsheadsize) {
   int i, l, pos;
   char str[70];
   char *pc = (char *) ptr;

   if ((pos = fitsfind(fitsbuf, keyword)) < 0)
      fitsadd(&fitsbuf,keyword,NULL,pfitsheadsize);

   switch(type) {
   case H_INT:	sprintf(str, "%20d", (t_type==T_SHORT)?
			*(short *)ptr: *(int *)ptr);
      break;

   case H_FLOAT:	sprintf(str, "        %12.4f", (t_type==T_DOUBLE)?
				*(double *)ptr: *(float *)ptr);
      break;

   case H_EXPO:	sprintf(str, "    %16.9e", (t_type==T_DOUBLE)?
			*(double *)ptr: *(float *)ptr);
      break;
   case H_BOOL:	if (*(int *)ptr)
      sprintf(str, "                   T");
   else
      sprintf(str, "                   F");
      break;
   case H_STRING:	l = strlen(pc);
      for (i=0; i < l; i++) str[i] = pc[i];
      for (i=l; i < 69; i++) str[i] = ' ';
      str[69] = '\0';
      break;
   case H_COMMENT:	sprintf(str, "%69s", (char *)ptr);
      break;
   default:		error(EXIT_FAILURE,
			      "*FATAL ERROR*: Unknown FITS type in ",
			      "fitswrite()");
      break;
   }

   if ((pos = fitsfind(fitsbuf, keyword)) < 0) {
      return FAILURE;
   }  else  {
     
      fitsbuf += 80*pos+10;
      i = 0;
      while (str[i] != '\0') {
	 fitsbuf[i] = str[i];
	 i++;
      }
   }
  
   return SUCCESS;
}

/* 
 * PURPOSE: Update the fields 'fitshead' and 'fitsheadsize' of a FITSSTRUCT structure from the values 
 * of other fields of the FITSSTRUCT structure.
 *
 * INPUT: *field --- FITSSTRUCT, a fits header structure
 *
 */
int updatefitsheader(fitsstruct *field) {
   int status;
   // extern softinfo Soft;
   
   if (DEBUH) std::cout << "updatefitsheader entry " << std::endl;
   
   if (field->fitsheadsize < FBSIZE) field->fitsheadsize=FBSIZE;
   if (field->fitshead == NULL)
      {
	 field->fitshead = creafitsheader();
	 field->fitsheadsize=FBSIZE;
      }
   fitswrite(field->fitshead, "NAXIS   ", &(field->naxis), H_INT, T_INT,&(field->fitsheadsize));
	

   if (field->naxis > 0)
      fitswrite(field->fitshead, "NAXIS1  ", &(field->width), H_INT, T_INT,&(field->fitsheadsize));

   if (field->naxis > 1)
      fitswrite(field->fitshead, "NAXIS2  ", &(field->height), H_INT, T_INT,&(field->fitsheadsize));

   if (field->naxis > 2) 
      fitswrite(field->fitshead, "NAXIS3  ", &((field->TabAxis)[2]), H_INT, T_INT,&(field->fitsheadsize));
   fitswrite(field->fitshead, "BITPIX  ", &(field->bitpix), H_INT, T_INT,&(field->fitsheadsize));
   if (((field->bzero != 0.) || (field->bscale != 1.)) && (field->bscale != 0.))
      {
	 fitswrite(field->fitshead, "BSCALE  ",&(field->bscale), H_EXPO, T_DOUBLE,&(field->fitsheadsize));
      }
   if (FITSABS(field->bzero) > FLOAT_EPSILON)
      fitswrite(field->fitshead, "BZERO  ", &(field->bzero), H_EXPO, T_DOUBLE,&(field->fitsheadsize));

   if (FITSABS(field->crpixx)  > FLOAT_EPSILON)
      fitswrite(field->fitshead, "CRPIX1  ", &(field->crpixx), H_EXPO, T_DOUBLE,&(field->fitsheadsize));
   if (FITSABS(field->crpixy) > FLOAT_EPSILON)
      fitswrite(field->fitshead, "CRPIX2  ", &(field->crpixy), H_EXPO, T_DOUBLE,&(field->fitsheadsize));
   if (FITSABS((*field).TabRef[2]) > FLOAT_EPSILON)
      fitswrite(field->fitshead, "CRPIX3  ", &((field->TabRef)[2]), H_EXPO, T_DOUBLE,&(field->fitsheadsize));

   if (FITSABS(field->crvalx) > FLOAT_EPSILON)
      fitswrite(field->fitshead, "CRVAL1  ", &(field->crvalx), H_EXPO, T_DOUBLE,&(field->fitsheadsize));
   if (FITSABS(field->crvaly) > FLOAT_EPSILON)
      fitswrite(field->fitshead, "CRVAL2  ", &(field->crvaly), H_EXPO, T_DOUBLE,&(field->fitsheadsize));
   if (FITSABS((*field).TabValRef[2]) > FLOAT_EPSILON)
      fitswrite(field->fitshead, "CRVAL3  ", &((field->TabValRef)[2]), H_EXPO, T_DOUBLE,&(field->fitsheadsize));

   if (FITSABS(field->cdeltx) > FLOAT_EPSILON)
      fitswrite(field->fitshead, "CDELT1  ", &(field->cdeltx), H_EXPO, T_DOUBLE,&(field->fitsheadsize));
   if (FITSABS(field->cdelty) > FLOAT_EPSILON)
      fitswrite(field->fitshead, "CDELT2  ", &(field->cdelty), H_EXPO, T_DOUBLE,&(field->fitsheadsize));
   if (FITSABS((*field).TabStep[2]) > FLOAT_EPSILON)
      fitswrite(field->fitshead, "CDELT3  ", &((field->TabStep)[2]), H_EXPO, T_DOUBLE,&(field->fitsheadsize));
 
   if ((strlen(field->ctypex) > 1) && (strlen(field->ctypey) > 1)) {
      fitswrite(field->fitshead, "CTYPE1  ", 
		(void*)  "          ", H_STRING, T_INT,&(field->fitsheadsize));
      fitswrite(field->fitshead, "CTYPE1  ", 
		field->ctypex, H_STRING, T_INT,&(field->fitsheadsize));
      fitswrite(field->fitshead, "CTYPE2  ", 
		(void*)  "          ", H_STRING, T_INT,&(field->fitsheadsize));
      fitswrite(field->fitshead, "CTYPE2  ", 
		field->ctypey, H_STRING, T_INT,&(field->fitsheadsize));
   }

   //if (DEBUH) std::cout <<  Soft.banner() << std::endl;
   /* Add the soft copyright in the history field of the FITSSTRUCT structure */
   //	status = fitsaddhist_com(field,Soft.banner(),"HISTORY");

   if (DEBUH) std::cout << " field->origin = " << field->origin << std::endl;
   if (strlen(field->origin)  > 1) 
      {
	 status = fitsaddhist_com(field,field->origin,"HISTORY"); 
	 field->origin = "";
      }

   if (DEBUH) std::cout << " field->rident = " << field->rident << std::endl;

   if (strlen(field->rident)  > 1) 
      status = fitsaddhist_com(field,field->rident,"HISTORY"); 
   if (DEBUH) std::cout << "5" << std::endl;

   status = fitswritehist(field);

   field->bytepix = FITSABS(field->bitpix) / 8;
   if (DEBUH) std::cout << "END" << std::endl;

   return status;
}

/*
 * PURPOSE: Write down on the disk the FITS header enclosed in the field 'fitshead' of a FITSSTRUCT
 *    structure. Before writing down on the disk, one must update the header from the fields of the FITSSTRUCT
 *    structure.
 *
 * INPUT: *Header --- FITSSTRUCT, fits header structure
 *
 */ 
void writeimagehead(fitsstruct *Header) {

   /* Update the header hold in the field 'fitshead' */
   updatefitsheader(Header);


   /* Write down on the disk */
   QFWRITE(Header->fitshead,Header->fitsheadsize,Header->file,Header->filename);
}

/* read and convert input data stream in float. */
void  writedataf(FILE *file, char *filename, int bitpix, int Nelem, float *ptr, float bscale, float bzero) {
   int i,size=Nelem;
   char *bufdata;
   short *bufs;
   int *bufi;
   float *buff;
   double *bufd;
   float Val;
   long NbrByte = size * (FITSABS(bitpix) / 8);
   long FitsNbrByte = FBSIZE - (int) (NbrByte % FBSIZE);
 
   // std::cout << " ABS(bitpix) = " << FITSABS(bitpix) << " Size = " << size << " NbrByte = " << NbrByte << " FitsNbrByte = " << FitsNbrByte << std::endl;
  
   // if (!(bufdata=new char [FitsNbrByte])) 
   //     error(EXIT_FAILURE, "*Error*: Not enough memory in ",  "writedataf()");
#if BYTESWAPPED == 1
   bufdata = (char*)malloc((size_t)NbrByte);
#else
   if (bitpix != BP_FLOAT)
      bufdata = (char*)malloc((size_t)NbrByte);
   else bufdata = (char *) ptr;
#endif
    
   char *FitsBit = new char [FitsNbrByte];
   for (i = 0; i < FitsNbrByte; i++) FitsBit[i] = 0;
     	       
   bufs = (short *) bufdata;
   bufi = (int *) bufdata;
   buff = (float *) bufdata;
   bufd = (double *) bufdata;

   // std::cout << "writedataf" << std::endl << "BITPIX = " << bitpix  << std::endl;

   switch(bitpix) {
   case BP_BYTE:	for (i=0; i< size; i++) {
      Val = (ptr[i] - bzero) / bscale;
      //if (Val > 127) Val = 127;
      //if (Val < -128) Val = -128;
      bufdata[i] = (char) Val;
   }
      break;
   case BP_SHORT:
      for (i=0; i< size; i++) {
	 Val = (ptr[i] - bzero) / bscale;
	 if (Val > 0) bufs[i] = (int) (Val+0.5);
	 else  bufs[i] = (int) (Val-0.5);
      }
      /* the 3 following lines come from CFITSIO */
#if BYTESWAPPED == 1
      ffswap2((short int *)bufdata, size); /* reverse order of bytes in each value */
#endif
      break;
   case BP_INT:
      for (i=0; i< size; i++) {
	 Val = (float)((double)(ptr[i] - bzero) / bscale);
	 if (Val > 0) bufi[i] = (int) (Val+0.5);
	 else  bufi[i] = (int) (Val-0.5);
      }
      /* the 3 following lines come from CFITSIO */
#if BYTESWAPPED == 1
      ffswap4((INT32BIT *) bufdata, size); /* reverse order of bytes in each value */
#endif
      break;
   case BP_FLOAT:
      for (i=0; i< size; i++)
	 buff[i] = (ptr[i] - bzero) / bscale;
#if BYTESWAPPED == 1
      ffswap4((INT32BIT *) bufdata, size); /* reverse order of bytes in each value */
#endif
      break;
   case BP_DOUBLE:
      for (i=0; i< size; i++)
	 bufd[i] = (double)((ptr[i] - bzero) / bscale);
#if BYTESWAPPED == 1
      ffswap8((double *)bufdata, size); /* reverse order of bytes in each value */
#endif
      break;
   default: error(EXIT_FAILURE,
		  "*FATAL ERROR*: unknown BITPIX type in ",
		  "writedata()");
      break;
   }
   //  std::cout << "Nelem = " << Nelem << std::endl;
   //  std::cout << "bitpix = " << bitpix << std::endl;
   //  std::cout << "NbrByte = " << NbrByte << std::endl;
  
   QFWRITE(bufdata, NbrByte,file, filename);
   QFWRITE(FitsBit, FitsNbrByte,file, filename);
  
#if BYTESWAPPED == 1
   free((char *) bufdata);
#else
   if (bitpix != BP_FLOAT) free((char *) bufdata);
#endif
  
   delete [] FitsBit;
   // delete bufdata;
}

void  writedataf(fitsstruct *field, float *ptr) {
   // std::cout << "filename = " << field->filename << std::endl;
   // std::cout << "npix = " << field->npix << std::endl;
   // std::cout << "bscale = " << field->bscale << std::endl;
   // std::cout << "bzero = " << field->bzero << std::endl;
   // std::cout << "bitpix = " << field->bitpix << std::endl;

   writedataf(field->file, field->filename, field->bitpix,field->npix, ptr, (float)field->bscale, (float)field->bzero);
}

/*
 * Search for a FITS keyword in a fits header of nblock blocks.
 */
char *fitsnfind(char *fitsbuf, const char *str, int nblock) {
   int i;

   for (i=0;i<36*nblock;i++)
      if (!strncmp(&fitsbuf[80*i], str, strlen(str)))
	 return &fitsbuf[80*i];

   return (char *)NULL;
}

/* 
 * read data from the FITS-file header 
 */
char  *readfitshead(FILE *file, char *filename, int *nblock) {
   int     n;
   char    *buf;

   if (!(buf= new char[FBSIZE]))
      error(EXIT_FAILURE, "*Error*: Not enough memory in ", "readfitshead()");

   /*Find the number of FITS blocks of the header while reading it */
   QFREAD(buf, FBSIZE, file, filename);

   if ( strncmp(buf, "SIMPLE  ", 8) && strncmp(buf, "XTENSION", 8) )
      error(EXIT_FAILURE, filename, " is NOT a FITS file!");

   for (n=1; !fitsnfind(buf,"END     ", n); n++) {
      if (!(buf=(char *)realloc(buf, (size_t)(FBSIZE*(n+1)))))
	 error(EXIT_FAILURE, "*Error*: Not enough memory in ", "readfitshead()");
      QFREAD(&buf[FBSIZE*n], FBSIZE, file, filename);
   }
   *nblock = n;
   return  buf;
}

/*
 * Extract some data from the FITS-file header.
 */
void readimagehead(fitsstruct *field) {
   int		i, n;
   char		*buf, st[80], *point;

   buf = readfitshead(field->file, field->filename, &n);
   field->naxis = FITSTOI("NAXIS   ", 0);
   if(field->naxis < 1)
      error(EXIT_FAILURE, field->filename, " NAXIS is not equal to 1,2, or 3 !");

   field->bitpix = FITSTOI("BITPIX  ", 0);
   if (field->bitpix != BP_BYTE
       && field->bitpix != BP_SHORT
       && field->bitpix != BP_INT
       && field->bitpix != BP_FLOAT
       && field->bitpix != BP_DOUBLE)
      error(EXIT_FAILURE, "Sorry, I don't know that kind of data.", "");

   field->bitpix = FITSTOI("BITPIX  ", 0);
   if (field->bitpix != BP_BYTE
       && field->bitpix != BP_SHORT
       && field->bitpix != BP_INT
       && field->bitpix != BP_FLOAT
       && field->bitpix != BP_DOUBLE)
      error(EXIT_FAILURE, "Sorry, I don't know that kind of data.", "");

   field->bytepix = (field->bitpix>0?field->bitpix:-field->bitpix)>>3;
   if (field->naxis > 0) {
      field->width =  FITSTOI("NAXIS1  ", 0);
      field->crpixx = FITSTOF("CRPIX1  ", 0.);
      field->crvalx = FITSTOF("CRVAL1  ", 0.0);
      field->cdeltx = FITSTOF("CDELT1  ", 0.0);
      (field->TabAxis)[0] = field->width;
      (field->TabRef)[0] = field->crpixx;
      (field->TabValRef)[0] = field->crvalx;
      (field->TabStep)[0] = field->cdeltx;
      field->npix = field->width;
      field->crotax = FITSTOF("CROTA1  ", 0.0);
      FITSTOS("CTYPE1 ", field->ctypex, "       ");
   }
   if (field->naxis > 1)  {
      field->height = FITSTOI("NAXIS2  ", 0);
      field->crpixy = FITSTOF("CRPIX2  ", 0.);
      field->crvaly = FITSTOF("CRVAL2  ", 0.0);
      field->cdelty = FITSTOF("CDELT2  ", 0.0);
      (field->TabAxis)[1] = field->height;
      (field->TabRef)[1] = field->crpixy;
      (field->TabValRef)[1] = field->crvaly;
      (field->TabStep)[1] = field->cdelty;
      field->npix = field->width*field->height;
      field->crotay = FITSTOF("CROTA2  ", 0.0);
      FITSTOS("CTYPE2 ", field->ctypey, "       ");
   }
   if (field->naxis > 2) {
      (field->TabAxis)[2] = FITSTOI("NAXIS3  ", 0);
      (field->TabRef)[2] = FITSTOF("CRPIX3  ", 0.);
      (field->TabValRef)[2] = FITSTOF("CRVAL3  ", 0.0);
      (field->TabStep)[2] = FITSTOF("CDELT3  ", 0.0);
      field->npix = field->npix*(field->TabAxis)[2];
   }

   field->bscale = FITSTOF("BSCALE  ", 1.0);
   field->bzero = FITSTOF("BZERO   ", 0.0);
   field->epoch = FITSTOF("EPOCH   ", 0.0);

   //FITSTOS("OBJECT  ", field->ident, "Unknown");

   field->fitshead = buf;
   field->fitsheadsize = n*FBSIZE;
   field->origin = "\0";
   strcpy(field->rident, "");

   /* HISTORY & COMMENT fields */
   //field->hist_size = fitsreadhist(field->fitshead,field->fitsheadsize,"HISTORY",&field->history);
   //field->com_size = fitsreadhist(field->fitshead,field->fitsheadsize,"COMMENT",&field->comment);

}

/*
 * read and convert input data stream in float.
 */
void  readdataf(FILE *file, char *filename, int bitpix, int Nelem, float *ptr, float bscale, float bzero) {
   int i,size=Nelem;
   char *bufdata;
   // float *ptrf;
   long NbrByte = size * (FITSABS(bitpix) / 8);

   //if (!(bufdata=new char [NbrByte]))
   //     error(EXIT_FAILURE, "*Error*: Not enough memory in ",  "readdata()");
   if (bitpix != BP_FLOAT)
      bufdata = (char*)malloc((size_t)NbrByte);
   else bufdata = (char *) ptr;
   QFREAD(bufdata, NbrByte, file, filename);
 
   /*    
	 #if BYTESWAPPED == 1
	 std::cout << "MACHINE = " << MACHINE <<  std::endl;
	 std::cout << "BYTESWAPPED = True" << std::endl;
	 #endif
   */
   switch(bitpix) {
   case BP_BYTE: 
      for (i=0; i< size; i++) ptr[i] = ((unsigned char *)bufdata)[i]*bscale+bzero;
      break;
     
   case BP_SHORT:    
      /* the 3 following lines come from CFITSIO */
#if BYTESWAPPED == 1
      ffswap2((short int *)bufdata, size);    /* reverse order of bytes in each bufdata */
#endif
     
      for (i=0; i< size; i++)
	 ptr[i] = ((short int *)bufdata)[i]*bscale+bzero;
      break;
     
   case BP_INT:      
      /* the 3 following lines come from CFITSIO */
#if BYTESWAPPED == 1
      ffswap4((INT32BIT *) bufdata, size);  /* reverse order of bytes in each value */
#endif
      for (i=0; i<size; i++)
	 ptr[i] = ((long int *)bufdata)[i]*bscale+bzero;
      break;
   case BP_FLOAT:
#if BYTESWAPPED == 1
      ffswap4((INT32BIT *) bufdata, size); /* reverse order of bytes in each value */
#endif
      for (i=0; i<size; i++) {
	 ptr[i] = ((float *) bufdata)[i]*bscale+bzero;
      }
      break;
   case BP_DOUBLE:
#if BYTESWAPPED == 1
      ffswap8((double *)bufdata, size);   /* reverse order of bytes in each value */
#endif
      for (i=0; i<size; i++)
	 ptr[i] = (float)(((double *)bufdata)[i]*bscale+bzero);
      break;
     
   default: error(EXIT_FAILURE,
		  "*FATAL ERROR*: unknown BITPIX type in ",
		  "readdata()");
      break;
   }
   if (bitpix != BP_FLOAT) free((char *) bufdata);
   // delete bufdata;
   return;
}

/*
 */
void  readdataf(fitsstruct *field, float *ptr) {
   readdataf(field->file, field->filename, field->bitpix,field->npix, ptr, (float)field->bscale, (float)field->bzero);
}

#endif
