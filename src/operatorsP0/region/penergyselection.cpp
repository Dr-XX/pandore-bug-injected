/* -*- c-basic-offset: 3; mode: c++ -*-
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
 * @author Régis Clouard - 1997-21-07
 * @author Régis Clouard - 2004-06-29 (>= <=)
 * @author Regis Clouard - 2006-06-02 (relation =3 and -3).
 */

#include <pandore.h>
using namespace pandore;

/**
 * @file penergyselection.cpp
 * Selects region from energy value.
 * energy = sum (pixel*pixel) / N
 */

struct attrs_energie {
   Ulong total;
   float sigma;
   float energie;
   bool acceptee;
};


/*
 * Fonction qui calcule les valeurs d'energie interieure
 * de chacune des regions.
 */
Errc PEnergySelection( const Reg2d &rgs, const Img2duc &ims, struct attrs_energie *regions ) {
   Point2d p;
   Ulong r;
   
   // Calcul des valeurs pour chacunes des regions.
   for (p.y = 0; p.y < rgs.Height(); p.y++)
   for (p.x = 0; p.x < rgs.Width(); p.x++)
   {
      if ((r=rgs[p])>0)
	 regions[r].sigma+=(float)ims[p]*ims[p];
      regions[r].total++;
   }
   
   for (r=1;r<=rgs.Labels();r++)
      if (regions[r].total)
	 regions[r].energie=regions[r].sigma/regions[r].total;
   
   return SUCCESS;
}

/*
 * Selectionne les regions ayant une energie de niveaux de gris
 * <= si relation=-2, < si relation=-1, > si relation=1, >= si relation=2
 * a la valeur fournie.
 */
Errc PEnergySelection( const Reg2d &rgs, const Img2duc &ims, Reg2d &rgd, int relation, float threshold ) {
   struct attrs_energie *regions;
   Ulong nreg=0,r;
  
   regions = (struct attrs_energie*)calloc(rgs.Labels()+1,sizeof(struct attrs_energie));
  
   PEnergySelection(rgs,ims,regions);
 
   float temp;
   // Selection des regions
   if (relation <= -3) {
      temp=regions[1].energie;
      for (r=2;r<=rgs.Labels();r++) {
	 if (regions[r].energie<temp) {
	    temp=regions[r].energie;
	 }
      }
      for (r=1;r<=rgs.Labels();r++) {
	 if (regions[r].energie==temp) {
	    regions[r].acceptee=true;
	    ++nreg;
	 } else {
	    regions[r].acceptee=false;
	 }
      }
   } else if (relation >= 3) {
      temp=regions[1].energie;
      for (r=2;r<=rgs.Labels();r++)
	 if (regions[r].energie>temp)
	    temp=regions[r].energie;
      for (r=1;r<=rgs.Labels();r++)
	 if (regions[r].energie==temp) {
	    regions[r].acceptee=true;
	    ++nreg;
	 } else {
	    regions[r].acceptee=false;
	 }
   } else if (relation == 2) {
      for (r=1;r<=rgs.Labels();r++) {
	 if (regions[r].energie>=threshold) {
	    regions[r].acceptee=true;
	    ++nreg;
	 } else {
	    regions[r].acceptee=false;
	 }
      }
   } else if (relation == 1) {
      for (r=1;r<=rgs.Labels();r++) {
	 if (regions[r].energie>threshold) {
	    regions[r].acceptee=true;
	    ++nreg;
	 } else {
	    regions[r].acceptee=false;
	 }
      }
   } else if (relation == 0) {
      for (r=1;r<=rgs.Labels();r++) {
	 if (regions[r].energie == threshold) {
	    regions[r].acceptee=true;
	    ++nreg;
	 } else {
	    regions[r].acceptee=false;
	 }
      }
   } else if (relation == -1) {
      for (r=1;r<=rgs.Labels();r++) {
	 if (regions[r].energie < threshold) {
	    regions[r].acceptee=true;
	    ++nreg;
	 } else {
	    regions[r].acceptee=false;
	 }
      }
   } else /* if (relation == -2) */ {
      for (r=1;r<=rgs.Labels();r++) {
	 if (regions[r].energie <= threshold) {
	    regions[r].acceptee=true;
	    ++nreg;
	 } else {
	    regions[r].acceptee=false;
	 }
      }
   }
  
   // Construction de la carte de sortie.
   Point2d p;
   for (p.y = 0; p.y < rgs.Height(); p.y++)
   for (p.x = 0; p.x < rgs.Width(); p.x++)
   {
      if (regions[rgs[p]].acceptee) {
	 rgd[p] = rgs[p];
      } else {
	 rgd[p] = 0;
      }
   }
   
   rgd.Labels(rgs.Labels());
   free(regions);
   return nreg;
}

