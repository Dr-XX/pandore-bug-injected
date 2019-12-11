/* -*- c-basic-offset: 3; mode: c++ -*-
 *
 * Copyright (c) 2013,  GREYC.
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
 * @author Régis Clouard - 1997-02-22
 */

#include <pandore.h>
using namespace pandore;

/**
 * @file prescale.cpp
 * Rescales the image according to scale factor in each direction.
 * This version uses the nearest neighbors interpolation.
 *
 * @param rescalex scale factor along X axis.
 * @param rescaley scale factor along Y axis.
 * @param rescalez scale factor along Z axis.
 * @return SUCCESS or FAILURE
 * imd(i, j) = ims(i/scaley, j/scalex).
 */
Errc PRescale( const Reg2d &rgs, Reg2d &rgd, const float scaley, const float scalex ) {
   Point2d p;

   if ( scalex <= 0 || scaley <= 0 ) {
      fprintf(stderr, "Error prescale: Bad parameter values: %f %f\n", scalex, scaley);
      return FAILURE;
   }

   for (p.y = 0 ; p.y < rgd.Height(); p.y++) {
      for (p.x = 0 ; p.x < rgd.Width(); p.x++) {
	 rgd[p] = rgs((int)(p.y / scaley), (int)(p.x / scalex));
      }

      
   }
   rgd.Labels(rgs.Labels());
   return SUCCESS;
}

template <typename T>
Errc PRescale( const Imx2d<T> &ims, Imx2d<T> &imd, const float scaley, const float scalex ) {
   Point2d p;

   if ( scalex <= 0 || scaley <= 0 ) {
      fprintf(stderr, "Error prescale: Bad parameter values: %f %f\n", scalex, scaley);
      return FAILURE;
   }

   for (int b = 0; b < imd.Bands(); b++) {
      for (p.y = 0 ; p.y < imd.Height(); p.y++) {
	 for (p.x = 0 ; p.x < imd.Width(); p.x++) {
	    imd[b][p] = ims(b, (int)(p.y / scaley), (int)(p.x / scalex));
	 }
      }
   }
   
   return SUCCESS;
}






template <typename T>
Errc PRescale( const Reg3d &rgs, Reg3d &rgd, const float scalez, const float scaley, const float scalex ) {
   Point3d p;

   if ( scalex <= 0 || scaley <= 0 || scalez <= 0) {
      fprintf(stderr, "Error prescale: Bad parameter values: %f %f %f\n", scalex, scaley, scalez);
      return FAILURE;
   }
   for (p.z = 0 ; p.z < rgd.Depth() ; p.z++) {
      int z1 = (int)(p.z / scalez);
      for (p.y = 0 ; p.y < rgd.Height() ; p.y++) {
	 int y1 = (int)(p.y / scaley);
	 for (p.x = 0 ; p.x < rgd.Width() ; p.x++) {
	    int x1 = (int)(p.x / scalex);
	    rgd(p.z, p.y, p.x) = rgs(z1, y1, x1);
	 }
      }
   }

   rgd.Labels(rgs.Labels());
   return SUCCESS;
}

template <typename T>
Errc PRescale( const Imx3d<T> &ims, Imx3d<T> &imd, const float scalez, const float scaley, const float scalex ) {
   Point3d p;

   if ( scalex <= 0 || scaley <= 0 || scalez <= 0) {
      fprintf(stderr, "Error prescale: Bad parameter values: %f %f %f\n", scalex, scaley, scalez);
      return FAILURE;
   }
   for (int b = 0; b < imd.Bands(); b++) {
      for (p.z = 0 ; p.z < imd.Depth() ; p.z++) {
	 int z1 = (int)(p.z / scalez);
	 for (p.y = 0 ; p.y < imd.Height() ; p.y++) {
	    int y1 = (int)(p.y / scaley);
	    for (p.x = 0 ; p.x < imd.Width() ; p.x++) {
	       int x1 = (int)(p.x / scalex);
	       imd(b, p.z, p.y, p.x) = ims(b, z1, y1, x1);
	    }
	 }
      }
   }
   return SUCCESS;
}











