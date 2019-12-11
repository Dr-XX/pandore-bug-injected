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
 * @author Regis Clouard - 2003-02-14
 */

#include <pandore.h>
using namespace pandore;

/**
 * @file parraygetvalue.cpp
 * Return an item value from an array stored in a collection.
 */
Errc PArrayGetValue( const Collection &col_in, const std::string &name_in, int index ) {
  std::string type = col_in.GetType(name_in);
  if (type == "Array:Char") {
    Char* in = col_in.GETARRAY(name_in, Char);
    Long nElem = col_in.GETARRAYSIZE(name_in, Char);
    if (index < 0 || index >= nElem) {
       return FAILURE;
    }
    return in[index];
  } else
  if (type == "Array:Uchar") {
    Uchar* in = col_in.GETARRAY(name_in, Uchar);
    Long nElem = col_in.GETARRAYSIZE(name_in, Uchar);
    if (index < 0 || index >= nElem) {
       return FAILURE;
    }
    return in[index];
  } else
  if (type == "Array:Short") {
    Short* in = col_in.GETARRAY(name_in, Short);
    Long nElem = col_in.GETARRAYSIZE(name_in, Short);
    if (index < 0 || index >= nElem) {
       return FAILURE;
    }
    return in[index];
  } else
  if (type == "Array:Ushort") {
    Ushort* in = col_in.GETARRAY(name_in, Ushort);
    Long nElem = col_in.GETARRAYSIZE(name_in, Ushort);
    if (index < 0 || index >= nElem) {
       return FAILURE;
    }
    return in[index];
  } else
  if (type == "Array:Long") {
    Long* in = col_in.GETARRAY(name_in, Long);
    Long nElem = col_in.GETARRAYSIZE(name_in, Long);
    if (index < 0 || index >= nElem) {
       return FAILURE;
    }
    return in[index];
  } else
  if (type == "Array:Ulong") {
    Ulong* in = col_in.GETARRAY(name_in, Ulong);
    Long nElem = col_in.GETARRAYSIZE(name_in, Ulong);
    if (index < 0 || index >= nElem) {
       return FAILURE;
    }
    return in[index];
  } else
  if (type == "Array:Llong") {
    Llong* in = col_in.GETARRAY(name_in, Llong);
    Long nElem = col_in.GETARRAYSIZE(name_in, Llong);
    if (index < 0 || index >= nElem) {
       return FAILURE;
    }
    return in[index];
  } else
  if (type == "Array:Ullong") {
    Ullong* in = col_in.GETARRAY(name_in, Ullong);
    Long nElem = col_in.GETARRAYSIZE(name_in, Ullong);
    if (index < 0 || index >= nElem) {
       return FAILURE;
    }
    return in[index];
  } else
  if (type == "Array:Float") {
    Float* in = col_in.GETARRAY(name_in, Float);
    Long nElem = col_in.GETARRAYSIZE(name_in, Float);
    if (index < 0 || index >= nElem) {
       return FAILURE;
    }
    return in[index];
  } else
  if (type == "Array:Double") {
    Double* in = col_in.GETARRAY(name_in, Double);
    Long nElem = col_in.GETARRAYSIZE(name_in, Double);
    if (index < 0 || index >= nElem) {
       return FAILURE;
    }
    return in[index];
  } else
     {
	std::cerr << "Error parraygetvalue: Invalid collection type" << std::endl;
	return FAILURE;
     }
  return SUCCESS;
}

#ifdef MAIN

/*
 * Modify only the following constants, and the function call.
 */
#define	USAGE	"usage: %s index name [col_in|-]"
#define	PARC	2
#define	FINC	1
#define	FOUTC	0
#define	MASK	0

int main( int argc, char *argv[] ) {
   Errc result;              // The result code of the execution.
   Pobject* mask;            // The region mask.
   Pobject* objin[FINC+1];   // The input objects;
   Pobject* objs[FINC+1];    // The source objects masked by the mask.
   Pobject* objout[FOUTC+1]; // The output object.
   Pobject* objd[FOUTC+1];   // The result object of the execution.
   char* parv[PARC+1];       // The input parameters.
   
   ReadArgs(argc, argv, PARC, FINC, FOUTC, &mask, objin, objs, objout, objd, parv, USAGE, MASK);

   if (objs[0]->Type() == Po_Collection) {
      Collection* ims1 = (Collection*)objs[0];
      result = PArrayGetValue(*ims1, parv[1], atoi(parv[0]));
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
