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
 * @author Alexandre Duret-Lutz - 1999-11-03
 * @author Regis Clouard - 2004-08-03 (version 5)
 * @author Regis Clouard - 2007-05-21 (fix bug on multispectral images)
 */

#include <pandore.h>
using namespace pandore;

#if (defined _MSC_VER) && (!defined snprintf)
#define snprintf _snprintf
#endif

/**
 * @file parray2im.cpp
 * Convert array to image.
 */
template <typename T, typename U>
Errc PArray2Im( const std::string name, const Collection &c, Imx3d<T> &imd, U * ) {
   int b;
   Ulong maxsize;
   std::string realname;
   char number[12];
   
   // Get max size
   maxsize = MAXULONG;
   for (b = 0; b < imd.Bands(); b++) {
      snprintf(number, sizeof(number), ".%d", b + 1);
      number[sizeof(number) - 1] = 0;
      if (imd.Bands() > 1) {
	 realname = name + number;
      } else {
	 realname = name;
      }
      Ulong _n = c.GETARRAYSIZE(realname, U);
      if (_n < maxsize) {
	 maxsize = _n;
      }
   }
   for (b = 0; b < imd.Bands(); b++) {
      snprintf(number, sizeof(number), ".%d", b + 1);
      number[sizeof(number) - 1] = 0;
      if (imd.Bands() > 1) {
	 realname = name + number;
      } else {
	 realname = name;
      }
      U* data = c.GETARRAY(realname, U);
      Ulong pos = 0;
      for (int z = 0; z< imd.Depth(); ++z) {
	 for (int y = 0; y< imd.Height(); ++y) {
	    for (int x = 0; x < imd.Width(); ++x) {
	       imd[b][z][y][x] = (T)data[pos];
	       ++pos;
	       if (pos > maxsize) {
		  break;
	       }
	    }
	 }
      }
   }
   return SUCCESS;
}













