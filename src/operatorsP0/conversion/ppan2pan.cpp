/* -*- c-basic-offset: 3 -*-
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
 * @author Regis Clouard - 2001-06-01
 * @author Regis Clouard - 2001-11-22 (fix bugs)
 * @author Regis Clouard - 2002-12-22 (version 4 .0)
 * @author Regis Clouard - 2003-05-13 (fix bug Reg2d, Reg3d)
 */

#include <string.h>
#include <pandore.h>
using namespace pandore;

/**
 * @file ppan2pan.cpp
 *
 * Convert old Pandore file to current Pandore file.
 */

typedef enum { // DO NOT CHANGE THIS ORDERING !
   Po3_object, 
   Po3_Img2duc, 
   Po3_Img2dsc, 
   Po3_Img2dus, 
   Po3_Img2dss, 
   Po3_Img2dsf, 
   Po3_Img3duc, 
   Po3_Img3dsc, 
   Po3_Img3dus, 
   Po3_Img3dss, 
   Po3_Img3dsf, 
   Po3_Reg2d, 
   Po3_Graph2d, 
   Po3_Imc2duc, 
   Po3_Imc2dsc, 
   Po3_Imc2dus, 
   Po3_Imc2dss, 
   Po3_Imc2dsf, 
   Po3_Reg3d, 
   Po3_Graph3d, 
   Po3_Img1duc, 
   Po3_Img1dsc, 
   Po3_Img1dus, 
   Po3_Img1dss, 
   Po3_Img1dsf, 
   Po3_Reg1d, 
   Po3_Imc3duc, 
   Po3_Collection, 
   Po3_Img1dsd, 
   Po3_Img2dsd, 
   Po3_Img3dsd, 
   Po3_Imc2dsd, 
   Po3_Imc3dsc, 
   Po3_Imc3dus, 
   Po3_Imc3dss, 
   Po3_Imc3dsf, 
   Po3_Imc3dsd, 
   Po3_Imx1duc, 
   Po3_Imx1dsc, 
   Po3_Imx1dus, 
   Po3_Imx1dss, 
   Po3_Imx1dsf, 
   Po3_Imx1dsd, 
   Po3_Imx2duc, 
   Po3_Imx2dsc, 
   Po3_Imx2dus, 
   Po3_Imx2dss, 
   Po3_Imx2dsf, 
   Po3_Imx2dsd, 
   Po3_Imx3duc, 
   Po3_Imx3dsc, 
   Po3_Imx3dus, 
   Po3_Imx3dss, 
   Po3_Imx3dsf, 
   Po3_Imx3dsd
}TypobjVersion3;

// Same architecture or different
// (Inter / Motorola)
bool ReverseByteOrder;

// Numero de la version.
static int version=2;

/*
 * Reverses high and low bytes.
 */
Short Reverse(Short s) {
   union{
      Short s;
      char c[2];
   } u, v;

   u.s =s;
   v.c[0]=u.c[1];
   v.c[1]=u.c[0];

   return v.s;
}

Long Reverse(Long l) {
   union{
      Long l;
      char c[4];
   } u, v;

   u.l = l;
   v.c[0]=u.c[3];
   v.c[1]=u.c[2];
   v.c[2]=u.c[1];
   v.c[3]=u.c[0];

   return v.l;
}

Float Reverse(Float l) {
   union {
      Float l;
      char c[4];
   } u, v;

   u.l = l;
   v.c[0]=u.c[3];
   v.c[1]=u.c[2];
   v.c[2]=u.c[1];
   v.c[3]=u.c[0];

   return v.l;
}


/*
 * Reverse high and low bytes.
 */
Double Reverse(Double l) {
   union {
      Double l;
      char c[8];
   } u, v;

   u.l = l;
   v.c[0]=u.c[7];
   v.c[1]=u.c[6];
   v.c[2]=u.c[5];
   v.c[3]=u.c[4];
   v.c[4]=u.c[3];
   v.c[5]=u.c[2];
   v.c[6]=u.c[1];
   v.c[7]=u.c[0];

   return v.l;
}

size_t freaddata( Char *ptr, size_t  nitems,  FILE *stream ) {
   return fread((void*)ptr, sizeof(Char), nitems, stream);
}

size_t freaddata( Uchar *ptr, size_t  nitems,  FILE *stream ) {
   return freaddata((Char*)ptr, nitems, stream);
}

size_t freaddata( Short *ptr, size_t  nitems,  FILE *stream ) {
   size_t n=0;

   for (size_t i=0;i<nitems;i++) {
      if (fread((void*)ptr, sizeof(Ushort), 1, stream)>0) {
	 if (ReverseByteOrder) {
	    *ptr=Reverse(*ptr);
	 }
	 n++;
	 ptr++;
      }else{
	 n=0;
	 break;
      }
   }
 return n;
}

size_t freaddata( Ushort *ptr, size_t  nitems,  FILE *stream ) {
   return freaddata((Short*)ptr, nitems, stream);
}


