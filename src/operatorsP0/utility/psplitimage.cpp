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
 * @author Ludovic SOLTYS - 02-02-2003
 * @author Regis Clouard - 2004-08-04 (add types)
 */

#include <stdio.h>
#include <string.h>
#include <pandore.h>
using namespace pandore;

/**
 * @file psplitimage.cpp
 * Eclate une image en 4 sous-images de dimensions moities.
 */




Errc PSplitImage( const Img2duc &ims, Img2duc &imd0, Img2duc &imd1, Img2duc &imd2, Img2duc &imd3 ) {
   int h = imd0.Height();
   int w = imd0.Width();
   
   for (int x = 0; x < w; x++) {
      for (int y = 0; y < h; y++) {
	 imd0[y][x] = ims[y][x];
	 imd1[y][x] = ims[y][x + w];
	 imd2[y][x] = ims[y + h][x];
	 imd3[y][x] = ims[y + h][x + w];
      }
   }
   
   return SUCCESS;
}

Errc PSplitImage( const Img2dsl &ims, Img2dsl &imd0, Img2dsl &imd1, Img2dsl &imd2, Img2dsl &imd3 ) {
   int h = imd0.Height();
   int w = imd0.Width();
   
   for (int x = 0; x < w; x++) {
      for (int y = 0; y < h; y++) {
	 imd0[y][x] = ims[y][x];
	 imd1[y][x] = ims[y][x + w];
	 imd2[y][x] = ims[y + h][x];
	 imd3[y][x] = ims[y + h][x + w];
      }
   }
   
   return SUCCESS;
}

Errc PSplitImage( const Img2dsf &ims, Img2dsf &imd0, Img2dsf &imd1, Img2dsf &imd2, Img2dsf &imd3 ) {
   int h = imd0.Height();
   int w = imd0.Width();
   
   for (int x = 0; x < w; x++) {
      for (int y = 0; y < h; y++) {
	 imd0[y][x] = ims[y][x];
	 imd1[y][x] = ims[y][x + w];
	 imd2[y][x] = ims[y + h][x];
	 imd3[y][x] = ims[y + h][x + w];
      }
   }
   
   return SUCCESS;
}

Errc PSplitImage( const Imc2duc &ims, Imc2duc &imd0, Imc2duc &imd1, Imc2duc &imd2, Imc2duc &imd3 ) {
   imd0.ColorSpace(ims.ColorSpace());
   imd1.ColorSpace(ims.ColorSpace());
   imd2.ColorSpace(ims.ColorSpace());
   imd3.ColorSpace(ims.ColorSpace());
   int h = imd0.Height();
   int w = imd0.Width();
   
   for (int x = 0; x < w; x++) {
      for (int y = 0; y < h; y++) {
	 for (int b = 0; b < ims.Bands(); b++) {
	    imd0[b][y][x] = ims[b][y][x];
	    imd1[b][y][x] = ims[b][y][x + w];
	    imd2[b][y][x] = ims[b][y + h][x];
	    imd3[b][y][x] = ims[b][y + h][x + w];
	 }
      }
   }
   
   return SUCCESS;
}

Errc PSplitImage( const Imc2dsl &ims, Imc2dsl &imd0, Imc2dsl &imd1, Imc2dsl &imd2, Imc2dsl &imd3 ) {
   imd0.ColorSpace(ims.ColorSpace());
   imd1.ColorSpace(ims.ColorSpace());
   imd2.ColorSpace(ims.ColorSpace());
   imd3.ColorSpace(ims.ColorSpace());
   int h = imd0.Height();
   int w = imd0.Width();
   
   for (int x = 0; x < w; x++) {
      for (int y = 0; y < h; y++) {
	 for (int b = 0; b < ims.Bands(); b++) {
	    imd0[b][y][x] = ims[b][y][x];
	    imd1[b][y][x] = ims[b][y][x + w];
	    imd2[b][y][x] = ims[b][y + h][x];
	    imd3[b][y][x] = ims[b][y + h][x + w];
	 }
      }
   }
   
   return SUCCESS;
}

