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

#ifdef _MSC_VER
#undef MAXCHAR
#undef MINCHAR
#undef MINSHORT
#undef MAXSHORT
#undef MINLONG
#undef MAXLONG
#endif

extern "C" {
#include "png/adler32.c"
#include "png/gzlib.c"
#include "png/inflate.c"
#include "png/zutil.c"
#include "png/trees.c"
#include "png/gzclose.c"
#include "png/inffast.c"
#include "png/compress.c"
#include "png/gzread.c"
#include "png/inftrees.c"
#include "png/crc32.c"
#include "png/gzwrite.c"
#include "png/deflate.c"
#include "png/infback.c"
#include "png/uncompr.c"

#include "png/pngpread.c"
#include "png/pngset.c"
#include "png/pngwtran.c"
#include "png/pngmem.c"
#include "png/pngrutil.c"
#include "png/pngwrite.c"
#include "png/pngread.c"
#include "png/pngwutil.c"
#include "png/pngerror.c"
#include "png/pngrio.c"
#include "png/pngtrans.c"
#include "png/pngget.c"
#include "png/pngrtran.c"
#include "png/pngwio.c"
#include "png/png.c"
#include "png/dpng.c"
}

bool isPNG( FILE *fp ) {
   int a = fgetc(fp); // skip
   a = fgetc(fp);
   int b = fgetc(fp);
   int c = fgetc(fp);
   return (a == 'P' && b == 'N' && c == 'G');
}

Errc PPng2Pan( FILE *fp, Pobject **objout ) {
   Uchar * data;
   unsigned long w, h;
   unsigned long bands;

   if (decompressPNG(fp, &data, &w, &h, &bands)) {
      if (bands == 1) {
	 *objout =  new Img2duc(h, w, data);
      } else {
	 *objout =  new Imc2duc(h, w, data);
      }
      return SUCCESS;
   }
   return FAILURE;
}

#ifdef MAIN
	
/*
 * Modify only the following constants, and the function call.
 */
#define	USAGE	"usage: %s im_in [im_out|-]"
#define	PARC	1
#define	FINC	0
#define	FOUTC	1
#undef MASK
#define	MASK	0

int main( int argc, char *argv[] ) {
   Errc result;                // The result code of the execution.
   Pobject* mask;              // The region mask.
   Pobject* objin[FINC + 1];   // The input objects.
   Pobject* objs[FINC + 1];    // The source objects masked by the mask.
   Pobject* objout[FOUTC + 1]; // The output objects.
   Pobject* objd[FOUTC + 1];   // The result objects of the execution.
   char* parv[PARC + 1];       // The input parameters.

   char* nomfichier;
   FILE* fdin = stdin;

   if ((argc == 2) && (!strcmp(argv[1], "-p"))) {
      fprintf(stdout,"%s 0 1 1\n",argv[0]);
      exit(0);
   }

   if ((argc == 2) && (!strcmp(argv[1],"-s"))) {
      fprintf(stdout,"\n");
      exit(0);
   }

   ReadArgs(argc, argv, PARC, FINC, FOUTC, &mask, objin, objs, objout, objd, parv, USAGE, MASK);
   // look for input filename.
   nomfichier = (argc > 1)? argv[1] : NULL;
   if (nomfichier && (strcmp(nomfichier,"-"))) {
      if ((fdin = fopen(nomfichier,"rb")) == NULL) {
	 fprintf(stderr,"Error ppn2pan: Cannot open file %s\n", nomfichier);
	 Exit(FAILURE);
      }
   }

   if ((result = PPng2Pan(fdin,&objd[0])) == SUCCESS) {
      WriteArgs(argc, argv, PARC, FINC, FOUTC, &mask, objin, objs, objout, objd, MASK);
   } else {
      fprintf(stderr,"Error ppng2pan: Bad PNG file: %s.\n", nomfichier);
      Exit(FAILURE);
   }

   fclose(fdin);
   
   Exit(result);
   return 0;
}

#endif