size_t freaddata( Long *ptr, size_t  nitems,  FILE *stream ) {
   size_t n=0;
   for (size_t i=0;i<nitems;i++) {
      if (fread((void*)ptr, sizeof(Long), 1, stream)>0) {
	 if (ReverseByteOrder)
	    *ptr=Reverse(*ptr);
	 n++;
	 ptr++;
      }else{
	 n=0;
	 break;
      }
   }
   return n;
}

size_t freaddata( Ulong *ptr, size_t  nitems,  FILE *stream ) {
   return freaddata((Long*)ptr, nitems, stream);
}

size_t freaddata( Float *ptr, size_t  nitems,  FILE *stream ) {
   size_t n=0;

   for (size_t i=0; i<nitems; i++) {
      if (fread((void*)ptr, sizeof(Long), 1, stream)>0) {
	 if (ReverseByteOrder) {
	    *ptr=Reverse(*ptr);
	 }
	 n++;
	 ptr++;
      }
   }
   return n;
}

size_t freaddata( Double *ptr, size_t  nitems,  FILE *stream ) {
   size_t n=0;

   for (size_t i=0;i<nitems;i++) {
      if (fread((void*)ptr, sizeof(Long), 1, stream)>0) {
	 if (ReverseByteOrder) {
	    *ptr=Reverse(*ptr);
	 }
	 n++;
	 ptr++;
      }else{
	 n=0;
	 break;
      }
   }
   return n;
}

/*
 * Return the type of the input object, 
 * and set the global variable
 * "version" to 2 ou 3.
 * -1 : unknow type.
 */
static int Header( FILE *fd ) {
   Po_header headfile;
   
   if (fread((void*)&headfile, sizeof(headfile), 1, fd)<1)
      return -1;
   
   if ((!strcmp(headfile.magic, "HORUSv1.0")) || 
       (!strcmp(headfile.magic, "PANDORE.1")) ||
       (!strcmp(headfile.magic, "PANDORE.2"))) {
      version =2;
      return headfile.Type();
   }else if (!strcmp(headfile.magic, "PANDORE30")) {
      version = 3;
      return headfile.Type();
   }else if (!strcmp(headfile.magic, "PANDORE04")) {
      version = 4;
      return headfile.Type();
   }else if (!strcmp(headfile.magic, "PANDORE05")) {
      version = 5;
      return headfile.Type();
   }else
      return -1;   
}

Pobject* PPan2Pan(FILE*, Collection *col) {
   // Pas d'attributs...

// Errc Collection::LoadData(FILE* df)
// {
//    Long size;
//    Long attr[3];

//    if (fread((void*)&size, sizeof(size), 1, df) < 1)
//       return FAILURE;
//    for ( int i = 0; i<size; ++i ) {   
//       if (fread((void*)attr, sizeof(attr), 1, df) < 1)
// 	 return FAILURE;
//       char *type = new char[attr[1]+1];
//       char *name = new char[attr[2]+1];
//       if ((fread(type, attr[1], 1, df) < 1) ||
// 	  (fread(name, attr[2], 1, df) < 1))
// 	 return FAILURE;
//       type[attr[1]] = 0;
//       name[attr[2]] = 0;
//       BundledObject* bo = LoadBundledObject(df, type, attr[0]);

//       if (!bo)
// 	 return FAILURE;
//       Set(name, bo);
//    }
//    return SUCCESS;
// }
   return col;
}

Pobject* PPan2Pan( FILE *fp, Img2duc *imd ) {
   Long attr[2];
   
   if (version>=4) {
      // Skip the number of bands.
      if (freaddata(attr, 1, fp) <= 0)
	 return NULL;
   }
   if (freaddata(attr, 2, fp) <= 0)
      return NULL;

   imd->New(attr[0], attr[1]);
   size_t n = (size_t)(imd->Height()*imd->Width());
   if (fread(imd->Vector(), sizeof(Char), n, fp)<n) {
      std::cerr << "Warning ppan2pan: Probably bad Pandore image contents.\n";
   }
   
   return imd;
}

Pobject* PPan2Pan( FILE *fp, Img2dsl *imd ) {
   Long attr[2];
   
   if (version>=4) {
      // Skip the number of bands.
      if (freaddata(attr, 1, fp) <= 0)
	 return NULL;
   }
   if (freaddata(attr, 2, fp) <= 0)
      return NULL;
   imd->New(attr[0], attr[1]);
 
   if (version < 4) {
      Ushort* data, *pdata;
      data = pdata = (Ushort*)calloc(imd->Height()*imd->Width(), sizeof(Ushort));
      size_t n = (size_t)(imd->Height()*imd->Width());
      if (freaddata(data, n, fp)<n) {
	 std::cerr << "Warning ppan2pan: Probably bad Pandore image contents.\n";
      }
      Long* p = imd->Vector();
      for (int y=0; y< imd->Height()*imd->Width(); y++) {
	 *(p++) = (*data++);
      }
      free(pdata);
   }else{
      size_t n = (size_t)(imd->Height()*imd->Width());
      if (freaddata(imd->Vector(), n, fp)<n) {
	 std::cerr << "Warning ppan2pan: Probably bad Pandore image contents.\n";
      }
   }
   return imd;

}