Errc PSplitImage( const Imc2dsf &ims, Imc2dsf &imd0, Imc2dsf &imd1, Imc2dsf &imd2, Imc2dsf &imd3 ) {
   imd0.ColorSpace(ims.ColorSpace());
   imd1.ColorSpace(ims.ColorSpace());
   imd2.ColorSpace(ims.ColorSpace());
   imd3.ColorSpace(ims.ColorSpace());
   int h = imd0.Height();
   int w = imd0.Width();
   
   for (int x = 0; x < w; x++) {
      for (int y = 0; y < h; y++) {
	 for (int b = 0; b < ims.Bands(); b++) {
	    imd0[b][y][x] = ims[b][y][x];
	    imd1[b][y][x] = ims[b][y][x + w];
	    imd2[b][y][x] = ims[b][y + h][x];
	    imd3[b][y][x] = ims[b][y + h][x + w];
	 }
      }
   }
   
   return SUCCESS;
}

Errc PSplitImage( const Imx2duc &ims, Imx2duc &imd0, Imx2duc &imd1, Imx2duc &imd2, Imx2duc &imd3 ) {
   int h = imd0.Height();
   int w = imd0.Width();
   
   for (int x = 0; x < w; x++) {
      for (int y = 0; y < h; y++) {
	 for (int b = 0;b < ims.Bands(); b++) {
	    imd0[b][y][x] = ims[b][y][x];
	    imd1[b][y][x] = ims[b][y][x + w];
	    imd2[b][y][x] = ims[b][y + h][x];
	    imd3[b][y][x] = ims[b][y + h][x + w];
	 }
      }
   }
   return SUCCESS;
}

Errc PSplitImage( const Imx2dsl &ims, Imx2dsl &imd0, Imx2dsl &imd1, Imx2dsl &imd2, Imx2dsl &imd3 ) {
   int h = imd0.Height();
   int w = imd0.Width();
   
   for (int x = 0; x < w; x++) {
      for (int y = 0; y < h; y++) {
	 for (int b = 0;b < ims.Bands(); b++) {
	    imd0[b][y][x] = ims[b][y][x];
	    imd1[b][y][x] = ims[b][y][x + w];
	    imd2[b][y][x] = ims[b][y + h][x];
	    imd3[b][y][x] = ims[b][y + h][x + w];
	 }
      }
   }
   return SUCCESS;
}

Errc PSplitImage( const Imx2dsf &ims, Imx2dsf &imd0, Imx2dsf &imd1, Imx2dsf &imd2, Imx2dsf &imd3 ) {
   int h = imd0.Height();
   int w = imd0.Width();
   
   for (int x = 0; x < w; x++) {
      for (int y = 0; y < h; y++) {
	 for (int b = 0;b < ims.Bands(); b++) {
	    imd0[b][y][x] = ims[b][y][x];
	    imd1[b][y][x] = ims[b][y][x + w];
	    imd2[b][y][x] = ims[b][y + h][x];
	    imd3[b][y][x] = ims[b][y + h][x + w];
	 }
      }
   }
   return SUCCESS;
}


