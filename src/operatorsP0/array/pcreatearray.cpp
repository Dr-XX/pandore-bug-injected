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
 * @author Alexandre Duret-Lutz - 1999-11-02
 * @author Régis Clouard - 2005-12-14 (allow size to be 0)
 */

#include <string.h>
#include <pandore.h>
using namespace pandore;

/**
 * @file pcreatearray.cpp
 * Creates a new array in a collection.
 */

#ifdef _MSC_VER
// Because Visual C++ does not support strcasecmp....
#include <ctype.h>
int strcasecmp( const char *s1, const char *s2 ) {
   int i;
   for (i = 0; s1[ i ] != '\0' && s2[ i ] != '\0'; ++i) {
      char ch1 = toupper(s1[i]);
      char ch2 = toupper(s2[i]);
      
      if (ch1 != ch2) {
	 return (int) (ch1 - ch2);
      }
   }
   
   return (int) (s1[i] - s2[i]);
}
#endif

/**
 * Cree un tableau de taille n, initialise a la valeur v
 */

Errc PCreateArray( Collection &col_out, Long size, Char value, std::string name ) {
   Char *t = new Char[size];
   
   for (int i = 0; i < size; ++i) {
      t[i] = value;
   }
   col_out.SETARRAY(name, Char, t, size);

   return SUCCESS;
}
Errc PCreateArray( Collection &col_out, Long size, Uchar value, std::string name ) {
   Uchar *t = new Uchar[size];
   
   for (int i = 0; i < size; ++i) {
      t[i] = value;
   }
   col_out.SETARRAY(name, Uchar, t, size);

   return SUCCESS;
}
Errc PCreateArray( Collection &col_out, Long size, Short value, std::string name ) {
   Short *t = new Short[size];
   
   for (int i = 0; i < size; ++i) {
      t[i] = value;
   }
   col_out.SETARRAY(name, Short, t, size);

   return SUCCESS;
}
Errc PCreateArray( Collection &col_out, Long size, Ushort value, std::string name ) {
   Ushort *t = new Ushort[size];
   
   for (int i = 0; i < size; ++i) {
      t[i] = value;
   }
   col_out.SETARRAY(name, Ushort, t, size);

   return SUCCESS;
}
Errc PCreateArray( Collection &col_out, Long size, Long value, std::string name ) {
   Long *t = new Long[size];
   
   for (int i = 0; i < size; ++i) {
      t[i] = value;
   }
   col_out.SETARRAY(name, Long, t, size);

   return SUCCESS;
}
Errc PCreateArray( Collection &col_out, Long size, Ulong value, std::string name ) {
   Ulong *t = new Ulong[size];
   
   for (int i = 0; i < size; ++i) {
      t[i] = value;
   }
   col_out.SETARRAY(name, Ulong, t, size);

   return SUCCESS;
}
Errc PCreateArray( Collection &col_out, Long size, Llong value, std::string name ) {
   Llong *t = new Llong[size];
   
   for (int i = 0; i < size; ++i) {
      t[i] = value;
   }
   col_out.SETARRAY(name, Llong, t, size);

   return SUCCESS;
}
Errc PCreateArray( Collection &col_out, Long size, Ullong value, std::string name ) {
   Ullong *t = new Ullong[size];
   
   for (int i = 0; i < size; ++i) {
      t[i] = value;
   }
   col_out.SETARRAY(name, Ullong, t, size);

   return SUCCESS;
}
Errc PCreateArray( Collection &col_out, Long size, Float value, std::string name ) {
   Float *t = new Float[size];
   
   for (int i = 0; i < size; ++i) {
      t[i] = value;
   }
   col_out.SETARRAY(name, Float, t, size);

   return SUCCESS;
}
Errc PCreateArray( Collection &col_out, Long size, Double value, std::string name ) {
   Double *t = new Double[size];
   
   for (int i = 0; i < size; ++i) {
      t[i] = value;
   }
   col_out.SETARRAY(name, Double, t, size);

   return SUCCESS;
}

