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

/**
 * @file ppan2d23d.cpp
 * Converts several 2D images to one 3D image.
 */

/**
 * Copy pixels from the input 2d image
 * into the slide #depth of the output 3d image.
 */
template <typename T>
Errc Insert( const Imx3d<T> &ims, Imx3d<T> &imd, int depth ) {
   for (int b = 0; b < ims.Bands(); b++) {
      memcpy(&(imd[b][depth][0][0]), &(ims[b][0][0][0]), sizeof(T) * ims.VectorSize());
   }
   return SUCCESS;
}

/**
 * Convert (nf-nd+1) 2d images to one 3d image.
 */
Errc PPan2d23d( char *nameFor2dImages, char *nameFor3dImage, int nd, int nf ) {
   char basename[512], suffixe[50];
   char nomfichier[256], formatnom[256];
   int length;
   int i;
   Pobject *objs, *objd;
   
   if (nd > nf) {
      fprintf(stderr, "Error ppan2d23d: Bad interval values %d %d\n", nd, nf);
      return FAILURE;
   }
   
   if (strlen(nameFor2dImages) > sizeof(basename)) {
      nameFor2dImages[sizeof(basename)  - 1] = 0;
   }
   // Get the basename of the 2D files.
   basename[0] = 0;
   suffixe[0] = 0;
   sscanf(nameFor2dImages, "%[^#]", basename);
   basename[sizeof(basename) - 1]= 0;
   i = strlen(basename);
   length = 0;

   while (nameFor2dImages[i+length] == '#') {
      length++;
   }
   strncpy(suffixe, &(nameFor2dImages[i+length]), sizeof(suffixe));
   
   if (length == 0) {
      fprintf(stderr, "Error ppan2d23d: Bad input file basename (no # inside): %s\n", nameFor2dImages);
      return FAILURE;
   }
   
   // Determine the format of the number (i.e. 1, 01, 001, 0001...)
   snprintf(formatnom, sizeof(formatnom), "%s%%0%dd%s", basename, length, suffixe);
   formatnom[sizeof(formatnom) - 1] = 0;
   snprintf(nomfichier, sizeof(nomfichier), formatnom, nd);
   nomfichier[sizeof(nomfichier) - 1] = 0;
   
   // Determine the type of the input image.
   objs = LoadFile(nomfichier);
   if (objs == NULL) {
      return FAILURE;
   }

   switch (objs->Type()) {
   case Po_Img2duc:
      objd = new Img3duc(nf - nd + 1, ((Img2duc*)objs)->Height(), ((Img2duc*)objs)->Width());
      for (i = nd; i <= nf; i++) {
	 snprintf(nomfichier,  sizeof(nomfichier), formatnom, i);
	 nomfichier[sizeof(nomfichier) - 1] = 0;
	 objs = LoadFile(nomfichier);
	 if (objs == NULL) {
	    fprintf(stderr, "Error ppan2d23d: Bad input files\n");
	    return FAILURE;
	 }
	 Insert(*((Img2duc*)objs), *((Img3duc*)objd), i - nd);
      }
      break;
   case Po_Img2dsl:
      objd = new Img3dsl(nf - nd + 1, ((Img2dsl*)objs)->Height(), ((Img2dsl*)objs)->Width());
      for (i = nd; i <= nf; i++) {
	 snprintf(nomfichier, sizeof(nomfichier), formatnom, i);
	 nomfichier[sizeof(nomfichier) - 1] = 0;
	 objs = LoadFile(nomfichier);
	 if (objs == NULL) {
	    fprintf(stderr, "Error ppan2d23d: Bad input files\n");
	    return FAILURE;
	 }
	 Insert(*((Img2dsl*)objs), *((Img3dsl*)objd), i - nd);
      }
      break;
   case Po_Img2dsf:
      objd = new Img3dsf(nf - nd + 1, ((Img2dsf*)objs)->Height(), ((Img2dsf*)objs)->Width());
      for (i = nd; i <= nf; i++) {
	 snprintf(nomfichier, sizeof(nomfichier), formatnom, i);
	 nomfichier[sizeof(nomfichier) - 1] = 0;
	 objs = LoadFile(nomfichier);
	 if (objs == NULL) {
	    fprintf(stderr, "Error ppan2d23d: Bad input files\n");
	    return FAILURE;
	 }
	 Insert(*((Img2dsf*)objs), *((Img3dsf*)objd), i - nd);
      }
      break;
   case Po_Imc2duc:
      objd = new Imc3duc(nf - nd + 1, ((Imc2duc*)objs)->Height(), ((Imc2duc*)objs)->Width());
      ((Imc3dsf*)objd)->ColorSpace(((Imc2dsf*)objs)->ColorSpace());
      for (i = nd; i <= nf; i++) {
	 snprintf(nomfichier, sizeof(nomfichier), formatnom, i);
	 nomfichier[sizeof(nomfichier) - 1] = 0;
	 objs = LoadFile(nomfichier);
	 if (objs == NULL) {
	    fprintf(stderr, "Error ppan2d23d: Bad input files\n");
	    return FAILURE;
	 }
	 Insert(*((Imc2duc*)objs), *((Imc3duc*)objd), i - nd);
      }
      break;
   case Po_Imc2dsl:
      objd = new Imc3duc(nf - nd + 1, ((Imc2duc*)objs)->Height(), ((Imc2duc*)objs)->Width());
      ((Imc3dsf*)objd)->ColorSpace(((Imc2dsf*)objs)->ColorSpace());
      for (i = nd; i <= nf; i++) {
	 snprintf(nomfichier, sizeof(nomfichier), formatnom, i);
	 nomfichier[sizeof(nomfichier) - 1] = 0;
	 objs = LoadFile(nomfichier);
	 if (objs == NULL) {
	    fprintf(stderr, "Error ppan2d23d: Bad input files\n");
	    return FAILURE;
	 }
	 Insert(*((Imc2duc*)objs), *((Imc3duc*)objd), i - nd);
      }
      break;
   case Po_Imc2dsf:
      objd = new Imc3dsf(nf - nd + 1, ((Imc2dsf*)objs)->Height(), ((Imc2dsf*)objs)->Width());
      ((Imc3dsf*)objd)->ColorSpace(((Imc2dsf*)objs)->ColorSpace());
      for (i = nd; i <= nf; i++) {
	 snprintf(nomfichier, sizeof(nomfichier), formatnom, i);
	 nomfichier[sizeof(nomfichier) - 1] = 0;
	 objs = LoadFile(nomfichier);
	 if (objs == NULL) {
	    fprintf(stderr, "Error ppan2d23d: Bad input files\n");
	    return FAILURE;
	 }
	 Insert(*((Imc2dsf*)objs), *((Imc3dsf*)objd), i - nd);
      }
      break;
   case Po_Imx2duc:
      objd = new Imx3duc(((Imx2duc*)objs)->Bands(), nf - nd + 1, ((Imx2duc*)objs)->Height(), ((Imx2duc*)objs)->Width());
      for (i = nd; i <= nf; i++) {
	 snprintf(nomfichier, sizeof(nomfichier), formatnom, i);
	 nomfichier[sizeof(nomfichier) - 1] = 0;
	 objs =LoadFile(nomfichier);
	 if (objs == NULL) {
	    fprintf(stderr, "Error ppan2d23d: Bad input files\n");
	    return FAILURE;
	 }
	 Insert(*((Imx2duc*)objs), *((Imx3duc*)objd), i-nd);
      }
      break;
   case Po_Imx2dsl:
      objd = new Imx3duc(((Imx2duc*)objs)->Bands(), nf - nd + 1, ((Imx2duc*)objs)->Height(), ((Imx2duc*)objs)->Width());
      for (i = nd; i <= nf; i++) {
	 snprintf(nomfichier, sizeof(nomfichier), formatnom, i);
	 nomfichier[sizeof(nomfichier) - 1] = 0;
	 objs = LoadFile(nomfichier);
	 if (objs == NULL) {
	    fprintf(stderr, "Error ppan2d23d: Bad input files\n");
	    return FAILURE;
	 }
	 Insert(*((Imx2duc*)objs), *((Imx3duc*)objd), i - nd);
      }
      break;
   case Po_Imx2dsf:
      objd = new Imx3dsf(((Imx2duc*)objs)->Bands(), nf - nd + 1, ((Imx2dsf*)objs)->Height(), ((Imx2dsf*)objs)->Width());
      for (i = nd; i <= nf; i++) {
	 snprintf(nomfichier, sizeof(nomfichier), formatnom, i);
	 nomfichier[sizeof(nomfichier) - 1] = 0;
	 objs = LoadFile(nomfichier);
	 if (objs == NULL) {
	    fprintf(stderr, "Error ppan2d23d: Bad input files\n");
	    return FAILURE;
	 }
	 Insert(*((Imx2dsf*)objs), *((Imx3dsf*)objd), i - nd);
      }
      break;

   default:
      std::cerr << "ERROR ppan2d23d: Input types not supported by this operator: " << objs->Name() << std::endl;
      return FAILURE;
   }
   
   return SaveFile(objd, nameFor3dImage);
}

