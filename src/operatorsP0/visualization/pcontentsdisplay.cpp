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
 * For more information, refer to:
 * https://clouard.users.greyc.fr/Pandore
 */

/**
 * @author Francois Angot - 2000-07-04
 * @author Regis Clouard - 2001-04-27 (version 3.00) 
 * @author Regis Clouard - 2002-11-28 (version 3.5)
 * @author Regis Clouard - 2006-05-12 (add 1D images).
 * @author Alexis Mignon - 2009-01-06 (fix bug on multispectral images).
*/

#include <pandore.h>
using namespace pandore;

/**
 * @file pcontentsdisplay.cpp
 * Displays the content of a Pandore object.
 */











Errc PContentsDisplay( const Graph2d &grs ) {
   Long i;
   GEdge *ptr;
   
   std::cout<<"Graph 2d: " << grs.Size() << " nodes:"<< std::endl;
   std::cout<<grs.Height() <<" rows, "<< grs.Width()<<" columns." << std::endl << std::endl;
   for (i=0;i<grs.Size();i++)
      if (grs[i]) {
	 std::cout<<i<<": y="<<grs[i]->seed.y<<" x="<<grs[i]->seed.x<<"; value="<<grs[i]->value<<", item="<<grs[i]->Item()<< std::endl << std::endl;
	 ptr=grs[i]->Neighbours();
	 if (ptr) {
	    for (;ptr!=NULL;ptr=ptr->Next())
	       std::cout<<"  " << ptr->Node() << ":"<< ptr->weight;
	    std::cout<<std::endl;
	 }
      } else {
	 std::cout<< i << ": NULL" << std::endl;
      }
   
   return SUCCESS;
}

Errc PContentsDisplay( const Graph3d &grs ) {
   Long i;
   GEdge *ptr;
   
   std::cout<<"Graph 3d: "<<grs.Size()<<" nodes:" << std::endl;
   std::cout<<grs.Depth()<<" slices, "<<grs.Height()<<"; rows, "<<grs.Width()<<" columns.\n\n";
   for (i=1;i<grs.Size();i++)
      if (grs[i]) {
	 std::cout<<i<<": z="<<grs[i]->seed.z<<" y="<<grs[i]->seed.y<<" x="<<grs[i]->seed.x<<"; value="<<grs[i]->value<<", item="<<grs[i]->Item()<<std::endl;
	 ptr=grs[i]->Neighbours();
	 if (ptr) {
	    for (;ptr!=NULL;ptr=ptr->Next())
	       std::cout<<"  "<<ptr->Node()<<":("<<ptr->weight<<");";
	    std::cout<< std::endl;
	 }
      } else {
	 std::cout<<i<<": NULL" << std::endl;
      }
   
   return SUCCESS;
}

#ifdef MAIN
#undef MAIN
namespace pcol2txt {
#include "collection/pcol2txt.cpp"
}
#define MAIN
#endif

/*
 * Collections.
 */ 
Errc PContentsDisplay( const Collection &c ) {
   std::list<std::string> l = c.List();
   
   std::cout<<"Type    : Collection (Collection)" << std::endl;
   std::cout<<"Nb attr : "<< l.size() << std::endl;
   
   return pcol2txt::PCol2Txt(c, stdout);
}



