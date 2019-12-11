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
 * @author Régis Clouard - 1997-07-22
 * @author Regis Clouard - 2001-04-11
 */

#if (defined _MSC_VER) && (!defined snprintf)
#define snprintf _snprintf
#endif

#include <string.h>
#include <pandore.h>
using namespace pandore;

/**
 * @file pproperty.cpp
 * Return a given property value of a Pandore object.
 */

template <typename T>
Errc PProperty( const Img1d<T> &img, int numattr ) {
   switch (numattr) {
   case 0 : return img.Width();
   case 1 : return 0;
   case 2 : return 0;
   case 3 : return img.Bands();
   case 4 : return -1;
   default: break;
   }
   std::cerr << "Error pproperty: Bad property index: " << numattr << std::endl;
   return FAILURE;
}

template <typename T>
Errc PProperty( const Imx1d<T> &img, int numattr ) {
   switch (numattr) {
   case 0 : return img.Width();
   case 1 : return 0;
   case 2 : return 0;
   case 3 : return img.Bands();
   case 4 : return -1;
   default: break;
   }
   std::cerr << "Error pproperty: Bad property index: " << numattr << std::endl;
   return FAILURE;
}

template <typename T>
Errc PProperty( const Img2d<T> &img, int numattr ) {
   switch (numattr) {
   case 0 : return img.Width();
   case 1 : return img.Height();
   case 2 : return 0;
   case 3 : return img.Bands();
   case 4 : return -1;
   default: break;
   }
   std::cerr << "Error pproperty: Bad property index: " << numattr << std::endl;
   return FAILURE;
}

template <typename T>
Errc PProperty( const Imc2d<T> &img, int numattr ) {
   switch (numattr) {
   case 0 : return img.Width();
   case 1 : return img.Height();
   case 2 : return 0;
   case 3 : return img.Bands();
   case 4 : return (Long)img.ColorSpace();
   default: break;
   }
   std::cerr << "Error pproperty: Bad property index: " << numattr << std::endl;
   return FAILURE;
}

template <typename T>
Errc PProperty( const Imx2d<T> &img, int numattr ) {
   switch (numattr) {
   case 0 : return img.Width();
   case 1 : return img.Height();
   case 2 : return 0;
   case 3 : return img.Bands();
   case 4 : return -1;
   default: break;
   }
   std::cerr << "Error pproperty: Bad property index: " << numattr << std::endl;
   return FAILURE;
}

/**
 * 3D.
 */
template <typename T>
Errc PProperty( const Img3d<T> &img, int numattr ) {
   switch (numattr) {
   case 0 : return img.Width();
   case 1 : return img.Height();
   case 2 : return img.Depth();
   case 3 : return img.Bands();
   case 4 : return -1;
   default: break;
   }
   std::cerr << "Error pproperty: Bad property index: " << numattr << std::endl;
   return FAILURE;
}

template <typename T>
Errc PProperty( const Imc3d<T> &img, int numattr ) {
   switch (numattr) {
   case 0 : return img.Width();
   case 1 : return img.Height();
   case 2 : return img.Depth();
   case 3 : return img.Bands();
   case 4 : return (Long)img.ColorSpace();
   default: break;
   }
   std::cerr << "Error pproperty: Bad property index: " << numattr << std::endl;
   return FAILURE;
}

/**
 * 3D.
 */
template <typename T>
Errc PProperty( const Imx3d<T> &img, int numattr ) {
   switch (numattr) {
   case 0 : return img.Width();
   case 1 : return img.Height();
   case 2 : return img.Depth();
   case 3 : return img.Bands();
   case 4 : return -1;
   default: break;
   }
   std::cerr << "Error pproperty: Bad property index: " << numattr << std::endl;
   return FAILURE;
}

Errc PProperty( const Reg2d &img, int numattr ) {
   switch (numattr) {
   case 0 : return img.Width();
   case 1 : return img.Height();
   case 2 : return 0;
   case 3 : return img.Labels();
   case 4 : return -1;
   default: break;
   }
   std::cerr << "Error pproperty: Bad property index: " << numattr << std::endl;
   return FAILURE;
}

Errc PProperty( const Reg3d &img, int numattr ) {
   switch (numattr) {
   case 0 : return img.Width();
   case 1 : return img.Height();
   case 2 : return img.Depth();
   case 3 : return img.Labels();
   case 4 : return -1;
   default: break;
   }
   std::cerr << "Error pproperty: Bad property index: " << numattr << std::endl;
   return FAILURE;
}


Errc PProperty( const Graph2d &grs, int numattr ) {
   switch (numattr) {
   case 0 : return grs.Width();
   case 1 : return grs.Height();
   case 2 : return 0;
   case 3 : return grs.Size();
   case 4 : return -1;
   default: break;
   }
   std::cerr << "Error pproperty: Bad property index: " << numattr << std::endl;
   return FAILURE;
}

