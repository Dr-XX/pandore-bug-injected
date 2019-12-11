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
 * @author Régis Clouard - 1997-07-23
 */

#include <pandore.h>
using namespace pandore;

/**
 * @file psetborder.cpp
 * Sets the border valuea to val with dimension (l,h,w).
 */









Errc PSetBorder( const Img2duc &ims, Img2duc &imd, Long ll, Long lr, Long hu, Long hd, Img2duc::ValueType val ) {
   register int x,y;
   
   if (lr>ims.Width() || ll>ims.Width() || hu>ims.Height() || hd>ims.Height() ||
       lr<0 || ll<0 || hu<0 || hd<0)
      return FAILURE;
   
   imd = ims;
   
   for (x=0; x<imd.Width(); x++) {
      for (y=0; y<hd; y++)
	 imd[y][x] = val;
      for (y=0; y<hu; y++)
	 imd[ims.Height()-y-1][x] = val;
   }

   for (y=0; y<imd.Height(); y++) {
      for (x=0; x<ll; x++)
	 imd[y][x] = val;
      for (x=0; x<lr; x++)
	 imd[y][ims.Width()-1-x] = val;
   }
   
   return SUCCESS;
}

Errc PSetBorder( const Img2dsl &ims, Img2dsl &imd, Long ll, Long lr, Long hu, Long hd, Img2dsl::ValueType val ) {
   register int x,y;
   
   if (lr>ims.Width() || ll>ims.Width() || hu>ims.Height() || hd>ims.Height() ||
       lr<0 || ll<0 || hu<0 || hd<0)
      return FAILURE;
   
   imd = ims;
   
   for (x=0; x<imd.Width(); x++) {
      for (y=0; y<hd; y++)
	 imd[y][x] = val;
      for (y=0; y<hu; y++)
	 imd[ims.Height()-y-1][x] = val;
   }

   for (y=0; y<imd.Height(); y++) {
      for (x=0; x<ll; x++)
	 imd[y][x] = val;
      for (x=0; x<lr; x++)
	 imd[y][ims.Width()-1-x] = val;
   }
   
   return SUCCESS;
}

Errc PSetBorder( const Img2dsf &ims, Img2dsf &imd, Long ll, Long lr, Long hu, Long hd, Img2dsf::ValueType val ) {
   register int x,y;
   
   if (lr>ims.Width() || ll>ims.Width() || hu>ims.Height() || hd>ims.Height() ||
       lr<0 || ll<0 || hu<0 || hd<0)
      return FAILURE;
   
   imd = ims;
   
   for (x=0; x<imd.Width(); x++) {
      for (y=0; y<hd; y++)
	 imd[y][x] = val;
      for (y=0; y<hu; y++)
	 imd[ims.Height()-y-1][x] = val;
   }

   for (y=0; y<imd.Height(); y++) {
      for (x=0; x<ll; x++)
	 imd[y][x] = val;
      for (x=0; x<lr; x++)
	 imd[y][ims.Width()-1-x] = val;
   }
   
   return SUCCESS;
}