/*
 * Fonction qui calcule les valeurs d'energie interieure
 * de chacune des regions.
 */
Errc PEnergySelection( const Reg2d &rgs, const Img2dsl &ims, struct attrs_energie *regions ) {
   Point2d p;
   Ulong r;
   
   // Calcul des valeurs pour chacunes des regions.
   for (p.y = 0; p.y < rgs.Height(); p.y++)
   for (p.x = 0; p.x < rgs.Width(); p.x++)
   {
      if ((r=rgs[p])>0)
	 regions[r].sigma+=(float)ims[p]*ims[p];
      regions[r].total++;
   }
   
   for (r=1;r<=rgs.Labels();r++)
      if (regions[r].total)
	 regions[r].energie=regions[r].sigma/regions[r].total;
   
   return SUCCESS;
}

/*
 * Selectionne les regions ayant une energie de niveaux de gris
 * <= si relation=-2, < si relation=-1, > si relation=1, >= si relation=2
 * a la valeur fournie.
 */
Errc PEnergySelection( const Reg2d &rgs, const Img2dsl &ims, Reg2d &rgd, int relation, float threshold ) {
   struct attrs_energie *regions;
   Ulong nreg=0,r;
  
   regions = (struct attrs_energie*)calloc(rgs.Labels()+1,sizeof(struct attrs_energie));
  
   PEnergySelection(rgs,ims,regions);
 
   float temp;
   // Selection des regions
   if (relation <= -3) {
      temp=regions[1].energie;
      for (r=2;r<=rgs.Labels();r++) {
	 if (regions[r].energie<temp) {
	    temp=regions[r].energie;
	 }
      }
      for (r=1;r<=rgs.Labels();r++) {
	 if (regions[r].energie==temp) {
	    regions[r].acceptee=true;
	    ++nreg;
	 } else {
	    regions[r].acceptee=false;
	 }
      }
   } else if (relation >= 3) {
      temp=regions[1].energie;
      for (r=2;r<=rgs.Labels();r++)
	 if (regions[r].energie>temp)
	    temp=regions[r].energie;
      for (r=1;r<=rgs.Labels();r++)
	 if (regions[r].energie==temp) {
	    regions[r].acceptee=true;
	    ++nreg;
	 } else {
	    regions[r].acceptee=false;
	 }
   } else if (relation == 2) {
      for (r=1;r<=rgs.Labels();r++) {
	 if (regions[r].energie>=threshold) {
	    regions[r].acceptee=true;
	    ++nreg;
	 } else {
	    regions[r].acceptee=false;
	 }
      }
   } else if (relation == 1) {
      for (r=1;r<=rgs.Labels();r++) {
	 if (regions[r].energie>threshold) {
	    regions[r].acceptee=true;
	    ++nreg;
	 } else {
	    regions[r].acceptee=false;
	 }
      }
   } else if (relation == 0) {
      for (r=1;r<=rgs.Labels();r++) {
	 if (regions[r].energie == threshold) {
	    regions[r].acceptee=true;
	    ++nreg;
	 } else {
	    regions[r].acceptee=false;
	 }
      }
   } else if (relation == -1) {
      for (r=1;r<=rgs.Labels();r++) {
	 if (regions[r].energie < threshold) {
	    regions[r].acceptee=true;
	    ++nreg;
	 } else {
	    regions[r].acceptee=false;
	 }
      }
   } else /* if (relation == -2) */ {
      for (r=1;r<=rgs.Labels();r++) {
	 if (regions[r].energie <= threshold) {
	    regions[r].acceptee=true;
	    ++nreg;
	 } else {
	    regions[r].acceptee=false;
	 }
      }
   }
  
   // Construction de la carte de sortie.
   Point2d p;
   for (p.y = 0; p.y < rgs.Height(); p.y++)
   for (p.x = 0; p.x < rgs.Width(); p.x++)
   {
      if (regions[rgs[p]].acceptee) {
	 rgd[p] = rgs[p];
      } else {
	 rgd[p] = 0;
      }
   }
   
   rgd.Labels(rgs.Labels());
   free(regions);
   return nreg;
}

