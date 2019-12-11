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
 * @author Regis Clouard - 2003-05-23
 * @author Regis Clouard - 2004-01-22 (new paremeters order)
 */

#include <pandore.h>
using namespace pandore;

/**
 * @file phitormiss.cpp
 * Morphological Hit or Miss transforamtion.
 */



/*
 * Lexicographic order between 2 colors (cx, cy, cz) and (cx1, cy1, cz1)
 */
template<typename T>
static bool Greater(T cx, T cy, T cz, T cx1, T cy1, T cz1) {
   if (cx1 < cx) {
       return true;
   } else if (cx1 > cx) {
      return false;
   }
   if (cy1 < cy) {
       return true;
   } else if (cy1 > cy) {
      return false;
   }
   if (cz1 < cz) {
       return true;
   } else if (cz1 > cz) {
      return false;
   } else {
      return false;
   }
}
template<typename T>
static bool Lower(T cx1, T cy1, T cz1, T cx, T cy, T cz) {
   if (cx1 < cx) {
      return true;
   } else if (cx1 > cx) {
      return false;
   }
   if (cy1 < cy) {
      return true;
   } else if (cy1 > cy) {
      return false;
   }
   if (cz1 < cz) {
      return true;
   } else if (cz1 > cz) {
      return false;
   } else {
      return false;
   }
}




Errc PHitOrMiss( const Img2duc &ims, const Img2duc &imse1, const Img2duc &imse2, Img2duc &imd) {
   Point2d p, p1, p2;
   Img2duc::ValueType val = 0;
   
   Point2d shift1(imse1.Size() / 2);
   Point2d shift2(imse2.Size() / 2);

   for (p.y = 0; p.y < ims.Height(); p.y++) {
      for (p.x = 0; p.x < ims.Width(); p.x++) {
	 // erosion of with the SE1
	 Img2duc::ValueType min = Limits<Img2duc::ValueType>::max();
	 for (p1.y = 0; p1.y < imse1.Height(); p1.y++) {
	    for (p1.x = 0; p1.x < imse1.Width(); p1.x++) {
	       p2 = p + p1 - shift1;
	       if (ims.Hold(p2) && imse1[p1] && ((val = ims[p2]) < min)) {
		  min = val;
	       }
	    }
	 }
	 // dilatation of with the SE2
	 Img2duc::ValueType max = Limits<Img2duc::ValueType>::min();
	 for (p1.y = 0; p1.y < imse2.Height(); p1.y++)
	    for (p1.x = 0; p1.x < imse2.Width(); p1.x++) {
	       p2 = p + p1 - shift2;
	       if (ims.Hold(p2) && imse2[p1] && ((val = ims[p2]) > max)) {
		  max = val;
	       }
	    }
	 if (min > max) {
	    imd[p] = min - max;
	 } else {
	    imd[p] = 0;
	 }
      }
   }
   return SUCCESS;
}

Errc PHitOrMiss( const Img2dsl &ims, const Img2duc &imse1, const Img2duc &imse2, Img2dsl &imd) {
   Point2d p, p1, p2;
   Img2dsl::ValueType val = 0;
   
   Point2d shift1(imse1.Size() / 2);
   Point2d shift2(imse2.Size() / 2);

   for (p.y = 0; p.y < ims.Height(); p.y++) {
      for (p.x = 0; p.x < ims.Width(); p.x++) {
	 // erosion of with the SE1
	 Img2dsl::ValueType min = Limits<Img2dsl::ValueType>::max();
	 for (p1.y = 0; p1.y < imse1.Height(); p1.y++) {
	    for (p1.x = 0; p1.x < imse1.Width(); p1.x++) {
	       p2 = p + p1 - shift1;
	       if (ims.Hold(p2) && imse1[p1] && ((val = ims[p2]) < min)) {
		  min = val;
	       }
	    }
	 }
	 // dilatation of with the SE2
	 Img2dsl::ValueType max = Limits<Img2dsl::ValueType>::min();
	 for (p1.y = 0; p1.y < imse2.Height(); p1.y++)
	    for (p1.x = 0; p1.x < imse2.Width(); p1.x++) {
	       p2 = p + p1 - shift2;
	       if (ims.Hold(p2) && imse2[p1] && ((val = ims[p2]) > max)) {
		  max = val;
	       }
	    }
	 if (min > max) {
	    imd[p] = min - max;
	 } else {
	    imd[p] = 0;
	 }
      }
   }
   return SUCCESS;
}

