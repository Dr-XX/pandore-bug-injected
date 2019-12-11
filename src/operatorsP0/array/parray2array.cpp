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
 * @author Alexandre Duret-Lutz - 1999-11-22
 */

#include <string.h>
#include <pandore.h>
using namespace pandore;

/**
 * @file parray2array.cpp
 * Convert the type of array items.
 */

#ifdef _MSC_VER
// Because Visual C++ does not support strcasecmp....
#include <ctype.h>
int strcasecmp( const char *s1, const char *s2 ) {
   int i;
   for (i = 0; s1[ i ] != '\0' && s2[ i ] != '\0'; ++i) {
      char ch1 = toupper(s1[i]);
      char ch2 = toupper(s2[i]);
      
      if (ch1 != ch2) {
	 return (int) (ch1 - ch2);
      }
   }
   
   return (int) (s1[i] - s2[i]);
}
#endif


Errc Array2Array_Char_Char( Collection &in, const std::string &name) {
  Char* s = in.GETARRAY(name, Char);
  Long size = in.GETARRAYSIZE(name, Char);
  Char* d = new Char[size];
  
  for (int i = 0; i < size; ++i) {
     d[i] = (Char)s[i];
  }
  
  in.SETARRAY(name, Char, d, size);
  return SUCCESS;
}
Errc Array2Array_Char_Uchar( Collection &in, const std::string &name) {
  Char* s = in.GETARRAY(name, Char);
  Long size = in.GETARRAYSIZE(name, Char);
  Uchar* d = new Uchar[size];
  
  for (int i = 0; i < size; ++i) {
     d[i] = (Uchar)s[i];
  }
  
  in.SETARRAY(name, Uchar, d, size);
  return SUCCESS;
}
Errc Array2Array_Char_Short( Collection &in, const std::string &name) {
  Char* s = in.GETARRAY(name, Char);
  Long size = in.GETARRAYSIZE(name, Char);
  Short* d = new Short[size];
  
  for (int i = 0; i < size; ++i) {
     d[i] = (Short)s[i];
  }
  
  in.SETARRAY(name, Short, d, size);
  return SUCCESS;
}
Errc Array2Array_Char_Ushort( Collection &in, const std::string &name) {
  Char* s = in.GETARRAY(name, Char);
  Long size = in.GETARRAYSIZE(name, Char);
  Ushort* d = new Ushort[size];
  
  for (int i = 0; i < size; ++i) {
     d[i] = (Ushort)s[i];
  }
  
  in.SETARRAY(name, Ushort, d, size);
  return SUCCESS;
}
Errc Array2Array_Char_Long( Collection &in, const std::string &name) {
  Char* s = in.GETARRAY(name, Char);
  Long size = in.GETARRAYSIZE(name, Char);
  Long* d = new Long[size];
  
  for (int i = 0; i < size; ++i) {
     d[i] = (Long)s[i];
  }
  
  in.SETARRAY(name, Long, d, size);
  return SUCCESS;
}
Errc Array2Array_Char_Ulong( Collection &in, const std::string &name) {
  Char* s = in.GETARRAY(name, Char);
  Long size = in.GETARRAYSIZE(name, Char);
  Ulong* d = new Ulong[size];
  
  for (int i = 0; i < size; ++i) {
     d[i] = (Ulong)s[i];
  }
  
  in.SETARRAY(name, Ulong, d, size);
  return SUCCESS;
}
Errc Array2Array_Char_Llong( Collection &in, const std::string &name) {
  Char* s = in.GETARRAY(name, Char);
  Long size = in.GETARRAYSIZE(name, Char);
  Llong* d = new Llong[size];
  
  for (int i = 0; i < size; ++i) {
     d[i] = (Llong)s[i];
  }
  
  in.SETARRAY(name, Llong, d, size);
  return SUCCESS;
}
Errc Array2Array_Char_Ullong( Collection &in, const std::string &name) {
  Char* s = in.GETARRAY(name, Char);
  Long size = in.GETARRAYSIZE(name, Char);
  Ullong* d = new Ullong[size];
  
  for (int i = 0; i < size; ++i) {
     d[i] = (Ullong)s[i];
  }
  
  in.SETARRAY(name, Ullong, d, size);
  return SUCCESS;
}
Errc Array2Array_Char_Float( Collection &in, const std::string &name) {
  Char* s = in.GETARRAY(name, Char);
  Long size = in.GETARRAYSIZE(name, Char);
  Float* d = new Float[size];
  
  for (int i = 0; i < size; ++i) {
     d[i] = (Float)s[i];
  }
  
  in.SETARRAY(name, Float, d, size);
  return SUCCESS;
}
Errc Array2Array_Char_Double( Collection &in, const std::string &name) {
  Char* s = in.GETARRAY(name, Char);
  Long size = in.GETARRAYSIZE(name, Char);
  Double* d = new Double[size];
  
  for (int i = 0; i < size; ++i) {
     d[i] = (Double)s[i];
  }
  
  in.SETARRAY(name, Double, d, size);
  return SUCCESS;
}
Errc Array2Array_Uchar_Char( Collection &in, const std::string &name) {
  Uchar* s = in.GETARRAY(name, Uchar);
  Long size = in.GETARRAYSIZE(name, Uchar);
  Char* d = new Char[size];
  
  for (int i = 0; i < size; ++i) {
     d[i] = (Char)s[i];
  }
  
  in.SETARRAY(name, Char, d, size);
  return SUCCESS;
}
Errc Array2Array_Uchar_Uchar( Collection &in, const std::string &name) {
  Uchar* s = in.GETARRAY(name, Uchar);
  Long size = in.GETARRAYSIZE(name, Uchar);
  Uchar* d = new Uchar[size];
  
  for (int i = 0; i < size; ++i) {
     d[i] = (Uchar)s[i];
  }
  
  in.SETARRAY(name, Uchar, d, size);
  return SUCCESS;
}
Errc Array2Array_Uchar_Short( Collection &in, const std::string &name) {
  Uchar* s = in.GETARRAY(name, Uchar);
  Long size = in.GETARRAYSIZE(name, Uchar);
  Short* d = new Short[size];
  
  for (int i = 0; i < size; ++i) {
     d[i] = (Short)s[i];
  }
  
  in.SETARRAY(name, Short, d, size);
  return SUCCESS;
}
Errc Array2Array_Uchar_Ushort( Collection &in, const std::string &name) {
  Uchar* s = in.GETARRAY(name, Uchar);
  Long size = in.GETARRAYSIZE(name, Uchar);
  Ushort* d = new Ushort[size];
  
  for (int i = 0; i < size; ++i) {
     d[i] = (Ushort)s[i];
  }
  
  in.SETARRAY(name, Ushort, d, size);
  return SUCCESS;
}
Errc Array2Array_Uchar_Long( Collection &in, const std::string &name) {
  Uchar* s = in.GETARRAY(name, Uchar);
  Long size = in.GETARRAYSIZE(name, Uchar);
  Long* d = new Long[size];
  
  for (int i = 0; i < size; ++i) {
     d[i] = (Long)s[i];
  }
  
  in.SETARRAY(name, Long, d, size);
  return SUCCESS;
}
Errc Array2Array_Uchar_Ulong( Collection &in, const std::string &name) {
  Uchar* s = in.GETARRAY(name, Uchar);
  Long size = in.GETARRAYSIZE(name, Uchar);
  Ulong* d = new Ulong[size];
  
  for (int i = 0; i < size; ++i) {
     d[i] = (Ulong)s[i];
  }
  
  in.SETARRAY(name, Ulong, d, size);
  return SUCCESS;
}
Errc Array2Array_Uchar_Llong( Collection &in, const std::string &name) {
  Uchar* s = in.GETARRAY(name, Uchar);
  Long size = in.GETARRAYSIZE(name, Uchar);
  Llong* d = new Llong[size];
  
  for (int i = 0; i < size; ++i) {
     d[i] = (Llong)s[i];
  }
  
  in.SETARRAY(name, Llong, d, size);
  return SUCCESS;
}
Errc Array2Array_Uchar_Ullong( Collection &in, const std::string &name) {
  Uchar* s = in.GETARRAY(name, Uchar);
  Long size = in.GETARRAYSIZE(name, Uchar);
  Ullong* d = new Ullong[size];
  
  for (int i = 0; i < size; ++i) {
     d[i] = (Ullong)s[i];
  }
  
  in.SETARRAY(name, Ullong, d, size);
  return SUCCESS;
}
Errc Array2Array_Uchar_Float( Collection &in, const std::string &name) {
  Uchar* s = in.GETARRAY(name, Uchar);
  Long size = in.GETARRAYSIZE(name, Uchar);
  Float* d = new Float[size];
  
  for (int i = 0; i < size; ++i) {
     d[i] = (Float)s[i];
  }
  
  in.SETARRAY(name, Float, d, size);
  return SUCCESS;
}
Errc Array2Array_Uchar_Double( Collection &in, const std::string &name) {
  Uchar* s = in.GETARRAY(name, Uchar);
  Long size = in.GETARRAYSIZE(name, Uchar);
  Double* d = new Double[size];
  
  for (int i = 0; i < size; ++i) {
     d[i] = (Double)s[i];
  }
  
  in.SETARRAY(name, Double, d, size);
  return SUCCESS;
}
Errc Array2Array_Short_Char( Collection &in, const std::string &name) {
  Short* s = in.GETARRAY(name, Short);
  Long size = in.GETARRAYSIZE(name, Short);
  Char* d = new Char[size];
  
  for (int i = 0; i < size; ++i) {
     d[i] = (Char)s[i];
  }
  
  in.SETARRAY(name, Char, d, size);
  return SUCCESS;
}
Errc Array2Array_Short_Uchar( Collection &in, const std::string &name) {
  Short* s = in.GETARRAY(name, Short);
  Long size = in.GETARRAYSIZE(name, Short);
  Uchar* d = new Uchar[size];
  
  for (int i = 0; i < size; ++i) {
     d[i] = (Uchar)s[i];
  }
  
  in.SETARRAY(name, Uchar, d, size);
  return SUCCESS;
}
Errc Array2Array_Short_Short( Collection &in, const std::string &name) {
  Short* s = in.GETARRAY(name, Short);
  Long size = in.GETARRAYSIZE(name, Short);
  Short* d = new Short[size];
  
  for (int i = 0; i < size; ++i) {
     d[i] = (Short)s[i];
  }
  
  in.SETARRAY(name, Short, d, size);
  return SUCCESS;
}
Errc Array2Array_Short_Ushort( Collection &in, const std::string &name) {
  Short* s = in.GETARRAY(name, Short);
  Long size = in.GETARRAYSIZE(name, Short);
  Ushort* d = new Ushort[size];
  
  for (int i = 0; i < size; ++i) {
     d[i] = (Ushort)s[i];
  }
  
  in.SETARRAY(name, Ushort, d, size);
  return SUCCESS;
}
Errc Array2Array_Short_Long( Collection &in, const std::string &name) {
  Short* s = in.GETARRAY(name, Short);
  Long size = in.GETARRAYSIZE(name, Short);
  Long* d = new Long[size];
  
  for (int i = 0; i < size; ++i) {
     d[i] = (Long)s[i];
  }
  
  in.SETARRAY(name, Long, d, size);
  return SUCCESS;
}
Errc Array2Array_Short_Ulong( Collection &in, const std::string &name) {
  Short* s = in.GETARRAY(name, Short);
  Long size = in.GETARRAYSIZE(name, Short);
  Ulong* d = new Ulong[size];
  
  for (int i = 0; i < size; ++i) {
     d[i] = (Ulong)s[i];
  }
  
  in.SETARRAY(name, Ulong, d, size);
  return SUCCESS;
}
Errc Array2Array_Short_Llong( Collection &in, const std::string &name) {
  Short* s = in.GETARRAY(name, Short);
  Long size = in.GETARRAYSIZE(name, Short);
  Llong* d = new Llong[size];
  
  for (int i = 0; i < size; ++i) {
     d[i] = (Llong)s[i];
  }
  
  in.SETARRAY(name, Llong, d, size);
  return SUCCESS;
}
Errc Array2Array_Short_Ullong( Collection &in, const std::string &name) {
  Short* s = in.GETARRAY(name, Short);
  Long size = in.GETARRAYSIZE(name, Short);
  Ullong* d = new Ullong[size];
  
  for (int i = 0; i < size; ++i) {
     d[i] = (Ullong)s[i];
  }
  
  in.SETARRAY(name, Ullong, d, size);
  return SUCCESS;
}
Errc Array2Array_Short_Float( Collection &in, const std::string &name) {
  Short* s = in.GETARRAY(name, Short);
  Long size = in.GETARRAYSIZE(name, Short);
  Float* d = new Float[size];
  
  for (int i = 0; i < size; ++i) {
     d[i] = (Float)s[i];
  }
  
  in.SETARRAY(name, Float, d, size);
  return SUCCESS;
}
Errc Array2Array_Short_Double( Collection &in, const std::string &name) {
  Short* s = in.GETARRAY(name, Short);
  Long size = in.GETARRAYSIZE(name, Short);
  Double* d = new Double[size];
  
  for (int i = 0; i < size; ++i) {
     d[i] = (Double)s[i];
  }
  
  in.SETARRAY(name, Double, d, size);
  return SUCCESS;
}
Errc Array2Array_Ushort_Char( Collection &in, const std::string &name) {
  Ushort* s = in.GETARRAY(name, Ushort);
  Long size = in.GETARRAYSIZE(name, Ushort);
  Char* d = new Char[size];
  
  for (int i = 0; i < size; ++i) {
     d[i] = (Char)s[i];
  }
  
  in.SETARRAY(name, Char, d, size);
  return SUCCESS;
}
Errc Array2Array_Ushort_Uchar( Collection &in, const std::string &name) {
  Ushort* s = in.GETARRAY(name, Ushort);
  Long size = in.GETARRAYSIZE(name, Ushort);
  Uchar* d = new Uchar[size];
  
  for (int i = 0; i < size; ++i) {
     d[i] = (Uchar)s[i];
  }
  
  in.SETARRAY(name, Uchar, d, size);
  return SUCCESS;
}
Errc Array2Array_Ushort_Short( Collection &in, const std::string &name) {
  Ushort* s = in.GETARRAY(name, Ushort);
  Long size = in.GETARRAYSIZE(name, Ushort);
  Short* d = new Short[size];
  
  for (int i = 0; i < size; ++i) {
     d[i] = (Short)s[i];
  }
  
  in.SETARRAY(name, Short, d, size);
  return SUCCESS;
}
Errc Array2Array_Ushort_Ushort( Collection &in, const std::string &name) {
  Ushort* s = in.GETARRAY(name, Ushort);
  Long size = in.GETARRAYSIZE(name, Ushort);
  Ushort* d = new Ushort[size];
  
  for (int i = 0; i < size; ++i) {
     d[i] = (Ushort)s[i];
  }
  
  in.SETARRAY(name, Ushort, d, size);
  return SUCCESS;
}
Errc Array2Array_Ushort_Long( Collection &in, const std::string &name) {
  Ushort* s = in.GETARRAY(name, Ushort);
  Long size = in.GETARRAYSIZE(name, Ushort);
  Long* d = new Long[size];
  
  for (int i = 0; i < size; ++i) {
     d[i] = (Long)s[i];
  }
  
  in.SETARRAY(name, Long, d, size);
  return SUCCESS;
}
Errc Array2Array_Ushort_Ulong( Collection &in, const std::string &name) {
  Ushort* s = in.GETARRAY(name, Ushort);
  Long size = in.GETARRAYSIZE(name, Ushort);
  Ulong* d = new Ulong[size];
  
  for (int i = 0; i < size; ++i) {
     d[i] = (Ulong)s[i];
  }
  
  in.SETARRAY(name, Ulong, d, size);
  return SUCCESS;
}
Errc Array2Array_Ushort_Llong( Collection &in, const std::string &name) {
  Ushort* s = in.GETARRAY(name, Ushort);
  Long size = in.GETARRAYSIZE(name, Ushort);
  Llong* d = new Llong[size];
  
  for (int i = 0; i < size; ++i) {
     d[i] = (Llong)s[i];
  }
  
  in.SETARRAY(name, Llong, d, size);
  return SUCCESS;
}
Errc Array2Array_Ushort_Ullong( Collection &in, const std::string &name) {
  Ushort* s = in.GETARRAY(name, Ushort);
  Long size = in.GETARRAYSIZE(name, Ushort);
  Ullong* d = new Ullong[size];
  
  for (int i = 0; i < size; ++i) {
     d[i] = (Ullong)s[i];
  }
  
  in.SETARRAY(name, Ullong, d, size);
  return SUCCESS;
}
Errc Array2Array_Ushort_Float( Collection &in, const std::string &name) {
  Ushort* s = in.GETARRAY(name, Ushort);
  Long size = in.GETARRAYSIZE(name, Ushort);
  Float* d = new Float[size];
  
  for (int i = 0; i < size; ++i) {
     d[i] = (Float)s[i];
  }
  
  in.SETARRAY(name, Float, d, size);
  return SUCCESS;
}
Errc Array2Array_Ushort_Double( Collection &in, const std::string &name) {
  Ushort* s = in.GETARRAY(name, Ushort);
  Long size = in.GETARRAYSIZE(name, Ushort);
  Double* d = new Double[size];
  
  for (int i = 0; i < size; ++i) {
     d[i] = (Double)s[i];
  }
  
  in.SETARRAY(name, Double, d, size);
  return SUCCESS;
}
Errc Array2Array_Long_Char( Collection &in, const std::string &name) {
  Long* s = in.GETARRAY(name, Long);
  Long size = in.GETARRAYSIZE(name, Long);
  Char* d = new Char[size];
  
  for (int i = 0; i < size; ++i) {
     d[i] = (Char)s[i];
  }
  
  in.SETARRAY(name, Char, d, size);
  return SUCCESS;
}
Errc Array2Array_Long_Uchar( Collection &in, const std::string &name) {
  Long* s = in.GETARRAY(name, Long);
  Long size = in.GETARRAYSIZE(name, Long);
  Uchar* d = new Uchar[size];
  
  for (int i = 0; i < size; ++i) {
     d[i] = (Uchar)s[i];
  }
  
  in.SETARRAY(name, Uchar, d, size);
  return SUCCESS;
}
Errc Array2Array_Long_Short( Collection &in, const std::string &name) {
  Long* s = in.GETARRAY(name, Long);
  Long size = in.GETARRAYSIZE(name, Long);
  Short* d = new Short[size];
  
  for (int i = 0; i < size; ++i) {
     d[i] = (Short)s[i];
  }
  
  in.SETARRAY(name, Short, d, size);
  return SUCCESS;
}
Errc Array2Array_Long_Ushort( Collection &in, const std::string &name) {
  Long* s = in.GETARRAY(name, Long);
  Long size = in.GETARRAYSIZE(name, Long);
  Ushort* d = new Ushort[size];
  
  for (int i = 0; i < size; ++i) {
     d[i] = (Ushort)s[i];
  }
  
  in.SETARRAY(name, Ushort, d, size);
  return SUCCESS;
}
Errc Array2Array_Long_Long( Collection &in, const std::string &name) {
  Long* s = in.GETARRAY(name, Long);
  Long size = in.GETARRAYSIZE(name, Long);
  Long* d = new Long[size];
  
  for (int i = 0; i < size; ++i) {
     d[i] = (Long)s[i];
  }
  
  in.SETARRAY(name, Long, d, size);
  return SUCCESS;
}
Errc Array2Array_Long_Ulong( Collection &in, const std::string &name) {
  Long* s = in.GETARRAY(name, Long);
  Long size = in.GETARRAYSIZE(name, Long);
  Ulong* d = new Ulong[size];
  
  for (int i = 0; i < size; ++i) {
     d[i] = (Ulong)s[i];
  }
  
  in.SETARRAY(name, Ulong, d, size);
  return SUCCESS;
}
Errc Array2Array_Long_Llong( Collection &in, const std::string &name) {
  Long* s = in.GETARRAY(name, Long);
  Long size = in.GETARRAYSIZE(name, Long);
  Llong* d = new Llong[size];
  
  for (int i = 0; i < size; ++i) {
     d[i] = (Llong)s[i];
  }
  
  in.SETARRAY(name, Llong, d, size);
  return SUCCESS;
}
Errc Array2Array_Long_Ullong( Collection &in, const std::string &name) {
  Long* s = in.GETARRAY(name, Long);
  Long size = in.GETARRAYSIZE(name, Long);
  Ullong* d = new Ullong[size];
  
  for (int i = 0; i < size; ++i) {
     d[i] = (Ullong)s[i];
  }
  
  in.SETARRAY(name, Ullong, d, size);
  return SUCCESS;
}
Errc Array2Array_Long_Float( Collection &in, const std::string &name) {
  Long* s = in.GETARRAY(name, Long);
  Long size = in.GETARRAYSIZE(name, Long);
  Float* d = new Float[size];
  
  for (int i = 0; i < size; ++i) {
     d[i] = (Float)s[i];
  }
  
  in.SETARRAY(name, Float, d, size);
  return SUCCESS;
}
Errc Array2Array_Long_Double( Collection &in, const std::string &name) {
  Long* s = in.GETARRAY(name, Long);
  Long size = in.GETARRAYSIZE(name, Long);
  Double* d = new Double[size];
  
  for (int i = 0; i < size; ++i) {
     d[i] = (Double)s[i];
  }
  
  in.SETARRAY(name, Double, d, size);
  return SUCCESS;
}
Errc Array2Array_Ulong_Char( Collection &in, const std::string &name) {
  Ulong* s = in.GETARRAY(name, Ulong);
  Long size = in.GETARRAYSIZE(name, Ulong);
  Char* d = new Char[size];
  
  for (int i = 0; i < size; ++i) {
     d[i] = (Char)s[i];
  }
  
  in.SETARRAY(name, Char, d, size);
  return SUCCESS;
}
Errc Array2Array_Ulong_Uchar( Collection &in, const std::string &name) {
  Ulong* s = in.GETARRAY(name, Ulong);
  Long size = in.GETARRAYSIZE(name, Ulong);
  Uchar* d = new Uchar[size];
  
  for (int i = 0; i < size; ++i) {
     d[i] = (Uchar)s[i];
  }
  
  in.SETARRAY(name, Uchar, d, size);
  return SUCCESS;
}
Errc Array2Array_Ulong_Short( Collection &in, const std::string &name) {
  Ulong* s = in.GETARRAY(name, Ulong);
  Long size = in.GETARRAYSIZE(name, Ulong);
  Short* d = new Short[size];
  
  for (int i = 0; i < size; ++i) {
     d[i] = (Short)s[i];
  }
  
  in.SETARRAY(name, Short, d, size);
  return SUCCESS;
}
Errc Array2Array_Ulong_Ushort( Collection &in, const std::string &name) {
  Ulong* s = in.GETARRAY(name, Ulong);
  Long size = in.GETARRAYSIZE(name, Ulong);
  Ushort* d = new Ushort[size];
  
  for (int i = 0; i < size; ++i) {
     d[i] = (Ushort)s[i];
  }
  
  in.SETARRAY(name, Ushort, d, size);
  return SUCCESS;
}
Errc Array2Array_Ulong_Long( Collection &in, const std::string &name) {
  Ulong* s = in.GETARRAY(name, Ulong);
  Long size = in.GETARRAYSIZE(name, Ulong);
  Long* d = new Long[size];
  
  for (int i = 0; i < size; ++i) {
     d[i] = (Long)s[i];
  }
  
  in.SETARRAY(name, Long, d, size);
  return SUCCESS;
}
Errc Array2Array_Ulong_Ulong( Collection &in, const std::string &name) {
  Ulong* s = in.GETARRAY(name, Ulong);
  Long size = in.GETARRAYSIZE(name, Ulong);
  Ulong* d = new Ulong[size];
  
  for (int i = 0; i < size; ++i) {
     d[i] = (Ulong)s[i];
  }
  
  in.SETARRAY(name, Ulong, d, size);
  return SUCCESS;
}
Errc Array2Array_Ulong_Llong( Collection &in, const std::string &name) {
  Ulong* s = in.GETARRAY(name, Ulong);
  Long size = in.GETARRAYSIZE(name, Ulong);
  Llong* d = new Llong[size];
  
  for (int i = 0; i < size; ++i) {
     d[i] = (Llong)s[i];
  }
  
  in.SETARRAY(name, Llong, d, size);
  return SUCCESS;
}
Errc Array2Array_Ulong_Ullong( Collection &in, const std::string &name) {
  Ulong* s = in.GETARRAY(name, Ulong);
  Long size = in.GETARRAYSIZE(name, Ulong);
  Ullong* d = new Ullong[size];
  
  for (int i = 0; i < size; ++i) {
     d[i] = (Ullong)s[i];
  }
  
  in.SETARRAY(name, Ullong, d, size);
  return SUCCESS;
}
Errc Array2Array_Ulong_Float( Collection &in, const std::string &name) {
  Ulong* s = in.GETARRAY(name, Ulong);
  Long size = in.GETARRAYSIZE(name, Ulong);
  Float* d = new Float[size];
  
  for (int i = 0; i < size; ++i) {
     d[i] = (Float)s[i];
  }
  
  in.SETARRAY(name, Float, d, size);
  return SUCCESS;
}
Errc Array2Array_Ulong_Double( Collection &in, const std::string &name) {
  Ulong* s = in.GETARRAY(name, Ulong);
  Long size = in.GETARRAYSIZE(name, Ulong);
  Double* d = new Double[size];
  
  for (int i = 0; i < size; ++i) {
     d[i] = (Double)s[i];
  }
  
  in.SETARRAY(name, Double, d, size);
  return SUCCESS;
}
Errc Array2Array_Llong_Char( Collection &in, const std::string &name) {
  Llong* s = in.GETARRAY(name, Llong);
  Long size = in.GETARRAYSIZE(name, Llong);
  Char* d = new Char[size];
  
  for (int i = 0; i < size; ++i) {
     d[i] = (Char)s[i];
  }
  
  in.SETARRAY(name, Char, d, size);
  return SUCCESS;
}
Errc Array2Array_Llong_Uchar( Collection &in, const std::string &name) {
  Llong* s = in.GETARRAY(name, Llong);
  Long size = in.GETARRAYSIZE(name, Llong);
  Uchar* d = new Uchar[size];
  
  for (int i = 0; i < size; ++i) {
     d[i] = (Uchar)s[i];
  }
  
  in.SETARRAY(name, Uchar, d, size);
  return SUCCESS;
}
Errc Array2Array_Llong_Short( Collection &in, const std::string &name) {
  Llong* s = in.GETARRAY(name, Llong);
  Long size = in.GETARRAYSIZE(name, Llong);
  Short* d = new Short[size];
  
  for (int i = 0; i < size; ++i) {
     d[i] = (Short)s[i];
  }
  
  in.SETARRAY(name, Short, d, size);
  return SUCCESS;
}
Errc Array2Array_Llong_Ushort( Collection &in, const std::string &name) {
  Llong* s = in.GETARRAY(name, Llong);
  Long size = in.GETARRAYSIZE(name, Llong);
  Ushort* d = new Ushort[size];
  
  for (int i = 0; i < size; ++i) {
     d[i] = (Ushort)s[i];
  }
  
  in.SETARRAY(name, Ushort, d, size);
  return SUCCESS;
}
Errc Array2Array_Llong_Long( Collection &in, const std::string &name) {
  Llong* s = in.GETARRAY(name, Llong);
  Long size = in.GETARRAYSIZE(name, Llong);
  Long* d = new Long[size];
  
  for (int i = 0; i < size; ++i) {
     d[i] = (Long)s[i];
  }
  
  in.SETARRAY(name, Long, d, size);
  return SUCCESS;
}
Errc Array2Array_Llong_Ulong( Collection &in, const std::string &name) {
  Llong* s = in.GETARRAY(name, Llong);
  Long size = in.GETARRAYSIZE(name, Llong);
  Ulong* d = new Ulong[size];
  
  for (int i = 0; i < size; ++i) {
     d[i] = (Ulong)s[i];
  }
  
  in.SETARRAY(name, Ulong, d, size);
  return SUCCESS;
}
Errc Array2Array_Llong_Llong( Collection &in, const std::string &name) {
  Llong* s = in.GETARRAY(name, Llong);
  Long size = in.GETARRAYSIZE(name, Llong);
  Llong* d = new Llong[size];
  
  for (int i = 0; i < size; ++i) {
     d[i] = (Llong)s[i];
  }
  
  in.SETARRAY(name, Llong, d, size);
  return SUCCESS;
}
Errc Array2Array_Llong_Ullong( Collection &in, const std::string &name) {
  Llong* s = in.GETARRAY(name, Llong);
  Long size = in.GETARRAYSIZE(name, Llong);
  Ullong* d = new Ullong[size];
  
  for (int i = 0; i < size; ++i) {
     d[i] = (Ullong)s[i];
  }
  
  in.SETARRAY(name, Ullong, d, size);
  return SUCCESS;
}
Errc Array2Array_Llong_Float( Collection &in, const std::string &name) {
  Llong* s = in.GETARRAY(name, Llong);
  Long size = in.GETARRAYSIZE(name, Llong);
  Float* d = new Float[size];
  
  for (int i = 0; i < size; ++i) {
     d[i] = (Float)s[i];
  }
  
  in.SETARRAY(name, Float, d, size);
  return SUCCESS;
}
Errc Array2Array_Llong_Double( Collection &in, const std::string &name) {
  Llong* s = in.GETARRAY(name, Llong);
  Long size = in.GETARRAYSIZE(name, Llong);
  Double* d = new Double[size];
  
  for (int i = 0; i < size; ++i) {
     d[i] = (Double)s[i];
  }
  
  in.SETARRAY(name, Double, d, size);
  return SUCCESS;
}
Errc Array2Array_Ullong_Char( Collection &in, const std::string &name) {
  Ullong* s = in.GETARRAY(name, Ullong);
  Long size = in.GETARRAYSIZE(name, Ullong);
  Char* d = new Char[size];
  
  for (int i = 0; i < size; ++i) {
     d[i] = (Char)s[i];
  }
  
  in.SETARRAY(name, Char, d, size);
  return SUCCESS;
}
Errc Array2Array_Ullong_Uchar( Collection &in, const std::string &name) {
  Ullong* s = in.GETARRAY(name, Ullong);
  Long size = in.GETARRAYSIZE(name, Ullong);
  Uchar* d = new Uchar[size];
  
  for (int i = 0; i < size; ++i) {
     d[i] = (Uchar)s[i];
  }
  
  in.SETARRAY(name, Uchar, d, size);
  return SUCCESS;
}
Errc Array2Array_Ullong_Short( Collection &in, const std::string &name) {
  Ullong* s = in.GETARRAY(name, Ullong);
  Long size = in.GETARRAYSIZE(name, Ullong);
  Short* d = new Short[size];
  
  for (int i = 0; i < size; ++i) {
     d[i] = (Short)s[i];
  }
  
  in.SETARRAY(name, Short, d, size);
  return SUCCESS;
}
Errc Array2Array_Ullong_Ushort( Collection &in, const std::string &name) {
  Ullong* s = in.GETARRAY(name, Ullong);
  Long size = in.GETARRAYSIZE(name, Ullong);
  Ushort* d = new Ushort[size];
  
  for (int i = 0; i < size; ++i) {
     d[i] = (Ushort)s[i];
  }
  
  in.SETARRAY(name, Ushort, d, size);
  return SUCCESS;
}
Errc Array2Array_Ullong_Long( Collection &in, const std::string &name) {
  Ullong* s = in.GETARRAY(name, Ullong);
  Long size = in.GETARRAYSIZE(name, Ullong);
  Long* d = new Long[size];
  
  for (int i = 0; i < size; ++i) {
     d[i] = (Long)s[i];
  }
  
  in.SETARRAY(name, Long, d, size);
  return SUCCESS;
}
Errc Array2Array_Ullong_Ulong( Collection &in, const std::string &name) {
  Ullong* s = in.GETARRAY(name, Ullong);
  Long size = in.GETARRAYSIZE(name, Ullong);
  Ulong* d = new Ulong[size];
  
  for (int i = 0; i < size; ++i) {
     d[i] = (Ulong)s[i];
  }
  
  in.SETARRAY(name, Ulong, d, size);
  return SUCCESS;
}
Errc Array2Array_Ullong_Llong( Collection &in, const std::string &name) {
  Ullong* s = in.GETARRAY(name, Ullong);
  Long size = in.GETARRAYSIZE(name, Ullong);
  Llong* d = new Llong[size];
  
  for (int i = 0; i < size; ++i) {
     d[i] = (Llong)s[i];
  }
  
  in.SETARRAY(name, Llong, d, size);
  return SUCCESS;
}
Errc Array2Array_Ullong_Ullong( Collection &in, const std::string &name) {
  Ullong* s = in.GETARRAY(name, Ullong);
  Long size = in.GETARRAYSIZE(name, Ullong);
  Ullong* d = new Ullong[size];
  
  for (int i = 0; i < size; ++i) {
     d[i] = (Ullong)s[i];
  }
  
  in.SETARRAY(name, Ullong, d, size);
  return SUCCESS;
}
Errc Array2Array_Ullong_Float( Collection &in, const std::string &name) {
  Ullong* s = in.GETARRAY(name, Ullong);
  Long size = in.GETARRAYSIZE(name, Ullong);
  Float* d = new Float[size];
  
  for (int i = 0; i < size; ++i) {
     d[i] = (Float)s[i];
  }
  
  in.SETARRAY(name, Float, d, size);
  return SUCCESS;
}
Errc Array2Array_Ullong_Double( Collection &in, const std::string &name) {
  Ullong* s = in.GETARRAY(name, Ullong);
  Long size = in.GETARRAYSIZE(name, Ullong);
  Double* d = new Double[size];
  
  for (int i = 0; i < size; ++i) {
     d[i] = (Double)s[i];
  }
  
  in.SETARRAY(name, Double, d, size);
  return SUCCESS;
}
Errc Array2Array_Float_Char( Collection &in, const std::string &name) {
  Float* s = in.GETARRAY(name, Float);
  Long size = in.GETARRAYSIZE(name, Float);
  Char* d = new Char[size];
  
  for (int i = 0; i < size; ++i) {
     d[i] = (Char)s[i];
  }
  
  in.SETARRAY(name, Char, d, size);
  return SUCCESS;
}
Errc Array2Array_Float_Uchar( Collection &in, const std::string &name) {
  Float* s = in.GETARRAY(name, Float);
  Long size = in.GETARRAYSIZE(name, Float);
  Uchar* d = new Uchar[size];
  
  for (int i = 0; i < size; ++i) {
     d[i] = (Uchar)s[i];
  }
  
  in.SETARRAY(name, Uchar, d, size);
  return SUCCESS;
}
Errc Array2Array_Float_Short( Collection &in, const std::string &name) {
  Float* s = in.GETARRAY(name, Float);
  Long size = in.GETARRAYSIZE(name, Float);
  Short* d = new Short[size];
  
  for (int i = 0; i < size; ++i) {
     d[i] = (Short)s[i];
  }
  
  in.SETARRAY(name, Short, d, size);
  return SUCCESS;
}
Errc Array2Array_Float_Ushort( Collection &in, const std::string &name) {
  Float* s = in.GETARRAY(name, Float);
  Long size = in.GETARRAYSIZE(name, Float);
  Ushort* d = new Ushort[size];
  
  for (int i = 0; i < size; ++i) {
     d[i] = (Ushort)s[i];
  }
  
  in.SETARRAY(name, Ushort, d, size);
  return SUCCESS;
}
Errc Array2Array_Float_Long( Collection &in, const std::string &name) {
  Float* s = in.GETARRAY(name, Float);
  Long size = in.GETARRAYSIZE(name, Float);
  Long* d = new Long[size];
  
  for (int i = 0; i < size; ++i) {
     d[i] = (Long)s[i];
  }
  
  in.SETARRAY(name, Long, d, size);
  return SUCCESS;
}
Errc Array2Array_Float_Ulong( Collection &in, const std::string &name) {
  Float* s = in.GETARRAY(name, Float);
  Long size = in.GETARRAYSIZE(name, Float);
  Ulong* d = new Ulong[size];
  
  for (int i = 0; i < size; ++i) {
     d[i] = (Ulong)s[i];
  }
  
  in.SETARRAY(name, Ulong, d, size);
  return SUCCESS;
}
Errc Array2Array_Float_Llong( Collection &in, const std::string &name) {
  Float* s = in.GETARRAY(name, Float);
  Long size = in.GETARRAYSIZE(name, Float);
  Llong* d = new Llong[size];
  
  for (int i = 0; i < size; ++i) {
     d[i] = (Llong)s[i];
  }
  
  in.SETARRAY(name, Llong, d, size);
  return SUCCESS;
}
Errc Array2Array_Float_Ullong( Collection &in, const std::string &name) {
  Float* s = in.GETARRAY(name, Float);
  Long size = in.GETARRAYSIZE(name, Float);
  Ullong* d = new Ullong[size];
  
  for (int i = 0; i < size; ++i) {
     d[i] = (Ullong)s[i];
  }
  
  in.SETARRAY(name, Ullong, d, size);
  return SUCCESS;
}
Errc Array2Array_Float_Float( Collection &in, const std::string &name) {
  Float* s = in.GETARRAY(name, Float);
  Long size = in.GETARRAYSIZE(name, Float);
  Float* d = new Float[size];
  
  for (int i = 0; i < size; ++i) {
     d[i] = (Float)s[i];
  }
  
  in.SETARRAY(name, Float, d, size);
  return SUCCESS;
}
Errc Array2Array_Float_Double( Collection &in, const std::string &name) {
  Float* s = in.GETARRAY(name, Float);
  Long size = in.GETARRAYSIZE(name, Float);
  Double* d = new Double[size];
  
  for (int i = 0; i < size; ++i) {
     d[i] = (Double)s[i];
  }
  
  in.SETARRAY(name, Double, d, size);
  return SUCCESS;
}
Errc Array2Array_Double_Char( Collection &in, const std::string &name) {
  Double* s = in.GETARRAY(name, Double);
  Long size = in.GETARRAYSIZE(name, Double);
  Char* d = new Char[size];
  
  for (int i = 0; i < size; ++i) {
     d[i] = (Char)s[i];
  }
  
  in.SETARRAY(name, Char, d, size);
  return SUCCESS;
}
Errc Array2Array_Double_Uchar( Collection &in, const std::string &name) {
  Double* s = in.GETARRAY(name, Double);
  Long size = in.GETARRAYSIZE(name, Double);
  Uchar* d = new Uchar[size];
  
  for (int i = 0; i < size; ++i) {
     d[i] = (Uchar)s[i];
  }
  
  in.SETARRAY(name, Uchar, d, size);
  return SUCCESS;
}
Errc Array2Array_Double_Short( Collection &in, const std::string &name) {
  Double* s = in.GETARRAY(name, Double);
  Long size = in.GETARRAYSIZE(name, Double);
  Short* d = new Short[size];
  
  for (int i = 0; i < size; ++i) {
     d[i] = (Short)s[i];
  }
  
  in.SETARRAY(name, Short, d, size);
  return SUCCESS;
}
Errc Array2Array_Double_Ushort( Collection &in, const std::string &name) {
  Double* s = in.GETARRAY(name, Double);
  Long size = in.GETARRAYSIZE(name, Double);
  Ushort* d = new Ushort[size];
  
  for (int i = 0; i < size; ++i) {
     d[i] = (Ushort)s[i];
  }
  
  in.SETARRAY(name, Ushort, d, size);
  return SUCCESS;
}
Errc Array2Array_Double_Long( Collection &in, const std::string &name) {
  Double* s = in.GETARRAY(name, Double);
  Long size = in.GETARRAYSIZE(name, Double);
  Long* d = new Long[size];
  
  for (int i = 0; i < size; ++i) {
     d[i] = (Long)s[i];
  }
  
  in.SETARRAY(name, Long, d, size);
  return SUCCESS;
}
Errc Array2Array_Double_Ulong( Collection &in, const std::string &name) {
  Double* s = in.GETARRAY(name, Double);
  Long size = in.GETARRAYSIZE(name, Double);
  Ulong* d = new Ulong[size];
  
  for (int i = 0; i < size; ++i) {
     d[i] = (Ulong)s[i];
  }
  
  in.SETARRAY(name, Ulong, d, size);
  return SUCCESS;
}
Errc Array2Array_Double_Llong( Collection &in, const std::string &name) {
  Double* s = in.GETARRAY(name, Double);
  Long size = in.GETARRAYSIZE(name, Double);
  Llong* d = new Llong[size];
  
  for (int i = 0; i < size; ++i) {
     d[i] = (Llong)s[i];
  }
  
  in.SETARRAY(name, Llong, d, size);
  return SUCCESS;
}
Errc Array2Array_Double_Ullong( Collection &in, const std::string &name) {
  Double* s = in.GETARRAY(name, Double);
  Long size = in.GETARRAYSIZE(name, Double);
  Ullong* d = new Ullong[size];
  
  for (int i = 0; i < size; ++i) {
     d[i] = (Ullong)s[i];
  }
  
  in.SETARRAY(name, Ullong, d, size);
  return SUCCESS;
}
Errc Array2Array_Double_Float( Collection &in, const std::string &name) {
  Double* s = in.GETARRAY(name, Double);
  Long size = in.GETARRAYSIZE(name, Double);
  Float* d = new Float[size];
  
  for (int i = 0; i < size; ++i) {
     d[i] = (Float)s[i];
  }
  
  in.SETARRAY(name, Float, d, size);
  return SUCCESS;
}
Errc Array2Array_Double_Double( Collection &in, const std::string &name) {
  Double* s = in.GETARRAY(name, Double);
  Long size = in.GETARRAYSIZE(name, Double);
  Double* d = new Double[size];
  
  for (int i = 0; i < size; ++i) {
     d[i] = (Double)s[i];
  }
  
  in.SETARRAY(name, Double, d, size);
  return SUCCESS;
}

