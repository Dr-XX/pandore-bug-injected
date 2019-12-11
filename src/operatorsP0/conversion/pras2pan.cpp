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
 * @author Regis Clouard - 1997-03-10
 */

#include <stdexcept>
#include <string.h>
#include <pandore.h>
using namespace pandore;

/**
 * @file pras2pan.cpp
 * Converts Rasterfile to Pandore file.
 * 
 * The Sun header structure is:
 * Parameter 	Description
 * int ras_magic 	Magic number
 * int ras_width 	Width (pixels) of image
 * int ras_height 	Height (pixels) of image
 * int ras_depth 	Depth (1, 8, 24, 32 bits) of pixel
 * int ras_length 	Length (bytes) of image
 * int ras_type 	Type of file; see RT_* below
 * int ras_maptype 	Type of colormap; see RMT_* below
 * int ras_maplength 	Length (bytes) of following map
 * 
 * 
 * After the header, the color map (if ras_maptype is not RMT_NONE)
 * follows for ras_maplength bytes, followed by an image of
 * ras_length bytes:
 * Macro name 	Description
 * RAS_MAGIC 	0x59a66a95
 * RT_STANDARD 	1 (Raw pixrect image in 68000 byte order)
 * RT_BYTE_ENCODED 	2 (Run-length compression of bytes)
 * RT_FORMAT_RGB 	3 ([X]RGB instead of [X]BGR)
 * RMT_NONE 	0 (ras_maplength is expected to be 0)
 * RMT_EQUAL_RGB 	1 (red[ras_maplength/3],green[],blue[])
 */

 struct rasterfile {
    int  ras_magic;
    int  ras_width;
    int  ras_height;
     int  ras_depth;
     int  ras_length;
     int  ras_type;
     int  ras_maptype;
     int  ras_maplength;
  };
  #define RMT_NONE  0       /* ras_maplength is expected to be 0 */
  #define RMT_EQUAL_RGB	1
  #define RAS_MAGIC 0x59a66a95

/**
 * Overrides the fread function to test the return valu.
 */
inline void _fread(void *data, size_t size, size_t n, FILE *stream) {
   if (fread(data,size,n,stream) < n) {
      throw std::exception();
   }
}

/*
  * Deformatage Rasterfile dans image.
 */
