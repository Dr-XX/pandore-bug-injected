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

#if (defined _MSC_VER) && (!defined snprintf)
#define snprintf _snprintf
#endif

#include <time.h>
#include <string.h>
#include <pandore.h>
using namespace pandore;
#include "fitsio.h"

/**
 * @file ppan2fits.cpp
 * Convert Pandore image to FITS image.
 * @param ims the input Pandore image.
 * @param name the name of the fits file.
 */

void command_line(fitsstruct* field, int argc, char* argv[]) {
   char* cmd = new char[512];
  time_t intps = time(NULL);
  
  field->origin=NULL; 
  cmd[0] = '\0';
  for (int k=0; k<argc; k++)  {
     snprintf(cmd, sizeof(cmd), "%s %s", cmd, argv[k]);
     cmd[sizeof(cmd) - 1] = 0;
  }
  fitsaddhist_com(field,cmd,"HISTORY");
  
  cmd[0] = '\0';    
  snprintf(cmd, sizeof(cmd), "Pandore converter Created on %s", ctime(&intps));
  cmd[sizeof(cmd) - 1] = 0;

#ifdef WIN32
  field->origin=_strdup(cmd);
#else
  field->origin=strdup(cmd);
#endif

  delete cmd;
}

Errc PPan2Fits(Img1duc& ims, char *name, int argc, char* argv[]) {
  FILE* f=stdout;
  fitsstruct header;
   
  if (name && (!(f = fopen(name,"wb")))) {
     return FAILURE;
  }
   
  Img1dsf buf(ims.Size());
  buf=ims;
   
  header.bitpix = BP_FLOAT;
  header.width =  ims.Width();
  header.naxis = 1;
  header.npix = ims.VectorSize();
  header.TabAxis[0] = ims.Width();
  header.filename = fitsname(name);
  header.file = fits_file_des_out(name);
  command_line(&header,argc,argv);
  writeimagehead(&header);
  writedataf(&header, (float*)(buf.Vector()));
   
  buf.Delete();
  return SUCCESS;
}

Errc PPan2Fits(Img1dsl& ims, char *name, int argc, char* argv[]) {
  FILE* f=stdout;
  fitsstruct header;
   
  if (name && (!(f = fopen(name,"wb")))) {
    return FAILURE;
  }
   
  Img1dsf buf(ims.Size());
  buf=ims;
   
  header.bitpix = BP_FLOAT;
  header.width =  ims.Width();
  header.naxis = 1;
  header.npix = ims.VectorSize();
  header.TabAxis[0] = ims.Width();
  header.filename = fitsname(name);
  header.file = fits_file_des_out(name);
  command_line(&header,argc,argv);
  writeimagehead(&header);
  writedataf(&header, (float*)(buf.Vector()));
   
  buf.Delete();
  return SUCCESS;
}

Errc PPan2Fits(Img1dsf& ims, char *name, int argc, char* argv[]) {
  FILE* f=stdout;
  fitsstruct header;
   
  if (name && (!(f = fopen(name,"wb")))) {
    return FAILURE;
  }
   
   
  header.bitpix = BP_FLOAT;
  header.width =  ims.Width();
  header.naxis = 1;
  header.npix = ims.VectorSize();
  header.TabAxis[0] = ims.Width();
  header.filename = fitsname(name);
  header.file = fits_file_des_out(name);
  command_line(&header,argc,argv);
  writeimagehead(&header);
  writedataf(&header, (float*)(ims.Vector()));
   
  return SUCCESS;
}

Errc PPan2Fits(Img2duc& ims, char *name, int argc, char* argv[]) {
  FILE* f=stdout;
  fitsstruct header;
   
  if (name && (!(f = fopen(name,"wb")))) {
    return FAILURE;
  }
   
  Img2dsf buf(ims.Size());
  buf=ims;
   
  header.bitpix = BP_FLOAT;
  header.width =  ims.Width();
  header.height =  ims.Height();
  header.naxis = 2;
  header.npix = ims.VectorSize();
  header.TabAxis[0] = ims.Width();
  header.TabAxis[1] = ims.Height();
  header.filename = fitsname(name);
  header.file = fits_file_des_out(name);
  command_line(&header,argc,argv);
  writeimagehead(&header);
  writedataf(&header, (float*)(buf.Vector()));
   
  buf.Delete();
  return SUCCESS;
}

