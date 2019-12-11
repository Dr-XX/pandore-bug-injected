/*
 #  
 #  File        : CImg.h
 #  
 #  Description : The C++ Template Image Processing Library
 #                ( http://cimg.sourceforge.net )
 #
 #  Copyright   : David Tschumperle
 #                ( http://www.greyc.ensicaen.fr/~dtschump/ )
 #   
 #  This software is governed by the CeCILL-C license under French law and
 #  abiding by the rules of distribution of free software.  You can  use, 
 #  modify and or redistribute the software under the terms of the CeCILL-C
 #  license as circulated by CEA, CNRS and INRIA at the following URL
 #  "http://www.cecill.info". 
 #  
 #  As a counterpart to the access to the source code and  rights to copy,
 #  modify and redistribute granted by the license, users are provided only
 #  with a limited warranty  and the software's author,  the holder of the
 #  economic rights,  and the successive licensors  have only  limited
 #  liability. 
 #  
 #  In this respect, the user's attention is drawn to the risks associated
 #  with loading,  using,  modifying and/or developing or reproducing the
 #  software by the user in light of its specific status of free software,
 #  that may mean  that it is complicated to manipulate,  and  that  also
 #  therefore means  that it is reserved for developers  and  experienced
 #  professionals having in-depth computer knowledge. Users are therefore
 #  encouraged to load and test the software's suitability as regards their
 #  requirements in conditions enabling the security of their systems and/or 
 #  data to be ensured and,  more generally, to use and operate it in the 
 #  same conditions as regards security. 
 #  
 #  The fact that you are presently reading this means that you have had
 #  knowledge of the CeCILL-C license and that you accept its terms.
 #  
 */

#ifndef cimg_version
#define cimg_version 1.16

#if (defined _MSC_VER) && (!defined snprintf)
#define snprintf _snprintf
#endif

// Avoid strange warning messages on Visual C++ express 2005.
#if ( defined(_MSC_VER) && _MSC_VER>=1400 )
#define _CRT_SECURE_NO_DEPRECATE 1
#define _CRT_NONSTDC_NO_DEPRECATE 1
#endif

// Standard C++ includes
#include <cstdio>
#include <cstdlib>
#include <cstdarg>
#include <cstring>
#include <cmath>
#include <ctime>

// Overcome VisualC++ 6.0 and DMC compilers namespace 'std::' bug
#if ( defined(_MSC_VER) && _MSC_VER<=1200 ) || defined(__DMC__)
#define std
#endif

/*
 #
 # Set CImg configuration flags.
 #   
 # If compilation flags are not adapted to your system,
 # you may override their values, before including
 # the header file "CImg.h" (use the #define directive).
 # 
 */

// Try to detect the current system and set value of 'cimg_OS'.
#ifndef cimg_OS
#if defined(sun)         || defined(__sun)      || defined(linux)       || defined(__linux) \
 || defined(__linux__)   || defined(__CYGWIN__) || defined(BSD)         || defined(__FreeBSD__) \
 || defined(__OPENBSD__) || defined(__MACOSX__) || defined(__APPLE__)   || defined(sgi) \
 || defined(__sgi)
// Unix-like (Linux, Solaris, BSD, Irix,...)
#define cimg_OS            1
#ifndef cimg_display_type
#define cimg_display_type  1
#endif
#ifndef cimg_color_terminal
#define cimg_color_terminal
#endif
#elif defined(_WIN32) || defined(__WIN32__)
// Windows
#define cimg_OS            2
#ifndef cimg_display_type
#define cimg_display_type  2
#endif
#else
// Unknown configuration : will compile with minimal dependencies.
#define cimg_OS            0
#ifndef cimg_display_type
#define cimg_display_type  0
#endif
#endif
#endif

// Architecture-dependent includes.
#if cimg_OS==1
#include <sys/time.h>
#include <unistd.h>
#elif cimg_OS==2
#include <windows.h>
// Discard unuseful macros in windows.h
// to allow compilation with VC++ 6.0.
#ifdef min
#undef min
#undef max
#undef abs
#endif
#endif
#if cimg_display_type==1
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/keysym.h>
#include <pthread.h>
#ifdef cimg_use_xshm
#include <sys/ipc.h>
#include <sys/shm.h>
#include <X11/extensions/XShm.h>
#endif
#ifdef cimg_use_xrandr
#include <X11/extensions/Xrandr.h>
#endif
#endif

// Configuration for native PNG and JPEG support
// Define 'cimg_use_png', 'cimg_use_jpeg' or 'cimg_use_tiff' to enable native PNG, JPEG or TIFF files support.
// This requires you link your code with the zlib/png, jpeg or tiff libraries.
// Without these libraries, PNG,JPEG and TIFF support will be done by the Image Magick's 'convert' tool, if installed
// (this is the case on most unix plateforms).
#ifdef cimg_use_png
extern "C" {
#include "png.h"
}
#endif
#ifdef cimg_use_jpeg
extern "C" {
#include "jpeglib.h"
}
#endif
#ifdef cimg_use_tiff
extern "C" {
#include "tiffio.h"
}
#endif
#ifdef cimg_use_magick
#include <fstream>
#include <iostream>
#include <algorithm>
#include <Magick++.h>
#endif

/*
 #
 #
 # Define some useful macros. Macros of the CImg Library are prefixed by 'cimg_'
 # Documented macros below may be safely used in your own code
 # (particularly useful for option parsing, image loops and neighborhoods).
 #
 #
 */

// Macros used to describe the program usage, and retrieve command line arguments
// (See corresponding module 'Retrieving command line arguments' in the generated documentation).
#define cimg_usage(usage) cimg_library::cimg::option((const char*)0,argc,argv,(const char*)0,usage)
#define cimg_option(name,defaut,usage) cimg_library::cimg::option(name,argc,argv,defaut,usage)
  
// Macros used for neighborhood definitions and manipulations.
// (see module 'Using Image Loops' in the generated documentation).
#define CImg_2x2x1(I,T) T I##cc,I##nc=0,I##cn,I##nn=0
#define CImg_3x1x1(I,T) T I##pp,I##cp,I##np=0
#define CImg_3x3x1(I,T) T I##pp,I##cp,I##np=0,I##pc,I##cc,I##nc=0,I##pn,I##cn,I##nn=0
#define CImg_4x1x1(I,T) T I##pp,I##cp,I##np=0,I##ap=0
#define CImg_4x4x1(I,T) T I##pp,I##cp,I##np=0,I##ap=0, \
                          I##pc,I##cc,I##nc=0,I##ac=0, \
                          I##pn,I##cn,I##nn=0,I##an=0, \
                          I##pa,I##ca,I##na=0,I##aa=0
#define CImg_5x1x1(I,T) T I##bb,I##pb,I##cb,I##nb=0,I##ab=0
#define CImg_5x5x1(I,T) T I##bb,I##pb,I##cb,I##nb=0,I##ab=0, \
                          I##bp,I##pp,I##cp,I##np=0,I##ap=0, \
                          I##bc,I##pc,I##cc,I##nc=0,I##ac=0, \
                          I##bn,I##pn,I##cn,I##nn=0,I##an=0, \
                          I##ba,I##pa,I##ca,I##na=0,I##aa=0
#define CImg_2x2x2(I,T) T I##ccc,I##ncc=0,I##cnc,I##nnc=0, \
                          I##ccn,I##ncn=0,I##cnn,I##nnn=0
#define CImg_3x3x3(I,T) T I##ppp,I##cpp,I##npp=0,I##pcp,I##ccp,I##ncp=0,I##pnp,I##cnp,I##nnp=0, \
                          I##ppc,I##cpc,I##npc=0,I##pcc,I##ccc,I##ncc=0,I##pnc,I##cnc,I##nnc=0, \
                          I##ppn,I##cpn,I##npn=0,I##pcn,I##ccn,I##ncn=0,I##pnn,I##cnn,I##nnn=0

#define CImg_2x2x1_ref(I,T,tab) T &I##cc=(tab)[0],&I##nc=(tab)[1],&I##cn=(tab)[2],&I##nn=(tab)[3]
#define CImg_3x3x1_ref(I,T,tab) T &I##pp=(tab)[0],&I##cp=(tab)[1],&I##np=(tab)[2], \
                                  &I##pc=(tab)[3],&I##cc=(tab)[4],&I##nc=(tab)[5], \
                                  &I##pn=(tab)[6],&I##cn=(tab)[7],&I##nn=(tab)[8]
#define CImg_4x4x1_ref(I,T,tab) T &I##pp=(tab)[0],&I##cp=(tab)[1],&I##np=(tab)[2],&I##ap=(tab)[3], \
                                  &I##pc=(tab)[4],&I##cc=(tab)[5],&I##nc=(tab)[6],&I##ap=(tab)[7], \
                                  &I##pn=(tab)[8],&I##cn=(tab)[9],&I##nn=(tab)[10],&I##aa=(tab)[11], \
                                  &I##pa=(tab)[12],&I##ca=(tab)[13],&I##na=(tab)[14],&I##aa=(tab)[15]
#define CImg_5x5x1_ref(I,T,tab) T &I##bb=(tab)[0],&I##pb=(tab)[1],&I##cb=(tab)[2],&I##nb=(tab)[3],&I##ab=(tab)[4], \
                                  &I##bp=(tab)[5],&I##pp=(tab)[6],&I##cp=(tab)[7],&I##np=(tab)[8],&I##ap=(tab)[9], \
                                  &I##bc=(tab)[10],&I##pc=(tab)[11],&I##cc=(tab)[12],&I##nc=(tab)[13],&I##ac=(tab)[14], \
                                  &I##bn=(tab)[15],&I##pn=(tab)[16],&I##cn=(tab)[17],&I##nn=(tab)[18],&I##an=(tab)[19], \
                                  &I##ba=(tab)[20],&I##pa=(tab)[21],&I##ca=(tab)[22],&I##na=(tab)[23],&I##aa=(tab)[24]
#define CImg_2x2x2_ref(I,T,tab) T &I##ccc=(tab)[0],&I##ncc=(tab)[1],&I##cnc=(tab)[2],&I##nnc=(tab)[3], \
                                  &I##ccn=(tab)[4],&I##ncn=(tab)[5],&I##cnn=(tab)[6],&I##nnn=(tab)[7]
#define CImg_3x3x3_ref(I,T,tab) T &I##ppp=(tab)[0],&I##cpp=(tab)[1],&I##npp=(tab)[2], \
                                  &I##pcp=(tab)[3],&I##ccp=(tab)[4],&I##ncp=(tab)[5], \
                                  &I##pnp=(tab)[6],&I##cnp=(tab)[7],&I##nnp=(tab)[8], \
                                  &I##ppc=(tab)[9],&I##cpc=(tab)[10],&I##npc=(tab)[11], \
                                  &I##pcc=(tab)[12],&I##ccc=(tab)[13],&I##ncc=(tab)[14], \
                                  &I##pnc=(tab)[15],&I##cnc=(tab)[16],&I##nnc=(tab)[17], \
                                  &I##ppn=(tab)[18],&I##cpn=(tab)[19],&I##npn=(tab)[20], \
                                  &I##pcn=(tab)[21],&I##ccn=(tab)[22],&I##ncn=(tab)[23], \
                                  &I##pnn=(tab)[24],&I##cnn=(tab)[25],&I##nnn=(tab)[26]

#define cimg_copy2x2x1(J,I) I##cc=J##cc, I##nc=J##nc, I##cn=J##cn, I##nn=J##nn
#define cimg_copy3x3x1(J,I) I##pp=J##pp, I##cp=J##cp, I##np=J##np, \
                            I##pc=J##pc, I##cc=J##cc, I##nc=J##nc, \
                            I##pn=J##pn, I##cn=J##cn, I##nn=J##nn
#define cimg_copy5x5x1(J,I) I##bb=J##bb, I##pb=J##pb, I##cb=J##cb, I##nb=J##nb, I##ab=J##ab, \
                            I##bp=J##bp, I##pp=J##pp, I##cp=J##cp, I##np=J##np, I##ap=J##ap, \
                            I##bc=J##bc, I##pc=J##pc, I##cc=J##cc, I##nc=J##nc, I##ac=J##ac, \
                            I##bn=J##bn, I##pn=J##pn, I##cn=J##cn, I##nn=J##nn, I##an=J##an, \
                            I##ba=J##ba, I##pa=J##pa, I##ca=J##ca, I##na=J##na, I##aa=J##aa

#define cimg_squaresum2x2x1(I) ( I##cc*I##cc + I##nc*I##nc + I##cn*I##cn + I##nn*I##nn )
#define cimg_squaresum3x3x1(I) ( I##pp*I##pp + I##cp*I##cp + I##np*I##np + \
                                 I##pc*I##pc + I##cc*I##cc + I##nc*I##nc + \
                                 I##pn*I##pn + I##cn*I##cn + I##nn*I##nn )
#define cimg_squaresum4x4x1(I) ( I##pp*I##pp + I##cp*I##cp + I##np*I##np + I##ap*I##ap + \
                                 I##pc*I##pc + I##cc*I##cc + I##nc*I##nc + I##ac*I##ac + \
                                 I##pn*I##pn + I##cn*I##cn + I##nn*I##nn + I##an*I##an + \
                                 I##pa*I##pa + I##ca*I##ca + I##na*I##na + I##aa*I##aa )
#define cimg_squaresum5x5x1(I) ( I##bb*I##bb + I##pb*I##pb + I##cb*I##cb + I##nb*I##nb + I##ab*I##ab + \
                                 I##bp*I##bp + I##pp*I##pp + I##cp*I##cp + I##np*I##np + I##ap*I##ap + \
                                 I##bc*I##bc + I##pc*I##pc + I##cc*I##cc + I##nc*I##nc + I##ac*I##ac + \
                                 I##bn*I##bn + I##pn*I##pn + I##cn*I##cn + I##nn*I##nn + I##an*I##an + \
                                 I##ba*I##ba + I##pa*I##pa + I##ca*I##ca + I##na*I##na + I##aa*I##aa )
#define cimg_squaresum2x2x2(I) ( I##ccc*I##ccc + I##ncc*I##ncc + I##cnc*I##cnc + I##nnc*I##nnc + \
                                 I##ccn*I##ccn + I##ncn*I##ncn + I##cnn*I##cnn + I##nnn*I##nnn )
#define cimg_squaresum3x3x3(I) ( I##ppp*I##ppp + I##cpp*I##cpp + I##npp*I##npp + \
                                 I##pcp*I##pcp + I##ccp*I##ccp + I##ncp*I##ncp + \
                                 I##pnp*I##pnp + I##cnp*I##cnp + I##nnp*I##nnp + \
                                 I##ppc*I##ppc + I##cpc*I##cpc + I##npc*I##npc + \
                                 I##pcc*I##pcc + I##ccc*I##ccc + I##ncc*I##ncc + \
                                 I##pnc*I##pnc + I##cnc*I##cnc + I##nnc*I##nnc + \
                                 I##ppn*I##ppn + I##cpn*I##cpn + I##npn*I##npn + \
                                 I##pcn*I##pcn + I##ccn*I##ccn + I##ncn*I##ncn + \
                                 I##pnn*I##pnn + I##cnn*I##cnn + I##nnn*I##nnn )

#define cimg_corr2x2x1(I,m) ( I##cc*(m)(0,0)+I##nc*(m)(1,0)+I##cn*(m)(0,1)+I##nn*(m)(1,1) )
#define cimg_corr3x3x1(I,m) ( I##pp*(m)(0,0)+I##cp*(m)(1,0)+I##np*(m)(2,0) + \
                              I##pc*(m)(0,1)+I##cc*(m)(1,1)+I##nc*(m)(2,1) + \
                              I##pn*(m)(0,2)+I##cn*(m)(1,2)+I##nn*(m)(2,2) )
#define cimg_corr4x4x1(I,m) ( I##pp*(m)(0,0)+I##cp*(m)(1,0)+I##np*(m)(2,0)+I##ap*(m)(3,0) + \
                              I##pc*(m)(0,1)+I##cc*(m)(1,1)+I##nc*(m)(2,1)+I##ac*(m)(3,1) + \
                              I##pn*(m)(0,2)+I##cn*(m)(1,2)+I##nn*(m)(2,2)+I##an*(m)(3,2) + \
                              I##pa*(m)(0,3)+I##ca*(m)(1,3)+I##na*(m)(2,3)+I##aa*(m)(3,3) )
#define cimg_corr5x5x1(I,m) ( I##bb*(m)(0,0)+I##pb*(m)(1,0)+I##cb*(m)(2,0)+I##nb*(m)(3,0)+I##ab*(m)(4,0) + \
                              I##bp*(m)(0,1)+I##pp*(m)(1,1)+I##cp*(m)(2,1)+I##np*(m)(3,1)+I##ap*(m)(4,1) + \
                              I##bc*(m)(0,2)+I##pc*(m)(1,2)+I##cc*(m)(2,2)+I##nc*(m)(3,2)+I##ac*(m)(4,2) + \
                              I##bn*(m)(0,3)+I##pn*(m)(1,3)+I##cn*(m)(2,3)+I##nn*(m)(3,3)+I##an*(m)(4,3) + \
                              I##ba*(m)(0,4)+I##pa*(m)(1,4)+I##ca*(m)(2,4)+I##na*(m)(3,4)+I##aa*(m)(4,4) )
#define cimg_corr2x2x2(I,m) ( I##ccc*(m)(0,0,0)+I##ncc*(m)(1,0,0)+I##cnc*(m)(0,1,0)+I##nnc*(m)(1,1,0) + \
                              I##ccn*(m)(0,0,1)+I##ncn*(m)(1,0,1)+I##cnn*(m)(0,1,1)+I##nnn*(m)(1,1,1) )
#define cimg_corr3x3x3(I,m) ( I##ppp*(m)(0,0,0)+I##cpp*(m)(1,0,0)+I##npp*(m)(2,0,0) + \
                              I##pcp*(m)(0,1,0)+I##ccp*(m)(1,1,0)+I##ncp*(m)(2,1,0) + \
                              I##pnp*(m)(0,2,0)+I##cnp*(m)(1,2,0)+I##nnp*(m)(2,2,0) + \
                              I##ppc*(m)(0,0,1)+I##cpc*(m)(1,0,1)+I##npc*(m)(2,0,1) + \
                              I##pcc*(m)(0,1,1)+I##ccc*(m)(1,1,1)+I##ncc*(m)(2,1,1) + \
                              I##pnc*(m)(0,2,1)+I##cnc*(m)(1,2,1)+I##nnc*(m)(2,2,1) + \
                              I##ppn*(m)(0,0,2)+I##cpn*(m)(1,0,2)+I##npn*(m)(2,0,2) + \
                              I##pcn*(m)(0,1,2)+I##ccn*(m)(1,1,2)+I##ncn*(m)(2,1,2) + \
                              I##pnn*(m)(0,2,2)+I##cnn*(m)(1,2,2)+I##nnn*(m)(2,2,2) )

#define cimg_conv2x2x1(I,m) ( I##cc*(m)(1,1)+I##nc*(m)(0,1)+I##cn*(m)(1,0)+I##nn*(m)(0,0) )
#define cimg_conv3x3x1(I,m) ( I##pp*(m)(2,2)+I##cp*(m)(1,2)+I##np*(m)(0,2) + \
                              I##pc*(m)(2,1)+I##cc*(m)(1,1)+I##nc*(m)(0,1) + \
                              I##pn*(m)(2,0)+I##cn*(m)(1,0)+I##nn*(m)(0,0) )
#define cimg_conv4x4x1(I,m) ( I##pp*(m)(3,3)+I##cp*(m)(2,3)+I##np*(m)(1,3)+I##ap*(m)(0,3) + \
                              I##pc*(m)(3,2)+I##cc*(m)(2,2)+I##nc*(m)(1,2)+I##ac*(m)(0,2) + \
                              I##pn*(m)(3,1)+I##cn*(m)(2,1)+I##nn*(m)(1,1)+I##an*(m)(0,1) + \
                              I##pa*(m)(3,0)+I##ca*(m)(2,0)+I##na*(m)(1,0)+I##aa*(m)(0,0) )
#define cimg_conv5x5x1(I,m) ( I##bb*(m)(4,4)+I##pb*(m)(3,4)+I##cb*(m)(2,4)+I##nb*(m)(1,4)+I##ab*(m)(0,4) + \
                              I##bp*(m)(4,3)+I##pp*(m)(3,3)+I##cp*(m)(2,3)+I##np*(m)(1,3)+I##ap*(m)(0,3) + \
                              I##bc*(m)(4,2)+I##pc*(m)(3,2)+I##cc*(m)(2,2)+I##nc*(m)(1,2)+I##ac*(m)(0,2) + \
                              I##bn*(m)(4,1)+I##pn*(m)(3,1)+I##cn*(m)(2,1)+I##nn*(m)(1,1)+I##an*(m)(0,1) + \
                              I##ba*(m)(4,0)+I##pa*(m)(3,0)+I##ca*(m)(2,0)+I##na*(m)(1,0)+I##aa*(m)(0,0) )
#define cimg_conv2x2x2(I,m) ( I##ccc*(m)(1,1,1)+I##ncc*(m)(0,1,1)+I##cnc*(m)(1,0,1)+I##nnc*(m)(0,0,1) + \
                              I##ccn*(m)(1,1,0)+I##ncn*(m)(0,1,0)+I##cnn*(m)(1,0,0)+I##nnn*(m)(0,0,0) )
#define cimg_conv3x3x3(I,m) ( I##ppp*(m)(2,2,2)+I##cpp*(m)(1,2,2)+I##npp*(m)(0,2,2) + \
                              I##pcp*(m)(2,1,2)+I##ccp*(m)(1,1,2)+I##ncp*(m)(0,1,2) + \
                              I##pnp*(m)(2,0,2)+I##cnp*(m)(1,0,2)+I##nnp*(m)(0,0,2) + \
                              I##ppc*(m)(2,2,1)+I##cpc*(m)(1,2,1)+I##npc*(m)(0,2,1) + \
                              I##pcc*(m)(2,1,1)+I##ccc*(m)(1,1,1)+I##ncc*(m)(0,1,1) + \
                              I##pnc*(m)(2,0,1)+I##cnc*(m)(1,0,1)+I##nnc*(m)(0,0,1) + \
                              I##ppn*(m)(2,2,0)+I##cpn*(m)(1,2,0)+I##npn*(m)(0,2,0) + \
                              I##pcn*(m)(2,1,0)+I##ccn*(m)(1,1,0)+I##ncn*(m)(0,1,0) + \
                              I##pnn*(m)(2,0,0)+I##cnn*(m)(1,0,0)+I##nnn*(m)(0,0,0) )

#define cimg_get2x2x1(img,x,y,z,v,I) \
   I##cc=(img)(x,    y,z,v), I##nc=(img)(_n##x,    y,z,v), \
   I##cn=(img)(x,_n##y,z,v), I##nn=(img)(_n##x,_n##y,z,v)
#define cimg_get3x3x1(img,x,y,z,v,I) \
  I##pp=(img)(_p##x,_p##y,z,v), I##cp=(img)(x,_p##y,z,v), I##np=(img)(_n##x,_p##y,z,v), \
  I##pc=(img)(_p##x,    y,z,v), I##cc=(img)(x,    y,z,v), I##nc=(img)(_n##x,    y,z,v), \
  I##pn=(img)(_p##x,_n##y,z,v), I##cn=(img)(x,_n##y,z,v), I##nn=(img)(_n##x,_n##y,z,v)
#define cimg_get4x4x1(img,x,y,z,v,I) \
  I##pp=(img)(_p##x,_p##y,z,v), I##cp=(img)(x,_p##y,z,v), I##np=(img)(_n##x,_p##y,z,v), I##ap=(img)(_a##x,_p##y,z,v), \
  I##pc=(img)(_p##x,    y,z,v), I##cc=(img)(x,    y,z,v), I##nc=(img)(_n##x,    y,z,v), I##ac=(img)(_a##x,    y,z,v), \
  I##pn=(img)(_p##x,_n##y,z,v), I##cn=(img)(x,_n##y,z,v), I##nn=(img)(_n##x,_n##y,z,v), I##an=(img)(_a##x,_n##y,z,v), \
  I##pa=(img)(_p##x,_a##y,z,v), I##ca=(img)(x,_a##y,z,v), I##na=(img)(_n##x,_a##y,z,v), I##aa=(img)(_a##x,_a##y,z,v)
#define cimg_get5x5x1(img,x,y,z,v,I) \
  I##bb=(img)(_b##x,_b##y,z,v), I##pb=(img)(_p##x,_b##y,z,v), I##cb=(img)(x,_b##y,z,v), I##nb=(img)(_n##x,_b##y,z,v), I##ab=(img)(_a##x,_b##y,z,v), \
  I##bp=(img)(_b##x,_p##y,z,v), I##pp=(img)(_p##x,_p##y,z,v), I##cp=(img)(x,_p##y,z,v), I##np=(img)(_n##x,_p##y,z,v), I##ap=(img)(_a##x,_p##y,z,v), \
  I##bc=(img)(_b##x,    y,z,v), I##pc=(img)(_p##x,    y,z,v), I##cc=(img)(x,    y,z,v), I##nc=(img)(_n##x,    y,z,v), I##ac=(img)(_a##x,    y,z,v), \
  I##bn=(img)(_b##x,_n##y,z,v), I##pn=(img)(_p##x,_n##y,z,v), I##cn=(img)(x,_n##y,z,v), I##nn=(img)(_n##x,_n##y,z,v), I##an=(img)(_a##x,_n##y,z,v), \
  I##ba=(img)(_b##x,_a##y,z,v), I##pa=(img)(_p##x,_a##y,z,v), I##ca=(img)(x,_a##y,z,v), I##na=(img)(_n##x,_a##y,z,v), I##aa=(img)(_a##x,_a##y,z,v)
#define cimg_get2x2x2(img,x,y,z,v,I) \
  I##ccc=(img)(x,y,    z,v), I##ncc=(img)(_n##x,y,    z,v), I##cnc=(img)(x,_n##y,    z,v), I##nnc=(img)(_n##x,_n##y,    z,v), \
  I##ccc=(img)(x,y,_n##z,v), I##ncc=(img)(_n##x,y,_n##z,v), I##cnc=(img)(x,_n##y,_n##z,v), I##nnc=(img)(_n##x,_n##y,_n##z,v)
#define cimg_get3x3x3(img,x,y,z,v,I) \
  I##ppp=(img)(_p##x,_p##y,_p##z,v), I##cpp=(img)(x,_p##y,_p##z,v), I##npp=(img)(_n##x,_p##y,_p##z,v), \
  I##pcp=(img)(_p##x,    y,_p##z,v), I##ccp=(img)(x,    y,_p##z,v), I##ncp=(img)(_n##x,    y,_p##z,v), \
  I##pnp=(img)(_p##x,_n##y,_p##z,v), I##cnp=(img)(x,_n##y,_p##z,v), I##nnp=(img)(_n##x,_n##y,_p##z,v), \
  I##ppc=(img)(_p##x,_p##y,    z,v), I##cpc=(img)(x,_p##y,    z,v), I##npc=(img)(_n##x,_p##y,    z,v), \
  I##pcc=(img)(_p##x,    y,    z,v), I##ccc=(img)(x,    y,    z,v), I##ncc=(img)(_n##x,    y,    z,v), \
  I##pnc=(img)(_p##x,_n##y,    z,v), I##cnc=(img)(x,_n##y,    z,v), I##nnc=(img)(_n##x,_n##y,    z,v), \
  I##ppn=(img)(_p##x,_p##y,_n##z,v), I##cpn=(img)(x,_p##y,_n##z,v), I##npn=(img)(_n##x,_p##y,_n##z,v), \
  I##pcn=(img)(_p##x,    y,_n##z,v), I##ccn=(img)(x,    y,_n##z,v), I##ncn=(img)(_n##x,    y,_n##z,v), \
  I##pnn=(img)(_p##x,_n##y,_n##z,v), I##cnn=(img)(x,_n##y,_n##z,v), I##nnn=(img)(_n##x,_n##y,_n##z,v)

#define CImg_2x2(I,T) CImg_2x2x1(I,T)
#define CImg_3x3(I,T) CImg_3x3x1(I,T)
#define CImg_4x4(I,T) CImg_4x4x1(I,T)
#define CImg_5x5(I,T) CImg_5x5x1(I,T)
#define CImg_2x2_ref(I,T,tab) CImg_2x2x1_ref(I,T,tab)
#define CImg_3x3_ref(I,T,tab) CImg_3x3x1_ref(I,T,tab)
#define CImg_4x4_ref(I,T,tab) CImg_4x4x1_ref(I,T,tab)
#define CImg_5x5_ref(I,T,tab) CImg_5x5x1_ref(I,T,tab)
#define cimg_copy2x2(J,I) cimg_copy2x2x1(J,I)
#define cimg_copy3x3(J,I) cimg_copy3x3x1(J,I)
#define cimg_copy5x5(J,I) cimg_copy5x5x1(J,I)
#define cimg_squaresum2x2(I) cimg_squaresum2x2x1(I)
#define cimg_squaresum3x3(I) cimg_squaresum3x3x1(I)
#define cimg_squaresum4x4(I) cimg_squaresum4x4x1(I)
#define cimg_squaresum5x5(I) cimg_squaresum5x5x1(I)
#define cimg_corr2x2(I) cimg_corr2x2x1(I)
#define cimg_corr3x3(I) cimg_corr3x3x1(I)
#define cimg_corr4x4(I) cimg_corr4x4x1(I)
#define cimg_corr5x5(I) cimg_corr5x5x1(I)
#define cimg_conv2x2(I) cimg_conv2x2x1(I)
#define cimg_conv3x3(I) cimg_conv3x3x1(I)
#define cimg_conv4x4(I) cimg_conv4x4x1(I)
#define cimg_conv5x5(I) cimg_conv5x5x1(I)
#define cimg_get2x2(img,x,y,z,k,I) cimg_get2x2x1(img,x,y,z,k,I)
#define cimg_get3x3(img,x,y,z,k,I) cimg_get3x3x1(img,x,y,z,k,I)
#define cimg_get4x4(img,x,y,z,k,I) cimg_get4x4x1(img,x,y,z,k,I)
#define cimg_get5x5(img,x,y,z,k,I) cimg_get5x5x1(img,x,y,z,k,I)
#define cimg_map2x2(img,x,y,z,k,I) cimg_map2x2x1(img,x,y,z,k,I)
#define cimg_map3x3(img,x,y,z,k,I) cimg_map3x3x1(img,x,y,z,k,I)
#define cimg_map4x4(img,x,y,z,k,I) cimg_map4x4x1(img,x,y,z,k,I)
#define cimg_map5x5(img,x,y,z,k,I) cimg_map5x5x1(img,x,y,z,k,I)

// Macros used to define special image loops.
// (see module 'Using Image Loops' in the generated documentation).
#define cimg_map(img,ptr,T_ptr)   for (T_ptr *ptr=(img).data+(img).size(); (ptr--)>(img).data; )
#define cimgl_map(list,l)         for (unsigned int l=0; l<(list).size; l++)
#define cimg_mapoff(img,off)      for (unsigned int off=0; off<(img).size(); off++)
#define cimg_mapX(img,x)          for (int x=0; x<(int)((img).width); x++)
#define cimg_mapY(img,y)          for (int y=0; y<(int)((img).height);y++)
#define cimg_mapZ(img,z)          for (int z=0; z<(int)((img).depth); z++)
#define cimg_mapV(img,v)          for (int v=0; v<(int)((img).dim);   v++)
#define cimg_mapXY(img,x,y)       cimg_mapY(img,y) cimg_mapX(img,x)
#define cimg_mapXZ(img,x,z)       cimg_mapZ(img,z) cimg_mapX(img,x)
#define cimg_mapYZ(img,y,z)       cimg_mapZ(img,z) cimg_mapY(img,y)
#define cimg_mapXV(img,x,v)       cimg_mapV(img,v) cimg_mapX(img,x)
#define cimg_mapYV(img,y,v)       cimg_mapV(img,v) cimg_mapY(img,y)
#define cimg_mapZV(img,z,v)       cimg_mapV(img,v) cimg_mapZ(img,z)
#define cimg_mapXYZ(img,x,y,z)    cimg_mapZ(img,z) cimg_mapXY(img,x,y)
#define cimg_mapXYV(img,x,y,v)    cimg_mapV(img,v) cimg_mapXY(img,x,y)
#define cimg_mapXZV(img,x,z,v)    cimg_mapV(img,v) cimg_mapXZ(img,x,z)
#define cimg_mapYZV(img,y,z,v)    cimg_mapV(img,v) cimg_mapYZ(img,y,z)
#define cimg_mapXYZV(img,x,y,z,v) cimg_mapV(img,v) cimg_mapXYZ(img,x,y,z)
#define cimg_imapX(img,x,n)       for (int x=(n); x<(int)((img).width-(n)); x++)
#define cimg_imapY(img,y,n)       for (int y=(n); y<(int)((img).height-(n)); y++)
#define cimg_imapZ(img,z,n)       for (int z=(n); z<(int)((img).depth-(n)); z++)
#define cimg_imapV(img,v,n)       for (int v=(n); v<(int)((img).dim-(n)); v++)
#define cimg_imapXY(img,x,y,n)    cimg_imapY(img,y,n) cimg_imapX(img,x,n)
#define cimg_imapXYZ(img,x,y,z,n) cimg_imapZ(img,z,n) cimg_imapXY(img,x,y,n)
#define cimg_bmapX(img,x,n)       for (int x=0; x<(int)((img).width);  x==(n)-1?(x=(img).width-(n)): x++)
#define cimg_bmapY(img,y,n)       for (int y=0; y<(int)((img).height); y==(n)-1?(x=(img).height-(n)):y++)
#define cimg_bmapZ(img,z,n)       for (int z=0; z<(int)((img).depth);  z==(n)-1?(x=(img).depth-(n)): z++)
#define cimg_bmapV(img,v,n)       for (int v=0; v<(int)((img).dim);    v==(n)-1?(x=(img).dim-(n)):   v++)
#define cimg_bmapXY(img,x,y,n)    cimg_mapY(img,y) for (int x=0; x<(int)((img).width); (y<(n) || y>=(int)((img).height)-(n))?x++: \
                                                          ((x<(n)-1 || x>=(int)((img).width)-(n))?x++:(x=(img).width-(n))))
#define cimg_bmapXYZ(img,x,y,z,n) cimg_mapYZ(img,y,z) for (int x=0; x<(int)((img).width); (y<(n) || y>=(int)((img).height)-(n) || z<(n) || z>=(int)((img).depth)-(n))?x++: \
                                                             ((x<(n)-1 || x>=(int)((img).width)-(n))?x++:(x=(img).width-(n))))
#define cimg_2mapX(img,x)         for (int x=0,_n##x=1; _n##x<(int)((img).width)   || x==--_n##x; x++, _n##x++)
#define cimg_2mapY(img,y)         for (int y=0,_n##y=1; _n##y<(int)((img).height)  || y==--_n##y; y++, _n##y++)
#define cimg_2mapZ(img,z)         for (int z=0,_n##z=1; _n##z<(int)((img).depth)   || z==--_n##z; z++, _n##z++)
#define cimg_2mapXY(img,x,y)      cimg_2mapY(img,y) cimg_2mapX(img,x)
#define cimg_2mapXZ(img,x,z)      cimg_2mapZ(img,z) cimg_2mapX(img,x)
#define cimg_2mapYZ(img,y,z)      cimg_2mapZ(img,z) cimg_2mapY(img,y)
#define cimg_2mapXYZ(img,x,y,z)   cimg_2mapZ(img,z) cimg_2mapXY(img,x,y)
#define cimg_3mapX(img,x)         for (int x=0,_p##x=0,_n##x=1; _n##x<(int)((img).width)  || x==--_n##x;  _p##x=x++,_n##x++)
#define cimg_3mapY(img,y)         for (int y=0,_p##y=0,_n##y=1; _n##y<(int)((img).height) || y==--_n##y;  _p##y=y++,_n##y++)
#define cimg_3mapZ(img,z)         for (int z=0,_p##z=0,_n##z=1; _n##z<(int)((img).depth)  || z==--_n##z;  _p##z=z++,_n##z++)
#define cimg_3mapXY(img,x,y)      cimg_3mapY(img,y) cimg_3mapX(img,x)
#define cimg_3mapXZ(img,x,z)      cimg_3mapZ(img,z) cimg_3mapX(img,x)
#define cimg_3mapYZ(img,y,z)      cimg_3mapZ(img,z) cimg_3mapY(img,y)
#define cimg_3mapXYZ(img,x,y,z)   cimg_3mapZ(img,z) cimg_3mapXY(img,x,y)
#define cimg_4mapX(img,x)         for (int _p##x=0,x=0,_n##x=1,_a##x=2; \
                                       _a##x<(int)((img).width)  || _n##x==--_a##x || x==(_a##x=--_n##x); \
                                       _p##x=x++,_n##x++,_a##x++)
#define cimg_4mapY(img,y)         for (int _p##y=0,y=0,_n##y=1,_a##y=2; \
                                       _a##y<(int)((img).height) || _n##y==--_a##y || y==(_a##y=--_n##y); \
                                       _p##y=y++,_n##y++,_a##y++)
#define cimg_4mapZ(img,z)         for (int _p##z=0,z=0,_n##z=1,_a##z=2; \
                                       _a##z<(int)((img).depth)  || _n##z==--_a##z || z==(_a##z=--_n##z); \
                                       _p##z=z++,_n##z++,_a##z++)
#define cimg_4mapXY(img,x,y)      cimg_4mapY(img,y) cimg_4mapX(img,x)
#define cimg_4mapXZ(img,x,z)      cimg_4mapZ(img,z) cimg_4mapX(img,x)
#define cimg_4mapYZ(img,y,z)      cimg_4mapZ(img,z) cimg_4mapY(img,y)
#define cimg_4mapXYZ(img,x,y,z)   cimg_4mapZ(img,z) cimg_4mapXY(img,x,y)
#define cimg_5mapX(img,x)         for (int _b##x=0,_p##x=0,x=0,_n##x=1,_a##x=2; \
                                       _a##x<(int)((img).width)  || _n##x==--_a##x || x==(_a##x=--_n##x); \
                                       _b##x=_p##x,_p##x=x++,_n##x++,_a##x++)
#define cimg_5mapY(img,y)         for (int _b##y=0,_p##y=0,y=0,_n##y=1,_a##y=2; \
                                       _a##y<(int)((img).height) || _n##y==--_a##y || y==(_a##y=--_n##y); \
                                       _b##y=_p##y,_p##y=y++,_n##y++,_a##y++)
#define cimg_5mapZ(img,z)         for (int _b##z=0,_p##z=0,z=0,_n##z=1,_a##z=2; \
                                       _a##z<(int)((img).depth)  || _n##z==--_a##z || z==(_a##z=--_n##z); \
                                       _b##z=_p##z,_p##z=z++,_n##z++,_a##z++)
#define cimg_5mapXY(img,x,y)      cimg_5mapY(img,y) cimg_5mapX(img,x)
#define cimg_5mapXZ(img,x,z)      cimg_5mapZ(img,z) cimg_5mapX(img,x)
#define cimg_5mapYZ(img,y,z)      cimg_5mapZ(img,z) cimg_5mapY(img,y)
#define cimg_5mapXYZ(img,x,y,z)   cimg_5mapZ(img,z) cimg_5mapXY(img,x,y)

#define cimg_map2x2x1(img,x,y,z,v,I) cimg_2mapY(img,y) \
       for (int _n##x=1, x=(int)((I##cc=(img)(0,    y,z,v)), \
                                 (I##cn=(img)(0,_n##y,z,v)), \
				 0); \
            (_n##x<(int)((img).width) && ((I##nc=(img)(_n##x,    y,z,v)), \
                                          (I##nn=(img)(_n##x,_n##y,z,v)), \
                                          1)) || x==--_n##x; \
            I##cc=I##nc, I##cn=I##nn, \
            x++,_n##x++ )

#define cimg_map3x3x1(img,x,y,z,v,I) cimg_3mapY(img,y) \
       for (int _n##x=1, _p##x=(int)((I##cp=I##pp=(img)(0,_p##y,z,v)), \
                                     (I##cc=I##pc=(img)(0,  y,z,v)), \
                                     (I##cn=I##pn=(img)(0,_n##y,z,v))), \
                                     x=_p##x=0; \
            (_n##x<(int)((img).width) && ((I##np=(img)(_n##x,_p##y,z,v)), \
                                          (I##nc=(img)(_n##x,    y,z,v)), \
                                          (I##nn=(img)(_n##x,_n##y,z,v)), \
                                          1)) || x==--_n##x; \
              I##pp=I##cp, I##pc=I##cc, I##pn=I##cn, \
              I##cp=I##np, I##cc=I##nc, I##cn=I##nn, \
              _p##x=x++,_n##x++ )


#define cimg_map4x4x1(img,x,y,z,v,I) cimg_4mapY(img,y) \
       for (int _a##x=2, _n##x=1, x=(int)((I##cp=I##pp=(img)(0,_p##y,z,v)), \
                                          (I##cc=I##pc=(img)(0,    y,z,v)), \
                                          (I##cn=I##pn=(img)(0,_n##y,z,v)), \
                                          (I##ca=I##pa=(img)(0,_a##y,z,v)), \
                                          (I##np=(img)(_n##x,_p##y,z,v)), \
                                          (I##nc=(img)(_n##x,    y,z,v)), \
                                          (I##nn=(img)(_n##x,_n##y,z,v)), \
                                          (I##na=(img)(_n##x,_a##y,z,v)), \
 				          0), _p##x=0; \
            (_a##x<(int)((img).width) && ((I##ap=(img)(_a##x,_p##y,z,v)), \
                                          (I##ac=(img)(_a##x,    y,z,v)), \
                                          (I##an=(img)(_a##x,_n##y,z,v)), \
                                          (I##aa=(img)(_a##x,_a##y,z,v)), \
                                          1)) || _n##x==--_a##x || x==(_a##x=--_n##x); \
              I##pp=I##cp, I##pc=I##cc, I##pn=I##cn, I##pa=I##ca, \
              I##cp=I##np, I##cc=I##nc, I##cn=I##nn, I##ca=I##na, \
              I##np=I##ap, I##nc=I##ac, I##nn=I##an, I##na=I##aa, \
              _p##x=x++, _n##x++, _a##x++ )

#define cimg_map5x5x1(img,x,y,z,v,I) cimg_5mapY(img,y) \
       for (int _a##x=2, _n##x=1, _b##x=(int)((I##cb=I##pb=I##bb=(img)(0,_b##y,z,v)), \
                                              (I##cp=I##pp=I##bp=(img)(0,_p##y,z,v)), \
                                              (I##cc=I##pc=I##bc=(img)(0,    y,z,v)), \
                                              (I##cn=I##pn=I##bn=(img)(0,_n##y,z,v)), \
                                              (I##ca=I##pa=I##ba=(img)(0,_a##y,z,v)), \
                                              (I##nb=(img)(_n##x,_b##y,z,v)), \
                                              (I##np=(img)(_n##x,_p##y,z,v)), \
                                              (I##nc=(img)(_n##x,   y,z,v)), \
                                              (I##nn=(img)(_n##x,_n##y,z,v)), \
                                              (I##na=(img)(_n##x,_a##y,z,v))), \
                                              x=0, _p##x=_b##x=0; \
            (_a##x<(int)((img).width) && ((I##ab=(img)(_a##x,_b##y,z,v)), \
                                          (I##ap=(img)(_a##x,_p##y,z,v)), \
                                          (I##ac=(img)(_a##x,    y,z,v)), \
                                          (I##an=(img)(_a##x,_n##y,z,v)), \
                                          (I##aa=(img)(_a##x,_a##y,z,v)), \
                                          1)) || _n##x==--_a##x || x==(_a##x=--_n##x); \
              I##bb=I##pb, I##bp=I##pp, I##bc=I##pc, I##bn=I##pn, I##ba=I##pa, \
              I##pb=I##cb, I##pp=I##cp, I##pc=I##cc, I##pn=I##cn, I##pa=I##ca, \
              I##cb=I##nb, I##cp=I##np, I##cc=I##nc, I##cn=I##nn, I##ca=I##na, \
              I##nb=I##ab, I##np=I##ap, I##nc=I##ac, I##nn=I##an, I##na=I##aa, \
              _b##x=_p##x, _p##x=x++, _n##x++, _a##x++ )

#define cimg_map2x2x2(img,x,y,z,v,I) cimg_2mapYZ(img,y,z) \
       for (int _n##x=1, x=(int)((I##ccc=(img)(0,    y,    z,v)), \
                                 (I##cnc=(img)(0,_n##y,    z,v)), \
                                 (I##ccn=(img)(0,    y,_n##z,v)), \
                                 (I##cnn=(img)(0,_n##y,_n##z,v)), \
                                 0); \
            (_n##x<(int)((img).width) && ((I##ncc=(img)(_n##x,    y,    z,v)), \
                                          (I##nnc=(img)(_n##x,_n##y,    z,v)), \
                                          (I##ncn=(img)(_n##x,    y,_n##z,v)), \
                                          (I##nnn=(img)(_n##x,_n##y,_n##z,v)), \
                                          1)) || x==--_n##x; \
              I##ccc=I##ncc, I##cnc=I##nnc, \
              I##ccn=I##ncn, I##cnn=I##nnn, \
              x++, _n##x++ )

#define cimg_map3x3x3(img,x,y,z,v,I) cimg_3mapYZ(img,y,z) \
       for (int _n##x=1, _p##x=(int)((I##cpp=I##ppp=(img)(0,_p##y,_p##z,v)), \
                                     (I##ccp=I##pcp=(img)(0,    y,_p##z,v)), \
                                     (I##cnp=I##pnp=(img)(0,_n##y,_p##z,v)), \
                                     (I##cpc=I##ppc=(img)(0,_p##y,    z,v)), \
                                     (I##ccc=I##pcc=(img)(0,    y,    z,v)), \
                                     (I##cnc=I##pnc=(img)(0,_n##y,    z,v)), \
                                     (I##cpn=I##ppn=(img)(0,_p##y,_n##z,v)), \
                                     (I##ccn=I##pcn=(img)(0,    y,_n##z,v)), \
                                     (I##cnn=I##pnn=(img)(0,_n##y,_n##z,v))),\
                                     x=_p##x=0; \
            (_n##x<(int)((img).width) && ((I##npp=(img)(_n##x,_p##y,_p##z,v)), \
                                          (I##ncp=(img)(_n##x,    y,_p##z,v)), \
                                          (I##nnp=(img)(_n##x,_n##y,_p##z,v)), \
                                          (I##npc=(img)(_n##x,_p##y,    z,v)), \
                                          (I##ncc=(img)(_n##x,    y,    z,v)), \
                                          (I##nnc=(img)(_n##x,_n##y,    z,v)), \
                                          (I##npn=(img)(_n##x,_p##y,_n##z,v)), \
                                          (I##ncn=(img)(_n##x,    y,_n##z,v)), \
                                          (I##nnn=(img)(_n##x,_n##y,_n##z,v)), \
                                          1)) || x==--_n##x; \
              I##ppp=I##cpp, I##pcp=I##ccp, I##pnp=I##cnp, \
              I##cpp=I##npp, I##ccp=I##ncp, I##cnp=I##nnp, \
              I##ppc=I##cpc, I##pcc=I##ccc, I##pnc=I##cnc, \
              I##cpc=I##npc, I##ccc=I##ncc, I##cnc=I##nnc, \
              I##ppn=I##cpn, I##pcn=I##ccn, I##pnn=I##cnn, \
              I##cpn=I##npn, I##ccn=I##ncn, I##cnn=I##nnn, \
              _p##x=x++, _n##x++ )

namespace cimg_library1_16 {

  // Define the CImg classes.
  template<typename T=float> struct CImg;
  template<typename T=float> struct CImgl;
  struct CImgStats;
  struct CImgDisplay;
  struct CImgException; 

  namespace cimg {

    // The bodies of the functions below are defined at the end of the file
    inline int dialog(const char *title,const char *msg,const char *button1_txt="OK",
		      const char *button2_txt=0,const char *button3_txt=0,
		      const char *button4_txt=0,const char *button5_txt=0,
		      const char *button6_txt=0,const bool centering = false);
  }
  
  /*
   #----------------------------------------------
   #
   #
   # Definition of the CImgException structures
   #
   #
   #----------------------------------------------
   */

  // Never use the following macro in your own code !
#define cimg_exception_err(etype,disp_flag) ;

  struct CImgException {
    char message[1024]; //!< Message associated with the error that thrown the exception.
    CImgException() { message[0]='\0'; }
    CImgException(const char *,...) { cimg_exception_err("CImgException",true); }
  };

  // The \ref CImgInstanceException class is used to throw an exception related
  // to a non suitable instance encountered in a library function call.  
  struct CImgInstanceException : CImgException { 
    CImgInstanceException(const char *,...) { cimg_exception_err("CImgInstanceException",true); }
  };

  // The \ref CImgArgumentException class is used to throw an exception related
  // to invalid arguments encountered in a library function call.
  struct CImgArgumentException : CImgException { 
    CImgArgumentException(const char *,...) { cimg_exception_err("CImgArgumentException",true); }
  };

  // The \ref CImgIOException class is used to throw an exception related 
  // to Input/Output file problems encountered in a library function call.
  struct CImgIOException : CImgException { 
    CImgIOException(const char *,...) { cimg_exception_err("CImgIOException",true); }
  };

  // The CImgDisplayException class is used to throw an exception related to display problems
  // encountered in a library function call.
  struct CImgDisplayException : CImgException {
    CImgDisplayException(const char *,...) { cimg_exception_err("CImgDisplayException",false); }
  };

  /*
   #-------------------------------------
   #
   #
   # Definition of the namespace 'cimg'
   #
   #
   #-------------------------------------
   */
  namespace cimg {

    // Define the trait that will be used to determine the best data type to work with.
    template<typename T,typename t> struct largest { typedef t type; };
    template<> struct largest<unsigned char,bool> { typedef unsigned char type; };
    template<> struct largest<unsigned char,char> { typedef short type; };
    template<> struct largest<char,bool> { typedef char type; };
    template<> struct largest<char,unsigned char> { typedef short type; };
    template<> struct largest<char,unsigned short> { typedef int type; };
    template<> struct largest<char,unsigned int> { typedef float type; };
    template<> struct largest<char,unsigned long> { typedef float type; };
    template<> struct largest<unsigned short,bool> { typedef unsigned short type; };
    template<> struct largest<unsigned short,unsigned char> { typedef unsigned short type; };
    template<> struct largest<unsigned short,char> { typedef short type; };
    template<> struct largest<unsigned short,short> { typedef int type; };
    template<> struct largest<short,bool> { typedef short type; };
    template<> struct largest<short,unsigned char> { typedef short type; };
    template<> struct largest<short,char> { typedef short type; };
    template<> struct largest<short,unsigned short> { typedef int type; };
    template<> struct largest<short,unsigned int> { typedef float type; };
    template<> struct largest<short,unsigned long> { typedef float type; };
    template<> struct largest<unsigned int,bool> { typedef unsigned int type; };
    template<> struct largest<unsigned int,unsigned char> { typedef unsigned int type; };
    template<> struct largest<unsigned int,char> { typedef unsigned int type; };
    template<> struct largest<unsigned int,unsigned short> { typedef unsigned int type; };
    template<> struct largest<unsigned int,short> { typedef float type; };
    template<> struct largest<unsigned int,int> { typedef float type; };
    template<> struct largest<int,bool> { typedef int type; };
    template<> struct largest<int,unsigned char> { typedef int type; };
    template<> struct largest<int,char> { typedef int type; };
    template<> struct largest<int,unsigned short> { typedef int type; };
    template<> struct largest<int,short> { typedef int type; };
    template<> struct largest<int,unsigned int> { typedef float type; };
    template<> struct largest<int,unsigned long> { typedef float type; };
    template<> struct largest<float,bool> { typedef float type; };
    template<> struct largest<float,unsigned char> { typedef float type; };
    template<> struct largest<float,char> { typedef float type; };
    template<> struct largest<float,unsigned short> { typedef float type; };
    template<> struct largest<float,short> { typedef float type; };
    template<> struct largest<float,unsigned int> { typedef float type; };
    template<> struct largest<float,int> { typedef float type; };
    template<> struct largest<float,unsigned long> { typedef float type; };
    template<> struct largest<float,long> { typedef float type; };
    template<> struct largest<double,bool> { typedef double type; };
    template<> struct largest<double,unsigned char> { typedef double type; };
    template<> struct largest<double,char> { typedef double type; };
    template<> struct largest<double,unsigned short> { typedef double type; };
    template<> struct largest<double,short> { typedef double type; };
    template<> struct largest<double,unsigned int> { typedef double type; };
    template<> struct largest<double,int> { typedef double type; };
    template<> struct largest<double,unsigned long> { typedef double type; };
    template<> struct largest<double,long> { typedef double type; };
    template<> struct largest<double,float> { typedef double type; };

    template<typename T> struct type {
      static T min() { return (T)-1>0?(T)0:(T)-((T)(1U<<(8*sizeof(T)-1))); }
      static T max() { return (T)-1>0?(T)-1:(T)((1U<<(8*sizeof(T)-1))-1); }
    };

    template<> struct type<unsigned char> {
      static unsigned char min() { return 0; }
      static unsigned char max() { return (unsigned char)~0U; }
    };
    template<> struct type<unsigned short> {
      static unsigned short min() { return 0; }
      static unsigned short max() { return (unsigned short)~0U; }
    };
    template<> struct type<unsigned int> {
      static unsigned int min() { return 0; }
      static unsigned int max() { return (unsigned int)~0U; }
    };
    template<> struct type<unsigned long> {
      static unsigned long min() { return 0; }
      static unsigned long max() { return (unsigned long)~0UL; }
    };
    template<> struct type<bool> {
      static bool min() { return false; }
      static bool max() { return true; }
    };
    template<> struct type<float> {
      static float min() { return -3.4E38f; }
      static float max() { return  3.4E38f; }
    };
    template<> struct type<double> {
      static double min() { return -1.7E308; }
      static double max() { return  1.7E308; }
    };
    
    // Define internal library variables.
#if cimg_display_type==1
    struct X11info {
      volatile unsigned int nb_wins;
      pthread_mutex_t* mutex;
      pthread_t*       event_thread;
      CImgDisplay*     wins[1024];
      Display*         display; 
      unsigned int     nb_bits;
      GC*              gc;
      bool             blue_first;
      bool             byte_order;
      bool             shm_enabled;
#ifdef cimg_use_xrandr
      XRRScreenSize *resolutions;
      Rotation curr_rotation;
      unsigned int curr_resolution;
      unsigned int nb_resolutions;
#endif      
      X11info():nb_wins(0),mutex(0),event_thread(0),display(0),
		nb_bits(0),gc(0),blue_first(false),byte_order(false),shm_enabled(false) {
#ifdef cimg_use_xrandr
	resolutions = 0;
	curr_rotation = 0;
	curr_resolution = nb_resolutions = 0;
#endif
      }
    };
#if defined(cimg_module)
    X11info& X11attr();
#elif defined(cimg_main)
    X11info& X11attr() { static X11info val; return val; }
#else
    inline X11info& X11attr() { static X11info val; return val; }
#endif

#elif cimg_display_type==2
    struct Win32info {
      HANDLE wait_event;
      Win32info() { wait_event = CreateEvent(0,FALSE,FALSE,0); }
    };
#if defined(module)
    Win32info& Win32attr();
#elif defined(cimg_main)
    Win32info& Win32attr() { static Win32info val; return val; }
#else
    inline Win32info& Win32attr() { static Win32info val; return val; }
#endif
#endif
    
#ifdef cimg_color_terminal
    const char t_normal[9]  = {0x1b,'[','0',';','0',';','0','m','\0'};
    const char t_red[11]    = {0x1b,'[','4',';','3','1',';','5','9','m','\0'};
    const char t_bold[5]    = {0x1b,'[','1','m','\0'};
    const char t_purple[11] = {0x1b,'[','0',';','3','5',';','5','9','m','\0'};
#else
    const char t_normal[1]  = {'\0'};
    const char *const t_red = cimg::t_normal, *const t_bold = cimg::t_normal, *const t_purple = cimg::t_normal;
#endif

#ifdef PI
#undef PI
#endif
    const double PI = 3.14159265358979323846;   //!< Definition of the mathematical constant PI
    
    // Definition of a 7x11 font, used to return a default font for drawing text.
    const unsigned int font7x11[7*11*256/32] = {
      0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x80000000,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
      0x0,0x0,0x0,0x0,0x0,0x0,0x90,0x0,0x7f0000,0x40000,0x0,0x0,0x4010c0a4,0x82000040,0x11848402,0x18480050,0x80430292,0x8023,0x9008000,
      0x40218140,0x4000040,0x21800402,0x18000051,0x1060500,0x8083,0x10000,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x24002,0x4031,0x80000000,0x10000,
      0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x7,0x81c0400,0x40020000,0x80070080,0x40440e00,0x0,0x0,0x1,0x88180000,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
      0x0,0x200000,0x0,0x0,0x80000,0x0,0x0,0x20212140,0x5000020,0x22400204,0x240000a0,0x40848500,0x4044,0x80010038,0x20424285,0xa000020,
      0x42428204,0x2428e0a0,0x82090a14,0x4104,0x85022014,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x10240a7,0x88484040,0x40800000,0x270c3,0x87811e0e,
      0x7c70e000,0x78,0x3c23c1ef,0x1f3e1e89,0xf1c44819,0xa23cf0f3,0xc3cff120,0xc18307f4,0x4040400,0x20000,0x80080080,0x40200,0x0,
      0x40000,0x2,0x8040000,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x8188,0x50603800,0xf3c00000,0x1c004003,0xc700003e,0x18180,0xc993880,0x10204081,
      0x2071ef9,0xf3e7cf9f,0x3e7c7911,0xe3c78f1e,0x7d1224,0x48906048,0x0,0x4000000,0x0,0x9000,0x0,0x0,0x2000,0x0,0x0,0x0,0x0,0x0,0x0,
      0x0,0x10240aa,0x14944080,0x23610000,0x68940,0x40831010,0x8891306,0x802044,0x44522208,0x90202088,0x40448819,0xb242890a,0x24011111,
      0x49448814,0x4040a00,0xe2c3c7,0x8e3f3cb9,0xc1c44216,0xee38b0f2,0xe78f9120,0xc18507e2,0x8040000,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
      0x101c207,0x88a04001,0x9c00000,0x2200a041,0x8200113a,0x8240,0x50a3110,0x2850a142,0x850c2081,0x2040204,0x8104592,0x142850a1,
      0x42cd1224,0x4888bc48,0x70e1c387,0xe3b3c70,0xe1c38e1c,0x38707171,0xc3870e1c,0x10791224,0x48906c41,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
      0x10003ee,0x15140080,0x21810000,0x48840,0x40851020,0x8911306,0x31fd804,0x9c522408,0x90204088,0x4045081a,0xba42890a,0x24011111,
      0x49285024,0x2041b00,0x132408,0x910844c8,0x4044821b,0x7244c913,0x24041111,0x49488822,0x8040000,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
      0x28204,0x85006001,0x6a414000,0x3a004043,0xc700113a,0x8245,0x50a3a00,0x2850a142,0x850c4081,0x2040204,0x81045d2,0x142850a1,
      0x24951224,0x48852250,0x8102040,0x81054089,0x12244204,0x8108992,0x24489122,0x991224,0x4888b222,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
      0x1000143,0xa988080,0x2147c01f,0x88840,0x83091c2c,0x1070f000,0xc000608,0xa48bc408,0x9e3c46f8,0x40460816,0xaa42f10b,0xc3811111,
      0x35102044,0x1041100,0xf22408,0x9f084488,0x40470212,0x62448912,0x6041111,0x55308846,0x8061c80,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
      0x1028704,0x8f805801,0x4be28fdf,0x220001f0,0x111a,0x60000182,0x82c5c710,0x44891224,0x489640f1,0xe3c78204,0x810e552,0x142850a1,
      0x18a51224,0x48822250,0x78f1e3c7,0x8f1f40f9,0xf3e7c204,0x8108912,0x24489122,0x7ea91224,0x4888a222,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
      0x10007e2,0x85648080,0x20010000,0x88841,0x8f8232,0x20881000,0xc1fc610,0xbefa2408,0x90204288,0x40450816,0xa642810a,0x4041110a,
      0x36282084,0x1042080,0x1122408,0x90084488,0x40450212,0x62448912,0x184110a,0x55305082,0x8042700,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
      0x1028207,0x82004801,0x68050040,0x1c000040,0x110a,0x60000001,0x45484d10,0x7cf9f3e7,0xcf944081,0x2040204,0x8104532,0x142850a1,
      0x18a51224,0x48822248,0x89122448,0x91244081,0x2040204,0x8108912,0x24489122,0xc91224,0x48852214,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x282,
      0x89630080,0x20010c00,0x30108842,0x810222,0x20882306,0x3001800,0x408a2208,0x90202288,0x40448814,0xa642810a,0x2041110a,0x26442104,
      0x840000,0x1122408,0x90084488,0x40448212,0x62448912,0x84130a,0x36485102,0x8040000,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x101c208,0x4f802801,
      0x8028040,0x40,0x130a,0x2,0x85e897a0,0x44891224,0x489c2081,0x2040204,0x8104532,0x142850a1,0x24cd1224,0x48823c44,0x89122448,
      0x91244081,0x2040204,0x8108912,0x24489122,0xc93264,0xc9852214,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x100028f,0x109f0080,0x20010c00,
      0x303071f3,0xc7011c1c,0x4071c306,0x802010,0x3907c1ef,0x1f201e89,0xf3844f90,0xa23c80f2,0x17810e04,0x228223f4,0x840000,0xfbc3c7,
      0x8f083c88,0x40444212,0x6238f0f2,0x7039d04,0x228423e2,0x8040000,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x1008780,0x2201800,0xf0014000,0x1f0,
      0x1d0a,0x5,0x851e140,0x83060c18,0x30671ef9,0xf3e7cf9f,0x3e7c7911,0xe3c78f1e,0x42f8e1c3,0x8702205c,0x7cf9f3e7,0xcf9b3c78,0xf1e3c204,
      0x8107111,0xc3870e1c,0x10f1d3a7,0x4e823c08,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x2,0x40,0x40000400,0x200000,0x0,0x2,0x0,0x0,0x0,0x0,0x18,
      0x0,0x4,0x44007f,0x0,0x400,0x400000,0x8010,0x0,0x6002,0x8040000,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x1000000,0x200800,0x0,0x0,0x100a,
      0x400000,0x44,0x0,0x400,0x0,0x0,0x0,0x0,0x0,0x0,0x800,0x0,0x0,0x0,0x0,0x62018,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x31,0x80000800,
      0x400000,0x0,0x4,0x0,0x0,0x0,0x0,0xc,0x0,0x7,0x3c0000,0x0,0x3800,0x3800000,0x8010,0x0,0x1c001,0x881c0000,0x0,0x0,0x0,0x0,0x0,0x0,
      0x0,0x0,0x207000,0x0,0x0,0x100a,0xc00000,0x3c,0x0,0xc00,0x0,0x0,0x0,0x0,0x0,0x0,0x1800,0x0,0x0,0x0,0x0,0x1c2070
    };

    // Definition of a 10x13 font (used in dialog boxes).
    const unsigned int font10x13[256*10*13/32] = {
      0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
      0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x80100c0,
      0x68000300,0x801,0xc00010,0x100c000,0x68100,0x100c0680,0x2,0x403000,0x1000000,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
      0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
      0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0xfc,0x0,0x0,0x0,0x0,0x0,0x4020120,
      0x58120480,0x402,0x1205008,0x2012050,0x58080,0x20120581,0x40000001,0x804812,0x2000000,0x0,0x300,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
      0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x140,0x80000,0x200402,0x800000,0x10,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
      0x0,0x7010,0x7000000,0x8000200,0x20000,0xc0002000,0x8008,0x0,0x0,0x0,0x0,0x808,0x4000000,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
      0x0,0x0,0x80000000,0x0,0x0,0x0,0x40000,0x0,0x0,0x0,0x0,0x480,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x70,0x80100c0,0x68000480,0x1001,
      0xc00010,0x1018000,0x68100,0x100c0680,0x4,0x403000,0x1020000,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x20140,0x28081883,0x200801,
      0x2a00000,0x10,0x1c0201c0,0x70040f80,0xc0f81c07,0x0,0x70,0x3e0303c0,0x3c3c0f83,0xe03c2107,0xe08810,0x18c31070,0x3c0703c0,
      0x783e0842,0x22222208,0x83e04010,0x1008000,0x4000200,0x20001,0x2002,0x408008,0x0,0x0,0x100000,0x0,0x1008,0x2000000,0x0,0x0,0x0,
      0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x20080,0x38000880,0x8078140f,0x81c00000,0x3e000,0xc020180,0x60080001,0xe0000002,0xc00042,0x108e2010,
      0xc0300c0,0x300c0303,0xf83c3e0f,0x83e0f81c,0x701c070,0x3c0c41c0,0x701c0701,0xc0001d08,0x42108421,0x8820088,0x4020120,0x58140480,
      0x802,0x1205008,0x3014050,0xc058080,0x20120581,0x40000002,0x804814,0x2020050,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x20140,
      0x281e2484,0x80200801,0x1c02000,0x10,0x22060220,0x880c0801,0x82208,0x80000001,0x20008,0x41030220,0x40220802,0x402102,0x209010,
      0x18c31088,0x22088220,0x80080842,0x22222208,0x80204010,0x1014000,0x200,0x20001,0x2000,0x8008,0x0,0x0,0x100000,0x0,0x1008,
      0x2000000,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x80,0x40000500,0x80800010,0x40200000,0x41000,0x12020040,0x10000003,0xa0000006,
      0x12000c4,0x31014000,0xc0300c0,0x300c0302,0x80402008,0x2008008,0x2008020,0x220c4220,0x88220882,0x20002208,0x42108421,0x8820088,
      0x0,0x300,0x0,0x0,0x0,0x14000000,0x0,0x200200,0x0,0x20000,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x20000,0xfc282504,0x80001000,
      0x82a02000,0x20,0x22020020,0x8140802,0x102208,0x80801006,0x18008,0x9c848220,0x80210802,0x802102,0x20a010,0x15429104,0x22104220,
      0x80080842,0x22221405,0x404008,0x1022000,0x703c0,0x381e0701,0xc0783c02,0xc09008,0x1d83c070,0x3c078140,0x381c0882,0x21242208,
      0x81e01008,0x2000000,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x201e0,0x40220500,0x80800027,0x20e02800,0x9c800,0x12020040,
      0x20000883,0xa0200002,0x120a044,0x11064010,0x12048120,0x48120484,0x80802008,0x2008008,0x2008020,0x210a4411,0x4411044,0x10884508,
      0x42108421,0x503c0b0,0x1c0701c0,0x701c0707,0x70381c07,0x1c07008,0x2008020,0x20f01c0,0x701c0701,0xc0201c08,0x82208822,0x883c088,
      0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x20000,0x50281903,0x20001000,0x80802000,0x20,0x22020040,0x30240f03,0xc0101c08,0x80801018,
      0x1fc06010,0xa48483c0,0x80210f03,0xe0803f02,0x20c010,0x15429104,0x22104220,0x70080841,0x41540805,0x804008,0x1041000,0x8220,
      0x40220881,0x882202,0x40a008,0x12422088,0x22088180,0x40100882,0x21241408,0x80201008,0x2031000,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
      0x0,0x20280,0x401c0200,0x700028,0x21205000,0x92800,0xc1fc080,0x10000883,0xa0200002,0x1205049,0x12c19010,0x12048120,0x48120484,
      0xf0803c0f,0x3c0f008,0x2008020,0x790a4411,0x4411044,0x10504908,0x42108421,0x5022088,0x2008020,0x8020080,0x88402208,0x82208808,
      0x2008020,0x1e088220,0x88220882,0x20002608,0x82208822,0x8822088,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x20000,0x501c0264,
      0xa0001000,0x8001fc00,0x7000020,0x22020080,0x83e0082,0x20202207,0x80000020,0x1020,0xa4848220,0x80210802,0x9c2102,0x20c010,
      0x12425104,0x3c1043c0,0x8080841,0x41540802,0x804008,0x1000000,0x78220,0x40220f81,0x882202,0x40c008,0x12422088,0x22088100,
      0x60100881,0x41540805,0x406008,0x1849000,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x20280,0xf0140200,0x880028,0x20e0a03f,0x709c800,
      0x201c0,0x60000881,0xa0000007,0xc0284b,0x122eb020,0x12048120,0x48120487,0x80802008,0x2008008,0x2008020,0x21094411,0x4411044,
      0x10204908,0x42108421,0x2022088,0x1e0781e0,0x781e0787,0xf8403e0f,0x83e0f808,0x2008020,0x22088220,0x88220882,0x21fc2a08,0x82208822,
      0x5022050,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x20001,0xf80a0294,0x40001000,0x80002000,0x20,0x22020100,0x8040082,0x20202200,
      0x80000018,0x1fc06020,0xa48fc220,0x80210802,0x842102,0x20a010,0x12425104,0x20104240,0x8080841,0x41541402,0x1004008,0x1000000,
      0x88220,0x40220801,0x882202,0x40a008,0x12422088,0x22088100,0x18100881,0x41540805,0x801008,0x2046000,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
      0x0,0x0,0x0,0x20280,0x401c0f80,0x80880028,0x20005001,0x94800,0x20000,0x880,0xa0000000,0x5015,0x4215040,0x3f0fc3f0,0xfc3f0fc8,
      0x80802008,0x2008008,0x2008020,0x21094411,0x4411044,0x10505108,0x42108421,0x203c088,0x22088220,0x88220888,0x80402008,0x2008008,
      0x2008020,0x22088220,0x88220882,0x20002a08,0x82208822,0x5022050,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0xa00a0494,0x60001000,
      0x80002004,0x8020,0x22020200,0x88040882,0x20402201,0x801006,0x18000,0x9f084220,0x40220802,0x442102,0x209010,0x10423088,0x20088220,
      0x8080840,0x80882202,0x2004008,0x1000000,0x88220,0x40220881,0x882202,0x409008,0x12422088,0x22088100,0x8100880,0x80881402,
      0x1001008,0x2000000,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x20280,0x40220200,0x80700027,0x20002801,0x92800,0x1fc000,0x980,
      0xa0000000,0xa017,0x84417840,0x21084210,0x84210848,0x80402008,0x2008008,0x2008020,0x2208c220,0x88220882,0x20882208,0x42108421,
      0x2020088,0x22088220,0x88220888,0xc8402208,0x82208808,0x2008020,0x22088220,0x88220882,0x20203208,0x82208822,0x2022020,0x0,0x0,0x0,
      0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x20000,0xa03c0463,0x90000801,0x2004,0x8040,0x1c0703e0,0x70040701,0xc0401c06,0x801001,0x20020,
      0x400843c0,0x3c3c0f82,0x3c2107,0x1c0881e,0x10423070,0x20070210,0xf0080780,0x80882202,0x3e04004,0x1000000,0x783c0,0x381e0701,
      0x782202,0x408808,0x12422070,0x3c078100,0x700c0780,0x80882202,0x1e01008,0x2000000,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x201e0,
      0xf8000200,0x80080010,0x40000001,0x41000,0x0,0xe80,0xa0000000,0x21,0x8e21038,0x21084210,0x84210848,0xf83c3e0f,0x83e0f81c,
      0x701c070,0x3c08c1c0,0x701c0701,0xc0005c07,0x81e0781e,0x20200b0,0x1e0781e0,0x781e0787,0x30381c07,0x1c07008,0x2008020,0x1c0881c0,
      0x701c0701,0xc0201c07,0x81e0781e,0x203c020,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x80000,0x801,0x4,0x40,0x0,0x0,0x0,0x1000,
      0x0,0x3c000000,0x0,0x0,0x0,0x0,0x10000,0x0,0x0,0x4004,0x1000000,0x0,0x0,0x80000,0x400000,0x0,0x20008000,0x0,0x4,0x1008,0x2000000,
      0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x80,0x0,0x8008000f,0x80000000,0x3e000,0x0,0x800,0xa0000400,0x0,0x0,0x0,0x0,0x80000,0x0,
      0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x100000,0x0,0x0,0x0,0x0,0x2000,0x0,0x4020040,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x80000,
      0x402,0x8,0x40,0x0,0x0,0x0,0x2000,0x0,0x0,0x0,0x0,0x0,0x0,0xc000,0x0,0x0,0x7004,0x70000fc,0x0,0x0,0x700000,0x800000,0x0,0x20008000,
      0x0,0x4,0x808,0x4000000,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x80,0x0,0x80f00000,0x0,0x0,0x0,0x800,0xa0001800,0x0,0x0,0x0,0x0,
      0x300000,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x600000,0x0,0x0,0x0,0x0,0x0,0x0,0x4020040
    };

    // Return a 'stringification' of standard integral types.
    const char* const bool_st    = "bool";
    const char* const uchar_st   = "unsigned char";
    const char* const char_st    = "char";
    const char* const ushort_st  = "unsigned short";
    const char* const short_st   = "short";
    const char* const uint_st    = "unsigned int";
    const char* const int_st     = "int";
    const char* const ulong_st   = "unsigned long";
    const char* const long_st    = "long";
    const char* const float_st   = "float";
    const char* const double_st  = "double";
    const char* const unknown_st = "unknown";
    template<typename t> inline const char* get_type(const t&) { return cimg::unknown_st; }
    inline const char* get_type(const bool&          ) { return cimg::bool_st;   }
    inline const char* get_type(const unsigned char& ) { return cimg::uchar_st;  }
    inline const char* get_type(const char&          ) { return cimg::char_st;   }
    inline const char* get_type(const unsigned short&) { return cimg::ushort_st; }
    inline const char* get_type(const short&         ) { return cimg::short_st;  }
    inline const char* get_type(const unsigned int&  ) { return cimg::uint_st;   }
    inline const char* get_type(const int&           ) { return cimg::int_st;    }
    inline const char* get_type(const unsigned long& ) { return cimg::ulong_st;  }
    inline const char* get_type(const long&          ) { return cimg::long_st;   }
    inline const char* get_type(const float&         ) { return cimg::float_st;  }
    inline const char* get_type(const double&        ) { return cimg::double_st; }
     								       
    inline int xln(const int x) {
      return x>0?(int)(1+std::log10((double)x)):1; 
    }
    
    inline char uncase(const char x) {
      return (char)((x<'A'||x>'Z')?x:x-'A'+'a'); 
    }
    
    // inline float atof(const char *str) {
    //   float x=0,y=1;
    //   if (!str) return 0; else { std::sscanf(str,"%g/%g",&x,&y); return x/y; }
    // }

    inline int strlen(const char *s) {
      if (s) { int k; for (k=0; s[k]; k++) ; return k; }
      return -1; 
    }

    inline int strncmp(const char *s1,const char *s2,const int l) {
      if (s1 && s2) { int n=0; for (int k=0; k<l; k++) n+=std::abs(s1[k] - s2[k]); return n; }
      return 0;
    }

    inline int strncasecmp(const char *s1,const char *s2,const int l) {
      if (s1 && s2) { int n=0; for (int k=0; k<l; k++) n+=std::abs(uncase(s1[k])-uncase(s2[k])); return n; }
      return 0;
    }
    
    inline int strcmp(const char *s1,const char *s2) { 
      const int l1 = cimg::strlen(s1), l2 = cimg::strlen(s2);
      return cimg::strncmp(s1,s2,1+(l1<l2?l1:l2));
    }

    inline int strcasecmp(const char *s1,const char *s2) { 
      const int l1 = cimg::strlen(s1), l2 = cimg::strlen(s2);
      return cimg::strncasecmp(s1,s2,1+(l1<l2?l1:l2));
    }

    inline int strfind(const char *s,const char c) {
      if (s) { 
        int l; for (l=cimg::strlen(s); l>=0 && s[l]!=c; l--) ;
        return l; 
      }
      return -1; 
    }

    inline const char *filename_split(const char *const filename, char *const body=0) {
      if (!filename) { if (body) body[0]='\0'; return 0; }
      int l = cimg::strfind(filename,'.');
      if (l>=0) { if (body) { std::strncpy(body,filename,l); body[l]='\0'; }}
      else { if (body) std::strcpy(body,filename); l=(int)std::strlen(filename)-1; }
      return filename+l+1;
    }
    
    inline std::FILE *fopen(const char *const path,const char *const mode) {
      if(!path || !mode) throw CImgArgumentException("cimg::fopen() : Can't open file '%s' with mode '%s'",path,mode);
      if (path[0]=='-') return (mode[0]=='r')?stdin:stdout; 
      else {
        std::FILE *dest = std::fopen(path,mode);
        if(!dest) throw CImgIOException("cimg::fopen() : File '%s' cannot be opened %s",
                                        path,mode[0]=='r'?"for reading":(mode[0]=='w'?"for writing":""),path);
        return dest;
      }
    }

    inline int fclose(std::FILE *file) {
      if (!file || file==stdin || file==stdout) return 0;
      const int errn=std::fclose(file);
      return errn;
    }
    template<typename T> inline int fread(T *ptr,const unsigned int nmemb,std::FILE *stream) {
      if (!ptr || nmemb<=0 || !stream)
        throw CImgArgumentException("cimg::fread() : Can't read %u x %u bytes of file pointer '%p' in buffer '%p'",
				    nmemb,sizeof(T),stream,ptr);
      const unsigned int errn = (unsigned int)std::fread((void*)ptr,sizeof(T),nmemb,stream);
      return errn;
    }
    template<typename T> inline int fwrite(const T *ptr,const unsigned int nmemb,std::FILE *stream) {
      if (!ptr || nmemb<=0 || !stream)
        throw CImgArgumentException("cimg::fwrite() : Can't write %u x %u bytes of file pointer '%p' from buffer '%p'",
				    nmemb,sizeof(T),stream,ptr);
      const unsigned int errn = (unsigned int)std::fwrite(ptr,sizeof(T),nmemb,stream);
      if(errn!=nmemb)
        throw CImgIOException("cimg::fwrite() : File writing problems, only %u/%u elements written",errn,nmemb);
      return errn;
    }
    
    // Exchange the values of variables \p a and \p b
    template<typename T> inline void swap(T& a,T& b) { T t=a; a=b; b=t; }
    template<typename T1,typename T2> inline void swap(T1& a1,T1& b1,T2& a2,T2& b2) {
      cimg::swap(a1,b1); cimg::swap(a2,b2); 
    }
    template<typename T1,typename T2,typename T3> inline void swap(T1& a1,T1& b1,T2& a2,T2& b2,T3& a3,T3& b3) {
      cimg::swap(a1,b1,a2,b2); cimg::swap(a3,b3); 
    }
    template<typename T1,typename T2,typename T3,typename T4>
    inline void swap(T1& a1,T1& b1,T2& a2,T2& b2,T3& a3,T3& b3,T4& a4,T4& b4) {
      cimg::swap(a1,b1,a2,b2,a3,b3); cimg::swap(a4,b4); 
    }
    template<typename T1,typename T2,typename T3,typename T4,typename T5>
    inline void swap(T1& a1,T1& b1,T2& a2,T2& b2,T3& a3,T3& b3,T4& a4,T4& b4,T5& a5,T5& b5) {
      cimg::swap(a1,b1,a2,b2,a3,b3,a4,b4); cimg::swap(a5,b5); 
    }
    template<typename T1,typename T2,typename T3,typename T4,typename T5,typename T6>
    inline void swap(T1& a1,T1& b1,T2& a2,T2& b2,T3& a3,T3& b3,T4& a4,T4& b4,T5& a5,T5& b5,T6& a6,T6& b6) {
      cimg::swap(a1,b1,a2,b2,a3,b3,a4,b4,a5,b5); cimg::swap(a6,b6);
    }
    
    template<typename T> inline void endian_swap(T* const buffer, const unsigned int size) {
      switch (sizeof(T)) {
      case 1: break;
      case 2: {
	for (unsigned short *ptr = (unsigned short*)buffer+size; ptr>(unsigned short*)buffer;) {
	  const unsigned short val = *(--ptr);
	  *ptr = (val>>8)|((val<<8));
	}
      } break;
      case 4: {
	for (unsigned int *ptr = (unsigned int*)buffer+size; ptr>(unsigned int*)buffer;) {
	  const unsigned int val = *(--ptr);
	  *ptr = (val>>24)|((val>>8)&0xff00)|((val<<8)&0xff0000)|(val<<24);
	}
      } break;
      default: {
	for (T* ptr = buffer+size; ptr>buffer; --ptr) {
	  unsigned char *pb=(unsigned char*)(--ptr), *pe=pb+sizeof(T);
	  for (int i=0; i<(int)sizeof(T)/2; i++) cimg::swap(*(pb++),*(--pe));
	} break;
      }
      }
    }
    template<typename T> inline T& endian_swap(T& a) { endian_swap(&a,1); return a; }
    
    //! Return \c false for little endian CPUs (Intel), \c true for big endian CPUs (Motorola).
      inline bool endian() { const int x=1; return reinterpret_cast<const unsigned char*>(&x)[0]?false:true; }

    
    //! Get the value of a system timer with a millisecond precision.
    inline unsigned long time() {
#if cimg_OS==1
      struct timeval st_time;
      gettimeofday(&st_time,0);
      return (unsigned long)(st_time.tv_usec/1000 + st_time.tv_sec*1000);
#elif cimg_OS==2
      static SYSTEMTIME st_time;
      GetSystemTime(&st_time);
      return (unsigned long)(st_time.wMilliseconds + 1000*(st_time.wSecond + 60*(st_time.wMinute + 60*st_time.wHour)));
#else 
      return 0;
#endif
    }

    //! Sleep for a certain numbers of milliseconds.
    /**
       This function frees the CPU ressources during the sleeping time.
       It may be used to temporize your program properly, without wasting CPU time.
       \sa wait(), time().
    **/
    inline void sleep(const unsigned int milliseconds) {
#if cimg_OS==1
      struct timespec tv;
      tv.tv_sec = milliseconds/1000;
      tv.tv_nsec = (milliseconds%1000)*1000000;
      nanosleep(&tv,0);
#elif cimg_OS==2
      Sleep(milliseconds);
#else
      (void)milliseconds;
#endif
    }

    inline unsigned int wait(const unsigned int milliseconds, unsigned long& timer) {
      if (!timer) timer = cimg::time();
      const unsigned long current_time = cimg::time();
      if (current_time>=timer+milliseconds) { timer = current_time; return 0; }
      const unsigned long time_diff = timer + milliseconds - current_time;
      timer = current_time + time_diff;
      cimg::sleep(time_diff);
      return (unsigned int)time_diff;
    }

    //! Wait for a certain number of milliseconds since the last call.
    /**
       This function is equivalent to sleep() but the waiting time is computed with regard to the last call
       of wait(). It may be used to temporize your program properly.
       \sa sleep(), time().
    **/
    inline unsigned int wait(const unsigned int milliseconds) {
      static unsigned long timer = 0;
      if (!timer) timer = cimg::time();
      return wait(milliseconds,timer);
    }

    template<typename T> inline const T rol(const T& a, const unsigned int n=1) {
      return (T)((a<<n)|(a>>((sizeof(T)<<3)-n))); 
    }
    
    template<typename T> inline const T ror(const T& a, const unsigned int n=1) {
      return (T)((a>>n)|(a<<((sizeof(T)<<3)-n))); 
    }

#if ( !defined(_MSC_VER) || _MSC_VER>1200 )
    //! Return the absolute value of \p a
    template<typename T> inline T abs(const T& a) { return a>=0?a:-a; }
    inline bool abs(const bool a) { return a; }
    inline unsigned char abs(const unsigned char a) { return a; }
    inline unsigned short abs(const unsigned short a) { return a; }
    inline unsigned int abs(const unsigned int a) { return a; }
    inline unsigned long abs(const unsigned long a) { return a; }
    inline double abs(const double a) { return std::fabs(a); }
    inline float abs(const float a)   { return (float)std::fabs((double)a); }
    inline int abs(const int a)       { return std::abs(a); }
    
    //! Return the minimum between \p a and \p b.
    template<typename T> inline const T& min(const T& a,const T& b) { return a<=b?a:b; }

    //! Return the minimum between \p a,\p b and \a c.
    template<typename T> inline const T& min(const T& a,const T& b,const T& c) { return cimg::min(cimg::min(a,b),c); }

    //! Return the minimum between \p a,\p b,\p c and \p d.
    template<typename T> inline const T& min(const T& a,const T& b,const T& c,const T& d) { return cimg::min(cimg::min(a,b,c),d); }

    //! Return the maximum between \p a and \p b.
    template<typename T> inline const T& max(const T& a,const T& b) { return a>=b?a:b; }

    //! Return the maximum between \p a,\p b and \p c.
    template<typename T> inline const T& max(const T& a,const T& b,const T& c) { return cimg::max(cimg::max(a,b),c); }

    //! Return the maximum between \p a,\p b,\p c and \p d.
    template<typename T> inline const T& max(const T& a,const T& b,const T& c,const T& d) { return cimg::max(cimg::max(a,b,c),d); }

    //! Return the sign of \p x.
    template<typename T> inline T sign(const T& x) { return (x<0)?(T)(-1):(x==0?(T)0:(T)1); }
#else
    
    // Special versions due to object reference bug in VisualC++ 6.0.
    template<typename T> inline const T abs(const T a) { return a>=0?a:-a; }
    template<typename T> inline const T min(const T a,const T b) { return a<=b?a:b; }
    template<typename T> inline const T min(const T a,const T b,const T c) { return cimg::min(cimg::min(a,b),c); }
    template<typename T> inline const T min(const T a,const T b,const T c,const T& d) { return cimg::min(cimg::min(a,b,c),d); }
    template<typename T> inline const T max(const T a,const T b) { return a>=b?a:b; }
    template<typename T> inline const T max(const T a,const T b,const T c) { return cimg::max(cimg::max(a,b),c); }
    template<typename T> inline const T max(const T a,const T b,const T c,const T& d) { return cimg::max(cimg::max(a,b,c),d); }
    template<typename T> inline char sign(const T x) { return (x<0)?-1:(x==0?0:1); }
#endif

    //! Return the nearest power of 2 higher than \p x.
    template<typename T> inline unsigned long nearest_pow2(const T& x) {
      unsigned long i=1;
      while (x>i) i<<=1;
      return i;
    }

    //! Return \p x modulo \p m (generic modulo).
    /**
       This modulo function accepts negative and floating-points modulo numbers \p m.
    **/
    inline double mod(const double& x,const double& m) { return x-m*std::floor(x/m); }
    inline float  mod(const float& x,const float& m)   { return (float)(x-m*std::floor((double)x/m)); }
    inline int    mod(const int x,const int m)         { return x>=0?x%m:(x%m?m+x%m:0); }

    //! Return minmod(\p a,\p b).
    /**
       The operator minmod(\p a,\p b) is defined to be :
       - minmod(\p a,\p b) = min(\p a,\p b), if (\p a * \p b)>0.
       - minmod(\p a,\p b) = 0,              if (\p a * \p b)<=0
    **/
    template<typename T> inline T minmod(const T& a,const T& b) { return a*b<=0?0:(a>0?(a<b?a:b):(a<b?b:a)); }

    //! Return a random variable between [0,1], followin a uniform distribution.
    inline double rand() { return (double)std::rand()/RAND_MAX; }

    //! Return a random variable between [-1,1], following a uniform distribution.
    inline double crand() { return 1-2*cimg::rand(); }

    //! Return a random variable following a gaussian distribution and a standard deviation of 1.
    inline double grand() {
      return std::sqrt(-2*std::log((double)(1e-10 + (1-2e-10)*cimg::rand())))*std::cos((double)(2*PI*cimg::rand())); 
    }

    inline double pythagore(double a, double b) {
      const double absa = cimg::abs(a), absb = cimg::abs(b);
      if (absa>absb) { const double tmp = absb/absa; return absa*std::sqrt(1.0+tmp*tmp); }
      else { const double tmp = absa/absb; return (absb==0?0:absb*std::sqrt(1.0+tmp*tmp)); }
    }
    
    // End of the 'cimg' namespace
  }
  
  /*
   #----------------------------------------
   #
   #
   #
   # Definition of the CImgStats structure
   #
   #
   #
   #----------------------------------------
   */
  //! Class used to compute basic statistics on pixel values of a \ref CImg<T> image.
  /** 
      Constructing a CImgStats instance from an image CImg<T> or a list CImgl<T>
      will compute the minimum, maximum and average pixel values of the input object.
      Optionally, the variance of the pixel values can be computed.
      Coordinates of the pixels whose values are minimum and maximum are also stored.
      The example below shows how to use CImgStats objects to retrieve simple statistics of an image :
      \code 
      const CImg<float> img("my_image.jpg");                 // Read JPEG image file.
      const CImgStats stats(img);                            // Compute basic statistics on the image.
      stats.print("My statistics");                          // Display statistics.
      std::printf("Max-Min = %lf",stats.max-stats.min);      // Compute the difference between extremum values.
      \endcode

      Note that statistics are computed by considering the set of \a scalar values of the image pixels.
      No vector-valued statistics are computed.
  **/
  struct CImgStats {
    double min;                 //!< Minimum of the pixel values.
    double max;                 //!< Maximum of the pixel values.
    double mean;                //!< Mean of the pixel values.
    double variance;            //!< Variance of the pixel values.
    int xmin;                   //!< X-coordinate of the pixel with minimum value.
    int ymin;                   //!< Y-coordinate of the pixel with minimum value.
    int zmin;                   //!< Z-coordinate of the pixel with minimum value.
    int vmin;                   //!< V-coordinate of the pixel with minimum value.
    int lmin;                   //!< Image number (for a list) containing the minimum pixel.
    int xmax;                   //!< X-coordinate of the pixel with maximum value.
    int ymax;                   //!< Y-coordinate of the pixel with maximum value.
    int zmax;                   //!< Z-coordinate of the pixel with maximum value.
    int vmax;                   //!< V-coordinate of the pixel with maximum value.
    int lmax;                   //!< Image number (for a list) containing the maximum pixel.
    
    //! Default constructor.
    CImgStats():min(0),max(0),mean(0),variance(0),xmin(-1),ymin(-1),zmin(-1),vmin(-1),lmin(-1),
		xmax(-1),ymax(-1),zmax(-1),vmax(-1),lmax(-1) {}

    //! In-place version of the default constructor
    CImgStats& assign() {
      min = max = mean = variance = 0;
      xmin = ymin = zmin = vmin = lmin = xmax = ymax = zmax = vmax = lmax = -1;
      return *this;
    }

    //! Copy constructor.
    CImgStats(const CImgStats& stats) {
      assign(stats);
    };

    //! In-place version of the copy constructor.
    CImgStats& assign(const CImgStats& stats) {
      min = stats.min;
      max = stats.max;
      mean = stats.mean;
      variance = stats.variance;
      xmin = stats.xmin; ymin = stats.ymin; zmin = stats.zmin; vmin = stats.vmin; lmin = stats.lmin;
      xmax = stats.xmax; ymax = stats.ymax; zmax = stats.zmax; vmax = stats.vmax; lmax = stats.lmax;
      return *this;
    }

    //! Constructor that computes statistics of an input image \p img.
    /** 
	\param img The input image.
	\param compute_variance If true, the \c variance field is computed, else it is set to 0.
    **/
    template<typename T> CImgStats(const CImg<T>& img, const bool compute_variance=true) {
      assign(img,compute_variance);
    }
      
    //! In-place version of the previous constructor.
    template<typename T> CImgStats& assign(const CImg<T>& img, const bool compute_variance=true) {
      if (img.is_empty())
	throw CImgArgumentException("CImgStats::CImgStats() : Specified input image (%u,%u,%u,%u,%p) is empty.",
				    img.width,img.height,img.depth,img.dim,img.data);
      mean = variance = 0;
      lmin = lmax = -1;
      T pmin=img[0], pmax=pmin, *ptrmin=img.data, *ptrmax=ptrmin;
      cimg_map(img,ptr,T) {
	const T& a=*ptr;
	mean+=(double)a;
	if (a<pmin) { pmin=a; ptrmin = ptr; }
	if (a>pmax) { pmax=a; ptrmax = ptr; }
      }
      mean/=img.size();
      min=(double)pmin;
      max=(double)pmax;
      unsigned long offmin = (unsigned long)(ptrmin-img.data), offmax = (unsigned long)(ptrmax-img.data);
      const unsigned long whz = img.width*img.height*img.depth, wh = img.width*img.height;      
      vmin = offmin/whz; offmin%=whz; zmin = offmin/wh; offmin%=wh; ymin = offmin/img.width; xmin = offmin%img.width;
      vmax = offmax/whz; offmax%=whz; zmax = offmax/wh; offmax%=wh; ymax = offmax/img.width; xmax = offmax%img.width;
      if (compute_variance) {
        cimg_map(img,ptr,T) { const double tmpf=(*ptr)-mean; variance+=tmpf*tmpf; }
	const unsigned int siz = img.size();
        if (siz>1) variance/=(siz-1); else variance=0;
      }
      return *this;
    }

    //! Constructor that computes statistics of an input image list \p list.
    /**
       \param list The input list of images.
       \param compute_variance If true, the \c variance field is computed, else it is set to 0.
    **/
    template<typename T> CImgStats(const CImgl<T>& list, const bool compute_variance=true) {
      assign(list,compute_variance);
    }

    //! In-place version of the previous constructor.
    template<typename T> CImgStats& assign(const CImgl<T>& list,const bool compute_variance=true) {
      if (list.is_empty())
	throw CImgArgumentException("CImgStats::CImgStats() : Specified input list (%u,%p) is empty.",
				    list.size,list.data);
      mean = variance = lmin = lmax = 0;
      T pmin = list[0][0], pmax = pmin, *ptrmin = list[0].data, *ptrmax = ptrmin;
      int psize = 0;
      cimgl_map(list,l) {
        cimg_map(list[l],ptr,T) {
          const T& a=*ptr;
          mean+=(double)a;
          if (a<pmin) { pmin=a; ptrmin = ptr; lmin = l; }
          if (a>pmax) { pmax=a; ptrmax = ptr; lmax = l; }
        }
        psize+=list[l].size();
      }
      mean/=psize;
      min=(double)pmin;
      max=(double)pmax;
      const CImg<T> &imin = list[lmin], &imax = list[lmax];
      unsigned long offmin = (ptrmin-imin.data), offmax = (ptrmax-imax.data);
      const unsigned long whz1 = imin.width*imin.height*imin.depth, wh1 = imin.width*imin.height;
      vmin = offmin/whz1; offmin%=whz1; zmin = offmin/wh1; offmin%=wh1; ymin = offmin/imin.width; xmin = offmin%imin.width;
      const unsigned long whz2 = imax.width*imax.height*imax.depth, wh2 = imax.width*imax.height;
      vmax = offmax/whz2; offmax%=whz2; zmax = offmax/wh2; offmax%=wh2; ymax = offmax/imax.width; xmax = offmax%imax.width;
      if (compute_variance) {
        cimgl_map(list,l) cimg_map(list[l],ptr,T) { const double tmpf=(*ptr)-mean; variance+=tmpf*tmpf; }
	if (psize>1) variance/=(psize-1); else variance=0;
      }
      return *this;
    }

    //! Assignement operator.
    CImgStats& operator=(const CImgStats& stats) {
      return assign(stats);
    }

  };

  /*
   #-------------------------------------------
   #
   #
   #
   # Definition of the CImgDisplay structure
   #
   #
   #
   #-------------------------------------------
   */

  //! This class represents a window which can display \ref CImg<T> images and handles mouse and keyboard events.
  /**
     Creating a \c CImgDisplay instance opens a window that can be used to display a \c CImg<T> image
     of a \c CImgl<T> image list inside. When a display is created, associated window events
     (such as mouse motion, keyboard and window size changes) are handled and can be easily
     detected by testing specific \c CImgDisplay data fields.
     See \ref cimg_displays for a complete tutorial on using the \c CImgDisplay class.
  **/

  struct CImgDisplay {
    
    //! Width of the display
    unsigned int width;
    
    //! Height of the display
    unsigned int height;

    //! Normalization type used for the display
    unsigned int normalization;

    //! Range of events detected by the display
    unsigned int events;

    //! Fullscreen state of the display
    bool is_fullscreen;

    //! Display title
    char* title;
    
    //! X-pos of the display on the screen
    volatile int window_x;
    
    //! Y-pos of the display on the screen
    volatile int window_y;

    //! Width of the underlying window
    volatile unsigned int window_width;

    //! Height of the underlying window
    volatile unsigned int window_height;

    //! X-coordinate of the mouse pointer on the display
    volatile int mouse_x;
    
    //! Y-coordinate of the mouse pointer on the display
    volatile int mouse_y;

    //! Button state of the mouse
    volatile unsigned int button;
    //! Wheel state of the mouse (Linux only)
    volatile int wheel;

    //! Key value if pressed
    volatile unsigned int key;

    //! Closed state of the window
    volatile bool is_closed;
    
    //! Resized state of the window
    volatile bool is_resized;
    
    //! Moved state of the window
    volatile bool is_moved;
    
    //! Event state of the window
    volatile bool is_event;
    
    double min,max;
    unsigned long timer, fps_frames, fps_timer;
    float fps_fps;

    //! Create a display window with a specified size \p pwidth x \p height.
    /** \param dimw       : Width of the display window.
        \param dimh       : Height of the display window.
        \param title      : Title of the display window.
        \param normalization_type  : Normalization type of the display window (see CImgDisplay::normalize).
	\param events_type : Type of events handled by the display window.
	\param fullscreen_flag : Fullscreen mode.
	\param closed_flag : Initially visible mode.      
        A black image will be initially displayed in the display window.
    **/
    CImgDisplay(const unsigned int dimw, const unsigned int dimh, const char *title=0,
                const unsigned int normalization_type=1, const unsigned int events_type=3,
                const bool fullscreen_flag=false, const bool closed_flag=false):
      width(0),height(0),normalization(0),events(0),is_fullscreen(false),title(0),
      window_x(0),window_y(0),window_width(0),window_height(0),
      mouse_x(0),mouse_y(0),button(0),wheel(0),key(0),is_closed(true),is_resized(false),is_moved(false),is_event(false),
      min(0),max(0) {
      assign(dimw,dimh,title,normalization_type,events_type,fullscreen_flag,closed_flag);
    }

    //! Create a display window from an image.
    /** \param img : Image that will be used to create the display window.
        \param title : Title of the display window
        \param normalization_type : Normalization type of the display window.
        \param events_type : Type of events handled by the display window.
	\param fullscreen_flag : Fullscreen mode.
	\param closed_flag : Initially visible mode.      
    **/
    template<typename T> 
    CImgDisplay(const CImg<T>& img,const char *title=0,
                const unsigned int normalization_type=1,const unsigned int events_type=3,
                const bool fullscreen_flag=false,const bool closed_flag=false):
      width(0),height(0),normalization(0),events(0),is_fullscreen(false),title(0),
      window_x(0),window_y(0),window_width(0),window_height(0),
      mouse_x(0),mouse_y(0),button(0),wheel(0),key(0),is_closed(true),is_resized(false),is_moved(false),is_event(false),
      min(0),max(0) {
      assign(img,title,normalization_type,events_type,fullscreen_flag,closed_flag);
    }
   
    //! Create a display window from an image list.
    /** \param list : The list of images to display.
        \param title : Title of the display window
        \param normalization_type : Normalization type of the display window.
	\param events_type : Type of events handled by the display window.
	\param fullscreen_flag : Fullscreen mode.
	\param closed_flag : Initially visible mode.      
    **/
    template<typename T> 
    CImgDisplay(const CImgl<T>& list,const char *title=0,
                const unsigned int normalization_type=1,const unsigned int events_type=3,
                const bool fullscreen_flag=false,const bool closed_flag=false):
      width(0),height(0),normalization(0),events(0),is_fullscreen(false),title(0),
      window_x(0),window_y(0),window_width(0),window_height(0),
      mouse_x(0),mouse_y(0),button(0),wheel(0),key(0),is_closed(true),is_resized(false),is_moved(false),is_event(false),
      min(0),max(0) {
      assign(list,title,normalization_type,events_type,fullscreen_flag,closed_flag);
    }

    //! Create a display window by copying another one.
    /** \param win   : Display window to copy.
        \param title : Title of the new display window.
    **/
    CImgDisplay(const CImgDisplay& disp):
      width(0),height(0),normalization(0),events(0),is_fullscreen(false),title(0),
      window_x(0),window_y(0),window_width(0),window_height(0),
      mouse_x(0),mouse_y(0),button(0),wheel(0),key(0),is_closed(true),is_resized(false),is_moved(false),is_event(false),
      min(0),max(0) {
      assign(disp);
    }

    //! Destructor
    ~CImgDisplay() { 
      assign(); 
    }

    //! Assignement operator
    CImgDisplay& operator=(const CImgDisplay& disp) {
      return assign(disp);
    }

    //! Return display width
    int dimx() const { 
      return (int)width; 
    }

    //! Return display height
    int dimy() const {
      return (int)height; 
    }

    //! Return display window width
    int window_dimx() const {
      return (int)window_width; 
    }

    //! Return display window height
    int window_dimy() const {
      return (int)window_height; 
    }   
    
    //! Return X-coordinate of the window
    int window_posx() const {
      return window_x;
    }
    
    //! Return Y-coordinate of the window
    int window_posy() const {
      return window_y;
    }
    
    //! Synchronized waiting function. Same as cimg::wait().
    /** \see cimg::wait()
     **/
    CImgDisplay& wait(const unsigned int milliseconds) { 
      cimg::wait(milliseconds, timer);
      return *this; 
    }

    //! Wait for an event
    CImgDisplay& wait() {
      wait(*this);
      return *this;
    }

    static void wait(CImgDisplay& disp1) {
      disp1.is_event = 0;
      while (!disp1.is_event) wait_all();
    }

    static void wait(CImgDisplay& disp1, CImgDisplay& disp2) {
      disp1.is_event = disp2.is_event = 0;
      while (!disp1.is_event && !disp2.is_event) wait_all();
    }

    static void wait(CImgDisplay& disp1, CImgDisplay& disp2, CImgDisplay& disp3) {
      disp1.is_event = disp2.is_event = disp3.is_event = 0;
      while (!disp1.is_event && !disp2.is_event && !disp3.is_event) wait_all();
    }

    static void wait(CImgDisplay& disp1, CImgDisplay& disp2, CImgDisplay& disp3, CImgDisplay& disp4) {
      disp1.is_event = disp2.is_event = disp3.is_event = disp4.is_event = 0;
      while (!disp1.is_event && !disp2.is_event && !disp3.is_event && !disp4.is_event) wait_all();
    }

      //! Display an image list CImgl<T> into a display window.
    /** First, all images of the list are appended into a single image used for visualization,
        then this image is displayed in the current display window.
        \param list     : The list of images to display.
        \param axe      : The axe used to append the image for visualization. Can be 'x' (default),'y','z' or 'v'.
        \param align : Defines the relative alignment of images when displaying images of different sizes.
        Can be '\p c' (centered, which is the default), '\p p' (top alignment) and '\p n' (bottom aligment).

        \see CImg::get_append()
    **/
    template<typename T> CImgDisplay& display(const CImgl<T>& list,const char axe='x',const char align='c') { 
      return display(list.get_append(axe,align)); 
    } 

    //! Resize a display window with the size of an image.
    /** \param img    : Input image. \p image.width and \p image.height give the new dimensions of the display window.
        \param redraw : If \p true (default), the current displayed image in the display window will
        be bloc-interpolated to fit the new dimensions. If \p false, a black image will be drawn in the resized window.
        \see CImgDisplay::is_resized, CImgDisplay::resizedimx(), CImgDisplay::resizedimy()
    **/
    template<typename T> CImgDisplay& resize(const CImg<T>& img, const bool redraw=true) { 
      return resize(img.width,img.height,redraw); 
    }

    //! Resize a display window using the size of the given display \p disp
    CImgDisplay& resize(const CImgDisplay& disp, const bool redraw=true) {
      return resize(disp.width,disp.height,redraw);
    }

    //! Resize a display window in its current size.
    CImgDisplay& resize(const bool redraw=true) {
      resize(window_width,window_height,redraw);
      return *this;
    }

    //! Toggle fullscreen mode
    CImgDisplay& toggle_fullscreen() {
      return assign(width,height,title,normalization,events,!is_fullscreen,is_closed);
    }

    // Inner routine used for fast resizing of buffer to display size.
    template<typename t, typename T> static void _render_resize(const T *ptrs, const unsigned int ws, const unsigned int hs,
								t *ptrd, const unsigned int wd, const unsigned int hd) {
      unsigned int *const offx = new unsigned int[wd], *const offy = new unsigned int[hd+1], *poffx, *poffy;
      float s, curr, old;
      s = (float)ws/wd;
      poffx = offx; curr=0; for (unsigned int x=0; x<wd; x++) { old=curr; curr+=s; *(poffx++) = (unsigned int)curr-(unsigned int)old; }
      s = (float)hs/hd;
      poffy = offy; curr=0; for (unsigned int y=0; y<hd; y++) { old=curr; curr+=s; *(poffy++) = ws*((unsigned int)curr-(unsigned int)old); }
      *poffy=0;
      poffy = offy;
      {for (unsigned int y=0; y<hd; ) {
	const T *ptr = ptrs;
	poffx = offx;
	for (unsigned int x=0; x<wd; x++) { *(ptrd++)=*ptr; ptr+=*(poffx++); }
	y++;
	unsigned int dy=*(poffy++);
	for (;!dy && y<hd; std::memcpy(ptrd, ptrd-wd, sizeof(t)*wd), y++, ptrd+=wd, dy=*(poffy++)) ;
	ptrs+=dy;
      }}
      delete[] offx; delete[] offy;
    }
    
    // When no display available
    //---------------------------
#if cimg_display_type==0

    //! Return the width of the screen resolution.
    static int screen_dimx() {
      return 0; 
    }

    //! Return the height of the screen resolution.
    static int screen_dimy() { 
      return 0; 
    }

    //! In-place version of the previous constructor
    CImgDisplay& assign(const unsigned int dimw, const unsigned int dimh, const char *title=0,
			const unsigned int normalization_type=1, const unsigned int events_type=3,
			const bool fullscreen_flag=false, const bool closed_flag=false) {
      throw CImgDisplayException("CImgDisplay() : Display has been required but is not available (cimg_display_type=0)");
      fps_timer = 0*(unsigned long)(dimw + dimh + title + normalization_type + events_type + (int)fullscreen_flag + (int)closed_flag);
      return *this;
    }

    //! In-place version of the previous constructor
    template<typename T> CImgDisplay& assign(const CImg<T>& img, const char *title=0,
					     const unsigned int normalization_type=1, const unsigned int events_type=3,
					     const bool fullscreen_flag=false, const bool closed_flag=false) {
      fps_timer = 0*(unsigned long)(img.width + title + normalization_type + events_type + (int)fullscreen_flag + (int)closed_flag);
      return assign(0,0);
    }
    
    //! In-place version of the previous constructor
    template<typename T> CImgDisplay& assign(const CImgl<T>& list, const char *title=0,
					     const unsigned int normalization_type=1, const unsigned int events_type=3,
					     const bool fullscreen_flag=false, const bool closed_flag=false) {
      fps_timer = 0*(unsigned long)(list.size + title + normalization_type + events_type + (int)fullscreen_flag + (int)closed_flag);
      return assign(0,0);
    }
    
    //! In-place version of the previous constructor
    CImgDisplay& assign(const CImgDisplay &disp) {
      return assign(disp.width,disp.height);
    }

    // In-place version of the destructor (should not be used by the user).
    CImgDisplay& assign() {
      return *this;
    }
    
    //! Display an image in a window.
    template<typename T> CImgDisplay& display(const CImg<T>& img) {
      fps_timer = 0*img.width;
      return *this; 
    }

    //! Resize window
    CImgDisplay& resize(const int width, const int height, const bool redraw=true) {
      fps_timer = 0*width*height*(int)redraw;
      return *this; 
    }

    //! Move window
    CImgDisplay& move(const int posx, const int posy) { 
      fps_timer = 0*posx*posy;
      return *this; 
    }

    //! Wait for a window event in any CImg window
    static void wait_all() {}

    //! Show a closed display
    CImgDisplay& show() {
      return *this; 
    }

    //! Close a visible display
    CImgDisplay& close() { 
      return *this; 
    }

    //! Re-paint image content in window
    CImgDisplay& paint() {
      return *this;
    }

    //! Render image buffer into GDI native image format
    template<typename T> CImgDisplay& render(const CImg<T>& img) {
      fps_timer = 0*img.width;
      return *this;
    }
    
    // X11-based display
    //-------------------
#elif cimg_display_type==1
    void *data;
    Window window;
    Window background_window;
    XImage *image;
    Colormap colormap;
    Atom wm_delete_window, wm_delete_protocol;
#ifdef cimg_use_xshm
    XShmSegmentInfo *shminfo;
#endif

    static int screen_dimx() { 
      int res = 0;
      if (!cimg::X11attr().display) {
	Display *disp = XOpenDisplay((std::getenv("DISPLAY") ? std::getenv("DISPLAY") : ":0.0"));
	if (!disp) throw CImgDisplayException("CImgDisplay::screen_dimx() : Can't open X11 display");
	res = DisplayWidth(disp,DefaultScreen(disp));
	XCloseDisplay(disp);
      } else {
#ifdef cimg_use_xrandr
	if (cimg::X11attr().resolutions && cimg::X11attr().curr_resolution)
	  res = cimg::X11attr().resolutions[cimg::X11attr().curr_resolution].width;
	else
#endif
	  res = DisplayWidth(cimg::X11attr().display,DefaultScreen(cimg::X11attr().display));
      }
      return res;
    }

    static int screen_dimy() { 
      int res = 0;
      if (!cimg::X11attr().display) {
	Display *disp = XOpenDisplay((std::getenv("DISPLAY") ? std::getenv("DISPLAY") : ":0.0"));
	if (!disp) throw CImgDisplayException("CImgDisplay::screen_dimy() : Can't open X11 display");
	res = DisplayHeight(disp,DefaultScreen(disp));
	XCloseDisplay(disp);
      } else {
#ifdef cimg_use_xrandr
	if (cimg::X11attr().resolutions && cimg::X11attr().curr_resolution)
	  res = cimg::X11attr().resolutions[cimg::X11attr().curr_resolution].height; else
#endif
	    res = DisplayHeight(cimg::X11attr().display,DefaultScreen(cimg::X11attr().display));
      }
      return res;
    }
    
    CImgDisplay& assign(const unsigned int dimw, const unsigned int dimh, const char *title=0,
			const unsigned int normalization_type=1, const unsigned int events_type=3,
			const bool fullscreen_flag=false, const bool closed_flag=false) {
      if (!dimw || !dimh) 
	throw CImgArgumentException("CImgDisplay::assign() : Specified window size (%u,%u) is not valid.",dimw,dimh);
      assign_lowlevel(dimw,dimh,title,normalization_type,events_type,fullscreen_flag,closed_flag);
      min = max = 0;
      std::memset(data,0,(cimg::X11attr().nb_bits==8?sizeof(unsigned char):
			  (cimg::X11attr().nb_bits==16?sizeof(unsigned short):sizeof(unsigned int)))*width*height);
      return paint();
    }
    
    template<typename T> CImgDisplay& assign(const CImg<T>& img, const char *title=0,
					     const unsigned int normalization_type=1, const unsigned int events_type=3,
					     const bool fullscreen_flag=false, const bool closed_flag=false) {
      if (img.is_empty())
	throw CImgArgumentException("CImgDisplay::CImgDisplay() : Specified input image (%u,%u,%u,%u,%p) is empty.",
				    img.width,img.height,img.depth,img.dim,img.data);
      CImg<T> tmp;
      const CImg<T>& nimg = (img.depth==1)?img:(tmp=img.get_projections2d(img.width/2,img.height/2,img.depth/2));
      assign_lowlevel(nimg.width,nimg.height,title,normalization_type,events_type,fullscreen_flag,closed_flag);
      if (normalization==2) { const CImgStats st(nimg,false); min=st.min; max=st.max; }
      return render(nimg).paint();
    }
    
    template<typename T> CImgDisplay& assign(const CImgl<T>& list, const char *title=0,
					     const unsigned int normalization_type=1, const unsigned int events_type=3,
					     const bool fullscreen_flag=false, const bool closed_flag=false) {
      if (list.is_empty())
	throw CImgArgumentException("CImgDisplay::CImgDisplay() : Specified input list (%u,%p) is empty.",
				    list.size,list.data);
      CImg<T> tmp;
      const CImg<T> img = list.get_append('x'), 
	&nimg = (img.depth==1)?img:(tmp=img.get_projections2d(img.width/2,img.height/2,img.depth/2));
      assign_lowlevel(nimg.width,nimg.height,title,normalization_type,events_type,fullscreen_flag,closed_flag);
      if (normalization==2) { const CImgStats st(nimg,false); min=st.min; max=st.max; }
      return render(nimg).paint();
    }
    
    CImgDisplay& assign(const CImgDisplay& win) {
      assign_lowlevel(win.width,win.height,win.title,win.normalization,win.events,win.is_fullscreen,win.is_closed);
      std::memcpy(data,win.data,(cimg::X11attr().nb_bits==8?sizeof(unsigned char):
				 cimg::X11attr().nb_bits==16?sizeof(unsigned short):
				 sizeof(unsigned int))*width*height);
      return paint();
    }

    CImgDisplay& assign() {
      if (width && height) {
	pthread_mutex_lock(cimg::X11attr().mutex);
	
	// Remove display window from event thread list
	unsigned int i;
	for (i=0; i<cimg::X11attr().nb_wins && cimg::X11attr().wins[i]!=this; i++);
	for (; i<cimg::X11attr().nb_wins-1; i++) cimg::X11attr().wins[i]=cimg::X11attr().wins[i+1];
	cimg::X11attr().nb_wins--;
	
	// Destroy window, image, colormap and title
	if (is_fullscreen) _desinit_fullscreen();
	XDestroyWindow(cimg::X11attr().display,window);
	window = 0;
#ifdef cimg_use_xshm
	if (shminfo) {
	  XShmDetach(cimg::X11attr().display, shminfo);
	  XDestroyImage(image);
	  shmdt(shminfo->shmaddr);
	  shmctl(shminfo->shmid,IPC_RMID,0);
	  delete shminfo;
	  shminfo = 0;
	} else
#endif
	  XDestroyImage(image);
	data = 0;
	image = 0;
	if (cimg::X11attr().nb_bits==8) XFreeColormap(cimg::X11attr().display,colormap);
	colormap = 0;
	XSync(cimg::X11attr().display, False);

	// Reset display variables
	if (title) delete[] title;
	width = height = normalization = events = 0;
	is_fullscreen = false;
	is_resized = is_moved = false;
	is_event = false;
	is_closed = true;
	title = 0;
	window_x = window_y = window_width = window_height = mouse_x = mouse_y = button = wheel = key = 0;
	min = max = 0;
	
	// End event thread and close display if necessary
	if (!cimg::X11attr().nb_wins) {
	  
	  // Kill event thread
	  pthread_cancel(*cimg::X11attr().event_thread);
	  pthread_mutex_unlock(cimg::X11attr().mutex);
	  pthread_join(*cimg::X11attr().event_thread,0);
	  delete cimg::X11attr().event_thread;
	  cimg::X11attr().event_thread = 0;
	  pthread_mutex_destroy(cimg::X11attr().mutex);
	  delete cimg::X11attr().mutex;
	  cimg::X11attr().mutex = 0;	  
	  XSync(cimg::X11attr().display, False);
	  XCloseDisplay(cimg::X11attr().display);
	  cimg::X11attr().display=0;
	  delete cimg::X11attr().gc;
	  cimg::X11attr().gc = 0;
	} else pthread_mutex_unlock(cimg::X11attr().mutex);
      }
      return *this;
    }
    
    template<typename T> CImgDisplay& display(const CImg<T>& img) {
      return render(img).paint(false);
    }
    
    CImgDisplay& resize(const int nwidth, const int nheight, const bool redraw=true) {
      if (!(nwidth && nheight))
	throw CImgArgumentException("CImgDisplay::resize() : Specified window size (%d,%d) is not valid.",
				    nwidth,nheight);
      const unsigned int
	tmpdimx=(nwidth>0)?nwidth:(-nwidth*width/100),
	tmpdimy=(nheight>0)?nheight:(-nheight*height/100),
	dimx = tmpdimx?tmpdimx:1,
	dimy = tmpdimy?tmpdimy:1;
      const bool
	is_disp_different = (width!=dimx || height!=dimy),
	is_win_different = (window_width!=dimx || window_height!=dimy);
      if (is_disp_different || is_win_different) {
	pthread_mutex_lock(cimg::X11attr().mutex);
	XResizeWindow(cimg::X11attr().display,window,dimx,dimy);
	window_width = dimx;
	window_height = dimy;
	is_resized = false;
	if (is_disp_different) {
	  switch (cimg::X11attr().nb_bits) {
	  case 8:  { unsigned char foo=0; _resize(foo,dimx,dimy,redraw);  } break;
	  case 16: { unsigned short foo=0; _resize(foo,dimx,dimy,redraw); } break;
	  default: { unsigned int foo=0; _resize(foo,dimx,dimy,redraw);   } break;
	  }
	  width = dimx;
	  height = dimy;
	}
	pthread_mutex_unlock(cimg::X11attr().mutex);
	if (is_fullscreen) move((screen_dimx()-width)/2,(screen_dimy()-height)/2);
	if (redraw) return paint();
      }      
      return *this;
    }
    
    CImgDisplay& move(const int posx, const int posy) {
      show();
      pthread_mutex_lock(cimg::X11attr().mutex);      
      XMoveWindow(cimg::X11attr().display,window,posx,posy);
      is_moved = false;
      window_x = posx;
      window_y = posy;
      pthread_mutex_unlock(cimg::X11attr().mutex);
      return paint();
    }
    
    static void wait_all() {
      pthread_mutex_lock(cimg::X11attr().mutex);
      bool flag = true;
      XEvent event;
      while (flag) {
	for (unsigned int i=0; i<cimg::X11attr().nb_wins; i++) {
	  cimg::X11attr().wins[i]->is_event = false;
	  const unsigned int xevent_type = (cimg::X11attr().wins[i]->events)&3;
	  const unsigned int emask =
	    ((xevent_type>=1)?ExposureMask|StructureNotifyMask:0)|
	    ((xevent_type>=2)?ButtonPressMask|KeyPressMask|PointerMotionMask|LeaveWindowMask:0)|
	    ((xevent_type>=3)?ButtonReleaseMask|KeyReleaseMask:0);
	  XSelectInput(cimg::X11attr().display,cimg::X11attr().wins[i]->window,emask);	
	}
	XNextEvent(cimg::X11attr().display, &event);
	for (unsigned int i=0; i<cimg::X11attr().nb_wins; i++)
	  if (!cimg::X11attr().wins[i]->is_closed && event.xany.window==cimg::X11attr().wins[i]->window) {
	    cimg::X11attr().wins[i]->_handle_events(&event);
	    if (cimg::X11attr().wins[i]->is_event) flag = false;
	  }
      }
      pthread_mutex_unlock(cimg::X11attr().mutex);
    }
 
    CImgDisplay& show() {
      if (is_closed) {
        pthread_mutex_lock(cimg::X11attr().mutex);
	if (is_fullscreen) _init_fullscreen();
	_map_window();
        is_closed = false;
        pthread_mutex_unlock(cimg::X11attr().mutex);
      }
      return paint();	
    }

    CImgDisplay& close() {
      if (!is_closed) {
        pthread_mutex_lock(cimg::X11attr().mutex);
	if (is_fullscreen) _desinit_fullscreen();
        XUnmapWindow(cimg::X11attr().display,window);
	window_x = window_y = -1;
        is_closed = true;
        pthread_mutex_unlock(cimg::X11attr().mutex);
      }
      return *this;
    }

    CImgDisplay& set_title(const char *format,...) { 
      char tmp[1024]={0}; 
      va_list ap; 
      va_start(ap, format); 
      std::vsprintf(tmp,format,ap); 
      va_end(ap);
      if (title) delete[] title;
      const int s = cimg::strlen(tmp)+1;
      title = new char[s];
      std::memcpy(title,tmp,s*sizeof(char));
      pthread_mutex_lock(cimg::X11attr().mutex);
      XStoreName(cimg::X11attr().display,window,tmp);
      pthread_mutex_unlock(cimg::X11attr().mutex);
      return *this; 
    }

    CImgDisplay& paint(const bool wait_expose=true) {      
      pthread_mutex_lock(cimg::X11attr().mutex);
      _paint(wait_expose);
      pthread_mutex_unlock(cimg::X11attr().mutex);	  
      return *this;
    }

    template<typename T> CImgDisplay& render(const CImg<T>& img, const bool flag8=false) {
      if (img.is_empty())
	throw CImgArgumentException("CImgDisplay::_render_image() : Specified input image (%u,%u,%u,%u,%p) is empty.",
				    img.width,img.height,img.depth,img.dim,img.data);
      if (img.depth!=1) return render(img.get_projections2d(img.width/2,img.height/2,img.depth/2)); 
      if (cimg::X11attr().nb_bits==8 && (img.width!=width || img.height!=height)) return render(img.get_resize(width,height,1,-100,1));
      if (cimg::X11attr().nb_bits==8 && !flag8 && img.dim==3) return render(img.get_RGBtoLUT(true),true);

      const unsigned int xymax = img.width*img.height;
      const T 
	*data1 = img.ptr(),
	*data2 = (img.dim>=2)?img.ptr(0,0,0,1):data1,
	*data3 = (img.dim>=3)?img.ptr(0,0,0,2):data1;
      if (cimg::X11attr().blue_first) cimg::swap(data1,data3);
      pthread_mutex_lock(cimg::X11attr().mutex);
      
      if (!normalization) {
	switch (cimg::X11attr().nb_bits) {
	case 8: {
	  _set_colormap(colormap,img.dim);
	  unsigned char *const ndata = (img.width==width && img.height==height)?(unsigned char*)data:new unsigned char[img.width*img.height];
	  unsigned char *ptrd = (unsigned char*)ndata;
	  switch (img.dim) {
	  case 1: for (unsigned int xy=0; xy<xymax; xy++) (*ptrd++) = (unsigned char)*(data1++);
	    break;
	  case 2: for (unsigned int xy=0; xy<xymax; xy++) {
	    const unsigned char R = (unsigned char)*(data1++), G = (unsigned char)*(data2++);
	    (*ptrd++) = (R&0xf0)|(G>>4);
	  } break;
	  default: for (unsigned int xy=0; xy<xymax; xy++) {
	    const unsigned char R = (unsigned char)*(data1++), G = (unsigned char)*(data2++), B = (unsigned char)*(data3++);
	    (*ptrd++) = (R&0xe0) | ((G>>5)<<2) | (B>>6);
	  } break;
	  }
	  if (ndata!=data) { _render_resize(ndata,img.width,img.height,(unsigned char*)data,width,height); delete[] ndata; }	
	} break;
	case 16: {
	  unsigned short *const ndata = (img.width==width && img.height==height)?(unsigned short*)data:new unsigned short[img.width*img.height];
	  unsigned char *ptrd = (unsigned char*)ndata;
	  const unsigned int M = 248;
	  if (cimg::X11attr().byte_order) for (unsigned int xy=0; xy<xymax; xy++) {
	    const unsigned char G = (unsigned char)*(data2++)>>2;
	    *(ptrd++) = ((unsigned char)*(data1++)&M) | (G>>3);
	    *(ptrd++) = (G<<5) | ((unsigned char)*(data3++)>>3);
	  } else for (unsigned int xy=0; xy<xymax; xy++) {
	    const unsigned char G = (unsigned char)*(data2++)>>2;
	    *(ptrd++) = (G<<5) | ((unsigned char)*(data3++)>>3);
	    *(ptrd++) = ((unsigned char)*(data1++)&M) | (G>>3);
	  }
	  if (ndata!=data) { _render_resize(ndata,img.width,img.height,(unsigned short*)data,width,height); delete[] ndata; } 
	} break;
	default: {
	  unsigned int *const ndata = (img.width==width && img.height==height)?(unsigned int*)data:new unsigned int[img.width*img.height];
	  unsigned char *ptrd = (unsigned char*)ndata;
	  if (cimg::X11attr().byte_order) for (unsigned int xy=0; xy<xymax; xy++) {
	    *(ptrd++) = 0;
	    *(ptrd++) = (unsigned char)*(data1++);
	    *(ptrd++) = (unsigned char)*(data2++);
	    *(ptrd++) = (unsigned char)*(data3++);
	  } else for (unsigned int xy=0; xy<xymax; xy++) {
	    *(ptrd++) = (unsigned char)*(data3++);
	    *(ptrd++) = (unsigned char)*(data2++);
	    *(ptrd++) = (unsigned char)*(data1++);
	    *(ptrd++) = 0;
	  }	      
	  if (ndata!=data) { _render_resize(ndata,img.width,img.height,(unsigned int*)data,width,height); delete[] ndata; } 
	} break;
	};
      } else {
	if ((min>max) || normalization==1) { const CImgStats st(img,false); min=st.min; max=st.max; }
	const T nmin = (T)min, delta = (T)max-nmin, mm=delta?delta:(T)1;
	switch (cimg::X11attr().nb_bits) {
	case 8: {
	  _set_colormap(colormap,img.dim);
	  unsigned char *const ndata = (img.width==width && img.height==height)?(unsigned char*)data:new unsigned char[img.width*img.height];
	  unsigned char *ptrd = (unsigned char*)ndata;
	  switch (img.dim) {
	  case 1: for (unsigned int xy=0; xy<xymax; xy++) {
	    const unsigned char R = (unsigned char)(255*(*(data1++)-nmin)/mm);
	    *(ptrd++) = R;
	  } break;
	  case 2: for (unsigned int xy=0; xy<xymax; xy++) {
	    const unsigned char
	      R = (unsigned char)(255*(*(data1++)-nmin)/mm),
	      G = (unsigned char)(255*(*(data2++)-nmin)/mm);
	    (*ptrd++) = (R&0xf0) | (G>>4);
	  } break;
	  default:
	    for (unsigned int xy=0; xy<xymax; xy++) {
	      const unsigned char
		R = (unsigned char)(255*(*(data1++)-nmin)/mm),
		G = (unsigned char)(255*(*(data2++)-nmin)/mm),
		B = (unsigned char)(255*(*(data3++)-nmin)/mm);
	      *(ptrd++) = (R&0xe0) | ((G>>5)<<2) | (B>>6);
	    } break;
	  }
	  if (ndata!=data) { _render_resize(ndata,img.width,img.height,(unsigned char*)data,width,height); delete[] ndata; } 
	} break;
	case 16: {
	  unsigned short *const ndata = (img.width==width && img.height==height)?(unsigned short*)data:new unsigned short[img.width*img.height];
	  unsigned char *ptrd = (unsigned char*)ndata;
	  const unsigned int M = 248;
	  if (cimg::X11attr().byte_order) for (unsigned int xy=0; xy<xymax; xy++) {
	    const unsigned char G = (unsigned char)(255*(*(data2++)-nmin)/mm)>>2;
	    *(ptrd++) = ((unsigned char)(255*(*(data1++)-nmin)/mm)&M) | (G>>3);
	    *(ptrd++) = (G<<5) | ((unsigned char)(255*(*(data3++)-nmin)/mm)>>3);
	  } else for (unsigned int xy=0; xy<xymax; xy++) {
	    const unsigned char G = (unsigned char)(255*(*(data2++)-nmin)/mm)>>2;
	    *(ptrd++) = (G<<5) | ((unsigned char)(255*(*(data3++)-nmin)/mm)>>3);
	    *(ptrd++) = ((unsigned char)(255*(*(data1++)-nmin)/mm)&M) | (G>>3);
	  }
	  if (ndata!=data) { _render_resize(ndata,img.width,img.height,(unsigned short*)data,width,height); delete[] ndata; } 
	} break;
	default: {
	  unsigned int *const ndata = (img.width==width && img.height==height)?(unsigned int*)data:new unsigned int[img.width*img.height];
	  unsigned char *ptrd = (unsigned char*)ndata;
	  if (cimg::X11attr().byte_order) for (unsigned int xy=0; xy<xymax; xy++) {
	    (*ptrd++) = 0;
	    (*ptrd++) = (unsigned char)(255*(*(data1++)-nmin)/mm);
	    (*ptrd++) = (unsigned char)(255*(*(data2++)-nmin)/mm);
	    (*ptrd++) = (unsigned char)(255*(*(data3++)-nmin)/mm);
	  } else for (unsigned int xy=0; xy<xymax; xy++) {
	    (*ptrd++) = (unsigned char)(255*(*(data3++)-nmin)/mm);
	    (*ptrd++) = (unsigned char)(255*(*(data2++)-nmin)/mm);
	    (*ptrd++) = (unsigned char)(255*(*(data1++)-nmin)/mm);
	    (*ptrd++) = 0;
	  }
	  if (ndata!=data) { _render_resize(ndata,img.width,img.height,(unsigned int*)data,width,height); delete[] ndata; } 
	} break;
	} 
      }
      
      pthread_mutex_unlock(cimg::X11attr().mutex);
      return *this;
    }

    static int _assign_lowlevel_xshm(Display *dpy, XErrorEvent *error) {      
      dpy = 0; error = 0;
      cimg::X11attr().shm_enabled = false;
      return 0;
    }
    
    void assign_lowlevel(const unsigned int dimw, const unsigned int dimh, const char *ptitle=0,
			 const unsigned int normalization_type=1, const unsigned int events_type=3,
			 const bool fullscreen_flag=false, const bool closed_flag=false) {

      // Allocate space for window title
      const int s = cimg::strlen(ptitle)+1;
      char *tmp_title = s?new char[s]:0;
      if (s) std::memcpy(tmp_title,ptitle,s*sizeof(char));

      // Destroy previous display window if existing
      if (width && height) assign();

      // Open X11 display if necessary.
      if (!cimg::X11attr().display) { 
        cimg::X11attr().nb_wins = 0;
        cimg::X11attr().mutex = new pthread_mutex_t;
        pthread_mutex_init(cimg::X11attr().mutex,0);

        cimg::X11attr().display = XOpenDisplay((std::getenv("DISPLAY") ? std::getenv("DISPLAY") : ":0.0"));
        if (!cimg::X11attr().display) 
	  throw CImgDisplayException("CImgDisplay::_create_window() : Can't open X11 display");
        cimg::X11attr().nb_bits = DefaultDepth(cimg::X11attr().display, DefaultScreen(cimg::X11attr().display));
	if (cimg::X11attr().nb_bits!=8 && cimg::X11attr().nb_bits!=16 && cimg::X11attr().nb_bits!=24)
	  throw CImgDisplayException("CImgDisplay::_create_window() : %u bits mode is not supported "
				     "(only 8, 16 and 24 bits modes are supported)",cimg::X11attr().nb_bits);
        cimg::X11attr().gc = new GC;
        *cimg::X11attr().gc = DefaultGC(cimg::X11attr().display,DefaultScreen(cimg::X11attr().display));
        Visual *visual = DefaultVisual(cimg::X11attr().display,DefaultScreen(cimg::X11attr().display));
	XVisualInfo vtemplate;
	vtemplate.visualid = XVisualIDFromVisual(visual);
	int nb_visuals;
	XVisualInfo *vinfo = XGetVisualInfo(cimg::X11attr().display,VisualIDMask,&vtemplate,&nb_visuals);
	if (vinfo && vinfo->red_mask<vinfo->blue_mask) cimg::X11attr().blue_first = true;
	cimg::X11attr().byte_order = ImageByteOrder(cimg::X11attr().display);

        pthread_mutex_lock(cimg::X11attr().mutex);
        cimg::X11attr().event_thread = new pthread_t;
        pthread_create(cimg::X11attr().event_thread,0,_events_thread,0);
      } else pthread_mutex_lock(cimg::X11attr().mutex);
      
      // Set display variables
      width = dimw;
      height = dimh;
      normalization = normalization_type&3;
      events = events_type&3;
      is_fullscreen = fullscreen_flag;
      title = tmp_title;
      window_x = window_y = 0;
      mouse_x = mouse_y = -1;
      button = wheel = key = 0;
      is_resized = is_moved = is_event = false;
      is_closed = closed_flag;
      
      // Create X11 window and palette (if 8bits display)
      if (is_fullscreen) {
	_init_fullscreen();
	const unsigned int sx = screen_dimx(), sy = screen_dimy();
	XSetWindowAttributes winattr;
	winattr.override_redirect = True;
	window = XCreateWindow(cimg::X11attr().display,
			       RootWindow(cimg::X11attr().display,DefaultScreen(cimg::X11attr().display)),
			       (sx-width)/2,(sy-height)/2,
			       width,height,0,0,InputOutput,CopyFromParent,CWOverrideRedirect,&winattr);
      } else
	window = XCreateSimpleWindow(cimg::X11attr().display,
				     RootWindow(cimg::X11attr().display,DefaultScreen(cimg::X11attr().display)),
				     0,0,width,height,2,0,0x0L);
      XStoreName(cimg::X11attr().display,window,title?title:" ");
      if (cimg::X11attr().nb_bits==8) {
	colormap = XCreateColormap(cimg::X11attr().display,window,DefaultVisual(cimg::X11attr().display,
										DefaultScreen(cimg::X11attr().display)),AllocAll);
	_set_colormap(colormap,3);
	XSetWindowColormap(cimg::X11attr().display,window,colormap);
      }

      // Create XImage
      const unsigned int bufsize = width*height*(cimg::X11attr().nb_bits==8?1:(cimg::X11attr().nb_bits==16?2:4));
#ifdef cimg_use_xshm
      shminfo = 0;
      if (XShmQueryExtension(cimg::X11attr().display)) {
	shminfo = new XShmSegmentInfo;
	image = XShmCreateImage(cimg::X11attr().display,DefaultVisual(cimg::X11attr().display,DefaultScreen(cimg::X11attr().display)),
				cimg::X11attr().nb_bits,ZPixmap,0,shminfo,width,height);
	if (!image) { delete shminfo; shminfo = 0; }
	else {
	  shminfo->shmid = shmget(IPC_PRIVATE, bufsize, IPC_CREAT | 0777);	  
	  if (shminfo->shmid==-1) { XDestroyImage(image); delete shminfo; shminfo = 0; }
	  else {
	    shminfo->shmaddr = image->data = (char*)(data = shmat(shminfo->shmid,0,0));
	    if (shminfo->shmaddr==(char*)-1) { XDestroyImage(image); shmctl(shminfo->shmid,IPC_RMID,0); delete shminfo; shminfo = 0; }
	    shminfo->readOnly = False;
	    cimg::X11attr().shm_enabled = true;
	    XErrorHandler oldXErrorHandler = XSetErrorHandler(_assign_lowlevel_xshm);
	    XShmAttach(cimg::X11attr().display, shminfo);
	    XSync(cimg::X11attr().display, False);
	    XSetErrorHandler(oldXErrorHandler);
	    if (!cimg::X11attr().shm_enabled) {
	      XDestroyImage(image);
	      shmdt(shminfo->shmaddr);
	      shmctl(shminfo->shmid,IPC_RMID,0);
	      delete shminfo; shminfo = 0;
	    }
	  }
	}
      }
      if (!shminfo)
#endif		
	{
	  data = std::malloc(bufsize);
	  image = XCreateImage(cimg::X11attr().display,DefaultVisual(cimg::X11attr().display,DefaultScreen(cimg::X11attr().display)),
			       cimg::X11attr().nb_bits,ZPixmap,0,(char*)data,width,height,8,0);
	}
    
      if (!is_closed) _map_window(); else { window_x = window_y = cimg::type<int>::min(); }
      
      if (events) { 
	wm_delete_window = XInternAtom(cimg::X11attr().display, "WM_DELETE_WINDOW", False);
	wm_delete_protocol = XInternAtom(cimg::X11attr().display, "WM_PROTOCOLS", False);
	XSetWMProtocols(cimg::X11attr().display, window, &wm_delete_window, 1); 
	if (is_fullscreen) XGrabKeyboard(cimg::X11attr().display, window, True, GrabModeAsync, GrabModeAsync, CurrentTime);
      }
      window_width = width;
      window_height = height;
      mouse_x = mouse_y = -1;
      button = wheel = key = 0;
      is_resized = is_moved = is_event = false;
      fps_timer = fps_frames = timer = 0;
      fps_fps = 0;

      cimg::X11attr().wins[cimg::X11attr().nb_wins++]=this;
      pthread_mutex_unlock(cimg::X11attr().mutex);
    }

    void _map_window() {
      XWindowAttributes attr;
      XEvent event;
      XSelectInput(cimg::X11attr().display,window,ExposureMask | StructureNotifyMask);
      bool exposed = false, mapped = false;
      XMapRaised(cimg::X11attr().display,window);      
      XSync(cimg::X11attr().display,False);
      do {
	XWindowEvent(cimg::X11attr().display,window,StructureNotifyMask | ExposureMask,&event);
	switch (event.type) {
	case MapNotify: mapped = true; break;
	case Expose: exposed = true; break;
	default:  XSync(cimg::X11attr().display, False); cimg::sleep(10);
	} 	
      } while (!(exposed && mapped));     
      do {
	XGetWindowAttributes(cimg::X11attr().display, window, &attr);
	if (attr.map_state!=IsViewable) { XSync(cimg::X11attr().display,False); cimg::sleep(10); }
      } while (attr.map_state != IsViewable);
      window_x = attr.x;
      window_y = attr.y;
    }

    void _set_colormap(Colormap& colormap, const unsigned int dim) {
      XColor palette[256];
      switch (dim) {
      case 1:  // palette for greyscale images
	for (unsigned int index=0; index<256; index++) {
	  palette[index].pixel = index;
	  palette[index].red = palette[index].green = palette[index].blue = index<<8;
	  palette[index].flags = DoRed | DoGreen | DoBlue;
	}
	break;
      case 2: // palette for RG images
	for (unsigned int index=0, r=8; r<256; r+=16)
	  for (unsigned int g=8; g<256; g+=16) {
	    palette[index].pixel = index;
	    palette[index].red   = palette[index].blue = r<<8;
	    palette[index].green = g<<8;
	    palette[index++].flags = DoRed | DoGreen | DoBlue;	    
	  }
	break;
      default: // palette for RGB images
	for (unsigned int index=0, r=16; r<256; r+=32)
	  for (unsigned int g=16; g<256; g+=32)
	    for (unsigned int b=32; b<256; b+=64) {
	      palette[index].pixel = index;
	      palette[index].red   = r<<8;
	      palette[index].green = g<<8;
	      palette[index].blue  = b<<8;
	      palette[index++].flags = DoRed | DoGreen | DoBlue;
	    }
	break;
      }      
      XStoreColors(cimg::X11attr().display,colormap,palette,256);      
    }

    void _paint(const bool wait_expose=true) {
      if (!is_closed) {
	if (wait_expose) {
	  static XEvent event;
	  event.xexpose.type = Expose;
	  event.xexpose.serial = 0;
	  event.xexpose.send_event = True;
	  event.xexpose.display = cimg::X11attr().display;
	  event.xexpose.window = window;
	  event.xexpose.x = 0;
	  event.xexpose.y = 0;
	  event.xexpose.width = (int)width;
	  event.xexpose.height = (int)height;
	  event.xexpose.count = 0;
	  XSendEvent(cimg::X11attr().display, window, False, 0, &event);
	} else {
#if cimg_use_xshm
	  if (shminfo) XShmPutImage(cimg::X11attr().display,window,*cimg::X11attr().gc,image,0,0,0,0,width,height,False);
	  else
#endif
	    XPutImage(cimg::X11attr().display,window,*cimg::X11attr().gc,image,0,0,0,0,width,height); 
	  XSync(cimg::X11attr().display, False);
	}
      }
    }
    
    template<typename T> void _resize(T foo, const unsigned int ndimx, const unsigned int ndimy, const bool redraw) {
      foo = 0;
#ifdef cimg_use_xshm
      if (shminfo) {
	XShmSegmentInfo *nshminfo = new XShmSegmentInfo;
	XImage *nimage = XShmCreateImage(cimg::X11attr().display,DefaultVisual(cimg::X11attr().display,DefaultScreen(cimg::X11attr().display)),
					 cimg::X11attr().nb_bits,ZPixmap,0,nshminfo,ndimx,ndimy);
	nshminfo->shmid = shmget(IPC_PRIVATE, ndimx*ndimy*sizeof(T), IPC_CREAT | 0777);
	nshminfo->shmaddr = nimage->data = (char*)shmat(nshminfo->shmid,0,0);
	nshminfo->readOnly = False;
	XShmAttach(cimg::X11attr().display, nshminfo);
	XSync(cimg::X11attr().display, False);
	T *const ndata = (T*)nimage->data;
	if (redraw) _render_resize((T*)data,width,height,ndata,ndimx,ndimy);
	else std::memset(ndata,0,sizeof(T)*ndimx*ndimy);
	XShmDetach(cimg::X11attr().display, shminfo);
	XDestroyImage(image);
	shmdt(shminfo->shmaddr);
	shmctl(shminfo->shmid,IPC_RMID,0);
	delete shminfo;
	shminfo = nshminfo;
	image = nimage;
	data = (void*)ndata;
      } else
#endif
	{
	  T *ndata = (T*)std::malloc(ndimx*ndimy*sizeof(T));
	  if (redraw) _render_resize((T*)data,width,height,ndata,ndimx,ndimy);
	  else std::memset(ndata,0,sizeof(T)*ndimx*ndimy);
	  data = (void*)ndata;
	  XDestroyImage(image);
	  image = XCreateImage(cimg::X11attr().display,DefaultVisual(cimg::X11attr().display,DefaultScreen(cimg::X11attr().display)),
			       cimg::X11attr().nb_bits,ZPixmap,0,(char*)data,ndimx,ndimy,8,0);
	}
    }

    void _init_fullscreen() {
      background_window = 0;
      if (is_fullscreen && !is_closed) {
#ifdef cimg_use_xrandr     
	int foo;
	if (XRRQueryExtension(cimg::X11attr().display,&foo,&foo)) {	
	  XRRRotations(cimg::X11attr().display, DefaultScreen(cimg::X11attr().display), &cimg::X11attr().curr_rotation);
	  if (!cimg::X11attr().resolutions) {
	    cimg::X11attr().resolutions = XRRSizes(cimg::X11attr().display,DefaultScreen(cimg::X11attr().display),&foo);
	    cimg::X11attr().nb_resolutions = (unsigned int)foo;
	  }
	  if (cimg::X11attr().resolutions) {
	    cimg::X11attr().curr_resolution = 0;
	    for (unsigned int i=0; i<cimg::X11attr().nb_resolutions; i++) {
	      const unsigned int
		nw = (unsigned int)(cimg::X11attr().resolutions[i].width),
		nh = (unsigned int)(cimg::X11attr().resolutions[i].height);
	      if (nw>=width && nh>=height &&
		  nw<=(unsigned int)(cimg::X11attr().resolutions[cimg::X11attr().curr_resolution].width) &&
		  nh<=(unsigned int)(cimg::X11attr().resolutions[cimg::X11attr().curr_resolution].height)) 
		cimg::X11attr().curr_resolution = i;
	    }
	    if (cimg::X11attr().curr_resolution>0) {
	      XRRScreenConfiguration *config = XRRGetScreenInfo(cimg::X11attr().display, DefaultRootWindow(cimg::X11attr().display));
	      XRRSetScreenConfig(cimg::X11attr().display, config, DefaultRootWindow(cimg::X11attr().display),
				 cimg::X11attr().curr_resolution, cimg::X11attr().curr_rotation, CurrentTime);
	      XRRFreeScreenConfigInfo(config);
	      XSync(cimg::X11attr().display, False);
	    }
	  }
	}
#endif
	const unsigned int sx = screen_dimx(), sy = screen_dimy();
	XSetWindowAttributes winattr;
	winattr.override_redirect = True;
	if (sx!=width || sy!=height) {
	  background_window = XCreateWindow(cimg::X11attr().display,
					    RootWindow(cimg::X11attr().display,DefaultScreen(cimg::X11attr().display)),0,0,
					    sx,sy,0,0,InputOutput,CopyFromParent,CWOverrideRedirect,&winattr);
	  const unsigned int bufsize = sx*sy*(cimg::X11attr().nb_bits==8?1:(cimg::X11attr().nb_bits==16?2:4));
	  void *background_data = std::malloc(bufsize);
	  std::memset(background_data,0,bufsize);
	  XImage *background_image = XCreateImage(cimg::X11attr().display,DefaultVisual(cimg::X11attr().display,DefaultScreen(cimg::X11attr().display)),
						  cimg::X11attr().nb_bits,ZPixmap,0,(char*)background_data,sx,sy,8,0);
	  XEvent event;
	  XSelectInput(cimg::X11attr().display,background_window,StructureNotifyMask);
	  XMapRaised(cimg::X11attr().display,background_window);
	  do XWindowEvent(cimg::X11attr().display,background_window,StructureNotifyMask,&event);
	  while (event.type!=MapNotify);
	  XPutImage(cimg::X11attr().display,background_window,*cimg::X11attr().gc,background_image,0,0,0,0,sx,sy); 
	  XWindowAttributes attr;
	  XGetWindowAttributes(cimg::X11attr().display, background_window, &attr);
	  while (attr.map_state != IsViewable) XSync(cimg::X11attr().display, False);
	  XDestroyImage(background_image);
	}
      }
    }

    void _desinit_fullscreen() {
      if (is_fullscreen) {
	XUngrabKeyboard(cimg::X11attr().display,CurrentTime);
#if cimg_use_xrandr
	if (cimg::X11attr().resolutions && cimg::X11attr().curr_resolution) {	
	  XRRScreenConfiguration *config = XRRGetScreenInfo(cimg::X11attr().display, DefaultRootWindow(cimg::X11attr().display));
	  XRRSetScreenConfig(cimg::X11attr().display, config, DefaultRootWindow(cimg::X11attr().display),
			     0, cimg::X11attr().curr_rotation, CurrentTime);
	  XRRFreeScreenConfigInfo(config);
	  XSync(cimg::X11attr().display, False);
	  cimg::X11attr().curr_resolution = 0;
	}
#endif
	if (background_window) XDestroyWindow(cimg::X11attr().display,background_window);
	background_window = 0;
	is_fullscreen = false;
      }
    }
    
    void _handle_events(const XEvent *const pevent) {
      XEvent event=*pevent;
      switch (event.type) {
      case ClientMessage:
	if ((int)event.xclient.message_type==(int)wm_delete_protocol && 
	    (int)event.xclient.data.l[0]==(int)wm_delete_window) {
	  XUnmapWindow(cimg::X11attr().display,window);
	  mouse_x = mouse_y = -1; 
	  button = key = 0;
	  is_closed = is_event = true;
	}
        break;
     case ConfigureNotify: {
        while (XCheckWindowEvent(cimg::X11attr().display,window,StructureNotifyMask,&event));
        const unsigned int
	  nw = event.xconfigure.width,
	  nh = event.xconfigure.height;
	const int
	  nx = event.xconfigure.x,
	  ny = event.xconfigure.y;
        if (nw && nh && (nw!=window_width || nh!=window_height)) { 
          window_width = nw; 
          window_height = nh; 
	  mouse_x = mouse_y = -1;
          XResizeWindow(cimg::X11attr().display,window,window_width,window_height);
          is_resized = is_event = true;
        }
	if (nx!=window_x || ny!=window_y) {
	  window_x = nx;
	  window_y = ny;
	  is_moved = is_event = true;
	}
     } break;
      case Expose: {
        while (XCheckWindowEvent(cimg::X11attr().display,window,ExposureMask,&event));
	_paint(false);
      	if (is_fullscreen) {
	  XWindowAttributes attr;
	  XGetWindowAttributes(cimg::X11attr().display, window, &attr);
	  while (attr.map_state != IsViewable) XSync(cimg::X11attr().display, False);
	  XSetInputFocus(cimg::X11attr().display, window, RevertToParent, CurrentTime);
	}
      } break;
      case ButtonPress: {
	do {
	  switch (event.xbutton.button) {
	  case 1: button|=1; is_event = true; break;
	  case 2: button|=4; is_event = true; break;
	  case 3: button|=2; is_event = true; break;
	  default: break;
	  }
	} while (XCheckWindowEvent(cimg::X11attr().display,window,ButtonPressMask,&event));
      } break;
      case ButtonRelease: {
	do {
	  switch (event.xbutton.button) {
	  case 1: button&=~1U; is_event = true; break;
	  case 2: button&=~4U; is_event = true; break;
	  case 3: button&=~2U; is_event = true; break;
	  case 4: wheel++; is_event = true; break;	    
	  case 5: wheel--; is_event = true; break;
	  default: break;
	  }
	} while (XCheckWindowEvent(cimg::X11attr().display,window,ButtonReleaseMask,&event));	
      } break;
      case KeyPress: {
        while (XCheckWindowEvent(cimg::X11attr().display,window,KeyPressMask,&event));
	char tmp;
	KeySym ksym;
	XLookupString(&event.xkey,&tmp,1,&ksym,0);
	key = (unsigned int)ksym;
	is_event = true;
      } break;
      case KeyRelease:
        while (XCheckWindowEvent(cimg::X11attr().display,window,KeyReleaseMask,&event));
        key = 0;
	is_event = true;
        break;
      case LeaveNotify:
        while (XCheckWindowEvent(cimg::X11attr().display,window,LeaveWindowMask,&event));
        mouse_x = mouse_y =-1; 
	is_event = true;
        break;
      case MotionNotify:
        while (XCheckWindowEvent(cimg::X11attr().display,window,PointerMotionMask,&event));
        mouse_x = event.xmotion.x; 
        mouse_y = event.xmotion.y;
        if (mouse_x<0 || mouse_y<0 || mouse_x>=dimx() || mouse_y>=dimy()) mouse_x = mouse_y = -1; 
	is_event = true;
        break;
      }
    }
    
    static void* _events_thread(void *arg) {
      arg = 0;
      XEvent event;
      pthread_setcanceltype(PTHREAD_CANCEL_DEFERRED,0);
      pthread_setcancelstate(PTHREAD_CANCEL_ENABLE,0);
      for (;;) {
        pthread_mutex_lock(cimg::X11attr().mutex);
        for (unsigned int i=0; i<cimg::X11attr().nb_wins; i++) {
          const unsigned int xevent_type = (cimg::X11attr().wins[i]->events)&3;
          const unsigned int emask =
            ((xevent_type>=1)?ExposureMask|StructureNotifyMask:0)|
            ((xevent_type>=2)?ButtonPressMask|KeyPressMask|PointerMotionMask|LeaveWindowMask:0)|
            ((xevent_type>=3)?ButtonReleaseMask|KeyReleaseMask:0);
          XSelectInput(cimg::X11attr().display,cimg::X11attr().wins[i]->window,emask);
        }
        bool event_flag = XCheckTypedEvent(cimg::X11attr().display, ClientMessage, &event);
        if (!event_flag) event_flag = XCheckMaskEvent(cimg::X11attr().display,
                                                      ExposureMask|StructureNotifyMask|ButtonPressMask|
                                                      KeyPressMask|PointerMotionMask|LeaveWindowMask|ButtonReleaseMask|
                                                      KeyReleaseMask,&event);
        if (event_flag) {
          for (unsigned int i=0; i<cimg::X11attr().nb_wins; i++)
            if (!cimg::X11attr().wins[i]->is_closed && event.xany.window==cimg::X11attr().wins[i]->window)
	      cimg::X11attr().wins[i]->_handle_events(&event);
        }
        pthread_mutex_unlock(cimg::X11attr().mutex);
	pthread_testcancel();
        cimg::sleep(7);
      }
      return 0;
    }

    // Windows-based display
    //-----------------------
#elif cimg_display_type==2
    CLIENTCREATESTRUCT ccs;
    BITMAPINFO bmi;
    unsigned int *data;
    DEVMODE curr_mode;
    HWND window;
    HWND background_window;
    HDC hdc;
    HANDLE thread;
    HANDLE created;
    HANDLE mutex;
    
    static int screen_dimx() { 
      DEVMODE mode;
      mode.dmSize = sizeof(DEVMODE);
      mode.dmDriverExtra = 0;
      EnumDisplaySettings(0,ENUM_CURRENT_SETTINGS,&mode);
      return mode.dmPelsWidth;      
    }
    
    static int screen_dimy() {
      DEVMODE mode;
      mode.dmSize = sizeof(DEVMODE); 
      mode.dmDriverExtra = 0;
      EnumDisplaySettings(0,ENUM_CURRENT_SETTINGS,&mode);
      return mode.dmPelsHeight;      
    }

    CImgDisplay& assign(const unsigned int dimw, const unsigned int dimh, const char *title=0,
			const unsigned int normalization_type=1, const unsigned int events_type=3,
			const bool fullscreen_flag=false, const bool closed_flag=false) {
      if (!dimw || !dimh) 
	throw CImgArgumentException("CImgDisplay::assign() : Specified window size (%u,%u) is not valid.",dimw,dimh);
      assign_lowlevel(dimw,dimh,title,normalization_type,events_type,fullscreen_flag,closed_flag);
      std::memset(data,0,sizeof(unsigned int)*width*height);
      min = max = 0;
      return paint();
    }

    template<typename T> CImgDisplay& assign(const CImg<T>& img, const char *title=0,
					     const unsigned int normalization_type=1, const unsigned int events_type=3,
					     const bool fullscreen_flag=false, const bool closed_flag=false) {
      if (img.is_empty())
	throw CImgArgumentException("CImgDisplay::CImgDisplay() : Specified input image (%u,%u,%u,%u,%p) is empty.",
				    img.width,img.height,img.depth,img.dim,img.data);
      CImg<T> tmp;
      const CImg<T>& nimg = (img.depth==1)?img:(tmp=img.get_projections2d(img.width/2,img.height/2,img.depth/2));
      assign_lowlevel(nimg.width,nimg.height,title,normalization_type,events_type,fullscreen_flag,closed_flag);
      if (normalization==2) { const CImgStats st(nimg,false); min=st.min; max=st.max; }
      return display(nimg);
    }

    template<typename T> CImgDisplay& assign(const CImgl<T>& list,const char *title=0,
					     const unsigned int normalization_type=1, const unsigned int events_type=3,
					     const bool fullscreen_flag=false, const bool closed_flag=false) {
      if (list.is_empty())
	throw CImgArgumentException("CImgDisplay::CImgDisplay() : Specified input list (%u,%p) is empty.",
				    list.size,list.data);
      CImg<T> tmp;
      const CImg<T> img = list.get_append('x'), 
	&nimg = (img.depth==1)?img:(tmp=img.get_projections2d(img.width/2,img.height/2,img.depth/2));
      assign_lowlevel(nimg.width,nimg.height,title,normalization_type,events_type,fullscreen_flag,closed_flag);
       if (normalization==2) { const CImgStats st(nimg,false); min=st.min; max=st.max; }
      return display(nimg);
    }

    CImgDisplay& assign(const CImgDisplay& win) {
      assign_lowlevel(win.width,win.height,win.title,win.normalization,win.events,win.is_fullscreen,win.is_closed);
      std::memcpy(data,win.data,sizeof(unsigned int)*width*height);
      return paint();
    }

    CImgDisplay& assign() {            
      DestroyWindow(window);
      if (events) TerminateThread(thread,0);
      if (data) delete[] data;
      if (title) delete[] title;
      if (is_fullscreen) _desinit_fullscreen();
      width = height = normalization = events = 0;
      is_fullscreen = is_resized = is_moved = is_event = false;
      is_closed = true;
      title = 0;
      window_x = window_y = window_width = window_height = mouse_x = mouse_y = button = wheel = key = 0;
      min = max = 0;
      return *this;
    }

   template<typename T> CImgDisplay& display(const CImg<T>& img) {
      return render(img).paint();
    }
  
    CImgDisplay& resize(const int nwidth, const int nheight, const bool redraw=true) {
      if (!(nwidth && nheight))
	throw CImgArgumentException("CImgDisplay::resize() : Specified window size (%d,%d) is not valid.",
				    nwidth,nheight);
      const unsigned int
	tmpdimx=(nwidth>0)?nwidth:(-nwidth*width/100),
	tmpdimy=(nheight>0)?nheight:(-nheight*height/100),
	dimx = tmpdimx?tmpdimx:1,
	dimy = tmpdimy?tmpdimy:1;
      const bool
	is_disp_different = (width!=dimx || height!=dimy),
	is_win_different = (window_width!=dimx || window_height!=dimy);

      if (is_disp_different || is_win_different) {
	RECT rect; rect.left=rect.top=0; rect.right = dimx-1; rect.bottom = dimy-1;
	AdjustWindowRect(&rect,WS_CAPTION | WS_SYSMENU | WS_THICKFRAME | WS_MINIMIZEBOX | WS_MAXIMIZEBOX,false);
	const int cwidth = rect.right-rect.left+1, cheight = rect.bottom-rect.top+1;
	SetWindowPos(window,0,0,0,cwidth,cheight,SWP_NOMOVE | SWP_NOZORDER | SWP_NOCOPYBITS);
	window_width  = dimx;
	window_height = dimy;
	is_resized = false;
	if (is_disp_different) {
	  unsigned int *ndata = new unsigned int[dimx*dimy];
	  if (redraw) _render_resize(data,width,height,ndata,dimx,dimy);
	  else std::memset(ndata,0x80,sizeof(unsigned int)*dimx*dimy);
	  delete[] data;
	  data = ndata;
	  bmi.bmiHeader.biWidth = dimx;
	  bmi.bmiHeader.biHeight = -(int)dimy;
	  width = dimx;
	  height = dimy;
	}
	if (is_fullscreen) move((screen_dimx()-width)/2,(screen_dimy()-height)/2);
	if (redraw) return paint();
      }      
      return *this;
    }
    
    CImgDisplay& move(const int posx,const int posy) {
      if (!is_fullscreen) {
	RECT rect; rect.left=rect.top=0; rect.right=window_width-1; rect.bottom=window_height-1;
	AdjustWindowRect(&rect,WS_CAPTION | WS_SYSMENU | WS_THICKFRAME | WS_MINIMIZEBOX | WS_MAXIMIZEBOX,false);
	const int border1 = (rect.right-rect.left+1-width)/2, border2 = rect.bottom-rect.top+1-height-border1;
	SetWindowPos(window,0,posx-border1,posy-border2,0,0,SWP_NOSIZE | SWP_NOZORDER);
      } else SetWindowPos(window,0,posx,posy,0,0,SWP_NOSIZE | SWP_NOZORDER);
      window_x = posx;
      window_y = posy;
      is_moved = false;
      return show();
    }

    static void wait_all() {
      WaitForSingleObject(cimg::Win32attr().wait_event,INFINITE);
    }

    CImgDisplay& show() {
      if (is_closed) {
	is_closed = false;
	if (is_fullscreen) _init_fullscreen();
        ShowWindow(window,SW_SHOW);	
	RECT rect; 
	rect.left=rect.top=0; rect.right=width-1; rect.bottom=height-1;
	AdjustWindowRect(&rect,WS_CAPTION | WS_SYSMENU | WS_THICKFRAME | WS_MINIMIZEBOX | WS_MAXIMIZEBOX,false);
	const int border1 = (rect.right-rect.left+1-width)/2, border2 = rect.bottom-rect.top+1-height-border1;
	GetWindowRect(window,&rect);
	window_x = rect.left + border1;
	window_y = rect.top + border2;
      }
      return paint();
    }

    CImgDisplay& close() {
      if (!is_closed && !is_fullscreen) {
	if (is_fullscreen) _desinit_fullscreen();
        ShowWindow(window,SW_HIDE);
        is_closed = true;
	window_x = window_y = 0;
      }
      return *this;
    }

    CImgDisplay& paint() {
      if (!is_closed) {
	WaitForSingleObject(mutex,INFINITE);
	SetDIBitsToDevice(hdc,0,0,width,height,0,0,0,height,data,&bmi,DIB_RGB_COLORS);
	ReleaseMutex(mutex);
      }
      return *this;
    }

    template<typename T> CImgDisplay& render(const CImg<T>& img) {
      if (img.is_empty())
	throw CImgArgumentException("CImgDisplay::_render_image() : Specified input image (%u,%u,%u,%u,%p) is empty.",
				    img.width,img.height,img.depth,img.dim,img.data);
      if (img.depth!=1) return render(img.get_projections2d(img.width/2,img.height/2,img.depth/2));
      
      const T 
	*data1 = img.ptr(),
	*data2 = (img.dim>=2)?img.ptr(0,0,0,1):data1,
	*data3 = (img.dim>=3)?img.ptr(0,0,0,2):data1;

      WaitForSingleObject(mutex,INFINITE);
      unsigned int 
	*const ndata = (img.width==width && img.height==height)?data:new unsigned int[img.width*img.height],
	*ptrd = ndata;
      
      if (!normalization) for (unsigned int xy = img.width*img.height; xy>0; xy--)
	*(ptrd++) = ((unsigned char)*(data1++)<<16) | ((unsigned char)*(data2++)<<8) | (unsigned char)*(data3++);
      else {
 	if ((min>max) || normalization==1) { const CImgStats st(img,false); min=st.min; max=st.max; }
	const T nmin = (T)min, delta = (T)max-nmin, mm = delta?delta:(T)1;
	for (unsigned int xy = img.width*img.height; xy>0; xy--) {
	  const unsigned char
	    R = (unsigned char)(255*(*(data1++)-nmin)/mm),
	    G = (unsigned char)(255*(*(data2++)-nmin)/mm),
	    B = (unsigned char)(255*(*(data3++)-nmin)/mm);
	  *(ptrd++) = (R<<16) | (G<<8) | (B);
	}
      }
      if (ndata!=data) { _render_resize(ndata,img.width,img.height,data,width,height); delete[] ndata; }
      ReleaseMutex(mutex);
      return *this;
    }
   

    CImgDisplay& assign_lowlevel(const unsigned int dimw,const unsigned int dimh,const char *ptitle=0,
				 const unsigned int normalization_type=1,const unsigned int events_type=3,
				 const bool fullscreen_flag=false,const bool closed_flag=false) {

      
      // Allocate space for window title
      const int s = cimg::strlen(ptitle)+1;
      char *tmp_title = s?new char[s]:0;
      if (s) std::memcpy(tmp_title,ptitle,s*sizeof(char));
      
      // Destroy previous window if existing
      if (width && height) assign();

      // Set display variables
      width = dimw;
      height = dimh;
      normalization = normalization_type&3;
      events = events_type&3;
      is_fullscreen = fullscreen_flag;
      title = tmp_title;
      window_x = window_y = 0;
      mouse_x = mouse_y = -1;
      button = wheel = key = 0;
      is_resized = is_moved = is_event = false;      
      is_closed = closed_flag;
      fps_timer = fps_frames = timer = 0;
      fps_fps = 0;

      if (is_fullscreen) _init_fullscreen();
	  
      // Create event thread
      void *arg = (void*)(new void*[2]);
      ((void**)arg)[0]=(void*)this;
      ((void**)arg)[1]=(void*)title;            
      if (events) {
	unsigned long ThreadID;
	mutex     = CreateMutex(0,FALSE,0);
	created   = CreateEvent(0,FALSE,FALSE,0);
	thread    = CreateThread(0,0,_events_thread,arg,0,&ThreadID);
	WaitForSingleObject(created,INFINITE);
      } else _events_thread(arg);
      
      return *this;
    }

    static LRESULT APIENTRY _handle_events(HWND window,UINT msg,WPARAM wParam,LPARAM lParam) {
      CImgDisplay* disp = (CImgDisplay*)GetWindowLong(window,GWL_USERDATA);
      MSG st_msg;

      switch(msg) {
      case WM_CLOSE:
	disp->mouse_x = disp->mouse_y = -1;
	disp->key = disp->button = disp->window_x = disp->window_y = 0;
        disp->is_closed=true;
        ReleaseMutex(disp->mutex);
        ShowWindow(disp->window,SW_HIDE);
	disp->is_event = true;
        SetEvent(cimg::Win32attr().wait_event);
        return 0;
      case WM_SIZE: {
        while (PeekMessage(&st_msg,window,WM_SIZE,WM_SIZE,PM_REMOVE));
        WaitForSingleObject(disp->mutex,INFINITE);
        const unsigned int nw = LOWORD(lParam),nh = HIWORD(lParam);
        if (nw && nh && (nw!=disp->width || nh!=disp->height)) { 
          disp->window_width  = nw; 
          disp->window_height = nh;
	  disp->mouse_x = disp->mouse_y = -1;
          disp->is_resized = disp->is_event = true;
	  SetEvent(cimg::Win32attr().wait_event);
        }
        ReleaseMutex(disp->mutex);
      } break;
      case WM_MOVE: {
        while (PeekMessage(&st_msg,window,WM_SIZE,WM_SIZE,PM_REMOVE));
	WaitForSingleObject(disp->mutex,INFINITE);
	const int nx = (int)(short)(LOWORD(lParam)), ny = (int)(short)(HIWORD(lParam));
	if (nx!=disp->window_x || ny!=disp->window_y) {
	  disp->window_x = nx;
	  disp->window_y = ny;
	  disp->is_moved = disp->is_event = true;
	  SetEvent(cimg::Win32attr().wait_event);
	}
        ReleaseMutex(disp->mutex);
      } break;
      case WM_PAINT:
	disp->paint();
        break;
      }
      if (disp->events>=2) switch(msg) {
      case WM_KEYDOWN:
        while (PeekMessage(&st_msg,window,WM_KEYDOWN,WM_KEYDOWN,PM_REMOVE)); 
        disp->key=(int)wParam;
	disp->is_event = true;
        SetEvent(cimg::Win32attr().wait_event);
        break;
      case WM_MOUSEMOVE: {
        while (PeekMessage(&st_msg,window,WM_MOUSEMOVE,WM_MOUSEMOVE,PM_REMOVE));
        disp->mouse_x = LOWORD(lParam);
        disp->mouse_y = HIWORD(lParam);
        if (disp->mouse_x<0 || disp->mouse_y<0 || disp->mouse_x>=disp->dimx() || disp->mouse_y>=disp->dimy())
	  disp->mouse_x=disp->mouse_y=-1;
	disp->is_event = true;
        SetEvent(cimg::Win32attr().wait_event);
      } break;
      case WM_LBUTTONDOWN: 
        disp->button|=1U; 
	disp->is_event = true;
        SetEvent(cimg::Win32attr().wait_event);
        break;
      case WM_RBUTTONDOWN: 
        disp->button|=2U; 
	disp->is_event = true;
        SetEvent(cimg::Win32attr().wait_event);
        break;
      case WM_MBUTTONDOWN: 
        disp->button|=4U; 
	disp->is_event = true;
        SetEvent(cimg::Win32attr().wait_event);
        break;
      }
      if (disp->events>=3) switch(msg) {
      case WM_KEYUP:
        while (PeekMessage(&st_msg,window,WM_KEYUP,WM_KEYUP,PM_REMOVE));
        disp->key=0;
	disp->is_event = true;
        SetEvent(cimg::Win32attr().wait_event);
        break;
      case WM_LBUTTONUP:
        disp->button&=~1U; 
	disp->is_event = true;
        SetEvent(cimg::Win32attr().wait_event);
        break;
      case WM_RBUTTONUP:
        disp->button&=~2U;
	disp->is_event = true;
        SetEvent(cimg::Win32attr().wait_event);
        break;
      case WM_MBUTTONUP:
        disp->button&=~4U;
	disp->is_event = true;
        SetEvent(cimg::Win32attr().wait_event);
        break;
      }
      return DefWindowProc(window,msg,wParam,lParam);
    }
  
    static DWORD WINAPI _events_thread(void* arg) {
      CImgDisplay *disp  = (CImgDisplay*)(((void**)arg)[0]);
      const char *title = (const char*)(((void**)arg)[1]);
      MSG msg;
      delete[] (void**)arg;
      disp->bmi.bmiHeader.biSize=sizeof(BITMAPINFOHEADER);
      disp->bmi.bmiHeader.biWidth=disp->width;
      disp->bmi.bmiHeader.biHeight=-(int)disp->height;
      disp->bmi.bmiHeader.biPlanes=1;
      disp->bmi.bmiHeader.biBitCount=32;
      disp->bmi.bmiHeader.biCompression=BI_RGB;
      disp->bmi.bmiHeader.biSizeImage=0;
      disp->bmi.bmiHeader.biXPelsPerMeter=1;
      disp->bmi.bmiHeader.biYPelsPerMeter=1;
      disp->bmi.bmiHeader.biClrUsed=0;
      disp->bmi.bmiHeader.biClrImportant=0;
      disp->data = new unsigned int[disp->width*disp->height];
      if (!disp->is_fullscreen) { // Normal window
        RECT rect;
        rect.left=rect.top=0; rect.right=disp->width-1; rect.bottom=disp->height-1;
	AdjustWindowRect(&rect,WS_CAPTION | WS_SYSMENU | WS_THICKFRAME | WS_MINIMIZEBOX | WS_MAXIMIZEBOX,false);
	const int border1 = (rect.right-rect.left+1-disp->width)/2, border2 = rect.bottom-rect.top+1-disp->height-border1;
       
#if defined(_MSC_VER) && _MSC_VER>1200
        disp->window = CreateWindowA("MDICLIENT",title?title:" ",
				     WS_OVERLAPPEDWINDOW | (disp->is_closed?0:WS_VISIBLE), CW_USEDEFAULT,CW_USEDEFAULT,
				     disp->width + 2*border1, disp->height + border1 + border2,
				     0,0,0,&(disp->ccs));
#else
        disp->window = CreateWindow("MDICLIENT",title?title:" ",
				    WS_OVERLAPPEDWINDOW | (disp->is_closed?0:WS_VISIBLE), CW_USEDEFAULT,CW_USEDEFAULT,
				    disp->width + 2*border1, disp->height + border1 + border2,
				    0,0,0,&(disp->ccs));
#endif
	if (!disp->is_closed) {
	  GetWindowRect(disp->window,&rect);	
	  disp->window_x = rect.left + border1;
	  disp->window_y = rect.top + border2;
	} else disp->window_x = disp->window_y = 0;
      } else { // Fullscreen window
	const unsigned int sx = screen_dimx(), sy = screen_dimy();
#if defined(_MSC_VER) && _MSC_VER>1200
	disp->window = CreateWindowA("MDICLIENT",title?title:" ",
				     WS_POPUP | (disp->is_closed?0:WS_VISIBLE), (sx-disp->width)/2, (sy-disp->height)/2,
				     disp->width,disp->height,0,0,0,&(disp->ccs));
#else
	disp->window = CreateWindow("MDICLIENT",title?title:" ",
				    WS_POPUP | (disp->is_closed?0:WS_VISIBLE), (sx-disp->width)/2, (sy-disp->height)/2,
				    disp->width,disp->height,0,0,0,&(disp->ccs));
#endif
	disp->window_x = disp->window_y = 0;
      }
      SetForegroundWindow(disp->window);
      disp->hdc = GetDC(disp->window);
      disp->window_width = disp->width;
      disp->window_height = disp->height;
      disp->mouse_x = disp->mouse_y = -1;
      disp->button = disp->wheel = disp->key = 0;
      disp->is_resized = disp->is_moved = disp->is_event = false;
      if (disp->events) {
        SetWindowLong(disp->window,GWL_USERDATA,(LONG)disp);
        SetWindowLong(disp->window,GWL_WNDPROC,(LONG)_handle_events);
        SetEvent(disp->created);
        while( GetMessage(&msg,0,0,0) ) DispatchMessage( &msg );
      }
      return 0;
    }

    void _init_fullscreen() {
      background_window = 0;
      if (is_fullscreen && !is_closed) {	  
	DEVMODE mode;
	unsigned int imode=0, ibest=0, bestbpp=0, bw=~0U, bh=~0U;
	for (mode.dmSize = sizeof(DEVMODE), mode.dmDriverExtra = 0; EnumDisplaySettings(0,imode,&mode); imode++) {
	  const unsigned int nw = mode.dmPelsWidth, nh = mode.dmPelsHeight;
	  if (nw>=width && nh>=height && mode.dmBitsPerPel>=bestbpp && nw<=bw && nh<=bh) {
	    bestbpp = mode.dmBitsPerPel;
	    ibest = imode; 
	    bw = nw; bh = nh;
	  }
	}
	if (bestbpp) {
	  curr_mode.dmSize = sizeof(DEVMODE); curr_mode.dmDriverExtra = 0;
	  EnumDisplaySettings(0,ENUM_CURRENT_SETTINGS,&curr_mode);
	  EnumDisplaySettings(0,ibest,&mode);
	  ChangeDisplaySettings(&mode,0);
	} else curr_mode.dmSize = 0;
	
	const unsigned int sx = screen_dimx(), sy = screen_dimy();
	if (sx!=width || sy!=height) {
	  CLIENTCREATESTRUCT background_ccs;
#if defined(_MSC_VER) && _MSC_VER>1200
	  background_window = CreateWindowA("MDICLIENT"," ",WS_POPUP | WS_VISIBLE, 0,0,sx,sy,0,0,0,&background_ccs);
#else
	  background_window = CreateWindow("MDICLIENT"," ",WS_POPUP | WS_VISIBLE, 0,0,sx,sy,0,0,0,&background_ccs);
#endif
	  SetForegroundWindow(background_window);
	}	    
      } else curr_mode.dmSize = 0;
    }
    
    void _desinit_fullscreen() {
      if (is_fullscreen) {
	if (background_window) DestroyWindow(background_window);
	background_window = 0;
	if (curr_mode.dmSize) ChangeDisplaySettings(&curr_mode,0);
	is_fullscreen = false;	
      }
    }
      
#endif
    
  };

  /*
   #--------------------------------------
   #
   #
   #
   # Definition of the CImg<T> structure
   #
   #
   #
   #--------------------------------------
   */

  template<typename T> struct CImg {
    unsigned int width;
    unsigned int height;
    unsigned int depth;
    unsigned int dim;
    bool is_shared;
    T *data;
    typedef T* iterator;
    typedef const T* const_iterator;

    //--------------------------------------
    //
    //! \name Constructors-Destructor-Copy
    //@{
    //--------------------------------------
    
    //! Default constructor.
    CImg():
      width(0),height(0),depth(0),dim(0),is_shared(false),data(0) {}
    
    //! Destructor.
    ~CImg() {
      if (data && !is_shared) delete[] data;
    }
    
    //! In-place version of the default constructor and the default destructor.
    CImg& assign() {
      if (data && !is_shared) delete[] data;
      width = height = depth = dim = 0; is_shared = false; data = 0;
      return *this;
    }

    //! Equivalent to assign() (introduced as a STL-like function name).
    CImg& clear() { 
      return assign(); 
    }

    //! Copy constructor.
    template<typename t> CImg(const CImg<t>& img):is_shared(false) {
      const unsigned int siz = img.size();
      if (img.data && siz) {
	width = img.width; height = img.height; depth = img.depth; dim = img.dim; data = new T[siz];
	const t *ptrs = img.data+siz; cimg_map(*this,ptrd,T) *ptrd = (T)(*(--ptrs));
      } else { width = height = depth = dim = 0; data = 0; }
    }

    CImg(const CImg& img) {
      const unsigned int siz = img.size();
      if (img.data && siz) {
	width = img.width; height = img.height; depth = img.depth; dim = img.dim; is_shared = img.is_shared;
	if (is_shared) data = const_cast<T*>(img.data);
	else { data = new T[siz]; std::memcpy(data,img.data,siz*sizeof(T)); }
      } else { width = height = depth = dim = 0; is_shared = false; data = 0; }
    }

    //! In-place version of the copy constructor.
    template<typename t> CImg& assign(const CImg<t>& img) {
      return assign(img.data,img.width,img.height,img.depth,img.dim);
    }
    
    //! Copy constructor allowing to force the shared state of the instance image.
    template<typename t> CImg(const CImg<t>& img, const bool shared):is_shared(false) {
      if (shared) throw CImgArgumentException("CImg<%s>::CImg() : Cannot construct a shared copy from a CImg<%s> image "
					      "(different pixel types).",pixel_type(),CImg<t>::pixel_type());
      const unsigned int siz = img.size();
      if (img.data && siz) {
	width = img.width; height = img.height; depth = img.depth; dim = img.dim; data = new T[siz];
	const t *ptrs = img.data+siz; cimg_map(*this,ptrd,T) *ptrd = (T)(*(--ptrs));
      } else { width = height = depth = dim = 0; data = 0; }
    }

    CImg(const CImg& img, const bool shared) {
      const unsigned int siz = img.size();
      if (img.data && siz) {
	width = img.width; height = img.height; depth = img.depth; dim = img.dim; is_shared = shared;
	if (is_shared) data = const_cast<T*>(img.data);
	else { data = new T[siz]; std::memcpy(data,img.data,siz*sizeof(T)); }
      } else { width = height = depth = dim = 0; is_shared = false; data = 0; }
    }

    // In-place version of the previous constructor.
    template<typename t> CImg& assign(const CImg<t>& img, const bool shared) {
      return assign(img.data,img.width,img.height,img.depth,img.dim,shared);
    }

    //! Assignement operator.
    template<typename t> CImg<T>& operator=(const CImg<t>& img) {
      return assign(img);
    }

    CImg& operator=(const CImg& img) {
      return assign(img);
    }

    //! Construct an image of size (\p dx,\p dy,\p dz,\p dv) with pixels of type \p T.
    explicit CImg(const unsigned int dx, const unsigned int dy=1, const unsigned int dz=1, const unsigned int dv=1):
      is_shared(false) {
      const unsigned int siz = dx*dy*dz*dv;
      if (siz) { width = dx; height = dy; depth = dz; dim = dv;	data = new T[siz]; }
      else { width = height = depth = dim = 0; data = 0; }
    }

    //! In-place version of the previous constructor.
    CImg& assign(const unsigned int dx, const unsigned int dy=1, const unsigned int dz=1, const unsigned int dv=1) {           
      const unsigned long siz = dx*dy*dz*dv, curr_siz = size();
      if (is_shared) {
	if (siz!=curr_siz)
	  throw CImgArgumentException("CImg<%s>::assign() : Cannot assign image (%u,%u,%u,%u) to shared instance image (%u,%u,%u,%u,%p).",
				      pixel_type(),dx,dy,dz,dv,width,height,depth,dim,data);
      } else {
	if (siz) {
	  if (siz!=curr_siz) { if (data) delete[] data; data = new T[siz]; }
	  width = dx; height = dy; depth = dz; dim = dv;
	} else { 
	  if (data) delete[] data;
	  width = height = depth = dim = 0; data = 0; 
	}	
      }
      return *this;
    }

    //! Construct an image of size (\p dx,\p dy,\p dz,\p dv) with pixels of type \p T and set all pixel values to \p val.
    explicit CImg(const unsigned int dx, const unsigned int dy, const unsigned int dz, const unsigned int dv, const T& val):
      is_shared(false) {
      const unsigned int siz = dx*dy*dz*dv;
      if (siz) { width = dx; height = dy; depth = dz; dim = dv; data = new T[siz]; fill(val); }
      else { width = height = depth = dim = 0; data = 0; }
    }

    //! In-place version of the previous constructor.
    CImg& assign(const unsigned int dx, const unsigned int dy, const unsigned int dz, const unsigned int dv, const T& val) {
      return assign(dx,dy,dz,dv).fill(val);
    }

    //! Construct an image from the filename \p filename.
    CImg(const char *const filename):width(0),height(0),depth(0),dim(0),is_shared(false),data(0) {
      assign(filename); 
    }

    //! Construct an image of size (\p dx,\p dy,\p dz,\p dv) with pixels of type \p T, from a raw pixel buffer \p data_buffer.
    template<typename t> CImg(const t *const data_buffer, const unsigned int dx, const unsigned int dy=1,
			      const unsigned int dz=1, const unsigned int dv=1, const bool shared=false):is_shared(false) {
      if (shared) throw CImgArgumentException("CImg<%s>::CImg() : Cannot construct a shared copy from a (%s*) buffer "
					      "(different pixel types).",pixel_type(),CImg<t>::pixel_type());
      const unsigned int siz = dx*dy*dz*dv;
      if (data_buffer && siz) {
	width = dx; height = dy; depth = dz; dim = dv; data = new T[siz];
	const t *ptrs = data_buffer+siz; cimg_map(*this,ptrd,T) *ptrd = (T)(*(--ptrs));
      } else { width = height = depth = dim = 0; data = 0; }
    }

#if (defined(_MSC_VER) && _MSC_VER<=1200)
    CImg(const T *const data_buffer, const unsigned int dx, const unsigned int dy,
	 const unsigned int dz, const unsigned int dv, const bool shared) {
#else
    CImg(const T *const data_buffer, const unsigned int dx, const unsigned int dy=1,
	 const unsigned int dz=1, const unsigned int dv=1, const bool shared=false) {
#endif
      const unsigned int siz = dx*dy*dz*dv;
      if (data_buffer && siz) {
	width = dx; height = dy; depth = dz; dim = dv; is_shared = shared;
	if (is_shared) data = const_cast<T*>(data_buffer);
	else { data = new T[siz]; std::memcpy(data,data_buffer,siz*sizeof(T)); }
      } else { width = height = depth = dim = 0; is_shared = false; data = 0; }
    }


    //! In-place version of the previous constructor.
    template<typename t> CImg& assign(const t *const data_buffer, const unsigned int dx, const unsigned int dy=1,
				      const unsigned int dz=1, const unsigned int dv=1) {
      assign(dx,dy,dz,dv);      
      const unsigned int siz = dx*dy*dz*dv;
      if (data_buffer && siz) { const t *ptrs = data_buffer+siz; cimg_map(*this,ptrd,T) *ptrd = (T)(*(--ptrs)); }
      else { width = height = depth = dim = 0; is_shared = false; data = 0; }
      return *this;
    }

#if (defined(_MSC_VER) && _MSC_VER<=1200)
    CImg& assign(const T *const data_buffer, const unsigned int dx, const unsigned int dy,
		 const unsigned int dz, const unsigned int dv) {
#else
    CImg& assign(const T *const data_buffer, const unsigned int dx, const unsigned int dy=1,
		 const unsigned int dz=1, const unsigned int dv=1) {
#endif
      assign(dx,dy,dz,dv);      
      const unsigned int siz = dx*dy*dz*dv;
      if (data_buffer && siz) std::memcpy(data,data_buffer,siz*sizeof(T));
      else { width = height = depth = dim = 0; is_shared = false; data = 0; }
      return *this;
    }

    //! In-place version of the previous constructor, allowing to force the shared state of the instance image.
    template<typename t> CImg& assign(const t *const data_buffer, const unsigned int dx, const unsigned int dy,
				      const unsigned int dz, const unsigned int dv, const bool shared) {
      if (shared) throw CImgArgumentException("CImg<%s>::assign() : Cannot define a shared copy from a CImg<%s> image "
					      "(different pixel types).",pixel_type(),CImg<t>::pixel_type());
      if (data && !is_shared) delete[] data;
      is_shared = false;
      const unsigned int siz = dx*dy*dz*dv;
      if (data_buffer && siz) {
	width = dx; height = dy; depth = dz; dim = dv; data = new T[siz];
	const t *ptrs = data_buffer+siz; cimg_map(*this,ptrd,T) *ptrd = (T)(*(--ptrs));
      } else { width = height = depth = dim = 0; data = 0; }
      return *this;
    }

    CImg& assign(const T *const data_buffer, const unsigned int dx, const unsigned int dy,
		 const unsigned int dz, const unsigned int dv, const bool shared) {
      if (data && !is_shared) delete[] data;
      const unsigned int siz = dx*dy*dz*dv;
      if (data_buffer && siz) {
	width = dx; height = dy; depth = dz; dim = dv; is_shared = shared;
	if (is_shared) data = const_cast<T*>(data_buffer);
	else { data = new T[siz]; std::memcpy(data,data_buffer,siz*sizeof(T)); }
      } else { width = height = depth = dim = 0; is_shared = false; data = 0; }
      return *this;
    }
    
    // INNER ROUTINE : Swap fields of an image (use it carefully!)
    // If an image is shared, its content is replaced by the non-shared image (which is unchanged).
    CImg& swap(CImg& img) {
      if (img.is_shared==is_shared) {
	cimg::swap(width,img.width);
	cimg::swap(height,img.height);
	cimg::swap(depth,img.depth);
	cimg::swap(dim,img.dim);
	cimg::swap(data,img.data);
      } else {
	if (img.is_shared) img.assign(*this);
	if (is_shared) assign(img);
      }
      return img;
    }
    
    //@}
    //-------------------------------------
    //
    //! \name Image Informations
    //@{
    //-------------------------------------
  
    //! Return the type of the pixel values.
    /**
       \return a string describing the type of the image pixels (template parameter \p T).
       - The string returned may contains spaces (<tt>"unsigned char"</tt>).
       - If the template parameter T does not correspond to a registered type, the string <tt>"unknown"</tt> is returned.
    **/
    static const char* pixel_type() { 
      static T val=0;
      return cimg::get_type(val); 
    }

    unsigned long size() const {
      return width*height*depth*dim; 
    }  

    int dimx() const {
      return (int)width; 
    }  

    int dimy() const {
      return (int)height; 
    }
  
    int dimz() const {
      return (int)depth; 
    }
  
    int dimv() const {
      return (int)dim;
    }
      
    //! Return \c true if image is empty.
    bool is_empty() const {
      return !(data && width && height && depth && dim); 
    }

    long offset(const int x=0, const int y=0, const int z=0, const int v=0) const {      
      return x + y*width + z*width*height + v*width*height*depth;
    }

    T* ptr(const unsigned int x=0, const unsigned int y=0, const unsigned int z=0, const unsigned int v=0) {
      const long off = offset(x,y,z,v);
      return data+off;
    }
    
    const T* ptr(const unsigned int x=0, const unsigned int y=0, const unsigned int z=0, const unsigned int v=0) const {
      const long off = offset(x,y,z,v);
      return data+off;
    }

    //! Return an iterator to the first image pixel
    iterator begin() { return data; }
    const_iterator begin() const { return data; }

    //! Return an iterator to the last image pixel
    iterator end() { return data + size(); }
    const_iterator end() const { return data + size(); }
    
    T& operator()(const unsigned int x,const unsigned int y=0,const unsigned int z=0,const unsigned int v=0) {
      const long off = offset(x,y,z,v);
      return data[off];
    }

    const T& operator()(const unsigned int x,const unsigned int y=0,const unsigned int z=0,const unsigned int v=0) const {
      const long off = offset(x,y,z,v);
      return data[off];
    }

    //! Return pixel value at a given position. Equivalent to operator().
    T& at(const unsigned int x,const unsigned int y=0,const unsigned int z=0,const unsigned int v=0) {
      const long off = offset(x,y,z,v);
      if (!data || off>=(long)size())
        throw CImgArgumentException("CImg<%s>::at() : Pixel access requested at (%u,%u,%u,%u) (offset=%d) "
				    "outside the image range (%u,%u,%u,%u) (size=%u)",
				    pixel_type(),x,y,z,v,off,width,height,depth,dim,size());
      return data[off];
    }

    const T& at(const unsigned int x,const unsigned int y=0,const unsigned int z=0,const unsigned int v=0) const {
      const long off = offset(x,y,z,v);
      if (!data || off>=(long)size())
        throw CImgArgumentException("CImg<%s>::at() : Pixel access requested at (%u,%u,%u,%u) (offset=%d) "
				    "outside the image range (%u,%u,%u,%u) (size=%u)",
				    pixel_type(),x,y,z,v,off,width,height,depth,dim,size());
      return data[off];
    }
    
    T& operator[](const unsigned long off) {
      return operator()(off);
    }

    const T& operator[](const unsigned long off) const {
      return operator()(off);
    }

    //! Return a reference to the last image value
    T& back() {
      return operator()(size()-1);
    }
    
    const T& back() const {
      return operator()(size()-1);
    }
    
    //! Return a reference to the first image value
    T& front() {
      return *data;
    }
    
    const T& front() const {
      return *data;
    }
    
    T pix4d(const int x, const int y, const int z, const int v, const T& out_val) const {
      return (x<0 || y<0 || z<0 || v<0 || x>=dimx() || y>=dimy() || z>=dimz() || v>=dimv())?out_val:(*this)(x,y,z,v);
    }

    T pix4d(const int x, const int y, const int z, const int v) const {
      return (*this)(x<0?0:(x>=dimx()?dimx()-1:x), y<0?0:(y>=dimy()?dimy()-1:y),
                     z<0?0:(z>=dimz()?dimz()-1:z), v<0?0:(v>=dimv()?dimv()-1:v));
    }

    T pix3d(const int x, const int y, const int z, const int v, const T& out_val) const {
      return (x<0 || y<0 || z<0 || x>=dimx() || y>=dimy() || z>=dimz())?out_val:(*this)(x,y,z,v);
    }

    const T& pix3d(const int x, const int y, const int z, const int v=0) const {
      return (*this)(x<0?0:(x>=dimx()?dimx()-1:x), y<0?0:(y>=dimy()?dimy()-1:y),
                     z<0?0:(z>=dimz()?dimz()-1:z),v);
    }

    //! Read a pixel value with Dirichlet or Neumann boundary conditions for the two first coordinates (\c x,\c y).
    T pix2d(const int x, const int y, const int z, const int v, const T& out_val) const {
      return (x<0 || y<0 || x>=dimx() || y>=dimy())?out_val:(*this)(x,y,z,v);
    }

    const T& pix2d(const int x,const int y,const int z=0,const int v=0) const {
      return (*this)(x<0?0:(x>=dimx()?dimx()-1:x), y<0?0:(y>=dimy()?dimy()-1:y),z,v);
    }

    //! Read a pixel value with Dirichlet or Neumann boundary conditions for the first coordinate \c x.
    T pix1d(const int x, const int y, const int z, const int v, const T& out_val) const {
      return (x<0 || x>=dimx())?out_val:(*this)(x,y,z,v);
    }

    const T& pix1d(const int x, const int y=0, const int z=0, const int v=0) const {
      return (*this)(x<0?0:(x>=dimx()?dimx()-1:x),y,z,v);
    }
    
    typename cimg::largest<T,float>::type linear_pix4d(const float ffx,const float ffy=0,const float ffz=0,const float ffv=0) const {
      const float
	fx = ffx<0?0:(ffx>width-1?width-1:ffx), fy = ffy<0?0:(ffy>height-1?height-1:ffy),
        fz = ffz<0?0:(ffz>depth-1?depth-1:ffz), fv = ffv<0?0:(ffv>dim-1?dim-1:ffv);
      const unsigned int x = (unsigned int)fx, y = (unsigned int)fy,  z = (unsigned int)fz, v = (unsigned int)fv;
      const float dx = fx-x, dy = fy-y, dz = fz-z, dv = fv-v;
      const unsigned int nx = dx>0?x+1:x, ny = dy>0?y+1:y,  nz = dz>0?z+1:z, nv = dv>0?v+1:v;
      const T
	&Icccc = (*this)(x,y,z,v),   &Inccc = (*this)(nx,y,z,v),   &Icncc = (*this)(x,ny,z,v),   &Inncc = (*this)(nx,ny,z,v),
	&Iccnc = (*this)(x,y,nz,v),  &Incnc = (*this)(nx,y,nz,v),  &Icnnc = (*this)(x,ny,nz,v),  &Innnc = (*this)(nx,ny,nz,v),
	&Icccn = (*this)(x,y,z,nv),  &Inccn = (*this)(nx,y,z,nv),  &Icncn = (*this)(x,ny,z,nv),  &Inncn = (*this)(nx,ny,z,nv),
	&Iccnn = (*this)(x,y,nz,nv), &Incnn = (*this)(nx,y,nz,nv), &Icnnn = (*this)(x,ny,nz,nv), &Innnn = (*this)(nx,ny,nz,nv);   
      return Icccc +
	dx*(Inccc-Icccc +
	    dy*(Icccc+Inncc-Icncc-Inccc +
		dz*(Iccnc+Innnc+Icncc+Inccc-Icnnc-Incnc-Icccc-Inncc +
		    dv*(Iccnn+Innnn+Icncn+Inccn+Icnnc+Incnc+Icccc+Inncc-Icnnn-Incnn-Icccn-Inncn-Iccnc-Innnc-Icncc-Inccc)) +
		dv*(Icccn+Inncn+Icncc+Inccc-Icncn-Inccn-Icccc-Inncc)) +
	    dz*(Icccc+Incnc-Iccnc-Inccc +
		dv*(Icccn+Incnn+Iccnc+Inccc-Iccnn-Inccn-Icccc-Incnc)) +
	    dv*(Icccc+Inccn-Inccc-Icccn)) +
	dy*(Icncc-Icccc +
	    dz*(Icccc+Icnnc-Iccnc-Icncc +
		dv*(Icccn+Icnnn+Iccnc+Icncc-Iccnn-Icncn-Icccc-Icnnc)) +
	    dv*(Icccc+Icncn-Icncc-Icccn)) +
	dz*(Iccnc-Icccc +
	    dv*(Icccc+Iccnn-Iccnc-Icccn)) +
	dv*(Icccn-Icccc);
    }

    typename cimg::largest<T,float>::type linear_pix3d(const float ffx,const float ffy=0,const float ffz=0,const int v=0) const {
      const float fx = ffx<0?0:(ffx>width-1?width-1:ffx), fy = ffy<0?0:(ffy>height-1?height-1:ffy), fz = ffz<0?0:(ffz>depth-1?depth-1:ffz);
      const unsigned int x = (unsigned int)fx, y = (unsigned int)fy, z = (unsigned int)fz;
      const float dx = fx-x, dy = fy-y, dz = fz-z;
      const unsigned int nx = dx>0?x+1:x, ny = dy>0?y+1:y, nz = dz>0?z+1:z;
      const T 
	&Iccc = (*this)(x,y,z,v),  &Incc = (*this)(nx,y,z,v),  &Icnc = (*this)(x,ny,z,v),  &Innc = (*this)(nx,ny,z,v),
	&Iccn = (*this)(x,y,nz,v), &Incn = (*this)(nx,y,nz,v), &Icnn = (*this)(x,ny,nz,v), &Innn = (*this)(nx,ny,nz,v);
      return Iccc +
	dx*(Incc-Iccc +
	    dy*(Iccc+Innc-Icnc-Incc +
		dz*(Iccn+Innn+Icnc+Incc-Icnn-Incn-Iccc-Innc)) +
	    dz*(Iccc+Incn-Iccn-Incc)) +
	dy*(Icnc-Iccc +
	    dz*(Iccc+Icnn-Iccn-Icnc)) +
	dz*(Iccn-Iccc);
    }

    //! Read a pixel value using linear interpolation for the two first coordinates (\c cx,\c cy).
    /**
       - Same as linear_pix4d(), except that linear interpolation and boundary checking is performed only on the two first coordinates.

       \sa operator()(), linear_pix4d(), linear_pix3d(), linear_pix1d(), linear_pix2d(), cubic_pix2d().
    **/
    typename cimg::largest<T,float>::type linear_pix2d(const float fx, const float fy, const int z, const int v,
						       const T& out_val) const {
      const int x = (int)fx-(fx>0?0:1), y = (int)fy-(fy>0?0:1), nx = x+1, ny = y+1;
      const float dx = fx-x, dy = fy-y;
      const T 
	Icc = pix2d(x,y,z,v,out_val),  Inc = pix2d(nx,y,z,v,out_val),
	Icn = pix2d(x,ny,z,v,out_val), Inn = pix2d(nx,ny,z,v,out_val);
      return Icc + dx*(Inc-Icc + dy*(Icc+Inn-Icn-Inc)) + dy*(Icn-Icc);
    }

    typename cimg::largest<T,float>::type linear_pix2d(const float ffx, const float ffy=0, const int z=0, const int v=0) const {
      const float fx = ffx<0?0:(ffx>width-1?width-1:ffx), fy = ffy<0?0:(ffy>height-1?height-1:ffy);
      const unsigned int x = (unsigned int)fx, y = (unsigned int)fy;
      const float dx = fx-x, dy = fy-y;
      const unsigned int nx = dx>0?x+1:x, ny = dy>0?y+1:y;
      const T &Icc = (*this)(x,y,z,v), &Inc = (*this)(nx,y,z,v), &Icn = (*this)(x,ny,z,v), &Inn = (*this)(nx,ny,z,v);
      return Icc + dx*(Inc-Icc + dy*(Icc+Inn-Icn-Inc)) + dy*(Icn-Icc);
    }

    typename cimg::largest<T,float>::type linear_pix1d(const float ffx,const int y=0,const int z=0,const int v=0) const {
      const float fx = ffx<0?0:(ffx>width-1?width-1:ffx);
      const unsigned int x = (unsigned int)fx;
      const float dx = fx-x;
      const unsigned int nx = dx>0?x+1:x;
      const T &Ic = (*this)(x,y,z,v), &In = (*this)(nx,y,z,v);
      return Ic + dx*(In-Ic);
    }

    // This function is used as a subroutine for cubic interpolation
    static float _cubic_R(const float x) {
      const float xp2 = x+2, xp1 = x+1, xm1 = x-1,
	nxp2 = xp2>0?xp2:0, nxp1 = xp1>0?xp1:0, nx = x>0?x:0, nxm1 = xm1>0?xm1:0;
      return (nxp2*nxp2*nxp2 - 4*nxp1*nxp1*nxp1 + 6*nx*nx*nx - 4*nxm1*nxm1*nxm1)/6.0f;
    }

    typename cimg::largest<T,float>::type cubic_pix1d(const float pfx,const int y=0,const int z=0,const int v=0) const {
      const float fx = pfx<0?0:(pfx>width-1?width-1:pfx);
      const unsigned int x = (unsigned int)fx, px = (int)x-1>=0?x-1:0, nx = x+1<width?x+1:width-1, ax = nx+1<width?nx+1:width-1;
      const float dx = fx-x;
      const T& a = (*this)(px,y,z,v), b = (*this)(x,y,z,v), c = (*this)(nx,y,z,v), d = (*this)(ax,y,z,v);
      const float Rxp = _cubic_R(-1-dx), Rxc = _cubic_R(dx), Rxn = _cubic_R(1-dx), Rxa = _cubic_R(2-dx);
      return Rxp*a + Rxc*b + Rxn*c + Rxa*d;
    }
   
    typename cimg::largest<T,float>::type cubic_pix2d(const float pfx,const float pfy=0,const int z=0,const int v=0) const {
      const float fx = pfx<0?0:(pfx>width-1?width-1:pfx), fy = pfy<0?0:(pfy>height-1?height-1:pfy);
      const unsigned int 
        x = (unsigned int)fx,  px = (int)x-1>=0?x-1:0, nx = x+1<width?x+1:width-1, ax = nx+1<width?nx+1:width-1,
        y = (unsigned int)fy,  py = (int)y-1>=0?y-1:0, ny = y+1<height?y+1:height-1, ay = ny+1<height?ny+1:height-1;
      const float dx = fx-x, dy = fy-y;
      const T& 
        a = (*this)(px,py,z,v), b = (*this)(x,py,z,v), c = (*this)(nx,py,z,v), d = (*this)(ax,py,z,v),
        e = (*this)(px, y,z,v), f = (*this)(x, y,z,v), g = (*this)(nx, y,z,v), h = (*this)(ax, y,z,v),
        i = (*this)(px,ny,z,v), j = (*this)(x,ny,z,v), k = (*this)(nx,ny,z,v), l = (*this)(ax,ny,z,v),
        m = (*this)(px,ay,z,v), n = (*this)(x,ay,z,v), o = (*this)(nx,ay,z,v), p = (*this)(ax,ay,z,v);
      const float
	Rxp = _cubic_R(-1-dx), Rxc = _cubic_R(dx), Rxn = _cubic_R(1-dx), Rxa = _cubic_R(2-dx),
	Ryp = _cubic_R(dy+1),  Ryc = _cubic_R(dy), Ryn = _cubic_R(dy-1), Rya = _cubic_R(dy-2);    
      return
	Rxp*Ryp*a + Rxc*Ryp*b + Rxn*Ryp*c + Rxa*Ryp*d +
	Rxp*Ryc*e + Rxc*Ryc*f + Rxn*Ryc*g + Rxa*Ryc*h +
	Rxp*Ryn*i + Rxc*Ryn*j + Rxn*Ryn*k + Rxa*Ryn*l +
	Rxp*Rya*m + Rxc*Rya*n + Rxn*Rya*o + Rxa*Rya*p;
    }

    //------------------------------------------
    //
    //! \name Arithmetic and Boolean Operators
    //@{
    //------------------------------------------
  
    //! Assign an image to the instance image.
    /**
       \param img Image to copy.
       
       - Replace the instance image by a copy of the image \c img.
       - The assignement is faster if input and output images have same template types.
       - Otherwise, pixel values are casted as in C.
       
       \par example:
       \code
       CImg<unsigned char> img("foo.jpg");     // Load image from a JPEG file.
       CImg<unsigned char> dest1;              // Define an empty image of unsigned char pixels.
       CImg<float> dest2;                      // Define an empty image of float pixels.
       dest1 = img;                            // Fast copy of img to dest1.
       dest2 = img;                            // Copy of img to dest2, with conversion of pixel to float values.
       \endcode
    **/

    //! Assign a value to each image pixel of the instance image.
    /**
       \param val Value to assign.

       - Replace all pixel values of the instance image by \c val.
       - Can be used to easily initialize an image.
       
       \par example:
       \code
       CImg<float> img(100,100);   // Define a 100x100 greyscale image.
       img = 3.14f;                // Set all pixel values to 3.14.
       \endcode
       
       \sa fill().
    **/
    CImg& operator=(const T& val) { 
      return fill(val); 
    }

    //! Assign values of a C-array to the instance image.
    /**
       \param buf Pointer to a C-style array having a size of (at least) <tt>this->size()</tt>.
       
       - Replace pixel values by the content of the array \c buf.
       - Warning : the value types in the array and in the image must be the same.
       
       \par example:
       \code
       float tab[4*4] = { 1,2,3,4, 5,6,7,8, 9,10,11,12, 13,14,15,16 };  // Define a 4x4 matrix in C-style.
       CImg<float> matrice(4,4);                                        // Define a 4x4 greyscale image.
       matrice = tab;                                                   // Fill the image by the values in tab.
       \endcode
    **/
    CImg& operator=(const T *buf) {
      if (buf) std::memcpy(data,buf,size()*sizeof(T));
      else assign();
      return *this; 
    }
      
    //! Addition.
    /**
       \param val Constant value added to each image pixel of the instance image.
    **/
    CImg operator+(const T& val) const { 
      return CImg<T>(*this,false)+=val; 
    }

    //! Addition.
    /**
       \param img Image added to the instance image.
    **/
    template<typename t> CImg<typename cimg::largest<T,t>::type> operator+(const CImg<t>& img) const {
      typedef typename cimg::largest<T,t>::type restype;
      return CImg<restype>(*this)+=img; 
    }

    //! Addition.
    friend CImg operator+(const T& val, const CImg<T>& img) {
      return CImg<T>(img)+=val; 
    }    

    //! In-place addition.
    /** This is the in-place version of operator+(). **/
    CImg& operator+=(const T& val) { 
      cimg_map(*this,ptr,T) (*ptr)+=val; 
      return *this; 
    }

    //! In-place addition.
    /** This is the in-place version of operator+(). **/
    template<typename t> CImg& operator+=(const CImg<t>& img) {
      const unsigned int smin = cimg::min(size(),img.size());
      t *ptrs = img.data+smin;
      for (T *ptrd = data+smin; ptrd>data; --ptrd, (*ptrd)=(T)((*ptrd)+(*(--ptrs)))) ;
      return *this;
    }

    //! In-place increment.
    /** Equivalent to \c operator+=(1). **/
    CImg& operator++() { 
      return (*this)+=1; 
    }

    //! Substraction.
    /**
       \param val Constant value substracted to each image pixel of the instance image.      
    **/
    CImg operator-(const T& val) const { 
      return CImg<T>(*this,false)-=val; 
    }

    //! Substraction.
    /**
       \param img Image substracted to the instance image.
    **/
    template<typename t> CImg<typename cimg::largest<T,t>::type> operator-(const CImg<t>& img) const {
      typedef typename cimg::largest<T,t>::type restype;
      return CImg<restype>(*this)-=img; 
    }

    //! Substraction.
    friend CImg operator-(const T& val, const CImg<T>& img) { 
      return CImg<T>(img.width,img.height,img.depth,img.dim,val)-=img; 
    }

    //! In-place substraction.
    /** This is the in-place version of operator-(). **/
    CImg& operator-=(const T& val) {
      cimg_map(*this,ptr,T) (*ptr)-=val;
      return *this; 
    }

    //! In-place substraction.
    /** This is the in-place version of operator-(). **/
    template<typename t> CImg& operator-=(const CImg<t>& img) {
      const unsigned int smin = cimg::min(size(),img.size());
      t *ptrs = img.data+smin;
      for (T *ptrd = data+smin; ptrd>data; --ptrd, (*ptrd)=(T)((*ptrd)-(*(--ptrs)))) ;
      return *this;
    }

    //! In-place decrement.
    /** Equivalent to \c operator-=(1). **/
    CImg& operator--() {
      return (*this)-=1; 
    }

    //! Multiplication.
    /**
       \param val Constant value multiplied to each image pixel of the instance image.
    **/
    CImg operator*(const double val) const {
      return CImg<T>(*this,false)*=val; 
    }

    //! Multiplication.
    /**
       Matrix multiplication.
    **/
    template<typename t> CImg<typename cimg::largest<T,t>::type> operator*(const CImg<t>& img) const {
      typedef typename cimg::largest<T,t>::type restype;
      if (width!=img.height) 
        throw CImgArgumentException("CImg<%s>::operator*() : can't multiply a matrix *this = (%ux%u) by a matrix (%ux%u)",
                                    pixel_type(),width,height,img.width,img.height);
      CImg<restype> res(img.width,height);
      restype val;
      cimg_mapXY(res,i,j) { val=0; cimg_mapX(*this,k) val+=(*this)(k,j)*img(i,k); res(i,j) = val; }
      return res;
    }

    //! Matrix multiplication with an image list
    template<typename t> CImgl<typename cimg::largest<T,t>::type> operator*(const CImgl<t>& list) const {
      typedef typename cimg::largest<T,t>::type restype;
      CImgl<restype> res(list.size);
      cimgl_map(res,l) res[l] = (*this)*list[l];
      return res;
    }
    
    //! Multiplication.
    friend CImg operator*(const double val, const CImg &img) {
      return CImg<T>(img)*=val; 
    }

    //! In-place multiplication.
    /** This is the in-place version of operator*(). **/
    CImg& operator*=(const double val) { 
      cimg_map(*this,ptr,T) (*ptr)=(T)((*ptr)*val);
      return *this; 
    }

    //! In-place multiplication.
    /** This is the in-place version of operator*(). **/
    template<typename t> CImg& operator*=(const CImg<t>& img) { 
      return ((*this)*img).swap(*this); 
    }
  
    //! Division.
    /**
       \param val Constant value divided to each image pixel of the instance image.
    **/
    CImg operator/(const double val) const {
      return CImg<T>(*this,false)/=val; 
    }
    
    //! In-place division.
    /** This is the in-place version of operator/(). **/
    CImg& operator/=(const double val) {
      cimg_map(*this,ptr,T) (*ptr)=(T)((*ptr)/val);
      return *this; 
    }

    //! Modulo.
    /**
       \param val Constant valued used for a modulo on each image pixel.
    **/
    CImg operator%(const T& val) const { 
      return CImg<T>(*this,false)%=val; 
    }  

    //! Modulo.
    /**
       \param img Image whose values are used for a modulo.
    **/
    CImg operator%(const CImg& img) const {
      return CImg<T>(*this,false)%=img; 
    }

    //! In-place modulo.
    /** This is the in-place version of operator%(). **/
    CImg& operator%=(const T& val) { cimg_map(*this,ptr,T) (*ptr)%=val; return *this; }
 
    //! In-place modulo.
    /** This is the in-place version of operator%(). **/
    CImg& operator%=(const CImg& img) {
      const unsigned int smin = cimg::min(size(),img.size());
      for (T *ptrs=img.data+smin, *ptrd=data+smin; ptrd>data; *(--ptrd)%=*(--ptrs)) ;
      return *this;
    }

    //! Bitwise AND.
    /**
       \param val Constant valued used for a bitwise AND on each image pixel.
    **/
    CImg operator&(const T& val) const {
      return CImg<T>(*this,false)&=val; 
    }
   
    //! Bitwise AND.
    /**
       \param img Image whose value are used for the AND.
    **/
    CImg operator&(const CImg& img) const { 
      return CImg<T>(*this,false)&=img; 
    }

    //! In-place bitwise AND.
    /** This is the in-place version of operator&(). **/
    CImg& operator&=(const T& val) {
      cimg_map(*this,ptr,T) (*ptr)&=val;
      return *this; 
    }

    //! In-place bitwise AND.
    /** This is the in-place version of operator&=(). **/
    CImg& operator&=(const CImg& img) {
      const unsigned int smin = cimg::min(size(),img.size());
      for (T *ptrs=img.data+smin, *ptrd=data+smin; ptrd>data; *(--ptrd)&=*(--ptrs)) ;
      return *this;
    }

    //! Bitwise OR.
    /**
       \param val Constant valued used for a bitwise OR on each image pixel.
    **/
    CImg operator|(const T& val) const {
      return CImg<T>(*this,false)|=val; 
    }
    
    //! Bitwise OR.
    /**
       \param img Image whose values are used for the OR.
    **/
    CImg operator|(const CImg& img) const { 
      return CImg<T>(*this,false)|=img; 
    }

    //! In-place bitwise OR.
    /** This is the in-place version of operator|(). **/
    CImg& operator|=(const T& val) { 
      cimg_map(*this,ptr,T) (*ptr)|=val;
      return *this; 
    }

    //! In-place bitwise OR.
    /** This is the in-place version of operator|(). **/
    CImg& operator|=(const CImg& img) {
      const unsigned int smin = cimg::min(size(),img.size());
      for (T *ptrs=img.data+smin, *ptrd=data+smin; ptrd>data; *(--ptrd)|=*(--ptrs)) ;
      return *this;
    }

    //! Bitwise XOR.
    /**
       \param val Constant valued used for a bitwise XOR on each image pixel.
    **/
    CImg operator^(const T& val) const { 
      return CImg<T>(*this,false)^=val; 
    }

    //! Bitwise XOR.
    /**
       \param img Image whose values are used for the XOR.
     **/
    CImg operator^(const CImg& img) const { 
      return CImg<T>(*this,false)^=img; 
    }  

    //! In-place bitwise XOR.
    /** This is the in-place version of operator^(). **/
    CImg& operator^=(const T& val) {
      cimg_map(*this,ptr,T) (*ptr)^=val;
      return *this; 
    }

    //! In-place bitwise XOR.
    /** This is the in-place version of operator^(). **/
    CImg& operator^=(const CImg& img) {
      const unsigned int smin = cimg::min(size(),img.size());
      for (T *ptrs=img.data+smin, *ptrd=data+smin; ptrd>data; *(--ptrd)^=*(--ptrs)) ;
      return *this;
    }
   
    //! Boolean NOT.
    CImg operator!() const {
      CImg<T> res(width,height,depth,dim);
      const T *ptrs = end();
      cimg_map(res,ptrd,T) *ptrd=!(*(--ptrs));
      return res;
    }

    //! Bitwise NOT.
    CImg operator~() const {
      CImg<T> res(width,height,depth,dim);
      const T *ptrs = end();
      cimg_map(res,ptrd,T) *ptrd=~(*(--ptrs));
      return res;
    }

    //! Boolean equality.
    template<typename t> bool operator==(const CImg<t>& img) const {
      const unsigned int siz = size();
      bool vequal = true;
      if (siz!=img.size()) return false;
      t *ptrs=img.data+siz;
      for (T *ptrd=data+siz; vequal && ptrd>data; vequal=vequal&&((*(--ptrd))==(*(--ptrs)))) ;
      return vequal;
    }

    //! Boolean difference.
    template<typename t> bool operator!=(const CImg<t>& img) const { return !((*this)==img); }
  
    //@}
    //-----------------------------------
    //
    //! \name Usual Image Transformations
    //@{
    //-----------------------------------
    
    CImg& fill(const T& val) {
      if (!is_empty()) {
	if (val!=0 && sizeof(T)!=1) cimg_map(*this,ptr,T) *ptr=val;
	else std::memset(data,(int)val,size()*sizeof(T));
      }
      return *this;
    }

    CImg& fill(const T& val0,const T& val1) {
      if (!is_empty()) {
	T *ptr, *ptr_end = end()-1;
	for (ptr=data; ptr<ptr_end; ) { *(ptr++)=val0; *(ptr++)=val1; }
	if (ptr!=ptr_end+1) *(ptr++)=val0;
      }
      return *this;
    }

    CImg& fill(const T& val0,const T& val1,const T& val2) {
      if (!is_empty()) {
	T *ptr, *ptr_end = end()-2;
	for (ptr=data; ptr<ptr_end; ) { *(ptr++)=val0; *(ptr++)=val1; *(ptr++)=val2; }     
	ptr_end+=2;
	switch (ptr_end-ptr) {
	case 2: *(--ptr_end)=val1;
	case 1: *(--ptr_end)=val0;
	}
      }
      return *this;
    }

    CImg& fill(const T& val0,const T& val1,const T& val2,const T& val3) {
      if (!is_empty()) {
	T *ptr, *ptr_end = end()-3;
	for (ptr=data; ptr<ptr_end; ) { *(ptr++)=val0; *(ptr++)=val1; *(ptr++)=val2; *(ptr++)=val3; }
	ptr_end+=3;
	switch (ptr_end-ptr) {
	case 3: *(--ptr_end)=val2;
	case 2: *(--ptr_end)=val1;
	case 1: *(--ptr_end)=val0;
	}
      }
      return *this;
    }

    CImg& fill(const T& val0,const T& val1,const T& val2,const T& val3,const T& val4) {
      if (!is_empty()) {
	T *ptr, *ptr_end = end()-4;
	for (ptr=data; ptr<ptr_end; ) { *(ptr++)=val0; *(ptr++)=val1; *(ptr++)=val2; *(ptr++)=val3; *(ptr++)=val4; }
	ptr_end+=4;
	switch (ptr_end-ptr) {
	case 4: *(--ptr_end)=val3;
	case 3: *(--ptr_end)=val2;
	case 2: *(--ptr_end)=val1;
	case 1: *(--ptr_end)=val0;
	}
      }
      return *this;
    }

    CImg& fill(const T& val0,const T& val1,const T& val2,const T& val3,const T& val4,const T& val5) {
      if (!is_empty()) {
	T *ptr, *ptr_end = end()-5;
	for (ptr=data; ptr<ptr_end; ) {
	  *(ptr++)=val0; *(ptr++)=val1; *(ptr++)=val2; *(ptr++)=val3; *(ptr++)=val4; *(ptr++)=val5; 
	}
	ptr_end+=5;
	switch (ptr_end-ptr) {
	case 5: *(--ptr_end)=val4;
	case 4: *(--ptr_end)=val3;
	case 3: *(--ptr_end)=val2;
	case 2: *(--ptr_end)=val1;
	case 1: *(--ptr_end)=val0;
	}
      }
      return *this;
    }

    CImg& fill(const T& val0,const T& val1,const T& val2,const T& val3,
	       const T& val4,const T& val5,const T& val6) {
      if (!is_empty()) {
	T *ptr, *ptr_end = end()-6; 
	for (ptr=data; ptr<ptr_end; ) {
	  *(ptr++)=val0; *(ptr++)=val1; *(ptr++)=val2; *(ptr++)=val3; *(ptr++)=val4; *(ptr++)=val5; *(ptr++)=val6;
	}
	ptr_end+=6;
	switch (ptr_end-ptr) {
	case 6: *(--ptr_end)=val5;
	case 5: *(--ptr_end)=val4;
	case 4: *(--ptr_end)=val3;
	case 3: *(--ptr_end)=val2;
	case 2: *(--ptr_end)=val1;
	case 1: *(--ptr_end)=val0;
	}
      }
      return *this;
    }

    CImg& fill(const T& val0,const T& val1,const T& val2,const T& val3,
	       const T& val4,const T& val5,const T& val6,const T& val7) {
      if (!is_empty()) {
	T *ptr, *ptr_end = end()-7;
	for (ptr=data; ptr<ptr_end; ) {
	  *(ptr++)=val0; *(ptr++)=val1; *(ptr++)=val2; *(ptr++)=val3;
	  *(ptr++)=val4; *(ptr++)=val5; *(ptr++)=val6; *(ptr++)=val7;
	}
	ptr_end+=7;
	switch (ptr_end-ptr) {
	case 7: *(--ptr_end)=val6;
	case 6: *(--ptr_end)=val5;
	case 5: *(--ptr_end)=val4;
	case 4: *(--ptr_end)=val3;
	case 3: *(--ptr_end)=val2;
	case 2: *(--ptr_end)=val1;
	case 1: *(--ptr_end)=val0;
	}
      }
      return *this;
    }

    CImg& fill(const T& val0,const T& val1,const T& val2,
	       const T& val3,const T& val4,const T& val5,
	       const T& val6,const T& val7,const T& val8) {
      if (!is_empty()) {
	T *ptr, *ptr_end = end()-8;
	for (ptr=data; ptr<ptr_end; ) {
	  *(ptr++)=val0; *(ptr++)=val1; *(ptr++)=val2; 
	  *(ptr++)=val3; *(ptr++)=val4; *(ptr++)=val5; 
	  *(ptr++)=val6; *(ptr++)=val7; *(ptr++)=val8;
	}
	ptr_end+=8;
	switch (ptr_end-ptr) {
	case 8: *(--ptr_end)=val7;
	case 7: *(--ptr_end)=val6;
	case 6: *(--ptr_end)=val5;
	case 5: *(--ptr_end)=val4;
	case 4: *(--ptr_end)=val3;
	case 3: *(--ptr_end)=val2;
	case 2: *(--ptr_end)=val1;
	case 1: *(--ptr_end)=val0;
	}
      }
      return *this;
    }

    //! Linear normalization of the pixel values between \a a and \a b.
    /**
       \param a = minimum pixel value after normalization.
       \param b = maximum pixel value after normalization.
       \see get_normalize(), cut(), get_cut().
    **/
    CImg& normalize(const T& a,const T& b) {
      if (!is_empty()) {
	const CImgStats st(*this,false);
	if (st.min==st.max) return fill(0);
	if (st.min!=a || st.max!=b) cimg_map(*this,ptr,T) *ptr=(T)((*ptr-st.min)/(st.max-st.min)*(b-a)+a);
      }
      return *this;
    }

    //! Cut pixel values between \a a and \a b.
    /**
       \param a = minimum pixel value after cut.
       \param b = maximum pixel value after cut.
       \see get_cut(), normalize(), get_normalize().
    **/
    CImg& cut(const T& a, const T& b) {
      if (!is_empty())
	cimg_map(*this,ptr,T) *ptr = (*ptr<a)?a:((*ptr>b)?b:*ptr);
      return *this;
    }

    //! Quantize pixel values into \n levels.
    /**
       \param n = number of quantification levels
       \see get_quantize().
    **/
    CImg& quantize(const unsigned int n=256) {
      if (!is_empty()) {
	if (!n) throw CImgArgumentException("CImg<%s>::quantize() : Cannot quantize image to 0 values.",
					    pixel_type());       
	const CImgStats st(*this,false);
	const double range = st.max-st.min;
	if (range>0) cimg_map(*this,ptr,T) {
	  const unsigned int val = (unsigned int)((*ptr-st.min)*n/range);
	  *ptr = (T)(st.min + cimg::min(val,n-1)*range);
	}
      }
      return *this;
    }
    
    //! Threshold the image.
    /**
       \param thres = threshold
       \see get_threshold().
    **/	
    CImg& threshold(const T& thres) {
      if (!is_empty()) cimg_map(*this,ptr,T) *ptr = *ptr<=thres?(T)0:(T)1;
      return *this;
    }

    //! Return a resized image.
    /**
       \param pdx = Number of columns (new size along the X-axis).
       \param pdy = Number of rows (new size along the Y-axis).
       \param pdz = Number of slices (new size along the Z-axis).
       \param pdv = Number of vector-channels (new size along the V-axis).
       \param interp = Resizing type :
       - 0 = no interpolation : additionnal space is filled with 0.
       - 1 = bloc interpolation (nearest point).
       - 2 = mosaic : image is repeated if necessary.
       - 3 = linear interpolation.
       - 4 = grid interpolation.
       - 5 = bi-cubic interpolation.
       \note If pd[x,y,z,v]<0, it corresponds to a percentage of the original size (the default value is -100).
    **/
    CImg get_resize(const int pdx=-100, const int pdy=-100, const int pdz=-100, const int pdv=-100,
		    const unsigned int interp=1) const {
      if (!pdx || !pdy || !pdz || !pdv) return CImg<T>();
      const unsigned int 
	dx = pdx<0?-pdx*width/100:pdx,
	dy = pdy<0?-pdy*height/100:pdy,
	dz = pdz<0?-pdz*depth/100:pdz, 
	dv = pdv<0?-pdv*dim/100:pdv;
      CImg res(dx?dx:1,dy?dy:1,dz?dz:1,dv?dv:1);
      if (is_empty()) return res.fill(0);
      if (width==res.width && height==res.height && depth==res.depth && dim==res.dim) return *this;
      switch (interp) {
      case 0:  // Zero filling
	res.fill(0).draw_image(*this,0,0,0,0);
	break;
      case 1: { // Nearest-neighbor interpolation
	unsigned int 
	  *const offx = new unsigned int[res.width],
	  *const offy = new unsigned int[res.height+1],
	  *const offz = new unsigned int[res.depth+1],
	  *const offv = new unsigned int[res.dim+1],
	  *poffx, *poffy, *poffz, *poffv;
	const unsigned int 
	  wh = width*height,
	  whd = width*height*depth,
	  rwh = res.width*res.height,
	  rwhd = res.width*res.height*res.depth;
	float s, curr, old;
	s = (float)width/res.width;
	poffx = offx; curr=0; { cimg_mapX(res,x) { old=curr; curr+=s; *(poffx++) = (unsigned int)curr-(unsigned int)old; }}
	s = (float)height/res.height;
	poffy = offy; curr=0; { cimg_mapY(res,y) { old=curr; curr+=s; *(poffy++) = width*((unsigned int)curr-(unsigned int)old); }} *poffy=0;
	s = (float)depth/res.depth;
	poffz = offz; curr=0; { cimg_mapZ(res,z) { old=curr; curr+=s; *(poffz++) = wh*((unsigned int)curr-(unsigned int)old); }} *poffz=0;
	s = (float)dim/res.dim;
	poffv = offv; curr=0; { cimg_mapV(res,v) { old=curr; curr+=s; *(poffv++) = whd*((unsigned int)curr-(unsigned int)old); }} *poffv=0;
       
	T *ptrd = res.ptr();	
	const T* ptrv = ptr();
	poffv = offv;
	for (unsigned int k=0; k<res.dim; ) {
	  const T *ptrz = ptrv;
	  poffz = offz;
	  for (unsigned int z=0; z<res.depth; ) {
	    const T *ptry = ptrz;
	    poffy = offy;
	    for (unsigned int y=0; y<res.height; ) {
	      const T *ptrx = ptry;
	      poffx = offx;
	      cimg_mapX(res,x) { *(ptrd++)=*ptrx; ptrx+=*(poffx++); }
	      y++;
	      unsigned int dy=*(poffy++);
	      for (;!dy && y<res.height; std::memcpy(ptrd, ptrd-res.width, sizeof(T)*res.width), y++, ptrd+=res.width, dy=*(poffy++)) ;
	      ptry+=dy;
	    }
	    z++;
	    unsigned int dz=*(poffz++);
	    for (;!dz && z<res.depth; std::memcpy(ptrd, ptrd-rwh, sizeof(T)*rwh), z++, ptrd+=rwh, dz=*(poffz++)) ;
	    ptrz+=dz;
	  }
	  k++;
	  unsigned int dv=*(poffv++);
	  for (;!dv && k<res.dim; std::memcpy(ptrd, ptrd-rwhd, sizeof(T)*rwhd), k++, ptrd+=rwhd, dv=*(poffv++)) ;
	  ptrv+=dv;
	}
	delete[] offx; delete[] offy; delete[] offz; delete[] offv;
      }	break;
      case 2: { // Mosaic filling
	for (unsigned int k=0; k<res.dim; k+=dim)
	  for (unsigned int z=0; z<res.depth; z+=depth)
	    for (unsigned int y=0; y<res.height; y+=height)
	      for (unsigned int x=0; x<res.width; x+=width) res.draw_image(*this,x,y,z,k);
      } break;
      case 3: { // Linear interpolation
	const bool bx = (res.width<width), by = (res.height<height), bz = (res.depth<depth), bk = (res.dim<dim);
	const float
	  sx = bx?(width-1.0f)/(res.width+1):(res.width>1?(width-1.0f)/(res.width-1):0),
	  sy = by?(height-1.0f)/(res.height+1):(res.height>1?(height-1.0f)/(res.height-1):0),
	  sz = bz?(depth-1.0f)/(res.depth+1):(res.depth>1?(depth-1.0f)/(res.depth-1):0),
	  sk = bk?(dim-1.0f)/(res.dim+1):(res.dim>1?(dim-1.0f)/(res.dim-1):0),
	  dx = bx?sx:0, dy = by?sy:0, dz = bz?sz:0, dk = bk?sk:0;       
	float cx,cy,cz,ck=dk;
	cimg_mapV(res,k) { cz = dz;
	cimg_mapZ(res,z) { cy = dy;
	cimg_mapY(res,y) { cx = dx;
	cimg_mapX(res,x) { res(x,y,z,k) = (T)linear_pix4d(cx,cy,cz,ck); cx+=sx;
	} cy+=sy;
	} cz+=sz;
	} ck+=sk;
	}
      } break;
      case 4: { // Grid filling
	res.fill(0);
	cimg_mapXYZV(*this,x,y,z,k) res(x*res.width/width,
					y*res.height/height,
					z*res.depth/depth,
					k*res.dim/dim) = (*this)(x,y,z,k);
      } break;
      case 5: { // Cubic interpolation
	const bool bx = (res.width<width), by = (res.height<height), bz = (res.depth<depth), bk = (res.dim<dim);
	const float
	  sx = bx?(width-1.0f)/(res.width+1):(res.width>1?(width-1.0f)/(res.width-1):0),
	  sy = by?(height-1.0f)/(res.height+1):(res.height>1?(height-1.0f)/(res.height-1):0),
	  sz = bz?(depth-1.0f)/(res.depth+1):(res.depth>1?(depth-1.0f)/(res.depth-1):0),
	  sk = bk?(dim-1.0f)/(res.dim+1):(res.dim>1?(dim-1.0f)/(res.dim-1):0),
	  dx = bx?sx:0, dy = by?sy:0, dz = bz?sz:0, dk = bk?sk:0;       
	float cx,cy,cz,ck=dk;
	cimg_mapV(res,k) { cz = dz;
	cimg_mapZ(res,z) { cy = dy;
	cimg_mapY(res,y) { cx = dx;
	cimg_mapX(res,x) { res(x,y,z,k) = (T)cubic_pix2d(cx,cy,(int)cz,(int)ck); cx+=sx;
	} cy+=sy;
	} cz+=sz;
	} ck+=sk;
	}
      } break;      
      }
      return res;
    }

    //! Return a resized image.
    /**
       \param src = Image giving the geometry of the resize.
       \param interp = Resizing type :
       - 0 = no interpolation : additionnal space is filled with 0.
       - 1 = bloc interpolation (nearest point).
       - 2 = mosaic : image is repeated if necessary.
       - 3 = linear interpolation.
       - 4 = grid interpolation.
       - 5 = bi-cubic interpolation.
       \note If pd[x,y,z,v]<0, it corresponds to a percentage of the original size (the default value is -100).
    **/
    template<typename t> CImg get_resize(const CImg<t>& src,const unsigned int interp=1) const {
      return get_resize(src.width,src.height,src.depth,src.dim,interp); 
    }  

    //! Resize the image.
    /**
       \param pdx = Number of columns (new size along the X-axis).
       \param pdy = Number of rows (new size along the Y-axis).
       \param pdz = Number of slices (new size along the Z-axis).
       \param pdv = Number of vector-channels (new size along the V-axis).
       \param interp = Resizing type :
       - 0 = no interpolation : additionnal space is filled with 0.
       - 1 = bloc interpolation (nearest point).
       - 2 = mosaic : image is repeated if necessary.
       - 3 = linear interpolation.
       - 4 = grid interpolation.
       - 5 = bi-cubic interpolation.
       \note If pd[x,y,z,v]<0, it corresponds to a percentage of the original size (the default value is -100).       
    **/
    CImg& resize(const int pdx=-100,const int pdy=-100,const int pdz=-100,const int pdv=-100,const unsigned int interp=1) {
      if (!pdx || !pdy || !pdz || !pdv) return assign();
      const unsigned int 
	dx = pdx<0?-pdx*width/100:pdx,
	dy = pdy<0?-pdy*height/100:pdy,
	dz = pdz<0?-pdz*depth/100:pdz, 
	dv = pdv<0?-pdv*dim/100:pdv;
      if (width==dx && height==dy && depth==dz && dim==dv) return *this;
      return get_resize(dx,dy,dz,dv,interp).swap(*this);
    }

    //! Resize the image.
    /**
       \param src = Image giving the geometry of the resize.
       \param interp = Resizing type :
       - 0 = no interpolation : additionnal space is filled with 0.
       - 1 = bloc interpolation (nearest point).
       - 2 = mosaic : image is repeated if necessary.
       - 3 = linear interpolation.
       - 4 = grid interpolation.
       - 5 = bi-cubic interpolation.
       \note If pd[x,y,z,v]<0, it corresponds to a percentage of the original size (the default value is -100).
    **/
    template<typename t> CImg& resize(const CImg<t>& src,const unsigned int interp=1) { 
      return resize(src.width,src.height,src.depth,src.dim,interp); 
    }

    //! Resize the image
    /**
       \param disp = Display giving the geometry of the resize.
       \param interp = Resizing type :
       - 0 = no interpolation : additionnal space is filled with 0.
       - 1 = bloc interpolation (nearest point).
       - 2 = mosaic : image is repeated if necessary.
       - 3 = linear interpolation.
       - 4 = grid interpolation.
       - 5 = bi-cubic interpolation.
       \note If pd[x,y,z,v]<0, it corresponds to a percentage of the original size (the default value is -100).
    **/
    CImg& resize(const CImgDisplay& disp,const unsigned int interp=1) {
      return resize(disp.width,disp.height,depth,dim,interp);
    }

    //! Return an half-resized image, using a special filter.
    /**
       \see resize_halfXY(), resize(), get_resize().
    **/
    CImg get_resize_halfXY() const {
      typedef typename cimg::largest<T,float>::type ftype;
      if (is_empty()) return CImg<T>();
      CImg<ftype> mask = CImg<ftype>::matrix(0.07842776544f, 0.1231940459f, 0.07842776544f,
					     0.1231940459f,  0.1935127547f, 0.1231940459f,
					     0.07842776544f, 0.1231940459f, 0.07842776544f);
      CImg_3x3x1(I,ftype);
      CImg dest(width/2,height/2,depth,dim);
      cimg_mapZV(*this,z,k) cimg_map3x3x1(*this,x,y,z,k,I) 
	if (x%2 && y%2) dest(x/2,y/2,z,k) = (T)cimg_conv3x3x1(I,mask);
      return dest;
    }

    //! Return a square region of the image, as a new image
    /**
       \param x0 = X-coordinate of the upper-left crop rectangle corner.
       \param y0 = Y-coordinate of the upper-left crop rectangle corner.
       \param z0 = Z-coordinate of the upper-left crop rectangle corner.
       \param v0 = V-coordinate of the upper-left crop rectangle corner.
       \param x1 = X-coordinate of the lower-right crop rectangle corner.
       \param y1 = Y-coordinate of the lower-right crop rectangle corner.
       \param z1 = Z-coordinate of the lower-right crop rectangle corner.
       \param v1 = V-coordinate of the lower-right crop rectangle corner.
       \param border_condition = Dirichlet (false) or Neumann border conditions.
       \see crop()
    **/
    CImg get_crop(const unsigned int x0,const unsigned int y0,const unsigned int z0,const unsigned int v0,
		  const unsigned int x1,const unsigned int y1,const unsigned int z1,const unsigned int v1,
		  const bool border_condition = false) const {
      if (is_empty()) 
      	throw CImgInstanceException("CImg<%s>::get_crop() : Instance image (%u,%u,%u,%u,%p) is empty.",
				    pixel_type(),width,height,depth,dim,data);
      const unsigned int dx=x1-x0+1, dy=y1-y0+1, dz=z1-z0+1, dv=v1-v0+1;
      CImg dest(dx,dy,dz,dv);
      if (x0>=width || x1>=width || y0>=height || y1>=height || z0>=depth || z1>=depth ||
	  v0>=dim || v1>=dim || x1<x0 || y1<y0 || z1<z0 || v1<v0)
	switch (border_condition) {
	case false: { cimg_mapXYZV(dest,x,y,z,v) dest(x,y,z,v) = pix4d(x0+x,y0+y,z0+z,v0+v,0); } break;
	default: { cimg_mapXYZV(dest,x,y,z,v) dest(x,y,z,v) = pix4d(x0+x,y0+y,z0+z,v0+v); } break;
	} else {
	  const T *psrc = ptr(x0,y0,z0,v0);
	  T *pdest = dest.ptr(0,0,0,0);
	  if (dx!=width)
	    for (unsigned int k=0; k<dv; k++) {
	      for (unsigned int z=0; z<dz; z++) {
		for (unsigned int y=0; y<dy; y++) {
		  std::memcpy(pdest,psrc,dx*sizeof(T));
		  pdest+=dx;
		  psrc+=width;
		}
		psrc+=width*(height-dy);
	      }
	      psrc+=width*height*(depth-dz);
	    }
	  else {
	    if (dy!=height)         
	      for (unsigned int k=0; k<dv; k++) {
		for (unsigned int z=0; z<dz; z++) {
		  std::memcpy(pdest,psrc,dx*dy*sizeof(T));
		  pdest+=dx*dy;
		  psrc+=width*height;
		}
		psrc+=width*height*(depth-dz);
	      }
	    else {
	      if (dz!=depth)
		for (unsigned int k=0; k<dv; k++) {
		  std::memcpy(pdest,psrc,dx*dy*dz*sizeof(T));
		  pdest+=dx*dy*dz;
		  psrc+=width*height*depth;
		}
	      else std::memcpy(pdest,psrc,dx*dy*dz*dv*sizeof(T));
	    }
	  }
	}
      return dest;
    }
    //! Return a square region of the image, as a new image
    /**
       \param x0 = X-coordinate of the upper-left crop rectangle corner.
       \param y0 = Y-coordinate of the upper-left crop rectangle corner.
       \param x1 = X-coordinate of the lower-right crop rectangle corner.
       \param y1 = Y-coordinate of the lower-right crop rectangle corner.
       \param border_condition = determine the type of border condition if
       some of the desired region is outside the image.
       \see crop()   
    **/
    CImg get_crop(const unsigned int x0,const unsigned int y0,
		  const unsigned int x1,const unsigned int y1,
		  const bool border_condition=false) const {
      return get_crop(x0,y0,0,0,x1,y1,depth-1,dim-1,border_condition);
    }

    //! Return a square region of the image, as a new image
    /**
       \param x0 = X-coordinate of the upper-left crop rectangle corner.
       \param x1 = X-coordinate of the lower-right crop rectangle corner.
       \param border_condition = determine the type of border condition if
       some of the desired region is outside the image.
       \see crop()   
    **/
    CImg get_crop(const unsigned int x0,const unsigned int x1,const bool border_condition=false) const {
      return get_crop(x0,0,0,0,x1,height-1,depth-1,dim-1,border_condition); 
    }

    //! Replace the image by a square region of the image
    /**
       \param x0 = X-coordinate of the upper-left crop rectangle corner.
       \param y0 = Y-coordinate of the upper-left crop rectangle corner.
       \param z0 = Z-coordinate of the upper-left crop rectangle corner.
       \param v0 = V-coordinate of the upper-left crop rectangle corner.
       \param x1 = X-coordinate of the lower-right crop rectangle corner.
       \param y1 = Y-coordinate of the lower-right crop rectangle corner.
       \param z1 = Z-coordinate of the lower-right crop rectangle corner.
       \param v1 = V-coordinate of the lower-right crop rectangle corner.
       \param border_condition = determine the type of border condition if
       some of the desired region is outside the image.
       \see get_crop()
    **/
    CImg& crop(const unsigned int x0,const unsigned int y0,const unsigned int z0,const unsigned int v0,
	       const unsigned int x1,const unsigned int y1,const unsigned int z1,const unsigned int v1,
	       const bool border_condition=false) {
      return get_crop(x0,y0,z0,v0,x1,y1,z1,v1,border_condition).swap(*this);
    }

    //! Replace the image by a square region of the image
    /**
       \param x0 = X-coordinate of the upper-left crop rectangle corner.
       \param y0 = Y-coordinate of the upper-left crop rectangle corner.
       \param z0 = Z-coordinate of the upper-left crop rectangle corner.
       \param x1 = X-coordinate of the lower-right crop rectangle corner.
       \param y1 = Y-coordinate of the lower-right crop rectangle corner.
       \param z1 = Z-coordinate of the lower-right crop rectangle corner.
       \param border_condition = determine the type of border condition if
       some of the desired region is outside the image.
       \see get_crop()
    **/
    CImg& crop(const unsigned int x0,const unsigned int y0,const unsigned int z0,
	       const unsigned int x1,const unsigned int y1,const unsigned int z1,
	       const bool border_condition=false) {
      return crop(x0,y0,z0,0,x1,y1,z1,dim-1,border_condition);
    }

    //! Replace the image by a square region of the image
    /**
       \param x0 = X-coordinate of the upper-left crop rectangle corner.
       \param y0 = Y-coordinate of the upper-left crop rectangle corner.
       \param x1 = X-coordinate of the lower-right crop rectangle corner.
       \param y1 = Y-coordinate of the lower-right crop rectangle corner.
       \param border_condition = determine the type of border condition if
       some of the desired region is outside the image.
       \see get_crop()
    **/
    CImg& crop(const unsigned int x0,const unsigned int y0,
	       const unsigned int x1,const unsigned int y1,
	       const bool border_condition=false) { 
      return crop(x0,y0,0,0,x1,y1,depth-1,dim-1,border_condition); 
    }

    //! Get a shared-memory image referencing a set of points of the instance image (const version).
    const CImg get_shared_points(const unsigned int x0, const unsigned int x1,
				 const unsigned int y0=0, const unsigned int z0=0, const unsigned int v0=0) const {
      const unsigned long beg = offset(x0,y0,z0,v0), end = offset(x1,y0,z0,v0);
      if (beg>end || beg>=size() || end>=size())
	throw CImgArgumentException("CImg<%s>::get_shared_points() : Cannot return a shared-memory subset (%u->%u,%u,%u,%u) from "
				    "a (%u,%u,%u,%u) image.",pixel_type(),x0,x1,y0,z0,v0,width,height,depth,dim);
      return CImg<T>(data+beg,x1-x0+1,1,1,1,true);
    }

    //! Get a copy of a set of points of the instance image.
    CImg get_points(const unsigned int x0, const unsigned int x1,
		    const unsigned int y0=0, const unsigned int z0=0, const unsigned int v0=0) const {      
      const unsigned long beg = offset(x0,y0,z0,v0), end = offset(x1,y0,z0,v0);
      if (beg>end || beg>=size() || end>=size())
	throw CImgArgumentException("CImg<%s>::get_points() : Cannot return a shared-memory subset (%u->%u,%u,%u,%u) from "
				    "a (%u,%u,%u,%u) image.",pixel_type(),x0,x1,y0,z0,v0,width,height,depth,dim);
      return CImg<T>(data+beg,x1-x0+1,1,1,1,false);
    }

    //! Return a shared-memory image referencing a set of lines of the instance image.
    CImg get_shared_lines(const unsigned int y0, const unsigned int y1,
			  const unsigned int z0=0, const unsigned int v0=0) {
      const unsigned long beg = offset(0,y0,z0,v0), end = offset(0,y1,z0,v0);
      if (beg>end || beg>=size() || end>=size())
	throw CImgArgumentException("CImg<%s>::get_shared_lines() : Cannot return a shared-memory subset (0->%u,%u->%u,%u,%u) from "
				    "a (%u,%u,%u,%u) image.",pixel_type(),width-1,y0,y1,z0,v0,width,height,depth,dim);
      return CImg<T>(data+beg,width,y1-y0+1,1,1,true);
    }
    
    //! Get a copy of a set of lines of the instance image.
    CImg get_lines(const unsigned int y0, const unsigned int y1,
		   const unsigned int z0=0, const unsigned int v0=0) const {
      const unsigned long beg = offset(0,y0,z0,v0), end = offset(0,y1,z0,v0);
      if (beg>end || beg>=size() || end>=size())
	throw CImgArgumentException("CImg<%s>::get_lines() : Cannot return a shared-memory subset (0->%u,%u->%u,%u,%u) from "
				    "a (%u,%u,%u,%u) image.",pixel_type(),width-1,y0,y1,z0,v0,width,height,depth,dim);
      return CImg<T>(data+beg,width,y1-y0+1,1,1,false);
    }

    //! Replace the instance image by a set of lines of the instance image.
    CImg& lines(const unsigned int y0, const unsigned int y1,
		const unsigned int z0=0, const unsigned int v0=0) const {
      return get_lines(y0,y1,z0,v0).swap(*this);
    }
    
    //! Return a shared-memory image referencing one particular line (y0,z0,v0) of the instance image.
    CImg get_shared_line(const unsigned int y0, const unsigned int z0=0, const unsigned int v0=0) { 
      return get_shared_lines(y0,y0,z0,v0);
    }

    //! Return a shared-memory image referencing one particular line (y0,z0,v0) of the instance image (const version).
    const CImg get_shared_line(const unsigned int y0,const unsigned int z0=0,const unsigned int v0=0) const { 
      return get_shared_lines(y0,y0,z0,v0);
    }
    
    //! Return a shared memory image referencing a set of planes (z0->z1,v0) of the instance image.
    CImg get_shared_planes(const unsigned int z0, const unsigned int z1, const unsigned int v0=0) {
      const unsigned long beg = offset(0,0,z0,v0), end = offset(0,0,z1,v0);
      if (beg>end || beg>=size() || end>=size())
	throw CImgArgumentException("CImg<%s>::get_shared_planes() : Cannot return a shared-memory subset (0->%u,0->%u,%u->%u,%u) from "
				    "a (%u,%u,%u,%u) image.",pixel_type(),width-1,height-1,z0,z1,v0,width,height,depth,dim);
      return CImg<T>(data+beg,width,height,z1-z0+1,1,true);
    }

    //! Return a shared-memory image referencing a set of planes (z0->z1,v0) of the instance image (const version).
    const CImg get_shared_planes(const unsigned int z0, const unsigned int z1, const unsigned int v0=0) const {
      const unsigned long beg = offset(0,0,z0,v0), end = offset(0,0,z1,v0);
      if (beg>end || beg>=size() || end>=size())
	throw CImgArgumentException("CImg<%s>::get_shared_planes() : Cannot return a shared-memory subset (0->%u,0->%u,%u->%u,%u) from "
				    "a (%u,%u,%u,%u) image.",pixel_type(),width-1,height-1,z0,z1,v0,width,height,depth,dim);
      return CImg<T>(data+beg,width,height,z1-z0+1,1,true);
    }
    
    //! Return a shared-memory image referencing a set of channels (v0->v1) of the instance image.
    CImg get_shared_channels(const unsigned int v0, const unsigned int v1) { 
      const unsigned long beg = offset(0,0,0,v0), end = offset(0,0,0,v1);
      if (beg>end || beg>=size() || end>=size())
	throw CImgArgumentException("CImg<%s>::get_shared_channels() : Cannot return a shared-memory subset (0->%u,0->%u,0->%u,%u->%u) from "
				    "a (%u,%u,%u,%u) image.",pixel_type(),width-1,height-1,depth-1,v0,v1,width,height,depth,dim);
      return CImg<T>(data+beg,width,height,depth,v1-v0+1,true);
    }
    
    //! Return a shared-memory image referencing a set of channels (v0->v1) of the instance image (const version).
    const CImg get_shared_channels(const unsigned int v0, const unsigned int v1) const { 
      const unsigned long beg = offset(0,0,0,v0), end = offset(0,0,0,v1);
      if (beg>end || beg>=size() || end>=size())
	throw CImgArgumentException("CImg<%s>::get_shared_channels() : Cannot return a shared-memory subset (0->%u,0->%u,0->%u,%u->%u) from "
				    "a (%u,%u,%u,%u) image.",pixel_type(),width-1,height-1,depth-1,v0,v1,width,height,depth,dim);
      return CImg<T>(data+beg,width,height,depth,v1-v0+1,true);
    }
    
    //! Return a shared-memory image referencing one channel v0 of the instance image.
    CImg get_shared_channel(const unsigned int v0) { 
      return get_shared_channels(v0,v0);
    }

    //! Return a shared-memory image referencing one channel v0 of the instance image (const version).
    const CImg get_shared_channel(const unsigned int v0) const { 
      return get_shared_channels(v0,v0);
    }

    //! Return a 2D representation of a 3D image, with three slices.
    CImg get_projections2d(const unsigned int px0,const unsigned int py0,const unsigned int pz0) const {
      if (is_empty()) return CImg<T>();
      const unsigned int
	x0=(px0>=width)?width-1:px0,
	y0=(py0>=height)?height-1:py0,
	z0=(pz0>=depth)?depth-1:pz0;
      CImg res(width+depth,height+depth,1,dim);
      res.fill((*this)[0]);
      { cimg_mapXYV(*this,x,y,k) res(x,y,0,k)        = (*this)(x,y,z0,k); }
      { cimg_mapYZV(*this,y,z,k) res(width+z,y,0,k)  = (*this)(x0,y,z,k); }
      { cimg_mapXZV(*this,x,z,k) res(x,height+z,0,k) = (*this)(x,y0,z,k); }
      return res;
    }

    //! Return the image histogram.
    /**
       The histogram H of an image I is a 1D-function where H(x) is the number of
       occurences of the value x in I.
       \param nblevels = Number of different levels of the computed histogram.
       For classical images, this value is 256 (default value). You should specify more levels
       if you are working with CImg<float> or images with high range of pixel values.
       \param val_min = Minimum value considered for the histogram computation. All pixel values lower than val_min
       won't be counted.
       \param val_max = Maximum value considered for the histogram computation. All pixel values higher than val_max
       won't be counted.
       \note If val_min==val_max==0 (default values), the function first estimates the minimum and maximum
       pixel values of the current image, then uses these values for the histogram computation.
       \result The histogram is returned as a 1D CImg<float> image H, having a size of (nblevels,1,1,1) such that
       H(0) and H(nblevels-1) are respectively equal to the number of occurences of the values val_min and val_max in I.
       \note Histogram computation always returns a 1D function. Histogram of multi-valued (such as color) images
       are not multi-dimensional.
       \see get_equalize_histogram(), equalize_histogram()
    **/
    CImg<float> get_histogram(const unsigned int nblevels=256,const T val_min=(T)0,const T val_max=(T)0) const {
      if (is_empty()) return CImg<float>();
      if (nblevels<1)
	throw CImgArgumentException("CImg<%s>::get_histogram() : Can't compute an histogram with %u levels",
				    pixel_type(),nblevels);
      T vmin=val_min, vmax=val_max;
      CImg<float> res(nblevels,1,1,1,0);
       if (vmin==vmax && vmin==0) { const CImgStats st(*this,false); vmin = (T)st.min; vmax = (T)st.max; }
      cimg_map(*this,ptr,T) {
	const int pos = (int)((*ptr-vmin)*(nblevels-1)/(vmax-vmin));
	if (pos>=0 && pos<(int)nblevels) res[pos]++; 
      }
      return res;
    }
    
    //! Equalize the image histogram
    /** This is the in-place version of \ref get_equalize_histogram() **/
    CImg& equalize_histogram(const unsigned int nblevels=256,const T val_min=(T)0,const T val_max=(T)0) {
      if (!is_empty()) {
	T vmin=val_min, vmax=val_max;
 	if (vmin==vmax && vmin==0) { const CImgStats st(*this,false); vmin = (T)st.min; vmax = (T)st.max; }
	CImg<float> hist = get_histogram(nblevels,vmin,vmax);
	float cumul=0;
	cimg_mapX(hist,pos) { cumul+=hist[pos]; hist[pos]=cumul; }
	cimg_map(*this,ptr,T) {
	  const int pos = (unsigned int)((*ptr-vmin)*(nblevels-1)/(vmax-vmin));
	  if (pos>=0 && pos<(int)nblevels) *ptr = (T)(vmin + (vmax-vmin)*hist[pos]/size());
	}
      }
      return *this;
    }

    //! Return the scalar image of vector norms.
    /**
       When dealing with vector-valued images (i.e images with dimv()>1), this function computes the L1,L2 or Linf norm of each
       vector-valued pixel.
       \param norm_type = Type of the norm being computed (1 = L1, 2 = L2, -1 = Linf).
       \return A scalar-valued image CImg<float> with size (dimx(),dimy(),dimz(),1), where each pixel is the norm
       of the corresponding pixels in the original vector-valued image.
       \see get_orientation_pointwise, orientation_pointwise, norm_pointwise.
    **/
    CImg<typename cimg::largest<T,float>::type> get_norm_pointwise(int norm_type=2) const {
      typedef typename cimg::largest<T,float>::type restype;
      if (is_empty()) return CImg<restype>();
      CImg<restype> res(width,height,depth);
      switch(norm_type) {
      case -1: {             // Linf norm
	cimg_mapXYZ(*this,x,y,z) {
	  restype n=0; cimg_mapV(*this,v) {
	    const restype tmp = (restype)cimg::abs((*this)(x,y,z,v));
	    if (tmp>n) n=tmp; res(x,y,z) = n;
	  }
	}
      } break;
      case 1: {              // L1 norm
	cimg_mapXYZ(*this,x,y,z) {
	  restype n=0; cimg_mapV(*this,v) n+=cimg::abs((*this)(x,y,z,v)); res(x,y,z) = n;
	}
      } break;
      default: {             // L2 norm
	cimg_mapXYZ(*this,x,y,z) {
	  restype n=0; cimg_mapV(*this,v) n+=(*this)(x,y,z,v)*(*this)(x,y,z,v); res(x,y,z) = (restype)std::sqrt((double)n);
	}
      } break;
      }
      return res;
    }

    //! Append an image to another one
    CImg get_append(const CImg<T>& img, const char axis='x', const char align='c') const {
      if (img.is_empty()) return *this;
      if (is_empty()) return img;
      CImgl<T> temp(2);
      temp[0].width = width; temp[0].height = height; temp[0].depth = depth;
      temp[0].dim = dim; temp[0].data = data;
      temp[1].width = img.width; temp[1].height = img.height; temp[1].depth = img.depth;
      temp[1].dim = img.dim; temp[1].data = img.data;
      const CImg<T> res = temp.get_append(axis,align);
      temp[0].width = temp[0].height = temp[0].depth = temp[0].dim = 0; temp[0].data = 0;
      temp[1].width = temp[1].height = temp[1].depth = temp[1].dim = 0; temp[1].data = 0;
      return res;
    }

    //! Append an image to another one (in-place version)
    CImg& append(const CImg<T>& img, const char axis='x', const char align='c') {
      if (img.is_empty()) return *this;
      if (is_empty()) return (*this=img);
      return get_append(img,axis,align).swap(*this);
    }

    //! Append an image to another one (in-place operator<< version)
    CImg& operator<<(const CImg<T>& img) {
      return append(img);
    }

    //@}
    //----------------------------
    //
    //! \name Color conversions
    //@{
    //----------------------------

    //! Return the default 256 colors palette.
    /**
       The default color palette is used by %CImg when displaying images on 256 colors displays.
       It consists in the quantification of the (R,G,B) color space using 3:3:2 bits for color coding
       (i.e 8 levels for the Red and Green and 4 levels for the Blue).
       \return A 256x1x1x3 color image defining the palette entries.
    **/
    static CImg<T> get_default_LUT8() {
      static CImg<T> palette;
      if (!palette.data) {
	palette.assign(256,1,1,3);
	for (unsigned int index=0, r=16; r<256; r+=32)
	  for (unsigned int g=16; g<256; g+=32)
	    for (unsigned int b=32; b<256; b+=64) {
	      palette(index,0) = r;
	      palette(index,1) = g;
	      palette(index++,2) = b;
	    }
      }
      return palette;
    }
    
    //! Convert color pixels from (R,G,B) to match a specified palette.
    /**
       This function return a (R,G,B) image where colored pixels are constrained to match entries
       of the specified color \c palette.
       \param palette User-defined palette that will constraint the color conversion.
       \param dithering Enable/Disable Floyd-Steinberg dithering.
       \param indexing If \c true, each resulting image pixel is an index to the given color palette.
       Otherwise, (R,G,B) values of the palette are copied instead.
    **/
    template<typename t> CImg<t> get_RGBtoLUT(const CImg<t>& palette, const bool dithering=true,const bool indexing=false) const {
      if (is_empty()) return CImg<t>();
      if (dim!=3) throw CImgInstanceException("CImg<%s>::RGBtoLUT() : Input image dimension is dim=%u, "
					      "should be a (R,G,B) image.",pixel_type(),dim);
      if (palette.data && palette.dim!=3)
	throw CImgArgumentException("CImg<%s>::RGBtoLUT() : Given palette dimension is dim=%u, "
				    "should be a (R,G,B) palette",pixel_type(),palette.dim);
      CImg<t> res(width,height,depth,indexing?1:3), pal = palette.data?palette:CImg<t>::get_default_LUT8();
      float *line1 = new float[3*width], *line2 = new float[3*width], *pline1 = line1, *pline2 = line2;
      cimg_mapZ(*this,z) {
	float *ptr=pline2; cimg_mapX(*this,x) { *(ptr++)=(*this)(x,0,z,0); *(ptr++)=(*this)(x,0,z,1); *(ptr++)=(*this)(x,0,z,2); }
	cimg_mapY(*this,y) {
	  cimg::swap(pline1,pline2);
	  if (y<dimy()-1) {
	    const int ny = y+1;
	    float *ptr=pline2; cimg_mapX(*this,x) { *(ptr++)=(*this)(x,ny,z,0); *(ptr++)=(*this)(x,ny,z,1); *(ptr++)=(*this)(x,ny,z,2); }
	  }
	  float *ptr1=pline1, *ptr2=pline2;
	  cimg_mapX(*this,x) {
	    float R = *(ptr1++), G = *(ptr1++), B = *(ptr1++);
	    R = R<0?0:(R>255?255:R); G = G<0?0:(G>255?255:G); B = B<0?0:(B>255?255:B);
	    int best_index = 0;
	    t Rbest=0,Gbest=0,Bbest=0;
	    if (palette.data) {	// find best match in given color palette
	      float min = cimg::type<float>::max();
	      cimg_mapX(palette,off) {
		const t Rp = palette(off,0), Gp = palette(off,1), Bp = palette(off,2);
		const float error = (float)((Rp-R)*(Rp-R) + (Gp-G)*(Gp-G) + (Bp-B)*(Bp-B));
		if (error<min) { min=error; best_index=off; Rbest=Rp; Gbest=Gp; Bbest=Bp; }
	      }
	    } else {
	      Rbest = (t)((unsigned char)R&0xe0); Gbest = (t)((unsigned char)G&0xe0); Bbest = (t)((unsigned char)B&0xc0);
	      best_index = (unsigned char)Rbest | ((unsigned char)Gbest>>3) | ((unsigned char)Bbest>>6);
	    }
	    if (indexing) res(x,y,z) = best_index;
	    else { res(x,y,z,0) = Rbest; res(x,y,z,1) = Gbest; res(x,y,z,2) = Bbest; }
	    if (dithering) { // apply dithering to neighborhood pixels if needed
	      const float dR = (float)(R-Rbest), dG = (float)(G-Gbest),	dB = (float)(B-Bbest);
	      if (x<dimx()-1) { *(ptr1++)+= dR*7/16; *(ptr1++)+= dG*7/16; *(ptr1++)+= dB*7/16; ptr1-=3; }
	      if (y<dimy()-1) {	
		*(ptr2++)+= dR*5/16; *(ptr2++)+= dG*5/16; *ptr2+= dB*5/16; ptr2-=2;
		if (x>0) { *(--ptr2)+= dB*3/16; *(--ptr2)+= dG*3/16; *(--ptr2)+= dR*3/16; ptr2+=3; }
		if (x<dimx()-1) { ptr2+=3; *(ptr2++)+= dR/16; *(ptr2++)+= dG/16; *ptr2+= dB/16; ptr2-=5; }
	      }
	    }
	    ptr2+=3;
	  }
	}
      }
      delete[] line1; delete[] line2;
      return res;
    }

    //! Convert color pixels from (R,G,B) to match the default 256 colors palette.
    /**
       Same as get_RGBtoLUT() with the default color palette given by get_default_LUT8().
    **/
    CImg<T> get_RGBtoLUT(const bool dithering=true, const bool indexing=false) const {
      CImg<T> foo;
      return get_RGBtoLUT(foo,dithering,indexing); 
    }
    
    //! Convert color pixels from (R,G,B) to match the specified color palette.
    /** This is the in-place version of get_RGBtoLUT(). **/
    CImg& RGBtoLUT(const CImg<T>& palette,const bool dithering=true,const bool indexing=false) {
      return get_RGBtoLUT(palette,dithering,indexing).swap(*this);
    }

    //! Convert color pixels from (R,G,B) to match the specified color palette.
    /** This is the in-place version of get_RGBtoLUT(). **/
    CImg& RGBtoLUT(const bool dithering=true,const bool indexing=false) {
      CImg<T> foo;
      return get_RGBtoLUT(foo,dithering,indexing).swap(*this); 
    }
    
    //! Convert an indexed image to a (R,G,B) image using the specified color palette.    
    template<typename t> CImg<t> get_LUTtoRGB(const CImg<t>& palette) const {
      if (is_empty()) return CImg<t>();
      if (dim!=1) throw CImgInstanceException("CImg<%s>::LUTtoRGB() : Input image dimension is dim=%u, "
					      "should be a LUT image",pixel_type(),dim);
      if (palette.data && palette.dim!=3) 
	throw CImgArgumentException("CImg<%s>::LUTtoRGB() : Given palette dimension is dim=%u, "
				    "should be a (R,G,B) palette",pixel_type(),palette.dim);
      CImg<t> res(width,height,depth,3);
      CImg<t> pal = palette.data?palette:get_default_LUT8();      
      cimg_mapXYZ(*this,x,y,z) {
	const unsigned int index = (unsigned int)(*this)(x,y,z);
	res(x,y,z,0) = pal(index,0);
	res(x,y,z,1) = pal(index,1);
	res(x,y,z,2) = pal(index,2);
      }
      return res;
    }

    //! Convert an indexed image (with the default palette) to a (R,G,B) image.
    CImg<T> get_LUTtoRGB() const { 
      CImg<T> foo;
      return get_LUTtoRGB(foo); 
    }

    //! In-place version of get_LUTtoRGB().
    CImg& LUTtoRGB(const CImg<T>& palette) { 
      return get_LUTtoRGB(palette).swap(*this); 
    }

    //! In-place version of get_LUTroRGB().
    CImg& LUTtoRGB() { 
      CImg<T> foo;
      return get_LUTtoRGB(foo).swap(*this); 
    }

    //! Convert color pixels from (R,G,B) to (H,S,V).
    CImg& RGBtoHSV() {
      if (!is_empty()) {
	if (dim!=3) throw CImgInstanceException("CImg<%s>::RGBtoHSV() : Input image dimension is dim=%u, "
						"should be a (R,G,B) image.",pixel_type(),dim);
	cimg_mapXYZ(*this,x,y,z) {
	  const float
	    R = (float)((*this)(x,y,z,0)/255.0f),
	    G = (float)((*this)(x,y,z,1)/255.0f),
	    B = (float)((*this)(x,y,z,2)/255.0f);
	  const float m = cimg::min(R,G,B), v = cimg::max(R,G,B);
	  float h,s;
	  if (v==m) { h=-1; s=0; } else {
	    const float 
	      f = (R==m)?(G-B):((G==m)?(B-R):(R-G)),
	      i = (R==m)?3.0f:((G==m)?5.0f:1.0f);
	    h = (i-f/(v-m));
	    s = (v-m)/v;
	    if (h>=6.0f) h-=6.0f;
	    h*=(float)cimg::PI/3.0f;
	  }
	  (*this)(x,y,z,0) = (T)h;
	  (*this)(x,y,z,1) = (T)s;
	  (*this)(x,y,z,2) = (T)v;
	}
      }
      return *this;
    }

    //! Convert color pixels from (H,S,V) to (R,G,B).
    CImg& HSVtoRGB() {
      if (!is_empty()) {
	if (dim!=3) throw CImgInstanceException("CImg<%s>::HSVtoRGB() : Input image dimension is dim=%u, "
						"should be a (H,S,V) image",pixel_type(),dim);
	cimg_mapXYZ(*this,x,y,z) {
	  float
	    H = (float)((*this)(x,y,z,0)),
	    S = (float)((*this)(x,y,z,1)),
	    V = (float)((*this)(x,y,z,2));
	  float R=0,G=0,B=0;
	  if (H<0) R=G=B=V;
	  else {
	    H/=(float)cimg::PI/3.0f;
	    const int i = (int)std::floor(H);
	    const float
	      f = (i&1)?(H-i):(1.0f-H+i),
	      m = V*(1.0f-S),
	      n = V*(1.0f-S*f);
	    switch(i) {
	    case 6:
	    case 0: R=V; G=n; B=m; break;
	    case 1: R=n; G=V; B=m; break;
	    case 2: R=m; G=V; B=n; break;
	    case 3: R=m; G=n; B=V; break;
	    case 4: R=n; G=m; B=V; break;
	    case 5: R=V; G=m; B=n; break;
	    }
	  }
	  (*this)(x,y,z,0) = (T)(R*255.0f);
	  (*this)(x,y,z,1) = (T)(G*255.0f);
	  (*this)(x,y,z,2) = (T)(B*255.0f);
	}
      }
      return *this;
    }
    
    //! Convert color pixels from (R,G,B) to (Y,Cb,Cr)_8 (Thanks to Chen Wang).
    CImg& RGBtoYCbCr() {
      if (!is_empty()) {
	if (dim!=3) throw CImgInstanceException("CImg<%s>::RGBtoYCbCr() : Input image dimension is dim=%u, "
						"should be a (R,G,B) image (dim=3)",pixel_type(),dim);
	cimg_mapXYZ(*this,x,y,z) {
	  const int 
	    R = (int)((*this)(x,y,z,0)),
	    G = (int)((*this)(x,y,z,1)),
	    B = (int)((*this)(x,y,z,2));
	  const int
	    Y  = ((66*R+129*G+25*B+128)>>8) + 16,
	    Cb = ((-38*R-74*G+112*B+128)>>8) + 128,
	    Cr = ((112*R-94*G-18*B+128)>>8) + 128;
	  (*this)(x,y,z,0) = (T)(Y<0?0:(Y>255?255:Y));
	  (*this)(x,y,z,1) = (T)(Cb<0?0:(Cb>255?255:Cb));
	  (*this)(x,y,z,2) = (T)(Cr<0?0:(Cr>255?255:Cr));
	}
      }
      return *this;
    }
    
    //! Convert color pixels from (Y,Cb,Cr)_8 to (R,G,B).
    CImg& YCbCrtoRGB() {
      if (!is_empty()) {
	if (dim!=3) throw CImgInstanceException("CImg<%s>::YCbCrtoRGB() : Input image dimension is dim=%u, "
						"should be a (Y,Cb,Cr)_8 image (dim=3)",pixel_type(),dim);
	cimg_mapXYZ(*this,x,y,z) {
	  const int 
	    Y  = (int)((*this)(x, y, z, 0)-16), 
	    Cb = (int)((*this)(x, y, z, 1)-128),
	    Cr = (int)((*this)(x, y, z, 2)-128);
	  const int 
	    R = ((298*Y + 409*Cr + 128) >> 8 ),
	    G = ((298*Y - 100*Cb - 208*Cr + 128) >> 8 ),
	    B = ((298*Y + 516*Cb + 128) >> 8 );
	  (*this)(x,y,z,0) = (T)(R<0?0:(R>255?255:R));
	  (*this)(x,y,z,1) = (T)(G<0?0:(G>255?255:G));
	  (*this)(x,y,z,2) = (T)(B<0?0:(B>255?255:B));
	}
      }
      return *this;
    }
      
    //! Convert color pixels from (R,G,B) to (Y,U,V).
    CImg& RGBtoYUV() {
      if (!is_empty()) {
	if (dim!=3) throw CImgInstanceException("CImg<%s>::RGBtoYUV() : Input image dimension is dim=%u, "
						"should be a (R,G,B) image (dim=3)",pixel_type(),dim);
	cimg_mapXYZ(*this,x,y,z) {
	  const float
	    R = (*this)(x,y,z,0)/255.0f,
	    G = (*this)(x,y,z,1)/255.0f,
	    B = (*this)(x,y,z,2)/255.0f,
	    Y = (T)(0.299*R + 0.587*G + 0.114*B);
	  (*this)(x,y,z,0) = Y;
	  (*this)(x,y,z,1) = (T)(0.492*(B-Y));
	  (*this)(x,y,z,2) = (T)(0.877*(R-Y));
	}
      }
      return *this;
    }

    //! Convert color pixels from (Y,U,V) to (R,G,B).
    CImg& YUVtoRGB() {
      if (!is_empty()) {
	if (dim!=3) throw CImgInstanceException("CImg<%s>::YUVtoRGB() : Input image dimension is dim=%u, "
						"should be a (Y,U,V) image (dim=3)",pixel_type(),dim);
	cimg_mapXYZ(*this,x,y,z) {
	  const T Y = (*this)(x,y,z,0), U = (*this)(x,y,z,1), V = (*this)(x,y,z,2);
	  (*this)(x,y,z,0) = (T)((Y + 1.140*V)*255.0f);
	  (*this)(x,y,z,1) = (T)((Y - 0.395*U - 0.581*V)*255.0f);
	  (*this)(x,y,z,2) = (T)((Y + 2.032*U)*255.0f);
	}
      }
      return *this;
    }  

    //! Convert color pixels from (R,G,B) to (X,Y,Z)_709.    
    CImg& RGBtoXYZ() {
      if (!is_empty()) {
	if (dim!=3) throw CImgInstanceException("CImg<%s>::RGBtoXYZ() : Input image dimension is dim=%u, "
						"should be a (R,G,B) image (dim=3)",pixel_type(),dim);
	cimg_mapXYZ(*this,x,y,z) {
	  const float
	    R = (float)((*this)(x,y,z,0)/255.0f),
	    G = (float)((*this)(x,y,z,1)/255.0f),
	    B = (float)((*this)(x,y,z,2)/255.0f);
	  (*this)(x,y,z,0) = (T)(0.412453*R + 0.357580*G + 0.180423*B);
	  (*this)(x,y,z,1) = (T)(0.212671*R + 0.715160*G + 0.072169*B);
	  (*this)(x,y,z,2) = (T)(0.019334*R + 0.119193*G + 0.950227*B);
	}
      }
      return *this;
    }

    //! Convert (X,Y,Z)_709 pixels of a color image into the (R,G,B) color space.
    CImg& XYZtoRGB() {
      if (!is_empty()) {
	if (dim!=3) throw CImgInstanceException("CImg<%s>::XYZtoRGB() : Input image dimension is dim=%u, "
						"should be a (X,Y,Z) image (dim=3)",pixel_type(),dim);
	cimg_mapXYZ(*this,x,y,z) {
	  const float 
	    X = (float)(255.0f*(*this)(x,y,z,0)),
	    Y = (float)(255.0f*(*this)(x,y,z,1)),
	    Z = (float)(255.0f*(*this)(x,y,z,2));
	  (*this)(x,y,z,0) = (T)(3.240479*X  - 1.537150*Y - 0.498535*Z);
	  (*this)(x,y,z,1) = (T)(-0.969256*X + 1.875992*Y + 0.041556*Z);
	  (*this)(x,y,z,2) = (T)(0.055648*X  - 0.204043*Y + 1.057311*Z);
	}
      }
      return *this;
    }

    //! Convert (X,Y,Z)_709 pixels of a color image into the (L*,a*,b*) color space.
#define cimg_Labf(x)  ((x)>=0.008856?(std::pow(x,1/3.0)):(7.787*(x)+16.0/116.0))
#define cimg_Labfi(x) ((x)>=0.206893?((x)*(x)*(x)):(((x)-16.0/116.0)/7.787))

    CImg& XYZtoLab() {
      if (!is_empty()) {
	if (dim!=3) throw CImgInstanceException("CImg<%s>::XYZtoLab() : Input image dimension is dim=%u, "
						"should be a (X,Y,Z) image (dim=3)",pixel_type(),dim);
	const double
	  Xn = 0.412453 + 0.357580 + 0.180423,
	  Yn = 0.212671 + 0.715160 + 0.072169,
	  Zn = 0.019334 + 0.119193 + 0.950227;
	cimg_mapXYZ(*this,x,y,z) {
	  const T X = (*this)(x,y,z,0), Y = (*this)(x,y,z,1), Z = (*this)(x,y,z,2);
	  const double
	    XXn = X/Xn, YYn = Y/Yn, ZZn = Z/Zn,
	    fX = cimg_Labf(XXn), fY = cimg_Labf(YYn), fZ = cimg_Labf(ZZn);
	  (*this)(x,y,z,0) = (T)(116*fY-16);
	  (*this)(x,y,z,1) = (T)(500*(fX-fY));
	  (*this)(x,y,z,2) = (T)(200*(fY-fZ));
	}
      }
      return *this;
    }

    //! Convert (L,a,b) pixels of a color image into the (X,Y,Z) color space.
    CImg& LabtoXYZ() {
      if (!is_empty()) {
	if (dim!=3) throw CImgInstanceException("CImg<%s>::LabtoXYZ() : Input image dimension is dim=%u, "
						"should be a (X,Y,Z) image (dim=3)",pixel_type(),dim);
	const double
	  Xn = 0.412453 + 0.357580 + 0.180423,
	  Yn = 0.212671 + 0.715160 + 0.072169,
	  Zn = 0.019334 + 0.119193 + 0.950227;
	cimg_mapXYZ(*this,x,y,z) {
	  const T L = (*this)(x,y,z,0), a = (*this)(x,y,z,1), b = (*this)(x,y,z,2);
	  const double
	    cY = (L+16)/116.0,
	    Y = Yn*cimg_Labfi(cY),
	    pY = std::pow(Y/Yn,1.0/3),
	    cX = a/500+pY,
	    X = Xn*cX*cX*cX,
	    cZ = pY-b/200,
	    Z = Zn*cZ*cZ*cZ;
	  (*this)(x,y,z,0) = (T)(X);
	  (*this)(x,y,z,1) = (T)(Y);
	  (*this)(x,y,z,2) = (T)(Z);
	}
      }
      return *this;
    }

    //! Convert (X,Y,Z)_709 pixels of a color image into the (x,y,Y) color space.
    CImg& XYZtoxyY() {
      if (!is_empty()) {
	if (dim!=3) throw CImgInstanceException("CImg<%s>::XYZtoxyY() : Input image dimension is dim=%u, "
						"should be a (X,Y,Z) image (dim=3)",pixel_type(),dim);
	cimg_mapXYZ(*this,x,y,z) {
	  const T X = (*this)(x,y,z,0), Y = (*this)(x,y,z,1), Z = (*this)(x,y,z,2), sum = (X+Y+Z), nsum = sum>0?sum:1;
	  (*this)(x,y,z,0) = X/nsum;
	  (*this)(x,y,z,1) = Y/nsum;
	  (*this)(x,y,z,2) = Y;
	}
      }
      return *this;
    }
    
    //! Convert (x,y,Y) pixels of a color image into the (X,Y,Z)_709 color space.
    CImg& xyYtoXYZ() {
      if (!is_empty()) {
	if (dim!=3) throw CImgInstanceException("CImg<%s>::xyYtoXYZ() : Input image dimension is dim=%u, "
						"should be a (x,y,Y) image (dim=3)",pixel_type(),dim);
	cimg_mapXYZ(*this,x,y,z) {
	  const T px = (*this)(x,y,z,0), py = (*this)(x,y,z,1), Y = (*this)(x,y,z,2), ny = py>0?py:1;
	  (*this)(x,y,z,0) = (T)(px*Y/ny);
	  (*this)(x,y,z,1) = Y;
	  (*this)(x,y,z,2) = (T)((1-px-py)*Y/ny);
	}
      }
      return *this;
    }
    
    //! In-place version of get_RGBtoLab().
    CImg& RGBtoLab() { 
      return RGBtoXYZ().XYZtoLab(); 
    }

    //! In-place version of get_LabtoRGb().
    CImg& LabtoRGB() {
      return LabtoXYZ().XYZtoRGB(); 
    }

    //! In-place version of get_RGBtoxyY().
    CImg& RGBtoxyY() { 
      return RGBtoXYZ().XYZtoxyY(); 
    }

    //! In-place version of get_xyYtoRGB().
    CImg& xyYtoRGB() {
      return xyYtoXYZ().XYZtoRGB(); 
    }

    //! Convert a (R,G,B) image to a (H,S,V) one.
    CImg get_RGBtoHSV() const { 
      return CImg<T>(*this,false).RGBtoHSV(); 
    }

    //! Convert a (H,S,V) image to a (R,G,B) one.
    CImg get_HSVtoRGB() const { 
      return CImg<T>(*this,false).HSVtoRGB(); 
    }

    //! Convert a (R,G,B) image to a (Y,Cb,Cr) one.
    CImg get_RGBtoYCbCr() const { 
      return CImg<T>(*this,false).RGBtoYCbCr(); 
    }

    //! Convert a (Y,Cb,Cr) image to a (R,G,B) one.
    CImg get_YCbCrtoRGB() const {
      return CImg<T>(*this,false).YCbCrtoRGB(); 
    }

    //! Convert a (R,G,B) image into a (Y,U,V) one.
    CImg<typename cimg::largest<T,float>::type> get_RGBtoYUV() const {
      typedef typename cimg::largest<T,float>::type restype;
      return CImg<restype>(*this,false).RGBtoYUV();
    }
    
    //! Convert a (Y,U,V) image into a (R,G,B) one.
    CImg get_YUVtoRGB() const {
      return CImg<T>(*this,false).YUVtoRGB(); 
    }

    //! Convert a (R,G,B) image to a (X,Y,Z) one.
    CImg<typename cimg::largest<T,float>::type> get_RGBtoXYZ() const { 
      typedef typename cimg::largest<T,float>::type restype;
      return CImg<restype>(*this,false).RGBtoXYZ(); 
    }
    
    //! Convert a (X,Y,Z) image to a (R,G,B) one.
    CImg get_XYZtoRGB() const { 
      return CImg<T>(*this,false).XYZtoRGB(); 
    }

    //! Convert a (X,Y,Z) image to a (L,a,b) one.
    CImg get_XYZtoLab() const {
      return CImg<T>(*this,false).XYZtoLab(); 
    }

    //! Convert a (L,a,b) image to a (X,Y,Z) one.
    CImg get_LabtoXYZ() const {
      return CImg<T>(*this,false).LabtoXYZ(); 
    }

    //! Convert a (X,Y,Z) image to a (x,y,Y) one.
    CImg get_XYZtoxyY() const {
      return CImg<T>(*this,false).XYZtoxyY(); 
    }

    //! Convert a (x,y,Y) image to a (X,Y,Z) one.
    CImg get_xyYtoXYZ() const {
      return CImg<T>(*this,false).xyYtoXYZ(); 
    }

    //! Convert a (R,G,B) image to a (L,a,b) one.
    CImg get_RGBtoLab() const {
      return CImg<T>(*this,false).RGBtoLab(); 
    }

    //! Convert a (L,a,b) image to a (R,G,B) one.
    CImg get_LabtoRGB() const { 
      return CImg<T>(*this,false).LabtoRGB(); 
    }

    //! Convert a (R,G,B) image to a (x,y,Y) one.
    CImg get_RGBtoxyY() const {
      return CImg<T>(*this,false).RGBtoxyY(); 
    }

    //! Convert a (x,y,Y) image to a (R,G,B) one.
    CImg get_xyYtoRGB() const {
      return CImg<T>(*this,false).xyYtoRGB(); 
    }
    
    //@}
    //-------------------
    //
    //! \name Drawing
    //@{
    //-------------------

    // Should be used only by member functions. Not an user-friendly function.
    // Pre-requisites : x0<x1, y-coordinate is valid, col is valid.
    CImg& draw_scanline(const int x0,const int x1,const int y,const T *const color,
			const float opacity=1, const float brightness=1, const bool init=false) {
      static float nopacity=0, copacity=0;
      static unsigned int whz=0;
      static const T* col = 0;
      if (init) { 
	nopacity = cimg::abs(opacity);
	copacity = 1-cimg::max(opacity,0.0f); 
	whz = width*height*depth;
	col = color;
      } else {
	const int nx0 = cimg::max(x0,0), nx1 = cimg::min(x1,(int)width-1), dx = nx1-nx0;
	T *ptrd = ptr(0,y) + nx0;  // avoid problems when cimg_debug=2
	if (dx>=0) {
	  if (opacity>=1) {
	    int off = whz-dx-1;
	    if (sizeof(T)!=1) cimg_mapV(*this,k) {
	      const T val = (T)(*(col++)*brightness);
	      for (int x=dx; x>=0; x--) *(ptrd++)=val;
	      ptrd+=off;
	    } else cimg_mapV(*this,k) { std::memset(ptrd,(int)(*(col++)*brightness),dx+1); ptrd+=whz; }
	    col-=dim;
	  } else {
	    int off = whz-dx-1;
	    cimg_mapV(*this,k) {
	      const T val = (T)(*(col++)*brightness);
	      for (int x=dx; x>=0; x--) { *ptrd = (T)(val*nopacity + *ptrd*copacity); ptrd++; }
	      ptrd+=off;
	    }
	    col-=dim;
	  }
	}
      }
      return *this;
    }
    
    CImg& draw_scanline(const T *const color,const float opacity=1) { return draw_scanline(0,0,0,color,opacity,1.0f,true); }

    //! Draw a colored point in the instance image, at coordinates (\c x0,\c y0,\c z0).
    /**
       \param x0 = X-coordinate of the vector-valued pixel to plot.
       \param y0 = Y-coordinate of the vector-valued pixel to plot.
       \param z0 = Z-coordinate of the vector-valued pixel to plot.
       \param color = array of dimv() values of type \c T, defining the drawing color.
       \param opacity = opacity of the drawing.
       \note Clipping is supported.
    **/
    CImg& draw_point(const int x0,const int y0,const int z0,
                     const T *const color,const float opacity=1) {
      if (!is_empty()) {
	if (!color) throw CImgArgumentException("CImg<%s>::draw_point() : Specified color is (null)",pixel_type());
	if (x0>=0 && y0>=0 && z0>=0 && x0<dimx() && y0<dimy() && z0<dimz()) {
	  const T *col=color;
	  const unsigned int whz = width*height*depth;
	  const float nopacity = cimg::abs(opacity), copacity = 1-cimg::max(opacity,0.0f);
	  T *ptrd = ptr(x0,y0,z0,0);
	  if (opacity>=1) cimg_mapV(*this,k) { *ptrd = *(col++); ptrd+=whz; }
	  else cimg_mapV(*this,k) { *ptrd=(T)(*(col++)*nopacity + *ptrd*copacity); ptrd+=whz; }
	}
      }
      return *this;
    }

    //! Draw a colored point in the instance image, at coordinates (\c x0,\c y0).
    /**
       \param x0 = X-coordinate of the vector-valued pixel to plot.
       \param y0 = Y-coordinate of the vector-valued pixel to plot.
       \param color = array of dimv() values of type \c T, defining the drawing color.
       \param opacity = opacity of the drawing.
       \note Clipping is supported.
    **/
    CImg& draw_point(const int x0,const int y0,const T *const color,const float opacity=1) { 
      return draw_point(x0,y0,0,color,opacity); 
    }

    //! Draw a 2D colored line in the instance image, at coordinates (\c x0,\c y0)-(\c x1,\c y1).
    /**
       \param x0 = X-coordinate of the starting point of the line.
       \param y0 = Y-coordinate of the starting point of the line.
       \param x1 = X-coordinate of the ending point of the line.
       \param y1 = Y-coordinate of the ending point of the line.
       \param color = array of dimv() values of type \c T, defining the drawing color.
       \param pattern = An integer whose bits describes the line pattern.
       \param opacity = opacity of the drawing.
       \note Clipping is supported.
    **/
    CImg& draw_line(const int x0,const int y0,const int x1,const int y1,
                    const T *const color,const unsigned int pattern=~0L,const float opacity=1) {
      if (!is_empty()) {
	if (!color) throw CImgArgumentException("CImg<%s>::draw_line() : Specified color is (null)",pixel_type());
	const T* col=color;
	unsigned int hatch=1;     
	int nx0=x0, nx1=x1, ny0=y0, ny1=y1;
	if (nx0>nx1) cimg::swap(nx0,nx1,ny0,ny1);
	if (nx1<0 || nx0>=dimx()) return *this;
	if (nx0<0) { ny0-=nx0*(ny1-ny0)/(nx1-nx0); nx0=0; }
	if (nx1>=dimx()) { ny1+=(nx1-dimx())*(ny0-ny1)/(nx1-nx0); nx1=dimx()-1;}
	if (ny0>ny1) cimg::swap(nx0,nx1,ny0,ny1);
	if (ny1<0 || ny0>=dimy()) return *this;
	if (ny0<0) { nx0-=ny0*(nx1-nx0)/(ny1-ny0); ny0=0; }
	if (ny1>=dimy()) { nx1+=(ny1-dimy())*(nx0-nx1)/(ny1-ny0); ny1=dimy()-1;}
	const unsigned int dmax = (unsigned int)cimg::max(cimg::abs(nx1-nx0),ny1-ny0), whz = width*height*depth;
	const float px = dmax?(nx1-nx0)/(float)dmax:0, py = dmax?(ny1-ny0)/(float)dmax:0;
	float x = (float)nx0, y = (float)ny0;
	if (opacity>=1) for (unsigned int t=0; t<=dmax; t++) {
	  if (!(~pattern) || (~pattern && pattern&hatch)) {
	    T* ptrd = ptr((unsigned int)x,(unsigned int)y,0,0);      
	    cimg_mapV(*this,k) { *ptrd=*(col++); ptrd+=whz; }
	    col-=dim;
	  }
	  x+=px; y+=py; if (pattern) hatch=(hatch<<1)+(hatch>>(sizeof(unsigned int)*8-1));
	} else {
	  const float nopacity = cimg::abs(opacity), copacity=1-cimg::max(opacity,0.0f);
	  for (unsigned int t=0; t<=dmax; t++) {
	    if (!(~pattern) || (~pattern && pattern&hatch)) {
	      T* ptrd = ptr((unsigned int)x,(unsigned int)y,0,0);
	      cimg_mapV(*this,k) { *ptrd = (T)(*(col++)*nopacity + copacity*(*ptrd)); ptrd+=whz; }
	      col-=dim;
	    }
	    x+=px; y+=py; if (pattern) hatch=(hatch<<1)+(hatch>>(sizeof(unsigned int)*8-1));
	  }
	}
      }
      return *this;
    }
  
    //! Draw a 3D colored line in the instance image, at coordinates (\c x0,\c y0,\c z0)-(\c x1,\c y1,\c z1).
    /**
       \param x0 = X-coordinate of the starting point of the line.
       \param y0 = Y-coordinate of the starting point of the line.
       \param z0 = Z-coordinate of the starting point of the line.
       \param x1 = X-coordinate of the ending point of the line.
       \param y1 = Y-coordinate of the ending point of the line.
       \param z1 = Z-coordinate of the ending point of the line.
       \param color = array of dimv() values of type \c T, defining the drawing color.
       \param pattern = An integer whose bits describes the line pattern.
       \param opacity = opacity of the drawing.
       \note Clipping is supported.
    **/
    CImg& draw_line(const int x0,const int y0,const int z0,const int x1,const int y1,const int z1,
                    const T *const color,const unsigned int pattern=~0L,const float opacity=1) {
      if (!is_empty()) {
	if (!color) throw CImgArgumentException("CImg<%s>::draw_line() : Specified color is (null)",pixel_type());
	const T* col=color;
	unsigned int hatch=1;
	int nx0=x0, ny0=y0, nz0=z0, nx1=x1, ny1=y1, nz1=z1;
	if (nx0>nx1) cimg::swap(nx0,nx1,ny0,ny1,nz0,nz1);
	if (nx1<0 || nx0>=dimx()) return *this;
	if (nx0<0) { const int D=nx1-nx0; ny0-=nx0*(ny1-ny0)/D; nz0-=nx0*(nz1-nz0)/D; nx0=0; }
	if (nx1>=dimx()) { const int d=nx1-dimx(), D=nx1-nx0; ny1+=d*(ny0-ny1)/D; nz1+=d*(nz0-nz1)/D; nx1=dimx()-1;}
	if (ny0>ny1) cimg::swap(nx0,nx1,ny0,ny1,nz0,nz1);
	if (ny1<0 || ny0>=dimy()) return *this;
	if (ny0<0) { const int D=ny1-ny0; nx0-=ny0*(nx1-nx0)/D; nz0-=ny0*(nz1-nz0)/D; ny0=0; }
	if (ny1>=dimy()) { const int d=ny1-dimy(), D=ny1-ny0; nx1+=d*(nx0-nx1)/D; nz1+=d*(nz0-nz1)/D; ny1=dimy()-1;}
	if (nz0>nz1) cimg::swap(nx0,nx1,ny0,ny1,nz0,nz1);
	if (nz1<0 || nz0>=dimz()) return *this;
	if (nz0<0) { const int D=nz1-nz0; nx0-=nz0*(nx1-nx0)/D; ny0-=nz0*(ny1-ny0)/D; nz0=0; }
	if (nz1>=dimz()) { const int d=nz1-dimz(), D=nz1-nz0; nx1+=d*(nx0-nx1)/D; ny1+=d*(ny0-ny1)/D; nz1=dimz()-1;}
	const unsigned int dmax = (unsigned int)cimg::max(cimg::abs(nx1-nx0),cimg::abs(ny1-ny0),nz1-nz0), whz = width*height*depth;
	const float px = dmax?(nx1-nx0)/(float)dmax:0, py = dmax?(ny1-ny0)/(float)dmax:0, pz = dmax?(nz1-nz0)/(float)dmax:0;
	float x = (float)nx0, y = (float)ny0, z = (float)nz0;
	if (opacity>=1) for (unsigned int t=0; t<=dmax; t++) { 
	  if (!(~pattern) || (~pattern && pattern&hatch)) {
	    T* ptrd = ptr((unsigned int)x,(unsigned int)y,(unsigned int)z,0);
	    cimg_mapV(*this,k) { *ptrd=*(col++); ptrd+=whz; }        
	    col-=dim; 
	  }
	  x+=px; y+=py; z+=pz; if (pattern) hatch=(hatch<<1)+(hatch>>(sizeof(unsigned int)*8-1));
	} else {
	  const float nopacity = cimg::abs(opacity), copacity = 1-cimg::max(opacity,0.0f);
	  for (unsigned int t=0; t<=dmax; t++) { 
	    if (!(~pattern) || (~pattern && pattern&hatch)) {
	      T* ptrd = ptr((unsigned int)x,(unsigned int)y,(unsigned int)z,0);
	      cimg_mapV(*this,k) { *ptrd = (T)(*(col++)*nopacity + copacity*(*ptrd)); ptrd+=whz; }
	      col-=dim; 
	    }
	    x+=px; y+=py; z+=pz; if (pattern) hatch=(hatch<<1)+(hatch>>(sizeof(unsigned int)*8-1));        
	  }
	}
      }
      return *this;
    }

    //! Draw a 2D textured line in the instance image, at coordinates (\c x0,\c y0)-(\c x1,\c y1).
    /**
       \param x0 = X-coordinate of the starting point of the line.
       \param y0 = Y-coordinate of the starting point of the line.
       \param x1 = X-coordinate of the ending point of the line.
       \param y1 = Y-coordinate of the ending point of the line.
       \param texture = a colored texture image used to draw the line color.
       \param tx0 = X-coordinate of the starting point of the texture.
       \param ty0 = Y-coordinate of the starting point of the texture.
       \param tx1 = X-coordinate of the ending point of the texture.
       \param ty1 = Y-coordinate of the ending point of the texture.
       \param opacity = opacity of the drawing.
       \note Clipping is supported, but texture coordinates do not support clipping.
    **/
    template<typename t> CImg& draw_line(const int x0,const int y0,const int x1,const int y1,
                                         const CImg<t>& texture,
                                         const int tx0,const int ty0,const int tx1,const int ty1,
                                         const float opacity=1) {
      if (!is_empty()) {
	if (texture.is_empty() || texture.dim<dim)
	  throw CImgArgumentException("CImg<%s>::draw_line() : specified texture (%u,%u,%u,%u,%p) has wrong dimensions.",
				      pixel_type(),texture.width,texture.height,texture.depth,texture.dim,texture.data);
	int nx0=x0, ny0=y0, nx1=x1, ny1=y1, ntx0=tx0, nty0=ty0, ntx1=tx1, nty1=ty1;
	if (nx0>nx1) cimg::swap(nx0,nx1,ny0,ny1,ntx0,ntx1,nty0,nty1);
	if (nx1<0 || nx0>=dimx()) return *this;
	if (nx0<0) { const int D=nx1-nx0; ny0-=nx0*(ny1-ny0)/D; ntx0-=nx0*(ntx1-ntx0)/D; nty0-=nx0*(nty1-nty0)/D; nx0=0; }
	if (nx1>=dimx()) { const int d=nx1-dimx(),D=nx1-nx0; ny1+=d*(ny0-ny1)/D; ntx1+=d*(ntx0-ntx1)/D; nty1+=d*(nty0-nty1)/D; nx1=dimx()-1; }
	if (ny0>ny1) cimg::swap(nx0,nx1,ny0,ny1,ntx0,ntx1,nty0,nty1);
	if (ny1<0 || ny0>=dimy()) return *this;
	if (ny0<0) { const int D=ny1-ny0; nx0-=ny0*(nx1-nx0)/D; ntx0-=ny0*(ntx1-ntx0)/D; nty0-=ny0*(nty1-nty0)/D; ny0=0; }
	if (ny1>=dimy()) { const int d=ny1-dimy(),D=ny1-ny0; nx1+=d*(nx0-nx1)/D; ntx1+=d*(ntx0-ntx1)/D; nty1+=d*(nty0-nty1)/D; ny1=dimy()-1; }
	const unsigned int dmax = (unsigned int)cimg::max(cimg::abs(nx1-nx0),ny1-ny0), 
	  whz = width*height*depth, twhz = texture.width*texture.height*texture.depth;
	const float px = dmax?(nx1-nx0)/(float)dmax:0, py = dmax?(ny1-ny0)/(float)dmax:0,
	  tpx = dmax?(ntx1-ntx0)/(float)dmax:0, tpy = dmax?(nty1-nty0)/(float)dmax:0;
	float x = (float)nx0, y = (float)ny0, tx = (float)ntx0, ty = (float)nty0;
	if (opacity>=1) for (unsigned int tt=0; tt<=dmax; tt++) { 
	  T *ptrd = ptr((unsigned int)x,(unsigned int)y,0,0);
	  const t *ptrs = texture.ptr((unsigned int)tx,(unsigned int)ty,0,0);
	  cimg_mapV(*this,k) { *ptrd = (T)(*ptrs); ptrd+=whz; ptrs+=twhz; }
	  x+=px; y+=py; tx+=tpx; ty+=tpy;
	} else {
	  const float nopacity = cimg::abs(opacity), copacity = 1-cimg::max(opacity,0.0f);
	  for (unsigned int tt=0; tt<=dmax; tt++) { 
	    T *ptrd = ptr((unsigned int)x,(unsigned int)y,0,0);
	    const t *ptrs = texture.ptr((unsigned int)tx,(unsigned int)ty,0,0);
	    cimg_mapV(*this,k) { *ptrd = (T)(nopacity*(*ptrs) + copacity*(*ptrd)); ptrd+=whz; ptrs+=twhz; }
	    x+=px; y+=py; tx+=tpx; ty+=tpy;
	  }
	}
      }
      return *this;
    }

    //! Draw a 2D colored arrow in the instance image, at coordinates (\c x0,\c y0)->(\c x1,\c y1).
    /**
       \param x0 = X-coordinate of the starting point of the arrow (tail).
       \param y0 = Y-coordinate of the starting point of the arrow (tail).
       \param x1 = X-coordinate of the ending point of the arrow (head).
       \param y1 = Y-coordinate of the ending point of the arrow (head).
       \param color = array of dimv() values of type \c T, defining the drawing color.
       \param angle = aperture angle of the arrow head
       \param length = length of the arrow head. If <0, described as a percentage of the arrow length.
       \param pattern = An integer whose bits describes the line pattern.
       \param opacity = opacity of the drawing.
       \note Clipping is supported.
    **/
    CImg& draw_arrow(const int x0,const int y0,const int x1,const int y1,
                     const T *const color,
                     const float angle=30,const float length=-10,const unsigned int pattern=~0L,const float opacity=1) {
      if (!is_empty()) {
	const float u = (float)(x0-x1), v = (float)(y0-y1), sq = u*u+v*v,
	  deg = (float)(angle*cimg::PI/180), ang = (sq>0)?(float)std::atan2(v,u):0.0f,
	  l = (length>=0)?length:-length*(float)std::sqrt(sq)/100;
	if (sq>0) {
	  const double cl = std::cos(ang-deg), sl = std::sin(ang-deg), cr = std::cos(ang+deg), sr = std::sin(ang+deg);        
	  const int 
	    xl = x1+(int)(l*cl), yl = y1+(int)(l*sl),
	    xr = x1+(int)(l*cr), yr = y1+(int)(l*sr),
	    xc = x1+(int)((l+1)*(cl+cr))/2, yc = y1+(int)((l+1)*(sl+sr))/2;
	  draw_line(x0,y0,xc,yc,color,pattern,opacity).draw_triangle(x1,y1,xl,yl,xr,yr,color,opacity);
	} else draw_point(x0,y0,color,opacity);
      }
      return *this;
    }

    //! Draw a sprite image in the instance image, at coordinates (\c x0,\c y0,\c z0,\c v0).
    /**
       \param sprite = sprite image.
       \param x0 = X-coordinate of the sprite position in the instance image.
       \param y0 = Y-coordinate of the sprite position in the instance image.
       \param z0 = Z-coordinate of the sprite position in the instance image.
       \param v0 = V-coordinate of the sprite position in the instance image.
       \param opacity = opacity of the drawing.
       \note Clipping is supported.
    **/
    template<typename t> CImg& draw_image(const CImg<t>& sprite,
                                          const int x0=0,const int y0=0,const int z0=0,const int v0=0,const float opacity=1) {
      if (!is_empty()) {
	if (sprite.is_empty())
	  throw CImgArgumentException("CImg<%s>::draw_image() : Specified sprite image (%u,%u,%u,%u,%p) is empty.",
				      pixel_type(),sprite.width,sprite.height,sprite.depth,sprite.dim,sprite.data);
	const bool bx=(x0<0), by=(y0<0), bz=(z0<0), bv=(v0<0);
	const int 
	  lX = sprite.dimx() - (x0+sprite.dimx()>dimx()?x0+sprite.dimx()-dimx():0) + (bx?x0:0),
	  lY = sprite.dimy() - (y0+sprite.dimy()>dimy()?y0+sprite.dimy()-dimy():0) + (by?y0:0),
	  lZ = sprite.dimz() - (z0+sprite.dimz()>dimz()?z0+sprite.dimz()-dimz():0) + (bz?z0:0),
	  lV = sprite.dimv() - (v0+sprite.dimv()>dimv()?v0+sprite.dimv()-dimv():0) + (bv?v0:0);
	const t *ptrs = sprite.ptr()-(bx?x0:0)-(by?y0*sprite.dimx():0)+(bz?z0*sprite.dimx()*sprite.dimy():0)+
	  (bv?v0*sprite.dimx()*sprite.dimy()*sprite.dimz():0);
	const unsigned int
	  offX = width-lX, soffX = sprite.width-lX,
	  offY = width*(height-lY), soffY = sprite.width*(sprite.height-lY),
	  offZ = width*height*(depth-lZ), soffZ = sprite.width*sprite.height*(sprite.depth-lZ);
	const float nopacity = cimg::abs(opacity), copacity = 1-cimg::max(opacity,0.0f);
	T *ptrd = ptr(x0<0?0:x0,y0<0?0:y0,z0<0?0:z0,v0<0?0:v0);
	if (lX>0 && lY>0 && lZ>0 && lV>0)
	  for (int v=0; v<lV; v++) {
	    for (int z=0; z<lZ; z++) {
	      for (int y=0; y<lY; y++) {
		if (opacity>=1) for (int x=0; x<lX; x++) *(ptrd++) = (T)(*(ptrs++));
		else for (int x=0; x<lX; x++) { *ptrd = (T)(nopacity*(*(ptrs++)) + copacity*(*ptrd)); ptrd++; }
		ptrd+=offX; ptrs+=soffX;
	      }
	      ptrd+=offY; ptrs+=soffY;
	    }
	    ptrd+=offZ; ptrs+=soffZ;
	  }
      }
      return *this;
    }

#if ( !defined(_MSC_VER) || _MSC_VER>1300 )
    CImg& draw_image(const CImg& sprite,const int x0=0,const int y0=0,const int z0=0,const int v0=0,const float opacity=1) {
      if (!is_empty()) {
	if (sprite.is_empty())
	  throw CImgArgumentException("CImg<%s>::draw_image() : Specified sprite image (%u,%u,%u,%u,%p) is empty.",
				      pixel_type(),sprite.width,sprite.height,sprite.depth,sprite.dim,sprite.data);
	if (this==&sprite) return draw_image(CImg<T>(sprite),x0,y0,z0,v0,opacity);
	const bool bx=(x0<0), by=(y0<0), bz=(z0<0), bv=(v0<0);
	const int 
	  lX = sprite.dimx() - (x0+sprite.dimx()>dimx()?x0+sprite.dimx()-dimx():0) + (bx?x0:0),
	  lY = sprite.dimy() - (y0+sprite.dimy()>dimy()?y0+sprite.dimy()-dimy():0) + (by?y0:0),
	  lZ = sprite.dimz() - (z0+sprite.dimz()>dimz()?z0+sprite.dimz()-dimz():0) + (bz?z0:0),
	  lV = sprite.dimv() - (v0+sprite.dimv()>dimv()?v0+sprite.dimv()-dimv():0) + (bv?v0:0);
	const T *ptrs = sprite.ptr()-(bx?x0:0)-(by?y0*sprite.dimx():0)+(bz?z0*sprite.dimx()*sprite.dimy():0)+
	  (bv?v0*sprite.dimx()*sprite.dimy()*sprite.dimz():0);
	const unsigned int
	  offX = width-lX, soffX = sprite.width-lX,
	  offY = width*(height-lY), soffY = sprite.width*(sprite.height-lY),
	  offZ = width*height*(depth-lZ), soffZ = sprite.width*sprite.height*(sprite.depth-lZ),
	  slX = lX*sizeof(T);    
	const float nopacity = cimg::abs(opacity), copacity = 1-cimg::max(opacity,0.0f);
	T *ptrd = ptr(x0<0?0:x0,y0<0?0:y0,z0<0?0:z0,v0<0?0:v0);
	if (lX>0 && lY>0 && lZ>0 && lV>0)
	  for (int v=0; v<lV; v++) {
	    for (int z=0; z<lZ; z++) {
	      if (opacity>=1) for (int y=0; y<lY; y++) { std::memcpy(ptrd,ptrs,slX); ptrd+=width; ptrs+=sprite.width; }
	      else for (int y=0; y<lY; y++) {
		      for (int x=0; x<lX; x++) { *ptrd = (T)(nopacity*(*(ptrs++)) + copacity*(*ptrd)); ptrd++; }
		ptrd+=offX; ptrs+=soffX;
	      }
	      ptrd+=offY; ptrs+=soffY;
	    }
	    ptrd+=offZ; ptrs+=soffZ;
	  }
      }
      return *this;
    }
#endif

    //! Draw a masked sprite image in the instance image, at coordinates (\c x0,\c y0,\c z0,\c v0).
    /**
       \param sprite = sprite image.
       \param mask = mask image.
       \param x0 = X-coordinate of the sprite position in the instance image.
       \param y0 = Y-coordinate of the sprite position in the instance image.
       \param z0 = Z-coordinate of the sprite position in the instance image.
       \param v0 = V-coordinate of the sprite position in the instance image.
       \param mask_valmax = Maximum pixel value of the mask image \c mask.
       \param opacity = opacity of the drawing.
       \note Pixel values of \c mask set the opacity of the corresponding pixels in \c sprite.
       \note Clipping is supported.
       \note Dimensions along x,y and z of \c sprite and \c mask must be the same.
    **/
    template<typename ti,typename tm> CImg& draw_image(const CImg<ti>& sprite,const CImg<tm>& mask,
                                                       const int x0=0,const int y0=0,const int z0=0,const int v0=0,
                                                       const tm mask_valmax=1,const float opacity=1) {
      if (!is_empty()) {
	if (sprite.is_empty())
	  throw CImgArgumentException("CImg<%s>::draw_image() : Specified sprite image (%u,%u,%u,%u,%p) is empty.",
				      pixel_type(),sprite.width,sprite.height,sprite.depth,sprite.dim,sprite.data);
	if (mask.is_empty())
	  throw CImgArgumentException("CImg<%s>::draw_image() : Specified mask image (%u,%u,%u,%u,%p) is empty.",
				      pixel_type(),mask.width,mask.height,mask.depth,mask.dim,mask.data);
	if ((void*)this==(void*)&sprite) return draw_image(CImg<T>(sprite),mask,x0,y0,z0,v0);
	if(mask.width!=sprite.width || mask.height!=sprite.height || mask.depth!=sprite.depth)
	  throw CImgArgumentException("CImg<%s>::draw_image() : Mask dimension is (%u,%u,%u,%u), while sprite is (%u,%u,%u,%u)",
				      pixel_type(),mask.width,mask.height,mask.depth,mask.dim,sprite.width,sprite.height,sprite.depth,sprite.dim);
	const bool bx=(x0<0), by=(y0<0), bz=(z0<0), bv=(v0<0);
	const int
	  lX = sprite.dimx() - (x0+sprite.dimx()>dimx()?x0+sprite.dimx()-dimx():0) + (bx?x0:0),
	  lY = sprite.dimy() - (y0+sprite.dimy()>dimy()?y0+sprite.dimy()-dimy():0) + (by?y0:0),
	  lZ = sprite.dimz() - (z0+sprite.dimz()>dimz()?z0+sprite.dimz()-dimz():0) + (bz?z0:0),      
	  lV = sprite.dimv() - (v0+sprite.dimv()>dimv()?v0+sprite.dimv()-dimv():0) + (bv?v0:0);    
	const int coff = -(bx?x0:0)-(by?y0*mask.dimx():0)-(bz?z0*mask.dimx()*mask.dimy():0)-
	  (bv?v0*mask.dimx()*mask.dimy()*mask.dimz():0),
	  ssize = mask.dimx()*mask.dimy()*mask.dimz();
	const ti *ptrs = sprite.ptr() + coff;
	const tm *ptrm = mask.ptr() + coff;
	const unsigned int
	  offX = width-lX, soffX = sprite.width-lX,
	  offY = width*(height-lY), soffY = sprite.width*(sprite.height-lY),
	  offZ = width*height*(depth-lZ), soffZ = sprite.width*sprite.height*(sprite.depth-lZ);
	T *ptrd = ptr(x0<0?0:x0,y0<0?0:y0,z0<0?0:z0,v0<0?0:v0);
	if (lX>0 && lY>0 && lZ>0 && lV>0)
	  for (int v=0; v<lV; v++) {
	    ptrm = mask.data + (ptrm - mask.data)%ssize;
	    for (int z=0; z<lZ; z++) {
	      for (int y=0; y<lY; y++) {
		for (int x=0; x<lX; x++) {
		  const float mopacity = *(ptrm++)*opacity,
		    nopacity = cimg::abs(mopacity), copacity = mask_valmax-cimg::max(mopacity,0.0f);
		  *ptrd = (T)((nopacity*(*(ptrs++))+copacity*(*ptrd))/mask_valmax);
		  ptrd++;
		}
		ptrd+=offX; ptrs+=soffX; ptrm+=soffX;
	      }
	      ptrd+=offY; ptrs+=soffY; ptrm+=soffY;
	    }
	    ptrd+=offZ; ptrs+=soffZ; ptrm+=soffZ;
	  }
      }
      return *this;
    }

    //! Draw a 4D filled rectangle in the instance image, at coordinates (\c x0,\c y0,\c z0,\c v0)-(\c x1,\c y1,\c z1,\c v1).
    /**
       \param x0 = X-coordinate of the upper-left rectangle corner in the instance image.
       \param y0 = Y-coordinate of the upper-left rectangle corner in the instance image.
       \param z0 = Z-coordinate of the upper-left rectangle corner in the instance image.
       \param v0 = V-coordinate of the upper-left rectangle corner in the instance image.
       \param x1 = X-coordinate of the lower-right rectangle corner in the instance image.
       \param y1 = Y-coordinate of the lower-right rectangle corner in the instance image.
       \param z1 = Z-coordinate of the lower-right rectangle corner in the instance image.
       \param v1 = V-coordinate of the lower-right rectangle corner in the instance image.
       \param val = scalar value used to fill the rectangle area.
       \param opacity = opacity of the drawing.
       \note Clipping is supported.
    **/
    CImg& draw_rectangle(const int x0,const int y0,const int z0,const int v0,
                         const int x1,const int y1,const int z1,const int v1,
                         const T& val,const float opacity=1.0f) {
      if (!is_empty()) {	
	const bool bx=(x0<x1), by=(y0<y1), bz=(z0<z1), bv=(v0<v1);
	const int nx0=bx?x0:x1, nx1=bx?x1:x0, ny0=by?y0:y1, ny1=by?y1:y0, nz0=bz?z0:z1, nz1=bz?z1:z0, nv0=bv?v0:v1, nv1=bv?v1:v0;
	const int 
	  lX = (1+nx1-nx0) + (nx1>=dimx()?dimx()-1-nx1:0) + (nx0<0?nx0:0),
	  lY = (1+ny1-ny0) + (ny1>=dimy()?dimy()-1-ny1:0) + (ny0<0?ny0:0),
	  lZ = (1+nz1-nz0) + (nz1>=dimz()?dimz()-1-nz1:0) + (nz0<0?nz0:0),
	  lV = (1+nv1-nv0) + (nv1>=dimv()?dimv()-1-nv1:0) + (nv0<0?nv0:0);
	const unsigned int offX = width-lX, offY = width*(height-lY), offZ = width*height*(depth-lZ);
	const float nopacity = cimg::abs(opacity), copacity = 1-cimg::max(opacity,0.0f);
	T *ptrd = ptr(nx0<0?0:nx0,ny0<0?0:ny0,nz0<0?0:nz0,nv0<0?0:nv0);
	if (lX>0 && lY>0 && lZ>0 && lV>0)
	  for (int v=0; v<lV; v++) {
	    for (int z=0; z<lZ; z++) {
	      for (int y=0; y<lY; y++) {
		if (opacity>=1) {
		  if (sizeof(T)!=1) { for (int x=0; x<lX; x++) *(ptrd++) = val; ptrd+=offX; }
		  else { std::memset(ptrd,(int)val,lX); ptrd+=width; }
		} else { for (int x=0; x<lX; x++) { *ptrd = (T)(nopacity*val+copacity*(*ptrd)); ptrd++; } ptrd+=offX; }
	      }
	      ptrd+=offY;
	    }
	    ptrd+=offZ;
	  }  
      }
      return *this;
    }

    //! Draw a 3D filled colored rectangle in the instance image, at coordinates (\c x0,\c y0,\c z0)-(\c x1,\c y1,\c z1).
    /**
       \param x0 = X-coordinate of the upper-left rectangle corner in the instance image.
       \param y0 = Y-coordinate of the upper-left rectangle corner in the instance image.
       \param z0 = Z-coordinate of the upper-left rectangle corner in the instance image.
       \param x1 = X-coordinate of the lower-right rectangle corner in the instance image.
       \param y1 = Y-coordinate of the lower-right rectangle corner in the instance image.
       \param z1 = Z-coordinate of the lower-right rectangle corner in the instance image.
       \param color = array of dimv() values of type \c T, defining the drawing color.
       \param opacity = opacity of the drawing.
       \note Clipping is supported.
    **/
    CImg& draw_rectangle(const int x0,const int y0,const int z0,
                         const int x1,const int y1,const int z1,
                         const T *const color,const float opacity=1) {
      if (!color) throw CImgArgumentException("CImg<%s>::draw_rectangle : specified color is (null)",pixel_type());
      cimg_mapV(*this,k) draw_rectangle(x0,y0,z0,k,x1,y1,z1,k,color[k],opacity);
      return *this;
    }

    //! Draw a 2D filled colored rectangle in the instance image, at coordinates (\c x0,\c y0)-(\c x1,\c y1).
    /**
       \param x0 = X-coordinate of the upper-left rectangle corner in the instance image.
       \param y0 = Y-coordinate of the upper-left rectangle corner in the instance image.
       \param x1 = X-coordinate of the lower-right rectangle corner in the instance image.
       \param y1 = Y-coordinate of the lower-right rectangle corner in the instance image.
       \param color = array of dimv() values of type \c T, defining the drawing color.
       \param opacity = opacity of the drawing.
       \note Clipping is supported.
    **/
    CImg& draw_rectangle(const int x0,const int y0,const int x1,const int y1,
                         const T *const color,const float opacity=1) {
      draw_rectangle(x0,y0,0,x1,y1,depth-1,color,opacity);
      return *this;
    }
  
    //! Draw a 2D filled colored triangle in the instance image, at coordinates (\c x0,\c y0)-(\c x1,\c y1)-(\c x2,\c y2).
    /**
       \param x0 = X-coordinate of the first corner in the instance image.
       \param y0 = Y-coordinate of the first corner in the instance image.
       \param x1 = X-coordinate of the second corner in the instance image.
       \param y1 = Y-coordinate of the second corner in the instance image.
       \param x2 = X-coordinate of the third corner in the instance image.
       \param y2 = Y-coordinate of the third corner in the instance image.
       \param color = array of dimv() values of type \c T, defining the drawing color.
       \param opacity = opacity of the drawing (<1)
       \param brightness = brightness of the drawing (in [0,1])
       \note Clipping is supported.
    **/
    CImg& draw_triangle(const int x0,const int y0,
                        const int x1,const int y1,
                        const int x2,const int y2,
                        const T *const color,
			const float opacity=1,
			const float brightness=1) {
      draw_scanline(color,opacity);
      int nx0=x0,ny0=y0,nx1=x1,ny1=y1,nx2=x2,ny2=y2;
      if (ny0>ny1) cimg::swap(nx0,nx1,ny0,ny1);
      if (ny0>ny2) cimg::swap(nx0,nx2,ny0,ny2);
      if (ny1>ny2) cimg::swap(nx1,nx2,ny1,ny2);
      if (ny0>=dimy() || ny2<0) return *this;
      const float 
        p1 = (ny1-ny0)?(nx1-nx0)/(float)(ny1-ny0):(nx1-nx0),
        p2 = (ny2-ny0)?(nx2-nx0)/(float)(ny2-ny0):(nx2-nx0),
        p3 = (ny2-ny1)?(nx2-nx1)/(float)(ny2-ny1):(nx2-nx1);
      float xleft = (float)nx0, xright = xleft, pleft = (p1<p2)?p1:p2, pright = (p1<p2)?p2:p1;
      if (ny0<0) { xleft-=ny0*pleft; xright-=ny0*pright; }
      const int ya = ny1>dimy()?height:ny1;
      for (int y=ny0<0?0:ny0; y<ya; y++) {
	draw_scanline((int)xleft,(int)xright,y,color,opacity,brightness); 
	xleft+=pleft; xright+=pright; 
      }
      if (p1<p2) { xleft=(float)nx1;  pleft=p3;  if (ny1<0) xleft-=ny1*pleft; } 
      else       { xright=(float)nx1; pright=p3; if (ny1<0) xright-=ny1*pright; }
      const int yb = ny2>=dimy()?height-1:ny2;
      for (int yy=ny1<0?0:ny1; yy<=yb; yy++) { 
	draw_scanline((int)xleft,(int)xright,yy,color,opacity,brightness);
	xleft+=pleft; xright+=pright; 
      }
      return *this;
    }

    template<typename t> CImg& draw_text(const char *const text,
                                         const int x0,const int y0,
                                         const T *const fgcolor,const T *const bgcolor,
                                         const CImgl<t>& font,const float opacity=1) {
      if (!text)
	throw CImgArgumentException("CImg<%s>::draw_text() : Specified input string is (null).",pixel_type());
      if (font.is_empty())
	throw CImgArgumentException("CImg<%s>::draw_text() : Specified font (%u,%p) is empty.",
				    pixel_type(),font.size,font.data);

      if (is_empty()) {
	// If needed, pre-compute needed size of the image
	int x=0, y=0, w=0;
	for (int i=0; i<cimg::strlen(text); i++) {
	  const unsigned char c = text[i];
	  switch (c) {
	  case '\n': y+=font[' '].height; if (x>w) w=x; x=0; break;
	  case '\t': x+=4*font[' '].width; break;
	  default: if (c<font.size) x+=font[c].width;
	  }
	}	
	if (x!=0) {
	  if (x>w) w=x;
	  y+=font[' '].height;
	}
	assign(x0+w,y0+y,1,font[' '].dim,0);
	if (bgcolor) cimg_mapV(*this,k) get_shared_channel(k).fill(bgcolor[k]);
      }
      
      int x = x0, y = y0;
      CImg<T> letter;
      for (int i=0; i<cimg::strlen(text); i++) {
        const unsigned char c = text[i];
        switch (c) {
        case '\n': y+=font[' '].height; x=x0; break;
        case '\t': x+=4*font[' '].width; break;
        default: if (c<font.size) {
	  letter = font[c];
	  const CImg& mask = (c+256)<(int)font.size?font[c+256]:font[c];
	  if (fgcolor) for (unsigned int p=0; p<letter.width*letter.height; p++) 
	    if (mask(p)) cimg_mapV(*this,k) letter(p,0,0,k) = (T)(letter(p,0,0,k)*fgcolor[k]);
	  if (bgcolor) for (unsigned int p=0; p<letter.width*letter.height; p++)
	    if (!mask(p)) cimg_mapV(*this,k) letter(p,0,0,k) = bgcolor[k];
	  if (!bgcolor && font.size>=512) draw_image(letter,mask,x,y,0,0,(T)1,opacity); 
	  else draw_image(letter,x,y,0,0,opacity);
	  x+=letter.width;
	}
          break;
        }
      }
      return *this;
    }

    //! Draw a text into the instance image.
    /**
       \param text = a C-string containing the text to display.
       \param x0 = X-coordinate of the text in the instance image.
       \param y0 = Y-coordinate of the text in the instance image.
       \param fgcolor = an array of dimv() values of type \c T, defining the foreground color (0 means 'transparent').
       \param bgcolor = an array of dimv() values of type \c T, defining the background color (0 means 'transparent').
       \param font_size = Height of the desired font (11,13,24,38 or 57)
       \param opacity = opacity of the drawing.
       \note Clipping is supported.
       \see get_font().
    **/
    CImg& draw_text(const char *const text,
		    const int x0,const int y0,
		    const T *const fgcolor,const T *const bgcolor=0,
		    const unsigned int font_size=11,const float opacity=1.0f) {
      return draw_text(text,x0,y0,fgcolor,bgcolor,CImgl<T>::get_font(font_size),opacity);
    }
    
  
    //! Draw a text into the instance image.
    /**
       \param x0 = X-coordinate of the text in the instance image.
       \param y0 = Y-coordinate of the text in the instance image.
       \param fgcolor = an array of dimv() values of type \c T, defining the foreground color (0 means 'transparent').
       \param bgcolor = an array of dimv() values of type \c T, defining the background color (0 means 'transparent').
       \param opacity = opacity of the drawing.
       \param format = a 'printf'-style format, followed by arguments.
       \note Clipping is supported.
    **/
    CImg& draw_text(const int x0,const int y0,
                    const T *const fgcolor,const T *const bgcolor, const unsigned int font_size,
                    const float opacity,const char *format,...) {
      char tmp[2048]; 
      std::va_list ap;
      va_start(ap,format);
      std::vsprintf(tmp,format,ap);
      va_end(ap);
      return draw_text(tmp,x0,y0,fgcolor,bgcolor,font_size,opacity);
    }

    template<typename t> CImg& draw_text(const int x0,const int y0,
                                         const T *const fgcolor,const T *const bgcolor,
                                         const CImgl<t>& font, const float opacity, const char *format,...) {
      char tmp[2048]; 
      std::va_list ap;
      va_start(ap,format); 
      std::vsprintf(tmp,format,ap);
      va_end(ap);
      return draw_text(tmp,x0,y0,fgcolor,bgcolor,font,opacity);
    }


    //! Draw a vector field in the instance image.
    /**
       \param flow = a 2d image of 2d vectors used as input data.
       \param color = an array of dimv() values of type \c T, defining the drawing color.
       \param sampling = length (in pixels) between each arrow.
       \param factor = length factor of each arrow (if <0, computed as a percentage of the maximum length).
       \param quiver_type = type of plot. Can be 0 (arrows) or 1 (segments).
       \param opacity = opacity of the drawing.
       \note Clipping is supported.
    **/
    template<typename t> 
    CImg& draw_quiver(const CImg<t>& flow, const T *const color,
		      const unsigned int sampling=25, const float factor=-20,
                      const int quiver_type=0, const float opacity=1) {
      if (!is_empty()) {
	if (flow.is_empty() || flow.dim!=2)
	  throw CImgArgumentException("CImg<%s>::draw_quiver() : Specified flow (%u,%u,%u,%u,%p) has wrong dimensions.",
				      pixel_type(),flow.width,flow.height,flow.depth,flow.dim,flow.data);
	if (!color) 
	  throw CImgArgumentException("CImg<%s>::draw_quiver() : Specified color is (null)",
				      pixel_type());
	if (sampling<=0)
	  throw CImgArgumentException("CImg<%s>::draw_quiver() : Incorrect sampling value = %g",
				      pixel_type(),sampling);

	float vmax,fact;
	if (factor<=0) {
	  const CImgStats st(flow.get_norm_pointwise(2),false);
	  vmax = (float)cimg::max(cimg::abs(st.min),cimg::abs(st.max));
	  fact = -factor;
	} else { fact = factor; vmax = 1; }
	
	for (unsigned int y=sampling/2; y<height; y+=sampling)
	  for (unsigned int x=sampling/2; x<width; x+=sampling) {
	    const unsigned int X = x*flow.width/width, Y = y*flow.height/height;
	    float u = (float)flow(X,Y,0,0)*fact/vmax, v = (float)flow(X,Y,0,1)*fact/vmax;
	    if (!quiver_type) {
	      const int xx = x+(int)u, yy = y+(int)v;
	      draw_arrow(x,y,xx,yy,color,45.0f,sampling/5.0f,~0L,opacity);
	    } else draw_line((int)(x-0.5*u),(int)(y-0.5*v),(int)(x+0.5*u),(int)(y+0.5*v),color,~0L,opacity);
	  }
      }
      return *this; 
    }

        //! Draw a 1D graph on the instance image.
    /**
       \param data = an image containing the graph values I = f(x).
       \param color = an array of dimv() values of type \c T, defining the drawing color.
       \param gtype = define the type of the plot :
                      - 0 = Plot using linear interpolation (segments).
		      - 1 = Plot with bars.
		      - 2 = Plot using cubic interpolation (3-polynomials).
       \param ymin = lower bound of the y-range.
       \param ymax = upper bound of the y-range.
       \param opacity = opacity of the drawing.
       \note
         - if \c ymin==ymax==0, the y-range is computed automatically from the input sample.
       \see draw_axe().
    **/
    template<typename t>
    CImg& draw_graph(const CImg<t>& data,const T *const color,const unsigned int gtype=0,
                     const double ymin=0,const double ymax=0,const float opacity=1) {
      if (!is_empty()) {
	if (!color) throw CImgArgumentException("CImg<%s>::draw_graph() : Specified color is (null)",pixel_type());
	T *color1 = new T[dim], *color2 = new T[dim];
	cimg_mapV(*this,k) { color1[k]=(T)(color[k]*0.6f); color2[k]=(T)(color[k]*0.3f); }
	CImgStats st;
	if (ymin==ymax) { st = CImgStats(data,false); cimg::swap(st.min,st.max); } else { st.min = ymin; st.max = ymax; }
	if (st.min==st.max) { st.min--; st.max++; }
	const float ca = height>1?(float)(st.max-st.min)/(height-1):0, cb = (float)st.min;
	const int Y0 = (int)(-cb/ca);
	int pY=0;
	cimg_mapoff(data,off) {     
	  const int Y = (int)((data[off]-cb)/ca);
	  switch (gtype) {
	  case 0: // plot with segments
	    if (off>0) draw_line((int)((off-1)*width/data.size()),pY,(int)(off*width/data.size()),Y,color,~0L,opacity);
	    break;
	  case 1: { // plot with bars
	    const unsigned int X = off*width/data.size(), nX = (off+1)*width/data.size()-1;
	    draw_rectangle(X,(int)Y0,nX,Y,color1,opacity);
	    draw_line(X,Y,X,(int)Y0,color2,~0L,opacity);
	    draw_line(X,(int)Y0,nX,(int)Y0,Y<=Y0?color2:color,~0L,opacity);
	    draw_line(nX,Y,nX,(int)Y0,color,~0L,opacity);
	    draw_line(X,Y,nX,Y,Y<=Y0?color:color2,~0L,opacity);
	  } break;
	  }        
	  pY=Y;
	}
	if (gtype==2) { // plot with cubic interpolation
	  const CImg<t> ndata = data.get_shared_points(0,data.size()-1);
	  cimg_mapX(*this,x) {
	    const int Y = (int)((ndata.cubic_pix1d((float)x*ndata.width/width)-cb)/ca);
	    if (x>0) draw_line(x,pY,x+1,Y,color,~0L,opacity);
	    pY=Y;
	  }
	}
	delete[] color1; delete[] color2;
      }
      return *this;     
    }

        //! Draw a labelled horizontal axis on the instance image.
    /** 
       \param x0 = lower bound of the x-range.
       \param x1 = upper bound of the x-range.
       \param y = Y-coordinate of the horizontal axis in the instance image.
       \param color = an array of dimv() values of type \c T, defining the drawing color.
       \param precision = precision of the labels.
       \param opacity = opacity of the drawing.
       \note if \c precision==0, precision of the labels is automatically computed.
       \see draw_graph().
    **/
    template<typename t> CImg& draw_axe(const CImg<t>& xvalues, const int y, const T *const color,
					const int precision=-1, const float opacity=1.0f) {
      if (!is_empty()) {	
	int siz = (int)xvalues.size()-1;
	if (siz<=0) draw_line(0,y,width-1,y,color,~0L,opacity);
	else {
	  if (xvalues[0]<xvalues[siz]) draw_arrow(0,y,width-1,y,color,30,5,~0L,opacity);
	  else draw_arrow(width-1,y,0,y,color,30,5,~0L,opacity);
	  const int yt = (y+14)<dimy()?(y+3):(y-14);
	  char txt[32];
	  cimg_mapoff(xvalues,x) {
	      if (precision>=0) snprintf(txt, sizeof(txt),"%.*g", precision, (double)xvalues(x));
	      else snprintf(txt, sizeof(txt), "%g", (double)xvalues(x));
	    const int xi=(int)(x*(width-1)/siz), xt = xi-(int)std::strlen(txt)*3;
	    draw_point(xi,y-1,color,opacity).draw_point(xi,y+1,color,opacity).
	      draw_text(txt,xt<0?0:xt,yt,color,0,11,opacity);
	  }
	}
      }
      return *this;
    }

    template<typename t> CImg& draw_axe(const int x, const CImg<t>& yvalues, const T *const color, 
					const int precision=-1, const float opacity=1.0f) {
      if (!is_empty()) {
	int siz = (int)yvalues.size()-1;
	if (siz<=0) draw_line(x,0,x,height-1,color,~0L,opacity);
	else {
	  if (yvalues[0]<yvalues[siz]) draw_arrow(x,0,x,height-1,color,30,5,~0L,opacity);
	  else draw_arrow(x,height-1,x,0,color,30,5,~0L,opacity);
	  char txt[32];
	  cimg_mapoff(yvalues,y) {
	      if (precision>=0) snprintf(txt, sizeof(txt), "%.*g",precision,(double)yvalues(y));
	      else snprintf(txt, sizeof(txt), "%g",(double)yvalues(y));
	    const int
	      yi = (int)(y*(height-1)/siz),
	      tmp = yi-5,
	      nyi = tmp<0?0:(tmp>=(int)height-11?(int)height-11:tmp),
	      xt = x-(int)std::strlen(txt)*7;
	    draw_point(x-1,yi,color,opacity).draw_point(x+1,yi,color,opacity);
	    if (xt>0) draw_text(txt,xt,nyi,color,0,11,opacity);
	    else draw_text(txt,x+3,nyi,color,0,11,opacity);
	  }
	}
      }
      return *this;
    }
    
    template<typename tx, typename ty> CImg& draw_axe(const CImg<tx>& xvalues, const CImg<ty>& yvalues, const T *const color,
						      const int precisionx=-1, const int precisiony=-1, const float opacity=1.0f) {
      if (!is_empty()) {
	const CImg<tx> nxvalues(xvalues.data,xvalues.size(),1,1,1,true);
	const int sizx = (int)xvalues.size()-1, wm1 = (int)(width)-1;
	if (sizx>0) {
	  float ox = (float)nxvalues[0];
	  for (unsigned int x=1; x<width; x++) {	   
	    const float nx = (float)nxvalues.linear_pix1d((float)x*sizx/wm1);
	    if (nx*ox<=0) { draw_axe(nx==0?x:x-1,yvalues,color,precisiony,opacity); break; }
	    ox = nx;
	  }
	}
	const CImg<ty> nyvalues(yvalues.data,yvalues.size(),1,1,1,true);		
	const int sizy = (int)yvalues.size()-1, hm1 = (int)(height)-1;
	if (sizy>0) {
	  float oy = (float)nyvalues[0];
	  for (unsigned int y=1; y<height; y++) {
	    const float ny = (float)nyvalues.linear_pix1d((float)y*sizy/hm1);
	    if (ny*oy<=0) { draw_axe(xvalues,ny==0?y:y-1,color,precisionx,opacity); break; }
	    oy = ny;
	  }
	}
      }
      return *this;
    }

    template<typename tx, typename ty> CImg& draw_axe(const tx& x0, const tx& x1, const ty& y0, const ty& y1,
						      const T *const color,
						      const int subdivisionx=-60, const int subdivisiony=-60,
						      const int precisionx=-1, const int precisiony=-1,
						      const float opacity=1.0f) {
      return draw_axe(CImg<tx>::sequence(subdivisionx>0?subdivisionx:-(int)width/subdivisionx,x0,x1),
		      CImg<ty>::sequence(subdivisiony>0?subdivisiony:-(int)height/subdivisiony,y0,y1),
		      color,precisionx,precisiony,opacity);
    }

    // INNER CLASS used by function CImg<>::draw_fill()
    template<typename T1,typename T2> struct _draw_fill {
      const T1 *const color;
      const float sigma,opacity;
      const CImg<T1> value;
      CImg<T2> region;      

      _draw_fill(const CImg<T1>& img,const int x,const int y,const int z,
                 const T *const pcolor,const float psigma,const float popacity):
        color(pcolor),sigma(psigma),opacity(popacity),
        value(img.get_vector_at(x,y,z)), region(CImg<T2>(img.width,img.height,img.depth,1,(T2)false)) {
      }
      
      _draw_fill& operator=(const _draw_fill& d) {
	color = d.color;
	sigma = d.sigma;
	opacity = d.opacity;
	value = d.value;
	region = d.region;
	return *this;
      }
    
      bool comp(const CImg<T1>& A,const CImg<T1>& B) const {
        bool res=true;
        const T *pA=A.data+A.size();
        for (const T *pB=B.data+B.size(); res && pA>A.data; res=(cimg::abs(*(--pA)-(*(--pB)))<=sigma) ) ;
        return res;
      }

      void fill(CImg<T1>& img,const int x,const int y,const int z) {
        if (x<0 || x>=img.dimx() || y<0 || y>=img.dimy() || z<0 || z>=img.dimz()) return;
        if (!region(x,y,z) && comp(value,img.get_vector_at(x,y,z))) {
          const T *col=color;
          const float nopacity = cimg::abs(opacity), copacity = 1-cimg::max(opacity,0.0f);
          int xmin,xmax;
          if (opacity>=1) cimg_mapV(img,k) img(x,y,z,k)=*(col++);
          else cimg_mapV(img,k) img(x,y,z,k)=(T1)(*(col++)*opacity+copacity*img(x,y,z,k));
          col-=img.dim;
          region(x,y,z) = (T2)true;
          for (xmin=x-1; xmin>=0 && comp(value,img.get_vector_at(xmin,y,z)); xmin--) {
            if (opacity>=1) cimg_mapV(img,k) img(xmin,y,z,k) = *(col++);
            else cimg_mapV(img,k) img(xmin,y,z,k)=(T1)(*(col++)*nopacity+copacity*img(xmin,y,z,k)); 
            col-=img.dim;
            region(xmin,y,z)=(T2)true;
          }
          for (xmax=x+1; xmax<img.dimx() && comp(value,img.get_vector_at(xmax,y,z)); xmax++) {
            if (opacity>=1) cimg_mapV(img,k) img(xmax,y,z,k) = *(col++);
            else cimg_mapV(img,k) img(xmax,y,z,k)=(T1)(*(col++)*nopacity+copacity*img(xmax,y,z,k));
            col-=img.dim;
            region(xmax,y,z)=(T2)true; 
          }
          xmin++; xmax--;
          for (; xmin<=xmax; xmin++) { 
            fill(img,xmin,y-1,z); 
            fill(img,xmin,y+1,z);
            fill(img,xmin,y,z-1); 
            fill(img,xmin,y,z+1);
          }
        }
      }        
    };

    CImg& draw_fill(const int x,const int y,const int z,const T *const color,const float sigma=0,const float opacity=1) {
      CImg<bool> tmp;
      return draw_fill(x,y,z,color,tmp,sigma,opacity);
    }

    //@}
    //----------------------------
    //
    //! \name Image Filtering
    //@{
    //----------------------------

#define cimg_deriche_map(x0,y0,z0,k0,nb,offset,T) { \
    ima = ptr(x0,y0,z0,k0); \
    I2 = *ima; ima+=offset; I1 = *ima; ima+=offset; \
    Y2 = *(Y++) = sumg0*I2; Y1 = *(Y++) = g0*I1 + sumg1*I2; \
    for (i=2; i<(nb); i++) { I1 = *ima; ima+=offset; \
        Y0 = *(Y++) = a1*I1 + a2*I2 + b1*Y1 + b2*Y2; \
        I2=I1; Y2=Y1; Y1=Y0; } \
    ima-=offset; I2 = *ima; Y2 = Y1 = parity*sumg1*I2; *ima = (T)(*(--Y)+Y2); \
    ima-=offset; I1 = *ima; *ima = (T)(*(--Y)+Y1); \
    for (i=(nb)-3; i>=0; i--) { Y0=a3*I1+a4*I2+b1*Y1+b2*Y2; ima-=offset; \
      I2=I1; I1=*ima; *ima=(T)(*(--Y)+Y0); Y2=Y1; Y1=Y0; } \
  }

    //! Apply a deriche filter on the image
    /**
       This is the in-place version of get_deriche
       \see get_deriche.
    **/
    CImg& deriche(const float sigma=1,const int order=0,const char axe='x',const unsigned int cond=1) {
      if (!is_empty()) {
	if (sigma<0 || order<0 || order>2)
	  throw CImgArgumentException("CImg<%s>::deriche() : Bad arguments (sigma=%g, order=%d)",pixel_type(),sigma,order);
	const float alpha=sigma>0?(1.695f/sigma):20,ea=(float)std::exp(alpha),ema=(float)std::exp(-alpha),em2a=ema*ema,b1=2*ema,b2=-em2a;
	float ek,ekn,parity,a1,a2,a3,a4,g0,sumg1,sumg0;
	double *Y,Y0,Y1,Y2;
	int i,offset,nb;
	T *ima,I1,I2;
	switch(order) {
	case 1:                 // first derivative
	  ek = -(1-ema)*(1-ema)*(1-ema)/(2*(ema+1)*ema); a1 = a4 = 0;  a2 = ek*ema; a3 = -ek*ema; parity =-1;
	  if (cond) { sumg1 = (ek*ea) / ((ea-1)*(ea-1)); g0 = 0; sumg0 = g0+sumg1; }
	  else g0 = sumg0 = sumg1 = 0;
	  break;
	case 2:               // second derivative
	  ekn = ( -2*(-1+3*ea-3*ea*ea+ea*ea*ea)/(3*ea+1+3*ea*ea+ea*ea*ea) );
	  ek = -(em2a-1)/(2*alpha*ema); a1 = ekn;  a2 = -ekn*(1+ek*alpha)*ema; a3 = ekn*(1-ek*alpha)*ema; a4 = -ekn*em2a; parity =1;
	  if (cond) { sumg1 = ekn/2; g0 = ekn; sumg0 = g0+sumg1; }
	  else g0=sumg0=sumg1=0;
	  break;
	default:              // smoothing
	  ek = (1-ema)*(1-ema) / (1+2*alpha*ema - em2a); a1 = ek;  a2 = ek*ema*(alpha-1); a3 = ek*ema*(alpha+1); a4 = -ek*em2a; parity = 1;
	  if (cond) { sumg1 = ek*(alpha*ea+ea-1) / ((ea-1)*(ea-1)); g0 = ek; sumg0 = g0+sumg1; }
	  else  g0=sumg0=sumg1=0;
	  break;
	}
	// filter init
	Y = new double[cimg::max(width,height,depth)];
	switch(cimg::uncase(axe)) {
	case 'x': if (width>1)  { offset = 1;            nb = width;  cimg_mapYZV(*this,y,z,k) cimg_deriche_map(0,y,z,k,nb,offset,T); }	break;
	case 'y': if (height>1) { offset = width;        nb = height; cimg_mapXZV(*this,x,z,k) cimg_deriche_map(x,0,z,k,nb,offset,T); }	break;
	case 'z': if (depth>1)  { offset = width*height; nb = depth;  cimg_mapXYV(*this,x,y,k) cimg_deriche_map(x,y,0,k,nb,offset,T); }	break;
	default: throw CImgArgumentException("CImg<%s>::deriche() : unknow axe '%c', must be 'x','y' or 'z'",pixel_type(),axe);
	}
	delete[] Y;
      }
      return *this;
    }

    //! Return the result of the Deriche filter
    /**
       The Canny-Deriche filter is a recursive algorithm allowing to compute blurred derivatives of
       order 0,1 or 2 of an image.
       \see blur
    **/
    // CImg get_deriche(const float sigma=1,const int order=0,const char axe='x',const unsigned int cond=1) const {
    //   return CImg<T>(*this,false).deriche(sigma,order,axe,cond);
    // }

    //! Blur the image with a Deriche filter (anisotropically)
    /**
       This is the in-place version of get_blur().
       \see get_blur().
    **/
    CImg& blur(const float sigmax,const float sigmay,const float sigmaz,const unsigned int cond=1) {
      if (!is_empty()) {
	if (width>1  && sigmax>0) deriche(sigmax,0,'x',cond);
	if (height>1 && sigmay>0) deriche(sigmay,0,'y',cond);
	if (depth>1  && sigmaz>0) deriche(sigmaz,0,'z',cond);
      }
      return *this;
    }
    
    //! Blur the image with a Canny-Deriche filter.
    /** This is the in-place version of get_blur(). **/
    CImg& blur(const float sigma,const unsigned int cond=1) { return blur(sigma,sigma,sigma,cond); }

    //! Return a blurred version of the image, using a Canny-Deriche filter.
    /**
       Blur the image with an anisotropic exponential filter (Deriche filter of order 0).
    **/
    // CImg get_blur(const float sigmax,const float sigmay,const float sigmaz,const unsigned int cond=1) const {
    //   return CImg<T>(*this,false).blur(sigmax,sigmay,sigmaz,cond); 
    // }
    
    //! Return a blurred version of the image, using a Canny-Deriche filter.
    CImg get_blur(const float sigma,const unsigned int cond=1) const {
      return CImg<T>(*this,false).blur(sigma,cond); 
    }

    //! Blur an image following a field of diffusion tensors.
    /** This is the in-place version of get_blur_anisotropic(). **/
    template<typename t> 
    CImg& blur_anisotropic(const CImg<t>& G, const float amplitude=30.0f, const float dl=0.8f,const float da=30.0f,
			   const float gauss_prec=2.0f, const unsigned int interpolation=0, const bool fast_approx=true) {
      
      // Check arguments and init variables
      if (!is_empty() && amplitude>0) {
	if (G.is_empty() || (G.dim!=3 && G.dim!=6) || G.width!=width || G.height!=height || G.depth!=depth)
	  throw CImgArgumentException("CImg<%s>::blur_anisotropic() : Specified tensor field (%u,%u,%u,%u) is not valid.",
				      pixel_type(),G.width,G.height,G.depth,G.dim);
	
	const int dx1 = dimx()-1, dy1 = dimy()-1, dz1 = dimz()-1;
	const bool threed = (G.dim>=6);
	CImg<t> dest(width,height,depth,dim,0), tmp(dim), W(width,height,depth,threed?4:3);
	int N = 0;
	const float sqrt2amplitude = (float)std::sqrt(2*amplitude);
	
	if (threed)
	  // 3D version of the algorithm
	  for (float phi=(180%(int)da)/2.0f; phi<=180; phi+=da) {
	    const float phir = (float)(phi*cimg::PI/180), datmp = (float)(da/std::cos(phir)), da2 = datmp<1?360.0f:datmp;
	    for (float theta=0; theta<360; (theta+=da2),N++) {
	      const float thetar = (float)(theta*cimg::PI/180),
		vx = (float)(std::cos(thetar)*std::cos(phir)),
		vy = (float)(std::sin(thetar)*std::cos(phir)),
		vz = (float)std::sin(phir);
	      const t 
		*pa = G.ptr(0,0,0,0), *pb = G.ptr(0,0,0,1), *pc = G.ptr(0,0,0,2),
		*pd = G.ptr(0,0,0,3), *pe = G.ptr(0,0,0,4), *pf = G.ptr(0,0,0,5);
	      t *pd0 = W.ptr(0,0,0,0), *pd1 = W.ptr(0,0,0,1), *pd2 = W.ptr(0,0,0,2), *pd3 = W.ptr(0,0,0,3);
	      cimg_mapXYZ(G,xg,yg,zg) {
		const t
		  a = *(pa++), b = *(pb++), c = *(pc++),
		  d = *(pd++), e = *(pe++), f = *(pf++),
		  u = a*vx + b*vy + c*vz,
		  v = b*vx + d*vy + e*vz,
		  w = c*vx + e*vy + f*vz,
		  n = (t)std::sqrt(1e-5+u*u+v*v+w*w),
		  dln = dl/n;
		*(pd0++) = u*dln;
		*(pd1++) = v*dln;
		*(pd2++) = w*dln;
		*(pd3++) = n;
	      }
	      
	      cimg_mapXYZ(*this,x,y,z) {
		tmp.fill(0);
		const t cu = W(x,y,z,0), cv = W(x,y,z,1), cw = W(x,y,z,2), n = W(x,y,z,3);
		const float
		  fsigma = (float)(n*sqrt2amplitude),
		  length = gauss_prec*fsigma,
		  fsigma2 = 2*fsigma*fsigma;
		float l, S=0, pu=cu, pv=cv, pw=cw, X=(float)x, Y=(float)y, Z=(float)z;
		if (fast_approx) switch (interpolation) {
		case 0: // Nearest neighbor interpolation
		  for (l=0; l<length; l+=dl) {
		    const float 
		      Xn = X<0?0:(X>=dx1?dx1:X),
		      Yn = Y<0?0:(Y>=dy1?dy1:Y),
		      Zn = Z<0?0:(Z>=dz1?dz1:Z);
		    const int xi = (int)(Xn+0.5f), yi = (int)(Yn+0.5f), zi = (int)(Zn+0.5f);
		    t u = W(xi,yi,zi,0), v = W(xi,yi,zi,1), w = W(xi,yi,zi,2);
		    if ((pu*u+pv*v+pw*w)<0) { u=-u; v=-v; w=-w; }
		    cimg_mapV(*this,k) tmp[k]+=(t)(*this)(xi,yi,zi,k);
		    X+=(pu=u); Y+=(pv=v); Z+=(pw=w); S++;
		  } break;
		case 1: // Linear interpolation
		  for (l=0; l<length; l+=dl) {
		    t u = (t)(W.linear_pix3d(X,Y,Z,0)), v = (t)(W.linear_pix3d(X,Y,Z,1)), w = (t)(W.linear_pix3d(X,Y,Z,2));
		    if ((pu*u+pv*v+pw*w)<0) { u=-u; v=-v; w=-w; }
		    cimg_mapV(*this,k) tmp[k]+=(t)linear_pix3d(X,Y,Z,k);
		    X+=(pu=u); Y+=(pv=v); Z+=(pw=w); S++;
		  } break;
		default: // 2nd order Runge Kutta interpolation
		  for (l=0; l<length; l+=dl) {
		    t u0 = (t)(0.5f*dl*W.linear_pix3d(X,Y,Z,0)), v0 = (t)(0.5f*dl*W.linear_pix3d(X,Y,Z,1)), w0 = (t)(0.5f*dl*W.linear_pix3d(X,Y,Z,2));
		    if ((pu*u0+pv*v0+pw*w0)<0) { u0=-u0; v0=-v0; w0=-w0; }
		    t u = (t)(W.linear_pix3d(X+u0,Y+v0,Z+w0,0)), v = (t)(W.linear_pix3d(X+u0,Y+v0,Z+w0,1)), w = (t)(W.linear_pix3d(X+u0,Y+v0,Z+w0,2));
		    if ((pu*u+pv*v+pw*w)<0) { u=-u; v=-v; w=-w; }
		    cimg_mapV(*this,k) tmp[k]+=(t)linear_pix3d(X,Y,Z,k);
		    X+=(pu=u); Y+=(pv=v); Z+=(pw=w); S++;
		  } break;
		}
		else switch (interpolation) {
		case 0: // Nearest neighbor interpolation
		  for (l=0; l<length; l+=dl) {
		    const float 
		      coef = (float)std::exp(-l*l/fsigma2),
		      Xn = X<0?0:(X>=dx1?dx1:X),
		      Yn = Y<0?0:(Y>=dy1?dy1:Y),
		      Zn = Z<0?0:(Z>=dz1?dz1:Z);
		    const int xi = (int)(Xn+0.5f), yi = (int)(Yn+0.5f), zi = (int)(Zn+0.5f);
		    t u = W(xi,yi,zi,0), v = W(xi,yi,zi,1), w = W(xi,yi,zi,2);
		    if ((pu*u+pv*v+pw*w)<0) { u=-u; v=-v; w=-w; }
		    cimg_mapV(*this,k) tmp[k]+=(t)(coef*(*this)(xi,yi,zi,k));
		    X+=(pu=u); Y+=(pv=v); Z+=(pw=w); S+=coef;
		  } break;
		case 1: // Linear interpolation
		  for (l=0; l<length; l+=dl) {
		    const float coef = (float)std::exp(-l*l/fsigma2);
		    t u = (t)(W.linear_pix3d(X,Y,Z,0)), v = (t)(W.linear_pix3d(X,Y,Z,1)), w = (t)(W.linear_pix3d(X,Y,Z,2));
		    if ((pu*u+pv*v+pw*w)<0) { u=-u; v=-v; w=-w; }
		    cimg_mapV(*this,k) tmp[k]+=(t)(coef*linear_pix3d(X,Y,Z,k));
		    X+=(pu=u); Y+=(pv=v); Z+=(pw=w); S+=coef;
		  } break;
		default: // 2nd order Runge Kutta interpolation
		  for (l=0; l<length; l+=dl) {
		    const float coef = (float)std::exp(-l*l/fsigma2);
		    t u0 = (t)(0.5f*dl*W.linear_pix3d(X,Y,Z,0)), v0 = (t)(0.5f*dl*W.linear_pix3d(X,Y,Z,1)), w0 = (t)(0.5f*dl*W.linear_pix3d(X,Y,Z,2));
		    if ((pu*u0+pv*v0+pw*w0)<0) { u0=-u0; v0=-v0; w0=-w0; }
		    t u = (t)(W.linear_pix3d(X+u0,Y+v0,Z+w0,0)), v = (t)(W.linear_pix3d(X+u0,Y+v0,Z+w0,1)), w = (t)(W.linear_pix3d(X+u0,Y+v0,Z+w0,2));
		    if ((pu*u+pv*v+pw*w)<0) { u=-u; v=-v; w=-w; }
		    cimg_mapV(*this,k) tmp[k]+=(t)(coef*linear_pix3d(X,Y,Z,k));
		    X+=(pu=u); Y+=(pv=v); Z+=(pw=w); S+=coef;
		  } break;
		}
		if (S>0) cimg_mapV(dest,k) dest(x,y,z,k)+=tmp[k]/S;
		else cimg_mapV(dest,k) dest(x,y,z,k)+=(t)((*this)(x,y,z,k));
	      }
	    }
	  } else
	    // 2D version of the algorithm
	    for (float theta=(360%(int)da)/2.0f; theta<360; (theta+=da),N++) {
	      const float thetar = (float)(theta*cimg::PI/180), vx = (float)(std::cos(thetar)), vy = (float)(std::sin(thetar));
	      
	      const t *pa = G.ptr(0,0,0,0), *pb = G.ptr(0,0,0,1), *pc = G.ptr(0,0,0,2);
	      t *pd0 = W.ptr(0,0,0,0), *pd1 = W.ptr(0,0,0,1), *pd2 = W.ptr(0,0,0,2);
	      cimg_mapXY(G,xg,yg) {
		const t
		  a = *(pa++), b = *(pb++), c = *(pc++), 
		  u = a*vx + b*vy, v = b*vx + c*vy,
		  n = (t)std::sqrt(1e-5+u*u+v*v),
		  dln = dl/n;
		*(pd0++) = u*dln;
		*(pd1++) = v*dln;
		*(pd2++) = n;
	      }
	      
	      cimg_mapXY(*this,x,y) {
		tmp.fill(0);
		const t cu = W(x,y,0,0), cv = W(x,y,0,1), n = W(x,y,0,2);
		const float
		  fsigma = (float)(n*sqrt2amplitude),
		  length = gauss_prec*fsigma,
		  fsigma2 = 2*fsigma*fsigma;
		float l, S=0, pu=cu, pv=cv, X=(float)x, Y=(float)y;
		if (fast_approx) switch (interpolation) {		  
		case 0: // Nearest-neighbor interpolation
		  for (l=0; l<length; l+=dl) {
		    const float 
		      Xn = X<0?0:(X>=dx1?dx1:X),
		      Yn = Y<0?0:(Y>=dy1?dy1:Y);
		    const int xi = (int)(Xn+0.5f), yi = (int)(Yn+0.5f);
		    t u = W(xi,yi,0,0), v = W(xi,yi,0,1);
		    if ((pu*u+pv*v)<0) { u=-u; v=-v; }
		    cimg_mapV(*this,k) tmp[k]+=(t)(*this)(xi,yi,0,k);
		    X+=(pu=u); Y+=(pv=v); S++;
		  } break;
		case 1: // Linear interpolation
		  for (l=0; l<length; l+=dl) {
		    t u = (t)(W.linear_pix2d(X,Y,0,0)), v = (t)(W.linear_pix2d(X,Y,0,1));
		    if ((pu*u+pv*v)<0) { u=-u; v=-v; }
		    cimg_mapV(*this,k) tmp[k]+=(t)linear_pix2d(X,Y,0,k);
		    X+=(pu=u); Y+=(pv=v); S++;
		  } break;
		default: // 2nd-order Runge-kutta interpolation
		  for (l=0; l<length; l+=dl) {
		    t u0 = (t)(0.5f*dl*W.linear_pix2d(X,Y,0,0)), v0 = (t)(0.5f*dl*W.linear_pix2d(X,Y,0,1));
		    if ((pu*u0+pv*v0)<0) { u0=-u0; v0=-v0; }
		    t u = (t)(W.linear_pix2d(X+u0,Y+v0,0,0)), v = (t)(W.linear_pix2d(X+u0,Y+v0,0,1));
		    if ((pu*u+pv*v)<0) { u=-u; v=-v; }
		    cimg_mapV(*this,k) tmp[k]+=(t)linear_pix2d(X,Y,0,k);
		    X+=(pu=u); Y+=(pv=v); S++;
		  } break;
		}
		else switch (interpolation) {		  
		case 0: // Nearest-neighbor interpolation
		  for (l=0; l<length; l+=dl) {
		    const float 
		      coef = (float)std::exp(-l*l/fsigma2),
		      Xn = X<0?0:(X>=dx1?dx1:X),
		      Yn = Y<0?0:(Y>=dy1?dy1:Y);
		    const int xi = (int)(Xn+0.5f), yi = (int)(Yn+0.5f);
		    t u = W(xi,yi,0,0), v = W(xi,yi,0,1);
		    if ((pu*u+pv*v)<0) { u=-u; v=-v; }
		    cimg_mapV(*this,k) tmp[k]+=(t)(coef*(*this)(xi,yi,0,k));
		    X+=(pu=u); Y+=(pv=v); S+=coef;
		  } break;
		case 1: // Linear interpolation
		  for (l=0; l<length; l+=dl) {
		    const float coef = (float)std::exp(-l*l/fsigma2);
		    t u = (t)(W.linear_pix2d(X,Y,0,0)), v = (t)(W.linear_pix2d(X,Y,0,1));
		    if ((pu*u+pv*v)<0) { u=-u; v=-v; }
		    cimg_mapV(*this,k) tmp[k]+=(t)(coef*linear_pix2d(X,Y,0,k));
		    X+=(pu=u); Y+=(pv=v); S+=coef;
		  } break;
		default: // 2nd-order Runge-kutta interpolation
		  for (l=0; l<length; l+=dl) {
		    const float coef = (float)std::exp(-l*l/fsigma2);
		    t u0 = (t)(0.5f*dl*W.linear_pix2d(X,Y,0,0)), v0 = (t)(0.5f*dl*W.linear_pix2d(X,Y,0,1));
		    if ((pu*u0+pv*v0)<0) { u0=-u0; v0=-v0; }
		    t u = (t)(W.linear_pix2d(X+u0,Y+v0,0,0)), v = (t)(W.linear_pix2d(X+u0,Y+v0,0,1));
		    if ((pu*u+pv*v)<0) { u=-u; v=-v; }
		    cimg_mapV(*this,k) tmp[k]+=(t)(coef*linear_pix2d(X,Y,0,k));
		    X+=(pu=u); Y+=(pv=v); S+=coef;
		  } break;
		}
		if (S>0) cimg_mapV(dest,k) dest(x,y,0,k)+=tmp[k]/S;
		else cimg_mapV(dest,k) dest(x,y,0,k)+=(t)((*this)(x,y,0,k));
	      }
	    }
	const float *ptrs = dest.data+dest.size(); cimg_map(*this,ptrd,T) *ptrd = (T)(*(--ptrs)/N);
      }
      return *this;
    }

    //! Blur an image following a field of diffusion tensors.
    CImg& blur_anisotropic(const float amplitude, const float sharpness=0.8f, const float anisotropy=0.3f,
			   const float alpha=1.2f,const float sigma=0.8f, const float dl=0.8f,const float da=30.0f,
			   const float gauss_prec=2.0f, const unsigned int interpolation=0, const bool fast_approx=true) {
      if (!is_empty() && amplitude>0) {
	if (amplitude==0) return *this;
	if (amplitude<0 || sharpness<0 || anisotropy<0 || anisotropy>1 || alpha<0 || sigma<0 || dl<0 || da<0 || gauss_prec<0)
	  throw CImgArgumentException("CImg<%s>::blur_anisotropic() : Given parameters are amplitude(%g), sharpness(%g), "
				      "anisotropy(%g), alpha(%g), sigma(%g), dl(%g), da(%g), gauss_prec(%g).\n"
				      "Admissible parameters are in the range : amplitude>0, sharpness>0, anisotropy in [0,1], "
				      "alpha>0, sigma>0, dl>0, da>0, gauss_prec>0.",
				      pixel_type(),amplitude,sharpness,anisotropy,alpha,sigma,dl,da,gauss_prec);
	const bool threed = (depth>1);
	CImg<float> G(width,height,depth,(threed?6:3),0);
	const float power1 = 0.5f*sharpness, power2 = power1/(1e-7f+1.0f-anisotropy);
	const CImgStats stats(*this,false);
	const float valmin = (float)stats.min, valmax = (float)stats.max;
	const CImg<float> blurred = CImg<float>(*this).blur(alpha).normalize(valmin,valmax);
	
	if (threed) { // Field for 3D volumes    
	  CImg<float> val(3),vec(3,3);
	  CImg_3x3x3(I,float);
	  cimg_mapV(*this,k) cimg_map3x3x3(blurred,x,y,z,k,I) {
	    const float 
	      ixf = Incc-Iccc, iyf = Icnc-Iccc, izf = Iccn-Iccc,
	      ixb = Iccc-Ipcc, iyb = Iccc-Icpc, izb = Iccc-Iccp;
	    G(x,y,z,0) += 0.5f*(ixf*ixf + ixb*ixb);
	    G(x,y,z,1) += 0.25f*(ixf*iyf + ixf*iyb + ixb*iyf + ixb*iyb);
	    G(x,y,z,2) += 0.25f*(ixf*izf + ixf*izb + ixb*izf + ixb*izb);
	    G(x,y,z,3) += 0.5f*(iyf*iyf + iyb*iyb);
	    G(x,y,z,4) += 0.25f*(iyf*izf + iyf*izb + iyb*izf + iyb*izb);
	    G(x,y,z,5) += 0.5f*(izf*izf + izb*izb);
	  }
	  if (sigma>0) G.blur(sigma);
	  cimg_mapXYZ(*this,x,y,z) {
	    G.get_tensor_at(x,y,z).symmetric_eigen(val,vec);
	    const float l1 = val[2], l2 = val[1], l3 = val[0],
	      ux = vec(0,0), uy = vec(0,1), uz = vec(0,2),
	      vx = vec(1,0), vy = vec(1,1), vz = vec(1,2),
	      wx = vec(2,0), wy = vec(2,1), wz = vec(2,2),
	      n1 = (float)std::pow(1.0f+l1+l2+l3,-power1),
	      n2 = (float)std::pow(1.0f+l1+l2+l3,-power2);
	    G(x,y,z,0) = n1*(ux*ux + vx*vx) + n2*wx*wx;
	    G(x,y,z,1) = n1*(ux*uy + vx*vy) + n2*wx*wy;
	    G(x,y,z,2) = n1*(ux*uz + vx*vz) + n2*wx*wz;
	    G(x,y,z,3) = n1*(uy*uy + vy*vy) + n2*wy*wy;
	    G(x,y,z,4) = n1*(uy*uz + vy*vz) + n2*wy*wz;
	    G(x,y,z,5) = n1*(uz*uz + vz*vz) + n2*wz*wz;
	  }
	} else { // Field for 2D images
	  CImg<float> val(2),vec(2,2);
	  CImg_3x3x1(I,float);
	  cimg_mapV(*this,k) cimg_map3x3x1(blurred,x,y,0,k,I) {
	    const float
	      ixf = Inc-Icc, iyf = Icn-Icc,
	      ixb = Icc-Ipc, iyb = Icc-Icp;
	    G(x,y,0,0) += 0.5f*(ixf*ixf+ixb*ixb);
	    G(x,y,0,1) += 0.25f*(ixf*iyf+ixf*iyb+ixb*iyf+ixb*iyb);
	    G(x,y,0,2) += 0.5f*(iyf*iyf+iyb*iyb);
	  }
	  if (sigma>0) G.blur(sigma);
	  cimg_mapXY(*this,x,y) {
	    G.get_tensor_at(x,y).symmetric_eigen(val,vec);
	    const float l1 = val[1], l2 = val[0],
	      ux = vec(1,0), uy = vec(1,1),
	      vx = vec(0,0), vy = vec(0,1),
	      n1 = (float)std::pow(1.0f+l1+l2,-power1),
	      n2 = (float)std::pow(1.0f+l1+l2,-power2);
	    G(x,y,0,0) = n1*ux*ux + n2*vx*vx;
	    G(x,y,0,1) = n1*ux*uy + n2*vx*vy;
	    G(x,y,0,2) = n1*uy*uy + n2*vy*vy;
	  }
	}
	blur_anisotropic(G,amplitude,dl,da,gauss_prec,interpolation,fast_approx);
      }
      return *this;
    }

    //! Blur an image in an anisotropic way.
    CImg get_blur_anisotropic(const float amplitude, const float sharpness=0.8f, const float anisotropy=0.3f,
			      const float alpha=1.2f, const float sigma=0.8f, const float dl=0.8f,
			      const float da=30.0f, const float gauss_prec=2.0f, const unsigned int interpolation=0,
			      const bool fast_approx=true) const {
      
      return CImg<T>(*this,false).blur_anisotropic(amplitude,sharpness,anisotropy,alpha,sigma,dl,da,gauss_prec,interpolation,fast_approx);
    }

    //@}
    //-----------------------------
    //
    //! \name Matrix and Vectors
    //@{
    //-----------------------------

    //! Return a vector with specified coefficients
    static CImg vector(const T& a1) {
      return CImg<T>(1,1).fill(a1); 
    }

    //! Return a vector with specified coefficients
    static CImg vector(const T& a1,const T& a2) {
      return CImg<T>(1,2).fill(a1,a2); 
    }

    //! Return a vector with specified coefficients
    static CImg vector(const T& a1,const T& a2,const T& a3) { 
      return CImg<T>(1,3).fill(a1,a2,a3); 
    }

    //! Return a vector with specified coefficients
    static CImg vector(const T& a1,const T& a2,const T& a3,const T& a4) {
      return CImg<T>(1,4).fill(a1,a2,a3,a4); 
    }

    //! Return a vector with specified coefficients
    static CImg vector(const T& a1,const T& a2,const T& a3,const T& a4,const T& a5) {
      return CImg<T>(1,5).fill(a1,a2,a3,a4,a5); 
    }

    //! Return a vector with specified coefficients
    static CImg vector(const T& a1,const T& a2,const T& a3,const T& a4,const T& a5,const T& a6) { 
      return CImg<T>(1,6).fill(a1,a2,a3,a4,a5,a6); 
    }

    //! Return a vector with specified coefficients
    static CImg vector(const T& a1,const T& a2,const T& a3,const T& a4,
		       const T& a5,const T& a6,const T& a7) {
      return CImg<T>(1,7).fill(a1,a2,a3,a4,a5,a6,a7); 
    }

    //! Return a vector with specified coefficients
    static CImg vector(const T& a1,const T& a2,const T& a3,const T& a4,
		       const T& a5,const T& a6,const T& a7,const T& a8) { 
      return CImg<T>(1,8).fill(a1,a2,a3,a4,a5,a6,a7,a8); 
    }

    //! Return a vector with specified coefficients
    static CImg vector(const T& a1,const T& a2,const T& a3,const T& a4,
		       const T& a5,const T& a6,const T& a7,const T& a8,const T& a9) { 
      return CImg<T>(1,9).fill(a1,a2,a3,a4,a5,a6,a7,a8,a9); 
    }

    //! Return a vector with specified coefficients
    static CImg vector(const T& a1,const T& a2,const T& a3,const T& a4,
		       const T& a5,const T& a6,const T& a7,const T& a8,
		       const T& a9,const T& a10) { 
      return CImg<T>(1,10).fill(a1,a2,a3,a4,a5,a6,a7,a8,a9,a10); 
    }

    //! Return a vector with specified coefficients
    static CImg vector(const T& a1,const T& a2,const T& a3,const T& a4,
		       const T& a5,const T& a6,const T& a7,const T& a8,
		       const T& a9,const T& a10, const T& a11) { 
      return CImg<T>(1,11).fill(a1,a2,a3,a4,a5,a6,a7,a8,a9,a10,a11); 
    }

    //! Return a vector with specified coefficients
    static CImg vector(const T& a1,const T& a2,const T& a3,const T& a4,
		       const T& a5,const T& a6,const T& a7,const T& a8,
		       const T& a9,const T& a10, const T& a11, const T& a12) { 
      return CImg<T>(1,12).fill(a1,a2,a3,a4,a5,a6,a7,a8,a9,a10,a11,a12); 
    }  

    //! Return a vector with specified coefficients
    static CImg vector(const T& a1,const T& a2,const T& a3,const T& a4,
		       const T& a5,const T& a6,const T& a7,const T& a8,
		       const T& a9,const T& a10, const T& a11, const T& a12,
		       const T& a13) { 
      return CImg<T>(1,13).fill(a1,a2,a3,a4,a5,a6,a7,a8,a9,a10,a11,a12,a13); 
    }

    //! Return a 1x1 square matrix with specified coefficients
    static CImg matrix(const T& a1) {
      return vector(a1); 
    }

    //! Return a 2x2 square matrix with specified coefficients
    static CImg matrix(const T& a1,const T& a2,
		       const T& a3,const T& a4) { 
      return CImg<T>(2,2).fill(a1,a2,
			       a3,a4);
    }

    //! Return a 3x3 square matrix with specified coefficients
    static CImg matrix(const T& a1,const T& a2,const T& a3,
		       const T& a4,const T& a5,const T& a6,
		       const T& a7,const T& a8,const T& a9) {
      return CImg<T>(3,3).fill(a1,a2,a3,
			       a4,a5,a6,
			       a7,a8,a9);
    }

    //! Return a 4x4 square matrix with specified coefficients
    static CImg matrix(const T& a1,const T& a2,const T& a3,const T& a4,
		       const T& a5,const T& a6,const T& a7,const T& a8,
		       const T& a9,const T& a10,const T& a11,const T& a12,
		       const T& a13,const T& a14,const T& a15,const T& a16) {
      return CImg<T>(4,4).fill(a1,a2,a3,a4,
			       a5,a6,a7,a8,
			       a9,a10,a11,a12,
			       a13,a14,a15,a16);
    }

    //! Return a 5x5 square matrix with specified coefficients
    static CImg matrix(const T& a1,const T& a2,const T& a3,const T& a4,const T& a5,
		       const T& a6,const T& a7,const T& a8,const T& a9,const T& a10,
		       const T& a11,const T& a12,const T& a13,const T& a14,const T& a15,
		       const T& a16,const T& a17,const T& a18,const T& a19,const T& a20,
		       const T& a21,const T& a22,const T& a23,const T& a24,const T& a25) {
      return CImg<T>(5,5).fill(a1,a2,a3,a4,a5,
			       a6,a7,a8,a9,a10,
			       a11,a12,a13,a14,a15,
			       a16,a17,a18,a19,a20,
			       a21,a22,a23,a24,a25);
    }

    CImg& matrix() {
      const unsigned int siz = size();
      switch (siz) {
      case 1: break;
      case 4: width = height = 2; break;
      case 9: width = height = 3; break;
      case 16: width = height = 4; break;
      case 25: width = height = 5; break;
      case 36: width = height = 6; break;
      case 49: width = height = 7; break;
      case 64: width = height = 8; break;
      case 81: width = height = 9; break;
      case 100: width = height = 10; break;
      default: {
	unsigned int i=11, i2=i*i;
	while (i2<siz) { i2+=2*i+1; i++; }
	if (i2==siz) width = height = i;
	else throw CImgInstanceException("CImg<%s>::matrix() : Image size = %u is not a square number",pixel_type(),siz);
      } break;
      }
      return *this;
    }

    CImg get_matrix() const {
      return CImg<T>(*this).matrix();
    }

    //! Return a 1x1 symmetric matrix with specified coefficients
    static CImg tensor(const T& a1) { 
      return matrix(a1); 
    }

    //! Return a 2x2 symmetric matrix tensor with specified coefficients
    static CImg tensor(const T& a1,const T& a2,const T& a3) { 
      return matrix(a1,a2,
		    a2,a3); 
    }

    //! Return a 3x3 symmetric matrix with specified coefficients
    static CImg tensor(const T& a1,const T& a2,const T& a3,const T& a4,const T& a5,const T& a6) {
      return matrix(a1,a2,a3,
		    a2,a4,a5,
		    a3,a5,a6);
    }

    CImg get_tensor() const {
      CImg<T> res;
      const unsigned int siz = size();
      switch (siz) {
      case 1: break;
      case 3: 
	res.assign(2,2);
	res(0,0) = (*this)(0);
	res(1,0) = res(0,1) = (*this)(1);
	res(1,1) = (*this)(2);
	break;
      case 6:
	res.assign(3,3);
	res(0,0) = (*this)(0);
	res(1,0) = res(0,1) = (*this)(1);
	res(2,0) = res(0,2) = (*this)(2);
	res(1,1) = (*this)(3);
	res(2,1) = res(1,2) = (*this)(4);
	res(2,2) = (*this)(5);
	break;
      default:
	throw CImgInstanceException("CImg<%s>::get_tensor() : Wrong vector dimension = %u in instance image.",
				    pixel_type(), dim);
	break;
      }
      return res;
    }

    CImg& tensor() {
      return get_tensor().swap(*this);
    }

    //! Return a 2x2 diagonal matrix with specified coefficients
    static CImg diagonal(const T& a1,const T& a2) { 
      return matrix(a1,0,
		    0,a2); 
    }

    //! Return a N-numbered sequence vector from \p a0 to \p a1
    CImg& sequence(const T& a0, const T& a1) {
      const unsigned int siz = size()-1;
      const float delta = (float)((float)a1-a0);
      T* ptr = data;      
      cimg_mapoff(*this,l) *(ptr++) = (T)(a0 + delta*l/siz);
      return *this;
    }
    
    static CImg sequence(const unsigned N, const T& a0, const T& a1) {
      return CImg<T>(N).sequence(a0,a1);
    }
  
    //! Return a new image corresponding to the \a diffusion \a tensor located at (\p x,\p y,\p z) of the current vector-valued image.
    CImg get_tensor_at(const unsigned int x=0,const unsigned int y=0,const unsigned int z=0) const {      
      if (dim==6) return tensor((*this)(x,y,z,0),(*this)(x,y,z,1),(*this)(x,y,z,2),
					  (*this)(x,y,z,3),(*this)(x,y,z,4),(*this)(x,y,z,5));
      if (dim==3) return tensor((*this)(x,y,z,0),(*this)(x,y,z,1),(*this)(x,y,z,2));
      return tensor((*this)(x,y,z,0));
    }

    //! Compute the SVD of a general matrix.   
    template<typename t> const CImg& SVD(CImg<t>& U, CImg<t>& S, CImg<t>& V,
					 const bool sorting=true, const unsigned int max_iter=40) const {
      if (is_empty()) { U.assign(); S.assign(); V.assign(); }
      else {
	U = *this;
	if (S.size()<width) S.assign(1,width);
	if (V.width<width || V.height<height) V.assign(width,width);
	CImg<t> rv1(width);  
	t anorm=0,c,f,g=0,h,s,scale=0;
	int l=0,nm=0;
	
	cimg_mapX(U,i) {
	  l = i+1; rv1[i] = scale*g; g = s = scale = 0;
	  if (i<dimy()) {
	    for (int k=i; k<dimy(); k++) scale+= cimg::abs(U(i,k));
	    if (scale) {
	      for (int k=i; k<dimy(); k++) { U(i,k)/=scale; s+= U(i,k)*U(i,k); }
	      f = U(i,i); g = (t)((f>=0?-1:1)*std::sqrt(s)); h=f*g-s; U(i,i)=f-g;
	      for (int j=l; j<dimx(); j++) {
		s = 0; for (int k=i; k<dimy(); k++) s+= U(i,k)*U(j,k);
		f = s/h;
		{ for (int k=i; k<dimy(); k++) U(j,k)+= f*U(i,k); }
	      }
	      { for (int k=i; k<dimy(); k++) U(i,k)*= scale; }
	    }
	  }
	  S[i]=scale*g;
	  
	  g = s = scale = 0;
	  if (i<dimy() && i!=dimx()-1) {
	    for (int k=l; k<dimx(); k++) scale += cimg::abs(U(k,i));
	    if (scale) {
	      for (int k=l; k<dimx(); k++) { U(k,i)/= scale; s+= U(k,i)*U(k,i); }
	      f = U(l,i); g = (t)((f>=0?-1:1)*std::sqrt(s)); h = f*g-s; U(l,i) = f-g;
	      { for (int k=l; k<dimx(); k++) rv1[k]=U(k,i)/h; }
	      for (int j=l; j<dimy(); j++) {
		s=0; for (int k=l; k<dimx(); k++) s+= U(k,j)*U(k,i);
		{ for (int k=l; k<dimx(); k++) U(k,j)+= s*rv1[k]; }
	      }
	      { for (int k=l; k<dimx(); k++) U(k,i)*= scale; }
	    }
	  }
	  anorm=cimg::max((t)anorm,(cimg::abs(S[i])+cimg::abs(rv1[i])));
	}
	
	{ for (int i=dimx()-1; i>=0; i--) {
	  if (i<dimx()-1) {
	    if (g) {
	      { for (int j=l; j<dimx(); j++) V(i,j) =(U(j,i)/U(l,i))/g; }
	      for (int j=l; j<dimx(); j++) {
		s=0; for (int k=l; k<dimx(); k++) s+= U(k,i)*V(j,k);
		{ for (int k=l; k<dimx(); k++) V(j,k)+= s*V(i,k); }
	      }
	    }
	    for (int j=l; j<dimx(); j++) V(j,i)=V(i,j)=0.0;
	  }
	  V(i,i) = 1.0; g = rv1[i]; l = i;
	}
	}
	
	{ for (int i=cimg::min(dimx(),dimy())-1; i>=0; i--) {
	  l = i+1; g = S[i];
	  for (int j=l; j<dimx(); j++) U(j,i) = 0;
	  if (g) {
	    g = 1/g;
	    for (int j=l; j<dimx(); j++) {
	      s=0; for (int k=l; k<dimy(); k++) s+= U(i,k)*U(j,k);
	      f = (s/U(i,i))*g;
	      { for (int k=i; k<dimy(); k++) U(j,k)+= f*U(i,k); }
	    }
	    { for (int j=i; j<dimy(); j++) U(i,j)*= g; }
	  } else for (int j=i; j<dimy(); j++) U(i,j)=0;
	  U(i,i)++;
	}
	}
	
	for (int k=dimx()-1; k>=0; k--) {
	  for (unsigned int its=0; its<max_iter; its++) {
	    bool flag = true;
	    for (l=k; l>=0; l--) {
	      nm = l-1;
	      if ((cimg::abs(rv1[l])+anorm)==anorm) { flag = false; break; }
	      if ((cimg::abs(S[nm])+anorm)==anorm) break;
	    }
	    if (flag) {
	      c = 0; s = 1;
	      for (int i=l; i<=k; i++) {
		f = s*rv1[i]; rv1[i] = c*rv1[i];
		if ((cimg::abs(f)+anorm)==anorm) break;
		g = S[i]; h = (t)cimg::pythagore(f,g); S[i] = h; h = 1/h; c = g*h; s = -f*h;
		cimg_mapY(U,j) { const t y = U(nm,j), z = U(i,j); U(nm,j) = y*c+z*s; U(i,j) = z*c-y*s; }
	      }
	    }
	    const t& z = S[k];
	    if (l==k) { if (z<0) { S[k] = -z; cimg_mapX(U,j) V(k,j) = -V(k,j); } break; }
	    nm = k-1; 
	    t x = S[l], y = S[nm]; 
	    g = rv1[nm]; h = rv1[k];
	    f = ((y-z)*(y+z)+(g-h)*(g+h))/(2*h*y);
	    g = (t)cimg::pythagore(f,1.0);
	    f = ((x-z)*(x+z)+h*((y/(f+ (f>=0?g:-g)))-h))/x;
	    c = s = 1;
	    for (int j=l; j<=nm; j++) {
	      const int i = j+1;
	      g = rv1[i]; h = s*g; g = c*g;
	      t y = S[i];
	      t z = (t)cimg::pythagore(f,h); 
	      rv1[j] = z; c = f/z; s = h/z;
	      f = x*c+g*s; g = g*c-x*s; h = y*s; y*=c;
	      cimg_mapX(U,jj) { const t x = V(j,jj), z = V(i,jj); V(j,jj) = x*c+z*s; V(i,jj) = z*c-x*s; }
	      z = (t)cimg::pythagore(f,h); S[j] = z;
	      if (z) { z = 1/z; c = f*z; s = h*z; }
	      f = c*g+s*y; x = c*y-s*g;
	      { cimg_mapY(U,jj) { const t y = U(j,jj); z = U(i,jj); U(j,jj) = y*c+z*s; U(i,jj) = z*c-y*s; }}
	    }
	    rv1[l] = 0; rv1[k]=f; S[k]=x;
	  }
	}
	
	if (sorting) {
	  CImg<int> permutations(width);
	  CImg<t> tmp(width);
	  S.sort(permutations,false);
	  cimg_mapY(U,k) {
	    cimg_mapX(permutations,x) tmp(x) = U(permutations(x),k);
	    std::memcpy(U.ptr(0,k),tmp.data,sizeof(t)*width);
	  }
	  { cimg_mapY(V,k) {
	    cimg_mapX(permutations,x) tmp(x) = V(permutations(x),k);
	    std::memcpy(V.ptr(0,k),tmp.data,sizeof(t)*width);
	  }}	  
	}
      }
      return *this;
    }

    //! Compute the eigenvalues and eigenvectors of a matrix.
    template<typename t> const CImg<T>& eigen(CImg<t>& val, CImg<t> &vec) const {
      if (is_empty()) { val.assign(); vec.assign(); }
      else {
	if (width!=height || depth>1 || dim>1)
	  throw CImgInstanceException("CImg<%s>::eigen() : Instance object (%u,%u,%u,%u,%p) is empty.",
				      pixel_type(),width,height,depth,dim,data);
	if (val.size()<width) val.assign(1,width);
	if (vec.size()<width*width) vec.assign(width,width);
	switch(width) {
	case 1: { val[0]=(t)(*this)[0]; vec[0]=(t)1; } break;
	case 2: {
	  const double a = (*this)[0], b = (*this)[1], c = (*this)[2], d = (*this)[3], e = a+d;
	  double f = e*e-4*(a*d-b*c);
	  f = std::sqrt(f);
	  const double l1 = 0.5*(e-f), l2 = 0.5*(e+f);
	  const double theta1 = std::atan2(l2-a,b), theta2 = std::atan2(l1-a,b);
	  val[0]=(t)l2;
	  val[1]=(t)l1;
	  vec(0,0) = (t)std::cos(theta1);
	  vec(0,1) = (t)std::sin(theta1);
	  vec(1,0) = (t)std::cos(theta2);
	  vec(1,1) = (t)std::sin(theta2);
	} break;
	default: 
	  throw CImgInstanceException("CImg<%s>::eigen() : Eigenvalues computation of general matrices is limited"
				      "to 2x2 matrices (given is %ux%u)", pixel_type(),width,height);
	}
      }
      return *this;
    }

    //! Compute the eigenvalues and eigenvectors of a symmetric matrix.
    template<typename t> const CImg<T>& symmetric_eigen(CImg<t>& val, CImg<t>& vec) const {
      if (is_empty()) { val.assign(); vec.assign(); }
      else {
	if (width!=height || depth>1 || dim>1)
	  throw CImgInstanceException("CImg<%s>::eigen() : Instance object (%u,%u,%u,%u,%p) is empty.",
				      pixel_type(),width,height,depth,dim,data);
	
	if (val.size()<width) val.assign(1,width);
	if (vec.data && vec.size()<width*width) vec.assign(width,width);
	if (width<3) return eigen(val,vec);     
	CImg<t> V(width,width);
	SVD(vec,val,V,false);
	cimg_mapX(vec,x) {       // check for negative eigenvalues
	  t scal=0;
	  cimg_mapY(vec,y) scal+=vec(x,y)*V(x,y);
	  if (scal<0) val[x]=-val[x];
	}
	CImg<int> permutations(width);  // sort eigenvalues in decreasing order
	CImg<t> tmp(width);
	val.sort(permutations,false);
	cimg_mapY(vec,k) {
	  cimg_mapX(permutations,x) tmp(x) = vec(permutations(x),k);
	  std::memcpy(vec.ptr(0,k),tmp.data,sizeof(t)*width);
	}
      }
      return *this;
    }
    
template<typename t> CImg<T>& _quicksort(const int min,const int max,CImg<t>& permutations,const bool increasing) {
      if (min<max) {      
	const int mid = (min+max)/2;
	if (increasing) {
	  if ((*this)[min]>(*this)[mid]) {
	    cimg::swap((*this)[min],(*this)[mid]); cimg::swap(permutations[min],permutations[mid]); }
	  if ((*this)[mid]>(*this)[max]) {
	    cimg::swap((*this)[max],(*this)[mid]); cimg::swap(permutations[max],permutations[mid]); }
	  if ((*this)[min]>(*this)[mid]) {
	    cimg::swap((*this)[min],(*this)[mid]); cimg::swap(permutations[min],permutations[mid]); }
	} else {
	  if ((*this)[min]<(*this)[mid]) {
	    cimg::swap((*this)[min],(*this)[mid]); cimg::swap(permutations[min],permutations[mid]); }
	  if ((*this)[mid]<(*this)[max]) {
	    cimg::swap((*this)[max],(*this)[mid]); cimg::swap(permutations[max],permutations[mid]); }
	  if ((*this)[min]<(*this)[mid]) {
	    cimg::swap((*this)[min],(*this)[mid]); cimg::swap(permutations[min],permutations[mid]); }
	}
	if (max-min>=3) {
	  const T pivot = (*this)[mid];
	  int i = min, j = max;
	  if (increasing) {
	    do {
	      while ((*this)[i]<pivot) i++;
	      while ((*this)[j]>pivot) j--;
	      if (i<=j) {
		cimg::swap((*this)[i],(*this)[j]);
		cimg::swap(permutations[i++],permutations[j--]);
	      }
	    } while (i<=j);
	  } else {
	    do {
	      while ((*this)[i]>pivot) i++;
	      while ((*this)[j]<pivot) j--;
	      if (i<=j) {
		cimg::swap((*this)[i],(*this)[j]);
		cimg::swap(permutations[i++],permutations[j--]);
	      }
	    } while (i<=j);
	  }
	  if (min<j) _quicksort(min,j,permutations,increasing);
	  if (i<max) _quicksort(i,max,permutations,increasing);
	}
      }
      return *this;
    }

    //! Sort values of a vector and get permutations.
    template<typename t>
    CImg<T>& sort(CImg<t>& permutations,const bool increasing=true) {
      if (is_empty()) permutations.assign();
      else {
	if (permutations.size()!=size()) permutations.assign(size());
	cimg_mapoff(permutations,off) permutations[off] = off;
	_quicksort(0,size()-1,permutations,increasing); 
      }
      return *this;
    }

    //@}
    //-------------------
    //
    //! \name Display
    //@{
    //-------------------
 
    //! Display an image into a CImgDisplay window.
    const CImg& display(CImgDisplay& disp) const { disp.display(*this); return *this; }

    //! Display an image in a window, with a default title. See also \see display() for details on parameters.
    const CImg& display(const int min_size=128,const int max_size=1024) const { return display(" ",min_size,max_size); }
    
    //@}
    //----------------------
    //
    //! \name Input-Output
    //@{
    //----------------------


    //! Load a YUV image sequence file.
    static CImg get_load_yuv(const char *filename,
			 const unsigned int sizex, const unsigned int sizey, 
			 const unsigned int first_frame=0, const int last_frame=-1,
			 const bool yuv2rgb = true) {
      return CImgl<T>::get_load_yuv(filename,sizex,sizey,first_frame,last_frame,yuv2rgb).get_append('z','c');
    }

    //! Load a YUV image sequence file (in-place).
    CImg& load_yuv(const char *filename,
		   const unsigned int sizex, const unsigned int sizey, 
		   const unsigned int first_frame=0, const int last_frame=-1,
		   const bool yuv2rgb = true) { 
      return get_load_yuv(filename,sizex,sizey,first_frame,last_frame,yuv2rgb).swap(*this); 
    }

    //! Load an image from an ANALYZE7.5 file
    static CImg get_load_analyze(const char *filename, float *voxsize = 0) {

      // Open header and data files
      std::FILE *file_header=0, *file=0;
      char body[1024];
      const char *ext = cimg::filename_split(filename,body);
      if (!cimg::strcasecmp(ext,"hdr") || !cimg::strcasecmp(ext,"img")) {
	std::sprintf(body+cimg::strlen(body),".hdr");
	file_header = cimg::fopen(body,"rb");
	std::sprintf(body+cimg::strlen(body)-3,"img");
	file = cimg::fopen(body,"rb");
      } else throw CImgIOException("CImg<%s>::get_load_analyze() : Cannot load filename '%s' as an analyze format",pixel_type(),filename);

      // Read header
      bool endian = false;
      unsigned int header_size;
      cimg::fread(&header_size,1,file_header);
      if (header_size>=4096) { endian = true; cimg::endian_swap(header_size); }
      unsigned char *header = new unsigned char[header_size];
      cimg::fread(header+4,header_size-4,file_header);
      cimg::fclose(file_header);
      if (endian) {
	cimg::endian_swap((short*)(header+40),5);
        cimg::endian_swap((short*)(header+70),1);
        cimg::endian_swap((short*)(header+72),1);
        cimg::endian_swap((float*)(header+76),4);
        cimg::endian_swap((float*)(header+112),1);
      }
      unsigned short *dim = (unsigned short*)(header+40), dimx=1, dimy=1, dimz=1, dimv=1;
      if (dim[0]>=1) dimx = dim[1];
      if (dim[0]>=2) dimy = dim[2];
      if (dim[0]>=3) dimz = dim[3];
      if (dim[0]>=4) dimv = dim[4];
      
      float scalefactor = *(float*)(header+112); if (scalefactor==0) scalefactor=1;
      const unsigned short datatype = *(short*)(header+70);
      if (voxsize) { const float *vsize = (float*)(header+76); voxsize[0] = vsize[1]; voxsize[1] = vsize[2]; voxsize[2] = vsize[3]; }
      delete[] header;

      // Read pixel data
      CImg dest(dimx,dimy,dimz,dimv);
      switch (datatype) {
      case 2: {
	unsigned char *buffer = new unsigned char[dimx*dimy*dimz*dimv];
	cimg::fread(buffer,dimx*dimy*dimz*dimv,file);
	cimg_mapoff(dest,off) dest.data[off] = (T)(buffer[off]*scalefactor);
	delete[] buffer;
      } break;
      case 4: {
	short *buffer = new short[dimx*dimy*dimz*dimv];
	cimg::fread(buffer,dimx*dimy*dimz*dimv,file);
	if (endian) cimg::endian_swap(buffer,dimx*dimy*dimz*dimv);
	cimg_mapoff(dest,off) dest.data[off] = (T)(buffer[off]*scalefactor);
	delete[] buffer;
      } break;
      case 8: {
	int *buffer = new int[dimx*dimy*dimz*dimv];
	cimg::fread(buffer,dimx*dimy*dimz*dimv,file);
	if (endian) cimg::endian_swap(buffer,dimx*dimy*dimz*dimv);
	cimg_mapoff(dest,off) dest.data[off] = (T)(buffer[off]*scalefactor);
	delete[] buffer;
      } break;
      case 16: {
	float *buffer = new float[dimx*dimy*dimz*dimv];
	cimg::fread(buffer,dimx*dimy*dimz*dimv,file);
	if (endian) cimg::endian_swap(buffer,dimx*dimy*dimz*dimv);
	cimg_mapoff(dest,off) dest.data[off] = (T)(buffer[off]*scalefactor);
	delete[] buffer;
      } break;
      case 64: {
	double *buffer = new double[dimx*dimy*dimz*dimv];
	cimg::fread(buffer,dimx*dimy*dimz*dimv,file);
	if (endian) cimg::endian_swap(buffer,dimx*dimy*dimz*dimv);
	cimg_mapoff(dest,off) dest.data[off] = (T)(buffer[off]*scalefactor);
	delete[] buffer;
      } break;
      default: throw CImgIOException("CImg<%s>::get_load_analyze() : Cannot read images width 'datatype = %d'",pixel_type(),datatype);
      }
      cimg::fclose(file);
      return dest;
    }

    //! In-place version of get_load_analyze()
    CImg& load_analyze(const char *filename, float *voxsize = 0) {
      return get_load_analyze(filename,voxsize).swap(*this); 
    }

    //! Load PAR-REC (Philips) image file
    static CImg get_load_parrec(const char *filename,const char axe='v',const char align='p') {
      return CImgl<T>::get_load_parrec(filename).get_append(axe,align);
    }

    CImg& load_parrecX(const char *filename, const char axis='v', const char align='p') {
      return get_load_parrec(filename,axis,align).swap(*this);
    }
    
    //! Save the image as an ANALYZE7.5 file.
    const CImg& save_analyze(const char *filename,const float *const voxsize=0) const {
      if (is_empty()) throw CImgInstanceException("CImg<%s>::save_analyze() : Instance image (%u,%u,%u,%u,%p) is empty.",
						  pixel_type(),width,height,depth,dim,data);
      if (!filename) throw CImgArgumentException("CImg<%s>::save_analyze() : Specified filename is (null).",pixel_type());
      std::FILE *file;
      char header[348],hname[1024],iname[1024];
      const char *ext = cimg::filename_split(filename);
      short datatype=-1;
      std::memset(header,0,348);
      if (!ext[0]) { snprintf(hname,sizeof(hname), "%s.hdr",filename); snprintf(iname,sizeof(iname),"%s.img",filename); }
      if (!cimg::strncasecmp(ext,"hdr",3)) { 
	std::strcpy(hname,filename); std::strcpy(iname,filename); std::sprintf(iname+cimg::strlen(iname)-3,"img"); 
      }
      if (!cimg::strncasecmp(ext,"img",3)) {
	std::strcpy(hname,filename);
	std::strcpy(iname,filename);
	std::sprintf(hname+cimg::strlen(iname)-3,"hdr"); 
      }
      int *p=(int*)header;
      *p = 348;
      std::sprintf(header+4,"CImg");
      std::sprintf(header+14," ");
      ((short*)(header+36))[0] = 4096;
      ((char*)(header+38))[0] = 114;
      ((short*)(header+40))[0] = 4;
      ((short*)(header+40))[1] = width;
      ((short*)(header+40))[2] = height;
      ((short*)(header+40))[3] = depth;
      ((short*)(header+40))[4] = dim;      
      if (!cimg::strcasecmp(pixel_type(),"bool"))           datatype = 2;
      if (!cimg::strcasecmp(pixel_type(),"unsigned char"))  datatype = 2;
      if (!cimg::strcasecmp(pixel_type(),"char"))           datatype = 2;
      if (!cimg::strcasecmp(pixel_type(),"unsigned short")) datatype = 4;
      if (!cimg::strcasecmp(pixel_type(),"short"))          datatype = 4;
      if (!cimg::strcasecmp(pixel_type(),"unsigned int"))   datatype = 8;
      if (!cimg::strcasecmp(pixel_type(),"int"))            datatype = 8;
      if (!cimg::strcasecmp(pixel_type(),"unsigned long"))  datatype = 8;
      if (!cimg::strcasecmp(pixel_type(),"long"))           datatype = 8;
      if (!cimg::strcasecmp(pixel_type(),"float"))          datatype = 16;
      if (!cimg::strcasecmp(pixel_type(),"double"))         datatype = 64;
      if (datatype<0)
	throw CImgIOException("CImg<%s>::save_analyze() : Cannot save image '%s' since pixel type (%s)"
			      "is not handled in Analyze7.5 specifications.\n",
			      pixel_type(),filename,pixel_type());
      ((short*)(header+70))[0] = datatype;
      ((short*)(header+72))[0] = sizeof(T);
      ((float*)(header+112))[0] = 1;
      ((float*)(header+76))[0] = 0;
      if (voxsize) {
        ((float*)(header+76))[1] = voxsize[0];
        ((float*)(header+76))[2] = voxsize[1];
        ((float*)(header+76))[3] = voxsize[2];
      } else ((float*)(header+76))[1] = ((float*)(header+76))[2] = ((float*)(header+76))[3] = 1;
      file = cimg::fopen(hname,"wb");
      cimg::fwrite(header,348,file);
      cimg::fclose(file);
      file = cimg::fopen(iname,"wb");
      cimg::fwrite(data,size(),file);
      cimg::fclose(file);
      return *this;
    }

    //@}
    //---------------------------
    //
    //! \name Plugins
    //@{
    //---------------------------
#ifdef cimg_plugin
#include cimg_plugin
#endif
    //@}
  };

  /*
   #-----------------------------------------
   #
   #
   #
   # Definition of the CImgl<> structure
   #
   #
   #
   #------------------------------------------
   */

  //! Class representing list of images CImg<T>.
  template<typename T> struct CImgl {       

    //! Size of the list (number of elements inside)
    unsigned int size;
    
    //! Allocation size of the list
    unsigned int allocsize;

    //! Pointer to the first list element
    CImg<T> *data;

    //! Define a CImgl<T>::iterator
    typedef CImg<T>* iterator;

    //! Define a CImgl<T>::const_iterator
    typedef const CImg<T>* const_iterator;
    
    //------------------------------------------
    //
    //! \name Constructors - Destructor - Copy
    //@{
    //------------------------------------------

    //! Default constructor
    CImgl():
      size(0),allocsize(0),data(0) {}
    
    //! Destructor
    ~CImgl() {
      if (data) delete[] data;
    }
    
    //! In-place version of the default constructor and default destructor
    CImgl& assign() {
      if (data) delete[] data;
      size = allocsize = 0;
      data = 0;
      return *this;
    }

    //! Equivalent to assign() (STL-compliant name)
    CImgl& clear() { 
      return assign(); 
    }

    //! Copy constructor
    template<typename t> CImgl(const CImgl<t>& list):
      size(0),allocsize(0),data(0) {
      assign(list);
    }

    CImgl(const CImgl& list):
      size(0),allocsize(0),data(0) {
      assign(list);
    }

    //! Copy constructor that create a shared object
    template<typename t> CImgl(const CImgl<t>& list, const bool shared):
      size(0),allocsize(0),data(0) {
      assign(list,shared);
    }

    CImgl(const CImgl& list, const bool shared):
      size(0),allocsize(0),data(0) {
      assign(list,shared);
    }

    //! In-place version of the copy constructor   
    template<typename t> CImgl& assign(const CImgl<t>& list, const bool shared=false) {
      assign(list.size);
      cimgl_map(*this,l) (*this)[l].assign(list[l],shared);
      return *this;
    }

    //! Assignement operator
    template<typename t> CImgl& operator=(const CImgl<t>& list) { 
      return assign(list);
    }

    CImgl& operator=(const CImgl& list) { 
      return assign(list);
    }
        
    //! Construct an image list containing n empty images
    CImgl(const unsigned int n):
      size(n) {
      data = new CImg<T>[allocsize=cimg::nearest_pow2(n)];
    }
    
    //! In-place version of the previous constructor
    CImgl& assign(const unsigned int n) {
      if (n) {
	if (allocsize<n || allocsize>(n<<2)) {
	  if (data) delete[] data;
	  data = new CImg<T>[allocsize=cimg::nearest_pow2(n)];
	}
	size = n;
      } else return assign();
      return *this;
    }
        
    //! Construct an image list containing n images with specified size
    CImgl(const unsigned int n, const unsigned int width, const unsigned int height=1,
	  const unsigned int depth=1, const unsigned int dim=1):
      size(0),allocsize(0),data(0) {
      assign(n,width,height,depth,dim);
    }

    //! In-place version of the previous constructor
    CImgl& assign(const unsigned int n, const unsigned int width, const unsigned int height=1,
		  const unsigned int depth=1, const unsigned int dim=1) {
      const unsigned int siz = width*height*depth*dim;
      if (n && siz) { assign(n); cimgl_map(*this,l) data[l].assign(width,height,depth,dim); }
      else return assign();
      return *this;
    }

    //! Construct an image list containing n images with specified size, filled with val
    CImgl(const unsigned int n, const unsigned int width, const unsigned int height,
	  const unsigned int depth, const unsigned int dim, const T& val):
      size(0),allocsize(0),data(0) {
      assign(n,width,height,depth,dim,val);
    }

    //@}
    //------------------------------
    //
    //! \name Arithmetics Operators
    //@{
    //------------------------------

    //! Return a string describing the type of the image pixels in the list (template parameter \p T).
    static const char* pixel_type() { T val; return cimg::get_type(val); }
    
    //! Return \p true if list is empty
    bool is_empty() const { 
      return (!data || !size); 
    }

    //! Add each image of the current list with the corresponding image in the list \p list.
    template<typename t> CImgl& operator+=(const CImgl<t>& list) {
      const unsigned int sizemax = min(size,list.size);
      for (unsigned int l=0; l<sizemax; l++) (*this)[l]+=list[l];
      return *this;
    }

    //! Add each image of the current list with an image \p img.
    template<typename t> CImgl& operator+=(const CImg<t>& img) { 
      cimgl_map(*this,l) (*this)[l]+=img;
      return *this; 
    }

    //! Add each image of the current list with a value \p val.
    CImgl& operator+=(const T& val) { 
      cimgl_map(*this,l) (*this)[l]+=val;
      return *this; 
    }
    
    //! Subtract each image of the current list with the corresponding image in the list \p list.
    template<typename t> CImgl& operator-=(const CImgl<t>& list) {
      const unsigned int sizemax = min(size,list.size);
      for (unsigned int l=0; l<sizemax; l++) (*this)[l]-=list[l];
      return *this;
    }

    //! Substract each image of the current list with an image \p img.
    template<typename t> CImgl& operator-=(const CImg<t>& img) { 
      cimgl_map(*this,l) (*this)[l]-=img;
      return *this; 
    }
       
    //! Substract each image of the current list with a value \p val.
    CImgl& operator-=(const T& val) { 
      cimgl_map(*this,l) (*this)[l]-=val;
      return *this; 
    }

    //! Multiply each image of the current list by an image \p img.
    template<typename t> CImgl& operator*=(const CImg<t>& img) { 
      cimgl_map(*this,l) (*this)[l]*=img;
      return *this; 
    }

    //! Multiply each image of the current list by a value \p val.
    CImgl& operator*=(const double val) { 
      cimgl_map(*this,l) (*this)[l]*=val;
      return *this; 
    }

    //! Divide each image of the current list by a value \p val.
    CImgl& operator/=(const double val) { 
      cimgl_map(*this,l) (*this)[l]/=val;
      return *this; 
    }

    //! Return a new image list corresponding to the addition of each image of the current list with the corresponding image in the list \p list.
    template<typename t> CImgl<typename cimg::largest<t,T>::type> operator+(const CImgl<t>& list) const { 
      typedef typename cimg::largest<t,T>::type restype;
      return CImgl<restype>(*this)+=list; 
    }

    //! Return a new image list corresponding to the addition of each image of the current list with an image \p img.
    template<typename t> CImgl<typename cimg::largest<t,T>::type> operator+(const CImg<t>& img) const { 
      typedef typename cimg::largest<t,T>::type restype;
      return CImgl<restype>(*this)+=img;
    }

    //! Return a new image list corresponding to the addition of each image of the current list with a value \p val.
    CImgl operator+(const T& val) const { 
      return CImgl<T>(*this)+=val;
    }

    //! Return a new image list corresponding to the substraction of each image of the current list with the corresponding image in the list \p list.
    template<typename t> CImgl<typename cimg::largest<t,T>::type> operator-(const CImgl& list) const {
      typedef typename cimg::largest<t,T>::type restype;
      return CImgl<restype>(*this)-=list; 
    }

    //! Return a new image list corresponding to the substraction of each image of the current list with an image \p img.
    template<typename t> CImgl<typename cimg::largest<t,T>::type> operator-(const CImg<t>& img) const { 
      typedef typename cimg::largest<t,T>::type restype;
      return CImgl<restype>(*this)-=img;
    }

    //! Return a new image list corresponding to the substraction of each image of the current list with a value \p val.
    CImgl operator-(const T& val) const { 
      return CImgl<T>(*this)-=val;  
    }
      
    //! Return a new image list corresponding to the matrix multiplication of each image of the current list by the matrix \p img
    template<typename t> CImgl<typename cimg::largest<T,t>::type> operator*(const CImg<t>& img) const {
      typedef typename cimg::largest<T,t>::type restype;
      CImgl<restype> res(size);
      cimgl_map(res,l) res[l] = data[l]*img;
      return res;
    }
    
    //! Return a new image list corresponding to the multiplication of each image of the current list by a value \p val.
    CImgl operator*(const double val) const {
      return CImgl<T>(*this)*=val;  
    }
    
    //! Return a new image list corresponding to the division of each image of the current list by a value \p val.
    CImgl operator/(const double val) const { 
      return CImgl<T>(*this)/=val;  
    }
    
    //! Return a new image list corresponding to the addition of each image of the current list with a value \p val;
    template<typename t> friend CImgl<typename cimg::largest<T,t>::type> operator+(const T& val, const CImgl<t>& list) {
      typedef typename cimg::largest<T,t>::type restype;
      return CImgl<restype>(list)+=val; 
    }
    
    //! Return a new image list corresponding to the scalar multiplication of each image of the current list by a value \p val.
    friend CImgl operator*(const double val, const CImgl& list) {
      return CImgl<T>(list)*=val; 
    }
      
    //@}
    //-------------------------
    //
    //! \name List Manipulation
    //@{
    //-------------------------
    
    //! Return a reference to the i-th element of the image list.
    CImg<T>& operator[](const unsigned int pos) {
      return data[pos];
    }
    
    const CImg<T>& operator[](const unsigned int pos) const {
      return data[pos];
    }
    
    //! Equivalent to CImgl<T>::operator[]
    CImg<T>& operator()(const unsigned int pos) { return (*this)[pos]; }
    const CImg<T>& operator()(const unsigned int pos) const { return (*this)[pos]; }

    //! Return a reference to (x,y,z,v) pixel of the pos-th image of the list
    T& operator()(const unsigned int pos, const unsigned int x, const unsigned int y=0,
		  const unsigned int z=0, const unsigned int v=0) {
      return (*this)[pos](x,y,z,v);
    }
    const T& operator()(const unsigned int pos, const unsigned int x, const unsigned int y=0,
			const unsigned int z=0, const unsigned int v=0) const {
      return (*this)[pos](x,y,z,v);
    }

    //! Equivalent to CImgl<T>::operator[], with boundary checking
    CImg<T>& at(const unsigned int pos) {
      if (pos>=size)
	throw CImgArgumentException("CImgl<%s>::at() : bad list position %u, in a list of %u images",
				    pixel_type(),pos,size);
      return data[pos];
    }

    const CImg<T>& at(const unsigned int pos) const {
      if (pos>=size)
	throw CImgArgumentException("CImgl<%s>::at() : bad list position %u, in a list of %u images",
				    pixel_type(),pos,size);
      return data[pos];
    }

    //! Insert a copy of the image \p img into the current image list, at position \p pos.
    CImgl& insert(const CImg<T>& img, const unsigned int pos, const bool shared=false) {
      if (pos>size) 
	throw CImgArgumentException("CImgl<%s>::insert() : Can't insert at position %u into a list with %u elements",
				    pixel_type(),pos,size);
      CImg<T> *new_data = (++size>allocsize)?new CImg<T>[allocsize?(allocsize<<=1):(allocsize=1)]:0;
      if (!size || !data) { data = new_data; *data = img; }
      else {
	if (new_data) { // Insert with reallocation
	  if (pos) std::memcpy(new_data,data,sizeof(CImg<T>)*pos);
	  if (pos!=size-1) std::memcpy(new_data+pos+1,data+pos,sizeof(CImg<T>)*(size-1-pos));
	  std::memset(data,0,sizeof(CImg<T>)*(size-1));
	  delete[] data;
	  data = new_data;
	}
	else if (pos!=size-1) memmove(data+pos+1,data+pos,sizeof(CImg<T>)*(size-1-pos));
	if (shared && !img.is_empty()) {
	  data[pos].width = img.width; data[pos].height = img.height; data[pos].depth = img.depth; data[pos].dim = img.dim;
	  data[pos].is_shared = true; data[pos].data = img.data;
	} else {
	  data[pos].width = data[pos].height = data[pos].depth = data[pos].dim = 0; data[pos].data = 0;
	  data[pos] = img;
	}
      }
      return *this;
    }

    //! Insert a copy of the image \p img at the current image list.
    CImgl& insert(const CImg<T>& img) { 
      return insert(img,size); 
    }
    
    //! Insert n copies of the image \p img into the current image list, at position \p pos.
    CImgl& insert(const unsigned int n, const CImg<T>& img, const unsigned int pos) { 
      for (unsigned int i=0; i<n; i++) insert(img,pos);
      return *this;
    }

    //! Insert n copies of the image \p img at the end of the list.
    CImgl& insert(const unsigned int n, const CImg<T>& img) { 
      for (unsigned int i=0; i<n; i++) insert(img);
      return *this;
    }

    //! Insert a copy of the image list \p list into the current image list, starting from position \p pos.
    CImgl& insert(const CImgl<T>& list, const unsigned int pos) { 
      if (this!=&list) cimgl_map(list,l) insert(list[l],pos+l);
      else insert(CImgl<T>(list),pos);
      return *this; 
    }

    //! Append a copy of the image list \p list at the current image list.
    CImgl& insert(const CImgl<T>& list) {
      return insert(list,size); 
    }

    //! Insert n copies of the list \p list at position \p pos of the current list.
    CImgl& insert(const unsigned int n, const CImgl<T>& list, const unsigned int pos) {
      for (unsigned int i=0; i<n; i++) insert(list,pos);
      return *this;
    }

    //! Insert n copies of the list at the end of the current list
    CImgl& insert(const unsigned int n, const CImgl<T>& list) {
      for (unsigned int i=0; i<n; i++) insert(list);
      return *this;
    }

    //! Reverse list order
    CImgl& reverse() {
      for (unsigned int l=0; l<size/2; l++) (*this)[l].swap((*this)[size-1-l]);
      return *this;
    }
    
    //! Get reversed list
    CImgl& get_reverse() const { return CImgl<T>(*this).reverse(); }

    //! Insert image at the end of the list
    CImgl& operator<<(const CImg<T>& img) { 
      return insert(img);
    }
    
    //! Remove last image of the list
    CImgl& operator>>(CImg<T>& img) {
      if (size) { img = (*this)[size-1]; return remove(size-1); }
      img.assign();
      return *this;
    }

    //! Get a sub-list
    const CImgl get_crop(const unsigned int i0, const unsigned int i1, const bool shared=false) const {
      if (i0>i1 || i1>=size)
	throw CImgArgumentException("CImgl<%s>::get_crop() : Cannot get a sub-list (%u->%u) from a list of %u images",
				    pixel_type(),i0,i1,size);
      CImgl<T> res(i1-i0+1);
      cimgl_map(res,l) res[l].assign((*this)[i0+l],shared);
      return res;
    }
    
    //@}
    //----------------------------------
    //
    //! \name Input-Output and Display
    //@{
    //----------------------------------
    
    //! Load PAR-REC (Philips) image file
    static CImgl get_load_parrec(const char *filename) {
      char body[1024], filenamepar[1024], filenamerec[1024];
      const char *ext = cimg::filename_split(filename,body);
      if (!cimg::strncmp(ext,"par",3)) { std::strcpy(filenamepar,filename); snprintf(filenamerec,sizeof(filenamerec),"%s.rec",body); }
      if (!cimg::strncmp(ext,"PAR",3)) { std::strcpy(filenamepar,filename); snprintf(filenamerec,sizeof(filenamerec),"%s.REC",body); }
      if (!cimg::strncmp(ext,"rec",3)) { std::strcpy(filenamerec,filename); snprintf(filenamepar,sizeof(filenamepar),"%s.par",body); }
      if (!cimg::strncmp(ext,"REC",3)) { std::strcpy(filenamerec,filename); snprintf(filenamepar,sizeof(filenamepar),"%s.PAR",body); }
      std::FILE *file = cimg::fopen(filenamepar,"r");

      // Parse header file
      CImgl<float> st_slices;
      CImgl<unsigned int> st_global;
      int err;
      char line[256]={0};
      do { err=std::fscanf(file,"%255[^\n]%*c",line); } while (err!=EOF && (line[0]=='#' || line[0]=='.'));
      do { 
	unsigned int sn,sizex,sizey,pixsize;
	float rs,ri,ss;
	err=std::fscanf(file,"%u%*u%*u%*u%*u%*u%*u%u%*u%u%u%g%g%g%*[^\n]",&sn,&pixsize,&sizex,&sizey,&ri,&rs,&ss);
	if (err==7) {
	  st_slices.insert(CImg<float>::vector((float)sn,(float)pixsize,(float)sizex,(float)sizey,ri,rs,ss,0));	  
	  unsigned int i; for (i=0; i<st_global.size && sn<=st_global[i][2]; i++) ;
	  if (i==st_global.size) st_global.insert(CImg<unsigned int>::vector(sizex,sizey,sn));
	  else {
	    CImg<unsigned int> &vec = st_global[i];
	    if (sizex>vec[0]) vec[0] = sizex;
	    if (sizey>vec[1]) vec[1] = sizey;
	    vec[2] = sn;
	  }
	  st_slices[st_slices.size-1][7] = (float)i;
	}
      } while (err==7);
      
      // Read data
      std::FILE *file2 = cimg::fopen(filenamerec,"rb");
      CImgl<T> dest;
      {	cimgl_map(st_global,l) {
	const CImg<unsigned int>& vec = st_global[l];
	dest.insert(CImg<T>(vec[0],vec[1],vec[2]));
      }}
      
      cimgl_map(st_slices,l) {
	const CImg<float>& vec = st_slices[l];
	const unsigned int
	  sn = (unsigned int)vec[0]-1,
	  pixsize = (unsigned int)vec[1],
	  sizex = (unsigned int)vec[2],
	  sizey = (unsigned int)vec[3],
	  imn = (unsigned int)vec[7];
	const float ri = vec[4], rs = vec[5], ss = vec[6];
	switch (pixsize) {
	case 8: {
	  CImg<unsigned char> buf(sizex,sizey);
	  cimg::fread(buf.data,sizex*sizey,file2);
	  if (cimg::endian()) cimg::endian_swap(buf.data,sizex*sizey);
	  CImg<T>& img = dest[imn];
	  cimg_mapXY(img,x,y) img(x,y,sn) = (T)(( buf(x,y)*rs + ri )/(rs*ss));
	} break;
	case 16: {
	  CImg<unsigned short> buf(sizex,sizey);
	  cimg::fread(buf.data,sizex*sizey,file2);
	  if (cimg::endian()) cimg::endian_swap(buf.data,sizex*sizey);
	  CImg<T>& img = dest[imn];
	  cimg_mapXY(img,x,y) img(x,y,sn) = (T)(( buf(x,y)*rs + ri )/(rs*ss));
	} break;
	case 32: {
	  CImg<unsigned int> buf(sizex,sizey);
	  cimg::fread(buf.data,sizex*sizey,file2);
	  if (cimg::endian()) cimg::endian_swap(buf.data,sizex*sizey);
	  CImg<T>& img = dest[imn];
	  cimg_mapXY(img,x,y) img(x,y,sn) = (T)(( buf(x,y)*rs + ri )/(rs*ss));
	} break;
	default:
	  throw CImgIOException("CImg<%s>::get_load_parrec() : Cannot handle image with pixsize = %d bits\n",
				pixel_type(),pixsize);
	  
	}
      }
      
      cimg::fclose(file);
      cimg::fclose(file2);  
      if (!dest.size)
	throw CImgIOException("CImg<%s>::get_load_parrec() : filename '%s' does not appear to be a valid PAR-REC file",
			      pixel_type(),filename);
      return dest;
    }

    //! In-place version of get_load_parrec().
    CImgl& load_parrec(const char *filename) {
      return get_load_parrec(filename).swap(*this); 
    }

    //! Load YUV image sequence.
    static CImgl get_load_yuv(const char *filename,
			  const unsigned int sizex, const unsigned int sizey,
			  const unsigned int first_frame=0, const int last_frame=-1,
			  const bool yuv2rgb=true) {
      if (sizex%2 || sizey%2)
	throw CImgArgumentException("CImgl<%s>::get_load_yuv() : Image dimensions along X and Y must be even numbers (given are %ux%u)\n",
				    pixel_type(),sizex,sizey);
      if (!sizex || !sizey)
	throw CImgArgumentException("CImgl<%s>::get_load_yuv() : Given image sequence size (%u,%u) is invalid",
				    pixel_type(),sizex,sizey);      
      if (last_frame>0 && first_frame>(unsigned int)last_frame)
	throw CImgArgumentException("CImgl<%s>::get_load_yuv() : Given first frame %u is posterior to last frame %d.",
			    pixel_type(),first_frame,last_frame);
      if (!sizex || !sizey)
	throw CImgArgumentException("CImgl<%s>::get_load_yuv() : Given frame size (%u,%u) is invalid.",
				    pixel_type(),sizex,sizey);
      CImgl<T> res;
      CImg<unsigned char> tmp(sizex,sizey,1,3), UV(sizex/2,sizey/2,1,2);
      std::FILE *file = cimg::fopen(filename,"rb");
      bool stopflag = false;
      int err;
      if (first_frame) {
	err = std::fseek(file,first_frame*(sizex*sizey + sizex*sizey/2),SEEK_CUR);
	if (err) throw CImgIOException("CImgl<%s>::get_load_yuv() : File '%s' doesn't contain frame number %u "
				       "(out of range error).",pixel_type(),filename,first_frame);
      }
      unsigned int frame;
      for (frame = first_frame; !stopflag && (last_frame<0 || frame<=(unsigned int)last_frame); frame++) {
	tmp.fill(0);
	// TRY to read the luminance, don't replace by cimg::fread !
	err = (int)std::fread((void*)(tmp.ptr()),1,(size_t)(tmp.width*tmp.height),file); 
	if (err!=(int)(tmp.width*tmp.height)) {
	  stopflag = true;
	} else {
	  UV.fill(0);
	  // TRY to read the luminance, don't replace by cimg::fread !
	  err = (int)std::fread((void*)(UV.ptr()),1,(size_t)(UV.size()),file);
	  if (err!=(int)(UV.size())) {
	    stopflag = true;
	  } else {
	    cimg_mapXY(UV,x,y) {
	      const int x2=2*x, y2=2*y;
	      tmp(x2,y2,1) = tmp(x2+1,y2,1) = tmp(x2,y2+1,1) = tmp(x2+1,y2+1,1) = UV(x,y,0);
	      tmp(x2,y2,2) = tmp(x2+1,y2,2) = tmp(x2,y2+1,2) = tmp(x2+1,y2+1,2) = UV(x,y,1);
	    }
	    if (yuv2rgb) tmp.YCbCrtoRGB();
	    res.insert(tmp);
	  }
	}
      }
      return res;
    }

    CImgl& load_yuv(const char *filename,
		    const unsigned int sizex, const unsigned int sizey,
		    const unsigned int first_frame=0, const int last_frame=-1,
		    const bool yuv2rgb=true) {
      return get_load_yuv(filename,sizex,sizey,first_frame,last_frame,yuv2rgb).swap(*this);
    }

    //! Return a single image which is the concatenation of all images of the current CImgl instance.
    /**
       \param axe : specify the axe for image concatenation. Can be 'x','y','z' or 'v'.
       \param align : specify the alignment for image concatenation. Can be 'p' (top), 'c' (center) or 'n' (bottom).
       \return A CImg<T> image corresponding to the concatenation is returned.
    **/
    CImg<T> get_append(const char axe='x',const char align='c') const {
      if (is_empty()) return CImg<T>();
      unsigned int dx=0,dy=0,dz=0,dv=0,pos=0;
      CImg<T> res;
      switch(cimg::uncase(axe)) {
      case 'x': {
	cimgl_map(*this,l) {
	  const CImg<T>& img = (*this)[l];
	  dx += img.width;
	  dy = cimg::max(dy,img.height);
	  dz = cimg::max(dz,img.depth);
	  dv = cimg::max(dv,img.dim);
	}
	res.assign(dx,dy,dz,dv,0);
	switch (cimg::uncase(align)) {
	case 'p' : { cimgl_map(*this,ll) { res.draw_image((*this)[ll],pos,0,0,0); pos+=(*this)[ll].width; }} break;
	case 'n' : { cimgl_map(*this,ll) { 
	      res.draw_image((*this)[ll],pos,dy-(*this)[ll].height,dz-(*this)[ll].depth,dv-(*this)[ll].dim); pos+=(*this)[ll].width;
	    }} break;
	default  : { cimgl_map(*this,ll) {
	      res.draw_image((*this)[ll],pos,(dy-(*this)[ll].height)/2,(dz-(*this)[ll].depth)/2,(dv-(*this)[ll].dim)/2);
	      pos+=(*this)[ll].width; 
	    }} break;
	}
      }	break;
      case 'y': {
	cimgl_map(*this,l) {
	  const CImg<T>& img = (*this)[l];
	  dx = cimg::max(dx,img.width);
	  dy += img.height;
	  dz = cimg::max(dz,img.depth);
	  dv = cimg::max(dv,img.dim);
	}
	res.assign(dx,dy,dz,dv,0);
	switch (cimg::uncase(align)) {
	case 'p': { cimgl_map(*this,ll) { res.draw_image((*this)[ll],0,pos,0,0); pos+=(*this)[ll].height; }} break;
	case 'n': { cimgl_map(*this,ll) { 
	      res.draw_image((*this)[ll],dx-(*this)[ll].width,pos,dz-(*this)[ll].depth,dv-(*this)[ll].dim); pos+=(*this)[ll].height;
	    }} break;
	default : { cimgl_map(*this,ll) { 
	      res.draw_image((*this)[ll],(dx-(*this)[ll].width)/2,pos,(dz-(*this)[ll].depth)/2,(dv-(*this)[ll].dim)/2);
	      pos+=(*this)[ll].height; 
	    }} break;
	}
      }	break;
      case 'z': {
	cimgl_map(*this,l) {
	  const CImg<T>& img = (*this)[l];
	  dx = cimg::max(dx,img.width);
	  dy = cimg::max(dy,img.height);
	  dz += img.depth;
	  dv = cimg::max(dv,img.dim);
	}
	res.assign(dx,dy,dz,dv,0);
	switch (cimg::uncase(align)) {
	case 'p': { cimgl_map(*this,ll) { res.draw_image((*this)[ll],0,0,pos,0); pos+=(*this)[ll].depth; }} break;
	case 'n': { cimgl_map(*this,ll) { 
	      res.draw_image((*this)[ll],dx-(*this)[ll].width,dy-(*this)[ll].height,pos,dv-(*this)[ll].dim); pos+=(*this)[ll].depth;
	    }} break;
	case 'c': { cimgl_map(*this,ll) { 
	      res.draw_image((*this)[ll],(dx-(*this)[ll].width)/2,(dy-(*this)[ll].height)/2,pos,(dv-(*this)[ll].dim)/2);
	      pos+=(*this)[ll].depth; 
	    }} break;
	}
      }	break;
      case 'v': {
	cimgl_map(*this,l) {
	  const CImg<T>& img = (*this)[l];
	  dx = cimg::max(dx,img.width);
	  dy = cimg::max(dy,img.height);
	  dz = cimg::max(dz,img.depth);
	  dv += img.dim;
	}
	res.assign(dx,dy,dz,dv,0);
	switch (cimg::uncase(align)) {
	case 'p': { cimgl_map(*this,ll) { res.draw_image((*this)[ll],0,0,0,pos); pos+=(*this)[ll].dim; }} break;
	case 'n': { cimgl_map(*this,ll) { 
	      res.draw_image((*this)[ll],dx-(*this)[ll].width,dy-(*this)[ll].height,dz-(*this)[ll].depth,pos); pos+=(*this)[ll].dim;
	    }} break;
	case 'c': { cimgl_map(*this,ll) { 
	      res.draw_image((*this)[ll],(dx-(*this)[ll].width)/2,(dy-(*this)[ll].height)/2,(dz-(*this)[ll].depth)/2,pos);
	      pos+=(*this)[ll].dim; 
	    }} break;
	}
      } break;
      default: throw CImgArgumentException("CImg<%s>::get_append() : unknow axe '%c', must be 'x','y','z' or 'v'",pixel_type(),axe);
      }
      return res;
    }

    // Create an auto-cropped font (along the X axis) from a input font \p font.
    CImgl<T> get_crop_font() const {
      CImgl<T> res;
      cimgl_map(*this,l) {
	const CImg<T>& letter = (*this)[l];
        int xmin = letter.width, xmax = 0;
        cimg_mapXY(letter,x,y) if (letter(x,y)) { if (x<xmin) xmin=x; if (x>xmax) xmax=x; }
        if (xmin>xmax) res.insert(CImg<T>(letter.width,letter.height,1,letter.dim,0));
	else res.insert(letter.get_crop(xmin,0,xmax,letter.height-1));
      }
      res[' '].resize(res['f'].width);
      res[' '+256].resize(res['f'].width);
      return res;
    }
    
      CImgl<T>& crop_font() {
      return get_crop_font().swap(*this);
    }
    
    static CImgl<T> get_font(const unsigned int *const font,const unsigned int w,const unsigned int h,
			     const unsigned int paddingx, const unsigned int paddingy, const bool variable_size=true) {
      CImgl<T> res = CImgl<T>(256,w,h,1,3).insert(CImgl<T>(256,w,h,1,1));
      const unsigned int *ptr = font;
      unsigned int m = 0, val = 0;
      for (unsigned int y=0; y<h; y++)
	for (unsigned int x=0; x<256*w; x++) {
	  m>>=1; if (!m) { m=0x80000000; val = *(ptr++); }
	  CImg<T>& img = res[x/w], &mask = res[x/w+256];
	  unsigned int xm = x%w;
	  img(xm,y,0) = img(xm,y,1) = img(xm,y,2) = mask(xm,y,0) = (T)((val&m)?1:0);
	}
      if (variable_size) {
	  res.crop_font();
      }
      if (paddingx || paddingy) cimgl_map(res,l) res[l].resize(res[l].dimx()+paddingx, res[l].dimy()+paddingy,1,-100,0);
      return res;
    }

    //! Return a CImg pre-defined font with desired size
    /**
       \param font_height = height of the desired font (can be 11,13,24,38 or 57)
       \param fixed_size = tell if the font has a fixed or variable width.
    **/
    static CImgl<T> get_font(const unsigned int font_width, const bool variable_size=true) {
      if (font_width <= 11) {
	static CImgl<T> font7x11, nfont7x11;
	if (!variable_size && font7x11.is_empty()) {
	    font7x11 = get_font(cimg::font7x11,7,11,1,0,false);
	}
	if (variable_size && nfont7x11.is_empty()) {
	    nfont7x11 = get_font(cimg::font7x11,7,11,1,0,true);
	}
	return variable_size ? nfont7x11 : font7x11;
      } else {
	static CImgl<T> font10x13, nfont10x13;
	if (!variable_size && font10x13.is_empty()) {
	    font10x13 = get_font(cimg::font10x13,10,13,1,0,false);
	}
	if (variable_size && nfont10x13.is_empty()) {
	    nfont10x13 = get_font(cimg::font10x13,10,13,1,0,true);
	}
	return variable_size ? nfont10x13 : font10x13;
      } 
    }
    
    const CImgl& display(const char axe='x',const char align='c',
			 const int min_size=128,const int max_size=1024) const {
      return display(" ",axe,align,min_size,max_size); 
    }

    // Swap fields of two CImgl instances.
    CImgl& swap(CImgl& list) {
      cimg::swap(size,list.size);
      cimg::swap(allocsize,list.allocsize);
      cimg::swap(data,list.data);
      return list;
    }
  };

  // End of cimg_library:: namespace
}

// Overcome VisualC++ 6.0 and DMC compilers namespace bug
#if ( defined(_MSC_VER) || defined(__DMC__) ) && defined(std)
#undef std
#endif

#endif

// Local Variables:
// mode: c++
// End:
