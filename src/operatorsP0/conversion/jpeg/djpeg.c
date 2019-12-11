/*
 * djpeg.c
 *
 * Copyright (C) 1991-1997, Thomas G. Lane.
 * Modified 2009 by Guido Vollbeding.
 * This file is part of the Independent JPEG Group's software.
 * For conditions of distribution and use, see the accompanying README file.
 *
 * This file contains a command-line user interface for the JPEG decompressor.
 * It should work on any system with Unix- or MS-DOS-style command lines.
 *
 * Two different command line styles are permitted, depending on the
 * compile-time switch TWO_FILE_COMMANDLINE:
 *	djpeg [options]  inputfile outputfile
 *	djpeg [options]  [inputfile]
 * In the second style, output is always to standard output, which you'd
 * normally redirect to a file or pipe to some other program.  Input is
 * either from a named file or from standard input (typically redirected).
 * The second style is convenient on Unix but is unhelpful on systems that
 * don't support pipes.  Also, you MUST use the first style if your system
 * doesn't do binary I/O to stdin/stdout.
 * To simplify script writing, the "-outfile" switch is provided.  The syntax
 *	djpeg [options]  -outfile outputfile  inputfile
 * works regardless of which command line style is used.
 */

#include "cdjpeg.h"		/* Common decls for cjpeg/djpeg applications */
#include "jversion.h"		/* for version message */

#include <ctype.h>		/* to declare isprint() */

#ifdef USE_CCOMMAND		/* command-line reader for Macintosh */
#ifdef __MWERKS__
#include <SIOUX.h>              /* Metrowerks needs this */
#include <console.h>		/* ... and this */
#endif
#ifdef THINK_C
#include <console.h>		/* Think declares it here */
#endif
#endif

typedef struct {
  struct djpeg_dest_struct pub;	/* public fields */

/*   boolean is_os2;		/\* saves the OS2 format request flag *\/ */

/*   jvirt_sarray_ptr whole_image;	/\* needed to reverse row order *\/ */
/*   JDIMENSION row_width;		/\* physical width of one row in the BMP file *\/ */
  JDIMENSION cur_output_row;	/* next row# to write to virtual array */
} bmp_dest_struct;

typedef bmp_dest_struct * bmp_dest_ptr;

djpeg_dest_ptr
jinit_write_pan (j_decompress_ptr cinfo) {
  bmp_dest_ptr dest;
  JDIMENSION row_width;

  /* Create module interface object, fill in method pointers */
  dest = (bmp_dest_ptr)(*cinfo->mem->alloc_small) ((j_common_ptr) cinfo, JPOOL_IMAGE,
						   SIZEOF(bmp_dest_struct));
  /* Calculate output image dimensions so we can allocate space */
  jpeg_calc_output_dimensions(cinfo);
  
  /* Determine width of rows in the BMP file (padded to 4-byte boundary). */
  row_width = cinfo->output_width * cinfo->output_components;

  /* Allocate space for inversion array, prepare for write pass */
  dest->cur_output_row = 0;
  if (cinfo->progress != NULL) {
    cd_progress_ptr progress = (cd_progress_ptr) cinfo->progress;
    progress->total_extra_passes++; /* count file input as separate pass */
  }

  /* Create decompressor output buffer. */
  dest->pub.buffer = (*cinfo->mem->alloc_sarray)
    ((j_common_ptr) cinfo, JPOOL_IMAGE, row_width, (JDIMENSION) 1);
  dest->pub.buffer_height = 1;

  return (djpeg_dest_ptr) dest;
}

void
my_error_exit (j_common_ptr cinfo)
{
  /* Always display the message */
  (*cinfo->err->output_message) (cinfo);

  /* Let the memory manager delete any temp files before we die */
  jpeg_destroy(cinfo);
  Exit(FAILURE);
}

boolean
isJPEG( FILE * infile)
{
   int c, c2;
   c=fgetc(infile);
   c2=fgetc(infile);
   if (c != 0xFF || c2 != (int)0xD8) {
      return 0;
   }
   return 1;
}

/*
 * The main program.
 */
int decompressJpeg( FILE * input_file, unsigned char **data,
		    unsigned int * w, unsigned int * h,
		    int *bands ) {
   struct jpeg_decompress_struct cinfo;
   struct jpeg_error_mgr jerr;
   djpeg_dest_ptr dest_mgr = NULL;
   
   /* Initialize the JPEG decompression object with default error handling. */
   cinfo.err = jpeg_std_error(&jerr);
   jerr.error_exit = my_error_exit;
   
   jpeg_create_decompress(&cinfo);

   /* Specify data source for decompression */
   jpeg_stdio_src(&cinfo, input_file);
   
   /* Read file header, set default decompression parameters */
   (void) jpeg_read_header(&cinfo, TRUE);
   
   /* Initialize the output module now to let it override any crucial
    * option settings (for instance, GIF wants to force color quantization).
    */
   dest_mgr = jinit_write_pan(&cinfo);
   
   /* Start decompressor */
   (void)jpeg_start_decompress(&cinfo);
   
   /* Process data */
   if (cinfo.out_color_space == JCS_GRAYSCALE) {
      *bands=1;
   } else {
      *bands=3;
   }
   *w= cinfo.image_width;
   *h= cinfo.image_height;
   *data = (unsigned char *)malloc((*w) * (*h) * *bands);

   while (cinfo.output_scanline < cinfo.output_height) {
      (void)jpeg_read_scanlines(&cinfo, dest_mgr->buffer, dest_mgr->buffer_height);
      bmp_dest_ptr dest = (bmp_dest_ptr)dest_mgr;      
      int row = dest->cur_output_row++;
      
      /* Transfer data.  Note destination values must be in BGR order
       * (even though Microsoft's own documents say the opposite).
       */
      JSAMPROW inptr = dest->pub.buffer[0];
      for ( unsigned int col = 0; col<cinfo.output_width; col++ ) {
	 for ( int b=0; b<*bands; b++ ) {
 	    (*data)[b*(*h**w)+row**w+col] = *inptr++; 
	 }
      }
   }

   /* Finish decompression and release memory.
    * I must do it in this order because output module has allocated memory
    * of lifespan JPOOL_IMAGE; it needs to finish before releasing memory.
    */
   (void)jpeg_finish_decompress(&cinfo);

   jpeg_destroy_decompress(&cinfo);
   return 1;
}
