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
 * @author Sophie Schupp - 2003-01-08
 */

#include <math.h>
#include <pandore.h>
using namespace pandore;

/**
 * @file pmalikperonafiltering.cpp
 * Operateur de diffusion non lineaire qui lisse les
 * regions tout en preservant les contours.
 */


Errc PMalikPeronaFiltering( const Img2duc &ims, Img2duc &imd, int iterations, int edgethreshold ) {
   const float t=0.2F;
   int k=edgethreshold;
   Point2d p;

   Img2dsf *im1 = new Img2dsf(ims.Size());
   Img2dsf *im2 = new Img2dsf(ims.Size());
   Img2dsf *im3;

   (*im1)=ims;

   // Les iterations.
   for (; iterations>0; iterations--) {
   for (p.y = 1; p.y < ims.Height() - 1; p.y++)
   for (p.x = 1; p.x < ims.Width() - 1; p.x++)
      {
	 float accu=0;
	 for (int v=0; v<4; v++) {
	    float dif=(*im1)[p+v4[v]] - (*im1)[p];
	    accu += exp(-ABS(dif)/k) * dif;
	 }
	 (*im2)[p]=(*im1)[p]+(t* accu);
      }
      
      im3=im2;
      im2=im1;
      im1=im3;
   }
   for (p.y = 0; p.y < imd.Height(); p.y++)
   for (p.x = 0; p.x < imd.Width(); p.x++)
   imd[p]=(Img2duc::ValueType)(*im2)[p];
   
   delete im1;
   delete im2;
   
   return SUCCESS;
}

Errc PMalikPeronaFiltering( const Img2dsl &ims, Img2dsl &imd, int iterations, int edgethreshold ) {
   const float t=0.2F;
   int k=edgethreshold;
   Point2d p;

   Img2dsf *im1 = new Img2dsf(ims.Size());
   Img2dsf *im2 = new Img2dsf(ims.Size());
   Img2dsf *im3;

   (*im1)=ims;

   // Les iterations.
   for (; iterations>0; iterations--) {
   for (p.y = 1; p.y < ims.Height() - 1; p.y++)
   for (p.x = 1; p.x < ims.Width() - 1; p.x++)
      {
	 float accu=0;
	 for (int v=0; v<4; v++) {
	    float dif=(*im1)[p+v4[v]] - (*im1)[p];
	    accu += exp(-ABS(dif)/k) * dif;
	 }
	 (*im2)[p]=(*im1)[p]+(t* accu);
      }
      
      im3=im2;
      im2=im1;
      im1=im3;
   }
   for (p.y = 0; p.y < imd.Height(); p.y++)
   for (p.x = 0; p.x < imd.Width(); p.x++)
   imd[p]=(Img2dsl::ValueType)(*im2)[p];
   
   delete im1;
   delete im2;
   
   return SUCCESS;
}

Errc PMalikPeronaFiltering( const Img2dsf &ims, Img2dsf &imd, int iterations, int edgethreshold ) {
   const float t=0.2F;
   int k=edgethreshold;
   Point2d p;

   Img2dsf *im1 = new Img2dsf(ims.Size());
   Img2dsf *im2 = new Img2dsf(ims.Size());
   Img2dsf *im3;

   (*im1)=ims;

   // Les iterations.
   for (; iterations>0; iterations--) {
   for (p.y = 1; p.y < ims.Height() - 1; p.y++)
   for (p.x = 1; p.x < ims.Width() - 1; p.x++)
      {
	 float accu=0;
	 for (int v=0; v<4; v++) {
	    float dif=(*im1)[p+v4[v]] - (*im1)[p];
	    accu += exp(-ABS(dif)/k) * dif;
	 }
	 (*im2)[p]=(*im1)[p]+(t* accu);
      }
      
      im3=im2;
      im2=im1;
      im1=im3;
   }
   for (p.y = 0; p.y < imd.Height(); p.y++)
   for (p.x = 0; p.x < imd.Width(); p.x++)
   imd[p]=(Img2dsf::ValueType)(*im2)[p];
   
   delete im1;
   delete im2;
   
   return SUCCESS;
}

Errc PMalikPeronaFiltering( const Img3duc &ims, Img3duc &imd, int iterations, int edgethreshold ) {
   const float t=0.2F;
   int k=edgethreshold;
   Point3d p;

   Img3dsf *im1 = new Img3dsf(ims.Size());
   Img3dsf *im2 = new Img3dsf(ims.Size());
   Img3dsf *im3;

   (*im1)=ims;

   // Les iterations.
   for (; iterations>0; iterations--) {
   for (p.z = 1; p.z < ims.Depth() - 1; p.z++)
   for (p.y = 1; p.y < ims.Height() - 1; p.y++)
   for (p.x = 1; p.x < ims.Width() - 1; p.x++)
      {
	 float accu=0;
	 for (int v=0; v<6; v++) {
	    float dif=(*im1)[p+v6[v]] - (*im1)[p];
	    accu += exp(-ABS(dif)/k) * dif;
	 }
	 (*im2)[p]=(*im1)[p]+(t* accu);
      }
      
      im3=im2;
      im2=im1;
      im1=im3;
   }
   for (p.z = 0; p.z < imd.Depth(); p.z++)
   for (p.y = 0; p.y < imd.Height(); p.y++)
   for (p.x = 0; p.x < imd.Width(); p.x++)
   imd[p]=(Img3duc::ValueType)(*im2)[p];
   
   delete im1;
   delete im2;
   
   return SUCCESS;
}

