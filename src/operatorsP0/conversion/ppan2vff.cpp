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
 * @author Francois Angot - 1995-12-13
 */

#include <string.h>
#include <pandore.h>
using namespace pandore;

/**
 * @file ppan2vff.cpp
 * Convert pandore file to vff file.
 */
Errc PPan2Vff( const Img3duc &ims, char *filename ) {
   Long j,k;
   FILE *fp;
   
   if ((filename == NULL) || (!strcmp(filename, "-"))) {
      fp = stdout;
   } else if ((fp=fopen(filename,"w")) == NULL) {
      fprintf(stderr, "Error ppan2vff: Cannot open %s\n", filename);
      return FAILURE;
   }
   
   fprintf(fp,"ncaa\ntype=raster;\nformat=slice;\nrank=3;\nbands=1;\n");
   fprintf(fp,"bits=8;\nsize=%d %d %d;\n",ims.Width(),ims.Height(),ims.Depth());
   fprintf(fp,"origin=0.0 0.0 0.0;\n");
   fprintf(fp,"extent=%f %f %f;\n",(Float)ims.Width(),(Float)ims.Height(),(Float)ims.Depth());
   fprintf(fp,"aspect=1.0000 1.0000 1.0000;\n");
   fprintf(fp,"\f\n");
   
   for (k=0;k<ims.Depth();k++) {
      for (j=0;j<ims.Height();j++) {
	 fwrite(&ims(k,j,0),sizeof(Uchar),ims.Width(),fp);
      }
   }
   fclose(fp);
   
   return SUCCESS;
}

Errc PPan2Vff( const Img3dsl &ims, char *filename ) {
   Long i,j,k;
   FILE *fp;
   Uchar c;
   Long s,max;
   
   if ((filename==NULL) || (!strcmp(filename,"-"))) {
      fp = stdout;
   } else if ((fp = fopen(filename,"w")) == NULL) {
      fprintf(stderr, "Error ppan2vff: Cannot open %s\n", filename);
      return FAILURE;
   }
   
   fprintf(fp,"ncaa\ntype=raster;\nformat=slice;\nrank=3;\nbands=1;\n");
   fprintf(fp,"bits=8;\nsize=%d %d %d;\n",ims.Width(),ims.Height(),ims.Depth());
   fprintf(fp,"origin=0.0 0.0 0.0;\n");
   fprintf(fp,"extent=%f %f %f;\n",(Float)ims.Width(),(Float)ims.Height(),(Float)ims.Depth());
   fprintf(fp,"aspect=1.0000 1.0000 1.0000;\n");
   fprintf(fp,"\f\n");
   
   max=0;
   for (k=0;k<ims.Depth();k++) {
      for (j=0;j<ims.Height();j++) {
	 for (i=0;i<ims.Width();i++) {
	    s=ims(k,j,i);
	    if (s>max) {
	       max=s;
	    }
	 }
      }
   }
   for (k=0;k<ims.Depth();k++) {
      for (j=0;j<ims.Height();j++) {
	 for (i=0;i<ims.Width();i++) {
	    s=ims(k,j,i)*255;
	    c=(Uchar)(s/max);
	    fwrite(&c,sizeof(Uchar),1,fp);
	 }
      }
   }
   fclose(fp);
   
   return SUCCESS;
}

Errc PPan2Vff( Img3dsf &ims, char *filename ) {
   Long i,j,k;
   FILE *fp;
   Uchar c;
   float f,max;
   
   if ((filename == NULL) || (!strcmp(filename, "-"))) {
      fp = stdout;
   } else if ((fp = fopen(filename, "w")) == NULL) {
      fprintf(stderr, "Error ppan2vff: Cannot open %s\n", filename);
      return FAILURE;
   }
   
   fprintf(fp,"ncaa\ntype=raster;\nformat=slice;\nrank=3;\nbands=1;\n");
   fprintf(fp,"bits=8;\nsize=%d %d %d;\n",ims.Width(),ims.Height(),ims.Depth());
   fprintf(fp,"origin=0.0 0.0 0.0;\n");
   fprintf(fp,"extent=%f %f %f;\n",(Float)ims.Width(),(Float)ims.Height(),(Float)ims.Depth());
   fprintf(fp,"aspect=1.0000 1.0000 1.0000;\n");
   fprintf(fp,"\f\n");
   
   max=0;
   for (k=0;k<ims.Depth();k++) {
      for (j=0;j<ims.Height();j++) {
	 for (i=0;i<ims.Width();i++) {
	    f=ims(k,j,i);
	    if (f>max) {
	       max=f;
	    }
	 }
      }
   }
   
   for (k=0;k<ims.Depth();k++) {
      for (j=0;j<ims.Height();j++) {
	 for (i=0;i<ims.Width();i++) {
	    f=ims(k,j,i)*255;
	    c=(Uchar)(f/max);
	    fwrite(&c,sizeof(Uchar),1,fp);
	 }
      }
   }
   fclose(fp);
   
   return SUCCESS;
}

Errc PPan2Vff( const Reg3d &rgs, char *filename ) {
   Long i,j,k;
   FILE *fp;
   Uchar c;
   
   if ((filename == NULL) || (!strcmp(filename, "-"))) {
      fp = stdout;
   } else if ((fp=fopen(filename, "w")) == NULL) {
      fprintf(stderr, "Error ppan2vff: Cannot open %s\n", filename);
      return FAILURE;
   }
   
   fprintf(fp,"ncaa\ntype=raster;\nformat=slice;\nrank=3;\nbands=1;\n");
   fprintf(fp,"bits=8;\nsize=%d %d %d;\n",rgs.Width(),rgs.Height(),rgs.Depth());
   fprintf(fp,"origin=0.0 0.0 0.0;\n");
   fprintf(fp,"extent=%f %f %f;\n",(Float)rgs.Width(),(Float)rgs.Height(),(Float)rgs.Depth());
   fprintf(fp,"aspect=1.0000 1.0000 1.0000;\n");
   fprintf(fp,"\f\n");
   
   for (k=0;k<rgs.Depth();k++) {
      for (j=0;j<rgs.Height();j++) {
	 for (i=0;i<rgs.Width();i++) {
	    c=(Uchar)(rgs(k,j,i)%255);
	    fwrite(&c,sizeof(Uchar),1,fp);
	 }
      }
   }
   fclose(fp);
   
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
   case Po_Img3duc :{
      Img3duc* const ims=(Img3duc*)objs[0];
      
      result=PPan2Vff(*ims,(argc>2)? argv[2] : NULL);
   }break;
   case Po_Img3dsl :{
      Img3dsl* const ims=(Img3dsl*)objs[0];
      
      result=PPan2Vff(*ims,(argc>2)? argv[2] : NULL);
   }break;
   case Po_Img3dsf :{
      Img3dsf* const ims=(Img3dsf*)objs[0];
      
      result=PPan2Vff(*ims,(argc>2)? argv[2] : NULL);
   }break;
   case Po_Reg3d :{
      Reg3d* const rgs=(Reg3d*)objs[0];
      
      result=PPan2Vff(*rgs,(argc>2)? argv[2] : NULL);
   }break;
   default :
      PrintErrorFormat(objin,FINC);
      result = FAILURE;
   }
   
   if (result) {
      WriteArgs(argc,argv,PARC,FINC,FOUTC,&mask,objin,objs,objout,objd,MASK);
   }
   Exit(result);
   return 0;
}

#endif
