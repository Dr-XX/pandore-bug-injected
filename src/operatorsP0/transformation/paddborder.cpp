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
 * @author Regis Clouard - 2003-01-08
 * @author Regis Clouard - cd (add shrink border)
 */

#define _USE_MATH_DEFINES
#include <cmath>
#include <pandore.h>
using namespace pandore;

/**
 * @file paddborder.cpp
 * Resize image.
 */
template <typename T>
Errc PAddBorder( const Imx3d<T> &ims, Imx3d<T> &imd, int ll, int , int hu, int , int df, int ) {
   Point3d p, q;

   for ( int b = 0; b < imd.Bands(); b++) {
      for ( p.z = 0; p.z < imd.Depth(); p.z++) {
	 for ( p.y = 0; p.y < imd.Height(); p.y++) {
	    for ( p.x = 0; p.x < imd.Width(); p.x++) {
	       q.x = p.x - ll;
	       q.y = p.y - hu;
	       q.z = p.z - df;
	       if (ims.Hold(q)) {
		  imd[b][p] = ims[b][q];
	       } else {
		  imd[b][p] = 0;
	       }
	    }
	 }
      }
   }
   return SUCCESS;
}




#ifdef MAIN
#define	USAGE	"usage: %s widthl widthr heightu heightd depthf depthb [-m mask] [im_in|-] [im_out|-]"
#define	PARC	6
#define	FINC	1
#define	FOUTC	1
#define	MASK	2

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
      const int ll = atoi(parv[0]);
      const int lr = atoi(parv[1]);
 
      const int hu = atoi(parv[2]);
      const int hb = atoi(parv[3]);

      PobjectProps props = ims->Props();
      props.ncol = ims->Width() + ll + lr;
      props.nrow = ims->Height() + hu + hb;

      objd[0] = new Img2duc(props);
      Img2duc* const imd = (Img2duc*)objd[0];
      
      result = PAddBorder(*ims, *imd, ll, lr, hu, hb, 0, 0);
      goto end;
   }
   if (objs[0]->Type() == Po_Img2dsl) {
      Img2dsl* const ims = (Img2dsl*)objs[0];
      const int ll = atoi(parv[0]);
      const int lr = atoi(parv[1]);
 
      const int hu = atoi(parv[2]);
      const int hb = atoi(parv[3]);

      PobjectProps props = ims->Props();
      props.ncol = ims->Width() + ll + lr;
      props.nrow = ims->Height() + hu + hb;

      objd[0] = new Img2dsl(props);
      Img2dsl* const imd = (Img2dsl*)objd[0];
      
      result = PAddBorder(*ims, *imd, ll, lr, hu, hb, 0, 0);
      goto end;
   }
   if (objs[0]->Type() == Po_Img2dsf) {
      Img2dsf* const ims = (Img2dsf*)objs[0];
      const int ll = atoi(parv[0]);
      const int lr = atoi(parv[1]);
 
      const int hu = atoi(parv[2]);
      const int hb = atoi(parv[3]);

      PobjectProps props = ims->Props();
      props.ncol = ims->Width() + ll + lr;
      props.nrow = ims->Height() + hu + hb;

      objd[0] = new Img2dsf(props);
      Img2dsf* const imd = (Img2dsf*)objd[0];
      
      result = PAddBorder(*ims, *imd, ll, lr, hu, hb, 0, 0);
      goto end;
   }
   if (objs[0]->Type() == Po_Img3duc) {
      Img3duc* const ims = (Img3duc*)objs[0];
      const int ll = atoi(parv[0]);
      const int lr = atoi(parv[1]);
 
      const int hu = atoi(parv[2]);
      const int hb = atoi(parv[3]);

      const int df = atoi(parv[4]);
      const int de = atoi(parv[5]);

      PobjectProps props = ims->Props();
      props.ndep = ims->Depth() + df + de;
      props.nrow = ims->Height() + hu + hb;
      props.ncol = ims->Width() + ll + lr;

      objd[0] = new Img3duc(props);
      Img3duc* const imd = (Img3duc*)objd[0];

      result = PAddBorder(*ims, *imd, ll, lr,  hu, hb, df, de);
      goto end;
   }
   if (objs[0]->Type() == Po_Img3dsl) {
      Img3dsl* const ims = (Img3dsl*)objs[0];
      const int ll = atoi(parv[0]);
      const int lr = atoi(parv[1]);
 
      const int hu = atoi(parv[2]);
      const int hb = atoi(parv[3]);

      const int df = atoi(parv[4]);
      const int de = atoi(parv[5]);

      PobjectProps props = ims->Props();
      props.ndep = ims->Depth() + df + de;
      props.nrow = ims->Height() + hu + hb;
      props.ncol = ims->Width() + ll + lr;

      objd[0] = new Img3dsl(props);
      Img3dsl* const imd = (Img3dsl*)objd[0];

      result = PAddBorder(*ims, *imd, ll, lr,  hu, hb, df, de);
      goto end;
   }
   if (objs[0]->Type() == Po_Img3dsf) {
      Img3dsf* const ims = (Img3dsf*)objs[0];
      const int ll = atoi(parv[0]);
      const int lr = atoi(parv[1]);
 
      const int hu = atoi(parv[2]);
      const int hb = atoi(parv[3]);

      const int df = atoi(parv[4]);
      const int de = atoi(parv[5]);

      PobjectProps props = ims->Props();
      props.ndep = ims->Depth() + df + de;
      props.nrow = ims->Height() + hu + hb;
      props.ncol = ims->Width() + ll + lr;

      objd[0] = new Img3dsf(props);
      Img3dsf* const imd = (Img3dsf*)objd[0];

      result = PAddBorder(*ims, *imd, ll, lr,  hu, hb, df, de);
      goto end;
   }
   if (objs[0]->Type() == Po_Imc2duc) {
      Imc2duc* const ims = (Imc2duc*)objs[0];
      const int ll = atoi(parv[0]);
      const int lr = atoi(parv[1]);
 
      const int hu = atoi(parv[2]);
      const int hb = atoi(parv[3]);

      PobjectProps props = ims->Props();
      props.ncol = ims->Width() + ll + lr;
      props.nrow = ims->Height() + hu + hb;

      objd[0] = new Imc2duc(props);
      Imc2duc* const imd = (Imc2duc*)objd[0];
      
      result = PAddBorder(*ims, *imd, ll, lr, hu, hb, 0, 0);
      goto end;
   }
   if (objs[0]->Type() == Po_Imc2dsl) {
      Imc2dsl* const ims = (Imc2dsl*)objs[0];
      const int ll = atoi(parv[0]);
      const int lr = atoi(parv[1]);
 
      const int hu = atoi(parv[2]);
      const int hb = atoi(parv[3]);

      PobjectProps props = ims->Props();
      props.ncol = ims->Width() + ll + lr;
      props.nrow = ims->Height() + hu + hb;

      objd[0] = new Imc2dsl(props);
      Imc2dsl* const imd = (Imc2dsl*)objd[0];
      
      result = PAddBorder(*ims, *imd, ll, lr, hu, hb, 0, 0);
      goto end;
   }
   if (objs[0]->Type() == Po_Imc2dsf) {
      Imc2dsf* const ims = (Imc2dsf*)objs[0];
      const int ll = atoi(parv[0]);
      const int lr = atoi(parv[1]);
 
      const int hu = atoi(parv[2]);
      const int hb = atoi(parv[3]);

      PobjectProps props = ims->Props();
      props.ncol = ims->Width() + ll + lr;
      props.nrow = ims->Height() + hu + hb;

      objd[0] = new Imc2dsf(props);
      Imc2dsf* const imd = (Imc2dsf*)objd[0];
      
      result = PAddBorder(*ims, *imd, ll, lr, hu, hb, 0, 0);
      goto end;
   }
   if (objs[0]->Type() == Po_Imc3duc) {
      Imc3duc* const ims = (Imc3duc*)objs[0];
      const int ll = atoi(parv[0]);
      const int lr = atoi(parv[1]);
 
      const int hu = atoi(parv[2]);
      const int hb = atoi(parv[3]);

      const int df = atoi(parv[4]);
      const int de = atoi(parv[5]);

      PobjectProps props = ims->Props();
      props.ndep = ims->Depth() + df + de;
      props.nrow = ims->Height() + hu + hb;
      props.ncol = ims->Width() + ll + lr;

      objd[0] = new Imc3duc(props);
      Imc3duc* const imd = (Imc3duc*)objd[0];

      result = PAddBorder(*ims, *imd, ll, lr,  hu, hb, df, de);
      goto end;
   }
   if (objs[0]->Type() == Po_Imc3dsl) {
      Imc3dsl* const ims = (Imc3dsl*)objs[0];
      const int ll = atoi(parv[0]);
      const int lr = atoi(parv[1]);
 
      const int hu = atoi(parv[2]);
      const int hb = atoi(parv[3]);

      const int df = atoi(parv[4]);
      const int de = atoi(parv[5]);

      PobjectProps props = ims->Props();
      props.ndep = ims->Depth() + df + de;
      props.nrow = ims->Height() + hu + hb;
      props.ncol = ims->Width() + ll + lr;

      objd[0] = new Imc3dsl(props);
      Imc3dsl* const imd = (Imc3dsl*)objd[0];

      result = PAddBorder(*ims, *imd, ll, lr,  hu, hb, df, de);
      goto end;
   }
   if (objs[0]->Type() == Po_Imc3dsf) {
      Imc3dsf* const ims = (Imc3dsf*)objs[0];
      const int ll = atoi(parv[0]);
      const int lr = atoi(parv[1]);
 
      const int hu = atoi(parv[2]);
      const int hb = atoi(parv[3]);

      const int df = atoi(parv[4]);
      const int de = atoi(parv[5]);

      PobjectProps props = ims->Props();
      props.ndep = ims->Depth() + df + de;
      props.nrow = ims->Height() + hu + hb;
      props.ncol = ims->Width() + ll + lr;

      objd[0] = new Imc3dsf(props);
      Imc3dsf* const imd = (Imc3dsf*)objd[0];

      result = PAddBorder(*ims, *imd, ll, lr,  hu, hb, df, de);
      goto end;
   }
   if (objs[0]->Type() == Po_Imx2duc) {
      Imx2duc* const ims = (Imx2duc*)objs[0];
      const int ll = atoi(parv[0]);
      const int lr = atoi(parv[1]);
 
      const int hu = atoi(parv[2]);
      const int hb = atoi(parv[3]);

      PobjectProps props = ims->Props();
      props.ncol = ims->Width() + ll + lr;
      props.nrow = ims->Height() + hu + hb;

      objd[0] = new Imx2duc(props);
      Imx2duc* const imd = (Imx2duc*)objd[0];
      
      result = PAddBorder(*ims, *imd, ll, lr, hu, hb, 0, 0);
      goto end;
   }
   if (objs[0]->Type() == Po_Imx2dsl) {
      Imx2dsl* const ims = (Imx2dsl*)objs[0];
      const int ll = atoi(parv[0]);
      const int lr = atoi(parv[1]);
 
      const int hu = atoi(parv[2]);
      const int hb = atoi(parv[3]);

      PobjectProps props = ims->Props();
      props.ncol = ims->Width() + ll + lr;
      props.nrow = ims->Height() + hu + hb;

      objd[0] = new Imx2dsl(props);
      Imx2dsl* const imd = (Imx2dsl*)objd[0];
      
      result = PAddBorder(*ims, *imd, ll, lr, hu, hb, 0, 0);
      goto end;
   }
   if (objs[0]->Type() == Po_Imx2dsf) {
      Imx2dsf* const ims = (Imx2dsf*)objs[0];
      const int ll = atoi(parv[0]);
      const int lr = atoi(parv[1]);
 
      const int hu = atoi(parv[2]);
      const int hb = atoi(parv[3]);

      PobjectProps props = ims->Props();
      props.ncol = ims->Width() + ll + lr;
      props.nrow = ims->Height() + hu + hb;

      objd[0] = new Imx2dsf(props);
      Imx2dsf* const imd = (Imx2dsf*)objd[0];
      
      result = PAddBorder(*ims, *imd, ll, lr, hu, hb, 0, 0);
      goto end;
   }
   if (objs[0]->Type() == Po_Imx3duc) {
      Imx3duc* const ims = (Imx3duc*)objs[0];
      const int ll = atoi(parv[0]);
      const int lr = atoi(parv[1]);
 
      const int hu = atoi(parv[2]);
      const int hb = atoi(parv[3]);

      const int df = atoi(parv[4]);
      const int de = atoi(parv[5]);

      PobjectProps props = ims->Props();
      props.ndep = ims->Depth() + df + de;
      props.nrow = ims->Height() + hu + hb;
      props.ncol = ims->Width() + ll + lr;

      objd[0] = new Imx3duc(props);
      Imx3duc* const imd = (Imx3duc*)objd[0];

      result = PAddBorder(*ims, *imd, ll, lr,  hu, hb, df, de);
      goto end;
   }
   if (objs[0]->Type() == Po_Imx3dsl) {
      Imx3dsl* const ims = (Imx3dsl*)objs[0];
      const int ll = atoi(parv[0]);
      const int lr = atoi(parv[1]);
 
      const int hu = atoi(parv[2]);
      const int hb = atoi(parv[3]);

      const int df = atoi(parv[4]);
      const int de = atoi(parv[5]);

      PobjectProps props = ims->Props();
      props.ndep = ims->Depth() + df + de;
      props.nrow = ims->Height() + hu + hb;
      props.ncol = ims->Width() + ll + lr;

      objd[0] = new Imx3dsl(props);
      Imx3dsl* const imd = (Imx3dsl*)objd[0];

      result = PAddBorder(*ims, *imd, ll, lr,  hu, hb, df, de);
      goto end;
   }
   if (objs[0]->Type() == Po_Imx3dsf) {
      Imx3dsf* const ims = (Imx3dsf*)objs[0];
      const int ll = atoi(parv[0]);
      const int lr = atoi(parv[1]);
 
      const int hu = atoi(parv[2]);
      const int hb = atoi(parv[3]);

      const int df = atoi(parv[4]);
      const int de = atoi(parv[5]);

      PobjectProps props = ims->Props();
      props.ndep = ims->Depth() + df + de;
      props.nrow = ims->Height() + hu + hb;
      props.ncol = ims->Width() + ll + lr;

      objd[0] = new Imx3dsf(props);
      Imx3dsf* const imd = (Imx3dsf*)objd[0];

      result = PAddBorder(*ims, *imd, ll, lr,  hu, hb, df, de);
      goto end;
   }
   if (objs[0]->Type() == Po_Reg2d) {
      Reg2d* const ims = (Reg2d*)objs[0];
      const int ll = atoi(parv[0]);
      const int lr = atoi(parv[1]);
 
      const int hu = atoi(parv[2]);
      const int hb = atoi(parv[3]);

      PobjectProps props = ims->Props();
      props.ncol = ims->Width() + ll + lr;
      props.nrow = ims->Height() + hu + hb;

      objd[0] = new Reg2d(props);
      Reg2d* const imd = (Reg2d*)objd[0];
      
      result = PAddBorder(*ims, *imd, ll, lr, hu, hb, 0, 0);
      goto end;
   }
   if (objs[0]->Type() == Po_Reg3d) {
      Reg3d* const ims = (Reg3d*)objs[0];
      const int ll = atoi(parv[0]);
      const int lr = atoi(parv[1]);
 
      const int hu = atoi(parv[2]);
      const int hb = atoi(parv[3]);

      const int df = atoi(parv[4]);
      const int de = atoi(parv[5]);

      PobjectProps props = ims->Props();
      props.ndep = ims->Depth() + df + de;
      props.nrow = ims->Height() + hu + hb;
      props.ncol = ims->Width() + ll + lr;

      objd[0] = new Reg3d(props);
      Reg3d* const imd = (Reg3d*)objd[0];

      result = PAddBorder(*ims, *imd, ll, lr,  hu, hb, df, de);
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
