/* -*- mode: c++; c-basic-offset: 3 -*-
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

#include <math.h>
#include <pandore.h>
using namespace pandore;

//#define DEBUG

/**
 * @file pentropybinarization.cpp
 * @brief Binarization based on maximum entropy criterion.
 *
 * J.C Yen, F.J Chang, S. Chang, "A New Criterion for Automatic Multilevel Thresholding",
 * IEEE Trans. On Image Processing, Vol. 4, No. 3,pp 370-378, 1995.
 */

/**
 * Thresholds the input image with the Maximum Entropy Criterion.
 * @param ims	the input image.
 * @param imd	the output image.
 * @return the threshold value.
 */

Errc PMaximumEntropyBinarization( const Img2duc &ims, Img2duc &imd ) {
   std::map<int, float> h;
   std::map<int, double> hc;
   std::map<int, double> hce;
   std::map<int, double> te;
   std::map<int, float>::iterator i;

   Point2d p;

   // Compute the histogram   
   for (p.y = 0; p.y < ims.Height(); p.y++)
   for (p.x = 0; p.x < ims.Width(); p.x++)
   {
      h[(int)ims[p]]++;
   }
   // Compute the cumulated histogram and the cumulated entropy histogram.
   const long nbpix = ims.VectorSize();
   i = h.begin();
   double hcp = hc[i->first] = i->second / nbpix;
   double hep = hce[i->first] = hcp * log(hcp);
#ifdef DEBUG
   printf("hc[%d]=%lf hce[%d]=%lf\n", (int)i->first, hc[i->first], (int)i->first, hce[i->first]);
#endif
   for ( i++; i != h.end(); ++i) {
      double pi = i->second/nbpix;
      hcp = hc[i->first] = hcp + pi;
      hep = hce[i->first] = hep + (pi * log(pi));
#ifdef DEBUG
      printf("hc[%d]=%lf pi*lnpi=%lf hce[%d]=%lf\n", (int)i->first, hc[i->first], pi * log(pi), (int)i->first, hce[i->first]);
#endif
   }
   
   std::map<int, float>::iterator begin = h.begin();
   std::map<int, float>::iterator end = h.end();
   std::map<int, float>::iterator temp;
   begin++;
   end--;
   for (i = begin; i != end; ++i) {
      temp = i;
      temp--;
      double Ps = hc[temp->first];
      double Hs = hce[temp->first];
      double Hps = hep-Hs;
      te[i->first] = log(Ps * (1.0 - Ps)) - (Hs / Ps) - (Hps / (1.0 - Ps));
#ifdef DEBUG
      printf("Ps=%lf, ln(Ps*(1-Ps)=%lf Hs=%lf, Hps=%lf (%lf %lf)\n", Ps, log(Ps * (1.0 - Ps)), Hs, Hps, hep, hce[i->first]);
      printf("Te[%d]=%lf\n", (int)i->first, te[i->first]);
#endif
   }
   
   // Get the maximum of entropy to determine the threshold.
   i = begin;
   int maximum = i->first;
   for (; i != end; ++i) {
#ifdef DEBUG
      printf("comparaison = %lf %lf\n", te[i->first], te[maximum]);
#endif
      if (te[i->first] > te[maximum]) {
	 maximum = i->first;
      }
   }
   
   // Use thresholds to build the output image.
   for (p.y = 0; p.y < ims.Height(); p.y++)
   for (p.x = 0; p.x < ims.Width(); p.x++)
   {
      if (ims[p] < maximum) {
	 imd[p] = 0;
      } else {
	 imd[p] = MAXUCHAR;
      }
   }
   return maximum;
}

