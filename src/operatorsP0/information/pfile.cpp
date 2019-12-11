/** -*- c-basic-offset: 3 ; mode: c++ -*-
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
 * @author Régis Clouard - 1995-10-08
 * @author Francois Angot - 1998-10-01
 * @author Olivier Lezoray - 1999-10-11
 * @author Alexandre Duret-Lutz - 1999-10-11
 * @author Régis Clouard - 2001-04-03 (version 3.00)
 * @author Régis Clouard - 2002-12-09 (version 4.00)
 */

#include <string.h>
#include <pandore.h>
using namespace pandore;

/**
 * @file pfile.cpp
 * Display related information of the input object.
 */

/**
 * Returns the name of the colorspace.
 */
const char *ColorName( PColorSpace c ) {
   switch (c) {
      case RGB: return "RGB";
      case XYZ: return "XYZ";
      case LUV: return "LUV";
      case LAB: return "LAB";
      case HSL: return "HSl";
      case AST: return "AST";
      case I1I2I3: return "I1I2I3";
      case LCH: return "LCH";
      case WRY: return "WRY";
      case RNGNBN: return "RNGNBN";
      case YCBCR: return "YCBCR";
      case YCH1CH2: return "YCH1CH2";
      case YIQ: return "YIQ";
      case YUV: return "YUV";
      default: return "unknown...";
   }
}

/**
 * Collections.
 */
Errc PFile( const Collection &c ) {
   std::list<std::string> l = c.List();

   fprintf(stdout, "Type      : Collection (Bundle of elements)\n");
   fprintf(stdout, "Number of elements : %d\n", (int)l.size());

   std::list<std::string>::const_iterator i;
   for ( i = l.begin(); i != l.end(); ++i ) {
      BundledObject* bo = c.Get(*i);
      if (bo->NbrElements()>1) {
	 fprintf(stdout, "\t%s\t(%d items)\t%s\n", i->c_str(), bo->NbrElements(), bo->Type().c_str());
      }else {
	 fprintf(stdout, "\t%s\t(%d items)\t%s\n", i->c_str(), bo->NbrElements(), bo->Type().c_str());
      }
   }
   return SUCCESS;
}

/**
 * Image X 1d
 */
Errc PFile( const Imx1duc &ims ) {
   fprintf(stdout, "Type      : Imx1duc (multispectral 1d image of %s)\n", (TypeName<Imx1duc::ValueType>::Name()).c_str());
   fprintf(stdout, "Size      : %d columns\n", ims.Width());
   if (ims.Bands() > 1) {
      fprintf(stdout, "Bands     : %d bands\n", ims.Bands());
   } else {
      fprintf(stdout, "Bands     : %d band\n", ims.Bands());
   }
   return SUCCESS;
}
/**
 * Image X 1d
 */
Errc PFile( const Imx1dsl &ims ) {
   fprintf(stdout, "Type      : Imx1dsl (multispectral 1d image of %s)\n", (TypeName<Imx1dsl::ValueType>::Name()).c_str());
   fprintf(stdout, "Size      : %d columns\n", ims.Width());
   if (ims.Bands() > 1) {
      fprintf(stdout, "Bands     : %d bands\n", ims.Bands());
   } else {
      fprintf(stdout, "Bands     : %d band\n", ims.Bands());
   }
   return SUCCESS;
}
/**
 * Image X 1d
 */
Errc PFile( const Imx1dsf &ims ) {
   fprintf(stdout, "Type      : Imx1dsf (multispectral 1d image of %s)\n", (TypeName<Imx1dsf::ValueType>::Name()).c_str());
   fprintf(stdout, "Size      : %d columns\n", ims.Width());
   if (ims.Bands() > 1) {
      fprintf(stdout, "Bands     : %d bands\n", ims.Bands());
   } else {
      fprintf(stdout, "Bands     : %d band\n", ims.Bands());
   }
   return SUCCESS;
}

/**
 * Image X 2d
 */
