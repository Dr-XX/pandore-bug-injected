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
 */

#include <pandore.h>
using namespace pandore;

/**
 * @file pinsertsubimage.cpp
 * Insert subimage into image.
 */













Errc PInsertSubImage( const Img1duc &ims1, const Img1duc &ims2, Img1duc &imd, Long cx ) {
   Point1d p,q;
   
   imd = ims1;
   q.x=cx;
   
   for (p.x = 0; p.x < ims2.Width(); p.x++)
   {
      imd[p+q] = ims2[p];
   }
   
   return SUCCESS;
}

Errc PInsertSubImage( const Img1dsl &ims1, const Img1dsl &ims2, Img1dsl &imd, Long cx ) {
   Point1d p,q;
   
   imd = ims1;
   q.x=cx;
   
   for (p.x = 0; p.x < ims2.Width(); p.x++)
   {
      imd[p+q] = ims2[p];
   }
   
   return SUCCESS;
}

Errc PInsertSubImage( const Img1dsf &ims1, const Img1dsf &ims2, Img1dsf &imd, Long cx ) {
   Point1d p,q;
   
   imd = ims1;
   q.x=cx;
   
   for (p.x = 0; p.x < ims2.Width(); p.x++)
   {
      imd[p+q] = ims2[p];
   }
   
   return SUCCESS;
}

Errc PInsertSubImage( const Img2duc &ims1, const Img2duc &ims2, Img2duc &imd, Long cy, Long cx ) {
   Point2d p,q;
   
   imd = ims1;
   q.x=cx; q.y=cy;
   
   for (p.y = 0; p.y < ims2.Height(); p.y++)
   for (p.x = 0; p.x < ims2.Width(); p.x++)
   {
      imd[p+q] = ims2[p];
   }
   
   return SUCCESS;
}

Errc PInsertSubImage( const Img2dsl &ims1, const Img2dsl &ims2, Img2dsl &imd, Long cy, Long cx ) {
   Point2d p,q;
   
   imd = ims1;
   q.x=cx; q.y=cy;
   
   for (p.y = 0; p.y < ims2.Height(); p.y++)
   for (p.x = 0; p.x < ims2.Width(); p.x++)
   {
      imd[p+q] = ims2[p];
   }
   
   return SUCCESS;
}

Errc PInsertSubImage( const Img2dsf &ims1, const Img2dsf &ims2, Img2dsf &imd, Long cy, Long cx ) {
   Point2d p,q;
   
   imd = ims1;
   q.x=cx; q.y=cy;
   
   for (p.y = 0; p.y < ims2.Height(); p.y++)
   for (p.x = 0; p.x < ims2.Width(); p.x++)
   {
      imd[p+q] = ims2[p];
   }
   
   return SUCCESS;
}

Errc PInsertSubImage( const Img3duc &ims1, const Img3duc &ims2, Img3duc &imd, Long cz, Long cy, Long cx ) {
   Point3d p,q;
   
   imd = ims1;
   q.x=cx; q.y=cy; q.z=cz;
   
   for (p.z = 0; p.z < ims2.Depth(); p.z++)
   for (p.y = 0; p.y < ims2.Height(); p.y++)
   for (p.x = 0; p.x < ims2.Width(); p.x++)
   {
      imd[p+q] = ims2[p];
   }
   
   return SUCCESS;
}

Errc PInsertSubImage( const Img3dsl &ims1, const Img3dsl &ims2, Img3dsl &imd, Long cz, Long cy, Long cx ) {
   Point3d p,q;
   
   imd = ims1;
   q.x=cx; q.y=cy; q.z=cz;
   
   for (p.z = 0; p.z < ims2.Depth(); p.z++)
   for (p.y = 0; p.y < ims2.Height(); p.y++)
   for (p.x = 0; p.x < ims2.Width(); p.x++)
   {
      imd[p+q] = ims2[p];
   }
   
   return SUCCESS;
}

Errc PInsertSubImage( const Img3dsf &ims1, const Img3dsf &ims2, Img3dsf &imd, Long cz, Long cy, Long cx ) {
   Point3d p,q;
   
   imd = ims1;
   q.x=cx; q.y=cy; q.z=cz;
   
   for (p.z = 0; p.z < ims2.Depth(); p.z++)
   for (p.y = 0; p.y < ims2.Height(); p.y++)
   for (p.x = 0; p.x < ims2.Width(); p.x++)
   {
      imd[p+q] = ims2[p];
   }
   
   return SUCCESS;
}


