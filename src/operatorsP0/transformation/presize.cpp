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
 * @author Regis Clouard - 1997-02-22
 */

#include <math.h>
#include <pandore.h>
using namespace pandore;

/**
 * @file presize.cpp
 * Resize image.
 */









Errc PResize( const Img2duc &ims, Img2duc &imd ) {
   double tx, ty;
   Point2d p;
   
   tx = (double)ims.Width() / imd.Width();
   ty = (double)ims.Height() / imd.Height();
   
   for (p.y = 0; p.y < imd.Height(); p.y++)
   for (p.x = 0; p.x < imd.Width(); p.x++)
   {
      imd[p] = ims[int(p.y * ty)][int(p.x * tx)];
   }

   return SUCCESS;
}

Errc PResize( const Img2dsl &ims, Img2dsl &imd ) {
   double tx, ty;
   Point2d p;
   
   tx = (double)ims.Width() / imd.Width();
   ty = (double)ims.Height() / imd.Height();
   
   for (p.y = 0; p.y < imd.Height(); p.y++)
   for (p.x = 0; p.x < imd.Width(); p.x++)
   {
      imd[p] = ims[int(p.y * ty)][int(p.x * tx)];
   }

   return SUCCESS;
}

Errc PResize( const Img2dsf &ims, Img2dsf &imd ) {
   double tx, ty;
   Point2d p;
   
   tx = (double)ims.Width() / imd.Width();
   ty = (double)ims.Height() / imd.Height();
   
   for (p.y = 0; p.y < imd.Height(); p.y++)
   for (p.x = 0; p.x < imd.Width(); p.x++)
   {
      imd[p] = ims[int(p.y * ty)][int(p.x * tx)];
   }

   return SUCCESS;
}

Errc PResize( const Img3duc &ims, Img3duc &imd ) {
   double tx, ty, tz;
   Point3d p;
   
   tx = (double)ims.Width() / imd.Width();
   ty = (double)ims.Height() / imd.Height();
   tz = (double)ims.Depth() / imd.Depth();
   
   for (p.z = 0; p.z < imd.Depth(); p.z++)
   for (p.y = 0; p.y < imd.Height(); p.y++)
   for (p.x = 0; p.x < imd.Width(); p.x++)
   {
      imd[p] = ims[int(p.z * tz)][int(p.y * ty)][int(p.x * tx)];
   }

   return SUCCESS;
}

Errc PResize( const Img3dsl &ims, Img3dsl &imd ) {
   double tx, ty, tz;
   Point3d p;
   
   tx = (double)ims.Width() / imd.Width();
   ty = (double)ims.Height() / imd.Height();
   tz = (double)ims.Depth() / imd.Depth();
   
   for (p.z = 0; p.z < imd.Depth(); p.z++)
   for (p.y = 0; p.y < imd.Height(); p.y++)
   for (p.x = 0; p.x < imd.Width(); p.x++)
   {
      imd[p] = ims[int(p.z * tz)][int(p.y * ty)][int(p.x * tx)];
   }

   return SUCCESS;
}

Errc PResize( const Img3dsf &ims, Img3dsf &imd ) {
   double tx, ty, tz;
   Point3d p;
   
   tx = (double)ims.Width() / imd.Width();
   ty = (double)ims.Height() / imd.Height();
   tz = (double)ims.Depth() / imd.Depth();
   
   for (p.z = 0; p.z < imd.Depth(); p.z++)
   for (p.y = 0; p.y < imd.Height(); p.y++)
   for (p.x = 0; p.x < imd.Width(); p.x++)
   {
      imd[p] = ims[int(p.z * tz)][int(p.y * ty)][int(p.x * tx)];
   }

   return SUCCESS;
}

