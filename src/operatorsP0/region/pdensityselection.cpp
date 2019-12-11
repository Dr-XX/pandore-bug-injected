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
 * @author Regis Clouard - Jan 24, 2012
 */

#include <math.h>
#include <pandore.h>
using namespace pandore;

/**
 * @file pdensityselection.cpp
 * Select regions from their density degree.
 * density = inscribing circle radius / circumscribing circle radius.
 */
struct attrs_density {
   Ulong surface;
   Ulong pixelCount;
   float density;
   bool acceptee;
};

/**
 * 1- find center of mass.
 * 2 -find max radius of inscribing radius (farest from the center of mass)
 *    and min radius of circumscribing radius (nearest from the center of mass).
 */
Errc PDensitySelection( const Reg2d &rgs, const Img2duc &ims, struct attrs_density *regions ) {
   register int	y,x;
   Ulong r;
   
   if (rgs.Labels() <1) {
      std::cerr << "Error pdensityselection: Bad region map: no region inside." << std::endl;
      return FAILURE;
   }
   
   for (y=0;y<rgs.Height(); y++) {
      for (x=0;x<rgs.Width(); x++) {
	 if ((r=rgs(y,x))>0) {
	    regions[r].surface++;
	    if (ims(y,x)>0) regions[r].pixelCount++;
	 }
      }
   }

   for (r=1;r<=rgs.Labels();r++) {
      if (regions[r].surface>0) {
	 regions[r].density=(float)regions[r].pixelCount/regions[r].surface;
      }
   }

   return SUCCESS;
}

/*
 * Selectionne les regions ayant une densite en nombre de pixels
 * inferieure si relation < 0, egal si relation =0 , superieur sinon
 * a la valeur fournie.
 */
Errc PDensitySelection( const Reg2d &rgs, const Img2duc &ims, Reg2d &rgd, int relation, float threshold ) {
   register int y,x;
   struct attrs_density *regions;
   Ulong nreg=0,r;

   // Initialization  
   regions = (struct attrs_density*)calloc(rgs.Labels()+1,sizeof(struct attrs_density));

   PDensitySelection(rgs, ims, regions); // Calcule la valeur de densité.

   float temp;
   // Selection des regions
   if (relation <= -3) {
      temp=regions[1].density;
      for (r=2;r<=rgs.Labels();r++)
	 if (regions[r].density<temp)
	    temp=regions[r].density;
      for (r=1;r<=rgs.Labels();r++)
	 if (regions[r].density==temp) {
	    regions[r].acceptee=true;
	    ++nreg;
	 } else
	    regions[r].acceptee=false;
   } else if (relation >= 3) {
      temp=regions[1].density;
      for (r=2;r<=rgs.Labels();r++)
	 if (regions[r].density>temp)
	    temp=regions[r].density;
      for (r=1;r<=rgs.Labels();r++)
	 if (regions[r].density==temp) {
	    regions[r].acceptee=true;
	    ++nreg;
	 } else
	    regions[r].acceptee=false;
   } else if (relation == 2) {
      for (r=1;r<=rgs.Labels();r++) {
	 if (regions[r].density>=threshold) {
	    regions[r].acceptee=true;
	    ++nreg;
	 }else
	    regions[r].acceptee=false;
      }
   } else if (relation == 1) {
      for (r=1;r<=rgs.Labels();r++) {
	 if (regions[r].density>threshold) {
	    regions[r].acceptee=true;
	    ++nreg;
	 }else
	    regions[r].acceptee=false;
      }
   } else if (relation == 0) {
      for (r=1;r<=rgs.Labels();r++) {
	 if (regions[r].density == threshold) {
	    regions[r].acceptee=true;
	    ++nreg;
	 }else
	    regions[r].acceptee=false;
      }
   } else if (relation == -1) {
      for (r=1;r<=rgs.Labels();r++) {
	 if (regions[r].density < threshold) {
	    regions[r].acceptee=true;
	    ++nreg;
	 }else
	    regions[r].acceptee=false;
      }
   } else /* if (relation == -2) */ {
      for (r=1;r<=rgs.Labels();r++) {
	 if (regions[r].density <= threshold) {
	    regions[r].acceptee=true;
	    ++nreg;
	 }else
	    regions[r].acceptee=false;
      }
   }
  
   // Construction de la carte de sortie.
   for (y=0;y<rgs.Height(); y++) {
      for (x=0;x<rgs.Width(); x++) {
	 if (regions[rgs(y,x)].acceptee) {
	    rgd(y,x) = rgs(y,x);
	 } else {
	    rgd(y,x) = 0;
	 }
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
#define	USAGE	"usage: %s relation threshold [-m mask] [rg_in|-] [im_in|-] [rg_out|-]"
#define	PARC	2
#define	FINC	2
#define	FOUTC	1
#define	MASK	2

int main( int argc, char *argv[] ) {
   Errc result;              // The result code of the execution.
   Pobject* mask;            // The region mask.
   Pobject* objin[FINC+1];   // The input objects.
   Pobject* objs[FINC+1];    // The source objects masked by the stencil.
   Pobject* objout[FOUTC+1]; // The output object.
   Pobject* objd[FOUTC+1];   // The result object of the execution.
   char* parv[PARC+1];       // The input parameters.
   
   ReadArgs(argc,argv,PARC,FINC,FOUTC,&mask,objin,objs,objout,objd,parv,USAGE,MASK);
   
   if (objs[0]->Type()==Po_Reg2d && objs[1]->Type()==Po_Img2duc) {
      Reg2d* const rgs=(Reg2d*)objs[0];
      Img2duc* const ims=(Img2duc*)objs[1];
      objd[0]=new Reg2d(rgs->Size());
      Reg2d* const rgd=(Reg2d*)objd[0];
      
      result=PDensitySelection(*rgs,*ims, *rgd, atoi(parv[0]), (float)atof(parv[1]));
   } else {
      PrintErrorFormat(objin,FINC);
      result=FAILURE;
   }
   
   if (result)
      WriteArgs(argc,argv,PARC,FINC,FOUTC,&mask,objin,objs,objout,objd,MASK);
   
   Exit(result);
   return(0);
}

#endif