Errc PInsertSubImage( const Imc2duc &ims1, const Imc2duc &ims2, Imc2duc &imd, Long cy, Long cx ) {
   Point2d p,q;
   
   imd = ims1;
   q.x=cx; q.y=cy;

   for (p.y = 0; p.y < ims2.Height(); p.y++)
   for (p.x = 0; p.x < ims2.Width(); p.x++)
   {
      imd.X[p+q] = ims2.X[p];
      imd.Y[p+q] = ims2.Y[p];
      imd.Z[p+q] = ims2.Z[p];
   }

   return SUCCESS;
}

Errc PInsertSubImage( const Imc2dsl &ims1, const Imc2dsl &ims2, Imc2dsl &imd, Long cy, Long cx ) {
   Point2d p,q;
   
   imd = ims1;
   q.x=cx; q.y=cy;

   for (p.y = 0; p.y < ims2.Height(); p.y++)
   for (p.x = 0; p.x < ims2.Width(); p.x++)
   {
      imd.X[p+q] = ims2.X[p];
      imd.Y[p+q] = ims2.Y[p];
      imd.Z[p+q] = ims2.Z[p];
   }

   return SUCCESS;
}

Errc PInsertSubImage( const Imc2dsf &ims1, const Imc2dsf &ims2, Imc2dsf &imd, Long cy, Long cx ) {
   Point2d p,q;
   
   imd = ims1;
   q.x=cx; q.y=cy;

   for (p.y = 0; p.y < ims2.Height(); p.y++)
   for (p.x = 0; p.x < ims2.Width(); p.x++)
   {
      imd.X[p+q] = ims2.X[p];
      imd.Y[p+q] = ims2.Y[p];
      imd.Z[p+q] = ims2.Z[p];
   }

   return SUCCESS;
}

Errc PInsertSubImage( const Imc3duc &ims1, const Imc3duc &ims2, Imc3duc &imd, Long cz, Long cy, Long cx ) {
   Point3d p,q;
   
   imd = ims1;
   q.x=cx; q.y=cy; q.z=cz;
   
   for (p.z = 0; p.z < ims2.Depth(); p.z++)
   for (p.y = 0; p.y < ims2.Height(); p.y++)
   for (p.x = 0; p.x < ims2.Width(); p.x++)
   {
      imd.X[p+q] = ims2.X[p];
      imd.Y[p+q] = ims2.Y[p];
      imd.Z[p+q] = ims2.Z[p];
   }
   
   return SUCCESS;
}

Errc PInsertSubImage( const Imc3dsl &ims1, const Imc3dsl &ims2, Imc3dsl &imd, Long cz, Long cy, Long cx ) {
   Point3d p,q;
   
   imd = ims1;
   q.x=cx; q.y=cy; q.z=cz;
   
   for (p.z = 0; p.z < ims2.Depth(); p.z++)
   for (p.y = 0; p.y < ims2.Height(); p.y++)
   for (p.x = 0; p.x < ims2.Width(); p.x++)
   {
      imd.X[p+q] = ims2.X[p];
      imd.Y[p+q] = ims2.Y[p];
      imd.Z[p+q] = ims2.Z[p];
   }
   
   return SUCCESS;
}

Errc PInsertSubImage( const Imc3dsf &ims1, const Imc3dsf &ims2, Imc3dsf &imd, Long cz, Long cy, Long cx ) {
   Point3d p,q;
   
   imd = ims1;
   q.x=cx; q.y=cy; q.z=cz;
   
   for (p.z = 0; p.z < ims2.Depth(); p.z++)
   for (p.y = 0; p.y < ims2.Height(); p.y++)
   for (p.x = 0; p.x < ims2.Width(); p.x++)
   {
      imd.X[p+q] = ims2.X[p];
      imd.Y[p+q] = ims2.Y[p];
      imd.Z[p+q] = ims2.Z[p];
   }
   
   return SUCCESS;
}


