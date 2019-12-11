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
 * @author Régis Clouard - 2006-11-17
 */

#include <pandore.h>
using namespace pandore;

/**
 * @file plinearrescale.cpp
 * Rescales the image according to scale factor in each axis direction.
 * This version uses the bilinear interpolation.
 *
 * @param rescalex scale factor along X axis.
 * @param rescaley scale factor along Y axis.
 * @param rescalez scale factor along Z axis.
 * @return SUCCESS or FAILURE
 * imd(i,j) = ims(i/scaley,j/scalex).
 */
template <typename T>
Errc PLinearRescale( const Imx2d<T> &ims, Imx2d<T> &imd, const float scaley, const float scalex ) {
   Point2d p;
   int b;

   // The left bottom side
   for (b = 0; b < imd.Bands(); b++) {
      for (p.y = imd.Height()-(long)scaley; p.y < imd.Height(); p.y++) {
	 for (p.x = imd.Width()-(long)scalex; p.x < imd.Width(); p.x++) {
 	    imd(b, p) = ims(b, ims.Height()-1, ims.Width()-1);
	 }
      }
   }

   // The left side of the image
   for (b = 0; b < imd.Bands(); b++) {
      for (p.y = 0 ; p.y < imd.Height()-(long)scaley; p.y++) {
	 for (p.x = imd.Width()-(long)scalex; p.x < imd.Width(); p.x++) {
	    const float oldx = p.x / scalex;
	    const float oldy = p.y / scaley;
	    const int sx = (int)oldx;
	    const int sy = (int)oldy;
	    const float dx = oldx - sx;
	    const float dy = oldy - sy;
	    if (sx < ims.Width()-1) {
	       imd(b, p) = (T)((1.0f-dx) * (1.0f-dy) * ims(b, sy, sx)
			      + (1.0f-dx)*dy * ims(b, sy+1, sx)
			      + dx * (1.0f-dy) * ims(b, sy, sx+1)
			      + dx * dy * ims(b, sy+1, sx+1));
	    } else {
	       imd(b, p) = (T)((1.0f-dy) * ims(b, sy, sx)
			      + dy * ims(b, sy+1, sx));
	    }
	 }
      }
   }
   
   // The bottom side 
   for (b = 0; b < imd.Bands(); b++) {
      for (p.y = imd.Height()-(long)scaley; p.y < imd.Height(); p.y++) {
	 for (p.x = 0; p.x < imd.Width()-(long)scalex; p.x++) {
	    const float oldx = p.x / scalex;
	    const float oldy = p.y / scaley;
	    const int sx = (int)oldx;
	    const int sy = (int)oldy;
	    const float dx = oldx - sx;
	    const float dy = oldy - sy;
	    if (sy < ims.Height()-1) {
	       imd(b, p) = (T)((1.0f-dx) * (1.0f-dy) * ims(b, sy, sx)
			      + (1.0f-dx)*dy * ims(b, sy+1, sx)
			      + dx * (1.0f-dy) * ims(b, sy, sx+1)
			      + dx * dy * ims(b, sy+1, sx+1));
	    } else {
	       imd(b, p) = (T)((1.0f-dx) * ims(b, sy, sx)
			      + dx * ims(b, sy, sx+1));
	    }
	 }
      }
   }
   // General case
   for (b = 0; b < imd.Bands(); b++) {
      for (p.y = 0 ; p.y < imd.Height()-(long)scaley; p.y++) {
	 for (p.x = 0 ; p.x < imd.Width()-(long)scalex; p.x++) {
	    const float oldx = p.x / scalex;
	    const float oldy = p.y / scaley;
	    const float dx = oldx - int(oldx);
	    const float dy = oldy - int(oldy);
	    const int sx = (int)oldx;
	    const int sy = (int)oldy;
	    imd(b, p) = (T)((1.0f-dx) * (1.0f-dy) * ims(b, sy, sx)
			   + (1.0f-dx)*dy * ims(b, sy+1, sx)
			   + dx * (1.0f-dy) * ims(b, sy, sx+1)
			   + dx * dy * ims(b, sy+1, sx+1));
	 }
      }
   }

   return SUCCESS;
}



