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
 * @author Régis Clouard - 2003-05-16
 * @author Régis Clouard - 2004-06-29 (>=, <=)
 * @author Regis Clouard - 2006-06-02 (relation =3 and -3).
 * @author Regis Clouard - 2007-04-02 (fix bug on the calculation of the sphericity).
 */

#include <math.h>
#include <pandore.h>
using namespace pandore;

/**
 * @file psphericityselection.cpp
 * Select regions from their sphericity degree.
 * sphericity = inscribing circle radius / circumscribing circle radius.
 */
struct attrs_sphericite2d {
   double gx, gy; // Center of mass.
   float circumscribing;
   Ulong surface;
   float inscribing;
   float sphericite;
   bool acceptee;
};

struct attrs_sphericite3d {
   double gx, gy, gz; // Center of mass.
   float circumscribing;
   Ulong volume;
   float inscribing;
   float sphericite;
   bool acceptee;
};

/**
 * 1- find center of mass.
 * 2 -find max radius of inscribing radius (farest from the center of mass)
 *    and min radius of circumscribing radius (nearest from the center of mass).
 */
Errc PSphericitySelection( const Reg3d &rgs, struct attrs_sphericite3d *regions ) {
   if (rgs.Labels() < 1) {
      std::cerr << "Error psphericityselection: Bad region map: no region inside." << std::endl;
      return FAILURE;
   }

   // Calculate 1th moment.
   for (int z = 0; z < rgs.Depth(); z++) {
      for (int y = 0; y < rgs.Height(); y++) {
         for (int x = 0; x < rgs.Width(); x++) {
	    Ulong r;
            if ((r = rgs(z, y, x)) > 0) {
               regions[r].gx += (double)x;
               regions[r].gy += (double)y;
               regions[r].gz += (double)z;
               regions[r].volume ++;
            }
         }
      }
   }
   // Calculate center of mass.
   for (Ulong r = 1; r <= rgs.Labels(); r++) {
      if (regions[r].volume > 0) {
         regions[r].gx = regions[r].gx / (double)regions[r].volume;
         regions[r].gy = regions[r].gy / (double)regions[r].volume;
         regions[r].gz = regions[r].gz / (double)regions[r].volume;
         regions[r].inscribing = MAXFLOAT;
         regions[r].circumscribing = 0;
      }
   }

   // Find max inscribing pixel (farest from the center of mass).
   // Find min circumscribing pixel (nearest from the  center of mass).
   for (int z = 0; z < rgs.Depth(); z++) {
      for (int y = 0; y < rgs.Height(); y++) {
         for (int x = 0; x < rgs.Width(); x++) {
            const Point3d p = Point3d(z, y, x);
	    Ulong r;
            if ((r = rgs[p]) > 0) {
               bool bord = false;
               for (int i = 0; i < 26; i++) {
                  const Point3d v = p + v26[i];
                  if (rgs.Hold(v) && rgs[v] != r) {
                     bord = true;
                     break;
                  }
               }

               if (bord) {
                  float dist = (float)(SQR(regions[r].gx - x) + SQR(regions[r].gy - y) + SQR(regions[r].gz - z));
                  if (dist < regions[r].inscribing) {
                     regions[r].inscribing = dist;
		  }
                  if (dist > regions[r].circumscribing) {
                     regions[r].circumscribing = dist;
		  }
               }
            }
         }
      }
   }

   for (Ulong r = 1; r <= rgs.Labels(); r++) {
      if (regions[r].circumscribing > 0) {
         regions[r].sphericite = (float)(sqrt(regions[r].inscribing) / sqrt(regions[r].circumscribing));
      }
   }

   return SUCCESS;
}

/*
 * Selectionne les regions ayant une sphericite en nombre de pixels
 * inferieure si relation < 0, egal si relation = <0 , superieur sinon
 * a la valeur fournie.
 */
