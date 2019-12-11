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
 * @author Olivier Lezoray - 1997-07-04
 */

#include <pandore.h>
using namespace pandore;

/**
 * @file pcolorize.cpp
 * Replace the label of a region
 * by the average value of that region in the associated image.
 */





Errc PColorize( const Img2duc &ims, Reg2d &rgs, Img2duc &imd ) {
  int x, y, i;
  int indice;
  int NbreRegions = rgs.Labels();
  Float **valeurs;

  valeurs=(float**)malloc((NbreRegions+1)*sizeof(float*));
  for (i = 0;i < NbreRegions + 1; i++) {
     valeurs[i] = (float*)calloc(2,sizeof(float));
  }
  // Compute region average
  for (y=0;y<ims.Height();y++) {
     for (x=0;x<ims.Width();x++) {
	indice=(int)rgs[y][x];
	valeurs[indice][0]++;
	valeurs[indice][1]+=ims[y][x];
     }
  }
  for (i=0;i<NbreRegions+1;i++) {
     if (valeurs[i][0]!=0.F) {
	valeurs[i][1]/=valeurs[i][0];
     } else {
	valeurs[i][1]=0.F;
     }
  }
  for (y=0;y<ims.Height();y++) {
     for (x=0;x<ims.Width();x++) {
	indice=(int)rgs[y][x];
	imd[y][x]=(Img2duc::ValueType)valeurs[indice][1];
     }
  }
  for (i=0;i<NbreRegions+1;i++) {
     free(valeurs[i]);
  }
  free(valeurs);
  
  return SUCCESS;
}

Errc PColorize( const Img2dsl &ims, Reg2d &rgs, Img2dsl &imd ) {
  int x, y, i;
  int indice;
  int NbreRegions = rgs.Labels();
  Float **valeurs;

  valeurs=(float**)malloc((NbreRegions+1)*sizeof(float*));
  for (i = 0;i < NbreRegions + 1; i++) {
     valeurs[i] = (float*)calloc(2,sizeof(float));
  }
  // Compute region average
  for (y=0;y<ims.Height();y++) {
     for (x=0;x<ims.Width();x++) {
	indice=(int)rgs[y][x];
	valeurs[indice][0]++;
	valeurs[indice][1]+=ims[y][x];
     }
  }
  for (i=0;i<NbreRegions+1;i++) {
     if (valeurs[i][0]!=0.F) {
	valeurs[i][1]/=valeurs[i][0];
     } else {
	valeurs[i][1]=0.F;
     }
  }
  for (y=0;y<ims.Height();y++) {
     for (x=0;x<ims.Width();x++) {
	indice=(int)rgs[y][x];
	imd[y][x]=(Img2dsl::ValueType)valeurs[indice][1];
     }
  }
  for (i=0;i<NbreRegions+1;i++) {
     free(valeurs[i]);
  }
  free(valeurs);
  
  return SUCCESS;
}

Errc PColorize( const Img2dsf &ims, Reg2d &rgs, Img2dsf &imd ) {
  int x, y, i;
  int indice;
  int NbreRegions = rgs.Labels();
  Float **valeurs;

  valeurs=(float**)malloc((NbreRegions+1)*sizeof(float*));
  for (i = 0;i < NbreRegions + 1; i++) {
     valeurs[i] = (float*)calloc(2,sizeof(float));
  }
  // Compute region average
  for (y=0;y<ims.Height();y++) {
     for (x=0;x<ims.Width();x++) {
	indice=(int)rgs[y][x];
	valeurs[indice][0]++;
	valeurs[indice][1]+=ims[y][x];
     }
  }
  for (i=0;i<NbreRegions+1;i++) {
     if (valeurs[i][0]!=0.F) {
	valeurs[i][1]/=valeurs[i][0];
     } else {
	valeurs[i][1]=0.F;
     }
  }
  for (y=0;y<ims.Height();y++) {
     for (x=0;x<ims.Width();x++) {
	indice=(int)rgs[y][x];
	imd[y][x]=(Img2dsf::ValueType)valeurs[indice][1];
     }
  }
  for (i=0;i<NbreRegions+1;i++) {
     free(valeurs[i]);
  }
  free(valeurs);
  
  return SUCCESS;
}

