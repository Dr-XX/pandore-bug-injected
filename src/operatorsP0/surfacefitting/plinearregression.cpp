/* -*- c-basic-offset: 3;mode:c++ -*-
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
 * @author Régis Clouard - May 06, 2010
 */

/**
 * @file plinearregression.cpp
 *
 * The algorithm computes orthogonal linear regression
 * (vertical, horizontal, depth)
 * in order to convert any arbitrary image into a flat
 * background image without any pictorial content.
 * (inspired from V. Miszalok, 2006)
 */

#include <pandore.h>
using namespace pandore;

template <typename T>
void RegressionX( const Imx2d<T> &ims, float ***slope, float ***y0 ) {
   const float mid = ims.Height()/2.f;

   float sy2=0.f;
   for ( Long y=0; y < ims.Height(); y++ ) {
      sy2 += (y-mid)*(y-mid);
   }
   for ( Long b=0; b < ims.Bands(); b++ ) {
      for ( Long x=0; x < ims.Width(); x++ ) {
	 float sum = 0;
	 for (Long y=0; y < ims.Height(); y++ ) {
	    sum += (float)ims(b,y,x);
	 }
	 float mean = sum / ims.Height();
	 float sxy = 0.f;
	 for ( Long y=0; y < ims.Height(); y++ ) {
	    sxy += (y-mid)*(ims(b,y,x) - mean);
	 }
	 slope[1][b][x] = sxy / sy2;
	 y0   [1][b][x] = mean - slope[1][b][x]*mid;
      }
   }
}

template <typename T>
void RegressionY( float ***slope, float ***y0, const Imx2d<T> &imd) {
   const float mid = imd.Width()/2.f;
   
   float sx2=0.f;
   for ( Long x=0; x < imd.Width(); x++ ) { 
      sx2 += (x-mid)*(x-mid);
   }
   for ( Long b=0; b < imd.Bands(); b++ ) {
      for ( Long y=0; y < imd.Height(); y++ ) {
	 float sum = 0;
	 for ( Long x=0; x < imd.Width(); x++ ) {
	    sum += slope[1][b][x]*y + y0[1][b][x];
	 }
	 float mean = sum / imd.Width();
	 float sxy = 0.f;
	 for ( Long x=0; x < imd.Width(); x++ ) { 
	    sxy += (x-mid)*(slope[1][b][x]*y + y0[1][b][x] - mean);
	 }
	 slope[0][b][y] = sxy / sx2;
	 y0   [0][b][y] = mean - slope[0][b][y]*mid;
      }
   }
}

template <typename T>
Errc PlinearRegression( const Imx2d<T> &ims, Imx2d<T> &imd ) {
   int sizemax = MAX(ims.Height(),ims.Width());

   float ***slope;
   float ***y0;
   try {
      slope = new float **[2];
      for (int i=0;i<2; i++) {
	 slope[i] = new float *[ims.Bands()];
	 for (int j=0;j<ims.Bands(); j++) {
	    slope[i][j] = new float[sizemax];
	 }
      }
      
      y0 = new float **[2];
      for (int i=0;i<2; i++) {
	 y0[i] = new float *[ims.Bands()];
	 for (int j=0;j<ims.Bands(); j++) {
	    y0[i][j] = new float[sizemax];
	 }
      }
   } catch (std::bad_alloc) {
      std::cerr<<"Error plinearregression: No more memory." <<std::endl;
      return FAILURE;
   }

   RegressionX(ims,slope,y0);
   RegressionY(slope, y0, imd);

   const T max = Limits<T>::max();
   const T min = Limits<T>::min();
   for ( int y=0; y < imd.Height(); y++ ) {
      for ( int x=0; x < imd.Width(); x++ ) {
	 for ( int b=0; b < ims.Bands(); b++ ) { 
	    Float c = (Float)( slope[0][b][y]*x + y0[0][b][y] );
	    if (c < min) {
	       imd(b,y,x) = min;
	    } else if (c > max) {
	       imd(b,y,x) = max;
	    } else {
	       imd(b,y,x) = (T)c;
	    }
	 }
      }
   }
   
   // Delete arrays
   for (int i=0;i<2; i++) {
      for (int j=0;j<ims.Bands(); j++) {
	 delete slope[i][j];
	 delete y0[i][j];
      }
      delete[] slope[i];
      delete[] y0[i];
   }
   delete[] slope;
   delete[] y0;

   return SUCCESS;
}



