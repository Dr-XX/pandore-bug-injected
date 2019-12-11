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
 */

#include <math.h>
#include <pandore.h>
using namespace pandore;

#if (defined _MSC_VER) && (!defined snprintf)
#define snprintf _snprintf
#endif

/**
 * @file parrayeuclideannorm.cpp
 * La norme d'un vecteur a N composantes
 */

void DistAttr_( const std::list<Char*> in, Float *out, int nVect )  {
   std::list<Char*>::const_iterator j;
   
   for (int i = 0; i < nVect; ++i) {
      Float sqr = 0;
      for (j = in.begin(); j != in.end(); ++j) {
	 sqr += (float)((*j)[i]) * (float)((*j)[i]);
      }
      out[i] = sqrt(sqr);
   }
}
void DistAttr_( const std::list<Uchar*> in, Float *out, int nVect )  {
   std::list<Uchar*>::const_iterator j;
   
   for (int i = 0; i < nVect; ++i) {
      Float sqr = 0;
      for (j = in.begin(); j != in.end(); ++j) {
	 sqr += (float)((*j)[i]) * (float)((*j)[i]);
      }
      out[i] = sqrt(sqr);
   }
}
void DistAttr_( const std::list<Short*> in, Float *out, int nVect )  {
   std::list<Short*>::const_iterator j;
   
   for (int i = 0; i < nVect; ++i) {
      Float sqr = 0;
      for (j = in.begin(); j != in.end(); ++j) {
	 sqr += (float)((*j)[i]) * (float)((*j)[i]);
      }
      out[i] = sqrt(sqr);
   }
}
void DistAttr_( const std::list<Ushort*> in, Float *out, int nVect )  {
   std::list<Ushort*>::const_iterator j;
   
   for (int i = 0; i < nVect; ++i) {
      Float sqr = 0;
      for (j = in.begin(); j != in.end(); ++j) {
	 sqr += (float)((*j)[i]) * (float)((*j)[i]);
      }
      out[i] = sqrt(sqr);
   }
}
void DistAttr_( const std::list<Long*> in, Float *out, int nVect )  {
   std::list<Long*>::const_iterator j;
   
   for (int i = 0; i < nVect; ++i) {
      Float sqr = 0;
      for (j = in.begin(); j != in.end(); ++j) {
	 sqr += (float)((*j)[i]) * (float)((*j)[i]);
      }
      out[i] = sqrt(sqr);
   }
}
void DistAttr_( const std::list<Ulong*> in, Float *out, int nVect )  {
   std::list<Ulong*>::const_iterator j;
   
   for (int i = 0; i < nVect; ++i) {
      Float sqr = 0;
      for (j = in.begin(); j != in.end(); ++j) {
	 sqr += (float)((*j)[i]) * (float)((*j)[i]);
      }
      out[i] = sqrt(sqr);
   }
}
void DistAttr_( const std::list<Llong*> in, Float *out, int nVect )  {
   std::list<Llong*>::const_iterator j;
   
   for (int i = 0; i < nVect; ++i) {
      Float sqr = 0;
      for (j = in.begin(); j != in.end(); ++j) {
	 sqr += (float)((*j)[i]) * (float)((*j)[i]);
      }
      out[i] = sqrt(sqr);
   }
}
void DistAttr_( const std::list<Ullong*> in, Float *out, int nVect )  {
   std::list<Ullong*>::const_iterator j;
   
   for (int i = 0; i < nVect; ++i) {
      Float sqr = 0;
      for (j = in.begin(); j != in.end(); ++j) {
	 sqr += (float)((*j)[i]) * (float)((*j)[i]);
      }
      out[i] = sqrt(sqr);
   }
}
void DistAttr_( const std::list<Float*> in, Float *out, int nVect )  {
   std::list<Float*>::const_iterator j;
   
   for (int i = 0; i < nVect; ++i) {
      Float sqr = 0;
      for (j = in.begin(); j != in.end(); ++j) {
	 sqr += (float)((*j)[i]) * (float)((*j)[i]);
      }
      out[i] = sqrt(sqr);
   }
}
void DistAttr_( const std::list<Double*> in, Float *out, int nVect )  {
   std::list<Double*>::const_iterator j;
   
   for (int i = 0; i < nVect; ++i) {
      Float sqr = 0;
      for (j = in.begin(); j != in.end(); ++j) {
	 sqr += (float)((*j)[i]) * (float)((*j)[i]);
      }
      out[i] = sqrt(sqr);
   }
}

