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
 * https://clouard.users.greyc.fr/Pantheon
 */

#ifndef _FILEUTILS_H
#define _FILEUTILS_H

#ifndef WIN32
#include <dirent.h>
#include <strings.h>
#else
#include "dirent.h"
#endif

#include <sys/stat.h>
#include <errno.h>
#include <string> 
#include <string.h> 
#include <iostream>
#include <algorithm>
#include <vector>

const std::string PATH_SEPARATOR =
#ifdef _WIN32
                                    "\\";
#else
                                    "/";
#endif

inline const std::string
basename( const std::string & path ) {
   const char path_separator = PATH_SEPARATOR.at(0);
   const char *absolutePath = path.c_str();
   const char *base = strrchr(absolutePath, path_separator);
   return base ? std::string(base + 1) : path;
}

inline const std::string 
dirname( const std::string & path ) {
   char path_separator = PATH_SEPARATOR.at(0);
   const char *absolutePath = path.c_str();
   const char *base = strrchr(absolutePath, path_separator);
   if (base) {
      return std::string(path, 0, (int)(base + 1 - absolutePath));
   } else {
      return path;
   }
}

inline const char *
getSubdirectoryName( const char * directory1, const char * directory2 ) {
   return directory1 + strlen(directory2);
}

inline bool
endsWith( const std::string & a, const std::string & b ) {
   if (b.size() > a.size()) {
      return false;
   }
   return std::equal(a.begin() + a.size() - b.size(), a.end(), b.begin());
}

inline bool
isDirectory( const std::string & dir ) {
   struct stat fileInfo;
   stat(dir.c_str(), &fileInfo);
   return S_ISDIR(fileInfo.st_mode);
}

inline bool
isFile( const std::string file ) {
   struct stat fileInfo;
   if (!stat(file.c_str(), &fileInfo)) {
      return S_ISREG(fileInfo.st_mode);
   } else {
      return false;
   }
}

inline bool
getAllFilesWithExtension( const std::string & baseDir, const std::string & extension, std::vector<std::string> &files ) {
   DIR *dp;
   struct dirent *dirp;
   
   if ((dp = opendir(baseDir.c_str())) == NULL) {
      std::cerr << "[ERROR: " << errno << " ] Couldn't open folder " << baseDir << "." << std::endl;
      return false;
   } else {
      while ((dirp = readdir(dp)) != NULL) {
	 if (dirp->d_name != std::string(".") && dirp->d_name != std::string("..")) {
	    if (isDirectory(baseDir + dirp->d_name)) {
//	       std::cout << "[DIR]\t" << baseDir << dirp->d_name << "/" << std::endl;
	       getAllFilesWithExtension(baseDir + dirp->d_name + PATH_SEPARATOR, extension, files);
	    } else {
	       // a file
	       //files.push_back(string(dirp->d_name));
	       std::string filename = dirp->d_name;
	       if (endsWith(filename, extension)) {
//		  std::cout << "[FILE]\t" << baseDir << filename << std::endl;
		  files.push_back(baseDir + filename);
	       }
	    }
	 }
      }
      closedir(dp);
   }
   return true;
}

inline void
getSubdirectories( const std::string & baseDir, std::vector<std::string> &directories ) {
   DIR *dp;
   struct dirent *dirp;
   
   if ((dp = opendir(baseDir.c_str())) == NULL) {
      std::cerr << "[ERROR: " << errno << " ] Couldn't open " << baseDir << "." << std::endl;
      return;
   } else {
      while ((dirp = readdir(dp)) != NULL) {
	 if (dirp->d_name != std::string(".") && dirp->d_name != std::string("..")) {
	    if (isDirectory(baseDir + dirp->d_name)) {
	       directories.push_back(baseDir + dirp->d_name);
	    }
	 }
      }
      closedir(dp);
   }
}

inline bool
isRegularDirectory( const std::string & dirname ) {
   DIR *dp;
   if ((dp = opendir(dirname.c_str())) == NULL) {
      return false;
   }
   closedir(dp);
   return true;
}

#endif
