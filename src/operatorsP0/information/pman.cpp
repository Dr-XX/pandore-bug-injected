/* -*- c-basic-offset: 3 -*-
 *
 * Copyright (c) 2013, GREYC.
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
 * @file pman.cpp
 *
 * Display the related documentation of an operator (a la man).
 * options:  -k keyword : to find out all operators related to a keyword.
 *           -M path    : Specifies an alternate search path for manual pages.
 *           -f file    : Display the file.
 *           -s section : Specifies sections of the manual for man to search.
 */
 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#if _MSC_VER
#define snprintf _snprintf
#endif


/*
 * Search paths definition
 */

#ifndef LANG
#define LANG "en"
#endif

#ifdef _WIN32
#define DIRPATH "%s\\doc\\operators\\" LANG
#define DOCNAME "operatorsP"
#else
#define DIRPATH "%s/doc/operators/" LANG
#define DOCNAME "operatorsP"
#endif

#define WIDTH 70

#ifdef _MSC_VER
// Because Visual C++ does not support strcasecmp....
int strcasecmp( const char *s1, const char *s2 ) {
   int i;
   for (i = 0; s1[i] != '\0' && s2[i] != '\0'; ++i) {
      char ch1 = toupper(s1[i]);
      char ch2 = toupper(s2[i]);
      
      if (ch1 != ch2) {
	 return (int)(ch1 - ch2);
      }
   }
   
   return (int)(s1[i] - s2[i]);
}
#else
#include <unistd.h>
#endif

/*
 * Print a message and exit.
 */
void Error( int num, const char *arg ) {
   switch (num) {
   case 0:
   case 1:
      fprintf(stdout, "usage: %s [-M path] operator ...\n", arg);
      fprintf(stdout, "       %s [-M path] -k keyword...\n", arg);
      break;
   case 2: fprintf(stderr, "Error pman: No value for %s\n", arg);
      break;
   case 3: fprintf(stderr, "Error pman: No manual entry for %s\n", arg);
      break;
   case 4: fprintf(stderr, "Error pman: Cannot create new tempory files\n");
      break;
   }
   exit(num);
}

/*
 * Reads and returns a tag from file fi.
 * Returns true if it is a tag.
 */
int readTag( FILE* fi, char *tag ) {
   int	charac;
   int	n = 0;

   while (((charac = fgetc(fi)) != EOF) && (charac != '>')) {
      tag[n++] = (char)toupper(charac);
   }
   tag[n] = '\0';
   return (charac == '>');
}

/*
 * Reads the header of the html pages
 * and prints out the related man header.
 * Header: from the begin to the first H1.
 */
int writeHeader( FILE *fi, FILE *fo, char *name ) {
   int 	charac;
   char	tag[1000];

   /* Write header of the man pages. */
   if (fo != NULL) {
      fprintf(fo, "%s(1)\t\tPANDORE OPERATORS\t\t%s(1)\n\n", name, name);
   }
   /* Escape characters until the first H1 tag. */
   while ((charac = fgetc(fi)) != EOF) {
      if (charac == '<') {
	 if (!readTag(fi, tag)) {
	    return 0;
	 }
	 if (!strcmp(tag, "H1")) {
	    return 1;
	 }
      }
   }
   return 0;
}

#define NORMOD 0	// Normal
#define HMOD 1		// Title.
#define IMOD 2		// Italique
#define RETMOD 4	// Flag to avoir carriage return when encounter the first <br>.
#define PREMOD 8	// Preformated mode.

/*
 * Parse a tag: Determine the next mode.
 * pos is the current position of the next character.
 */