template <typename T>
Errc PLinearRescale( const Imx3d<T> &ims, Imx3d<T> &imd, const float scalez, const float scaley, const float scalex ) {
   Point3d p;
   int b;
   
   // The left bottom side 
   for (b = 0; b < imd.Bands(); b++) {
      for (p.z = imd.Depth()-(long)scalez; p.z < imd.Depth(); p.z++) {
	 for (p.y = imd.Height()-(long)scaley; p.y < imd.Height(); p.y++) {
	    for (p.x = imd.Width()-(long)scalex; p.x < imd.Width(); p.x++) {
	       imd(b, p) = (T)ims(b, ims.Depth()-1, ims.Height()-1, ims.Width()-1);
	    }
	 }
      }
   }

   
   // The left side of the image
   for (b = 0; b < imd.Bands(); b++) {
      for (p.z = 0; p.z < imd.Depth()-(long)scalez; p.z++) {
	 for (p.y = 0 ; p.y < imd.Height()-(long)scaley; p.y++) {
	    for (p.x = imd.Width()-(long)scalex; p.x < imd.Width(); p.x++) {
	       const float oldx = p.x / scalex;
	       const float oldy = p.y / scaley;
	       const float oldz = p.z / scalez;
	       const int sx = (int)oldx;
	       const int sy = (int)oldy;
	       const int sz = (int)oldz;
	       const float dx = oldx - sx;
	       const float dy = oldy - sy;
	       const float dz = oldz - sz;
	       if (sx < ims.Width()-1) {
		  imd(b, p) = (T)((1.0f-dx)*(1.0f-dy)*(1.0f-dz) * ims(b, sz, sy, sx)
				 + (1.0f-dx)*dy*(1.0f-dz) * ims(b, sz, sy+1, sx)
				 + dx*(1.0f-dy)*(1.0f-dz) * ims(b, sz, sy, sx+1)
				 + dx*dy*(1.0f-dz) * ims(b, sz, sy+1, sx+1)
				 
				 + (1.0f-dx)*(1.0f-dy)*dz * ims(b, sz+1, sy, sx)
				 + (1.0f-dx)*dy*dz * ims(b, sz+1, sy+1, sx)
				 + dx*(1.0f-dy)*dz * ims(b, sz+1, sy, sx+1)
				 + dx*dy*dz * ims(b, sz+1, sy+1, sx+1));
	       } else {
		  imd(b, p) = (T)((1.0f-dy) * ims(b, sz, sy, sx)
				 + dy * ims(b, sz, sy+1, sx)
				 +(1.0f-dy) * ims(b, sz+1, sy, sx)
				 + dy * ims(b, sz+1, sy+1, sx));
	       }
	    }
	 }
      }
   }
   // The bottom side of the image
   for (b = 0; b < imd.Bands(); b++) { 
      for (p.z = 0; p.z < imd.Depth()-(long)scalez; p.z++) {
	 for (p.y = imd.Height()-(long)scaley; p.y < imd.Height(); p.y++) {
	    for (p.x = imd.Width()-(long)scalex; p.x < imd.Width(); p.x++) {
	       const float oldx = p.x / scalex;
	       const float oldy = p.y / scaley;
	       const float oldz = p.z / scalez;
	       const int sx = (int)oldx;
	       const int sy = (int)oldy;
	       const int sz = (int)oldz;
	       const float dx = oldx - sx;
	       const float dy = oldy - sy;
	       const float dz = oldz - sz;
	       if (sy < ims.Height()-1) {
		  imd(b, p) = (T)((1.0f-dx)*(1.0f-dy)*(1.0f-dz) * ims(b, sz, sy, sx)
				 + (1.0f-dx)*dy*(1.0f-dz) * ims(b, sz, sy+1, sx)
				 + dx*(1.0f-dy)*(1.0f-dz) * ims(b, sz, sy, sx+1)
				 + dx*dy*(1.0f-dz) * ims(b, sz, sy+1, sx+1)
				 
				 + (1.0f-dx)*(1.0f-dy)*dz * ims(b, sz+1, sy, sx)
				 + (1.0f-dx)*dy*dz * ims(b, sz+1, sy+1, sx)
				 + dx*(1.0f-dy)*dz * ims(b, sz+1, sy, sx+1)
				 + dx*dy*dz * ims(b, sz+1, sy+1, sx+1));
	       } else {
		  imd(b, p) = (T)((1.0f-dx) * ims(b, sz, sy, sx)
				 + dx * ims(b, sz, sy, sx+1)
				 + (1.0f-dx) * ims(b, sz+1, sy, sx)
				 + dx * ims(b, sz+1, sy, sx+1));
	       }
	    }
	 }
      }
   }   

   // The back side of the image
   for (b = 0; b < imd.Bands(); b++) {
      for (p.z = imd.Depth()-(long)scalez; p.z < imd.Depth(); p.z++) {
	 for (p.y = 0 ; p.y < imd.Height()-(long)scaley; p.y++) {
	    for (p.x = imd.Width()-(long)scalex; p.x < imd.Width(); p.x++) {
	       const float oldx = p.x / scalex;
	       const float oldy = p.y / scaley;
	       const float oldz = p.z / scalez;
	       const int sx = (int)oldx;
	       const int sy = (int)oldy;
	       const int sz = (int)oldz;
	       const float dx = oldx - sx;
	       const float dy = oldy - sy;
	       const float dz = oldz - sz;
	       if (sz < ims.Depth()-1) {
		  imd(b, p) = (T)((1.0f-dx)*(1.0f-dy)*(1.0f-dz) * ims(b, sz, sy, sx)
				 + (1.0f-dx)*dy*(1.0f-dz) * ims(b, sz, sy+1, sx)
				 + dx*(1.0f-dy)*(1.0f-dz) * ims(b, sz, sy, sx+1)
				 + dx*dy*(1.0f-dz) * ims(b, sz, sy+1, sx+1)
				 
				 + (1.0f-dx)*(1.0f-dy)*dz * ims(b, sz+1, sy, sx)
				 + (1.0f-dx)*dy*dz * ims(b, sz+1, sy+1, sx)
				 + dx*(1.0f-dy)*dz * ims(b, sz+1, sy, sx+1)
				 + dx*dy*dz * ims(b, sz+1, sy+1, sx+1));
	       } else {
		  imd(b, p) = (T)((1.0f-dx)*(1.0f-dy)* ims(b, sz, sy, sx)
				 + (1.0f-dx)*dy* ims(b, sz, sy+1, sx)
				 + dx*(1.0f-dy) * ims(b, sz, sy, sx+1)
				 + dx*dy* ims(b, sz, sy+1, sx+1));
	       }
	    }
	 }
      }
   }
   
   for (b = 0; b < imd.Bands(); b++) {
      for (p.z = 0 ; p.z < imd.Depth()-(long)scalez ; p.z++) {
	 for (p.y = 0 ; p.y < imd.Height()-(long)scaley ; p.y++) {
	    for (p.x = 0 ; p.x < imd.Width()-(long)scalex ; p.x++) {
	       const float oldx = p.x / scalex;
	       const float oldy = p.y / scaley;
	       const float oldz = p.z / scalez;
	       const int sx = (int)oldx;
	       const int sy = (int)oldy;
	       const int sz = (int)oldz;
	       const float dx = oldx - sx;
	       const float dy = oldy - sy;
	       const float dz = oldz - sz;
	       imd(b, p) = (T)((1.0f-dx)*(1.0f-dy)*(1.0f-dz) * ims(b, sz, sy, sx)
			      + (1.0f-dx)*dy*(1.0f-dz) * ims(b, sz, sy+1, sx)
			      + dx*(1.0f-dy)*(1.0f-dz) * ims(b, sz, sy, sx+1)
			      + dx*dy*(1.0f-dz) * ims(b, sz, sy+1, sx+1)
			      
			      + (1.0f-dx)*(1.0f-dy)*dz * ims(b, sz+1, sy, sx)
			      + (1.0f-dx)*dy*dz * ims(b, sz+1, sy+1, sx)
			      + dx*(1.0f-dy)*dz * ims(b, sz+1, sy, sx+1)
			      + dx*dy*dz * ims(b, sz+1, sy+1, sx+1));
	    }
	 }
      }
   }    
   
   return SUCCESS;
}







