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
 * @author Régis Clouard - May 06, 2010
 */

/**
 * @file ppolynomialfitting.cpp
 *
 * Fits a polynomial to the image data.
 * Reference:  based on Mickeal Schmid.
 */

#include <stdexcept>
#include <pandore.h>
using namespace pandore;

/**
 * Inverts a symmetric n*n matrix.
 * Needs a well-conditioned matrix.
 *
 * @return false on error (singular matrix).
 */
void invertSymmetricMatrix( double **matrix , int n ) {
   int *processed = new int[n];
   double *qq = new double[n];
   double *pp = new double[n];
   double pivot0 = 1e-30;	// absolute value of the first (largest) pivot
   
   for (int j=0; j<n; j++)	// find pivot
      processed[j]=0;
   
   for (int i=0; i<n; i++) {       // loop: process all matrix lines&columns
      double max = pivot0 * 1e-14; // pivot must be bigger than this
      int iPivot = -1;
      for (int j=0; j<n; j++) {    // find pivot
	 if (!processed[j]) {
	    double abs = matrix[j][j];
	    abs = abs<0 ? -abs : abs;
	    if (abs > max)  {
	       max = abs;
	       iPivot = j;
	    }
	 }
      }
      if (iPivot < 0) {
	 delete[] processed;
	 delete[] qq;
	 delete[] pp;
	 throw std::runtime_error("Error: Negative pivot.");
      }
      if (i==0)
	 pivot0 = max;
      processed[iPivot] = true;
      pp[iPivot] = 1.;
      double q = 1./matrix[iPivot][iPivot];
      qq[iPivot] = q;
      matrix[iPivot][iPivot] = 0.;
      for (int j=0; j<iPivot; j++) {
	 pp[j] = matrix[j][iPivot];
	 qq[j] = matrix[j][iPivot]*q*(processed[j]?1:-1);
	 matrix[j][iPivot] = 0.;
      }
      for (int j=iPivot+1; j<n; j++) {
	 pp[j] = matrix[iPivot][j]*(processed[j]?-1:1);
	 qq[j] = -matrix[iPivot][j]*q;
	 matrix[iPivot][j] = 0.;
      }
      for (int j=0; j<n; j++) {
	 for (int k=j; k<n; k++) {
	    matrix[j][k] += pp[j]*qq[k];
	 }
      }
   }

   for (int i=1; i<n; i++) { //fill bottom left to make symmetric again
      for (int j=0; j<i; j++) {
	 matrix[i][j] = matrix[j][i];
      }
   }

   delete[] processed;
   delete[] qq;
   delete[] pp;
}

/** The matrix used for the fit */
double **matrix;
/** The powers of x and y for each element of the base */
int *xPower, *yPower;
/**
 * A list of the powers of all x & y coordinates required, arranged as
 * 0^0, 0^1, 0^(endPower-1), 1^0, 1^1, ... 1^(endPower-1), ... 
 * for better numeric performance, coordinates are transformed to
 * -1.5 ... 1.5 range within the roi.
 */
double *powerOfX, *powerOfY;
/** The highest power in powerOfN, plus 1*/
int endPower;

/** The number of  base functions (and parameter) in the fit. */
int baseSize;

/**
 * Calculates the base, tabulate powers of integers required and
 * calculates the matrix needed for fitting.
 */
