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
 * @author Régis Clouard - Jun 13, 2011
 */

#include <pandore.h>
using namespace pandore;

/**
 * @file pcopyborder.cpp
 * Copies the border values of the ims1 into ims2.
 */




 


Errc PCopyBorder( const Img2duc &ims1, const Img2duc &ims2, Img2duc &imd, Long ll, Long lr, Long hu, Long hb ) {  
   if (lr > ims1.Width() || ll > ims1.Width() || hu > ims1.Height() || hb > ims1.Height() ||
       lr < 0 || ll < 0 || hu < 0 || hb < 0) {
      std::cerr<<"Error pcopyborder: bad parameter values" <<std::endl;
      return FAILURE;
   }   
   if (ims1.Size() != ims2.Size()) {
      std::cerr<<"Error pcopyborder: input images incompatible sizes" <<std::endl;
      return FAILURE;
   }   
   imd = ims1;
   
   register int x, y;
   for (x = 0; x < imd.Width(); x++) {
      for (y = 0; y < hb; y++) {
	 imd[y][x] = ims2[y][x];
      }
      for (y = 0; y < hu; y++) {
	 imd[ims2.Height() - y - 1][x] = ims2[ims2.Height() - y - 1][x];
      }
   }
   
   for (y = 0; y < imd.Height(); y++) {
      for (x = 0; x < ll; x++) {
	 imd[y][x] = ims2[y][x];
      }
      for (x= 0; x < lr; x++) {
	 imd[y][ims2.Width() - 1 - x] = ims2[y][ims2.Width() - 1 - x];
      }
   }
   
   return SUCCESS;
}

Errc PCopyBorder( const Img2dsl &ims1, const Img2dsl &ims2, Img2dsl &imd, Long ll, Long lr, Long hu, Long hb ) {  
   if (lr > ims1.Width() || ll > ims1.Width() || hu > ims1.Height() || hb > ims1.Height() ||
       lr < 0 || ll < 0 || hu < 0 || hb < 0) {
      std::cerr<<"Error pcopyborder: bad parameter values" <<std::endl;
      return FAILURE;
   }   
   if (ims1.Size() != ims2.Size()) {
      std::cerr<<"Error pcopyborder: input images incompatible sizes" <<std::endl;
      return FAILURE;
   }   
   imd = ims1;
   
   register int x, y;
   for (x = 0; x < imd.Width(); x++) {
      for (y = 0; y < hb; y++) {
	 imd[y][x] = ims2[y][x];
      }
      for (y = 0; y < hu; y++) {
	 imd[ims2.Height() - y - 1][x] = ims2[ims2.Height() - y - 1][x];
      }
   }
   
   for (y = 0; y < imd.Height(); y++) {
      for (x = 0; x < ll; x++) {
	 imd[y][x] = ims2[y][x];
      }
      for (x= 0; x < lr; x++) {
	 imd[y][ims2.Width() - 1 - x] = ims2[y][ims2.Width() - 1 - x];
      }
   }
   
   return SUCCESS;
}

Errc PCopyBorder( const Img2dsf &ims1, const Img2dsf &ims2, Img2dsf &imd, Long ll, Long lr, Long hu, Long hb ) {  
   if (lr > ims1.Width() || ll > ims1.Width() || hu > ims1.Height() || hb > ims1.Height() ||
       lr < 0 || ll < 0 || hu < 0 || hb < 0) {
      std::cerr<<"Error pcopyborder: bad parameter values" <<std::endl;
      return FAILURE;
   }   
   if (ims1.Size() != ims2.Size()) {
      std::cerr<<"Error pcopyborder: input images incompatible sizes" <<std::endl;
      return FAILURE;
   }   
   imd = ims1;
   
   register int x, y;
   for (x = 0; x < imd.Width(); x++) {
      for (y = 0; y < hb; y++) {
	 imd[y][x] = ims2[y][x];
      }
      for (y = 0; y < hu; y++) {
	 imd[ims2.Height() - y - 1][x] = ims2[ims2.Height() - y - 1][x];
      }
   }
   
   for (y = 0; y < imd.Height(); y++) {
      for (x = 0; x < ll; x++) {
	 imd[y][x] = ims2[y][x];
      }
      for (x= 0; x < lr; x++) {
	 imd[y][ims2.Width() - 1 - x] = ims2[y][ims2.Width() - 1 - x];
      }
   }
   
   return SUCCESS;
}

