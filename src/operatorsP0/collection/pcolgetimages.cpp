/* -*- mode: c++; c-basic-offset: 3 -*-
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
 * @author Nicolas Briand - 2005-25-07
 * @bug 
 * @todo
 */

#include <iostream>
#include <list>

#include <pandore.h>
using namespace pandore;

#if (defined _MSC_VER) && (!defined snprintf)
#define snprintf _snprintf
#endif

/**
 * @file pcolgetimages.cpp
 * @brief Extracts an array of images from in a collection.
 *
 * Extracts the different Pobjects from a collection and saves these
 * them in different files with the name specified in the collection.
 * @param c_in The Collection.
 */
Errc PColGetImages( Collection &c_in ) {
  std::list<std::string> l = c_in.List();
  std::list<std::string>::iterator i = l.begin();
  for (i = l.begin(); i != l.end(); i++) {
    std::string type = c_in.GetType(*i);
   if (type == "Pobject:Collection") {
      Pobject *out = c_in.GETPOBJECT(*i,Collection);
      const char *c = i->c_str();
      char t[64];
      snprintf(t, sizeof(t), "%s.pan", c);
      t[sizeof(t) - 1] = 0;
#ifdef DEBUG
      fprintf(stderr, "%s -> %s\n", type.c_str(), t);
#endif
      if (out->SaveFile(t) == FAILURE) {
	 return FAILURE;
      }
   } else if (type == "PArray:Collection") {
      Collection **out = c_in.GETPARRAY(*i, Collection);
      int nb = (int)(c_in.GETPARRAYSIZE(*i, Collection));
      const char *c = i->c_str();
      for (int j = 0; j < nb; j++) {
	 char t[64];
	 snprintf(t, sizeof(t), "%s.%d.pan", c, j + 1);
	 t[sizeof(t) - 1] = 0;
#ifdef DEBUG
	 fprintf(stderr,"%s -> %s\n", type.c_str(), t);
#endif
	 if (out[j]->SaveFile(t) == FAILURE) {
	    return FAILURE;
	 }
      }
   } else
   if (type == "Pobject:Img1duc") {
      Pobject *out = c_in.GETPOBJECT(*i,Img1duc);
      const char *c = i->c_str();
      char t[64];
      snprintf(t, sizeof(t), "%s.pan", c);
      t[sizeof(t) - 1] = 0;
#ifdef DEBUG
      fprintf(stderr, "%s -> %s\n", type.c_str(), t);
#endif
      if (out->SaveFile(t) == FAILURE) {
	 return FAILURE;
      }
   } else if (type == "PArray:Img1duc") {
      Img1duc **out = c_in.GETPARRAY(*i, Img1duc);
      int nb = (int)(c_in.GETPARRAYSIZE(*i, Img1duc));
      const char *c = i->c_str();
      for (int j = 0; j < nb; j++) {
	 char t[64];
	 snprintf(t, sizeof(t), "%s.%d.pan", c, j + 1);
	 t[sizeof(t) - 1] = 0;
#ifdef DEBUG
	 fprintf(stderr,"%s -> %s\n", type.c_str(), t);
#endif
	 if (out[j]->SaveFile(t) == FAILURE) {
	    return FAILURE;
	 }
      }
   } else
   if (type == "Pobject:Img1dsl") {
      Pobject *out = c_in.GETPOBJECT(*i,Img1dsl);
      const char *c = i->c_str();
      char t[64];
      snprintf(t, sizeof(t), "%s.pan", c);
      t[sizeof(t) - 1] = 0;
#ifdef DEBUG
      fprintf(stderr, "%s -> %s\n", type.c_str(), t);
#endif
      if (out->SaveFile(t) == FAILURE) {
	 return FAILURE;
      }
   } else if (type == "PArray:Img1dsl") {
      Img1dsl **out = c_in.GETPARRAY(*i, Img1dsl);
      int nb = (int)(c_in.GETPARRAYSIZE(*i, Img1dsl));
      const char *c = i->c_str();
      for (int j = 0; j < nb; j++) {
	 char t[64];
	 snprintf(t, sizeof(t), "%s.%d.pan", c, j + 1);
	 t[sizeof(t) - 1] = 0;
#ifdef DEBUG
	 fprintf(stderr,"%s -> %s\n", type.c_str(), t);
#endif
	 if (out[j]->SaveFile(t) == FAILURE) {
	    return FAILURE;
	 }
      }
   } else
   if (type == "Pobject:Img1dsf") {
      Pobject *out = c_in.GETPOBJECT(*i,Img1dsf);
      const char *c = i->c_str();
      char t[64];
      snprintf(t, sizeof(t), "%s.pan", c);
      t[sizeof(t) - 1] = 0;
#ifdef DEBUG
      fprintf(stderr, "%s -> %s\n", type.c_str(), t);
#endif
      if (out->SaveFile(t) == FAILURE) {
	 return FAILURE;
      }
   } else if (type == "PArray:Img1dsf") {
      Img1dsf **out = c_in.GETPARRAY(*i, Img1dsf);
      int nb = (int)(c_in.GETPARRAYSIZE(*i, Img1dsf));
      const char *c = i->c_str();
      for (int j = 0; j < nb; j++) {
	 char t[64];
	 snprintf(t, sizeof(t), "%s.%d.pan", c, j + 1);
	 t[sizeof(t) - 1] = 0;
#ifdef DEBUG
	 fprintf(stderr,"%s -> %s\n", type.c_str(), t);
#endif
	 if (out[j]->SaveFile(t) == FAILURE) {
	    return FAILURE;
	 }
      }
   } else
   if (type == "Pobject:Img2duc") {
      Pobject *out = c_in.GETPOBJECT(*i,Img2duc);
      const char *c = i->c_str();
      char t[64];
      snprintf(t, sizeof(t), "%s.pan", c);
      t[sizeof(t) - 1] = 0;
#ifdef DEBUG
      fprintf(stderr, "%s -> %s\n", type.c_str(), t);
#endif
      if (out->SaveFile(t) == FAILURE) {
	 return FAILURE;
      }
   } else if (type == "PArray:Img2duc") {
      Img2duc **out = c_in.GETPARRAY(*i, Img2duc);
      int nb = (int)(c_in.GETPARRAYSIZE(*i, Img2duc));
      const char *c = i->c_str();
      for (int j = 0; j < nb; j++) {
	 char t[64];
	 snprintf(t, sizeof(t), "%s.%d.pan", c, j + 1);
	 t[sizeof(t) - 1] = 0;
#ifdef DEBUG
	 fprintf(stderr,"%s -> %s\n", type.c_str(), t);
#endif
	 if (out[j]->SaveFile(t) == FAILURE) {
	    return FAILURE;
	 }
      }
   } else
   if (type == "Pobject:Img2dsl") {
      Pobject *out = c_in.GETPOBJECT(*i,Img2dsl);
      const char *c = i->c_str();
      char t[64];
      snprintf(t, sizeof(t), "%s.pan", c);
      t[sizeof(t) - 1] = 0;
#ifdef DEBUG
      fprintf(stderr, "%s -> %s\n", type.c_str(), t);
#endif
      if (out->SaveFile(t) == FAILURE) {
	 return FAILURE;
      }
   } else if (type == "PArray:Img2dsl") {
      Img2dsl **out = c_in.GETPARRAY(*i, Img2dsl);
      int nb = (int)(c_in.GETPARRAYSIZE(*i, Img2dsl));
      const char *c = i->c_str();
      for (int j = 0; j < nb; j++) {
	 char t[64];
	 snprintf(t, sizeof(t), "%s.%d.pan", c, j + 1);
	 t[sizeof(t) - 1] = 0;
#ifdef DEBUG
	 fprintf(stderr,"%s -> %s\n", type.c_str(), t);
#endif
	 if (out[j]->SaveFile(t) == FAILURE) {
	    return FAILURE;
	 }
      }
   } else
   if (type == "Pobject:Img2dsf") {
      Pobject *out = c_in.GETPOBJECT(*i,Img2dsf);
      const char *c = i->c_str();
      char t[64];
      snprintf(t, sizeof(t), "%s.pan", c);
      t[sizeof(t) - 1] = 0;
#ifdef DEBUG
      fprintf(stderr, "%s -> %s\n", type.c_str(), t);
#endif
      if (out->SaveFile(t) == FAILURE) {
	 return FAILURE;
      }
   } else if (type == "PArray:Img2dsf") {
      Img2dsf **out = c_in.GETPARRAY(*i, Img2dsf);
      int nb = (int)(c_in.GETPARRAYSIZE(*i, Img2dsf));
      const char *c = i->c_str();
      for (int j = 0; j < nb; j++) {
	 char t[64];
	 snprintf(t, sizeof(t), "%s.%d.pan", c, j + 1);
	 t[sizeof(t) - 1] = 0;
#ifdef DEBUG
	 fprintf(stderr,"%s -> %s\n", type.c_str(), t);
#endif
	 if (out[j]->SaveFile(t) == FAILURE) {
	    return FAILURE;
	 }
      }
   } else
   if (type == "Pobject:Img3duc") {
      Pobject *out = c_in.GETPOBJECT(*i,Img3duc);
      const char *c = i->c_str();
      char t[64];
      snprintf(t, sizeof(t), "%s.pan", c);
      t[sizeof(t) - 1] = 0;
#ifdef DEBUG
      fprintf(stderr, "%s -> %s\n", type.c_str(), t);
#endif
      if (out->SaveFile(t) == FAILURE) {
	 return FAILURE;
      }
   } else if (type == "PArray:Img3duc") {
      Img3duc **out = c_in.GETPARRAY(*i, Img3duc);
      int nb = (int)(c_in.GETPARRAYSIZE(*i, Img3duc));
      const char *c = i->c_str();
      for (int j = 0; j < nb; j++) {
	 char t[64];
	 snprintf(t, sizeof(t), "%s.%d.pan", c, j + 1);
	 t[sizeof(t) - 1] = 0;
#ifdef DEBUG
	 fprintf(stderr,"%s -> %s\n", type.c_str(), t);
#endif
	 if (out[j]->SaveFile(t) == FAILURE) {
	    return FAILURE;
	 }
      }
   } else
   if (type == "Pobject:Img3dsl") {
      Pobject *out = c_in.GETPOBJECT(*i,Img3dsl);
      const char *c = i->c_str();
      char t[64];
      snprintf(t, sizeof(t), "%s.pan", c);
      t[sizeof(t) - 1] = 0;
#ifdef DEBUG
      fprintf(stderr, "%s -> %s\n", type.c_str(), t);
#endif
      if (out->SaveFile(t) == FAILURE) {
	 return FAILURE;
      }
   } else if (type == "PArray:Img3dsl") {
      Img3dsl **out = c_in.GETPARRAY(*i, Img3dsl);
      int nb = (int)(c_in.GETPARRAYSIZE(*i, Img3dsl));
      const char *c = i->c_str();
      for (int j = 0; j < nb; j++) {
	 char t[64];
	 snprintf(t, sizeof(t), "%s.%d.pan", c, j + 1);
	 t[sizeof(t) - 1] = 0;
#ifdef DEBUG
	 fprintf(stderr,"%s -> %s\n", type.c_str(), t);
#endif
	 if (out[j]->SaveFile(t) == FAILURE) {
	    return FAILURE;
	 }
      }
   } else
   if (type == "Pobject:Img3dsf") {
      Pobject *out = c_in.GETPOBJECT(*i,Img3dsf);
      const char *c = i->c_str();
      char t[64];
      snprintf(t, sizeof(t), "%s.pan", c);
      t[sizeof(t) - 1] = 0;
#ifdef DEBUG
      fprintf(stderr, "%s -> %s\n", type.c_str(), t);
#endif
      if (out->SaveFile(t) == FAILURE) {
	 return FAILURE;
      }
   } else if (type == "PArray:Img3dsf") {
      Img3dsf **out = c_in.GETPARRAY(*i, Img3dsf);
      int nb = (int)(c_in.GETPARRAYSIZE(*i, Img3dsf));
      const char *c = i->c_str();
      for (int j = 0; j < nb; j++) {
	 char t[64];
	 snprintf(t, sizeof(t), "%s.%d.pan", c, j + 1);
	 t[sizeof(t) - 1] = 0;
#ifdef DEBUG
	 fprintf(stderr,"%s -> %s\n", type.c_str(), t);
#endif
	 if (out[j]->SaveFile(t) == FAILURE) {
	    return FAILURE;
	 }
      }
   } else
   if (type == "Pobject:Reg1d") {
      Pobject *out = c_in.GETPOBJECT(*i,Reg1d);
      const char *c = i->c_str();
      char t[64];
      snprintf(t, sizeof(t), "%s.pan", c);
      t[sizeof(t) - 1] = 0;
#ifdef DEBUG
      fprintf(stderr, "%s -> %s\n", type.c_str(), t);
#endif
      if (out->SaveFile(t) == FAILURE) {
	 return FAILURE;
      }
   } else if (type == "PArray:Reg1d") {
      Reg1d **out = c_in.GETPARRAY(*i, Reg1d);
      int nb = (int)(c_in.GETPARRAYSIZE(*i, Reg1d));
      const char *c = i->c_str();
      for (int j = 0; j < nb; j++) {
	 char t[64];
	 snprintf(t, sizeof(t), "%s.%d.pan", c, j + 1);
	 t[sizeof(t) - 1] = 0;
#ifdef DEBUG
	 fprintf(stderr,"%s -> %s\n", type.c_str(), t);
#endif
	 if (out[j]->SaveFile(t) == FAILURE) {
	    return FAILURE;
	 }
      }
   } else
   if (type == "Pobject:Reg2d") {
      Pobject *out = c_in.GETPOBJECT(*i,Reg2d);
      const char *c = i->c_str();
      char t[64];
      snprintf(t, sizeof(t), "%s.pan", c);
      t[sizeof(t) - 1] = 0;
#ifdef DEBUG
      fprintf(stderr, "%s -> %s\n", type.c_str(), t);
#endif
      if (out->SaveFile(t) == FAILURE) {
	 return FAILURE;
      }
   } else if (type == "PArray:Reg2d") {
      Reg2d **out = c_in.GETPARRAY(*i, Reg2d);
      int nb = (int)(c_in.GETPARRAYSIZE(*i, Reg2d));
      const char *c = i->c_str();
      for (int j = 0; j < nb; j++) {
	 char t[64];
	 snprintf(t, sizeof(t), "%s.%d.pan", c, j + 1);
	 t[sizeof(t) - 1] = 0;
#ifdef DEBUG
	 fprintf(stderr,"%s -> %s\n", type.c_str(), t);
#endif
	 if (out[j]->SaveFile(t) == FAILURE) {
	    return FAILURE;
	 }
      }
   } else
   if (type == "Pobject:Reg3d") {
      Pobject *out = c_in.GETPOBJECT(*i,Reg3d);
      const char *c = i->c_str();
      char t[64];
      snprintf(t, sizeof(t), "%s.pan", c);
      t[sizeof(t) - 1] = 0;
#ifdef DEBUG
      fprintf(stderr, "%s -> %s\n", type.c_str(), t);
#endif
      if (out->SaveFile(t) == FAILURE) {
	 return FAILURE;
      }
   } else if (type == "PArray:Reg3d") {
      Reg3d **out = c_in.GETPARRAY(*i, Reg3d);
      int nb = (int)(c_in.GETPARRAYSIZE(*i, Reg3d));
      const char *c = i->c_str();
      for (int j = 0; j < nb; j++) {
	 char t[64];
	 snprintf(t, sizeof(t), "%s.%d.pan", c, j + 1);
	 t[sizeof(t) - 1] = 0;
#ifdef DEBUG
	 fprintf(stderr,"%s -> %s\n", type.c_str(), t);
#endif
	 if (out[j]->SaveFile(t) == FAILURE) {
	    return FAILURE;
	 }
      }
   } else
   if (type == "Pobject:Graph2d") {
      Pobject *out = c_in.GETPOBJECT(*i,Graph2d);
      const char *c = i->c_str();
      char t[64];
      snprintf(t, sizeof(t), "%s.pan", c);
      t[sizeof(t) - 1] = 0;
#ifdef DEBUG
      fprintf(stderr, "%s -> %s\n", type.c_str(), t);
#endif
      if (out->SaveFile(t) == FAILURE) {
	 return FAILURE;
      }
   } else if (type == "PArray:Graph2d") {
      Graph2d **out = c_in.GETPARRAY(*i, Graph2d);
      int nb = (int)(c_in.GETPARRAYSIZE(*i, Graph2d));
      const char *c = i->c_str();
      for (int j = 0; j < nb; j++) {
	 char t[64];
	 snprintf(t, sizeof(t), "%s.%d.pan", c, j + 1);
	 t[sizeof(t) - 1] = 0;
#ifdef DEBUG
	 fprintf(stderr,"%s -> %s\n", type.c_str(), t);
#endif
	 if (out[j]->SaveFile(t) == FAILURE) {
	    return FAILURE;
	 }
      }
   } else
   if (type == "Pobject:Graph3d") {
      Pobject *out = c_in.GETPOBJECT(*i,Graph3d);
      const char *c = i->c_str();
      char t[64];
      snprintf(t, sizeof(t), "%s.pan", c);
      t[sizeof(t) - 1] = 0;
#ifdef DEBUG
      fprintf(stderr, "%s -> %s\n", type.c_str(), t);
#endif
      if (out->SaveFile(t) == FAILURE) {
	 return FAILURE;
      }
   } else if (type == "PArray:Graph3d") {
      Graph3d **out = c_in.GETPARRAY(*i, Graph3d);
      int nb = (int)(c_in.GETPARRAYSIZE(*i, Graph3d));
      const char *c = i->c_str();
      for (int j = 0; j < nb; j++) {
	 char t[64];
	 snprintf(t, sizeof(t), "%s.%d.pan", c, j + 1);
	 t[sizeof(t) - 1] = 0;
#ifdef DEBUG
	 fprintf(stderr,"%s -> %s\n", type.c_str(), t);
#endif
	 if (out[j]->SaveFile(t) == FAILURE) {
	    return FAILURE;
	 }
      }
   } else
   if (type == "Pobject:Imc2duc") {
      Pobject *out = c_in.GETPOBJECT(*i,Imc2duc);
      const char *c = i->c_str();
      char t[64];
      snprintf(t, sizeof(t), "%s.pan", c);
      t[sizeof(t) - 1] = 0;
#ifdef DEBUG
      fprintf(stderr, "%s -> %s\n", type.c_str(), t);
#endif
      if (out->SaveFile(t) == FAILURE) {
	 return FAILURE;
      }
   } else if (type == "PArray:Imc2duc") {
      Imc2duc **out = c_in.GETPARRAY(*i, Imc2duc);
      int nb = (int)(c_in.GETPARRAYSIZE(*i, Imc2duc));
      const char *c = i->c_str();
      for (int j = 0; j < nb; j++) {
	 char t[64];
	 snprintf(t, sizeof(t), "%s.%d.pan", c, j + 1);
	 t[sizeof(t) - 1] = 0;
#ifdef DEBUG
	 fprintf(stderr,"%s -> %s\n", type.c_str(), t);
#endif
	 if (out[j]->SaveFile(t) == FAILURE) {
	    return FAILURE;
	 }
      }
   } else
   if (type == "Pobject:Imc2dsl") {
      Pobject *out = c_in.GETPOBJECT(*i,Imc2dsl);
      const char *c = i->c_str();
      char t[64];
      snprintf(t, sizeof(t), "%s.pan", c);
      t[sizeof(t) - 1] = 0;
#ifdef DEBUG
      fprintf(stderr, "%s -> %s\n", type.c_str(), t);
#endif
      if (out->SaveFile(t) == FAILURE) {
	 return FAILURE;
      }
   } else if (type == "PArray:Imc2dsl") {
      Imc2dsl **out = c_in.GETPARRAY(*i, Imc2dsl);
      int nb = (int)(c_in.GETPARRAYSIZE(*i, Imc2dsl));
      const char *c = i->c_str();
      for (int j = 0; j < nb; j++) {
	 char t[64];
	 snprintf(t, sizeof(t), "%s.%d.pan", c, j + 1);
	 t[sizeof(t) - 1] = 0;
#ifdef DEBUG
	 fprintf(stderr,"%s -> %s\n", type.c_str(), t);
#endif
	 if (out[j]->SaveFile(t) == FAILURE) {
	    return FAILURE;
	 }
      }
   } else
   if (type == "Pobject:Imc2dsf") {
      Pobject *out = c_in.GETPOBJECT(*i,Imc2dsf);
      const char *c = i->c_str();
      char t[64];
      snprintf(t, sizeof(t), "%s.pan", c);
      t[sizeof(t) - 1] = 0;
#ifdef DEBUG
      fprintf(stderr, "%s -> %s\n", type.c_str(), t);
#endif
      if (out->SaveFile(t) == FAILURE) {
	 return FAILURE;
      }
   } else if (type == "PArray:Imc2dsf") {
      Imc2dsf **out = c_in.GETPARRAY(*i, Imc2dsf);
      int nb = (int)(c_in.GETPARRAYSIZE(*i, Imc2dsf));
      const char *c = i->c_str();
      for (int j = 0; j < nb; j++) {
	 char t[64];
	 snprintf(t, sizeof(t), "%s.%d.pan", c, j + 1);
	 t[sizeof(t) - 1] = 0;
#ifdef DEBUG
	 fprintf(stderr,"%s -> %s\n", type.c_str(), t);
#endif
	 if (out[j]->SaveFile(t) == FAILURE) {
	    return FAILURE;
	 }
      }
   } else
   if (type == "Pobject:Imc3duc") {
      Pobject *out = c_in.GETPOBJECT(*i,Imc3duc);
      const char *c = i->c_str();
      char t[64];
      snprintf(t, sizeof(t), "%s.pan", c);
      t[sizeof(t) - 1] = 0;
#ifdef DEBUG
      fprintf(stderr, "%s -> %s\n", type.c_str(), t);
#endif
      if (out->SaveFile(t) == FAILURE) {
	 return FAILURE;
      }
   } else if (type == "PArray:Imc3duc") {
      Imc3duc **out = c_in.GETPARRAY(*i, Imc3duc);
      int nb = (int)(c_in.GETPARRAYSIZE(*i, Imc3duc));
      const char *c = i->c_str();
      for (int j = 0; j < nb; j++) {
	 char t[64];
	 snprintf(t, sizeof(t), "%s.%d.pan", c, j + 1);
	 t[sizeof(t) - 1] = 0;
#ifdef DEBUG
	 fprintf(stderr,"%s -> %s\n", type.c_str(), t);
#endif
	 if (out[j]->SaveFile(t) == FAILURE) {
	    return FAILURE;
	 }
      }
   } else
   if (type == "Pobject:Imc3dsl") {
      Pobject *out = c_in.GETPOBJECT(*i,Imc3dsl);
      const char *c = i->c_str();
      char t[64];
      snprintf(t, sizeof(t), "%s.pan", c);
      t[sizeof(t) - 1] = 0;
#ifdef DEBUG
      fprintf(stderr, "%s -> %s\n", type.c_str(), t);
#endif
      if (out->SaveFile(t) == FAILURE) {
	 return FAILURE;
      }
   } else if (type == "PArray:Imc3dsl") {
      Imc3dsl **out = c_in.GETPARRAY(*i, Imc3dsl);
      int nb = (int)(c_in.GETPARRAYSIZE(*i, Imc3dsl));
      const char *c = i->c_str();
      for (int j = 0; j < nb; j++) {
	 char t[64];
	 snprintf(t, sizeof(t), "%s.%d.pan", c, j + 1);
	 t[sizeof(t) - 1] = 0;
#ifdef DEBUG
	 fprintf(stderr,"%s -> %s\n", type.c_str(), t);
#endif
	 if (out[j]->SaveFile(t) == FAILURE) {
	    return FAILURE;
	 }
      }
   } else
   if (type == "Pobject:Imc3dsf") {
      Pobject *out = c_in.GETPOBJECT(*i,Imc3dsf);
      const char *c = i->c_str();
      char t[64];
      snprintf(t, sizeof(t), "%s.pan", c);
      t[sizeof(t) - 1] = 0;
#ifdef DEBUG
      fprintf(stderr, "%s -> %s\n", type.c_str(), t);
#endif
      if (out->SaveFile(t) == FAILURE) {
	 return FAILURE;
      }
   } else if (type == "PArray:Imc3dsf") {
      Imc3dsf **out = c_in.GETPARRAY(*i, Imc3dsf);
      int nb = (int)(c_in.GETPARRAYSIZE(*i, Imc3dsf));
      const char *c = i->c_str();
      for (int j = 0; j < nb; j++) {
	 char t[64];
	 snprintf(t, sizeof(t), "%s.%d.pan", c, j + 1);
	 t[sizeof(t) - 1] = 0;
#ifdef DEBUG
	 fprintf(stderr,"%s -> %s\n", type.c_str(), t);
#endif
	 if (out[j]->SaveFile(t) == FAILURE) {
	    return FAILURE;
	 }
      }
   } else
   if (type == "Pobject:Imx1duc") {
      Pobject *out = c_in.GETPOBJECT(*i,Imx1duc);
      const char *c = i->c_str();
      char t[64];
      snprintf(t, sizeof(t), "%s.pan", c);
      t[sizeof(t) - 1] = 0;
#ifdef DEBUG
      fprintf(stderr, "%s -> %s\n", type.c_str(), t);
#endif
      if (out->SaveFile(t) == FAILURE) {
	 return FAILURE;
      }
   } else if (type == "PArray:Imx1duc") {
      Imx1duc **out = c_in.GETPARRAY(*i, Imx1duc);
      int nb = (int)(c_in.GETPARRAYSIZE(*i, Imx1duc));
      const char *c = i->c_str();
      for (int j = 0; j < nb; j++) {
	 char t[64];
	 snprintf(t, sizeof(t), "%s.%d.pan", c, j + 1);
	 t[sizeof(t) - 1] = 0;
#ifdef DEBUG
	 fprintf(stderr,"%s -> %s\n", type.c_str(), t);
#endif
	 if (out[j]->SaveFile(t) == FAILURE) {
	    return FAILURE;
	 }
      }
   } else
   if (type == "Pobject:Imx1dsl") {
      Pobject *out = c_in.GETPOBJECT(*i,Imx1dsl);
      const char *c = i->c_str();
      char t[64];
      snprintf(t, sizeof(t), "%s.pan", c);
      t[sizeof(t) - 1] = 0;
#ifdef DEBUG
      fprintf(stderr, "%s -> %s\n", type.c_str(), t);
#endif
      if (out->SaveFile(t) == FAILURE) {
	 return FAILURE;
      }
   } else if (type == "PArray:Imx1dsl") {
      Imx1dsl **out = c_in.GETPARRAY(*i, Imx1dsl);
      int nb = (int)(c_in.GETPARRAYSIZE(*i, Imx1dsl));
      const char *c = i->c_str();
      for (int j = 0; j < nb; j++) {
	 char t[64];
	 snprintf(t, sizeof(t), "%s.%d.pan", c, j + 1);
	 t[sizeof(t) - 1] = 0;
#ifdef DEBUG
	 fprintf(stderr,"%s -> %s\n", type.c_str(), t);
#endif
	 if (out[j]->SaveFile(t) == FAILURE) {
	    return FAILURE;
	 }
      }
   } else
   if (type == "Pobject:Imx1dsf") {
      Pobject *out = c_in.GETPOBJECT(*i,Imx1dsf);
      const char *c = i->c_str();
      char t[64];
      snprintf(t, sizeof(t), "%s.pan", c);
      t[sizeof(t) - 1] = 0;
#ifdef DEBUG
      fprintf(stderr, "%s -> %s\n", type.c_str(), t);
#endif
      if (out->SaveFile(t) == FAILURE) {
	 return FAILURE;
      }
   } else if (type == "PArray:Imx1dsf") {
      Imx1dsf **out = c_in.GETPARRAY(*i, Imx1dsf);
      int nb = (int)(c_in.GETPARRAYSIZE(*i, Imx1dsf));
      const char *c = i->c_str();
      for (int j = 0; j < nb; j++) {
	 char t[64];
	 snprintf(t, sizeof(t), "%s.%d.pan", c, j + 1);
	 t[sizeof(t) - 1] = 0;
#ifdef DEBUG
	 fprintf(stderr,"%s -> %s\n", type.c_str(), t);
#endif
	 if (out[j]->SaveFile(t) == FAILURE) {
	    return FAILURE;
	 }
      }
   } else
   if (type == "Pobject:Imx2duc") {
      Pobject *out = c_in.GETPOBJECT(*i,Imx2duc);
      const char *c = i->c_str();
      char t[64];
      snprintf(t, sizeof(t), "%s.pan", c);
      t[sizeof(t) - 1] = 0;
#ifdef DEBUG
      fprintf(stderr, "%s -> %s\n", type.c_str(), t);
#endif
      if (out->SaveFile(t) == FAILURE) {
	 return FAILURE;
      }
   } else if (type == "PArray:Imx2duc") {
      Imx2duc **out = c_in.GETPARRAY(*i, Imx2duc);
      int nb = (int)(c_in.GETPARRAYSIZE(*i, Imx2duc));
      const char *c = i->c_str();
      for (int j = 0; j < nb; j++) {
	 char t[64];
	 snprintf(t, sizeof(t), "%s.%d.pan", c, j + 1);
	 t[sizeof(t) - 1] = 0;
#ifdef DEBUG
	 fprintf(stderr,"%s -> %s\n", type.c_str(), t);
#endif
	 if (out[j]->SaveFile(t) == FAILURE) {
	    return FAILURE;
	 }
      }
   } else
   if (type == "Pobject:Imx2dsl") {
      Pobject *out = c_in.GETPOBJECT(*i,Imx2dsl);
      const char *c = i->c_str();
      char t[64];
      snprintf(t, sizeof(t), "%s.pan", c);
      t[sizeof(t) - 1] = 0;
#ifdef DEBUG
      fprintf(stderr, "%s -> %s\n", type.c_str(), t);
#endif
      if (out->SaveFile(t) == FAILURE) {
	 return FAILURE;
      }
   } else if (type == "PArray:Imx2dsl") {
      Imx2dsl **out = c_in.GETPARRAY(*i, Imx2dsl);
      int nb = (int)(c_in.GETPARRAYSIZE(*i, Imx2dsl));
      const char *c = i->c_str();
      for (int j = 0; j < nb; j++) {
	 char t[64];
	 snprintf(t, sizeof(t), "%s.%d.pan", c, j + 1);
	 t[sizeof(t) - 1] = 0;
#ifdef DEBUG
	 fprintf(stderr,"%s -> %s\n", type.c_str(), t);
#endif
	 if (out[j]->SaveFile(t) == FAILURE) {
	    return FAILURE;
	 }
      }
   } else
   if (type == "Pobject:Imx2dsf") {
      Pobject *out = c_in.GETPOBJECT(*i,Imx2dsf);
      const char *c = i->c_str();
      char t[64];
      snprintf(t, sizeof(t), "%s.pan", c);
      t[sizeof(t) - 1] = 0;
#ifdef DEBUG
      fprintf(stderr, "%s -> %s\n", type.c_str(), t);
#endif
      if (out->SaveFile(t) == FAILURE) {
	 return FAILURE;
      }
   } else if (type == "PArray:Imx2dsf") {
      Imx2dsf **out = c_in.GETPARRAY(*i, Imx2dsf);
      int nb = (int)(c_in.GETPARRAYSIZE(*i, Imx2dsf));
      const char *c = i->c_str();
      for (int j = 0; j < nb; j++) {
	 char t[64];
	 snprintf(t, sizeof(t), "%s.%d.pan", c, j + 1);
	 t[sizeof(t) - 1] = 0;
#ifdef DEBUG
	 fprintf(stderr,"%s -> %s\n", type.c_str(), t);
#endif
	 if (out[j]->SaveFile(t) == FAILURE) {
	    return FAILURE;
	 }
      }
   } else
   if (type == "Pobject:Imx3duc") {
      Pobject *out = c_in.GETPOBJECT(*i,Imx3duc);
      const char *c = i->c_str();
      char t[64];
      snprintf(t, sizeof(t), "%s.pan", c);
      t[sizeof(t) - 1] = 0;
#ifdef DEBUG
      fprintf(stderr, "%s -> %s\n", type.c_str(), t);
#endif
      if (out->SaveFile(t) == FAILURE) {
	 return FAILURE;
      }
   } else if (type == "PArray:Imx3duc") {
      Imx3duc **out = c_in.GETPARRAY(*i, Imx3duc);
      int nb = (int)(c_in.GETPARRAYSIZE(*i, Imx3duc));
      const char *c = i->c_str();
      for (int j = 0; j < nb; j++) {
	 char t[64];
	 snprintf(t, sizeof(t), "%s.%d.pan", c, j + 1);
	 t[sizeof(t) - 1] = 0;
#ifdef DEBUG
	 fprintf(stderr,"%s -> %s\n", type.c_str(), t);
#endif
	 if (out[j]->SaveFile(t) == FAILURE) {
	    return FAILURE;
	 }
      }
   } else
   if (type == "Pobject:Imx3dsl") {
      Pobject *out = c_in.GETPOBJECT(*i,Imx3dsl);
      const char *c = i->c_str();
      char t[64];
      snprintf(t, sizeof(t), "%s.pan", c);
      t[sizeof(t) - 1] = 0;
#ifdef DEBUG
      fprintf(stderr, "%s -> %s\n", type.c_str(), t);
#endif
      if (out->SaveFile(t) == FAILURE) {
	 return FAILURE;
      }
   } else if (type == "PArray:Imx3dsl") {
      Imx3dsl **out = c_in.GETPARRAY(*i, Imx3dsl);
      int nb = (int)(c_in.GETPARRAYSIZE(*i, Imx3dsl));
      const char *c = i->c_str();
      for (int j = 0; j < nb; j++) {
	 char t[64];
	 snprintf(t, sizeof(t), "%s.%d.pan", c, j + 1);
	 t[sizeof(t) - 1] = 0;
#ifdef DEBUG
	 fprintf(stderr,"%s -> %s\n", type.c_str(), t);
#endif
	 if (out[j]->SaveFile(t) == FAILURE) {
	    return FAILURE;
	 }
      }
   } else
   if (type == "Pobject:Imx3dsf") {
      Pobject *out = c_in.GETPOBJECT(*i,Imx3dsf);
      const char *c = i->c_str();
      char t[64];
      snprintf(t, sizeof(t), "%s.pan", c);
      t[sizeof(t) - 1] = 0;
#ifdef DEBUG
      fprintf(stderr, "%s -> %s\n", type.c_str(), t);
#endif
      if (out->SaveFile(t) == FAILURE) {
	 return FAILURE;
      }
   } else if (type == "PArray:Imx3dsf") {
      Imx3dsf **out = c_in.GETPARRAY(*i, Imx3dsf);
      int nb = (int)(c_in.GETPARRAYSIZE(*i, Imx3dsf));
      const char *c = i->c_str();
      for (int j = 0; j < nb; j++) {
	 char t[64];
	 snprintf(t, sizeof(t), "%s.%d.pan", c, j + 1);
	 t[sizeof(t) - 1] = 0;
#ifdef DEBUG
	 fprintf(stderr,"%s -> %s\n", type.c_str(), t);
#endif
	 if (out[j]->SaveFile(t) == FAILURE) {
	    return FAILURE;
	 }
      }
   } else
   if (type == "Pobject:Point1d") {
      Pobject *out = c_in.GETPOBJECT(*i,Point1d);
      const char *c = i->c_str();
      char t[64];
      snprintf(t, sizeof(t), "%s.pan", c);
      t[sizeof(t) - 1] = 0;
#ifdef DEBUG
      fprintf(stderr, "%s -> %s\n", type.c_str(), t);
#endif
      if (out->SaveFile(t) == FAILURE) {
	 return FAILURE;
      }
   } else if (type == "PArray:Point1d") {
      Point1d **out = c_in.GETPARRAY(*i, Point1d);
      int nb = (int)(c_in.GETPARRAYSIZE(*i, Point1d));
      const char *c = i->c_str();
      for (int j = 0; j < nb; j++) {
	 char t[64];
	 snprintf(t, sizeof(t), "%s.%d.pan", c, j + 1);
	 t[sizeof(t) - 1] = 0;
#ifdef DEBUG
	 fprintf(stderr,"%s -> %s\n", type.c_str(), t);
#endif
	 if (out[j]->SaveFile(t) == FAILURE) {
	    return FAILURE;
	 }
      }
   } else
   if (type == "Pobject:Point2d") {
      Pobject *out = c_in.GETPOBJECT(*i,Point2d);
      const char *c = i->c_str();
      char t[64];
      snprintf(t, sizeof(t), "%s.pan", c);
      t[sizeof(t) - 1] = 0;
#ifdef DEBUG
      fprintf(stderr, "%s -> %s\n", type.c_str(), t);
#endif
      if (out->SaveFile(t) == FAILURE) {
	 return FAILURE;
      }
   } else if (type == "PArray:Point2d") {
      Point2d **out = c_in.GETPARRAY(*i, Point2d);
      int nb = (int)(c_in.GETPARRAYSIZE(*i, Point2d));
      const char *c = i->c_str();
      for (int j = 0; j < nb; j++) {
	 char t[64];
	 snprintf(t, sizeof(t), "%s.%d.pan", c, j + 1);
	 t[sizeof(t) - 1] = 0;
#ifdef DEBUG
	 fprintf(stderr,"%s -> %s\n", type.c_str(), t);
#endif
	 if (out[j]->SaveFile(t) == FAILURE) {
	    return FAILURE;
	 }
      }
   } else
   if (type == "Pobject:Point3d") {
      Pobject *out = c_in.GETPOBJECT(*i,Point3d);
      const char *c = i->c_str();
      char t[64];
      snprintf(t, sizeof(t), "%s.pan", c);
      t[sizeof(t) - 1] = 0;
#ifdef DEBUG
      fprintf(stderr, "%s -> %s\n", type.c_str(), t);
#endif
      if (out->SaveFile(t) == FAILURE) {
	 return FAILURE;
      }
   } else if (type == "PArray:Point3d") {
      Point3d **out = c_in.GETPARRAY(*i, Point3d);
      int nb = (int)(c_in.GETPARRAYSIZE(*i, Point3d));
      const char *c = i->c_str();
      for (int j = 0; j < nb; j++) {
	 char t[64];
	 snprintf(t, sizeof(t), "%s.%d.pan", c, j + 1);
	 t[sizeof(t) - 1] = 0;
#ifdef DEBUG
	 fprintf(stderr,"%s -> %s\n", type.c_str(), t);
#endif
	 if (out[j]->SaveFile(t) == FAILURE) {
	    return FAILURE;
	 }
      }
   } else
   if (type == "Pobject:Dimension1d") {
      Pobject *out = c_in.GETPOBJECT(*i,Dimension1d);
      const char *c = i->c_str();
      char t[64];
      snprintf(t, sizeof(t), "%s.pan", c);
      t[sizeof(t) - 1] = 0;
#ifdef DEBUG
      fprintf(stderr, "%s -> %s\n", type.c_str(), t);
#endif
      if (out->SaveFile(t) == FAILURE) {
	 return FAILURE;
      }
   } else if (type == "PArray:Dimension1d") {
      Dimension1d **out = c_in.GETPARRAY(*i, Dimension1d);
      int nb = (int)(c_in.GETPARRAYSIZE(*i, Dimension1d));
      const char *c = i->c_str();
      for (int j = 0; j < nb; j++) {
	 char t[64];
	 snprintf(t, sizeof(t), "%s.%d.pan", c, j + 1);
	 t[sizeof(t) - 1] = 0;
#ifdef DEBUG
	 fprintf(stderr,"%s -> %s\n", type.c_str(), t);
#endif
	 if (out[j]->SaveFile(t) == FAILURE) {
	    return FAILURE;
	 }
      }
   } else
   if (type == "Pobject:Dimension2d") {
      Pobject *out = c_in.GETPOBJECT(*i,Dimension2d);
      const char *c = i->c_str();
      char t[64];
      snprintf(t, sizeof(t), "%s.pan", c);
      t[sizeof(t) - 1] = 0;
#ifdef DEBUG
      fprintf(stderr, "%s -> %s\n", type.c_str(), t);
#endif
      if (out->SaveFile(t) == FAILURE) {
	 return FAILURE;
      }
   } else if (type == "PArray:Dimension2d") {
      Dimension2d **out = c_in.GETPARRAY(*i, Dimension2d);
      int nb = (int)(c_in.GETPARRAYSIZE(*i, Dimension2d));
      const char *c = i->c_str();
      for (int j = 0; j < nb; j++) {
	 char t[64];
	 snprintf(t, sizeof(t), "%s.%d.pan", c, j + 1);
	 t[sizeof(t) - 1] = 0;
#ifdef DEBUG
	 fprintf(stderr,"%s -> %s\n", type.c_str(), t);
#endif
	 if (out[j]->SaveFile(t) == FAILURE) {
	    return FAILURE;
	 }
      }
   } else
   if (type == "Pobject:Dimension3d") {
      Pobject *out = c_in.GETPOBJECT(*i,Dimension3d);
      const char *c = i->c_str();
      char t[64];
      snprintf(t, sizeof(t), "%s.pan", c);
      t[sizeof(t) - 1] = 0;
#ifdef DEBUG
      fprintf(stderr, "%s -> %s\n", type.c_str(), t);
#endif
      if (out->SaveFile(t) == FAILURE) {
	 return FAILURE;
      }
   } else if (type == "PArray:Dimension3d") {
      Dimension3d **out = c_in.GETPARRAY(*i, Dimension3d);
      int nb = (int)(c_in.GETPARRAYSIZE(*i, Dimension3d));
      const char *c = i->c_str();
      for (int j = 0; j < nb; j++) {
	 char t[64];
	 snprintf(t, sizeof(t), "%s.%d.pan", c, j + 1);
	 t[sizeof(t) - 1] = 0;
#ifdef DEBUG
	 fprintf(stderr,"%s -> %s\n", type.c_str(), t);
#endif
	 if (out[j]->SaveFile(t) == FAILURE) {
	    return FAILURE;
	 }
      }
   } else
   continue;
  }
  return SUCCESS;
}

