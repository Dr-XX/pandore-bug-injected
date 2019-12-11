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
 * @author Regis Clouard - 1999-10-26
 */

#include <string.h>
#include <pandore.h>
using namespace pandore;

#include <regionlut.h>

/**
 * @file ppan2bmp.cpp
 * Convert Pandore image to BMP image.
 */ 

#define ecritc(c, fp) putc(c, fp)

void ecriti(int i, FILE* fp) {
   int b0, b1, b2, b3;
  
   b0 = (((unsigned int) i)) & 0xFF;
   b1 = (((unsigned int) i)>>8) & 0xFF;
   b2 = (((unsigned int) i)>>16) & 0xFF;
   b3 = (((unsigned int) i)>>24) & 0xFF;
   putc(b0, fp);putc(b1, fp);putc(b2, fp);putc(b3, fp);
}

void ecrits(short i, FILE* fp) {
   int b0, b1;
  
   b0 = (((unsigned int) i)) & 0xFF;
   b1 = (((unsigned int) i)>>8) & 0xFF;
   putc(b0, fp);putc(b1, fp);
}

/*
 * Build header.
 */
void EnteteBmp( FILE *fp, int nrow, int ncol, int nbbits ) {
   int bl, nc;
   int ll;
  
   ecritc('B', fp);ecritc('M', fp);		// Magic number
   ll = (((ncol * nbbits) + 31) / 32)*4; 	// Taille d'une ligne.
   if (nbbits == 24)
      nc=0;
   else
      nc = 256;
   bl = 14+40+(nc*4);
   ecriti(bl + (ll*nrow), fp);
   ecrits(0, fp);				// Reserved.
   ecrits(0, fp);
   ecriti(bl, fp);				// bfoffbits.
  
   ecriti(40, fp);
   ecriti(ncol, fp);
   ecriti(nrow, fp);
   ecrits(1, fp);
   ecrits(nbbits, fp);
   ecriti(0, fp);
   ecriti(ll*nrow, fp);
   ecriti(75*39, fp);
   ecriti(75*39, fp);
   ecriti(nc, fp);
   ecriti(nc, fp);
}

/*
 * Transforme un octet image en sa valeur hexadecimale affichable.
 */
void CouleursBmp( FILE* df ) {
   for (int i=0; i<256; i++) {
      ecritc(i, df);ecritc(i, df);ecritc(i, df);
      ecritc(0, df);
   }
}

/*
 * Sauvegarde des données 8 bits.
 */
void DonneesBmp( FILE *df, const Img2duc &ims ) {
   int bf, i, j;
  
   bf = ((ims.Width()+3)/4) * 4;
   for (i=ims.Height()-1;i>=0; i--) {
      for (j=0; j<ims.Width();j++) {
	 ecritc(ims[i][j], df);
      }
      for (; j<bf; j++) {
	 ecritc(0, df);
      }
   }
}

/*
 * Sauvegarde des données 24 bits.
 */
void DonneesBmp( FILE *df, const Reg2d &rgs ) {
   int bf;

   Uchar *data = new Uchar[3*rgs.VectorSize()];

   Uchar lut[3][MAXCOLORS];

   buildRegionLut(lut);

   const Ulong *datasrc = rgs.Vector();
   unsigned long length  = rgs.VectorSize();
   for (Ulong i = 0; i< length; i++ ) {
      Ulong  pixel = datasrc[i]%MAXCOLORS;
      data[i] = lut[0][pixel];
      data[length+i] = lut[1][pixel];
      data[2*length+i] = lut[2][pixel];
   }
   
   bf = (4-((rgs.Width()*3)%4)) & 0x03; 	// padding 32 bits.
   for (int y=rgs.Height()-1; y>=0; y--) {
      for (int x=0; x<rgs.Width(); x++) {
	 ecritc(data[rgs.VectorSize()*2+(y*rgs.Width()+x)], df);
	 ecritc(data[rgs.VectorSize()+(y*rgs.Width()+x)], df);
	 ecritc(data[y*rgs.Width()+x], df);
      }
      for (int x=0; x<bf; x++)
	 ecritc(0, df);
   }
   delete[] data;
}

/*
 * Sauvegarde des données 24 bits.
 */
void DonneesBmp( FILE *df, const Imc2duc &ims ) {
   int bf, i, j;
  
   bf = (4-((ims.Width()*3)%4)) & 0x03; 	// padding 32 bits.
   for (i=ims.Height()-1;i>=0;i--) {
      for (j=0;j<ims.Width();j++) {
	 ecritc(ims(2, i, j), df);
	 ecritc(ims(1, i, j), df);
	 ecritc(ims(0, i, j), df);
      }
      for (j=0; j<bf;j++) {
	 ecritc(0, df);
      }
   }
}

/*
 * Conclue le fichier Bmp.
 */
void EpilogueBmp( FILE * /*df*/ ) { }

/*
 * Produit un fichier Postscript imprimable.
 * Traduit le fichier Pandore en format PostScript.
 * Le fichier est en format EBmpF, pour etre repris dans les
 * logiciels WORD, DRAW...
 */
Errc PPan2Bmp( const Img2duc &ims, char *fich ) {
   FILE *dfd;
  
   if ((fich==NULL) || (!strcmp(fich, "-"))) {
      dfd = stdout;
   } else if (!(dfd = fopen(fich, "wb"))) {
      fprintf(stderr, "Error ppan2bmp: Cannot create BMP file %s\n", fich);
      return FAILURE;
   }
   
   EnteteBmp(dfd, ims.Height(), ims.Width(), 8);
   CouleursBmp(dfd);
   DonneesBmp(dfd, ims);
   EpilogueBmp(dfd);
   fclose(dfd);
   return SUCCESS;
}

Errc PPan2Bmp( const Reg2d& rgs, char *fich ) {
   FILE *dfd;
  
   if ((fich == NULL) || (!strcmp(fich, "-"))) {
      dfd = stdout;
   } else if (!(dfd = fopen(fich, "wb"))) {
      fprintf(stderr, "Error ppan2bmp: Cannot create BMP file %s\n", fich);
      Exit(FAILURE);
   }
  
   EnteteBmp(dfd, rgs.Height(), rgs.Width(), 24);
   DonneesBmp(dfd, rgs);
   EpilogueBmp(dfd);
   fclose(dfd);
  
   return SUCCESS;
}

Errc PPan2Bmp( const Imc2duc &ims, char *fich ) {
   FILE *dfd;
  
   if ((fich == NULL) || (!strcmp(fich, "-"))) {
      dfd = stdout;
   } else {
      if  (!(dfd = fopen(fich, "wb"))) {
	 fprintf(stderr, "Error  ppan2bmp: Cannot create BMP file %s\n", fich);
	 Exit(FAILURE);
      }
   }
   
   EnteteBmp(dfd, ims.Height(), ims.Width(), 24);
   DonneesBmp(dfd, ims);
   EpilogueBmp(dfd);
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
   
   switch(objs[0]->Type()) {
   case Po_Img2duc :{
      Img2duc* const ims=(Img2duc*)objs[0];

      result=PPan2Bmp(*ims, (argc>2)? argv[2] : NULL);
   }break;
   case Po_Reg2d : {
      Reg2d* const rgs=(Reg2d*)objs[0];

      result=PPan2Bmp(*rgs, (argc>2)? argv[2] : NULL);
   }break;
   case Po_Imc2duc : {
      Imc2duc* const ims=(Imc2duc*)objs[0];

      result=PPan2Bmp(*ims, (argc>2)? argv[2] : NULL);
   }break;
   default :
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