Errc PSetBorder( const Imc2duc &ims, Imc2duc &imd, Long ll, Long lr, Long hu, Long hd, Imc2duc::ValueType val ) {
   register int x,y,b;
   
   if (lr>ims.Width()/2 || ll>ims.Width()/2 || hu>ims.Height()/2 || hd>ims.Height()/2 ||
       lr<0 || ll<0 || hu<0 || hd<0)
      return FAILURE;
   
   imd = ims;

   for (x=0; x<imd.Width(); x++) {
      for (y=0; y<hu; y++)
	 for (b=0;b<imd.Bands();b++)
	    imd[b][y][x] = val;
      for (y=0; y<hd; y++)
	 for (b=0;b<imd.Bands();b++)
	    imd[b][ims.Height()-y-1][x] = val;
   }

   for (y=0; y<imd.Height(); y++) {
      for (x=0; x<ll; x++)
	 for (b=0;b<imd.Bands();b++)
	    imd[b][y][x] = val;
      for (x=0; x<lr; x++)
	 for (b=0;b<imd.Bands();b++)
	    imd[b][y][ims.Width()-1-x] = val;
   }

   return SUCCESS;
}
Errc PSetBorder( const Imc2dsl &ims, Imc2dsl &imd, Long ll, Long lr, Long hu, Long hd, Imc2dsl::ValueType val ) {
   register int x,y,b;
   
   if (lr>ims.Width()/2 || ll>ims.Width()/2 || hu>ims.Height()/2 || hd>ims.Height()/2 ||
       lr<0 || ll<0 || hu<0 || hd<0)
      return FAILURE;
   
   imd = ims;

   for (x=0; x<imd.Width(); x++) {
      for (y=0; y<hu; y++)
	 for (b=0;b<imd.Bands();b++)
	    imd[b][y][x] = val;
      for (y=0; y<hd; y++)
	 for (b=0;b<imd.Bands();b++)
	    imd[b][ims.Height()-y-1][x] = val;
   }

   for (y=0; y<imd.Height(); y++) {
      for (x=0; x<ll; x++)
	 for (b=0;b<imd.Bands();b++)
	    imd[b][y][x] = val;
      for (x=0; x<lr; x++)
	 for (b=0;b<imd.Bands();b++)
	    imd[b][y][ims.Width()-1-x] = val;
   }

   return SUCCESS;
}
Errc PSetBorder( const Imc2dsf &ims, Imc2dsf &imd, Long ll, Long lr, Long hu, Long hd, Imc2dsf::ValueType val ) {
   register int x,y,b;
   
   if (lr>ims.Width()/2 || ll>ims.Width()/2 || hu>ims.Height()/2 || hd>ims.Height()/2 ||
       lr<0 || ll<0 || hu<0 || hd<0)
      return FAILURE;
   
   imd = ims;

   for (x=0; x<imd.Width(); x++) {
      for (y=0; y<hu; y++)
	 for (b=0;b<imd.Bands();b++)
	    imd[b][y][x] = val;
      for (y=0; y<hd; y++)
	 for (b=0;b<imd.Bands();b++)
	    imd[b][ims.Height()-y-1][x] = val;
   }

   for (y=0; y<imd.Height(); y++) {
      for (x=0; x<ll; x++)
	 for (b=0;b<imd.Bands();b++)
	    imd[b][y][x] = val;
      for (x=0; x<lr; x++)
	 for (b=0;b<imd.Bands();b++)
	    imd[b][y][ims.Width()-1-x] = val;
   }

   return SUCCESS;
}
Errc PSetBorder( const Imx2duc &ims, Imx2duc &imd, Long ll, Long lr, Long hu, Long hd, Imx2duc::ValueType val ) {
   register int x,y,b;
   
   if (lr>ims.Width()/2 || ll>ims.Width()/2 || hu>ims.Height()/2 || hd>ims.Height()/2 ||
       lr<0 || ll<0 || hu<0 || hd<0)
      return FAILURE;
   
   imd = ims;

   for (x=0; x<imd.Width(); x++) {
      for (y=0; y<hu; y++)
	 for (b=0;b<imd.Bands();b++)
	    imd[b][y][x] = val;
      for (y=0; y<hd; y++)
	 for (b=0;b<imd.Bands();b++)
	    imd[b][ims.Height()-y-1][x] = val;
   }

   for (y=0; y<imd.Height(); y++) {
      for (x=0; x<ll; x++)
	 for (b=0;b<imd.Bands();b++)
	    imd[b][y][x] = val;
      for (x=0; x<lr; x++)
	 for (b=0;b<imd.Bands();b++)
	    imd[b][y][ims.Width()-1-x] = val;
   }

   return SUCCESS;
}
Errc PSetBorder( const Imx2dsl &ims, Imx2dsl &imd, Long ll, Long lr, Long hu, Long hd, Imx2dsl::ValueType val ) {
   register int x,y,b;
   
   if (lr>ims.Width()/2 || ll>ims.Width()/2 || hu>ims.Height()/2 || hd>ims.Height()/2 ||
       lr<0 || ll<0 || hu<0 || hd<0)
      return FAILURE;
   
   imd = ims;

   for (x=0; x<imd.Width(); x++) {
      for (y=0; y<hu; y++)
	 for (b=0;b<imd.Bands();b++)
	    imd[b][y][x] = val;
      for (y=0; y<hd; y++)
	 for (b=0;b<imd.Bands();b++)
	    imd[b][ims.Height()-y-1][x] = val;
   }

   for (y=0; y<imd.Height(); y++) {
      for (x=0; x<ll; x++)
	 for (b=0;b<imd.Bands();b++)
	    imd[b][y][x] = val;
      for (x=0; x<lr; x++)
	 for (b=0;b<imd.Bands();b++)
	    imd[b][y][ims.Width()-1-x] = val;
   }

   return SUCCESS;
}
Errc PSetBorder( const Imx2dsf &ims, Imx2dsf &imd, Long ll, Long lr, Long hu, Long hd, Imx2dsf::ValueType val ) {
   register int x,y,b;
   
   if (lr>ims.Width()/2 || ll>ims.Width()/2 || hu>ims.Height()/2 || hd>ims.Height()/2 ||
       lr<0 || ll<0 || hu<0 || hd<0)
      return FAILURE;
   
   imd = ims;

   for (x=0; x<imd.Width(); x++) {
      for (y=0; y<hu; y++)
	 for (b=0;b<imd.Bands();b++)
	    imd[b][y][x] = val;
      for (y=0; y<hd; y++)
	 for (b=0;b<imd.Bands();b++)
	    imd[b][ims.Height()-y-1][x] = val;
   }

   for (y=0; y<imd.Height(); y++) {
      for (x=0; x<ll; x++)
	 for (b=0;b<imd.Bands();b++)
	    imd[b][y][x] = val;
      for (x=0; x<lr; x++)
	 for (b=0;b<imd.Bands();b++)
	    imd[b][y][ims.Width()-1-x] = val;
   }

   return SUCCESS;
}
Errc PSetBorder( const Reg2d &ims, Reg2d &imd, Long ll, Long lr, Long hu, Long hd, Reg2d::ValueType val ) {
   register int x,y;
   
   if (lr>ims.Width() || ll>ims.Width() || hu>ims.Height() || hd>ims.Height() ||
       lr<0 || ll<0 || hu<0 || hd<0)
      return FAILURE;
   
   imd = ims;
   
   for (x=0; x<imd.Width(); x++) {
      for (y=0; y<hd; y++)
	 imd(y,x) = val;
      for (y=0; y<hu; y++)
	 imd(ims.Height()-y-1,x) = val;
   }

   for (y=0; y<imd.Height(); y++) {
      for (x=0; x<ll; x++)
	 imd(y,x) = val;
      for (x=0; x<lr; x++)
	 imd(y,ims.Width()-1-x) = val;
   }
   if (val>ims.Labels()) {
      imd.Labels(val);
   } else {
      imd.Labels(ims.Labels());
   }
   return SUCCESS;
}


