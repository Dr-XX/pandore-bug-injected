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
 * @author Alexandre Duret-Lutz. 1999-10-08
 */

#include <pandore.h>
using namespace pandore;

/**
 * @file pobject2col.cpp
 * Convert any Pandore object into a collection.
 */

Errc PObject2Col( Collection &obj_in,  Collection &col_out, const std::string &name ) {
   col_out.SETPOBJECT(name, Collection, &obj_in);
   
   return SUCCESS;
}
Errc PObject2Col( Img1duc &obj_in,  Collection &col_out, const std::string &name ) {
   col_out.SETPOBJECT(name, Img1duc, &obj_in);
   
   return SUCCESS;
}
Errc PObject2Col( Img1dsl &obj_in,  Collection &col_out, const std::string &name ) {
   col_out.SETPOBJECT(name, Img1dsl, &obj_in);
   
   return SUCCESS;
}
Errc PObject2Col( Img1dsf &obj_in,  Collection &col_out, const std::string &name ) {
   col_out.SETPOBJECT(name, Img1dsf, &obj_in);
   
   return SUCCESS;
}
Errc PObject2Col( Img2duc &obj_in,  Collection &col_out, const std::string &name ) {
   col_out.SETPOBJECT(name, Img2duc, &obj_in);
   
   return SUCCESS;
}
Errc PObject2Col( Img2dsl &obj_in,  Collection &col_out, const std::string &name ) {
   col_out.SETPOBJECT(name, Img2dsl, &obj_in);
   
   return SUCCESS;
}
Errc PObject2Col( Img2dsf &obj_in,  Collection &col_out, const std::string &name ) {
   col_out.SETPOBJECT(name, Img2dsf, &obj_in);
   
   return SUCCESS;
}
Errc PObject2Col( Img3duc &obj_in,  Collection &col_out, const std::string &name ) {
   col_out.SETPOBJECT(name, Img3duc, &obj_in);
   
   return SUCCESS;
}
Errc PObject2Col( Img3dsl &obj_in,  Collection &col_out, const std::string &name ) {
   col_out.SETPOBJECT(name, Img3dsl, &obj_in);
   
   return SUCCESS;
}
Errc PObject2Col( Img3dsf &obj_in,  Collection &col_out, const std::string &name ) {
   col_out.SETPOBJECT(name, Img3dsf, &obj_in);
   
   return SUCCESS;
}
Errc PObject2Col( Reg1d &obj_in,  Collection &col_out, const std::string &name ) {
   col_out.SETPOBJECT(name, Reg1d, &obj_in);
   
   return SUCCESS;
}
Errc PObject2Col( Reg2d &obj_in,  Collection &col_out, const std::string &name ) {
   col_out.SETPOBJECT(name, Reg2d, &obj_in);
   
   return SUCCESS;
}
Errc PObject2Col( Reg3d &obj_in,  Collection &col_out, const std::string &name ) {
   col_out.SETPOBJECT(name, Reg3d, &obj_in);
   
   return SUCCESS;
}
Errc PObject2Col( Graph2d &obj_in,  Collection &col_out, const std::string &name ) {
   col_out.SETPOBJECT(name, Graph2d, &obj_in);
   
   return SUCCESS;
}
Errc PObject2Col( Graph3d &obj_in,  Collection &col_out, const std::string &name ) {
   col_out.SETPOBJECT(name, Graph3d, &obj_in);
   
   return SUCCESS;
}
Errc PObject2Col( Imc2duc &obj_in,  Collection &col_out, const std::string &name ) {
   col_out.SETPOBJECT(name, Imc2duc, &obj_in);
   
   return SUCCESS;
}
Errc PObject2Col( Imc2dsl &obj_in,  Collection &col_out, const std::string &name ) {
   col_out.SETPOBJECT(name, Imc2dsl, &obj_in);
   
   return SUCCESS;
}
Errc PObject2Col( Imc2dsf &obj_in,  Collection &col_out, const std::string &name ) {
   col_out.SETPOBJECT(name, Imc2dsf, &obj_in);
   
   return SUCCESS;
}
Errc PObject2Col( Imc3duc &obj_in,  Collection &col_out, const std::string &name ) {
   col_out.SETPOBJECT(name, Imc3duc, &obj_in);
   
   return SUCCESS;
}
Errc PObject2Col( Imc3dsl &obj_in,  Collection &col_out, const std::string &name ) {
   col_out.SETPOBJECT(name, Imc3dsl, &obj_in);
   
   return SUCCESS;
}
Errc PObject2Col( Imc3dsf &obj_in,  Collection &col_out, const std::string &name ) {
   col_out.SETPOBJECT(name, Imc3dsf, &obj_in);
   
   return SUCCESS;
}
Errc PObject2Col( Imx1duc &obj_in,  Collection &col_out, const std::string &name ) {
   col_out.SETPOBJECT(name, Imx1duc, &obj_in);
   
   return SUCCESS;
}
Errc PObject2Col( Imx1dsl &obj_in,  Collection &col_out, const std::string &name ) {
   col_out.SETPOBJECT(name, Imx1dsl, &obj_in);
   
   return SUCCESS;
}
Errc PObject2Col( Imx1dsf &obj_in,  Collection &col_out, const std::string &name ) {
   col_out.SETPOBJECT(name, Imx1dsf, &obj_in);
   
   return SUCCESS;
}
Errc PObject2Col( Imx2duc &obj_in,  Collection &col_out, const std::string &name ) {
   col_out.SETPOBJECT(name, Imx2duc, &obj_in);
   
   return SUCCESS;
}
Errc PObject2Col( Imx2dsl &obj_in,  Collection &col_out, const std::string &name ) {
   col_out.SETPOBJECT(name, Imx2dsl, &obj_in);
   
   return SUCCESS;
}
Errc PObject2Col( Imx2dsf &obj_in,  Collection &col_out, const std::string &name ) {
   col_out.SETPOBJECT(name, Imx2dsf, &obj_in);
   
   return SUCCESS;
}
Errc PObject2Col( Imx3duc &obj_in,  Collection &col_out, const std::string &name ) {
   col_out.SETPOBJECT(name, Imx3duc, &obj_in);
   
   return SUCCESS;
}
Errc PObject2Col( Imx3dsl &obj_in,  Collection &col_out, const std::string &name ) {
   col_out.SETPOBJECT(name, Imx3dsl, &obj_in);
   
   return SUCCESS;
}
Errc PObject2Col( Imx3dsf &obj_in,  Collection &col_out, const std::string &name ) {
   col_out.SETPOBJECT(name, Imx3dsf, &obj_in);
   
   return SUCCESS;
}
Errc PObject2Col( Point1d &obj_in,  Collection &col_out, const std::string &name ) {
   col_out.SETPOBJECT(name, Point1d, &obj_in);
   
   return SUCCESS;
}
Errc PObject2Col( Point2d &obj_in,  Collection &col_out, const std::string &name ) {
   col_out.SETPOBJECT(name, Point2d, &obj_in);
   
   return SUCCESS;
}
Errc PObject2Col( Point3d &obj_in,  Collection &col_out, const std::string &name ) {
   col_out.SETPOBJECT(name, Point3d, &obj_in);
   
   return SUCCESS;
}
Errc PObject2Col( Dimension1d &obj_in,  Collection &col_out, const std::string &name ) {
   col_out.SETPOBJECT(name, Dimension1d, &obj_in);
   
   return SUCCESS;
}
Errc PObject2Col( Dimension2d &obj_in,  Collection &col_out, const std::string &name ) {
   col_out.SETPOBJECT(name, Dimension2d, &obj_in);
   
   return SUCCESS;
}
Errc PObject2Col( Dimension3d &obj_in,  Collection &col_out, const std::string &name ) {
   col_out.SETPOBJECT(name, Dimension3d, &obj_in);
   
   return SUCCESS;
}