Errc PSphericitySelection( const Reg3d &rgs, Reg3d &rgd, int relation, float threshold ) {
   struct attrs_sphericite3d *regions;
   Ulong nreg = 0, r;

   // Initialization  
   regions = (struct attrs_sphericite3d*)calloc(rgs.Labels()+1,sizeof(struct attrs_sphericite3d));

   PSphericitySelection(rgs, regions); // Calcule la valeur de sphericite.

   float temp;
   // Selection des regions
   if (relation <= -3) {
      temp = regions[1].sphericite;
      for (r = 2; r <= rgs.Labels(); r++) {
         if (regions[r].sphericite < temp) {
            temp = regions[r].sphericite;
	 }
      }
      for (r = 1; r <= rgs.Labels(); r++) {
         if (regions[r].sphericite == temp) {
            regions[r].acceptee = true;
            ++nreg;
         } else {
            regions[r].acceptee = false;
	 }
      }
   } else if (relation >= 3) {
      temp = regions[1].sphericite;
      for (r = 2; r <= rgs.Labels(); r++) {
         if (regions[r].sphericite > temp) {
            temp = regions[r].sphericite;
	 }
      }
      for (r = 1; r <= rgs.Labels();r++) {
         if (regions[r].sphericite == temp) {
            regions[r].acceptee = true;
            ++nreg;
         } else {
            regions[r].acceptee = false;
	 }
      }
   } else if (relation == 2) {
      for (r = 1; r <= rgs.Labels(); r++) {
         if (regions[r].sphericite >= threshold) {
            regions[r].acceptee = true;
            ++nreg;
         } else {
            regions[r].acceptee = false;
	 }
      }
   } else if (relation == 1) {
      for (r = 1; r <= rgs.Labels(); r++) {
         if (regions[r].sphericite > threshold) {
            regions[r].acceptee = true;
            ++nreg;
         } else {
            regions[r].acceptee = false;
	 }
      }
   } else if (relation == 0) {
      for (r = 1; r <= rgs.Labels(); r++) {
         if (regions[r].sphericite == threshold) {
            regions[r].acceptee = true;
            ++nreg;
         } else {
            regions[r].acceptee = false;
	 }
      }
   } else if (relation == -1) {
      for (r = 1; r <= rgs.Labels(); r++) {
         if (regions[r].sphericite < threshold) {
            regions[r].acceptee = true;
            ++nreg;
         } else {
            regions[r].acceptee = false;
	 }
      }
   } else /* if (relation == -2) */ {
      for (r = 1; r <= rgs.Labels(); r++) {
         if (regions[r].sphericite <= threshold) {
            regions[r].acceptee = true;
            ++nreg;
         } else {
            regions[r].acceptee = false;
	 }
      }
   }
  
   // Construction de la carte de sortie.
   for (int z = 0; z < rgs.Depth(); z++) {
      for (int y = 0; y < rgs.Height(); y++) {
         for (int x = 0; x < rgs.Width(); x++) {
            if (regions[rgs(z, y, x)].acceptee) {
               rgd(z, y, x) = rgs(z, y, x);
            } else {
               rgd(z, y, x) = 0;
	    }
         }
      }
   }

   rgd.Labels(rgs.Labels());
   free(regions);
   return nreg;
}

/**
 * 1- find center of mass.
 * 2 -find max radius of inscribing radius (farest from the center of mass)
 *    and min radius of circumscribing radius (nearest from the center of mass).
 */
Errc PSphericitySelection( const Reg2d &rgs, struct attrs_sphericite2d *regions ) {
   if (rgs.Labels() < 1) {
      std::cerr << "Error psphericityselection: Bad region map: no region inside." << std::endl;
      return FAILURE;
   }

   // Calculate 1th moment.
   for (int y = 0; y < rgs.Height(); y++)
      for (int x = 0; x < rgs.Width(); x++) {
	 Ulong r = rgs(y, x);
	 if (r > 0) {
	    regions[r].gx += (double)x;
	    regions[r].gy += (double)y;
	    regions[r].surface ++;
	 }
      }
   // Calculate center of mass.
   for (Ulong r = 1; r <= rgs.Labels(); r++) {
      if (regions[r].surface > 0) {
	 regions[r].gx = regions[r].gx / (double)regions[r].surface;
	 regions[r].gy = regions[r].gy / (double)regions[r].surface;
	 regions[r].inscribing = MAXFLOAT;
	 regions[r].circumscribing = 0;
      }
   }

   // Find max inscribing pixel (farest from the center of mass).
   // Find min circumscribing pixel (nearest from the  center of mass).
   Point2d p;
   for (p.y = 0; p.y < rgs.Height(); p.y++) {
      for (p.x = 0; p.x < rgs.Width(); p.x++) {
	 Ulong r;
	 if ((r = rgs[p]) > 0) {
	    bool border = false;
	    for (int v = 0; v < 8; v++) {
	       Point2d p1(p.y + v8y[v], p.x + v8x[v]);
	       if (rgs.Hold(p1) && rgs[p1] != r) {
		  border = true;
		  break;
	       }
	    }
	    
	    if (border) {
	       float dist = (float)(SQR(regions[r].gx - p.x) + SQR(regions[r].gy - p.y));
	       if (dist < regions[r].inscribing) {
		  regions[r].inscribing = dist;
	       }
	       if (dist > regions[r].circumscribing) {
		  regions[r].circumscribing = dist;
	       }
	    }
	 }
      }
   }

   for (Ulong r = 1; r <= rgs.Labels(); r++) {
      if (regions[r].circumscribing > 0) {
	 regions[r].sphericite = (float)(sqrt(regions[r].inscribing) / sqrt(regions[r].circumscribing));
      }
   }

   return SUCCESS;
}

/*
 * Selectionne les regions ayant une sphericite en nombre de pixels
 * inferieure si relation < 0, egal si relation = 0 , superieur sinon
 * a la valeur fournie.
 */