Errc PCopyBorder( const Imc2duc &ims1, const Imc2duc &ims2, Imc2duc &imd, Long ll, Long lr, Long hu, Long hb ) {
   if (lr > ims1.Width()/2 || ll > ims1.Width()/2 || hu > ims1.Height()/2 || hb > ims1.Height()/2 ||
   lr < 0 || ll < 0 || hu < 0 || hb < 0) {
      std::cerr << "pcopyborder: Error: bad parameter values" << std::endl;
      return FAILURE;
   }
   if (ims1.Size() != ims2.Size()) {
      std::cerr << "pcopyborder: Error: input images incompatible sizes" << std::endl;
      return FAILURE;
   }   
   
   imd = ims1;
   
   register int x, y, b;
   for (x = 0; x < imd.Width(); x++) {
      for (y = 0; y < hu; y++) {
	 for (b=0;b<imd.Bands();b++) {
	    imd[b][y][x] = ims2[b][y][x];
	 }
      }
      for (y=0; y<hb; y++)
	 for (b=0;b<imd.Bands();b++)
	    imd[b][ims2.Height()-y-1][x] = ims2[b][ims2.Height()-y-1][x];
   }
   
   for (y=0; y<imd.Height(); y++) {
      for (x=0; x<ll; x++) {
	 for (b=0;b<imd.Bands();b++) {
	    imd[b][y][x] = ims2[b][y][x];
	 }
      }
      for (x=0; x<lr; x++) {
	 for (b=0;b<imd.Bands();b++) {
	    imd[b][y][ims2.Width()-1-x] = ims2[b][y][ims2.Width()-1-x];
	 }
      }
   }
   
   return SUCCESS;
}
Errc PCopyBorder( const Imc2dsl &ims1, const Imc2dsl &ims2, Imc2dsl &imd, Long ll, Long lr, Long hu, Long hb ) {
   if (lr > ims1.Width()/2 || ll > ims1.Width()/2 || hu > ims1.Height()/2 || hb > ims1.Height()/2 ||
   lr < 0 || ll < 0 || hu < 0 || hb < 0) {
      std::cerr << "pcopyborder: Error: bad parameter values" << std::endl;
      return FAILURE;
   }
   if (ims1.Size() != ims2.Size()) {
      std::cerr << "pcopyborder: Error: input images incompatible sizes" << std::endl;
      return FAILURE;
   }   
   
   imd = ims1;
   
   register int x, y, b;
   for (x = 0; x < imd.Width(); x++) {
      for (y = 0; y < hu; y++) {
	 for (b=0;b<imd.Bands();b++) {
	    imd[b][y][x] = ims2[b][y][x];
	 }
      }
      for (y=0; y<hb; y++)
	 for (b=0;b<imd.Bands();b++)
	    imd[b][ims2.Height()-y-1][x] = ims2[b][ims2.Height()-y-1][x];
   }
   
   for (y=0; y<imd.Height(); y++) {
      for (x=0; x<ll; x++) {
	 for (b=0;b<imd.Bands();b++) {
	    imd[b][y][x] = ims2[b][y][x];
	 }
      }
      for (x=0; x<lr; x++) {
	 for (b=0;b<imd.Bands();b++) {
	    imd[b][y][ims2.Width()-1-x] = ims2[b][y][ims2.Width()-1-x];
	 }
      }
   }
   
   return SUCCESS;
}
Errc PCopyBorder( const Imc2dsf &ims1, const Imc2dsf &ims2, Imc2dsf &imd, Long ll, Long lr, Long hu, Long hb ) {
   if (lr > ims1.Width()/2 || ll > ims1.Width()/2 || hu > ims1.Height()/2 || hb > ims1.Height()/2 ||
   lr < 0 || ll < 0 || hu < 0 || hb < 0) {
      std::cerr << "pcopyborder: Error: bad parameter values" << std::endl;
      return FAILURE;
   }
   if (ims1.Size() != ims2.Size()) {
      std::cerr << "pcopyborder: Error: input images incompatible sizes" << std::endl;
      return FAILURE;
   }   
   
   imd = ims1;
   
   register int x, y, b;
   for (x = 0; x < imd.Width(); x++) {
      for (y = 0; y < hu; y++) {
	 for (b=0;b<imd.Bands();b++) {
	    imd[b][y][x] = ims2[b][y][x];
	 }
      }
      for (y=0; y<hb; y++)
	 for (b=0;b<imd.Bands();b++)
	    imd[b][ims2.Height()-y-1][x] = ims2[b][ims2.Height()-y-1][x];
   }
   
   for (y=0; y<imd.Height(); y++) {
      for (x=0; x<ll; x++) {
	 for (b=0;b<imd.Bands();b++) {
	    imd[b][y][x] = ims2[b][y][x];
	 }
      }
      for (x=0; x<lr; x++) {
	 for (b=0;b<imd.Bands();b++) {
	    imd[b][y][ims2.Width()-1-x] = ims2[b][y][ims2.Width()-1-x];
	 }
      }
   }
   
   return SUCCESS;
}
Errc PCopyBorder( const Imx2duc &ims1, const Imx2duc &ims2, Imx2duc &imd, Long ll, Long lr, Long hu, Long hb ) {
   if (lr > ims1.Width()/2 || ll > ims1.Width()/2 || hu > ims1.Height()/2 || hb > ims1.Height()/2 ||
   lr < 0 || ll < 0 || hu < 0 || hb < 0) {
      std::cerr << "pcopyborder: Error: bad parameter values" << std::endl;
      return FAILURE;
   }
   if (ims1.Size() != ims2.Size()) {
      std::cerr << "pcopyborder: Error: input images incompatible sizes" << std::endl;
      return FAILURE;
   }   
   
   imd = ims1;
   
   register int x, y, b;
   for (x = 0; x < imd.Width(); x++) {
      for (y = 0; y < hu; y++) {
	 for (b=0;b<imd.Bands();b++) {
	    imd[b][y][x] = ims2[b][y][x];
	 }
      }
      for (y=0; y<hb; y++)
	 for (b=0;b<imd.Bands();b++)
	    imd[b][ims2.Height()-y-1][x] = ims2[b][ims2.Height()-y-1][x];
   }
   
   for (y=0; y<imd.Height(); y++) {
      for (x=0; x<ll; x++) {
	 for (b=0;b<imd.Bands();b++) {
	    imd[b][y][x] = ims2[b][y][x];
	 }
      }
      for (x=0; x<lr; x++) {
	 for (b=0;b<imd.Bands();b++) {
	    imd[b][y][ims2.Width()-1-x] = ims2[b][y][ims2.Width()-1-x];
	 }
      }
   }
   
   return SUCCESS;
}
Errc PCopyBorder( const Imx2dsl &ims1, const Imx2dsl &ims2, Imx2dsl &imd, Long ll, Long lr, Long hu, Long hb ) {
   if (lr > ims1.Width()/2 || ll > ims1.Width()/2 || hu > ims1.Height()/2 || hb > ims1.Height()/2 ||
   lr < 0 || ll < 0 || hu < 0 || hb < 0) {
      std::cerr << "pcopyborder: Error: bad parameter values" << std::endl;
      return FAILURE;
   }
   if (ims1.Size() != ims2.Size()) {
      std::cerr << "pcopyborder: Error: input images incompatible sizes" << std::endl;
      return FAILURE;
   }   
   
   imd = ims1;
   
   register int x, y, b;
   for (x = 0; x < imd.Width(); x++) {
      for (y = 0; y < hu; y++) {
	 for (b=0;b<imd.Bands();b++) {
	    imd[b][y][x] = ims2[b][y][x];
	 }
      }
      for (y=0; y<hb; y++)
	 for (b=0;b<imd.Bands();b++)
	    imd[b][ims2.Height()-y-1][x] = ims2[b][ims2.Height()-y-1][x];
   }
   
   for (y=0; y<imd.Height(); y++) {
      for (x=0; x<ll; x++) {
	 for (b=0;b<imd.Bands();b++) {
	    imd[b][y][x] = ims2[b][y][x];
	 }
      }
      for (x=0; x<lr; x++) {
	 for (b=0;b<imd.Bands();b++) {
	    imd[b][y][ims2.Width()-1-x] = ims2[b][y][ims2.Width()-1-x];
	 }
      }
   }
   
   return SUCCESS;
}
Errc PCopyBorder( const Imx2dsf &ims1, const Imx2dsf &ims2, Imx2dsf &imd, Long ll, Long lr, Long hu, Long hb ) {
   if (lr > ims1.Width()/2 || ll > ims1.Width()/2 || hu > ims1.Height()/2 || hb > ims1.Height()/2 ||
   lr < 0 || ll < 0 || hu < 0 || hb < 0) {
      std::cerr << "pcopyborder: Error: bad parameter values" << std::endl;
      return FAILURE;
   }
   if (ims1.Size() != ims2.Size()) {
      std::cerr << "pcopyborder: Error: input images incompatible sizes" << std::endl;
      return FAILURE;
   }   
   
   imd = ims1;
   
   register int x, y, b;
   for (x = 0; x < imd.Width(); x++) {
      for (y = 0; y < hu; y++) {
	 for (b=0;b<imd.Bands();b++) {
	    imd[b][y][x] = ims2[b][y][x];
	 }
      }
      for (y=0; y<hb; y++)
	 for (b=0;b<imd.Bands();b++)
	    imd[b][ims2.Height()-y-1][x] = ims2[b][ims2.Height()-y-1][x];
   }
   
   for (y=0; y<imd.Height(); y++) {
      for (x=0; x<ll; x++) {
	 for (b=0;b<imd.Bands();b++) {
	    imd[b][y][x] = ims2[b][y][x];
	 }
      }
      for (x=0; x<lr; x++) {
	 for (b=0;b<imd.Bands();b++) {
	    imd[b][y][ims2.Width()-1-x] = ims2[b][y][ims2.Width()-1-x];
	 }
      }
   }
   
   return SUCCESS;
}
Errc PCopyBorder( const Reg2d &ims1, const Reg2d &ims2, Reg2d &imd, Long ll, Long lr, Long hu, Long hb ) {
      if (lr > ims1.Width() || ll > ims1.Width() || hu > ims1.Height() || hb > ims1.Height() ||
	  lr < 0 || ll < 0 || hu < 0 || hb < 0) {
	 std::cerr << "Error pcopyborder: bad parameter values" << std::endl;
	 return FAILURE;
      }   
      if (ims1.Size() != ims2.Size()) {
	 std::cerr << "Error pcopyborder: input images incompatible sizes" << std::endl;
	 return FAILURE;
      }   
      imd = ims1;
      
      register int x, y;
      for (x = 0; x < imd.Width(); x++) {
	 for (y = 0; y < hb; y++) {
	    imd[y][x] = ims2[y][x];
	    for (y = 0; y < hu; y++) {
	       imd[ims2.Height() - y - 1][x] = ims2[ims2.Height() - y - 1][x];
	    }
	 }
      }
      for (y = 0; y < imd.Height(); y++) {
	 for (x = 0; x < ll; x++) {
	    imd[y][x] = ims2[y][x];
	 }
	 for (x = 0; x < lr; x++) {
	    imd[y][ims2.Width() - 1 - x] = ims2[y][ims2.Width() - 1 - x];
	 }
      }
      
      imd.Labels(ims1.Labels());
      return SUCCESS;
   }
   
