/* -*- c-basic-offset: 3; mode:c++ -*-
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
 */

#include <pandore.h>
using namespace pandore;

/**
 * @file parraymean.cpp
 * Calcul de la moyenne d'un tableau dans une collection
 */

Char Moyenne( const Char* p, Long pSize ) {
   Char somme = 0;
   
   for (int pos = 0; pos < pSize; ++pos) {
      somme += p[pos];
   }
   
   return (Char)(somme / pSize);
}

Uchar Moyenne( const Uchar* p, Long pSize ) {
   Uchar somme = 0;
   
   for (int pos = 0; pos < pSize; ++pos) {
      somme += p[pos];
   }
   
   return (Uchar)(somme / pSize);
}

Short Moyenne( const Short* p, Long pSize ) {
   Short somme = 0;
   
   for (int pos = 0; pos < pSize; ++pos) {
      somme += p[pos];
   }
   
   return (Short)(somme / pSize);
}

Ushort Moyenne( const Ushort* p, Long pSize ) {
   Ushort somme = 0;
   
   for (int pos = 0; pos < pSize; ++pos) {
      somme += p[pos];
   }
   
   return (Ushort)(somme / pSize);
}

Long Moyenne( const Long* p, Long pSize ) {
   Long somme = 0;
   
   for (int pos = 0; pos < pSize; ++pos) {
      somme += p[pos];
   }
   
   return (Long)(somme / pSize);
}

Ulong Moyenne( const Ulong* p, Long pSize ) {
   Ulong somme = 0;
   
   for (int pos = 0; pos < pSize; ++pos) {
      somme += p[pos];
   }
   
   return (Ulong)(somme / pSize);
}

Llong Moyenne( const Llong* p, Long pSize ) {
   Llong somme = 0;
   
   for (int pos = 0; pos < pSize; ++pos) {
      somme += p[pos];
   }
   
   return (Llong)(somme / pSize);
}

Ullong Moyenne( const Ullong* p, Long pSize ) {
   Ullong somme = 0;
   
   for (int pos = 0; pos < pSize; ++pos) {
      somme += p[pos];
   }
   
   return (Ullong)(somme / pSize);
}

Float Moyenne( const Float* p, Long pSize ) {
   Float somme = 0;
   
   for (int pos = 0; pos < pSize; ++pos) {
      somme += p[pos];
   }
   
   return (Float)(somme / pSize);
}

Double Moyenne( const Double* p, Long pSize ) {
   Double somme = 0;
   
   for (int pos = 0; pos < pSize; ++pos) {
      somme += p[pos];
   }
   
   return (Double)(somme / pSize);
}


Errc PArrayMean(  const Collection &col_in, Collection &col_out, 
		  const std::string &name_in, const std::string &name_out ) {
   std::string type = col_in.GetType(name_in);
   if (type == "Array:Char") {
      Char z =  Moyenne(col_in.GETARRAY(name_in, Char), col_in.GETARRAYSIZE(name_in, Char));
      col_out.SETVALUE(name_out, Char, z);
   } else
   if (type == "Array:Uchar") {
      Uchar z =  Moyenne(col_in.GETARRAY(name_in, Uchar), col_in.GETARRAYSIZE(name_in, Uchar));
      col_out.SETVALUE(name_out, Uchar, z);
   } else
   if (type == "Array:Short") {
      Short z =  Moyenne(col_in.GETARRAY(name_in, Short), col_in.GETARRAYSIZE(name_in, Short));
      col_out.SETVALUE(name_out, Short, z);
   } else
   if (type == "Array:Ushort") {
      Ushort z =  Moyenne(col_in.GETARRAY(name_in, Ushort), col_in.GETARRAYSIZE(name_in, Ushort));
      col_out.SETVALUE(name_out, Ushort, z);
   } else
   if (type == "Array:Long") {
      Long z =  Moyenne(col_in.GETARRAY(name_in, Long), col_in.GETARRAYSIZE(name_in, Long));
      col_out.SETVALUE(name_out, Long, z);
   } else
   if (type == "Array:Ulong") {
      Ulong z =  Moyenne(col_in.GETARRAY(name_in, Ulong), col_in.GETARRAYSIZE(name_in, Ulong));
      col_out.SETVALUE(name_out, Ulong, z);
   } else
   if (type == "Array:Llong") {
      Llong z =  Moyenne(col_in.GETARRAY(name_in, Llong), col_in.GETARRAYSIZE(name_in, Llong));
      col_out.SETVALUE(name_out, Llong, z);
   } else
   if (type == "Array:Ullong") {
      Ullong z =  Moyenne(col_in.GETARRAY(name_in, Ullong), col_in.GETARRAYSIZE(name_in, Ullong));
      col_out.SETVALUE(name_out, Ullong, z);
   } else
   if (type == "Array:Float") {
      Float z =  Moyenne(col_in.GETARRAY(name_in, Float), col_in.GETARRAYSIZE(name_in, Float));
      col_out.SETVALUE(name_out, Float, z);
   } else
   if (type == "Array:Double") {
      Double z =  Moyenne(col_in.GETARRAY(name_in, Double), col_in.GETARRAYSIZE(name_in, Double));
      col_out.SETVALUE(name_out, Double, z);
   } else
      {
	 std::cerr << "Error parraymean: Invalid collection type" << std::endl;
	 return FAILURE;
      }

   return SUCCESS;
}

#ifdef MAIN

/*
 * Modify only the following constants, and the function call.
 */
#define	USAGE	"usage: %s name-in name-out [col_in|-] [col_out|-]"
#define	PARC	2
#define	FINC	1
#define	FOUTC	1
#define	MASK	0

int main( int argc, char *argv[] ) {
   Errc  result;               // The result code of the execution.
   Pobject* mask;              // The region mask.
   Pobject* objin[FINC + 1];   // The input objects;
   Pobject* objs[FINC + 1];    // The source objects masked by the mask.
   Pobject* objout[FOUTC + 1]; // The output object.
   Pobject* objd[FOUTC + 1];   // The result object of the execution.
   char* parv[PARC + 1];       // The input parameters.

   ReadArgs(argc, argv, PARC, FINC, FOUTC, &mask, objin, objs, objout, objd, parv, USAGE, MASK);

   if (objs[0]->Type() == Po_Collection) {
      Collection* ims1 = (Collection*)objs[0];
      Collection* imd1 = new Collection;
      objd[0] = imd1;
      result = PArrayMean(*ims1, *imd1, parv[0], parv[1]);
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
