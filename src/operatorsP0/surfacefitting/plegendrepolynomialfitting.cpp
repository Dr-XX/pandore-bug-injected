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
 * @file plegendrepolynomialfitting.cpp
 *
 * @bref Approximates an image by a polynomial.
 *
 * Uses the orthogonality relation of the Legendre polynomials
 * to expand an image as a double sum of those functions.
 * The sum is then evaluated to produce an image that approximates
 * a projection onto the space of polynomial images.
 * Another description is a least squares fit.
 * Reference: Bob Doughery, 2005 (ImageJ Plugin)
 */

#include <stdexcept>
#include <pandore.h>
using namespace pandore;

/**
 * Uses the recurrence relation to evaluate a
 * series of Legendre polynomials at t.
 */
double *Legendre( double x, int n ) {
   double *p = new double[n];
   p[0] = 1;
   if (n > 1) {
      p[1] = x;
      for ( int l = 1; l < (n-1); l++ ) {
	 p[l+1] = ((2*l + 1)*x*p[l] - l*p[l-1])/(l + 1);
      }
   }
   return p;
}

double *normX, *normY;
double **px, **py;
double *wx, *wy;
double **a;

template <typename T>
Errc filter( const Imx2d<T> &ims, Imx2d<T> &imd, int nx, int ny ) {
   const T max = Limits<T>::max();
   const T min = Limits<T>::min();
   
   for (int b = 0; b < ims.Bands(); b++) {
      const T *data = ims.Vector(b);
      T *out = imd.Vector(b);
      for (int my = 0; my < ny; my++) {
	 for (int mx = 0; mx < nx; mx++) {
	    double sum = 0;
	    for (int j = 0; j < ims.Height(); j++) {
	       for (int i = 0; i < ims.Width(); i++) {
		  sum += wx[i]*wy[j]*px[i][mx]*py[j][my]*data[i + ims.Width()*j];
	       }
	    }
	    a[mx][my] = sum/(normX[mx]*normY[my]);
	 }
      }
      
      // Use the computed coefficients to evaluate the new image
      for (int j = 0; j < imd.Height(); j++) {
	 for (int i = 0; i < imd.Width(); i++) {
	    double sum = 0;
	    for (int my = 0; my < ny; my++) {
	       for (int mx = 0; mx < nx; mx++) {
		  sum += a[mx][my]*px[i][mx]*py[j][my];
	       }
	    }
	    if (sum<min) {
	       out[i + j*imd.Width()] = min;
	    } else if (sum>max) {
	       out[i + j*imd.Width()] = max;
	    } else {
	       out[i + j*imd.Width()] = (T)sum;
	    }
	 }
      }
   }

   return SUCCESS;
}

/**
 * @param nx degree x (0 to w-2). Default value 2.
 * @param ny degree y (0 to h-2). Default value 2.
 */
