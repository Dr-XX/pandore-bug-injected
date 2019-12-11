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

#define JPEG_INTERNALS
#include "jpeg/jerror.c"
#include "jpeg/jutils.c"
#include "jpeg/jcapimin.c"
#include "jpeg/jcomapi.c"
#include "jpeg/jcapistd.c"
#include "jpeg/jdatadst.c"
#include "jpeg/jcinit.c"
#include "jpeg/jcparam.c"
#include "jpeg/jccolor.c"
#include "jpeg/jcsample.c"
#include "jpeg/jmemmgr.c"
#include "jpeg/jaricom.c"
#include "jpeg/jcarith.c"
#include "jpeg/jchuff.c"
#include "jpeg/jcdctmgr.c"
#include "jpeg/jcprepct.c"
#include "jpeg/jcmarker.c"
#include "jpeg/jfdctint.c"
#include "jpeg/jcmainct.c"
#include "jpeg/jccoefct.c"
#include "jpeg/jfdctflt.c"
#include "jpeg/jfdctfst.c"
#include "jpeg/jcmaster.c"
#include "jpeg/jmemnobs.c"
#include "jpeg/cjpeg.c"

Errc PPan2Jpeg( const Img2duc &ims, char *fich, float quality ) {
   FILE *dfd;
  
   if (quality < 0 || quality > 1) {
      fprintf(stderr,"Error ppan2jpeg: Bad quality value [0..1]: %f\n", quality);
      Exit(FAILURE);
   }

   if ((fich == NULL) || (!strcmp(fich, "-"))) {
      dfd = stdout;
   } else if  (!(dfd = fopen(fich, "wb"))) {
      fprintf(stderr, "Error ppan2jpeg: Cannot create JPEG file %s\n", fich);
      Exit(FAILURE);
   }
   Pan2Jpeg(dfd, ims.Vector(), ims.Width(), ims.Height(), ims.Bands(), (int)(100*quality));
   fclose(dfd);
   return SUCCESS;
}

Errc PPan2Jpeg( const Imc2duc &ims, char *fich, float quality ) {
   FILE *dfd;
   if (quality < 0 || quality > 1) {
      fprintf(stderr, "Error ppan2jpeg: Bad quality value [0..1]: %f\n", quality);
      Exit(FAILURE);
   }
   if ((fich == NULL) || (!strcmp(fich, "-"))) {
      dfd = stdout;
   } else if  (!(dfd = fopen(fich,"wb"))) {
      fprintf(stderr,"Error ppan2jpeg: Cannot create JPEG file %s\n",fich);
      Exit(FAILURE);
   }
   Pan2Jpeg(dfd, ims.Vector(), ims.Width(), ims.Height(), ims.Bands(), (int)(100 * quality));

   fclose(dfd);
   return SUCCESS;
}

#ifdef MAIN

/*
 * Modify only the following constants, and the function call.
 */
#define	USAGE	"usage: %s quality [im_in|-] [im_out|-]"
#define	PARC	1
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
      Img2duc* const ims = (Img2duc*)objs[0];

      result = PPan2Jpeg(*ims, (argc > 2 + PARC)? argv[2 + PARC] : NULL, (float)atof(parv[0]));
   } break;
   case Po_Imc2duc :{
      Imc2duc* const ims = (Imc2duc*)objs[0];

      result = PPan2Jpeg(*ims, (argc > 2 + PARC)? argv[2 + PARC] : NULL, (float)atof(parv[0]));
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
