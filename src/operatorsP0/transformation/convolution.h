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
 * https://clouard.users.greyc.fr/Pandore/
 */

/**
 * @author Régis Clouard - Feb 12, 2011
 *
 * The implementation of the convotution used
 * by the interpolation filters.
 */

template <typename T>
Errc PConvolution( const Imx2d<T> &ims, Imx2d<T> &imd, const float factory, const float factorx ) {
   float scalex = std::min(factorx, 1.0f);
   float scaley = std::min(factory, 1.0f);
   double supportx = FilterRadius / scalex;
   double supporty = FilterRadius / scaley;

   if (supportx <= 0.5) { supportx = 0.5 + 1e-12; scalex = 1.0; }
   if (supporty <= 0.5) { supporty = 0.5 + 1e-12; scaley = 1.0; }

   for (int b=0; b<imd.Bands(); ++b) {

      for (int y=0; y<imd.Height(); ++y) {
 	 for (int x=0; x<imd.Width(); ++x) {
	    const float centerx = (x+0.5f) / factorx;
	    const float centery = (y+0.5f) / factory;
	    const int startx = (int)std::max(centerx-supportx-0.5, (double)0);
	    const int stopx  = (int)std::min(centerx+supportx+0.5, (double)(ims.Width()-1));
	    const int starty = (int)std::max(centery-supporty-0.5, (double)0);
	    const int stopy  = (int)std::min(centery+supporty+0.5, (double)(ims.Height()-1));

	    const int nxmax = stopx - startx;
	    const int nymax = stopy - starty;

	    double value = 0.0;
	    double density = 0.0;

	    float sy = starty - centery + 0.5f;
	    for (int ny=0; ny<=nymax; ++ny, ++sy) {
	       float sx = startx - centerx + 0.5f;
	       for (int nx=0; nx<=nxmax; ++nx, ++sx) {
		  double contribution = Kernel(sx*scalex) * Kernel(sy*scaley);
		  density += contribution;
		  value += contribution*ims(b,starty+ny,startx+nx);
	       }
	    }
	    if (density == 0) {
	       imd(b,y,x) = 0;
	    } else {
	       /* Normalize. */
	       value /= density;
	       if (value > Limits<T>::max()) {
		  imd(b,y,x) = Limits<T>::max();
	       } else if (value < Limits<T>::min()) {
		  imd(b,y,x) = Limits<T>::min();
	       } else {
		  imd(b,y,x) = (T)value;
	       }
	    }
	 }
      }
   }
   return SUCCESS;
}

template <typename T>
Errc PConvolution( const Imx3d<T> &ims, Imx3d<T> &imd, const float factorz, const float factory, const float factorx ) {
   float scalex = std::min(factorx, 1.0f);
   float scaley = std::min(factory, 1.0f);
   float scalez = std::min(factorz, 1.0f);
   double supportx = FilterRadius / scalex;
   double supporty = FilterRadius / scaley;
   double supportz = FilterRadius / scalez;

   if (supportx <= 0.5) { supportx = 0.5 + 1e-12; scalex = 1.0; }
   if (supporty <= 0.5) { supporty = 0.5 + 1e-12; scaley = 1.0; }
   if (supportz <= 0.5) { supportz = 0.5 + 1e-12; scalez = 1.0; }

   for (int b=0; b<imd.Bands(); ++b) {

      for (int z=0; z<imd.Depth(); ++z) {
	 for (int y=0; y<imd.Height(); ++y) {
	    for (int x=0; x<imd.Width(); ++x) {
	       const float centerx = (x+0.5f) / factorx;
	       const float centery = (y+0.5f) / factory;
	       const float centerz = (z+0.5f) / factorz;

	       const int startx = (int)std::max(centerx-supportx-0.5, (double)0);
	       const int stopx  = (int)std::min(centerx+supportx+0.5, (double)(ims.Width()-1));
	       const int starty = (int)std::max(centery-supporty-0.5, (double)0);
	       const int stopy  = (int)std::min(centery+supporty+0.5, (double)(ims.Height()-1));
	       const int startz = (int)std::max(centerz-supportz-0.5, (double)0);
	       const int stopz  = (int)std::min(centerz+supportz+0.5, (double)(ims.Depth()-1));
	       
	       const int nxmax = stopx - startx;
	       const int nymax = stopy - starty;
	       const int nzmax = stopz - startz;
	       
	       double value = 0.0;
	       double density = 0.0;
	       
	       float sz = startz - centerz + 0.5f;
	       for (int nz=0; nz<=nzmax; ++nz, ++sz) {
		  float sy = starty - centery + 0.5f;
		  for (int ny=0; ny<=nymax; ++ny, ++sy) {
		     float sx = startx - centerx + 0.5f;
		     for (int nx=0; nx<=nxmax; ++nx, ++sx) {
			double contribution = Kernel(sx*scalex) * Kernel(sy*scaley) * Kernel(sz*scalez);
			density += contribution;
			value += contribution*ims(b,startz+nz,starty+ny,startx+nx);
		     }
		  }
	       }
	       if (density == 0) {
		  imd(b,z,y,x) = 0;
	       } else {
		  /* Normalize. */
		  value /= density;
		  if (value > Limits<T>::max()) {
		     imd(b,z,y,x) = Limits<T>::max();
		  } else if (value < Limits<T>::min()) {
		     imd(b,z,y,x) = Limits<T>::min();
		  } else {
		     imd(b,z,y,x) = (T)value;
		  }
	       }
	    }
	 }
      }
   }
   return SUCCESS;
}