Errc PSetBorder( const Img3duc &ims, Img3duc &imd, Long ll, Long lr, Long hu, Long hd, Long df, Long db, Img3duc::ValueType val ) {
   register int	x,y,z;

   if (lr>ims.Width() || ll>ims.Width() || hu>ims.Height() || hd>ims.Height() || df>ims.Depth() || db>ims.Depth() ||
       lr<0 || ll<0 || hu<0 || hd<0 || df<0 || db<0)
      return FAILURE;
   
   imd = ims;
   for (y=0; y<imd.Height(); y++)
      for (x=0; x<imd.Width(); x++) {
	 for (z=0; z<df; z++)
	    imd[z][y][x] = val;
	 for (z=0; z<db; z++)
	    imd[imd.Depth()-1-z][y][x] = val;
      }
   for (z=0; z<imd.Depth(); z++)
      for (x=0; x<imd.Width(); x++) {
	 for (y=0; y<hu; y++) 
	    imd[z][y][x] = val;
	 for (y=0; y<hd; y++) 
	    imd[z][imd.Height()-1-y][x] = val;
      }
   for (z=0; z<imd.Depth(); z++)
      for (y=0; y<imd.Height(); y++) {
	 for (x=0; x<ll; x++)
	    imd[z][y][x] = val;
	 for (x=0; x<lr; x++)
	    imd[z][y][imd.Width()-1-x] = val;
      }
   return SUCCESS;
}
Errc PSetBorder( const Img3dsl &ims, Img3dsl &imd, Long ll, Long lr, Long hu, Long hd, Long df, Long db, Img3dsl::ValueType val ) {
   register int	x,y,z;

   if (lr>ims.Width() || ll>ims.Width() || hu>ims.Height() || hd>ims.Height() || df>ims.Depth() || db>ims.Depth() ||
       lr<0 || ll<0 || hu<0 || hd<0 || df<0 || db<0)
      return FAILURE;
   
   imd = ims;
   for (y=0; y<imd.Height(); y++)
      for (x=0; x<imd.Width(); x++) {
	 for (z=0; z<df; z++)
	    imd[z][y][x] = val;
	 for (z=0; z<db; z++)
	    imd[imd.Depth()-1-z][y][x] = val;
      }
   for (z=0; z<imd.Depth(); z++)
      for (x=0; x<imd.Width(); x++) {
	 for (y=0; y<hu; y++) 
	    imd[z][y][x] = val;
	 for (y=0; y<hd; y++) 
	    imd[z][imd.Height()-1-y][x] = val;
      }
   for (z=0; z<imd.Depth(); z++)
      for (y=0; y<imd.Height(); y++) {
	 for (x=0; x<ll; x++)
	    imd[z][y][x] = val;
	 for (x=0; x<lr; x++)
	    imd[z][y][imd.Width()-1-x] = val;
      }
   return SUCCESS;
}
Errc PSetBorder( const Img3dsf &ims, Img3dsf &imd, Long ll, Long lr, Long hu, Long hd, Long df, Long db, Img3dsf::ValueType val ) {
   register int	x,y,z;

   if (lr>ims.Width() || ll>ims.Width() || hu>ims.Height() || hd>ims.Height() || df>ims.Depth() || db>ims.Depth() ||
       lr<0 || ll<0 || hu<0 || hd<0 || df<0 || db<0)
      return FAILURE;
   
   imd = ims;
   for (y=0; y<imd.Height(); y++)
      for (x=0; x<imd.Width(); x++) {
	 for (z=0; z<df; z++)
	    imd[z][y][x] = val;
	 for (z=0; z<db; z++)
	    imd[imd.Depth()-1-z][y][x] = val;
      }
   for (z=0; z<imd.Depth(); z++)
      for (x=0; x<imd.Width(); x++) {
	 for (y=0; y<hu; y++) 
	    imd[z][y][x] = val;
	 for (y=0; y<hd; y++) 
	    imd[z][imd.Height()-1-y][x] = val;
      }
   for (z=0; z<imd.Depth(); z++)
      for (y=0; y<imd.Height(); y++) {
	 for (x=0; x<ll; x++)
	    imd[z][y][x] = val;
	 for (x=0; x<lr; x++)
	    imd[z][y][imd.Width()-1-x] = val;
      }
   return SUCCESS;
}
Errc PSetBorder( const Imc3duc &ims, Imc3duc &imd, Long ll, Long lr, Long hu, Long hd, Long df, Long db, Imc3duc::ValueType val ) {
   register int	x,y,z,b;

   if (lr>ims.Width() || ll>ims.Width() || hu>ims.Height() || hd>ims.Height() || df>ims.Depth() || db>ims.Depth() ||
       lr<0 || ll<0 || hu<0 || hd<0 || df<0 || db<0)
      return FAILURE;
   
   imd = ims;
   for (y=0; y<imd.Height(); y++)
      for (x=0; x<imd.Width(); x++) {
	 for (z=0; z<df; z++)
	    for (b=0;b<imd.Bands();b++)
	       imd[b][z][y][x] = val;
	 for (z=0; z<db; z++)
	    for (b=0;b<imd.Bands();b++)
	       imd[b][imd.Depth()-1-z][y][x] = val;
      }
   
   for (z=0; z<imd.Depth(); z++)
      for (x=0; x<imd.Width(); x++) {
	 for (y=0; y<hu; y++)
	    for (b=0;b<imd.Bands();b++)
	       imd[b][z][y][x] = val;
	 for (y=0; y<hd; y++)
	    for (b=0;b<imd.Bands();b++)
	       imd[b][z][imd.Height()-1-y][x] = val;
      }
   
   for (z=0; z<imd.Depth(); z++)
      for (y=0; y<imd.Height(); y++) {
	 for (x=0; x<ll; x++)
	    for (b=0;b<imd.Bands();b++)
	       imd[b][z][y][x] = val;
	 for (x=0; x<lr; x++)
	    for (b=0;b<imd.Bands();b++)
	       imd[b][z][y][imd.Width()-1-x] = val;
      }
   return SUCCESS;
}

