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
 * @author Francois Angot - 1997-21-07
 */

#include <string.h>
#include <pandore.h>
using namespace pandore;

/**
 * @author ptxt2pan.cpp	
 * Creation of image from text file.
 */
Errc PTxt2Pan( Img2duc &imd, char *nom, Long h, Long l ) {
   FILE *fp;
   Long x, y, u;
   
   imd.New(h, l);
   imd = 0;
   
   if (nom && (fp = fopen(nom, "rt"))) {
      while ((!feof(fp))&&(fscanf(fp, "%d", &u) != -1)) {
	 if (fscanf(fp, "%d", &x)==-1) {
	    return FAILURE;
	 }
	 if (fscanf(fp, "%d", &y) == -1) {
	    return FAILURE;
	 }
	 // set fscanf to h to avoid unused message
	 h = fscanf(fp, "%*[^\n[]"); // Skip the end of the line
	 if (imd.Hold(y, x)) {
	    imd[y][x]=(Uchar)u;
	 }
      } 
      fclose(fp);
   } else {
      std::cerr << "Error ptxt2pan: Cannot read file: " << nom << std::endl;
      return FAILURE;
   }
   return SUCCESS;
}

Errc PTxt2Pan( Img2dsl &imd, char *nom, Long h, Long l ) {
   FILE *fp;
   Long x, y, u;
   
   imd.New(h, l);
   
   imd=0;
   
   if (nom && (fp = fopen(nom, "rt"))) {
      while ((!feof(fp))&&(fscanf(fp, "%d", &u)!=-1)) {
	 if (fscanf(fp, "%d", &x) == -1) {
	    return FAILURE;
	 }
	 if (fscanf(fp, "%d", &y) == -1) {
	    return FAILURE;
	 }
	 // set fscanf to h to avoid unused message
	 h = fscanf(fp, "%*[^\n[]"); // Skip the end of the line
	 if (imd.Hold(y, x)) {
	    imd[y][x]=(Long)u;
	 }
      }
      fclose(fp);
   } else {
      std::cerr << "Error ptxt2pan: Cannot read file: " << nom << std::endl;
      return FAILURE;
   }

   return SUCCESS;
}

Errc PTxt2Pan( Img2dsf &imd, char *nom, Long h, Long l ) {
   FILE *fp;
   Long x, y;
   Float u;
   imd.New(h, l);
   
   imd = 0;
   
   if (nom && (fp = fopen(nom, "rt"))) {
      while ((!feof(fp))&&(fscanf(fp, "%f", &u)!=-1)) {
	 if (fscanf(fp, "%d", &x)==-1) return FAILURE;
	 if (fscanf(fp, "%d", &y)==-1) return FAILURE ;
	 // set fscanf to h to avoid unused message
	 h=fscanf(fp, "%*[^\n[]"); // Skip the end of the line
	 if (imd.Hold(y, x))
	    imd[y][x]=u;
      }
      fclose(fp);
   } else {
      std::cerr << "Error ptxt2pan: Cannot read file: " << nom << std::endl;
      return FAILURE;
   }   
   
   return(SUCCESS);
}

Errc PTxt2Pan( Img3duc &imd, char *nom, Long d, Long h, Long l ) {
   FILE *fp;
   Long x, y, z, u;
   
   imd.New(d, h, l);
   imd = 0;
   
   if (nom && (fp = fopen(nom, "rt"))) {
      while ((!feof(fp)) && (fscanf(fp, "%d", &u) != -1)) {
	 if (fscanf(fp, "%d", &x) == -1) {
	    return FAILURE;
	 }
	 if (fscanf(fp, "%d", &y) == -1) {
	    return FAILURE;
	 }
	 if (fscanf(fp, "%d", &z) == -1) {
	    return FAILURE;
	 }
	 if (imd.Hold(z, y, x)) {
	    imd[z][y][x] = (Uchar)u;
	 }
      }
      fclose(fp);
   } else {
      return FAILURE;
   }
   return SUCCESS;
}

Errc PTxt2Pan( Img3dsl &imd, char *nom, Long d, Long h, Long l ) {
   FILE *fp;
   Long x, y, z, u;
   
   imd.New(d, h, l);
   
   imd = 0;  
   if (nom && (fp = fopen(nom, "rt"))) {
      while ((!feof(fp)) && (fscanf(fp, "%d", &u) != -1)) {
	 if (fscanf(fp, "%d", &x) == -1) {
	    return FAILURE;
	 }
	 if (fscanf(fp, "%d", &y) == -1) {
	    return FAILURE;
	 }
	 if (fscanf(fp, "%d", &z) == -1) {
	    return FAILURE;
	 }
	 if (imd.Hold(z, y, x)) {
	    imd[z][y][x] = (Long)u;
	 }
      }
      fclose(fp);
   } else {
      std::cerr << "Error ptxt2pan: Cannot read file: " << nom << std::endl;
      return FAILURE;
   }

   return SUCCESS;
}