Errc PArrayEuclideanNorm( const Collection &col_in, Collection &col_out, 
			  const std::string &name_in, const std::string &name_out ) {
   char name[256];
   int nbr = 1;
   snprintf(name, sizeof(name), "%s.%d", name_in.c_str(), nbr);	
   name[sizeof(name) - 1] = 0;


   std::string type = col_in.GetType(name);
   if (type == "Array:Char") {
      Long nVect = MAXLONG;
      std::list<Char*> l;
      do {
	 Long nVect_ = col_in.GETARRAYSIZE(name, Char);
	 if (nVect_ < nVect) {
	    nVect = nVect_;
	 }
	 l.push_back(col_in.GETARRAY(name, Char));
	 ++nbr;
	 snprintf(name, sizeof(name), "%s.%d", name_in.c_str(), nbr);
	 name[sizeof(name) - 1] = 0;

      } while ( col_in.Exists(name) && type == col_in.GetType(name));
      Float* result = new Float[nVect];
      DistAttr_(l, result, nVect);
      col_out.SETARRAY(name_out, Float, result, nVect);
   } else
   if (type == "Array:Uchar") {
      Long nVect = MAXLONG;
      std::list<Uchar*> l;
      do {
	 Long nVect_ = col_in.GETARRAYSIZE(name, Uchar);
	 if (nVect_ < nVect) {
	    nVect = nVect_;
	 }
	 l.push_back(col_in.GETARRAY(name, Uchar));
	 ++nbr;
	 snprintf(name, sizeof(name), "%s.%d", name_in.c_str(), nbr);
	 name[sizeof(name) - 1] = 0;

      } while ( col_in.Exists(name) && type == col_in.GetType(name));
      Float* result = new Float[nVect];
      DistAttr_(l, result, nVect);
      col_out.SETARRAY(name_out, Float, result, nVect);
   } else
   if (type == "Array:Short") {
      Long nVect = MAXLONG;
      std::list<Short*> l;
      do {
	 Long nVect_ = col_in.GETARRAYSIZE(name, Short);
	 if (nVect_ < nVect) {
	    nVect = nVect_;
	 }
	 l.push_back(col_in.GETARRAY(name, Short));
	 ++nbr;
	 snprintf(name, sizeof(name), "%s.%d", name_in.c_str(), nbr);
	 name[sizeof(name) - 1] = 0;

      } while ( col_in.Exists(name) && type == col_in.GetType(name));
      Float* result = new Float[nVect];
      DistAttr_(l, result, nVect);
      col_out.SETARRAY(name_out, Float, result, nVect);
   } else
   if (type == "Array:Ushort") {
      Long nVect = MAXLONG;
      std::list<Ushort*> l;
      do {
	 Long nVect_ = col_in.GETARRAYSIZE(name, Ushort);
	 if (nVect_ < nVect) {
	    nVect = nVect_;
	 }
	 l.push_back(col_in.GETARRAY(name, Ushort));
	 ++nbr;
	 snprintf(name, sizeof(name), "%s.%d", name_in.c_str(), nbr);
	 name[sizeof(name) - 1] = 0;

      } while ( col_in.Exists(name) && type == col_in.GetType(name));
      Float* result = new Float[nVect];
      DistAttr_(l, result, nVect);
      col_out.SETARRAY(name_out, Float, result, nVect);
   } else
   if (type == "Array:Long") {
      Long nVect = MAXLONG;
      std::list<Long*> l;
      do {
	 Long nVect_ = col_in.GETARRAYSIZE(name, Long);
	 if (nVect_ < nVect) {
	    nVect = nVect_;
	 }
	 l.push_back(col_in.GETARRAY(name, Long));
	 ++nbr;
	 snprintf(name, sizeof(name), "%s.%d", name_in.c_str(), nbr);
	 name[sizeof(name) - 1] = 0;

      } while ( col_in.Exists(name) && type == col_in.GetType(name));
      Float* result = new Float[nVect];
      DistAttr_(l, result, nVect);
      col_out.SETARRAY(name_out, Float, result, nVect);
   } else
   if (type == "Array:Ulong") {
      Long nVect = MAXLONG;
      std::list<Ulong*> l;
      do {
	 Long nVect_ = col_in.GETARRAYSIZE(name, Ulong);
	 if (nVect_ < nVect) {
	    nVect = nVect_;
	 }
	 l.push_back(col_in.GETARRAY(name, Ulong));
	 ++nbr;
	 snprintf(name, sizeof(name), "%s.%d", name_in.c_str(), nbr);
	 name[sizeof(name) - 1] = 0;

      } while ( col_in.Exists(name) && type == col_in.GetType(name));
      Float* result = new Float[nVect];
      DistAttr_(l, result, nVect);
      col_out.SETARRAY(name_out, Float, result, nVect);
   } else
   if (type == "Array:Llong") {
      Long nVect = MAXLONG;
      std::list<Llong*> l;
      do {
	 Long nVect_ = col_in.GETARRAYSIZE(name, Llong);
	 if (nVect_ < nVect) {
	    nVect = nVect_;
	 }
	 l.push_back(col_in.GETARRAY(name, Llong));
	 ++nbr;
	 snprintf(name, sizeof(name), "%s.%d", name_in.c_str(), nbr);
	 name[sizeof(name) - 1] = 0;

      } while ( col_in.Exists(name) && type == col_in.GetType(name));
      Float* result = new Float[nVect];
      DistAttr_(l, result, nVect);
      col_out.SETARRAY(name_out, Float, result, nVect);
   } else
   if (type == "Array:Ullong") {
      Long nVect = MAXLONG;
      std::list<Ullong*> l;
      do {
	 Long nVect_ = col_in.GETARRAYSIZE(name, Ullong);
	 if (nVect_ < nVect) {
	    nVect = nVect_;
	 }
	 l.push_back(col_in.GETARRAY(name, Ullong));
	 ++nbr;
	 snprintf(name, sizeof(name), "%s.%d", name_in.c_str(), nbr);
	 name[sizeof(name) - 1] = 0;

      } while ( col_in.Exists(name) && type == col_in.GetType(name));
      Float* result = new Float[nVect];
      DistAttr_(l, result, nVect);
      col_out.SETARRAY(name_out, Float, result, nVect);
   } else
   if (type == "Array:Float") {
      Long nVect = MAXLONG;
      std::list<Float*> l;
      do {
	 Long nVect_ = col_in.GETARRAYSIZE(name, Float);
	 if (nVect_ < nVect) {
	    nVect = nVect_;
	 }
	 l.push_back(col_in.GETARRAY(name, Float));
	 ++nbr;
	 snprintf(name, sizeof(name), "%s.%d", name_in.c_str(), nbr);
	 name[sizeof(name) - 1] = 0;

      } while ( col_in.Exists(name) && type == col_in.GetType(name));
      Float* result = new Float[nVect];
      DistAttr_(l, result, nVect);
      col_out.SETARRAY(name_out, Float, result, nVect);
   } else
   if (type == "Array:Double") {
      Long nVect = MAXLONG;
      std::list<Double*> l;
      do {
	 Long nVect_ = col_in.GETARRAYSIZE(name, Double);
	 if (nVect_ < nVect) {
	    nVect = nVect_;
	 }
	 l.push_back(col_in.GETARRAY(name, Double));
	 ++nbr;
	 snprintf(name, sizeof(name), "%s.%d", name_in.c_str(), nbr);
	 name[sizeof(name) - 1] = 0;

      } while ( col_in.Exists(name) && type == col_in.GetType(name));
      Float* result = new Float[nVect];
      DistAttr_(l, result, nVect);
      col_out.SETARRAY(name_out, Float, result, nVect);
   } else
      {
	 std::cerr << "Error parrayeuclideannorm: Invalid collection type" << std::endl;
	 return FAILURE;
      }
   return SUCCESS;
}


#ifdef MAIN

/*
 * Modify only the following constants, and the function call.
 */
#define	USAGE	"usage: %s name_in name_out [im_in|-] [im_out|-]"
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
      result = PArrayEuclideanNorm(*ims1, *imd1, parv[0], parv[1]);
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
