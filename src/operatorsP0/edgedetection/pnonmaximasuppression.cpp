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
 * @author Régis Clouard - 1997-21-07
 */

#include <pandore.h>
using namespace pandore;

/**
 * @file pnonmaximasuppression.cpp
 * A non-maxima suppression method for edge detection.
 * Suppress each point that one of its neigbours in the direction
 * the gradient is greather than it.
 */


Errc PNonMaximaSuppression( const Img2duc &imsa, const Img2duc &imsd, Img2duc &imd) {
   Point2d p;
   
   imd.Frame(imsa,1); // Border kept the same initial values.
   
   for (p.y = 0; p.y < imsa.Height(); p.y++)
   for (p.x = 0; p.x < imsa.Width(); p.x++)
   {
      int k = (imsd[p]+0) % 8; // Orthogonal directions.
      int l = (imsd[p]+(8/2)) % 8;
      
      if (((imsa.Hold(p+v8[k])) && (imsa[p]<imsa[p+v8[k]])) ||
	  ((imsa.Hold(p+v8[l])) && (imsa[p]<imsa[p+v8[l]])))
	 imd[p] = 0;
      else
	 imd[p] = imsa[p];
   }
   
   return SUCCESS;
}

Errc PNonMaximaSuppression( const Img2dsl &imsa, const Img2duc &imsd, Img2dsl &imd) {
   Point2d p;
   
   imd.Frame(imsa,1); // Border kept the same initial values.
   
   for (p.y = 0; p.y < imsa.Height(); p.y++)
   for (p.x = 0; p.x < imsa.Width(); p.x++)
   {
      int k = (imsd[p]+0) % 8; // Orthogonal directions.
      int l = (imsd[p]+(8/2)) % 8;
      
      if (((imsa.Hold(p+v8[k])) && (imsa[p]<imsa[p+v8[k]])) ||
	  ((imsa.Hold(p+v8[l])) && (imsa[p]<imsa[p+v8[l]])))
	 imd[p] = 0;
      else
	 imd[p] = imsa[p];
   }
   
   return SUCCESS;
}

Errc PNonMaximaSuppression( const Img2dsf &imsa, const Img2duc &imsd, Img2dsf &imd) {
   Point2d p;
   
   imd.Frame(imsa,1); // Border kept the same initial values.
   
   for (p.y = 0; p.y < imsa.Height(); p.y++)
   for (p.x = 0; p.x < imsa.Width(); p.x++)
   {
      int k = (imsd[p]+0) % 8; // Orthogonal directions.
      int l = (imsd[p]+(8/2)) % 8;
      
      if (((imsa.Hold(p+v8[k])) && (imsa[p]<imsa[p+v8[k]])) ||
	  ((imsa.Hold(p+v8[l])) && (imsa[p]<imsa[p+v8[l]])))
	 imd[p] = 0;
      else
	 imd[p] = imsa[p];
   }
   
   return SUCCESS;
}

Errc PNonMaximaSuppression( const Img3duc &imsa, const Img3duc &imsd, Img3duc &imd) {
   Point3d p;
   
   imd.Frame(imsa,1); // Border kept the same initial values.
   
   for (p.z = 0; p.z < imsa.Depth(); p.z++)
   for (p.y = 0; p.y < imsa.Height(); p.y++)
   for (p.x = 0; p.x < imsa.Width(); p.x++)
   {
      int k = (imsd[p]+0) % 26; // Orthogonal directions.
      int l = (imsd[p]+(26/2)) % 26;
      
      if (((imsa.Hold(p+v26[k])) && (imsa[p]<imsa[p+v26[k]])) ||
	  ((imsa.Hold(p+v26[l])) && (imsa[p]<imsa[p+v26[l]])))
	 imd[p] = 0;
      else
	 imd[p] = imsa[p];
   }
   
   return SUCCESS;
}

Errc PNonMaximaSuppression( const Img3dsl &imsa, const Img3duc &imsd, Img3dsl &imd) {
   Point3d p;
   
   imd.Frame(imsa,1); // Border kept the same initial values.
   
   for (p.z = 0; p.z < imsa.Depth(); p.z++)
   for (p.y = 0; p.y < imsa.Height(); p.y++)
   for (p.x = 0; p.x < imsa.Width(); p.x++)
   {
      int k = (imsd[p]+0) % 26; // Orthogonal directions.
      int l = (imsd[p]+(26/2)) % 26;
      
      if (((imsa.Hold(p+v26[k])) && (imsa[p]<imsa[p+v26[k]])) ||
	  ((imsa.Hold(p+v26[l])) && (imsa[p]<imsa[p+v26[l]])))
	 imd[p] = 0;
      else
	 imd[p] = imsa[p];
   }
   
   return SUCCESS;
}