template <typename T>
Errc PLegendrePolynomialFitting( const Imx2d<T> &ims, Imx2d<T> &imd, int nx, int ny ) {
   if ( nx < 0 || nx > 10 || ny < 0 || ny > 10) {
      throw std::invalid_argument("Error: Bad parameter values.");
   }
   nx++;; ny++; // ??

   // Compute legendre polynomial in the roi.
   px = new double*[ims.Width()];
   wx = new double[ims.Width()];
   double hx = 2;
   if (ims.Width() > 1) hx = 2./(ims.Width()-1);
   for (int i = 0; i < ims.Width(); i++) {
      double tx = -1 + i*hx;
      px[i] = Legendre(tx, nx);
      wx[i] = 1;
   }
   if (ims.Height() >= 9) {
      wx[0] = 17./48.;
      wx[1] = 59./48.;
      wx[2] = 43./48.;
      wx[3] = 49./48.;
      wx[ims.Width()-1] = 17./48.;
      wx[ims.Width()-2] = 59./48.;
      wx[ims.Width()-3] = 43./48.;
      wx[ims.Width()-4] = 49./48.;
   } else if (ims.Width() == 8) {
      wx[0] = 7.*751./17280;
      wx[1] = 7.*3577./17280;
      wx[2] = 7.*1323./17280;
      wx[3] = 7.*2989./17280;
      wx[4] = 7.*2989./17280;
      wx[5] = 7.*1323./17280;
      wx[6] = 7.*3577./17280;
      wx[7] = 7.*751./17280;
   } else if (ims.Width() == 7) {
      wx[0] = 41./140.;
      wx[1] = 216./140.;
      wx[2] = 27./140.;
      wx[3] = 272./140.;
      wx[4] = 27./140.;
      wx[5] = 216./140.;
      wx[6] = 41./140.;
   } else if (ims.Width() == 6) {
      wx[0] = 5.*19./288.;
      wx[1] = 5.*75./288.;
      wx[2] = 5.*50./288.;
      wx[3] = 5.*50./288.;
      wx[4] = 5.*75./288.;
      wx[5] = 5.*19./288.;
   } else if (ims.Width() == 5) {
      wx[0] = 2.*7./45.;
      wx[1] = 32.*7./45.;
      wx[2] = 12.*7./45.;
      wx[3] = 32.*7./45.;
      wx[4] = 2.*7./45.;
   } else if (ims.Width() == 4) {
      wx[0] = 3.*1./8.;
      wx[1] = 3.*3./8.;
      wx[2] = 3.*3./8.;
      wx[3] = 3.*1./8.;
   } else if (ims.Width() == 3) {
      wx[0] = 1./3.;
      wx[1] = 4./3.;
      wx[2] = 1./3.;
   }
   py = new double*[ims.Height()];
   wy = new double[ims.Height()];
   double hy = 2;
   if (ims.Height() > 1) hy = 2./(ims.Height()-1);
   for (int j = 0; j < ims.Height(); j++) {
      double ty = -1 + j*hy;
      py[j] = Legendre(ty, ny);
      wy[j] = 1;
   }
   if (ims.Height() >= 9) {
      wy[0] = 17./48.;
      wy[1] = 59./48.;
      wy[2] = 43./48.;
      wy[3] = 49./48.;
      wy[ims.Height()-1] = 17./48.;
      wy[ims.Height()-2] = 59./48.;
      wy[ims.Height()-3] = 43./48.;
      wy[ims.Height()-4] = 49./48.;
   } else if (ims.Height() == 8) {
      wy[0] = 7.*751./17280;
      wy[1] = 7.*3577./17280;
      wy[2] = 7.*1323./17280;
      wy[3] = 7.*2989./17280;
      wy[4] = 7.*2989./17280;
      wy[5] = 7.*1323./17280;
      wy[6] = 7.*3577./17280;
      wy[7] = 7.*751./17280;
   } else if (ims.Height() == 7) {
      wy[0] = 41./140.;
      wy[1] = 216./140.;
      wy[2] = 27./140.;
      wy[3] = 272./140.;
      wy[4] = 27./140.;
      wy[5] = 216./140.;
      wy[6] = 41./140.;
   } else if (ims.Height() == 6) {
      wy[0] = 5.*19./288.;
      wy[1] = 5.*75./288.;
      wy[2] = 5.*50./288.;
      wy[3] = 5.*50./288.;
      wy[4] = 5.*75./288.;
      wy[5] = 5.*19./288.;
   } else if (ims.Height() == 5) {
      wy[0] = 2.*7./45.;
      wy[1] = 32.*7./45.;
      wy[2] = 12.*7./45.;
      wy[3] = 32.*7./45.;
      wy[4] = 2.*7./45.;
   } else if (ims.Height() == 4) {
      wy[0] = 3.*1./8.;
      wy[1] = 3.*3./8.;
      wy[2] = 3.*3./8.;
      wy[3] = 3.*1./8.;
   } else if (ims.Height() == 3) {
      wy[0] = 1./3.;
      wy[1] = 4./3.;
      wy[2] = 1./3.;
   }

   a = new double*[nx];
   for (int i=0; i<nx; i++) {
      a[i]= new double[ny];
   }

   // Normalization factors.  The theoretical value is 2/(2*m + 1), 
   // but the numerical integral is not perfect.
   normX = new double[nx];
   normY = new double[ny];
   for (int mx = 0; mx < nx; mx++) {
      double sum = 0;
      for (int i = 0; i < ims.Width(); i++) {
	 sum += wx[i]*px[i][mx]*px[i][mx];
      }
      normX[mx] = sum;
   }
   for (int my = 0; my < ny; my++) {
      double sum = 0;
      for (int j = 0; j < ims.Height(); j++) {
	 sum += wy[j]*py[j][my]*py[j][my];
      }
      normY[my] = sum;
   }

   filter(ims, imd, nx, ny);
   
   delete wx;
   delete wy;
   
   for (int i=0; i<ims.Width() ; i++ ) {
      delete[] px[i];
   }
   delete[] px;
   for (int i=0; i<ims.Height() ; i++ ) {
      delete[] py[i];
   }
   delete[] py;

   for (int i=0; i<nx; i++) {
      delete[] a[i];
   }
   delete[] a;

   delete[] normX;
   delete[] normY;

   return SUCCESS;   
}