Errc PPan2Fits(Img2dsl& ims, char *name, int argc, char* argv[]) {
  FILE* f=stdout;
  fitsstruct header;
   
  if (name && (!(f = fopen(name,"wb")))) {
    return FAILURE;
  }
   
  Img2dsf buf(ims.Size());
  buf=ims;
   
  header.bitpix = BP_FLOAT;
  header.width =  ims.Width();
  header.height =  ims.Height();
  header.naxis = 2;
  header.npix = ims.VectorSize();
  header.TabAxis[0] = ims.Width();
  header.TabAxis[1] = ims.Height();
  header.filename = fitsname(name);
  header.file = fits_file_des_out(name);
  command_line(&header,argc,argv);
  writeimagehead(&header);
  writedataf(&header, (float*)(buf.Vector()));
   
  buf.Delete();
  return SUCCESS;
}

Errc PPan2Fits(Img2dsf& ims, char *name, int argc, char* argv[]) {
  FILE* f=stdout;
   
  fitsstruct header;
   
  if (name && (!(f = fopen(name,"wb")))) {
    return FAILURE;
  }
      
  header.bitpix = BP_FLOAT;
  header.width =  ims.Width();
  header.height =  ims.Height();
  header.naxis = 2;
  header.npix = ims.VectorSize();
  header.TabAxis[0] = ims.Width();
  header.TabAxis[1] = ims.Height();
  header.filename = fitsname(name);
  header.file = fits_file_des_out(name);
  command_line(&header,argc,argv);
  //printf("%d %d %d %d %d %s %s",header.bitpix,header.width,header.height,header.naxis,header.npix,header.filename,header.origin);
  writeimagehead(&header);
  writedataf(&header, (float*)(ims.Vector()));
   
  return SUCCESS;
}

Errc PPan2Fits(Img3duc& ims, char *name, int argc, char* argv[]) {
  FILE* f=stdout;
  fitsstruct header;
   
  if (name && (!(f = fopen(name,"wb")))) {
    return FAILURE;
  }
   
  Img3dsf buf(ims.Size());
  buf=ims;  
   
  header.bitpix = BP_FLOAT;
  header.width =  ims.Width();
  header.height =  ims.Height();
  header.naxis = 3;
  header.npix = ims.VectorSize();
  header.TabAxis[0] = ims.Width();
  header.TabAxis[1] = ims.Height();
  header.TabAxis[2] = ims.Depth();
  header.filename = fitsname(name);
  header.file = fits_file_des_out(name);
  command_line(&header,argc,argv);
  writeimagehead(&header);
  writedataf(&header, (float*)(buf.Vector()));
   
  buf.Delete();
  return SUCCESS;
}

Errc PPan2Fits(Img3dsl& ims, char *name, int argc, char* argv[]) {
  FILE* f=stdout;
  fitsstruct header;
   
  if (name && (!(f = fopen(name,"wb")))) {
    return FAILURE;
  }
   
  Img3dsf buf(ims.Size());
  buf=ims;  
   
  header.bitpix = BP_FLOAT;
  header.width =  ims.Width();
  header.height =  ims.Height();
  header.naxis = 3;
  header.npix = ims.VectorSize();
  header.TabAxis[0] = ims.Width();
  header.TabAxis[1] = ims.Height();
  header.TabAxis[2] = ims.Depth();
  header.filename = fitsname(name);
  header.file = fits_file_des_out(name);
  command_line(&header,argc,argv);
  writeimagehead(&header);
  writedataf(&header, (float*)(buf.Vector()));
   
  buf.Delete();
  return SUCCESS;
}

Errc PPan2Fits(Img3dsf& ims, char *name, int argc, char* argv[]) {
  FILE* f=stdout;
  fitsstruct header;
   
  if (name && (!(f = fopen(name,"wb")))) {
    return FAILURE;
  }
      
  header.bitpix = BP_FLOAT;
  header.width =  ims.Width();
  header.height =  ims.Height();
  header.naxis = 3;
  header.npix = ims.VectorSize();
  header.TabAxis[0] = ims.Width();
  header.TabAxis[1] = ims.Height();
  header.TabAxis[2] = ims.Depth();
  header.filename = fitsname(name);
  header.file = fits_file_des_out(name);
  command_line(&header,argc,argv);
  writeimagehead(&header);
  writedataf(&header, (float*)(ims.Vector()));
   
  return SUCCESS;
}