Errc PCopyBorder( const Img3duc &ims1, const Img3duc &ims2, Img3duc &imd, Long ll, Long lr, Long hu, Long hb, Long df, Long db ) {
   if (lr > ims1.Width() || ll > ims1.Width() || hu > ims1.Height() || hb > ims1.Height() || df > ims1.Depth() || db > ims1.Depth() ||
       lr < 0 || ll < 0 || hu < 0 || hb < 0 || df < 0 || db < 0) {
      std::cerr << "pcopyborder: Error: bad parameter values" << std::endl;
      return FAILURE;
   }
   if (ims1.Size() != ims2.Size()) {
      std::cerr << "pcopyborder: Error: input images incompatible sizes" << std::endl;
      return FAILURE;
   }
   
   imd = ims1;

   register int	x, y, z;
   for (y = 0; y < imd.Height(); y++) {
      for (x = 0; x < imd.Width(); x++) {
	 for (z = 0; z < df; z++) {
	    imd[z][y][x] = ims2[z][y][x];
	 }
	 for (z = 0; z < db; z++) {
	    imd[imd.Depth() - 1 - z][y][x] = ims2[imd.Depth() - 1 - z][y][x];
	 }
      }
   }
   for (z = 0; z < imd.Depth(); z++) {
      for (x = 0; x < imd.Width(); x++) {
	 for (y = 0; y < hu; y++)  {
	    imd[z][y][x] = ims2[z][y][x];
	 }
	 for (y = 0; y < hb; y++)  {
	    imd[z][imd.Height() - 1 - y][x] = ims2[z][imd.Height() - 1 - y][x];
	 }
      }
   }
   for (z = 0; z < imd.Depth(); z++) {
      for (y = 0; y < imd.Height(); y++) {
	 for (x = 0; x < ll; x++) {
	    imd[z][y][x] = ims2[z][y][x];
	 }
	 for (x = 0; x < lr; x++) {
	    imd[z][y][imd.Width() - 1 - x] = ims2[z][y][imd.Width() - 1 - x];
	 }
      }
   }
   return SUCCESS;
}

Errc PCopyBorder( const Img3dsl &ims1, const Img3dsl &ims2, Img3dsl &imd, Long ll, Long lr, Long hu, Long hb, Long df, Long db ) {
   if (lr > ims1.Width() || ll > ims1.Width() || hu > ims1.Height() || hb > ims1.Height() || df > ims1.Depth() || db > ims1.Depth() ||
       lr < 0 || ll < 0 || hu < 0 || hb < 0 || df < 0 || db < 0) {
      std::cerr << "pcopyborder: Error: bad parameter values" << std::endl;
      return FAILURE;
   }
   if (ims1.Size() != ims2.Size()) {
      std::cerr << "pcopyborder: Error: input images incompatible sizes" << std::endl;
      return FAILURE;
   }
   
   imd = ims1;

   register int	x, y, z;
   for (y = 0; y < imd.Height(); y++) {
      for (x = 0; x < imd.Width(); x++) {
	 for (z = 0; z < df; z++) {
	    imd[z][y][x] = ims2[z][y][x];
	 }
	 for (z = 0; z < db; z++) {
	    imd[imd.Depth() - 1 - z][y][x] = ims2[imd.Depth() - 1 - z][y][x];
	 }
      }
   }
   for (z = 0; z < imd.Depth(); z++) {
      for (x = 0; x < imd.Width(); x++) {
	 for (y = 0; y < hu; y++)  {
	    imd[z][y][x] = ims2[z][y][x];
	 }
	 for (y = 0; y < hb; y++)  {
	    imd[z][imd.Height() - 1 - y][x] = ims2[z][imd.Height() - 1 - y][x];
	 }
      }
   }
   for (z = 0; z < imd.Depth(); z++) {
      for (y = 0; y < imd.Height(); y++) {
	 for (x = 0; x < ll; x++) {
	    imd[z][y][x] = ims2[z][y][x];
	 }
	 for (x = 0; x < lr; x++) {
	    imd[z][y][imd.Width() - 1 - x] = ims2[z][y][imd.Width() - 1 - x];
	 }
      }
   }
   return SUCCESS;
}