Errc PInsertSubImage( const Imx1duc &ims1, const Imx1duc &ims2, Imx1duc &imd, Long cx ) {
   Point1d p,q;
   
   imd = ims1;
   q.x=cx;
   
   for (p.x = 0; p.x < ims2.Width(); p.x++)
   {
      for (int b=0; b<ims2.Bands();b++) {
	 imd[b][p+q] = ims2[b][p];
      }
   }

   return SUCCESS;
}

Errc PInsertSubImage( const Imx1dsl &ims1, const Imx1dsl &ims2, Imx1dsl &imd, Long cx ) {
   Point1d p,q;
   
   imd = ims1;
   q.x=cx;
   
   for (p.x = 0; p.x < ims2.Width(); p.x++)
   {
      for (int b=0; b<ims2.Bands();b++) {
	 imd[b][p+q] = ims2[b][p];
      }
   }

   return SUCCESS;
}

Errc PInsertSubImage( const Imx1dsf &ims1, const Imx1dsf &ims2, Imx1dsf &imd, Long cx ) {
   Point1d p,q;
   
   imd = ims1;
   q.x=cx;
   
   for (p.x = 0; p.x < ims2.Width(); p.x++)
   {
      for (int b=0; b<ims2.Bands();b++) {
	 imd[b][p+q] = ims2[b][p];
      }
   }

   return SUCCESS;
}

Errc PInsertSubImage( const Imx2duc &ims1, const Imx2duc &ims2, Imx2duc &imd, Long cy, Long cx ) {
   Point2d p,q;
   
   imd = ims1;
   q.x=cx; q.y=cy;
   
   for (p.y = 0; p.y < ims2.Height(); p.y++)
   for (p.x = 0; p.x < ims2.Width(); p.x++)
   {
      for (int b=0; b<ims2.Bands();b++) {
	 imd[b][p+q] = ims2[b][p];
      }
   }

   return SUCCESS;
}

Errc PInsertSubImage( const Imx2dsl &ims1, const Imx2dsl &ims2, Imx2dsl &imd, Long cy, Long cx ) {
   Point2d p,q;
   
   imd = ims1;
   q.x=cx; q.y=cy;
   
   for (p.y = 0; p.y < ims2.Height(); p.y++)
   for (p.x = 0; p.x < ims2.Width(); p.x++)
   {
      for (int b=0; b<ims2.Bands();b++) {
	 imd[b][p+q] = ims2[b][p];
      }
   }

   return SUCCESS;
}

Errc PInsertSubImage( const Imx2dsf &ims1, const Imx2dsf &ims2, Imx2dsf &imd, Long cy, Long cx ) {
   Point2d p,q;
   
   imd = ims1;
   q.x=cx; q.y=cy;
   
   for (p.y = 0; p.y < ims2.Height(); p.y++)
   for (p.x = 0; p.x < ims2.Width(); p.x++)
   {
      for (int b=0; b<ims2.Bands();b++) {
	 imd[b][p+q] = ims2[b][p];
      }
   }

   return SUCCESS;
}

Errc PInsertSubImage( const Imx3duc &ims1, const Imx3duc &ims2, Imx3duc &imd, Long cz, Long cy, Long cx ) {
   Point3d p,q;
   
   imd = ims1;
   q.x=cx; q.y=cy; q.z=cz;
   
   for (p.z = 0; p.z < ims2.Depth(); p.z++)
   for (p.y = 0; p.y < ims2.Height(); p.y++)
   for (p.x = 0; p.x < ims2.Width(); p.x++)
   {
      for (int b=0; b<ims2.Bands();b++) {
	 imd[b][p+q] = ims2[b][p];
      }
   }
   
   return SUCCESS;
}

Errc PInsertSubImage( const Imx3dsl &ims1, const Imx3dsl &ims2, Imx3dsl &imd, Long cz, Long cy, Long cx ) {
   Point3d p,q;
   
   imd = ims1;
   q.x=cx; q.y=cy; q.z=cz;
   
   for (p.z = 0; p.z < ims2.Depth(); p.z++)
   for (p.y = 0; p.y < ims2.Height(); p.y++)
   for (p.x = 0; p.x < ims2.Width(); p.x++)
   {
      for (int b=0; b<ims2.Bands();b++) {
	 imd[b][p+q] = ims2[b][p];
      }
   }
   
   return SUCCESS;
}

