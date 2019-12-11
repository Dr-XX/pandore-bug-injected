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
 * @author Régis Clouard - 2001-05-18
 * @author Régis Clouard - 2003-03-31
 * @author Régis Clouard - 2005-01-10 (+ 1d)
 */

#include <string.h>
#include <iostream>
#include <fstream>
#include <pandore.h>
using namespace pandore;

/**
 * @file ppan2txt.cpp
 * Creation a text file with non nul pixels of image.
 */



Errc PPan2Txt( const Img1duc &ims, char *filename ) {
   Point1d p;
   std::ostream *fp;
   
   if (!filename || !strcmp(filename, "-")) {
      fp = &std::cout;
   } else {
      fp = new std::ofstream(filename);
   }
   
   if (filename && (!fp->good())) {
      std::cerr << "Error ppan2txt: cannot read into file: " << filename << std::endl;
      return FAILURE;
   }

   for (p.x = 0; p.x < ims.Width(); p.x++)
   if (ims[p]) {
      *fp << (float)ims[p] << " " << p.x << std::endl;
   }
   
   return SUCCESS;
}

Errc PPan2Txt( const Img1dsl &ims, char *filename ) {
   Point1d p;
   std::ostream *fp;
   
   if (!filename || !strcmp(filename, "-")) {
      fp = &std::cout;
   } else {
      fp = new std::ofstream(filename);
   }
   
   if (filename && (!fp->good())) {
      std::cerr << "Error ppan2txt: cannot read into file: " << filename << std::endl;
      return FAILURE;
   }

   for (p.x = 0; p.x < ims.Width(); p.x++)
   if (ims[p]) {
      *fp << (float)ims[p] << " " << p.x << std::endl;
   }
   
   return SUCCESS;
}

Errc PPan2Txt( const Img1dsf &ims, char *filename ) {
   Point1d p;
   std::ostream *fp;
   
   if (!filename || !strcmp(filename, "-")) {
      fp = &std::cout;
   } else {
      fp = new std::ofstream(filename);
   }
   
   if (filename && (!fp->good())) {
      std::cerr << "Error ppan2txt: cannot read into file: " << filename << std::endl;
      return FAILURE;
   }

   for (p.x = 0; p.x < ims.Width(); p.x++)
   if (ims[p]) {
      *fp << (float)ims[p] << " " << p.x << std::endl;
   }
   
   return SUCCESS;
}

Errc PPan2Txt( const Img2duc &ims, char *filename ) {
   Point2d p;
   std::ostream *fp;
   
   if (!filename || !strcmp(filename, "-")) {
      fp = &std::cout;
   } else {
      fp = new std::ofstream(filename);
   }
   
   if (filename && (!fp->good())) {
      std::cerr << "Error ppan2txt: cannot read into file: " << filename << std::endl;
      return FAILURE;
   }


   for (p.y = 0; p.y < ims.Height(); p.y++)
   for (p.x = 0; p.x < ims.Width(); p.x++)
   if (ims[p]) {
      *fp << (float)ims[p] << " " << p.x << " " << p.y << std::endl;
   }
   
   return SUCCESS;
}

Errc PPan2Txt( const Img2dsl &ims, char *filename ) {
   Point2d p;
   std::ostream *fp;
   
   if (!filename || !strcmp(filename, "-")) {
      fp = &std::cout;
   } else {
      fp = new std::ofstream(filename);
   }
   
   if (filename && (!fp->good())) {
      std::cerr << "Error ppan2txt: cannot read into file: " << filename << std::endl;
      return FAILURE;
   }


   for (p.y = 0; p.y < ims.Height(); p.y++)
   for (p.x = 0; p.x < ims.Width(); p.x++)
   if (ims[p]) {
      *fp << (float)ims[p] << " " << p.x << " " << p.y << std::endl;
   }
   
   return SUCCESS;
}

Errc PPan2Txt( const Img2dsf &ims, char *filename ) {
   Point2d p;
   std::ostream *fp;
   
   if (!filename || !strcmp(filename, "-")) {
      fp = &std::cout;
   } else {
      fp = new std::ofstream(filename);
   }
   
   if (filename && (!fp->good())) {
      std::cerr << "Error ppan2txt: cannot read into file: " << filename << std::endl;
      return FAILURE;
   }


   for (p.y = 0; p.y < ims.Height(); p.y++)
   for (p.x = 0; p.x < ims.Width(); p.x++)
   if (ims[p]) {
      *fp << (float)ims[p] << " " << p.x << " " << p.y << std::endl;
   }
   
   return SUCCESS;
}

Errc PPan2Txt( const Reg2d &ims, char *filename ) {
   Point2d p;
   std::ostream *fp;
   
   if (!filename || !strcmp(filename, "-")) {
      fp = &std::cout;
   } else {
      fp = new std::ofstream(filename);
   }
   
   if (filename && (!fp->good())) {
      std::cerr << "Error ppan2txt: cannot read into file: " << filename << std::endl;
      return FAILURE;
   }


   for (p.y = 0; p.y < ims.Height(); p.y++)
   for (p.x = 0; p.x < ims.Width(); p.x++)
   if (ims[p]) {
      *fp << (float)ims[p] << " " << p.x << " " << p.y << std::endl;
   }
   
   return SUCCESS;
}