Errc PFile( const Imx2duc &ims ) {
   fprintf(stdout, "Type      : Imx2duc (multispectral 2d image of %s)\n", (TypeName<Imx2duc::ValueType>::Name()).c_str());
   fprintf(stdout, "Size      : %d columns x %d rows\n", ims.Width(), ims.Height());
   if (ims.Bands() > 1) {
      fprintf(stdout, "Bands     : %d bands\n", ims.Bands());
   } else {
      fprintf(stdout, "Bands     : %d band\n", ims.Bands());
   }
   return SUCCESS;
}
/**
 * Image X 2d
 */
Errc PFile( const Imx2dsl &ims ) {
   fprintf(stdout, "Type      : Imx2dsl (multispectral 2d image of %s)\n", (TypeName<Imx2dsl::ValueType>::Name()).c_str());
   fprintf(stdout, "Size      : %d columns x %d rows\n", ims.Width(), ims.Height());
   if (ims.Bands() > 1) {
      fprintf(stdout, "Bands     : %d bands\n", ims.Bands());
   } else {
      fprintf(stdout, "Bands     : %d band\n", ims.Bands());
   }
   return SUCCESS;
}
/**
 * Image X 2d
 */
Errc PFile( const Imx2dsf &ims ) {
   fprintf(stdout, "Type      : Imx2dsf (multispectral 2d image of %s)\n", (TypeName<Imx2dsf::ValueType>::Name()).c_str());
   fprintf(stdout, "Size      : %d columns x %d rows\n", ims.Width(), ims.Height());
   if (ims.Bands() > 1) {
      fprintf(stdout, "Bands     : %d bands\n", ims.Bands());
   } else {
      fprintf(stdout, "Bands     : %d band\n", ims.Bands());
   }
   return SUCCESS;
}

/**
 * Image X 3d
 */
Errc PFile( const Imx3duc &ims ) {
   fprintf(stdout, "Type      : Imx3duc (multispectral 3d image of %s)\n", (TypeName<Imx3duc::ValueType>::Name()).c_str());
   fprintf(stdout, "Size      : %d columns x %d rows x %d planes\n", ims.Width(), ims.Height(), ims.Depth());
   if (ims.Bands()>1) {
      fprintf(stdout, "Bands     : %d bands\n", ims.Bands());
   } else {
      fprintf(stdout, "Bands     : %d band\n", ims.Bands());
   }
   return SUCCESS;
}
/**
 * Image X 3d
 */
Errc PFile( const Imx3dsl &ims ) {
   fprintf(stdout, "Type      : Imx3dsl (multispectral 3d image of %s)\n", (TypeName<Imx3dsl::ValueType>::Name()).c_str());
   fprintf(stdout, "Size      : %d columns x %d rows x %d planes\n", ims.Width(), ims.Height(), ims.Depth());
   if (ims.Bands()>1) {
      fprintf(stdout, "Bands     : %d bands\n", ims.Bands());
   } else {
      fprintf(stdout, "Bands     : %d band\n", ims.Bands());
   }
   return SUCCESS;
}
/**
 * Image X 3d
 */
Errc PFile( const Imx3dsf &ims ) {
   fprintf(stdout, "Type      : Imx3dsf (multispectral 3d image of %s)\n", (TypeName<Imx3dsf::ValueType>::Name()).c_str());
   fprintf(stdout, "Size      : %d columns x %d rows x %d planes\n", ims.Width(), ims.Height(), ims.Depth());
   if (ims.Bands()>1) {
      fprintf(stdout, "Bands     : %d bands\n", ims.Bands());
   } else {
      fprintf(stdout, "Bands     : %d band\n", ims.Bands());
   }
   return SUCCESS;
}


/**
 * Image G 1d
 */
Errc PFile( const Img1duc &ims ) {
   fprintf(stdout, "Type      : Img1duc (gray 1d image of %s)\n", (TypeName<Img1duc::ValueType>::Name()).c_str());
   fprintf(stdout, "Size      : %d columns\n", ims.Width());
   return SUCCESS;
}
/**
 * Image G 1d
 */