Errc PContentsDisplay( const Img2duc &ims ) {
   register int y, x;
   
   std::cout <<"Gray image 2d of " << (TypeName<Img2duc::ValueType>::Name()).c_str() << std::endl;
   std::cout << ims.Height() << "rows "<< ims.Width() << " columns." << std::endl << std::endl;
   std::cout << "Legend: y,x : (value)" << std::endl;
   for (y = 0; y < ims.Height(); y++)
   for (x = 0; x < ims.Width(); x++)
   {
      // (Select::Largest) to avoid cout << char.
      if (ims[y][x])
	 std::cout<<y<<","<<x<<":("<<(Select< Img2duc::ValueType, Short>::Largest)ims[y][x]<<")\t";
   }
   return SUCCESS;
}
Errc PContentsDisplay( const Img2dsl &ims ) {
   register int y, x;
   
   std::cout <<"Gray image 2d of " << (TypeName<Img2dsl::ValueType>::Name()).c_str() << std::endl;
   std::cout << ims.Height() << "rows "<< ims.Width() << " columns." << std::endl << std::endl;
   std::cout << "Legend: y,x : (value)" << std::endl;
   for (y = 0; y < ims.Height(); y++)
   for (x = 0; x < ims.Width(); x++)
   {
      // (Select::Largest) to avoid cout << char.
      if (ims[y][x])
	 std::cout<<y<<","<<x<<":("<<(Select< Img2dsl::ValueType, Short>::Largest)ims[y][x]<<")\t";
   }
   return SUCCESS;
}
Errc PContentsDisplay( const Img2dsf &ims ) {
   register int y, x;
   
   std::cout <<"Gray image 2d of " << (TypeName<Img2dsf::ValueType>::Name()).c_str() << std::endl;
   std::cout << ims.Height() << "rows "<< ims.Width() << " columns." << std::endl << std::endl;
   std::cout << "Legend: y,x : (value)" << std::endl;
   for (y = 0; y < ims.Height(); y++)
   for (x = 0; x < ims.Width(); x++)
   {
      // (Select::Largest) to avoid cout << char.
      if (ims[y][x])
	 std::cout<<y<<","<<x<<":("<<(Select< Img2dsf::ValueType, Short>::Largest)ims[y][x]<<")\t";
   }
   return SUCCESS;
}
Errc PContentsDisplay( const Reg2d &ims ) {
   register int y, x;
   
   std::cout <<"Gray image 2d of " << (TypeName<Reg2d::ValueType>::Name()).c_str() << std::endl;
   std::cout << ims.Height() << "rows "<< ims.Width() << " columns." << std::endl << std::endl;
   std::cout << "Legend: y,x : (value)" << std::endl;
   for (y = 0; y < ims.Height(); y++)
   for (x = 0; x < ims.Width(); x++)
   {
      // (Select::Largest) to avoid cout << char.
      if (ims[y][x])
	 std::cout<<y<<","<<x<<":("<<(Select< Reg2d::ValueType, Short>::Largest)ims[y][x]<<")\t";
   }
   return SUCCESS;
}
Errc PContentsDisplay( const Imc2duc &ims ) {
   register int y, x;
   
   std::cout<<"Color image 2d of "<< (TypeName<Imc2duc::ValueType>::Name()).c_str()<< std::endl;
   std::cout<<ims.Height()<<" rows, "<<ims.Width()<<" columns." << std::endl << std::endl;
   std::cout<<"Legend: y,x : (value,value,value)" << std::endl;
   for (y = 0; y < ims.Height(); y++)
   for (x = 0; x < ims.Width(); x++)
   {
      if (ims.X[y][x] || ims.Y[y][x] || ims.Z[y][x])
	 std::cout<<y<<","<<x<<":("<<(Select< Imc2duc::ValueType,Short>::Largest)ims.X[y][x]<<","
		  <<(Select< Imc2duc::ValueType,Short>::Largest)ims.Y[y][x]<<","
		  <<(Select< Imc2duc::ValueType,Short>::Largest)ims.Z[y][x]<<")\t";
   }
   return SUCCESS;
}
Errc PContentsDisplay( const Imc2dsl &ims ) {
   register int y, x;
   
   std::cout<<"Color image 2d of "<< (TypeName<Imc2dsl::ValueType>::Name()).c_str()<< std::endl;
   std::cout<<ims.Height()<<" rows, "<<ims.Width()<<" columns." << std::endl << std::endl;
   std::cout<<"Legend: y,x : (value,value,value)" << std::endl;
   for (y = 0; y < ims.Height(); y++)
   for (x = 0; x < ims.Width(); x++)
   {
      if (ims.X[y][x] || ims.Y[y][x] || ims.Z[y][x])
	 std::cout<<y<<","<<x<<":("<<(Select< Imc2dsl::ValueType,Short>::Largest)ims.X[y][x]<<","
		  <<(Select< Imc2dsl::ValueType,Short>::Largest)ims.Y[y][x]<<","
		  <<(Select< Imc2dsl::ValueType,Short>::Largest)ims.Z[y][x]<<")\t";
   }
   return SUCCESS;
}
Errc PContentsDisplay( const Imc2dsf &ims ) {
   register int y, x;
   
   std::cout<<"Color image 2d of "<< (TypeName<Imc2dsf::ValueType>::Name()).c_str()<< std::endl;
   std::cout<<ims.Height()<<" rows, "<<ims.Width()<<" columns." << std::endl << std::endl;
   std::cout<<"Legend: y,x : (value,value,value)" << std::endl;
   for (y = 0; y < ims.Height(); y++)
   for (x = 0; x < ims.Width(); x++)
   {
      if (ims.X[y][x] || ims.Y[y][x] || ims.Z[y][x])
	 std::cout<<y<<","<<x<<":("<<(Select< Imc2dsf::ValueType,Short>::Largest)ims.X[y][x]<<","
		  <<(Select< Imc2dsf::ValueType,Short>::Largest)ims.Y[y][x]<<","
		  <<(Select< Imc2dsf::ValueType,Short>::Largest)ims.Z[y][x]<<")\t";
   }
   return SUCCESS;
}
Errc PContentsDisplay( const Imx2duc &ims ) {
   Point2d p;
   
   std::cout <<"Multispectral image 2d of " << (TypeName<Imx2duc::ValueType>::Name()).c_str() << std::endl;
   std::cout << ims.Width() << " columns." << std::endl << std::endl;
   std::cout << "Legend: y,x : (value, value, ...)" << std::endl;
   for (p.y = 0; p.y < ims.Height(); p.y++)
   for (p.x = 0; p.x < ims.Width(); p.x++)
   {
      // (Select::Largest) to avoid cout << char.
      bool empty=true;
      for (int b=0; b<ims.Bands(); b++) if (ims[b][p] != 0) { empty=false; break; }
      if (! empty) {
	 std::cout <<p.y<<","<<p.x<<":(";
	 std::cout << (Select< Imx2duc::ValueType, Short>::Largest)ims[0][p];
	 for (int b=1; b<ims.Bands(); b++) {
	    std::cout << "," << (Select< Imx2duc::ValueType, Short>::Largest)ims[b][p];
	 }
	 std::cout <<")\t";
      }
   }
   return SUCCESS;
}
Errc PContentsDisplay( const Imx2dsl &ims ) {
   Point2d p;
   
   std::cout <<"Multispectral image 2d of " << (TypeName<Imx2dsl::ValueType>::Name()).c_str() << std::endl;
   std::cout << ims.Width() << " columns." << std::endl << std::endl;
   std::cout << "Legend: y,x : (value, value, ...)" << std::endl;
   for (p.y = 0; p.y < ims.Height(); p.y++)
   for (p.x = 0; p.x < ims.Width(); p.x++)
   {
      // (Select::Largest) to avoid cout << char.
      bool empty=true;
      for (int b=0; b<ims.Bands(); b++) if (ims[b][p] != 0) { empty=false; break; }
      if (! empty) {
	 std::cout <<p.y<<","<<p.x<<":(";
	 std::cout << (Select< Imx2dsl::ValueType, Short>::Largest)ims[0][p];
	 for (int b=1; b<ims.Bands(); b++) {
	    std::cout << "," << (Select< Imx2dsl::ValueType, Short>::Largest)ims[b][p];
	 }
	 std::cout <<")\t";
      }
   }
   return SUCCESS;
}
Errc PContentsDisplay( const Imx2dsf &ims ) {
   Point2d p;
   
   std::cout <<"Multispectral image 2d of " << (TypeName<Imx2dsf::ValueType>::Name()).c_str() << std::endl;
   std::cout << ims.Width() << " columns." << std::endl << std::endl;
   std::cout << "Legend: y,x : (value, value, ...)" << std::endl;
   for (p.y = 0; p.y < ims.Height(); p.y++)
   for (p.x = 0; p.x < ims.Width(); p.x++)
   {
      // (Select::Largest) to avoid cout << char.
      bool empty=true;
      for (int b=0; b<ims.Bands(); b++) if (ims[b][p] != 0) { empty=false; break; }
      if (! empty) {
	 std::cout <<p.y<<","<<p.x<<":(";
	 std::cout << (Select< Imx2dsf::ValueType, Short>::Largest)ims[0][p];
	 for (int b=1; b<ims.Bands(); b++) {
	    std::cout << "," << (Select< Imx2dsf::ValueType, Short>::Largest)ims[b][p];
	 }
	 std::cout <<")\t";
      }
   }
   return SUCCESS;
}