Errc PInsertSubImage( const Imx3dsf &ims1, const Imx3dsf &ims2, Imx3dsf &imd, Long cz, Long cy, Long cx ) {
   Point3d p,q;
   
   imd = ims1;
   q.x=cx; q.y=cy; q.z=cz;
   
   for (p.z = 0; p.z < ims2.Depth(); p.z++)
   for (p.y = 0; p.y < ims2.Height(); p.y++)
   for (p.x = 0; p.x < ims2.Width(); p.x++)
   {
      for (int b=0; b<ims2.Bands();b++) {
	 imd[b][p+q] = ims2[b][p];
      }
   }
   
   return SUCCESS;
}


Errc PInsertSubImage( const Reg2d &ims1, const Reg2d &ims2, Reg2d &imd, Long cy, Long cx ) {
   Point2d p,q;

   imd = ims1;
   q.x=cx; q.y=cy;
   
   for (p.y = 0; p.y < ims2.Height(); p.y++)
   for (p.x = 0; p.x < ims2.Width(); p.x++)
   {
      imd[p+q] = ims2[p]+ims1.Labels();
   }
   
   return imd.Labels(ims1.Labels()+ims2.Labels());
}

Errc PInsertSubImage( const Reg3d &ims1, const Reg3d &ims2, Reg3d &imd, Long cz, Long cy, Long cx) {
   Point3d p,q;
   
   imd = ims1;
   q.x=cx; q.y=cy; q.z=cz;
   
   for (p.z = 0; p.z < ims2.Depth(); p.z++)
   for (p.y = 0; p.y < ims2.Height(); p.y++)
   for (p.x = 0; p.x < ims2.Width(); p.x++)
   {
      imd[p+q] = ims2[p]+ims1.Labels();
   }
   
   return imd.Labels(ims1.Labels()+ims2.Labels());
}