Errc PHitOrMiss( const Img2dsf &ims, const Img2duc &imse1, const Img2duc &imse2, Img2dsf &imd) {
   Point2d p, p1, p2;
   Img2dsf::ValueType val = 0;
   
   Point2d shift1(imse1.Size() / 2);
   Point2d shift2(imse2.Size() / 2);

   for (p.y = 0; p.y < ims.Height(); p.y++) {
      for (p.x = 0; p.x < ims.Width(); p.x++) {
	 // erosion of with the SE1
	 Img2dsf::ValueType min = Limits<Img2dsf::ValueType>::max();
	 for (p1.y = 0; p1.y < imse1.Height(); p1.y++) {
	    for (p1.x = 0; p1.x < imse1.Width(); p1.x++) {
	       p2 = p + p1 - shift1;
	       if (ims.Hold(p2) && imse1[p1] && ((val = ims[p2]) < min)) {
		  min = val;
	       }
	    }
	 }
	 // dilatation of with the SE2
	 Img2dsf::ValueType max = Limits<Img2dsf::ValueType>::min();
	 for (p1.y = 0; p1.y < imse2.Height(); p1.y++)
	    for (p1.x = 0; p1.x < imse2.Width(); p1.x++) {
	       p2 = p + p1 - shift2;
	       if (ims.Hold(p2) && imse2[p1] && ((val = ims[p2]) > max)) {
		  max = val;
	       }
	    }
	 if (min > max) {
	    imd[p] = min - max;
	 } else {
	    imd[p] = 0;
	 }
      }
   }
   return SUCCESS;
}

Errc PHitOrMiss( const Img3duc &ims, const Img3duc &imse1, const Img3duc &imse2, Img3duc &imd ) {
   Point3d p, p1, p2;
   Img3duc::ValueType val = 0;

   Point3d shift1(imse1.Size() / 2);
   Point3d shift2(imse2.Size() / 2);

   for (p.z = 0; p.z < ims.Depth(); p.z++) {
      for (p.y = 0; p.y < ims.Height(); p.y++) {
	 for (p.x = 0; p.x < ims.Width(); p.x++) {
	    Img3duc::ValueType min = Limits<Img3duc::ValueType>::max();
	    
	    for (p1.z = 0; p1.z < imse1.Depth(); p1.z++) {
	       for (p1.y = 0; p1.y < imse1.Height(); p1.y++) {
		  for (p1.x = 0; p1.x < imse1.Width(); p1.x++) {
		     p2 = p + p1 - shift1;
		     if (ims.Hold(p2) && imse1[p1] && ((val = ims[p2]) < min)) {
			min = val;
		     }
		  }
	       }
	    }
	    Img3duc::ValueType max  =Limits<Img3duc::ValueType>::min();
	    for (p1.z = 0; p1.z  <imse2.Depth(); p1.z++) {
	       for (p1.y = 0; p1.y < imse2.Height(); p1.y++) {
		  for (p1.x = 0; p1.x < imse2.Width(); p1.x++) {
		     p2 = p + p1 - shift2;
		     if (ims.Hold(p2) && imse2[p1] && ((val = ims[p2]) > max)) {
			max = val;
		     }
		  }
	       }
	    }
	    if (min > max) {
	       imd[p] = min - max;
	    } else {
	       imd[p] = 0;
	    }
	 }
      }
   }
   return SUCCESS;
}