Errc PArray2Im( const Collection &ims1, Pobject* &objd, const std::string name, int ndep, int nrow, int ncol ) {
   Errc result;
   std::string type_name;
   int type;

   // Verification des arguments.
   if ((ndep != 0) && (ncol * nrow == 0)) {
      Exit(FAILURE);
   }
   if ((nrow != 0) && (ncol == 0)) {
      Exit(FAILURE);
   }
   if (ims1.Exists(name+".4")) {
      type = 2;
      type_name = ims1.GetType(name+".1");
   } else if (ims1.Exists(name+".3")) {
      type = 1;
      type_name = ims1.GetType(name+".1");
   } else{
      type = 0;
      type_name = ims1.GetType(name);
   }
   
   if ((type_name == "Array:Char") && (type == 0) && (ndep == 0)) {
      Img2d<Uchar>* imd1 = new Img2d<Uchar>(nrow, ncol);
      objd = imd1;
      result = PArray2Im(name, ims1, *imd1, (Char*)NULL);
   } else
   if ((type_name == "Array:Uchar") && (type == 0) && (ndep == 0)) {
      Img2d<Uchar>* imd1 = new Img2d<Uchar>(nrow, ncol);
      objd = imd1;
      result = PArray2Im(name, ims1, *imd1, (Uchar*)NULL);
   } else
   if ((type_name == "Array:Short") && (type == 0) && (ndep == 0)) {
      Img2d<Long>* imd1 = new Img2d<Long>(nrow, ncol);
      objd = imd1;
      result = PArray2Im(name, ims1, *imd1, (Short*)NULL);
   } else
   if ((type_name == "Array:Ushort") && (type == 0) && (ndep == 0)) {
      Img2d<Long>* imd1 = new Img2d<Long>(nrow, ncol);
      objd = imd1;
      result = PArray2Im(name, ims1, *imd1, (Ushort*)NULL);
   } else
   if ((type_name == "Array:Ulong") && (type == 0) && (ndep == 0)) {
      Img2d<Long>* imd1 = new Img2d<Long>(nrow, ncol);
      objd = imd1;
      result = PArray2Im(name, ims1, *imd1, (Ulong*)NULL);
   } else
   if ((type_name == "Array:Long") && (type == 0) && (ndep == 0)) {
      Img2d<Long>* imd1 = new Img2d<Long>(nrow, ncol);
      objd = imd1;
      result = PArray2Im(name, ims1, *imd1, (Long*)NULL);
   } else
   if ((type_name == "Array:Float") && (type == 0) && (ndep == 0)) {
      Img2d<Float>* imd1 = new Img2d<Float>(nrow, ncol);
      objd = imd1;
      result = PArray2Im(name, ims1, *imd1, (Float*)NULL);
   } else
   if ((type_name == "Array:Double") && (type == 0) && (ndep == 0)) {
      Img2d<Float>* imd1 = new Img2d<Float>(nrow, ncol);
      objd = imd1;
      result = PArray2Im(name, ims1, *imd1, (Double*)NULL);
   } else
   if ((type_name == "Array:Char") && (type == 1) && (ndep == 0)) {
      Imc2d<Uchar>* imd1 = new Imc2d<Uchar>(nrow, ncol);
      objd = imd1;
      result = PArray2Im(name, ims1, *imd1, (Char*)NULL);
   } else
   if ((type_name == "Array:Uchar") && (type == 1) && (ndep == 0)) {
      Imc2d<Uchar>* imd1 = new Imc2d<Uchar>(nrow, ncol);
      objd = imd1;
      result = PArray2Im(name, ims1, *imd1, (Uchar*)NULL);
   } else
   if ((type_name == "Array:Short") && (type == 1) && (ndep == 0)) {
      Imc2d<Long>* imd1 = new Imc2d<Long>(nrow, ncol);
      objd = imd1;
      result = PArray2Im(name, ims1, *imd1, (Short*)NULL);
   } else
   if ((type_name == "Array:Ushort") && (type == 1) && (ndep == 0)) {
      Imc2d<Long>* imd1 = new Imc2d<Long>(nrow, ncol);
      objd = imd1;
      result = PArray2Im(name, ims1, *imd1, (Ushort*)NULL);
   } else
   if ((type_name == "Array:Ulong") && (type == 1) && (ndep == 0)) {
      Imc2d<Long>* imd1 = new Imc2d<Long>(nrow, ncol);
      objd = imd1;
      result = PArray2Im(name, ims1, *imd1, (Ulong*)NULL);
   } else
   if ((type_name == "Array:Long") && (type == 1) && (ndep == 0)) {
      Imc2d<Long>* imd1 = new Imc2d<Long>(nrow, ncol);
      objd = imd1;
      result = PArray2Im(name, ims1, *imd1, (Long*)NULL);
   } else
   if ((type_name == "Array:Float") && (type == 1) && (ndep == 0)) {
      Imc2d<Float>* imd1 = new Imc2d<Float>(nrow, ncol);
      objd = imd1;
      result = PArray2Im(name, ims1, *imd1, (Float*)NULL);
   } else
   if ((type_name == "Array:Double") && (type == 1) && (ndep == 0)) {
      Imc2d<Float>* imd1 = new Imc2d<Float>(nrow, ncol);
      objd = imd1;
      result = PArray2Im(name, ims1, *imd1, (Double*)NULL);
   } else
   if ((type_name == "Array:Char") && (type == 0) && (ndep > 0)) {
      Img3d<Uchar>* imd1 = new Img3d<Uchar>(ndep, nrow, ncol);
      objd = imd1;
      result = PArray2Im(name, ims1, *imd1, (Char*)NULL);
   } else
   if ((type_name == "Array:Uchar") && (type == 0) && (ndep > 0)) {
      Img3d<Uchar>* imd1 = new Img3d<Uchar>(ndep, nrow, ncol);
      objd = imd1;
      result = PArray2Im(name, ims1, *imd1, (Uchar*)NULL);
   } else
   if ((type_name == "Array:Short") && (type == 0) && (ndep > 0)) {
      Img3d<Long>* imd1 = new Img3d<Long>(ndep, nrow, ncol);
      objd = imd1;
      result = PArray2Im(name, ims1, *imd1, (Short*)NULL);
   } else
   if ((type_name == "Array:Ushort") && (type == 0) && (ndep > 0)) {
      Img3d<Long>* imd1 = new Img3d<Long>(ndep, nrow, ncol);
      objd = imd1;
      result = PArray2Im(name, ims1, *imd1, (Ushort*)NULL);
   } else
   if ((type_name == "Array:Ulong") && (type == 0) && (ndep > 0)) {
      Img3d<Long>* imd1 = new Img3d<Long>(ndep, nrow, ncol);
      objd = imd1;
      result = PArray2Im(name, ims1, *imd1, (Ulong*)NULL);
   } else
   if ((type_name == "Array:Long") && (type == 0) && (ndep > 0)) {
      Img3d<Long>* imd1 = new Img3d<Long>(ndep, nrow, ncol);
      objd = imd1;
      result = PArray2Im(name, ims1, *imd1, (Long*)NULL);
   } else
   if ((type_name == "Array:Float") && (type == 0) && (ndep > 0)) {
      Img3d<Float>* imd1 = new Img3d<Float>(ndep, nrow, ncol);
      objd = imd1;
      result = PArray2Im(name, ims1, *imd1, (Float*)NULL);
   } else
   if ((type_name == "Array:Double") && (type == 0) && (ndep > 0)) {
      Img3d<Float>* imd1 = new Img3d<Float>(ndep, nrow, ncol);
      objd = imd1;
      result = PArray2Im(name, ims1, *imd1, (Double*)NULL);
   } else
 if ((type_name == "Array:Char") && (type == 2) && (ndep > 0)) {
    // Get the number of bands.
    std::list<std::string> l = ims1.List();
    std::list<std::string>::const_iterator i;
    int bands=0;
    for ( i = l.begin(); i != l.end(); ++i, ++bands) ;
    
    Imx3d<Uchar>* imd1=new Imx3d<Uchar>(bands, ndep, nrow, ncol);
    objd = imd1;
    result = PArray2Im(name, ims1, *imd1, (Char*)NULL);
 } else
 if ((type_name == "Array:Uchar") && (type == 2) && (ndep > 0)) {
    // Get the number of bands.
    std::list<std::string> l = ims1.List();
    std::list<std::string>::const_iterator i;
    int bands=0;
    for ( i = l.begin(); i != l.end(); ++i, ++bands) ;
    
    Imx3d<Uchar>* imd1=new Imx3d<Uchar>(bands, ndep, nrow, ncol);
    objd = imd1;
    result = PArray2Im(name, ims1, *imd1, (Uchar*)NULL);
 } else
 if ((type_name == "Array:Short") && (type == 2) && (ndep > 0)) {
    // Get the number of bands.
    std::list<std::string> l = ims1.List();
    std::list<std::string>::const_iterator i;
    int bands=0;
    for ( i = l.begin(); i != l.end(); ++i, ++bands) ;
    
    Imx3d<Long>* imd1=new Imx3d<Long>(bands, ndep, nrow, ncol);
    objd = imd1;
    result = PArray2Im(name, ims1, *imd1, (Short*)NULL);
 } else
 if ((type_name == "Array:Ushort") && (type == 2) && (ndep > 0)) {
    // Get the number of bands.
    std::list<std::string> l = ims1.List();
    std::list<std::string>::const_iterator i;
    int bands=0;
    for ( i = l.begin(); i != l.end(); ++i, ++bands) ;
    
    Imx3d<Long>* imd1=new Imx3d<Long>(bands, ndep, nrow, ncol);
    objd = imd1;
    result = PArray2Im(name, ims1, *imd1, (Ushort*)NULL);
 } else
 if ((type_name == "Array:Ulong") && (type == 2) && (ndep > 0)) {
    // Get the number of bands.
    std::list<std::string> l = ims1.List();
    std::list<std::string>::const_iterator i;
    int bands=0;
    for ( i = l.begin(); i != l.end(); ++i, ++bands) ;
    
    Imx3d<Long>* imd1=new Imx3d<Long>(bands, ndep, nrow, ncol);
    objd = imd1;
    result = PArray2Im(name, ims1, *imd1, (Ulong*)NULL);
 } else
 if ((type_name == "Array:Long") && (type == 2) && (ndep > 0)) {
    // Get the number of bands.
    std::list<std::string> l = ims1.List();
    std::list<std::string>::const_iterator i;
    int bands=0;
    for ( i = l.begin(); i != l.end(); ++i, ++bands) ;
    
    Imx3d<Long>* imd1=new Imx3d<Long>(bands, ndep, nrow, ncol);
    objd = imd1;
    result = PArray2Im(name, ims1, *imd1, (Long*)NULL);
 } else
 if ((type_name == "Array:Float") && (type == 2) && (ndep > 0)) {
    // Get the number of bands.
    std::list<std::string> l = ims1.List();
    std::list<std::string>::const_iterator i;
    int bands=0;
    for ( i = l.begin(); i != l.end(); ++i, ++bands) ;
    
    Imx3d<Float>* imd1=new Imx3d<Float>(bands, ndep, nrow, ncol);
    objd = imd1;
    result = PArray2Im(name, ims1, *imd1, (Float*)NULL);
 } else
 if ((type_name == "Array:Double") && (type == 2) && (ndep > 0)) {
    // Get the number of bands.
    std::list<std::string> l = ims1.List();
    std::list<std::string>::const_iterator i;
    int bands=0;
    for ( i = l.begin(); i != l.end(); ++i, ++bands) ;
    
    Imx3d<Float>* imd1=new Imx3d<Float>(bands, ndep, nrow, ncol);
    objd = imd1;
    result = PArray2Im(name, ims1, *imd1, (Double*)NULL);
 } else
   {
      fprintf(stderr, "Error parray2im: bad collection\n");
      result = FAILURE;
   }
   return result;
}

#ifdef MAIN

/*
 * Modify only the following constants, and the function call.
 */
#define	USAGE	"usage: %s w h d name [col_in|-] [im_out|-]"
#define	PARC	4
#define	FINC	1
#define	FOUTC	1
#define	MASK	0

int main( int argc, char *argv[] ) {
   Errc result;              // The result code of the execution.
   Pobject* mask;            // The region mask.
   Pobject* objin[FINC+1];   // The input objects;
   Pobject* objs[FINC+1];    // The source objects masked by the carte.
   Pobject* objout[FOUTC+1]; // The output object.
   Pobject* objd[FOUTC+1];   // The result object of the execution.
   char* parv[PARC+1];       // The input parameters.
   
   ReadArgs(argc, argv, PARC, FINC, FOUTC, &mask, objin, objs, objout, objd, parv, USAGE, MASK);
   
   if (objs[0]->Type() == Po_Collection) {
      Collection* ims1=(Collection*)objs[0];
      
      result = PArray2Im(*ims1, objd[0], std::string(argv[4]), atoi(parv[2]), atoi(parv[1]), atoi(parv[0]));
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