void processTag( FILE *fi, FILE *fo, char *tag, int *mode, int *pos, char *name ) {
   static int listnumber = 0;

   if ((!strcasecmp(tag, "BR")) ||
       (!strcasecmp(tag, "P"))  || (!strcasecmp(tag, "/P"))) {
      if (*mode == RETMOD) { // Special Header: Escape the first BR.
	 fputc('-', fo);
	 *pos = *pos +1;
	 *mode = NORMOD;
      } else{
	 fputs("\n", fo);
	 *pos = 0;
      }
      return;
   }
   
   // Special address
   if (!strcasecmp(tag, "ADDRESS")) {
      signed char charac;
      int i = 0;

      while (((charac = fgetc(fi)) != EOF)
	     && (charac != '<')
	     && (i < 255)) {
	 if (charac != '\n' && charac != '\r') { // Avoid truncated name. 
	    name[i++] = charac;
	    name[i] = '\0';
	 }
      }
      ungetc(charac, fi);
      return;
   }
  
   if (!strcasecmp(tag, "I")) {
      *mode = IMOD;
      return;
   }
   if (!strcasecmp(tag, "/I")) {
      *mode = NORMOD;
      return;
   }
   if (!strcasecmp(tag, "H3")) {
      *mode = HMOD;
      fputs("\n\n", fo);
      return;
   }
   if (!strcasecmp(tag, "/H3")) {
      *mode = NORMOD;
      *pos = 0;
      return;
   }

   if (!strcasecmp(tag, "PRE")) {
      *mode = PREMOD;
      *pos = 0;
      return;
   }
   if (!strcasecmp(tag, "/PRE")) {
      *mode = NORMOD;
      *pos = 0;
      return;
   }
   if (!strcasecmp(tag, "OL")) {
      listnumber = 1;
      *pos = 0;
      return;
   }
   if (!strcasecmp(tag, "UL")) {
      listnumber = 0;
      *pos = 0;
      return;
   }
   if (!strcasecmp(tag, "LI")) {
      if (listnumber > 0) {
	 fprintf(fo, "\n        %d. ", listnumber++);
      } else {
	 fputs("\n        - ", fo);
      }
      *pos = 6;
      return;
   }
}

/**
 * Convert an HTML string to its releted an ASCI string.
 * skip accent and so on.
 */
char* parseHtmlString( char *htmlString ) {
   static char converted[255];
   unsigned int j = 0;
   
   for (unsigned int i = 0; i < strlen(htmlString); i++) {
      if (htmlString[i] == '&') {
	 converted[j++] = htmlString[i + 1];
	 for (; i < strlen(htmlString) && htmlString[i] != ';'; i++) ;
      } else {
	 converted[j++] = htmlString[i];
      }
   }
   converted[j] = '\0';
   return converted;
}

/*
 * Replaces html accent by ASCII version.
 */
const char* parseAccent( FILE *fi ) {
   static char accent[22];
   char charac;
   int i = 0;

   charac = fgetc(fi);
   accent[0] = charac;

   while ((!feof(fi)) && ((accent[++i] = fgetc(fi)) != ';') && (i < 20)) ;

   accent[++i] = '\0';
   if (!strcasecmp(&accent[1], "circ;")) {
      accent[1] = '\0';
      return accent;
   }
   if (!strcasecmp(&accent[1], "acute;")) {
      accent[1] = '\0';
      return accent;
   }
   if (!strcasecmp(&accent[1], "grave;")) {
      accent[1] = '\0';
      return accent;
   }
   if (!strcasecmp(&accent[1], "cedil;")) {
      accent[1] = '\0';
      return accent;
   }
   if (!strcasecmp(&accent[1], "uml;")) {
      accent[1] = '\0';
      return accent;
   }
   if (!strcmp(accent, "amp;")) {
      return "&";
   }
   if (!strcmp(accent, "nbsp;")) {
      return " ";
   } 
   if (!strcmp(accent, "gt;")) {
      return ">";
   }
   if (!strcmp(accent, "lt;")) {
      return "<";
   }
   if (!strcmp(accent, "le;")) {
      return "<=";
   }
   if (!strcmp(accent, "ge;")) {
      return ">=";
   }
   
   // Not an accent...restore.
   fseek(fi, -i, SEEK_CUR);
   return "&";
}

/*
 * Writes character "charac" at position *pos (depends on "mode").
 */