Errc PHitOrMiss( const Img3dsl &ims, const Img3duc &imse1, const Img3duc &imse2, Img3dsl &imd ) {
   Point3d p, p1, p2;
   Img3dsl::ValueType val = 0;

   Point3d shift1(imse1.Size() / 2);
   Point3d shift2(imse2.Size() / 2);

   for (p.z = 0; p.z < ims.Depth(); p.z++) {
      for (p.y = 0; p.y < ims.Height(); p.y++) {
	 for (p.x = 0; p.x < ims.Width(); p.x++) {
	    Img3dsl::ValueType min = Limits<Img3dsl::ValueType>::max();
	    
	    for (p1.z = 0; p1.z < imse1.Depth(); p1.z++) {
	       for (p1.y = 0; p1.y < imse1.Height(); p1.y++) {
		  for (p1.x = 0; p1.x < imse1.Width(); p1.x++) {
		     p2 = p + p1 - shift1;
		     if (ims.Hold(p2) && imse1[p1] && ((val = ims[p2]) < min)) {
			min = val;
		     }
		  }
	       }
	    }
	    Img3dsl::ValueType max  =Limits<Img3dsl::ValueType>::min();
	    for (p1.z = 0; p1.z  <imse2.Depth(); p1.z++) {
	       for (p1.y = 0; p1.y < imse2.Height(); p1.y++) {
		  for (p1.x = 0; p1.x < imse2.Width(); p1.x++) {
		     p2 = p + p1 - shift2;
		     if (ims.Hold(p2) && imse2[p1] && ((val = ims[p2]) > max)) {
			max = val;
		     }
		  }
	       }
	    }
	    if (min > max) {
	       imd[p] = min - max;
	    } else {
	       imd[p] = 0;
	    }
	 }
      }
   }
   return SUCCESS;
}

Errc PHitOrMiss( const Img3dsf &ims, const Img3duc &imse1, const Img3duc &imse2, Img3dsf &imd ) {
   Point3d p, p1, p2;
   Img3dsf::ValueType val = 0;

   Point3d shift1(imse1.Size() / 2);
   Point3d shift2(imse2.Size() / 2);

   for (p.z = 0; p.z < ims.Depth(); p.z++) {
      for (p.y = 0; p.y < ims.Height(); p.y++) {
	 for (p.x = 0; p.x < ims.Width(); p.x++) {
	    Img3dsf::ValueType min = Limits<Img3dsf::ValueType>::max();
	    
	    for (p1.z = 0; p1.z < imse1.Depth(); p1.z++) {
	       for (p1.y = 0; p1.y < imse1.Height(); p1.y++) {
		  for (p1.x = 0; p1.x < imse1.Width(); p1.x++) {
		     p2 = p + p1 - shift1;
		     if (ims.Hold(p2) && imse1[p1] && ((val = ims[p2]) < min)) {
			min = val;
		     }
		  }
	       }
	    }
	    Img3dsf::ValueType max  =Limits<Img3dsf::ValueType>::min();
	    for (p1.z = 0; p1.z  <imse2.Depth(); p1.z++) {
	       for (p1.y = 0; p1.y < imse2.Height(); p1.y++) {
		  for (p1.x = 0; p1.x < imse2.Width(); p1.x++) {
		     p2 = p + p1 - shift2;
		     if (ims.Hold(p2) && imse2[p1] && ((val = ims[p2]) > max)) {
			max = val;
		     }
		  }
	       }
	    }
	    if (min > max) {
	       imd[p] = min - max;
	    } else {
	       imd[p] = 0;
	    }
	 }
      }
   }
   return SUCCESS;
}

Errc PHitOrMiss( const Imc2duc &ims, const Img2duc &imse1, const Img2duc &imse2, Imc2duc &imd ) {
   Point2d p, p1, p2;
   Point2d shift1(imse1.Size() / 2);
   Point2d shift2(imse2.Size() / 2);
   
   for (p.y = 0; p.y < ims.Height(); p.y++) {
      for (p.x = 0; p.x < ims.Width(); p.x++) {
	 Imc2duc::ValueType minX = Limits<Imc2duc::ValueType>::min();
	 Imc2duc::ValueType minY = Limits<Imc2duc::ValueType>::min();
	 Imc2duc::ValueType minZ = Limits<Imc2duc::ValueType>::min();
	 
	 for (p1.y = 0; p1.y < imse1.Height(); p1.y++) {
	    for (p1.x = 0; p1.x < imse1.Width(); p1.x++) {
	       p2 = p + p1 - shift1;
	       if (ims.Hold(p2)
		   && imse1[p1]
		   && Lower(ims.X[p2], ims.Y[p2], ims.Z[p2], minX, minY, minZ)) {
		  minX = ims.X[p2];
		  minY = ims.Y[p2];
		  minZ = ims.Z[p2];
	       }
	    }
	 }
	 Imc2duc::ValueType maxX = Limits<Imc2duc::ValueType>::min();
	 Imc2duc::ValueType maxY = Limits<Imc2duc::ValueType>::min();
	 Imc2duc::ValueType maxZ = Limits<Imc2duc::ValueType>::min();
	 for (p1.y = 0; p1.y < imse2.Height(); p1.y++) {
	    for (p1.x = 0; p1.x < imse2.Width(); p1.x++) {
	       p2 = p + p1 - shift2;
	       if (ims.Hold(p2)
		   && imse2[p1]
		   && Greater(ims.X[p2], ims.Y[p2], ims.Z[p2], maxX, maxY, maxZ)) {
		  maxX = ims.X[p2];
		  maxY = ims.Y[p2];
		  maxZ = ims.Z[p2];
	       }
	    }
	 }
	 if (Greater(minX, minY, minZ, maxX, maxY, maxZ) ) {
	    imd.X[p] = MIN(minX - maxX, 0);
	    imd.Y[p] = MIN(minY - maxY, 0);
	    imd.Z[p] = MIN(maxZ - maxZ, 0);
	 } else {
	    imd.X[p] = 0;
	    imd.Y[p] = 0;
	    imd.Z[p] = 0;
	 }
      }
   }
   return SUCCESS;
}