Errc PResize( const Imc2duc &ims, Imc2duc &imd ) {
   double tx, ty;
   Point2d p;
   
   imd.ColorSpace(ims.ColorSpace());
 
   tx = (double)ims.Width() / imd.Width();
   ty = (double)ims.Height() / imd.Height();
   
   for (p.y = 0; p.y < imd.Height(); p.y++)
   for (p.x = 0; p.x < imd.Width(); p.x++)
   {
      imd.X[p] = ims.X[int(p.y * ty)][int(p.x * tx)];
      imd.Y[p] = ims.Y[int(p.y * ty)][int(p.x * tx)];
      imd.Z[p] = ims.Z[int(p.y * ty)][int(p.x * tx)];
   }

   return SUCCESS;
}

Errc PResize( const Imc2dsl &ims, Imc2dsl &imd ) {
   double tx, ty;
   Point2d p;
   
   imd.ColorSpace(ims.ColorSpace());
 
   tx = (double)ims.Width() / imd.Width();
   ty = (double)ims.Height() / imd.Height();
   
   for (p.y = 0; p.y < imd.Height(); p.y++)
   for (p.x = 0; p.x < imd.Width(); p.x++)
   {
      imd.X[p] = ims.X[int(p.y * ty)][int(p.x * tx)];
      imd.Y[p] = ims.Y[int(p.y * ty)][int(p.x * tx)];
      imd.Z[p] = ims.Z[int(p.y * ty)][int(p.x * tx)];
   }

   return SUCCESS;
}

Errc PResize( const Imc2dsf &ims, Imc2dsf &imd ) {
   double tx, ty;
   Point2d p;
   
   imd.ColorSpace(ims.ColorSpace());
 
   tx = (double)ims.Width() / imd.Width();
   ty = (double)ims.Height() / imd.Height();
   
   for (p.y = 0; p.y < imd.Height(); p.y++)
   for (p.x = 0; p.x < imd.Width(); p.x++)
   {
      imd.X[p] = ims.X[int(p.y * ty)][int(p.x * tx)];
      imd.Y[p] = ims.Y[int(p.y * ty)][int(p.x * tx)];
      imd.Z[p] = ims.Z[int(p.y * ty)][int(p.x * tx)];
   }

   return SUCCESS;
}

Errc PResize( const Imc3duc &ims, Imc3duc &imd ) {
   double tx, ty, tz;
   Point3d p;

   imd.ColorSpace(ims.ColorSpace());
   
   tx = (double)ims.Width() / imd.Width();
   ty = (double)ims.Height() / imd.Height();
   tz = (double)ims.Depth() / imd.Depth();
   
   for (p.z = 0; p.z < imd.Depth(); p.z++)
   for (p.y = 0; p.y < imd.Height(); p.y++)
   for (p.x = 0; p.x < imd.Width(); p.x++)
   {
      imd.X[p] = ims.X[int(p.z * tz)][int(p.y * ty)][int(p.x * tx)];
      imd.Y[p] = ims.Y[int(p.z * tz)][int(p.y * ty)][int(p.x * tx)];
      imd.Z[p] = ims.Z[int(p.z * tz)][int(p.y * ty)][int(p.x * tx)];
   }

   return SUCCESS;
}

Errc PResize( const Imc3dsl &ims, Imc3dsl &imd ) {
   double tx, ty, tz;
   Point3d p;

   imd.ColorSpace(ims.ColorSpace());
   
   tx = (double)ims.Width() / imd.Width();
   ty = (double)ims.Height() / imd.Height();
   tz = (double)ims.Depth() / imd.Depth();
   
   for (p.z = 0; p.z < imd.Depth(); p.z++)
   for (p.y = 0; p.y < imd.Height(); p.y++)
   for (p.x = 0; p.x < imd.Width(); p.x++)
   {
      imd.X[p] = ims.X[int(p.z * tz)][int(p.y * ty)][int(p.x * tx)];
      imd.Y[p] = ims.Y[int(p.z * tz)][int(p.y * ty)][int(p.x * tx)];
      imd.Z[p] = ims.Z[int(p.z * tz)][int(p.y * ty)][int(p.x * tx)];
   }

   return SUCCESS;
}