Errc PColorize( const Imc2duc &ims, Reg2d &rgs, Imc2duc &imd ) {
  int x,y,i,j,indice;
  int NbreRegions=rgs.Labels();
  Float **valeurs;

  valeurs=(float**)malloc((NbreRegions+1)*sizeof(float*));
  for (i=0;i<NbreRegions+1;i++)
    valeurs[i]=(float*)calloc(4,sizeof(float));

  // Compute average.
  for (y=0;y<ims.Height();y++) {
    for (x=0;x<ims.Width();x++) {
      indice=(int)rgs[y][x];
      valeurs[indice][0]++;
      valeurs[indice][1]+=ims.X[y][x];
      valeurs[indice][2]+=ims.Y[y][x];
      valeurs[indice][3]+=ims.Z[y][x];
    }
  }
  for (i=0;i<NbreRegions+1;i++) {
     for (j=1;j<4;j++) {
	if (valeurs[i][0]!=0.F) {
	   valeurs[i][j]/=valeurs[i][0];
	} else {
	   valeurs[i][j]=0.F;
	}
     }
  }
  for (y=0;y<ims.Height();y++) {
    for (x=0;x<ims.Width();x++) {
	indice=(int)rgs[y][x];
	imd.X[y][x]=(Imc2duc::ValueType)valeurs[indice][1];
	imd.Y[y][x]=(Imc2duc::ValueType)valeurs[indice][2];
	imd.Z[y][x]=(Imc2duc::ValueType)valeurs[indice][3];
      }
  }
  for (i=0;i<NbreRegions+1;i++) {
    free(valeurs[i]);
  }
  free(valeurs);

  return SUCCESS;
}

Errc PColorize( const Imc2dsl &ims, Reg2d &rgs, Imc2dsl &imd ) {
  int x,y,i,j,indice;
  int NbreRegions=rgs.Labels();
  Float **valeurs;

  valeurs=(float**)malloc((NbreRegions+1)*sizeof(float*));
  for (i=0;i<NbreRegions+1;i++)
    valeurs[i]=(float*)calloc(4,sizeof(float));

  // Compute average.
  for (y=0;y<ims.Height();y++) {
    for (x=0;x<ims.Width();x++) {
      indice=(int)rgs[y][x];
      valeurs[indice][0]++;
      valeurs[indice][1]+=ims.X[y][x];
      valeurs[indice][2]+=ims.Y[y][x];
      valeurs[indice][3]+=ims.Z[y][x];
    }
  }
  for (i=0;i<NbreRegions+1;i++) {
     for (j=1;j<4;j++) {
	if (valeurs[i][0]!=0.F) {
	   valeurs[i][j]/=valeurs[i][0];
	} else {
	   valeurs[i][j]=0.F;
	}
     }
  }
  for (y=0;y<ims.Height();y++) {
    for (x=0;x<ims.Width();x++) {
	indice=(int)rgs[y][x];
	imd.X[y][x]=(Imc2dsl::ValueType)valeurs[indice][1];
	imd.Y[y][x]=(Imc2dsl::ValueType)valeurs[indice][2];
	imd.Z[y][x]=(Imc2dsl::ValueType)valeurs[indice][3];
      }
  }
  for (i=0;i<NbreRegions+1;i++) {
    free(valeurs[i]);
  }
  free(valeurs);

  return SUCCESS;
}

