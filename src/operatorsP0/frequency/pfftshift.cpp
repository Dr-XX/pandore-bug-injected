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

#include <pandore.h>
using namespace pandore;

/**
 * @file pfftshift.cpp
 * Fait une permutation des 4 sous-images. (1<-> 4, 2<-> 3)
 */

template <typename T>
Errc PFFTShift( const Img2d<T> &imsr, const Img2d<T> &imsi, Img2d<T> &imdr, Img2d<T> &imdi ) {
   int midx = imsr.Width() >> 1;
   int midy = imsr.Height() >> 1;
   int finx, finy;
   int y;

   // Cas impair -> +1
   finx = (imsr.Width() % 2 == 1)?  midx + 1 : midx;
   finy = (imsr.Height() % 2 == 1)?  midy + 1 : midy;

   // Cadre 1 -> Cadre 4.
   for (y = 0; y < finy; y++) {
      for (int x = 0; x < finx; x++) {
	 imdr[y][x] = imsr[y + midy][x + midx];
	 imdi[y][x] = imsi[y + midy][x + midx];
      }
   }
   // Cadre 2 -> Cadre 4.
   for (y = 0; y < finy; y++) {
      for (int x = finx; x < imsr.Width(); x++) {
	 imdr[y][x] = imsr[y + midy][x - finx];
	 imdi[y][x] = imsi[y + midy][x - finx];
      }
   }
   
   // Cadre 3 -> cadre 2.
   for (y = finy ; y < imsr.Height(); y++) {
      for (int x = 0; x < finx; x++) {
	 imdr[y][x] = imsr[y - finy][x + midx];
	 imdi[y][x] = imsi[y - finy][x + midx];
      }
   }
   // Cadre 4 -> Cadre 1.
   for (y = finy; y < imsr.Height(); y++) {
      for (int x = finx; x < imsr.Width(); x++) {
	 imdr[y][x] = imsr[y - finy][x - finx];
	 imdi[y][x] = imsi[y - finy][x - finx];
      }
   }
   return SUCCESS;
}

/*
 * Modify only the following constants, and the function call.
 */
#ifdef MAIN
#define	USAGE	"usage: %s [-m mask] [imr_in|-] [imi_in|-] [imr_out|-] [imi_out|-]"
#define	PARC	0
#define	FINC	2
#define	FOUTC	2
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
      Img2duc* const ims_reel = (Img2duc*)objs[0];
      Img2duc* const ims_imag = (Img2duc*)objs[1];
      objd[0] = new Img2duc(ims_reel->Size());
      Img2duc* const imd_reel = (Img2duc*)objd[0];
      objd[1] = new Img2duc(ims_imag->Size());
      Img2duc* const imd_imag = (Img2duc*)objd[1];
      
      result = PFFTShift(*ims_reel, *ims_imag, *imd_reel, *imd_imag);
      
      goto end;
   }
   if (objs[0]->Type() == Po_Img2dsl) {
      Img2dsl* const ims_reel = (Img2dsl*)objs[0];
      Img2dsl* const ims_imag = (Img2dsl*)objs[1];
      objd[0] = new Img2dsl(ims_reel->Size());
      Img2dsl* const imd_reel = (Img2dsl*)objd[0];
      objd[1] = new Img2dsl(ims_imag->Size());
      Img2dsl* const imd_imag = (Img2dsl*)objd[1];
      
      result = PFFTShift(*ims_reel, *ims_imag, *imd_reel, *imd_imag);
      
      goto end;
   }
   if (objs[0]->Type() == Po_Img2dsf) {
      Img2dsf* const ims_reel = (Img2dsf*)objs[0];
      Img2dsf* const ims_imag = (Img2dsf*)objs[1];
      objd[0] = new Img2dsf(ims_reel->Size());
      Img2dsf* const imd_reel = (Img2dsf*)objd[0];
      objd[1] = new Img2dsf(ims_imag->Size());
      Img2dsf* const imd_imag = (Img2dsf*)objd[1];
      
      result = PFFTShift(*ims_reel, *ims_imag, *imd_reel, *imd_imag);
      
      goto end;
   }
   {
      PrintErrorFormat(objin, FINC);
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