Errc PSphericitySelection( const Reg2d &rgs, Reg2d &rgd, int relation, float threshold ) {
   struct attrs_sphericite2d *regions;
   Ulong nreg = 0;

   // Initialization  
   regions = (struct attrs_sphericite2d*)calloc(rgs.Labels()+1,sizeof(struct attrs_sphericite2d));

   PSphericitySelection(rgs,regions); // Calcule la valeur de sphericite.

   float temp;
   // Selection des regions
   if (relation <= -3) {
      temp = regions[1].sphericite;
      for (Ulong r = 2; r <= rgs.Labels(); r++) {
	 if (regions[r].sphericite < temp) {
	    temp = regions[r].sphericite;
	 }
      }
      for (Ulong r = 1; r <= rgs.Labels(); r++) {
	 if (regions[r].sphericite == temp) {
	    regions[r].acceptee = true;
	    ++nreg;
	 } else {
	    regions[r].acceptee = false;
	 }
      }
   } else if (relation >= 3) {
      temp = regions[1].sphericite;
      for (Ulong r = 2; r <= rgs.Labels(); r++) {
	 if (regions[r].sphericite>temp) {
	    temp = regions[r].sphericite;
	 }
      }
      for (Ulong r = 1; r <= rgs.Labels(); r++) {
	 if (regions[r].sphericite == temp) {
	    regions[r].acceptee = true;
	    ++nreg;
	 } else {
	    regions[r].acceptee = false;
	 }
      }
   } else if (relation == 2) {
      for (Ulong r = 1; r <= rgs.Labels(); r++) {
	 if (regions[r].sphericite >= threshold) {
	    regions[r].acceptee = true;
	    ++nreg;
	 } else {
	    regions[r].acceptee = false;
	 }
      }
   } else if (relation == 1) {
      for (Ulong r = 1; r <= rgs.Labels(); r++) {
	 if (regions[r].sphericite > threshold) {
	    regions[r].acceptee = true;
	    ++nreg;
	 } else {
	    regions[r].acceptee = false;
	 }
      }
   } else if (relation == 0) {
      for (Ulong r = 1; r <= rgs.Labels(); r++) {
	 if (regions[r].sphericite == threshold) {
	    regions[r].acceptee = true;
	    ++nreg;
	 } else {
	    regions[r].acceptee = false;
	 }
      }
   } else if (relation == -1) {
      for (Ulong r = 1; r <= rgs.Labels(); r++) {
	 if (regions[r].sphericite < threshold) {
	    regions[r].acceptee = true;
	    ++nreg;
	 } else {
	    regions[r].acceptee = false;
	 }
      }
   } else /* if (relation == -2) */ {
      for (Ulong r = 1; r <= rgs.Labels(); r++) {
	 if (regions[r].sphericite <= threshold) {
	    regions[r].acceptee = true;
	    ++nreg;
	 } else {
	    regions[r].acceptee = false;
	 }
      }
   }
  
   // Construction de la carte de sortie.
   for (int y = 0; y < rgs.Height(); y++) {
      for (int x = 0; x < rgs.Width(); x++) {
	 if (regions[rgs(y, x)].acceptee) {
	    rgd(y, x) = rgs(y, x);
	 } else {
	    rgd(y, x) = 0;
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
#define	USAGE	"usage: %s relation threshold [-m mask] [rg_in|-] [rg_out|-]"
#define	PARC	2
#define	FINC	1
#define	FOUTC	1
#define	MASK	2

int main( int argc, char *argv[] ) {
   Errc result;                // The result code of the execution.
   Pobject* mask;              // The region mask.
   Pobject* objin[FINC + 1];   // The input objects.
   Pobject* objs[FINC + 1];    // The source objects masked by the stencil.
   Pobject* objout[FOUTC + 1]; // The output object.
   Pobject* objd[FOUTC + 1];   // The result object of the execution.
   char* parv[PARC + 1];       // The input parameters.
   
   ReadArgs(argc, argv, PARC, FINC, FOUTC, &mask, objin, objs, objout, objd, parv, USAGE, MASK);
   switch(objs[0]->Type()) {
   case Po_Reg2d : {
      Reg2d* const rgs = (Reg2d*)objs[0];
      objd[0] = new Reg2d(rgs->Size());
      Reg2d* const rgd = (Reg2d*)objd[0];
      
      result = PSphericitySelection(*rgs, *rgd, atoi(parv[0]), (float)atof(parv[1]));
   }break;
   case Po_Reg3d : {
      Reg3d* const rgs = (Reg3d*)objs[0];
      objd[0] = new Reg3d(rgs->Size());
      Reg3d* const rgd = (Reg3d*)objd[0];
      
      result = PSphericitySelection(*rgs, *rgd, atoi(parv[0]), (float)atof(parv[1]));
   }break;
   default:
      PrintErrorFormat(objin,FINC);
      result = FAILURE;
   }
   
   if (result) {
      WriteArgs(argc, argv, PARC, FINC, FOUTC, &mask, objin, objs, objout, objd, MASK);
   }
   Exit(result);
   return(0);
}

#endif