Pobject* PPan2Pan( FILE *fp, Img2dsf *imd ) {
   Long attr[2];
   
   if (version>=4) {
      // Skip the number of bands.
      if (freaddata(attr, 1, fp) <= 0) {
	 return NULL;
      }
   }
   if (freaddata(attr, 2, fp) <= 0) {
      return NULL;
   }
   imd->New(attr[0], attr[1]);
   size_t n = (size_t)(imd->Height()*imd->Width());
   if (freaddata(imd->Vector(), n, fp)<n) {
      std::cerr << "Warning ppan2pan: Probably bad Pandore image contents.\n";
   }
   return imd;
   
}

// Case double -> float
Pobject* PPan2Pan2( FILE *fp, Img2dsf *imd ) {
   Long attr[2];
   
   if (version>=4) {
      // Skip the number of bands.
      if (freaddata(attr, 1, fp) <= 0) {
	 return NULL;
      }
   }
   if (freaddata(attr, 2, fp) <= 0) {
      return NULL;
   }
   imd->New(attr[0], attr[1]);
   
   double* data, *pdata;
   data = pdata = (double*)calloc(imd->VectorSize(), sizeof(double));
   size_t n = (size_t)imd->VectorSize();
   if (freaddata(data, n, fp)<n) {
      std::cerr << "Warning ppan2pan: Probably bad Pandore image contents.\n";
   }
   
   float* p = imd->Vector();
   for (int y=0; y< imd->Height()*imd->Width(); y++) {
      *(p++) = (float)(*data++);
   }
   free(pdata);
   
   return imd;
}

Pobject* PPan2Pan( FILE *fp, Img3duc *imd ) {
   Long attr[4];

   if (version == 2) {
      if (freaddata(attr, 3, fp) <= 0) {
	 return NULL;
      }
      imd->New(attr[0], attr[1], attr[2]);
   }
   if (version >= 3) {
      if (freaddata(attr, 4, fp) <= 0) {
	 return NULL;
      }
      imd->New(attr[1], attr[2], attr[3]);
   }   

   size_t n = (size_t)imd->Depth()*imd->Height()*imd->Width();
   if (freaddata(imd->Vector(), n, fp)<n) {
      std::cerr << "Warning ppan2pan: Probably bad Pandore image contents.\n";
   }
   
   return imd;
}

Pobject* PPan2Pan( FILE *fp, Img3dsl *imd ) {
   Long attr[4];

   if (version == 2) {
      if (freaddata(attr, 3, fp) <= 0) {
	 return NULL;
      }
      imd->New(attr[0], attr[1], attr[2]);
   }
   if (version >= 3) {
      if (freaddata(attr, 4, fp) <= 0) {
	 return NULL;
      }
      imd->New(attr[1], attr[2], attr[3]);
   }   

   if (version < 4) {
      Ushort* data, *pdata;
      data = pdata = (Ushort*)calloc(imd->Depth()*imd->Height()*imd->Width(), sizeof(Ushort));
      size_t n = (size_t)(imd->Depth()*imd->Height()*imd->Width());
      if (freaddata(data, n, fp)<n) {
	 std::cerr << "Warning ppan2pan: Probably bad Pandore image contents.\n";
      }
      Long* p = imd->Vector();
      for (int y=0; y< imd->Depth()*imd->Height()*imd->Width(); y++) {
	 *(p++) = (*data++);
      }
      free(pdata);
   } else {
      size_t n = (size_t)(imd->Depth()*imd->Height()*imd->Width());
      if (freaddata(imd->Vector(), n, fp)<n) {
	 std::cerr << "Warning ppan2pan: Probably bad Pandore image contents.\n";
      }
   }
   
   return imd;
}

/**
 * Reads a Img3dsf image.
 */
Pobject* PPan2Pan( FILE *fp, Img3dsf *imd ) {
   Long attr[4];

   if (version == 2) {
      if (freaddata(attr, 3, fp) <= 0) {
	 return NULL;
      }
      imd->New(attr[0], attr[1], attr[2]);
   }
   if (version >= 3) {
      if (freaddata(attr, 4, fp) <= 0) {
	 return NULL;
      }
      imd->New(attr[1], attr[2], attr[3]);
   }

   size_t n = (size_t)(imd->Depth()*imd->Height()*imd->Width());
   if (freaddata(imd->Vector(), n, fp)<n) {
      std::cerr <<"Warning ppan2pan: Probably bad Pandore image contents."<<std::endl;
   }
   
   return imd;
}

