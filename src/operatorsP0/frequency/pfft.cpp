/* -*- c-basic-offset: 3 ; mode: c++ -*-
 *
 * Copyright (c),  GREYC.
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
 * @author Herissay & Berthet - 2001-02-01
 * @author Régis Clouard - 2002-03-22
 * @author Régis Clouard - 2005-10-24 (fix bug on fft())
 * @author Régis Clouard - 2006-02-15 (add 3D)
 */

#define _USE_MATH_DEFINES
#include <math.h>
#include <pandore.h>
using namespace pandore;

/**
 * @file pfft.cpp
 * @brief Fast Fourier Transform.
 *
 * This function processes only gray level images (2D or 3D).
 * It uses and produces complex images.
 * A complex image is composed of 2 images : the real part and the imaginary part.
 */

#define SWAP(a, b) { tempr = (a); (a) = (b); (b) = tempr; }

/**
 * @author  Copr. 1986-92 Numerical Recipes Software 'j9#.
 * Ndim : image dimension (2d, 3d).
 * @param data the vector of data.
 * @param nn the size of each dimension.
 * @param ndim the dimension (2d or 3d).
 */
Errc fournFFT( double *data, const long nn[], int ndim ) {
   int idim;
   unsigned long i1, i2, i3, i2rev, i3rev, ip1, ip2, ip3, ifp1, ifp2;
   unsigned long ibit, k1, k2, n, nprev, nrem, ntot;
   double tempi, tempr;
   double theta, wi, wpi, wpr, wr, wtemp;
   
   // Total number of complex values.
   for (ntot = 1, idim = 0; idim < ndim; idim++) {
      ntot *=  nn[idim];
   }
   --data;// Due to Numerical Recipies algorithm implementation :: array[1...n];
   
   nprev = 1;
   // main loop over dimensions.

   for (idim = 0; idim < ndim; idim++) {
      n = nn[idim];
      nrem = ntot / (n * nprev);
      ip1 = nprev << 1; // multiplied by 2.
      ip2 = ip1 * n;
      ip3 = ip2 * nrem;
      i2rev = 1;
      
      for (i2 = 1; i2 <= ip2; i2 += ip1) { // Bit reversal section of the routine.
	 if (i2 < i2rev) {
	    for (i1 = i2; i1 <= i2 + ip1 - 2; i1 += 2) {
	       for (i3 = i1; i3 <= ip3;i3 += ip2) {
		  i3rev = i2rev + i3 - i2;
		  SWAP(data[i3], data[i3rev]);
		  SWAP(data[i3 + 1], data[i3rev + 1]);
	       }
	    }
	 }
	 ibit = ip2 >> 1; // Divided by 2.
	 while (ibit >=  ip1 && i2rev > ibit) {
	    i2rev -=  ibit;
	    ibit >>=  1;
	 }
	 i2rev +=  ibit;
      }
      ifp1 = ip1;
      
      while (ifp1 < ip2) {
	 ifp2 = ifp1 << 1;
	 theta = 1 * (M_PI * 2) / (ifp2 / ip1); // isign = +1 for transform
	 wtemp = sin(0.5 * theta);
	 wpr = -2.0 * wtemp * wtemp;
	 wpi = sin(theta);
	 wr = 1.0;
	 wi = 0.0;
	 for (i3 = 1;i3 <= ifp1; i3 += ip1) {
	    for (i1 = i3; i1 <= i3 + ip1 - 2; i1 += 2) {
	       for (i2 = i1; i2 <= ip3; i2 += ifp2) {
		  k1 = i2;
		  k2 = k1 + ifp1;
		  tempr = (double)wr * data[k2] - (double)wi * data[k2 + 1];
		  tempi = (double)wr * data[k2 + 1] + (double)wi * data[k2];
		  data[k2] = data[k1] - tempr;
		  data[k2 + 1] = data[k1 + 1] - tempi;
		  data[k1] += tempr;
		  data[k1+1] += tempi;
	       }
	    }
	    wr = (wtemp = wr) * wpr - wi * wpi + wr;
	    wi = wi * wpr + wtemp * wpi + wi;
	 }
	 ifp1 = ifp2;
      }
      nprev *=  n;
   }
   
   return SUCCESS;
}

#undef SWAP

/**
 * Determines the next power of 2a from num.
 */
static int nextpow2( double num ) {
   double f;
   int exp;
   
   f = frexp(fabs(num), &exp);
   if (f && f ==  0.5) {
      exp = exp - 1;
   }
   
   return((int)pow(2., exp));
}

/**
 * Gray image.
 * Images MUST have their dimensions all powers of 2 -> reshape.
 */
template <typename T>
Errc PFFT( const Imx3d<T> &ims_reel, const Imx3d<T> &ims_imag, Imx3dsf &imd_reel, Imx3dsf &imd_imag ) {
   long nn[3];
   unsigned long n;
   double *data, *pf;
   Point3d p;

   // Build a vector with couples (ims_reel[i][j], im_imag[i][j]).
   // return dimensions ->all power of 2.
   // New values are set to 0.
   nn[0] = imd_reel.Width();
   nn[1] = imd_reel.Height();
   nn[2] = imd_reel.Depth();

   n =  (imd_reel.Depth() < 1)? 2 : 3;
 
   data = pf = (double*)calloc(2 * imd_reel.VectorSize(), sizeof(double));
   
   if (!data) {
      std::cerr << "Error pfft: Images too big. Not enough memory\n" << 2*imd_reel.VectorSize()*sizeof(double);
      return FAILURE;
   }

   for (p.z = 0; p.z < ims_reel.Depth(); p.z++) {
      for (p.y = 0; p.y < ims_reel.Height(); p.y++) {
	 for (p.x = 0;p.x < ims_reel.Width();p.x++) {
	    *(pf++) = (double)ims_reel[0][p];
	    *(pf++) = (double)ims_imag[0][p];
	 }
	 pf += 2 * (nn[0] - p.x);
      }
      pf += 2 * ((nn[1] - p.y) * nn[0]);
   }
   
   fournFFT(data, nn, n);
   
   pf = data;
   
   // Build outputs images.
   for (p.z = 0; p.z < imd_reel.Depth(); p.z++) {
      for (p.y = 0; p.y < imd_reel.Height(); p.y++) {
	 for (p.x = 0; p.x < imd_reel.Width(); p.x++) {
	    imd_reel[0][p] =  (float)*(pf++);
	    imd_imag[0][p] = (float)*(pf++);
	 }
	 // Discard those points.
	 pf += 2 * (nn[0] - p.x);
      }
      pf += 2 * ((nn[1] - p.y) * nn[0]);
   }
   
   free(data);
   
   return SUCCESS;
}