Errc PTxt2Pan( Img3dsf &imd, char *nom, Long d, Long h, Long l ) {
   FILE *fp;
   Long x, y, z;
   float u;
   
   imd.New(d, h, l);
   
   imd = 0;  
   if (nom && (fp=fopen(nom, "rt"))) {
      while ((!feof(fp))&&(fscanf(fp, "%f", &u) != -1)) {
	 if (fscanf(fp, "%d", &x) == -1) {
	    return FAILURE;
	 }
	 if (fscanf(fp, "%d", &y) == -1) {
	    return FAILURE;
	 }
	 if (fscanf(fp, "%d", &z) == -1) {
	    return FAILURE;
	 }
	 if (imd.Hold(z, y, x)) {
	    imd[z][y][x] = u;
	 }
      }
      fclose(fp);
   } else {
      std::cerr << "Error ptxt2pan: Cannot read file: " << nom << std::endl;
      return FAILURE;
   }

   return SUCCESS;
}

#ifdef MAIN

/*
 * Modify only the following constants, and the function call.
 */
#define	USAGE	"usage: %s type width height depth file_in [im_out|-]"
#define	PARC	4
#define	FINC	0
#define	FOUTC	1
#define	MASK	0

int main( int argc, char *argv[] ) {
   Errc result;              // The result code of the execution.
   Pobject* mask;            // The region mask.
   Pobject* objin[FINC+1];   // The input objects.
   Pobject* objs[FINC+1];    // The source objects masked by the mask.
   Pobject* objout[FOUTC+1]; // The output object.
   Pobject* objd[FOUTC+1];   // The result object of the execution.
   char* parv[PARC+1];       // The input parameters.

   if (argc < 2) {
      fprintf(stderr, USAGE"\n", argv[0]);
      Exit((Errc)FAILURE);
   }
   
   if ((argc == 2) && (!strcmp(argv[1], "-p"))) {
      fprintf(stdout, "%s 4 1 1\n", argv[0]);
      exit(0);
   }
   
   ReadArgs(argc, argv, PARC, FINC, FOUTC, &mask, objin, objs, objout, objd, parv, USAGE, MASK);
   
   switch(atoi(parv[0])) {
   case 0:{
      objd[0]=new Img2duc;
      Img2duc* const imd=(Img2duc*)objd[0];
      
      result=PTxt2Pan(*imd, argv[5], (Long)atol(parv[2]), (Long)atol(parv[1]));
   } break;
   case 1:{
      objd[0]=new Img2dsl;
      Img2dsl* const imd=(Img2dsl*)objd[0];
      
      result=PTxt2Pan(*imd, argv[5], (Long)atol(parv[2]), (Long)atol(parv[1]));
   } break;
   case 2:{
      objd[0]=new Img2dsf;
      Img2dsf* const imd=(Img2dsf*)objd[0];
      
      result=PTxt2Pan(*imd, argv[5], (Long)atol(parv[2]), (Long)atol(parv[1]));
   } break;
   case 3:{
      objd[0]=new Img3duc;
      Img3duc* const imd=(Img3duc*)objd[0];
      
      result=PTxt2Pan(*imd, argv[5], (Long)atol(parv[3]), (Long)atol(parv[2]), (Long)atol(parv[1]));
   } break;
   case 4:{
      objd[0]=new Img3dsl;
      Img3dsl* const imd=(Img3dsl*)objd[0];
      
      result=PTxt2Pan(*imd, argv[5], (Long)atol(parv[3]), (Long)atol(parv[2]), (Long)atol(parv[1]));
   } break;
   case 5:{
      objd[0]=new Img3dsf;
      Img3dsf* const imd=(Img3dsf*)objd[0];
      
      result=PTxt2Pan(*imd, argv[5], (Long)atol(parv[3]), (Long)atol(parv[2]), (Long)atol(parv[1]));
   } break;
   default:
      PrintErrorFormat(objin, FINC);
      result = FAILURE;
   }
   
   if (result) {
      WriteArgs(argc, argv, PARC, FINC+1, FOUTC, &mask, objin, objs, objout, objd, MASK);
   }
   Exit(result);
   return 0;
}

#endif
