/* -*- c-basic-offset: 3 -*-
 *
 * Copyright (c), GREYC.
 * All rights reserved
 *
 * You may use this file under the terms of the BSD license as follows:
 *
 * 6 Boulevard Maréchal Juin
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

/*
 * @author Régis Clouard - May 29, 2013
 */
#include<cmath>
#include <pandore.h>
using namespace pandore;

Errc PGetWindowAroundPoints( const Img2duc &ims1, const Img2duc &ims2, Img2duc &imd, int h, int w ) {
   if ( w>ims1.Width() || h>ims1.Height() || w<1 || h<1 ) {
      std::cerr << "Error pgetwindowaroundpoints: Bad parameters." << std::endl;
      return FAILURE;
   }
   if ( ims1.Size() != ims2.Size()) {
      std::cerr << "Error pgetwindowaroundpoints: incompatible image size." << std::endl;
      return FAILURE;
   }

   const int yoffset = h/2;
   const int yend = h-yoffset;
   const int xoffset = w/2;
   const int xend = w-xoffset;

   imd=0;
   for (int y=0; y<imd.Height(); y++) {
      for (int x=0; x<imd.Width(); x++) {
	 if (ims2(y,x)>0) {
	    for (int v=-yoffset; v<yend; v++) {
	       for (int u=-xoffset; u<xend; u++) {
		  if (ims1.Hold(y+v,x+u)) {
		     imd(y+v,x+u) = ims1(y+v,x+u);
		  }
	       }
	    }
	 }
      }
   }

   return SUCCESS;
}

Errc PGetWindowAroundPoints( const Img3duc &ims1, const Img3duc &ims2, Img3duc &imd, int d, int h, int w ) {
   if (w>ims1.Width() || h>ims1.Height() || d>ims1.Depth()
       || w<1 || h<1 || d<1 ) {
      std::cerr << "Error pgetwindowaroundpoints: Bad parameters." << std::endl;
      return FAILURE;
   }
   if ( ims1.Size() != ims2.Size()) {
      std::cerr << "Error pgetwindowaroundpoints: incompatible image size." << std::endl;
      return FAILURE;
   }

   const int zoffset = d/2;
   const int zend = d-zoffset;
   const int yoffset = h/2;
   const int yend = h-yoffset;
   const int xoffset = w/2;
   const int xend = w-xoffset;

   imd=0;
   for (int z=0; z<imd.Depth(); z++) {
      for (int y=0; y<imd.Height(); y++) {
	 for (int x=0; x<imd.Width(); x++) {
	    if (ims2(z,y,x)>0) {
	       for (int w=-zoffset; w<zend; w++) {
		  for (int v=-yoffset; v<yend; v++) {
		     for (int u=-xoffset; u<xend; u++) {
			if (ims1.Hold(z+w,y+v,x+u)) {
			   imd(z+w,y+v,x+u) = ims1(z+w,y+v,x+u);
			}
		     }
		  }
	       }
	    }
	 }
      }
   }

   return SUCCESS;
}

#ifdef MAIN
	
/*
 * Modify only the following constants, and the function call.
 */
#define	USAGE	"usage: %s width height depth [-m mask] [im_in1|-] [im_in2|-] [im_out|-]"
#define	PARC	3
#define	FINC	2
#define	FOUTC	1
#define	MASK	0

int main( int argc, char *argv[] ) {
   Errc result;              // The result code of the execution.
   Pobject* mask;            // The region map.
   Pobject* objin[FINC+1];   // The input objects.
   Pobject* objs[FINC+1];    // The source objects masked.
   Pobject* objout[FOUTC+1]; // The output object.
   Pobject* objd[FOUTC+1];   // The result object of the execution.
   char* parv[PARC+1];       // The input parameters.

   ReadArgs(argc,argv,PARC,FINC,FOUTC,&mask,objin,objs,objout,objd,parv,USAGE,MASK);
   if (objs[0]->Type()==Po_Img2duc || objs[1]->Type()==Po_Img2duc) {
      Img2duc* const ims1=(Img2duc*)objs[0];
      Img2duc* const ims2=(Img2duc*)objs[1];
      objd[0]=new Img2duc(ims1->Props());
      Img2duc* const imd=(Img2duc*)objd[0];
      
      result=PGetWindowAroundPoints(*ims1, *ims2, *imd, atoi(parv[1]), atoi(parv[0]));
      goto end;
   }

   if (objs[0]->Type()==Po_Img3duc || objs[1]->Type()==Po_Img3duc) {
      Img3duc* const ims1=(Img3duc*)objs[0];
      Img3duc* const ims2=(Img3duc*)objs[1];
      objd[0]=new Img3duc(ims1->Props());
      Img3duc* const imd=(Img3duc*)objd[0];
      
      result=PGetWindowAroundPoints(*ims1, *ims2, *imd, atoi(parv[2]), atoi(parv[1]), atoi(parv[0]));
      goto end;
   }
end:
  if (result)
	WriteArgs(argc,argv,PARC,FINC,FOUTC,&mask,objin,objs,objout,objd,MASK);
  Exit(result);
  return 0;
}
#endif