Errc PLinearRescale( const Graph2d &grs, Graph2d &grd, float rescaley, float rescalex ) {
   GEdge *l;

   for (int i=0; i< grs.Size(); i++) {
      if (grs[i] != NULL) {
	 grd.Add(i, grs[i]->Item(), (int)(grs[i]->seed.y*rescaley), (int)(grs[i]->seed.x*rescalex));
	 grd[i]->value=grs[i]->value;
	 for (l=grs[i]->Neighbours(); l!=NULL; l=l->Next()) {
	    if (i>l->Node()) {
	       grd.Link(i, l->Node(), l->weight);
	    }
	 }
      }
   }
   
   return SUCCESS;
}

Errc PLinearRescale( const Graph3d &grs, Graph3d &grd, float rescalez, float rescaley, float rescalex ) {
   GEdge *l;

   for (int i=0; i< grs.Size(); i++) {
      if (grs[i] != NULL) {
	 grd.Add(i, grs[i]->Item(), (int)(grs[i]->seed.z*rescalez), (int)(grs[i]->seed.y*rescaley), (int)(grs[i]->seed.x*rescalex));
	 grd[i]->value=grs[i]->value;
	 for (l=grs[i]->Neighbours(); l!=NULL; l=l->Next()) {
	    if (i>l->Node()) {
	       grd.Link(i, l->Node(), l->weight);
	    }
	 }
      }
   }
   
   return SUCCESS;
}