Errc PNonMaximaSuppression( const Img3dsf &imsa, const Img3duc &imsd, Img3dsf &imd) {
   Point3d p;
   
   imd.Frame(imsa,1); // Border kept the same initial values.
   
   for (p.z = 0; p.z < imsa.Depth(); p.z++)
   for (p.y = 0; p.y < imsa.Height(); p.y++)
   for (p.x = 0; p.x < imsa.Width(); p.x++)
   {
      int k = (imsd[p]+0) % 26; // Orthogonal directions.
      int l = (imsd[p]+(26/2)) % 26;
      
      if (((imsa.Hold(p+v26[k])) && (imsa[p]<imsa[p+v26[k]])) ||
	  ((imsa.Hold(p+v26[l])) && (imsa[p]<imsa[p+v26[l]])))
	 imd[p] = 0;
      else
	 imd[p] = imsa[p];
   }
   
   return SUCCESS;
}


#ifdef MAIN
#define USAGE	"usage: %s [-m mask] [im_amp|-] [im_dir|-] [im_out|-]"
#define PARC	0
#define FINC	2
#define FOUTC	1
#define MASK	1 // Masking and unmasking.

int main( int argc, char *argv[] ) {
   Errc result;                // The result code of the execution.
   Pobject* mask;              // The region map.
   Pobject* objin[FINC + 1];   // The input objects.
   Pobject* objs[FINC + 1];    // The source objects masked.
   Pobject* objout[FOUTC + 1]; // The output object.
   Pobject* objd[FOUTC + 1];   // The result object of the execution.
   char* parv[PARC + 1];       // The input parameters.

   ReadArgs(argc, argv, PARC, FINC, FOUTC, &mask, objin, objs, objout, objd, parv, USAGE, MASK); 
   if ((objs[0]->Type() == Po_Img2duc) && (objs[1]->Type() == Po_Img2duc)) {
      Img2duc* const ims1=(Img2duc*)objs[0];
      Img2duc* const ims2=(Img2duc*)objs[1];
      objd[0]=new Img2duc(ims1->Props());
      Img2duc* const imd=(Img2duc*)objd[0];
      
      result=PNonMaximaSuppression(*ims1,*ims2,*imd);
      goto end;
   }
   if ((objs[0]->Type() == Po_Img2dsl) && (objs[1]->Type() == Po_Img2duc)) {
      Img2dsl* const ims1=(Img2dsl*)objs[0];
      Img2duc* const ims2=(Img2duc*)objs[1];
      objd[0]=new Img2dsl(ims1->Props());
      Img2dsl* const imd=(Img2dsl*)objd[0];
      
      result=PNonMaximaSuppression(*ims1,*ims2,*imd);
      goto end;
   }
   if ((objs[0]->Type() == Po_Img2dsf) && (objs[1]->Type() == Po_Img2duc)) {
      Img2dsf* const ims1=(Img2dsf*)objs[0];
      Img2duc* const ims2=(Img2duc*)objs[1];
      objd[0]=new Img2dsf(ims1->Props());
      Img2dsf* const imd=(Img2dsf*)objd[0];
      
      result=PNonMaximaSuppression(*ims1,*ims2,*imd);
      goto end;
   }
   if ((objs[0]->Type() == Po_Img3duc) && (objs[1]->Type() == Po_Img3duc)) {
      Img3duc* const ims1=(Img3duc*)objs[0];
      Img3duc* const ims2=(Img3duc*)objs[1];
      objd[0]=new Img3duc(ims1->Props());
      Img3duc* const imd=(Img3duc*)objd[0];
      
      result=PNonMaximaSuppression(*ims1,*ims2,*imd);
      goto end;
   }
   if ((objs[0]->Type() == Po_Img3dsl) && (objs[1]->Type() == Po_Img3duc)) {
      Img3dsl* const ims1=(Img3dsl*)objs[0];
      Img3duc* const ims2=(Img3duc*)objs[1];
      objd[0]=new Img3dsl(ims1->Props());
      Img3dsl* const imd=(Img3dsl*)objd[0];
      
      result=PNonMaximaSuppression(*ims1,*ims2,*imd);
      goto end;
   }
   if ((objs[0]->Type() == Po_Img3dsf) && (objs[1]->Type() == Po_Img3duc)) {
      Img3dsf* const ims1=(Img3dsf*)objs[0];
      Img3duc* const ims2=(Img3duc*)objs[1];
      objd[0]=new Img3dsf(ims1->Props());
      Img3dsf* const imd=(Img3dsf*)objd[0];
      
      result=PNonMaximaSuppression(*ims1,*ims2,*imd);
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
