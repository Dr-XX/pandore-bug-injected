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
 * @author Laurent Quesnel - 2002-06-10
 * @author Régis Clouard - Nov 28, 2006
 * @author Régis Clouard - May 25, 2011
 */

#define _USE_MATH_DEFINES
#include <cmath>
#include <iostream>
#include <pandore.h>
using namespace pandore; 

/**
 * @file phoughlines.cpp
 * Algorithm for Hough Transform
 */

const double deg2rad = M_PI/180.0f;

class Triplet {
   public:
      Triplet() {}
      Long rho;
      Long theta;
      Long votes;
};

class Accumulator {
   public:
      Accumulator( int rhoRange, int minTheta, int maxTheta ): _rhoRange(rhoRange), _thetaRange(180), _minTheta(minTheta), _maxTheta(maxTheta) {
	 _votes = new long*[2*rhoRange];
	 for (int r=0; r < 2*rhoRange; r++) {
	    _votes[r] = new long[_thetaRange+1]; // + 1 for 180
	    for (int o=0; o < _thetaRange+1; o++) {
	       _votes[r][o] = 0;
	    }
	 }
	 _tabCos = new float[_thetaRange+1];
	 _tabSin = new float[_thetaRange+1];
 	 for ( int o=0; o<_thetaRange+1; o++ ) {
 	    _tabSin[o] = (float)(sin(o*deg2rad));
	    _tabCos[o] = (float)(cos(o*deg2rad));
	 }
      }
      ~Accumulator() {   // Free accumulator
	 for (int r=0; r < 2*_rhoRange; r++) {
	    delete[] _votes[r];
	 }
	 delete[] _votes;
	 delete[] _tabCos;
	 delete[] _tabSin;
      }
      
      void Add( int y, int x ) {
	 for (int o=_minTheta; o<_maxTheta; o++) {
	    const int angle = o%180;
 	    const double rho = x*_tabCos[angle]+y*_tabSin[angle];
	    _votes[(int)(rho+_rhoRange+0.5)][angle]++;
	 }
      }
      void Remove( int y, int x ) {
	 for (int o=0; o<_thetaRange; o++) {
 	    const double rho = x*_tabCos[o]+y*_tabSin[o];
	    _votes[(int)(rho+_rhoRange+0.5)][o]--;
	 }
      }

      Triplet GetMaxVotes() {
	 Triplet t;
	 t.votes=0;
	 // merge 0 and 180.
	 for (int r=0; r<2*_rhoRange; r++) {
	    _votes[r][0]= _votes[r][_thetaRange];
	    _votes[r][_thetaRange]=0;
	    for (int o=0; o<_thetaRange; o++) {
	       if (_votes[r][o] > t.votes) {
		  t.rho = r-_rhoRange;
		  t.theta = o;
		  t.votes = _votes[r][o];
	       }
	    }
	 }
	 return t;
      }
   private:
      int _rhoRange;
      int _thetaRange;
      int _minTheta;
      int _maxTheta;
      float *_tabSin;
      float *_tabCos;
      long **_votes;
};

/**
 * Displays a straight line on the output image imd, and
 * removes the related parameters from the accumulator.
 */
static void DrawLine( Img2duc &imd, const Point2d pti, const Point2d ptf, const Img2duc &ims, Accumulator &accumulator, int lineThickness ) {
   int x,y,max,min;
   float pente;

   // Write the related line into the output image.
   if (pti.x!=ptf.x)
      pente=(float)(pti.y-ptf.y)/(pti.x-ptf.x);
   else
      pente=MAXFLOAT;
   
   if ((pente<-1) || (pente>1)) {
      if (ptf.y<pti.y) {
	 min=ptf.y; max=pti.y;
      } else {
	 min=pti.y; max=ptf.y;
      }
      for (y=max;y>=min;y--) {
	 x=(int)((y-ptf.y)/pente +ptf.x);
	 if (imd.Hold(y,x)) {
	    imd(y,x)=255;
	    // Remove related parameters from the accumulator
	    // Line thickness is 3.
	    for (int x1=x-lineThickness; x1<=x+lineThickness; x1++) {
	       if (x1 >=0 && x1<imd.Width() && ims(y,x1)>0) {
		  accumulator.Remove(y,x1);
	       }
	    }
	 }
      }
   } else {
      if (ptf.x<pti.x) {
	 min=ptf.x; max=pti.x;
      } else {
	 min=pti.x; max=ptf.x;
      }
      for (x=max;x>=min;x--) {
	 y=(int)(pente*(x-ptf.x)+ptf.y);
	 if (imd.Hold(y,x)) {
	    imd(y,x)=255;
	    // Remove related parameters from the accumulator
	    // Line thinkness is 3.
	    for (int y1=y-lineThickness; y1<=y+lineThickness;y1++) {
	       if (y1 >=0 && y1<imd.Height() && ims(y1,x)>0) {
		  accumulator.Remove(y1,x);
	       }
	    }
	 }
      }
   }
}