Errc PHitOrMiss( const Imc2dsl &ims, const Img2duc &imse1, const Img2duc &imse2, Imc2dsl &imd ) {
   Point2d p, p1, p2;
   Point2d shift1(imse1.Size() / 2);
   Point2d shift2(imse2.Size() / 2);
   
   for (p.y = 0; p.y < ims.Height(); p.y++) {
      for (p.x = 0; p.x < ims.Width(); p.x++) {
	 Imc2dsl::ValueType minX = Limits<Imc2dsl::ValueType>::min();
	 Imc2dsl::ValueType minY = Limits<Imc2dsl::ValueType>::min();
	 Imc2dsl::ValueType minZ = Limits<Imc2dsl::ValueType>::min();
	 
	 for (p1.y = 0; p1.y < imse1.Height(); p1.y++) {
	    for (p1.x = 0; p1.x < imse1.Width(); p1.x++) {
	       p2 = p + p1 - shift1;
	       if (ims.Hold(p2)
		   && imse1[p1]
		   && Lower(ims.X[p2], ims.Y[p2], ims.Z[p2], minX, minY, minZ)) {
		  minX = ims.X[p2];
		  minY = ims.Y[p2];
		  minZ = ims.Z[p2];
	       }
	    }
	 }
	 Imc2dsl::ValueType maxX = Limits<Imc2dsl::ValueType>::min();
	 Imc2dsl::ValueType maxY = Limits<Imc2dsl::ValueType>::min();
	 Imc2dsl::ValueType maxZ = Limits<Imc2dsl::ValueType>::min();
	 for (p1.y = 0; p1.y < imse2.Height(); p1.y++) {
	    for (p1.x = 0; p1.x < imse2.Width(); p1.x++) {
	       p2 = p + p1 - shift2;
	       if (ims.Hold(p2)
		   && imse2[p1]
		   && Greater(ims.X[p2], ims.Y[p2], ims.Z[p2], maxX, maxY, maxZ)) {
		  maxX = ims.X[p2];
		  maxY = ims.Y[p2];
		  maxZ = ims.Z[p2];
	       }
	    }
	 }
	 if (Greater(minX, minY, minZ, maxX, maxY, maxZ) ) {
	    imd.X[p] = MIN(minX - maxX, 0);
	    imd.Y[p] = MIN(minY - maxY, 0);
	    imd.Z[p] = MIN(maxZ - maxZ, 0);
	 } else {
	    imd.X[p] = 0;
	    imd.Y[p] = 0;
	    imd.Z[p] = 0;
	 }
      }
   }
   return SUCCESS;
}