Errc PSetBorder( const Imc3dsl &ims, Imc3dsl &imd, Long ll, Long lr, Long hu, Long hd, Long df, Long db, Imc3dsl::ValueType val ) {
   register int	x,y,z,b;

   if (lr>ims.Width() || ll>ims.Width() || hu>ims.Height() || hd>ims.Height() || df>ims.Depth() || db>ims.Depth() ||
       lr<0 || ll<0 || hu<0 || hd<0 || df<0 || db<0)
      return FAILURE;
   
   imd = ims;
   for (y=0; y<imd.Height(); y++)
      for (x=0; x<imd.Width(); x++) {
	 for (z=0; z<df; z++)
	    for (b=0;b<imd.Bands();b++)
	       imd[b][z][y][x] = val;
	 for (z=0; z<db; z++)
	    for (b=0;b<imd.Bands();b++)
	       imd[b][imd.Depth()-1-z][y][x] = val;
      }
   
   for (z=0; z<imd.Depth(); z++)
      for (x=0; x<imd.Width(); x++) {
	 for (y=0; y<hu; y++)
	    for (b=0;b<imd.Bands();b++)
	       imd[b][z][y][x] = val;
	 for (y=0; y<hd; y++)
	    for (b=0;b<imd.Bands();b++)
	       imd[b][z][imd.Height()-1-y][x] = val;
      }
   
   for (z=0; z<imd.Depth(); z++)
      for (y=0; y<imd.Height(); y++) {
	 for (x=0; x<ll; x++)
	    for (b=0;b<imd.Bands();b++)
	       imd[b][z][y][x] = val;
	 for (x=0; x<lr; x++)
	    for (b=0;b<imd.Bands();b++)
	       imd[b][z][y][imd.Width()-1-x] = val;
      }
   return SUCCESS;
}

