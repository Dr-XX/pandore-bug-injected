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
 * @author Alexandre Duret-Lutz - 1999-11-03
 */

#include <pandore.h>
using namespace pandore;

/**
 * @file prelabelisation.cpp
 * Relabelise une carte de regions a partir d'un tableau
 */


Errc PRelabelisation( const std::string &a_in, const Collection &in, const Reg1d &reg_in, Reg1d &reg_out ) {
   std::string type = in.GetType(a_in);
   Long* lab  = in.GETARRAY(a_in,Long);
   Long labnbr = in.GETARRAYSIZE(a_in,Long);
   Point1d p;
   
   for (p.x = 0; p.x < reg_in.Width(); p.x++)
   {
      Long r = reg_in[p];
      reg_out[p] = (r==0 || r>labnbr) ? 0 : lab[r-1];
   }   
   reg_out.Labels(labnbr);

   return SUCCESS;
}

Errc PRelabelisation( const std::string &a_in, const Collection &in, const Reg2d &reg_in, Reg2d &reg_out ) {
   std::string type = in.GetType(a_in);
   Long* lab  = in.GETARRAY(a_in,Long);
   Long labnbr = in.GETARRAYSIZE(a_in,Long);
   Point2d p;
   
   for (p.y = 0; p.y < reg_in.Height(); p.y++)
   for (p.x = 0; p.x < reg_in.Width(); p.x++)
   {
      Long r = reg_in[p];
      reg_out[p] = (r==0 || r>labnbr) ? 0 : lab[r-1];
   }   
   reg_out.Labels(labnbr);

   return SUCCESS;
}

Errc PRelabelisation( const std::string &a_in, const Collection &in, const Reg3d &reg_in, Reg3d &reg_out ) {
   std::string type = in.GetType(a_in);
   Long* lab  = in.GETARRAY(a_in,Long);
   Long labnbr = in.GETARRAYSIZE(a_in,Long);
   Point3d p;
   
   for (p.z = 0; p.z < reg_in.Depth(); p.z++)
   for (p.y = 0; p.y < reg_in.Height(); p.y++)
   for (p.x = 0; p.x < reg_in.Width(); p.x++)
   {
      Long r = reg_in[p];
      reg_out[p] = (r==0 || r>labnbr) ? 0 : lab[r-1];
   }   
   reg_out.Labels(labnbr);

   return SUCCESS;
}

#ifdef MAIN
#define USAGE	"usage: %s attr [col_in|-] [rg_in|-] [rg_out|-]"
#define PARC	1
#define FINC	2
#define FOUTC	1
#define MASK	0
int main( int argc, char *argv[] ) {
   Errc result;                // The result code of the execution.
   Pobject* mask;              // The region map.
   Pobject* objin[FINC + 1];   // The input objects.
   Pobject* objs[FINC + 1];    // The source objects masked.
   Pobject* objout[FOUTC + 1]; // The output object.
   Pobject* objd[FOUTC + 1];   // The result object of the execution.
   char* parv[PARC + 1];       // The input parameters.

   ReadArgs(argc, argv, PARC, FINC, FOUTC, &mask, objin, objs, objout, objd, parv, USAGE, MASK); 
   if (objs[0]->Type() == Po_Collection && 
       objs[1]->Type() == Po_Reg1d) {
      Collection* ims1=(Collection*)objs[0];	
      Reg1d* ims2=(Reg1d*)objs[1];
      Reg1d* imd1=new Reg1d(ims2->Size());
      objd[0] = imd1;
      result = PRelabelisation(argv[1],*ims1,*ims2,*imd1);
      goto end;
   }
   if (objs[0]->Type() == Po_Collection && 
       objs[1]->Type() == Po_Reg2d) {
      Collection* ims1=(Collection*)objs[0];	
      Reg2d* ims2=(Reg2d*)objs[1];
      Reg2d* imd1=new Reg2d(ims2->Size());
      objd[0] = imd1;
      result = PRelabelisation(argv[1],*ims1,*ims2,*imd1);
      goto end;
   }
   if (objs[0]->Type() == Po_Collection && 
       objs[1]->Type() == Po_Reg3d) {
      Collection* ims1=(Collection*)objs[0];	
      Reg3d* ims2=(Reg3d*)objs[1];
      Reg3d* imd1=new Reg3d(ims2->Size());
      objd[0] = imd1;
      result = PRelabelisation(argv[1],*ims1,*ims2,*imd1);
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