/*
 * Fonction qui calcule les valeurs d'energie interieure
 * de chacune des regions.
 */
Errc PEnergySelection( const Reg2d &rgs, const Img2dsf &ims, struct attrs_energie *regions ) {
   Point2d p;
   Ulong r;
   
   // Calcul des valeurs pour chacunes des regions.
   for (p.y = 0; p.y < rgs.Height(); p.y++)
   for (p.x = 0; p.x < rgs.Width(); p.x++)
   {
      if ((r=rgs[p])>0)
	 regions[r].sigma+=(float)ims[p]*ims[p];
      regions[r].total++;
   }
   
   for (r=1;r<=rgs.Labels();r++)
      if (regions[r].total)
	 regions[r].energie=regions[r].sigma/regions[r].total;
   
   return SUCCESS;
}

/*
 * Selectionne les regions ayant une energie de niveaux de gris
 * <= si relation=-2, < si relation=-1, > si relation=1, >= si relation=2
 * a la valeur fournie.
 */
Errc PEnergySelection( const Reg2d &rgs, const Img2dsf &ims, Reg2d &rgd, int relation, float threshold ) {
   struct attrs_energie *regions;
   Ulong nreg=0,r;
  
   regions = (struct attrs_energie*)calloc(rgs.Labels()+1,sizeof(struct attrs_energie));
  
   PEnergySelection(rgs,ims,regions);
 
   float temp;
   // Selection des regions
   if (relation <= -3) {
      temp=regions[1].energie;
      for (r=2;r<=rgs.Labels();r++) {
	 if (regions[r].energie<temp) {
	    temp=regions[r].energie;
	 }
      }
      for (r=1;r<=rgs.Labels();r++) {
	 if (regions[r].energie==temp) {
	    regions[r].acceptee=true;
	    ++nreg;
	 } else {
	    regions[r].acceptee=false;
	 }
      }
   } else if (relation >= 3) {
      temp=regions[1].energie;
      for (r=2;r<=rgs.Labels();r++)
	 if (regions[r].energie>temp)
	    temp=regions[r].energie;
      for (r=1;r<=rgs.Labels();r++)
	 if (regions[r].energie==temp) {
	    regions[r].acceptee=true;
	    ++nreg;
	 } else {
	    regions[r].acceptee=false;
	 }
   } else if (relation == 2) {
      for (r=1;r<=rgs.Labels();r++) {
	 if (regions[r].energie>=threshold) {
	    regions[r].acceptee=true;
	    ++nreg;
	 } else {
	    regions[r].acceptee=false;
	 }
      }
   } else if (relation == 1) {
      for (r=1;r<=rgs.Labels();r++) {
	 if (regions[r].energie>threshold) {
	    regions[r].acceptee=true;
	    ++nreg;
	 } else {
	    regions[r].acceptee=false;
	 }
      }
   } else if (relation == 0) {
      for (r=1;r<=rgs.Labels();r++) {
	 if (regions[r].energie == threshold) {
	    regions[r].acceptee=true;
	    ++nreg;
	 } else {
	    regions[r].acceptee=false;
	 }
      }
   } else if (relation == -1) {
      for (r=1;r<=rgs.Labels();r++) {
	 if (regions[r].energie < threshold) {
	    regions[r].acceptee=true;
	    ++nreg;
	 } else {
	    regions[r].acceptee=false;
	 }
      }
   } else /* if (relation == -2) */ {
      for (r=1;r<=rgs.Labels();r++) {
	 if (regions[r].energie <= threshold) {
	    regions[r].acceptee=true;
	    ++nreg;
	 } else {
	    regions[r].acceptee=false;
	 }
      }
   }
  
   // Construction de la carte de sortie.
   Point2d p;
   for (p.y = 0; p.y < rgs.Height(); p.y++)
   for (p.x = 0; p.x < rgs.Width(); p.x++)
   {
      if (regions[rgs[p]].acceptee) {
	 rgd[p] = rgs[p];
      } else {
	 rgd[p] = 0;
      }
   }
   
   rgd.Labels(rgs.Labels());
   free(regions);
   return nreg;
}

