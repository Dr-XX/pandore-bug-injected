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

#include <pandore.h>
using namespace pandore;

#if (defined _MSC_VER) && (!defined snprintf)
#define snprintf _snprintf
#endif

Errc PArraysMax(  const Collection &col_in, Collection &col_out, const std::string &name ) {
   Long nbrcomp;
   std::string type;
   Long nin;
   char tmp[255];
   
   if (!col_in.NbOf(name, type, nbrcomp, nin)) {
      std::cerr << "Error parraysnorm: Invalid collection" << std::endl;
      return FAILURE;
   }

   if (type == "Array:Char") {
      Char** in = col_in.GETNARRAYS(name, Char, nbrcomp, nin);
      for (int e = 0; e < nbrcomp; ++e) {
	 Char max = in[e][0];
	 for (int i = 1; i < nin; ++i) {
	    if (max < in[e][i]) {
	       max = in[e][i];
	    }
	 }
	 snprintf(tmp, sizeof(tmp), "%s.%d", name.c_str(), e + 1);
	 tmp[sizeof(tmp) - 1] = 0;
	 col_out.SETVALUE(tmp, Double, max);
      }
   } else
   if (type == "Array:Uchar") {
      Uchar** in = col_in.GETNARRAYS(name, Uchar, nbrcomp, nin);
      for (int e = 0; e < nbrcomp; ++e) {
	 Uchar max = in[e][0];
	 for (int i = 1; i < nin; ++i) {
	    if (max < in[e][i]) {
	       max = in[e][i];
	    }
	 }
	 snprintf(tmp, sizeof(tmp), "%s.%d", name.c_str(), e + 1);
	 tmp[sizeof(tmp) - 1] = 0;
	 col_out.SETVALUE(tmp, Double, max);
      }
   } else
   if (type == "Array:Short") {
      Short** in = col_in.GETNARRAYS(name, Short, nbrcomp, nin);
      for (int e = 0; e < nbrcomp; ++e) {
	 Short max = in[e][0];
	 for (int i = 1; i < nin; ++i) {
	    if (max < in[e][i]) {
	       max = in[e][i];
	    }
	 }
	 snprintf(tmp, sizeof(tmp), "%s.%d", name.c_str(), e + 1);
	 tmp[sizeof(tmp) - 1] = 0;
	 col_out.SETVALUE(tmp, Double, max);
      }
   } else
   if (type == "Array:Ushort") {
      Ushort** in = col_in.GETNARRAYS(name, Ushort, nbrcomp, nin);
      for (int e = 0; e < nbrcomp; ++e) {
	 Ushort max = in[e][0];
	 for (int i = 1; i < nin; ++i) {
	    if (max < in[e][i]) {
	       max = in[e][i];
	    }
	 }
	 snprintf(tmp, sizeof(tmp), "%s.%d", name.c_str(), e + 1);
	 tmp[sizeof(tmp) - 1] = 0;
	 col_out.SETVALUE(tmp, Double, max);
      }
   } else
   if (type == "Array:Long") {
      Long** in = col_in.GETNARRAYS(name, Long, nbrcomp, nin);
      for (int e = 0; e < nbrcomp; ++e) {
	 Long max = in[e][0];
	 for (int i = 1; i < nin; ++i) {
	    if (max < in[e][i]) {
	       max = in[e][i];
	    }
	 }
	 snprintf(tmp, sizeof(tmp), "%s.%d", name.c_str(), e + 1);
	 tmp[sizeof(tmp) - 1] = 0;
	 col_out.SETVALUE(tmp, Double, max);
      }
   } else
   if (type == "Array:Ulong") {
      Ulong** in = col_in.GETNARRAYS(name, Ulong, nbrcomp, nin);
      for (int e = 0; e < nbrcomp; ++e) {
	 Ulong max = in[e][0];
	 for (int i = 1; i < nin; ++i) {
	    if (max < in[e][i]) {
	       max = in[e][i];
	    }
	 }
	 snprintf(tmp, sizeof(tmp), "%s.%d", name.c_str(), e + 1);
	 tmp[sizeof(tmp) - 1] = 0;
	 col_out.SETVALUE(tmp, Double, max);
      }
   } else
   if (type == "Array:Llong") {
      Llong** in = col_in.GETNARRAYS(name, Llong, nbrcomp, nin);
      for (int e = 0; e < nbrcomp; ++e) {
	 Llong max = in[e][0];
	 for (int i = 1; i < nin; ++i) {
	    if (max < in[e][i]) {
	       max = in[e][i];
	    }
	 }
	 snprintf(tmp, sizeof(tmp), "%s.%d", name.c_str(), e + 1);
	 tmp[sizeof(tmp) - 1] = 0;
	 col_out.SETVALUE(tmp, Double, max);
      }
   } else
   if (type == "Array:Ullong") {
      Ullong** in = col_in.GETNARRAYS(name, Ullong, nbrcomp, nin);
      for (int e = 0; e < nbrcomp; ++e) {
	 Ullong max = in[e][0];
	 for (int i = 1; i < nin; ++i) {
	    if (max < in[e][i]) {
	       max = in[e][i];
	    }
	 }
	 snprintf(tmp, sizeof(tmp), "%s.%d", name.c_str(), e + 1);
	 tmp[sizeof(tmp) - 1] = 0;
	 col_out.SETVALUE(tmp, Double, max);
      }
   } else
   if (type == "Array:Float") {
      Float** in = col_in.GETNARRAYS(name, Float, nbrcomp, nin);
      for (int e = 0; e < nbrcomp; ++e) {
	 Float max = in[e][0];
	 for (int i = 1; i < nin; ++i) {
	    if (max < in[e][i]) {
	       max = in[e][i];
	    }
	 }
	 snprintf(tmp, sizeof(tmp), "%s.%d", name.c_str(), e + 1);
	 tmp[sizeof(tmp) - 1] = 0;
	 col_out.SETVALUE(tmp, Double, max);
      }
   } else
   if (type == "Array:Double") {
      Double** in = col_in.GETNARRAYS(name, Double, nbrcomp, nin);
      for (int e = 0; e < nbrcomp; ++e) {
	 Double max = in[e][0];
	 for (int i = 1; i < nin; ++i) {
	    if (max < in[e][i]) {
	       max = in[e][i];
	    }
	 }
	 snprintf(tmp, sizeof(tmp), "%s.%d", name.c_str(), e + 1);
	 tmp[sizeof(tmp) - 1] = 0;
	 col_out.SETVALUE(tmp, Double, max);
      }
   } else
   {
      std::cerr << "Error parraysmax: Invalid collection type" << std::endl;     
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
      result = PArraysMax(*ims1, *imd1, parv[0]);
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