Pobject* PPan2Pan2( FILE *fp, Img3dsf *imd ) {
   Long attr[4];
   if (version == 2) {
      if (freaddata(attr, 3, fp) <= 0)
	 return NULL;
      imd->New(attr[0], attr[1], attr[2]);
   }
   if (version >= 3) {
      if (freaddata(attr, 4, fp) <= 0)
	 return NULL;
      imd->New(attr[1], attr[2], attr[3]);
   }   

   Double* data, *pdata;
   data = pdata = (Double*)calloc(imd->VectorSize(), sizeof(Double));
   size_t n = (size_t)(imd->VectorSize());
   if (freaddata(data, n, fp)<n) {
      std::cerr<<"Warning ppan2pan: Probably bad Pandore image contents."<<std::endl;
   }
   
   Float* p = imd->Vector();
   for (int y=0; y< imd->Height()*imd->Width(); y++)
      *(p++) = (Float)(*data++);
   free(pdata);
   
   return imd;
}

Pobject* PPan2Pan( FILE *fp, Imc2duc *imd ) {
   Long attr[2];

   if (version>=4) {
      // Skip the number of bands.
      if (freaddata(attr, 1, fp) <= 0)
	 return NULL;
   }

   if (freaddata(attr, 2, fp) <= 0)
      return NULL;
   imd->New(attr[0], attr[1]);

   if (freaddata(attr, 1, fp) <= 0)
      return NULL;
   imd->ColorSpace((PColorSpace)attr[0]);
   for (int b=0; b<3; b++)
      if (fread(imd->Vector(b), sizeof(Char), (size_t)(imd->Height()*imd->Width()), fp)<=0)
	 return NULL;
   
   return imd;
}

Pobject* PPan2Pan( FILE *fp, Imc2dsl *imd ) {
   Long attr[2];
   
   if (version>=4) {
      // Skip the number of bands.
      if (freaddata(attr, 1, fp) <= 0)
	 return NULL;
   }
   if (freaddata(attr, 2, fp) <= 0)
      return NULL;
   imd->New(attr[0], attr[1]);
   
   if (freaddata(attr, 1, fp) <= 0)
      return NULL;
   imd->ColorSpace((PColorSpace)attr[0]);
   size_t n = (size_t)(imd->Height()*imd->Width());
   for (int b=0; b<3; b++)
      if (freaddata(imd->Vector(b), n, fp)<n) {
	 std::cerr<<"Warning ppan2pan: Probably bad Pandore image contents."<<std::endl;
      }
   return imd;
}

Pobject* PPan2Pan( FILE *fp, Imc2dsf *imd ) {
   Long attr[2];

   if (version>=4) {
      // Skip the number of bands.
      if (freaddata(attr, 1, fp) <= 0)
	 return NULL;
   }

   if (freaddata(attr, 2, fp) <= 0)
      return NULL;
   imd->New(attr[0], attr[1]);
   if (freaddata(attr, 1, fp) <= 0)
      return NULL;
   imd->ColorSpace((PColorSpace)attr[0]);

   for (int b=0; b<3; b++)
      if (freaddata(imd->Vector(b), (size_t)(imd->Height()*imd->Width()), fp)<=1)
      return NULL;
      
   return imd;
   
}

Pobject* PPan2Pan2( FILE *fp, Imc2dsf *imd ) {
   Long attr[2];
   
   if (version>=4) {
      // Skip the number of bands.
      if (freaddata(attr, 1, fp) <= 0)
	 return NULL;
   }

   if (freaddata(attr, 2, fp) <= 0)
      return NULL;
   imd->New(attr[0], attr[1]);
   if (freaddata(attr, 1, fp) <= 0)
      return NULL;
   imd->ColorSpace((PColorSpace)attr[0]);

   Double * data, * pdata;
   Float *p;
   
   data = pdata = (Double*)calloc(imd->VectorSize(), sizeof(Double));
   if (freaddata(data, (size_t)(imd->VectorSize()), fp)<=0)
      return NULL;
   p = imd->VectorX();
   int y;
   for (y=0; y< (int)imd->VectorSize(); y++)
      *(p++) = (Float)(*data++);
   
   data = pdata = (Double*)calloc(imd->VectorSize(), sizeof(Double));
   if (freaddata(data, (Long)(imd->VectorSize()), fp)<=0)
      return NULL;
   p = imd->VectorY();
   for (y=0; y< (int)imd->VectorSize(); y++)
      *(p++) = (Float)(*data++);
   
   data = pdata = (Double*)calloc(imd->VectorSize(), sizeof(Double));
   if (freaddata(data, (size_t)(imd->VectorSize()), fp)<=0)
      return NULL;
   p = imd->VectorZ();
   for (y=0; y< (int)imd->VectorSize(); y++)
      *(p++) = (Float)(*data++);
   free(pdata);      
   
   return imd;
   
}

Pobject* PPan2Pan( FILE *fp, Imc3duc *imd ) {
   Long attr[3];
   
   if (version>=4) {
      // Skip the number of bands.
      if (freaddata(attr, 1, fp) <= 0)
	 return NULL;
   }
   if (freaddata(attr, 3, fp) <= 0)
      return NULL;
   imd->New(attr[0], attr[1], attr[2]);
   if (freaddata(attr, 1, fp) <= 0)
      return NULL;
   imd->ColorSpace((PColorSpace)attr[0]);

   for (int b=0; b<3; b++)
      if (freaddata(imd->Vector(b), (int)(imd->VectorSize()), fp)<=1)
	 return NULL;
   
   return imd;
}