static void BuildStraightLine( Img2duc &imd, Long rho, Long angle, const Img2duc &ims, Accumulator &accumulator,int lineThickness) {
   const double deg2rad = M_PI/180.0f;
   // the line equation is: c = ax + by --> y = (-ax + c)/b
   double a = cos(angle*deg2rad);
   double b = sin(angle*deg2rad);
   double c = rho;
 
   int x0 = -imd.Width();
   int x1 = imd.Width();
   double y0, y1;

   if (b != 0) {
      y0 = (-a*x0 + c)/b;
      y1 = (-a*x1 + c)/b;

      Point2d pti((int)(y0 + 0.5), x0);
      Point2d ptf((int)(y1 + 0.5), x1);
      DrawLine(imd, pti, ptf, ims, accumulator,lineThickness);
   } else {
      x0 = rho;
      y0 = 0;
      x1 = rho;
      y1 =imd.Height();

      Point2d pti((int)(y0 + 0.5), x0);
      Point2d ptf((int)(y1 + 0.5), x1);
      DrawLine(imd, pti, ptf, ims, accumulator,lineThickness);
   }
}

/**
 * @param lines the maximum number of lines to detect.
 * @param minangle the minimum angle for acceptable lines;
 * @param maxangle the maximum angle for acceptable lines.
 */
Errc PHoughLines( const Img2duc &ims, Img2duc &imd, int lines, Long minangle, Long maxangle, int lineThickness) {
   if (lines<=0) {
      std::cerr << "Error phouglines: Bad parameter value for lines: " << lines<< std::endl;
      return FAILURE;
   }

   const int rhoRange = (Long)ceil(sqrt((double)SQR(ims.Height())+SQR(ims.Width())));

   Long minTheta = (450-maxangle)%360;
   Long maxTheta = (450-minangle)%360;
   if (minTheta>maxTheta) maxTheta+=360;

#ifndef NDEBUG
   fprintf(stderr,"anglemin = %d angle max = %d\n",minTheta, maxTheta);
#endif

   // Accumulator is used to store all line coordinates.
   Accumulator accumulator(rhoRange, minTheta, maxTheta); 

   // Transform into polar coordinates: r>=0 et 2*M_PI>=o>=0
   for (int y=0; y < ims.Height(); y++) {
      for (int x=0; x < ims.Width(); x++) {
	 if ((ims(y,x))) { // a contour point
	    accumulator.Add(y, x);
	 }
      }
   }
   imd=0;
   
   // Detect the requested number of lines ...
   int detectedLines =0;
   while (lines>0) {
      // Search for the maximum votes.
      Triplet max = accumulator.GetMaxVotes();
      if (max.votes<1) break;

#ifndef NDEBUG
      fprintf(stderr,"Max value = %ld in rho=%ld and teta=%ld\n",(long)max.votes,(long)max.rho,(long)max.theta);
#endif
      BuildStraightLine(imd,max.rho,max.theta,ims,accumulator,lineThickness);
      lines--;
      detectedLines++;
   }; // fin du while

   return detectedLines;
}


#ifdef MAIN

/*
 * Modify only the following constants, and the operator switches.
 */

#define	USAGE	"usage: %s lines minangle maxangle linethickness [-m mask] [im_in|-] [im_out|-]"
#define	PARC	4  // Number of parameters
#define	FINC	1  // Number of input images
#define	FOUTC	1  // Number of output images
#define	MASK	1  // Level of masking

int main( int argc, char *argv[] ) {
   Errc result;              // The result code of the execution.
   Pobject* mask;            // The region map.
   Pobject* objin[FINC+1];   // The input objects.
   Pobject* objs[FINC+1];    // The source objects masked.
   Pobject* objout[FOUTC+1]; // The output object.
   Pobject* objd[FOUTC+1];   // The result object of the execution.
   char* parv[PARC+1];       // The input parameters.

   ReadArgs(argc,argv,PARC,FINC,FOUTC,&mask,objin,objs,objout,objd,parv,USAGE,MASK);
   if (objs[0]->Type() == Po_Img2duc) {
      Img2duc* const ims=(Img2duc*)objs[0];
      objd[0]=new Img2duc(ims->Size());
      Img2duc* const imd=(Img2duc*)objd[0];

      result=PHoughLines(*ims,*imd,atoi(parv[0]),(Long)atoi(parv[1]),(Long)atoi(parv[2]),(int)atoi(parv[3]));
      goto end;
   }	
   {
      PrintErrorFormat(objin,FINC);
      result = FAILURE;
   }	

 end:
   if (result) {
      WriteArgs(argc,argv,PARC,FINC,FOUTC,&mask,objin,objs,objout,objd,MASK);
   }
   Exit(result);
   return 0;
}
#endif