Errc PRescale( const Graph2d &grs, Graph2d &grd,  const float zoomy, const float zoomx ) {
   GEdge *l;

   for (int i = 0; i < grs.Size(); i++) {
      if (grs[i] != NULL) {
	 grd.Add(i, grs[i]->Item(), (int)(grs[i]->seed.y * zoomy), (int)(grs[i]->seed.x * zoomx));
	 grd[i]->value = grs[i]->value;
	 for (l = grs[i]->Neighbours(); l != NULL; l = l->Next()) {
	    if (i > l->Node()) {
	       grd.Link(i, l->Node(), l->weight);
	    }
	 }
      }
   }
   
   return SUCCESS;
}

Errc PRescale( const Graph3d &grs, Graph3d &grd, const float zoomz, const float zoomy, const float zoomx ) {
   GEdge *l;
   
   for (int i = 0; i < grs.Size(); i++) {
      if (grs[i] != NULL) {
	 grd.Add(i, grs[i]->Item(),
		 (int)(grs[i]->seed.z * zoomz),
		 (int)(grs[i]->seed.y * zoomy),
		 (int)(grs[i]->seed.x * zoomx));
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
#define	USAGE	"usage: %s zoomx zoomy zoomz [im_in|-] [im_out|-]"
#define	PARC	3
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
   if (objs[0]->Type() == Po_Img2duc) {
      const float scalex = (float)atof(parv[0]);
      const float scaley = (float)atof(parv[1]);
      if ( scalex <= 0 || scaley <= 0) {
	 result = FAILURE;
      } else {
	 Img2duc* const ims = (Img2duc*)objs[0];
	 objd[0] = new Img2duc(Long(ims->Height() * scaley),
			    Long(ims->Width() * scalex));
	 Img2duc* const imd = (Img2duc*)objd[0];
	 result = PRescale(*ims, *imd, scaley, scalex);
      }
      goto end;
   }
   if (objs[0]->Type() == Po_Img2dsl) {
      const float scalex = (float)atof(parv[0]);
      const float scaley = (float)atof(parv[1]);
      if ( scalex <= 0 || scaley <= 0) {
	 result = FAILURE;
      } else {
	 Img2dsl* const ims = (Img2dsl*)objs[0];
	 objd[0] = new Img2dsl(Long(ims->Height() * scaley),
			    Long(ims->Width() * scalex));
	 Img2dsl* const imd = (Img2dsl*)objd[0];
	 result = PRescale(*ims, *imd, scaley, scalex);
      }
      goto end;
   }
   if (objs[0]->Type() == Po_Img2dsf) {
      const float scalex = (float)atof(parv[0]);
      const float scaley = (float)atof(parv[1]);
      if ( scalex <= 0 || scaley <= 0) {
	 result = FAILURE;
      } else {
	 Img2dsf* const ims = (Img2dsf*)objs[0];
	 objd[0] = new Img2dsf(Long(ims->Height() * scaley),
			    Long(ims->Width() * scalex));
	 Img2dsf* const imd = (Img2dsf*)objd[0];
	 result = PRescale(*ims, *imd, scaley, scalex);
      }
      goto end;
   }
   if (objs[0]->Type() == Po_Img3duc) {
      const float scalex = (float)atof(parv[0]);
      const float scaley = (float)atof(parv[1]);
      const float scalez = (float)atof(parv[2]);
      if ( scalex <= 0 || scaley <= 0 || scalez <= 0) {
	 result = FAILURE;
      } else {
	 Img3duc* const ims = (Img3duc*)objs[0];
	 objd[0] = new Img3duc(Long(ims->Depth() * scalez),
			    Long(ims->Height() * scaley),
			    Long(ims->Width() * scalex));
	 Img3duc* const imd = (Img3duc*)objd[0];
	 result = PRescale(*ims, *imd, scalez, scaley, scalex);
      }
      goto end;
   }
   if (objs[0]->Type() == Po_Img3dsl) {
      const float scalex = (float)atof(parv[0]);
      const float scaley = (float)atof(parv[1]);
      const float scalez = (float)atof(parv[2]);
      if ( scalex <= 0 || scaley <= 0 || scalez <= 0) {
	 result = FAILURE;
      } else {
	 Img3dsl* const ims = (Img3dsl*)objs[0];
	 objd[0] = new Img3dsl(Long(ims->Depth() * scalez),
			    Long(ims->Height() * scaley),
			    Long(ims->Width() * scalex));
	 Img3dsl* const imd = (Img3dsl*)objd[0];
	 result = PRescale(*ims, *imd, scalez, scaley, scalex);
      }
      goto end;
   }
   if (objs[0]->Type() == Po_Img3dsf) {
      const float scalex = (float)atof(parv[0]);
      const float scaley = (float)atof(parv[1]);
      const float scalez = (float)atof(parv[2]);
      if ( scalex <= 0 || scaley <= 0 || scalez <= 0) {
	 result = FAILURE;
      } else {
	 Img3dsf* const ims = (Img3dsf*)objs[0];
	 objd[0] = new Img3dsf(Long(ims->Depth() * scalez),
			    Long(ims->Height() * scaley),
			    Long(ims->Width() * scalex));
	 Img3dsf* const imd = (Img3dsf*)objd[0];
	 result = PRescale(*ims, *imd, scalez, scaley, scalex);
      }
      goto end;
   }
if (objs[0]->Type() == Po_Imc2duc) {
      const float scalex = (float)atof(parv[0]);
      const float scaley = (float)atof(parv[1]);
      if ( scalex <= 0 || scaley <= 0) {
	 result = FAILURE;
      } else {
	 Imc2duc* const ims = (Imc2duc*)objs[0];
	 objd[0] = new Imc2duc(Long(ims->Height() * scaley),
			    Long(ims->Width() * scalex));
	 Imc2duc* const imd = (Imc2duc*)objd[0];
	 imd->ColorSpace(ims->ColorSpace());
	 result = PRescale(*ims, *imd, scaley, scalex);
      }
      goto end;
   }
if (objs[0]->Type() == Po_Imc2dsl) {
      const float scalex = (float)atof(parv[0]);
      const float scaley = (float)atof(parv[1]);
      if ( scalex <= 0 || scaley <= 0) {
	 result = FAILURE;
      } else {
	 Imc2dsl* const ims = (Imc2dsl*)objs[0];
	 objd[0] = new Imc2dsl(Long(ims->Height() * scaley),
			    Long(ims->Width() * scalex));
	 Imc2dsl* const imd = (Imc2dsl*)objd[0];
	 imd->ColorSpace(ims->ColorSpace());
	 result = PRescale(*ims, *imd, scaley, scalex);
      }
      goto end;
   }
if (objs[0]->Type() == Po_Imc2dsf) {
      const float scalex = (float)atof(parv[0]);
      const float scaley = (float)atof(parv[1]);
      if ( scalex <= 0 || scaley <= 0) {
	 result = FAILURE;
      } else {
	 Imc2dsf* const ims = (Imc2dsf*)objs[0];
	 objd[0] = new Imc2dsf(Long(ims->Height() * scaley),
			    Long(ims->Width() * scalex));
	 Imc2dsf* const imd = (Imc2dsf*)objd[0];
	 imd->ColorSpace(ims->ColorSpace());
	 result = PRescale(*ims, *imd, scaley, scalex);
      }
      goto end;
   }
   if (objs[0]->Type() == Po_Imc3duc) {
      const float scalex = (float)atof(parv[0]);
      const float scaley = (float)atof(parv[1]);
      const float scalez = (float)atof(parv[2]);
      if ( scalex <= 0 || scaley <= 0 || scalez <= 0) {
	 result = FAILURE;
      } else {
	 Imc3duc* const ims = (Imc3duc*)objs[0];
	 objd[0] = new Imc3duc(Long(ims->Depth() * scalez),
			    Long(ims->Height() * scaley),
			    Long(ims->Width() * scalex));
	 Imc3duc* const imd = (Imc3duc*)objd[0];
	 result = PRescale(*ims, *imd, scalez, scaley, scalex);
	 imd->ColorSpace(ims->ColorSpace());
      }
      goto end;
   }
   if (objs[0]->Type() == Po_Imc3dsl) {
      const float scalex = (float)atof(parv[0]);
      const float scaley = (float)atof(parv[1]);
      const float scalez = (float)atof(parv[2]);
      if ( scalex <= 0 || scaley <= 0 || scalez <= 0) {
	 result = FAILURE;
      } else {
	 Imc3dsl* const ims = (Imc3dsl*)objs[0];
	 objd[0] = new Imc3dsl(Long(ims->Depth() * scalez),
			    Long(ims->Height() * scaley),
			    Long(ims->Width() * scalex));
	 Imc3dsl* const imd = (Imc3dsl*)objd[0];
	 result = PRescale(*ims, *imd, scalez, scaley, scalex);
	 imd->ColorSpace(ims->ColorSpace());
      }
      goto end;
   }
   if (objs[0]->Type() == Po_Imc3dsf) {
      const float scalex = (float)atof(parv[0]);
      const float scaley = (float)atof(parv[1]);
      const float scalez = (float)atof(parv[2]);
      if ( scalex <= 0 || scaley <= 0 || scalez <= 0) {
	 result = FAILURE;
      } else {
	 Imc3dsf* const ims = (Imc3dsf*)objs[0];
	 objd[0] = new Imc3dsf(Long(ims->Depth() * scalez),
			    Long(ims->Height() * scaley),
			    Long(ims->Width() * scalex));
	 Imc3dsf* const imd = (Imc3dsf*)objd[0];
	 result = PRescale(*ims, *imd, scalez, scaley, scalex);
	 imd->ColorSpace(ims->ColorSpace());
      }
      goto end;
   }
   if (objs[0]->Type() == Po_Imx2duc) {
      const float scalex = (float)atof(parv[0]);
      const float scaley = (float)atof(parv[1]);
      if ( scalex <= 0 || scaley <= 0) {
	 result = FAILURE;
      } else {
	 Imx2duc* const ims = (Imx2duc*)objs[0];
	 objd[0] = new Imx2duc(ims->Bands(),
			    Long(ims->Height() * scaley),
			    Long(ims->Width() * scalex));
	 Imx2duc* const imd = (Imx2duc*)objd[0];
	 result = PRescale(*ims, *imd, scaley, scalex);
      }
      goto end;
   }

   if (objs[0]->Type() == Po_Imx2dsl) {
      const float scalex = (float)atof(parv[0]);
      const float scaley = (float)atof(parv[1]);
      if ( scalex <= 0 || scaley <= 0) {
	 result = FAILURE;
      } else {
	 Imx2dsl* const ims = (Imx2dsl*)objs[0];
	 objd[0] = new Imx2dsl(ims->Bands(),
			    Long(ims->Height() * scaley),
			    Long(ims->Width() * scalex));
	 Imx2dsl* const imd = (Imx2dsl*)objd[0];
	 result = PRescale(*ims, *imd, scaley, scalex);
      }
      goto end;
   }

   if (objs[0]->Type() == Po_Imx2dsf) {
      const float scalex = (float)atof(parv[0]);
      const float scaley = (float)atof(parv[1]);
      if ( scalex <= 0 || scaley <= 0) {
	 result = FAILURE;
      } else {
	 Imx2dsf* const ims = (Imx2dsf*)objs[0];
	 objd[0] = new Imx2dsf(ims->Bands(),
			    Long(ims->Height() * scaley),
			    Long(ims->Width() * scalex));
	 Imx2dsf* const imd = (Imx2dsf*)objd[0];
	 result = PRescale(*ims, *imd, scaley, scalex);
      }
      goto end;
   }

   if (objs[0]->Type() == Po_Imx3duc) {
      const float scalex = (float)atof(parv[0]);
      const float scaley = (float)atof(parv[1]);
      const float scalez = (float)atof(parv[2]);
      if ( scalex <= 0 || scaley <= 0 || scalez <= 0) {
	 result = FAILURE;
      } else {
	 Imx3duc* const ims = (Imx3duc*)objs[0];
	 objd[0] = new Imx3duc(ims->Bands(),
			    Long(ims->Depth() * scalez),
			    Long(ims->Height() * scaley),
			    Long(ims->Width() * scalex));
	 Imx3duc* const imd = (Imx3duc*)objd[0];
	 result = PRescale(*ims, *imd, scalez, scaley, scalex);
      }
      goto end;
   }

   if (objs[0]->Type() == Po_Imx3dsl) {
      const float scalex = (float)atof(parv[0]);
      const float scaley = (float)atof(parv[1]);
      const float scalez = (float)atof(parv[2]);
      if ( scalex <= 0 || scaley <= 0 || scalez <= 0) {
	 result = FAILURE;
      } else {
	 Imx3dsl* const ims = (Imx3dsl*)objs[0];
	 objd[0] = new Imx3dsl(ims->Bands(),
			    Long(ims->Depth() * scalez),
			    Long(ims->Height() * scaley),
			    Long(ims->Width() * scalex));
	 Imx3dsl* const imd = (Imx3dsl*)objd[0];
	 result = PRescale(*ims, *imd, scalez, scaley, scalex);
      }
      goto end;
   }

   if (objs[0]->Type() == Po_Imx3dsf) {
      const float scalex = (float)atof(parv[0]);
      const float scaley = (float)atof(parv[1]);
      const float scalez = (float)atof(parv[2]);
      if ( scalex <= 0 || scaley <= 0 || scalez <= 0) {
	 result = FAILURE;
      } else {
	 Imx3dsf* const ims = (Imx3dsf*)objs[0];
	 objd[0] = new Imx3dsf(ims->Bands(),
			    Long(ims->Depth() * scalez),
			    Long(ims->Height() * scaley),
			    Long(ims->Width() * scalex));
	 Imx3dsf* const imd = (Imx3dsf*)objd[0];
	 result = PRescale(*ims, *imd, scalez, scaley, scalex);
      }
      goto end;
   }

   if (objs[0]->Type() == Po_Reg2d) {
      const float scalex = (float)atof(parv[0]);
      const float scaley = (float)atof(parv[1]);
      if ( scalex <= 0 || scaley <= 0) {
	 result = FAILURE;
      } else {
	 Reg2d* const rgs = (Reg2d*)objs[0];
	 objd[0] = new Reg2d(Long(rgs->Height() * scaley),
			    Long(rgs->Width() * scalex));
	 Reg2d* const rgd = (Reg2d*)objd[0];
	 result = PRescale(*rgs, *rgd, scaley, scalex);
      }
      goto end;
   }

   if (objs[0]->Type() == Po_Reg3d) {
      const float scalex = (float)atof(parv[0]);
      const float scaley = (float)atof(parv[1]);
      const float scalez = (float)atof(parv[2]);
      if ( scalex <= 0 || scaley <= 0 || scalez <= 0) {
	 result = FAILURE;
      } else {
	 Reg3d* const rgs = (Reg3d*)objs[0];
	 objd[0] = new Reg3d(Long(rgs->Depth() * scalez),
			    Long(rgs->Height() * scaley),
			    Long(rgs->Width() * scalex));
	 Reg3d* const rgd = (Reg3d*)objd[0];
	 result = PRescale(*rgs, *rgd, scalez, scaley, scalex);
      }
      goto end;
   }

   if (objs[0]->Type() == Po_Graph2d) {
      Graph2d* const grs = (Graph2d*)objs[0];
      objd[0] = new Graph2d(grs->Size(), Long(grs->Height() * atoi(parv[1])),
			 Long(grs->Width() * atoi(parv[0])));
      Graph2d* const grd = (Graph2d*)objd[0];
      if ( (atof(parv[0]) <= 0) || (atof(parv[1]) <= 0)) {
	 result = FAILURE;
      } else {
	 result = PRescale(*grs, *grd, (float)atof(parv[1]), (float)atof(parv[0]));
      }
      goto end;
   }

   if (objs[0]->Type() == Po_Graph3d) {
      Graph3d* const grs = (Graph3d*)objs[0];
      objd[0] = new Graph3d(grs->Size(),
			 Long(atoi(parv[2]) * grs->Depth()),
			 Long(atoi(parv[1]) * grs->Height()),
			 Long(atoi(parv[0]) * grs->Width()));
      Graph3d* const grd = (Graph3d*)objd[0];
      if ( (atof(parv[0]) <= 0) || (atof(parv[1]) <= 0) || (atof(parv[2]) <= 0)) {
	 result = FAILURE;
      } else {
	 result = PRescale(*grs, *grd, (float)atof(parv[2]), (float)atof(parv[1]), (float)atof(parv[0]));
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
