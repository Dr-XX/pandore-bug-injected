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
 * @author Regis Clouard - 1999-11-15
 */

#include <string.h>
#include <pandore.h>
using namespace pandore;

/**
 * @file ppan2ps.cpp
 * Produit un fichier PostScript a partir d'un fichier Pandore.
 */

// coeff de conversion de pixel-image en cm
#define COEFCM .016933

// 72 pts Postscript par pouce = 28.3/cm
#define IN2PS 72
#define CM2PS 28.346

// Taille d'une feuille.
#define PAGE_WIDTH (17*IN2PS)/2
#define PAGE_HEIGHT 11*IN2PS

/*
 * Write the header of the POswtScript file.
 * It depends on the colorp value.
 */
void EntetePS( FILE * df, char * nom, int xmin, int ymin, int xmax, int ymax, bool colorp ) {
   // Usual comments.
   fprintf(df,"%%!PS-Adobe-2.0 EPSF-2.0\n");
   fprintf(df,"%%%%Title: %s\n",nom);
   fprintf(df,"%%%%Creator: Pan2Ps - By Regis Clouard\n");
   fprintf(df,"%%%%BoundingBox: %d %d %d %d\n",xmin,ymin,xmax,ymax);
   fprintf(df,"%%%%Pages:1\n");
   fprintf(df,"%%%%EndComments\n");
   fprintf(df,"%%%%BeginProlog\n\n");
   if (colorp) {
      fprintf(df,"/display_image {\n");
      fprintf(df,"/colorimage where {\n");
      fprintf(df,"     pop colorimage\n");
      fprintf(df,"    } {\n");
      fprintf(df,"        pop pop pop\n");
      fprintf(df,"        /imagebuf 3 string def\n");
      fprintf(df,"        /str1 1 string def\n");
      fprintf(df,"        {\n");
      fprintf(df,"            str1 dup 0\n");
      fprintf(df,"            currentfile imagebuf readhexstring pop\n");
      fprintf(df,"            {} forall\n");
      fprintf(df,"            .11 mul exch .59 mul add exch .3 mul add\n");
      fprintf(df,"            round cvi put\n");
      fprintf(df,"         } image\n");
      fprintf(df,"    } ifelse\n");
      fprintf(df,"} def\n");
      fprintf(df,"%%%%EndProlog\n");
   }
}

/*
 * Definition du corps de la procedure d'affichage de l'image PostScript.
 * It depends on the 
 */
void CorpsPS( FILE* df, int xpage, int ypage, int rotation, float xzoom,
	      float yzoom, int nlig, int ncol, bool colorp ) {
   int	impslarg = (int)(xzoom * ncol * COEFCM * CM2PS);
   int	impshaut = (int)(yzoom * nlig * COEFCM * CM2PS);
  
   fprintf(df,"%%%%Page: 1 1\n");
   fprintf(df,"%% remember original state\n");
   fprintf(df,"/origstate save def\n");
  
   fprintf(df,"%% build a temporary dictionary\n");
   fprintf(df,"1 dict begin\n");
   fprintf(df,"  /DataString %d string def\n",ncol);
   fprintf(df,"%% lower left corner\n");
   fprintf(df,"%d %d translate\n",xpage,ypage);
  
   fprintf(df,"%% landscape mode (90) or portrait mode (0)\n");
   fprintf(df,"%d rotate\n",rotation);
  
   fprintf(df,"%% size of image (on paper, in 1/72inch coords)\n");
   fprintf(df,"%d %d scale\n",impslarg,impshaut);
   // Control instructions.
   fprintf(df,"%%dimensions of data [mapping matrix]\n");
   fprintf(df,"%d %d %d [%d 0 0 -%d 0 0]\n",ncol,nlig,8,ncol,nlig);
   if (colorp)  {
      fprintf(df,"{currentfile DataString readhexstring pop} false 3 display_image\n");
   }else{
      fprintf(df,"{currentfile DataString readhexstring pop} bind image\n");
   }
}

/*
 *
 */
void Convertie( FILE* df, unsigned char octet ) {
   static	unsigned char	hxtab[]= "0123456789abcdef";
  
   fprintf(df,"%c",hxtab[octet >> 4]);
   fprintf(df,"%c",hxtab[octet & 0x0f]);
}

/*
 * Traduit les donnees de l'image en
 * caracteres affichables sous forme hexadecimaux.
 */
