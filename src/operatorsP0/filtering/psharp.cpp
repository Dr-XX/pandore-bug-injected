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
 * @author Régis Clouard - 1997-07-19
 * @author Régis Clouard - 2001-05-09 (version 3.00)
 * @author Régis Clouard - 2006-01-13 (new version)
 */

#include <pandore.h>
using namespace pandore;

/**
 * @file psharp.cpp
 * @brief Contrast sharp using mask:
 * sharp = image + k*laplacian image.
 *
 * Laplacian Masks:	 0  -1   0	    -1  -1  -1
 *		        -1   4  -1 pour 4V  -1   8 -1 pour 8V
 *		         0  -1   0	    -1  -1  -1
 */


Errc PSharp( const Img1duc &ims, Img1duc &imd, int connexity, float k ) {
   Float gx;
   Point1d p;
   
   imd.Frame(ims, 1);
   
   if (connexity == 2) {
   for (p.x = 1; p.x < ims.Width() - 1; p.x++)
      {
	 gx = 0.0F;
	 for (int v = 0; v < 2; v++) {
	    gx += (Float)ims[p+v2[v]];
	 }
 	 gx = (float)ims[p] + (k / 2) * (2 * ims[p] - gx);
	 if (gx > Limits<Img1duc::ValueType>::max()) {
	    imd[p]=Limits<Img1duc::ValueType>::max();
	 } else if (gx < 0) {
	    imd[p] = 0;
	 } else {
	    imd[p] = (Img1duc::ValueType)gx;
	 }
      }
   } else {
   for (p.x = 1; p.x < ims.Width() - 1; p.x++)
      {
	 gx = 0.0F;
	 for (int v = 0; v < 2; v++) {
	    gx += (Float)ims[p + v2[v]];
	 }
 	 gx = (float)ims[p] + (k / 2) * (2 * ims[p] - gx);
	 if (gx > Limits<Img1duc::ValueType>::max()) {
	    imd[p] = Limits<Img1duc::ValueType>::max();
	 } else if (gx < Limits<Img1duc::ValueType>::min()) {
	    imd[p] = Limits<Img1duc::ValueType>::min();
	 } else {
	    imd[p] = (Img1duc::ValueType)gx;
	 }
      }
   }
   
   return SUCCESS;
}
Errc PSharp( const Img1dsl &ims, Img1dsl &imd, int connexity, float k ) {
   Float gx;
   Point1d p;
   
   imd.Frame(ims, 1);
   
   if (connexity == 2) {
   for (p.x = 1; p.x < ims.Width() - 1; p.x++)
      {
	 gx = 0.0F;
	 for (int v = 0; v < 2; v++) {
	    gx += (Float)ims[p+v2[v]];
	 }
 	 gx = (float)ims[p] + (k / 2) * (2 * ims[p] - gx);
	 if (gx > Limits<Img1dsl::ValueType>::max()) {
	    imd[p]=Limits<Img1dsl::ValueType>::max();
	 } else if (gx < 0) {
	    imd[p] = 0;
	 } else {
	    imd[p] = (Img1dsl::ValueType)gx;
	 }
      }
   } else {
   for (p.x = 1; p.x < ims.Width() - 1; p.x++)
      {
	 gx = 0.0F;
	 for (int v = 0; v < 2; v++) {
	    gx += (Float)ims[p + v2[v]];
	 }
 	 gx = (float)ims[p] + (k / 2) * (2 * ims[p] - gx);
	 if (gx > Limits<Img1dsl::ValueType>::max()) {
	    imd[p] = Limits<Img1dsl::ValueType>::max();
	 } else if (gx < Limits<Img1dsl::ValueType>::min()) {
	    imd[p] = Limits<Img1dsl::ValueType>::min();
	 } else {
	    imd[p] = (Img1dsl::ValueType)gx;
	 }
      }
   }
   
   return SUCCESS;
}
Errc PSharp( const Img1dsf &ims, Img1dsf &imd, int connexity, float k ) {
   Float gx;
   Point1d p;
   
   imd.Frame(ims, 1);
   
   if (connexity == 2) {
   for (p.x = 1; p.x < ims.Width() - 1; p.x++)
      {
	 gx = 0.0F;
	 for (int v = 0; v < 2; v++) {
	    gx += (Float)ims[p+v2[v]];
	 }
 	 gx = (float)ims[p] + (k / 2) * (2 * ims[p] - gx);
	 if (gx > Limits<Img1dsf::ValueType>::max()) {
	    imd[p]=Limits<Img1dsf::ValueType>::max();
	 } else if (gx < 0) {
	    imd[p] = 0;
	 } else {
	    imd[p] = (Img1dsf::ValueType)gx;
	 }
      }
   } else {
   for (p.x = 1; p.x < ims.Width() - 1; p.x++)
      {
	 gx = 0.0F;
	 for (int v = 0; v < 2; v++) {
	    gx += (Float)ims[p + v2[v]];
	 }
 	 gx = (float)ims[p] + (k / 2) * (2 * ims[p] - gx);
	 if (gx > Limits<Img1dsf::ValueType>::max()) {
	    imd[p] = Limits<Img1dsf::ValueType>::max();
	 } else if (gx < Limits<Img1dsf::ValueType>::min()) {
	    imd[p] = Limits<Img1dsf::ValueType>::min();
	 } else {
	    imd[p] = (Img1dsf::ValueType)gx;
	 }
      }
   }
   
   return SUCCESS;
}
Errc PSharp( const Img2duc &ims, Img2duc &imd, int connexity, float k ) {
   Float gx;
   Point2d p;
   
   imd.Frame(ims, 1);
   
   if (connexity == 4) {
   for (p.y = 1; p.y < ims.Height() - 1; p.y++)
   for (p.x = 1; p.x < ims.Width() - 1; p.x++)
      {
	 gx = 0.0F;
	 for (int v = 0; v < 4; v++) {
	    gx += (Float)ims[p+v4[v]];
	 }
 	 gx = (float)ims[p] + (k / 4) * (4 * ims[p] - gx);
	 if (gx > Limits<Img2duc::ValueType>::max()) {
	    imd[p]=Limits<Img2duc::ValueType>::max();
	 } else if (gx < 0) {
	    imd[p] = 0;
	 } else {
	    imd[p] = (Img2duc::ValueType)gx;
	 }
      }
   } else {
   for (p.y = 1; p.y < ims.Height() - 1; p.y++)
   for (p.x = 1; p.x < ims.Width() - 1; p.x++)
      {
	 gx = 0.0F;
	 for (int v = 0; v < 8; v++) {
	    gx += (Float)ims[p + v8[v]];
	 }
 	 gx = (float)ims[p] + (k / 8) * (8 * ims[p] - gx);
	 if (gx > Limits<Img2duc::ValueType>::max()) {
	    imd[p] = Limits<Img2duc::ValueType>::max();
	 } else if (gx < Limits<Img2duc::ValueType>::min()) {
	    imd[p] = Limits<Img2duc::ValueType>::min();
	 } else {
	    imd[p] = (Img2duc::ValueType)gx;
	 }
      }
   }
   
   return SUCCESS;
}
Errc PSharp( const Img2dsl &ims, Img2dsl &imd, int connexity, float k ) {
   Float gx;
   Point2d p;
   
   imd.Frame(ims, 1);
   
   if (connexity == 4) {
   for (p.y = 1; p.y < ims.Height() - 1; p.y++)
   for (p.x = 1; p.x < ims.Width() - 1; p.x++)
      {
	 gx = 0.0F;
	 for (int v = 0; v < 4; v++) {
	    gx += (Float)ims[p+v4[v]];
	 }
 	 gx = (float)ims[p] + (k / 4) * (4 * ims[p] - gx);
	 if (gx > Limits<Img2dsl::ValueType>::max()) {
	    imd[p]=Limits<Img2dsl::ValueType>::max();
	 } else if (gx < 0) {
	    imd[p] = 0;
	 } else {
	    imd[p] = (Img2dsl::ValueType)gx;
	 }
      }
   } else {
   for (p.y = 1; p.y < ims.Height() - 1; p.y++)
   for (p.x = 1; p.x < ims.Width() - 1; p.x++)
      {
	 gx = 0.0F;
	 for (int v = 0; v < 8; v++) {
	    gx += (Float)ims[p + v8[v]];
	 }
 	 gx = (float)ims[p] + (k / 8) * (8 * ims[p] - gx);
	 if (gx > Limits<Img2dsl::ValueType>::max()) {
	    imd[p] = Limits<Img2dsl::ValueType>::max();
	 } else if (gx < Limits<Img2dsl::ValueType>::min()) {
	    imd[p] = Limits<Img2dsl::ValueType>::min();
	 } else {
	    imd[p] = (Img2dsl::ValueType)gx;
	 }
      }
   }
   
   return SUCCESS;
}
Errc PSharp( const Img2dsf &ims, Img2dsf &imd, int connexity, float k ) {
   Float gx;
   Point2d p;
   
   imd.Frame(ims, 1);
   
   if (connexity == 4) {
   for (p.y = 1; p.y < ims.Height() - 1; p.y++)
   for (p.x = 1; p.x < ims.Width() - 1; p.x++)
      {
	 gx = 0.0F;
	 for (int v = 0; v < 4; v++) {
	    gx += (Float)ims[p+v4[v]];
	 }
 	 gx = (float)ims[p] + (k / 4) * (4 * ims[p] - gx);
	 if (gx > Limits<Img2dsf::ValueType>::max()) {
	    imd[p]=Limits<Img2dsf::ValueType>::max();
	 } else if (gx < 0) {
	    imd[p] = 0;
	 } else {
	    imd[p] = (Img2dsf::ValueType)gx;
	 }
      }
   } else {
   for (p.y = 1; p.y < ims.Height() - 1; p.y++)
   for (p.x = 1; p.x < ims.Width() - 1; p.x++)
      {
	 gx = 0.0F;
	 for (int v = 0; v < 8; v++) {
	    gx += (Float)ims[p + v8[v]];
	 }
 	 gx = (float)ims[p] + (k / 8) * (8 * ims[p] - gx);
	 if (gx > Limits<Img2dsf::ValueType>::max()) {
	    imd[p] = Limits<Img2dsf::ValueType>::max();
	 } else if (gx < Limits<Img2dsf::ValueType>::min()) {
	    imd[p] = Limits<Img2dsf::ValueType>::min();
	 } else {
	    imd[p] = (Img2dsf::ValueType)gx;
	 }
      }
   }
   
   return SUCCESS;
}
Errc PSharp( const Img3duc &ims, Img3duc &imd, int connexity, float k ) {
   Float gx;
   Point3d p;
   
   imd.Frame(ims, 1);
   
   if (connexity == 6) {
   for (p.z = 1; p.z < ims.Depth() - 1; p.z++)
   for (p.y = 1; p.y < ims.Height() - 1; p.y++)
   for (p.x = 1; p.x < ims.Width() - 1; p.x++)
      {
	 gx = 0.0F;
	 for (int v = 0; v < 6; v++) {
	    gx += (Float)ims[p+v6[v]];
	 }
 	 gx = (float)ims[p] + (k / 6) * (6 * ims[p] - gx);
	 if (gx > Limits<Img3duc::ValueType>::max()) {
	    imd[p]=Limits<Img3duc::ValueType>::max();
	 } else if (gx < 0) {
	    imd[p] = 0;
	 } else {
	    imd[p] = (Img3duc::ValueType)gx;
	 }
      }
   } else {
   for (p.z = 1; p.z < ims.Depth() - 1; p.z++)
   for (p.y = 1; p.y < ims.Height() - 1; p.y++)
   for (p.x = 1; p.x < ims.Width() - 1; p.x++)
      {
	 gx = 0.0F;
	 for (int v = 0; v < 26; v++) {
	    gx += (Float)ims[p + v26[v]];
	 }
 	 gx = (float)ims[p] + (k / 26) * (26 * ims[p] - gx);
	 if (gx > Limits<Img3duc::ValueType>::max()) {
	    imd[p] = Limits<Img3duc::ValueType>::max();
	 } else if (gx < Limits<Img3duc::ValueType>::min()) {
	    imd[p] = Limits<Img3duc::ValueType>::min();
	 } else {
	    imd[p] = (Img3duc::ValueType)gx;
	 }
      }
   }
   
   return SUCCESS;
}
Errc PSharp( const Img3dsl &ims, Img3dsl &imd, int connexity, float k ) {
   Float gx;
   Point3d p;
   
   imd.Frame(ims, 1);
   
   if (connexity == 6) {
   for (p.z = 1; p.z < ims.Depth() - 1; p.z++)
   for (p.y = 1; p.y < ims.Height() - 1; p.y++)
   for (p.x = 1; p.x < ims.Width() - 1; p.x++)
      {
	 gx = 0.0F;
	 for (int v = 0; v < 6; v++) {
	    gx += (Float)ims[p+v6[v]];
	 }
 	 gx = (float)ims[p] + (k / 6) * (6 * ims[p] - gx);
	 if (gx > Limits<Img3dsl::ValueType>::max()) {
	    imd[p]=Limits<Img3dsl::ValueType>::max();
	 } else if (gx < 0) {
	    imd[p] = 0;
	 } else {
	    imd[p] = (Img3dsl::ValueType)gx;
	 }
      }
   } else {
   for (p.z = 1; p.z < ims.Depth() - 1; p.z++)
   for (p.y = 1; p.y < ims.Height() - 1; p.y++)
   for (p.x = 1; p.x < ims.Width() - 1; p.x++)
      {
	 gx = 0.0F;
	 for (int v = 0; v < 26; v++) {
	    gx += (Float)ims[p + v26[v]];
	 }
 	 gx = (float)ims[p] + (k / 26) * (26 * ims[p] - gx);
	 if (gx > Limits<Img3dsl::ValueType>::max()) {
	    imd[p] = Limits<Img3dsl::ValueType>::max();
	 } else if (gx < Limits<Img3dsl::ValueType>::min()) {
	    imd[p] = Limits<Img3dsl::ValueType>::min();
	 } else {
	    imd[p] = (Img3dsl::ValueType)gx;
	 }
      }
   }
   
   return SUCCESS;
}
Errc PSharp( const Img3dsf &ims, Img3dsf &imd, int connexity, float k ) {
   Float gx;
   Point3d p;
   
   imd.Frame(ims, 1);
   
   if (connexity == 6) {
   for (p.z = 1; p.z < ims.Depth() - 1; p.z++)
   for (p.y = 1; p.y < ims.Height() - 1; p.y++)
   for (p.x = 1; p.x < ims.Width() - 1; p.x++)
      {
	 gx = 0.0F;
	 for (int v = 0; v < 6; v++) {
	    gx += (Float)ims[p+v6[v]];
	 }
 	 gx = (float)ims[p] + (k / 6) * (6 * ims[p] - gx);
	 if (gx > Limits<Img3dsf::ValueType>::max()) {
	    imd[p]=Limits<Img3dsf::ValueType>::max();
	 } else if (gx < 0) {
	    imd[p] = 0;
	 } else {
	    imd[p] = (Img3dsf::ValueType)gx;
	 }
      }
   } else {
   for (p.z = 1; p.z < ims.Depth() - 1; p.z++)
   for (p.y = 1; p.y < ims.Height() - 1; p.y++)
   for (p.x = 1; p.x < ims.Width() - 1; p.x++)
      {
	 gx = 0.0F;
	 for (int v = 0; v < 26; v++) {
	    gx += (Float)ims[p + v26[v]];
	 }
 	 gx = (float)ims[p] + (k / 26) * (26 * ims[p] - gx);
	 if (gx > Limits<Img3dsf::ValueType>::max()) {
	    imd[p] = Limits<Img3dsf::ValueType>::max();
	 } else if (gx < Limits<Img3dsf::ValueType>::min()) {
	    imd[p] = Limits<Img3dsf::ValueType>::min();
	 } else {
	    imd[p] = (Img3dsf::ValueType)gx;
	 }
      }
   }
   
   return SUCCESS;
}
Errc PSharp( const Imx2duc &ims, Imx2duc &imd, int connexity, float k ) {
   Float gx;
   Point2d p;
   
   imd.Frame(ims, 1);
   
   if (connexity == 4) {
      for (int b = 0; b < ims.Bands(); b++) {
   for (p.y = 1; p.y < ims.Height() - 1; p.y++)
   for (p.x = 1; p.x < ims.Width() - 1; p.x++)
	 {
	    gx = 0.0F;
	    for (int v = 0; v < 4; v++) {
	       gx += (Float)ims[b][p+v4[v]];
	    }
	    gx = (float)ims[b][p] + (k / 4) * (4 * ims[b][p] - gx);
	    if (gx > Limits<Imx2duc::ValueType>::max()) {
	       imd[b][p] = Limits<Imx2duc::ValueType>::max();
	    } else if (gx < 0) {
	       imd[b][p] = 0;
	    } else {
	       imd[b][p] = (Imx2duc::ValueType)gx;
	    }
	 }
      }
   } else {
      for (int b = 0; b < ims.Bands(); b++) {
   for (p.y = 1; p.y < ims.Height() - 1; p.y++)
   for (p.x = 1; p.x < ims.Width() - 1; p.x++)
	 {
	    gx = 0.0F;
	    for (int v = 0; v < 8; v++)
	       gx += (Float)ims[b][p + v8[v]];
	    gx = (float)ims[b][p] + (k / 8) * (8 * ims[b][p] - gx);
	    if (gx > Limits<Imx2duc::ValueType>::max()) {
	       imd[b][p] = Limits<Imx2duc::ValueType>::max();
	    } else if (gx < 0) {
	       imd[b][p] = 0;
	    } else {
	       imd[b][p] = (Imx2duc::ValueType)gx;
	    }
	 }
      }
   }
   
   return SUCCESS;
}
Errc PSharp( const Imx2dsl &ims, Imx2dsl &imd, int connexity, float k ) {
   Float gx;
   Point2d p;
   
   imd.Frame(ims, 1);
   
   if (connexity == 4) {
      for (int b = 0; b < ims.Bands(); b++) {
   for (p.y = 1; p.y < ims.Height() - 1; p.y++)
   for (p.x = 1; p.x < ims.Width() - 1; p.x++)
	 {
	    gx = 0.0F;
	    for (int v = 0; v < 4; v++) {
	       gx += (Float)ims[b][p+v4[v]];
	    }
	    gx = (float)ims[b][p] + (k / 4) * (4 * ims[b][p] - gx);
	    if (gx > Limits<Imx2dsl::ValueType>::max()) {
	       imd[b][p] = Limits<Imx2dsl::ValueType>::max();
	    } else if (gx < 0) {
	       imd[b][p] = 0;
	    } else {
	       imd[b][p] = (Imx2dsl::ValueType)gx;
	    }
	 }
      }
   } else {
      for (int b = 0; b < ims.Bands(); b++) {
   for (p.y = 1; p.y < ims.Height() - 1; p.y++)
   for (p.x = 1; p.x < ims.Width() - 1; p.x++)
	 {
	    gx = 0.0F;
	    for (int v = 0; v < 8; v++)
	       gx += (Float)ims[b][p + v8[v]];
	    gx = (float)ims[b][p] + (k / 8) * (8 * ims[b][p] - gx);
	    if (gx > Limits<Imx2dsl::ValueType>::max()) {
	       imd[b][p] = Limits<Imx2dsl::ValueType>::max();
	    } else if (gx < 0) {
	       imd[b][p] = 0;
	    } else {
	       imd[b][p] = (Imx2dsl::ValueType)gx;
	    }
	 }
      }
   }
   
   return SUCCESS;
}
Errc PSharp( const Imx2dsf &ims, Imx2dsf &imd, int connexity, float k ) {
   Float gx;
   Point2d p;
   
   imd.Frame(ims, 1);
   
   if (connexity == 4) {
      for (int b = 0; b < ims.Bands(); b++) {
   for (p.y = 1; p.y < ims.Height() - 1; p.y++)
   for (p.x = 1; p.x < ims.Width() - 1; p.x++)
	 {
	    gx = 0.0F;
	    for (int v = 0; v < 4; v++) {
	       gx += (Float)ims[b][p+v4[v]];
	    }
	    gx = (float)ims[b][p] + (k / 4) * (4 * ims[b][p] - gx);
	    if (gx > Limits<Imx2dsf::ValueType>::max()) {
	       imd[b][p] = Limits<Imx2dsf::ValueType>::max();
	    } else if (gx < 0) {
	       imd[b][p] = 0;
	    } else {
	       imd[b][p] = (Imx2dsf::ValueType)gx;
	    }
	 }
      }
   } else {
      for (int b = 0; b < ims.Bands(); b++) {
   for (p.y = 1; p.y < ims.Height() - 1; p.y++)
   for (p.x = 1; p.x < ims.Width() - 1; p.x++)
	 {
	    gx = 0.0F;
	    for (int v = 0; v < 8; v++)
	       gx += (Float)ims[b][p + v8[v]];
	    gx = (float)ims[b][p] + (k / 8) * (8 * ims[b][p] - gx);
	    if (gx > Limits<Imx2dsf::ValueType>::max()) {
	       imd[b][p] = Limits<Imx2dsf::ValueType>::max();
	    } else if (gx < 0) {
	       imd[b][p] = 0;
	    } else {
	       imd[b][p] = (Imx2dsf::ValueType)gx;
	    }
	 }
      }
   }
   
   return SUCCESS;
}
Errc PSharp( const Imx3duc &ims, Imx3duc &imd, int connexity, float k ) {
   Float gx;
   Point3d p;
   
   imd.Frame(ims, 1);
   
   if (connexity == 6) {
      for (int b = 0; b < ims.Bands(); b++) {
   for (p.z = 1; p.z < ims.Depth() - 1; p.z++)
   for (p.y = 1; p.y < ims.Height() - 1; p.y++)
   for (p.x = 1; p.x < ims.Width() - 1; p.x++)
	 {
	    gx = 0.0F;
	    for (int v = 0; v < 6; v++) {
	       gx += (Float)ims[b][p+v6[v]];
	    }
	    gx = (float)ims[b][p] + (k / 6) * (6 * ims[b][p] - gx);
	    if (gx > Limits<Imx3duc::ValueType>::max()) {
	       imd[b][p] = Limits<Imx3duc::ValueType>::max();
	    } else if (gx < 0) {
	       imd[b][p] = 0;
	    } else {
	       imd[b][p] = (Imx3duc::ValueType)gx;
	    }
	 }
      }
   } else {
      for (int b = 0; b < ims.Bands(); b++) {
   for (p.z = 1; p.z < ims.Depth() - 1; p.z++)
   for (p.y = 1; p.y < ims.Height() - 1; p.y++)
   for (p.x = 1; p.x < ims.Width() - 1; p.x++)
	 {
	    gx = 0.0F;
	    for (int v = 0; v < 26; v++)
	       gx += (Float)ims[b][p + v26[v]];
	    gx = (float)ims[b][p] + (k / 26) * (26 * ims[b][p] - gx);
	    if (gx > Limits<Imx3duc::ValueType>::max()) {
	       imd[b][p] = Limits<Imx3duc::ValueType>::max();
	    } else if (gx < 0) {
	       imd[b][p] = 0;
	    } else {
	       imd[b][p] = (Imx3duc::ValueType)gx;
	    }
	 }
      }
   }
   
   return SUCCESS;
}
Errc PSharp( const Imx3dsl &ims, Imx3dsl &imd, int connexity, float k ) {
   Float gx;
   Point3d p;
   
   imd.Frame(ims, 1);
   
   if (connexity == 6) {
      for (int b = 0; b < ims.Bands(); b++) {
   for (p.z = 1; p.z < ims.Depth() - 1; p.z++)
   for (p.y = 1; p.y < ims.Height() - 1; p.y++)
   for (p.x = 1; p.x < ims.Width() - 1; p.x++)
	 {
	    gx = 0.0F;
	    for (int v = 0; v < 6; v++) {
	       gx += (Float)ims[b][p+v6[v]];
	    }
	    gx = (float)ims[b][p] + (k / 6) * (6 * ims[b][p] - gx);
	    if (gx > Limits<Imx3dsl::ValueType>::max()) {
	       imd[b][p] = Limits<Imx3dsl::ValueType>::max();
	    } else if (gx < 0) {
	       imd[b][p] = 0;
	    } else {
	       imd[b][p] = (Imx3dsl::ValueType)gx;
	    }
	 }
      }
   } else {
      for (int b = 0; b < ims.Bands(); b++) {
   for (p.z = 1; p.z < ims.Depth() - 1; p.z++)
   for (p.y = 1; p.y < ims.Height() - 1; p.y++)
   for (p.x = 1; p.x < ims.Width() - 1; p.x++)
	 {
	    gx = 0.0F;
	    for (int v = 0; v < 26; v++)
	       gx += (Float)ims[b][p + v26[v]];
	    gx = (float)ims[b][p] + (k / 26) * (26 * ims[b][p] - gx);
	    if (gx > Limits<Imx3dsl::ValueType>::max()) {
	       imd[b][p] = Limits<Imx3dsl::ValueType>::max();
	    } else if (gx < 0) {
	       imd[b][p] = 0;
	    } else {
	       imd[b][p] = (Imx3dsl::ValueType)gx;
	    }
	 }
      }
   }
   
   return SUCCESS;
}
Errc PSharp( const Imx3dsf &ims, Imx3dsf &imd, int connexity, float k ) {
   Float gx;
   Point3d p;
   
   imd.Frame(ims, 1);
   
   if (connexity == 6) {
      for (int b = 0; b < ims.Bands(); b++) {
   for (p.z = 1; p.z < ims.Depth() - 1; p.z++)
   for (p.y = 1; p.y < ims.Height() - 1; p.y++)
   for (p.x = 1; p.x < ims.Width() - 1; p.x++)
	 {
	    gx = 0.0F;
	    for (int v = 0; v < 6; v++) {
	       gx += (Float)ims[b][p+v6[v]];
	    }
	    gx = (float)ims[b][p] + (k / 6) * (6 * ims[b][p] - gx);
	    if (gx > Limits<Imx3dsf::ValueType>::max()) {
	       imd[b][p] = Limits<Imx3dsf::ValueType>::max();
	    } else if (gx < 0) {
	       imd[b][p] = 0;
	    } else {
	       imd[b][p] = (Imx3dsf::ValueType)gx;
	    }
	 }
      }
   } else {
      for (int b = 0; b < ims.Bands(); b++) {
   for (p.z = 1; p.z < ims.Depth() - 1; p.z++)
   for (p.y = 1; p.y < ims.Height() - 1; p.y++)
   for (p.x = 1; p.x < ims.Width() - 1; p.x++)
	 {
	    gx = 0.0F;
	    for (int v = 0; v < 26; v++)
	       gx += (Float)ims[b][p + v26[v]];
	    gx = (float)ims[b][p] + (k / 26) * (26 * ims[b][p] - gx);
	    if (gx > Limits<Imx3dsf::ValueType>::max()) {
	       imd[b][p] = Limits<Imx3dsf::ValueType>::max();
	    } else if (gx < 0) {
	       imd[b][p] = 0;
	    } else {
	       imd[b][p] = (Imx3dsf::ValueType)gx;
	    }
	 }
      }
   }
   
   return SUCCESS;
}
Errc PSharp( const Imc2duc &ims, Imc2duc &imd, int connexity, float k ) {
   Float gx;
   Point2d p;
   
   imd.Frame(ims, 1);
   
   if (connexity == 4) {
      for (int b = 0; b < ims.Bands(); b++) {
   for (p.y = 1; p.y < ims.Height() - 1; p.y++)
   for (p.x = 1; p.x < ims.Width() - 1; p.x++)
	 {
	    gx = 0.0F;
	    for (int v = 0; v < 4; v++) {
	       gx += (Float)ims[b][p+v4[v]];
	    }
	    gx = (float)ims[b][p] + (k / 4) * (4 * ims[b][p] - gx);
	    if (gx > Limits<Imc2duc::ValueType>::max()) {
	       imd[b][p] = Limits<Imc2duc::ValueType>::max();
	    } else if (gx < 0) {
	       imd[b][p] = 0;
	    } else {
	       imd[b][p] = (Imc2duc::ValueType)gx;
	    }
	 }
      }
   } else {
      for (int b = 0; b < ims.Bands(); b++) {
   for (p.y = 1; p.y < ims.Height() - 1; p.y++)
   for (p.x = 1; p.x < ims.Width() - 1; p.x++)
	 {
	    gx = 0.0F;
	    for (int v = 0; v < 8; v++)
	       gx += (Float)ims[b][p + v8[v]];
	    gx = (float)ims[b][p] + (k / 8) * (8 * ims[b][p] - gx);
	    if (gx > Limits<Imc2duc::ValueType>::max()) {
	       imd[b][p] = Limits<Imc2duc::ValueType>::max();
	    } else if (gx < 0) {
	       imd[b][p] = 0;
	    } else {
	       imd[b][p] = (Imc2duc::ValueType)gx;
	    }
	 }
      }
   }
   
   return SUCCESS;
}
Errc PSharp( const Imc2dsl &ims, Imc2dsl &imd, int connexity, float k ) {
   Float gx;
   Point2d p;
   
   imd.Frame(ims, 1);
   
   if (connexity == 4) {
      for (int b = 0; b < ims.Bands(); b++) {
   for (p.y = 1; p.y < ims.Height() - 1; p.y++)
   for (p.x = 1; p.x < ims.Width() - 1; p.x++)
	 {
	    gx = 0.0F;
	    for (int v = 0; v < 4; v++) {
	       gx += (Float)ims[b][p+v4[v]];
	    }
	    gx = (float)ims[b][p] + (k / 4) * (4 * ims[b][p] - gx);
	    if (gx > Limits<Imc2dsl::ValueType>::max()) {
	       imd[b][p] = Limits<Imc2dsl::ValueType>::max();
	    } else if (gx < 0) {
	       imd[b][p] = 0;
	    } else {
	       imd[b][p] = (Imc2dsl::ValueType)gx;
	    }
	 }
      }
   } else {
      for (int b = 0; b < ims.Bands(); b++) {
   for (p.y = 1; p.y < ims.Height() - 1; p.y++)
   for (p.x = 1; p.x < ims.Width() - 1; p.x++)
	 {
	    gx = 0.0F;
	    for (int v = 0; v < 8; v++)
	       gx += (Float)ims[b][p + v8[v]];
	    gx = (float)ims[b][p] + (k / 8) * (8 * ims[b][p] - gx);
	    if (gx > Limits<Imc2dsl::ValueType>::max()) {
	       imd[b][p] = Limits<Imc2dsl::ValueType>::max();
	    } else if (gx < 0) {
	       imd[b][p] = 0;
	    } else {
	       imd[b][p] = (Imc2dsl::ValueType)gx;
	    }
	 }
      }
   }
   
   return SUCCESS;
}
Errc PSharp( const Imc2dsf &ims, Imc2dsf &imd, int connexity, float k ) {
   Float gx;
   Point2d p;
   
   imd.Frame(ims, 1);
   
   if (connexity == 4) {
      for (int b = 0; b < ims.Bands(); b++) {
   for (p.y = 1; p.y < ims.Height() - 1; p.y++)
   for (p.x = 1; p.x < ims.Width() - 1; p.x++)
	 {
	    gx = 0.0F;
	    for (int v = 0; v < 4; v++) {
	       gx += (Float)ims[b][p+v4[v]];
	    }
	    gx = (float)ims[b][p] + (k / 4) * (4 * ims[b][p] - gx);
	    if (gx > Limits<Imc2dsf::ValueType>::max()) {
	       imd[b][p] = Limits<Imc2dsf::ValueType>::max();
	    } else if (gx < 0) {
	       imd[b][p] = 0;
	    } else {
	       imd[b][p] = (Imc2dsf::ValueType)gx;
	    }
	 }
      }
   } else {
      for (int b = 0; b < ims.Bands(); b++) {
   for (p.y = 1; p.y < ims.Height() - 1; p.y++)
   for (p.x = 1; p.x < ims.Width() - 1; p.x++)
	 {
	    gx = 0.0F;
	    for (int v = 0; v < 8; v++)
	       gx += (Float)ims[b][p + v8[v]];
	    gx = (float)ims[b][p] + (k / 8) * (8 * ims[b][p] - gx);
	    if (gx > Limits<Imc2dsf::ValueType>::max()) {
	       imd[b][p] = Limits<Imc2dsf::ValueType>::max();
	    } else if (gx < 0) {
	       imd[b][p] = 0;
	    } else {
	       imd[b][p] = (Imc2dsf::ValueType)gx;
	    }
	 }
      }
   }
   
   return SUCCESS;
}
Errc PSharp( const Imc3duc &ims, Imc3duc &imd, int connexity, float k ) {
   Float gx;
   Point3d p;
   
   imd.Frame(ims, 1);
   
   if (connexity == 6) {
      for (int b = 0; b < ims.Bands(); b++) {
   for (p.z = 1; p.z < ims.Depth() - 1; p.z++)
   for (p.y = 1; p.y < ims.Height() - 1; p.y++)
   for (p.x = 1; p.x < ims.Width() - 1; p.x++)
	 {
	    gx = 0.0F;
	    for (int v = 0; v < 6; v++) {
	       gx += (Float)ims[b][p+v6[v]];
	    }
	    gx = (float)ims[b][p] + (k / 6) * (6 * ims[b][p] - gx);
	    if (gx > Limits<Imc3duc::ValueType>::max()) {
	       imd[b][p] = Limits<Imc3duc::ValueType>::max();
	    } else if (gx < 0) {
	       imd[b][p] = 0;
	    } else {
	       imd[b][p] = (Imc3duc::ValueType)gx;
	    }
	 }
      }
   } else {
      for (int b = 0; b < ims.Bands(); b++) {
   for (p.z = 1; p.z < ims.Depth() - 1; p.z++)
   for (p.y = 1; p.y < ims.Height() - 1; p.y++)
   for (p.x = 1; p.x < ims.Width() - 1; p.x++)
	 {
	    gx = 0.0F;
	    for (int v = 0; v < 26; v++)
	       gx += (Float)ims[b][p + v26[v]];
	    gx = (float)ims[b][p] + (k / 26) * (26 * ims[b][p] - gx);
	    if (gx > Limits<Imc3duc::ValueType>::max()) {
	       imd[b][p] = Limits<Imc3duc::ValueType>::max();
	    } else if (gx < 0) {
	       imd[b][p] = 0;
	    } else {
	       imd[b][p] = (Imc3duc::ValueType)gx;
	    }
	 }
      }
   }
   
   return SUCCESS;
}
Errc PSharp( const Imc3dsl &ims, Imc3dsl &imd, int connexity, float k ) {
   Float gx;
   Point3d p;
   
   imd.Frame(ims, 1);
   
   if (connexity == 6) {
      for (int b = 0; b < ims.Bands(); b++) {
   for (p.z = 1; p.z < ims.Depth() - 1; p.z++)
   for (p.y = 1; p.y < ims.Height() - 1; p.y++)
   for (p.x = 1; p.x < ims.Width() - 1; p.x++)
	 {
	    gx = 0.0F;
	    for (int v = 0; v < 6; v++) {
	       gx += (Float)ims[b][p+v6[v]];
	    }
	    gx = (float)ims[b][p] + (k / 6) * (6 * ims[b][p] - gx);
	    if (gx > Limits<Imc3dsl::ValueType>::max()) {
	       imd[b][p] = Limits<Imc3dsl::ValueType>::max();
	    } else if (gx < 0) {
	       imd[b][p] = 0;
	    } else {
	       imd[b][p] = (Imc3dsl::ValueType)gx;
	    }
	 }
      }
   } else {
      for (int b = 0; b < ims.Bands(); b++) {
   for (p.z = 1; p.z < ims.Depth() - 1; p.z++)
   for (p.y = 1; p.y < ims.Height() - 1; p.y++)
   for (p.x = 1; p.x < ims.Width() - 1; p.x++)
	 {
	    gx = 0.0F;
	    for (int v = 0; v < 26; v++)
	       gx += (Float)ims[b][p + v26[v]];
	    gx = (float)ims[b][p] + (k / 26) * (26 * ims[b][p] - gx);
	    if (gx > Limits<Imc3dsl::ValueType>::max()) {
	       imd[b][p] = Limits<Imc3dsl::ValueType>::max();
	    } else if (gx < 0) {
	       imd[b][p] = 0;
	    } else {
	       imd[b][p] = (Imc3dsl::ValueType)gx;
	    }
	 }
      }
   }
   
   return SUCCESS;
}
Errc PSharp( const Imc3dsf &ims, Imc3dsf &imd, int connexity, float k ) {
   Float gx;
   Point3d p;
   
   imd.Frame(ims, 1);
   
   if (connexity == 6) {
      for (int b = 0; b < ims.Bands(); b++) {
   for (p.z = 1; p.z < ims.Depth() - 1; p.z++)
   for (p.y = 1; p.y < ims.Height() - 1; p.y++)
   for (p.x = 1; p.x < ims.Width() - 1; p.x++)
	 {
	    gx = 0.0F;
	    for (int v = 0; v < 6; v++) {
	       gx += (Float)ims[b][p+v6[v]];
	    }
	    gx = (float)ims[b][p] + (k / 6) * (6 * ims[b][p] - gx);
	    if (gx > Limits<Imc3dsf::ValueType>::max()) {
	       imd[b][p] = Limits<Imc3dsf::ValueType>::max();
	    } else if (gx < 0) {
	       imd[b][p] = 0;
	    } else {
	       imd[b][p] = (Imc3dsf::ValueType)gx;
	    }
	 }
      }
   } else {
      for (int b = 0; b < ims.Bands(); b++) {
   for (p.z = 1; p.z < ims.Depth() - 1; p.z++)
   for (p.y = 1; p.y < ims.Height() - 1; p.y++)
   for (p.x = 1; p.x < ims.Width() - 1; p.x++)
	 {
	    gx = 0.0F;
	    for (int v = 0; v < 26; v++)
	       gx += (Float)ims[b][p + v26[v]];
	    gx = (float)ims[b][p] + (k / 26) * (26 * ims[b][p] - gx);
	    if (gx > Limits<Imc3dsf::ValueType>::max()) {
	       imd[b][p] = Limits<Imc3dsf::ValueType>::max();
	    } else if (gx < 0) {
	       imd[b][p] = 0;
	    } else {
	       imd[b][p] = (Imc3dsf::ValueType)gx;
	    }
	 }
      }
   }
   
   return SUCCESS;
}

