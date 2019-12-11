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
 * @author Regis Clouard - 06 May 2011
 */

#include <pandore.h>
using namespace pandore;

Errc PEdgeBasedRAGPruning( const Reg2d &rgs, const Graph2d &gs, const Img2duc &ims, Graph2d &gd ) {
   int count=0;

   Point2d p;
   gd=gs;
   for (p.y=0; p.y<ims.Height(); p.y++ ) {
      for (p.x=0; p.x<ims.Width(); p.x++ ) {
	 if (ims[p]>0) {
	    // Remove link between two adjacencents regions
	    // if the current edge point
	    // is on two regions boundary
	    for (int v=0; v<2; v++ ) {
	       Point2d p1=p+v4[v];
	       Point2d p2=p+v4[v+2];
	       if (rgs.Hold(p1) && rgs.Hold(p2)
		   && ims[p1]==0 && ims[p2]==0 
		   && (rgs[p1]!=rgs[p2]) ) {
		  // unlink the two regions.
		  gd.Unlink((Long)rgs[p1],(Long)rgs[p2]);
		  count++;
	       }
	    }
	 }
      }
   }
   
   return count; // retourne le nombre de regions fusionnees
}

Errc PEdgeBasedRAGPruning( const Reg3d &rgs, const Graph3d  &gs, const Img3duc &ims, Graph3d &gd ) {
   int count=0;

   Point3d p;
   gd=gs;
   for (p.z=0; p.z<ims.Depth(); p.z++ ) {
      for (p.y=0; p.y<ims.Height(); p.y++ ) {
	 for (p.x=0; p.x<ims.Width(); p.x++ ) {
	    if (ims[p]>0) {
	       // Remove link between two adjacencents regions
	       // if the current edge point
	       // is on two regions boundary
	    for (int v=0; v<3; v++ ) {
	       Point3d p1=p+v6[v];
	       Point3d p2=p+v6[v+3];
	       if (rgs.Hold(p1) && rgs.Hold(p2)
		   && ims[p1]==0 && ims[p2]==0 
		   && (rgs[p1]!=rgs[p2]) ) {
		  // unlink the two regions.
		  gd.Unlink((Long)rgs[p1],(Long)rgs[p2]);
		  count++;
	       }
	    }
	    }
	 }
      }
   }
   return count; // retourne le nombre de regions fusionnees
}



#ifdef MAIN

/*
 * Modify only the following constants, and the operator switches.
 */
#define	USAGE	"usage: %s [-m mask] [rg_in|-] [gr_in|-] [im_in|-] [gr_out|-]"
#define	PARC	0  // Number of parameters
#define	FINC	3  // Number of input images
#define	FOUTC	1  // Number of output images
#define	MASK	0  // Level of masking

int main( int argc, char *argv[] ) {
   Errc result;                // The result code of the execution.
   Pobject* mask;              // The region map.
   Pobject* objin[FINC + 1];   // The input objects.
   Pobject* objs[FINC + 1];    // The source objects masked.
   Pobject* objout[FOUTC + 1]; // The output object.
   Pobject* objd[FOUTC + 1];   // The result object of the execution.
   char* parv[PARC + 1];       // The input parameters.

   ReadArgs(argc, argv, PARC, FINC, FOUTC, &mask, objin, objs, objout, objd, parv, USAGE, MASK); 
   // verifier toutes les entrees
   if (objs[0]->Type()==Po_Reg2d && objs[1]->Type()==Po_Graph2d && objs[2]->Type() == Po_Img2duc) {
      Reg2d* const rgs=(Reg2d*)objs[0];  
      Graph2d* const gs=(Graph2d*)objs[1];  
      Img2duc* const ims=(Img2duc*)objs[2];
      objd[0]=new Graph2d(gs->Size());
      Graph2d* const gd=(Graph2d*)objd[0];
      
      result = PEdgeBasedRAGPruning(*rgs,*gs,*ims,*gd);
      goto end;
   }
   // verifier toutes les entrees
   if (objs[0]->Type()==Po_Reg3d && objs[1]->Type()==Po_Graph3d && objs[2]->Type() == Po_Img3duc) {
      Reg3d* const rgs=(Reg3d*)objs[0];  
      Graph3d* const gs=(Graph3d*)objs[1];  
      Img3duc* const ims=(Img3duc*)objs[2];
      objd[0]=new Graph3d(gs->Size());
      Graph3d* const gd=(Graph3d*)objd[0];
      
      result = PEdgeBasedRAGPruning(*rgs,*gs,*ims,*gd);
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
