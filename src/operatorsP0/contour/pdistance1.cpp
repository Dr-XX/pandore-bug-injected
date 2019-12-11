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
 * @author Régis Clouard - 1996-11-04
 * @author Francois Angot - 1999-10-13
 * @author Régis Clouard - 2002-05-22
 */

/**
 * @file pdistance1.cpp
 * Calculates distance to nearest contours.
 */

#include <pandore.h>
using namespace pandore;

/*
 * Parameters	:	d1	: linear distance 
 *			d2	: diagonal distance 
 * Algorithm	:	1- Set distance to 0 contour points, and to MAX others.
 *			2- For each point, set distance to min of all neighbours +d1 or +d2.
 */

int distanced1[]={0,9,11};
int distanced2[]={1,3,5,7,10,12};
int distanced3[]={2,4,6,8};



Errc PDistance1( const Img2duc &ims, Img2dsf &imd, float d1, float d2 ) {
   Point2d p;
   int v;
   float min, val=0;
   float dist = d2-d1;
   
   // Initialization (distance=0 to contours, MAXLONG to others).
   for (p.y=0;p.y<ims.Height(); p.y++)
      for (p.x=0;p.x<ims.Width(); p.x++)
	 imd[p] = (ims[p])? 0 : MAXFLOAT;
   
   // 1st pass : forward pass.
   for (p.y=0;p.y<imd.Height(); p.y++) {
      for (p.x=0;p.x<imd.Width(); p.x++) {
	 min = (float)imd[p];
	 for (v=0;v<4;v++) {
	    if (imd.Hold(p+v8[v]) && ((val=(float)(imd[p+v8[v]] + d1 + (v % 2)*dist)) < min)) {
	       min = val;
	    }
	 }
	 imd[p] = min;
      }
   }
   // 2nd pass; backward pass.
   for (p.y=imd.Height()-1; p.y>= 0; p.y--) {
      for (p.x=imd.Width()-1; p.x>= 0; p.x--) {
	 min = (float)imd[p];
	 for (v=4;v<8;v++) {
	    if (imd.Hold(p+v8[v]) && ((val=(float)(imd[p+v8[v]] + d1 + (v % 2)*dist)) < min)) {
	       min = val;
	    }
	 }
	 imd[p] = min;
      }
   }
   return SUCCESS;
}

Errc PDistance1( const Img2dsl &ims, Img2dsf &imd, float d1, float d2 ) {
   Point2d p;
   int v;
   float min, val=0;
   float dist = d2-d1;
   
   // Initialization (distance=0 to contours, MAXLONG to others).
   for (p.y=0;p.y<ims.Height(); p.y++)
      for (p.x=0;p.x<ims.Width(); p.x++)
	 imd[p] = (ims[p])? 0 : MAXFLOAT;
   
   // 1st pass : forward pass.
   for (p.y=0;p.y<imd.Height(); p.y++) {
      for (p.x=0;p.x<imd.Width(); p.x++) {
	 min = (float)imd[p];
	 for (v=0;v<4;v++) {
	    if (imd.Hold(p+v8[v]) && ((val=(float)(imd[p+v8[v]] + d1 + (v % 2)*dist)) < min)) {
	       min = val;
	    }
	 }
	 imd[p] = min;
      }
   }
   // 2nd pass; backward pass.
   for (p.y=imd.Height()-1; p.y>= 0; p.y--) {
      for (p.x=imd.Width()-1; p.x>= 0; p.x--) {
	 min = (float)imd[p];
	 for (v=4;v<8;v++) {
	    if (imd.Hold(p+v8[v]) && ((val=(float)(imd[p+v8[v]] + d1 + (v % 2)*dist)) < min)) {
	       min = val;
	    }
	 }
	 imd[p] = min;
      }
   }
   return SUCCESS;
}

