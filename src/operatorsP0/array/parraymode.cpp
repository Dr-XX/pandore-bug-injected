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

#include <cmath>
#include <pandore.h>
using namespace pandore;

/**
 * @file parraymode.cpp
 * Calcul de la moyenne d'un tableau dans une collection
 */

Char PArrayMode( const Char* p, Long pSize, int precision ) {
   std::map< int, long > bins;

   Char min = p[0];
   for (int pos = 1; pos < pSize; ++pos) {
      if (min > p[pos]) {
	 min = p[pos];
      }
   }

   for (int pos = 0; pos < pSize; ++pos) {
      ++bins[ (int)((p[pos] - min) / precision)];
   }

   std::map< int, long >::const_iterator h;
   h = bins.begin();
   int mode = h->first;
   for (; h != bins.end(); h++) {
      if (bins[mode] < bins[h->first]) {
	 mode = h->first;
      }
   }

   bins.clear();
   
   return (Char)((mode + 0.5) * precision + min);
}

Uchar PArrayMode( const Uchar* p, Long pSize, int precision ) {
   std::map< int, long > bins;

   Uchar min = p[0];
   for (int pos = 1; pos < pSize; ++pos) {
      if (min > p[pos]) {
	 min = p[pos];
      }
   }

   for (int pos = 0; pos < pSize; ++pos) {
      ++bins[ (int)((p[pos] - min) / precision)];
   }

   std::map< int, long >::const_iterator h;
   h = bins.begin();
   int mode = h->first;
   for (; h != bins.end(); h++) {
      if (bins[mode] < bins[h->first]) {
	 mode = h->first;
      }
   }

   bins.clear();
   
   return (Uchar)((mode + 0.5) * precision + min);
}

Short PArrayMode( const Short* p, Long pSize, int precision ) {
   std::map< int, long > bins;

   Short min = p[0];
   for (int pos = 1; pos < pSize; ++pos) {
      if (min > p[pos]) {
	 min = p[pos];
      }
   }

   for (int pos = 0; pos < pSize; ++pos) {
      ++bins[ (int)((p[pos] - min) / precision)];
   }

   std::map< int, long >::const_iterator h;
   h = bins.begin();
   int mode = h->first;
   for (; h != bins.end(); h++) {
      if (bins[mode] < bins[h->first]) {
	 mode = h->first;
      }
   }

   bins.clear();
   
   return (Short)((mode + 0.5) * precision + min);
}

Ushort PArrayMode( const Ushort* p, Long pSize, int precision ) {
   std::map< int, long > bins;

   Ushort min = p[0];
   for (int pos = 1; pos < pSize; ++pos) {
      if (min > p[pos]) {
	 min = p[pos];
      }
   }

   for (int pos = 0; pos < pSize; ++pos) {
      ++bins[ (int)((p[pos] - min) / precision)];
   }

   std::map< int, long >::const_iterator h;
   h = bins.begin();
   int mode = h->first;
   for (; h != bins.end(); h++) {
      if (bins[mode] < bins[h->first]) {
	 mode = h->first;
      }
   }

   bins.clear();
   
   return (Ushort)((mode + 0.5) * precision + min);
}

Long PArrayMode( const Long* p, Long pSize, int precision ) {
   std::map< int, long > bins;

   Long min = p[0];
   for (int pos = 1; pos < pSize; ++pos) {
      if (min > p[pos]) {
	 min = p[pos];
      }
   }

   for (int pos = 0; pos < pSize; ++pos) {
      ++bins[ (int)((p[pos] - min) / precision)];
   }

   std::map< int, long >::const_iterator h;
   h = bins.begin();
   int mode = h->first;
   for (; h != bins.end(); h++) {
      if (bins[mode] < bins[h->first]) {
	 mode = h->first;
      }
   }

   bins.clear();
   
   return (Long)((mode + 0.5) * precision + min);
}

Ulong PArrayMode( const Ulong* p, Long pSize, int precision ) {
   std::map< int, long > bins;

   Ulong min = p[0];
   for (int pos = 1; pos < pSize; ++pos) {
      if (min > p[pos]) {
	 min = p[pos];
      }
   }

   for (int pos = 0; pos < pSize; ++pos) {
      ++bins[ (int)((p[pos] - min) / precision)];
   }

   std::map< int, long >::const_iterator h;
   h = bins.begin();
   int mode = h->first;
   for (; h != bins.end(); h++) {
      if (bins[mode] < bins[h->first]) {
	 mode = h->first;
      }
   }

   bins.clear();
   
   return (Ulong)((mode + 0.5) * precision + min);
}

Llong PArrayMode( const Llong* p, Long pSize, int precision ) {
   std::map< int, long > bins;

   Llong min = p[0];
   for (int pos = 1; pos < pSize; ++pos) {
      if (min > p[pos]) {
	 min = p[pos];
      }
   }

   for (int pos = 0; pos < pSize; ++pos) {
      ++bins[ (int)((p[pos] - min) / precision)];
   }

   std::map< int, long >::const_iterator h;
   h = bins.begin();
   int mode = h->first;
   for (; h != bins.end(); h++) {
      if (bins[mode] < bins[h->first]) {
	 mode = h->first;
      }
   }

   bins.clear();
   
   return (Llong)((mode + 0.5) * precision + min);
}