#ifdef MAIN
#define	USAGE	"usage: %s rescalex rescaley rescalez [im_in|-] [im_out|-]"
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
   if (scalex <=0 || scaley <= 0) {
      result = FAILURE;
   } else {
      Img2duc* const ims = (Img2duc*)objs[0];
      objd[0] = new Img2duc(Long(ims->Height()*scaley), Long(ims->Width()*scalex));
      Img2duc* const imd = (Img2duc*)objd[0];
      result=PLinearRescale(*ims, *imd, scaley, scalex);
   }
   goto end;
 }
if (objs[0]->Type() == Po_Img2dsl) {
   const float scalex = (float)atof(parv[0]);
   const float scaley = (float)atof(parv[1]);
   if (scalex <=0 || scaley <= 0) {
      result = FAILURE;
   } else {
      Img2dsl* const ims = (Img2dsl*)objs[0];
      objd[0] = new Img2dsl(Long(ims->Height()*scaley), Long(ims->Width()*scalex));
      Img2dsl* const imd = (Img2dsl*)objd[0];
      result=PLinearRescale(*ims, *imd, scaley, scalex);
   }
   goto end;
 }
if (objs[0]->Type() == Po_Img2dsf) {
   const float scalex = (float)atof(parv[0]);
   const float scaley = (float)atof(parv[1]);
   if (scalex <=0 || scaley <= 0) {
      result = FAILURE;
   } else {
      Img2dsf* const ims = (Img2dsf*)objs[0];
      objd[0] = new Img2dsf(Long(ims->Height()*scaley), Long(ims->Width()*scalex));
      Img2dsf* const imd = (Img2dsf*)objd[0];
      result=PLinearRescale(*ims, *imd, scaley, scalex);
   }
   goto end;
 }
if (objs[0]->Type() == Po_Imc2duc) {
   const float scalex = (float)atof(parv[0]);
   const float scaley = (float)atof(parv[1]);
   if (scalex <= 0 || scaley <= 0) {
      result = FAILURE;
    } else {
      Imc2duc* const ims = (Imc2duc*)objs[0];
      objd[0] = new Imc2duc(Long(ims->Height()*scaley), Long(ims->Width()*scalex));
      Imc2duc* const imd = (Imc2duc*)objd[0];
      imd->ColorSpace(ims->ColorSpace());
      result = PLinearRescale(*ims, *imd, scaley, scalex);
   }
   goto end;
 }
if (objs[0]->Type() == Po_Imc2dsl) {
   const float scalex = (float)atof(parv[0]);
   const float scaley = (float)atof(parv[1]);
   if (scalex <= 0 || scaley <= 0) {
      result = FAILURE;
    } else {
      Imc2dsl* const ims = (Imc2dsl*)objs[0];
      objd[0] = new Imc2dsl(Long(ims->Height()*scaley), Long(ims->Width()*scalex));
      Imc2dsl* const imd = (Imc2dsl*)objd[0];
      imd->ColorSpace(ims->ColorSpace());
      result = PLinearRescale(*ims, *imd, scaley, scalex);
   }
   goto end;
 }
