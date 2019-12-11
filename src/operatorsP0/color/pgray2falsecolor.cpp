/* -*- c-basic-offset: 3; mode: c++ -*-
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

#define _USE_MATH_DEFINES
#include <cmath>
#include <pandore.h>
using namespace pandore;

/**
 * @file pgray2falsecolor.cpp	
 * Converts a gray scale image to false color image.
 *
 * @param lut the chosen algorithm.
 * 	algorithms are
 *              0 : rainbow.
 * 	        1 : regions
 */


/**
 * 
 * @param lut the chosen algorithm.
 */
int ** build( int lutId, int ncols ) {
   int **lut = (int**)malloc(3 * sizeof(int*));
   for (int c = 0; c < 3; c++) {
      lut[c] = (int*)malloc(ncols * sizeof(int));
   }
   int i;
   switch(lutId) {
   case 0: { // RAINBOW
     for (i=0; i<ncols; i++) {
       int j = ncols-1-i;
       double d =cos((double)((j-ncols*0.16) *(M_PI/ncols)));
       d = (d < 0.0) ? 0.0 : d;
       lut[2][i] = (Uchar)(d*ncols);
       d = (d=cos((double)((j-ncols*0.52) *(M_PI/ncols)))) < 0.0 ? 0.0 :d;
       lut[1][i] = (Uchar)(d*ncols);
       d = (d=cos((double)((j-ncols*0.82) *(M_PI/ncols)))) < 0.0 ? 0.0 :d;
       double e=cos((double)((j  *(M_PI/ncols))));
       e = (e < 0.0)? 0.0 : e;
       lut[0][i] = (Uchar)(d*ncols + e *(ncols/2));
     }
     break;
   case 1: { // REGION
      lut[0][0]=0;
      lut[1][0]=0;
      lut[2][0]=0;
      for (i=1; i<ncols; i++) { 
	 switch(i%15) {
	    break;
	 case 0:{
	    // red
	    lut[0][i]=255;
	    lut[1][i]=0;
	    lut[2][i]=0;
	 }
	    break;
	case 1:{
	    // green
	    lut[0][i]=0;
	    lut[1][i]=255;
	    lut[2][i]=0;
	 }
	    break;
	 case 2:{
	    // blue
	    lut[0][i]=0;
	    lut[1][i]=0;
	    lut[2][i]=255;
	 }
	    break;
	 case 3:{
	    // yellow
	    lut[0][i]=255;
	    lut[1][i]=255;
	    lut[2][i]=0;
	 }
	    break;
	 case 4:{
	    // dark olive green
	    lut[0][i]=85;
	    lut[1][i]=107;
	    lut[2][i]=47;
	 }
	    break;
	 case 14:{
	    // magenta
	    lut[0][i]=255;
	    lut[1][i]=0;
	    lut[2][i]=255;
	 }
	    break;
	 case 5:{
	    // cyan
	    lut[0][i]=0;
	    lut[1][i]=255;
	    lut[2][i]=255;
	 }
	    break;
	 case 6:{
	    // chocolat
	    lut[0][i]=210;
	    lut[1][i]=105;
	    lut[2][i]=30;
	 }
	    break;
	 case 7:{
	    // gray
	    lut[0][i]=127;
	    lut[1][i]=127;
	    lut[2][i]=127;
	 }
	    break;
	 case 8:{
	    // violet
	    lut[0][i]=238;
	    lut[1][i]=130;
	    lut[2][i]=238;
	 }
	    break;
	 case 9:{
	    // brown
	    lut[0][i]=165;
	    lut[1][i]=42;
	    lut[2][i]=42;
	 }
	    break;
	 case 10:{
	    // orange
	    lut[0][i]=255;
	    lut[1][i]=165;
	    lut[2][i]=0;
	 }
	    break;
	 case 11:{
	    // gold4
	    lut[0][i]=139;
	    lut[1][i]=117;
	    lut[2][i]=0;
	 }
	    break;
	 case 12:{
	    // mistyrose2
	    lut[0][i]=138;
	    lut[1][i]=213;
	    lut[2][i]=210;
	 }
	    break;
	 case 13:{
	    // white
	    lut[0][i]=255;
	    lut[1][i]=255;
	    lut[2][i]=255;
	 }
	    break;
	 }
      }
   }
   }
      break;
   default: break;
   }
   return lut; 
}