Errc PSetBorder( const Imc3dsf &ims, Imc3dsf &imd, Long ll, Long lr, Long hu, Long hd, Long df, Long db, Imc3dsf::ValueType val ) {
   register int	x,y,z,b;

   if (lr>ims.Width() || ll>ims.Width() || hu>ims.Height() || hd>ims.Height() || df>ims.Depth() || db>ims.Depth() ||
       lr<0 || ll<0 || hu<0 || hd<0 || df<0 || db<0)
      return FAILURE;
   
   imd = ims;
   for (y=0; y<imd.Height(); y++)
      for (x=0; x<imd.Width(); x++) {
	 for (z=0; z<df; z++)
	    for (b=0;b<imd.Bands();b++)
	       imd[b][z][y][x] = val;
	 for (z=0; z<db; z++)
	    for (b=0;b<imd.Bands();b++)
	       imd[b][imd.Depth()-1-z][y][x] = val;
      }
   
   for (z=0; z<imd.Depth(); z++)
      for (x=0; x<imd.Width(); x++) {
	 for (y=0; y<hu; y++)
	    for (b=0;b<imd.Bands();b++)
	       imd[b][z][y][x] = val;
	 for (y=0; y<hd; y++)
	    for (b=0;b<imd.Bands();b++)
	       imd[b][z][imd.Height()-1-y][x] = val;
      }
   
   for (z=0; z<imd.Depth(); z++)
      for (y=0; y<imd.Height(); y++) {
	 for (x=0; x<ll; x++)
	    for (b=0;b<imd.Bands();b++)
	       imd[b][z][y][x] = val;
	 for (x=0; x<lr; x++)
	    for (b=0;b<imd.Bands();b++)
	       imd[b][z][y][imd.Width()-1-x] = val;
      }
   return SUCCESS;
}

Errc PSetBorder( const Imx3duc &ims, Imx3duc &imd, Long ll, Long lr, Long hu, Long hd, Long df, Long db, Imx3duc::ValueType val ) {
   register int	x,y,z,b;

   if (lr>ims.Width() || ll>ims.Width() || hu>ims.Height() || hd>ims.Height() || df>ims.Depth() || db>ims.Depth() ||
       lr<0 || ll<0 || hu<0 || hd<0 || df<0 || db<0)
      return FAILURE;
   
   imd = ims;
   for (y=0; y<imd.Height(); y++)
      for (x=0; x<imd.Width(); x++) {
	 for (z=0; z<df; z++)
	    for (b=0;b<imd.Bands();b++)
	       imd[b][z][y][x] = val;
	 for (z=0; z<db; z++)
	    for (b=0;b<imd.Bands();b++)
	       imd[b][imd.Depth()-1-z][y][x] = val;
      }
   
   for (z=0; z<imd.Depth(); z++)
      for (x=0; x<imd.Width(); x++) {
	 for (y=0; y<hu; y++)
	    for (b=0;b<imd.Bands();b++)
	       imd[b][z][y][x] = val;
	 for (y=0; y<hd; y++)
	    for (b=0;b<imd.Bands();b++)
	       imd[b][z][imd.Height()-1-y][x] = val;
      }
   
   for (z=0; z<imd.Depth(); z++)
      for (y=0; y<imd.Height(); y++) {
	 for (x=0; x<ll; x++)
	    for (b=0;b<imd.Bands();b++)
	       imd[b][z][y][x] = val;
	 for (x=0; x<lr; x++)
	    for (b=0;b<imd.Bands();b++)
	       imd[b][z][y][imd.Width()-1-x] = val;
      }
   return SUCCESS;
}

Errc PSetBorder( const Imx3dsl &ims, Imx3dsl &imd, Long ll, Long lr, Long hu, Long hd, Long df, Long db, Imx3dsl::ValueType val ) {
   register int	x,y,z,b;

   if (lr>ims.Width() || ll>ims.Width() || hu>ims.Height() || hd>ims.Height() || df>ims.Depth() || db>ims.Depth() ||
       lr<0 || ll<0 || hu<0 || hd<0 || df<0 || db<0)
      return FAILURE;
   
   imd = ims;
   for (y=0; y<imd.Height(); y++)
      for (x=0; x<imd.Width(); x++) {
	 for (z=0; z<df; z++)
	    for (b=0;b<imd.Bands();b++)
	       imd[b][z][y][x] = val;
	 for (z=0; z<db; z++)
	    for (b=0;b<imd.Bands();b++)
	       imd[b][imd.Depth()-1-z][y][x] = val;
      }
   
   for (z=0; z<imd.Depth(); z++)
      for (x=0; x<imd.Width(); x++) {
	 for (y=0; y<hu; y++)
	    for (b=0;b<imd.Bands();b++)
	       imd[b][z][y][x] = val;
	 for (y=0; y<hd; y++)
	    for (b=0;b<imd.Bands();b++)
	       imd[b][z][imd.Height()-1-y][x] = val;
      }
   
   for (z=0; z<imd.Depth(); z++)
      for (y=0; y<imd.Height(); y++) {
	 for (x=0; x<ll; x++)
	    for (b=0;b<imd.Bands();b++)
	       imd[b][z][y][x] = val;
	 for (x=0; x<lr; x++)
	    for (b=0;b<imd.Bands();b++)
	       imd[b][z][y][imd.Width()-1-x] = val;
      }
   return SUCCESS;
}

