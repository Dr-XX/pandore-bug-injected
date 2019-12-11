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

/**
 * @file pmaxcorrelationbinarization.cpp
 * @brief Binarization based on maximum correlation criterion.
 *
 * J.C Yen, F.J Chang, S. Chang, "A New Criterion for Automatic Multilevel Thresholding",
 * IEEE Trans. On Image Processing, Vol. 4, No. 3,pp 370-378, 1995.
 */

/**
 * Thresholds the input image with the Maximum Correlation Criterion.
 * @param ims the input image.
 * @param imd the output image.
 * @return the threshold value.
 */

Errc PMaximumCorrelationBinarization( const Img2duc &ims, Img2duc &imd ) {
   std::map<Img2duc::ValueType, float> h;
   std::map<Img2duc::ValueType, double> hc;
   std::map<Img2duc::ValueType, double> hcc;
   std::map<Img2duc::ValueType, double> tc;
   std::map<Img2duc::ValueType, float>::iterator i;

   Point2d p;

   // Compute the histogram
   for (p.y = 0; p.y < ims.Height(); p.y++)
   for (p.x = 0; p.x < ims.Width(); p.x++)
   {
      h[ims[p]]++;
   }
   // Computes the cumulated histogram and the cumulated correlation histogram.
   const long nbpix = ims.VectorSize();
   i = h.begin();
   double hcp = hc[i->first] = i->second / nbpix;
   double hccp = hcc[i->first] = hcp * hcp;
#ifdef DEBUG
   printf("hc[%d]=%lg hcc[%d]=%lg\n", (int)i->first, hc[i->first], (int)i->first, hcc[i->first]);
#endif

   for ( i++; i != h.end(); ++i) {
      double pi = i->second/nbpix;
      hcp = hc[i->first] = hcp + pi;
      hccp = hcc[i->first] = hccp + (pi * pi);
#ifdef DEBUG
      printf("hc[%d]=%lf pi*pi=%lg hcc[%d]=%lf\n", (int)i->first, hc[i->first], (float)(pi * pi), (int)i->first, hcc[i->first]);
#endif
   }

   std::map<Img2duc::ValueType, float>::iterator begin = h.begin();
   std::map<Img2duc::ValueType, float>::iterator end = h.end();
   std::map<Img2duc::ValueType, float>::iterator temp;
   begin++;
   end --;
   for (i = begin; i != end; ++i) {
      temp = i;
      temp--;
      Double Ps = hc[temp->first];
      Double Gs = hcc[temp->first];
      Double Gps = hccp - Gs;
      tc[i->first] = - log(Gs * Gps) + 2.0F * log(Ps * (1.0F - Ps));
#ifdef DEBUG
       printf("Tc[%d]=%f\n", (int)i->first, tc[i->first]);
       printf("   Ps=%g, Gs=%g, Gps=%g (%g %g)\n", Ps, Gs, Gps, hccp, hcc[i->first]);
       printf("   (Gs*gps)=%g, Ps*(1-Ps)=%g)\n", (float)(Gs * Gps), (float)(Ps * (1.0F * Ps)));
#endif
   }

   // Get the maximum of the correlation to determine the threshold.
   i = begin;
   Img2duc::ValueType maximum = (Img2duc::ValueType)i->first;
   for (; i != end; ++i) {
#ifdef DEBUG
      printf("comparaison = %f %f\n", tc[i->first], tc[maximum]);
#endif
      if (tc[i->first] > tc[maximum]) {
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

Errc PMaximumCorrelationBinarization( const Img2dsl &ims, Img2duc &imd ) {
   std::map<Img2dsl::ValueType, float> h;
   std::map<Img2dsl::ValueType, double> hc;
   std::map<Img2dsl::ValueType, double> hcc;
   std::map<Img2dsl::ValueType, double> tc;
   std::map<Img2dsl::ValueType, float>::iterator i;

   Point2d p;

   // Compute the histogram
   for (p.y = 0; p.y < ims.Height(); p.y++)
   for (p.x = 0; p.x < ims.Width(); p.x++)
   {
      h[ims[p]]++;
   }
   // Computes the cumulated histogram and the cumulated correlation histogram.
   const long nbpix = ims.VectorSize();
   i = h.begin();
   double hcp = hc[i->first] = i->second / nbpix;
   double hccp = hcc[i->first] = hcp * hcp;
#ifdef DEBUG
   printf("hc[%d]=%lg hcc[%d]=%lg\n", (int)i->first, hc[i->first], (int)i->first, hcc[i->first]);
#endif

   for ( i++; i != h.end(); ++i) {
      double pi = i->second/nbpix;
      hcp = hc[i->first] = hcp + pi;
      hccp = hcc[i->first] = hccp + (pi * pi);
#ifdef DEBUG
      printf("hc[%d]=%lf pi*pi=%lg hcc[%d]=%lf\n", (int)i->first, hc[i->first], (float)(pi * pi), (int)i->first, hcc[i->first]);
#endif
   }

   std::map<Img2dsl::ValueType, float>::iterator begin = h.begin();
   std::map<Img2dsl::ValueType, float>::iterator end = h.end();
   std::map<Img2dsl::ValueType, float>::iterator temp;
   begin++;
   end --;
   for (i = begin; i != end; ++i) {
      temp = i;
      temp--;
      Double Ps = hc[temp->first];
      Double Gs = hcc[temp->first];
      Double Gps = hccp - Gs;
      tc[i->first] = - log(Gs * Gps) + 2.0F * log(Ps * (1.0F - Ps));
#ifdef DEBUG
       printf("Tc[%d]=%f\n", (int)i->first, tc[i->first]);
       printf("   Ps=%g, Gs=%g, Gps=%g (%g %g)\n", Ps, Gs, Gps, hccp, hcc[i->first]);
       printf("   (Gs*gps)=%g, Ps*(1-Ps)=%g)\n", (float)(Gs * Gps), (float)(Ps * (1.0F * Ps)));
#endif
   }

   // Get the maximum of the correlation to determine the threshold.
   i = begin;
   Img2dsl::ValueType maximum = (Img2dsl::ValueType)i->first;
   for (; i != end; ++i) {
#ifdef DEBUG
      printf("comparaison = %f %f\n", tc[i->first], tc[maximum]);
#endif
      if (tc[i->first] > tc[maximum]) {
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

Errc PMaximumCorrelationBinarization( const Img2dsf &ims, Img2duc &imd ) {
   std::map<Img2dsf::ValueType, float> h;
   std::map<Img2dsf::ValueType, double> hc;
   std::map<Img2dsf::ValueType, double> hcc;
   std::map<Img2dsf::ValueType, double> tc;
   std::map<Img2dsf::ValueType, float>::iterator i;

   Point2d p;

   // Compute the histogram
   for (p.y = 0; p.y < ims.Height(); p.y++)
   for (p.x = 0; p.x < ims.Width(); p.x++)
   {
      h[ims[p]]++;
   }
   // Computes the cumulated histogram and the cumulated correlation histogram.
   const long nbpix = ims.VectorSize();
   i = h.begin();
   double hcp = hc[i->first] = i->second / nbpix;
   double hccp = hcc[i->first] = hcp * hcp;
#ifdef DEBUG
   printf("hc[%d]=%lg hcc[%d]=%lg\n", (int)i->first, hc[i->first], (int)i->first, hcc[i->first]);
#endif

   for ( i++; i != h.end(); ++i) {
      double pi = i->second/nbpix;
      hcp = hc[i->first] = hcp + pi;
      hccp = hcc[i->first] = hccp + (pi * pi);
#ifdef DEBUG
      printf("hc[%d]=%lf pi*pi=%lg hcc[%d]=%lf\n", (int)i->first, hc[i->first], (float)(pi * pi), (int)i->first, hcc[i->first]);
#endif
   }

   std::map<Img2dsf::ValueType, float>::iterator begin = h.begin();
   std::map<Img2dsf::ValueType, float>::iterator end = h.end();
   std::map<Img2dsf::ValueType, float>::iterator temp;
   begin++;
   end --;
   for (i = begin; i != end; ++i) {
      temp = i;
      temp--;
      Double Ps = hc[temp->first];
      Double Gs = hcc[temp->first];
      Double Gps = hccp - Gs;
      tc[i->first] = - log(Gs * Gps) + 2.0F * log(Ps * (1.0F - Ps));
#ifdef DEBUG
       printf("Tc[%d]=%f\n", (int)i->first, tc[i->first]);
       printf("   Ps=%g, Gs=%g, Gps=%g (%g %g)\n", Ps, Gs, Gps, hccp, hcc[i->first]);
       printf("   (Gs*gps)=%g, Ps*(1-Ps)=%g)\n", (float)(Gs * Gps), (float)(Ps * (1.0F * Ps)));
#endif
   }

   // Get the maximum of the correlation to determine the threshold.
   i = begin;
   Img2dsf::ValueType maximum = (Img2dsf::ValueType)i->first;
   for (; i != end; ++i) {
#ifdef DEBUG
      printf("comparaison = %f %f\n", tc[i->first], tc[maximum]);
#endif
      if (tc[i->first] > tc[maximum]) {
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

Errc PMaximumCorrelationBinarization( const Img3duc &ims, Img3duc &imd ) {
   std::map<Img3duc::ValueType, float> h;
   std::map<Img3duc::ValueType, double> hc;
   std::map<Img3duc::ValueType, double> hcc;
   std::map<Img3duc::ValueType, double> tc;
   std::map<Img3duc::ValueType, float>::iterator i;

   Point3d p;

   // Compute the histogram
   for (p.z = 0; p.z < ims.Depth(); p.z++)
   for (p.y = 0; p.y < ims.Height(); p.y++)
   for (p.x = 0; p.x < ims.Width(); p.x++)
   {
      h[ims[p]]++;
   }
   // Computes the cumulated histogram and the cumulated correlation histogram.
   const long nbpix = ims.VectorSize();
   i = h.begin();
   double hcp = hc[i->first] = i->second / nbpix;
   double hccp = hcc[i->first] = hcp * hcp;
#ifdef DEBUG
   printf("hc[%d]=%lg hcc[%d]=%lg\n", (int)i->first, hc[i->first], (int)i->first, hcc[i->first]);
#endif

   for ( i++; i != h.end(); ++i) {
      double pi = i->second/nbpix;
      hcp = hc[i->first] = hcp + pi;
      hccp = hcc[i->first] = hccp + (pi * pi);
#ifdef DEBUG
      printf("hc[%d]=%lf pi*pi=%lg hcc[%d]=%lf\n", (int)i->first, hc[i->first], (float)(pi * pi), (int)i->first, hcc[i->first]);
#endif
   }

   std::map<Img3duc::ValueType, float>::iterator begin = h.begin();
   std::map<Img3duc::ValueType, float>::iterator end = h.end();
   std::map<Img3duc::ValueType, float>::iterator temp;
   begin++;
   end --;
   for (i = begin; i != end; ++i) {
      temp = i;
      temp--;
      Double Ps = hc[temp->first];
      Double Gs = hcc[temp->first];
      Double Gps = hccp - Gs;
      tc[i->first] = - log(Gs * Gps) + 2.0F * log(Ps * (1.0F - Ps));
#ifdef DEBUG
       printf("Tc[%d]=%f\n", (int)i->first, tc[i->first]);
       printf("   Ps=%g, Gs=%g, Gps=%g (%g %g)\n", Ps, Gs, Gps, hccp, hcc[i->first]);
       printf("   (Gs*gps)=%g, Ps*(1-Ps)=%g)\n", (float)(Gs * Gps), (float)(Ps * (1.0F * Ps)));
#endif
   }

   // Get the maximum of the correlation to determine the threshold.
   i = begin;
   Img3duc::ValueType maximum = (Img3duc::ValueType)i->first;
   for (; i != end; ++i) {
#ifdef DEBUG
      printf("comparaison = %f %f\n", tc[i->first], tc[maximum]);
#endif
      if (tc[i->first] > tc[maximum]) {
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

Errc PMaximumCorrelationBinarization( const Img3dsl &ims, Img3duc &imd ) {
   std::map<Img3dsl::ValueType, float> h;
   std::map<Img3dsl::ValueType, double> hc;
   std::map<Img3dsl::ValueType, double> hcc;
   std::map<Img3dsl::ValueType, double> tc;
   std::map<Img3dsl::ValueType, float>::iterator i;

   Point3d p;

   // Compute the histogram
   for (p.z = 0; p.z < ims.Depth(); p.z++)
   for (p.y = 0; p.y < ims.Height(); p.y++)
   for (p.x = 0; p.x < ims.Width(); p.x++)
   {
      h[ims[p]]++;
   }
   // Computes the cumulated histogram and the cumulated correlation histogram.
   const long nbpix = ims.VectorSize();
   i = h.begin();
   double hcp = hc[i->first] = i->second / nbpix;
   double hccp = hcc[i->first] = hcp * hcp;
#ifdef DEBUG
   printf("hc[%d]=%lg hcc[%d]=%lg\n", (int)i->first, hc[i->first], (int)i->first, hcc[i->first]);
#endif

   for ( i++; i != h.end(); ++i) {
      double pi = i->second/nbpix;
      hcp = hc[i->first] = hcp + pi;
      hccp = hcc[i->first] = hccp + (pi * pi);
#ifdef DEBUG
      printf("hc[%d]=%lf pi*pi=%lg hcc[%d]=%lf\n", (int)i->first, hc[i->first], (float)(pi * pi), (int)i->first, hcc[i->first]);
#endif
   }

   std::map<Img3dsl::ValueType, float>::iterator begin = h.begin();
   std::map<Img3dsl::ValueType, float>::iterator end = h.end();
   std::map<Img3dsl::ValueType, float>::iterator temp;
   begin++;
   end --;
   for (i = begin; i != end; ++i) {
      temp = i;
      temp--;
      Double Ps = hc[temp->first];
      Double Gs = hcc[temp->first];
      Double Gps = hccp - Gs;
      tc[i->first] = - log(Gs * Gps) + 2.0F * log(Ps * (1.0F - Ps));
#ifdef DEBUG
       printf("Tc[%d]=%f\n", (int)i->first, tc[i->first]);
       printf("   Ps=%g, Gs=%g, Gps=%g (%g %g)\n", Ps, Gs, Gps, hccp, hcc[i->first]);
       printf("   (Gs*gps)=%g, Ps*(1-Ps)=%g)\n", (float)(Gs * Gps), (float)(Ps * (1.0F * Ps)));
#endif
   }

   // Get the maximum of the correlation to determine the threshold.
   i = begin;
   Img3dsl::ValueType maximum = (Img3dsl::ValueType)i->first;
   for (; i != end; ++i) {
#ifdef DEBUG
      printf("comparaison = %f %f\n", tc[i->first], tc[maximum]);
#endif
      if (tc[i->first] > tc[maximum]) {
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

Errc PMaximumCorrelationBinarization( const Img3dsf &ims, Img3duc &imd ) {
   std::map<Img3dsf::ValueType, float> h;
   std::map<Img3dsf::ValueType, double> hc;
   std::map<Img3dsf::ValueType, double> hcc;
   std::map<Img3dsf::ValueType, double> tc;
   std::map<Img3dsf::ValueType, float>::iterator i;

   Point3d p;

   // Compute the histogram
   for (p.z = 0; p.z < ims.Depth(); p.z++)
   for (p.y = 0; p.y < ims.Height(); p.y++)
   for (p.x = 0; p.x < ims.Width(); p.x++)
   {
      h[ims[p]]++;
   }
   // Computes the cumulated histogram and the cumulated correlation histogram.
   const long nbpix = ims.VectorSize();
   i = h.begin();
   double hcp = hc[i->first] = i->second / nbpix;
   double hccp = hcc[i->first] = hcp * hcp;
#ifdef DEBUG
   printf("hc[%d]=%lg hcc[%d]=%lg\n", (int)i->first, hc[i->first], (int)i->first, hcc[i->first]);
#endif

   for ( i++; i != h.end(); ++i) {
      double pi = i->second/nbpix;
      hcp = hc[i->first] = hcp + pi;
      hccp = hcc[i->first] = hccp + (pi * pi);
#ifdef DEBUG
      printf("hc[%d]=%lf pi*pi=%lg hcc[%d]=%lf\n", (int)i->first, hc[i->first], (float)(pi * pi), (int)i->first, hcc[i->first]);
#endif
   }

   std::map<Img3dsf::ValueType, float>::iterator begin = h.begin();
   std::map<Img3dsf::ValueType, float>::iterator end = h.end();
   std::map<Img3dsf::ValueType, float>::iterator temp;
   begin++;
   end --;
   for (i = begin; i != end; ++i) {
      temp = i;
      temp--;
      Double Ps = hc[temp->first];
      Double Gs = hcc[temp->first];
      Double Gps = hccp - Gs;
      tc[i->first] = - log(Gs * Gps) + 2.0F * log(Ps * (1.0F - Ps));
#ifdef DEBUG
       printf("Tc[%d]=%f\n", (int)i->first, tc[i->first]);
       printf("   Ps=%g, Gs=%g, Gps=%g (%g %g)\n", Ps, Gs, Gps, hccp, hcc[i->first]);
       printf("   (Gs*gps)=%g, Ps*(1-Ps)=%g)\n", (float)(Gs * Gps), (float)(Ps * (1.0F * Ps)));
#endif
   }

   // Get the maximum of the correlation to determine the threshold.
   i = begin;
   Img3dsf::ValueType maximum = (Img3dsf::ValueType)i->first;
   for (; i != end; ++i) {
#ifdef DEBUG
      printf("comparaison = %f %f\n", tc[i->first], tc[maximum]);
#endif
      if (tc[i->first] > tc[maximum]) {
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
      
      result = PMaximumCorrelationBinarization(*ims, *imd);
      goto end;
   }
   if (objs[0]->Type() == Po_Img2dsl) {
      Img2dsl* const ims = (Img2dsl*)objs[0];
      objd[0] = new Img2duc(ims->Props());
      Img2duc* const imd = (Img2duc*)objd[0];
      
      result = PMaximumCorrelationBinarization(*ims, *imd);
      goto end;
   }
   if (objs[0]->Type() == Po_Img2dsf) {
      Img2dsf* const ims = (Img2dsf*)objs[0];
      objd[0] = new Img2duc(ims->Props());
      Img2duc* const imd = (Img2duc*)objd[0];
      
      result = PMaximumCorrelationBinarization(*ims, *imd);
      goto end;
   }
   if (objs[0]->Type() == Po_Img3duc) {
      Img3duc* const ims = (Img3duc*)objs[0];
      objd[0] = new Img3duc(ims->Props());
      Img3duc* const imd = (Img3duc*)objd[0];
      
      result = PMaximumCorrelationBinarization(*ims, *imd);
      goto end;
   }
   if (objs[0]->Type() == Po_Img3dsl) {
      Img3dsl* const ims = (Img3dsl*)objs[0];
      objd[0] = new Img3duc(ims->Props());
      Img3duc* const imd = (Img3duc*)objd[0];
      
      result = PMaximumCorrelationBinarization(*ims, *imd);
      goto end;
   }
   if (objs[0]->Type() == Po_Img3dsf) {
      Img3dsf* const ims = (Img3dsf*)objs[0];
      objd[0] = new Img3duc(ims->Props());
      Img3duc* const imd = (Img3duc*)objd[0];
      
      result = PMaximumCorrelationBinarization(*ims, *imd);
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