Errc PMaximumEntropyBinarization( const Img2dsl &ims, Img2duc &imd ) {
   std::map<int, float> h;
   std::map<int, double> hc;
   std::map<int, double> hce;
   std::map<int, double> te;
   std::map<int, float>::iterator i;

   Point2d p;

   // Compute the histogram   
   for (p.y = 0; p.y < ims.Height(); p.y++)
   for (p.x = 0; p.x < ims.Width(); p.x++)
   {
      h[(int)ims[p]]++;
   }
   // Compute the cumulated histogram and the cumulated entropy histogram.
   const long nbpix = ims.VectorSize();
   i = h.begin();
   double hcp = hc[i->first] = i->second / nbpix;
   double hep = hce[i->first] = hcp * log(hcp);
#ifdef DEBUG
   printf("hc[%d]=%lf hce[%d]=%lf\n", (int)i->first, hc[i->first], (int)i->first, hce[i->first]);
#endif
   for ( i++; i != h.end(); ++i) {
      double pi = i->second/nbpix;
      hcp = hc[i->first] = hcp + pi;
      hep = hce[i->first] = hep + (pi * log(pi));
#ifdef DEBUG
      printf("hc[%d]=%lf pi*lnpi=%lf hce[%d]=%lf\n", (int)i->first, hc[i->first], pi * log(pi), (int)i->first, hce[i->first]);
#endif
   }
   
   std::map<int, float>::iterator begin = h.begin();
   std::map<int, float>::iterator end = h.end();
   std::map<int, float>::iterator temp;
   begin++;
   end--;
   for (i = begin; i != end; ++i) {
      temp = i;
      temp--;
      double Ps = hc[temp->first];
      double Hs = hce[temp->first];
      double Hps = hep-Hs;
      te[i->first] = log(Ps * (1.0 - Ps)) - (Hs / Ps) - (Hps / (1.0 - Ps));
#ifdef DEBUG
      printf("Ps=%lf, ln(Ps*(1-Ps)=%lf Hs=%lf, Hps=%lf (%lf %lf)\n", Ps, log(Ps * (1.0 - Ps)), Hs, Hps, hep, hce[i->first]);
      printf("Te[%d]=%lf\n", (int)i->first, te[i->first]);
#endif
   }
   
   // Get the maximum of entropy to determine the threshold.
   i = begin;
   int maximum = i->first;
   for (; i != end; ++i) {
#ifdef DEBUG
      printf("comparaison = %lf %lf\n", te[i->first], te[maximum]);
#endif
      if (te[i->first] > te[maximum]) {
	 maximum = i->first;
      }
   }
   
   // Use thresholds to build the output image.
   for (p.y = 0; p.y < ims.Height(); p.y++)
   for (p.x = 0; p.x < ims.Width(); p.x++)
   {
      if (ims[p] < maximum) {
	 imd[p] = 0;
      } else {
	 imd[p] = MAXUCHAR;
      }
   }
   return maximum;
}

Errc PMaximumEntropyBinarization( const Img2dsf &ims, Img2duc &imd ) {
   std::map<int, float> h;
   std::map<int, double> hc;
   std::map<int, double> hce;
   std::map<int, double> te;
   std::map<int, float>::iterator i;

   Point2d p;

   // Compute the histogram   
   for (p.y = 0; p.y < ims.Height(); p.y++)
   for (p.x = 0; p.x < ims.Width(); p.x++)
   {
      h[(int)ims[p]]++;
   }
   // Compute the cumulated histogram and the cumulated entropy histogram.
   const long nbpix = ims.VectorSize();
   i = h.begin();
   double hcp = hc[i->first] = i->second / nbpix;
   double hep = hce[i->first] = hcp * log(hcp);
#ifdef DEBUG
   printf("hc[%d]=%lf hce[%d]=%lf\n", (int)i->first, hc[i->first], (int)i->first, hce[i->first]);
#endif
   for ( i++; i != h.end(); ++i) {
      double pi = i->second/nbpix;
      hcp = hc[i->first] = hcp + pi;
      hep = hce[i->first] = hep + (pi * log(pi));
#ifdef DEBUG
      printf("hc[%d]=%lf pi*lnpi=%lf hce[%d]=%lf\n", (int)i->first, hc[i->first], pi * log(pi), (int)i->first, hce[i->first]);
#endif
   }
   
   std::map<int, float>::iterator begin = h.begin();
   std::map<int, float>::iterator end = h.end();
   std::map<int, float>::iterator temp;
   begin++;
   end--;
   for (i = begin; i != end; ++i) {
      temp = i;
      temp--;
      double Ps = hc[temp->first];
      double Hs = hce[temp->first];
      double Hps = hep-Hs;
      te[i->first] = log(Ps * (1.0 - Ps)) - (Hs / Ps) - (Hps / (1.0 - Ps));
#ifdef DEBUG
      printf("Ps=%lf, ln(Ps*(1-Ps)=%lf Hs=%lf, Hps=%lf (%lf %lf)\n", Ps, log(Ps * (1.0 - Ps)), Hs, Hps, hep, hce[i->first]);
      printf("Te[%d]=%lf\n", (int)i->first, te[i->first]);
#endif
   }
   
   // Get the maximum of entropy to determine the threshold.
   i = begin;
   int maximum = i->first;
   for (; i != end; ++i) {
#ifdef DEBUG
      printf("comparaison = %lf %lf\n", te[i->first], te[maximum]);
#endif
      if (te[i->first] > te[maximum]) {
	 maximum = i->first;
      }
   }
   
   // Use thresholds to build the output image.
   for (p.y = 0; p.y < ims.Height(); p.y++)
   for (p.x = 0; p.x < ims.Width(); p.x++)
   {
      if (ims[p] < maximum) {
	 imd[p] = 0;
      } else {
	 imd[p] = MAXUCHAR;
      }
   }
   return maximum;
}

