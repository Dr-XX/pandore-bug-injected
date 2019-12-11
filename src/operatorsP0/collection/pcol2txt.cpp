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
 * @author Alexandre Duret-Lutz - 2000-06-23
 * @author Regis Clouard - 2001-02-01
 */

/**
 * @file pcol2txt.cpp
 * Affichage des infos contenues dans une collection a l'ecran ou dans un
 * fichier texte
 */

#include <string.h>
#include <pandore.h>
using namespace pandore;

#ifdef _MSC_VER
// Because Visual C++ does not support strcasecmp....
#include <ctype.h>
int strcasecmp( const char *s1, const char *s2 ) {
   int i;
   for (i = 0; s1[i] != '\0' && s2[i] != '\0'; ++i ) {
      char ch1 = toupper(s1[i]);
      char ch2 = toupper(s2[i]);
      
      if (ch1 != ch2) {
	 return (int) (ch1 - ch2);
      }
   }
   
   return (int) (s1[i] - s2[i]);
}
#endif

/*
 * Affiche les informations utiles concernant l'objet Collection.
 */
Errc PCol2Txt( const Collection& c, FILE *fd ) {
   std::list<std::string> l = c.List();
   
   std::list<std::string>::const_iterator i;
   for ( i = l.begin(); i != l.end(); ++i ) {
      BundledObject* bo = c.Get(*i);
      char type_att[256];
      char att[256];

      strncpy(type_att, bo->Type().c_str(), sizeof(type_att));
      strncpy(att, i->c_str(), sizeof(att));

      fprintf(fd, "%15s\t\"%s\":\t", type_att, att);
      
      if (!strcasecmp(type_att, "Double")) {
	 Double &f = c.GETVALUE(att, Double);
	 fprintf(fd, "%f\n", f);
      } else if (!strcasecmp(type_att, "Float")) {
	 Float &f=c.GETVALUE(att, Float);
	 fprintf(fd, "%f\n", f);
      } else if (!strcasecmp(type_att, "Long")) {
	 Long &l = c.GETVALUE(att, Long);
	 fprintf(fd, "%d\n", l);
      } else if (!strcasecmp(type_att, "Ulong")) {
	 Ulong &l = c.GETVALUE(att, Ulong);
	 fprintf(fd, "%u\n", l);
      } else if (!strcasecmp(type_att, "Short")) {
	 Short &l=c.GETVALUE(att, Short);
	 fprintf(fd, "%hd\n", l);
      } else if (!strcasecmp(type_att, "Ushort")) {
	 Ushort &l = c.GETVALUE(att, Ushort);
	 fprintf(fd, "%hu\n", l);
      } else if (!strcasecmp(type_att, "Char")) {
	 Char &uc = c.GETVALUE(att, Char);
	 fprintf(fd, "%d\n", (int)uc);
      } else if (!strcasecmp(type_att, "Uchar")) {
	 Uchar &uc = c.GETVALUE(att, Uchar);
	 fprintf(fd, "%d\n", (int)uc);
      } else if (!strcasecmp(type_att, "Array:Double")) {
	 Double *tab = c.GETARRAY(att, Double);
	 Long taille = c.GETARRAYSIZE(att, Double);
	 for (int j = 0; j < taille; j++) {
	    fprintf(fd, "%f\t", tab[j]);
	 }
	 fprintf(fd, "\n");
      } else if (!strcasecmp(type_att, "Array:Float")) {
	 Float *tab = c.GETARRAY(att, Float);
	 Long taille = c.GETARRAYSIZE(att, Float);
	 for (int j = 0; j < taille; j++) {
	    fprintf(fd, "%f\t", tab[j]);
	 }
	 fprintf(fd, "\n");
      } else if (!strcasecmp(type_att, "Array:Long")) {
	 Long *tab = c.GETARRAY(att, Long);
	 Long taille = c.GETARRAYSIZE(att, Long);
	 for (int j = 0; j < taille; j++) {
	    fprintf(fd, "%d\t", tab[j]);
	 }
	 fprintf(fd, "\n");
      } else if (!strcasecmp(type_att, "Array:Ulong")) {
	 Ulong *tab = c.GETARRAY(att, Ulong);
	 Long taille = c.GETARRAYSIZE(att, Ulong);
	 for (int j = 0; j < taille; j++) {
	    fprintf(fd, "%u\t", tab[j]);
	 }
	 fprintf(fd, "\n");
      } else if (!strcasecmp(type_att, "Array:Short")) {
	 Short *tab = c.GETARRAY(att, Short);
	 long taille = c.GETARRAYSIZE(att, Short);
	 for (int j = 0; j < taille; j++) {
	    fprintf(fd, "%hd\t", tab[j]);
	 }
	 fprintf(fd, "\n");
      }  else if (!strcasecmp(type_att, "Array:UShort")) {
	 Ushort *tab = c.GETARRAY(att, Ushort);
	 Long taille = c.GETARRAYSIZE(att, Ushort);
	 for (int j = 0; j < taille; j++) {
	    fprintf(fd, "%hu\t", tab[j]);
	 }
	 fprintf(fd, "\n");
      } else if (!strcasecmp(type_att, "Array:Char")) {
	 Char *tab = c.GETARRAY(att, Char);
	 Long taille = c.GETARRAYSIZE(att, Char);
	 for (int j = 0; j < taille; j++) {
	    fprintf(fd, "%d\t", (int)tab[j]);
	 }
	 fprintf(fd, "\n");
      } else if (!strcasecmp(type_att, "Array:Uchar")) {
	 Uchar *tab = c.GETARRAY(att, Uchar);
	 Long taille = c.GETARRAYSIZE(att, Uchar);
	 for (int j = 0; j < taille; j++) {
	    fprintf(fd, "%d\t", (int)tab[j]);
	 }
	 fprintf(fd, "\n");
      } else {
	 fprintf(fd, " unprintable value(s)\n");
	 return FAILURE;
      }
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
   Errc result;	               // The result code of the execution.
   Pobject* mask;               // The region map use as map.
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
	 } else if ((argc >= 3) && (strcmp(argv[1], "-m")) && (strcmp(argv[2], "-"))) {
	    fd = fopen(argv[2], "wt");
	 }
	 
	 result = PCol2Txt(*cs, fd);
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