Errc PContentsDisplay( const Img3duc &ims ) {
   register int z, y, x;
   
   std::cout<<"Gray image 3d of " << (TypeName<Img3duc::ValueType>::Name()).c_str() << std::endl;
   std::cout<< ims.Depth() << "slices, " << ims.Height() << " rows, " << ims.Width() << " columns." << std::endl << std::endl;
   std::cout<<"Legend: z,y,x : (value)" << std::endl;
   for (z = 0; z < ims.Depth(); z++)
   for (y = 0; y < ims.Height(); y++)
   for (x = 0; x < ims.Width(); x++)
   {
      if (ims[z][y][x])
	 std::cout<<z<<","<<y<<","<<x<<":("<<(Select< Img3duc::ValueType,Short>::Largest)ims[z][y][x]<<")\t";
   }
   return SUCCESS;
}
Errc PContentsDisplay( const Img3dsl &ims ) {
   register int z, y, x;
   
   std::cout<<"Gray image 3d of " << (TypeName<Img3dsl::ValueType>::Name()).c_str() << std::endl;
   std::cout<< ims.Depth() << "slices, " << ims.Height() << " rows, " << ims.Width() << " columns." << std::endl << std::endl;
   std::cout<<"Legend: z,y,x : (value)" << std::endl;
   for (z = 0; z < ims.Depth(); z++)
   for (y = 0; y < ims.Height(); y++)
   for (x = 0; x < ims.Width(); x++)
   {
      if (ims[z][y][x])
	 std::cout<<z<<","<<y<<","<<x<<":("<<(Select< Img3dsl::ValueType,Short>::Largest)ims[z][y][x]<<")\t";
   }
   return SUCCESS;
}
Errc PContentsDisplay( const Img3dsf &ims ) {
   register int z, y, x;
   
   std::cout<<"Gray image 3d of " << (TypeName<Img3dsf::ValueType>::Name()).c_str() << std::endl;
   std::cout<< ims.Depth() << "slices, " << ims.Height() << " rows, " << ims.Width() << " columns." << std::endl << std::endl;
   std::cout<<"Legend: z,y,x : (value)" << std::endl;
   for (z = 0; z < ims.Depth(); z++)
   for (y = 0; y < ims.Height(); y++)
   for (x = 0; x < ims.Width(); x++)
   {
      if (ims[z][y][x])
	 std::cout<<z<<","<<y<<","<<x<<":("<<(Select< Img3dsf::ValueType,Short>::Largest)ims[z][y][x]<<")\t";
   }
   return SUCCESS;
}
Errc PContentsDisplay( const Reg3d &ims ) {
   register int z, y, x;
   
   std::cout<<"Gray image 3d of " << (TypeName<Reg3d::ValueType>::Name()).c_str() << std::endl;
   std::cout<< ims.Depth() << "slices, " << ims.Height() << " rows, " << ims.Width() << " columns." << std::endl << std::endl;
   std::cout<<"Legend: z,y,x : (value)" << std::endl;
   for (z = 0; z < ims.Depth(); z++)
   for (y = 0; y < ims.Height(); y++)
   for (x = 0; x < ims.Width(); x++)
   {
      if (ims[z][y][x])
	 std::cout<<z<<","<<y<<","<<x<<":("<<(Select< Reg3d::ValueType,Short>::Largest)ims[z][y][x]<<")\t";
   }
   return SUCCESS;
}
Errc PContentsDisplay( const Imc3duc &ims ) {
   register int z, y, x;
   
   std::cout<<"Color image 3d of "<<(TypeName<Imc3duc::ValueType>::Name()).c_str() << std::endl;
   std::cout<<ims.Depth()<<" slices, "<<ims.Height()<<" rows, "<<ims.Width()<<" columns." << std::endl << std::endl;
   std::cout<<"Legend: z,y,x : (value,value,value)"<< std::endl;
   for (z = 0; z < ims.Depth(); z++)
   for (y = 0; y < ims.Height(); y++)
   for (x = 0; x < ims.Width(); x++)
   {
      if (ims.X[z][y][x] || ims.Y[z][y][x] || ims.Z[z][y][x])
	 std::cout<<z<<","<<y<<","<<x<<":("<<(Select< Imc3duc::ValueType,Short>::Largest)ims.X[z][y][x]<<","
		  <<(Select< Imc3duc::ValueType,Short>::Largest)ims.Y[z][y][x]<<","
		  <<(Select< Imc3duc::ValueType,Short>::Largest)ims.Z[z][y][x]<<")\t";
   }
   return SUCCESS;
}
Errc PContentsDisplay( const Imc3dsl &ims ) {
   register int z, y, x;
   
   std::cout<<"Color image 3d of "<<(TypeName<Imc3dsl::ValueType>::Name()).c_str() << std::endl;
   std::cout<<ims.Depth()<<" slices, "<<ims.Height()<<" rows, "<<ims.Width()<<" columns." << std::endl << std::endl;
   std::cout<<"Legend: z,y,x : (value,value,value)"<< std::endl;
   for (z = 0; z < ims.Depth(); z++)
   for (y = 0; y < ims.Height(); y++)
   for (x = 0; x < ims.Width(); x++)
   {
      if (ims.X[z][y][x] || ims.Y[z][y][x] || ims.Z[z][y][x])
	 std::cout<<z<<","<<y<<","<<x<<":("<<(Select< Imc3dsl::ValueType,Short>::Largest)ims.X[z][y][x]<<","
		  <<(Select< Imc3dsl::ValueType,Short>::Largest)ims.Y[z][y][x]<<","
		  <<(Select< Imc3dsl::ValueType,Short>::Largest)ims.Z[z][y][x]<<")\t";
   }
   return SUCCESS;
}
Errc PContentsDisplay( const Imc3dsf &ims ) {
   register int z, y, x;
   
   std::cout<<"Color image 3d of "<<(TypeName<Imc3dsf::ValueType>::Name()).c_str() << std::endl;
   std::cout<<ims.Depth()<<" slices, "<<ims.Height()<<" rows, "<<ims.Width()<<" columns." << std::endl << std::endl;
   std::cout<<"Legend: z,y,x : (value,value,value)"<< std::endl;
   for (z = 0; z < ims.Depth(); z++)
   for (y = 0; y < ims.Height(); y++)
   for (x = 0; x < ims.Width(); x++)
   {
      if (ims.X[z][y][x] || ims.Y[z][y][x] || ims.Z[z][y][x])
	 std::cout<<z<<","<<y<<","<<x<<":("<<(Select< Imc3dsf::ValueType,Short>::Largest)ims.X[z][y][x]<<","
		  <<(Select< Imc3dsf::ValueType,Short>::Largest)ims.Y[z][y][x]<<","
		  <<(Select< Imc3dsf::ValueType,Short>::Largest)ims.Z[z][y][x]<<")\t";
   }
   return SUCCESS;
}
Errc PContentsDisplay( const Imx3duc &ims ) {
   Point3d p;
 
   std::cout <<"Multispectral image 2d of " << (TypeName<Imx3duc::ValueType>::Name()).c_str() << std::endl;
   std::cout << ims.Width() << " columns." << std::endl << std::endl;
   std::cout << "Legend: z,y,x : (value, value, ...)" << std::endl;
   for (p.z = 0; p.z < ims.Depth(); p.z++)
   for (p.y = 0; p.y < ims.Height(); p.y++)
   for (p.x = 0; p.x < ims.Width(); p.x++)
   {
      // (Select::Largest) to avoid cout << char.
      bool empty=true;
      for (int b=0; b<ims.Bands(); b++) if (ims[b][p] != 0) { empty=false; break; }
      if (! empty) {
	 std::cout <<p.z<<","<<p.y<<","<<p.x<<":(";
	 std::cout << (Select< Imx3duc::ValueType, Short>::Largest)ims[0][p];
	 for (int b=1; b<ims.Bands(); b++) {
	    std::cout <<","<< (Select< Imx3duc::ValueType, Short>::Largest)ims[b][p];
	 }
	 std::cout <<")\t";
      }
   }
   return SUCCESS;
}
Errc PContentsDisplay( const Imx3dsl &ims ) {
   Point3d p;
 
   std::cout <<"Multispectral image 2d of " << (TypeName<Imx3dsl::ValueType>::Name()).c_str() << std::endl;
   std::cout << ims.Width() << " columns." << std::endl << std::endl;
   std::cout << "Legend: z,y,x : (value, value, ...)" << std::endl;
   for (p.z = 0; p.z < ims.Depth(); p.z++)
   for (p.y = 0; p.y < ims.Height(); p.y++)
   for (p.x = 0; p.x < ims.Width(); p.x++)
   {
      // (Select::Largest) to avoid cout << char.
      bool empty=true;
      for (int b=0; b<ims.Bands(); b++) if (ims[b][p] != 0) { empty=false; break; }
      if (! empty) {
	 std::cout <<p.z<<","<<p.y<<","<<p.x<<":(";
	 std::cout << (Select< Imx3dsl::ValueType, Short>::Largest)ims[0][p];
	 for (int b=1; b<ims.Bands(); b++) {
	    std::cout <<","<< (Select< Imx3dsl::ValueType, Short>::Largest)ims[b][p];
	 }
	 std::cout <<")\t";
      }
   }
   return SUCCESS;
}
Errc PContentsDisplay( const Imx3dsf &ims ) {
   Point3d p;
 
   std::cout <<"Multispectral image 2d of " << (TypeName<Imx3dsf::ValueType>::Name()).c_str() << std::endl;
   std::cout << ims.Width() << " columns." << std::endl << std::endl;
   std::cout << "Legend: z,y,x : (value, value, ...)" << std::endl;
   for (p.z = 0; p.z < ims.Depth(); p.z++)
   for (p.y = 0; p.y < ims.Height(); p.y++)
   for (p.x = 0; p.x < ims.Width(); p.x++)
   {
      // (Select::Largest) to avoid cout << char.
      bool empty=true;
      for (int b=0; b<ims.Bands(); b++) if (ims[b][p] != 0) { empty=false; break; }
      if (! empty) {
	 std::cout <<p.z<<","<<p.y<<","<<p.x<<":(";
	 std::cout << (Select< Imx3dsf::ValueType, Short>::Largest)ims[0][p];
	 for (int b=1; b<ims.Bands(); b++) {
	    std::cout <<","<< (Select< Imx3dsf::ValueType, Short>::Largest)ims[b][p];
	 }
	 std::cout <<")\t";
      }
   }
   return SUCCESS;
}

