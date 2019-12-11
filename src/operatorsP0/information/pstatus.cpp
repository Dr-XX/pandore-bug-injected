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
 * @author Régis Clouard - 2000-07-04
 * @author Regis Clouard - 2001-04-27 (version 3.00)
 * @author Regis Clouard - Jun 27, 2010 (add strings)
 * @author Cyril Bazin - Jan 17, 2012 (getenv + parallelism)
 */

#if (defined _MSC_VER) && (!defined snprintf)
#define snprintf _snprintf
#endif

#if defined _WIN32 || defined WIN32
#include <io.h>
#endif
#include <string.h>
#include <pandore.h>
using namespace pandore;

/**
 * @file pstatus.cpp
 *
 * Displays the return value of the lastest executed operator.
 * Result is stored in the file: $HOME/.pandore
 */


/**
 * Reads the RESFIL file and display its content
 * on the standard output.
 */
int main( int argc, char *argv[] ) {
#if defined _WIN32 || defined WIN32
   FILE* fdos;
#endif
   Char type;
   FILE *fp;
   char nomf[256];
   char *dir;
   int number = 0;

   // display prototype  
   if ((argc > 1) && (!strcmp(argv[1], "-p"))) {
      fprintf(stdout, "%s 0 0 0\n", argv[0]);
      exit(0);
   }

   // display usage
   if (argc > 1) {
      fprintf(stderr, "usage: %s\n", argv[0]);
      exit(0);
   }

   if (!(dir = getenv(PANDORE_TMP))) {
#if defined _WIN32 || defined WIN32
      if ((dir = getenv("TEMP")) == 0) {
	 dir = const_cast<char *>("c:\\Temp");
      }
#else
      dir = const_cast<char *>("/tmp");
      number = (int)getppid();
#endif
   }
#if defined _WIN32 || defined WIN32
   snprintf(nomf, sizeof(nomf), "%s\\%s%d", dir, STATUS_FILE, number);
   nomf[sizeof(nomf) - 1]= 0;
#else
   snprintf(nomf, sizeof(nomf), "%s/%s%d", dir, STATUS_FILE, number);
   nomf[sizeof(nomf) - 1]= 0;
#endif
   
   if ((fp = fopen(nomf, "rb")) != 0) {
      if (fread((char*)&type, sizeof(Char), 1, fp) < 1) {
	 exit(1);
      }
      switch (type) {
	 case 'E': // FAILURE
	    fprintf(stdout, "FAILURE\n");
#if defined _WIN32 || defined WIN32
	    if ((fdos = fopen("pset.bat", "wt")) != 0) {
	       fprintf(fdos, "set %%1=FAILURE");
	       fclose(fdos);
	    }	
#endif
	    break;
	 case 'S': // SUCCESS
	    fprintf(stdout, "SUCCESS\n");
#if defined _WIN32 || defined WIN32
	    if ((fdos = fopen("pset.bat", "wt")) != 0) {
	       fprintf(fdos, "set %%1=SUCCESS");
	       fclose(fdos);
	    }
#endif
	    break;
	 case 'T': {// Text (string)
	    char stat[255];
	    int t;
	    if ((t = fread((char*)&stat, sizeof(char), 255, fp)) < 1) {
	       exit(1);
	    }
	    
	    stat[t] = '\0';
	    fprintf(stdout, "%s\n", stat);
#if defined _WIN32 || defined WIN32
	    if ((fdos = fopen("pset.bat", "wt")) != 0) {
	       fprintf(fdos, "set %%1=%s", stat);
	       fclose(fdos);
	    }
#endif
	 }
	    break;
	 case '0': {
	    Char stat;
	    if (fread((char*)&stat, sizeof(Char), 1, fp) < 1) {
	       exit(1);
	    }
	    fprintf(stdout, "%d\n", (int)stat);
#if defined _WIN32 || defined WIN32
	    if ((fdos = fopen("pset.bat", "wt")) != 0) {
	       fprintf(fdos, "set %%1=%d\n", (int)stat);
	       fclose(fdos);
	    }
#endif
	 } break;
	 case '1': {
	    Short stat;
	    if (fread((char*)&stat, sizeof(Short), 1, fp) < 1) {
	       exit(1);
	    }
	    fprintf(stdout, "%d\n", (int)stat);
#if defined _WIN32 || defined WIN32
	    if ((fdos = fopen("pset.bat", "wt")) != 0) {
	       fprintf(fdos, "set %%1=%d\n", (int)stat);
	       fclose(fdos);
	    }
#endif
	 } break;
	 case '2': {
	    Long stat;
	    if (fread((char*)&stat, sizeof(Long), 1, fp) < 1) {
	       exit(1);
	    }
	    fprintf(stdout, "%ld\n", (long)stat);
#if defined _WIN32 || defined WIN32
	    if ((fdos = fopen("pset.bat", "wt")) != 0) {
	       fprintf(fdos, "set %%1=%ld\n", (long)stat);
	       fclose(fdos);
	    }
#endif
	 } break;
	 case '3': {
	    Uchar stat;
	    if (fread((char*)&stat, sizeof(Uchar), 1, fp) < 1) {
	       exit(1);
	    }
	    fprintf(stdout, "%u\n", (unsigned int)stat);
#if defined _WIN32 || defined WIN32
	    if ((fdos = fopen("pset.bat", "wt")) != 0) {
	       fprintf(fdos, "set %%1=%u\n", (unsigned int)stat);
	       fclose(fdos);
	    }
#endif
	 } break;
	 case '4': {
	    Ushort stat;
	    if (fread((char*)&stat, sizeof(Ushort), 1, fp) < 1) {
	       exit(1);
	    }
	    fprintf(stdout, "%u\n", (unsigned int)stat);
#if defined _WIN32 || defined WIN32
	    if ((fdos = fopen("pset.bat", "wt")) != 0) {
	       fprintf(fdos, "set %%1=%u\n", (unsigned int)stat);
	       fclose(fdos);
	    }
#endif
	 } break;
	 case '5': {
	    Ulong stat;
	    if (fread((char*)&stat, sizeof(Ulong), 1, fp) < 1) {
	       exit(1);
	    }
	    fprintf(stdout, "%lu\n", (unsigned long)stat);
#if defined _WIN32 || defined WIN32
	    if ((fdos = fopen("pset.bat", "wt")) != 0) {
	       fprintf(fdos, "set %%1=%lu\n", (unsigned long)stat);
	       fclose(fdos);
	    }
#endif
	 } break;
	 case '6': {
	    Float stat;
	    if (fread((char*)&stat, sizeof(Float), 1, fp) < 1) {
	       exit(1);
	    }
	    fprintf(stdout, "%f\n", (float)stat);
#if defined _WIN32 || defined WIN32
	    if ((fdos = fopen("pset.bat", "wt")) != 0) {
	       fprintf(fdos, "set %%1=%f\n", (float)stat);
	       fclose(fdos);
	    }
#endif
	 } break;
	 case '7': {
	    Double stat;
	    if (fread((char*)&stat, sizeof(Double), 1, fp) < 1) {
	       exit(1);
	    }
	    fprintf(stdout, "%e\n", (double)stat);
#if defined _WIN32 || defined WIN32
	    if ((fdos = fopen("pset.bat", "wt")) != 0) {
	       fprintf(fdos, "set %%1=%e\n", (double)stat);
	       fclose(fdos);
	    }
#endif
	 } break;
	 case '8': {
	    Llong stat;
	    if (fread((char*)&stat, sizeof(Llong), 1, fp) < 1) {
	       exit(1);
	    }
	    fprintf(stdout, "%lld\n", (long long)stat);
#if defined _WIN32 || defined WIN32
	    if ((fdos = fopen("pset.bat", "wt")) != 0) {
	       fprintf(fdos, "set %%1=%lld\n", (long long)stat);
	       fclose(fdos);
	    }
#endif
	 } break;
	 case '9': {
	    Ullong stat;
	    if (fread((char*)&stat, sizeof(Ullong), 1, fp) < 1) {
	       exit(1);
	    }
	    fprintf(stdout, "%llu\n", (long long unsigned)stat);
#if defined _WIN32 || defined WIN32
	    if ((fdos = fopen("pset.bat", "wt")) != 0) {
	       fprintf(fdos, "set %%1=%llu\n", (long long unsigned)stat);
	       fclose(fdos);
	    }
#endif
	 } break;
      }
      fclose(fp);
      return 0;
   }
   return 1;
}