template <typename T>
Errc calculateMatrixEtc( const Imx3d<T> &ims, const Uchar *mask, int xOrder, int yOrder, int xyOrder ) {
   // t h e   b a s e
   baseSize = xyOrder*(xyOrder-1)/2;
   if (baseSize < 0) baseSize = 0;
   baseSize += xOrder + yOrder + 1;
   xPower = new int[baseSize];
   yPower = new int[baseSize];
   int iBase = 0;                      //get powers of x, y in the base
   for (int ix=0; ix<=xOrder; ix++, iBase++) {
      xPower[iBase] = ix;
      yPower[iBase] = 0;
   }
   for (int iy=1; iy<=yOrder; iy++, iBase++) {
      yPower[iBase] = iy;
      xPower[iBase] = 0;
   }
   for (int ix=1; ix<xyOrder; ix++) {
      for (int iy=1; ix+iy<=xyOrder; iy++, iBase++) {
	 xPower[iBase] = ix;
	 yPower[iBase] = iy;
      }
   }

   endPower = xOrder > yOrder ? xOrder : yOrder;
   if (xyOrder-1 > endPower) endPower = xyOrder-1;
   endPower++;
   powerOfX = new double[endPower*ims.Width()];
   powerOfY = new double[endPower*ims.Height()];

   for (int x=0,p=0; x<ims.Width(); x++) {    //tabulate powers of x coordinates
      double power = 1.;
      double xScaled = 3*(x-0)/(double)ims.Width()-1.5; //scale to get well-conditioned matrix
      for (int iPow = 0; iPow<endPower; iPow++,p++) {
	 powerOfX[p] = power;
	 power *= xScaled;
      }
   } 
   for (int y=0,p=0; y<ims.Height(); y++) {    //tabulate powers of y coordinates
      double power = 1.;
      double yScaled = 3*(y-0)/(double)ims.Height()-1.5;
      for (int iPow = 0; iPow<endPower; iPow++,p++) {
	 powerOfY[p] = power;
	 power *= yScaled;
      }
   }

   // t h e   m a t r i x

   matrix = new double*[baseSize];
   for (int i=0; i<baseSize; i++) {
      matrix[i]= new double[baseSize];
      for (int j=0; j<baseSize; j++) {
	 matrix[i][j]=0;
      }
   }

   for (int y=1, powerY=y*endPower; y<ims.Height()-1; y++, powerY+=endPower) {
      for (int x=1, p=ims.Width()*y+x, m=(y-0)*ims.Width(), powerX=x*endPower; x<ims.Width()-1; x++, p++, m++, powerX+=endPower) {
	 if (mask[m] != 0) {
	    for (iBase=0; iBase<baseSize; iBase++) {
	       double fct = powerOfX[powerX+xPower[iBase]]*powerOfY[powerY+yPower[iBase]];
	       matrix[iBase][iBase] += fct*fct;
	       for (int iBase1=0; iBase1<iBase; iBase1++) {
		  double fct1 = powerOfX[powerX+xPower[iBase1]]*powerOfY[powerY+yPower[iBase1]];
		  matrix[iBase][iBase1] += fct*fct1;
		  matrix[iBase1][iBase] += fct*fct1;
	       }
	    }
	 }
      }
   }
   invertSymmetricMatrix(matrix,baseSize);
   return SUCCESS;
}