/*
 * Fonction qui calcule les valeurs d'energie interieure
 * de chacune des regions.
 */
Errc PEnergySelection( const Reg3d &rgs, const Img3duc &ims, struct attrs_energie *regions ) {
   Point3d p;
   Ulong r;
   
   // Calcul des valeurs pour chacunes des regions.
   for (p.z = 0; p.z < rgs.Depth(); p.z++)
   for (p.y = 0; p.y < rgs.Height(); p.y++)
   for (p.x = 0; p.x < rgs.Width(); p.x++)
   {
      if ((r=rgs[p])>0)
	 regions[r].sigma+=(float)ims[p]*ims[p];
      regions[r].total++;
   }
   
   for (r=1;r<=rgs.Labels();r++)
      if (regions[r].total)
	 regions[r].energie=regions[r].sigma/regions[r].total;
   
   return SUCCESS;
}

/*
 * Selectionne les regions ayant une energie de niveaux de gris
 * <= si relation=-2, < si relation=-1, > si relation=1, >= si relation=2
 * a la valeur fournie.
 */
Errc PEnergySelection( const Reg3d &rgs, const Img3duc &ims, Reg3d &rgd, int relation, float threshold ) {
   struct attrs_energie *regions;
   Ulong nreg=0,r;
  
   regions = (struct attrs_energie*)calloc(rgs.Labels()+1,sizeof(struct attrs_energie));
  
   PEnergySelection(rgs,ims,regions);
 
   float temp;
   // Selection des regions
   if (relation <= -3) {
      temp=regions[1].energie;
      for (r=2;r<=rgs.Labels();r++) {
	 if (regions[r].energie<temp) {
	    temp=regions[r].energie;
	 }
      }
      for (r=1;r<=rgs.Labels();r++) {
	 if (regions[r].energie==temp) {
	    regions[r].acceptee=true;
	    ++nreg;
	 } else {
	    regions[r].acceptee=false;
	 }
      }
   } else if (relation >= 3) {
      temp=regions[1].energie;
      for (r=2;r<=rgs.Labels();r++)
	 if (regions[r].energie>temp)
	    temp=regions[r].energie;
      for (r=1;r<=rgs.Labels();r++)
	 if (regions[r].energie==temp) {
	    regions[r].acceptee=true;
	    ++nreg;
	 } else {
	    regions[r].acceptee=false;
	 }
   } else if (relation == 2) {
      for (r=1;r<=rgs.Labels();r++) {
	 if (regions[r].energie>=threshold) {
	    regions[r].acceptee=true;
	    ++nreg;
	 } else {
	    regions[r].acceptee=false;
	 }
      }
   } else if (relation == 1) {
      for (r=1;r<=rgs.Labels();r++) {
	 if (regions[r].energie>threshold) {
	    regions[r].acceptee=true;
	    ++nreg;
	 } else {
	    regions[r].acceptee=false;
	 }
      }
   } else if (relation == 0) {
      for (r=1;r<=rgs.Labels();r++) {
	 if (regions[r].energie == threshold) {
	    regions[r].acceptee=true;
	    ++nreg;
	 } else {
	    regions[r].acceptee=false;
	 }
      }
   } else if (relation == -1) {
      for (r=1;r<=rgs.Labels();r++) {
	 if (regions[r].energie < threshold) {
	    regions[r].acceptee=true;
	    ++nreg;
	 } else {
	    regions[r].acceptee=false;
	 }
      }
   } else /* if (relation == -2) */ {
      for (r=1;r<=rgs.Labels();r++) {
	 if (regions[r].energie <= threshold) {
	    regions[r].acceptee=true;
	    ++nreg;
	 } else {
	    regions[r].acceptee=false;
	 }
      }
   }
  
   // Construction de la carte de sortie.
   Point3d p;
   for (p.z = 0; p.z < rgs.Depth(); p.z++)
   for (p.y = 0; p.y < rgs.Height(); p.y++)
   for (p.x = 0; p.x < rgs.Width(); p.x++)
   {
      if (regions[rgs[p]].acceptee) {
	 rgd[p] = rgs[p];
      } else {
	 rgd[p] = 0;
      }
   }
   
   rgd.Labels(rgs.Labels());
   free(regions);
   return nreg;
}