Errc PDistance1( const Img2dsf &ims, Img2dsf &imd, float d1, float d2 ) {
   Point2d p;
   int v;
   float min, val=0;
   float dist = d2-d1;
   
   // Initialization (distance=0 to contours, MAXLONG to others).
   for (p.y=0;p.y<ims.Height(); p.y++)
      for (p.x=0;p.x<ims.Width(); p.x++)
	 imd[p] = (ims[p])? 0 : MAXFLOAT;
   
   // 1st pass : forward pass.
   for (p.y=0;p.y<imd.Height(); p.y++) {
      for (p.x=0;p.x<imd.Width(); p.x++) {
	 min = (float)imd[p];
	 for (v=0;v<4;v++) {
	    if (imd.Hold(p+v8[v]) && ((val=(float)(imd[p+v8[v]] + d1 + (v % 2)*dist)) < min)) {
	       min = val;
	    }
	 }
	 imd[p] = min;
      }
   }
   // 2nd pass; backward pass.
   for (p.y=imd.Height()-1; p.y>= 0; p.y--) {
      for (p.x=imd.Width()-1; p.x>= 0; p.x--) {
	 min = (float)imd[p];
	 for (v=4;v<8;v++) {
	    if (imd.Hold(p+v8[v]) && ((val=(float)(imd[p+v8[v]] + d1 + (v % 2)*dist)) < min)) {
	       min = val;
	    }
	 }
	 imd[p] = min;
      }
   }
   return SUCCESS;
}

Errc PDistance1( const Img3duc &ims, Img3dsf &imd, float d1, float d2, float d3 ) {
   Point3d p;
   float min, val=0;
   
   // Initialization (distance=0 to contours, MAXLONG to others).
   for (p.z=0;p.z<ims.Depth(); p.z++)
      for (p.y=0;p.y<ims.Height(); p.y++)
	 for (p.x=0;p.x<ims.Width(); p.x++)
	    imd[p] = (ims[p])? 0 : MAXFLOAT;
   
   // 1st pass : forward pass.
   for (p.z=0;p.z<imd.Depth(); p.z++)
      for (p.y=0;p.y<imd.Height(); p.y++)
	 for (p.x=1;p.x<imd.Width()-1; p.x++) {
	    min = (float)imd[p];
	    int i;
	    for (i=0;i<3;i++) {
	       if (imd.Hold(p+v26[distanced1[i]]) && ((val=(float)(imd[p+v26[distanced1[i]]]+ d1))<min))
		  min = val;
	    }
	    for (i=0;i<6;i++) {
	       if (imd.Hold(p+v26[distanced2[i]]) && ((val=(float)(imd[p+v26[distanced2[i]]]+ d2))<min))
		  min = val;
	    }
	    for (i=0;i<4;i++) {
	       if (imd.Hold(p+v26[distanced3[i]]) && ((val=(float)(imd[p+v26[distanced2[i]]]+ d3))<min))
		  min = val;
	    }
	    imd[p] = min;
	 }
   
   // 2nd pass; backward pass.
   for (p.z=imd.Depth()-1; p.z>= 0; p.z--) {
      for (p.y=imd.Height()-1; p.y>= 0; p.y--) {
	 for (p.x=imd.Width()-1; p.x>= 0; p.x--) {
	    min = (float)imd[p];
	    int i;
	    for (i=0;i<3;i++) {
	       if (imd.Hold(p+v26[distanced1[i]+13]) && ((val=(float)(imd[p+v26[distanced1[i]+13]]+ d1))<min))
		  min = val;
	    }
	    for (i=0;i<6;i++) {
	       if (imd.Hold(p+v26[distanced2[i]+13]) && ((val=(float)(imd[p+v26[distanced2[i]+13]]+ d2))<min))
		  min = val;
	    }
	    for (i=0;i<4;i++) {
	       if (imd.Hold(p+v26[distanced3[i]+13]) && ((val=(float)(imd[p+v26[distanced2[i]+13]]+ d3))<min))
		  min = val;
	    }
	    imd[p] = min;
	 }
      }
   }
   
   return SUCCESS;
}