template <typename T>
Errc PPolynomialFitting( const Imx2d<T> &ims, const Img2d<Uchar> &imm, Imx2d<T> &imd, int xOrder, int yOrder, int xyOrder ) {
   const T max = Limits<T>::max();
   const T min = Limits<T>::min();

   if (xOrder<0 || xOrder>10 || yOrder<0 || yOrder>10 || xyOrder<0 || xyOrder>10 ) {
      throw std::invalid_argument("Error: Bad parameter values");
   }

   Uchar *mask = imm.Vector();
   if (!calculateMatrixEtc(ims, mask, xOrder, yOrder, xyOrder)) {
      throw std::runtime_error("Error: Problem during processing...");
   }

   double *vector = new double[baseSize];         //calculate fit
   double *coeff = new double[baseSize];
   for (int b=0; b<ims.Bands(); b++ ) {
      for (int i=0; i<baseSize; i++) {
	 vector[i]=0;
	 coeff[i]=0;
      }

      T *pixels = ims.Vector(b);
      for (int y=0, powerY=y*endPower; y<0+ims.Height(); y++, powerY+=endPower) {
	 for (int x=0, p=ims.Width()*y+x, m=(y-0)*ims.Width(), powerX=x*endPower; x<ims.Width(); x++, p++, m++, powerX+=endPower) {
	    if (mask[m] != 0) {
	       for (int iBase=0; iBase<baseSize; iBase++) {
		  vector[iBase] += pixels[p]*powerOfX[powerX+xPower[iBase]]*powerOfY[powerY+yPower[iBase]];
	       }
	    }
	 }
      }
      for (int i=0; i<baseSize; i++) {
	 for (int j=0; j<baseSize; j++) {
	    coeff[i] += matrix[i][j]*vector[j];
	 }
      }
      // apply fit
      T *outpixels = imd.Vector(b);
      for (int y=0, p=0, powerY=0; y<ims.Height(); y++, powerY+=endPower) {
	 for (int x=0, powerX=0; x<ims.Width(); x++, p++, powerX+=endPower) {
	    double background = 0.;
	    for (int iBase=0; iBase<baseSize; iBase++) {
	       background += coeff[iBase]*powerOfX[powerX+xPower[iBase]]*powerOfY[powerY+yPower[iBase]];
	    }
	    if (background<min) {
	       outpixels[p]=min;
	    } else if (background>max) {
	       outpixels[p]= max;
	    } else {
	       outpixels[p] = (T)background;}
	 }
      }
   }
   delete[] vector;
   delete[] coeff;
   delete[] xPower;
   delete[] yPower;
   delete[] powerOfX;
   delete[] powerOfY;
   for (int i=0; i<baseSize; i++) {
      delete[] matrix[i];
   }
   delete[] matrix;
   return SUCCESS;
}