void DonneesPS( FILE* df, const Img2duc& ims ) {
   int	ncar=0;
   register int	i,j;
  
   for (i=0; i<ims.Height(); i++) {
      for (j=0; j<ims.Width(); j++) {
	 Convertie(df,ims[i][j]);
	 if (((++ncar) % 33) == 0) { // Just for wrapping.
	    fprintf(df,"\n");
	    ncar=0;
	 }
      }
   }
}

/*
 * Conclue le fichier PS.
 * Si le fichier est destine a etre imprimante
 * rajoute le mot cle "showpage".
 */
void EpiloguePS( FILE* df, bool imprime ) {
   fprintf(df,"\n");
   if (imprime)
      fprintf(df,"showpage\n");
   fprintf(df,"%% stop using temporary dictionary\n");
   fprintf(df,"end\n");
  
   fprintf(df,"%% restore original state\n");
   fprintf(df,"origstate restore\n");
   fprintf(df,"%%%%Trailer\n");
}

/*
 * Convert grey image to PostScript file.
 */
Errc PPan2PS( const Img2duc &ims, char *fich ) {
   FILE* dfd;
   int	xmin,ymin,xmax,ymax;
   int	rotation=0;
  
   if ((fich==NULL) || (!strcmp(fich,"-"))) {
      dfd = stdout;
   } else if (!(dfd = fopen(fich, "w"))) {
      fprintf(stderr,"Error ppan2ps: Impossible de créer le fichier PostScript %s\n",fich);
      Exit(FAILURE);
   }

   xmin = 0;
   ymin = 0;
   xmax = (int)(ims.Width()*COEFCM*CM2PS);
   ymax = ymin + (int)(ims.Height()*COEFCM*CM2PS);
  
   EntetePS(dfd,fich,xmin,ymin,xmax,ymax,false);
   CorpsPS(dfd,xmin,ymax,rotation,1.0,1.0,ims.Height(),ims.Width(),false);
   DonneesPS(dfd,ims);
   EpiloguePS(dfd,true);
  
   fclose(dfd);
   return SUCCESS;
}


/*
 * Traduit les donnees de l'image en
 * caracteres affichables sous forme hexadecimaux.
 */
void DonneesPS( FILE* df, const Imc2duc &ims ) {
   int	ncar=0;
   register int	i,j;
  
   for (i=0; i<ims.Height(); i++) {
      for (j=0; j<ims.Width(); j++) {
	 Convertie(df,ims.X[i][j]);
	 Convertie(df,ims.Y[i][j]);
	 Convertie(df,ims.Z[i][j]);
	 if (((++ncar) % 11) == 0) { // Just for wrapping.
	    fprintf(df,"\n");
	    ncar=0;
	 }
      }
   }
}

/*
 * Convert color image to PostScript file
 */
Errc PPan2PS( const Imc2duc &ims, char *fich ) {
   FILE* dfd;
   int	xmin,ymin,xmax,ymax;
   int	rotation=0;
  
   if ((fich==NULL) || (!strcmp(fich,"-"))) {
      dfd = stdout;
   } else if  (!(dfd = fopen(fich,"w"))) {
      fprintf(stderr, "Error ppan2ps: Impossible de creer le fichier PostScript %s\n",fich);
      Exit(FAILURE);
   }
   xmin = 0;
   ymin = 0;
   xmax = (int)(ims.Width()*COEFCM*CM2PS);
   ymax = ymin + (int)(ims.Height()*COEFCM*CM2PS);
  
   EntetePS(dfd,fich,xmin,ymin,xmax,ymax,true);
   CorpsPS(dfd,xmin,ymax,rotation,1.0,1.0,ims.Height(),ims.Width(),true);
   DonneesPS(dfd,ims);
   EpiloguePS(dfd,true);
  
   fclose(dfd);
   return SUCCESS;
}

/*
 * Traduit les donnees de l'image en
 * caracteres affichables sous forme hexadecimaux.
 */