Errc PContentsDisplay( const Img1duc &ims ) {
   Point1d p;
   
   std::cout <<"Gray image 1d of " << (TypeName<Img1duc::ValueType>::Name()).c_str() << std::endl;
   std::cout << ims.Width() << " columns." << std::endl << std::endl;
   std::cout << "Legend: x : (value)" << std::endl;
   for (p.x = 0; p.x < ims.Width(); p.x++)
   {
      // (Select::Largest) to avoid cout << char.
      if (ims[p])
	 std::cout << p.x <<":("<<(Select< Img1duc::ValueType, Short>::Largest)ims[p]<<")\t";
   }
   return SUCCESS;
}
Errc PContentsDisplay( const Img1dsl &ims ) {
   Point1d p;
   
   std::cout <<"Gray image 1d of " << (TypeName<Img1dsl::ValueType>::Name()).c_str() << std::endl;
   std::cout << ims.Width() << " columns." << std::endl << std::endl;
   std::cout << "Legend: x : (value)" << std::endl;
   for (p.x = 0; p.x < ims.Width(); p.x++)
   {
      // (Select::Largest) to avoid cout << char.
      if (ims[p])
	 std::cout << p.x <<":("<<(Select< Img1dsl::ValueType, Short>::Largest)ims[p]<<")\t";
   }
   return SUCCESS;
}
Errc PContentsDisplay( const Img1dsf &ims ) {
   Point1d p;
   
   std::cout <<"Gray image 1d of " << (TypeName<Img1dsf::ValueType>::Name()).c_str() << std::endl;
   std::cout << ims.Width() << " columns." << std::endl << std::endl;
   std::cout << "Legend: x : (value)" << std::endl;
   for (p.x = 0; p.x < ims.Width(); p.x++)
   {
      // (Select::Largest) to avoid cout << char.
      if (ims[p])
	 std::cout << p.x <<":("<<(Select< Img1dsf::ValueType, Short>::Largest)ims[p]<<")\t";
   }
   return SUCCESS;
}
Errc PContentsDisplay( const Imx1duc &ims ) {
   Point1d p;
   
   std::cout <<"Multispectral image 1d of " << (TypeName<Imx1duc::ValueType>::Name()).c_str() << std::endl;
   std::cout << ims.Width() << " columns." << std::endl << std::endl;
   std::cout << "Legend: x : (value, value, ...)" << std::endl;
   for (p.x = 0; p.x < ims.Width(); p.x++)
   {
      // (Select::Largest) to avoid cout << char.
      bool empty=true;
      for (int b=0; b<ims.Bands(); b++) if (ims[b][p] != 0) { empty=false; break; }
      if (! empty) {
	 std::cout <<p.x<<":(";
         std::cout << (Select< Imx1duc::ValueType, Short>::Largest)ims[0][p];
	 for (int b=1; b<ims.Bands(); b++) {
	    std::cout <<","<< (Select< Imx1duc::ValueType, Short>::Largest)ims[b][p];
	 }
	 std::cout <<")\t";
      }
   }
   return SUCCESS;
}
Errc PContentsDisplay( const Imx1dsl &ims ) {
   Point1d p;
   
   std::cout <<"Multispectral image 1d of " << (TypeName<Imx1dsl::ValueType>::Name()).c_str() << std::endl;
   std::cout << ims.Width() << " columns." << std::endl << std::endl;
   std::cout << "Legend: x : (value, value, ...)" << std::endl;
   for (p.x = 0; p.x < ims.Width(); p.x++)
   {
      // (Select::Largest) to avoid cout << char.
      bool empty=true;
      for (int b=0; b<ims.Bands(); b++) if (ims[b][p] != 0) { empty=false; break; }
      if (! empty) {
	 std::cout <<p.x<<":(";
         std::cout << (Select< Imx1dsl::ValueType, Short>::Largest)ims[0][p];
	 for (int b=1; b<ims.Bands(); b++) {
	    std::cout <<","<< (Select< Imx1dsl::ValueType, Short>::Largest)ims[b][p];
	 }
	 std::cout <<")\t";
      }
   }
   return SUCCESS;
}
Errc PContentsDisplay( const Imx1dsf &ims ) {
   Point1d p;
   
   std::cout <<"Multispectral image 1d of " << (TypeName<Imx1dsf::ValueType>::Name()).c_str() << std::endl;
   std::cout << ims.Width() << " columns." << std::endl << std::endl;
   std::cout << "Legend: x : (value, value, ...)" << std::endl;
   for (p.x = 0; p.x < ims.Width(); p.x++)
   {
      // (Select::Largest) to avoid cout << char.
      bool empty=true;
      for (int b=0; b<ims.Bands(); b++) if (ims[b][p] != 0) { empty=false; break; }
      if (! empty) {
	 std::cout <<p.x<<":(";
         std::cout << (Select< Imx1dsf::ValueType, Short>::Largest)ims[0][p];
	 for (int b=1; b<ims.Bands(); b++) {
	    std::cout <<","<< (Select< Imx1dsf::ValueType, Short>::Largest)ims[b][p];
	 }
	 std::cout <<")\t";
      }
   }
   return SUCCESS;
}