Ullong PArrayMode( const Ullong* p, Long pSize, int precision ) {
   std::map< int, long > bins;

   Ullong min = p[0];
   for (int pos = 1; pos < pSize; ++pos) {
      if (min > p[pos]) {
	 min = p[pos];
      }
   }

   for (int pos = 0; pos < pSize; ++pos) {
      ++bins[ (int)((p[pos] - min) / precision)];
   }

   std::map< int, long >::const_iterator h;
   h = bins.begin();
   int mode = h->first;
   for (; h != bins.end(); h++) {
      if (bins[mode] < bins[h->first]) {
	 mode = h->first;
      }
   }

   bins.clear();
   
   return (Ullong)((mode + 0.5) * precision + min);
}

Float PArrayMode( const Float* p, Long pSize, int precision ) {
   std::map< int, long > bins;

   Float min = p[0];
   for (int pos = 1; pos < pSize; ++pos) {
      if (min > p[pos]) {
	 min = p[pos];
      }
   }

   for (int pos = 0; pos < pSize; ++pos) {
      ++bins[ (int)((p[pos] - min) / precision)];
   }

   std::map< int, long >::const_iterator h;
   h = bins.begin();
   int mode = h->first;
   for (; h != bins.end(); h++) {
      if (bins[mode] < bins[h->first]) {
	 mode = h->first;
      }
   }

   bins.clear();
   
   return (Float)((mode + 0.5) * precision + min);
}

Double PArrayMode( const Double* p, Long pSize, int precision ) {
   std::map< int, long > bins;

   Double min = p[0];
   for (int pos = 1; pos < pSize; ++pos) {
      if (min > p[pos]) {
	 min = p[pos];
      }
   }

   for (int pos = 0; pos < pSize; ++pos) {
      ++bins[ (int)((p[pos] - min) / precision)];
   }

   std::map< int, long >::const_iterator h;
   h = bins.begin();
   int mode = h->first;
   for (; h != bins.end(); h++) {
      if (bins[mode] < bins[h->first]) {
	 mode = h->first;
      }
   }

   bins.clear();
   
   return (Double)((mode + 0.5) * precision + min);
}


Errc PArrayMode(  const Collection &col_in, Collection &col_out, 
		  const std::string &name_in, const std::string &name_out,
		  int precision ) {
   std::string type = col_in.GetType(name_in);
   if (type == "Array:Char") {
      Char value = PArrayMode(col_in.GETARRAY(name_in, Char), col_in.GETARRAYSIZE(name_in, Char), precision);
      col_out.SETVALUE(name_out, Char, value);
   } else
   if (type == "Array:Uchar") {
      Uchar value = PArrayMode(col_in.GETARRAY(name_in, Uchar), col_in.GETARRAYSIZE(name_in, Uchar), precision);
      col_out.SETVALUE(name_out, Uchar, value);
   } else
   if (type == "Array:Short") {
      Short value = PArrayMode(col_in.GETARRAY(name_in, Short), col_in.GETARRAYSIZE(name_in, Short), precision);
      col_out.SETVALUE(name_out, Short, value);
   } else
   if (type == "Array:Ushort") {
      Ushort value = PArrayMode(col_in.GETARRAY(name_in, Ushort), col_in.GETARRAYSIZE(name_in, Ushort), precision);
      col_out.SETVALUE(name_out, Ushort, value);
   } else
   if (type == "Array:Long") {
      Long value = PArrayMode(col_in.GETARRAY(name_in, Long), col_in.GETARRAYSIZE(name_in, Long), precision);
      col_out.SETVALUE(name_out, Long, value);
   } else
   if (type == "Array:Ulong") {
      Ulong value = PArrayMode(col_in.GETARRAY(name_in, Ulong), col_in.GETARRAYSIZE(name_in, Ulong), precision);
      col_out.SETVALUE(name_out, Ulong, value);
   } else
   if (type == "Array:Llong") {
      Llong value = PArrayMode(col_in.GETARRAY(name_in, Llong), col_in.GETARRAYSIZE(name_in, Llong), precision);
      col_out.SETVALUE(name_out, Llong, value);
   } else
   if (type == "Array:Ullong") {
      Ullong value = PArrayMode(col_in.GETARRAY(name_in, Ullong), col_in.GETARRAYSIZE(name_in, Ullong), precision);
      col_out.SETVALUE(name_out, Ullong, value);
   } else
   if (type == "Array:Float") {
      Float value = PArrayMode(col_in.GETARRAY(name_in, Float), col_in.GETARRAYSIZE(name_in, Float), precision);
      col_out.SETVALUE(name_out, Float, value);
   } else
   if (type == "Array:Double") {
      Double value = PArrayMode(col_in.GETARRAY(name_in, Double), col_in.GETARRAYSIZE(name_in, Double), precision);
      col_out.SETVALUE(name_out, Double, value);
   } else
      {
	 std::cerr << "Error parraymode: Invalid collection type" << std::endl;
	 return FAILURE;
      }

   return SUCCESS;
}

#ifdef MAIN

/*
 * Modify only the following constants, and the function call.
 */
#define	USAGE	"usage: %s name-in name-out precision [col_in|-] [col_out|-]"
#define	PARC	3
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
      result = PArrayMode(*ims1, *imd1, parv[0], parv[1], atoi(parv[2]));
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