#ifdef MAIN
#define USAGE   "USAGE : %s [im_in|-] [im_out|-]"
#define PARC    0
#define FINC    1
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
  
//   if (argc<=2) {
//     fprintf(stderr,USAGE,argv[0]);
//     Exit((Errc)FAILURE);
//   }
   
  ReadArgs(argc,argv,PARC,FINC,FOUTC,&mask,objin,objs,objout,objd,parv,USAGE,MASK);
  if (objs[0]->Type()==Po_Img1duc) {
    Img1duc* const ims=(Img1duc*)objs[0];
   
    if ((argc>2)&&(strcmp(argv[2],"-")))
      result=PPan2Fits(*ims,argv[2],argc,argv);
    else
      result=PPan2Fits(*ims,NULL,argc,argv);
    goto end;
  }
  if (objs[0]->Type()==Po_Img1dsl) {
    Img1dsl* const ims=(Img1dsl*)objs[0];
   
    if ((argc>2)&&(strcmp(argv[2],"-")))
      result=PPan2Fits(*ims,argv[2],argc,argv);
    else
      result=PPan2Fits(*ims,NULL,argc,argv);
    goto end;
  }
  if (objs[0]->Type()==Po_Img1dsf) {
    Img1dsf* const ims=(Img1dsf*)objs[0];
   
    if ((argc>2)&&(strcmp(argv[2],"-")))
      result=PPan2Fits(*ims,argv[2],argc,argv);
    else
      result=PPan2Fits(*ims,NULL,argc,argv);
    goto end;
  }
  if (objs[0]->Type()==Po_Img2duc) {
    Img2duc* const ims=(Img2duc*)objs[0];
   
    if ((argc>2)&&(strcmp(argv[2],"-")))
      result=PPan2Fits(*ims,argv[2],argc,argv);
    else
      result=PPan2Fits(*ims,NULL,argc,argv);
    goto end;
  }
  if (objs[0]->Type()==Po_Img2dsl) {
    Img2dsl* const ims=(Img2dsl*)objs[0];
   
    if ((argc>2)&&(strcmp(argv[2],"-")))
      result=PPan2Fits(*ims,argv[2],argc,argv);
    else
      result=PPan2Fits(*ims,NULL,argc,argv);
    goto end;
  }
  if (objs[0]->Type()==Po_Img2dsf) {
    Img2dsf* const ims=(Img2dsf*)objs[0];
   
    if ((argc>2)&&(strcmp(argv[2],"-")))
      result=PPan2Fits(*ims,argv[2],argc,argv);
    else
      result=PPan2Fits(*ims,NULL,argc,argv);
    goto end;
  }
  if (objs[0]->Type()==Po_Img3duc) {
    Img3duc* const ims=(Img3duc*)objs[0];
   
    if ((argc>2)&&(strcmp(argv[2],"-")))
      result=PPan2Fits(*ims,argv[2],argc,argv);
    else
      result=PPan2Fits(*ims,NULL,argc,argv);
    goto end;
  }
  if (objs[0]->Type()==Po_Img3dsl) {
    Img3dsl* const ims=(Img3dsl*)objs[0];
   
    if ((argc>2)&&(strcmp(argv[2],"-")))
      result=PPan2Fits(*ims,argv[2],argc,argv);
    else
      result=PPan2Fits(*ims,NULL,argc,argv);
    goto end;
  }
  if (objs[0]->Type()==Po_Img3dsf) {
    Img3dsf* const ims=(Img3dsf*)objs[0];
   
    if ((argc>2)&&(strcmp(argv[2],"-")))
      result=PPan2Fits(*ims,argv[2],argc,argv);
    else
      result=PPan2Fits(*ims,NULL,argc,argv);
    goto end;
  }
  {
    PrintErrorFormat(objin,FINC);
    result = FAILURE;
  }		

 end:
  WriteArgs(argc,argv,PARC,FINC,FOUTC,&mask,objin,objs,objout,objd,MASK);
  Exit(result);
  return 0;
}

#endif