/*
 * Fonction qui calcule les valeurs d'energie interieure
 * de chacune des regions.
 */
Errc PEnergySelection( const Reg3d &rgs, const Img3dsl &ims, struct attrs_energie *regions ) {
   Point3d p;
   Ulong r;
   
   // Calcul des valeurs pour chacunes des regions.
   for (p.z = 0; p.z < rgs.Depth(); p.z++)
   for (p.y = 0; p.y < rgs.Height(); p.y++)
   for (p.x = 0; p.x < rgs.Width(); p.x++)
   {
      if ((r=rgs[p])>0)
	 regions[r].sigma+=(float)ims[p]*ims[p];
      regions[r].total++;
   }
   
   for (r=1;r<=rgs.Labels();r++)
      if (regions[r].total)
	 regions[r].energie=regions[r].sigma/regions[r].total;
   
   return SUCCESS;
}

/*
 * Selectionne les regions ayant une energie de niveaux de gris
 * <= si relation=-2, < si relation=-1, > si relation=1, >= si relation=2
 * a la valeur fournie.
 */
Errc PEnergySelection( const Reg3d &rgs, const Img3dsl &ims, Reg3d &rgd, int relation, float threshold ) {
   struct attrs_energie *regions;
   Ulong nreg=0,r;
  
   regions = (struct attrs_energie*)calloc(rgs.Labels()+1,sizeof(struct attrs_energie));
  
   PEnergySelection(rgs,ims,regions);
 
   float temp;
   // Selection des regions
   if (relation <= -3) {
      temp=regions[1].energie;
      for (r=2;r<=rgs.Labels();r++) {
	 if (regions[r].energie<temp) {
	    temp=regions[r].energie;
	 }
      }
      for (r=1;r<=rgs.Labels();r++) {
	 if (regions[r].energie==temp) {
	    regions[r].acceptee=true;
	    ++nreg;
	 } else {
	    regions[r].acceptee=false;
	 }
      }
   } else if (relation >= 3) {
      temp=regions[1].energie;
      for (r=2;r<=rgs.Labels();r++)
	 if (regions[r].energie>temp)
	    temp=regions[r].energie;
      for (r=1;r<=rgs.Labels();r++)
	 if (regions[r].energie==temp) {
	    regions[r].acceptee=true;
	    ++nreg;
	 } else {
	    regions[r].acceptee=false;
	 }
   } else if (relation == 2) {
      for (r=1;r<=rgs.Labels();r++) {
	 if (regions[r].energie>=threshold) {
	    regions[r].acceptee=true;
	    ++nreg;
	 } else {
	    regions[r].acceptee=false;
	 }
      }
   } else if (relation == 1) {
      for (r=1;r<=rgs.Labels();r++) {
	 if (regions[r].energie>threshold) {
	    regions[r].acceptee=true;
	    ++nreg;
	 } else {
	    regions[r].acceptee=false;
	 }
      }
   } else if (relation == 0) {
      for (r=1;r<=rgs.Labels();r++) {
	 if (regions[r].energie == threshold) {
	    regions[r].acceptee=true;
	    ++nreg;
	 } else {
	    regions[r].acceptee=false;
	 }
      }
   } else if (relation == -1) {
      for (r=1;r<=rgs.Labels();r++) {
	 if (regions[r].energie < threshold) {
	    regions[r].acceptee=true;
	    ++nreg;
	 } else {
	    regions[r].acceptee=false;
	 }
      }
   } else /* if (relation == -2) */ {
      for (r=1;r<=rgs.Labels();r++) {
	 if (regions[r].energie <= threshold) {
	    regions[r].acceptee=true;
	    ++nreg;
	 } else {
	    regions[r].acceptee=false;
	 }
      }
   }
  
   // Construction de la carte de sortie.
   Point3d p;
   for (p.z = 0; p.z < rgs.Depth(); p.z++)
   for (p.y = 0; p.y < rgs.Height(); p.y++)
   for (p.x = 0; p.x < rgs.Width(); p.x++)
   {
      if (regions[rgs[p]].acceptee) {
	 rgd[p] = rgs[p];
      } else {
	 rgd[p] = 0;
      }
   }
   
   rgd.Labels(rgs.Labels());
   free(regions);
   return nreg;
}