#ifdef MAIN

#define	USAGE	"usage: %s first-image-number last-image-number im_in_template [im_out|-]"
#define	PARC	2
#define	FINC	0
#define	FOUTC	0
#define	MASK	0

int main( int argc, char *argv[] ) {
   Errc result;              // The result code of the execution.
   Pobject* mask;            // The region mask.
   Pobject* objin[FINC+1];   // The input objects.
   Pobject* objs[FINC+1];    // The source objects masked by the mask.
   Pobject* objout[FOUTC+1]; // The output objects.
   Pobject* objd[FOUTC+1];   // The result objects of the execution.
   char* parv[PARC+1];       // The input parameters.
   
   if (argc < 2) {
      fprintf(stderr, USAGE"\n", argv[0]);
      Exit((Errc)FAILURE);
   }
   
   if ((argc == 2) && (!strcmp(argv[1], "-p"))) {
      fprintf(stdout, "%s 2 1 1\n", argv[0]);
      exit(0);
   }
 
   ReadArgs(argc, argv, PARC, FINC, FOUTC, &mask, objin, objs, objout, objd, parv, USAGE, MASK);
   
   result = PPan2d23d(argv[3], argv[4], atoi(parv[0]), atoi(parv[1]));
   
   if (result) {
      WriteArgs(argc, argv, PARC, FINC, FOUTC, &mask, objin, objs, objout, objd, MASK);
   }
   Exit(result);
   return 0;
}
#endif
