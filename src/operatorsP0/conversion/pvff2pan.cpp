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
 * @author Francois Angot - 1996-12-16
 */

#include <string.h>
#include <pandore.h>
using namespace pandore;

/**
 * @file pvff2pan.cpp
 * Convert VFF file to 3d pandore image
 */ 

int OpenVFF( FILE *fp, Ulong *ndep, Ulong *nrow, Ulong *ncol ) {
   char chaine[256], c;
   Ushort i;
   Ushort bits;
   
   if ((fread(chaine, sizeof(char), 5, fp) < 5) || (strncmp(chaine, "ncaa", 4))) {
      std::cerr << "Error pvff2pan: Bad VFF format." << std::endl;
      Exit(FAILURE);
   }
   
   c = fgetc(fp);
   while (c != '\f') {
      i = 0;
      while (c != '=') {
	 chaine[i++] = c;
	 c = fgetc(fp);
      }
      chaine[i] = 0;
	 
      if (!strcmp(chaine, "size")) {
	 i = 0;
	 c = fgetc(fp);
	 while (c != ';') {
	    chaine[i++] = c;
	    c = fgetc(fp);
	 }
	 chaine[i] = 0;
	 sscanf(chaine, "%ud %ud %ud", (unsigned int*)ncol, (unsigned int*)nrow, (unsigned int*)ndep);
      }
      else if (!strcmp(chaine, "bits")) {
	 i = 0;
	 c = fgetc(fp);
	 while (c != ';') {
	    chaine[i++] = c;
	    c = fgetc(fp);
	 }
	 chaine[i]=0;
	 sscanf(chaine, "%hd", (short*)&bits);
      } else {
	 while (c != ';') {
	    c = fgetc(fp);
	 }
      }
      
      while (( (c = fgetc(fp)) == ' ') || (c == '\n')) ;
   }
   
   c = fgetc(fp);

   return bits;
}

/*
 *
 */
Errc PVff2Pan( FILE *fp, Img3duc &imd ) {
   for (int k = 0; k < imd.Depth(); k++) {
      for (int j = 0 ; j < imd.Height(); j++) {
	 if (fread(&imd(k, j, 0), sizeof(Uchar), imd.Width(), fp) < 1) {
	    return FAILURE;
	 }
      }
   }
   
   return SUCCESS;
}

Errc PVff2Pan( FILE *fp, Img3dsl &imd ) {
   for (int z = 0; z < imd.Depth(); z++) {
      for (int y = 0; y < imd.Height(); y++) {
	 for (int x = 0; x < imd.Width(); x++) {
	    Ushort val;
	    if (fread(&val, sizeof(Ushort), 1, fp) < 1) {
	       return FAILURE;
	    }
	    imd(z, y, x) = (Long)val;
	 }
      }
   }
   return SUCCESS;
}

#ifdef MAIN

/*
 * Modify only the following constants, and the function call.
 */
#define	USAGE	"usage: %s im_in [im_out|-]"
#define	PARC	0
#define	FINC	0
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
   
   ReadArgs(argc, argv, PARC, FINC, FOUTC, &mask, objin, objs, objout, objd, parv, USAGE, MASK);
   
   if (argc < 2) {
      fprintf(stderr, USAGE"\n", argv[0]);
      Exit((Errc)FAILURE);
   }
   
   if ((argc == 2) && (!strcmp(argv[1], "-p"))) {
      fprintf(stdout, "%s 0 1 1\n", argv[0]);
      exit(0);
   }
 
   FILE *fp;
   if ((fp = fopen(argv[1], "r")) == NULL) {
      fprintf(stderr, "Error pvff2pan: Cannot open %s\n", argv[1]);
      Exit(FAILURE);
   }
   Ulong ndep, nrow, ncol;
   int bits= OpenVFF(fp, &ndep, &nrow , &ncol);

   switch (bits) {
   case 8 : {
      objd[0] = new Img3duc(ndep, nrow, ncol);
      Img3duc* const imd = (Img3duc*)objd[0];
      result = PVff2Pan(fp, *imd);
      break;
   }
   case 16 : {
      objd[0] = new Img3dsl(ndep, nrow, ncol);
      Img3dsl* const imd = (Img3dsl*)objd[0];
      result = PVff2Pan(fp, *imd);
      break;
   }
   default :
      fprintf(stderr, "Error pvff2pan: Bad VFF file.\n");
      break;
   }
   
   fclose(fp);
   
   if (result) {
      WriteArgs(argc, argv, PARC, FINC + 1, FOUTC, &mask, objin, objs, objout, objd, MASK);
   }
   Exit(result);
   return 0;
}

#endif