Errc PFile( const Img1dsl &ims ) {
   fprintf(stdout, "Type      : Img1dsl (gray 1d image of %s)\n", (TypeName<Img1dsl::ValueType>::Name()).c_str());
   fprintf(stdout, "Size      : %d columns\n", ims.Width());
   return SUCCESS;
}
/**
 * Image G 1d
 */
Errc PFile( const Img1dsf &ims ) {
   fprintf(stdout, "Type      : Img1dsf (gray 1d image of %s)\n", (TypeName<Img1dsf::ValueType>::Name()).c_str());
   fprintf(stdout, "Size      : %d columns\n", ims.Width());
   return SUCCESS;
}

/**
 * Image G 2d
 */
Errc PFile( const Img2duc &ims ) {
   fprintf(stdout, "Type      : Img2duc (gray 2d image of %s)\n", (TypeName<Img2duc::ValueType>::Name()).c_str());
   fprintf(stdout, "Size      : %d columns x %d rows\n", ims.Width(), ims.Height());
   return SUCCESS;
}
/**
 * Image G 2d
 */
Errc PFile( const Img2dsl &ims ) {
   fprintf(stdout, "Type      : Img2dsl (gray 2d image of %s)\n", (TypeName<Img2dsl::ValueType>::Name()).c_str());
   fprintf(stdout, "Size      : %d columns x %d rows\n", ims.Width(), ims.Height());
   return SUCCESS;
}
/**
 * Image G 2d
 */
Errc PFile( const Img2dsf &ims ) {
   fprintf(stdout, "Type      : Img2dsf (gray 2d image of %s)\n", (TypeName<Img2dsf::ValueType>::Name()).c_str());
   fprintf(stdout, "Size      : %d columns x %d rows\n", ims.Width(), ims.Height());
   return SUCCESS;
}

/**
 * Image G 3d
 */
Errc PFile( const Img3duc &ims ) {
   fprintf(stdout, "Type      : Img3duc (gray 3d image of %s)\n", (TypeName<Img3duc::ValueType>::Name()).c_str());
   fprintf(stdout, "Size      : %d columns x %d rows x %d planes\n", ims.Width(), ims.Height(), ims.Depth());
   return SUCCESS;
}
/**
 * Image G 3d
 */
Errc PFile( const Img3dsl &ims ) {
   fprintf(stdout, "Type      : Img3dsl (gray 3d image of %s)\n", (TypeName<Img3dsl::ValueType>::Name()).c_str());
   fprintf(stdout, "Size      : %d columns x %d rows x %d planes\n", ims.Width(), ims.Height(), ims.Depth());
   return SUCCESS;
}
/**
 * Image G 3d
 */
Errc PFile( const Img3dsf &ims ) {
   fprintf(stdout, "Type      : Img3dsf (gray 3d image of %s)\n", (TypeName<Img3dsf::ValueType>::Name()).c_str());
   fprintf(stdout, "Size      : %d columns x %d rows x %d planes\n", ims.Width(), ims.Height(), ims.Depth());
   return SUCCESS;
}

/**
 * regions
 */
Errc PFile( const Reg1d &rgs ) {
   fprintf(stdout, "Type      : Reg1d (1d region map)\n");
   fprintf(stdout, "Size      : %d columns\n", rgs.Width());
   fprintf(stdout, "Label max : %d\n", rgs.Labels());
   return SUCCESS;
}

Errc PFile( const Reg2d &rgs ) {
   fprintf(stdout, "Type      : Reg2d (2d region map)\n");
   fprintf(stdout, "Size      : %d columns x %d rows\n", rgs.Width(), rgs.Height());
   fprintf(stdout, "Label max : %d\n", rgs.Labels());
   return SUCCESS;
}

Errc PFile( const Reg3d &rgs ) {
   fprintf(stdout, "Type      : Reg3d (3d region map)\n");
   fprintf(stdout, "Size      : %d columns x %d rows x %d planes\n", rgs.Width(), rgs.Height(), rgs.Depth());
   fprintf(stdout, "Label max : %d\n", rgs.Labels());
   return SUCCESS;
}