Errc PColorize( const Imc2dsf &ims, Reg2d &rgs, Imc2dsf &imd ) {
  int x,y,i,j,indice;
  int NbreRegions=rgs.Labels();
  Float **valeurs;

  valeurs=(float**)malloc((NbreRegions+1)*sizeof(float*));
  for (i=0;i<NbreRegions+1;i++)
    valeurs[i]=(float*)calloc(4,sizeof(float));

  // Compute average.
  for (y=0;y<ims.Height();y++) {
    for (x=0;x<ims.Width();x++) {
      indice=(int)rgs[y][x];
      valeurs[indice][0]++;
      valeurs[indice][1]+=ims.X[y][x];
      valeurs[indice][2]+=ims.Y[y][x];
      valeurs[indice][3]+=ims.Z[y][x];
    }
  }
  for (i=0;i<NbreRegions+1;i++) {
     for (j=1;j<4;j++) {
	if (valeurs[i][0]!=0.F) {
	   valeurs[i][j]/=valeurs[i][0];
	} else {
	   valeurs[i][j]=0.F;
	}
     }
  }
  for (y=0;y<ims.Height();y++) {
    for (x=0;x<ims.Width();x++) {
	indice=(int)rgs[y][x];
	imd.X[y][x]=(Imc2dsf::ValueType)valeurs[indice][1];
	imd.Y[y][x]=(Imc2dsf::ValueType)valeurs[indice][2];
	imd.Z[y][x]=(Imc2dsf::ValueType)valeurs[indice][3];
      }
  }
  for (i=0;i<NbreRegions+1;i++) {
    free(valeurs[i]);
  }
  free(valeurs);

  return SUCCESS;
}

Errc PColorize( const Img3duc &ims, Reg3d &rgs, Img3duc &imd ) {
  int x,y,z,i,indice;
  int NbreRegions=rgs.Labels();
  Float **valeurs;

  valeurs=(float**)malloc((NbreRegions+1)*sizeof(float*));
  for (i=0;i<NbreRegions+1;i++)
    valeurs[i]=(float*)calloc(2,sizeof(float));

  // Compute region average
  for (z=0;z<ims.Depth();z++) {
     for (y=0;y<ims.Height();y++) {
	for (x=0;x<ims.Width();x++) {
	   indice=(int)rgs[z][y][x];
	   valeurs[indice][0]++;
	   valeurs[indice][1]+=ims[z][y][x];
	}
     }
  }
  for (i=0;i<NbreRegions+1;i++) {
     if (valeurs[i][0]!=0.F) {
	valeurs[i][1]/=valeurs[i][0];
     } else {
	valeurs[i][1]=0.F;
     }
  }
      
  for (z=0;z<ims.Depth();z++) {
     for (y=0;y<ims.Height();y++) {
	for (x=0;x<ims.Width();x++) {
	   indice=(int)rgs[z][y][x];
	   imd[z][y][x]=(Img3duc::ValueType)valeurs[indice][1];
	}
     }
  }
  for (i=0;i<NbreRegions+1;i++) {
    free(valeurs[i]);
  }
  free(valeurs);
  
  return SUCCESS;
}

Errc PColorize( const Img3dsl &ims, Reg3d &rgs, Img3dsl &imd ) {
  int x,y,z,i,indice;
  int NbreRegions=rgs.Labels();
  Float **valeurs;

  valeurs=(float**)malloc((NbreRegions+1)*sizeof(float*));
  for (i=0;i<NbreRegions+1;i++)
    valeurs[i]=(float*)calloc(2,sizeof(float));

  // Compute region average
  for (z=0;z<ims.Depth();z++) {
     for (y=0;y<ims.Height();y++) {
	for (x=0;x<ims.Width();x++) {
	   indice=(int)rgs[z][y][x];
	   valeurs[indice][0]++;
	   valeurs[indice][1]+=ims[z][y][x];
	}
     }
  }
  for (i=0;i<NbreRegions+1;i++) {
     if (valeurs[i][0]!=0.F) {
	valeurs[i][1]/=valeurs[i][0];
     } else {
	valeurs[i][1]=0.F;
     }
  }
      
  for (z=0;z<ims.Depth();z++) {
     for (y=0;y<ims.Height();y++) {
	for (x=0;x<ims.Width();x++) {
	   indice=(int)rgs[z][y][x];
	   imd[z][y][x]=(Img3dsl::ValueType)valeurs[indice][1];
	}
     }
  }
  for (i=0;i<NbreRegions+1;i++) {
    free(valeurs[i]);
  }
  free(valeurs);
  
  return SUCCESS;
}