Errc PMaximumEntropyBinarization( const Img3duc &ims, Img3duc &imd ) {
   std::map<int, float> h;
   std::map<int, double> hc;
   std::map<int, double> hce;
   std::map<int, double> te;
   std::map<int, float>::iterator i;

   Point3d p;

   // Compute the histogram   
   for (p.z = 0; p.z < ims.Depth(); p.z++)
   for (p.y = 0; p.y < ims.Height(); p.y++)
   for (p.x = 0; p.x < ims.Width(); p.x++)
   {
      h[(int)ims[p]]++;
   }
   // Compute the cumulated histogram and the cumulated entropy histogram.
   const long nbpix = ims.VectorSize();
   i = h.begin();
   double hcp = hc[i->first] = i->second / nbpix;
   double hep = hce[i->first] = hcp * log(hcp);
#ifdef DEBUG
   printf("hc[%d]=%lf hce[%d]=%lf\n", (int)i->first, hc[i->first], (int)i->first, hce[i->first]);
#endif
   for ( i++; i != h.end(); ++i) {
      double pi = i->second/nbpix;
      hcp = hc[i->first] = hcp + pi;
      hep = hce[i->first] = hep + (pi * log(pi));
#ifdef DEBUG
      printf("hc[%d]=%lf pi*lnpi=%lf hce[%d]=%lf\n", (int)i->first, hc[i->first], pi * log(pi), (int)i->first, hce[i->first]);
#endif
   }
   
   std::map<int, float>::iterator begin = h.begin();
   std::map<int, float>::iterator end = h.end();
   std::map<int, float>::iterator temp;
   begin++;
   end--;
   for (i = begin; i != end; ++i) {
      temp = i;
      temp--;
      double Ps = hc[temp->first];
      double Hs = hce[temp->first];
      double Hps = hep-Hs;
      te[i->first] = log(Ps * (1.0 - Ps)) - (Hs / Ps) - (Hps / (1.0 - Ps));
#ifdef DEBUG
      printf("Ps=%lf, ln(Ps*(1-Ps)=%lf Hs=%lf, Hps=%lf (%lf %lf)\n", Ps, log(Ps * (1.0 - Ps)), Hs, Hps, hep, hce[i->first]);
      printf("Te[%d]=%lf\n", (int)i->first, te[i->first]);
#endif
   }
   
   // Get the maximum of entropy to determine the threshold.
   i = begin;
   int maximum = i->first;
   for (; i != end; ++i) {
#ifdef DEBUG
      printf("comparaison = %lf %lf\n", te[i->first], te[maximum]);
#endif
      if (te[i->first] > te[maximum]) {
	 maximum = i->first;
      }
   }
   
   // Use thresholds to build the output image.
   for (p.z = 0; p.z < ims.Depth(); p.z++)
   for (p.y = 0; p.y < ims.Height(); p.y++)
   for (p.x = 0; p.x < ims.Width(); p.x++)
   {
      if (ims[p] < maximum) {
	 imd[p] = 0;
      } else {
	 imd[p] = MAXUCHAR;
      }
   }
   return maximum;
}