void writeChar( int charac, int mode, int *pos, FILE *fo ) {
   if (charac == '\r') {
      return;
   }
   switch (mode) {
   case HMOD: // Title mode
      if (charac == '\n') {
	 charac = ' ';        // Escape return characters.
      }
      if ((*pos == 0) && (charac == ' ')) {   // Escape blanks in the beginning of a line.
	 break;
      }
      *pos = ((*pos) + 1) % WIDTH;
      fputc(toupper(charac), fo);
      break;

   case NORMOD:
   case RETMOD:  // Normal mode
      if (charac == '\n') {
	 charac = ' ';        // Escape return characters.
      }
      if (charac == '\r') {
	 charac = ' ';        // Escape return characters.
      }
      if ((*pos == 0) && (charac == ' ')) {  // Escape blanks in the beginning of a line.
	 break;
      }
      if (*pos == 0) {
	 fputs("\n     ", fo);
      }
      fputc(charac, fo);
      *pos = ((*pos) + 1) % WIDTH;
      return;
       
   case IMOD:  // Italic mode
      if (charac == '\n') {
	 charac = ' ';        // Escape return characters.
      }
      if ((*pos == 0) && (charac == ' ')) {   // Escape blanks in the beginning of a line.
	 return;
      }
      if (*pos == 0) {
	 fputs("\n     ", fo);
      }
      /* Escape blanks in the beginning of a line. */
      fputs("_\b", fo);
      fputc(charac, fo);
      *pos = ((*pos) + 1) % WIDTH;
      return;
       
   case PREMOD: // predefined mode (Display as it is.)
      if (*pos == 0) {
	 fputs("\n     ", fo);
      }
      if (charac == '\n') {
	 *pos = 0;
      } else {
	 fputc(charac, fo);
	 *pos = (*pos) + 1;
      }
      break;
   }
}

/*
 * Reads the body of the man pages.
 * Body : Until the first ADDRESS tag.
 */
int writeBody( FILE* fi, FILE* fo, char *name ) {
   int	charac;
   char	tag[1000];
   int	mode = NORMOD;
   int	x;

   fputs("NAME", fo);
   mode = RETMOD;
   x = 0;
   // Parse all characters until the ADDRESS tag/
   while( (charac = fgetc(fi)) > 1 ) {
      switch(charac) {
      case '<':
	 readTag(fi, tag);
	 processTag(fi, fo, tag, &mode, &x, name);
	 break;
      case '&': { // HTML special character.
	 const char* convertedString = parseAccent(fi);
	 for (int i = 0; i < (int)strlen(convertedString); i++) {
	    writeChar(convertedString[i], mode, &x, fo);
	 }
      }
	 break;
      default: // Just write the stuff out to file.
	 writeChar(charac, mode, &x, fo);
	 break;
      }
   }
   return(0);
}

/*
 * Prints out the footer of the man pages.
 */
void writeFooter( FILE * /*fi*/, FILE *fo, char *name ) {
   fprintf(fo, "\n\nGREYC-IMAGE\t\t%s\n", parseHtmlString(name));
}

/**
 * Returns true if the specified name is a filename.
 */
bool isHTMLFile(char *name) {
#define EXTENSION ".html"
   int l = strlen(name);
   if (l < 5) {
      return false;
   }
   l = l - 5;
   return !strncmp(&name[l], EXTENSION, 5);
}

#ifdef _MSC_VER

#include <io.h>

/**
 * Searches file "keywords.html" in each directory "dirpath/operatorsPxxxx".
 * @param dirpath a valid directory (at least one character).
 * @param keywords the name of a Pandore operator.
 * @return the file descriptor or NULL.
 */
FILE* getDirectory( char *dirpath, char *keyword ) {
   struct _finddata_t c_file;
   long hFile;
   char currentDir[512];

   snprintf(currentDir, sizeof(currentDir), "%s\\*.*", dirpath);
   currentDir[sizeof(currentDir) - 1] = 0;
   /* Find subdirectories in the pandore directory */
   if ((hFile = _findfirst(currentDir, &c_file)) == -1L) {
      return NULL;
   } else  {
      do {
	 char finame[256];
	 snprintf(finame, sizeof(finame), "%s\\%s\\%s.html", dirpath, c_file.name, keyword);
	 finame[sizeof(finame) - 1] = 0;
	 FILE *fi;
	 if ((fi = fopen(finame, "rt")) != NULL) {
	    _findclose(hFile);
	    return fi;
	 }
      } while (_findnext( hFile, &c_file ) == 0);
   }
   _findclose(hFile);
   return NULL;
}