#ifdef MAIN
#define	USAGE	"usage: %s [im_in|-] [im_out|-]"
#define	PARC	0
#define	FINC	1
#define	FOUTC	1
#define	MASK	0

int main( int argc, char *argv[] ) {
   Errc result;                // The result code of the execution.
   Pobject* mask;              // The region map.
   Pobject* objin[FINC + 1];   // The input objects.
   Pobject* objs[FINC + 1];    // The source objects masked.
   Pobject* objout[FOUTC + 1]; // The output object.
   Pobject* objd[FOUTC + 1];   // The result object of the execution.
   char* parv[PARC + 1];       // The input parameters.

   ReadArgs(argc, argv, PARC, FINC, FOUTC, &mask, objin, objs, objout, objd, parv, USAGE, MASK); 
   if (objs[0]->Type()==Po_Img2duc) {
      Img2duc *const ims=(Img2duc *)objs[0];
      objd[0]=new Img2duc(ims->Props());
      Img2duc *const imd=(Img2duc *)objd[0];

      result=PlinearRegression(*ims,*imd);
      goto end;
   }
   if (objs[0]->Type()==Po_Img2dsl) {
      Img2dsl *const ims=(Img2dsl *)objs[0];
      objd[0]=new Img2dsl(ims->Props());
      Img2dsl *const imd=(Img2dsl *)objd[0];

      result=PlinearRegression(*ims,*imd);
      goto end;
   }
   if (objs[0]->Type()==Po_Img2dsf) {
      Img2dsf *const ims=(Img2dsf *)objs[0];
      objd[0]=new Img2dsf(ims->Props());
      Img2dsf *const imd=(Img2dsf *)objd[0];

      result=PlinearRegression(*ims,*imd);
      goto end;
   }
   if (objs[0]->Type()==Po_Imc2duc) {
      Imc2duc *const ims=(Imc2duc *)objs[0];
      objd[0]=new Imc2duc(ims->Props());
      Imc2duc *const imd=(Imc2duc *)objd[0];

      result=PlinearRegression(*ims,*imd);
      goto end;
   }
   if (objs[0]->Type()==Po_Imc2dsl) {
      Imc2dsl *const ims=(Imc2dsl *)objs[0];
      objd[0]=new Imc2dsl(ims->Props());
      Imc2dsl *const imd=(Imc2dsl *)objd[0];

      result=PlinearRegression(*ims,*imd);
      goto end;
   }
   if (objs[0]->Type()==Po_Imc2dsf) {
      Imc2dsf *const ims=(Imc2dsf *)objs[0];
      objd[0]=new Imc2dsf(ims->Props());
      Imc2dsf *const imd=(Imc2dsf *)objd[0];

      result=PlinearRegression(*ims,*imd);
      goto end;
   }
   if (objs[0]->Type()==Po_Imx2duc) {
      Imx2duc *const ims=(Imx2duc *)objs[0];
      objd[0]=new Imx2duc(ims->Props());
      Imx2duc *const imd=(Imx2duc *)objd[0];

      result=PlinearRegression(*ims,*imd);
      goto end;
   }
   if (objs[0]->Type()==Po_Imx2dsl) {
      Imx2dsl *const ims=(Imx2dsl *)objs[0];
      objd[0]=new Imx2dsl(ims->Props());
      Imx2dsl *const imd=(Imx2dsl *)objd[0];

      result=PlinearRegression(*ims,*imd);
      goto end;
   }
   if (objs[0]->Type()==Po_Imx2dsf) {
      Imx2dsf *const ims=(Imx2dsf *)objs[0];
      objd[0]=new Imx2dsf(ims->Props());
      Imx2dsf *const imd=(Imx2dsf *)objd[0];

      result=PlinearRegression(*ims,*imd);
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
