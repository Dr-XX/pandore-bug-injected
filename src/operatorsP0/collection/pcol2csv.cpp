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
 * @file pcol2csv.cpp
 * Converts a collection into a csv file.
 * csv format: label1 label2 label3 label4
 *             val11   val21 val31  val41
 *              ..      ..    ..     ..
 *             val1n   val2n val3n  val4n
 */

#include <string.h>
#include <pandore.h>
using namespace pandore;

#if (defined _MSC_VER) && (!defined snprintf)
#define snprintf _snprintf
#endif

#ifdef _MSC_VER
// Because Visual C++ does not support strcasecmp....
#include <ctype.h>
int strcasecmp( const char *s1, const char *s2 ) {
   int i;
   for (i = 0; s1[i] != '\0' && s2[ i ] != '\0'; ++i ) {
      char ch1 = toupper(s1[i]);
      char ch2 = toupper(s2[i]);
      
      if (ch1 != ch2) {
	 return (int) (ch1 - ch2);
      }
   }
   
   return (int) (s1[i] - s2[i]);
}
#endif

#define MAXLENGTH 15

class Node {
   char ** stringArray;
   int length;
   
public:
   Node( ): stringArray(0), length(0) { }
   void setSize( int size ) {
      length = size;
      stringArray = new char*[size];
      for (int i = 0; i < size; i++) {
	 stringArray[i] = new char[MAXLENGTH];
      }
   }
   char* get(const int i) const { 
      if (i >= length) {
	 return const_cast<char *>("");
      } else {
	 return stringArray[i];
      }
   }
};

/*
 * Display the collection into a csv format.
 */