Errc PMaximumEntropyBinarization( const Img3dsl &ims, Img3duc &imd ) {
   std::map<int, float> h;
   std::map<int, double> hc;
   std::map<int, double> hce;
   std::map<int, double> te;
   std::map<int, float>::iterator i;

   Point3d p;

   // Compute the histogram   
   for (p.z = 0; p.z < ims.Depth(); p.z++)
   for (p.y = 0; p.y < ims.Height(); p.y++)
   for (p.x = 0; p.x < ims.Width(); p.x++)
   {
      h[(int)ims[p]]++;
   }
   // Compute the cumulated histogram and the cumulated entropy histogram.
   const long nbpix = ims.VectorSize();
   i = h.begin();
   double hcp = hc[i->first] = i->second / nbpix;
   double hep = hce[i->first] = hcp * log(hcp);
#ifdef DEBUG
   printf("hc[%d]=%lf hce[%d]=%lf\n", (int)i->first, hc[i->first], (int)i->first, hce[i->first]);
#endif
   for ( i++; i != h.end(); ++i) {
      double pi = i->second/nbpix;
      hcp = hc[i->first] = hcp + pi;
      hep = hce[i->first] = hep + (pi * log(pi));
#ifdef DEBUG
      printf("hc[%d]=%lf pi*lnpi=%lf hce[%d]=%lf\n", (int)i->first, hc[i->first], pi * log(pi), (int)i->first, hce[i->first]);
#endif
   }
   
   std::map<int, float>::iterator begin = h.begin();
   std::map<int, float>::iterator end = h.end();
   std::map<int, float>::iterator temp;
   begin++;
   end--;
   for (i = begin; i != end; ++i) {
      temp = i;
      temp--;
      double Ps = hc[temp->first];
      double Hs = hce[temp->first];
      double Hps = hep-Hs;
      te[i->first] = log(Ps * (1.0 - Ps)) - (Hs / Ps) - (Hps / (1.0 - Ps));
#ifdef DEBUG
      printf("Ps=%lf, ln(Ps*(1-Ps)=%lf Hs=%lf, Hps=%lf (%lf %lf)\n", Ps, log(Ps * (1.0 - Ps)), Hs, Hps, hep, hce[i->first]);
      printf("Te[%d]=%lf\n", (int)i->first, te[i->first]);
#endif
   }
   
   // Get the maximum of entropy to determine the threshold.
   i = begin;
   int maximum = i->first;
   for (; i != end; ++i) {
#ifdef DEBUG
      printf("comparaison = %lf %lf\n", te[i->first], te[maximum]);
#endif
      if (te[i->first] > te[maximum]) {
	 maximum = i->first;
      }
   }
   
   // Use thresholds to build the output image.
   for (p.z = 0; p.z < ims.Depth(); p.z++)
   for (p.y = 0; p.y < ims.Height(); p.y++)
   for (p.x = 0; p.x < ims.Width(); p.x++)
   {
      if (ims[p] < maximum) {
	 imd[p] = 0;
      } else {
	 imd[p] = MAXUCHAR;
      }
   }
   return maximum;
}