Errc PProperty( const Graph3d &grs, int numattr ) {
   switch (numattr) {
   case 0 : return grs.Width();
   case 1 : return grs.Height();
   case 2 : return grs.Depth();
   case 3 : return grs.Size();
   case 4 : return -1;
   default: break;
   }
   std::cerr << "Error pproperty: Bad property index: " << numattr << std::endl;
   return FAILURE;
}

Pobject *LoadFileHeader( const char *filename ) {
   FILE *fd;
   Errc result;
   Po_header headfile;
   int ver;
   bool invertBytes;

   if (!(fd = pandore::Pfopen(filename, PLOAD))) {
      return NULL;
   }
   headfile = ReadHeader(fd, ver, invertBytes);
   Pobject *obj = NewObject(headfile.Type());

   if (obj != NULL) {
      obj->setInversionMode(invertBytes);
      result =  obj->LoadAttributes(fd);
   } else {
      result = false;
   }

   if (!result) {
      if ((filename)) {
	 std::cerr << "Error: bad Pandore format : " << filename << std::endl;
      } else {
	 std::cerr << "Error: bad Pandore format : standard input" << std::endl;
      }
   }
   if (filename != NULL) {
      fclose(fd);
   }

   return (result == SUCCESS) ? obj : NULL;
}


#ifdef MAIN
#define USAGE	"usage: %s property_index [im_in|-]"
#define PARC	1
#define FINC	1
#define FOUTC	0
#define MASK	0