#define	USAGE	"usage: %s name type size value [col_out|-]"
#define	PARC	4
#define	FINC	0
#define	FOUTC	1
#define	MASK	0

int main( int argc, char *argv[] ) {
   Errc result;                // The result code of the execution.
   Pobject* mask;              // The region map.
   Pobject* objin[0 + 1];   // The input objects.
   Pobject* objs[0 + 1];    // The source objects masked.
   Pobject* objout[1 + 1]; // The output object.
   Pobject* objd[1 + 1];   // The result object of the execution.
   char* parv[4 + 1];       // The input parameters.

   ReadArgs(argc, argv, 4, 0, 1, &mask, objin, objs, objout, objd, parv, USAGE, 0); 
   if (!strcasecmp(argv[2], "Char")) {
      objd[0] = new Collection;
      Collection* const imd = (Collection*)objd[0];
      result = PCreateArray(*imd, atol(parv[2]), (Char)atof(parv[3]), argv[1]);
      goto end;
   }
   if (!strcasecmp(argv[2], "Uchar")) {
      objd[0] = new Collection;
      Collection* const imd = (Collection*)objd[0];
      result = PCreateArray(*imd, atol(parv[2]), (Uchar)atof(parv[3]), argv[1]);
      goto end;
   }
   if (!strcasecmp(argv[2], "Short")) {
      objd[0] = new Collection;
      Collection* const imd = (Collection*)objd[0];
      result = PCreateArray(*imd, atol(parv[2]), (Short)atof(parv[3]), argv[1]);
      goto end;
   }
   if (!strcasecmp(argv[2], "Ushort")) {
      objd[0] = new Collection;
      Collection* const imd = (Collection*)objd[0];
      result = PCreateArray(*imd, atol(parv[2]), (Ushort)atof(parv[3]), argv[1]);
      goto end;
   }
   if (!strcasecmp(argv[2], "Long")) {
      objd[0] = new Collection;
      Collection* const imd = (Collection*)objd[0];
      result = PCreateArray(*imd, atol(parv[2]), (Long)atof(parv[3]), argv[1]);
      goto end;
   }
   if (!strcasecmp(argv[2], "Ulong")) {
      objd[0] = new Collection;
      Collection* const imd = (Collection*)objd[0];
      result = PCreateArray(*imd, atol(parv[2]), (Ulong)atof(parv[3]), argv[1]);
      goto end;
   }
   if (!strcasecmp(argv[2], "Llong")) {
      objd[0] = new Collection;
      Collection* const imd = (Collection*)objd[0];
      result = PCreateArray(*imd, atol(parv[2]), (Llong)atof(parv[3]), argv[1]);
      goto end;
   }
   if (!strcasecmp(argv[2], "Ullong")) {
      objd[0] = new Collection;
      Collection* const imd = (Collection*)objd[0];
      result = PCreateArray(*imd, atol(parv[2]), (Ullong)atof(parv[3]), argv[1]);
      goto end;
   }
   if (!strcasecmp(argv[2], "Float")) {
      objd[0] = new Collection;
      Collection* const imd = (Collection*)objd[0];
      result = PCreateArray(*imd, atol(parv[2]), (Float)atof(parv[3]), argv[1]);
      goto end;
   }
   if (!strcasecmp(argv[2], "Double")) {
      objd[0] = new Collection;
      Collection* const imd = (Collection*)objd[0];
      result = PCreateArray(*imd, atol(parv[2]), (Double)atof(parv[3]), argv[1]);
      goto end;
   }
  {
     PrintErrorFormat(objin, 0, argv); 
     result = FAILURE; 
  }	

end:
  if (result) {
	WriteArgs(argc, argv, 4, 0, 1, &mask, objin, objs, objout, objd, 0); 
  }
  Exit(result); 
  return 0; 
}