#ifdef MAIN
#define	USAGE	"usage: %s xorder yorder [im_in|-] [im_out|-]"
#define	PARC	2
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
      Img2duc *const ims = (Img2duc *)objs[0];
      objd[0] = new Img2duc(ims->Props());
      Img2duc *imd = (Img2duc *)objd[0];

   try {
      result = PLegendrePolynomialFitting(*ims, *imd, atoi(parv[0]), atoi(parv[1]));
   } catch(const std::exception &e ) {
      std::cerr << "Error plegendrepolynomialfitting:" << e.what() << std::endl;
      result = FAILURE;
   }
   goto end;
   }
   if (objs[0]->Type() == Po_Img2dsl) {
      Img2dsl *const ims = (Img2dsl *)objs[0];
      objd[0] = new Img2dsl(ims->Props());
      Img2dsl *imd = (Img2dsl *)objd[0];

   try {
      result = PLegendrePolynomialFitting(*ims, *imd, atoi(parv[0]), atoi(parv[1]));
   } catch(const std::exception &e ) {
      std::cerr << "Error plegendrepolynomialfitting:" << e.what() << std::endl;
      result = FAILURE;
   }
   goto end;
   }
   if (objs[0]->Type() == Po_Img2dsf) {
      Img2dsf *const ims = (Img2dsf *)objs[0];
      objd[0] = new Img2dsf(ims->Props());
      Img2dsf *imd = (Img2dsf *)objd[0];

   try {
      result = PLegendrePolynomialFitting(*ims, *imd, atoi(parv[0]), atoi(parv[1]));
   } catch(const std::exception &e ) {
      std::cerr << "Error plegendrepolynomialfitting:" << e.what() << std::endl;
      result = FAILURE;
   }
   goto end;
   }
   if (objs[0]->Type() == Po_Imc2duc) {
      Imc2duc *const ims = (Imc2duc *)objs[0];
      objd[0] = new Imc2duc(ims->Props());
      Imc2duc *imd = (Imc2duc *)objd[0];

   try {
      result = PLegendrePolynomialFitting(*ims, *imd, atoi(parv[0]), atoi(parv[1]));
   } catch(const std::exception &e ) {
      std::cerr << "Error plegendrepolynomialfitting:" << e.what() << std::endl;
      result = FAILURE;
   }
   goto end;
   }
   if (objs[0]->Type() == Po_Imc2dsl) {
      Imc2dsl *const ims = (Imc2dsl *)objs[0];
      objd[0] = new Imc2dsl(ims->Props());
      Imc2dsl *imd = (Imc2dsl *)objd[0];

   try {
      result = PLegendrePolynomialFitting(*ims, *imd, atoi(parv[0]), atoi(parv[1]));
   } catch(const std::exception &e ) {
      std::cerr << "Error plegendrepolynomialfitting:" << e.what() << std::endl;
      result = FAILURE;
   }
   goto end;
   }
   if (objs[0]->Type() == Po_Imc2dsf) {
      Imc2dsf *const ims = (Imc2dsf *)objs[0];
      objd[0] = new Imc2dsf(ims->Props());
      Imc2dsf *imd = (Imc2dsf *)objd[0];

   try {
      result = PLegendrePolynomialFitting(*ims, *imd, atoi(parv[0]), atoi(parv[1]));
   } catch(const std::exception &e ) {
      std::cerr << "Error plegendrepolynomialfitting:" << e.what() << std::endl;
      result = FAILURE;
   }
   goto end;
   }
   if (objs[0]->Type() == Po_Imx2duc) {
      Imx2duc *const ims = (Imx2duc *)objs[0];
      objd[0] = new Imx2duc(ims->Props());
      Imx2duc *imd = (Imx2duc *)objd[0];

   try {
      result = PLegendrePolynomialFitting(*ims, *imd, atoi(parv[0]), atoi(parv[1]));
   } catch(const std::exception &e ) {
      std::cerr << "Error plegendrepolynomialfitting:" << e.what() << std::endl;
      result = FAILURE;
   }
   goto end;
   }
   if (objs[0]->Type() == Po_Imx2dsl) {
      Imx2dsl *const ims = (Imx2dsl *)objs[0];
      objd[0] = new Imx2dsl(ims->Props());
      Imx2dsl *imd = (Imx2dsl *)objd[0];

   try {
      result = PLegendrePolynomialFitting(*ims, *imd, atoi(parv[0]), atoi(parv[1]));
   } catch(const std::exception &e ) {
      std::cerr << "Error plegendrepolynomialfitting:" << e.what() << std::endl;
      result = FAILURE;
   }
   goto end;
   }
   if (objs[0]->Type() == Po_Imx2dsf) {
      Imx2dsf *const ims = (Imx2dsf *)objs[0];
      objd[0] = new Imx2dsf(ims->Props());
      Imx2dsf *imd = (Imx2dsf *)objd[0];

   try {
      result = PLegendrePolynomialFitting(*ims, *imd, atoi(parv[0]), atoi(parv[1]));
   } catch(const std::exception &e ) {
      std::cerr << "Error plegendrepolynomialfitting:" << e.what() << std::endl;
      result = FAILURE;
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