Errc PPan2Txt( const Img3duc &ims, char *filename ) {
   Point3d p;
   std::ostream *fp;
   
   if (!filename || !strcmp(filename, "-")) {
      fp = &std::cout;
   } else {
      fp = new std::ofstream(filename);
   }
   
   if (filename && (!fp->good())) {
      std::cerr << "Error ppan2txt: cannot read into file: " <<filename<< std::endl;
      return FAILURE;
   }

   for (p.z = 0; p.z < ims.Depth(); p.z++)
   for (p.y = 0; p.y < ims.Height(); p.y++)
   for (p.x = 0; p.x < ims.Width(); p.x++)
   if (ims[p]) {
      *fp << (float)ims[p] << " " << p.x << " " << p.y <<" " << p.z << std::endl;
   }

   return SUCCESS;
}

Errc PPan2Txt( const Img3dsl &ims, char *filename ) {
   Point3d p;
   std::ostream *fp;
   
   if (!filename || !strcmp(filename, "-")) {
      fp = &std::cout;
   } else {
      fp = new std::ofstream(filename);
   }
   
   if (filename && (!fp->good())) {
      std::cerr << "Error ppan2txt: cannot read into file: " <<filename<< std::endl;
      return FAILURE;
   }

   for (p.z = 0; p.z < ims.Depth(); p.z++)
   for (p.y = 0; p.y < ims.Height(); p.y++)
   for (p.x = 0; p.x < ims.Width(); p.x++)
   if (ims[p]) {
      *fp << (float)ims[p] << " " << p.x << " " << p.y <<" " << p.z << std::endl;
   }

   return SUCCESS;
}

Errc PPan2Txt( const Img3dsf &ims, char *filename ) {
   Point3d p;
   std::ostream *fp;
   
   if (!filename || !strcmp(filename, "-")) {
      fp = &std::cout;
   } else {
      fp = new std::ofstream(filename);
   }
   
   if (filename && (!fp->good())) {
      std::cerr << "Error ppan2txt: cannot read into file: " <<filename<< std::endl;
      return FAILURE;
   }

   for (p.z = 0; p.z < ims.Depth(); p.z++)
   for (p.y = 0; p.y < ims.Height(); p.y++)
   for (p.x = 0; p.x < ims.Width(); p.x++)
   if (ims[p]) {
      *fp << (float)ims[p] << " " << p.x << " " << p.y <<" " << p.z << std::endl;
   }

   return SUCCESS;
}

Errc PPan2Txt( const Reg3d &ims, char *filename ) {
   Point3d p;
   std::ostream *fp;
   
   if (!filename || !strcmp(filename, "-")) {
      fp = &std::cout;
   } else {
      fp = new std::ofstream(filename);
   }
   
   if (filename && (!fp->good())) {
      std::cerr << "Error ppan2txt: cannot read into file: " <<filename<< std::endl;
      return FAILURE;
   }

   for (p.z = 0; p.z < ims.Depth(); p.z++)
   for (p.y = 0; p.y < ims.Height(); p.y++)
   for (p.x = 0; p.x < ims.Width(); p.x++)
   if (ims[p]) {
      *fp << (float)ims[p] << " " << p.x << " " << p.y <<" " << p.z << std::endl;
   }

   return SUCCESS;
}


#ifdef MAIN
#define	USAGE	"usage: %s [im_in|-] [file_out|-]"
#define	PARC	0
#define	FINC	1
#define	FOUTC	0
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
   if (objs[0]->Type() == Po_Img1duc) {
      Img1duc* const ims = (Img1duc*)objs[0];
   
      result = PPan2Txt(*ims, (argc > 2) ? argv[2] : NULL);
      goto end;
   }
   if (objs[0]->Type() == Po_Img1dsl) {
      Img1dsl* const ims = (Img1dsl*)objs[0];
   
      result = PPan2Txt(*ims, (argc > 2) ? argv[2] : NULL);
      goto end;
   }
   if (objs[0]->Type() == Po_Img1dsf) {
      Img1dsf* const ims = (Img1dsf*)objs[0];
   
      result = PPan2Txt(*ims, (argc > 2) ? argv[2] : NULL);
      goto end;
   }
if (objs[0]->Type() == Po_Img2duc) {
   Img2duc* const ims = (Img2duc*)objs[0];
   
   result = PPan2Txt(*ims, (argc > 2) ? argv[2] : NULL);
   goto end;
}
if (objs[0]->Type() == Po_Img2dsl) {
   Img2dsl* const ims = (Img2dsl*)objs[0];
   
   result = PPan2Txt(*ims, (argc > 2) ? argv[2] : NULL);
   goto end;
}
if (objs[0]->Type() == Po_Img2dsf) {
   Img2dsf* const ims = (Img2dsf*)objs[0];
   
   result = PPan2Txt(*ims, (argc > 2) ? argv[2] : NULL);
   goto end;
}
if (objs[0]->Type() == Po_Reg2d) {
   Reg2d* const ims = (Reg2d*)objs[0];
   
   result = PPan2Txt(*ims, (argc > 2) ? argv[2] : NULL);
   goto end;
}
   if (objs[0]->Type() == Po_Img3duc) {
      Img3duc* const ims=(Img3duc*)objs[0];
      
      result = PPan2Txt(*ims, (argc > 2) ? argv[2] : NULL);
      goto end;
   }
   if (objs[0]->Type() == Po_Img3dsl) {
      Img3dsl* const ims=(Img3dsl*)objs[0];
      
      result = PPan2Txt(*ims, (argc > 2) ? argv[2] : NULL);
      goto end;
   }
   if (objs[0]->Type() == Po_Img3dsf) {
      Img3dsf* const ims=(Img3dsf*)objs[0];
      
      result = PPan2Txt(*ims, (argc > 2) ? argv[2] : NULL);
      goto end;
   }
   if (objs[0]->Type() == Po_Reg3d) {
      Reg3d* const ims=(Reg3d*)objs[0];
      
      result = PPan2Txt(*ims, (argc > 2) ? argv[2] : NULL);
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
