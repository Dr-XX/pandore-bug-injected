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
 * @file pvisu.h
 * @brief Allows the usage of the Pandore viewer inside an application.
 * usage : Img2duc x; Visu(x);
 *         Graph2d g; Visu(g);
 *         Imc3dsf i; Visu(i);
 */

#undef _DRAWINGMODE
#ifdef MAIN
#undef MAIN
#include "src/viewers/Qt/pvisu.cpp"
#define MAIN
#else
#undef MAIN
#include "src/viewers/Qt/pvisu.cpp"
#endif

template <typename T>
/**
 * Displays the specified Pandore object in a windows.
 * @param object a regular Pandore object. 
 */
void Visu( T &object ) {
   QWidget *visual = NULL;
   static int _index = 1;
   
   // Use the name imagex where x is the instance index.
#define NAMELENGTH 30
   nomentree  = (char*)malloc(NAMELENGTH * sizeof(char));
   snprintf(nomentree, NAMELENGTH, "Pandore image #%02d", _index++);
   nomentree[NAMELENGTH - 1] = '\0';
   _DRAWINGMODE = false;
   
   Long pid;
   if ((pid = fork()) > 0) {
      return;
   } else if (pid == -1) {
      Exit(FAILURE);
   }
   
   int argcQt = 0;
   QApplication app(argcQt, 0);
   
   visual = Visu(object, 0);
   if (visual) {
      app.exec();
      delete visual;
   }
   Exit(pid);
}