Errc PDistance1( const Img3dsl &ims, Img3dsf &imd, float d1, float d2, float d3 ) {
   Point3d p;
   float min, val=0;
   
   // Initialization (distance=0 to contours, MAXLONG to others).
   for (p.z=0;p.z<ims.Depth(); p.z++)
      for (p.y=0;p.y<ims.Height(); p.y++)
	 for (p.x=0;p.x<ims.Width(); p.x++)
	    imd[p] = (ims[p])? 0 : MAXFLOAT;
   
   // 1st pass : forward pass.
   for (p.z=0;p.z<imd.Depth(); p.z++)
      for (p.y=0;p.y<imd.Height(); p.y++)
	 for (p.x=1;p.x<imd.Width()-1; p.x++) {
	    min = (float)imd[p];
	    int i;
	    for (i=0;i<3;i++) {
	       if (imd.Hold(p+v26[distanced1[i]]) && ((val=(float)(imd[p+v26[distanced1[i]]]+ d1))<min))
		  min = val;
	    }
	    for (i=0;i<6;i++) {
	       if (imd.Hold(p+v26[distanced2[i]]) && ((val=(float)(imd[p+v26[distanced2[i]]]+ d2))<min))
		  min = val;
	    }
	    for (i=0;i<4;i++) {
	       if (imd.Hold(p+v26[distanced3[i]]) && ((val=(float)(imd[p+v26[distanced2[i]]]+ d3))<min))
		  min = val;
	    }
	    imd[p] = min;
	 }
   
   // 2nd pass; backward pass.
   for (p.z=imd.Depth()-1; p.z>= 0; p.z--) {
      for (p.y=imd.Height()-1; p.y>= 0; p.y--) {
	 for (p.x=imd.Width()-1; p.x>= 0; p.x--) {
	    min = (float)imd[p];
	    int i;
	    for (i=0;i<3;i++) {
	       if (imd.Hold(p+v26[distanced1[i]+13]) && ((val=(float)(imd[p+v26[distanced1[i]+13]]+ d1))<min))
		  min = val;
	    }
	    for (i=0;i<6;i++) {
	       if (imd.Hold(p+v26[distanced2[i]+13]) && ((val=(float)(imd[p+v26[distanced2[i]+13]]+ d2))<min))
		  min = val;
	    }
	    for (i=0;i<4;i++) {
	       if (imd.Hold(p+v26[distanced3[i]+13]) && ((val=(float)(imd[p+v26[distanced2[i]+13]]+ d3))<min))
		  min = val;
	    }
	    imd[p] = min;
	 }
      }
   }
   
   return SUCCESS;
}

Errc PDistance1( const Img3dsf &ims, Img3dsf &imd, float d1, float d2, float d3 ) {
   Point3d p;
   float min, val=0;
   
   // Initialization (distance=0 to contours, MAXLONG to others).
   for (p.z=0;p.z<ims.Depth(); p.z++)
      for (p.y=0;p.y<ims.Height(); p.y++)
	 for (p.x=0;p.x<ims.Width(); p.x++)
	    imd[p] = (ims[p])? 0 : MAXFLOAT;
   
   // 1st pass : forward pass.
   for (p.z=0;p.z<imd.Depth(); p.z++)
      for (p.y=0;p.y<imd.Height(); p.y++)
	 for (p.x=1;p.x<imd.Width()-1; p.x++) {
	    min = (float)imd[p];
	    int i;
	    for (i=0;i<3;i++) {
	       if (imd.Hold(p+v26[distanced1[i]]) && ((val=(float)(imd[p+v26[distanced1[i]]]+ d1))<min))
		  min = val;
	    }
	    for (i=0;i<6;i++) {
	       if (imd.Hold(p+v26[distanced2[i]]) && ((val=(float)(imd[p+v26[distanced2[i]]]+ d2))<min))
		  min = val;
	    }
	    for (i=0;i<4;i++) {
	       if (imd.Hold(p+v26[distanced3[i]]) && ((val=(float)(imd[p+v26[distanced2[i]]]+ d3))<min))
		  min = val;
	    }
	    imd[p] = min;
	 }
   
   // 2nd pass; backward pass.
   for (p.z=imd.Depth()-1; p.z>= 0; p.z--) {
      for (p.y=imd.Height()-1; p.y>= 0; p.y--) {
	 for (p.x=imd.Width()-1; p.x>= 0; p.x--) {
	    min = (float)imd[p];
	    int i;
	    for (i=0;i<3;i++) {
	       if (imd.Hold(p+v26[distanced1[i]+13]) && ((val=(float)(imd[p+v26[distanced1[i]+13]]+ d1))<min))
		  min = val;
	    }
	    for (i=0;i<6;i++) {
	       if (imd.Hold(p+v26[distanced2[i]+13]) && ((val=(float)(imd[p+v26[distanced2[i]+13]]+ d2))<min))
		  min = val;
	    }
	    for (i=0;i<4;i++) {
	       if (imd.Hold(p+v26[distanced3[i]+13]) && ((val=(float)(imd[p+v26[distanced2[i]+13]]+ d3))<min))
		  min = val;
	    }
	    imd[p] = min;
	 }
      }
   }
   
   return SUCCESS;
}