/**
 * Returns the list of index files in the dirpath path.
 * @param dirpath the directory that contains the index files. 
 * @param indexFiles the name of the index found.
 * @return the number of index.
 */
int getWindex( char *dirpath, char **indexFiles ) {
   struct _finddata_t c_file;
   long hFile;
   int nbfic = 0;
   char currentDir[512];

   snprintf(currentDir, sizeof(currentDir), "%s\\*.*", dirpath);

   /* Find subdirectories in the pandore directory */
   if ((hFile = _findfirst(currentDir, &c_file)) == -1L) {
      return 0;
   }else {
      do {
	 if (!strncmp(DOCNAME, c_file.name, strlen(DOCNAME)) && isHTMLFile(c_file.name)) {
	    const int size = 256;
	    indexFiles[nbfic] = (char*)malloc(size * sizeof(char));
	    snprintf(indexFiles[nbfic], size, "%s\\%s", dirpath, c_file.name);
	    indexFiles[nbfic][size - 1] = 0;
	    nbfic++;
	 }
      } while (_findnext( hFile, &c_file ) == 0);
   }
   _findclose(hFile);

   return nbfic;
}
#else
/* case of Unix, MACOS, Linux */

#include <dirent.h>

/**
 * Searches file "keywords.html" in each directory "dirpath/operatorsPxxxx".
 * @param dirpath a valid directory (at least one character).
 * @param keywords the name of a Pandore operator.
 * @return the file descriptor or NULL.
 */
FILE* getDirectory( char *dirpath, char *keyword ) {
   DIR *dirp = opendir(dirpath);

   if (dirp == NULL) {
      return NULL;
   }
   struct dirent *direntp;
   for (direntp = readdir(dirp); direntp != NULL; direntp = readdir(dirp)) {
      char finame[256];

      snprintf(finame, sizeof(finame), "%s/%s/%s.html", dirpath, direntp->d_name, keyword);
      finame[sizeof(finame) - 1] = 0;
      FILE *fi;
      if ( !strncmp(direntp->d_name, DOCNAME, strlen(DOCNAME))
	   && (fi = fopen(finame, "rt")) != NULL) {
	 closedir(dirp);
	 return fi;
      }
   }

   if (dirp != NULL) {
      closedir(dirp);
   }
   return NULL;
}

/**
 * Returns the list of index files form the dirpath path.
 * @param dirpath the directory that contains the index files. 
 * @param indexFiles the name of the index found.
 * @return the number of index.
 */
int getWindex( char *dirpath, char **indexFiles ) {
   DIR *dirp = opendir(dirpath);
   int nbfic = 0;
   struct dirent *direntp;

   for (direntp = readdir(dirp); direntp != NULL; direntp = readdir(dirp)) {
      if (!strncmp(DOCNAME, direntp->d_name, strlen(DOCNAME)) &&
	  isHTMLFile(direntp->d_name)) {
	 const int size = 512;
	 indexFiles[nbfic] = (char*)malloc(size * sizeof(char));
	 snprintf(indexFiles[nbfic], size, "%s/%s", dirpath, direntp->d_name);
	 indexFiles[nbfic][size - 1] = 0;
	 nbfic++;
      }
   }

   if (dirp != NULL) {
      closedir(dirp);
   }
   return nbfic;
}

#endif

/**
 * Parses arguments.
 */