#ifdef MAIN
#define	USAGE	"usage: %s [-m mask] [im_in|-]"
#define	PARC	0
#define	FINC	1
#define	FOUTC	0
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
   if (objs[0]->Type()==Po_Img2duc) {
      Img2duc* const ims=(Img2duc*)objs[0];
      
      result=PContentsDisplay(*ims);
      goto end;
   }
   if (objs[0]->Type()==Po_Img2dsl) {
      Img2dsl* const ims=(Img2dsl*)objs[0];
      
      result=PContentsDisplay(*ims);
      goto end;
   }
   if (objs[0]->Type()==Po_Img2dsf) {
      Img2dsf* const ims=(Img2dsf*)objs[0];
      
      result=PContentsDisplay(*ims);
      goto end;
   }
   if (objs[0]->Type()==Po_Reg2d) {
      Reg2d* const ims=(Reg2d*)objs[0];
      
      result=PContentsDisplay(*ims);
      goto end;
   }
   if (objs[0]->Type()==Po_Imc2duc) {
      Imc2duc* const ims=(Imc2duc*)objs[0];
      
      result=PContentsDisplay(*ims);
      goto end;
   }
   if (objs[0]->Type()==Po_Imc2dsl) {
      Imc2dsl* const ims=(Imc2dsl*)objs[0];
      
      result=PContentsDisplay(*ims);
      goto end;
   }
   if (objs[0]->Type()==Po_Imc2dsf) {
      Imc2dsf* const ims=(Imc2dsf*)objs[0];
      
      result=PContentsDisplay(*ims);
      goto end;
   }
   if (objs[0]->Type()==Po_Imx2duc) {
      Imx2duc* const ims=(Imx2duc*)objs[0];
      
      result=PContentsDisplay(*ims);
      goto end;
   }
   if (objs[0]->Type()==Po_Imx2dsl) {
      Imx2dsl* const ims=(Imx2dsl*)objs[0];
      
      result=PContentsDisplay(*ims);
      goto end;
   }
   if (objs[0]->Type()==Po_Imx2dsf) {
      Imx2dsf* const ims=(Imx2dsf*)objs[0];
      
      result=PContentsDisplay(*ims);
      goto end;
   }
   if (objs[0]->Type()==Po_Img3duc) {
      Img3duc* const ims=(Img3duc*)objs[0];				   
      result=PContentsDisplay(*ims);
      goto end;
   }
   if (objs[0]->Type()==Po_Img3dsl) {
      Img3dsl* const ims=(Img3dsl*)objs[0];				   
      result=PContentsDisplay(*ims);
      goto end;
   }
   if (objs[0]->Type()==Po_Img3dsf) {
      Img3dsf* const ims=(Img3dsf*)objs[0];				   
      result=PContentsDisplay(*ims);
      goto end;
   }
   if (objs[0]->Type()==Po_Reg3d) {
      Reg3d* const ims=(Reg3d*)objs[0];				   
      result=PContentsDisplay(*ims);
      goto end;
   }
   if (objs[0]->Type()==Po_Imc3duc) {
      Imc3duc* const ims=(Imc3duc*)objs[0];
      
      result=PContentsDisplay(*ims);
      goto end;
   }
   if (objs[0]->Type()==Po_Imc3dsl) {
      Imc3dsl* const ims=(Imc3dsl*)objs[0];
      
      result=PContentsDisplay(*ims);
      goto end;
   }
   if (objs[0]->Type()==Po_Imc3dsf) {
      Imc3dsf* const ims=(Imc3dsf*)objs[0];
      
      result=PContentsDisplay(*ims);
      goto end;
   }
   if (objs[0]->Type()==Po_Imx3duc) {
      Imx3duc* const ims=(Imx3duc*)objs[0];
      
      result=PContentsDisplay(*ims);
      goto end;
   }
   if (objs[0]->Type()==Po_Imx3dsl) {
      Imx3dsl* const ims=(Imx3dsl*)objs[0];
      
      result=PContentsDisplay(*ims);
      goto end;
   }
   if (objs[0]->Type()==Po_Imx3dsf) {
      Imx3dsf* const ims=(Imx3dsf*)objs[0];
      
      result=PContentsDisplay(*ims);
      goto end;
   }
   if (objs[0]->Type()==Po_Img1duc) {
      Img1duc* const ims=(Img1duc*)objs[0];
      
      result=PContentsDisplay(*ims);
      goto end;
   }
   if (objs[0]->Type()==Po_Img1dsl) {
      Img1dsl* const ims=(Img1dsl*)objs[0];
      
      result=PContentsDisplay(*ims);
      goto end;
   }
   if (objs[0]->Type()==Po_Img1dsf) {
      Img1dsf* const ims=(Img1dsf*)objs[0];
      
      result=PContentsDisplay(*ims);
      goto end;
   }
   if (objs[0]->Type()==Po_Imx1duc) {
      Imx1duc* const ims=(Imx1duc*)objs[0];
      
      result=PContentsDisplay(*ims);
      goto end;
   }
   if (objs[0]->Type()==Po_Imx1dsl) {
      Imx1dsl* const ims=(Imx1dsl*)objs[0];
      
      result=PContentsDisplay(*ims);
      goto end;
   }
   if (objs[0]->Type()==Po_Imx1dsf) {
      Imx1dsf* const ims=(Imx1dsf*)objs[0];
      
      result=PContentsDisplay(*ims);
      goto end;
   }
   if (objs[0]->Type()==Po_Graph2d) {
      Graph2d* const grs=(Graph2d*)objs[0];
      
      result=PContentsDisplay(*grs);
      goto end;
   }
   if (objs[0]->Type()==Po_Graph3d) {
      Graph3d* const grs=(Graph3d*)objs[0];
      
      result=PContentsDisplay(*grs);
      goto end;
   }
   if (objs[0]->Type()==Po_Collection) {
      Collection* const col=(Collection*)objs[0];
      
      result=PContentsDisplay(*col);
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
