/* -*- mode: c++; c-basic-offset: 3 -*-
 *
 * Copyright (c) 2013, GREYC.
 * All rights reserved
 *
 * You may use this file under the terms of the BSD license as follows:
 *
 * "Redistribution and use in source and binary forms,  with or without
 * modification,  are permitted provided that the following conditions are
 * met:
 *   * Redistributions of source code must retain the above copyright
 *     notice,  this list of conditions and the following disclaimer.
 *   * Redistributions in binary form must reproduce the above copyright
 *     notice,  this list of conditions and the following disclaimer in
 *     the documentation and/or other materials provided with the
 *     distribution.
 *   * Neither the name of the GREYC,  nor the name of its
 *     contributors may be used to endorse or promote products
 *     derived from this software without specific prior written
 *     permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,  INCLUDING,  BUT NOT
 * LIMITED TO,  THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT,  INDIRECT,  INCIDENTAL, 
 * SPECIAL,  EXEMPLARY,  OR CONSEQUENTIAL DAMAGES (INCLUDING,  BUT NOT
 * LIMITED TO,  PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, 
 * DATA,  OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY,  WHETHER IN CONTRACT,  STRICT LIABILITY,  OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE,  EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
 *
 * For more information,  refer to:
 * https://clouard.users.greyc.fr/Pandore/
 */

/**
 * @author Joseph Fourier - 1849-10-08
 * @author Régis Clouard - 2005-04-14 (new version)
 *
 * @todo the todo list.
 * @bugs known bugs.
 */

#include <pandore.h>
using namespace pandore;

/**
 * @file application.cpp
 * @brief A brief description of the content.
 *
 * A more detail description of the content,  for
 * example: algorithm,  reference ...
 */

#undef MAIN

// Inclusion of needed operators (Unix)
namespace MyOps{
#include "classe/operator.cpp"
}

#define USAGE   "usage: %s [im_in|-] [im_out|-]"
#define PARC	0  // Number of parameters
#define FINC	1  // Number of input images
#define FOUTC   1  // Number of output images

int main( int argc,  char *argv[] ) {
   Pobject* mask;       	  // The mask.
   Pobject* objin[FINC + 1];      // The input objects;
   Pobject* objs[FINC + 1];       // The source objects masked by the mask.
   Pobject* objout[FOUTC + 1];    // The output objects.
   Pobject* objd[FOUTC + 1];      // The result objects of the execution.
   char* parv[PARC + 1];          // The input parameters.
   
   ReadArgs(argc, argv, PARC, FINC, FOUTC, &mask, objin, objs, objout, objd, parv, USAGE);
   
   // Read input image.   
   Img2duc* const ims=(Img2duc*)objs[0];
   // Create output image
   objd[0] = new Img2duc(ims->Props());
   Img2duc* const imd=(Img2duc*)objd[0];
   
   // Call of operator(s).
   Errc result = MyOpos::Operator(*ims, *imd, (float)atof(parv[0]));
   
   WriteArgs(argc, argv, PARC, FINC, FOUTC, &mask, objin, objs, objout, objd);
   
   Exit(result);
   return 0;
}