Errc PDistance1( const Graph2d &grs, Graph2d &grd, float , float ) {
   int i,d;
   bool stop;
   Fifo<Point2d> f;
   Point2d pt;
   GEdge *ptr;
  
   grd=grs;
  
   d=1;
   for (i=1;i<grs.Size();i++) {
      grd[i]->value=0;
      if (!grs[i]->value) {
	 for (ptr=grs[i]->Neighbours();ptr!=NULL;ptr=ptr->Next()) {
	    if (grs[ptr->Node()]->value) {
	       f.Push(Point2d(i,i));
	       grd[i]->value=(Graph2d::ValueType)d;
	       break;
	    }
	 }
      }
   }
  
   f.Push(Point2d(0,0));
   stop = false;
   while (!stop) {
      pt=f.Pop();
      if (!pt.x) {
	 if (f.Empty()) {
	    stop=true;
	 } else {
	    d++;
	    f.Push(Point2d(0,0));
	 }
      } else {
	 for (ptr=grs[pt.x]->Neighbours();ptr!=NULL;ptr=ptr->Next()) {
	    if ((!grs[ptr->Node()]->value)&&(!grd[ptr->Node()]->value)) {
	       grd[ptr->Node()]->value=(Graph2d::ValueType)(d+1);
	       f.Push(Point2d(ptr->Node(),ptr->Node()));
	    }
	 }
      }
   }
   return SUCCESS;
}


#ifdef MAIN
#define USAGE "usage: %s d1 d2 d3 [-m mask] [im_in|-] [im_out|-]"
#define PARC  3
#define FINC  1
#define FOUTC 1
#define MASK  2

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
      objd[0]=new Img2dsf(ims->Size());
      Img2dsf* const imd=(Img2dsf*)objd[0];
      
      result=PDistance1(*ims,*imd, (float)atof(parv[0]), (float)atof(parv[1]));
      goto end;
}
   if (objs[0]->Type() == Po_Img2dsl) {
      Img2dsl* const ims=(Img2dsl*)objs[0];
      objd[0]=new Img2dsf(ims->Size());
      Img2dsf* const imd=(Img2dsf*)objd[0];
      
      result=PDistance1(*ims,*imd, (float)atof(parv[0]), (float)atof(parv[1]));
      goto end;
}
   if (objs[0]->Type() == Po_Img2dsf) {
      Img2dsf* const ims=(Img2dsf*)objs[0];
      objd[0]=new Img2dsf(ims->Size());
      Img2dsf* const imd=(Img2dsf*)objd[0];
      
      result=PDistance1(*ims,*imd, (float)atof(parv[0]), (float)atof(parv[1]));
      goto end;
}
   if (objs[0]->Type() == Po_Img3duc) {
      Img3duc* const ims=(Img3duc*)objs[0];
      objd[0]=new Img3dsf(ims->Props());
      Img3dsf* const imd=(Img3dsf*)objd[0];
      
      result=PDistance1(*ims,*imd, (float)atof(parv[0]), (float)atof(parv[1]), (float)atof(parv[2]));
      goto end;
}
   if (objs[0]->Type() == Po_Img3dsl) {
      Img3dsl* const ims=(Img3dsl*)objs[0];
      objd[0]=new Img3dsf(ims->Props());
      Img3dsf* const imd=(Img3dsf*)objd[0];
      
      result=PDistance1(*ims,*imd, (float)atof(parv[0]), (float)atof(parv[1]), (float)atof(parv[2]));
      goto end;
}
   if (objs[0]->Type() == Po_Img3dsf) {
      Img3dsf* const ims=(Img3dsf*)objs[0];
      objd[0]=new Img3dsf(ims->Props());
      Img3dsf* const imd=(Img3dsf*)objd[0];
      
      result=PDistance1(*ims,*imd, (float)atof(parv[0]), (float)atof(parv[1]), (float)atof(parv[2]));
      goto end;
}
   if (objs[0]->Type() == Po_Graph2d) {
      Graph2d* const grs=(Graph2d*)objs[0];
      objd[0]=new Graph2d(grs->Size());
      Graph2d* const grd=(Graph2d*)objd[0];
      result=PDistance1(*grs,*grd,(float)atof(parv[0]), (float)atof(parv[1]));
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