Errc PSetBorder( const Imx3dsf &ims, Imx3dsf &imd, Long ll, Long lr, Long hu, Long hd, Long df, Long db, Imx3dsf::ValueType val ) {
   register int	x,y,z,b;

   if (lr>ims.Width() || ll>ims.Width() || hu>ims.Height() || hd>ims.Height() || df>ims.Depth() || db>ims.Depth() ||
       lr<0 || ll<0 || hu<0 || hd<0 || df<0 || db<0)
      return FAILURE;
   
   imd = ims;
   for (y=0; y<imd.Height(); y++)
      for (x=0; x<imd.Width(); x++) {
	 for (z=0; z<df; z++)
	    for (b=0;b<imd.Bands();b++)
	       imd[b][z][y][x] = val;
	 for (z=0; z<db; z++)
	    for (b=0;b<imd.Bands();b++)
	       imd[b][imd.Depth()-1-z][y][x] = val;
      }
   
   for (z=0; z<imd.Depth(); z++)
      for (x=0; x<imd.Width(); x++) {
	 for (y=0; y<hu; y++)
	    for (b=0;b<imd.Bands();b++)
	       imd[b][z][y][x] = val;
	 for (y=0; y<hd; y++)
	    for (b=0;b<imd.Bands();b++)
	       imd[b][z][imd.Height()-1-y][x] = val;
      }
   
   for (z=0; z<imd.Depth(); z++)
      for (y=0; y<imd.Height(); y++) {
	 for (x=0; x<ll; x++)
	    for (b=0;b<imd.Bands();b++)
	       imd[b][z][y][x] = val;
	 for (x=0; x<lr; x++)
	    for (b=0;b<imd.Bands();b++)
	       imd[b][z][y][imd.Width()-1-x] = val;
      }
   return SUCCESS;
}

Errc PSetBorder( const Reg3d &ims, Reg3d &imd, Long ll, Long lr, Long hu, Long hd, Long df, Long db, Reg3d::ValueType val ) {
   register int	x,y,z;

   if (lr>ims.Width() || ll>ims.Width() || hu>ims.Height() || hd>ims.Height() || df>ims.Depth() || db>ims.Depth() ||
       lr<0 || ll<0 || hu<0 || hd<0 || df<0 || db<0) {
      return FAILURE;
   }
   imd = ims;
   for (y=0; y<imd.Height(); y++) {
      for (x=0; x<imd.Width(); x++) {
	 for (z=0; z<df; z++) {
	    imd(z,y,x) = val;
	 }
	 for (z=0; z<db; z++) {
	    imd(imd.Depth()-1-z,y,x) = val;
	 }
      }
   }
   for (z=0; z<imd.Depth(); z++) {
      for (x=0; x<imd.Width(); x++) {
	 for (y=0; y<hu; y++) {
	    imd(z,y,x) = val;
	 }
	 for (y=0; y<hd; y++) {
	    imd(z,imd.Height()-1-y,x) = val;
	 }
      }
   }
   for (z=0; z<imd.Depth(); z++) {
      for (y=0; y<imd.Height(); y++) {
	 for (x=0; x<ll; x++) {
	    imd(z,y,x) = val;
	 }
	 for (x=0; x<lr; x++) {
	    imd(z,y,imd.Width()-1-x) = val;
	 }
      }
   }
   if (val > ims.Labels()) {
      imd.Labels(val);
   } else {
      imd.Labels(ims.Labels());
   }
   return SUCCESS;
}


