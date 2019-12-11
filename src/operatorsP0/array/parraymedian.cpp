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
 * Comparison function.
 */
int compare( const double *val1,  const double *val2 ) {
   if (*val1 > *val2) {
      return 1;
   }  else if (*val1 < *val2) {
      return -1;
   } else {
      return 0;
   }
}

/**
 * PMedianValue uses qsort: the median value is at the middle position
 * of the pixel array.
 */
double PMedianValue( double* data, Long size ) {
   qsort(data, size, sizeof(double), (int (*)(const void*,  const void*))compare);
   
   if (size % 2) { // Odd number of elements.
      return data[size / 2];
   } else { // Even number of elements.
      return (data[size / 2 - 1 ] + data[size / 2]) / 2.0;
   }
}

Errc PArrayMedian(  const Collection &col_in, Collection &col_out, 
		  const std::string &name_in, const std::string &name_out ) {
   std::string type = col_in.GetType(name_in);
   if (type == "Array:Char") {
      Char * initialArray = col_in.GETARRAY(name_in, Char);
      Long size = col_in.GETARRAYSIZE(name_in, Char);
      double *array =  new double[size];
      
      for (int i = 0; i < size; ++i) {
	 array[i] = (double)initialArray[i];
      }
      Char value = (Char)PMedianValue(array, size);
      col_out.SETVALUE(name_out, Char, value);
   } else
   if (type == "Array:Uchar") {
      Uchar * initialArray = col_in.GETARRAY(name_in, Uchar);
      Long size = col_in.GETARRAYSIZE(name_in, Uchar);
      double *array =  new double[size];
      
      for (int i = 0; i < size; ++i) {
	 array[i] = (double)initialArray[i];
      }
      Uchar value = (Uchar)PMedianValue(array, size);
      col_out.SETVALUE(name_out, Uchar, value);
   } else
   if (type == "Array:Short") {
      Short * initialArray = col_in.GETARRAY(name_in, Short);
      Long size = col_in.GETARRAYSIZE(name_in, Short);
      double *array =  new double[size];
      
      for (int i = 0; i < size; ++i) {
	 array[i] = (double)initialArray[i];
      }
      Short value = (Short)PMedianValue(array, size);
      col_out.SETVALUE(name_out, Short, value);
   } else
   if (type == "Array:Ushort") {
      Ushort * initialArray = col_in.GETARRAY(name_in, Ushort);
      Long size = col_in.GETARRAYSIZE(name_in, Ushort);
      double *array =  new double[size];
      
      for (int i = 0; i < size; ++i) {
	 array[i] = (double)initialArray[i];
      }
      Ushort value = (Ushort)PMedianValue(array, size);
      col_out.SETVALUE(name_out, Ushort, value);
   } else
   if (type == "Array:Long") {
      Long * initialArray = col_in.GETARRAY(name_in, Long);
      Long size = col_in.GETARRAYSIZE(name_in, Long);
      double *array =  new double[size];
      
      for (int i = 0; i < size; ++i) {
	 array[i] = (double)initialArray[i];
      }
      Long value = (Long)PMedianValue(array, size);
      col_out.SETVALUE(name_out, Long, value);
   } else
   if (type == "Array:Ulong") {
      Ulong * initialArray = col_in.GETARRAY(name_in, Ulong);
      Long size = col_in.GETARRAYSIZE(name_in, Ulong);
      double *array =  new double[size];
      
      for (int i = 0; i < size; ++i) {
	 array[i] = (double)initialArray[i];
      }
      Ulong value = (Ulong)PMedianValue(array, size);
      col_out.SETVALUE(name_out, Ulong, value);
   } else
   if (type == "Array:Llong") {
      Llong * initialArray = col_in.GETARRAY(name_in, Llong);
      Long size = col_in.GETARRAYSIZE(name_in, Llong);
      double *array =  new double[size];
      
      for (int i = 0; i < size; ++i) {
	 array[i] = (double)initialArray[i];
      }
      Llong value = (Llong)PMedianValue(array, size);
      col_out.SETVALUE(name_out, Llong, value);
   } else
   if (type == "Array:Ullong") {
      Ullong * initialArray = col_in.GETARRAY(name_in, Ullong);
      Long size = col_in.GETARRAYSIZE(name_in, Ullong);
      double *array =  new double[size];
      
      for (int i = 0; i < size; ++i) {
	 array[i] = (double)initialArray[i];
      }
      Ullong value = (Ullong)PMedianValue(array, size);
      col_out.SETVALUE(name_out, Ullong, value);
   } else
   if (type == "Array:Float") {
      Float * initialArray = col_in.GETARRAY(name_in, Float);
      Long size = col_in.GETARRAYSIZE(name_in, Float);
      double *array =  new double[size];
      
      for (int i = 0; i < size; ++i) {
	 array[i] = (double)initialArray[i];
      }
      Float value = (Float)PMedianValue(array, size);
      col_out.SETVALUE(name_out, Float, value);
   } else
   if (type == "Array:Double") {
      Double * initialArray = col_in.GETARRAY(name_in, Double);
      Long size = col_in.GETARRAYSIZE(name_in, Double);
      double *array =  new double[size];
      
      for (int i = 0; i < size; ++i) {
	 array[i] = (double)initialArray[i];
      }
      Double value = (Double)PMedianValue(array, size);
      col_out.SETVALUE(name_out, Double, value);
   } else
      {
	 std::cerr << "Error parraymedian: Invalid collection type" << std::endl;
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
      result = PArrayMedian(*ims1, *imd1, parv[0], parv[1]);
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
