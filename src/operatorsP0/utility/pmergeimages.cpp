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
 * @author Régis Clouard - 1995-10-12
 * @author Régis Clouard - 2004-08-04 (add types)
 */

#include <pandore.h>
using namespace pandore;

/**
 * @file pmergeimages.cpp
 * Insertion de 4 sous-images en 1 seule.
 */



Errc PMergeImages( const Img2duc &ims0, const Img2duc &ims1, const Img2duc &ims2, const Img2duc &ims3, Img2duc &imd ) {
   int h=ims0.Height();
   int w=ims0.Width();
   
   for (int y=0; y<h; y++) {
      for (int x=0; x<w; x++) {
	 imd[y][x]=ims0[y][x];
	 imd[y][x+w]=ims1[y][x];
	 imd[y+h][x]=ims2[y][x];
	 imd[y+h][x+w]=ims3[y][x];
      }
   }
   return SUCCESS;
}

Errc PMergeImages( const Img2dsl &ims0, const Img2dsl &ims1, const Img2dsl &ims2, const Img2dsl &ims3, Img2dsl &imd ) {
   int h=ims0.Height();
   int w=ims0.Width();
   
   for (int y=0; y<h; y++) {
      for (int x=0; x<w; x++) {
	 imd[y][x]=ims0[y][x];
	 imd[y][x+w]=ims1[y][x];
	 imd[y+h][x]=ims2[y][x];
	 imd[y+h][x+w]=ims3[y][x];
      }
   }
   return SUCCESS;
}

Errc PMergeImages( const Img2dsf &ims0, const Img2dsf &ims1, const Img2dsf &ims2, const Img2dsf &ims3, Img2dsf &imd ) {
   int h=ims0.Height();
   int w=ims0.Width();
   
   for (int y=0; y<h; y++) {
      for (int x=0; x<w; x++) {
	 imd[y][x]=ims0[y][x];
	 imd[y][x+w]=ims1[y][x];
	 imd[y+h][x]=ims2[y][x];
	 imd[y+h][x+w]=ims3[y][x];
      }
   }
   return SUCCESS;
}

Errc PMergeImages( const Imc2duc &ims0, const Imc2duc &ims1, const Imc2duc &ims2, const Imc2duc &ims3, Imc2duc &imd) {
   int h=ims0.Height();
   int w=ims0.Width();
   
   imd.ColorSpace(ims0.ColorSpace());
   for (int y=0; y<h; y++)
      for (int x=0; x<w; x++) {
	 for (int b=0;b<ims0.Bands();b++) {
	    imd[b][y][x]=ims0[b][y][x];
	    imd[b][y][x+w]=ims1[b][y][x];
	    imd[b][y+h][x]=ims2[b][y][x];
	    imd[b][y+h][x+w]=ims3[b][y][x];
	 }
      }
   
   return SUCCESS;
}

Errc PMergeImages( const Imc2dsl &ims0, const Imc2dsl &ims1, const Imc2dsl &ims2, const Imc2dsl &ims3, Imc2dsl &imd) {
   int h=ims0.Height();
   int w=ims0.Width();
   
   imd.ColorSpace(ims0.ColorSpace());
   for (int y=0; y<h; y++)
      for (int x=0; x<w; x++) {
	 for (int b=0;b<ims0.Bands();b++) {
	    imd[b][y][x]=ims0[b][y][x];
	    imd[b][y][x+w]=ims1[b][y][x];
	    imd[b][y+h][x]=ims2[b][y][x];
	    imd[b][y+h][x+w]=ims3[b][y][x];
	 }
      }
   
   return SUCCESS;
}