/*
 * Fonction qui calcule les valeurs d'energie interieure
 * de chacune des regions.
 */
Errc PEnergySelection( const Reg3d &rgs, const Img3dsf &ims, struct attrs_energie *regions ) {
   Point3d p;
   Ulong r;
   
   // Calcul des valeurs pour chacunes des regions.
   for (p.z = 0; p.z < rgs.Depth(); p.z++)
   for (p.y = 0; p.y < rgs.Height(); p.y++)
   for (p.x = 0; p.x < rgs.Width(); p.x++)
   {
      if ((r=rgs[p])>0)
	 regions[r].sigma+=(float)ims[p]*ims[p];
      regions[r].total++;
   }
   
   for (r=1;r<=rgs.Labels();r++)
      if (regions[r].total)
	 regions[r].energie=regions[r].sigma/regions[r].total;
   
   return SUCCESS;
}

/*
 * Selectionne les regions ayant une energie de niveaux de gris
 * <= si relation=-2, < si relation=-1, > si relation=1, >= si relation=2
 * a la valeur fournie.
 */
Errc PEnergySelection( const Reg3d &rgs, const Img3dsf &ims, Reg3d &rgd, int relation, float threshold ) {
   struct attrs_energie *regions;
   Ulong nreg=0,r;
  
   regions = (struct attrs_energie*)calloc(rgs.Labels()+1,sizeof(struct attrs_energie));
  
   PEnergySelection(rgs,ims,regions);
 
   float temp;
   // Selection des regions
   if (relation <= -3) {
      temp=regions[1].energie;
      for (r=2;r<=rgs.Labels();r++) {
	 if (regions[r].energie<temp) {
	    temp=regions[r].energie;
	 }
      }
      for (r=1;r<=rgs.Labels();r++) {
	 if (regions[r].energie==temp) {
	    regions[r].acceptee=true;
	    ++nreg;
	 } else {
	    regions[r].acceptee=false;
	 }
      }
   } else if (relation >= 3) {
      temp=regions[1].energie;
      for (r=2;r<=rgs.Labels();r++)
	 if (regions[r].energie>temp)
	    temp=regions[r].energie;
      for (r=1;r<=rgs.Labels();r++)
	 if (regions[r].energie==temp) {
	    regions[r].acceptee=true;
	    ++nreg;
	 } else {
	    regions[r].acceptee=false;
	 }
   } else if (relation == 2) {
      for (r=1;r<=rgs.Labels();r++) {
	 if (regions[r].energie>=threshold) {
	    regions[r].acceptee=true;
	    ++nreg;
	 } else {
	    regions[r].acceptee=false;
	 }
      }
   } else if (relation == 1) {
      for (r=1;r<=rgs.Labels();r++) {
	 if (regions[r].energie>threshold) {
	    regions[r].acceptee=true;
	    ++nreg;
	 } else {
	    regions[r].acceptee=false;
	 }
      }
   } else if (relation == 0) {
      for (r=1;r<=rgs.Labels();r++) {
	 if (regions[r].energie == threshold) {
	    regions[r].acceptee=true;
	    ++nreg;
	 } else {
	    regions[r].acceptee=false;
	 }
      }
   } else if (relation == -1) {
      for (r=1;r<=rgs.Labels();r++) {
	 if (regions[r].energie < threshold) {
	    regions[r].acceptee=true;
	    ++nreg;
	 } else {
	    regions[r].acceptee=false;
	 }
      }
   } else /* if (relation == -2) */ {
      for (r=1;r<=rgs.Labels();r++) {
	 if (regions[r].energie <= threshold) {
	    regions[r].acceptee=true;
	    ++nreg;
	 } else {
	    regions[r].acceptee=false;
	 }
      }
   }
  
   // Construction de la carte de sortie.
   Point3d p;
   for (p.z = 0; p.z < rgs.Depth(); p.z++)
   for (p.y = 0; p.y < rgs.Height(); p.y++)
   for (p.x = 0; p.x < rgs.Width(); p.x++)
   {
      if (regions[rgs[p]].acceptee) {
	 rgd[p] = rgs[p];
      } else {
	 rgd[p] = 0;
      }
   }
   
   rgd.Labels(rgs.Labels());
   free(regions);
   return nreg;
}