Errc PHitOrMiss( const Imc2dsf &ims, const Img2duc &imse1, const Img2duc &imse2, Imc2dsf &imd ) {
   Point2d p, p1, p2;
   Point2d shift1(imse1.Size() / 2);
   Point2d shift2(imse2.Size() / 2);
   
   for (p.y = 0; p.y < ims.Height(); p.y++) {
      for (p.x = 0; p.x < ims.Width(); p.x++) {
	 Imc2dsf::ValueType minX = Limits<Imc2dsf::ValueType>::min();
	 Imc2dsf::ValueType minY = Limits<Imc2dsf::ValueType>::min();
	 Imc2dsf::ValueType minZ = Limits<Imc2dsf::ValueType>::min();
	 
	 for (p1.y = 0; p1.y < imse1.Height(); p1.y++) {
	    for (p1.x = 0; p1.x < imse1.Width(); p1.x++) {
	       p2 = p + p1 - shift1;
	       if (ims.Hold(p2)
		   && imse1[p1]
		   && Lower(ims.X[p2], ims.Y[p2], ims.Z[p2], minX, minY, minZ)) {
		  minX = ims.X[p2];
		  minY = ims.Y[p2];
		  minZ = ims.Z[p2];
	       }
	    }
	 }
	 Imc2dsf::ValueType maxX = Limits<Imc2dsf::ValueType>::min();
	 Imc2dsf::ValueType maxY = Limits<Imc2dsf::ValueType>::min();
	 Imc2dsf::ValueType maxZ = Limits<Imc2dsf::ValueType>::min();
	 for (p1.y = 0; p1.y < imse2.Height(); p1.y++) {
	    for (p1.x = 0; p1.x < imse2.Width(); p1.x++) {
	       p2 = p + p1 - shift2;
	       if (ims.Hold(p2)
		   && imse2[p1]
		   && Greater(ims.X[p2], ims.Y[p2], ims.Z[p2], maxX, maxY, maxZ)) {
		  maxX = ims.X[p2];
		  maxY = ims.Y[p2];
		  maxZ = ims.Z[p2];
	       }
	    }
	 }
	 if (Greater(minX, minY, minZ, maxX, maxY, maxZ) ) {
	    imd.X[p] = MIN(minX - maxX, 0);
	    imd.Y[p] = MIN(minY - maxY, 0);
	    imd.Z[p] = MIN(maxZ - maxZ, 0);
	 } else {
	    imd.X[p] = 0;
	    imd.Y[p] = 0;
	    imd.Z[p] = 0;
	 }
      }
   }
   return SUCCESS;
}

Errc PHitOrMiss( const Imc3duc &ims, const Img3duc &imse1, const Img3duc &imse2, Imc3duc &imd ) {
   Point3d p, p1, p2;
   
   Point3d shift1(imse1.Size() / 2);
   Point3d shift2(imse2.Size() / 2);

   for (p.z = 0; p.z < ims.Depth(); p.y++) {
      for (p.y = 0; p.y < ims.Height(); p.y++) {
	 for (p.x = 0; p.x < ims.Width(); p.x++) {
	    Imc3duc::ValueType minX, minY, minZ;
	    minX = Limits<Imc3duc::ValueType>::max();
	    minY = Limits<Imc3duc::ValueType>::max();
	    minZ = Limits<Imc3duc::ValueType>::max();
	    
	    for (p1.z = 0; p1.z < imse1.Depth(); p1.z++) {
	       for (p1.y = 0; p1.y < imse1.Height(); p1.y++) {
		  for (p1.x = 0; p1.x < imse1.Width(); p1.x++) {
		     p2 = p + p1 - shift1;
		     if (ims.Hold(p2)
			 && imse1[p1]
			 && Lower(ims.X[p2], ims.Y[p2], ims.Z[p2], minX, minY, minZ)) {
			minX = ims.X[p2];
			minY = ims.Y[p2];
			minZ = ims.Z[p2];
		     }
		  }
	       }
	    }
	    Imc3duc::ValueType maxX, maxY, maxZ;
	    maxX = Limits<Imc3duc::ValueType>::min();
	    maxY = Limits<Imc3duc::ValueType>::min();
	    maxZ = Limits<Imc3duc::ValueType>::min();
	    
	    for (p1.z = 0; p1.z < imse2.Depth(); p1.z++) {
	       for (p1.y = 0; p1.y < imse2.Height(); p1.y++) {
		  for (p1.x = 0; p1.x < imse2.Width(); p1.x++) {
		     p2 = p + p1 - shift2;
		     if (ims.Hold(p2)
			 && imse2[p1]
			 && Greater(ims.X[p2], ims.Y[p2], ims.Z[p2], maxX, maxY, maxZ)) {
			maxX = ims.X[p2];
			maxY = ims.Y[p2];
			maxZ = ims.Z[p2];
		     }
		  }
	       }
	    }
	    if (Greater(minX, minY, minZ, maxX, maxY, maxZ) ) {
	       imd.X[p] = MIN(minX - maxX, 0);
	       imd.Y[p] = MIN(minY - maxY, 0);
	       imd.Z[p] = MIN(maxZ - maxZ, 0);
	    } else {
	       imd.X[p] = 0;
	       imd.Y[p] = 0;
	       imd.Z[p] = 0;
	    }
	 }
      }
   }
   return SUCCESS;
}

