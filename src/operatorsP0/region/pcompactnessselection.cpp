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
 * @author Régis Clouard - 1997-21-07
 * @author Régis Clouard - 2004-06-29
 * @author Regis Clouard - 2006-06-02 (relation =3 and -3).
 * @author Sébastien Bernery - Jun 08, 2013
 */

#define _USE_MATH_DEFINES
#include <cmath>
#include <pandore.h>
using namespace pandore;

/**
 * @file pcompacitnessselection.cpp
 * Selects regions from the compactness degree.
 * Compacitness =  (4*PI*Surface) / perimetre*perimetre.
 (cf. Wikipedia: sphericity
 */

#define SQRT2 1.414213562373f

float getCompactness(int surface, int volume) {
    static const float piracinecube = (float)pow(M_PI, 0.33);
    const float temp = piracinecube * (float)pow((6 * volume), 0.6666);
    // SQRT2 is correcting coefficient (empirical).
    return temp / (surface / SQRT2);
}

void keepCells(int relation, float threshold, std::map<Ulong, float> labels, std::vector<Ulong>& keep) {
    Ulong particular = 0;
    float particularcompactness = 0;
    if (relation <= -3) {
        particularcompactness = MAXFLOAT;
    }
    for (std::map<Ulong, float>::iterator it = labels.begin(); it != labels.end(); ++it) {
        if (relation <= -3) { // minimum
            if (it->second < particularcompactness) {
                particular = it->first;
                particularcompactness = it->second;
            }
        } else if (relation >= 3) { // maximum
            if (it->second > particularcompactness) {
                particular = it->first;
                particularcompactness = it->second;
            }
        } else if (relation == -2) { 
            if (it->second <= threshold)
                keep.push_back(it->first);
        } else if (relation == 2) {
            if (it->second >= threshold)
                keep.push_back(it->first);
        } else if (relation == -1) {
            if (it->second < threshold)
                keep.push_back(it->first);
        } else if (relation == 1) {
            if (it->second > threshold)
                keep.push_back(it->first);
        } else if (relation == 0) {
            if (it->second == threshold)
                keep.push_back(it->first);
        }
    }
    if (relation <= -3 || relation >= 3) {
        keep.push_back(particular);
    }
}

Errc PCompactnessSelection(const Reg3d& regs, std::map<Ulong, float>& labels) {
    std::map<Ulong, Ulong> volumes;
    std::map<Ulong, Ulong> surfaces;
    
    for (int z = 0; z < regs.Depth(); z++) {
        for (int y = 0; y < regs.Height(); y++) {
            for (int x = 0; x < regs.Width(); x++) {
                Ulong value = regs(z, y, x);
                if (value == 0) {
                    continue;
		}
                volumes[value] += 1;
                const Point3d p = Point3d(z, y, x);

                bool border = false;
                for (int v = 0; v < 26; v++) {
                    const Point3d voisin = p + v26[v];
                    if (regs.Hold(voisin) && regs[voisin] != value) {
                        border = true;
                        break;
                    }
                }
                if (border == true)
                    surfaces[value] += 1;
            }
        }
    }
    for (std::map<Ulong, Ulong>::iterator it = volumes.begin(); it != volumes.end(); ++it) {
        const Ulong surface = surfaces[it->first];
        const Ulong volume = it->second;
        labels[it->first] = getCompactness(surface, volume);
    }
    return SUCCESS;
}


Errc PCompactnessSelection(Reg3d& regs, Reg3d& regd, int relation, float threshold) {
    std::map<Ulong, float> labels;
    PCompactnessSelection(regs, labels);
    
    std::vector<Ulong> keep;
    keepCells(relation, threshold, labels, keep);

    for (int z = 0; z < regs.Depth(); z++) {
        for (int y = 0; y < regs.Height(); y++) {
            for (int x = 0; x < regs.Width(); x++) {
                const Ulong value = regs(z, y, x);
                for (std::vector<Ulong>::iterator it = keep.begin(); it != keep.end(); ++it) {
                    if (*it == value) {
                        regd(z, y, x) = value;
                        break;
                    }
                }
            }
        }
    }
    
    regd.Labels(regs.Labels());
    return (Long)keep.size();
}

struct attrs_compacite {
   float perimetre;
   float surface;
   float compacite;
   bool acceptee;
};

/*
 * Fonction qui calcule la valeur de compacite de chacune des regions
 * presentes (sauf la region 0 qui est le fond)
 * Rem : la surface et le perimetre sont calcules en pixels.
 */
Errc PCompactnessSelection( const Reg2d &rgs, struct attrs_compacite *regions ) {
   for (int y=0; y<rgs.Height(); y++) {
      for (int x=0; x<rgs.Width(); x++) {
	 Ulong r;
	 if ((r=rgs(y,x))>0) {
	    regions[r].surface++;
	    bool bord=false;
	    for (int v=0;v<4;v++) {
	       if ( (!(rgs.Hold(y+v4[v].y,x+v4[v].x))) || (rgs(y+v4[v].y,x+v4[v].x)!=r)) {
		  regions[r].perimetre++;
		  bord=true;
		  break;
	       }
	    }
	    if (!bord) {
	       for (int v=0;v<4;v++) {
		  if (rgs(y+v8[v*2+1].y,x+v8[v*2+1].x)!=r) {
		     // Pour les coins en creux on ajoute un petit quart de perimetre.
		     // et un bon demi de surface.
		     // obtenu par l'experience...
		     regions[r].perimetre+=0.25;
		     regions[r].surface+=0.5;	
		  }
	       }
	    }
	 }
      }
   }

   for (Ulong r=1; r<=rgs.Labels(); r++) {
      if (regions[r].perimetre>0) {
	 regions[r].compacite=(float)((4.0*M_PI*regions[r].surface)/(SQR(regions[r].perimetre)));
	 if (regions[r].compacite > 1) {
	    regions[r].compacite = 1;
	 }
      }
   }
   return SUCCESS;
}