/**
 * Image C 2d
 */
Errc PFile( const Imc2duc &ims ) {
   fprintf(stdout, "Type      : Imc2duc (color 2d image of %s)\n", (TypeName<Imc2duc::ValueType>::Name()).c_str());
   fprintf(stdout, "Size      : %d columns x %d rows\n", ims.Width(), ims.Height());
   fprintf(stdout, "Colorspace: %s\n", ColorName(ims.ColorSpace()));
   return SUCCESS;
}
/**
 * Image C 2d
 */
Errc PFile( const Imc2dsl &ims ) {
   fprintf(stdout, "Type      : Imc2dsl (color 2d image of %s)\n", (TypeName<Imc2dsl::ValueType>::Name()).c_str());
   fprintf(stdout, "Size      : %d columns x %d rows\n", ims.Width(), ims.Height());
   fprintf(stdout, "Colorspace: %s\n", ColorName(ims.ColorSpace()));
   return SUCCESS;
}
/**
 * Image C 2d
 */
Errc PFile( const Imc2dsf &ims ) {
   fprintf(stdout, "Type      : Imc2dsf (color 2d image of %s)\n", (TypeName<Imc2dsf::ValueType>::Name()).c_str());
   fprintf(stdout, "Size      : %d columns x %d rows\n", ims.Width(), ims.Height());
   fprintf(stdout, "Colorspace: %s\n", ColorName(ims.ColorSpace()));
   return SUCCESS;
}

/**
 * Image C 3d
 */
Errc PFile( const Imc3duc &ims ) {
   fprintf(stdout, "Type      : Imc3duc (color 3d image of %s)\n", (TypeName<Imc3duc::ValueType>::Name()).c_str());
   fprintf(stdout, "Size      : %d columns x %d rows x %d planes\n", ims.Width(), ims.Height(), ims.Depth());
   fprintf(stdout, "Colorspace: %s\n", ColorName(ims.ColorSpace()));
   return SUCCESS;
}
/**
 * Image C 3d
 */
Errc PFile( const Imc3dsl &ims ) {
   fprintf(stdout, "Type      : Imc3dsl (color 3d image of %s)\n", (TypeName<Imc3dsl::ValueType>::Name()).c_str());
   fprintf(stdout, "Size      : %d columns x %d rows x %d planes\n", ims.Width(), ims.Height(), ims.Depth());
   fprintf(stdout, "Colorspace: %s\n", ColorName(ims.ColorSpace()));
   return SUCCESS;
}
/**
 * Image C 3d
 */
Errc PFile( const Imc3dsf &ims ) {
   fprintf(stdout, "Type      : Imc3dsf (color 3d image of %s)\n", (TypeName<Imc3dsf::ValueType>::Name()).c_str());
   fprintf(stdout, "Size      : %d columns x %d rows x %d planes\n", ims.Width(), ims.Height(), ims.Depth());
   fprintf(stdout, "Colorspace: %s\n", ColorName(ims.ColorSpace()));
   return SUCCESS;
}

/**
 * Graph
 */
Errc PFile( const Graph2d &gs ) {
   fprintf(stdout, "Type      : Graph2d (graph 2d)\n");
   fprintf(stdout, "Nb nodes  : %d\n", (gs[0] == NULL) ? gs.Size() - 1 : gs.Size());
   fprintf(stdout, "Size      : %d columns x %d rows\n", gs.Width(), gs.Height());
   return SUCCESS;
}

Errc PFile( const Graph3d &gs ) {
   fprintf(stdout, "Type      : Graph3d (graph 3d)\n");
   fprintf(stdout, "Nb nodes  : %d\n", (gs[0] == NULL) ? gs.Size() - 1 : gs.Size());
   fprintf(stdout, "Size      : %d columns x %d rows x %d planes\n", gs.Width(), gs.Height(), gs.Depth());
   return SUCCESS;
}

#ifdef MAIN