void DonneesPS( FILE* df, const Reg2d &regs ) {
   int	ncar=0;
   register int	i,j;
   int lut[15][3];
  
   // Random lut
   for (i=0; i<15;i++) {
      lut[i][0]=rand();
      lut[i][1]=rand();
      lut[i][2]=rand();
   }
   for (i=0; i<regs.Height(); i++) {
      for (j=0; j<regs.Width(); j++) {
	 Convertie(df,lut[regs[i][j]%15][0]);
	 Convertie(df,lut[regs[i][j]%15][1]);
	 Convertie(df,lut[regs[i][j]%15][2]);
	 if (((++ncar) % 11) == 0) { // Just for wrapping.
	    fprintf(df,"\n");
	    ncar=0;
	 }
      }
   }
}

/*
 * Convert region map to PostScript file
 */
Errc PPan2PS( const Reg2d &regs, char *fich ) {
   FILE*	dfd;
   int	xmin,ymin,xmax,ymax;
   int	rotation=0;
  
   if ((fich==NULL) || (!strcmp(fich,"-"))) {
      dfd = stdout;
   } else if  (!(dfd = fopen(fich,"w"))) {
      fprintf(stderr,"Error ppan2ps: Impossible de creer le fichier PostScript %s\n",fich);
      Exit(FAILURE);
   }
   xmin = 0;
   ymin = 0;
   xmax = (int)(regs.Width()*COEFCM*CM2PS);
   ymax = ymin + (int)(regs.Height()*COEFCM*CM2PS);
  
   EntetePS(dfd,fich,xmin,ymin,xmax,ymax,false);
   CorpsPS(dfd,xmin,ymax,rotation,1.0,1.0,regs.Height(),regs.Width(),false);
   DonneesPS(dfd,regs);
   EpiloguePS(dfd,true);
  
   fclose(dfd);
   return SUCCESS;
}

/*
 * Convert Long image to Uchar image
 * and then to PostScript file
 */
Errc PPan2PS( const Img2dsl &ims, char *fich ) {
   FILE *dfd;
   int	i,j;
   int	xmin,ymin,xmax,ymax;
   int	rotation=0;
   Img2duc imi(ims.Size());
   int max,coef;

   if ((fich == NULL) || (!strcmp(fich, "-"))) {
      dfd = stdout;
   } else if  (!(dfd = fopen(fich,"w"))) {
      fprintf(stderr,"Error ppan2ps: Cannot create PostScript file %s\n",fich);
      Exit(FAILURE);
   }
   xmin = 0;
   ymin = 0;
   xmax = (int)(ims.Width()*COEFCM*CM2PS);
   ymax = ymin + (int)(ims.Height()*COEFCM*CM2PS);
  
   max = 0;
   for (i = 0; i < ims.Height(); i++) {
      for (j = 0; j < ims.Width(); j++) {
	 if (max > ims[i][j]) {
	    max = ims[i][j];
	 }
      }
   }
   coef = (max / (MAXUCHAR + 1)) + 1;
   for (i = 0; i < ims.Height(); i++) {
      for (j = 0; j < ims.Width(); j++) {
	 imi[i][j] = Uchar(ims[i][j] / coef);
      }
   }
   
   EntetePS(dfd,fich,xmin,ymin,xmax,ymax,true);
   CorpsPS(dfd,xmin,ymax,rotation,1.0,1.0,imi.Height(),imi.Width(),true);
   DonneesPS(dfd,imi);
   EpiloguePS(dfd,true);
   
   fclose(dfd);
   return SUCCESS;
}

#ifdef MAIN

#define	USAGE	"usage: %s [im_in|-] [im_out|-]"
#define PARC	0
#define FINC	1
#define FOUTC	1
#define MASK	0

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
      result = PPan2PS(*ims, (argc == 3)? argv[2] : NULL);
   } break;
   case Po_Img2dsl :{
      Img2dsl* const ims=(Img2dsl*)objs[0];
      result = PPan2PS(*ims, (argc == 3)? argv[2] : NULL);
   } break;
   case Po_Imc2duc :{
      Imc2duc* const ims=(Imc2duc*)objs[0];
      result = PPan2PS(*ims, (argc ==3)? argv[2] : NULL);
   } break;
   case Po_Reg2d :{
      Reg2d* const ims=(Reg2d*)objs[0];
      result = PPan2PS(*ims, (argc ==3)? argv[2] : NULL);
   } break;
   default :
      PrintErrorFormat(objin,FINC);
      result = FAILURE;
   }
   
   if (result) {
      WriteArgs(argc,argv,PARC,FINC,FOUTC,&mask,objin,objs,objout,objd);
   }
   Exit(result);
   return 0;
}

#endif