Errc PResize( const Imc3dsf &ims, Imc3dsf &imd ) {
   double tx, ty, tz;
   Point3d p;

   imd.ColorSpace(ims.ColorSpace());
   
   tx = (double)ims.Width() / imd.Width();
   ty = (double)ims.Height() / imd.Height();
   tz = (double)ims.Depth() / imd.Depth();
   
   for (p.z = 0; p.z < imd.Depth(); p.z++)
   for (p.y = 0; p.y < imd.Height(); p.y++)
   for (p.x = 0; p.x < imd.Width(); p.x++)
   {
      imd.X[p] = ims.X[int(p.z * tz)][int(p.y * ty)][int(p.x * tx)];
      imd.Y[p] = ims.Y[int(p.z * tz)][int(p.y * ty)][int(p.x * tx)];
      imd.Z[p] = ims.Z[int(p.z * tz)][int(p.y * ty)][int(p.x * tx)];
   }

   return SUCCESS;
}

Errc PResize( const Imx2duc &ims, Imx2duc &imd ) {
   double tx, ty;
   Point2d p;

   tx = (double)ims.Width() / imd.Width();
   ty = (double)ims.Height() / imd.Height();
   
   for (p.y = 0; p.y < imd.Height(); p.y++)
   for (p.x = 0; p.x < imd.Width(); p.x++)
   {
      for (int b = 0; b < ims.Bands(); b++) {
	 imd[b][p] = ims[b][int(p.y * ty)][int(p.x * tx)];
      }
   }
   
   return SUCCESS;
}

Errc PResize( const Imx2dsl &ims, Imx2dsl &imd ) {
   double tx, ty;
   Point2d p;

   tx = (double)ims.Width() / imd.Width();
   ty = (double)ims.Height() / imd.Height();
   
   for (p.y = 0; p.y < imd.Height(); p.y++)
   for (p.x = 0; p.x < imd.Width(); p.x++)
   {
      for (int b = 0; b < ims.Bands(); b++) {
	 imd[b][p] = ims[b][int(p.y * ty)][int(p.x * tx)];
      }
   }
   
   return SUCCESS;
}

Errc PResize( const Imx2dsf &ims, Imx2dsf &imd ) {
   double tx, ty;
   Point2d p;

   tx = (double)ims.Width() / imd.Width();
   ty = (double)ims.Height() / imd.Height();
   
   for (p.y = 0; p.y < imd.Height(); p.y++)
   for (p.x = 0; p.x < imd.Width(); p.x++)
   {
      for (int b = 0; b < ims.Bands(); b++) {
	 imd[b][p] = ims[b][int(p.y * ty)][int(p.x * tx)];
      }
   }
   
   return SUCCESS;
}

Errc PResize( const Imx3duc &ims, Imx3duc &imd ) {
   double tx, ty, tz;
   Point3d p;

   tx = (double)ims.Width() / imd.Width();
   ty = (double)ims.Height() / imd.Height();
   tz = (double)ims.Depth() / imd.Depth();
   
   for (p.z = 0; p.z < imd.Depth(); p.z++)
   for (p.y = 0; p.y < imd.Height(); p.y++)
   for (p.x = 0; p.x < imd.Width(); p.x++)
   {
      for (int b = 0; b < ims.Bands(); b++) {
	 imd[b][p] = ims[b][int(p.z * tz)][int(p.y * ty)][int(p.x * tx)];
      }
   }

   return SUCCESS;
}

Errc PResize( const Imx3dsl &ims, Imx3dsl &imd ) {
   double tx, ty, tz;
   Point3d p;

   tx = (double)ims.Width() / imd.Width();
   ty = (double)ims.Height() / imd.Height();
   tz = (double)ims.Depth() / imd.Depth();
   
   for (p.z = 0; p.z < imd.Depth(); p.z++)
   for (p.y = 0; p.y < imd.Height(); p.y++)
   for (p.x = 0; p.x < imd.Width(); p.x++)
   {
      for (int b = 0; b < ims.Bands(); b++) {
	 imd[b][p] = ims[b][int(p.z * tz)][int(p.y * ty)][int(p.x * tx)];
      }
   }

   return SUCCESS;
}