Errc PMergeImages( const Imc2dsf &ims0, const Imc2dsf &ims1, const Imc2dsf &ims2, const Imc2dsf &ims3, Imc2dsf &imd) {
   int h=ims0.Height();
   int w=ims0.Width();
   
   imd.ColorSpace(ims0.ColorSpace());
   for (int y=0; y<h; y++)
      for (int x=0; x<w; x++) {
	 for (int b=0;b<ims0.Bands();b++) {
	    imd[b][y][x]=ims0[b][y][x];
	    imd[b][y][x+w]=ims1[b][y][x];
	    imd[b][y+h][x]=ims2[b][y][x];
	    imd[b][y+h][x+w]=ims3[b][y][x];
	 }
      }
   
   return SUCCESS;
}

Errc PMergeImages( const Imx2duc &ims0, const Imx2duc &ims1, const Imx2duc &ims2, const Imx2duc &ims3, Imx2duc &imd) {
   int h=ims0.Height();
   int w=ims0.Width();
   
   for (int y=0; y<h; y++) {
      for (int x=0; x<w; x++) {
	 for (int b=0;b<ims0.Bands();b++) {
	    imd[b][y][x]=ims0[b][y][x];
	    imd[b][y][x+w]=ims1[b][y][x];
	    imd[b][y+h][x]=ims2[b][y][x];
	    imd[b][y+h][x+w]=ims3[b][y][x];
	 }
      }
   }
   return SUCCESS;
}

Errc PMergeImages( const Imx2dsl &ims0, const Imx2dsl &ims1, const Imx2dsl &ims2, const Imx2dsl &ims3, Imx2dsl &imd) {
   int h=ims0.Height();
   int w=ims0.Width();
   
   for (int y=0; y<h; y++) {
      for (int x=0; x<w; x++) {
	 for (int b=0;b<ims0.Bands();b++) {
	    imd[b][y][x]=ims0[b][y][x];
	    imd[b][y][x+w]=ims1[b][y][x];
	    imd[b][y+h][x]=ims2[b][y][x];
	    imd[b][y+h][x+w]=ims3[b][y][x];
	 }
      }
   }
   return SUCCESS;
}

Errc PMergeImages( const Imx2dsf &ims0, const Imx2dsf &ims1, const Imx2dsf &ims2, const Imx2dsf &ims3, Imx2dsf &imd) {
   int h=ims0.Height();
   int w=ims0.Width();
   
   for (int y=0; y<h; y++) {
      for (int x=0; x<w; x++) {
	 for (int b=0;b<ims0.Bands();b++) {
	    imd[b][y][x]=ims0[b][y][x];
	    imd[b][y][x+w]=ims1[b][y][x];
	    imd[b][y+h][x]=ims2[b][y][x];
	    imd[b][y+h][x+w]=ims3[b][y][x];
	 }
      }
   }
   return SUCCESS;
}