#ifdef MAIN
#define USAGE "usage: %s [im_in1|-] [im_in2|-] [im_out1|-] [im_out2|-]"
#define PARC  0 //PARC represente le nombre de parametres exact.
#define FINC  2 //nombre d"images d'entree.
#define FOUTC 2 //nombre d'images de sortie.
#define MASK  0 //operation de masquage.

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
      Img2duc* const ims_reel = (Img2duc*)objs[0];
      Img2duc* const ims_imag = (Img2duc*)objs[1];
      int nrow = nextpow2(ims_reel->Height());
      int ncol = nextpow2(ims_reel->Width());
      objd[0] = new Img2dsf(nrow, ncol);
      Img2dsf* const imd_reel = (Img2dsf*)objd[0];
      objd[1] = new Img2dsf(nrow, ncol);
      Img2dsf* const imd_imag = (Img2dsf*)objd[1];

      result = PFFT(*ims_reel, *ims_imag, *imd_reel, *imd_imag);

      goto end;
   }
   if (objs[0]->Type() == Po_Img2dsl) {
      Img2dsl* const ims_reel = (Img2dsl*)objs[0];
      Img2dsl* const ims_imag = (Img2dsl*)objs[1];
      int nrow = nextpow2(ims_reel->Height());
      int ncol = nextpow2(ims_reel->Width());
      objd[0] = new Img2dsf(nrow, ncol);
      Img2dsf* const imd_reel = (Img2dsf*)objd[0];
      objd[1] = new Img2dsf(nrow, ncol);
      Img2dsf* const imd_imag = (Img2dsf*)objd[1];

      result = PFFT(*ims_reel, *ims_imag, *imd_reel, *imd_imag);

      goto end;
   }
   if (objs[0]->Type() == Po_Img2dsf) {
      Img2dsf* const ims_reel = (Img2dsf*)objs[0];
      Img2dsf* const ims_imag = (Img2dsf*)objs[1];
      int nrow = nextpow2(ims_reel->Height());
      int ncol = nextpow2(ims_reel->Width());
      objd[0] = new Img2dsf(nrow, ncol);
      Img2dsf* const imd_reel = (Img2dsf*)objd[0];
      objd[1] = new Img2dsf(nrow, ncol);
      Img2dsf* const imd_imag = (Img2dsf*)objd[1];

      result = PFFT(*ims_reel, *ims_imag, *imd_reel, *imd_imag);

      goto end;
   }
   if (objs[0]->Type() == Po_Img3duc) {
      Img3duc* const ims_reel = (Img3duc*)objs[0];
      Img3duc* const ims_imag = (Img3duc*)objs[1];
      int ndep = nextpow2(ims_reel->Depth());
      int nrow = nextpow2(ims_reel->Height());
      int ncol = nextpow2(ims_reel->Width());
      objd[0] = new Img3dsf(ndep, nrow, ncol); 
      Img3dsf* const imd_reel = (Img3dsf*)objd[0];
      objd[1] = new Img3dsf(ndep, nrow, ncol);
      Img3dsf* const imd_imag = (Img3dsf*)objd[1];

      result = PFFT(*ims_reel, *ims_imag, *imd_reel, *imd_imag);

      goto end;
   }
   if (objs[0]->Type() == Po_Img3dsl) {
      Img3dsl* const ims_reel = (Img3dsl*)objs[0];
      Img3dsl* const ims_imag = (Img3dsl*)objs[1];
      int ndep = nextpow2(ims_reel->Depth());
      int nrow = nextpow2(ims_reel->Height());
      int ncol = nextpow2(ims_reel->Width());
      objd[0] = new Img3dsf(ndep, nrow, ncol); 
      Img3dsf* const imd_reel = (Img3dsf*)objd[0];
      objd[1] = new Img3dsf(ndep, nrow, ncol);
      Img3dsf* const imd_imag = (Img3dsf*)objd[1];

      result = PFFT(*ims_reel, *ims_imag, *imd_reel, *imd_imag);

      goto end;
   }
   if (objs[0]->Type() == Po_Img3dsf) {
      Img3dsf* const ims_reel = (Img3dsf*)objs[0];
      Img3dsf* const ims_imag = (Img3dsf*)objs[1];
      int ndep = nextpow2(ims_reel->Depth());
      int nrow = nextpow2(ims_reel->Height());
      int ncol = nextpow2(ims_reel->Width());
      objd[0] = new Img3dsf(ndep, nrow, ncol); 
      Img3dsf* const imd_reel = (Img3dsf*)objd[0];
      objd[1] = new Img3dsf(ndep, nrow, ncol);
      Img3dsf* const imd_imag = (Img3dsf*)objd[1];

      result = PFFT(*ims_reel, *ims_imag, *imd_reel, *imd_imag);

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
