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
 * @author Jalal Fadili - 2005-12-20
 * With courtesy of Jean-Luc Starck (CEA 1996).
 */


/**
 * @file pfits2pan.cpp
 * Convert FITS image to Pandore image.
 */ 
#
#include <string.h>
#include <pandore.h>
using namespace pandore;
#include "fitsio.h"

Errc PFits2Pan( fitsstruct *FitsHeader, Img1dsf &imd ) {
   readdataf(FitsHeader, (float*)imd.Vector());
   return SUCCESS;
}

Errc PFits2Pan( fitsstruct *FitsHeader, Img2dsf &imd ) {
   readdataf(FitsHeader, (float*)imd.Vector());

   return SUCCESS;
}

Errc PFits2Pan( fitsstruct *FitsHeader, Img3dsf &imd ) {
   readdataf(FitsHeader, (float*)imd.Vector());

   return SUCCESS;
}

#ifdef MAIN
#define USAGE   "USAGE : %s [im_in|-] [im_out|-]"
#define PARC    0
#define FINC    0
#define FOUTC   1
#define MASK    0

int main(int argc, char* argv[]) {
   Errc result;              // The result code of the execution.
   Pobject *mask;            // The mask.
   Pobject *objin[FINC+1];   // The input objects.
   Pobject *objs[FINC+1];    // The source objects masked by mask.
   Pobject *objout[FOUTC+1]; // The output object unmasked by mask.
   Pobject *objd[FOUTC+1];   // The result object.
   char *parv[PARC+1];       // The input parameters.

   fitsstruct header;
  
   if ((argc==2) && (!strcmp(argv[1],"-p"))) {
      fprintf(stdout,"%s 0 1 1\n",argv[0]);
      exit(0);
   }
  
   if ((argc==2) && (!strcmp(argv[1],"-s"))) {
      fprintf(stdout,"\n");
      exit(0);
   }
   	
   ReadArgs(argc,argv,PARC,FINC,FOUTC,&mask,objin,objs,objout,objd,parv,USAGE,MASK);  
#ifdef WIN32
   header.filename = _strdup(fitsname(argv[1]));
#else
   header.filename = strdup(fitsname(argv[1]));
#endif
   header.file = fits_file_des_in(argv[1]);
   readimagehead(&header);
   if (header.naxis == 0) {
      std::cerr << "Error pfits2pan: bad NAXIS keywords in " << argv[1] << std::endl;
      exit(0);
   }
   if (header.naxis == 1) {
      objd[0]=new Img1dsf((header.TabAxis)[0]);
      Img1dsf* const imd=(Img1dsf*)objd[0];
      result=PFits2Pan(&header, *imd);
      fclose(header.file);
      goto end;
   }
   if (header.naxis == 2) {
      objd[0]=new Img2dsf(header.height, header.width);
      Img2dsf* const imd=(Img2dsf*)objd[0];
      result=PFits2Pan(&header, *imd);
      fclose(header.file);
      goto end;
   }
   if (header.naxis == 3) {
      //printf("X=%d Y=%d Z=%d bitpix=%d\n",(header.TabAxis)[0],(header.TabAxis)[1],(header.TabAxis)[2],header.bitpix);
      objd[0]=new Img3dsf((header.TabAxis)[2],(header.TabAxis)[0],(header.TabAxis)[1]);
      Img3dsf* const imd=(Img3dsf*)objd[0];
      result=PFits2Pan(&header, *imd);
      fclose(header.file);
      goto end;
   }
   if (header.naxis != 0 && header.naxis != 1 && header.naxis != 2 && header.naxis != 3) {
      std::cerr << "Error pfits2pan: bad NAXIS keywords in " << argv[1] << std::endl;
      std::cerr << "                 NAXIS must between 1 and 3" << std::endl;
      goto end;
   }
      
   {
      PrintErrorFormat(objin,FINC);
      result = FAILURE;
   }	

 end:
   WriteArgs(argc,argv,PARC,FINC+1,FOUTC,&mask,objin,objs,objout,objd,MASK);
   Exit(result);
   return 0;
}

#endif