Errc PCopyBorder( const Img3dsf &ims1, const Img3dsf &ims2, Img3dsf &imd, Long ll, Long lr, Long hu, Long hb, Long df, Long db ) {
   if (lr > ims1.Width() || ll > ims1.Width() || hu > ims1.Height() || hb > ims1.Height() || df > ims1.Depth() || db > ims1.Depth() ||
       lr < 0 || ll < 0 || hu < 0 || hb < 0 || df < 0 || db < 0) {
      std::cerr << "pcopyborder: Error: bad parameter values" << std::endl;
      return FAILURE;
   }
   if (ims1.Size() != ims2.Size()) {
      std::cerr << "pcopyborder: Error: input images incompatible sizes" << std::endl;
      return FAILURE;
   }
   
   imd = ims1;

   register int	x, y, z;
   for (y = 0; y < imd.Height(); y++) {
      for (x = 0; x < imd.Width(); x++) {
	 for (z = 0; z < df; z++) {
	    imd[z][y][x] = ims2[z][y][x];
	 }
	 for (z = 0; z < db; z++) {
	    imd[imd.Depth() - 1 - z][y][x] = ims2[imd.Depth() - 1 - z][y][x];
	 }
      }
   }
   for (z = 0; z < imd.Depth(); z++) {
      for (x = 0; x < imd.Width(); x++) {
	 for (y = 0; y < hu; y++)  {
	    imd[z][y][x] = ims2[z][y][x];
	 }
	 for (y = 0; y < hb; y++)  {
	    imd[z][imd.Height() - 1 - y][x] = ims2[z][imd.Height() - 1 - y][x];
	 }
      }
   }
   for (z = 0; z < imd.Depth(); z++) {
      for (y = 0; y < imd.Height(); y++) {
	 for (x = 0; x < ll; x++) {
	    imd[z][y][x] = ims2[z][y][x];
	 }
	 for (x = 0; x < lr; x++) {
	    imd[z][y][imd.Width() - 1 - x] = ims2[z][y][imd.Width() - 1 - x];
	 }
      }
   }
   return SUCCESS;
}


Errc PCopyBorder( const Imc3duc &ims1, const Imc3duc &ims2, Imc3duc &imd, Long df, Long db, Long hu, Long hb, Long ll, Long lr ) {
   register int	x, y, z, b;

   if (lr > ims1.Width() || ll > ims1.Width() || hu > ims1.Height() || hb > ims1.Height() || df > ims1.Depth() || db > ims1.Depth() ||
       lr < 0 || ll < 0 || hu < 0 || hb < 0 || df < 0 || db < 0) {
      std::cerr << "pcopyborder: Error: bad parameter values" << std::endl;
      return FAILURE;
   }
   if (ims1.Size() != ims2.Size()) {
      std::cerr << "pcopyborder: Error: input images incompatible sizes" << std::endl;
      return FAILURE;
   }
   
   imd = ims1;
   for (y = 0; y < imd.Height(); y++) {
      for (x = 0; x < imd.Width(); x++) {
	 for (z = 0; z < df; z++) {
	    for (b = 0; b < imd.Bands(); b++) {
	       imd[b][z][y][x] = ims2[b][z][y][x];
	    }
	 }
	 for (z = 0; z < db; z++) {
	    for (b = 0; b < imd.Bands(); b++) {
	       imd[b][imd.Depth() - 1 - z][y][x] = ims2[b][imd.Depth() - 1 - z][y][x];
	    }
	 }
      }
   }
   for (z=0; z<imd.Depth(); z++) {
      for (x=0; x<imd.Width(); x++) {
	 for (y=0; y<hu; y++) {
	    for (b=0;b<imd.Bands();b++) {
	       imd[b][z][y][x] = ims2[b][z][y][x];
	    }
	 }
	 for (y=0; y<hb; y++) {
	    for (b=0;b<imd.Bands();b++) {
	       imd[b][z][imd.Height()-1-y][x] = ims2[b][z][imd.Height()-1-y][x];
	    }
	 }
      }
   }
   for (z=0; z<imd.Depth(); z++) {
      for (y=0; y<imd.Height(); y++) {
	 for (x=0; x<ll; x++) {
	    for (b=0;b<imd.Bands();b++) {
	       imd[b][z][y][x] = ims2[b][z][y][x];
	    }
	 }
	 for (x=0; x<lr; x++) {
	    for (b=0;b<imd.Bands();b++) {
	       imd[b][z][y][imd.Width()-1-x] = ims2[b][z][y][imd.Width()-1-x];
	    }
	 }
      }
   }
   return SUCCESS;
}