Errc PResize( const Imx3dsf &ims, Imx3dsf &imd ) {
   double tx, ty, tz;
   Point3d p;

   tx = (double)ims.Width() / imd.Width();
   ty = (double)ims.Height() / imd.Height();
   tz = (double)ims.Depth() / imd.Depth();
   
   for (p.z = 0; p.z < imd.Depth(); p.z++)
   for (p.y = 0; p.y < imd.Height(); p.y++)
   for (p.x = 0; p.x < imd.Width(); p.x++)
   {
      for (int b = 0; b < ims.Bands(); b++) {
	 imd[b][p] = ims[b][int(p.z * tz)][int(p.y * ty)][int(p.x * tx)];
      }
   }

   return SUCCESS;
}

Errc PResize( const Reg2d &rgs, Reg2d &rgd ) {
   double tx, ty;
   Point2d p;
   
   tx = (double)rgs.Width() / rgd.Width();
   ty = (double)rgs.Height() / rgd.Height();
   
   for (p.y = 0; p.y < rgd.Height(); p.y++)
   for (p.x = 0; p.x < rgd.Width(); p.x++)
   {
      rgd[p] = rgs[int(p.y * ty)][int(p.x * tx)];
   }

   rgd.Labels(rgs.Labels());
   return SUCCESS;
}

Errc PResize( const Reg3d &rgs, Reg3d &rgd ) {
   double tx, ty, tz;
   Point3d p;
   
   tx = (double)rgs.Width() / rgd.Width();
   ty = (double)rgs.Height() / rgd.Height();
   tz = (double)rgs.Depth() / rgd.Depth();
   
   for (p.z = 0; p.z < rgd.Depth(); p.z++)
   for (p.y = 0; p.y < rgd.Height(); p.y++)
   for (p.x = 0; p.x < rgd.Width(); p.x++)
   {
      rgd[p] = rgs[int(p.z * tz)][int(p.y * ty)][int(p.x * tx)];
   }

   rgd.Labels(rgs.Labels());
   return SUCCESS;
}

Errc PResize( const Graph2d &grs, Graph2d &grd ) {
   double tx, ty;
   GEdge *l;

   tx = (double)grs.Width() / grd.Width();
   ty = (double)grs.Height() / grd.Height();
   
   for (int i = 0; i < grs.Size(); i++) {
      if (grs[i] != NULL) {
	 grd.Add(i, grs[i]->Item(), (int)(grs[i]->seed.y / ty), (int)(grs[i]->seed.x / tx));
	 grd[i]->value=grs[i]->value;
	 for (l = grs[i]->Neighbours(); l != NULL; l = l->Next()) {
	    if (i>l->Node()) {
	       grd.Link(i, l->Node(), l->weight);
	    }
	 }
      }
   }
   
   return SUCCESS;
}

Errc PResize( const Graph3d &grs, Graph3d &grd ) {
   double tx, ty, tz;
   GEdge *l;
   
   tx = (double)grs.Width() / grd.Width();
   ty = (double)grs.Height() / grd.Height();
   tz = (double)grs.Depth() / grd.Depth();
   
   for (int i = 0; i < grs.Size(); i++) {
      if (grs[i] != NULL) {
	 grd.Add(i,grs[i]->Item(), (int)(grs[i]->seed.z / tz), (int)(grs[i]->seed.y / ty), (int)(grs[i]->seed.x / tx));
	 grd[i]->value = grs[i]->value;
	 for (l = grs[i]->Neighbours(); l != NULL; l = l->Next()) {
	    if (i>l->Node()) {
	       grd.Link(i, l->Node(), l->weight);
	    }
	 }
      }
   }
      
   return SUCCESS;
}