Errc PMaximumEntropyBinarization( const Img3dsf &ims, Img3duc &imd ) {
   std::map<int, float> h;
   std::map<int, double> hc;
   std::map<int, double> hce;
   std::map<int, double> te;
   std::map<int, float>::iterator i;

   Point3d p;

   // Compute the histogram   
   for (p.z = 0; p.z < ims.Depth(); p.z++)
   for (p.y = 0; p.y < ims.Height(); p.y++)
   for (p.x = 0; p.x < ims.Width(); p.x++)
   {
      h[(int)ims[p]]++;
   }
   // Compute the cumulated histogram and the cumulated entropy histogram.
   const long nbpix = ims.VectorSize();
   i = h.begin();
   double hcp = hc[i->first] = i->second / nbpix;
   double hep = hce[i->first] = hcp * log(hcp);
#ifdef DEBUG
   printf("hc[%d]=%lf hce[%d]=%lf\n", (int)i->first, hc[i->first], (int)i->first, hce[i->first]);
#endif
   for ( i++; i != h.end(); ++i) {
      double pi = i->second/nbpix;
      hcp = hc[i->first] = hcp + pi;
      hep = hce[i->first] = hep + (pi * log(pi));
#ifdef DEBUG
      printf("hc[%d]=%lf pi*lnpi=%lf hce[%d]=%lf\n", (int)i->first, hc[i->first], pi * log(pi), (int)i->first, hce[i->first]);
#endif
   }
   
   std::map<int, float>::iterator begin = h.begin();
   std::map<int, float>::iterator end = h.end();
   std::map<int, float>::iterator temp;
   begin++;
   end--;
   for (i = begin; i != end; ++i) {
      temp = i;
      temp--;
      double Ps = hc[temp->first];
      double Hs = hce[temp->first];
      double Hps = hep-Hs;
      te[i->first] = log(Ps * (1.0 - Ps)) - (Hs / Ps) - (Hps / (1.0 - Ps));
#ifdef DEBUG
      printf("Ps=%lf, ln(Ps*(1-Ps)=%lf Hs=%lf, Hps=%lf (%lf %lf)\n", Ps, log(Ps * (1.0 - Ps)), Hs, Hps, hep, hce[i->first]);
      printf("Te[%d]=%lf\n", (int)i->first, te[i->first]);
#endif
   }
   
   // Get the maximum of entropy to determine the threshold.
   i = begin;
   int maximum = i->first;
   for (; i != end; ++i) {
#ifdef DEBUG
      printf("comparaison = %lf %lf\n", te[i->first], te[maximum]);
#endif
      if (te[i->first] > te[maximum]) {
	 maximum = i->first;
      }
   }
   
   // Use thresholds to build the output image.
   for (p.z = 0; p.z < ims.Depth(); p.z++)
   for (p.y = 0; p.y < ims.Height(); p.y++)
   for (p.x = 0; p.x < ims.Width(); p.x++)
   {
      if (ims[p] < maximum) {
	 imd[p] = 0;
      } else {
	 imd[p] = MAXUCHAR;
      }
   }
   return maximum;
}


#ifdef MAIN
#define USAGE	"usage: %s [-m mask] [im_in|-] [im_out|-]"
#define PARC	0
#define FINC	1
#define FOUTC	1
#define MASK	1

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
      objd[0] = new Img2duc(ims->Props());
      Img2duc* const imd = (Img2duc*)objd[0];
      
      result = PMaximumEntropyBinarization(*ims, *imd);
      goto end;
   }
   if (objs[0]->Type() == Po_Img2dsl) {
      Img2dsl* const ims = (Img2dsl*)objs[0];
      objd[0] = new Img2duc(ims->Props());
      Img2duc* const imd = (Img2duc*)objd[0];
      
      result = PMaximumEntropyBinarization(*ims, *imd);
      goto end;
   }
   if (objs[0]->Type() == Po_Img2dsf) {
      Img2dsf* const ims = (Img2dsf*)objs[0];
      objd[0] = new Img2duc(ims->Props());
      Img2duc* const imd = (Img2duc*)objd[0];
      
      result = PMaximumEntropyBinarization(*ims, *imd);
      goto end;
   }
   if (objs[0]->Type() == Po_Img3duc) {
      Img3duc* const ims = (Img3duc*)objs[0];
      objd[0] = new Img3duc(ims->Props());
      Img3duc* const imd = (Img3duc*)objd[0];
      
      result = PMaximumEntropyBinarization(*ims, *imd);
      goto end;
   }
   if (objs[0]->Type() == Po_Img3dsl) {
      Img3dsl* const ims = (Img3dsl*)objs[0];
      objd[0] = new Img3duc(ims->Props());
      Img3duc* const imd = (Img3duc*)objd[0];
      
      result = PMaximumEntropyBinarization(*ims, *imd);
      goto end;
   }
   if (objs[0]->Type() == Po_Img3dsf) {
      Img3dsf* const ims = (Img3dsf*)objs[0];
      objd[0] = new Img3duc(ims->Props());
      Img3duc* const imd = (Img3duc*)objd[0];
      
      result = PMaximumEntropyBinarization(*ims, *imd);
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
