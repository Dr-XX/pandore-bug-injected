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
 * @file pcolsetobject.cpp
 * Converts any Pandore object into a collection.
 */
Errc PColSetObject( Collection &col_out, Collection &obj_in, const std::string& name_in ) {
   col_out.SETPOBJECT(name_in, Collection, &obj_in);
   
   return SUCCESS;
}
Errc PColSetObject( Collection &col_out, Img1duc &obj_in, const std::string& name_in ) {
   col_out.SETPOBJECT(name_in, Img1duc, &obj_in);
   
   return SUCCESS;
}
Errc PColSetObject( Collection &col_out, Img1dsl &obj_in, const std::string& name_in ) {
   col_out.SETPOBJECT(name_in, Img1dsl, &obj_in);
   
   return SUCCESS;
}
Errc PColSetObject( Collection &col_out, Img1dsf &obj_in, const std::string& name_in ) {
   col_out.SETPOBJECT(name_in, Img1dsf, &obj_in);
   
   return SUCCESS;
}
Errc PColSetObject( Collection &col_out, Img2duc &obj_in, const std::string& name_in ) {
   col_out.SETPOBJECT(name_in, Img2duc, &obj_in);
   
   return SUCCESS;
}
Errc PColSetObject( Collection &col_out, Img2dsl &obj_in, const std::string& name_in ) {
   col_out.SETPOBJECT(name_in, Img2dsl, &obj_in);
   
   return SUCCESS;
}
Errc PColSetObject( Collection &col_out, Img2dsf &obj_in, const std::string& name_in ) {
   col_out.SETPOBJECT(name_in, Img2dsf, &obj_in);
   
   return SUCCESS;
}
Errc PColSetObject( Collection &col_out, Img3duc &obj_in, const std::string& name_in ) {
   col_out.SETPOBJECT(name_in, Img3duc, &obj_in);
   
   return SUCCESS;
}
Errc PColSetObject( Collection &col_out, Img3dsl &obj_in, const std::string& name_in ) {
   col_out.SETPOBJECT(name_in, Img3dsl, &obj_in);
   
   return SUCCESS;
}
Errc PColSetObject( Collection &col_out, Img3dsf &obj_in, const std::string& name_in ) {
   col_out.SETPOBJECT(name_in, Img3dsf, &obj_in);
   
   return SUCCESS;
}
Errc PColSetObject( Collection &col_out, Reg1d &obj_in, const std::string& name_in ) {
   col_out.SETPOBJECT(name_in, Reg1d, &obj_in);
   
   return SUCCESS;
}
Errc PColSetObject( Collection &col_out, Reg2d &obj_in, const std::string& name_in ) {
   col_out.SETPOBJECT(name_in, Reg2d, &obj_in);
   
   return SUCCESS;
}
Errc PColSetObject( Collection &col_out, Reg3d &obj_in, const std::string& name_in ) {
   col_out.SETPOBJECT(name_in, Reg3d, &obj_in);
   
   return SUCCESS;
}
Errc PColSetObject( Collection &col_out, Graph2d &obj_in, const std::string& name_in ) {
   col_out.SETPOBJECT(name_in, Graph2d, &obj_in);
   
   return SUCCESS;
}
Errc PColSetObject( Collection &col_out, Graph3d &obj_in, const std::string& name_in ) {
   col_out.SETPOBJECT(name_in, Graph3d, &obj_in);
   
   return SUCCESS;
}
Errc PColSetObject( Collection &col_out, Imc2duc &obj_in, const std::string& name_in ) {
   col_out.SETPOBJECT(name_in, Imc2duc, &obj_in);
   
   return SUCCESS;
}
Errc PColSetObject( Collection &col_out, Imc2dsl &obj_in, const std::string& name_in ) {
   col_out.SETPOBJECT(name_in, Imc2dsl, &obj_in);
   
   return SUCCESS;
}
Errc PColSetObject( Collection &col_out, Imc2dsf &obj_in, const std::string& name_in ) {
   col_out.SETPOBJECT(name_in, Imc2dsf, &obj_in);
   
   return SUCCESS;
}
Errc PColSetObject( Collection &col_out, Imc3duc &obj_in, const std::string& name_in ) {
   col_out.SETPOBJECT(name_in, Imc3duc, &obj_in);
   
   return SUCCESS;
}
Errc PColSetObject( Collection &col_out, Imc3dsl &obj_in, const std::string& name_in ) {
   col_out.SETPOBJECT(name_in, Imc3dsl, &obj_in);
   
   return SUCCESS;
}
Errc PColSetObject( Collection &col_out, Imc3dsf &obj_in, const std::string& name_in ) {
   col_out.SETPOBJECT(name_in, Imc3dsf, &obj_in);
   
   return SUCCESS;
}
Errc PColSetObject( Collection &col_out, Imx1duc &obj_in, const std::string& name_in ) {
   col_out.SETPOBJECT(name_in, Imx1duc, &obj_in);
   
   return SUCCESS;
}
Errc PColSetObject( Collection &col_out, Imx1dsl &obj_in, const std::string& name_in ) {
   col_out.SETPOBJECT(name_in, Imx1dsl, &obj_in);
   
   return SUCCESS;
}
Errc PColSetObject( Collection &col_out, Imx1dsf &obj_in, const std::string& name_in ) {
   col_out.SETPOBJECT(name_in, Imx1dsf, &obj_in);
   
   return SUCCESS;
}
Errc PColSetObject( Collection &col_out, Imx2duc &obj_in, const std::string& name_in ) {
   col_out.SETPOBJECT(name_in, Imx2duc, &obj_in);
   
   return SUCCESS;
}
Errc PColSetObject( Collection &col_out, Imx2dsl &obj_in, const std::string& name_in ) {
   col_out.SETPOBJECT(name_in, Imx2dsl, &obj_in);
   
   return SUCCESS;
}
Errc PColSetObject( Collection &col_out, Imx2dsf &obj_in, const std::string& name_in ) {
   col_out.SETPOBJECT(name_in, Imx2dsf, &obj_in);
   
   return SUCCESS;
}
Errc PColSetObject( Collection &col_out, Imx3duc &obj_in, const std::string& name_in ) {
   col_out.SETPOBJECT(name_in, Imx3duc, &obj_in);
   
   return SUCCESS;
}
Errc PColSetObject( Collection &col_out, Imx3dsl &obj_in, const std::string& name_in ) {
   col_out.SETPOBJECT(name_in, Imx3dsl, &obj_in);
   
   return SUCCESS;
}
Errc PColSetObject( Collection &col_out, Imx3dsf &obj_in, const std::string& name_in ) {
   col_out.SETPOBJECT(name_in, Imx3dsf, &obj_in);
   
   return SUCCESS;
}
Errc PColSetObject( Collection &col_out, Point1d &obj_in, const std::string& name_in ) {
   col_out.SETPOBJECT(name_in, Point1d, &obj_in);
   
   return SUCCESS;
}
Errc PColSetObject( Collection &col_out, Point2d &obj_in, const std::string& name_in ) {
   col_out.SETPOBJECT(name_in, Point2d, &obj_in);
   
   return SUCCESS;
}
Errc PColSetObject( Collection &col_out, Point3d &obj_in, const std::string& name_in ) {
   col_out.SETPOBJECT(name_in, Point3d, &obj_in);
   
   return SUCCESS;
}
Errc PColSetObject( Collection &col_out, Dimension1d &obj_in, const std::string& name_in ) {
   col_out.SETPOBJECT(name_in, Dimension1d, &obj_in);
   
   return SUCCESS;
}
Errc PColSetObject( Collection &col_out, Dimension2d &obj_in, const std::string& name_in ) {
   col_out.SETPOBJECT(name_in, Dimension2d, &obj_in);
   
   return SUCCESS;
}
Errc PColSetObject( Collection &col_out, Dimension3d &obj_in, const std::string& name_in ) {
   col_out.SETPOBJECT(name_in, Dimension3d, &obj_in);
   
   return SUCCESS;
}