Errc PCol2Csv( const Collection& c, FILE *fd) {
   std::list<std::string> l = c.List();

   std::list<std::string>::const_iterator i;
   int colCount=0;
   for ( i = l.begin(); i != l.end(); ++i ) {
      BundledObject* bo = c.Get(*i);
      char type_att[256];
      char att[256];
      strncpy(type_att, bo->Type().c_str(), 256);
      strncpy(att, i->c_str(), 256);
      colCount++;
      fprintf(fd, "%s; ", att);
   }
   fprintf(fd, "\n");

   Node *array = new Node[colCount];
   int maxSize = 0;
   colCount = 0;
   for ( i = l.begin(); i != l.end(); ++i ) {
      BundledObject* bo = c.Get(*i);
      char type_att[25];
      char att[25];
      strncpy(type_att, bo->Type().c_str(), sizeof(type_att));
      strncpy(att, i->c_str(), sizeof(att));
      if (!strcasecmp(type_att, "Double")) {
	 array[colCount].setSize(1);
	 Double &f = c.GETVALUE(att, Double);
	 if (maxSize < 1) {
	    maxSize = 1;
	 }
 	 snprintf(array[colCount].get(0), MAXLENGTH, "%f", f);
	 array[colCount].get(0)[MAXLENGTH - 1] = 0;
      } else if (!strcasecmp(type_att, "Float")) {
	 array[colCount].setSize(1);
	 if (maxSize < 1) {
	    maxSize = 1;
	 }
	 Float &f = c.GETVALUE(att, Float);
 	 snprintf(array[colCount].get(0), MAXLENGTH, "%f", f);
	 array[colCount].get(0)[MAXLENGTH - 1] = 0;
      } else if (!strcasecmp(type_att, "Long")) {
	 array[colCount].setSize(1);
	 if (maxSize < 1) {
	    maxSize = 1;
	 }
	 Long &l = c.GETVALUE(att, Long);
	 snprintf(array[colCount].get(0), MAXLENGTH, "%d", l);
	 array[colCount].get(0)[MAXLENGTH - 1] = 0;
      } else if (!strcasecmp(type_att, "Ulong")) {
	 array[colCount].setSize(1);
	 if (maxSize < 1) {
	    maxSize = 1;
	 }
	 Ulong &l = c.GETVALUE(att, Ulong);
	 snprintf(array[colCount].get(0), MAXLENGTH, "%u", l);
	 array[colCount].get(0)[MAXLENGTH - 1] = 0;
      } else if (!strcasecmp(type_att, "Short")) {
	 array[colCount].setSize(1);
	 if (maxSize < 1) {
	    maxSize = 1;
	 }
	 Short &l = c.GETVALUE(att, Short);
	 snprintf(array[colCount].get(0), MAXLENGTH, "%hd", l);
	 array[colCount].get(0)[MAXLENGTH - 1] = 0;
      } else if (!strcasecmp(type_att, "Ushort")) {
	 array[colCount].setSize(1);
	 if (maxSize < 1) {
	    maxSize = 1;
	 }
	 Ushort &l = c.GETVALUE(att, Ushort);
	 snprintf(array[colCount].get(0), MAXLENGTH, "%hu", l);
	 array[colCount].get(0)[MAXLENGTH - 1] = 0;
      } else if (!strcasecmp(type_att, "Char")) {
	 array[colCount].setSize(1);
	 if (maxSize < 1) {
	    maxSize = 1;
	 }
	 Char &uc = c.GETVALUE(att, Char);
	 snprintf(array[colCount].get(0), MAXLENGTH, "%d", (int)uc);
	 array[colCount].get(0)[MAXLENGTH - 1] = 0;
      } else if (!strcasecmp(type_att, "Uchar")) {
	 array[colCount].setSize(1);
	 if (maxSize < 1) {
	    maxSize = 1;
	 }
	 Uchar &uc = c.GETVALUE(att, Uchar);
	 snprintf(array[colCount].get(0), MAXLENGTH, "%d", (int)uc);
	 array[colCount].get(0)[MAXLENGTH - 1] = 0;
      } else if (!strcasecmp(type_att, "Array:Double")) {
	 Double *tab = c.GETARRAY(att, Double);
	 Long taille = c.GETARRAYSIZE(att, Double);
	 array[colCount].setSize(taille);
	 if (taille > maxSize) {
	    maxSize = taille;
	 }
	 for (int j = 0; j < taille; j++) {
	    snprintf(array[colCount].get(j), MAXLENGTH, "%f", tab[j]);
	    array[colCount].get(j)[MAXLENGTH - 1] = 0;
	 }
	 fprintf(fd, "\n");
      } else if (!strcasecmp(type_att, "Array:Float")) {
	 Float *tab = c.GETARRAY(att, Float);
	 Long taille = c.GETARRAYSIZE(att, Float);
	 if (taille > maxSize) {
	    maxSize = taille;
	 }
	 array[colCount].setSize(taille);
	 for (int j = 0; j < taille; j++) {
	    snprintf(array[colCount].get(j), MAXLENGTH, "%f", tab[j]);
	    array[colCount].get(j)[MAXLENGTH - 1] = 0;
	 }
      } else if (!strcasecmp(type_att, "Array:Long")) {
	 Long *tab = c.GETARRAY(att, Long);
	 Long taille = c.GETARRAYSIZE(att, Long);
	 if (taille > maxSize) {
	    maxSize = taille;
	 }
	 array[colCount].setSize(taille);
	 for (int j = 0; j < taille; j++) {
	    snprintf(array[colCount].get(j), MAXLENGTH, "%d", tab[j]);
	    array[colCount].get(j)[MAXLENGTH - 1] = 0;
	 }
      } else if (!strcasecmp(type_att, "Array:Ulong")) {
	 Ulong *tab = c.GETARRAY(att, Ulong);
	 Long taille = c.GETARRAYSIZE(att, Ulong);
	 if (taille > maxSize) {
	    maxSize = taille;
	 }
	 array[colCount].setSize(taille);
	 for (int j = 0; j < taille; j++) {
	    snprintf(array[colCount].get(j), MAXLENGTH, "%u", tab[j]);
	    array[colCount].get(j)[MAXLENGTH - 1] = 0;
	 }
      } else if (!strcasecmp(type_att, "Array:Short")) {
	 Short *tab = c.GETARRAY(att, Short);
	 long taille = c.GETARRAYSIZE(att, Short);
	 if (taille > maxSize) {
	    maxSize = taille;
	 }
	 array[colCount].setSize(taille);
	 for (int j = 0; j < taille; j++) {
	    snprintf(array[colCount].get(j), MAXLENGTH, "%hd", tab[j]);
	    array[colCount].get(j)[MAXLENGTH - 1] = 0;
	 }
      }  else if (!strcasecmp(type_att, "Array:UShort")) {
	 Ushort *tab = c.GETARRAY(att, Ushort);
	 Long taille = c.GETARRAYSIZE(att, Ushort);
	 if (taille > maxSize) {
	    maxSize = taille;
	 }
	 array[colCount].setSize(taille);
	 for (int j = 0; j < taille; j++) {
	    snprintf(array[colCount].get(j), MAXLENGTH, "%hu", tab[j]);
	    array[colCount].get(j)[MAXLENGTH - 1] = 0;
	 }
      } else if (!strcasecmp(type_att, "Array:Char")) {
	 Char *tab = c.GETARRAY(att, Char);
	 Long taille = c.GETARRAYSIZE(att, Char);
	 if (taille > maxSize) {
	    maxSize = taille;
	 }
	 array[colCount].setSize(taille);
	 for (int j = 0; j < taille; j++) {
	    snprintf(array[colCount].get(j), MAXLENGTH, "%d", (int)tab[j]);
	    array[colCount].get(j)[MAXLENGTH - 1] = 0;
	 }
      } else if (!strcasecmp(type_att, "Array:Uchar")) {
	 Uchar *tab=c.GETARRAY(att, Uchar);
	 Long taille=c.GETARRAYSIZE(att, Uchar);
	 if (taille > maxSize) {
	    maxSize = taille;
	 }
	 array[colCount].setSize(taille);
	 for (int j = 0; j < taille; j++) {
	    snprintf(array[colCount].get(j), MAXLENGTH, "%d", (int)tab[j]);
	    array[colCount].get(j)[MAXLENGTH - 1] = 0;
	 }
      }
      colCount++;
   }
   for (int i = 0; i< maxSize; i++ ) {
      for (int j = 0; j < colCount; j++) {
	 fprintf(fd, "%s; ", array[j].get(i));
      }
      fprintf(fd, "\n");
   }
   return SUCCESS;
}

