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

/**
 * @author Régis Clouard - 1994-10-13
 * @author Régis Clouard - 2003-01-02
 * @author Régis Clouard - 2004-03-03 (bord).
 * @author Régis Clouard - 2005-10-21 (+ new parameter).
 * @author Régis Clouard - May 09, 2010 (with borders).
 * @author Régis Clouard - Jun 11, 2013 (new algorithm with integral image).
 */

#include <pandore.h>
using namespace pandore;

/**
 * @file pmeanfiltering.cpp
 * Filtre moyenneur.
 *
 * Convolutes image with a linear mask : 1.
 * For example, halfsize=1, the mask is
 *	 1/9 1/9 1/9
 *	 1/9 1/9 1/9
 *	 1/9 1/9 1/9
 */

template < typename T >
Errc PMeanFiltering( const Imx1d<T> &ims, Imx1d<T> &imd, const int halfsize ) {  
   if (halfsize < 1 || 2 * halfsize >= ims.Width() ) {
      std::cerr << "Error pmeanfiltering: parameter 'halfsize' out of bound:" << halfsize << std::endl;
   }

   float total = SQR(2.0F * halfsize + 1);
   float s;
   const Long bands = ims.Bands();
   const Long width = ims.Width();

   Img1d<Float> imi(ims.Props());
   
   for (int b = 0; b < bands; b++) {
      // columns
      Point1d p;
      
      for (p.x = 0; p.x < width; p.x++ ) {
	 s = 0.0F;
	 Point1d px = p;
	 for (Long m = -halfsize; m <= halfsize; m++) {
	    px.x = p.x + m;
	    if (px.x < 0) {
	       px.x = 0;
	    } else if (px.x >= width) {
	       px.x = width - 1;
	    }
	    s += ims[b][px];
	 }
	 imd[b][p] = (T)(s / total);
      }
   }
   return SUCCESS;
}

template < typename T >
Errc PMeanFiltering( const Imx2d<T> &ims, Imx2d<T> &imd, const int halfsize ) {  
   if (halfsize < 1 || 2 * halfsize >= ims.Width() || 2 * halfsize >= ims.Height()) {
      std::cerr << "Error pmeanfiltering: parameter 'halfsize' out of bound:" << halfsize << std::endl;
   }

  Img2dsf integral(ims.Size());

  for (int b = 0; b < ims.Bands(); b++) {
     float sum = 0;
     for (int x = 0; x < ims.Width(); x++) {
	sum += (float)ims(b, 0, x);
	integral(0, x) = sum;
     }
     for (int y = 1; y < ims.Height(); y++) {
	float sum = 0;
	for (int x = 0; x < ims.Width(); x++) {
	   sum += (float)ims(b, y, x);
	   integral(y, x) = integral(y - 1, x) + sum;
	}
     }
     
     // Binarization
     for (int y = 0; y < ims.Height(); y++) {
	for (int x = 0; x < ims.Width(); x++) {
	   const int y1 = (y - halfsize <= 1) ? 1 : y - halfsize;
	   const int y2 = (y + halfsize >= ims.Height()) ? ims.Height() - 1 : y + halfsize;
	   
	   const int x1 = (x - halfsize <= 1) ? 1 : x - halfsize;
	   const int x2 = (x + halfsize >= ims.Width()) ? ims.Width() - 1 : x + halfsize;
	   const float count = (float)((x2 - x1 + 1) * (y2 - y1 + 1));
	   const float sum = integral(y2, x2) - integral(y1 - 1, x2) - integral(y2, x1 - 1) + integral(y1 - 1, x1 - 1);
	   
	   float mean = sum/count;
	   if (mean > Limits<T>::max()) { // Due to float approximation.
	      mean = (float)Limits<T>::max();
	   } else if (mean < Limits<T>::min()) {
	      mean = (float)Limits<T>::min();
	   }
	   imd(b, y, x) = (T)mean;
	}
     }
  }
  
  return SUCCESS;
}

class IntegralImage {
   private:
      int depth;
      int height;
      int width;
      Double *data;
      
   public:
      IntegralImage( int d, int h, int w ): depth(d), height(h), width(w) {
	 data = new Double[d*h*w];
      }
      
      ~IntegralImage() {
	 delete[] data;
      }
      Double& operator()(int z, int y, int x) {
	 return data[z * (width * height) + y * width + x];
      }
};

