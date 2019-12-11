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
 * @author Régis Clouard - 1997-12-23
 * @author Régis Clouard - 2003-05-15 (fix bug)
 * @author Régis Clouard - 2004-06-29 (>=, <=)
 * @author Regis Clouard - 2006-06-02 (relation =3 and -3).
 */

#define _USE_MATH_DEFINES
#include <cmath>
#include <pandore.h>
using namespace pandore;

/**
 * @file prectangularityselection.cpp
 * Selects regions from rectangularity degree.
 * Remarque : un rectangle = surface region / surface du rectangle exinscrit.
 */

#define ANGLE 10

struct attrs_rectangularite {
   float xmin[ANGLE],ymin[ANGLE],xmax[ANGLE],ymax[ANGLE];
   float rectangularite;
   float surface;
   bool acceptee;
};

/*
 * Fonction qui calcule le facteur de rectangularite de chacune des
 * regions presentes.
 * ANGLE directions sont essayees en meme temps : 0, 1*90/ANGLE, 2*90/ANGLE...
 * La rectangularite est pris sur celle qui donne la plus forte valeur.
 */
Errc PRectangularitySelection( const Reg2d &rgs, struct attrs_rectangularite *regions ) {
   register int x,y;
   float x1,y1;
   float lx;
   float ly;
   int ang;
   float rect;
   double teta;
   Reg2d::ValueType r;

   // Initialisation.
   for (r=1;r<=rgs.Labels();r++) {
      for (ang=0;ang<ANGLE;ang++) {
	 regions[r].xmin[ang]=(float)rgs.Height()*rgs.Width();
	 regions[r].ymin[ang]=(float)rgs.Height()*rgs.Width();
	 regions[r].xmax[ang]=(float)-rgs.Height()*rgs.Width();;
	 regions[r].ymax[ang]=(float)-rgs.Height()*rgs.Width();;
      }
   }
   
   // Calcule le rectangle exinscrit dans chacun des reperes tournants.
   for (y=0;y<rgs.Height(); y++) {
      for (x=0;x<rgs.Width(); x++) {
	 if ((r=rgs[y][x]) > 0) {
	    regions[r].surface++;
	    int v;
	    bool bord = false;
	    for (v = 0; v < 4; v++) {
	       if ( (!(rgs.Hold(y+v4[v].y,x+v4[v].x))) || (rgs[y+v4[v].y][x+v4[v].x]!=r)) {
		  bord=true;
		  break;
	       }
	    }
	    if (!bord) {
	       for (v = 0; v < 4; v++) {
		  if (rgs[y+v8[v*2+1].y][x+v8[v*2+1].x]!=r) {
		     // Pour les creux on ajoute un bon demi de surface.
		     regions[r].surface+=0.5;	
		  }
	       }
	    }

	    for (ang = 0; ang < ANGLE; ang++) {
	       if (x==0 && y==0) {
		  x1 = y1 = 0.0F;
	       } else {
		  teta=-((double)ang*M_PI/(2*ANGLE)) + (atan2((double)y,(double)x));
		  // Calcul des coordonnees en flottant pour eviter
		  // les problemes d'arrondi.
		  x1 = (float)(sqrt((double)(y*y)+(double)(x*x))*sin(teta));
	          y1 = (float)(sqrt((double)(y*y)+(double)(x*x))*cos(teta));
	       }
	       
	       regions[r].xmin[ang]=MIN(x1,regions[r].xmin[ang]);
	       regions[r].xmax[ang]=MAX(x1,regions[r].xmax[ang]);
	       regions[r].ymin[ang]=MIN(y1,regions[r].ymin[ang]);
	       regions[r].ymax[ang]=MAX(y1,regions[r].ymax[ang]);
	    }
	 }
      }
   }

   for (r=1;r<=rgs.Labels();r++) {
      regions[r].rectangularite=0;
      for (ang=0;ang<ANGLE;ang++) {
	 lx=fabs(regions[r].xmax[ang]-regions[r].xmin[ang])+1.0F;
	 ly=fabs(regions[r].ymax[ang]-regions[r].ymin[ang])+1.0F;
	 if ((lx*ly)>0) {
	    rect=regions[r].surface/(ly*lx);
	    if (rect > regions[r].rectangularite) {
	       if (rect > 1) {
		  regions[r].rectangularite=1;
	       } else {
		  regions[r].rectangularite=rect;
	       }
	    }
	 }
      }
   }

   return SUCCESS;
}

/*
 * Selectionne les regions ayant une rectangularite en nombre de pixels
 * inferieure si relation < 0, egal si relation =0 , superieur sinon
 * a la valeur fournie.
 */