#ifdef MAIN
#define	USAGE	"usage: %s [im_in0|-] [im_in1|-] [im_in2|-] [im_in3|-] [im_out|-]"
#define	PARC	0
#define	FINC	4
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
      Img2duc* const ims0=(Img2duc*)objs[0];
      Img2duc* const ims1=(Img2duc*)objs[1];
      Img2duc* const ims2=(Img2duc*)objs[2];
      Img2duc* const ims3=(Img2duc*)objs[3];
      
      if (( ims1->Width()!=ims0->Width()) || (ims0->Height()!=ims1->Height())
	  || ( ims2->Width()!=ims0->Width()) || (ims0->Height()!=ims2->Height())
	  || ( ims3->Width()!=ims0->Width()) || (ims0->Height()!=ims3->Height())) {
	 result = FAILURE;
      } else {
	 objd[0]=new Img2duc(2*ims1->Height(), 2*ims1->Width());
	 Img2duc* const imd=(Img2duc*)objd[0];
	 result=PMergeImages(*ims0, *ims1, *ims2, *ims3, *imd);
      }
      goto end;
   }
   if (objs[0]->Type()==Po_Img2dsl) {
      Img2dsl* const ims0=(Img2dsl*)objs[0];
      Img2dsl* const ims1=(Img2dsl*)objs[1];
      Img2dsl* const ims2=(Img2dsl*)objs[2];
      Img2dsl* const ims3=(Img2dsl*)objs[3];
      
      if (( ims1->Width()!=ims0->Width()) || (ims0->Height()!=ims1->Height())
	  || ( ims2->Width()!=ims0->Width()) || (ims0->Height()!=ims2->Height())
	  || ( ims3->Width()!=ims0->Width()) || (ims0->Height()!=ims3->Height())) {
	 result = FAILURE;
      } else {
	 objd[0]=new Img2dsl(2*ims1->Height(), 2*ims1->Width());
	 Img2dsl* const imd=(Img2dsl*)objd[0];
	 result=PMergeImages(*ims0, *ims1, *ims2, *ims3, *imd);
      }
      goto end;
   }
   if (objs[0]->Type()==Po_Img2dsf) {
      Img2dsf* const ims0=(Img2dsf*)objs[0];
      Img2dsf* const ims1=(Img2dsf*)objs[1];
      Img2dsf* const ims2=(Img2dsf*)objs[2];
      Img2dsf* const ims3=(Img2dsf*)objs[3];
      
      if (( ims1->Width()!=ims0->Width()) || (ims0->Height()!=ims1->Height())
	  || ( ims2->Width()!=ims0->Width()) || (ims0->Height()!=ims2->Height())
	  || ( ims3->Width()!=ims0->Width()) || (ims0->Height()!=ims3->Height())) {
	 result = FAILURE;
      } else {
	 objd[0]=new Img2dsf(2*ims1->Height(), 2*ims1->Width());
	 Img2dsf* const imd=(Img2dsf*)objd[0];
	 result=PMergeImages(*ims0, *ims1, *ims2, *ims3, *imd);
      }
      goto end;
   }
   if (objs[0]->Type()==Po_Imc2duc) {
      Imc2duc* const ims0=(Imc2duc*)objs[0];
      Imc2duc* const ims1=(Imc2duc*)objs[1];
      Imc2duc* const ims2=(Imc2duc*)objs[2];
      Imc2duc* const ims3=(Imc2duc*)objs[3];
      
      if (( ims1->Width()!=ims0->Width()) || (ims0->Height()!=ims1->Height())
	  || ( ims2->Width()!=ims0->Width()) || (ims0->Height()!=ims2->Height())
	  || ( ims3->Width()!=ims0->Width()) || (ims0->Height()!=ims3->Height())) {
	 result = FAILURE;
      } else {
	 objd[0]=new Imc2duc(2*ims1->Height(), 2*ims1->Width());
	 Imc2duc* const imd=(Imc2duc*)objd[0];
	 result=PMergeImages(*ims0, *ims1, *ims2, *ims3, *imd);
      }
      goto end;
   }
   if (objs[0]->Type()==Po_Imc2dsl) {
      Imc2dsl* const ims0=(Imc2dsl*)objs[0];
      Imc2dsl* const ims1=(Imc2dsl*)objs[1];
      Imc2dsl* const ims2=(Imc2dsl*)objs[2];
      Imc2dsl* const ims3=(Imc2dsl*)objs[3];
      
      if (( ims1->Width()!=ims0->Width()) || (ims0->Height()!=ims1->Height())
	  || ( ims2->Width()!=ims0->Width()) || (ims0->Height()!=ims2->Height())
	  || ( ims3->Width()!=ims0->Width()) || (ims0->Height()!=ims3->Height())) {
	 result = FAILURE;
      } else {
	 objd[0]=new Imc2dsl(2*ims1->Height(), 2*ims1->Width());
	 Imc2dsl* const imd=(Imc2dsl*)objd[0];
	 result=PMergeImages(*ims0, *ims1, *ims2, *ims3, *imd);
      }
      goto end;
   }
   if (objs[0]->Type()==Po_Imc2dsf) {
      Imc2dsf* const ims0=(Imc2dsf*)objs[0];
      Imc2dsf* const ims1=(Imc2dsf*)objs[1];
      Imc2dsf* const ims2=(Imc2dsf*)objs[2];
      Imc2dsf* const ims3=(Imc2dsf*)objs[3];
      
      if (( ims1->Width()!=ims0->Width()) || (ims0->Height()!=ims1->Height())
	  || ( ims2->Width()!=ims0->Width()) || (ims0->Height()!=ims2->Height())
	  || ( ims3->Width()!=ims0->Width()) || (ims0->Height()!=ims3->Height())) {
	 result = FAILURE;
      } else {
	 objd[0]=new Imc2dsf(2*ims1->Height(), 2*ims1->Width());
	 Imc2dsf* const imd=(Imc2dsf*)objd[0];
	 result=PMergeImages(*ims0, *ims1, *ims2, *ims3, *imd);
      }
      goto end;
   }
   if (objs[0]->Type()==Po_Imx2duc) {
      Imx2duc* const ims0=(Imx2duc*)objs[0];
      Imx2duc* const ims1=(Imx2duc*)objs[1];
      Imx2duc* const ims2=(Imx2duc*)objs[2];
      Imx2duc* const ims3=(Imx2duc*)objs[3];
      
      if (( ims1->Width()!=ims0->Width()) || (ims0->Height()!=ims1->Height())
	  || ( ims2->Width()!=ims0->Width()) || (ims0->Height()!=ims2->Height())
	  || ( ims3->Width()!=ims0->Width()) || (ims0->Height()!=ims3->Height())) {
	 result = FAILURE;
      } else {
	 objd[0]=new Imx2duc(ims0->Bands(),2*ims1->Height(), 2*ims1->Width());
	 Imx2duc* const imd=(Imx2duc*)objd[0];
	 result=PMergeImages(*ims0, *ims1, *ims2, *ims3, *imd);
      }
      goto end;
   }
   if (objs[0]->Type()==Po_Imx2dsl) {
      Imx2dsl* const ims0=(Imx2dsl*)objs[0];
      Imx2dsl* const ims1=(Imx2dsl*)objs[1];
      Imx2dsl* const ims2=(Imx2dsl*)objs[2];
      Imx2dsl* const ims3=(Imx2dsl*)objs[3];
      
      if (( ims1->Width()!=ims0->Width()) || (ims0->Height()!=ims1->Height())
	  || ( ims2->Width()!=ims0->Width()) || (ims0->Height()!=ims2->Height())
	  || ( ims3->Width()!=ims0->Width()) || (ims0->Height()!=ims3->Height())) {
	 result = FAILURE;
      } else {
	 objd[0]=new Imx2dsl(ims0->Bands(),2*ims1->Height(), 2*ims1->Width());
	 Imx2dsl* const imd=(Imx2dsl*)objd[0];
	 result=PMergeImages(*ims0, *ims1, *ims2, *ims3, *imd);
      }
      goto end;
   }
   if (objs[0]->Type()==Po_Imx2dsf) {
      Imx2dsf* const ims0=(Imx2dsf*)objs[0];
      Imx2dsf* const ims1=(Imx2dsf*)objs[1];
      Imx2dsf* const ims2=(Imx2dsf*)objs[2];
      Imx2dsf* const ims3=(Imx2dsf*)objs[3];
      
      if (( ims1->Width()!=ims0->Width()) || (ims0->Height()!=ims1->Height())
	  || ( ims2->Width()!=ims0->Width()) || (ims0->Height()!=ims2->Height())
	  || ( ims3->Width()!=ims0->Width()) || (ims0->Height()!=ims3->Height())) {
	 result = FAILURE;
      } else {
	 objd[0]=new Imx2dsf(ims0->Bands(),2*ims1->Height(), 2*ims1->Width());
	 Imx2dsf* const imd=(Imx2dsf*)objd[0];
	 result=PMergeImages(*ims0, *ims1, *ims2, *ims3, *imd);
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