Pobject* PPan2Pan( FILE *fp, Imc3dsl *imd ) {
   Long attr[3];
   
   if (version>=4) {
      // Skip the number of bands.
      if (freaddata(attr, 1, fp) <= 0)
	 return NULL;
   }
   if (freaddata(attr, 3, fp) <= 0)
      return NULL;
   imd->New(attr[0], attr[1], attr[2]);
   if (freaddata(attr, 1, fp) <= 0)
      return NULL;
   imd->ColorSpace((PColorSpace)attr[0]);
   
   for (int b=0; b<3; b++)
      if (freaddata(imd->Vector(b), (int)(imd->VectorSize()), fp)<=1)
	 return NULL;
   return imd;
}

Pobject* PPan2Pan( FILE *fp, Imc3dsf *imd ) {
   Long attr[3];
   
   if (version>=4) {
      // Skip the number of bands.
      if (freaddata(attr, 1, fp) <= 0)
	 return NULL;
   }
   if (freaddata(attr, 3, fp) <= 0)
      return NULL;
   imd->New(attr[0], attr[1], attr[2]);
   if (freaddata(attr, 1, fp) <= 0)
      return NULL;
   imd->ColorSpace((PColorSpace)attr[0]);

   for (int b=0; b<3; b++)
      if (freaddata(imd->Vector(b), (int)(imd->VectorSize()), fp)<=1)
	 return NULL;
   
   return imd;
}

Pobject* PPan2Pan( FILE *fp, Imx2duc *imd ) {
   Long attr[3];

   if (version>=4) {
      // Skip the number of bands.
      if (freaddata(attr, 1, fp) <= 0)
	 return NULL;
   }

   if (freaddata(attr, 3, fp) <= 0) {
      return NULL;
   }
   imd->New(attr[0], attr[1], attr[2]);

   if (freaddata(attr, 1, fp) <= 0) {
      return NULL;
   }
   for (int b=0; b<imd->Bands(); b++) {
      if (fread(imd->Vector(b), sizeof(Char), (size_t)(imd->Height()*imd->Width()), fp)<=0)  {
	 return NULL;
      }
   }
   return imd;
}

Pobject* PPan2Pan( FILE *fp, Imx2dsl *imd ) {
   Long attr[3];
   
   if (version>=4) {
      // Skip the number of bands.
      if (freaddata(attr, 1, fp) <= 0)
	 return NULL;
   }
   if (freaddata(attr, 3, fp) <= 0) {
      return NULL;
   }
   imd->New(attr[0], attr[1], attr[2]);
   
   if (freaddata(attr, 1, fp) <= 0) {
      return NULL;
   }
   for (int b=0; b<imd->Bands(); b++) {
      if (freaddata(imd->Vector(b), (int)(imd->Height()*imd->Width()), fp)<=1) {
	 return NULL;
      }
   }
   return imd;
}

Pobject* PPan2Pan( FILE *fp, Imx2dsf *imd ) {
   Long attr[3];

   if (version>=4) {
      // Skip the number of bands.
      if (freaddata(attr, 1, fp) <= 0)
	 return NULL;
   }

   if (freaddata(attr, 3, fp) <= 0) {
      return NULL;
   }
   imd->New(attr[0], attr[1], attr[2]);
   if (freaddata(attr, 1, fp) <= 0) {
      return NULL;
   }
   for (int b=0; b<imd->Bands(); b++) {
      if (freaddata(imd->Vector(b), (int)(imd->Height()*imd->Width()), fp)<=1) {
	 return NULL;
      }
   }
   return imd;
}

Pobject* PPan2Pan( FILE *fp, Imx3duc *imd ) {
   Long attr[4];
   
   if (version>=4) {
      // Skip the number of bands.
      if (freaddata(attr, 1, fp) <= 0) {
	 return NULL;
      }
   }
   if (freaddata(attr, 4, fp) <= 0) {
      return NULL;
   }
   imd->New(attr[0], attr[1], attr[2], attr[3]);
   if (freaddata(attr, 1, fp) <= 0) {
      return NULL;
   }
   for (int b=0; b<imd->Bands(); b++) {
      if (freaddata(imd->Vector(b), (int)(imd->VectorSize()), fp)<=1) {
	 return NULL;
      }
   }
   
   return imd;
}

Pobject* PPan2Pan( FILE* fp, Imx3dsl *imd ) {
   Long attr[4];
   
   if (version>=4) {
      // Skip the number of bands.
      if (freaddata(attr, 1, fp) <= 0) {
	 return NULL;
      }
   }
   if (freaddata(attr, 4, fp) <= 0) {
      return NULL;
   }
   imd->New(attr[0], attr[1], attr[2], attr[3]);
   if (freaddata(attr, 1, fp) <= 0) {
      return NULL;
   }
   
   for (int b=0; b<imd->Bands(); b++) {
      if (freaddata(imd->Vector(b), (int)(imd->VectorSize()), fp)<=1) {
	 return NULL;
      }
   }
   return imd;
}