Errc PHitOrMiss( const Imc3dsl &ims, const Img3duc &imse1, const Img3duc &imse2, Imc3dsl &imd ) {
   Point3d p, p1, p2;
   
   Point3d shift1(imse1.Size() / 2);
   Point3d shift2(imse2.Size() / 2);

   for (p.z = 0; p.z < ims.Depth(); p.y++) {
      for (p.y = 0; p.y < ims.Height(); p.y++) {
	 for (p.x = 0; p.x < ims.Width(); p.x++) {
	    Imc3dsl::ValueType minX, minY, minZ;
	    minX = Limits<Imc3dsl::ValueType>::max();
	    minY = Limits<Imc3dsl::ValueType>::max();
	    minZ = Limits<Imc3dsl::ValueType>::max();
	    
	    for (p1.z = 0; p1.z < imse1.Depth(); p1.z++) {
	       for (p1.y = 0; p1.y < imse1.Height(); p1.y++) {
		  for (p1.x = 0; p1.x < imse1.Width(); p1.x++) {
		     p2 = p + p1 - shift1;
		     if (ims.Hold(p2)
			 && imse1[p1]
			 && Lower(ims.X[p2], ims.Y[p2], ims.Z[p2], minX, minY, minZ)) {
			minX = ims.X[p2];
			minY = ims.Y[p2];
			minZ = ims.Z[p2];
		     }
		  }
	       }
	    }
	    Imc3dsl::ValueType maxX, maxY, maxZ;
	    maxX = Limits<Imc3dsl::ValueType>::min();
	    maxY = Limits<Imc3dsl::ValueType>::min();
	    maxZ = Limits<Imc3dsl::ValueType>::min();
	    
	    for (p1.z = 0; p1.z < imse2.Depth(); p1.z++) {
	       for (p1.y = 0; p1.y < imse2.Height(); p1.y++) {
		  for (p1.x = 0; p1.x < imse2.Width(); p1.x++) {
		     p2 = p + p1 - shift2;
		     if (ims.Hold(p2)
			 && imse2[p1]
			 && Greater(ims.X[p2], ims.Y[p2], ims.Z[p2], maxX, maxY, maxZ)) {
			maxX = ims.X[p2];
			maxY = ims.Y[p2];
			maxZ = ims.Z[p2];
		     }
		  }
	       }
	    }
	    if (Greater(minX, minY, minZ, maxX, maxY, maxZ) ) {
	       imd.X[p] = MIN(minX - maxX, 0);
	       imd.Y[p] = MIN(minY - maxY, 0);
	       imd.Z[p] = MIN(maxZ - maxZ, 0);
	    } else {
	       imd.X[p] = 0;
	       imd.Y[p] = 0;
	       imd.Z[p] = 0;
	    }
	 }
      }
   }
   return SUCCESS;
}