/**
 * Remplace chaque niveau de gris par sa probabilite d'etre un point noir.
 */

Errc PGray2FalseColor( const Img2duc &ims, Imc2duc &imd, int lutId) {
  Point2d p;
  int **lut = NULL;

  lut = build(lutId, 256);
  if (lut == NULL) {
     std::cerr << "Error pgray2falsecolor: Unknow lut index "<< lutId << std::endl;
    return FAILURE;
  }
   for (p.y = 0; p.y < ims.Height(); p.y++)
   for (p.x = 0; p.x < ims.Width(); p.x++)
  {
     Img2duc::ValueType val = ims[p];
     imd.X[p] = lut[0][val];
     imd.Y[p] = lut[1][val];
     imd.Z[p] = lut[2][val];
  }
  return SUCCESS;
}

Errc PGray2FalseColor( const Img3duc &ims, Imc3duc &imd, int lutId) {
  Point3d p;
  int **lut = NULL;

  lut = build(lutId, 256);
  if (lut == NULL) {
     std::cerr << "Error pgray2falsecolor: Unknow lut index "<< lutId << std::endl;
    return FAILURE;
  }
   for (p.z = 0; p.z < ims.Depth(); p.z++)
   for (p.y = 0; p.y < ims.Height(); p.y++)
   for (p.x = 0; p.x < ims.Width(); p.x++)
  {
     Img3duc::ValueType val = ims[p];
     imd.X[p] = lut[0][val];
     imd.Y[p] = lut[1][val];
     imd.Z[p] = lut[2][val];
  }
  return SUCCESS;
}


#ifdef MAIN
#define	USAGE	"usage: %s lut [-m mask] [in_im|-] [out_im|-]"
#define	PARC	1
#define	FINC	1
#define	FOUTC	1
#define	MASK	1
int main( int argc, char *argv[] ) {
   Errc result;                // The result code of the execution.
   Pobject* mask;              // The region map.
   Pobject* objin[FINC + 1];   // The input objects.
   Pobject* objs[FINC + 1];    // The source objects masked.
   Pobject* objout[FOUTC + 1]; // The output object.
   Pobject* objd[FOUTC + 1];   // The result object of the execution.
   char* parv[PARC + 1];       // The input parameters.

   ReadArgs(argc, argv, PARC, FINC, FOUTC, &mask, objin, objs, objout, objd, parv, USAGE, MASK); 
   if (objs[0]->Type() == Po_Img2duc) {
      Img2duc* const ims = (Img2duc*)objs[0];
      objd[0] = new Imc2duc(ims->Size());
      Imc2duc* const imd = (Imc2duc*)objd[0];
      
      result = PGray2FalseColor(*ims, *imd, atoi(parv[0]));
      goto end;
   }
   if (objs[0]->Type() == Po_Img3duc) {
      Img3duc* const ims = (Img3duc*)objs[0];
      objd[0] = new Imc3duc(ims->Size());
      Imc3duc* const imd = (Imc3duc*)objd[0];
      
      result = PGray2FalseColor(*ims, *imd, atoi(parv[0]));
      goto end;
   }
  {
     PrintErrorFormat(objin, FINC, argv); 
     result = FAILURE; 
  }	

end:
  if (result) {
	WriteArgs(argc, argv, PARC, FINC, FOUTC, &mask, objin, objs, objout, objd, MASK); 
  }
  Exit(result); 
  return 0; 
}
#endif
