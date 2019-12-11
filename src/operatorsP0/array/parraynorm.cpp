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
 */

#include <pandore.h>
using namespace pandore;

/**
 * @file parraynorm.cpp
 * Convert the type of value to double, and
 * normalize values between 0, 1.
 */

Errc PArrayNorm( Collection &col_in_out, const std::string &name ) {
  std::string type = col_in_out.GetType(name);
  if (type == "Array:Char") {
    Char* in = col_in_out.GETARRAY(name, Char);
    Long nElem = col_in_out.GETARRAYSIZE(name, Char);
    Double* out = new Double[nElem];
    for (Long e = 0; e< nElem; ++e) {
      out[e] = (Double)in[e] / (Double)Limits<Char>::max();
    }
    col_in_out.SETARRAY(name, Double, out, nElem);
  } else
  if (type == "Array:Uchar") {
    Uchar* in = col_in_out.GETARRAY(name, Uchar);
    Long nElem = col_in_out.GETARRAYSIZE(name, Uchar);
    Double* out = new Double[nElem];
    for (Long e = 0; e< nElem; ++e) {
      out[e] = (Double)in[e] / (Double)Limits<Uchar>::max();
    }
    col_in_out.SETARRAY(name, Double, out, nElem);
  } else
  if (type == "Array:Short") {
    Short* in = col_in_out.GETARRAY(name, Short);
    Long nElem = col_in_out.GETARRAYSIZE(name, Short);
    Double* out = new Double[nElem];
    for (Long e = 0; e< nElem; ++e) {
      out[e] = (Double)in[e] / (Double)Limits<Short>::max();
    }
    col_in_out.SETARRAY(name, Double, out, nElem);
  } else
  if (type == "Array:Ushort") {
    Ushort* in = col_in_out.GETARRAY(name, Ushort);
    Long nElem = col_in_out.GETARRAYSIZE(name, Ushort);
    Double* out = new Double[nElem];
    for (Long e = 0; e< nElem; ++e) {
      out[e] = (Double)in[e] / (Double)Limits<Ushort>::max();
    }
    col_in_out.SETARRAY(name, Double, out, nElem);
  } else
  if (type == "Array:Long") {
    Long* in = col_in_out.GETARRAY(name, Long);
    Long nElem = col_in_out.GETARRAYSIZE(name, Long);
    Double* out = new Double[nElem];
    for (Long e = 0; e< nElem; ++e) {
      out[e] = (Double)in[e] / (Double)Limits<Long>::max();
    }
    col_in_out.SETARRAY(name, Double, out, nElem);
  } else
  if (type == "Array:Ulong") {
    Ulong* in = col_in_out.GETARRAY(name, Ulong);
    Long nElem = col_in_out.GETARRAYSIZE(name, Ulong);
    Double* out = new Double[nElem];
    for (Long e = 0; e< nElem; ++e) {
      out[e] = (Double)in[e] / (Double)Limits<Ulong>::max();
    }
    col_in_out.SETARRAY(name, Double, out, nElem);
  } else
  if (type == "Array:Llong") {
    Llong* in = col_in_out.GETARRAY(name, Llong);
    Long nElem = col_in_out.GETARRAYSIZE(name, Llong);
    Double* out = new Double[nElem];
    for (Long e = 0; e< nElem; ++e) {
      out[e] = (Double)in[e] / (Double)Limits<Llong>::max();
    }
    col_in_out.SETARRAY(name, Double, out, nElem);
  } else
  if (type == "Array:Ullong") {
    Ullong* in = col_in_out.GETARRAY(name, Ullong);
    Long nElem = col_in_out.GETARRAYSIZE(name, Ullong);
    Double* out = new Double[nElem];
    for (Long e = 0; e< nElem; ++e) {
      out[e] = (Double)in[e] / (Double)Limits<Ullong>::max();
    }
    col_in_out.SETARRAY(name, Double, out, nElem);
  } else
  if (type == "Array:Float") {
    Float* in = col_in_out.GETARRAY(name, Float);
    Long nElem = col_in_out.GETARRAYSIZE(name, Float);
    Double* out = new Double[nElem];
    for (Long e = 0; e< nElem; ++e) {
      out[e] = (Double)in[e] / (Double)Limits<Float>::max();
    }
    col_in_out.SETARRAY(name, Double, out, nElem);
  } else
  if (type == "Array:Double") {
    Double* in = col_in_out.GETARRAY(name, Double);
    Long nElem = col_in_out.GETARRAYSIZE(name, Double);
    Double* out = new Double[nElem];
    for (Long e = 0; e< nElem; ++e) {
      out[e] = (Double)in[e] / (Double)Limits<Double>::max();
    }
    col_in_out.SETARRAY(name, Double, out, nElem);
  } else
     {
	std::cerr << "Error parraynorm: Invalid collection type" << std::endl;
	return FAILURE;
     }
  return SUCCESS;
}

#ifdef MAIN

/*
 * Modify only the following constants, and the function call.
 */
#define	USAGE	"usage: %s name [col_in|-] [col_out|-]"
#define	PARC	1
#define	FINC	1
#define	FOUTC	1
#define	MASK	0

int main( int argc, char *argv[] ) {
   Errc  result;             // The result code of the execution.
   Pobject* mask;            // The region mask.
   Pobject* objin[FINC+1];   // The input objects;
   Pobject* objs[FINC+1];    // The source objects masked by the mask.
   Pobject* objout[FOUTC+1]; // The output object.
   Pobject* objd[FOUTC+1];   // The result object of the execution.
   char* parv[PARC+1];      // The input parameters.
   
   ReadArgs(argc, argv, PARC, FINC, FOUTC, &mask, objin, objs, objout, objd, parv, USAGE, MASK);
   
   if (objs[0]->Type() == Po_Collection) {
      Collection* ims1 = (Collection*)objs[0];
      objd[0] = ims1;
      result = PArrayNorm(*ims1, parv[0]);
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