/*
 * Selectionne les regions ayant une surface en nomnbre de pixels
 * <= si relation=-2, < si relation=-11, > si relation=1, >= si relation=2
 * a la valeur fournie.
 */
Errc PCompactnessSelection( const Reg2d &rgs, Reg2d &rgd, int relation, float threshold ) {
   struct attrs_compacite *regions;
   Ulong nreg=0;
   
   regions = (struct attrs_compacite*)calloc(rgs.Labels()+1,sizeof(struct attrs_compacite));
   PCompactnessSelection(rgs,regions);
   
   float temp;
   // Selection des regions
   if (relation <= -3) {
      temp=regions[1].compacite;
      for (Ulong r=2; r<=rgs.Labels(); r++) {
	 if (regions[r].compacite < temp) {
	    temp=regions[r].compacite;
	 }
      }
      for (Ulong r=1; r<=rgs.Labels(); r++) {
	 if (regions[r].compacite==temp) {
	    regions[r].acceptee=true;
	    ++nreg;
	 } else {
	    regions[r].acceptee=false;
	 }
      }
   } else if (relation >= 3) {
      temp=regions[1].compacite;
      for (Ulong r=2; r<=rgs.Labels(); r++) {
	 if (regions[r].compacite>temp) {
	    temp=regions[r].compacite;
	 }
      }
      for (Ulong r=1; r<=rgs.Labels(); r++) {
	 if (regions[r].compacite==temp) {
	    regions[r].acceptee=true;
	    ++nreg;
	 } else {
	    regions[r].acceptee=false;
	 }
      }
   } else if (relation == 2) {
      for (Ulong r=1; r<=rgs.Labels(); r++) {
	 if (regions[r].compacite >= threshold) {
	    regions[r].acceptee=true;
	    ++nreg;
	 } else {
	    regions[r].acceptee=false;
	 }
      }
   } else if (relation == 1) {
      for (Ulong r=1; r<=rgs.Labels(); r++) {
	 if (regions[r].compacite > threshold) {
	    regions[r].acceptee=true;
	    ++nreg;
	 }else {
	    regions[r].acceptee=false;
	 }
      }
   } else if (relation == 0) {
      for (Ulong r=1; r<=rgs.Labels(); r++) {
	 if (regions[r].compacite == threshold) {
	    regions[r].acceptee=true;
	    ++nreg;
	 } else {
	    regions[r].acceptee=false;
	 }
      }
   } else if (relation == -1) {
      for (Ulong r=1; r<=rgs.Labels(); r++) {
	 if (regions[r].compacite < threshold) {
	    regions[r].acceptee=true;
	    ++nreg;
	 } else {
	    regions[r].acceptee=false;
	 }
      }
   } else /* if (relation == -2) */ {
      for (Ulong r=1; r<=rgs.Labels(); r++) {
	 if (regions[r].compacite <= threshold) {
	    regions[r].acceptee=true;
	    ++nreg;
	 } else {
	    regions[r].acceptee=false;
	 }
      }
   }

   // Construction de la carte de sortie.
   for (int y=0; y<rgs.Height(); y++) {
      for (int x=0; x<rgs.Width(); x++) {
	 if (regions[rgs(y,x)].acceptee) {
	    rgd(y,x)= rgs(y,x);
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
#define	USAGE	"usage: %s relation threshold [-m mask] [rg_in|-] [rg_out|-]"
#define	PARC	2
#define	FINC	1
#define	FOUTC	1
#define	MASK	2

int main( int argc, char *argv[] ) {
   Errc result;              // The result code of the execution.
   Pobject* mask;            // The region maks.
   Pobject* objin[FINC+1];   // The input objects.
   Pobject* objs[FINC+1];    // The source objects masked by the maks.
   Pobject* objout[FOUTC+1]; // The output object.
   Pobject* objd[FOUTC+1];   // The result object of the execution.
   char* parv[PARC+1];       // The input parameters.
   
   ReadArgs(argc,argv,PARC,FINC,FOUTC,&mask,objin,objs,objout,objd,parv,USAGE,MASK);
   
   switch(objs[0]->Type()) {
   case Po_Reg2d :{
      Reg2d* const rgs=(Reg2d*)objs[0];
      objd[0]=new Reg2d(rgs->Size());
      Reg2d* const rgd=(Reg2d*)objd[0];
      
      result=PCompactnessSelection(*rgs,*rgd,atoi(parv[0]),(float)atof(parv[1]));
   }break;
   case Po_Reg3d :{
      Reg3d* const rgs=(Reg3d*)objs[0];
      objd[0]=new Reg3d(rgs->Size());
      Reg3d* const rgd=(Reg3d*)objd[0];
      
      result=PCompactnessSelection(*rgs,*rgd,atoi(parv[0]),(float)atof(parv[1]));
   }break;

   default :
      PrintErrorFormat(objin,FINC);
      result=FAILURE;
   }
   
   if (result)
      WriteArgs(argc,argv,PARC,FINC,FOUTC,&mask,objin,objs,objout,objd,MASK);
   
   Exit(result);
   return 0;
}

#endif
