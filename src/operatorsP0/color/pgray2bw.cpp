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
 * @author Régis Clouard - 1997-21-07
 */

#include <pandore.h>
using namespace pandore;

/**
 * @file pgray2bw.cpp	
 * Converts a a gray scale image to black and white image.
 * Algorithm :
 *     Replace each gray level by its probability to be black.
 */

#define PLOT	16

static Uchar matrice[PLOT][PLOT] = {
{128,	32,	160,	8,	136,	40,	168,	2,	130,	34,	162,	10,	138,	42,	170,	192},
{64,	224,	96,	200,	72,	232,	104,	194,	66,	226,	98,	202,	74,	234,	106,	48},
{176,	16,	144,	56,	184,	24,	152,	50,	178,	18,	146,	58,	186,	26,	154,	240},
{112,	208,	80,	248,	120,	216,	88,	242,	114,	210,	82,	250,	122,	218,	90,	12},
{140,	44,	172,	4,	132,	36,	164,	14,	142,	46,	174,	6,	134,	38,	166,	204},
{76,	236,	108,	196,	68,	228,	100,	206,	78,	238,	110,	198,	70,	230,	102,	60},
{188,	28,	156,	52,	180,	20,	148,	62,	190,	30,	158,	54,	182,	22,	150,	252},
{124,	220,	92,	244,	116,	212,	84,	254,	126,	222,	94,	246,	118,	214,	86,	3},
{131,	35,	163,	11,	139,	43,	171,	1,	129,	33,	161,	9,	137,	41,	169,	195},
{67,	227,	99,	203,	75,	235,	107,	193,	65,	225,	97,	201,	73,	233,	105,	51},
{179,	19,	147,	59,	187,	27,	155,	49,	177,	17,	145,	57,	185,	25,	153,	243},
{115,	211,	83,	251,	123,	219,	91,	241,	113,	209,	81,	249,	121,	217,	89,	15},
{143,	47,	175,	7,	135,	39,	167,	13,	141,	45,	173,	5,	133,	37,	165,	207},
{79,	239,	111,	199,	71,	231,	103,	205,	77,	237,	109,	197,	69,	229,	101,	63},
{191,	31,	159,	55,	183,	23,	151,	61,	189,	29,	157,	53,	181,	21,	149,	254},
{254,	127,	223,	95,	247,	119,	215,	87,	253,	125,	221,	93,	245,	117,	213,	85}};


Errc PGray2BW( const Img2duc &ims, Img2duc &imd ) {
   register int	x,y;
   
   for (y=0;y<ims.Height();y++)
      for (x=0;x<ims.Width();x++)
	 imd[y][x]=(ims[y][x]>=matrice[y%PLOT][x%PLOT])? MAXUCHAR : 0;
   
   return SUCCESS;
}

Errc PGray2BW( const Img3duc &ims, Img3duc &imd ) {
   register int	x,y,z;
   
   for (z=0;z<ims.Depth();z++)
      for (y=0;y<ims.Height();y++)
	 for (x=0;x<ims.Width();x++)
	    imd[z][y][x]=(ims[z][y][x]>=matrice[y%PLOT][x%PLOT])? MAXUCHAR : 0;
   
   return SUCCESS;
}

#ifdef MAIN

/*
 * Modify only the following constants, and the function call.
 */
#define	USAGE	"usage: %s [-m mask] [in_im|-] [out_im|-]"
#define	PARC	0
#define	FINC	1
#define	FOUTC	1
#define	MASK	1

int main( int argc, char *argv[] ) {
   Errc result;              // The result code of the execution.
   Pobject* mask;            // The region mask.
   Pobject* objin[FINC+1];   // The input objects.
   Pobject* objs[FINC+1];    // The source objects masked by the mask.
   Pobject* objout[FOUTC+1]; // The output object.
   Pobject* objd[FOUTC+1];   // The result object of the execution.
   char* parv[PARC+1];       // The input parameters.
   
   ReadArgs(argc,argv,PARC,FINC,FOUTC,&mask,objin,objs,objout,objd,parv,USAGE,MASK);
   
   switch(objs[0]->Type()) {
   case Po_Img2duc : {
      Img2duc* const ims=(Img2duc*)objs[0];
      objd[0]=new Img2duc(ims->Props());
      Img2duc* const imd=(Img2duc*)objd[0];
      
      result=PGray2BW(*ims,*imd);
   } break;
   case Po_Img3duc : {
      Img3duc* const ims=(Img3duc*)objs[0];
      objd[0]=new Img3duc(ims->Props());
      Img3duc* const imd=(Img3duc*)objd[0];
      
      result=PGray2BW(*ims,*imd);
   } break;
   default :
      PrintErrorFormat(objin,FINC);
      result=FAILURE;
   }
   
   if (result) {
      WriteArgs(argc,argv,PARC,FINC,FOUTC,&mask,objin,objs,objout,objd,MASK);
   }
   Exit(result);
   return 0;
}

#endif