template < typename T >
Errc PMeanFiltering( const Imx3d<T> &ims, Imx3d<T> &imd, const int halfsize ) { 
   if (halfsize < 1 || 2 * halfsize >= ims.Width()
       || 2 * halfsize >= ims.Height()
       || 2 * halfsize >= ims.Depth()) {
      std::cerr << "Error pmeanfiltering: parameter 'halfsize' out of bound:" << halfsize << std::endl;
   }

   // Compute the integral image
   IntegralImage integral(ims.Depth(), ims.Height(), ims.Width());
   //   Img3dsf integral(ims.Size());
   
   for (int b = 0; b < ims.Bands(); b++) {
      for (int z = 0; z < ims.Depth(); z++) {
	 for (int y = 0; y < ims.Height(); y++) {
	    float sum = 0;
	    for (int x = 0; x < ims.Width(); x++) {
	       sum += (float)ims(b, z, y, x);
	       if (z == 0) {
		  if (y == 0) {
		     integral(z, y, x) = sum;
		  } else {
		     integral(z, y, x) = integral(z, y - 1, x)  + sum;
		  }
	       } else {
		  if (y == 0) {
		     integral(z, y, x) = integral(z - 1, y, x) + sum;
		  } else {	
		     integral(z, y, x) = integral(z - 1, y, x) 
			+ integral(z, y - 1, x)
			- integral(z - 1, y - 1, x)
			+ sum;
		  }
	       }
	    }
	 }
      }

      // Binarization
      for (int z = 0; z < ims.Depth(); z++) {
	 for (int y = 0; y < ims.Height(); y++) {
	    for (int x = 0; x < ims.Width(); x++) {
	       const int z1 = (z - halfsize <= 1) ? 1 : z - halfsize;
	       const int z2 = (z + halfsize >= ims.Depth()) ? ims.Depth() - 1 : z + halfsize;

	       const int y1 = (y - halfsize <= 1) ? 1 : y - halfsize;
	       const int y2 = (y + halfsize >= ims.Height()) ? ims.Height() - 1 : y + halfsize;
	       
	       const int x1 = (x - halfsize <= 1) ? 1 : x - halfsize;
	       const int x2 = (x + halfsize >= ims.Width()) ? ims.Width() - 1 : x + halfsize;
	       const float count = (float)((x2 - x1 + 1) * (y2 - y1 + 1) * (z2 - z1 + 1) );
	       
	       const float sum1 = (float)(integral(z2, y2, x2)
		  - integral(z2, y2, x1 - 1) - integral(z1 - 1, y2, x2)
		  + integral(z1 - 1, y2, x1 - 1));
	       const float sum2 = (float)(integral(z2, y1-1, x2)
		  - integral(z2, y1 - 1, x1 - 1) - integral(z1 - 1, y1 - 1, x2)
		  + integral(z1 - 1, y1-1, x1 - 1));
	       
	       const float sum = sum1 - sum2;
	       float mean = sum/count;
	       if (mean > Limits<T>::max()) { // Due to float approximation.
		  mean = (float)Limits<T>::max();
	       } else if (mean < Limits<T>::min()) {
		  mean = (float)Limits<T>::min();
	       }
	       imd(b, z, y, x) = (T)mean;
	    }
	 }
      }
   }

   return SUCCESS;
}




Errc PMeanFiltering( const Graph2d &gs, Graph2d &gd, int ) {
   int i;
   GEdge *ptr;
   Ulong nb;
  
   gd = gs;
   for (i = 1; i < gd.Size(); i++) {
      nb = 0;
      gd[i]->value = gs[i]->value;
      for (ptr = gs[i]->Neighbours(); ptr != NULL; ptr = ptr->Next()) {
	 nb++;
	 gd[i]->value += gs[ptr->Node()]->value;
      }
      if (nb) {
	 gd[i]->value /= nb;
      }
   }
  
   return SUCCESS;
}

Errc PMeanFiltering( const Graph3d &gs, Graph3d &gd, int ) {
   int i;
   GEdge *ptr;
   Ulong nb;
  
   gd = gs;
   for (i = 1; i < gd.Size(); i++) {
      nb = 0;
      gd[i]->value = gs[i]->value;
      for (ptr = gs[i]->Neighbours(); ptr != NULL; ptr = ptr->Next()) {
	 nb++;
	 gd[i]->value += gs[ptr->Node()]->value;
      }
      if (nb) {
	 gd[i]->value /= nb;
      }
   }
  
   return SUCCESS;
}


#ifdef MAIN
	
/*
 * Modify only the following constants, and the function call.
 */
#define	USAGE	"usage: %s halfsize [-m mask] [im_in|-] [im_out|-]"
#define	PARC	1
#define	FINC	1
#define	FOUTC	1
#define	MASK	3

