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
 * @author Régis Clouard - 2003-05-13 (change formula)
 * @author Régis Clouard - 2004-06-29 (>=, <=)
 * @author Regis Clouard - 2006-06-02 (relation =3 and -3).
 */

#include <pandore.h>
using namespace pandore;

/**
 * @file pperimeterselection.cpp
 * Selects regions from perimeter length.
 * Remarque : perimeter is calculated in pixel unit.
 */

struct attrs_perimetre {
   float perimetre;
   bool acceptee;
};

/*
 * Computes the perimeter of each region.
 */
Errc PPerimeterSelection( const Reg2d &rgs, struct attrs_perimetre *regions ) {
   register int y,x,v;
   Reg2d::ValueType r;
   bool bord;

   for (y=0;y<rgs.Height();y++) {
      for (x=0;x<rgs.Width();x++) {
	 if ((r=rgs[y][x])>0) {
	    bord=false;
	    for (v=0;v<4;v++) {
	       if ( (!(rgs.Hold(y+v4[v].y,x+v4[v].x))) || (rgs[y+v4[v].y][x+v4[v].x]!=r)) {
		  regions[r].perimetre++;
		  bord=true;
		  break;
	       }
	    }
	    if (!bord) {
	       for (v=0;v<4;v++) {
		  if (rgs[y+v8[v*2+1].y][x+v8[v*2+1].x]!=r) {
		     // Pour les coins en creux on ajoute un petit quart de perimetre.
		     // obtenu par l'experience...
		     regions[r].perimetre+=0.25;
		  }
	       }
	    }
	 }
      }
   }
   return SUCCESS;
}

/*
 * Selectionne les regions ayant une valeur de caracteristique
 * inferieure si relation < 0, egale si relation =0 , superieure sinon
 * a la valeur fournie.
 */
Errc PPerimeterSelection( const Reg2d &rgs, Reg2d &rgd, int relation, Ulong threshold ) {
   register int y,x;
   struct attrs_perimetre *regions;
   Ulong nreg=0,r;
   
   regions = (struct attrs_perimetre*)calloc(rgs.Labels()+1,sizeof(struct attrs_perimetre));
   
   PPerimeterSelection(rgs,regions);
   regions[0].acceptee=false;

   float temp;
   // Selection des regions
   if (relation <= -3) {
      temp=regions[1].perimetre;
      for (r=2;r<=rgs.Labels();r++)
	 if (regions[r].perimetre<temp)
	    temp=regions[r].perimetre;
      for (r=1;r<=rgs.Labels();r++)
	 if (regions[r].perimetre==temp) {
	    regions[r].acceptee=true;
	    ++nreg;
	 } else
	    regions[r].acceptee=false;
   } else if (relation >= 3) {
      temp=regions[1].perimetre;
      for (r=2;r<=rgs.Labels();r++)
	 if (regions[r].perimetre>temp)
	    temp=regions[r].perimetre;
      for (r=1;r<=rgs.Labels();r++)
	 if (regions[r].perimetre==temp) {
	    regions[r].acceptee=true;
	    ++nreg;
	 } else
	    regions[r].acceptee=false;
   } else if (relation == 2) {
      for (r=1;r<=rgs.Labels();r++) {
	 if ((Ulong)regions[r].perimetre>=threshold) {
	    regions[r].acceptee=true;
	    ++nreg;
	 }else
	    regions[r].acceptee=false;
      }
   } else if (relation == 1) {
      for (r=1;r<=rgs.Labels();r++) {
	 if ((Ulong)regions[r].perimetre>threshold) {
	    regions[r].acceptee=true;
	    ++nreg;
	 }else
	    regions[r].acceptee=false;
      }
   } else if (relation == 0) {
      for (r=1;r<=rgs.Labels();r++) {
	 if ((Ulong)regions[r].perimetre == threshold) {
	    regions[r].acceptee=true;
	    ++nreg;
	 }else
	    regions[r].acceptee=false;
      }
   } else if (relation == -1) {
      for (r=1;r<=rgs.Labels();r++) {
	 if ((Ulong)regions[r].perimetre < threshold) {
	    regions[r].acceptee=true;
	    ++nreg;
	 }else
	    regions[r].acceptee=false;
      }
   } else /* if (relation == -2) */ {
      for (r=1;r<=rgs.Labels();r++) {
	 if ((Ulong)regions[r].perimetre <= threshold) {
	    regions[r].acceptee=true;
	    ++nreg;
	 }else
	    regions[r].acceptee=false;
      }
   }
   
   // Construction de la carte de sortie.
   for (y=0;y<rgs.Height(); y++)
      for (x=0;x<rgs.Width(); x++) {
	 if (regions[rgs[y][x]].acceptee)
	    rgd[y][x] = rgs[y][x];
	 else
	    rgd[y][x] = 0;
      }

   rgd.Labels(rgs.Labels());
   free(regions);
   return nreg;
}

#ifdef MAIN
/*
 * Modify only the following constants, and the function call.
 */
#define USAGE	"usage: %s relation threshold [-m mask] [rg_in|-] [rg_out|-]"
#define PARC	2
#define FINC	1
#define FOUTC	1
#define MASK	2

int main( int argc, char *argv[] ) {
   Errc result;              // The result code of the execution.
   Pobject* mask;            // The region mask.
   Pobject* objin[FINC+1];   // The input objects.
   Pobject* objs[FINC+1];    // The source objects masked by the mask.
   Pobject* objout[FOUTC+1]; // The output object.
   Pobject* objd[FOUTC+1];   // The result object of the execution.
   char* parv[PARC+1];       // The input parameters.
   
   ReadArgs(argc,argv,PARC,FINC,FOUTC,&mask,objin,objs,objout,objd,parv,USAGE,MASK);
   
   switch(objs[0]->Type()) {
   case Po_Reg2d :{
      Reg2d* const rgs=(Reg2d*)objs[0];
      objd[0]=new Reg2d(rgs->Size());
      Reg2d* const rgd=(Reg2d*)objd[0];
      
      result=PPerimeterSelection(*rgs,*rgd,atoi(parv[0]),(Ulong)atol(parv[1]));
   }break;
   default:
      PrintErrorFormat(objin,FINC);
      result=FAILURE;
   }
   
   if (result)
      WriteArgs(argc,argv,PARC,FINC,FOUTC,&mask,objin,objs,objout,objd,MASK);
   
   Exit(result);
   return 0;
}

#endif
