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
 * @author Francois Angot - 1995-02-08
 * @author Regis Clouard - 2003-01-24
 */

#include <math.h>
#include <pandore.h>
using namespace pandore;

/**
 * @file pnodedisc.cpp	
 * Draw a cercle around graph nodes.
 */

static float Distancee( Point2d p1, Point2d p2 ) {
   return((float)(p1.x-p2.x)*(p1.x-p2.x)+(p1.y-p2.y)*(p1.y-p2.y));
}


static float Distancee( Point3d p1, Point3d p2 ) {
   return((float)(p1.x-p2.x)*(p1.x-p2.x)+(p1.y-p2.y)*(p1.y-p2.y)+(p1.z-p2.z)*(p1.z-p2.z));
}

void TraceDisque( Reg2d &rgd, Point2d centre, float rayon, Ulong coul ) {
   int xi, xf, yi, yf, x, y;
   float r;

   if (rayon <=0) return;
   
   r=rayon*rayon;
   xi=(int)(centre.x-rayon);
   if (xi<0) xi=0;
   xf=(int)(centre.x+rayon);
   if (xf>=rgd.Width()) xf=rgd.Width();
   yi=(int)(centre.y-rayon);
   if (yi<0) yi=0;
   yf=(int)(centre.y+rayon);
   if (yf>=rgd.Height()) yf=rgd.Height();   
   for (x=xi;x<xf;x++)
      for (y=yi;y<yf;y++)
	 if (Distancee(centre, Point2d(y, x))<r) {
	    rgd[y][x]=(Reg2d::ValueType)coul;
	 }
}

void TraceDisque( Reg3d &rgd, Point3d centre, float rayon, Ulong coul ) {
   int xi, xf, yi, yf, zi, zf, x, y, z;
   float r;

   if (rayon <=0) return;
   
   r=rayon*rayon*rayon;
   xi=(int)(centre.x-rayon);
   if (xi<0) xi=0;
   xf=(int)(centre.x+rayon);
   if (xf>=rgd.Width()) xf=rgd.Width();
   yi=(int)(centre.y-rayon);
   if (yi<0) yi=0;
   yf=(int)(centre.y+rayon);
   if (yf>=rgd.Height()) yf=rgd.Height();   
   zi=(int)(centre.z-rayon);
   if (zi<0) zi=0;
   zf=(int)(centre.z+rayon);
   if (zf>=rgd.Depth()) zf=rgd.Depth();   
   for (z=zi;z<zf;z++) {
      for (x=xi;x<xf;x++) {
	 for (y=yi;y<yf;y++) {
	    if (Distancee(centre, Point3d(z, y, x))<r) {
	       rgd[z][y][x]=(Reg3d::ValueType)coul;
	    }
	 }
      }
   }
}

/*
 * Trace des disques de rayon attr autour des sommets du graphe.
 */
Errc PNodeDisc( const Graph2d &gs, Reg2d &rgd ) {
   Ulong labels=0;
   rgd=0;  
   for (int i=0;i<gs.Size();i++) {
      if ((gs[i])) {
	 TraceDisque(rgd, gs[i]->seed, (float)gs[i]->value, gs[i]->Item());
	 if (gs[i]->Item() > (Long)labels) {
	    labels= gs[i]->Item();
	 }
      }
   }
   return rgd.Labels(labels);
}

/*
 * Trace des disques de rayon attr autour des sommets du graphe.
 */
Errc PNodeDisc( const Graph3d &gs, Reg3d &rgd ) {
   Ulong labels=0;
   rgd=0;

   for (int i=0;i<gs.Size();i++) {
      if ((gs[i])) {
	 TraceDisque(rgd, gs[i]->seed, (float)gs[i]->value, gs[i]->Item());
	 if (gs[i]->Item() > (Long)labels) {
	    labels= gs[i]->Item();
	 }
      }
   }
   return rgd.Labels(labels);
}

#ifdef MAIN

/*
 * Modify only the following constants, and the function call.
 */
#define	USAGE	"usage: %s [-m mask] [gr_in|-] [im_out|-]"
#define	PARC	0
#define	FINC	1
#define	FOUTC	1
#define	MASK	2

int main( int argc, char *argv[] ) {
   Errc result;	             // The result code of the execution.
   Pobject* mask;	     // The region mask.
   Pobject* objin[FINC+1];   // The input objects.
   Pobject* objs[FINC+1];    // The source objects masked by the mask.
   Pobject* objout[FOUTC+1]; // The output object.
   Pobject* objd[FOUTC+1];   // The result object of the execution.
   char* parv[PARC+1];       // The input parameters.
   
   ReadArgs(argc, argv, PARC, FINC, FOUTC, &mask, objin, objs, objout, objd, parv, USAGE, MASK);
   
   switch(objs[0]->Type()) {
   case Po_Graph2d : {
      Graph2d* const gs=(Graph2d*)objs[0];
      
      objd[0]=new Reg2d(gs->Height(), gs->Width());
      Reg2d* const rgd=(Reg2d*)objd[0];
      
      result=PNodeDisc(*gs, *rgd);
   } break;
   case Po_Graph3d : {
      Graph3d* const gs=(Graph3d*)objs[0];
      
      objd[0]=new Reg3d(gs->Depth(), gs->Height(), gs->Width());
      Reg3d* const rgd=(Reg3d*)objd[0];
      
      result=PNodeDisc(*gs, *rgd);
   } break;
   
   default:
      PrintErrorFormat(objin, FINC);
      result = FAILURE;
   }
   
   if (result) {
      WriteArgs(argc, argv, PARC, FINC, FOUTC, &mask, objin, objs, objout, objd, MASK);
   }
   
   Exit(result);
   return 0;
}

#endif