Pobject* PPan2Pan( FILE *fp, Imx3dsf *imd ) {
   Long attr[4];
   
   if (version>=4) {
      // Skip the number of bands.
      if (freaddata(attr, 1, fp) <= 0)
	 return NULL;
   }
   if (freaddata(attr, 4, fp) <= 0) {
      return NULL;
   }
   imd->New(attr[0], attr[1], attr[2], attr[3]);
   if (freaddata(attr, 1, fp) <= 0) {
      return NULL;
   }
   
   for (int b=0; b<imd->Bands(); b++) {
      if (freaddata(imd->Vector(b), (int)(imd->VectorSize()), fp)<=1) {
	 return NULL;
      }
   }
   
   return imd;
}


/**
 * Les cartes de regions: assez special.
 */
Pobject* PPan2Pan( FILE *fp, Reg2d *imd ) {
   // Attributes.
   Long attr[2];

   if (version>=4) {
      // Skip the number of bands.
      if (freaddata(attr, 1, fp) <= 0)
	 return NULL;
   }
   if (freaddata(attr, 2, fp) <= 0) {
      return NULL;
   }
   imd->New(attr[0], attr[1]);
   
   if (freaddata(&attr[0], 1, fp) < 1)
      return NULL;
   imd->Labels(attr[0]);

   // DATA from version 3.
   if (version < 4) {
      Ushort* data, *pdata;
      data = pdata = (Ushort*)calloc(imd->Height()*imd->Width(), sizeof(Ushort));
      size_t n = (size_t)(int)(imd->Height()*imd->Width());
      if (freaddata(data, n, fp)<n) {
	 std::cerr<<"Warning ppan2pan: Probably bad Pandore image contents."<<std::endl;
      }
      
      Ulong* p = imd->Vector();
      for (int y=0; y< (int)imd->VectorSize(); y++) {
	 *(p++) = (*data++);
      }
      free(pdata);
   } else { // Version 4.
      imd->LoadData(fp);
      Long* pimd=(Long*)imd->Vector();
      if (ReverseByteOrder) {
	 if (imd->Labels() <= MAXUCHAR) {
	    // Nothing to do?
	 } else if (imd->Labels()<=MAXUSHORT) {
	    // reverse only the two last bytes.
	    for (Ulong xy=0;xy<imd->VectorSize();xy++, pimd++) {
	       Short x=(Short)(*pimd&0XFFFF);
	       *pimd=Reverse(x);
	    }
	 } else {
	    for (Ulong xy=0;xy<imd->VectorSize();xy++, pimd++) *pimd=Reverse(*pimd);
	 }
      }
   }
   
   return imd;
}

Pobject* PPan2Pan( FILE *fp, Reg3d *imd ) {
   Long attr[4];
   
   if (version>=4) {
      // Skip the number of bands.
      if (freaddata(attr, 1, fp) <= 0)
	 return NULL;
   }

   if (freaddata(attr, 4, fp) <= 0) {
      return NULL;
   }
   imd->New(attr[0], attr[1], attr[2]);
   imd->Labels(attr[3]);

   if (version < 4) {
      Ushort* data, *pdata;
      data = pdata = (Ushort*)calloc(imd->Depth()*imd->Height()*imd->Width(), sizeof(Ushort));
      size_t n = (size_t)(imd->VectorSize());
      if (freaddata(data, n, fp)<n) {
	 std::cerr<<"Warning ppan2pan: Probably bad Pandore image contents."<<std::endl;
      }
      Ulong* p = imd->Vector();
      for (int y=0; y<(int)imd->VectorSize(); y++)
	 *(p++) = (*data++);
      free(pdata);
   } else {
      imd->LoadData(fp);
       Long* pimd=(Long*)imd->Vector();
      if (ReverseByteOrder) {
	 if (imd->Labels() <= MAXUCHAR) {
	    // Rien a faire?
	 } else if (imd->Labels()<=MAXUSHORT) {
	    // Ne renverser que les deux derniers octets du long.
	    for (Ulong xy=0;xy<imd->VectorSize();xy++, pimd++) {
	       Short x=(Short)*pimd&0XFFFF;
	       *pimd=Reverse(x);
	    }
	 } else {
	    for (Ulong xy=0;xy<imd->VectorSize();xy++, pimd++) {
	       *pimd=Reverse(*pimd);
	    }
	 }
      }
   }
   return imd;
}