#ifdef MAIN
#define	USAGE	"usage: %s  [im_in|-] [im_out0|-] [im_out1|-] [im_out2|-] [im_out3|-]"
#define	PARC	0
#define	FINC	1
#define	FOUTC	4
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
   if (objs[0]->Type() == Po_Img2duc) {
      Img2duc* const ims = (Img2duc*)objs[0];	
      
      objd[0] = new Img2duc((ims->Height()) / 2, (ims->Width()) / 2);
      objd[1] = new Img2duc((ims->Height()) / 2, (ims->Width()) / 2);
      objd[2] = new Img2duc((ims->Height()) / 2, (ims->Width()) / 2);
      objd[3] = new Img2duc((ims->Height()) / 2, (ims->Width()) / 2);
      
      Img2duc* const imd0 = (Img2duc*)objd[0];
      Img2duc* const imd1 = (Img2duc*)objd[1];
      Img2duc* const imd2 = (Img2duc*)objd[2];
      Img2duc* const imd3 = (Img2duc*)objd[3];
      
      result = PSplitImage(*ims, *imd0, *imd1, *imd2, *imd3);
      goto end;
   }

   if (objs[0]->Type() == Po_Img2dsl) {
      Img2dsl* const ims = (Img2dsl*)objs[0];	
      
      objd[0] = new Img2dsl((ims->Height()) / 2, (ims->Width()) / 2);
      objd[1] = new Img2dsl((ims->Height()) / 2, (ims->Width()) / 2);
      objd[2] = new Img2dsl((ims->Height()) / 2, (ims->Width()) / 2);
      objd[3] = new Img2dsl((ims->Height()) / 2, (ims->Width()) / 2);
      
      Img2dsl* const imd0 = (Img2dsl*)objd[0];
      Img2dsl* const imd1 = (Img2dsl*)objd[1];
      Img2dsl* const imd2 = (Img2dsl*)objd[2];
      Img2dsl* const imd3 = (Img2dsl*)objd[3];
      
      result = PSplitImage(*ims, *imd0, *imd1, *imd2, *imd3);
      goto end;
   }

   if (objs[0]->Type() == Po_Img2dsf) {
      Img2dsf* const ims = (Img2dsf*)objs[0];	
      
      objd[0] = new Img2dsf((ims->Height()) / 2, (ims->Width()) / 2);
      objd[1] = new Img2dsf((ims->Height()) / 2, (ims->Width()) / 2);
      objd[2] = new Img2dsf((ims->Height()) / 2, (ims->Width()) / 2);
      objd[3] = new Img2dsf((ims->Height()) / 2, (ims->Width()) / 2);
      
      Img2dsf* const imd0 = (Img2dsf*)objd[0];
      Img2dsf* const imd1 = (Img2dsf*)objd[1];
      Img2dsf* const imd2 = (Img2dsf*)objd[2];
      Img2dsf* const imd3 = (Img2dsf*)objd[3];
      
      result = PSplitImage(*ims, *imd0, *imd1, *imd2, *imd3);
      goto end;
   }

   if (objs[0]->Type() == Po_Imc2duc) {
      Imc2duc* const ims = (Imc2duc*)objs[0];	
      
      objd[0] = new Imc2duc((ims->Height()) / 2, (ims->Width()) / 2);
      objd[1] = new Imc2duc((ims->Height()) / 2, (ims->Width()) / 2);
      objd[2] = new Imc2duc((ims->Height()) / 2, (ims->Width()) / 2);
      objd[3] = new Imc2duc((ims->Height()) / 2, (ims->Width()) / 2);
      
      Imc2duc* const imd0 = (Imc2duc*)objd[0];
      Imc2duc* const imd1 = (Imc2duc*)objd[1];
      Imc2duc* const imd2 = (Imc2duc*)objd[2];
      Imc2duc* const imd3 = (Imc2duc*)objd[3];
      
      result = PSplitImage(*ims, *imd0, *imd1, *imd2, *imd3);
      goto end;
   }

   if (objs[0]->Type() == Po_Imc2dsl) {
      Imc2dsl* const ims = (Imc2dsl*)objs[0];	
      
      objd[0] = new Imc2dsl((ims->Height()) / 2, (ims->Width()) / 2);
      objd[1] = new Imc2dsl((ims->Height()) / 2, (ims->Width()) / 2);
      objd[2] = new Imc2dsl((ims->Height()) / 2, (ims->Width()) / 2);
      objd[3] = new Imc2dsl((ims->Height()) / 2, (ims->Width()) / 2);
      
      Imc2dsl* const imd0 = (Imc2dsl*)objd[0];
      Imc2dsl* const imd1 = (Imc2dsl*)objd[1];
      Imc2dsl* const imd2 = (Imc2dsl*)objd[2];
      Imc2dsl* const imd3 = (Imc2dsl*)objd[3];
      
      result = PSplitImage(*ims, *imd0, *imd1, *imd2, *imd3);
      goto end;
   }

   if (objs[0]->Type() == Po_Imc2dsf) {
      Imc2dsf* const ims = (Imc2dsf*)objs[0];	
      
      objd[0] = new Imc2dsf((ims->Height()) / 2, (ims->Width()) / 2);
      objd[1] = new Imc2dsf((ims->Height()) / 2, (ims->Width()) / 2);
      objd[2] = new Imc2dsf((ims->Height()) / 2, (ims->Width()) / 2);
      objd[3] = new Imc2dsf((ims->Height()) / 2, (ims->Width()) / 2);
      
      Imc2dsf* const imd0 = (Imc2dsf*)objd[0];
      Imc2dsf* const imd1 = (Imc2dsf*)objd[1];
      Imc2dsf* const imd2 = (Imc2dsf*)objd[2];
      Imc2dsf* const imd3 = (Imc2dsf*)objd[3];
      
      result = PSplitImage(*ims, *imd0, *imd1, *imd2, *imd3);
      goto end;
   }

   if (objs[0]->Type() == Po_Imx2duc) {
      Imx2duc* const ims = (Imx2duc*)objs[0];
      
      objd[0] = new Imx2duc(ims->Bands(), (ims->Height()) / 2, (ims->Width()) / 2);
      objd[1] = new Imx2duc(ims->Bands(), (ims->Height()) / 2, (ims->Width()) / 2);
      objd[2] = new Imx2duc(ims->Bands(), (ims->Height()) / 2, (ims->Width()) / 2);
      objd[3] = new Imx2duc(ims->Bands(), (ims->Height()) / 2, (ims->Width()) / 2);
       
      Imx2duc* const imd0 = (Imx2duc*)objd[0];
      Imx2duc* const imd1 = (Imx2duc*)objd[1];
      Imx2duc* const imd2 = (Imx2duc*)objd[2];
      Imx2duc* const imd3 = (Imx2duc*)objd[3];
      
      result = PSplitImage(*ims, *imd0, *imd1, *imd2, *imd3);
      goto end;
   }

   if (objs[0]->Type() == Po_Imx2dsl) {
      Imx2dsl* const ims = (Imx2dsl*)objs[0];
      
      objd[0] = new Imx2dsl(ims->Bands(), (ims->Height()) / 2, (ims->Width()) / 2);
      objd[1] = new Imx2dsl(ims->Bands(), (ims->Height()) / 2, (ims->Width()) / 2);
      objd[2] = new Imx2dsl(ims->Bands(), (ims->Height()) / 2, (ims->Width()) / 2);
      objd[3] = new Imx2dsl(ims->Bands(), (ims->Height()) / 2, (ims->Width()) / 2);
       
      Imx2dsl* const imd0 = (Imx2dsl*)objd[0];
      Imx2dsl* const imd1 = (Imx2dsl*)objd[1];
      Imx2dsl* const imd2 = (Imx2dsl*)objd[2];
      Imx2dsl* const imd3 = (Imx2dsl*)objd[3];
      
      result = PSplitImage(*ims, *imd0, *imd1, *imd2, *imd3);
      goto end;
   }

   if (objs[0]->Type() == Po_Imx2dsf) {
      Imx2dsf* const ims = (Imx2dsf*)objs[0];
      
      objd[0] = new Imx2dsf(ims->Bands(), (ims->Height()) / 2, (ims->Width()) / 2);
      objd[1] = new Imx2dsf(ims->Bands(), (ims->Height()) / 2, (ims->Width()) / 2);
      objd[2] = new Imx2dsf(ims->Bands(), (ims->Height()) / 2, (ims->Width()) / 2);
      objd[3] = new Imx2dsf(ims->Bands(), (ims->Height()) / 2, (ims->Width()) / 2);
       
      Imx2dsf* const imd0 = (Imx2dsf*)objd[0];
      Imx2dsf* const imd1 = (Imx2dsf*)objd[1];
      Imx2dsf* const imd2 = (Imx2dsf*)objd[2];
      Imx2dsf* const imd3 = (Imx2dsf*)objd[3];
      
      result = PSplitImage(*ims, *imd0, *imd1, *imd2, *imd3);
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