#ifdef MAIN
/*
 * Modify only the following constants, and the operator switches.
 */
#define USAGE	"usage: %s relation threshold [-m mask] [rg_in|-] [ims_in|-] [rg_out|-]"
#define PARC	2
#define FINC	2
#define FOUTC	1
#define MASK	2

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
   if (objs[0]->Type()==Po_Reg2d && objs[1]->Type()==Po_Img2duc) {
      Reg2d* const rgs=(Reg2d*)objs[0];
      Img2duc* const ims=(Img2duc*)objs[1];
      objd[0]=new Reg2d(rgs->Size());
      Reg2d* const rgd=(Reg2d*)objd[0];
      
      result=PEnergySelection(*rgs,*ims,*rgd,atoi(parv[0]),(float)atof(parv[1]));
      goto end;
   }
// verifier toutes les entrees
   if (objs[0]->Type()==Po_Reg2d && objs[1]->Type()==Po_Img2dsl) {
      Reg2d* const rgs=(Reg2d*)objs[0];
      Img2dsl* const ims=(Img2dsl*)objs[1];
      objd[0]=new Reg2d(rgs->Size());
      Reg2d* const rgd=(Reg2d*)objd[0];
      
      result=PEnergySelection(*rgs,*ims,*rgd,atoi(parv[0]),(float)atof(parv[1]));
      goto end;
   }
// verifier toutes les entrees
   if (objs[0]->Type()==Po_Reg2d && objs[1]->Type()==Po_Img2dsf) {
      Reg2d* const rgs=(Reg2d*)objs[0];
      Img2dsf* const ims=(Img2dsf*)objs[1];
      objd[0]=new Reg2d(rgs->Size());
      Reg2d* const rgd=(Reg2d*)objd[0];
      
      result=PEnergySelection(*rgs,*ims,*rgd,atoi(parv[0]),(float)atof(parv[1]));
      goto end;
   }
// verifier toutes les entrees
   if (objs[0]->Type()==Po_Reg3d && objs[1]->Type()==Po_Img3duc) {
      Reg3d* const rgs=(Reg3d*)objs[0];
      Img3duc* const ims=(Img3duc*)objs[1];
      objd[0]=new Reg3d(rgs->Size());
      Reg3d* const rgd=(Reg3d*)objd[0];
      
      result=PEnergySelection(*rgs,*ims,*rgd,atoi(parv[0]),(float)atof(parv[1]));
      goto end;
   }
// verifier toutes les entrees
   if (objs[0]->Type()==Po_Reg3d && objs[1]->Type()==Po_Img3dsl) {
      Reg3d* const rgs=(Reg3d*)objs[0];
      Img3dsl* const ims=(Img3dsl*)objs[1];
      objd[0]=new Reg3d(rgs->Size());
      Reg3d* const rgd=(Reg3d*)objd[0];
      
      result=PEnergySelection(*rgs,*ims,*rgd,atoi(parv[0]),(float)atof(parv[1]));
      goto end;
   }
// verifier toutes les entrees
   if (objs[0]->Type()==Po_Reg3d && objs[1]->Type()==Po_Img3dsf) {
      Reg3d* const rgs=(Reg3d*)objs[0];
      Img3dsf* const ims=(Img3dsf*)objs[1];
      objd[0]=new Reg3d(rgs->Size());
      Reg3d* const rgd=(Reg3d*)objd[0];
      
      result=PEnergySelection(*rgs,*ims,*rgd,atoi(parv[0]),(float)atof(parv[1]));
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
