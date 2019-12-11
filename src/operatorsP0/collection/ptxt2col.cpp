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
 * @author Alexandre Duret-Lutz - 1999-11-20
 * @author Christine Porquet - 2000-06-23
 */

/**
 * @file ptxt2col.cpp
 * Creation of a collection from a text file.
 */

#include <string.h>
#include <iostream>
#include <fstream>
//#include <algorithm>
#include <pandore.h>
using namespace pandore;

#ifdef _MSC_VER
// Because Visual C++ does not support strcasecmp....
#include <ctype.h>

int strcasecmp( const char *s1, const char *s2 ) {
   int i;
   for (i = 0; s1[i] != '\0' && s2[i] != '\0'; ++i) {
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
 * @param filename a string or "-" to use standard input.
 */
Errc PTxt2Col( const std::string& filename, Collection& c ) {
   std::istream *i;
   if (filename == "-")  {
      i = &std::cin;
   } else {
      i = new std::ifstream(filename.c_str());
   }
   char w[512];
   char nc[256];
   
   std::string n;
   *i >> w;
   w[sizeof(w) - 1] =0;

   while (*i) {
      if (!(*i >> nc)) {
	 goto error;
      }
      n = nc;
      if (!strcasecmp(w, "Char")) {
	 *i >> w;
	 if (!isdigit(w[0])) {
	    goto error;
	 }
	 Char r = (Char)atof(w);
	 c.SETVALUE(n, Char, r);
	 *i >> w;
      } else if (!strcasecmp(w, "Array:Char")) {
	 *i >> w;
	 if (isalpha(w[0])) {
	    Char *r = new Char[strlen(w)]; 
	    strncpy(r, w, strlen(w));
	    c.SETARRAY(n, Char, r, strlen(r));
	    *i >> w;
	 } else {
	    std::list<Char> l;
	    while (*i >> w && (isdigit(w[0]) || w[0]=='x' )) {
	       if (w[0] == 'x') {
		  Char t = l.front();
		  if (!(*i >> w)) {
		     goto error;
		  }
		  Long rep = atol(w);
		  while (--rep) l.push_back(t);
	       } else {
		  l.push_back((Char)atof(w));
	       }
	    }
	    Char* r = new Char[l.size()];
	    std::copy(l.begin(), l.end(), r);
	    c.SETARRAY(n, Char, r, l.size());
	    *i >> w;
	 }
      } else if (!strcasecmp(w, "Uchar")) {
	 *i >> w;
	 if (!isdigit(w[0])) {
	    goto error;
	 }
	 Uchar r = (Uchar)atof(w);
	 c.SETVALUE(n, Uchar, r);
	 *i >> w;
      } else if (!strcasecmp(w, "Array:Uchar")) {
	 std::list<Uchar> l;
	 while (*i >> w && (isdigit(w[0]) || w[0]=='x' )) {
	    if (w[0] == 'x') {
	       Uchar t = l.front();
	       if (!(*i >> w))  {
		  goto error;
	       }
	       Long rep = atol(w);
	       while (--rep) {
		  l.push_back(t);
	       }
	    } else {
	       l.push_back((Uchar)atof(w));
	    }
	 }
	 Uchar* r = new Uchar[l.size()];
	 c.SETARRAY(n, Uchar, r, l.size());
	 std::copy(l.begin(), l.end(), r);
      } else if (!strcasecmp(w, "Short")) {
	 *i >> w;
	 if (!isdigit(w[0])) {
	    goto error;
	 }
	 Short r = (Short)atof(w);
	 c.SETVALUE(n, Short, r);
	 *i >> w;
      } else if (!strcasecmp(w, "Array:Short")) {
	 std::list<Short> l;
	 while (*i >> w && (isdigit(w[0]) || w[0]=='x' )) {
	    if (w[0] == 'x') {
	       Short t = l.front();
	       if (!(*i >> w))  {
		  goto error;
	       }
	       Long rep = atol(w);
	       while (--rep) {
		  l.push_back(t);
	       }
	    } else {
	       l.push_back((Short)atof(w));
	    }
	 }
	 Short* r = new Short[l.size()];
	 c.SETARRAY(n, Short, r, l.size());
	 std::copy(l.begin(), l.end(), r);
      } else if (!strcasecmp(w, "Ushort")) {
	 *i >> w;
	 if (!isdigit(w[0])) {
	    goto error;
	 }
	 Ushort r = (Ushort)atof(w);
	 c.SETVALUE(n, Ushort, r);
	 *i >> w;
      } else if (!strcasecmp(w, "Array:Ushort")) {
	 std::list<Ushort> l;
	 while (*i >> w && (isdigit(w[0]) || w[0]=='x' )) {
	    if (w[0] == 'x') {
	       Ushort t = l.front();
	       if (!(*i >> w))  {
		  goto error;
	       }
	       Long rep = atol(w);
	       while (--rep) {
		  l.push_back(t);
	       }
	    } else {
	       l.push_back((Ushort)atof(w));
	    }
	 }
	 Ushort* r = new Ushort[l.size()];
	 c.SETARRAY(n, Ushort, r, l.size());
	 std::copy(l.begin(), l.end(), r);
      } else if (!strcasecmp(w, "Long")) {
	 *i >> w;
	 if (!isdigit(w[0])) {
	    goto error;
	 }
	 Long r = (Long)atof(w);
	 c.SETVALUE(n, Long, r);
	 *i >> w;
      } else if (!strcasecmp(w, "Array:Long")) {
	 std::list<Long> l;
	 while (*i >> w && (isdigit(w[0]) || w[0]=='x' )) {
	    if (w[0] == 'x') {
	       Long t = l.front();
	       if (!(*i >> w))  {
		  goto error;
	       }
	       Long rep = atol(w);
	       while (--rep) {
		  l.push_back(t);
	       }
	    } else {
	       l.push_back((Long)atof(w));
	    }
	 }
	 Long* r = new Long[l.size()];
	 c.SETARRAY(n, Long, r, l.size());
	 std::copy(l.begin(), l.end(), r);
      } else if (!strcasecmp(w, "Ulong")) {
	 *i >> w;
	 if (!isdigit(w[0])) {
	    goto error;
	 }
	 Ulong r = (Ulong)atof(w);
	 c.SETVALUE(n, Ulong, r);
	 *i >> w;
      } else if (!strcasecmp(w, "Array:Ulong")) {
	 std::list<Ulong> l;
	 while (*i >> w && (isdigit(w[0]) || w[0]=='x' )) {
	    if (w[0] == 'x') {
	       Ulong t = l.front();
	       if (!(*i >> w)) {
		  goto error;
	       }
	       Long rep = atol(w);
	       while (--rep) {
		  l.push_back(t);
	       }
	    } else {
		  l.push_back((Ulong)atof(w));
	    }
	 }
	 Ulong* r = new Ulong[l.size()];
	 c.SETARRAY(n, Ulong, r, l.size());
	 std::copy(l.begin(), l.end(), r);
      } else if (!strcasecmp(w, "Float")) {
	 *i >> w;
	 if (!isdigit(w[0])) {
	    goto error;
	 }
	 Float r = (Float)atof(w);
	 c.SETVALUE(n, Float, r);
	 *i >> w;
      } else if (!strcasecmp(w, "Array:Float")) {
	 std::list<Float> l;
	 while (*i >> w && (isdigit(w[0]) || w[0]=='x' )) {
	    if (w[0] == 'x') {
	       Float t = l.front();
	       if (!(*i >> w))  {
		  goto error;
	       }
	       Long rep = atol(w);
	       while (--rep) {
		  l.push_back(t);
	       }
	    } else
	       l.push_back((Float)atof(w));
	 }
	 Float* r = new Float[l.size()];
	 c.SETARRAY(n, Float, r, l.size());
	 std::copy(l.begin(), l.end(), r);
      } else if (!strcasecmp(w, "Double")) {
	 *i >> w;
	 if (!isdigit(w[0])) {
	    goto error;
	 }
	 Double r = (Double)atof(w);
	 c.SETVALUE(n, Double, r);
	 *i >> w;
      } else if (!strcasecmp(w, "Array:Double")) {
	 std::list<Double> l;
	 while (*i >> w && (isdigit(w[0]) || w[0]=='x' )) {
	    if (w[0] == 'x') {
	       Double t = l.front();
	       if (!(*i >> w))  {
		  goto error;
	       }
	       Long rep = atol(w);
	       while (--rep) {
		  l.push_back(t);
	       }
	    } else {
		  l.push_back((Double)atof(w));
	    }
	 }
	 Double* r = new Double[l.size()];
	 c.SETARRAY(n, Double, r, l.size());
	 std::copy(l.begin(), l.end(), r);
      } else {
	 std::cerr << "Error ptxt2col: Unsupported type: " << w << std::endl;
	 goto error;
      }
   }
   if (filename != "-") {
      delete i;
   }
   return SUCCESS;

error:
   if (filename != "-") {
      delete i;
   }
   return FAILURE;
   }
   
   
#ifdef MAIN
   
/*
 * Modify only the following constants, and the operator switches.
 */
#define USAGE	"usage: %s file_in [col_out|-]"
#define PARC	1
#define FINC	0
#define FOUTC	1
#define MASK	0

int main( int argc, char *argv[] ) {
   Errc result;                // The result code of the execution.
   Pobject* mask;              // The region mask.
   Pobject* objin[FINC + 1];   // The input objects.
   Pobject* objs[FINC + 1];    // The source objects masked by the mask.
   Pobject* objout[FOUTC + 1]; // The output object.
   Pobject* objd[FOUTC + 1];   // The result object of the execution.
   char* parv[PARC + 1];       // The input parameters.
   
   ReadArgs(argc, argv, PARC, FINC, FOUTC, &mask, objin, objs, objout, objd, parv, USAGE, MASK);
   
   Collection* c = new Collection;
   objd[0] = c;
   result = PTxt2Col(parv[0], *c);
   
   if (result) {
      WriteArgs(argc, argv, PARC, FINC, FOUTC, &mask, objin, objs, objout, objd, MASK);
   }
   
   Exit(result);
   return 0;
}

#endif