Errc PHitOrMiss( const Imc3dsf &ims, const Img3duc &imse1, const Img3duc &imse2, Imc3dsf &imd ) {
   Point3d p, p1, p2;
   
   Point3d shift1(imse1.Size() / 2);
   Point3d shift2(imse2.Size() / 2);

   for (p.z = 0; p.z < ims.Depth(); p.y++) {
      for (p.y = 0; p.y < ims.Height(); p.y++) {
	 for (p.x = 0; p.x < ims.Width(); p.x++) {
	    Imc3dsf::ValueType minX, minY, minZ;
	    minX = Limits<Imc3dsf::ValueType>::max();
	    minY = Limits<Imc3dsf::ValueType>::max();
	    minZ = Limits<Imc3dsf::ValueType>::max();
	    
	    for (p1.z = 0; p1.z < imse1.Depth(); p1.z++) {
	       for (p1.y = 0; p1.y < imse1.Height(); p1.y++) {
		  for (p1.x = 0; p1.x < imse1.Width(); p1.x++) {
		     p2 = p + p1 - shift1;
		     if (ims.Hold(p2)
			 && imse1[p1]
			 && Lower(ims.X[p2], ims.Y[p2], ims.Z[p2], minX, minY, minZ)) {
			minX = ims.X[p2];
			minY = ims.Y[p2];
			minZ = ims.Z[p2];
		     }
		  }
	       }
	    }
	    Imc3dsf::ValueType maxX, maxY, maxZ;
	    maxX = Limits<Imc3dsf::ValueType>::min();
	    maxY = Limits<Imc3dsf::ValueType>::min();
	    maxZ = Limits<Imc3dsf::ValueType>::min();
	    
	    for (p1.z = 0; p1.z < imse2.Depth(); p1.z++) {
	       for (p1.y = 0; p1.y < imse2.Height(); p1.y++) {
		  for (p1.x = 0; p1.x < imse2.Width(); p1.x++) {
		     p2 = p + p1 - shift2;
		     if (ims.Hold(p2)
			 && imse2[p1]
			 && Greater(ims.X[p2], ims.Y[p2], ims.Z[p2], maxX, maxY, maxZ)) {
			maxX = ims.X[p2];
			maxY = ims.Y[p2];
			maxZ = ims.Z[p2];
		     }
		  }
	       }
	    }
	    if (Greater(minX, minY, minZ, maxX, maxY, maxZ) ) {
	       imd.X[p] = MIN(minX - maxX, 0);
	       imd.Y[p] = MIN(minY - maxY, 0);
	       imd.Z[p] = MIN(maxZ - maxZ, 0);
	    } else {
	       imd.X[p] = 0;
	       imd.Y[p] = 0;
	       imd.Z[p] = 0;
	    }
	 }
      }
   }
   return SUCCESS;
}


#ifdef MAIN
#define	USAGE	"usage: %s [-m mask] [im_se1|-] [im_se2|-] [im_in|-] [im_out|-]"
#define	PARC	0
#define	FINC	3
#define	FOUTC	1
#define	MASK	1