Errc PMalikPeronaFiltering( const Img3dsl &ims, Img3dsl &imd, int iterations, int edgethreshold ) {
   const float t=0.2F;
   int k=edgethreshold;
   Point3d p;

   Img3dsf *im1 = new Img3dsf(ims.Size());
   Img3dsf *im2 = new Img3dsf(ims.Size());
   Img3dsf *im3;

   (*im1)=ims;

   // Les iterations.
   for (; iterations>0; iterations--) {
   for (p.z = 1; p.z < ims.Depth() - 1; p.z++)
   for (p.y = 1; p.y < ims.Height() - 1; p.y++)
   for (p.x = 1; p.x < ims.Width() - 1; p.x++)
      {
	 float accu=0;
	 for (int v=0; v<6; v++) {
	    float dif=(*im1)[p+v6[v]] - (*im1)[p];
	    accu += exp(-ABS(dif)/k) * dif;
	 }
	 (*im2)[p]=(*im1)[p]+(t* accu);
      }
      
      im3=im2;
      im2=im1;
      im1=im3;
   }
   for (p.z = 0; p.z < imd.Depth(); p.z++)
   for (p.y = 0; p.y < imd.Height(); p.y++)
   for (p.x = 0; p.x < imd.Width(); p.x++)
   imd[p]=(Img3dsl::ValueType)(*im2)[p];
   
   delete im1;
   delete im2;
   
   return SUCCESS;
}

Errc PMalikPeronaFiltering( const Img3dsf &ims, Img3dsf &imd, int iterations, int edgethreshold ) {
   const float t=0.2F;
   int k=edgethreshold;
   Point3d p;

   Img3dsf *im1 = new Img3dsf(ims.Size());
   Img3dsf *im2 = new Img3dsf(ims.Size());
   Img3dsf *im3;

   (*im1)=ims;

   // Les iterations.
   for (; iterations>0; iterations--) {
   for (p.z = 1; p.z < ims.Depth() - 1; p.z++)
   for (p.y = 1; p.y < ims.Height() - 1; p.y++)
   for (p.x = 1; p.x < ims.Width() - 1; p.x++)
      {
	 float accu=0;
	 for (int v=0; v<6; v++) {
	    float dif=(*im1)[p+v6[v]] - (*im1)[p];
	    accu += exp(-ABS(dif)/k) * dif;
	 }
	 (*im2)[p]=(*im1)[p]+(t* accu);
      }
      
      im3=im2;
      im2=im1;
      im1=im3;
   }
   for (p.z = 0; p.z < imd.Depth(); p.z++)
   for (p.y = 0; p.y < imd.Height(); p.y++)
   for (p.x = 0; p.x < imd.Width(); p.x++)
   imd[p]=(Img3dsf::ValueType)(*im2)[p];
   
   delete im1;
   delete im2;
   
   return SUCCESS;
}


#ifdef MAIN
#define USAGE	"usage: %s iterations edgethreshold [-m mask] [im_in|-] [im_out|-]"
#define PARC	2
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
   if (objs[0]->Type()==Po_Img2duc) {
      Img2duc* const ims=(Img2duc*)objs[0];
      objd[0]=new Img2duc(ims->Props());
      Img2duc * const imd=(Img2duc*)objd[0];
      
      result=PMalikPeronaFiltering(*ims,*imd, atoi(parv[0]),atoi(parv[1]));
      goto end;
   }
   if (objs[0]->Type()==Po_Img2dsl) {
      Img2dsl* const ims=(Img2dsl*)objs[0];
      objd[0]=new Img2dsl(ims->Props());
      Img2dsl * const imd=(Img2dsl*)objd[0];
      
      result=PMalikPeronaFiltering(*ims,*imd, atoi(parv[0]),atoi(parv[1]));
      goto end;
   }
   if (objs[0]->Type()==Po_Img2dsf) {
      Img2dsf* const ims=(Img2dsf*)objs[0];
      objd[0]=new Img2dsf(ims->Props());
      Img2dsf * const imd=(Img2dsf*)objd[0];
      
      result=PMalikPeronaFiltering(*ims,*imd, atoi(parv[0]),atoi(parv[1]));
      goto end;
   }
   if (objs[0]->Type()==Po_Img3duc) {
      Img3duc* const ims=(Img3duc*)objs[0];
      objd[0]=new Img3duc(ims->Props());
      Img3duc * const imd=(Img3duc*)objd[0];
      
      result=PMalikPeronaFiltering(*ims,*imd, atoi(parv[0]),atoi(parv[1]));
      goto end;
   }
   if (objs[0]->Type()==Po_Img3dsl) {
      Img3dsl* const ims=(Img3dsl*)objs[0];
      objd[0]=new Img3dsl(ims->Props());
      Img3dsl * const imd=(Img3dsl*)objd[0];
      
      result=PMalikPeronaFiltering(*ims,*imd, atoi(parv[0]),atoi(parv[1]));
      goto end;
   }
   if (objs[0]->Type()==Po_Img3dsf) {
      Img3dsf* const ims=(Img3dsf*)objs[0];
      objd[0]=new Img3dsf(ims->Props());
      Img3dsf * const imd=(Img3dsf*)objd[0];
      
      result=PMalikPeronaFiltering(*ims,*imd, atoi(parv[0]),atoi(parv[1]));
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