Pobject* PPan2Pan( FILE *fp, Graph2d *grd ) {
   Long attr[3];
   
   if (freaddata(attr, 3, fp) < 3) {
      return NULL;
   }
   grd->New(attr[0], attr[1], attr[2]);

   register int i;
   Long suiv;
   Long val;
   Long wgh;
   Point2d p;
   
   for (i=0;i<grd->Size();i++) {
      if (freaddata(&suiv, 1, fp) < 1) {
	 return NULL;
      }
      if (suiv >= 0) {
	 if (freaddata(&val, 1, fp)<1) {
	    return NULL;
	 }
	 if (freaddata(&p.y, 1, fp)<1) return NULL;
	 if (freaddata(&p.x, 1, fp)<1) return NULL;
	 if (!grd->Add(i, suiv, p)) return NULL;
	 (*grd)[i]->value=(Graph2d::ValueType)val;
	 
	 if (freaddata(&suiv, 1, fp)<1)
	    return NULL;
	 while (suiv>-1) {
	    if (freaddata(&wgh, 1, fp)<1) {
	       return NULL;
	    }
	    if (i>suiv) {	// Link with former nodes.
	       grd->Link(i, suiv, (Graph2d::ValueType)wgh);
	    }
	    if (freaddata(&suiv, 1, fp)<1) {
	       return NULL;
	    }
	 }
      }
   }

   return grd;
}

Pobject* PPan2Pan( FILE* fp, Graph3d *grd ) {
   Long attr[4];
   
   if (freaddata(attr, 4, fp) < 4) {
      return NULL;
   }
   grd->New(attr[0], attr[1], attr[2], attr[3]);
   
   register int i;
   Long suiv;
   Long val;
   Long wgh;
   Point3d p;
   
   for (i=0;i<grd->Size();i++) {
      if (freaddata(&suiv, 1, fp) < 1) {
	 return NULL;
      }
      if (suiv >= 0) {
	 if (freaddata(&val, 1, fp)<1)
	    return NULL;
	 if (freaddata(&p.z, 1, fp)<1) return NULL;
	 if (freaddata(&p.y, 1, fp)<1) return NULL;
	 if (freaddata(&p.x, 1, fp)<1) return NULL;

	 if (!grd->Add(i, suiv, p)) return NULL;
	 (*grd)[i]->value=(Graph3d::ValueType)val;
	 
	 if (freaddata(&suiv, 1, fp)<1) {
	    return NULL;
	 }
	 while (suiv>-1) {
	    if (freaddata(&wgh, 1, fp)<1) {
	       return NULL;
	    }
	    if (i>suiv)	{ // Link with former nodes.
	       grd->Link(i, suiv, (Graph3d::ValueType)wgh);
	    }
	    if (freaddata(&suiv, 1, fp)<1) {
	       return NULL;
	    }
	 }
      }
   }
   return grd;
}

/*
 * MAIN
 */