Errc PCopyBorder( const Imc3dsl &ims1, const Imc3dsl &ims2, Imc3dsl &imd, Long df, Long db, Long hu, Long hb, Long ll, Long lr ) {
   register int	x, y, z, b;

   if (lr > ims1.Width() || ll > ims1.Width() || hu > ims1.Height() || hb > ims1.Height() || df > ims1.Depth() || db > ims1.Depth() ||
       lr < 0 || ll < 0 || hu < 0 || hb < 0 || df < 0 || db < 0) {
      std::cerr << "pcopyborder: Error: bad parameter values" << std::endl;
      return FAILURE;
   }
   if (ims1.Size() != ims2.Size()) {
      std::cerr << "pcopyborder: Error: input images incompatible sizes" << std::endl;
      return FAILURE;
   }
   
   imd = ims1;
   for (y = 0; y < imd.Height(); y++) {
      for (x = 0; x < imd.Width(); x++) {
	 for (z = 0; z < df; z++) {
	    for (b = 0; b < imd.Bands(); b++) {
	       imd[b][z][y][x] = ims2[b][z][y][x];
	    }
	 }
	 for (z = 0; z < db; z++) {
	    for (b = 0; b < imd.Bands(); b++) {
	       imd[b][imd.Depth() - 1 - z][y][x] = ims2[b][imd.Depth() - 1 - z][y][x];
	    }
	 }
      }
   }
   for (z=0; z<imd.Depth(); z++) {
      for (x=0; x<imd.Width(); x++) {
	 for (y=0; y<hu; y++) {
	    for (b=0;b<imd.Bands();b++) {
	       imd[b][z][y][x] = ims2[b][z][y][x];
	    }
	 }
	 for (y=0; y<hb; y++) {
	    for (b=0;b<imd.Bands();b++) {
	       imd[b][z][imd.Height()-1-y][x] = ims2[b][z][imd.Height()-1-y][x];
	    }
	 }
      }
   }
   for (z=0; z<imd.Depth(); z++) {
      for (y=0; y<imd.Height(); y++) {
	 for (x=0; x<ll; x++) {
	    for (b=0;b<imd.Bands();b++) {
	       imd[b][z][y][x] = ims2[b][z][y][x];
	    }
	 }
	 for (x=0; x<lr; x++) {
	    for (b=0;b<imd.Bands();b++) {
	       imd[b][z][y][imd.Width()-1-x] = ims2[b][z][y][imd.Width()-1-x];
	    }
	 }
      }
   }
   return SUCCESS;
}

Errc PCopyBorder( const Imc3dsf &ims1, const Imc3dsf &ims2, Imc3dsf &imd, Long df, Long db, Long hu, Long hb, Long ll, Long lr ) {
   register int	x, y, z, b;

   if (lr > ims1.Width() || ll > ims1.Width() || hu > ims1.Height() || hb > ims1.Height() || df > ims1.Depth() || db > ims1.Depth() ||
       lr < 0 || ll < 0 || hu < 0 || hb < 0 || df < 0 || db < 0) {
      std::cerr << "pcopyborder: Error: bad parameter values" << std::endl;
      return FAILURE;
   }
   if (ims1.Size() != ims2.Size()) {
      std::cerr << "pcopyborder: Error: input images incompatible sizes" << std::endl;
      return FAILURE;
   }
   
   imd = ims1;
   for (y = 0; y < imd.Height(); y++) {
      for (x = 0; x < imd.Width(); x++) {
	 for (z = 0; z < df; z++) {
	    for (b = 0; b < imd.Bands(); b++) {
	       imd[b][z][y][x] = ims2[b][z][y][x];
	    }
	 }
	 for (z = 0; z < db; z++) {
	    for (b = 0; b < imd.Bands(); b++) {
	       imd[b][imd.Depth() - 1 - z][y][x] = ims2[b][imd.Depth() - 1 - z][y][x];
	    }
	 }
      }
   }
   for (z=0; z<imd.Depth(); z++) {
      for (x=0; x<imd.Width(); x++) {
	 for (y=0; y<hu; y++) {
	    for (b=0;b<imd.Bands();b++) {
	       imd[b][z][y][x] = ims2[b][z][y][x];
	    }
	 }
	 for (y=0; y<hb; y++) {
	    for (b=0;b<imd.Bands();b++) {
	       imd[b][z][imd.Height()-1-y][x] = ims2[b][z][imd.Height()-1-y][x];
	    }
	 }
      }
   }
   for (z=0; z<imd.Depth(); z++) {
      for (y=0; y<imd.Height(); y++) {
	 for (x=0; x<ll; x++) {
	    for (b=0;b<imd.Bands();b++) {
	       imd[b][z][y][x] = ims2[b][z][y][x];
	    }
	 }
	 for (x=0; x<lr; x++) {
	    for (b=0;b<imd.Bands();b++) {
	       imd[b][z][y][imd.Width()-1-x] = ims2[b][z][y][imd.Width()-1-x];
	    }
	 }
      }
   }
   return SUCCESS;
}

Errc PCopyBorder( const Imx3duc &ims1, const Imx3duc &ims2, Imx3duc &imd, Long df, Long db, Long hu, Long hb, Long ll, Long lr ) {
   register int	x, y, z, b;

   if (lr > ims1.Width() || ll > ims1.Width() || hu > ims1.Height() || hb > ims1.Height() || df > ims1.Depth() || db > ims1.Depth() ||
       lr < 0 || ll < 0 || hu < 0 || hb < 0 || df < 0 || db < 0) {
      std::cerr << "pcopyborder: Error: bad parameter values" << std::endl;
      return FAILURE;
   }
   if (ims1.Size() != ims2.Size()) {
      std::cerr << "pcopyborder: Error: input images incompatible sizes" << std::endl;
      return FAILURE;
   }
   
   imd = ims1;
   for (y = 0; y < imd.Height(); y++) {
      for (x = 0; x < imd.Width(); x++) {
	 for (z = 0; z < df; z++) {
	    for (b = 0; b < imd.Bands(); b++) {
	       imd[b][z][y][x] = ims2[b][z][y][x];
	    }
	 }
	 for (z = 0; z < db; z++) {
	    for (b = 0; b < imd.Bands(); b++) {
	       imd[b][imd.Depth() - 1 - z][y][x] = ims2[b][imd.Depth() - 1 - z][y][x];
	    }
	 }
      }
   }
   for (z=0; z<imd.Depth(); z++) {
      for (x=0; x<imd.Width(); x++) {
	 for (y=0; y<hu; y++) {
	    for (b=0;b<imd.Bands();b++) {
	       imd[b][z][y][x] = ims2[b][z][y][x];
	    }
	 }
	 for (y=0; y<hb; y++) {
	    for (b=0;b<imd.Bands();b++) {
	       imd[b][z][imd.Height()-1-y][x] = ims2[b][z][imd.Height()-1-y][x];
	    }
	 }
      }
   }
   for (z=0; z<imd.Depth(); z++) {
      for (y=0; y<imd.Height(); y++) {
	 for (x=0; x<ll; x++) {
	    for (b=0;b<imd.Bands();b++) {
	       imd[b][z][y][x] = ims2[b][z][y][x];
	    }
	 }
	 for (x=0; x<lr; x++) {
	    for (b=0;b<imd.Bands();b++) {
	       imd[b][z][y][imd.Width()-1-x] = ims2[b][z][y][imd.Width()-1-x];
	    }
	 }
      }
   }
   return SUCCESS;
}

