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
 * @author Regis Clouard - Jun 03, 2012
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
#include "png/cpng.c"

#include <regionlut.h>

Errc PPan2Png( const Img2duc &ims, char *fich ) {
   FILE *dfd;
  
   if ((fich == NULL) || (!strcmp(fich, "-"))) {
      dfd = stdout;
   } else if  (!(dfd = fopen(fich, "wb"))) {
      fprintf(stderr, "Error ppan2png: Cannot create PNG file %s\n", fich);
      Exit(FAILURE);
   }
   Pan2Png(dfd, ims.Vector(), ims.Width(), ims.Height(), ims.Bands());
   fclose(dfd);
   return SUCCESS;
}

Errc PPan2Png( const Imc2duc &ims, char *fich ) {
   FILE *dfd;

   if ((fich == NULL) || (!strcmp(fich, "-"))) {
      dfd = stdout;
   } else if  (!(dfd = fopen(fich, "wb"))) {
      fprintf(stderr, "Error ppan2png: Cannot create PNG file %s\n", fich);
      Exit(FAILURE);
   }
   Pan2Png(dfd, ims.Vector(), ims.Width(), ims.Height(), ims.Bands());
   fclose(dfd);
   return SUCCESS;
}

Errc PPan2Png( const Reg2d &ims, char *fich ) {
   FILE *dfd;

   if ((fich == NULL) || (!strcmp(fich, "-"))) {
      dfd = stdout;
   } else if (!(dfd = fopen(fich, "wb"))) {
	 fprintf(stderr, "Error ppan2png: Cannot create PNG file %s\n", fich);
	 Exit(FAILURE);
   }
   Uchar *data = new Uchar[3*ims.VectorSize()];

   Uchar lut[3][MAXCOLORS];

   buildRegionLut(lut);

   const Ulong *datasrc = ims.Vector();
   unsigned long length  = ims.VectorSize();
   for (Ulong i = 0; i< length; i++ ) {
      Ulong  pixel = datasrc[i]%MAXCOLORS;
      data[i] = lut[0][pixel];
      data[length+i] = lut[1][pixel];
      data[2*length+i] = lut[2][pixel];
   }
   
   Pan2Png(dfd, data, ims.Width(), ims.Height(), 3);
   
   delete[] data;
   
   fclose(dfd);
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
#undef MASK
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

      result=PPan2Png(*ims,(argc>2+PARC)? argv[2+PARC] : NULL);
   }break;
   case Po_Imc2duc :{
      Imc2duc* const ims=(Imc2duc*)objs[0];

      result=PPan2Png(*ims,(argc>2+PARC)? argv[2+PARC] : NULL);
   }break;
   case Po_Reg2d :{
      Reg2d* const ims=(Reg2d*)objs[0];

      result=PPan2Png(*ims,(argc>2+PARC)? argv[2+PARC] : NULL);
   }break;
   default :
      PrintErrorFormat(objin,FINC);
      result = FAILURE;
   }
   
   if (result)
      WriteArgs(argc,argv,PARC,FINC,FOUTC,&mask,objin,objs,objout,objd,MASK);
   Exit(result);
   return 0;
}

#endif