Errc Array2Array( Collection &in, const std::string &name, const std::string &type_out ) {
  std::string type = in.GetType(name);
  if (type == "Array:Char") {
    if (!strcasecmp(type_out.c_str(), "Char")) {
      return Array2Array_Char_Char(in, name);
    }
    if (!strcasecmp(type_out.c_str(), "Uchar")) {
      return Array2Array_Char_Uchar(in, name);
    }
    if (!strcasecmp(type_out.c_str(), "Short")) {
      return Array2Array_Char_Short(in, name);
    }
    if (!strcasecmp(type_out.c_str(), "Ushort")) {
      return Array2Array_Char_Ushort(in, name);
    }
    if (!strcasecmp(type_out.c_str(), "Long")) {
      return Array2Array_Char_Long(in, name);
    }
    if (!strcasecmp(type_out.c_str(), "Ulong")) {
      return Array2Array_Char_Ulong(in, name);
    }
    if (!strcasecmp(type_out.c_str(), "Llong")) {
      return Array2Array_Char_Llong(in, name);
    }
    if (!strcasecmp(type_out.c_str(), "Ullong")) {
      return Array2Array_Char_Ullong(in, name);
    }
    if (!strcasecmp(type_out.c_str(), "Float")) {
      return Array2Array_Char_Float(in, name);
    }
    if (!strcasecmp(type_out.c_str(), "Double")) {
      return Array2Array_Char_Double(in, name);
    }
  } 
  if (type == "Array:Uchar") {
    if (!strcasecmp(type_out.c_str(), "Char")) {
      return Array2Array_Uchar_Char(in, name);
    }
    if (!strcasecmp(type_out.c_str(), "Uchar")) {
      return Array2Array_Uchar_Uchar(in, name);
    }
    if (!strcasecmp(type_out.c_str(), "Short")) {
      return Array2Array_Uchar_Short(in, name);
    }
    if (!strcasecmp(type_out.c_str(), "Ushort")) {
      return Array2Array_Uchar_Ushort(in, name);
    }
    if (!strcasecmp(type_out.c_str(), "Long")) {
      return Array2Array_Uchar_Long(in, name);
    }
    if (!strcasecmp(type_out.c_str(), "Ulong")) {
      return Array2Array_Uchar_Ulong(in, name);
    }
    if (!strcasecmp(type_out.c_str(), "Llong")) {
      return Array2Array_Uchar_Llong(in, name);
    }
    if (!strcasecmp(type_out.c_str(), "Ullong")) {
      return Array2Array_Uchar_Ullong(in, name);
    }
    if (!strcasecmp(type_out.c_str(), "Float")) {
      return Array2Array_Uchar_Float(in, name);
    }
    if (!strcasecmp(type_out.c_str(), "Double")) {
      return Array2Array_Uchar_Double(in, name);
    }
  } 
  if (type == "Array:Short") {
    if (!strcasecmp(type_out.c_str(), "Char")) {
      return Array2Array_Short_Char(in, name);
    }
    if (!strcasecmp(type_out.c_str(), "Uchar")) {
      return Array2Array_Short_Uchar(in, name);
    }
    if (!strcasecmp(type_out.c_str(), "Short")) {
      return Array2Array_Short_Short(in, name);
    }
    if (!strcasecmp(type_out.c_str(), "Ushort")) {
      return Array2Array_Short_Ushort(in, name);
    }
    if (!strcasecmp(type_out.c_str(), "Long")) {
      return Array2Array_Short_Long(in, name);
    }
    if (!strcasecmp(type_out.c_str(), "Ulong")) {
      return Array2Array_Short_Ulong(in, name);
    }
    if (!strcasecmp(type_out.c_str(), "Llong")) {
      return Array2Array_Short_Llong(in, name);
    }
    if (!strcasecmp(type_out.c_str(), "Ullong")) {
      return Array2Array_Short_Ullong(in, name);
    }
    if (!strcasecmp(type_out.c_str(), "Float")) {
      return Array2Array_Short_Float(in, name);
    }
    if (!strcasecmp(type_out.c_str(), "Double")) {
      return Array2Array_Short_Double(in, name);
    }
  } 
  if (type == "Array:Ushort") {
    if (!strcasecmp(type_out.c_str(), "Char")) {
      return Array2Array_Ushort_Char(in, name);
    }
    if (!strcasecmp(type_out.c_str(), "Uchar")) {
      return Array2Array_Ushort_Uchar(in, name);
    }
    if (!strcasecmp(type_out.c_str(), "Short")) {
      return Array2Array_Ushort_Short(in, name);
    }
    if (!strcasecmp(type_out.c_str(), "Ushort")) {
      return Array2Array_Ushort_Ushort(in, name);
    }
    if (!strcasecmp(type_out.c_str(), "Long")) {
      return Array2Array_Ushort_Long(in, name);
    }
    if (!strcasecmp(type_out.c_str(), "Ulong")) {
      return Array2Array_Ushort_Ulong(in, name);
    }
    if (!strcasecmp(type_out.c_str(), "Llong")) {
      return Array2Array_Ushort_Llong(in, name);
    }
    if (!strcasecmp(type_out.c_str(), "Ullong")) {
      return Array2Array_Ushort_Ullong(in, name);
    }
    if (!strcasecmp(type_out.c_str(), "Float")) {
      return Array2Array_Ushort_Float(in, name);
    }
    if (!strcasecmp(type_out.c_str(), "Double")) {
      return Array2Array_Ushort_Double(in, name);
    }
  } 
  if (type == "Array:Long") {
    if (!strcasecmp(type_out.c_str(), "Char")) {
      return Array2Array_Long_Char(in, name);
    }
    if (!strcasecmp(type_out.c_str(), "Uchar")) {
      return Array2Array_Long_Uchar(in, name);
    }
    if (!strcasecmp(type_out.c_str(), "Short")) {
      return Array2Array_Long_Short(in, name);
    }
    if (!strcasecmp(type_out.c_str(), "Ushort")) {
      return Array2Array_Long_Ushort(in, name);
    }
    if (!strcasecmp(type_out.c_str(), "Long")) {
      return Array2Array_Long_Long(in, name);
    }
    if (!strcasecmp(type_out.c_str(), "Ulong")) {
      return Array2Array_Long_Ulong(in, name);
    }
    if (!strcasecmp(type_out.c_str(), "Llong")) {
      return Array2Array_Long_Llong(in, name);
    }
    if (!strcasecmp(type_out.c_str(), "Ullong")) {
      return Array2Array_Long_Ullong(in, name);
    }
    if (!strcasecmp(type_out.c_str(), "Float")) {
      return Array2Array_Long_Float(in, name);
    }
    if (!strcasecmp(type_out.c_str(), "Double")) {
      return Array2Array_Long_Double(in, name);
    }
  } 
  if (type == "Array:Ulong") {
    if (!strcasecmp(type_out.c_str(), "Char")) {
      return Array2Array_Ulong_Char(in, name);
    }
    if (!strcasecmp(type_out.c_str(), "Uchar")) {
      return Array2Array_Ulong_Uchar(in, name);
    }
    if (!strcasecmp(type_out.c_str(), "Short")) {
      return Array2Array_Ulong_Short(in, name);
    }
    if (!strcasecmp(type_out.c_str(), "Ushort")) {
      return Array2Array_Ulong_Ushort(in, name);
    }
    if (!strcasecmp(type_out.c_str(), "Long")) {
      return Array2Array_Ulong_Long(in, name);
    }
    if (!strcasecmp(type_out.c_str(), "Ulong")) {
      return Array2Array_Ulong_Ulong(in, name);
    }
    if (!strcasecmp(type_out.c_str(), "Llong")) {
      return Array2Array_Ulong_Llong(in, name);
    }
    if (!strcasecmp(type_out.c_str(), "Ullong")) {
      return Array2Array_Ulong_Ullong(in, name);
    }
    if (!strcasecmp(type_out.c_str(), "Float")) {
      return Array2Array_Ulong_Float(in, name);
    }
    if (!strcasecmp(type_out.c_str(), "Double")) {
      return Array2Array_Ulong_Double(in, name);
    }
  } 
  if (type == "Array:Llong") {
    if (!strcasecmp(type_out.c_str(), "Char")) {
      return Array2Array_Llong_Char(in, name);
    }
    if (!strcasecmp(type_out.c_str(), "Uchar")) {
      return Array2Array_Llong_Uchar(in, name);
    }
    if (!strcasecmp(type_out.c_str(), "Short")) {
      return Array2Array_Llong_Short(in, name);
    }
    if (!strcasecmp(type_out.c_str(), "Ushort")) {
      return Array2Array_Llong_Ushort(in, name);
    }
    if (!strcasecmp(type_out.c_str(), "Long")) {
      return Array2Array_Llong_Long(in, name);
    }
    if (!strcasecmp(type_out.c_str(), "Ulong")) {
      return Array2Array_Llong_Ulong(in, name);
    }
    if (!strcasecmp(type_out.c_str(), "Llong")) {
      return Array2Array_Llong_Llong(in, name);
    }
    if (!strcasecmp(type_out.c_str(), "Ullong")) {
      return Array2Array_Llong_Ullong(in, name);
    }
    if (!strcasecmp(type_out.c_str(), "Float")) {
      return Array2Array_Llong_Float(in, name);
    }
    if (!strcasecmp(type_out.c_str(), "Double")) {
      return Array2Array_Llong_Double(in, name);
    }
  } 
  if (type == "Array:Ullong") {
    if (!strcasecmp(type_out.c_str(), "Char")) {
      return Array2Array_Ullong_Char(in, name);
    }
    if (!strcasecmp(type_out.c_str(), "Uchar")) {
      return Array2Array_Ullong_Uchar(in, name);
    }
    if (!strcasecmp(type_out.c_str(), "Short")) {
      return Array2Array_Ullong_Short(in, name);
    }
    if (!strcasecmp(type_out.c_str(), "Ushort")) {
      return Array2Array_Ullong_Ushort(in, name);
    }
    if (!strcasecmp(type_out.c_str(), "Long")) {
      return Array2Array_Ullong_Long(in, name);
    }
    if (!strcasecmp(type_out.c_str(), "Ulong")) {
      return Array2Array_Ullong_Ulong(in, name);
    }
    if (!strcasecmp(type_out.c_str(), "Llong")) {
      return Array2Array_Ullong_Llong(in, name);
    }
    if (!strcasecmp(type_out.c_str(), "Ullong")) {
      return Array2Array_Ullong_Ullong(in, name);
    }
    if (!strcasecmp(type_out.c_str(), "Float")) {
      return Array2Array_Ullong_Float(in, name);
    }
    if (!strcasecmp(type_out.c_str(), "Double")) {
      return Array2Array_Ullong_Double(in, name);
    }
  } 
  if (type == "Array:Float") {
    if (!strcasecmp(type_out.c_str(), "Char")) {
      return Array2Array_Float_Char(in, name);
    }
    if (!strcasecmp(type_out.c_str(), "Uchar")) {
      return Array2Array_Float_Uchar(in, name);
    }
    if (!strcasecmp(type_out.c_str(), "Short")) {
      return Array2Array_Float_Short(in, name);
    }
    if (!strcasecmp(type_out.c_str(), "Ushort")) {
      return Array2Array_Float_Ushort(in, name);
    }
    if (!strcasecmp(type_out.c_str(), "Long")) {
      return Array2Array_Float_Long(in, name);
    }
    if (!strcasecmp(type_out.c_str(), "Ulong")) {
      return Array2Array_Float_Ulong(in, name);
    }
    if (!strcasecmp(type_out.c_str(), "Llong")) {
      return Array2Array_Float_Llong(in, name);
    }
    if (!strcasecmp(type_out.c_str(), "Ullong")) {
      return Array2Array_Float_Ullong(in, name);
    }
    if (!strcasecmp(type_out.c_str(), "Float")) {
      return Array2Array_Float_Float(in, name);
    }
    if (!strcasecmp(type_out.c_str(), "Double")) {
      return Array2Array_Float_Double(in, name);
    }
  } 
  if (type == "Array:Double") {
    if (!strcasecmp(type_out.c_str(), "Char")) {
      return Array2Array_Double_Char(in, name);
    }
    if (!strcasecmp(type_out.c_str(), "Uchar")) {
      return Array2Array_Double_Uchar(in, name);
    }
    if (!strcasecmp(type_out.c_str(), "Short")) {
      return Array2Array_Double_Short(in, name);
    }
    if (!strcasecmp(type_out.c_str(), "Ushort")) {
      return Array2Array_Double_Ushort(in, name);
    }
    if (!strcasecmp(type_out.c_str(), "Long")) {
      return Array2Array_Double_Long(in, name);
    }
    if (!strcasecmp(type_out.c_str(), "Ulong")) {
      return Array2Array_Double_Ulong(in, name);
    }
    if (!strcasecmp(type_out.c_str(), "Llong")) {
      return Array2Array_Double_Llong(in, name);
    }
    if (!strcasecmp(type_out.c_str(), "Ullong")) {
      return Array2Array_Double_Ullong(in, name);
    }
    if (!strcasecmp(type_out.c_str(), "Float")) {
      return Array2Array_Double_Float(in, name);
    }
    if (!strcasecmp(type_out.c_str(), "Double")) {
      return Array2Array_Double_Double(in, name);
    }
  } 
 {
    std::cerr << "Error parray2array: Invalid collection type" << std::endl;
    return FAILURE;
 }
}

#ifdef MAIN

/*
 * Modify only the following constants, and the function call.
 */
#define	USAGE	"usage: %s name type [col_in|-] [col_out|-]"
#define	PARC	2
#define	FINC	1
#define	FOUTC	1
#define	MASK	0

int main( int argc, char *argv[] ) {
   Errc	 result;	       // The result code of the execution.
   Pobject* mask;	       // The region map.
   Pobject* objin[FINC + 1];   // The input objects;
   Pobject* objs[FINC + 1];    // The source objects masked by the mask.
   Pobject* objout[FOUTC + 1]; // The output object.
   Pobject* objd[FOUTC + 1];   // The result object of the execution.
   char* parv[PARC + 1];       // The input parameters.
   
   ReadArgs(argc, argv, PARC, FINC, FOUTC, &mask, objin, objs, objout, objd, parv, USAGE, MASK);
   
   if (objs[0]->Type() == Po_Collection) {
      Collection* ims1 = (Collection*)objs[0];
      objd[0] = ims1;
      result = Array2Array(*ims1, parv[0], parv[1]);
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