#ifdef MAIN
#define	USAGE	"usage: %s x y z [im_in1|-] [im_in2|-] [im_out|-]"
#define	PARC	3
#define	FINC	2
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
   if (objs[0]->Type() == Po_Img1duc && objs[1]->Type() == Po_Img1duc) {
      Img1duc* const ims1=(Img1duc*)objs[0];
      Img1duc* const ims2=(Img1duc*)objs[1];
      if ( atoi(parv[0])+ims2->Width()>ims1->Width()) {
	 result = FAILURE;
      } else {
	 objd[0]=new Img1duc(ims1->Props());
	 Img1duc* const imd=(Img1duc*)objd[0];
	 result=PInsertSubImage(*ims1,*ims2,*imd,atol(parv[0]));
      }
      goto end;
   }

   if (objs[0]->Type() == Po_Img1dsl && objs[1]->Type() == Po_Img1dsl) {
      Img1dsl* const ims1=(Img1dsl*)objs[0];
      Img1dsl* const ims2=(Img1dsl*)objs[1];
      if ( atoi(parv[0])+ims2->Width()>ims1->Width()) {
	 result = FAILURE;
      } else {
	 objd[0]=new Img1dsl(ims1->Props());
	 Img1dsl* const imd=(Img1dsl*)objd[0];
	 result=PInsertSubImage(*ims1,*ims2,*imd,atol(parv[0]));
      }
      goto end;
   }

   if (objs[0]->Type() == Po_Img1dsf && objs[1]->Type() == Po_Img1dsf) {
      Img1dsf* const ims1=(Img1dsf*)objs[0];
      Img1dsf* const ims2=(Img1dsf*)objs[1];
      if ( atoi(parv[0])+ims2->Width()>ims1->Width()) {
	 result = FAILURE;
      } else {
	 objd[0]=new Img1dsf(ims1->Props());
	 Img1dsf* const imd=(Img1dsf*)objd[0];
	 result=PInsertSubImage(*ims1,*ims2,*imd,atol(parv[0]));
      }
      goto end;
   }

   if (objs[0]->Type() == Po_Img2duc && objs[1]->Type() == Po_Img2duc) {
      Img2duc* const ims1=(Img2duc*)objs[0];
      Img2duc* const ims2=(Img2duc*)objs[1];
      if ( (atoi(parv[0])+ims2->Width()>ims1->Width()) || (atoi(parv[1])+ims2->Height()>ims1->Height())) {
	 result = FAILURE;
      } else {
	 objd[0]=new Img2duc(ims1->Props());
	 Img2duc* const imd=(Img2duc*)objd[0];
	 result=PInsertSubImage(*ims1,*ims2,*imd,atol(parv[1]),atol(parv[0]));
      }
      goto end;
   }

   if (objs[0]->Type() == Po_Img2dsl && objs[1]->Type() == Po_Img2dsl) {
      Img2dsl* const ims1=(Img2dsl*)objs[0];
      Img2dsl* const ims2=(Img2dsl*)objs[1];
      if ( (atoi(parv[0])+ims2->Width()>ims1->Width()) || (atoi(parv[1])+ims2->Height()>ims1->Height())) {
	 result = FAILURE;
      } else {
	 objd[0]=new Img2dsl(ims1->Props());
	 Img2dsl* const imd=(Img2dsl*)objd[0];
	 result=PInsertSubImage(*ims1,*ims2,*imd,atol(parv[1]),atol(parv[0]));
      }
      goto end;
   }

   if (objs[0]->Type() == Po_Img2dsf && objs[1]->Type() == Po_Img2dsf) {
      Img2dsf* const ims1=(Img2dsf*)objs[0];
      Img2dsf* const ims2=(Img2dsf*)objs[1];
      if ( (atoi(parv[0])+ims2->Width()>ims1->Width()) || (atoi(parv[1])+ims2->Height()>ims1->Height())) {
	 result = FAILURE;
      } else {
	 objd[0]=new Img2dsf(ims1->Props());
	 Img2dsf* const imd=(Img2dsf*)objd[0];
	 result=PInsertSubImage(*ims1,*ims2,*imd,atol(parv[1]),atol(parv[0]));
      }
      goto end;
   }

   if (objs[0]->Type() == Po_Img3duc && objs[1]->Type() == Po_Img3duc) {
      Img3duc* const ims1=(Img3duc*)objs[0];
      Img3duc* const ims2=(Img3duc*)objs[1];
      if ((atoi(parv[0])+ims2->Width()>ims1->Width()) || (atoi(parv[1])+ims2->Height()>ims1->Height()) || (atoi(parv[2])+ims2->Depth()>ims1->Depth())) {
	 result = FAILURE;
      } else {
	 objd[0]=new Img3duc(ims1->Props());
	 Img3duc* const imd=(Img3duc*)objd[0];
	 result=PInsertSubImage(*ims1,*ims2,*imd,atol(parv[2]),atol(parv[1]),atol(parv[0]));
      }
      goto end;
   }

   if (objs[0]->Type() == Po_Img3dsl && objs[1]->Type() == Po_Img3dsl) {
      Img3dsl* const ims1=(Img3dsl*)objs[0];
      Img3dsl* const ims2=(Img3dsl*)objs[1];
      if ((atoi(parv[0])+ims2->Width()>ims1->Width()) || (atoi(parv[1])+ims2->Height()>ims1->Height()) || (atoi(parv[2])+ims2->Depth()>ims1->Depth())) {
	 result = FAILURE;
      } else {
	 objd[0]=new Img3dsl(ims1->Props());
	 Img3dsl* const imd=(Img3dsl*)objd[0];
	 result=PInsertSubImage(*ims1,*ims2,*imd,atol(parv[2]),atol(parv[1]),atol(parv[0]));
      }
      goto end;
   }

   if (objs[0]->Type() == Po_Img3dsf && objs[1]->Type() == Po_Img3dsf) {
      Img3dsf* const ims1=(Img3dsf*)objs[0];
      Img3dsf* const ims2=(Img3dsf*)objs[1];
      if ((atoi(parv[0])+ims2->Width()>ims1->Width()) || (atoi(parv[1])+ims2->Height()>ims1->Height()) || (atoi(parv[2])+ims2->Depth()>ims1->Depth())) {
	 result = FAILURE;
      } else {
	 objd[0]=new Img3dsf(ims1->Props());
	 Img3dsf* const imd=(Img3dsf*)objd[0];
	 result=PInsertSubImage(*ims1,*ims2,*imd,atol(parv[2]),atol(parv[1]),atol(parv[0]));
      }
      goto end;
   }

   if (objs[0]->Type() == Po_Imc2duc && objs[1]->Type() == Po_Imc2duc) {
      Imc2duc* const ims1=(Imc2duc*)objs[0];
      Imc2duc* const ims2=(Imc2duc*)objs[1];
      if ( (atoi(parv[0])+ims2->Width()>ims1->Width()) || (atoi(parv[1])+ims2->Height()>ims1->Height())) {
	 result = FAILURE;
      } else {
	 objd[0]=new Imc2duc(ims1->Props());
	 Imc2duc* const imd=(Imc2duc*)objd[0];
	 result=PInsertSubImage(*ims1,*ims2,*imd,atol(parv[1]),atol(parv[0]));
      }
      goto end;
   }

   if (objs[0]->Type() == Po_Imc2dsl && objs[1]->Type() == Po_Imc2dsl) {
      Imc2dsl* const ims1=(Imc2dsl*)objs[0];
      Imc2dsl* const ims2=(Imc2dsl*)objs[1];
      if ( (atoi(parv[0])+ims2->Width()>ims1->Width()) || (atoi(parv[1])+ims2->Height()>ims1->Height())) {
	 result = FAILURE;
      } else {
	 objd[0]=new Imc2dsl(ims1->Props());
	 Imc2dsl* const imd=(Imc2dsl*)objd[0];
	 result=PInsertSubImage(*ims1,*ims2,*imd,atol(parv[1]),atol(parv[0]));
      }
      goto end;
   }

   if (objs[0]->Type() == Po_Imc2dsf && objs[1]->Type() == Po_Imc2dsf) {
      Imc2dsf* const ims1=(Imc2dsf*)objs[0];
      Imc2dsf* const ims2=(Imc2dsf*)objs[1];
      if ( (atoi(parv[0])+ims2->Width()>ims1->Width()) || (atoi(parv[1])+ims2->Height()>ims1->Height())) {
	 result = FAILURE;
      } else {
	 objd[0]=new Imc2dsf(ims1->Props());
	 Imc2dsf* const imd=(Imc2dsf*)objd[0];
	 result=PInsertSubImage(*ims1,*ims2,*imd,atol(parv[1]),atol(parv[0]));
      }
      goto end;
   }

   if (objs[0]->Type() == Po_Imc3duc && objs[1]->Type() == Po_Imc3duc) {
      Imc3duc* const ims1=(Imc3duc*)objs[0];
      Imc3duc* const ims2=(Imc3duc*)objs[1];
      if ((atoi(parv[0])+ims2->Width()>ims1->Width()) || (atoi(parv[1])+ims2->Height()>ims1->Height()) || (atoi(parv[2])+ims2->Depth()>ims1->Depth())) {
	 result = FAILURE;
      } else {
	 objd[0]=new Imc3duc(ims1->Props());
	 Imc3duc* const imd=(Imc3duc*)objd[0];
	 result=PInsertSubImage(*ims1,*ims2,*imd,atol(parv[2]),atol(parv[1]),atol(parv[0]));
      }
      goto end;
   }

   if (objs[0]->Type() == Po_Imc3dsl && objs[1]->Type() == Po_Imc3dsl) {
      Imc3dsl* const ims1=(Imc3dsl*)objs[0];
      Imc3dsl* const ims2=(Imc3dsl*)objs[1];
      if ((atoi(parv[0])+ims2->Width()>ims1->Width()) || (atoi(parv[1])+ims2->Height()>ims1->Height()) || (atoi(parv[2])+ims2->Depth()>ims1->Depth())) {
	 result = FAILURE;
      } else {
	 objd[0]=new Imc3dsl(ims1->Props());
	 Imc3dsl* const imd=(Imc3dsl*)objd[0];
	 result=PInsertSubImage(*ims1,*ims2,*imd,atol(parv[2]),atol(parv[1]),atol(parv[0]));
      }
      goto end;
   }

   if (objs[0]->Type() == Po_Imc3dsf && objs[1]->Type() == Po_Imc3dsf) {
      Imc3dsf* const ims1=(Imc3dsf*)objs[0];
      Imc3dsf* const ims2=(Imc3dsf*)objs[1];
      if ((atoi(parv[0])+ims2->Width()>ims1->Width()) || (atoi(parv[1])+ims2->Height()>ims1->Height()) || (atoi(parv[2])+ims2->Depth()>ims1->Depth())) {
	 result = FAILURE;
      } else {
	 objd[0]=new Imc3dsf(ims1->Props());
	 Imc3dsf* const imd=(Imc3dsf*)objd[0];
	 result=PInsertSubImage(*ims1,*ims2,*imd,atol(parv[2]),atol(parv[1]),atol(parv[0]));
      }
      goto end;
   }

   if (objs[0]->Type() == Po_Imx1duc && objs[1]->Type() == Po_Imx1duc) {
      Imx1duc* const ims1=(Imx1duc*)objs[0];
      Imx1duc* const ims2=(Imx1duc*)objs[1];
      if ( atoi(parv[0])+ims2->Width()>ims1->Width() ) {
	 result = FAILURE;
      } else {
	 objd[0]=new Imx1duc(ims1->Props());
	 Imx1duc* const imd=(Imx1duc*)objd[0];
	 result=PInsertSubImage(*ims1,*ims2,*imd,atol(parv[0]));
      }
      goto end;
   }

   if (objs[0]->Type() == Po_Imx1dsl && objs[1]->Type() == Po_Imx1dsl) {
      Imx1dsl* const ims1=(Imx1dsl*)objs[0];
      Imx1dsl* const ims2=(Imx1dsl*)objs[1];
      if ( atoi(parv[0])+ims2->Width()>ims1->Width() ) {
	 result = FAILURE;
      } else {
	 objd[0]=new Imx1dsl(ims1->Props());
	 Imx1dsl* const imd=(Imx1dsl*)objd[0];
	 result=PInsertSubImage(*ims1,*ims2,*imd,atol(parv[0]));
      }
      goto end;
   }

   if (objs[0]->Type() == Po_Imx1dsf && objs[1]->Type() == Po_Imx1dsf) {
      Imx1dsf* const ims1=(Imx1dsf*)objs[0];
      Imx1dsf* const ims2=(Imx1dsf*)objs[1];
      if ( atoi(parv[0])+ims2->Width()>ims1->Width() ) {
	 result = FAILURE;
      } else {
	 objd[0]=new Imx1dsf(ims1->Props());
	 Imx1dsf* const imd=(Imx1dsf*)objd[0];
	 result=PInsertSubImage(*ims1,*ims2,*imd,atol(parv[0]));
      }
      goto end;
   }

   if (objs[0]->Type() == Po_Imx2duc && objs[1]->Type() == Po_Imx2duc) {
      Imx2duc* const ims1=(Imx2duc*)objs[0];
      Imx2duc* const ims2=(Imx2duc*)objs[1];
      if ( (atoi(parv[0])+ims2->Width()>ims1->Width()) || (atoi(parv[1])+ims2->Height()>ims1->Height())) {
	 result = FAILURE;
      } else {
	 objd[0]=new Imx2duc(ims1->Props());
	 Imx2duc* const imd=(Imx2duc*)objd[0];
	 result=PInsertSubImage(*ims1,*ims2,*imd,atol(parv[1]),atol(parv[0]));
      }
      goto end;
   }

   if (objs[0]->Type() == Po_Imx2dsl && objs[1]->Type() == Po_Imx2dsl) {
      Imx2dsl* const ims1=(Imx2dsl*)objs[0];
      Imx2dsl* const ims2=(Imx2dsl*)objs[1];
      if ( (atoi(parv[0])+ims2->Width()>ims1->Width()) || (atoi(parv[1])+ims2->Height()>ims1->Height())) {
	 result = FAILURE;
      } else {
	 objd[0]=new Imx2dsl(ims1->Props());
	 Imx2dsl* const imd=(Imx2dsl*)objd[0];
	 result=PInsertSubImage(*ims1,*ims2,*imd,atol(parv[1]),atol(parv[0]));
      }
      goto end;
   }

   if (objs[0]->Type() == Po_Imx2dsf && objs[1]->Type() == Po_Imx2dsf) {
      Imx2dsf* const ims1=(Imx2dsf*)objs[0];
      Imx2dsf* const ims2=(Imx2dsf*)objs[1];
      if ( (atoi(parv[0])+ims2->Width()>ims1->Width()) || (atoi(parv[1])+ims2->Height()>ims1->Height())) {
	 result = FAILURE;
      } else {
	 objd[0]=new Imx2dsf(ims1->Props());
	 Imx2dsf* const imd=(Imx2dsf*)objd[0];
	 result=PInsertSubImage(*ims1,*ims2,*imd,atol(parv[1]),atol(parv[0]));
      }
      goto end;
   }

   if (objs[0]->Type() == Po_Imx3duc && objs[1]->Type() == Po_Imx3duc) {
      Imx3duc* const ims1=(Imx3duc*)objs[0];
      Imx3duc* const ims2=(Imx3duc*)objs[1];
      if ((atoi(parv[0])+ims2->Width()>ims1->Width()) || (atoi(parv[1])+ims2->Height()>ims1->Height()) || (atoi(parv[2])+ims2->Depth()>ims1->Depth())) {
	 result = FAILURE;
      } else {
	 objd[0]=new Imx3duc(ims1->Props());
	 Imx3duc* const imd=(Imx3duc*)objd[0];
	 result=PInsertSubImage(*ims1,*ims2,*imd,atol(parv[2]),atol(parv[1]),atol(parv[0]));
      }
      goto end;
   }

   if (objs[0]->Type() == Po_Imx3dsl && objs[1]->Type() == Po_Imx3dsl) {
      Imx3dsl* const ims1=(Imx3dsl*)objs[0];
      Imx3dsl* const ims2=(Imx3dsl*)objs[1];
      if ((atoi(parv[0])+ims2->Width()>ims1->Width()) || (atoi(parv[1])+ims2->Height()>ims1->Height()) || (atoi(parv[2])+ims2->Depth()>ims1->Depth())) {
	 result = FAILURE;
      } else {
	 objd[0]=new Imx3dsl(ims1->Props());
	 Imx3dsl* const imd=(Imx3dsl*)objd[0];
	 result=PInsertSubImage(*ims1,*ims2,*imd,atol(parv[2]),atol(parv[1]),atol(parv[0]));
      }
      goto end;
   }

   if (objs[0]->Type() == Po_Imx3dsf && objs[1]->Type() == Po_Imx3dsf) {
      Imx3dsf* const ims1=(Imx3dsf*)objs[0];
      Imx3dsf* const ims2=(Imx3dsf*)objs[1];
      if ((atoi(parv[0])+ims2->Width()>ims1->Width()) || (atoi(parv[1])+ims2->Height()>ims1->Height()) || (atoi(parv[2])+ims2->Depth()>ims1->Depth())) {
	 result = FAILURE;
      } else {
	 objd[0]=new Imx3dsf(ims1->Props());
	 Imx3dsf* const imd=(Imx3dsf*)objd[0];
	 result=PInsertSubImage(*ims1,*ims2,*imd,atol(parv[2]),atol(parv[1]),atol(parv[0]));
      }
      goto end;
   }

   if (objs[0]->Type() == Po_Reg2d && objs[1]->Type() == Po_Reg2d) {
      Reg2d* const ims1=(Reg2d*)objs[0];
      Reg2d* const ims2=(Reg2d*)objs[1];
      if ( (atoi(parv[0])+ims2->Width()>ims1->Width()) || (atoi(parv[1])+ims2->Height()>ims1->Height())) {
	 result = FAILURE;
      } else {
	 objd[0]=new Reg2d(ims1->Props());
	 Reg2d* const imd=(Reg2d*)objd[0];
	 result=PInsertSubImage(*ims1,*ims2,*imd,atol(parv[1]),atol(parv[0]));
      }
      goto end;
   }

   if (objs[0]->Type() == Po_Reg3d && objs[1]->Type() == Po_Reg3d) {
      Reg3d* const ims1=(Reg3d*)objs[0];
      Reg3d* const ims2=(Reg3d*)objs[1];
      if ((atoi(parv[0])+ims2->Width()>ims1->Width()) || (atoi(parv[1])+ims2->Height()>ims1->Height()) || (atoi(parv[2])+ims2->Depth()>ims1->Depth())) {
	 result = FAILURE;
      } else {
	 objd[0]=new Reg3d(ims1->Props());
	 Reg3d* const imd=(Reg3d*)objd[0];
	 result=PInsertSubImage(*ims1,*ims2,*imd,atol(parv[2]),atol(parv[1]),atol(parv[0]));
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