#ifdef MAIN
#define	USAGE	"usage: %s widthr widthl heightu heightb depthf depthb value [im_in|-] [im_out|-]"
#define	PARC	7
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
      Img2duc* const ims=(Img2duc*)objs[0];
      objd[0]=new Img2duc(ims->Size());
      Img2duc* const imd=(Img2duc*)objd[0];
      result=PSetBorder(*ims,*imd,atol(parv[0]),atol(parv[1]),
		     atol(parv[2]),atol(parv[3]),
		     (Img2duc::ValueType)atof(parv[6]));
      goto end;
   }

   if (objs[0]->Type() == Po_Img2dsl) {
      Img2dsl* const ims=(Img2dsl*)objs[0];
      objd[0]=new Img2dsl(ims->Size());
      Img2dsl* const imd=(Img2dsl*)objd[0];
      result=PSetBorder(*ims,*imd,atol(parv[0]),atol(parv[1]),
		     atol(parv[2]),atol(parv[3]),
		     (Img2dsl::ValueType)atof(parv[6]));
      goto end;
   }

   if (objs[0]->Type() == Po_Img2dsf) {
      Img2dsf* const ims=(Img2dsf*)objs[0];
      objd[0]=new Img2dsf(ims->Size());
      Img2dsf* const imd=(Img2dsf*)objd[0];
      result=PSetBorder(*ims,*imd,atol(parv[0]),atol(parv[1]),
		     atol(parv[2]),atol(parv[3]),
		     (Img2dsf::ValueType)atof(parv[6]));
      goto end;
   }

   if (objs[0]->Type() == Po_Imc2duc) {
      Imc2duc* const ims=(Imc2duc*)objs[0];
      objd[0]=new Imc2duc(ims->Bands(),ims->Size());
      Imc2duc* const imd=(Imc2duc*)objd[0];
      result=PSetBorder(*ims,*imd,atol(parv[0]),atol(parv[1]),
		     atol(parv[2]),atol(parv[3]),
		     (Imc2duc::ValueType)atof(parv[6]));
      goto end;
   }

   if (objs[0]->Type() == Po_Imc2dsl) {
      Imc2dsl* const ims=(Imc2dsl*)objs[0];
      objd[0]=new Imc2dsl(ims->Bands(),ims->Size());
      Imc2dsl* const imd=(Imc2dsl*)objd[0];
      result=PSetBorder(*ims,*imd,atol(parv[0]),atol(parv[1]),
		     atol(parv[2]),atol(parv[3]),
		     (Imc2dsl::ValueType)atof(parv[6]));
      goto end;
   }

   if (objs[0]->Type() == Po_Imc2dsf) {
      Imc2dsf* const ims=(Imc2dsf*)objs[0];
      objd[0]=new Imc2dsf(ims->Bands(),ims->Size());
      Imc2dsf* const imd=(Imc2dsf*)objd[0];
      result=PSetBorder(*ims,*imd,atol(parv[0]),atol(parv[1]),
		     atol(parv[2]),atol(parv[3]),
		     (Imc2dsf::ValueType)atof(parv[6]));
      goto end;
   }

   if (objs[0]->Type() == Po_Imx2duc) {
      Imx2duc* const ims=(Imx2duc*)objs[0];
      objd[0]=new Imx2duc(ims->Bands(),ims->Size());
      Imx2duc* const imd=(Imx2duc*)objd[0];
      result=PSetBorder(*ims,*imd,atol(parv[0]),atol(parv[1]),
		     atol(parv[2]),atol(parv[3]),
		     (Imx2duc::ValueType)atof(parv[6]));
      goto end;
   }

   if (objs[0]->Type() == Po_Imx2dsl) {
      Imx2dsl* const ims=(Imx2dsl*)objs[0];
      objd[0]=new Imx2dsl(ims->Bands(),ims->Size());
      Imx2dsl* const imd=(Imx2dsl*)objd[0];
      result=PSetBorder(*ims,*imd,atol(parv[0]),atol(parv[1]),
		     atol(parv[2]),atol(parv[3]),
		     (Imx2dsl::ValueType)atof(parv[6]));
      goto end;
   }

   if (objs[0]->Type() == Po_Imx2dsf) {
      Imx2dsf* const ims=(Imx2dsf*)objs[0];
      objd[0]=new Imx2dsf(ims->Bands(),ims->Size());
      Imx2dsf* const imd=(Imx2dsf*)objd[0];
      result=PSetBorder(*ims,*imd,atol(parv[0]),atol(parv[1]),
		     atol(parv[2]),atol(parv[3]),
		     (Imx2dsf::ValueType)atof(parv[6]));
      goto end;
   }

   if (objs[0]->Type() == Po_Reg2d) {
      Reg2d* const ims=(Reg2d*)objs[0];
      objd[0]=new Reg2d(ims->Size());
      Reg2d* const imd=(Reg2d*)objd[0];
      result=PSetBorder(*ims,*imd,atol(parv[0]),atol(parv[1]),
		     atol(parv[2]),atol(parv[3]),
		     (Reg2d::ValueType)atof(parv[6]));
      goto end;
   }

   if (objs[0]->Type() == Po_Img3duc) {
      Img3duc* const ims=(Img3duc*)objs[0];
      objd[0]=new Img3duc(ims->Size());
      Img3duc* const imd=(Img3duc*)objd[0];
      result=PSetBorder(*ims,*imd,atol(parv[0]),atol(parv[2]),
		     atol(parv[2]),atol(parv[3]),
		     atol(parv[4]),atol(parv[5]),
		     (Img3duc::ValueType)atof(parv[6]));
      goto end;
   }
   if (objs[0]->Type() == Po_Img3dsl) {
      Img3dsl* const ims=(Img3dsl*)objs[0];
      objd[0]=new Img3dsl(ims->Size());
      Img3dsl* const imd=(Img3dsl*)objd[0];
      result=PSetBorder(*ims,*imd,atol(parv[0]),atol(parv[2]),
		     atol(parv[2]),atol(parv[3]),
		     atol(parv[4]),atol(parv[5]),
		     (Img3dsl::ValueType)atof(parv[6]));
      goto end;
   }
   if (objs[0]->Type() == Po_Img3dsf) {
      Img3dsf* const ims=(Img3dsf*)objs[0];
      objd[0]=new Img3dsf(ims->Size());
      Img3dsf* const imd=(Img3dsf*)objd[0];
      result=PSetBorder(*ims,*imd,atol(parv[0]),atol(parv[2]),
		     atol(parv[2]),atol(parv[3]),
		     atol(parv[4]),atol(parv[5]),
		     (Img3dsf::ValueType)atof(parv[6]));
      goto end;
   }
   if (objs[0]->Type() == Po_Imc3duc) {
      Imc3duc* const ims=(Imc3duc*)objs[0];
      objd[0]=new Imc3duc(ims->Bands(),ims->Size());
      Imc3duc* const imd=(Imc3duc*)objd[0];
      result=PSetBorder(*ims,*imd,(Long)atoi(parv[0]),(Long)atoi(parv[1]),
		     (Long)atoi(parv[2]),(Long)atoi(parv[3]),
		     (Long)atoi(parv[4]),(Long)atoi(parv[5]),
		     (Imc3duc::ValueType)atoi(parv[6]));
      goto end;
   }
   if (objs[0]->Type() == Po_Imc3dsl) {
      Imc3dsl* const ims=(Imc3dsl*)objs[0];
      objd[0]=new Imc3dsl(ims->Bands(),ims->Size());
      Imc3dsl* const imd=(Imc3dsl*)objd[0];
      result=PSetBorder(*ims,*imd,(Long)atoi(parv[0]),(Long)atoi(parv[1]),
		     (Long)atoi(parv[2]),(Long)atoi(parv[3]),
		     (Long)atoi(parv[4]),(Long)atoi(parv[5]),
		     (Imc3dsl::ValueType)atoi(parv[6]));
      goto end;
   }
   if (objs[0]->Type() == Po_Imc3dsf) {
      Imc3dsf* const ims=(Imc3dsf*)objs[0];
      objd[0]=new Imc3dsf(ims->Bands(),ims->Size());
      Imc3dsf* const imd=(Imc3dsf*)objd[0];
      result=PSetBorder(*ims,*imd,(Long)atoi(parv[0]),(Long)atoi(parv[1]),
		     (Long)atoi(parv[2]),(Long)atoi(parv[3]),
		     (Long)atoi(parv[4]),(Long)atoi(parv[5]),
		     (Imc3dsf::ValueType)atoi(parv[6]));
      goto end;
   }
   if (objs[0]->Type() == Po_Imx3duc) {
      Imx3duc* const ims=(Imx3duc*)objs[0];
      objd[0]=new Imx3duc(ims->Bands(),ims->Size());
      Imx3duc* const imd=(Imx3duc*)objd[0];
      result=PSetBorder(*ims,*imd,(Long)atoi(parv[0]),(Long)atoi(parv[1]),
		     (Long)atoi(parv[2]),(Long)atoi(parv[3]),
		     (Long)atoi(parv[4]),(Long)atoi(parv[5]),
		     (Imx3duc::ValueType)atoi(parv[6]));
      goto end;
   }
   if (objs[0]->Type() == Po_Imx3dsl) {
      Imx3dsl* const ims=(Imx3dsl*)objs[0];
      objd[0]=new Imx3dsl(ims->Bands(),ims->Size());
      Imx3dsl* const imd=(Imx3dsl*)objd[0];
      result=PSetBorder(*ims,*imd,(Long)atoi(parv[0]),(Long)atoi(parv[1]),
		     (Long)atoi(parv[2]),(Long)atoi(parv[3]),
		     (Long)atoi(parv[4]),(Long)atoi(parv[5]),
		     (Imx3dsl::ValueType)atoi(parv[6]));
      goto end;
   }
   if (objs[0]->Type() == Po_Imx3dsf) {
      Imx3dsf* const ims=(Imx3dsf*)objs[0];
      objd[0]=new Imx3dsf(ims->Bands(),ims->Size());
      Imx3dsf* const imd=(Imx3dsf*)objd[0];
      result=PSetBorder(*ims,*imd,(Long)atoi(parv[0]),(Long)atoi(parv[1]),
		     (Long)atoi(parv[2]),(Long)atoi(parv[3]),
		     (Long)atoi(parv[4]),(Long)atoi(parv[5]),
		     (Imx3dsf::ValueType)atoi(parv[6]));
      goto end;
   }
   if (objs[0]->Type() == Po_Reg3d) {
      Reg3d* const ims=(Reg3d*)objs[0];
      objd[0]=new Reg3d(ims->Size());
      Reg3d* const imd=(Reg3d*)objd[0];
      result=PSetBorder(*ims,*imd,(Long)atoi(parv[0]),(Long)atoi(parv[1]),
		     (Long)atoi(parv[2]),(Long)atoi(parv[3]),
		     (Long)atoi(parv[4]),(Long)atoi(parv[5]),
		     (Reg3d::ValueType)atoi(parv[6]));
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
