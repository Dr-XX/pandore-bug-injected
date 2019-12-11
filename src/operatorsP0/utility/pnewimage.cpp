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
 * @author Regis Clouard - 2003-04-30
 */

#include <string.h>
#include <pandore.h>
using namespace pandore;
 
/**
 * @file pnewimage.cpp	
 * Creation of a new image.
 */

#ifdef MAIN

/*
 * Modify only the following constants, and the function call.
 */
#define	USAGE	"usage: %s  width height depth value [im_out|-]"
#define	PARC	4
#define	FINC	0
#define	FOUTC	1
#define	MASK	0

int main( int argc, char *argv[] ) {
   Errc result;              // The result code of the execution.
   Pobject* mask;            // The region map.
   Pobject* objin[FINC+1];   // The input objects.
   Pobject* objs[FINC+1];    // The source objects masked.
   Pobject* objout[FOUTC+1]; // The output object.
   Pobject* objd[FOUTC+1];   // The result object of the execution.
   char* parv[PARC+1];       // The input parameters.

   ReadArgs(argc,argv,PARC,FINC,FOUTC,&mask,objin,objs,objout,objd,parv,USAGE,MASK);
   
   if (atoi(parv[0])<=0) {
      return FAILURE;
   } else {
      if (atoi(parv[1])<=0) {
	 // If parv contains "." it is a float value
	 if (strstr(argv[4],".")) {
	    objd[0]=new Img1dsf((Long)atoi(parv[0]));
	    Img1dsf* const imd=(Img1dsf*)objd[0];
	    *imd=(Float)atof(parv[3]);
	 } else if (atoi(parv[3])> MAXUCHAR) {
	    // It is slong value
	    objd[0]=new Img1dsl((Long)atoi(parv[0]));
	    Img1dsl* const imd=(Img1dsl*)objd[0];
	    *imd=(Long)atoi(parv[3]);
	 } else {
	    // It is uchar value
	    objd[0]=new Img1duc((Long)atoi(parv[0]));
	    Img1duc* const imd=(Img1duc*)objd[0];
	    *imd=(Uchar)atoi(parv[3]);
	 }
	 result=SUCCESS;
      } else {
	 if (atoi(parv[2])<=0) {
	    if (strstr(argv[4],".")) {
	       objd[0]=new Img2dsf((Long)atoi(parv[1]),(Long)atoi(parv[0]));
	       Img2dsf* const imd=(Img2dsf*)objd[0];
	       *imd=(Float)atof(parv[3]);
	    } else if (atoi(parv[3])>MAXUCHAR || atoi(parv[3])<0) {
	       objd[0]=new Img2dsl((Long)atoi(parv[1]),(Long)atoi(parv[0]));
	       Img2dsl* const imd=(Img2dsl*)objd[0];
	       *imd=(Long)atoi(parv[3]);
	    } else {
	       objd[0]=new Img2duc((Long)atoi(parv[1]),(Long)atoi(parv[0]));
	       Img2duc* const imd=(Img2duc*)objd[0];
	       *imd=(Uchar)atoi(parv[3]);
	    }
	    result=SUCCESS;
	 } else {
	    if (strstr(argv[4],".")) {
	       objd[0]=new Img3dsf((Long)atoi(parv[2]),(Long)atoi(parv[1]),(Long)atoi(parv[0]));
	       Img3dsf* const imd=(Img3dsf*)objd[0];
	       *imd=(Float)atoi(parv[3]);
	    } else if (atoi(parv[3])>MAXUCHAR || atoi(parv[3])<0) {
	       objd[0]=new Img3dsl((Long)atoi(parv[2]),(Long)atoi(parv[1]),(Long)atoi(parv[0]));
	       Img3dsl* const imd=(Img3dsl*)objd[0];
	       *imd=(Long)atoi(parv[3]);
	    } else {
	       objd[0]=new Img3duc((Long)atoi(parv[2]),(Long)atoi(parv[1]),(Long)atoi(parv[0]));
	       Img3duc* const imd=(Img3duc*)objd[0];
	       *imd=(Uchar)atoi(parv[3]);
	    }
	    result=SUCCESS;
	 }
      }
   }

   if (result) {
      WriteArgs(argc,argv,PARC,FINC,FOUTC,&mask,objin,objs,objout,objd,MASK);
   }
   
   Exit(result);
   return 0;
}

#endif