Errc PCopyBorder( const Imx3dsl &ims1, const Imx3dsl &ims2, Imx3dsl &imd, Long df, Long db, Long hu, Long hb, Long ll, Long lr ) {
   register int	x, y, z, b;

   if (lr > ims1.Width() || ll > ims1.Width() || hu > ims1.Height() || hb > ims1.Height() || df > ims1.Depth() || db > ims1.Depth() ||
       lr < 0 || ll < 0 || hu < 0 || hb < 0 || df < 0 || db < 0) {
      std::cerr << "pcopyborder: Error: bad parameter values" << std::endl;
      return FAILURE;
   }
   if (ims1.Size() != ims2.Size()) {
      std::cerr << "pcopyborder: Error: input images incompatible sizes" << std::endl;
      return FAILURE;
   }
   
   imd = ims1;
   for (y = 0; y < imd.Height(); y++) {
      for (x = 0; x < imd.Width(); x++) {
	 for (z = 0; z < df; z++) {
	    for (b = 0; b < imd.Bands(); b++) {
	       imd[b][z][y][x] = ims2[b][z][y][x];
	    }
	 }
	 for (z = 0; z < db; z++) {
	    for (b = 0; b < imd.Bands(); b++) {
	       imd[b][imd.Depth() - 1 - z][y][x] = ims2[b][imd.Depth() - 1 - z][y][x];
	    }
	 }
      }
   }
   for (z=0; z<imd.Depth(); z++) {
      for (x=0; x<imd.Width(); x++) {
	 for (y=0; y<hu; y++) {
	    for (b=0;b<imd.Bands();b++) {
	       imd[b][z][y][x] = ims2[b][z][y][x];
	    }
	 }
	 for (y=0; y<hb; y++) {
	    for (b=0;b<imd.Bands();b++) {
	       imd[b][z][imd.Height()-1-y][x] = ims2[b][z][imd.Height()-1-y][x];
	    }
	 }
      }
   }
   for (z=0; z<imd.Depth(); z++) {
      for (y=0; y<imd.Height(); y++) {
	 for (x=0; x<ll; x++) {
	    for (b=0;b<imd.Bands();b++) {
	       imd[b][z][y][x] = ims2[b][z][y][x];
	    }
	 }
	 for (x=0; x<lr; x++) {
	    for (b=0;b<imd.Bands();b++) {
	       imd[b][z][y][imd.Width()-1-x] = ims2[b][z][y][imd.Width()-1-x];
	    }
	 }
      }
   }
   return SUCCESS;
}

Errc PCopyBorder( const Imx3dsf &ims1, const Imx3dsf &ims2, Imx3dsf &imd, Long df, Long db, Long hu, Long hb, Long ll, Long lr ) {
   register int	x, y, z, b;

   if (lr > ims1.Width() || ll > ims1.Width() || hu > ims1.Height() || hb > ims1.Height() || df > ims1.Depth() || db > ims1.Depth() ||
       lr < 0 || ll < 0 || hu < 0 || hb < 0 || df < 0 || db < 0) {
      std::cerr << "pcopyborder: Error: bad parameter values" << std::endl;
      return FAILURE;
   }
   if (ims1.Size() != ims2.Size()) {
      std::cerr << "pcopyborder: Error: input images incompatible sizes" << std::endl;
      return FAILURE;
   }
   
   imd = ims1;
   for (y = 0; y < imd.Height(); y++) {
      for (x = 0; x < imd.Width(); x++) {
	 for (z = 0; z < df; z++) {
	    for (b = 0; b < imd.Bands(); b++) {
	       imd[b][z][y][x] = ims2[b][z][y][x];
	    }
	 }
	 for (z = 0; z < db; z++) {
	    for (b = 0; b < imd.Bands(); b++) {
	       imd[b][imd.Depth() - 1 - z][y][x] = ims2[b][imd.Depth() - 1 - z][y][x];
	    }
	 }
      }
   }
   for (z=0; z<imd.Depth(); z++) {
      for (x=0; x<imd.Width(); x++) {
	 for (y=0; y<hu; y++) {
	    for (b=0;b<imd.Bands();b++) {
	       imd[b][z][y][x] = ims2[b][z][y][x];
	    }
	 }
	 for (y=0; y<hb; y++) {
	    for (b=0;b<imd.Bands();b++) {
	       imd[b][z][imd.Height()-1-y][x] = ims2[b][z][imd.Height()-1-y][x];
	    }
	 }
      }
   }
   for (z=0; z<imd.Depth(); z++) {
      for (y=0; y<imd.Height(); y++) {
	 for (x=0; x<ll; x++) {
	    for (b=0;b<imd.Bands();b++) {
	       imd[b][z][y][x] = ims2[b][z][y][x];
	    }
	 }
	 for (x=0; x<lr; x++) {
	    for (b=0;b<imd.Bands();b++) {
	       imd[b][z][y][imd.Width()-1-x] = ims2[b][z][y][imd.Width()-1-x];
	    }
	 }
      }
   }
   return SUCCESS;
}

Errc PCopyBorder( const Reg3d &ims1, const Reg3d &ims2, Reg3d &imd, Long ll, Long lr, Long hu, Long hb, Long df, Long db ) {
   if (lr > ims1.Width() || ll > ims1.Width() || hu > ims1.Height() || hb > ims1.Height() || df > ims1.Depth() || db > ims1.Depth() ||
       lr < 0 || ll < 0 || hu < 0 || hb < 0 || df < 0 || db < 0) {
      std::cerr << "pcopyborder: Error: bad parameter values" << std::endl;
      return FAILURE;
   }
   if (ims1.Size() != ims2.Size()) {
      std::cerr << "pcopyborder: Error: input images incompatible sizes" << std::endl;
      return FAILURE;
   }
   
   imd = ims1;
   register int	x, y, z;

   for (y = 0; y < imd.Height(); y++) {
      for (x = 0; x < imd.Width(); x++) {
	 for (z = 0; z < df; z++) {
	    imd[z][y][x] = ims2[z][y][x];
	 }
	 for (z = 0; z < db; z++) {
	    imd[imd.Depth() - 1 - z][y][x] = ims2[imd.Depth() - 1 - z][y][x];
	 }
      }
   }
   for (z = 0; z < imd.Depth(); z++) {
      for (x = 0; x < imd.Width(); x++) {
	 for (y = 0; y < hu; y++)  {
	    imd[z][y][x] = ims2[z][y][x];
	 }
	 for (y = 0; y < hb; y++)  {
	    imd[z][imd.Height() - 1 - y][x] = ims2[z][imd.Height() - 1 - y][x];
	 }
      }
   }
   for (z = 0; z < imd.Depth(); z++) {
      for (y = 0; y < imd.Height(); y++) {
	 for (x = 0; x < ll; x++) {
	    imd[z][y][x] = ims2[z][y][x];
	 }
	 for (x = 0; x < lr; x++) {
	    imd[z][y][imd.Width() - 1 - x] = ims2[z][y][imd.Width() - 1 - x];
	 }
      }
   }
   imd.Labels(ims1.Labels());
   return SUCCESS;
}