#ifdef MAIN
#define	USAGE	"usage: %s xorder yorder xyorder [im_in|-] [im_msk|-] [im_out|-]"
#define	PARC	3
#define	FINC	2
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
if ((objs[0]->Type()==Po_Img2duc) && (objs[1]->Type()==Po_Img2duc)) {
   Img2duc *const ims=(Img2duc *)objs[0];
   Img2duc *const imm=(Img2duc *)objs[1];
   objd[0]=new Img2duc(ims->Props());
   Img2duc *imd=(Img2duc*)objd[0];

   try {
      result=PPolynomialFitting(*ims, *imm, *imd, atoi(parv[0]), atoi(parv[1]), atoi(parv[2]));
   } catch(const std::exception &e ) {
      std::cerr << "Error ppolynomialfitting: " <<e.what() <<std::endl;
      result=FAILURE;
   }
   goto end;
}
if ((objs[0]->Type()==Po_Img2dsl) && (objs[1]->Type()==Po_Img2duc)) {
   Img2dsl *const ims=(Img2dsl *)objs[0];
   Img2duc *const imm=(Img2duc *)objs[1];
   objd[0]=new Img2dsl(ims->Props());
   Img2dsl *imd=(Img2dsl*)objd[0];

   try {
      result=PPolynomialFitting(*ims, *imm, *imd, atoi(parv[0]), atoi(parv[1]), atoi(parv[2]));
   } catch(const std::exception &e ) {
      std::cerr << "Error ppolynomialfitting: " <<e.what() <<std::endl;
      result=FAILURE;
   }
   goto end;
}
if ((objs[0]->Type()==Po_Img2dsf) && (objs[1]->Type()==Po_Img2duc)) {
   Img2dsf *const ims=(Img2dsf *)objs[0];
   Img2duc *const imm=(Img2duc *)objs[1];
   objd[0]=new Img2dsf(ims->Props());
   Img2dsf *imd=(Img2dsf*)objd[0];

   try {
      result=PPolynomialFitting(*ims, *imm, *imd, atoi(parv[0]), atoi(parv[1]), atoi(parv[2]));
   } catch(const std::exception &e ) {
      std::cerr << "Error ppolynomialfitting: " <<e.what() <<std::endl;
      result=FAILURE;
   }
   goto end;
}
if ((objs[0]->Type()==Po_Imx2duc) && (objs[1]->Type()==Po_Img2duc)) {
   Imx2duc *const ims=(Imx2duc *)objs[0];
   Img2duc *const imm=(Img2duc *)objs[1];
   objd[0]=new Imx2duc(ims->Props());
   Imx2duc *imd=(Imx2duc*)objd[0];

   try {
      result=PPolynomialFitting(*ims, *imm, *imd, atoi(parv[0]), atoi(parv[1]), atoi(parv[2]));
   } catch(const std::exception &e ) {
      std::cerr << "Error ppolynomialfitting: " <<e.what() <<std::endl;
      result=FAILURE;
   }
   goto end;
}
if ((objs[0]->Type()==Po_Imx2dsl) && (objs[1]->Type()==Po_Img2duc)) {
   Imx2dsl *const ims=(Imx2dsl *)objs[0];
   Img2duc *const imm=(Img2duc *)objs[1];
   objd[0]=new Imx2dsl(ims->Props());
   Imx2dsl *imd=(Imx2dsl*)objd[0];

   try {
      result=PPolynomialFitting(*ims, *imm, *imd, atoi(parv[0]), atoi(parv[1]), atoi(parv[2]));
   } catch(const std::exception &e ) {
      std::cerr << "Error ppolynomialfitting: " <<e.what() <<std::endl;
      result=FAILURE;
   }
   goto end;
}
if ((objs[0]->Type()==Po_Imx2dsf) && (objs[1]->Type()==Po_Img2duc)) {
   Imx2dsf *const ims=(Imx2dsf *)objs[0];
   Img2duc *const imm=(Img2duc *)objs[1];
   objd[0]=new Imx2dsf(ims->Props());
   Imx2dsf *imd=(Imx2dsf*)objd[0];

   try {
      result=PPolynomialFitting(*ims, *imm, *imd, atoi(parv[0]), atoi(parv[1]), atoi(parv[2]));
   } catch(const std::exception &e ) {
      std::cerr << "Error ppolynomialfitting: " <<e.what() <<std::endl;
      result=FAILURE;
   }
   goto end;
}
if ((objs[0]->Type()==Po_Imc2duc) && (objs[1]->Type()==Po_Img2duc)) {
   Imc2duc *const ims=(Imc2duc *)objs[0];
   Img2duc *const imm=(Img2duc *)objs[1];
   objd[0]=new Imc2duc(ims->Props());
   Imc2duc *imd=(Imc2duc*)objd[0];

   try {
      result=PPolynomialFitting(*ims, *imm, *imd, atoi(parv[0]), atoi(parv[1]), atoi(parv[2]));
   } catch(const std::exception &e ) {
      std::cerr << "Error ppolynomialfitting: " <<e.what() <<std::endl;
      result=FAILURE;
   }
   goto end;
}
if ((objs[0]->Type()==Po_Imc2dsl) && (objs[1]->Type()==Po_Img2duc)) {
   Imc2dsl *const ims=(Imc2dsl *)objs[0];
   Img2duc *const imm=(Img2duc *)objs[1];
   objd[0]=new Imc2dsl(ims->Props());
   Imc2dsl *imd=(Imc2dsl*)objd[0];

   try {
      result=PPolynomialFitting(*ims, *imm, *imd, atoi(parv[0]), atoi(parv[1]), atoi(parv[2]));
   } catch(const std::exception &e ) {
      std::cerr << "Error ppolynomialfitting: " <<e.what() <<std::endl;
      result=FAILURE;
   }
   goto end;
}
if ((objs[0]->Type()==Po_Imc2dsf) && (objs[1]->Type()==Po_Img2duc)) {
   Imc2dsf *const ims=(Imc2dsf *)objs[0];
   Img2duc *const imm=(Img2duc *)objs[1];
   objd[0]=new Imc2dsf(ims->Props());
   Imc2dsf *imd=(Imc2dsf*)objd[0];

   try {
      result=PPolynomialFitting(*ims, *imm, *imd, atoi(parv[0]), atoi(parv[1]), atoi(parv[2]));
   } catch(const std::exception &e ) {
      std::cerr << "Error ppolynomialfitting: " <<e.what() <<std::endl;
      result=FAILURE;
   }
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
