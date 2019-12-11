/* -*- mode: c++; c-basic-offset: 3 -*-
 *
 * Copyright (c) 2013, GREYC.
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
 * @file dpng.c
 * Decompression of a PNG image.
 * @author Regis Clouard - Jun 02, 2012
 */

#define _POSIX_SOURCE 1

#include "zlib.h"
#include "png.h"

/* Copied from pngpriv.h but only used in error messages below. */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int decompressPNG( FILE* fpin, unsigned char **data, unsigned long *image_width, unsigned long *image_height,  unsigned long *bands ) {
   png_structp png_ptr;
   png_infop read_info_ptr;
   int alpha = 0;
   png_uint_32 width=0, height = 0;
   int color_type = 0;
   int bit_depth;

   png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);

   read_info_ptr = png_create_info_struct(png_ptr);

   png_init_io(png_ptr, fpin);

   //   png_set_read_status_fn(png_ptr, NULL);

   png_read_info(png_ptr, read_info_ptr);

   png_set_scale_16(png_ptr);

   {
      int interlace_type, compression_type, filter_type;
      
      if (png_get_IHDR(png_ptr, read_info_ptr, &width, &height, &bit_depth, &color_type, &interlace_type, &compression_type, &filter_type)) {
	 if (color_type == PNG_COLOR_TYPE_GRAY && bit_depth < 8) {
	    png_set_expand_gray_1_2_4_to_8(png_ptr);
	 }
	 switch(color_type) {
	    case PNG_COLOR_TYPE_GRAY: {
	       *bands = 1;
	       alpha = 0;
	    } break;
	    case PNG_COLOR_TYPE_RGB: {
	       *bands = 3;
	       alpha = 0;
	    } break;
	    case PNG_COLOR_TYPE_GRAY_ALPHA: {
	       *bands = 1;
	       alpha = 1;
	    } break;
	    case PNG_COLOR_TYPE_RGB_ALPHA: {
	       *bands = 3;
	       alpha = 1;
	    } break;
	    case PNG_COLOR_TYPE_PALETTE: {
	       *bands = 3;
	       alpha = 0;
	       if (bit_depth < 8){
		  png_set_packing(png_ptr); // expand to bytes
	       }
	       png_set_expand(png_ptr);
	    } break;
	    default:
	       printf("Unknown PNG color format: %d\n", color_type);
	       return 0;
	 }
	 
	 *image_width = (unsigned long int)width;
	 *image_height = (unsigned long int)height;
	 *data = (unsigned char*)malloc( *image_width * *image_height * *bands);
      }
   }

#if defined(PNG_READ_INTERLACING_SUPPORTED)
   (void)png_set_interlace_handling(png_ptr);
#endif
   png_read_update_info(png_ptr, read_info_ptr);

   /* The easiest way to read the image: */
   png_bytep *row_pointers;
   row_pointers=(png_bytep*)malloc(height*sizeof(png_bytep));

   for (int row = 0; row < (int)height; row++) {
      row_pointers[row] = (png_byte*)png_malloc(png_ptr, png_get_rowbytes(png_ptr, read_info_ptr));
   }

   png_read_image(png_ptr, row_pointers);

   for (int row = 0; row < (int)height; row++) {
      for (int col=0; col < (int)width; col++) {
	 for (int b=0; b < (int)*bands; b++) {
	    (*data)[b*(width*height)+row*width+col]=row_pointers[row][((*bands+alpha)*col)+b];
	 }
      }
   }

   /* Read rest of file, and get additional chunks in info_ptr - REQUIRED */
   png_read_end(png_ptr, read_info_ptr);

   for (int row = 0; row < (int)height; row++) {
      png_free(png_ptr, row_pointers[row]);
   }

   free(row_pointers);

   /* Clean up after the read, and free any memory allocated - REQUIRED */
   png_destroy_read_struct(&png_ptr, &read_info_ptr, NULL);

   return 1;
}