int main( int argc, char *argv[] ) {
   Errc result;                // The result code of the execution.
   Pobject* mask;              // The region map.
   Pobject* objin[FINC + 1];   // The input objects.
   Pobject* objs[FINC + 1];    // The source objects masked.
   Pobject* objout[FOUTC + 1]; // The output object.
   Pobject* objd[FOUTC + 1];   // The result object of the execution.
   char* parv[PARC + 1];       // The input parameters.

   ReadArgs(argc, argv, PARC, FINC, FOUTC, &mask, objin, objs, objout, objd, parv, USAGE, MASK); 
   if (objs[1]->Type() == Po_Img2duc) {
      Img2duc* const ims = (Img2duc*)objs[2];
      Img2duc* const imse1 = (Img2duc*)objs[0];
      Img2duc* const imse2 = (Img2duc*)objs[1];
      objd[0] = new Img2duc(ims->Props());
      Img2duc* const imd = (Img2duc*)objd[0];
      
      result = PHitOrMiss(*ims, *imse1, *imse2, *imd);
      goto end;
   }
   if (objs[1]->Type() == Po_Img2dsl) {
      Img2dsl* const ims = (Img2dsl*)objs[2];
      Img2duc* const imse1 = (Img2duc*)objs[0];
      Img2duc* const imse2 = (Img2duc*)objs[1];
      objd[0] = new Img2dsl(ims->Props());
      Img2dsl* const imd = (Img2dsl*)objd[0];
      
      result = PHitOrMiss(*ims, *imse1, *imse2, *imd);
      goto end;
   }
   if (objs[1]->Type() == Po_Img2dsf) {
      Img2dsf* const ims = (Img2dsf*)objs[2];
      Img2duc* const imse1 = (Img2duc*)objs[0];
      Img2duc* const imse2 = (Img2duc*)objs[1];
      objd[0] = new Img2dsf(ims->Props());
      Img2dsf* const imd = (Img2dsf*)objd[0];
      
      result = PHitOrMiss(*ims, *imse1, *imse2, *imd);
      goto end;
   }
   if (objs[1]->Type() == Po_Img3duc) {
      Img3duc* const ims = (Img3duc*)objs[2];
      Img3duc* const imse1 = (Img3duc*)objs[0];
      Img3duc* const imse2 = (Img3duc*)objs[1];
      objd[0] = new Img3duc(ims->Props());
      Img3duc* const imd = (Img3duc*)objd[0];
      
      result = PHitOrMiss(*ims, *imse1, *imse2, *imd);
      goto end;
   }
   if (objs[1]->Type() == Po_Img3dsl) {
      Img3dsl* const ims = (Img3dsl*)objs[2];
      Img3duc* const imse1 = (Img3duc*)objs[0];
      Img3duc* const imse2 = (Img3duc*)objs[1];
      objd[0] = new Img3dsl(ims->Props());
      Img3dsl* const imd = (Img3dsl*)objd[0];
      
      result = PHitOrMiss(*ims, *imse1, *imse2, *imd);
      goto end;
   }
   if (objs[1]->Type() == Po_Img3dsf) {
      Img3dsf* const ims = (Img3dsf*)objs[2];
      Img3duc* const imse1 = (Img3duc*)objs[0];
      Img3duc* const imse2 = (Img3duc*)objs[1];
      objd[0] = new Img3dsf(ims->Props());
      Img3dsf* const imd = (Img3dsf*)objd[0];
      
      result = PHitOrMiss(*ims, *imse1, *imse2, *imd);
      goto end;
   }
   if (objs[1]->Type() == Po_Imc2duc) {
      Imc2duc* const ims = (Imc2duc*)objs[2];
      Img2duc* const imse1 = (Img2duc*)objs[0];
      Img2duc* const imse2 = (Img2duc*)objs[1];
      objd[0] = new Imc2duc(ims->Props());
      Imc2duc* const imd = (Imc2duc*)objd[0];
      
      result = PHitOrMiss(*ims, *imse1, *imse2, *imd);
      goto end;
   }
   if (objs[1]->Type() == Po_Imc2dsl) {
      Imc2dsl* const ims = (Imc2dsl*)objs[2];
      Img2duc* const imse1 = (Img2duc*)objs[0];
      Img2duc* const imse2 = (Img2duc*)objs[1];
      objd[0] = new Imc2dsl(ims->Props());
      Imc2dsl* const imd = (Imc2dsl*)objd[0];
      
      result = PHitOrMiss(*ims, *imse1, *imse2, *imd);
      goto end;
   }
   if (objs[1]->Type() == Po_Imc2dsf) {
      Imc2dsf* const ims = (Imc2dsf*)objs[2];
      Img2duc* const imse1 = (Img2duc*)objs[0];
      Img2duc* const imse2 = (Img2duc*)objs[1];
      objd[0] = new Imc2dsf(ims->Props());
      Imc2dsf* const imd = (Imc2dsf*)objd[0];
      
      result = PHitOrMiss(*ims, *imse1, *imse2, *imd);
      goto end;
   }
   if (objs[1]->Type() == Po_Imc3duc) {
      Imc3duc* const ims = (Imc3duc*)objs[2];
      Img3duc* const imse1 = (Img3duc*)objs[0];
      Img3duc* const imse2 = (Img3duc*)objs[1];
      objd[0] = new Imc3duc(ims->Props());
      Imc3duc* const imd = (Imc3duc*)objd[0];
      
      result = PHitOrMiss(*ims, *imse1, *imse2, *imd);
      goto end;
   }
   if (objs[1]->Type() == Po_Imc3dsl) {
      Imc3dsl* const ims = (Imc3dsl*)objs[2];
      Img3duc* const imse1 = (Img3duc*)objs[0];
      Img3duc* const imse2 = (Img3duc*)objs[1];
      objd[0] = new Imc3dsl(ims->Props());
      Imc3dsl* const imd = (Imc3dsl*)objd[0];
      
      result = PHitOrMiss(*ims, *imse1, *imse2, *imd);
      goto end;
   }
   if (objs[1]->Type() == Po_Imc3dsf) {
      Imc3dsf* const ims = (Imc3dsf*)objs[2];
      Img3duc* const imse1 = (Img3duc*)objs[0];
      Img3duc* const imse2 = (Img3duc*)objs[1];
      objd[0] = new Imc3dsf(ims->Props());
      Imc3dsf* const imd = (Imc3dsf*)objd[0];
      
      result = PHitOrMiss(*ims, *imse1, *imse2, *imd);
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