Errc PColorize( const Img3dsf &ims, Reg3d &rgs, Img3dsf &imd ) {
  int x,y,z,i,indice;
  int NbreRegions=rgs.Labels();
  Float **valeurs;

  valeurs=(float**)malloc((NbreRegions+1)*sizeof(float*));
  for (i=0;i<NbreRegions+1;i++)
    valeurs[i]=(float*)calloc(2,sizeof(float));

  // Compute region average
  for (z=0;z<ims.Depth();z++) {
     for (y=0;y<ims.Height();y++) {
	for (x=0;x<ims.Width();x++) {
	   indice=(int)rgs[z][y][x];
	   valeurs[indice][0]++;
	   valeurs[indice][1]+=ims[z][y][x];
	}
     }
  }
  for (i=0;i<NbreRegions+1;i++) {
     if (valeurs[i][0]!=0.F) {
	valeurs[i][1]/=valeurs[i][0];
     } else {
	valeurs[i][1]=0.F;
     }
  }
      
  for (z=0;z<ims.Depth();z++) {
     for (y=0;y<ims.Height();y++) {
	for (x=0;x<ims.Width();x++) {
	   indice=(int)rgs[z][y][x];
	   imd[z][y][x]=(Img3dsf::ValueType)valeurs[indice][1];
	}
     }
  }
  for (i=0;i<NbreRegions+1;i++) {
    free(valeurs[i]);
  }
  free(valeurs);
  
  return SUCCESS;
}

Errc PColorize( const Imc3duc &ims, Reg3d &rgs, Imc3duc &imd ) {
  int x,y,z,i,indice;
  int NbreRegions=rgs.Labels();
  Float **valeurs;

  valeurs=(float**)malloc((NbreRegions+1)*sizeof(float*));
  for (i=0;i<NbreRegions+1;i++)
    valeurs[i]=(float*)calloc(4,sizeof(float));

  // Compute region average
  for (z=0;z<ims.Depth();z++) {
     for (y=0;y<ims.Height();y++) {
	for (x=0;x<ims.Width();x++) {
	   indice=(int)rgs[z][y][x];
	   valeurs[indice][0]++;
	   valeurs[indice][1]+=ims.X[z][y][x];
	   valeurs[indice][2]+=ims.Y[z][y][x];
	   valeurs[indice][3]+=ims.Z[z][y][x];
	}
     }
  }
  for (i=0;i<NbreRegions+1;i++) {
     if (valeurs[i][0]!=0.F) {
	valeurs[i][1]/=valeurs[i][0];
     } else {
	valeurs[i][1]=0.F;
     }
  }
  for (z=0;z<ims.Depth();z++) {
     for (y=0;y<ims.Height();y++) {
	for (x=0;x<ims.Width();x++) {
	   indice=(int)rgs[z][y][x];
	   imd.X[z][y][x]=(Imc3duc::ValueType)valeurs[indice][1];
	   imd.Y[z][y][x]=(Imc3duc::ValueType)valeurs[indice][2];
	   imd.Z[z][y][x]=(Imc3duc::ValueType)valeurs[indice][3];
	}
     }
  }
  return SUCCESS;
}

Errc PColorize( const Imc3dsl &ims, Reg3d &rgs, Imc3dsl &imd ) {
  int x,y,z,i,indice;
  int NbreRegions=rgs.Labels();
  Float **valeurs;

  valeurs=(float**)malloc((NbreRegions+1)*sizeof(float*));
  for (i=0;i<NbreRegions+1;i++)
    valeurs[i]=(float*)calloc(4,sizeof(float));

  // Compute region average
  for (z=0;z<ims.Depth();z++) {
     for (y=0;y<ims.Height();y++) {
	for (x=0;x<ims.Width();x++) {
	   indice=(int)rgs[z][y][x];
	   valeurs[indice][0]++;
	   valeurs[indice][1]+=ims.X[z][y][x];
	   valeurs[indice][2]+=ims.Y[z][y][x];
	   valeurs[indice][3]+=ims.Z[z][y][x];
	}
     }
  }
  for (i=0;i<NbreRegions+1;i++) {
     if (valeurs[i][0]!=0.F) {
	valeurs[i][1]/=valeurs[i][0];
     } else {
	valeurs[i][1]=0.F;
     }
  }
  for (z=0;z<ims.Depth();z++) {
     for (y=0;y<ims.Height();y++) {
	for (x=0;x<ims.Width();x++) {
	   indice=(int)rgs[z][y][x];
	   imd.X[z][y][x]=(Imc3dsl::ValueType)valeurs[indice][1];
	   imd.Y[z][y][x]=(Imc3dsl::ValueType)valeurs[indice][2];
	   imd.Z[z][y][x]=(Imc3dsl::ValueType)valeurs[indice][3];
	}
     }
  }
  return SUCCESS;
}