Errc PRas2Pan( char *nomfichier, Pobject **objout ) {
   Long i,j;
   Long nlig;
   Long ncol;
   FILE *f;
   Uchar *data;
   Uchar *p;
   struct rasterfile entete;
   unsigned char octet;
   Uchar red[MAXUCHAR+1];
   Uchar green[MAXUCHAR+1];
   Uchar blue[MAXUCHAR+1];

   // verification des entrees.
   if ((nomfichier) && (strcmp(nomfichier,"-"))) {
      if ((f = fopen(nomfichier,"rb")) == NULL) {
	 fprintf(stderr,"Error pras2pan: Cannot open rasterfile %s\n",nomfichier);
	 return FAILURE;
      }
   }else{
      f = stdin;
   }

   // Chargement de l'entete du fichier Rasterfile
   _fread((char*)&entete,sizeof(struct rasterfile),1,f);
   if (entete.ras_magic != RAS_MAGIC) {
      fclose(f);
      fprintf(stderr,"Error pras2pan: Not a Rasterfile.\n");
      return FAILURE;
   }
   // read the colormap.
   if (entete.ras_maptype != RMT_NONE) {
      if (entete.ras_maptype == RMT_EQUAL_RGB) {
	 _fread(&red[0],sizeof(char),entete.ras_maplength/3,f);
	 _fread(&green[0],sizeof(char),entete.ras_maplength/3,f);
	 _fread(&blue[0],sizeof(char),entete.ras_maplength/3,f);
      }else // else skip unknown colormap.
	 fseek(f,entete.ras_maplength,1); 
   }
   nlig = entete.ras_height;
   ncol = entete.ras_width;
  
   if (entete.ras_depth == 1) {
      if ((data=(Uchar*)malloc(sizeof(Uchar)*nlig*ncol)) == NULL) {
	 fclose(f);
	 return FAILURE;
      }
      _fread((char*)data,sizeof(Uchar),(nlig*ncol)/8,f);
      p=data;octet=0;
      *objout = new Img2duc(nlig,ncol);
      for (i=0;i<nlig;i++) {
	 for (j=0;j<ncol;j++,octet++) {
	    if (octet==8) { p++; octet=0; }
	    (**((Img2duc**)objout))[i][j]=(*p>>octet) & 0x01;
	 }
	 int bf = (32-(ncol/8)%32)%32;
	 for (j=0; j< bf; j++)
	    _fread((char*)data,sizeof(Uchar),1,f);
      }
   }else if ((entete.ras_depth==8) && (entete.ras_maptype==RMT_NONE)) {
      if ((data=(Uchar*)malloc(sizeof(Uchar)*8*nlig*ncol))==NULL) {
	 fclose(f);
	 return FAILURE;
      }
      _fread((char*)data,sizeof(Uchar),nlig*ncol,f);
      p=data;
      Img2duc *img = new Img2duc(nlig,ncol);
      *objout = img;
      for (i=0;i<nlig;i++) {
	 for (j=0;j<ncol;j++,p++) {
	    (*img)[i][j] = *p;
	 }
	 int bf = (32-(ncol/8)%32)%32;
	 for (j=0; j< bf; j++)
	    _fread((char*)data,sizeof(Uchar),1,f);
      }
   }else if ((entete.ras_depth==8) && (entete.ras_maptype==RMT_EQUAL_RGB)) {
      if ((data=(Uchar*)malloc(sizeof(Uchar)*8*nlig*ncol))==NULL) {
	 fclose(f);
	 return FAILURE;
      }
      _fread((char*)data,sizeof(Uchar),nlig*ncol,f);
      p=data;
      Imc2duc *img = new Imc2duc(nlig,ncol);
      *objout = img;
      for (i=0;i<nlig;i++) {
	 for (j=0;j<ncol;j++,p++) {
	    (*img).X[i][j] = red[*p];
	    (*img).Y[i][j] = green[*p];
	    (*img).Z[i][j] = blue[*p];
	 }
	 int bf = (32-(ncol/8)%32)%32;
	 for (j=0; j< bf; j++)
	    _fread((char*)data,sizeof(Uchar),1,f);
      }
   }else if (entete.ras_depth == 24) {
      if ((data=(Uchar*)malloc(sizeof(Uchar)*3))==NULL) {
	 fclose(f);
	 return FAILURE;
      }
      Imc2duc *img = new Imc2duc(nlig,ncol);
      *objout = img;
      for (i=0;i<nlig;i++) {
	 for (j=0;j<ncol;j++) {
	    _fread((char*)data,sizeof(Uchar),3,f);
	    (*img).X[i][j]=data[0];
	    (*img).Y[i][j]=data[1];
	    (*img).Z[i][j]=data[2];
	 }
	 int bf = (32-ncol%32)%32;
	 for (j=0; j< bf; j++)
	    _fread((char*)data,sizeof(Uchar),1,f);
      
      }
   }else {
      fprintf(stderr,"Error pras2pan: Unknow rasterfile format\n");
      return FAILURE;
   }
  
   free((char*)data);
  
   fclose(f);
  
   return SUCCESS;
}

#ifdef MAIN


/*
 * Modify only the following constants, and the function calls.
 */
#define	USAGE	"usage: %s im_in [im_out|-]"
#define	PARC	0
#define	FINC	0
#define	FOUTC	1
#define	MASK	0

int main( int argc, char *argv[] ) {
   Pobject* mask;            // The region mask.
   Pobject* objin[FINC+1];   // The input objects.
   Pobject* objs[FINC+1];    // The source objects masked by the mask.
   Pobject* objout[FOUTC+1]; // The output objects.
   Pobject* objd[FOUTC+1];   // The result objects of the execution.
   char* parv[PARC+1];       // The input parameters.
   

   if (argc<2) {
      fprintf(stderr,USAGE"\n",argv[0]);
      Exit((Errc)FAILURE);
   }
   
   if ((argc==2) && (!strcmp(argv[1],"-p"))) {
      fprintf(stdout,"%s 0 1 1\n",argv[0]);
      exit(0);
   }
 
   ReadArgs(argc,argv,PARC,FINC,FOUTC,&mask,objin,objs,objout,objd,parv,USAGE,MASK);
   try {
      if (PRas2Pan(((argc>1)? argv[1]:NULL),&objd[0])) {
	 WriteArgs(argc,argv,PARC,FINC+1,FOUTC,&mask,objin,objs,objout,objd,MASK);
	 Exit(SUCCESS);
      } else {
	 Exit(FAILURE);
      }
   } catch (const std::exception & ) {
      std::cerr<<"Error pras2pan: Bad raster input file"<<std::endl;
      Exit(FAILURE);
   }
   return 0;
}

#endif