#ifdef MAIN

/*
 * Modify only the following constants, and the function call.
 */
#define	USAGE	"usage: %s [col_in|-] file_out"
#define	PARC	0
#define	FINC	1
#define	FOUTC	0
#define	MASK	0

int main( int argc, char *argv[] ) {
   Errc result;	              // The result code of the execution.
   Pobject* mask;              // The region map use as map.
   Pobject* objin[FINC + 1];   // The input objects.
   Pobject* objs[FINC + 1];    // The source objects masked by the map.
   Pobject* objout[FOUTC + 1]; // The output object.
   Pobject* objd[FOUTC + 1];   // The result object of the execution.
   char* parv[PARC + 1];       // The input parameters.
   
   if (argc < 2) {
      fprintf(stderr, USAGE, argv[0]);
      Exit((Errc)FAILURE);
   }
   
   if ((argc == 2) && (!strcmp(argv[1], "-p"))) {
      fprintf(stdout, "%s 0 1 1\n", argv[0]);
      exit(0);
   }
   
   ReadArgs(argc, argv, PARC, FINC, FOUTC, &mask, objin, objs, objout, objd, parv, USAGE, MASK);
   
   switch(objs[0]->Type()) {
   case Po_Collection: {
      Collection *cs = (Collection*)objs[0];
      objd[0] = new Collection;
      FILE* fd = stdout;
      
      // Find out the name of the text file.
      // Skip mask file if needed.
      if ((argc >= 5) && (!strcmp(argv[1], "-m")) && (strcmp(argv[4], "-"))) {
	 fd = fopen(argv[4], "wt");
      } else {
	 if ((argc >= 3) && (strcmp(argv[1], "-m")) && (strcmp(argv[2], "-"))) {
	    fd=fopen(argv[2], "wt");
	 }
      }
    
      result = PCol2Csv(*cs, fd);
      fclose(fd);
    
   } break;
   default:
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