#ifdef MAIN
#define	USAGE	"usage: %s name [col_in|-] [obj_in|-] [col_out|-]"
#define	PARC	1
#define	FINC	2
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
   if (objs[0]->Type() == Po_Collection && objs[1]->Type() == Po_Collection ) {
      Collection* const col_in = (Collection*)objs[0];
      Collection* const obj_in = (Collection*)objs[1];
      objd[0] = col_in;
      result = PColSetObject(*col_in, *obj_in, parv[0]);
      goto end;	
   }
   if (objs[0]->Type() == Po_Collection && objs[1]->Type() == Po_Img1duc ) {
      Collection* const col_in = (Collection*)objs[0];
      Img1duc* const obj_in = (Img1duc*)objs[1];
      objd[0] = col_in;
      result = PColSetObject(*col_in, *obj_in, parv[0]);
      goto end;	
   }
   if (objs[0]->Type() == Po_Collection && objs[1]->Type() == Po_Img1dsl ) {
      Collection* const col_in = (Collection*)objs[0];
      Img1dsl* const obj_in = (Img1dsl*)objs[1];
      objd[0] = col_in;
      result = PColSetObject(*col_in, *obj_in, parv[0]);
      goto end;	
   }
   if (objs[0]->Type() == Po_Collection && objs[1]->Type() == Po_Img1dsf ) {
      Collection* const col_in = (Collection*)objs[0];
      Img1dsf* const obj_in = (Img1dsf*)objs[1];
      objd[0] = col_in;
      result = PColSetObject(*col_in, *obj_in, parv[0]);
      goto end;	
   }
   if (objs[0]->Type() == Po_Collection && objs[1]->Type() == Po_Img2duc ) {
      Collection* const col_in = (Collection*)objs[0];
      Img2duc* const obj_in = (Img2duc*)objs[1];
      objd[0] = col_in;
      result = PColSetObject(*col_in, *obj_in, parv[0]);
      goto end;	
   }
   if (objs[0]->Type() == Po_Collection && objs[1]->Type() == Po_Img2dsl ) {
      Collection* const col_in = (Collection*)objs[0];
      Img2dsl* const obj_in = (Img2dsl*)objs[1];
      objd[0] = col_in;
      result = PColSetObject(*col_in, *obj_in, parv[0]);
      goto end;	
   }
   if (objs[0]->Type() == Po_Collection && objs[1]->Type() == Po_Img2dsf ) {
      Collection* const col_in = (Collection*)objs[0];
      Img2dsf* const obj_in = (Img2dsf*)objs[1];
      objd[0] = col_in;
      result = PColSetObject(*col_in, *obj_in, parv[0]);
      goto end;	
   }
   if (objs[0]->Type() == Po_Collection && objs[1]->Type() == Po_Img3duc ) {
      Collection* const col_in = (Collection*)objs[0];
      Img3duc* const obj_in = (Img3duc*)objs[1];
      objd[0] = col_in;
      result = PColSetObject(*col_in, *obj_in, parv[0]);
      goto end;	
   }
   if (objs[0]->Type() == Po_Collection && objs[1]->Type() == Po_Img3dsl ) {
      Collection* const col_in = (Collection*)objs[0];
      Img3dsl* const obj_in = (Img3dsl*)objs[1];
      objd[0] = col_in;
      result = PColSetObject(*col_in, *obj_in, parv[0]);
      goto end;	
   }
   if (objs[0]->Type() == Po_Collection && objs[1]->Type() == Po_Img3dsf ) {
      Collection* const col_in = (Collection*)objs[0];
      Img3dsf* const obj_in = (Img3dsf*)objs[1];
      objd[0] = col_in;
      result = PColSetObject(*col_in, *obj_in, parv[0]);
      goto end;	
   }
   if (objs[0]->Type() == Po_Collection && objs[1]->Type() == Po_Reg1d ) {
      Collection* const col_in = (Collection*)objs[0];
      Reg1d* const obj_in = (Reg1d*)objs[1];
      objd[0] = col_in;
      result = PColSetObject(*col_in, *obj_in, parv[0]);
      goto end;	
   }
   if (objs[0]->Type() == Po_Collection && objs[1]->Type() == Po_Reg2d ) {
      Collection* const col_in = (Collection*)objs[0];
      Reg2d* const obj_in = (Reg2d*)objs[1];
      objd[0] = col_in;
      result = PColSetObject(*col_in, *obj_in, parv[0]);
      goto end;	
   }
   if (objs[0]->Type() == Po_Collection && objs[1]->Type() == Po_Reg3d ) {
      Collection* const col_in = (Collection*)objs[0];
      Reg3d* const obj_in = (Reg3d*)objs[1];
      objd[0] = col_in;
      result = PColSetObject(*col_in, *obj_in, parv[0]);
      goto end;	
   }
   if (objs[0]->Type() == Po_Collection && objs[1]->Type() == Po_Graph2d ) {
      Collection* const col_in = (Collection*)objs[0];
      Graph2d* const obj_in = (Graph2d*)objs[1];
      objd[0] = col_in;
      result = PColSetObject(*col_in, *obj_in, parv[0]);
      goto end;	
   }
   if (objs[0]->Type() == Po_Collection && objs[1]->Type() == Po_Graph3d ) {
      Collection* const col_in = (Collection*)objs[0];
      Graph3d* const obj_in = (Graph3d*)objs[1];
      objd[0] = col_in;
      result = PColSetObject(*col_in, *obj_in, parv[0]);
      goto end;	
   }
   if (objs[0]->Type() == Po_Collection && objs[1]->Type() == Po_Imc2duc ) {
      Collection* const col_in = (Collection*)objs[0];
      Imc2duc* const obj_in = (Imc2duc*)objs[1];
      objd[0] = col_in;
      result = PColSetObject(*col_in, *obj_in, parv[0]);
      goto end;	
   }
   if (objs[0]->Type() == Po_Collection && objs[1]->Type() == Po_Imc2dsl ) {
      Collection* const col_in = (Collection*)objs[0];
      Imc2dsl* const obj_in = (Imc2dsl*)objs[1];
      objd[0] = col_in;
      result = PColSetObject(*col_in, *obj_in, parv[0]);
      goto end;	
   }
   if (objs[0]->Type() == Po_Collection && objs[1]->Type() == Po_Imc2dsf ) {
      Collection* const col_in = (Collection*)objs[0];
      Imc2dsf* const obj_in = (Imc2dsf*)objs[1];
      objd[0] = col_in;
      result = PColSetObject(*col_in, *obj_in, parv[0]);
      goto end;	
   }
   if (objs[0]->Type() == Po_Collection && objs[1]->Type() == Po_Imc3duc ) {
      Collection* const col_in = (Collection*)objs[0];
      Imc3duc* const obj_in = (Imc3duc*)objs[1];
      objd[0] = col_in;
      result = PColSetObject(*col_in, *obj_in, parv[0]);
      goto end;	
   }
   if (objs[0]->Type() == Po_Collection && objs[1]->Type() == Po_Imc3dsl ) {
      Collection* const col_in = (Collection*)objs[0];
      Imc3dsl* const obj_in = (Imc3dsl*)objs[1];
      objd[0] = col_in;
      result = PColSetObject(*col_in, *obj_in, parv[0]);
      goto end;	
   }
   if (objs[0]->Type() == Po_Collection && objs[1]->Type() == Po_Imc3dsf ) {
      Collection* const col_in = (Collection*)objs[0];
      Imc3dsf* const obj_in = (Imc3dsf*)objs[1];
      objd[0] = col_in;
      result = PColSetObject(*col_in, *obj_in, parv[0]);
      goto end;	
   }
   if (objs[0]->Type() == Po_Collection && objs[1]->Type() == Po_Imx1duc ) {
      Collection* const col_in = (Collection*)objs[0];
      Imx1duc* const obj_in = (Imx1duc*)objs[1];
      objd[0] = col_in;
      result = PColSetObject(*col_in, *obj_in, parv[0]);
      goto end;	
   }
   if (objs[0]->Type() == Po_Collection && objs[1]->Type() == Po_Imx1dsl ) {
      Collection* const col_in = (Collection*)objs[0];
      Imx1dsl* const obj_in = (Imx1dsl*)objs[1];
      objd[0] = col_in;
      result = PColSetObject(*col_in, *obj_in, parv[0]);
      goto end;	
   }
   if (objs[0]->Type() == Po_Collection && objs[1]->Type() == Po_Imx1dsf ) {
      Collection* const col_in = (Collection*)objs[0];
      Imx1dsf* const obj_in = (Imx1dsf*)objs[1];
      objd[0] = col_in;
      result = PColSetObject(*col_in, *obj_in, parv[0]);
      goto end;	
   }
   if (objs[0]->Type() == Po_Collection && objs[1]->Type() == Po_Imx2duc ) {
      Collection* const col_in = (Collection*)objs[0];
      Imx2duc* const obj_in = (Imx2duc*)objs[1];
      objd[0] = col_in;
      result = PColSetObject(*col_in, *obj_in, parv[0]);
      goto end;	
   }
   if (objs[0]->Type() == Po_Collection && objs[1]->Type() == Po_Imx2dsl ) {
      Collection* const col_in = (Collection*)objs[0];
      Imx2dsl* const obj_in = (Imx2dsl*)objs[1];
      objd[0] = col_in;
      result = PColSetObject(*col_in, *obj_in, parv[0]);
      goto end;	
   }
   if (objs[0]->Type() == Po_Collection && objs[1]->Type() == Po_Imx2dsf ) {
      Collection* const col_in = (Collection*)objs[0];
      Imx2dsf* const obj_in = (Imx2dsf*)objs[1];
      objd[0] = col_in;
      result = PColSetObject(*col_in, *obj_in, parv[0]);
      goto end;	
   }
   if (objs[0]->Type() == Po_Collection && objs[1]->Type() == Po_Imx3duc ) {
      Collection* const col_in = (Collection*)objs[0];
      Imx3duc* const obj_in = (Imx3duc*)objs[1];
      objd[0] = col_in;
      result = PColSetObject(*col_in, *obj_in, parv[0]);
      goto end;	
   }
   if (objs[0]->Type() == Po_Collection && objs[1]->Type() == Po_Imx3dsl ) {
      Collection* const col_in = (Collection*)objs[0];
      Imx3dsl* const obj_in = (Imx3dsl*)objs[1];
      objd[0] = col_in;
      result = PColSetObject(*col_in, *obj_in, parv[0]);
      goto end;	
   }
   if (objs[0]->Type() == Po_Collection && objs[1]->Type() == Po_Imx3dsf ) {
      Collection* const col_in = (Collection*)objs[0];
      Imx3dsf* const obj_in = (Imx3dsf*)objs[1];
      objd[0] = col_in;
      result = PColSetObject(*col_in, *obj_in, parv[0]);
      goto end;	
   }
   if (objs[0]->Type() == Po_Collection && objs[1]->Type() == Po_Point1d ) {
      Collection* const col_in = (Collection*)objs[0];
      Point1d* const obj_in = (Point1d*)objs[1];
      objd[0] = col_in;
      result = PColSetObject(*col_in, *obj_in, parv[0]);
      goto end;	
   }
   if (objs[0]->Type() == Po_Collection && objs[1]->Type() == Po_Point2d ) {
      Collection* const col_in = (Collection*)objs[0];
      Point2d* const obj_in = (Point2d*)objs[1];
      objd[0] = col_in;
      result = PColSetObject(*col_in, *obj_in, parv[0]);
      goto end;	
   }
   if (objs[0]->Type() == Po_Collection && objs[1]->Type() == Po_Point3d ) {
      Collection* const col_in = (Collection*)objs[0];
      Point3d* const obj_in = (Point3d*)objs[1];
      objd[0] = col_in;
      result = PColSetObject(*col_in, *obj_in, parv[0]);
      goto end;	
   }
   if (objs[0]->Type() == Po_Collection && objs[1]->Type() == Po_Dimension1d ) {
      Collection* const col_in = (Collection*)objs[0];
      Dimension1d* const obj_in = (Dimension1d*)objs[1];
      objd[0] = col_in;
      result = PColSetObject(*col_in, *obj_in, parv[0]);
      goto end;	
   }
   if (objs[0]->Type() == Po_Collection && objs[1]->Type() == Po_Dimension2d ) {
      Collection* const col_in = (Collection*)objs[0];
      Dimension2d* const obj_in = (Dimension2d*)objs[1];
      objd[0] = col_in;
      result = PColSetObject(*col_in, *obj_in, parv[0]);
      goto end;	
   }
   if (objs[0]->Type() == Po_Collection && objs[1]->Type() == Po_Dimension3d ) {
      Collection* const col_in = (Collection*)objs[0];
      Dimension3d* const obj_in = (Dimension3d*)objs[1];
      objd[0] = col_in;
      result = PColSetObject(*col_in, *obj_in, parv[0]);
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