/*
 * Modify only the following constants, and the function call.
 */
#define	USAGE	"usage: %s [obj_in|-]"
#define	PARC	0
#define	FINC	1
#define	FOUTC	0
#define	MASK	0

int main( int argc, char *argv[] ) {
   Errc result;
   FILE* fd = NULL;
   Errc error;
   Pobject* objs;
   Po_header headfile;
   int version;

   if (argc < 2) {
      fd = stdin;
   } else {
      if (!strcmp(argv[1], "-h")) {
	 fprintf(stderr, USAGE, argv[0]);
	 puts("\n");
	 exit(0);
      } else {
	 if (!strcmp(argv[1], "-p")) {
	    //	    fprintf(stdout, "%s %d %d %d\n", argv[0], 0, 1, 0);
	    exit(0);
	 } else {
	    if ((fd = fopen(argv[1], "rb")) == NULL) {
	       fprintf(stderr, "Error pfile: Cannot open file %s\n", argv[1]);
	       return -1;
	    }
	 }
      }
   }
   bool invert;
   headfile = ReadHeader(fd, version, invert);

   // Bad endian -> just print header properties.
   // Bad endian encoding: (Intel vs Motoroal)
   if ((objs = NewObject(headfile.Type())) == NULL) {
      if (version == 3) {
	 fprintf(stdout, "Version    : Pandore v%d\n", version);
	 fprintf(stdout, "Creator    : %s\n", headfile.ident);
	 fprintf(stdout, "Date       : 20%s\n", headfile.date);
	 fprintf(stdout, "Foreign encoding -> ");
	 fprintf(stdout, "Convert this file with pan2pan operator\n");
	 return 0;
      }
      
      if (version == 4 || version == 5) {
	 fprintf(stdout, "Creator   : %s\n", headfile.ident);
	 fprintf(stdout, "Date      : 2%s\n", headfile.date);
	 fprintf(stdout, "Foreign encoding -> ");
	 fprintf(stdout, "Convert this file with pan2pan operator\n");
	 return 0;
      }
      
      std::cerr << "Error pfile: Bad Pandore format" << std::endl;
      return 0;
   }

   if (version == 3) {
      fprintf(stdout, "Version   : Pandore v%d\n", version);
      fprintf(stdout, "Creator   : %s\n", headfile.ident);
      fprintf(stdout, "Date      : 20%s\n", headfile.date);
      fprintf(stdout, "Old version -> ");
      fprintf(stdout, "Convert this file with pan2pan operator\n");
      return 0;
   }

   // Good endian but ...   
   if (headfile.Type() == object) {
      std::cerr << "Error pfile: Unknown file format" << std::endl;
      return -1;
   }

   fprintf(stdout, "Creator   : %s\n", headfile.ident);
   fprintf(stdout, "Date      : 2%s\n", headfile.date);

   // ! Beware of the byte inversion encoding (little endian vs big endian).
   objs->setInversionMode(invert);
   // The object read his own data.
   error = objs->LoadAttributes(fd);

   switch(objs->Type()) {
      case Po_Reg1d : {
	 Reg1d* const ims = (Reg1d*)objs;

	 result = PFile(*ims);
      } break;
      case Po_Reg2d : {
	 Reg2d* const ims = (Reg2d*)objs;

	 result = PFile(*ims);
      } break;
      case Po_Reg3d : {
	 Reg3d* const ims = (Reg3d*)objs;

	 result = PFile(*ims);
      } break;
      case Po_Graph2d : {
	 Graph2d* const gs = (Graph2d*)objs; 
	 
	 result = PFile(*gs);
      } break;
      case Po_Graph3d : {
	 Graph3d* const gs = (Graph3d*)objs; 

	 result = PFile(*gs);
      } break;
      case Po_Collection : {
	 Collection* const c = (Collection*)objs;
	 objs->LoadData(fd);
	 result = PFile(*c);
      } break;
      default: break;
   }
   if (objs->Type() == Po_Imx1duc) {
      Imx1duc* const ims=(Imx1duc*)objs;
      
      result = PFile(*ims);
   } else
   if (objs->Type() == Po_Imx1dsl) {
      Imx1dsl* const ims=(Imx1dsl*)objs;
      
      result = PFile(*ims);
   } else
   if (objs->Type() == Po_Imx1dsf) {
      Imx1dsf* const ims=(Imx1dsf*)objs;
      
      result = PFile(*ims);
   } else
   if (objs->Type() == Po_Imx2duc) {
      Imx2duc* const ims = (Imx2duc*)objs;
      
      result = PFile(*ims);
   } else
   if (objs->Type() == Po_Imx2dsl) {
      Imx2dsl* const ims = (Imx2dsl*)objs;
      
      result = PFile(*ims);
   } else
   if (objs->Type() == Po_Imx2dsf) {
      Imx2dsf* const ims = (Imx2dsf*)objs;
      
      result = PFile(*ims);
   } else
   if (objs->Type() == Po_Imx3duc) {
      Imx3duc* const ims = (Imx3duc*)objs;
      
      result = PFile(*ims);
   } else
   if (objs->Type() == Po_Imx3dsl) {
      Imx3dsl* const ims = (Imx3dsl*)objs;
      
      result = PFile(*ims);
   } else
   if (objs->Type() == Po_Imx3dsf) {
      Imx3dsf* const ims = (Imx3dsf*)objs;
      
      result = PFile(*ims);
   } else
   if (objs->Type() == Po_Img1duc) {
      Img1duc* const ims = (Img1duc*)objs;
      
      result = PFile(*ims);
   } else
   if (objs->Type() == Po_Img1dsl) {
      Img1dsl* const ims = (Img1dsl*)objs;
      
      result = PFile(*ims);
   } else
   if (objs->Type() == Po_Img1dsf) {
      Img1dsf* const ims = (Img1dsf*)objs;
      
      result = PFile(*ims);
   } else
   if (objs->Type() == Po_Img2duc) {
      Img2duc* const ims = (Img2duc*)objs;
      
      result = PFile(*ims);
   } else
   if (objs->Type() == Po_Img2dsl) {
      Img2dsl* const ims = (Img2dsl*)objs;
      
      result = PFile(*ims);
   } else
   if (objs->Type() == Po_Img2dsf) {
      Img2dsf* const ims = (Img2dsf*)objs;
      
      result = PFile(*ims);
   } else
   if (objs->Type() == Po_Img3duc) {
      Img3duc* const ims = (Img3duc*)objs;
      
      result = PFile(*ims);
   } else
   if (objs->Type() == Po_Img3dsl) {
      Img3dsl* const ims = (Img3dsl*)objs;
      
      result = PFile(*ims);
   } else
   if (objs->Type() == Po_Img3dsf) {
      Img3dsf* const ims = (Img3dsf*)objs;
      
      result = PFile(*ims);
   } else
   if (objs->Type() == Po_Imc2duc) {
      Imc2duc* const ims = (Imc2duc*)objs;
      
      result = PFile(*ims);
   } else
   if (objs->Type() == Po_Imc2dsl) {
      Imc2dsl* const ims = (Imc2dsl*)objs;
      
      result = PFile(*ims);
   } else
   if (objs->Type() == Po_Imc2dsf) {
      Imc2dsf* const ims = (Imc2dsf*)objs;
      
      result = PFile(*ims);
   } else
   if (objs->Type() == Po_Imc3duc) {
      Imc3duc* const ims = (Imc3duc*)objs;
      
      result = PFile(*ims);
   } else
   if (objs->Type() == Po_Imc3dsl) {
      Imc3dsl* const ims = (Imc3dsl*)objs;
      
      result = PFile(*ims);
   } else
   if (objs->Type() == Po_Imc3dsf) {
      Imc3dsf* const ims = (Imc3dsf*)objs;
      
      result = PFile(*ims);
   } else

   fclose(fd); // Allow reading of data if necessary (e.g. collection).

   return 0;
}

#endif
