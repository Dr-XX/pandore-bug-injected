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
 * @file cpng.c
 * Compression of a PNG image.
 * @author Regis Clouard - Jun 02, 2012
 */

#define _POSIX_SOURCE 1

#include "zlib.h"
#include "png.h"

/* Test one file */
int Pan2Png( FILE* fp, unsigned char *data, int image_width, int image_height,  int bands ) {
   png_structp png_ptr;
   png_infop info_ptr;
   png_colorp palette = NULL;

   png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
   if (png_ptr == NULL) {
      return 0;
   }

   /* Allocate/initialize the image information data.  REQUIRED */
   info_ptr = png_create_info_struct(png_ptr);
   if (info_ptr == NULL) {
      png_destroy_write_struct(&png_ptr,  NULL);
      return 0;
   }
   
   /* Set up the output control if you are using standard C streams */
   png_init_io(png_ptr, fp);

   const int bit_depth=8;
   int color_type;
   if (bands == 3) {
      color_type=PNG_COLOR_TYPE_RGB;
   } else {
      color_type=PNG_COLOR_TYPE_GRAY;
   }

   png_set_IHDR(png_ptr, info_ptr, image_width, image_height, bit_depth,
		color_type, PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_BASE,
		PNG_FILTER_TYPE_BASE);
   
   /* Set the palette if there is one.  REQUIRED for indexed-color images */
   if (bands==3) {
      palette = (png_colorp)png_malloc(png_ptr, PNG_MAX_PALETTE_LENGTH
				       * png_sizeof(png_color));
      
      png_set_PLTE(png_ptr, info_ptr, palette, PNG_MAX_PALETTE_LENGTH);
   }

   /* Optional significant bit (sBIT) chunk */
   png_color_8 sig_bit;

   /* If we are dealing with a grayscale image then */
   sig_bit.gray = 8; 

   /* Otherwise, if we are dealing with a color image then */
   sig_bit.red = 8;
   sig_bit.green = 8;
   sig_bit.blue = 8;

   png_set_sBIT(png_ptr, info_ptr, &sig_bit);

   /* Write the file header information.  REQUIRED */
   png_write_info(png_ptr, info_ptr);

   png_byte *image=(png_byte*)malloc(image_width*bands*image_height*sizeof(png_byte));
   png_bytep *row_pointers = (png_bytep*)malloc(image_height*sizeof(png_bytep));

   if (image_height > (int)(PNG_UINT_32_MAX/png_sizeof(png_bytep))) {
      png_error (png_ptr, "Image is too tall to process in memory");
   }

   for (int r = 0; r < image_height; r++) {
      for (int c = 0; c < image_width; c++) {
	 for (int b=0; b<bands; b++) {
	    image[r*(image_width*bands)+(c*bands)+b]=data[b*(image_width*image_height)+r*image_width+c];
	 }
      }
   }
   for (int k = 0; k < image_height; k++) {
      row_pointers[k] = image + k*image_width*bands;
   }

   /* One of the following output methods is REQUIRED */

   png_write_image(png_ptr, row_pointers);

   /* It is REQUIRED to call this to finish writing the rest of the file */
   png_write_end(png_ptr, info_ptr);

   /* If you png_malloced a palette, free it here (don't free info_ptr->palette,
    * as recommended in versions 1.0.5m and earlier of this example; if
    * libpng mallocs info_ptr->palette, libpng will free it).  If you
    * allocated it with malloc() instead of png_malloc(), use free() instead
    * of png_free().
    */
   if (palette == NULL) {
      png_free(png_ptr, palette);
      palette = NULL;
   }

   /* Similarly, if you png_malloced any data that you passed in with
    * png_set_something(), such as a hist or trans array, free it here,
    * when you can be sure that libpng is through with it.
    */
   free(row_pointers);

   /* Clean up after the write, and free any memory allocated */
   png_destroy_write_struct(&png_ptr, &info_ptr);

   return 1;
}