#ifdef MAIN
#define	USAGE	"usage: %s width height depth [-m mask] [im_in|-] [im_out|-]"
#define	PARC	3
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
      if ((atol(parv[1]) <= 0) ||
	  (atol(parv[0]) <= 0) ) {
	 std::cerr << "Error presize: Bad parameter values: " << parv[0] << " " << parv[1] << std::endl;
	 result = FAILURE;
	 goto end;
      }

      Img2duc* const ims = (Img2duc*)objs[0];
      objd[0] = new Img2duc((Long)atol(parv[1]), (Long)atol(parv[0]));
      Img2duc* const imd = (Img2duc*)objd[0];
      result = PResize(*ims, *imd);
      goto end;
   }

   if (objs[0]->Type() == Po_Img2dsl) {
      if ((atol(parv[1]) <= 0) ||
	  (atol(parv[0]) <= 0) ) {
	 std::cerr << "Error presize: Bad parameter values: " << parv[0] << " " << parv[1] << std::endl;
	 result = FAILURE;
	 goto end;
      }

      Img2dsl* const ims = (Img2dsl*)objs[0];
      objd[0] = new Img2dsl((Long)atol(parv[1]), (Long)atol(parv[0]));
      Img2dsl* const imd = (Img2dsl*)objd[0];
      result = PResize(*ims, *imd);
      goto end;
   }

   if (objs[0]->Type() == Po_Img2dsf) {
      if ((atol(parv[1]) <= 0) ||
	  (atol(parv[0]) <= 0) ) {
	 std::cerr << "Error presize: Bad parameter values: " << parv[0] << " " << parv[1] << std::endl;
	 result = FAILURE;
	 goto end;
      }

      Img2dsf* const ims = (Img2dsf*)objs[0];
      objd[0] = new Img2dsf((Long)atol(parv[1]), (Long)atol(parv[0]));
      Img2dsf* const imd = (Img2dsf*)objd[0];
      result = PResize(*ims, *imd);
      goto end;
   }

   if (objs[0]->Type() == Po_Img3duc) {
      if ((atol(parv[2]) <= 0) ||
	  (atol(parv[1]) <= 0) ||
	  (atol(parv[0]) <= 0) ) {
	 std::cerr << "Error presize: Bad parameter values: " << parv[0] << " " << parv[1] << " "<< parv[2] << std::endl;
	 result = FAILURE;
	 goto end;
      }

      Img3duc* const ims = (Img3duc*)objs[0];
      objd[0] = new Img3duc((Long)atol(parv[2]), (Long)atol(parv[1]), (Long)atol(parv[0]));
      Img3duc* const imd = (Img3duc*)objd[0];
      result = PResize(*ims, *imd);
      goto end;
   }

   if (objs[0]->Type() == Po_Img3dsl) {
      if ((atol(parv[2]) <= 0) ||
	  (atol(parv[1]) <= 0) ||
	  (atol(parv[0]) <= 0) ) {
	 std::cerr << "Error presize: Bad parameter values: " << parv[0] << " " << parv[1] << " "<< parv[2] << std::endl;
	 result = FAILURE;
	 goto end;
      }

      Img3dsl* const ims = (Img3dsl*)objs[0];
      objd[0] = new Img3dsl((Long)atol(parv[2]), (Long)atol(parv[1]), (Long)atol(parv[0]));
      Img3dsl* const imd = (Img3dsl*)objd[0];
      result = PResize(*ims, *imd);
      goto end;
   }

   if (objs[0]->Type() == Po_Img3dsf) {
      if ((atol(parv[2]) <= 0) ||
	  (atol(parv[1]) <= 0) ||
	  (atol(parv[0]) <= 0) ) {
	 std::cerr << "Error presize: Bad parameter values: " << parv[0] << " " << parv[1] << " "<< parv[2] << std::endl;
	 result = FAILURE;
	 goto end;
      }

      Img3dsf* const ims = (Img3dsf*)objs[0];
      objd[0] = new Img3dsf((Long)atol(parv[2]), (Long)atol(parv[1]), (Long)atol(parv[0]));
      Img3dsf* const imd = (Img3dsf*)objd[0];
      result = PResize(*ims, *imd);
      goto end;
   }

   if (objs[0]->Type() == Po_Imc2duc) {
      if ((atol(parv[1]) <= 0) ||
	  (atol(parv[0]) <= 0) ) {
	 std::cerr << "Error presize: Bad parameter values: " << parv[0] <<" " << parv[1] << std::endl;
	 result = FAILURE;
	 goto end;
      }

      Imc2duc* const ims = (Imc2duc*)objs[0];
      objd[0] = new Imc2duc((Long)atol(parv[1]), (Long)atol(parv[0]));
      Imc2duc* const imd = (Imc2duc*)objd[0];
      result = PResize(*ims,*imd);
      goto end;
   }

   if (objs[0]->Type() == Po_Imc2dsl) {
      if ((atol(parv[1]) <= 0) ||
	  (atol(parv[0]) <= 0) ) {
	 std::cerr << "Error presize: Bad parameter values: " << parv[0] <<" " << parv[1] << std::endl;
	 result = FAILURE;
	 goto end;
      }

      Imc2dsl* const ims = (Imc2dsl*)objs[0];
      objd[0] = new Imc2dsl((Long)atol(parv[1]), (Long)atol(parv[0]));
      Imc2dsl* const imd = (Imc2dsl*)objd[0];
      result = PResize(*ims,*imd);
      goto end;
   }

   if (objs[0]->Type() == Po_Imc2dsf) {
      if ((atol(parv[1]) <= 0) ||
	  (atol(parv[0]) <= 0) ) {
	 std::cerr << "Error presize: Bad parameter values: " << parv[0] <<" " << parv[1] << std::endl;
	 result = FAILURE;
	 goto end;
      }

      Imc2dsf* const ims = (Imc2dsf*)objs[0];
      objd[0] = new Imc2dsf((Long)atol(parv[1]), (Long)atol(parv[0]));
      Imc2dsf* const imd = (Imc2dsf*)objd[0];
      result = PResize(*ims,*imd);
      goto end;
   }

   if (objs[0]->Type() == Po_Imc3duc) { 
      if ((atol(parv[2]) <= 0) ||
	  (atol(parv[1]) <= 0) ||
	  (atol(parv[0]) <= 0) ) {
	 std::cerr << "Error presize: Bad parameter values: " << parv[0] <<" " << parv[1] << " " << parv[2] << std::endl;
	 result = FAILURE;
	 goto end;
      }

      Imc3duc* const ims = (Imc3duc*)objs[0];
      objd[0] = new Imc3duc((Long)atol(parv[2]),(Long)atol(parv[1]),(Long)atol(parv[0]));
      Imc3duc* const imd = (Imc3duc*)objd[0];
      result = PResize(*ims, *imd);
      goto end;
   }

   if (objs[0]->Type() == Po_Imc3dsl) { 
      if ((atol(parv[2]) <= 0) ||
	  (atol(parv[1]) <= 0) ||
	  (atol(parv[0]) <= 0) ) {
	 std::cerr << "Error presize: Bad parameter values: " << parv[0] <<" " << parv[1] << " " << parv[2] << std::endl;
	 result = FAILURE;
	 goto end;
      }

      Imc3dsl* const ims = (Imc3dsl*)objs[0];
      objd[0] = new Imc3dsl((Long)atol(parv[2]),(Long)atol(parv[1]),(Long)atol(parv[0]));
      Imc3dsl* const imd = (Imc3dsl*)objd[0];
      result = PResize(*ims, *imd);
      goto end;
   }

   if (objs[0]->Type() == Po_Imc3dsf) { 
      if ((atol(parv[2]) <= 0) ||
	  (atol(parv[1]) <= 0) ||
	  (atol(parv[0]) <= 0) ) {
	 std::cerr << "Error presize: Bad parameter values: " << parv[0] <<" " << parv[1] << " " << parv[2] << std::endl;
	 result = FAILURE;
	 goto end;
      }

      Imc3dsf* const ims = (Imc3dsf*)objs[0];
      objd[0] = new Imc3dsf((Long)atol(parv[2]),(Long)atol(parv[1]),(Long)atol(parv[0]));
      Imc3dsf* const imd = (Imc3dsf*)objd[0];
      result = PResize(*ims, *imd);
      goto end;
   }

   if (objs[0]->Type() == Po_Imx2duc) {
      if ((atol(parv[1]) <= 0) ||
	  (atol(parv[0]) <= 0) ) {
	 std::cerr << "Error presize: Bad parameter values: " << parv[0] << " " << parv[1] << std::endl;
	 result = FAILURE;
	 goto end;
      }

      Imx2duc* const ims = (Imx2duc*)objs[0];
      objd[0] = new Imx2duc(ims->Bands(), (Long)atol(parv[1]), (Long)atol(parv[0]));
      Imx2duc* const imd = (Imx2duc*)objd[0];
      result = PResize(*ims, *imd);
      goto end;
   }

   if (objs[0]->Type() == Po_Imx2dsl) {
      if ((atol(parv[1]) <= 0) ||
	  (atol(parv[0]) <= 0) ) {
	 std::cerr << "Error presize: Bad parameter values: " << parv[0] << " " << parv[1] << std::endl;
	 result = FAILURE;
	 goto end;
      }

      Imx2dsl* const ims = (Imx2dsl*)objs[0];
      objd[0] = new Imx2dsl(ims->Bands(), (Long)atol(parv[1]), (Long)atol(parv[0]));
      Imx2dsl* const imd = (Imx2dsl*)objd[0];
      result = PResize(*ims, *imd);
      goto end;
   }

   if (objs[0]->Type() == Po_Imx2dsf) {
      if ((atol(parv[1]) <= 0) ||
	  (atol(parv[0]) <= 0) ) {
	 std::cerr << "Error presize: Bad parameter values: " << parv[0] << " " << parv[1] << std::endl;
	 result = FAILURE;
	 goto end;
      }

      Imx2dsf* const ims = (Imx2dsf*)objs[0];
      objd[0] = new Imx2dsf(ims->Bands(), (Long)atol(parv[1]), (Long)atol(parv[0]));
      Imx2dsf* const imd = (Imx2dsf*)objd[0];
      result = PResize(*ims, *imd);
      goto end;
   }

   if (objs[0]->Type() == Po_Imx3duc) {
      if ((atol(parv[2]) <= 0) ||
	  (atol(parv[1]) <= 0) ||
	  (atol(parv[0]) <= 0) ) {
	 std::cerr << "Error presize: Bad parameter values: " << parv[0] <<" " << parv[1] << " " << parv[2] << std::endl;
	 result = FAILURE;
	 goto end;
      }

      Imx3duc* const ims = (Imx3duc*)objs[0];
      objd[0] = new Imx3duc(ims->Bands(), (Long)atol(parv[2]), (Long)atol(parv[1]), (Long)atol(parv[0]));
      Imx3duc* const imd = (Imx3duc*)objd[0];
      result = PResize(*ims, *imd);
      goto end;
   }

   if (objs[0]->Type() == Po_Imx3dsl) {
      if ((atol(parv[2]) <= 0) ||
	  (atol(parv[1]) <= 0) ||
	  (atol(parv[0]) <= 0) ) {
	 std::cerr << "Error presize: Bad parameter values: " << parv[0] <<" " << parv[1] << " " << parv[2] << std::endl;
	 result = FAILURE;
	 goto end;
      }

      Imx3dsl* const ims = (Imx3dsl*)objs[0];
      objd[0] = new Imx3dsl(ims->Bands(), (Long)atol(parv[2]), (Long)atol(parv[1]), (Long)atol(parv[0]));
      Imx3dsl* const imd = (Imx3dsl*)objd[0];
      result = PResize(*ims, *imd);
      goto end;
   }

   if (objs[0]->Type() == Po_Imx3dsf) {
      if ((atol(parv[2]) <= 0) ||
	  (atol(parv[1]) <= 0) ||
	  (atol(parv[0]) <= 0) ) {
	 std::cerr << "Error presize: Bad parameter values: " << parv[0] <<" " << parv[1] << " " << parv[2] << std::endl;
	 result = FAILURE;
	 goto end;
      }

      Imx3dsf* const ims = (Imx3dsf*)objs[0];
      objd[0] = new Imx3dsf(ims->Bands(), (Long)atol(parv[2]), (Long)atol(parv[1]), (Long)atol(parv[0]));
      Imx3dsf* const imd = (Imx3dsf*)objd[0];
      result = PResize(*ims, *imd);
      goto end;
   }

   if (objs[0]->Type() == Po_Reg2d) {
      if ((atol(parv[1]) <= 0) ||
	  (atol(parv[0]) <= 0) ) {
	 std::cerr << "Error presize: Bad parameter values: " << parv[0] << " " << parv[1] << std::endl;
	 result = FAILURE;
	 goto end;
      }

      Reg2d* const rgs = (Reg2d*)objs[0];
      objd[0] = new Reg2d((Long)atol(parv[1]), (Long)atol(parv[0]));
      Reg2d* const rgd = (Reg2d*)objd[0];
      result = PResize(*rgs, *rgd);
      goto end;
   }

   if (objs[0]->Type() == Po_Reg3d) {
      if ((atol(parv[2]) <= 0) ||
	  (atol(parv[1]) <= 0) ||
	  (atol(parv[0]) <= 0) ) {
	 std::cerr << "Error presize: Bad parameter values: " << parv[0] << " " << parv[1] << " " << parv[2] << std::endl;
	 result = FAILURE;
	 goto end;
      }

      Reg3d* const rgs = (Reg3d*)objs[0];
      objd[0] = new Reg3d((Long)atol(parv[2]), (Long)atol(parv[1]), (Long)atol(parv[0]));
      Reg3d* const rgd = (Reg3d*)objd[0];
      result  =PResize(*rgs, *rgd);
      goto end;
   }

   if (objs[0]->Type() == Po_Graph2d) {
      if ((atol(parv[1]) <= 0) ||
	  (atol(parv[0]) <= 0) ) {
	 std::cerr << "Error presize: Bad parameter values: " << parv[0] << " " << parv[1] << std::endl;
	 result = FAILURE;
	 goto end;
      }

      Graph2d* const grs = (Graph2d*)objs[0];
      objd[0] = new Graph2d(grs->Size(), (Long)atol(parv[1]), (Long)atol(parv[0]));
      Graph2d* const grd = (Graph2d*)objd[0];

      result = PResize(*grs, *grd);
      goto end;
   }

   if (objs[0]->Type() == Po_Graph3d) {
      if ((atol(parv[2]) <= 0) ||
	  (atol(parv[1]) <= 0) ||
	  (atol(parv[0]) <= 0) ) {
	 std::cerr <<"Error presize: parameter values: " << atol(parv[0]) << " " << atol(parv[1]) << " " << atol(parv[2]) << std::endl;
	 result = FAILURE;
	 goto end;
      }
      Graph3d* const grs = (Graph3d*)objs[0];
      objd[0] = new Graph3d(grs->Size(),(Long)atol(parv[2]), (Long)atol(parv[1]), (Long)atol(parv[0]));
      Graph3d* const grd = (Graph3d*)objd[0];
      result = PResize(*grs,*grd);
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