#ifdef MAIN
#define	USAGE	"usage: %s widthr widthl heightu heightb depdthf depthb [im_in1|-] [im_in2|-] [im_out|-]"
#define	PARC	6
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
   if (objs[0]->Type() == Po_Img2duc) {
      Img2duc* const ims1 = (Img2duc*)objs[0];
      Img2duc* const ims2 = (Img2duc*)objs[1];
      objd[0] = new Img2duc(ims1->Size());
      Img2duc* const imd = (Img2duc*)objd[0];
      result = PCopyBorder(*ims1, *ims2, *imd, 
			   atol(parv[0]), atol(parv[1]), 
			   atol(parv[2]), atol(parv[3]));
      goto end;
   }
   if (objs[0]->Type() == Po_Img2dsl) {
      Img2dsl* const ims1 = (Img2dsl*)objs[0];
      Img2dsl* const ims2 = (Img2dsl*)objs[1];
      objd[0] = new Img2dsl(ims1->Size());
      Img2dsl* const imd = (Img2dsl*)objd[0];
      result = PCopyBorder(*ims1, *ims2, *imd, 
			   atol(parv[0]), atol(parv[1]), 
			   atol(parv[2]), atol(parv[3]));
      goto end;
   }
   if (objs[0]->Type() == Po_Img2dsf) {
      Img2dsf* const ims1 = (Img2dsf*)objs[0];
      Img2dsf* const ims2 = (Img2dsf*)objs[1];
      objd[0] = new Img2dsf(ims1->Size());
      Img2dsf* const imd = (Img2dsf*)objd[0];
      result = PCopyBorder(*ims1, *ims2, *imd, 
			   atol(parv[0]), atol(parv[1]), 
			   atol(parv[2]), atol(parv[3]));
      goto end;
   }
   if (objs[0]->Type() == Po_Imc2duc) {
      Imc2duc* const ims1=(Imc2duc*)objs[0];
      Imc2duc* const ims2=(Imc2duc*)objs[1];
      objd[0]=new Imc2duc(ims1->Bands(), ims1->Size());
      Imc2duc* const imd=(Imc2duc*)objd[0];
      result=PCopyBorder(*ims1, *ims2, *imd, 
			 atol(parv[0]), atol(parv[1]), 
			 atol(parv[2]), atol(parv[3]));
      goto end;
   }

   if (objs[0]->Type() == Po_Imc2dsl) {
      Imc2dsl* const ims1=(Imc2dsl*)objs[0];
      Imc2dsl* const ims2=(Imc2dsl*)objs[1];
      objd[0]=new Imc2dsl(ims1->Bands(), ims1->Size());
      Imc2dsl* const imd=(Imc2dsl*)objd[0];
      result=PCopyBorder(*ims1, *ims2, *imd, 
			 atol(parv[0]), atol(parv[1]), 
			 atol(parv[2]), atol(parv[3]));
      goto end;
   }

   if (objs[0]->Type() == Po_Imc2dsf) {
      Imc2dsf* const ims1=(Imc2dsf*)objs[0];
      Imc2dsf* const ims2=(Imc2dsf*)objs[1];
      objd[0]=new Imc2dsf(ims1->Bands(), ims1->Size());
      Imc2dsf* const imd=(Imc2dsf*)objd[0];
      result=PCopyBorder(*ims1, *ims2, *imd, 
			 atol(parv[0]), atol(parv[1]), 
			 atol(parv[2]), atol(parv[3]));
      goto end;
   }

   if (objs[0]->Type() == Po_Imx2duc) {
      Imx2duc* const ims1=(Imx2duc*)objs[0];
      Imx2duc* const ims2=(Imx2duc*)objs[1];
      objd[0]=new Imx2duc(ims1->Bands(), ims1->Size());
      Imx2duc* const imd=(Imx2duc*)objd[0];
      result=PCopyBorder(*ims1, *ims2, *imd, 
			 atol(parv[0]), atol(parv[1]), 
			 atol(parv[2]), atol(parv[3]));
      goto end;
   }

   if (objs[0]->Type() == Po_Imx2dsl) {
      Imx2dsl* const ims1=(Imx2dsl*)objs[0];
      Imx2dsl* const ims2=(Imx2dsl*)objs[1];
      objd[0]=new Imx2dsl(ims1->Bands(), ims1->Size());
      Imx2dsl* const imd=(Imx2dsl*)objd[0];
      result=PCopyBorder(*ims1, *ims2, *imd, 
			 atol(parv[0]), atol(parv[1]), 
			 atol(parv[2]), atol(parv[3]));
      goto end;
   }

   if (objs[0]->Type() == Po_Imx2dsf) {
      Imx2dsf* const ims1=(Imx2dsf*)objs[0];
      Imx2dsf* const ims2=(Imx2dsf*)objs[1];
      objd[0]=new Imx2dsf(ims1->Bands(), ims1->Size());
      Imx2dsf* const imd=(Imx2dsf*)objd[0];
      result=PCopyBorder(*ims1, *ims2, *imd, 
			 atol(parv[0]), atol(parv[1]), 
			 atol(parv[2]), atol(parv[3]));
      goto end;
   }

   if (objs[0]->Type() == Po_Reg2d) {
      Reg2d* const ims1 = (Reg2d*)objs[0];
      Reg2d* const ims2 = (Reg2d*)objs[1];
      objd[0] = new Reg2d(ims1->Size());
      Reg2d* const imd = (Reg2d*)objd[0];
      result = PCopyBorder(*ims1, *ims2, *imd, 
			   atol(parv[0]), atol(parv[1]), 
			   atol(parv[2]), atol(parv[3]));
      goto end;
   }

   if (objs[0]->Type() == Po_Img3duc) {
      Img3duc* const ims1 = (Img3duc*)objs[0];
      Img3duc* const ims2 = (Img3duc*)objs[1];
      objd[0] = new Img3duc(ims1->Size());
      Img3duc* const imd = (Img3duc*)objd[0];
      result = PCopyBorder(*ims1, *ims2, *imd, 
			   atol(parv[0]), atol(parv[1]), 
			   atol(parv[2]), atol(parv[3]), 
			   atol(parv[4]), atol(parv[5]));
      goto end;
   }
   if (objs[0]->Type() == Po_Img3dsl) {
      Img3dsl* const ims1 = (Img3dsl*)objs[0];
      Img3dsl* const ims2 = (Img3dsl*)objs[1];
      objd[0] = new Img3dsl(ims1->Size());
      Img3dsl* const imd = (Img3dsl*)objd[0];
      result = PCopyBorder(*ims1, *ims2, *imd, 
			   atol(parv[0]), atol(parv[1]), 
			   atol(parv[2]), atol(parv[3]), 
			   atol(parv[4]), atol(parv[5]));
      goto end;
   }
   if (objs[0]->Type() == Po_Img3dsf) {
      Img3dsf* const ims1 = (Img3dsf*)objs[0];
      Img3dsf* const ims2 = (Img3dsf*)objs[1];
      objd[0] = new Img3dsf(ims1->Size());
      Img3dsf* const imd = (Img3dsf*)objd[0];
      result = PCopyBorder(*ims1, *ims2, *imd, 
			   atol(parv[0]), atol(parv[1]), 
			   atol(parv[2]), atol(parv[3]), 
			   atol(parv[4]), atol(parv[5]));
      goto end;
   }
   if (objs[0]->Type() == Po_Imc3duc) {
      Imc3duc* const ims1 = (Imc3duc*)objs[0];
      Imc3duc* const ims2 = (Imc3duc*)objs[1];
      objd[0] = new Imc3duc(ims1->Bands(), ims1->Size());
      Imc3duc* const imd = (Imc3duc*)objd[0];
      result = PCopyBorder(*ims1, *ims2, *imd, 
			 (Long)atoi(parv[0]), (Long)atoi(parv[1]), 
			 (Long)atoi(parv[2]), (Long)atoi(parv[3]), 
			 (Long)atoi(parv[4]), (Long)atoi(parv[5]));
      goto end;
   }
   if (objs[0]->Type() == Po_Imc3dsl) {
      Imc3dsl* const ims1 = (Imc3dsl*)objs[0];
      Imc3dsl* const ims2 = (Imc3dsl*)objs[1];
      objd[0] = new Imc3dsl(ims1->Bands(), ims1->Size());
      Imc3dsl* const imd = (Imc3dsl*)objd[0];
      result = PCopyBorder(*ims1, *ims2, *imd, 
			 (Long)atoi(parv[0]), (Long)atoi(parv[1]), 
			 (Long)atoi(parv[2]), (Long)atoi(parv[3]), 
			 (Long)atoi(parv[4]), (Long)atoi(parv[5]));
      goto end;
   }
   if (objs[0]->Type() == Po_Imc3dsf) {
      Imc3dsf* const ims1 = (Imc3dsf*)objs[0];
      Imc3dsf* const ims2 = (Imc3dsf*)objs[1];
      objd[0] = new Imc3dsf(ims1->Bands(), ims1->Size());
      Imc3dsf* const imd = (Imc3dsf*)objd[0];
      result = PCopyBorder(*ims1, *ims2, *imd, 
			 (Long)atoi(parv[0]), (Long)atoi(parv[1]), 
			 (Long)atoi(parv[2]), (Long)atoi(parv[3]), 
			 (Long)atoi(parv[4]), (Long)atoi(parv[5]));
      goto end;
   }
   if (objs[0]->Type() == Po_Imx3duc) {
      Imx3duc* const ims1 = (Imx3duc*)objs[0];
      Imx3duc* const ims2 = (Imx3duc*)objs[1];
      objd[0] = new Imx3duc(ims1->Bands(), ims1->Size());
      Imx3duc* const imd = (Imx3duc*)objd[0];
      result = PCopyBorder(*ims1, *ims2, *imd, 
			 (Long)atoi(parv[0]), (Long)atoi(parv[1]), 
			 (Long)atoi(parv[2]), (Long)atoi(parv[3]), 
			 (Long)atoi(parv[4]), (Long)atoi(parv[5]));
      goto end;
   }
   if (objs[0]->Type() == Po_Imx3dsl) {
      Imx3dsl* const ims1 = (Imx3dsl*)objs[0];
      Imx3dsl* const ims2 = (Imx3dsl*)objs[1];
      objd[0] = new Imx3dsl(ims1->Bands(), ims1->Size());
      Imx3dsl* const imd = (Imx3dsl*)objd[0];
      result = PCopyBorder(*ims1, *ims2, *imd, 
			 (Long)atoi(parv[0]), (Long)atoi(parv[1]), 
			 (Long)atoi(parv[2]), (Long)atoi(parv[3]), 
			 (Long)atoi(parv[4]), (Long)atoi(parv[5]));
      goto end;
   }
   if (objs[0]->Type() == Po_Imx3dsf) {
      Imx3dsf* const ims1 = (Imx3dsf*)objs[0];
      Imx3dsf* const ims2 = (Imx3dsf*)objs[1];
      objd[0] = new Imx3dsf(ims1->Bands(), ims1->Size());
      Imx3dsf* const imd = (Imx3dsf*)objd[0];
      result = PCopyBorder(*ims1, *ims2, *imd, 
			 (Long)atoi(parv[0]), (Long)atoi(parv[1]), 
			 (Long)atoi(parv[2]), (Long)atoi(parv[3]), 
			 (Long)atoi(parv[4]), (Long)atoi(parv[5]));
      goto end;
   }
   if (objs[0]->Type() == Po_Reg3d) {
      Reg3d* const ims1 = (Reg3d*)objs[0];
      Reg3d* const ims2 = (Reg3d*)objs[1];
      objd[0] = new Reg3d(ims1->Size());
      Reg3d* const imd = (Reg3d*)objd[0];
      result = PCopyBorder(*ims1, *ims2, *imd, 
			 atol(parv[0]), atol(parv[1]), 
			 atol(parv[2]), atol(parv[3]), 
			 atol(parv[4]), atol(parv[5]));
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
