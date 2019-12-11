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

/*
 * @author Alexandre Duret-Lutz - 1999-11-02
 * @author Régis Clouard - 2004-02-09 (fix bug)
 */

/**
 * @file pcolcatenateitem.cpp
 * Concatenate attributes of 2 collections.
 * If an attribute have the same in both collection
 * then an array is created with the values.
 */

#include <pandore.h>
using namespace pandore;

/**
 * Concatenate attributes of collection cs to collection  cd.
 */
Errc PColCatenateItem( const Collection& cs, Collection& cd ) {
   std::list< std::string > l = cs.List();
   std::list< std::string >::const_iterator i; 
   for ( i = l.begin(); i != l.end() ; ++i ) {
      BundledObject* d_obj = cs.Get(*i);
      BundledObject* d_arr = d_obj->ToArray(); 
      std::string type = d_arr->Type();
      std::string s = type;
      s.resize(5);

      if (cd.Exists(*i)) {
 	 BundledObject* c_obj = cd.Get(*i);
 	 BundledObject* c_arr = c_obj->ToArray();

	 if ((s == "Array" || s == "PArra") && type == c_arr->Type()) {
	    c_arr->Append(d_arr);
	    if (c_obj != c_arr) {
	       cd.Set(*i, c_arr);
	    }
	 } // else keep the currenmt item.
      } else {
	 cd.Set(*i, d_obj->Clone());
	 if (d_obj != d_arr) {
	    delete d_arr;
	 }
      }
   }
   return SUCCESS;
}

#ifdef MAIN

/*
 * Modify only the following constants, and the function call.
 */
#define	USAGE	"usage: %s [col_in1|-] [col_in2|-] [col_out|-]"
#define	PARC	0
#define	FINC	2
#define	FOUTC	1
#define	MASK	0

int main( int argc, char *argv[] ) {
  Errc	 result;              // The result code of the execution.
  Pobject* mask;	      // The region map.
  Pobject* objin[FINC + 1];   // The input objects.
  Pobject* objs[FINC + 1];    // The source objects masked by the mask.
  Pobject* objout[FOUTC + 1]; // The output object.
  Pobject* objd[FOUTC + 1];   // The result object of the execution.
  char* parv[PARC + 1];	      // The input parameters.
  
  ReadArgs(argc, argv, PARC, FINC, FOUTC, &mask, objin, objs, objout, objd, parv, USAGE, MASK);
  
  if ((objs[0]->Type() == Po_Collection) && (objs[1]->Type() == Po_Collection)) {
    Collection* ims1 = (Collection*)objs[0];
    Collection* ims2 = (Collection*)objs[1];

    objd[0] = new Collection;
    Collection* const imd = (Collection*)objd[0];
    
    result = (PColCatenateItem(*ims1, *imd) == SUCCESS)
       && (PColCatenateItem(*ims2, *imd) == SUCCESS);

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