int main( int argc, char *argv[] ) {
   Errc result;                // The result code of the execution.
   Pobject* mask;              // The region map.
   Pobject* objin[FINC + 1];   // The input objects.
   Pobject* objs[FINC + 1];    // The source objects masked.
   Pobject* objout[FOUTC + 1]; // The output object.
   Pobject* objd[FOUTC + 1];   // The result object of the execution.
   char* parv[PARC + 1];       // The input parameters.
   char error[256];

   // -s: name of the parameters.
   if (argc >= 2 && !strcmp(argv[1], "-s")) {
      strncpy(error, USAGE, sizeof(error));
      char * p = error;
      while (*p && (*p != '%'|| *(p + 1) != 's')) {
	 p++;
      }
      if (*p) {
	 p += 3;
      }
      char *p1 = p, *p2;
      for (int i = 0; i < PARC; i++) {
	 p2 = p1;
	 while (*p1 && *p1 != ' ') {
	    p1++;
	 }
	 *(p1++) = '\0';
	 while (*p1 && *p1 == ' ') {
	    p1++;
	 }
	 std::cout << "$" << i << ":" << p2 << ";";
      }
      std::cout << std::endl;
      exit(0);
   }

   // Check the number of arguments or -p option.
   // Print PROTOTYPE : (name - number of parameters - number of inputs - number of output).
   if (argc >= 2 && !strcmp(argv[1], "-p")) {
      snprintf(error, sizeof(error), "%s %d %d %d", argv[0], PARC, FINC, FOUTC);
      error[sizeof(error) - 1] = 0;
      std::cout << error << std::endl;
      exit(0);
   }

   // Check the number of arguments or -h option.
   // Print USAGE.
   if (argc <= PARC || (argc >= 2 && !strcmp(argv[1], "-h"))) {
      snprintf(error, sizeof(error), USAGE, argv[0]);
      error[sizeof(error) - 1] = 0;
      std::cerr << error << std::endl;
      exit(0);
   }

   // Read parameters (all are floating point values).
   int k = 1;
   for (int i = 0; i < PARC; parv[i++] = argv[k++]) ;

   // Read all input files
   for (int i = 0; i < FINC; i++, k++) {
      char * filename = ((k >= argc) || (!strcmp(argv[k], "-"))) ? NULL : argv[k];
      objin[i] = LoadFileHeader(filename);
      if (objin[i] == NULL) {
	 Exit(FAILURE);
      }
      
      objs[i] = objin[i];
   }
   
   if (objs[0]->Type() == Po_Img1duc) {
      Img1duc* const ims = (Img1duc*)objs[0];
      result = PProperty(*ims,atoi(parv[0]));
      goto end;
   }
   if (objs[0]->Type() == Po_Img1dsl) {
      Img1dsl* const ims = (Img1dsl*)objs[0];
      result = PProperty(*ims,atoi(parv[0]));
      goto end;
   }
   if (objs[0]->Type() == Po_Img1dsf) {
      Img1dsf* const ims = (Img1dsf*)objs[0];
      result = PProperty(*ims,atoi(parv[0]));
      goto end;
   }
   if (objs[0]->Type() == Po_Imx1duc) {
      Imx1duc* const ims = (Imx1duc*)objs[0];
      result = PProperty(*ims,atoi(parv[0]));
      goto end;
   }
   if (objs[0]->Type() == Po_Imx1dsl) {
      Imx1dsl* const ims = (Imx1dsl*)objs[0];
      result = PProperty(*ims,atoi(parv[0]));
      goto end;
   }
   if (objs[0]->Type() == Po_Imx1dsf) {
      Imx1dsf* const ims = (Imx1dsf*)objs[0];
      result = PProperty(*ims,atoi(parv[0]));
      goto end;
   }
   if (objs[0]->Type() == Po_Img2duc) {
      Img2duc* const ims = (Img2duc*)objs[0];
      result = PProperty(*ims,atoi(parv[0]));
      goto end;
   }
   if (objs[0]->Type() == Po_Img2dsl) {
      Img2dsl* const ims = (Img2dsl*)objs[0];
      result = PProperty(*ims,atoi(parv[0]));
      goto end;
   }
   if (objs[0]->Type() == Po_Img2dsf) {
      Img2dsf* const ims = (Img2dsf*)objs[0];
      result = PProperty(*ims,atoi(parv[0]));
      goto end;
   }
   if (objs[0]->Type() == Po_Imx2duc) {
      Imx2duc* const ims = (Imx2duc*)objs[0];
      result = PProperty(*ims,atoi(parv[0]));
      goto end;
   }
   if (objs[0]->Type() == Po_Imx2dsl) {
      Imx2dsl* const ims = (Imx2dsl*)objs[0];
      result = PProperty(*ims,atoi(parv[0]));
      goto end;
   }
   if (objs[0]->Type() == Po_Imx2dsf) {
      Imx2dsf* const ims = (Imx2dsf*)objs[0];
      result = PProperty(*ims,atoi(parv[0]));
      goto end;
   }
   if (objs[0]->Type() == Po_Img3duc) {
      Img3duc* const ims = (Img3duc*)objs[0];
      result = PProperty(*ims,atoi(parv[0]));
      goto end;
   }
   if (objs[0]->Type() == Po_Img3dsl) {
      Img3dsl* const ims = (Img3dsl*)objs[0];
      result = PProperty(*ims,atoi(parv[0]));
      goto end;
   }
   if (objs[0]->Type() == Po_Img3dsf) {
      Img3dsf* const ims = (Img3dsf*)objs[0];
      result = PProperty(*ims,atoi(parv[0]));
      goto end;
   }
   if (objs[0]->Type() == Po_Imx3duc) {
      Imx3duc* const ims = (Imx3duc*)objs[0];
      result = PProperty(*ims,atoi(parv[0]));
      goto end;
   }
   if (objs[0]->Type() == Po_Imx3dsl) {
      Imx3dsl* const ims = (Imx3dsl*)objs[0];
      result = PProperty(*ims,atoi(parv[0]));
      goto end;
   }
   if (objs[0]->Type() == Po_Imx3dsf) {
      Imx3dsf* const ims = (Imx3dsf*)objs[0];
      result = PProperty(*ims,atoi(parv[0]));
      goto end;
   }
   if (objs[0]->Type() == Po_Imc2duc) {
      Imc2duc* const ims = (Imc2duc*)objs[0];
      result = PProperty(*ims,atoi(parv[0]));
      goto end;
   }
   if (objs[0]->Type() == Po_Imc2dsl) {
      Imc2dsl* const ims = (Imc2dsl*)objs[0];
      result = PProperty(*ims,atoi(parv[0]));
      goto end;
   }
   if (objs[0]->Type() == Po_Imc2dsf) {
      Imc2dsf* const ims =(Imc2dsf*)objs[0];
      result = PProperty(*ims,atoi(parv[0]));
      goto end;
   }
   if (objs[0]->Type() == Po_Imc3duc) {
      Imc3duc* const ims = (Imc3duc*)objs[0];
      result = PProperty(*ims,atoi(parv[0]));
      goto end;
   }
   if (objs[0]->Type() == Po_Imc3dsl) {
      Imc3dsl* const ims = (Imc3dsl*)objs[0];
      result = PProperty(*ims,atoi(parv[0]));
      goto end;
   }
   if (objs[0]->Type() == Po_Imc3dsf) {
      Imc3dsf* const ims = (Imc3dsf*)objs[0];
      result = PProperty(*ims,atoi(parv[0]));
      goto end;
   }
   if (objs[0]->Type() == Po_Reg2d) {
      Reg2d* const ims = (Reg2d*)objs[0];
      result = PProperty(*ims,atoi(parv[0]));
      goto end;
   }
   if (objs[0]->Type() == Po_Reg3d) {
      Reg3d* const ims = (Reg3d*)objs[0];
      result = PProperty(*ims,atoi(parv[0]));
      goto end;
   }
   if (objs[0]->Type() == Po_Graph2d) {
      Graph2d* const grs = (Graph2d*)objs[0];
      result = PProperty(*grs,atoi(parv[0]));
      goto end;
   }
   if (objs[0]->Type() == Po_Graph3d) {
      Graph3d* const grs = (Graph3d*)objs[0];
      result = PProperty(*grs,atoi(parv[0]));
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