if (objs[0]->Type() == Po_Imc2dsf) {
   const float scalex = (float)atof(parv[0]);
   const float scaley = (float)atof(parv[1]);
   if (scalex <= 0 || scaley <= 0) {
      result = FAILURE;
    } else {
      Imc2dsf* const ims = (Imc2dsf*)objs[0];
      objd[0] = new Imc2dsf(Long(ims->Height()*scaley), Long(ims->Width()*scalex));
      Imc2dsf* const imd = (Imc2dsf*)objd[0];
      imd->ColorSpace(ims->ColorSpace());
      result = PLinearRescale(*ims, *imd, scaley, scalex);
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
		       Long(ims->Height()*scaley), 
		       Long(ims->Width()*scalex));
      Imx2duc* const imd = (Imx2duc*)objd[0];
      result = PLinearRescale(*ims, *imd, scaley, scalex);
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
		       Long(ims->Height()*scaley), 
		       Long(ims->Width()*scalex));
      Imx2dsl* const imd = (Imx2dsl*)objd[0];
      result = PLinearRescale(*ims, *imd, scaley, scalex);
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
		       Long(ims->Height()*scaley), 
		       Long(ims->Width()*scalex));
      Imx2dsf* const imd = (Imx2dsf*)objd[0];
      result = PLinearRescale(*ims, *imd, scaley, scalex);
   }
   goto end;
 }

if (objs[0]->Type() == Po_Graph2d) {
   Graph2d* const grs = (Graph2d*)objs[0];
   objd[0] = new Graph2d(grs->Size(), Long(grs->Height()*atoi(parv[1])), Long(grs->Width()*atoi(parv[0])));
   Graph2d* const grd = (Graph2d*)objd[0];
   if ( (atof(parv[0])<=0) || (atof(parv[1])<=0)) result = FAILURE;
   else result = PLinearRescale(*grs, *grd, (float)atof(parv[1]), (float)atof(parv[0]));
   goto end;
 }
if (objs[0]->Type() == Po_Img3duc) {
   const float scalex = (float)atof(parv[0]);
   const float scaley = (float)atof(parv[1]);
   const float scalez = (float)atof(parv[2]);
   if (scalex <= 0 || scaley <= 0 || scalez <= 0) {
      result = FAILURE;
   } else {
      Img3duc* const ims = (Img3duc*)objs[0];
      objd[0] = new Img3duc(Long(ims->Depth()*scalez), Long(ims->Height()*scaley), Long(ims->Width()*scalex));
      Img3duc* const imd = (Img3duc*)objd[0];
      result = PLinearRescale(*ims, *imd, scalez, scaley, scalex);
   }
   goto end;
 }
if (objs[0]->Type() == Po_Img3dsl) {
   const float scalex = (float)atof(parv[0]);
   const float scaley = (float)atof(parv[1]);
   const float scalez = (float)atof(parv[2]);
   if (scalex <= 0 || scaley <= 0 || scalez <= 0) {
      result = FAILURE;
   } else {
      Img3dsl* const ims = (Img3dsl*)objs[0];
      objd[0] = new Img3dsl(Long(ims->Depth()*scalez), Long(ims->Height()*scaley), Long(ims->Width()*scalex));
      Img3dsl* const imd = (Img3dsl*)objd[0];
      result = PLinearRescale(*ims, *imd, scalez, scaley, scalex);
   }
   goto end;
 }
if (objs[0]->Type() == Po_Img3dsf) {
   const float scalex = (float)atof(parv[0]);
   const float scaley = (float)atof(parv[1]);
   const float scalez = (float)atof(parv[2]);
   if (scalex <= 0 || scaley <= 0 || scalez <= 0) {
      result = FAILURE;
   } else {
      Img3dsf* const ims = (Img3dsf*)objs[0];
      objd[0] = new Img3dsf(Long(ims->Depth()*scalez), Long(ims->Height()*scaley), Long(ims->Width()*scalex));
      Img3dsf* const imd = (Img3dsf*)objd[0];
      result = PLinearRescale(*ims, *imd, scalez, scaley, scalex);
   }
   goto end;
 }
if (objs[0]->Type() == Po_Imc3duc) {
   const float scalex = (float)atof(parv[0]);
   const float scaley = (float)atof(parv[1]);
   const float scalez = (float)atof(parv[2]);
   if (scalex <= 0 || scaley <= 0 || scalez <= 0) {
      result = FAILURE;
   } else {
      Imc3duc* const ims = (Imc3duc*)objs[0];
      objd[0] = new Imc3duc(Long(ims->Depth()*scalez), Long(ims->Height()*scaley), Long(ims->Width()*scalex));
      Imc3duc* const imd = (Imc3duc*)objd[0];
      result = PLinearRescale(*ims, *imd, scalez, scaley, scalex);
      imd->ColorSpace(ims->ColorSpace());
   }
   goto end;
 }
