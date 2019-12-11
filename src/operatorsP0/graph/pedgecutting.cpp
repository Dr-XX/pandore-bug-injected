/* -*- c-basic-offset: 3 -*-
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
 * @author Regis Clouard - 2007-03-14
 * @author Regis Clouard - 2007-03-30 (fix error on case high<low)
 */

/**
 * @file pedgecutting.cpp
 * Cuts edges from their values.
 */

#include <pandore.h>
using namespace pandore;

Errc PEdgeCutting( const Graph2d &gs, Graph2d &gd, Double low, Double high ) {
   gd=gs;

   if (low<(Double)Limits<Double>::min())
      low=(Double)Limits<Double>::min();
   else if (low>(Double)Limits<Double>::max())
      low=(Double)Limits<Double>::max();

   if (high>(Double)Limits<Double>::max())
      high=(Double)Limits<Double>::max();
   else if (high<(Double)Limits<Double>::min())
      high=(Double)Limits<Double>::min();

   if (high<low)
      for (int i=0; i<gs.Size(); i++) {
	 if (!gs[i]) continue;
	 GEdge *ptr;
	 for (ptr=gd[i]->Neighbours();ptr!=NULL;ptr=ptr->Next() ) {
	    if (ptr->weight>low || ptr->weight<high) {
	       gd.Unlink(i,ptr->Node());
	    }
	 }
      }   
   else
      for (int i=0; i<gs.Size(); i++) {
	 if (!gs[i]) continue;
	 GEdge *ptr;
	 for (ptr=gd[i]->Neighbours();ptr!=NULL;ptr=ptr->Next() ) {
	    if (ptr->weight>=low && ptr->weight<=high) {
	       gd.Unlink(i,ptr->Node());
	    }
	 }
      }   
   
   return SUCCESS;
}

Errc PEdgeCutting( Graph3d &gs, Graph3d &gd, Double low, Double high ) {
   gd=gs;

   if (low<(Double)Limits<Double>::min())
      low=(Double)Limits<Double>::min();
   else if (low>(Double)Limits<Double>::max())
      low=(Double)Limits<Double>::max();

   if (high>(Double)Limits<Double>::max())
      high=(Double)Limits<Double>::max();
   else if (high<(Double)Limits<Double>::min())
      high=(Double)Limits<Double>::min();

   if (high<low)
      for (int i=0; i<gs.Size(); i++) {
	 if (!gs[i]) continue;
	 GEdge *ptr;
	 for (ptr=gd[i]->Neighbours();ptr!=NULL;ptr=ptr->Next() ) {
	    if (ptr->weight>low || ptr->weight<high)
	       gd.Unlink(i,ptr->Node());
	 }
      }   
   else
      for (int i=0; i<gs.Size(); i++) {
	 if (!gs[i]) continue;
	 GEdge *ptr;
	 for (ptr=gd[i]->Neighbours();ptr!=NULL;ptr=ptr->Next() ) {
	    if (ptr->weight>=low && ptr->weight<=high)
	       gd.Unlink(i,ptr->Node());
	 }
      }   
   
   return SUCCESS;
}

#ifdef MAIN

/*
 * Modify only the following constants, and the function call.
 */
#define	USAGE	"usage: %s low high [-m mask] [gr_in|-] [gr_out|-]"
#define	PARC	2
#define	FINC	1
#define	FOUTC	1
#define	MASK	1 // Making , Unmasking.

int main( int argc, char *argv[] ) { 
   Errc result;              // The result code of the execution.
   Pobject* mask;            // The region mask.
   Pobject* objin[FINC+1];   // The input objects.
   Pobject* objs[FINC+1];    // The source objects masked.
   Pobject* objout[FOUTC+1]; // The output object.
   Pobject* objd[FOUTC+1];   // The result object of the execution.
   char* parv[PARC+1];       // The input parameters.
   
   ReadArgs(argc,argv,PARC,FINC,FOUTC,&mask,objin,objs,objout,objd,parv,USAGE,MASK);
   
   switch(objs[0]->Type()) {
   case Po_Graph2d :{
      Graph2d* const gs=(Graph2d*)objs[0];
      objd[0]=new Graph2d(gs->Size());
      Graph2d* const gd=(Graph2d*)objd[0];
      
      result=PEdgeCutting(*gs,*gd,(Double)atof(parv[0]),(Double)atof(parv[1]));
   }break;
   
   case Po_Graph3d :{
      Graph3d* const gs=(Graph3d*)objs[0];
      objd[0]=new Graph3d(gs->Size());
      Graph3d* const gd=(Graph3d*)objd[0];

      result=PEdgeCutting(*gs,*gd,(Double)atof(parv[0]),(Double)atof(parv[1]));
   }break;
   
   default:{
      PrintErrorFormat(objin,FINC);
      result = FAILURE;
   }
   }
   
   if (result)
      WriteArgs(argc,argv,PARC,FINC,FOUTC,&mask,objin,objs,objout,objd,MASK);
   Exit(result);
   return 0;
}

#endif