#ifdef MAIN
#define	USAGE	"usage: %s name [obj_in|-] [col_out|-]"
#define	PARC	1
#define	FINC	1
#define	FOUTC	1
#define	MASK	0

int main( int argc, char *argv[] ) {
   Errc result;                // The result code of the execution.
   Pobject* mask;              // The region map.
   Pobject* objin[FINC + 1];   // The input objects.
   Pobject* objs[FINC + 1];    // The source objects masked.
   Pobject* objout[FOUTC + 1]; // The output object.
   Pobject* objd[FOUTC + 1];   // The result object of the execution.
   char* parv[PARC + 1];       // The input parameters.

   ReadArgs(argc, argv, PARC, FINC, FOUTC, &mask, objin, objs, objout, objd, parv, USAGE, MASK); 
   if (objs[0]->Type() == Po_Collection) {
      Collection* const obs = (Collection*)objs[0];
      objd[0] = new Collection;
      Collection* const cold = (Collection*)objd[0];
      result = PObject2Col(*obs, *cold, parv[0]);
      goto end;	
   }
   if (objs[0]->Type() == Po_Img1duc) {
      Img1duc* const obs = (Img1duc*)objs[0];
      objd[0] = new Collection;
      Collection* const cold = (Collection*)objd[0];
      result = PObject2Col(*obs, *cold, parv[0]);
      goto end;	
   }
   if (objs[0]->Type() == Po_Img1dsl) {
      Img1dsl* const obs = (Img1dsl*)objs[0];
      objd[0] = new Collection;
      Collection* const cold = (Collection*)objd[0];
      result = PObject2Col(*obs, *cold, parv[0]);
      goto end;	
   }
   if (objs[0]->Type() == Po_Img1dsf) {
      Img1dsf* const obs = (Img1dsf*)objs[0];
      objd[0] = new Collection;
      Collection* const cold = (Collection*)objd[0];
      result = PObject2Col(*obs, *cold, parv[0]);
      goto end;	
   }
   if (objs[0]->Type() == Po_Img2duc) {
      Img2duc* const obs = (Img2duc*)objs[0];
      objd[0] = new Collection;
      Collection* const cold = (Collection*)objd[0];
      result = PObject2Col(*obs, *cold, parv[0]);
      goto end;	
   }
   if (objs[0]->Type() == Po_Img2dsl) {
      Img2dsl* const obs = (Img2dsl*)objs[0];
      objd[0] = new Collection;
      Collection* const cold = (Collection*)objd[0];
      result = PObject2Col(*obs, *cold, parv[0]);
      goto end;	
   }
   if (objs[0]->Type() == Po_Img2dsf) {
      Img2dsf* const obs = (Img2dsf*)objs[0];
      objd[0] = new Collection;
      Collection* const cold = (Collection*)objd[0];
      result = PObject2Col(*obs, *cold, parv[0]);
      goto end;	
   }
   if (objs[0]->Type() == Po_Img3duc) {
      Img3duc* const obs = (Img3duc*)objs[0];
      objd[0] = new Collection;
      Collection* const cold = (Collection*)objd[0];
      result = PObject2Col(*obs, *cold, parv[0]);
      goto end;	
   }
   if (objs[0]->Type() == Po_Img3dsl) {
      Img3dsl* const obs = (Img3dsl*)objs[0];
      objd[0] = new Collection;
      Collection* const cold = (Collection*)objd[0];
      result = PObject2Col(*obs, *cold, parv[0]);
      goto end;	
   }
   if (objs[0]->Type() == Po_Img3dsf) {
      Img3dsf* const obs = (Img3dsf*)objs[0];
      objd[0] = new Collection;
      Collection* const cold = (Collection*)objd[0];
      result = PObject2Col(*obs, *cold, parv[0]);
      goto end;	
   }
   if (objs[0]->Type() == Po_Reg1d) {
      Reg1d* const obs = (Reg1d*)objs[0];
      objd[0] = new Collection;
      Collection* const cold = (Collection*)objd[0];
      result = PObject2Col(*obs, *cold, parv[0]);
      goto end;	
   }
   if (objs[0]->Type() == Po_Reg2d) {
      Reg2d* const obs = (Reg2d*)objs[0];
      objd[0] = new Collection;
      Collection* const cold = (Collection*)objd[0];
      result = PObject2Col(*obs, *cold, parv[0]);
      goto end;	
   }
   if (objs[0]->Type() == Po_Reg3d) {
      Reg3d* const obs = (Reg3d*)objs[0];
      objd[0] = new Collection;
      Collection* const cold = (Collection*)objd[0];
      result = PObject2Col(*obs, *cold, parv[0]);
      goto end;	
   }
   if (objs[0]->Type() == Po_Graph2d) {
      Graph2d* const obs = (Graph2d*)objs[0];
      objd[0] = new Collection;
      Collection* const cold = (Collection*)objd[0];
      result = PObject2Col(*obs, *cold, parv[0]);
      goto end;	
   }
   if (objs[0]->Type() == Po_Graph3d) {
      Graph3d* const obs = (Graph3d*)objs[0];
      objd[0] = new Collection;
      Collection* const cold = (Collection*)objd[0];
      result = PObject2Col(*obs, *cold, parv[0]);
      goto end;	
   }
   if (objs[0]->Type() == Po_Imc2duc) {
      Imc2duc* const obs = (Imc2duc*)objs[0];
      objd[0] = new Collection;
      Collection* const cold = (Collection*)objd[0];
      result = PObject2Col(*obs, *cold, parv[0]);
      goto end;	
   }
   if (objs[0]->Type() == Po_Imc2dsl) {
      Imc2dsl* const obs = (Imc2dsl*)objs[0];
      objd[0] = new Collection;
      Collection* const cold = (Collection*)objd[0];
      result = PObject2Col(*obs, *cold, parv[0]);
      goto end;	
   }
   if (objs[0]->Type() == Po_Imc2dsf) {
      Imc2dsf* const obs = (Imc2dsf*)objs[0];
      objd[0] = new Collection;
      Collection* const cold = (Collection*)objd[0];
      result = PObject2Col(*obs, *cold, parv[0]);
      goto end;	
   }
   if (objs[0]->Type() == Po_Imc3duc) {
      Imc3duc* const obs = (Imc3duc*)objs[0];
      objd[0] = new Collection;
      Collection* const cold = (Collection*)objd[0];
      result = PObject2Col(*obs, *cold, parv[0]);
      goto end;	
   }
   if (objs[0]->Type() == Po_Imc3dsl) {
      Imc3dsl* const obs = (Imc3dsl*)objs[0];
      objd[0] = new Collection;
      Collection* const cold = (Collection*)objd[0];
      result = PObject2Col(*obs, *cold, parv[0]);
      goto end;	
   }
   if (objs[0]->Type() == Po_Imc3dsf) {
      Imc3dsf* const obs = (Imc3dsf*)objs[0];
      objd[0] = new Collection;
      Collection* const cold = (Collection*)objd[0];
      result = PObject2Col(*obs, *cold, parv[0]);
      goto end;	
   }
   if (objs[0]->Type() == Po_Imx1duc) {
      Imx1duc* const obs = (Imx1duc*)objs[0];
      objd[0] = new Collection;
      Collection* const cold = (Collection*)objd[0];
      result = PObject2Col(*obs, *cold, parv[0]);
      goto end;	
   }
   if (objs[0]->Type() == Po_Imx1dsl) {
      Imx1dsl* const obs = (Imx1dsl*)objs[0];
      objd[0] = new Collection;
      Collection* const cold = (Collection*)objd[0];
      result = PObject2Col(*obs, *cold, parv[0]);
      goto end;	
   }
   if (objs[0]->Type() == Po_Imx1dsf) {
      Imx1dsf* const obs = (Imx1dsf*)objs[0];
      objd[0] = new Collection;
      Collection* const cold = (Collection*)objd[0];
      result = PObject2Col(*obs, *cold, parv[0]);
      goto end;	
   }
   if (objs[0]->Type() == Po_Imx2duc) {
      Imx2duc* const obs = (Imx2duc*)objs[0];
      objd[0] = new Collection;
      Collection* const cold = (Collection*)objd[0];
      result = PObject2Col(*obs, *cold, parv[0]);
      goto end;	
   }
   if (objs[0]->Type() == Po_Imx2dsl) {
      Imx2dsl* const obs = (Imx2dsl*)objs[0];
      objd[0] = new Collection;
      Collection* const cold = (Collection*)objd[0];
      result = PObject2Col(*obs, *cold, parv[0]);
      goto end;	
   }
   if (objs[0]->Type() == Po_Imx2dsf) {
      Imx2dsf* const obs = (Imx2dsf*)objs[0];
      objd[0] = new Collection;
      Collection* const cold = (Collection*)objd[0];
      result = PObject2Col(*obs, *cold, parv[0]);
      goto end;	
   }
   if (objs[0]->Type() == Po_Imx3duc) {
      Imx3duc* const obs = (Imx3duc*)objs[0];
      objd[0] = new Collection;
      Collection* const cold = (Collection*)objd[0];
      result = PObject2Col(*obs, *cold, parv[0]);
      goto end;	
   }
   if (objs[0]->Type() == Po_Imx3dsl) {
      Imx3dsl* const obs = (Imx3dsl*)objs[0];
      objd[0] = new Collection;
      Collection* const cold = (Collection*)objd[0];
      result = PObject2Col(*obs, *cold, parv[0]);
      goto end;	
   }
   if (objs[0]->Type() == Po_Imx3dsf) {
      Imx3dsf* const obs = (Imx3dsf*)objs[0];
      objd[0] = new Collection;
      Collection* const cold = (Collection*)objd[0];
      result = PObject2Col(*obs, *cold, parv[0]);
      goto end;	
   }
   if (objs[0]->Type() == Po_Point1d) {
      Point1d* const obs = (Point1d*)objs[0];
      objd[0] = new Collection;
      Collection* const cold = (Collection*)objd[0];
      result = PObject2Col(*obs, *cold, parv[0]);
      goto end;	
   }
   if (objs[0]->Type() == Po_Point2d) {
      Point2d* const obs = (Point2d*)objs[0];
      objd[0] = new Collection;
      Collection* const cold = (Collection*)objd[0];
      result = PObject2Col(*obs, *cold, parv[0]);
      goto end;	
   }
   if (objs[0]->Type() == Po_Point3d) {
      Point3d* const obs = (Point3d*)objs[0];
      objd[0] = new Collection;
      Collection* const cold = (Collection*)objd[0];
      result = PObject2Col(*obs, *cold, parv[0]);
      goto end;	
   }
   if (objs[0]->Type() == Po_Dimension1d) {
      Dimension1d* const obs = (Dimension1d*)objs[0];
      objd[0] = new Collection;
      Collection* const cold = (Collection*)objd[0];
      result = PObject2Col(*obs, *cold, parv[0]);
      goto end;	
   }
   if (objs[0]->Type() == Po_Dimension2d) {
      Dimension2d* const obs = (Dimension2d*)objs[0];
      objd[0] = new Collection;
      Collection* const cold = (Collection*)objd[0];
      result = PObject2Col(*obs, *cold, parv[0]);
      goto end;	
   }
   if (objs[0]->Type() == Po_Dimension3d) {
      Dimension3d* const obs = (Dimension3d*)objs[0];
      objd[0] = new Collection;
      Collection* const cold = (Collection*)objd[0];
      result = PObject2Col(*obs, *cold, parv[0]);
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