Errc PRectangularitySelection( const Reg2d &rgs, Reg2d &rgd, int relation, float threshold ) {
   register int y,x;
   struct attrs_rectangularite *regions;
   Ulong nreg=0,r;

   // Initialization  
   regions = (struct attrs_rectangularite*)calloc(rgs.Labels()+1,sizeof(struct attrs_rectangularite));
  
   PRectangularitySelection(rgs,regions); // Calcule la valeur d'rectangularite.
  
   float temp;
   // Selection des regions
   if (relation <= -3) {
      temp=regions[1].rectangularite;
      for (r=2;r<=rgs.Labels();r++)
	 if (regions[r].rectangularite<temp) {
	    temp=regions[r].rectangularite;
	 }
      for (r=1;r<=rgs.Labels();r++)
	 if (regions[r].rectangularite==temp) {
	    regions[r].acceptee=true;
	    ++nreg;
	 } else {
	    regions[r].acceptee=false;
	 }
   } else if (relation >= 3) {
      temp=regions[1].rectangularite;
      for (r=2;r<=rgs.Labels();r++)
	 if (regions[r].rectangularite>temp) {
	    temp=regions[r].rectangularite;
	 }
      for (r=1;r<=rgs.Labels();r++)
	 if (regions[r].rectangularite==temp) {
	    regions[r].acceptee=true;
	    ++nreg;
	 } else {
	    regions[r].acceptee=false;
	 }
   } else if (relation == 2) {
      for (r=1;r<=rgs.Labels();r++) {
	 if (regions[r].rectangularite>=threshold) {
	    regions[r].acceptee=true;
	    ++nreg;
	 }else {
	    regions[r].acceptee=false;
	 }
      }
   } else if (relation == 1) {
      for (r=1;r<=rgs.Labels();r++) {
	 if (regions[r].rectangularite>threshold) {
	    regions[r].acceptee=true;
	    ++nreg;
	 } else {
	    regions[r].acceptee=false;
	 }
      }
   } else if (relation == 0) {
      for (r=1;r<=rgs.Labels();r++) {
	 if (regions[r].rectangularite == threshold) {
	    regions[r].acceptee=true;
	    ++nreg;
	 } else {
	    regions[r].acceptee=false;
	 }
      }
   } else if (relation == -1) {
      for (r=1;r<=rgs.Labels();r++) {
	 if (regions[r].rectangularite < threshold) {
	    regions[r].acceptee=true;
	    ++nreg;
	 } else {
	    regions[r].acceptee=false;
	 }
      }
   } else /* (relation == -2) */ {
      for (r=1;r<=rgs.Labels();r++) {
	 if (regions[r].rectangularite <= threshold) {
	    regions[r].acceptee=true;
	    ++nreg;
	 } else {
	    regions[r].acceptee=false;
	 }
      }
   }
  

   // Construction de la carte de sortie.
   for (y=0;y<rgs.Height(); y++)
      for (x=0;x<rgs.Width(); x++) {
	 if (regions[rgs[y][x]].acceptee) {
	    rgd[y][x] = rgs[y][x];
	 } else {
	    rgd[y][x] = 0;
	 }
      }

   rgd.Labels(rgs.Labels());
   free(regions);
   return nreg;
}

#ifdef MAIN

/*
 * Modify only the following constants, and the function call.
 */
#define	USAGE	"usage: %s relation threshold [-m mask] [rg_in|-] [rg_out|-]"
#define	PARC	2
#define	FINC	1
#define	FOUTC	1
#define	MASK	2

int main( int argc, char *argv[] ) {
   Errc result;              // The result code of the execution.
   Pobject* mask;	     // The region stencil.
   Pobject* objin[FINC+1];   // The input objects.
   Pobject* objs[FINC+1];    // The source objects masked by the stencil.
   Pobject* objout[FOUTC+1]; // The output object.
   Pobject* objd[FOUTC+1];   // The result object of the execution.
   char* parv[PARC+1];	     // The input parameters.

   ReadArgs(argc,argv,PARC,FINC,FOUTC,&mask,objin,objs,objout,objd,parv,USAGE,MASK);

   switch(objs[0]->Type()) {
   case Po_Reg2d :{
      Reg2d* const rgs=(Reg2d*)objs[0];
      objd[0]=new Reg2d(rgs->Size());
      Reg2d* const rgd=(Reg2d*)objd[0];

      result=PRectangularitySelection(*rgs,*rgd,atoi(parv[0]),(float)atof(parv[1]));
   }break;
   default:
      PrintErrorFormat(objin,FINC);
      result=FAILURE;
   }

   if (result) {
      WriteArgs(argc,argv,PARC,FINC,FOUTC,&mask,objin,objs,objout,objd,MASK);
   }

   Exit(result);
   return(0);
}

#endif