#ifdef MAIN

#define USAGE  "usage: %s [col_in|-]"
#define PARC   0 // Number of parameters
#define FINC   1 // Number of input images
#define FOUTC  0 // Number of output images
#define MASK   0 // Level of masking

int main( int argc, char *argv[] ) {
   Errc result;                // The result code of the execution.
   Pobject *mask;              // The mask.
   Pobject *objin[FINC + 1];   // The input objects.
   Pobject *objs[FINC + 1];    // The source objects masked by the mask.
   Pobject *objout[FOUTC + 1]; // The output object.
   Pobject *objd[FOUTC + 1];   // The result object of the execution.
   char* parv[PARC + 1];       // The input parameters.
   
   ReadArgs(argc, argv, PARC, FINC, FOUTC, &mask, objin, objs, objout, objd, parv, USAGE, MASK);
   
   switch (objs[0]->Type()) {
   case Po_Collection: {
      Collection *c = (Collection*)objs[0];
      result = PColGetImages(*c);
   } break;
   default:
      fprintf(stderr,"Error pcolgetimages: Pandore type not supported by this operator\n");
      result = FAILURE;
   }
   
   if (result) {
      WriteArgs(argc, argv, PARC, FINC, FOUTC, &mask, objin, objs, objout, objd, MASK);
   }
   Exit(result);
   return 0;
}

#endif
