/* -*- c-basic-offset: 3 ; mode: c++ -*-
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

//--------------------------------------------------------------------------
// IMPORTANT NOTICE :
//--------------------
// The source code of this Pandore operator is governed by a specific
// Free-Software License (the CeCiLL License), also applying to the
// CImg Library. Please read it carefully, if you want to use this module
// in your own project (file CImg.h).
// IN PARTICULAR, YOU ARE NOT ALLOWED TO USE THIS PANDORE MODULE IN A
// CLOSED-SOURCE PROPRIETARY PROJECT WITHOUT ASKING AN AUTHORIZATION
// TO THE CIMG LIBRARY AUTHOR ( http://www.greyc.ensicaen.fr/~dtschump/ )
//--------------------------------------------------------------------------

/**
 * @author David Tschumperlé - 2005-08-31
 */

/**
 * @file pplot1d.cpp
 * @brief Dessine une fonction 1D sous forme de courbe.
 */
#include <stdio.h>
#define cimg_OS 0
#define cimg_display_type 0
#include "CImg1-16.h"
using namespace cimg_library1_16;
#include <pandore.h>
using namespace pandore;

template<typename T>
Errc PPlot1d( const Img1d<T> &ims, Imc2duc &imd, Uchar func_type = 0, Float ymin = 0, Float ymax = 0) {
   CImg<T> img(ims.Width());
   cimg_mapX(img, x) img(x) = ims[x];
   CImg<unsigned char> visu(imd.Width(), imd.Height(), 1, 3, 0);
   const unsigned char white[3] = {255, 255, 255}, purple[3] = {255, 0, 255};
   CImgStats st;
   if (ymin == ymax && ymin == 0) {
      st = CImgStats(img, false); cimg::swap(st.min, st.max);
   } else {
      st.min = ymin; st.max = ymax;
   }
   visu.draw_graph(img, purple, func_type, st.min, st.max).draw_axe(0U, img.width-1, st.min, st.max, white);
   cimg_mapXYV(visu, x, y, k) imd[k][y][x] = visu(x, y, k);
   return SUCCESS;
}



#ifdef MAIN
#define USAGE   "usage: %s dimx dimy type ymin ymax [im_in|-] [im_out|-]"
#define	PARC	5
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
   if (objs[0]->Type() == Po_Img1duc) {
      Img1duc* const ims = (Img1duc*)objs[0];
      Short dimx = (Short)atoi(parv[0]);
      Short dimy = (Short)atoi(parv[1]);
      objd[0] = new Imc2duc(dimy, dimx);
      Imc2duc *imd = (Imc2duc*)objd[0];
      result = PPlot1d(*ims, *imd, (Uchar)atoi(parv[2]), (Float)atof(parv[3]), (Float)atof(parv[4]));
      goto end;
   }
   if (objs[0]->Type() == Po_Img1dsl) {
      Img1dsl* const ims = (Img1dsl*)objs[0];
      Short dimx = (Short)atoi(parv[0]);
      Short dimy = (Short)atoi(parv[1]);
      objd[0] = new Imc2duc(dimy, dimx);
      Imc2duc *imd = (Imc2duc*)objd[0];
      result = PPlot1d(*ims, *imd, (Uchar)atoi(parv[2]), (Float)atof(parv[3]), (Float)atof(parv[4]));
      goto end;
   }
   if (objs[0]->Type() == Po_Img1dsf) {
      Img1dsf* const ims = (Img1dsf*)objs[0];
      Short dimx = (Short)atoi(parv[0]);
      Short dimy = (Short)atoi(parv[1]);
      objd[0] = new Imc2duc(dimy, dimx);
      Imc2duc *imd = (Imc2duc*)objd[0];
      result = PPlot1d(*ims, *imd, (Uchar)atoi(parv[2]), (Float)atof(parv[3]), (Float)atof(parv[4]));
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