int main( int argc, char *argv[] ) {
   Errc result;                // The result code of the execution.
   Pobject* mask;              // The region map.
   Pobject* objin[FINC + 1];   // The input objects.
   Pobject* objs[FINC + 1];    // The source objects masked.
   Pobject* objout[FOUTC + 1]; // The output object.
   Pobject* objd[FOUTC + 1];   // The result object of the execution.
   char* parv[PARC + 1];       // The input parameters.

   ReadArgs(argc, argv, PARC, FINC, FOUTC, &mask, objin, objs, objout, objd, parv, USAGE, MASK); 
   if (objs[0]->Type() == Po_Img1duc) {
      Img1duc* const ims = (Img1duc*)objs[0];
      objd[0] = new Img1duc(ims->Props());
      Img1duc* const imd = (Img1duc*)objd[0];
      
      result = PMeanFiltering(*ims, *imd, atoi(parv[0]));
      goto end;
   }
   if (objs[0]->Type() == Po_Img1dsl) {
      Img1dsl* const ims = (Img1dsl*)objs[0];
      objd[0] = new Img1dsl(ims->Props());
      Img1dsl* const imd = (Img1dsl*)objd[0];
      
      result = PMeanFiltering(*ims, *imd, atoi(parv[0]));
      goto end;
   }
   if (objs[0]->Type() == Po_Img1dsf) {
      Img1dsf* const ims = (Img1dsf*)objs[0];
      objd[0] = new Img1dsf(ims->Props());
      Img1dsf* const imd = (Img1dsf*)objd[0];
      
      result = PMeanFiltering(*ims, *imd, atoi(parv[0]));
      goto end;
   }
   if (objs[0]->Type() == Po_Img2duc) {
      Img2duc* const ims = (Img2duc*)objs[0];
      objd[0] = new Img2duc(ims->Props());
      Img2duc* const imd = (Img2duc*)objd[0];
      
      result = PMeanFiltering(*ims, *imd, atoi(parv[0]));
      goto end;
   }
   if (objs[0]->Type() == Po_Img2dsl) {
      Img2dsl* const ims = (Img2dsl*)objs[0];
      objd[0] = new Img2dsl(ims->Props());
      Img2dsl* const imd = (Img2dsl*)objd[0];
      
      result = PMeanFiltering(*ims, *imd, atoi(parv[0]));
      goto end;
   }
   if (objs[0]->Type() == Po_Img2dsf) {
      Img2dsf* const ims = (Img2dsf*)objs[0];
      objd[0] = new Img2dsf(ims->Props());
      Img2dsf* const imd = (Img2dsf*)objd[0];
      
      result = PMeanFiltering(*ims, *imd, atoi(parv[0]));
      goto end;
   }
   if (objs[0]->Type() == Po_Img3duc) {
      Img3duc* const ims = (Img3duc*)objs[0];
      objd[0] = new Img3duc(ims->Props());
      Img3duc* const imd = (Img3duc*)objd[0];
      
      result = PMeanFiltering(*ims, *imd, atoi(parv[0]));
      goto end;
   }
   if (objs[0]->Type() == Po_Img3dsl) {
      Img3dsl* const ims = (Img3dsl*)objs[0];
      objd[0] = new Img3dsl(ims->Props());
      Img3dsl* const imd = (Img3dsl*)objd[0];
      
      result = PMeanFiltering(*ims, *imd, atoi(parv[0]));
      goto end;
   }
   if (objs[0]->Type() == Po_Img3dsf) {
      Img3dsf* const ims = (Img3dsf*)objs[0];
      objd[0] = new Img3dsf(ims->Props());
      Img3dsf* const imd = (Img3dsf*)objd[0];
      
      result = PMeanFiltering(*ims, *imd, atoi(parv[0]));
      goto end;
   }
   if (objs[0]->Type() == Po_Imc2duc) {
      Imc2duc* const ims = (Imc2duc*)objs[0];
      objd[0] = new Imc2duc(ims->Props());
      Imc2duc* const imd = (Imc2duc*)objd[0];
      
      result = PMeanFiltering(*ims, *imd, atoi(parv[0]));
      goto end;
   }
   if (objs[0]->Type() == Po_Imc2dsl) {
      Imc2dsl* const ims = (Imc2dsl*)objs[0];
      objd[0] = new Imc2dsl(ims->Props());
      Imc2dsl* const imd = (Imc2dsl*)objd[0];
      
      result = PMeanFiltering(*ims, *imd, atoi(parv[0]));
      goto end;
   }
   if (objs[0]->Type() == Po_Imc2dsf) {
      Imc2dsf* const ims = (Imc2dsf*)objs[0];
      objd[0] = new Imc2dsf(ims->Props());
      Imc2dsf* const imd = (Imc2dsf*)objd[0];
      
      result = PMeanFiltering(*ims, *imd, atoi(parv[0]));
      goto end;
   }
   if (objs[0]->Type() == Po_Imc3duc) {
      Imc3duc* const ims = (Imc3duc*)objs[0];
      objd[0] = new Imc3duc(ims->Props());
      Imc3duc* const imd = (Imc3duc*)objd[0];
      
      result = PMeanFiltering(*ims, *imd, atoi(parv[0]));
      goto end;
   }
   if (objs[0]->Type() == Po_Imc3dsl) {
      Imc3dsl* const ims = (Imc3dsl*)objs[0];
      objd[0] = new Imc3dsl(ims->Props());
      Imc3dsl* const imd = (Imc3dsl*)objd[0];
      
      result = PMeanFiltering(*ims, *imd, atoi(parv[0]));
      goto end;
   }
   if (objs[0]->Type() == Po_Imc3dsf) {
      Imc3dsf* const ims = (Imc3dsf*)objs[0];
      objd[0] = new Imc3dsf(ims->Props());
      Imc3dsf* const imd = (Imc3dsf*)objd[0];
      
      result = PMeanFiltering(*ims, *imd, atoi(parv[0]));
      goto end;
   }
   if (objs[0]->Type() == Po_Imx1duc) {
      Imx1duc* const ims = (Imx1duc*)objs[0];
      objd[0] = new Imx1duc(ims->Props());
      Imx1duc* const imd = (Imx1duc*)objd[0];
      
      result = PMeanFiltering(*ims, *imd, atoi(parv[0]));
      goto end;
   }
   if (objs[0]->Type() == Po_Imx1dsl) {
      Imx1dsl* const ims = (Imx1dsl*)objs[0];
      objd[0] = new Imx1dsl(ims->Props());
      Imx1dsl* const imd = (Imx1dsl*)objd[0];
      
      result = PMeanFiltering(*ims, *imd, atoi(parv[0]));
      goto end;
   }
   if (objs[0]->Type() == Po_Imx1dsf) {
      Imx1dsf* const ims = (Imx1dsf*)objs[0];
      objd[0] = new Imx1dsf(ims->Props());
      Imx1dsf* const imd = (Imx1dsf*)objd[0];
      
      result = PMeanFiltering(*ims, *imd, atoi(parv[0]));
      goto end;
   }
   if (objs[0]->Type() == Po_Imx2duc) {
      Imx2duc* const ims = (Imx2duc*)objs[0];
      objd[0] = new Imx2duc(ims->Props());
      Imx2duc* const imd = (Imx2duc*)objd[0];
      
      result = PMeanFiltering(*ims, *imd, atoi(parv[0]));
      goto end;
   }
   if (objs[0]->Type() == Po_Imx2dsl) {
      Imx2dsl* const ims = (Imx2dsl*)objs[0];
      objd[0] = new Imx2dsl(ims->Props());
      Imx2dsl* const imd = (Imx2dsl*)objd[0];
      
      result = PMeanFiltering(*ims, *imd, atoi(parv[0]));
      goto end;
   }
   if (objs[0]->Type() == Po_Imx2dsf) {
      Imx2dsf* const ims = (Imx2dsf*)objs[0];
      objd[0] = new Imx2dsf(ims->Props());
      Imx2dsf* const imd = (Imx2dsf*)objd[0];
      
      result = PMeanFiltering(*ims, *imd, atoi(parv[0]));
      goto end;
   }
   if (objs[0]->Type() == Po_Imx3duc) {
      Imx3duc* const ims = (Imx3duc*)objs[0];
      objd[0] = new Imx3duc(ims->Props());
      Imx3duc* const imd = (Imx3duc*)objd[0];
      
      result = PMeanFiltering(*ims, *imd, atoi(parv[0]));
      goto end;
   }
   if (objs[0]->Type() == Po_Imx3dsl) {
      Imx3dsl* const ims = (Imx3dsl*)objs[0];
      objd[0] = new Imx3dsl(ims->Props());
      Imx3dsl* const imd = (Imx3dsl*)objd[0];
      
      result = PMeanFiltering(*ims, *imd, atoi(parv[0]));
      goto end;
   }
   if (objs[0]->Type() == Po_Imx3dsf) {
      Imx3dsf* const ims = (Imx3dsf*)objs[0];
      objd[0] = new Imx3dsf(ims->Props());
      Imx3dsf* const imd = (Imx3dsf*)objd[0];
      
      result = PMeanFiltering(*ims, *imd, atoi(parv[0]));
      goto end;
   }
   if (objs[0]->Type() == Po_Graph2d) {
      Graph2d* const ims = (Graph2d*)objs[0];
      objd[0] = new Graph2d(ims->Props());
      Graph2d* const imd = (Graph2d*)objd[0];
      
      result = PMeanFiltering(*ims, *imd, atoi(parv[0]));
      goto end;
   }
   if (objs[0]->Type() == Po_Graph3d) {
      Graph3d* const ims = (Graph3d*)objs[0];
      objd[0] = new Graph3d(ims->Props());
      Graph3d* const imd = (Graph3d*)objd[0];
      
      result = PMeanFiltering(*ims, *imd, atoi(parv[0]));
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