Errc PColorize( const Imc3dsf &ims, Reg3d &rgs, Imc3dsf &imd ) {
  int x,y,z,i,indice;
  int NbreRegions=rgs.Labels();
  Float **valeurs;

  valeurs=(float**)malloc((NbreRegions+1)*sizeof(float*));
  for (i=0;i<NbreRegions+1;i++)
    valeurs[i]=(float*)calloc(4,sizeof(float));

  // Compute region average
  for (z=0;z<ims.Depth();z++) {
     for (y=0;y<ims.Height();y++) {
	for (x=0;x<ims.Width();x++) {
	   indice=(int)rgs[z][y][x];
	   valeurs[indice][0]++;
	   valeurs[indice][1]+=ims.X[z][y][x];
	   valeurs[indice][2]+=ims.Y[z][y][x];
	   valeurs[indice][3]+=ims.Z[z][y][x];
	}
     }
  }
  for (i=0;i<NbreRegions+1;i++) {
     if (valeurs[i][0]!=0.F) {
	valeurs[i][1]/=valeurs[i][0];
     } else {
	valeurs[i][1]=0.F;
     }
  }
  for (z=0;z<ims.Depth();z++) {
     for (y=0;y<ims.Height();y++) {
	for (x=0;x<ims.Width();x++) {
	   indice=(int)rgs[z][y][x];
	   imd.X[z][y][x]=(Imc3dsf::ValueType)valeurs[indice][1];
	   imd.Y[z][y][x]=(Imc3dsf::ValueType)valeurs[indice][2];
	   imd.Z[z][y][x]=(Imc3dsf::ValueType)valeurs[indice][3];
	}
     }
  }
  return SUCCESS;
}


#ifdef MAIN
#define	USAGE	"usage: %s [-m mask] [im_in|-] [rg_in] [im_out|-]"
#define	PARC	0
#define	FINC	2
#define	FOUTC	1
#define	MASK	2