Errc PPan2Pan( FILE* fp, Pobject **objout) {
  Long type;
  
  if ((type=Header(fp)) < 0) {
     std::cerr<<"Not an old version Pandore file"<< std::endl;
     return FAILURE;
  }

  // Intel file or Motorola file.
  if (type > 32767) { // Suppose that no exist more than 332767 object types.
     ReverseByteOrder = true;
     type = Reverse(type);
  } else {
     ReverseByteOrder = false;
  }

  if (version< 4) {
     switch (type) {
     case Po3_Img2duc : *objout=PPan2Pan(fp, new Img2duc);break;
     case Po3_Img2dsc : *objout=PPan2Pan(fp, new Img2duc);break;
     case Po3_Img2dus : *objout=PPan2Pan(fp, new Img2dsl);break;
     case Po3_Img2dss : *objout=PPan2Pan(fp, new Img2dsl);break;
     case Po3_Img2dsf : *objout=PPan2Pan(fp, new Img2dsf);break;
     case Po3_Img2dsd : *objout=PPan2Pan2(fp, new Img2dsf);break;
     case Po3_Img3duc : *objout=PPan2Pan(fp, new Img3duc);break;
     case Po3_Img3dsc : *objout=PPan2Pan(fp, new Img3duc);break;
     case Po3_Img3dus : *objout=PPan2Pan(fp, new Img3dsl);break;
     case Po3_Img3dss : *objout=PPan2Pan(fp, new Img3dsl);break;
     case Po3_Img3dsf : *objout=PPan2Pan(fp, new Img3dsf);break;
     case Po3_Img3dsd : *objout=PPan2Pan2(fp, new Img3dsf);break;
     case Po3_Reg2d   : *objout=PPan2Pan(fp, new Reg2d);break;
     case Po3_Graph2d : *objout=PPan2Pan(fp, new Graph2d);break;
     case Po3_Imc2duc : *objout=PPan2Pan(fp, new Imc2duc);break;
     case Po3_Imc2dsc : *objout=PPan2Pan(fp, new Imc2duc);break;
     case Po3_Imc2dus : *objout=PPan2Pan(fp, new Imc2dsl);break;
     case Po3_Imc2dss : *objout=PPan2Pan(fp, new Imc2dsl);break;
     case Po3_Imc2dsf : *objout=PPan2Pan(fp, new Imc2dsf);break;
     case Po3_Reg3d   : *objout=PPan2Pan(fp, new Reg3d);break;
     case Po3_Graph3d : *objout=PPan2Pan(fp, new Graph3d);break;
     case Po3_Collection : *objout=PPan2Pan(fp, new Collection);break;
     case Po3_Imc2dsd : *objout=PPan2Pan2(fp, new Imc2dsf);break;
     default : std::cerr<<"Error ppan2pan: Unknown Pandore file format"<<std::endl;break;
     }
  } else {
     switch (type) {
     case Po_Img2duc : *objout=PPan2Pan(fp, new Img2duc);break;
     case Po_Img2dsl : *objout=PPan2Pan(fp, new Img2dsl);break;
     case Po_Img2dsf : *objout=PPan2Pan(fp, new Img2dsf);break;
     case Po_Img3duc : *objout=PPan2Pan(fp, new Img3duc);break;
     case Po_Img3dsl : *objout=PPan2Pan(fp, new Img3dsl);break;
     case Po_Img3dsf : *objout=PPan2Pan(fp, new Img3dsf);break;

     case Po_Imc2duc : *objout=PPan2Pan(fp, new Imc2duc);break;
     case Po_Imc2dsl : *objout=PPan2Pan(fp, new Imc2dsl);break;
     case Po_Imc2dsf : *objout=PPan2Pan(fp, new Imc2dsf);break;

     case Po_Imc3duc : *objout=PPan2Pan(fp, new Imc3duc);break;	   
     case Po_Imc3dsl : *objout=PPan2Pan(fp, new Imc3dsl);break;
     case Po_Imc3dsf : *objout=PPan2Pan(fp, new Imc3dsf);break;

     case Po_Imx2duc : *objout=PPan2Pan(fp, new Imx2duc);break;
     case Po_Imx2dsl : *objout=PPan2Pan(fp, new Imx2dsl);break;
     case Po_Imx2dsf : *objout=PPan2Pan(fp, new Imx2dsf);break;

     case Po_Imx3duc : *objout=PPan2Pan(fp, new Imx3duc);break;	   
     case Po_Imx3dsl : *objout=PPan2Pan(fp, new Imx3dsl);break;
     case Po_Imx3dsf : *objout=PPan2Pan(fp, new Imx3dsf);break;

     case Po_Reg2d   : *objout=PPan2Pan(fp, new Reg2d);break;
     case Po_Reg3d   : *objout=PPan2Pan(fp, new Reg3d);break;

     case Po_Graph2d : *objout=PPan2Pan(fp, new Graph2d);break;
     case Po_Graph3d : *objout=PPan2Pan(fp, new Graph3d);break;
     case Po_Collection : *objout=PPan2Pan(fp, new Collection);break;

     default : std::cerr<<"Error ppan2pan: Unknown Pandore file format"<<std::endl; return FAILURE;
     }
  }
  return SUCCESS;
}

#ifdef MAIN
/*
 * Modify only the following constants, and the function call.
 */
#define USAGE	"usage: %s [im_in|-] [im_out|-]"
#define PARC	1
#define FINC	0
#define FOUTC	1
#define MASK	0

int main( int argc, char *argv[] ) {
  Errc result;              // The result code of the execution.
  Pobject* mask;            // The mask of input objects.
  Pobject* objin[FINC+1];   // The input objects.
  Pobject* objs[FINC+1];    // The source objects masked.
  Pobject* objout[FOUTC+1]; // The output object.
  Pobject* objd[FOUTC+1];   // The result object of the execution.
  char* parv[PARC+1];      // The input parameters.
  
   if (argc < 2) {
      fprintf(stderr, USAGE"\n", argv[0]);
      Exit((Errc)FAILURE);
   }
   
   if ((argc == 2) && (!strcmp(argv[1], "-p"))) {
      fprintf(stdout, "%s 0 1 1\n", argv[0]);
      exit(0);
   }

   if ((argc == 2) && (!strcmp(argv[1], "-s"))) {
      fprintf(stdout, "\n");
      exit(0);
   }
 
  ReadArgs(argc, argv, PARC, FINC, FOUTC, &mask, objin, objs, objout, objd, parv, USAGE, MASK);

  // Check inputs.
  FILE * fdin;
  char * nomfichier;

  nomfichier = (argc>1)? argv[1] : NULL;
  if ((nomfichier != NULL) && (strcmp(nomfichier, "-"))) {
    if ((fdin = fopen(nomfichier, "rb")) == NULL) {
      fprintf(stderr, "Error ppan2pan: Cannot open file %s\n", nomfichier);
      Exit(FAILURE);
    }
  } else {
    fdin = stdin;
  }

  if ((result = PPan2Pan(fdin, &objd[0])) == SUCCESS) {
     WriteArgs(argc, argv, PARC, FINC, FOUTC, &mask, objin, objs, objout, objd, MASK);
  }
  
  if (fdin != stdin) {
    fclose(fdin);
  }
  Exit(result);
  return 0;
}

#endif