int main( int argc, char *argv[] ) {
   FILE	*fi = NULL;
   FILE *fo = NULL;
   char	commandline[256];
   char	foname[12][256];
   char name[255];
   int	i;   

   // Check the command line.
   if (argc < 2) {    // No argument
      Error(1, argv[0]);
   }

   int argi = 1;
   if (!strcmp(argv[argi], "-p")) {
      exit(0);
   }

   if (!strcmp(argv[argi], "-h")) {
      Error(0, argv[0]);
   }

   // Parse options
   char **keywords = NULL;
   int nkeywords = 0;
   int type = 0;
   char	*pandorehome = getenv("PANDOREHOME");
   char	dirpath[512];

   snprintf(dirpath, sizeof(dirpath), DIRPATH, pandorehome);
   dirpath[sizeof(dirpath) - 1] = 0;
   while (argi < argc) {
      if (!strncmp(argv[argi], "-M", 2)) {
	 strncpy(dirpath, argv[++argi], sizeof(dirpath));
	 argi++;
      } else if (!strncmp(argv[argi], "-k", 2)) {
 	 keywords = &argv[++argi];
	 nkeywords = argc - argi;
	 argi = argc;
	 type = 1;
      } else {
 	 keywords = &argv[argi];
	 nkeywords = argc - argi;
	 argi = argc;
	 type = 0;
      }
   }

   // No value for path.
   if (strlen(dirpath) == 0) {
      Error(2, "PANDOREHOME");
   }
   
   switch(type) {
   case 0: // Print out the documentation of operator named by keywords
      // For each keywords
      for (i = 0; i < nkeywords; i++) {
	 fi = getDirectory(dirpath, keywords[i]);
	 if (fi == NULL) {
	    Error(3, keywords[i]);
	 }
	 
	 /* Can't create temporary file! */
#ifdef _WIN32
	 strncpy(foname[i], _tempnam(NULL, "pan"), sizeof(foname[i]));
	 foname[i][sizeof(foname[i]) - 1] = 0;
#else
 	 strncpy(foname[i], "/tmp/pandoreXXXXXX", sizeof(foname[i]));
	 foname[i][sizeof(foname[i]) - 1] = 0;
 	 if (mkstemp(foname[i]) < 0) {
	    fprintf(stderr, "Error pman: cannot display manual page.\n");
	    return 1;
	 }
#endif
	 if ((!foname[i]) || (!(fo = fopen(foname[i], "wt")))) {
	    Error(4, NULL);
	 }
	 /* parse html file. */
	 writeHeader(fi, fo, keywords[i]);
 	 writeBody(fi, fo, name);
	 writeFooter(fi, fo, name);
	 fclose(fo);
	 fclose(fi);
      }
#ifdef _MSC_VER
      strncpy(commandline, "more <", sizeof(commandline));
#else
      strncpy(commandline, "more ", sizeof(commandline));
#endif
      commandline[sizeof(commandline) - 1] = 0;
      /* run man program */
      for (i = 0; i < nkeywords; i++) {
	 int len = strlen(commandline) + strlen(foname[i]);
	 if (len + 3 > (int)sizeof(commandline)) {
	    break;
	 }
	 strncat(commandline, foname[i], len - 3);
	 strncat(commandline, " ", 2);
      }
      if (system(commandline) < 0) {
	 fprintf(stderr, "Error pman: cannot display manual page.\n");
	 return 1;
      }
      for (i = 0; i < nkeywords; i++) {
	 remove(foname[i]);
      }
      break;
  
   case 1: // Find out documentations that contain any of the given keywords
      // Looking for keywords in NBFIC index files: common et local.
      char *indexFiles[50];
      int nbfic = getWindex(dirpath, indexFiles);

      for (i = 0; i < nbfic; i++) {
	 /* Right documentation file */
	 if (!(fi = fopen(indexFiles[i], "r"))) {
	    break;
	 }
	 /* Skipping header */
	 writeHeader(fi, NULL, NULL);
	 char *dummy = fgets(commandline, sizeof(commandline), fi); /* Skipping title */
	 for ( ;!feof(fi) && dummy != 0; ) { 
	    dummy = fgets(commandline, sizeof(commandline), fi);
	    for (int k = 0; k < (int)strlen(commandline); k++) {
	       commandline[k] = (char)tolower(commandline[k]);
	    }

	    if (strstr(commandline, "<li><a href=")) {
	       /* Search each given words */
	       for (int j = 0; j < nkeywords; j++) {
		  if (strstr(commandline, keywords[j])) {
		     char *t1, *t2;
		     if (((t1 = strstr(commandline, ".html\">")) != NULL)
			 && ((t2 = strstr(commandline, "</a>")) != NULL)) {
			*(t2) = '\0';
			printf("%s", parseHtmlString(t1 + 7));
			printf("%s", parseHtmlString(t2 + 4));
			break;
		     }
		  }
	       }
	    }
	 }
	 fclose(fi);
      }
      for (i = 0; i < nbfic; i++) {
	 free(indexFiles[i]);
      }
      break;
   }
   putchar('\n');

   return 0;
}