if (objs[0]->Type() == Po_Imc3dsl) {
   const float scalex = (float)atof(parv[0]);
   const float scaley = (float)atof(parv[1]);
   const float scalez = (float)atof(parv[2]);
   if (scalex <= 0 || scaley <= 0 || scalez <= 0) {
      result = FAILURE;
   } else {
      Imc3dsl* const ims = (Imc3dsl*)objs[0];
      objd[0] = new Imc3dsl(Long(ims->Depth()*scalez), Long(ims->Height()*scaley), Long(ims->Width()*scalex));
      Imc3dsl* const imd = (Imc3dsl*)objd[0];
      result = PLinearRescale(*ims, *imd, scalez, scaley, scalex);
      imd->ColorSpace(ims->ColorSpace());
   }
   goto end;
 }
if (objs[0]->Type() == Po_Imc3dsf) {
   const float scalex = (float)atof(parv[0]);
   const float scaley = (float)atof(parv[1]);
   const float scalez = (float)atof(parv[2]);
   if (scalex <= 0 || scaley <= 0 || scalez <= 0) {
      result = FAILURE;
   } else {
      Imc3dsf* const ims = (Imc3dsf*)objs[0];
      objd[0] = new Imc3dsf(Long(ims->Depth()*scalez), Long(ims->Height()*scaley), Long(ims->Width()*scalex));
      Imc3dsf* const imd = (Imc3dsf*)objd[0];
      result = PLinearRescale(*ims, *imd, scalez, scaley, scalex);
      imd->ColorSpace(ims->ColorSpace());
   }
   goto end;
 }
if (objs[0]->Type() == Po_Imx3duc) {
   const float scalex = (float)atof(parv[0]);
   const float scaley = (float)atof(parv[1]);
   const float scalez = (float)atof(parv[2]);
   if (scalex <= 0 || scaley <= 0 || scalez <= 0) {
      result = FAILURE;
   } else {
      Imx3duc* const ims = (Imx3duc*)objs[0];
      objd[0] = new Imx3duc(ims->Bands(), Long(ims->Depth()*scalez), Long(ims->Height()*scaley), Long(ims->Width()*scalex));
      Imx3duc* const imd = (Imx3duc*)objd[0];
      result = PLinearRescale(*ims, *imd, scalez, scaley, scalex);
   }
   goto end;
 }

if (objs[0]->Type() == Po_Imx3dsl) {
   const float scalex = (float)atof(parv[0]);
   const float scaley = (float)atof(parv[1]);
   const float scalez = (float)atof(parv[2]);
   if (scalex <= 0 || scaley <= 0 || scalez <= 0) {
      result = FAILURE;
   } else {
      Imx3dsl* const ims = (Imx3dsl*)objs[0];
      objd[0] = new Imx3dsl(ims->Bands(), Long(ims->Depth()*scalez), Long(ims->Height()*scaley), Long(ims->Width()*scalex));
      Imx3dsl* const imd = (Imx3dsl*)objd[0];
      result = PLinearRescale(*ims, *imd, scalez, scaley, scalex);
   }
   goto end;
 }

if (objs[0]->Type() == Po_Imx3dsf) {
   const float scalex = (float)atof(parv[0]);
   const float scaley = (float)atof(parv[1]);
   const float scalez = (float)atof(parv[2]);
   if (scalex <= 0 || scaley <= 0 || scalez <= 0) {
      result = FAILURE;
   } else {
      Imx3dsf* const ims = (Imx3dsf*)objs[0];
      objd[0] = new Imx3dsf(ims->Bands(), Long(ims->Depth()*scalez), Long(ims->Height()*scaley), Long(ims->Width()*scalex));
      Imx3dsf* const imd = (Imx3dsf*)objd[0];
      result = PLinearRescale(*ims, *imd, scalez, scaley, scalex);
   }
   goto end;
 }

if (objs[0]->Type() == Po_Graph3d) {
   Graph3d* const grs = (Graph3d*)objs[0];
   objd[0] = new Graph3d(grs->Size(), Long(grs->Depth()*atoi(parv[2])), Long(grs->Height()*atoi(parv[1])), Long(grs->Width()*atoi(parv[0])));
   Graph3d* const grd = (Graph3d*)objd[0];
   if ( (atof(parv[0]) <= 0) || (atof(parv[1]) <= 0)|| (atof(parv[2]) <= 0)) {
      result = FAILURE;
   } else {
      result = PLinearRescale(*grs, *grd, (float)atof(parv[2]), (float)atof(parv[1]), (float)atof(parv[0]));
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