int main( int argc, char *argv[] ) {
   Errc result;                // The result code of the execution.
   Pobject* mask;              // The region map.
   Pobject* objin[FINC + 1];   // The input objects.
   Pobject* objs[FINC + 1];    // The source objects masked.
   Pobject* objout[FOUTC + 1]; // The output object.
   Pobject* objd[FOUTC + 1];   // The result object of the execution.
   char* parv[PARC + 1];       // The input parameters.

   ReadArgs(argc, argv, PARC, FINC, FOUTC, &mask, objin, objs, objout, objd, parv, USAGE, MASK); 
   if ((objs[0]->Type() == Po_Img2duc) && (objs[1]->Type() == Po_Reg2d)) {
     Img2duc* const ims=(Img2duc*)objs[0];
     Reg2d*   const ims1=(Reg2d*)objs[1];
     objd[0]=new Img2duc(ims->Size());
     Img2duc* const imd=(Img2duc*)objd[0];
     
     result=PColorize(*ims,*ims1,*imd);
     goto end;
   }
   if ((objs[0]->Type() == Po_Img2dsl) && (objs[1]->Type() == Po_Reg2d)) {
     Img2dsl* const ims=(Img2dsl*)objs[0];
     Reg2d*   const ims1=(Reg2d*)objs[1];
     objd[0]=new Img2dsl(ims->Size());
     Img2dsl* const imd=(Img2dsl*)objd[0];
     
     result=PColorize(*ims,*ims1,*imd);
     goto end;
   }
   if ((objs[0]->Type() == Po_Img2dsf) && (objs[1]->Type() == Po_Reg2d)) {
     Img2dsf* const ims=(Img2dsf*)objs[0];
     Reg2d*   const ims1=(Reg2d*)objs[1];
     objd[0]=new Img2dsf(ims->Size());
     Img2dsf* const imd=(Img2dsf*)objd[0];
     
     result=PColorize(*ims,*ims1,*imd);
     goto end;
   }
   if ((objs[0]->Type() == Po_Imc2duc) && (objs[1]->Type() == Po_Reg2d)) {
     Imc2duc* const ims=(Imc2duc*)objs[0];
     Reg2d* const ims1=(Reg2d*)objs[1];
     objd[0]=new Imc2duc(ims->Size());
     Imc2duc* const imd=(Imc2duc*)objd[0];
     
     result=PColorize(*ims,*ims1,*imd);
     goto end;
   }
   if ((objs[0]->Type() == Po_Imc2dsl) && (objs[1]->Type() == Po_Reg2d)) {
     Imc2dsl* const ims=(Imc2dsl*)objs[0];
     Reg2d* const ims1=(Reg2d*)objs[1];
     objd[0]=new Imc2dsl(ims->Size());
     Imc2dsl* const imd=(Imc2dsl*)objd[0];
     
     result=PColorize(*ims,*ims1,*imd);
     goto end;
   }
   if ((objs[0]->Type() == Po_Imc2dsf) && (objs[1]->Type() == Po_Reg2d)) {
     Imc2dsf* const ims=(Imc2dsf*)objs[0];
     Reg2d* const ims1=(Reg2d*)objs[1];
     objd[0]=new Imc2dsf(ims->Size());
     Imc2dsf* const imd=(Imc2dsf*)objd[0];
     
     result=PColorize(*ims,*ims1,*imd);
     goto end;
   }
   if ((objs[0]->Type() == Po_Img3duc) && (objs[1]->Type() == Po_Reg3d)) {
     Img3duc* const ims=(Img3duc*)objs[0];
     Reg3d*   const ims1=(Reg3d*)objs[1];
     objd[0]=new Img3duc(ims->Size());
     Img3duc* const imd=(Img3duc*)objd[0];
     
     result=PColorize(*ims,*ims1,*imd);
     goto end;
   }
   if ((objs[0]->Type() == Po_Img3dsl) && (objs[1]->Type() == Po_Reg3d)) {
     Img3dsl* const ims=(Img3dsl*)objs[0];
     Reg3d*   const ims1=(Reg3d*)objs[1];
     objd[0]=new Img3dsl(ims->Size());
     Img3dsl* const imd=(Img3dsl*)objd[0];
     
     result=PColorize(*ims,*ims1,*imd);
     goto end;
   }
   if ((objs[0]->Type() == Po_Img3dsf) && (objs[1]->Type() == Po_Reg3d)) {
     Img3dsf* const ims=(Img3dsf*)objs[0];
     Reg3d*   const ims1=(Reg3d*)objs[1];
     objd[0]=new Img3dsf(ims->Size());
     Img3dsf* const imd=(Img3dsf*)objd[0];
     
     result=PColorize(*ims,*ims1,*imd);
     goto end;
   }
   if ((objs[0]->Type() == Po_Imc3duc) && (objs[1]->Type() == Po_Reg3d)) {
     Imc3duc* const ims = (Imc3duc*)objs[0];
     Reg3d* const ims1 = (Reg3d*)objs[1];
     objd[0] = new Imc3duc(ims->Size());
     Imc3duc* const imd = (Imc3duc*)objd[0];
     
     result = PColorize(*ims,*ims1,*imd);
     goto end;
   }
   if ((objs[0]->Type() == Po_Imc3dsl) && (objs[1]->Type() == Po_Reg3d)) {
     Imc3dsl* const ims = (Imc3dsl*)objs[0];
     Reg3d* const ims1 = (Reg3d*)objs[1];
     objd[0] = new Imc3dsl(ims->Size());
     Imc3dsl* const imd = (Imc3dsl*)objd[0];
     
     result = PColorize(*ims,*ims1,*imd);
     goto end;
   }
   if ((objs[0]->Type() == Po_Imc3dsf) && (objs[1]->Type() == Po_Reg3d)) {
     Imc3dsf* const ims = (Imc3dsf*)objs[0];
     Reg3d* const ims1 = (Reg3d*)objs[1];
     objd[0] = new Imc3dsf(ims->Size());
     Imc3dsf* const imd = (Imc3dsf*)objd[0];
     
     result = PColorize(*ims,*ims1,*imd);
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