#ifdef MAIN
#define	USAGE	"usage: %s connectivity degree [-m mask] [im_in|-] [im_out|-]"
#define	PARC	2
#define	FINC	1
#define	FOUTC	1
#define	MASK	3

int main( int argc, char *argv[] ) {
   Errc result;                // The result code of the execution.
   Pobject* mask;              // The region map.
   Pobject* objin[FINC + 1];   // The input objects.
   Pobject* objs[FINC + 1];    // The source objects masked.
   Pobject* objout[FOUTC + 1]; // The output object.
   Pobject* objd[FOUTC + 1];   // The result object of the execution.
   char* parv[PARC + 1];       // The input parameters.

   ReadArgs(argc, argv, PARC, FINC, FOUTC, &mask, objin, objs, objout, objd, parv, USAGE, MASK); 
   if (objs[0]->Type() == Po_Img1duc) {
      Img1duc* const ims = (Img1duc*)objs[0];
      objd[0] = new Img1duc(ims->Props());
      Img1duc* const imd = (Img1duc*)objd[0];
      
      result = PSharp(*ims, *imd, atoi(parv[0]), (float)atof(parv[1]));
      goto end;
   }
   if (objs[0]->Type() == Po_Img1dsl) {
      Img1dsl* const ims = (Img1dsl*)objs[0];
      objd[0] = new Img1dsl(ims->Props());
      Img1dsl* const imd = (Img1dsl*)objd[0];
      
      result = PSharp(*ims, *imd, atoi(parv[0]), (float)atof(parv[1]));
      goto end;
   }
   if (objs[0]->Type() == Po_Img1dsf) {
      Img1dsf* const ims = (Img1dsf*)objs[0];
      objd[0] = new Img1dsf(ims->Props());
      Img1dsf* const imd = (Img1dsf*)objd[0];
      
      result = PSharp(*ims, *imd, atoi(parv[0]), (float)atof(parv[1]));
      goto end;
   }
   if (objs[0]->Type() == Po_Img2duc) {
      Img2duc* const ims = (Img2duc*)objs[0];
      objd[0] = new Img2duc(ims->Props());
      Img2duc* const imd = (Img2duc*)objd[0];
      
      result = PSharp(*ims, *imd, atoi(parv[0]), (float)atof(parv[1]));
      goto end;
   }
   if (objs[0]->Type() == Po_Img2dsl) {
      Img2dsl* const ims = (Img2dsl*)objs[0];
      objd[0] = new Img2dsl(ims->Props());
      Img2dsl* const imd = (Img2dsl*)objd[0];
      
      result = PSharp(*ims, *imd, atoi(parv[0]), (float)atof(parv[1]));
      goto end;
   }
   if (objs[0]->Type() == Po_Img2dsf) {
      Img2dsf* const ims = (Img2dsf*)objs[0];
      objd[0] = new Img2dsf(ims->Props());
      Img2dsf* const imd = (Img2dsf*)objd[0];
      
      result = PSharp(*ims, *imd, atoi(parv[0]), (float)atof(parv[1]));
      goto end;
   }
   if (objs[0]->Type() == Po_Img3duc) {
      Img3duc* const ims = (Img3duc*)objs[0];
      objd[0] = new Img3duc(ims->Props());
      Img3duc* const imd = (Img3duc*)objd[0];
      
      result = PSharp(*ims, *imd, atoi(parv[0]), (float)atof(parv[1]));
      goto end;
   }
   if (objs[0]->Type() == Po_Img3dsl) {
      Img3dsl* const ims = (Img3dsl*)objs[0];
      objd[0] = new Img3dsl(ims->Props());
      Img3dsl* const imd = (Img3dsl*)objd[0];
      
      result = PSharp(*ims, *imd, atoi(parv[0]), (float)atof(parv[1]));
      goto end;
   }
   if (objs[0]->Type() == Po_Img3dsf) {
      Img3dsf* const ims = (Img3dsf*)objs[0];
      objd[0] = new Img3dsf(ims->Props());
      Img3dsf* const imd = (Img3dsf*)objd[0];
      
      result = PSharp(*ims, *imd, atoi(parv[0]), (float)atof(parv[1]));
      goto end;
   }
   if (objs[0]->Type() == Po_Imx2duc) {
      Imx2duc* const ims = (Imx2duc*)objs[0];
      objd[0] = new Imx2duc(ims->Props());
      Imx2duc* const imd = (Imx2duc*)objd[0];
      
      result = PSharp(*ims, *imd, atoi(parv[0]), (float)atof(parv[1]));
      goto end;
   }
   if (objs[0]->Type() == Po_Imx2dsl) {
      Imx2dsl* const ims = (Imx2dsl*)objs[0];
      objd[0] = new Imx2dsl(ims->Props());
      Imx2dsl* const imd = (Imx2dsl*)objd[0];
      
      result = PSharp(*ims, *imd, atoi(parv[0]), (float)atof(parv[1]));
      goto end;
   }
   if (objs[0]->Type() == Po_Imx2dsf) {
      Imx2dsf* const ims = (Imx2dsf*)objs[0];
      objd[0] = new Imx2dsf(ims->Props());
      Imx2dsf* const imd = (Imx2dsf*)objd[0];
      
      result = PSharp(*ims, *imd, atoi(parv[0]), (float)atof(parv[1]));
      goto end;
   }
   if (objs[0]->Type() == Po_Imx3duc) {
      Imx3duc* const ims = (Imx3duc*)objs[0];
      objd[0] = new Imx3duc(ims->Props());
      Imx3duc* const imd = (Imx3duc*)objd[0];
      
      result = PSharp(*ims, *imd, atoi(parv[0]), (float)atof(parv[1]));
      goto end;
   }
   if (objs[0]->Type() == Po_Imx3dsl) {
      Imx3dsl* const ims = (Imx3dsl*)objs[0];
      objd[0] = new Imx3dsl(ims->Props());
      Imx3dsl* const imd = (Imx3dsl*)objd[0];
      
      result = PSharp(*ims, *imd, atoi(parv[0]), (float)atof(parv[1]));
      goto end;
   }
   if (objs[0]->Type() == Po_Imx3dsf) {
      Imx3dsf* const ims = (Imx3dsf*)objs[0];
      objd[0] = new Imx3dsf(ims->Props());
      Imx3dsf* const imd = (Imx3dsf*)objd[0];
      
      result = PSharp(*ims, *imd, atoi(parv[0]), (float)atof(parv[1]));
      goto end;
   }
   if (objs[0]->Type() == Po_Imc2duc) {
      Imc2duc* const ims = (Imc2duc*)objs[0];
      objd[0] = new Imc2duc(ims->Props());
      Imc2duc* const imd = (Imc2duc*)objd[0];
      
      result = PSharp(*ims, *imd, atoi(parv[0]), (float)atof(parv[1]));
      goto end;
   }
   if (objs[0]->Type() == Po_Imc2dsl) {
      Imc2dsl* const ims = (Imc2dsl*)objs[0];
      objd[0] = new Imc2dsl(ims->Props());
      Imc2dsl* const imd = (Imc2dsl*)objd[0];
      
      result = PSharp(*ims, *imd, atoi(parv[0]), (float)atof(parv[1]));
      goto end;
   }
   if (objs[0]->Type() == Po_Imc2dsf) {
      Imc2dsf* const ims = (Imc2dsf*)objs[0];
      objd[0] = new Imc2dsf(ims->Props());
      Imc2dsf* const imd = (Imc2dsf*)objd[0];
      
      result = PSharp(*ims, *imd, atoi(parv[0]), (float)atof(parv[1]));
      goto end;
   }
   if (objs[0]->Type() == Po_Imc3duc) {
      Imc3duc* const ims = (Imc3duc*)objs[0];
      objd[0] = new Imc3duc(ims->Props());
      Imc3duc* const imd = (Imc3duc*)objd[0];
      
      result = PSharp(*ims, *imd, atoi(parv[0]), (float)atof(parv[1]));
      goto end;
   }
   if (objs[0]->Type() == Po_Imc3dsl) {
      Imc3dsl* const ims = (Imc3dsl*)objs[0];
      objd[0] = new Imc3dsl(ims->Props());
      Imc3dsl* const imd = (Imc3dsl*)objd[0];
      
      result = PSharp(*ims, *imd, atoi(parv[0]), (float)atof(parv[1]));
      goto end;
   }
   if (objs[0]->Type() == Po_Imc3dsf) {
      Imc3dsf* const ims = (Imc3dsf*)objs[0];
      objd[0] = new Imc3dsf(ims->Props());
      Imc3dsf* const imd = (Imc3dsf*)objd[0];
      
      result = PSharp(*ims, *imd, atoi(parv[0]), (float)atof(parv[1]));
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
