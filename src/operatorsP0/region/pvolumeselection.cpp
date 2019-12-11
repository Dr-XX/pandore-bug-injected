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
 * @author Regis Clouard - 2003-07-22
 * @author Regis Clouard - 2004-06-29 (>=, <=)
 * @author Regis Clouard - 2006-06-02 (relation =3 and -3).
 */

#include <map>
#include <pandore.h>
using namespace pandore;

/**
 * @file pvolumeselection.cpp
 *
 * Selects regions from volume factor.
 * Remarque : volume is computed in voxels.
 */

struct attrs_volume{
  float	volume;
  bool	acceptee;
};

/*
 * Fonction qui calcule le volume de chacune des regions presentes.
 * Rem : le volume est calculee en pixels.
 */
void PVolumeSelection( const Reg3d &rgs, struct attrs_volume *regions ) {
   Reg3d::ValueType *prgs = rgs.Vector();
   Reg3d::ValueType *end = rgs.Vector() + rgs.VectorSize();
   
   for (; prgs < end; prgs++) {
      if (*prgs > 0) {
	 regions[*prgs].volume++;
      }
   }
}

/*
 * Selectionne les regions ayant une valeur de caracteristique
 * inferieure si relation < 0, egale si relation =0 , superieure sinon
 * a la valeur fournie.
 */
Errc PVolumeSelection( const Reg3d &rgs, Reg3d &rgd, int relation, Ulong threshold ) {
   struct attrs_volume *regions;
   Ulong nreg = 0;
   float temp;
   
   regions = (struct attrs_volume*)calloc(rgs.Labels()+1, sizeof(struct attrs_volume));
   
   PVolumeSelection(rgs, regions);
   regions[0].acceptee = false;
   // Selection des regions

   if (relation <= -3) {
      temp = regions[1].volume;
      for (Ulong r = 2; r <= rgs.Labels(); r++) {
	 if (regions[r].volume < temp  && regions[r].volume > 0) {
	    temp = regions[r].volume;
	 }
      }
      for (Ulong r = 1; r <= rgs.Labels(); r++) {
	 if (regions[r].volume == temp) {
	    regions[r].acceptee = true;
	    ++nreg;
	 } else {
	    regions[r].acceptee = false;
	 }
      }
   } else if (relation >= 3) {
      temp = regions[1].volume;
      for (Ulong r = 2; r <= rgs.Labels(); r++) {
	 if (regions[r].volume > temp) {
	    temp = regions[r].volume;
	 }
      }
      for (Ulong r = 1; r <= rgs.Labels(); r++) {
	 if (regions[r].volume == temp) {
	    regions[r].acceptee = true;
	    ++nreg;
	 } else {
	    regions[r].acceptee = false;
	 }
      }
   } else if (relation == 2) {
      for (Ulong r = 1; r <= rgs.Labels(); r++) {
	 if ((Ulong)regions[r].volume >= threshold) {
	    regions[r].acceptee = true;
	    ++nreg;
	 } else {
	    regions[r].acceptee = false;
	 }
      }
   } else if (relation == 1) {
      for (Ulong r = 1; r <= rgs.Labels(); r++) {
	 if ((Ulong)regions[r].volume > threshold) {
	    regions[r].acceptee = true;
	    ++nreg;
	 } else {
	    regions[r].acceptee= false;
	 }
      }
   } else if (relation == 0) {
      for (Ulong r = 1; r <= rgs.Labels(); r++) {
	 if ((Ulong)regions[r].volume == threshold) {
	    regions[r].acceptee = true;
	    ++nreg;
	 } else {
	    regions[r].acceptee= false;
	 }
      }
   } else if (relation == -1) {
      for (Ulong r = 1; r <= rgs.Labels(); r++) {
	 if ((Ulong)regions[r].volume < threshold && regions[r].volume > 0) {
	    regions[r].acceptee = true;
	    ++nreg;
	 } else {
	    regions[r].acceptee= false;
	 }
      }
   } else /* if (relation == -2) */ {
      for (Ulong r = 1; r <= rgs.Labels(); r++) {
	 if ((Ulong)regions[r].volume <= threshold && regions[r].volume > 0) {
	    regions[r].acceptee = true;
	    ++nreg;
	 } else {
	    regions[r].acceptee= false;
	 }
      }
   }


   // Construction de la carte de sortie.
   Reg3d::ValueType *prgs = rgs.Vector();
   Reg3d::ValueType *prgd = rgd.Vector();
   Reg3d::ValueType *end = rgs.Vector()+rgs.VectorSize();
   
   for (; prgs < end; prgs++, prgd++) {
      if (regions[*prgs].acceptee) {
	 *prgd = *prgs;
      } else {
	 *prgd = 0;
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
#define	MASK	1

int main( int argc, char *argv[] ) {
   Errc result;             // The result code of the execution.
   Pobject* mask;            // The region mask.
   Pobject* objin[FINC+1];   // The input objects.
   Pobject* objs[FINC+1];    // The source objects masked by the mask.
   Pobject* objout[FOUTC+1]; // The output object.
   Pobject* objd[FOUTC+1];   // The result object of the execution.
   char* parv[PARC+1];       // The input parameters.
   
   ReadArgs(argc, argv, PARC, FINC, FOUTC, &mask, objin, objs, objout, objd, parv, USAGE, MASK);
   
   switch(objs[0]->Type()) {
   case Po_Reg3d :{
      Reg3d* const rgs = (Reg3d*)objs[0];
      objd[0] = new Reg3d(rgs->Size());
      Reg3d* const rgd = (Reg3d*)objd[0];
      
      result = PVolumeSelection(*rgs, *rgd, atoi(parv[0]), (Ulong)atol(parv[1]));
   }break;
   default:
      PrintErrorFormat(objin, FINC);
      result=FAILURE;
   }
   
   if (result) {
      WriteArgs(argc, argv, PARC, FINC, FOUTC, &mask, objin, objs, objout, objd, MASK);
   }
   Exit(result);
   return 0;
}

#endif
