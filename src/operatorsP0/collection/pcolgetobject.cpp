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
 * @author Alexandre Duret-Lutz - 1999-10-08
 */

#include <pandore.h>
using namespace pandore;

/**
 * @file pcolgetobject.cpp
 * Extract a Pandore object from a collection.
 */

Errc PColGetObject( Collection &c_in, Pobject * &out, const std::string &name ) {
   std::string type = c_in.GetType(name);
   if (type == "Pobject:Collection")
      out = c_in.GETPOBJECT(name, Collection);
   else
   if (type == "Pobject:Img1duc")
      out = c_in.GETPOBJECT(name, Img1duc);
   else
   if (type == "Pobject:Img1dsl")
      out = c_in.GETPOBJECT(name, Img1dsl);
   else
   if (type == "Pobject:Img1dsf")
      out = c_in.GETPOBJECT(name, Img1dsf);
   else
   if (type == "Pobject:Img2duc")
      out = c_in.GETPOBJECT(name, Img2duc);
   else
   if (type == "Pobject:Img2dsl")
      out = c_in.GETPOBJECT(name, Img2dsl);
   else
   if (type == "Pobject:Img2dsf")
      out = c_in.GETPOBJECT(name, Img2dsf);
   else
   if (type == "Pobject:Img3duc")
      out = c_in.GETPOBJECT(name, Img3duc);
   else
   if (type == "Pobject:Img3dsl")
      out = c_in.GETPOBJECT(name, Img3dsl);
   else
   if (type == "Pobject:Img3dsf")
      out = c_in.GETPOBJECT(name, Img3dsf);
   else
   if (type == "Pobject:Reg1d")
      out = c_in.GETPOBJECT(name, Reg1d);
   else
   if (type == "Pobject:Reg2d")
      out = c_in.GETPOBJECT(name, Reg2d);
   else
   if (type == "Pobject:Reg3d")
      out = c_in.GETPOBJECT(name, Reg3d);
   else
   if (type == "Pobject:Graph2d")
      out = c_in.GETPOBJECT(name, Graph2d);
   else
   if (type == "Pobject:Graph3d")
      out = c_in.GETPOBJECT(name, Graph3d);
   else
   if (type == "Pobject:Imc2duc")
      out = c_in.GETPOBJECT(name, Imc2duc);
   else
   if (type == "Pobject:Imc2dsl")
      out = c_in.GETPOBJECT(name, Imc2dsl);
   else
   if (type == "Pobject:Imc2dsf")
      out = c_in.GETPOBJECT(name, Imc2dsf);
   else
   if (type == "Pobject:Imc3duc")
      out = c_in.GETPOBJECT(name, Imc3duc);
   else
   if (type == "Pobject:Imc3dsl")
      out = c_in.GETPOBJECT(name, Imc3dsl);
   else
   if (type == "Pobject:Imc3dsf")
      out = c_in.GETPOBJECT(name, Imc3dsf);
   else
   if (type == "Pobject:Imx1duc")
      out = c_in.GETPOBJECT(name, Imx1duc);
   else
   if (type == "Pobject:Imx1dsl")
      out = c_in.GETPOBJECT(name, Imx1dsl);
   else
   if (type == "Pobject:Imx1dsf")
      out = c_in.GETPOBJECT(name, Imx1dsf);
   else
   if (type == "Pobject:Imx2duc")
      out = c_in.GETPOBJECT(name, Imx2duc);
   else
   if (type == "Pobject:Imx2dsl")
      out = c_in.GETPOBJECT(name, Imx2dsl);
   else
   if (type == "Pobject:Imx2dsf")
      out = c_in.GETPOBJECT(name, Imx2dsf);
   else
   if (type == "Pobject:Imx3duc")
      out = c_in.GETPOBJECT(name, Imx3duc);
   else
   if (type == "Pobject:Imx3dsl")
      out = c_in.GETPOBJECT(name, Imx3dsl);
   else
   if (type == "Pobject:Imx3dsf")
      out = c_in.GETPOBJECT(name, Imx3dsf);
   else
   if (type == "Pobject:Point1d")
      out = c_in.GETPOBJECT(name, Point1d);
   else
   if (type == "Pobject:Point2d")
      out = c_in.GETPOBJECT(name, Point2d);
   else
   if (type == "Pobject:Point3d")
      out = c_in.GETPOBJECT(name, Point3d);
   else
   if (type == "Pobject:Dimension1d")
      out = c_in.GETPOBJECT(name, Dimension1d);
   else
   if (type == "Pobject:Dimension2d")
      out = c_in.GETPOBJECT(name, Dimension2d);
   else
   if (type == "Pobject:Dimension3d")
      out = c_in.GETPOBJECT(name, Dimension3d);
   else
      return FAILURE;
   return SUCCESS;
}

#ifdef MAIN

/*
 * Modify only the following constants, and the function call.
 */
#define	USAGE	"usage: %s name [col_in|-] [im_out|-]"
#define	PARC	1
#define	FINC	1
#define	FOUTC	1
#define	MASK	0

int main( int argc, char *argv[] ) {
   Errc result;                // The result code of the execution.
   Pobject* mask;              // The region mask.
   Pobject* objin[FINC + 1];   // The input objects;
   Pobject* objs[FINC + 1];    // The source objects masked by the mask.
   Pobject* objout[FOUTC + 1]; // The output object.
   Pobject* objd[FOUTC + 1];   // The result object of the execution.
   char* parv[PARC + 1];       // The input parameters.
   
   ReadArgs(argc, argv, PARC, FINC, FOUTC, &mask, objin, objs, objout, objd, parv, USAGE, MASK);

   if (objs[0]->Type() == Po_Collection) {
      Collection* ims1 = (Collection*)objs[0];
      result = PColGetObject(*ims1, objd[0], parv[0]);
   } else {
      PrintErrorFormat(objin, FINC);
      result = FAILURE;
   }
   
   if (result) {
      WriteArgs(argc, argv, PARC, FINC, FOUTC, &mask, objin, objs, objout, objd, MASK);
   }
   Exit(result);
   return 0;
}

#endif
