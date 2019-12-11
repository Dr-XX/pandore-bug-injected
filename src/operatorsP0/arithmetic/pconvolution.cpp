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

#include <pandore.h>
using namespace pandore;

/**
 * @file pconvolution.cpp
 * Convolution of an image by a kernel size LxK:
 * output(x, y) = sum (sum ( kernel[l, k]*input[y+l, x+k]) / norm;
 * where norm  sum (kernel[l, k]) / N ; N=L*K.
 * PROBLEM: The border are not treared: values are filled with 0.
 */

/**
 * Generates the kernel from a text file.
 * @param filename the fie that contains the kernel.
 */
Pobject *readKernel( char *filename ) {
   int dim[3];
   FILE* fp;
   char car;
   Pobject* pout = NULL;

   if (! (fp = fopen(filename, "rt"))) {
      return NULL;
   }
   int i = 0;
   do {
      if (fscanf(fp, "%d", &dim[i++]) < 1) {
	 return NULL;
      } else {
	 car = fgetc(fp);
      }
   } while (car == '*');

   if (i == 2) {
      if (dim[0] < 1 || dim[1] < 1) {
	 return NULL;
      }
      int imageHeight = dim[0];
      int imageWidth = dim[1];
      // case of even sizes.
      if (imageHeight%2 == 0) {
	 imageHeight += 1;
      }
      if (imageWidth%2 == 0) {
	 imageWidth += 1;
      }

      Img2dsf* imm = new Img2dsf(imageHeight, imageWidth);
      (*imm) = 0; ;
      // Lecture de valeurs.
      for (int l = 0; l < dim[0]; l++) {
	 for (int k = 0; k < dim[1]; k++) {
	    float val; 
	    if (fscanf(fp, "%f", &val) < 1) {
	       return NULL;
	    }
	    (*imm)(l, k) = val;
	 }
      }
      pout = imm;
   } else if (i == 3) {   
      if (dim[0] < 1 || dim[1] < 1 || dim[2] < 1) {
	 return NULL;
      }

      int imageDepth = dim[0];
      int imageHeight = dim[1];
      int imageWidth = dim[2];
      // case of even sizes.
      if (imageDepth%2 == 0) {
	 imageDepth += 1;
      }
      if (imageHeight%2 == 0) {
	 imageHeight += 1;
      }
      if (imageWidth%2 == 0) {
	 imageWidth += 1;
      }

      Img3dsf* imm = new Img3dsf(imageDepth, imageHeight, imageWidth);
      
      // Lecture de valeurs.
      for (int i = 0; i < dim[0]; i++) {
	 for (int l = 0; l < dim[1]; l++) {
	    for (int k = 0; k < dim[2]; k++) {
	       float val; 
	       if (fscanf(fp, "%f", &val) < 1 ) {
		  return NULL;
	       }
	       (*imm)(i, l, k) = val;
	    } 
	 }
      }
      pout = imm;
   }
   fclose(fp);
   return pout;
}

template <typename T1, typename T2>
inline void convolution2d_slow( const Imx2d<T1>& ims, Imx2d<T2> &imd, Img2dsf& imm, Point2d& p, Point2d& offset, float norm ) {
   for (int b = 0; b < ims.Bands(); b++) {
      float val = 0.0f;
      for (int l = -offset.y; l <= offset.y; l++) {
	 for (int k = -offset.x; k <= offset.x; k++) {
	    if (ims.Hold(p.y - l, p.x - k)) {
	       val += ims(b, p.y - l, p.x - k) * imm(l + offset.y, k + offset.x);
	    } else {
	       val += ims(b, p.y, p.x) * imm(l + offset.y, k + offset.x);
	    }
	 }
      }
      imd[b][p] = val / norm;
   }
}

template <typename T1, typename T2>
inline void convolution2d_fast( const Imx2d<T1>& ims, Imx2d<T2> &imd, Img2dsf& imm, Point2d& p, Point2d& offset, float norm ) {
   for (int b = 0; b < ims.Bands(); b++) {
      float val = 0.0f;
      for (int l = -offset.y; l <= offset.y; l++) {
	 for (int k = -offset.x; k <= offset.x; k++) {
	    val += ims(b, p.y - l, p.x - k) * imm(l + offset.y, k + offset.x);
	 }
      }
      imd[b][p] = val / norm;
   }
}

template <typename T1, typename T2>
Errc PConvolution( const Imx2d<T1> &ims, Imx2d<T2> &imd, char *kernel_file ) {
   Img2dsf *imm = (Img2dsf*)readKernel(kernel_file);

   if (!imm) {
      std::cerr << "Error pconvolution: Bad kernel file: " << kernel_file << std::endl;
      return FAILURE;
   }

   Point2d offset(imm->Height() / 2, imm->Width() / 2);
   
   float norm = 0.0f;
   for (int l = 0; l < imm->Height(); l++) {
      for (int k = 0; k < imm->Width(); k++) {
	 norm += (*imm)(l, k);
      }
   }
   if (norm == 0) {
      norm = 1.0F;
   }

   Point2d p;
   for (p.y = 0; p.y < offset.y; p.y++) {
      for (p.x = 0; p.x < ims.Width(); p.x++) {
	 convolution2d_slow(ims, imd, *imm, p, offset, norm);
      }
   }
   for (p.y = ims.Height() - offset.y; p.y < ims.Height(); p.y++) {
      for (p.x = 0; p.x < ims.Width(); p.x++) {
	 convolution2d_slow(ims, imd, *imm, p, offset, norm);
      }
   }
   for (p.y = offset.y; p.y < ims.Height() - offset.y; p.y++) {
      for (p.x = 0; p.x < offset.x; p.x++) {
	 convolution2d_slow(ims, imd, *imm, p, offset, norm);
      }
   }
   for (p.y = offset.y; p.y < ims.Height() - offset.y; p.y++) {
      for (p.x = ims.Width() - offset.x; p.x < ims.Width(); p.x++) {
	 convolution2d_slow(ims, imd, *imm, p, offset, norm);
      }
   }

   // general case 
   for (p.y = offset.y; p.y < ims.Height() - offset.y; p.y++) {
      for (p.x = offset.x; p.x < ims.Width() - offset.x; p.x++) {
	 convolution2d_fast(ims, imd, *imm, p, offset, norm);
      }
   }
   
   delete imm;
   
   return SUCCESS;
}


template <typename T1, typename T2>
inline void convolution3d_slow( const Imx3d<T1>& ims, Imx3d<T2> &imd, Img3dsf& imm, Point3d& p, Point3d& offset, float norm ) {
   for (int b=0; b<ims.Bands(); b++) {
      float val=0.0f;
      for (int i=-offset.z; i<=offset.z; i++) {
	 for (int l=-offset.y; l<=offset.y; l++) {
	    for (int k=-offset.x; k<=offset.x; k++) {
	       if (ims.Hold(p.z-i, p.y-l, p.x-k)) {
		  val += ims[b][p.z-i][p.y-l][p.x-k] * imm[i+offset.z][l+offset.y][k+offset.x];
	       } else {
		  val += ims[b][p.z][p.y][p.x] * imm[i+offset.z][l+offset.y][k+offset.x];
	       }
	    }
	 }
      }
      imd[b][p]=val/norm;
   }
}


template <typename T1, typename T2>
inline void convolution3d_fast( const Imx3d<T1>& ims, Imx3d<T2> &imd, Img3dsf& imm, Point3d& p, Point3d& offset, float norm ) {
   for (int b=0; b<ims.Bands(); b++) {
      float val=0.0f;
      for (int i=-offset.z; i<=offset.z; i++) {
	 for (int l=-offset.y; l<=offset.y; l++) {
	    for (int k=-offset.x; k<=offset.x; k++) {
	       val += ims[b][p.z-i][p.y-l][p.x-k] * imm[i+offset.z][l+offset.y][k+offset.x];
	    }
	 }
      }
      imd[b][p]=val/norm;
   }
}

template <typename T1, typename T2>
Errc PConvolution( const Imx3d<T1> &ims, Imx3d<T2> &imd, char *kernel_file ) {
   Img3dsf *imm = (Img3dsf*)readKernel(kernel_file);

   if (!imm) {
      std::cerr << "Error pconvolution: Bad kernel file: " << kernel_file << std::endl;
      return FAILURE;
   }

   Point3d offset(imm->Depth() / 2, imm->Height() / 2, imm->Width() / 2);
   
   float norm = 0.0f;
   for (int i = 0; i < imm->Depth(); i++) {
      for (int l = 0; l < imm->Height(); l++) {
	 for (int k = 0; k < imm->Width(); k++) {
	    norm += (*imm)(i, l, k);
	 }
      }
   }
   if (norm == 0) {
      norm = 1.0F;
   }
   
   // general case    
   Point3d p;
   for (p.z = offset.z; p.z < ims.Depth() - offset.z; p.z++) {
      
      for (p.y = 0; p.y < offset.y; p.y++) {
	 for (p.x = 0; p.x < ims.Width(); p.x++) {
	    convolution3d_slow(ims, imd, *imm, p, offset, norm);
	 }
      }
      for (p.y = ims.Height() - offset.y; p.y < ims.Height(); p.y++) {
	 for (p.x = 0; p.x < ims.Width(); p.x++) {
	    convolution3d_slow(ims, imd, *imm, p, offset, norm);
	 }
      }
      for (p.y = offset.y; p.y < ims.Height() - offset.y; p.y++) {
	 for (p.x = 0; p.x < offset.x; p.x++) {
	    convolution3d_slow(ims, imd, *imm, p, offset, norm);
	 }
      }
      for (p.y = offset.y; p.y < ims.Height() - offset.y; p.y++) {
	 for (p.x = ims.Width() - offset.x; p.x < ims.Width(); p.x++) {
	    convolution3d_slow(ims, imd, *imm, p, offset, norm);
	 }
      }
    }

   for (p.z = 0; p.z < offset.z; p.z++) {
      for (p.y = 0; p.y < ims.Height(); p.y++) {
	 for (p.x = 0; p.x < ims.Width(); p.x++) {
	    convolution3d_slow(ims, imd, *imm, p, offset, norm);
	 }
      }
   }
   
   for (p.z = ims.Depth() - offset.z; p.z < ims.Depth(); p.z++) {
      for (p.y = 0; p.y < ims.Height(); p.y++) {
	 for (p.x = 0; p.x < ims.Width(); p.x++) {
	    convolution3d_slow(ims, imd, *imm, p, offset, norm);
	 }
      }
   }
   
   for (p.z = offset.z; p.z < ims.Depth() - offset.z; p.z++) {
      for (p.y = offset.y; p.y < ims.Height() - offset.y; p.y++) {
	 for (p.x = offset.x; p.x < ims.Width() - offset.x; p.x++) {
	    convolution3d_fast(ims, imd, *imm, p, offset, norm);
	 }
      }
   }

   delete imm;
   
   return SUCCESS;
}




#ifdef MAIN
#define	USAGE	"usage: %s kernel-file [-m mask] [im_in|-] [im_out|-]"
#define	PARC	1
#define	FINC	1
#define	FOUTC	1
#define	MASK	1

int main( int argc, char *argv[] ) {
   Errc result;                // The result code of the execution.
   Pobject* mask;              // The region map.
   Pobject* objin[FINC + 1];   // The input objects.
   Pobject* objs[FINC + 1];    // The source objects masked.
   Pobject* objout[FOUTC + 1]; // The output object.
   Pobject* objd[FOUTC + 1];   // The result object of the execution.
   char* parv[PARC + 1];       // The input parameters.

   ReadArgs(argc, argv, PARC, FINC, FOUTC, &mask, objin, objs, objout, objd, parv, USAGE, MASK); 
   if (objs[0]->Type() == Po_Img2duc) {
      Img2duc* const ims = (Img2duc*)objs[0];
      objd[0] = new Img2dsf(ims->Props());
      Img2dsf* imd = (Img2dsf*)objd[0];
      
      result = PConvolution(*ims, *imd, parv[0]);
      goto end;
   }
   if (objs[0]->Type() == Po_Img2dsl) {
      Img2dsl* const ims = (Img2dsl*)objs[0];
      objd[0] = new Img2dsf(ims->Props());
      Img2dsf* imd = (Img2dsf*)objd[0];
      
      result = PConvolution(*ims, *imd, parv[0]);
      goto end;
   }
   if (objs[0]->Type() == Po_Img2dsf) {
      Img2dsf* const ims = (Img2dsf*)objs[0];
      objd[0] = new Img2dsf(ims->Props());
      Img2dsf* imd = (Img2dsf*)objd[0];
      
      result = PConvolution(*ims, *imd, parv[0]);
      goto end;
   }
   if (objs[0]->Type() == Po_Imc2duc) {
      Imc2duc* const ims = (Imc2duc*)objs[0];
      objd[0] = new Imc2dsf(ims->Props());
      Imc2dsf* imd = (Imc2dsf*)objd[0];
      
      result = PConvolution(*ims, *imd, parv[0]);
      goto end;
   }
   if (objs[0]->Type() == Po_Imc2dsl) {
      Imc2dsl* const ims = (Imc2dsl*)objs[0];
      objd[0] = new Imc2dsf(ims->Props());
      Imc2dsf* imd = (Imc2dsf*)objd[0];
      
      result = PConvolution(*ims, *imd, parv[0]);
      goto end;
   }
   if (objs[0]->Type() == Po_Imc2dsf) {
      Imc2dsf* const ims = (Imc2dsf*)objs[0];
      objd[0] = new Imc2dsf(ims->Props());
      Imc2dsf* imd = (Imc2dsf*)objd[0];
      
      result = PConvolution(*ims, *imd, parv[0]);
      goto end;
   }
   if (objs[0]->Type() == Po_Imx2duc) {
      Imx2duc* const ims = (Imx2duc*)objs[0];
      objd[0] = new Imx2dsf(ims->Props());
      Imx2dsf* imd = (Imx2dsf*)objd[0];
      
      result = PConvolution(*ims, *imd, parv[0]);
      goto end;
   }
   if (objs[0]->Type() == Po_Imx2dsl) {
      Imx2dsl* const ims = (Imx2dsl*)objs[0];
      objd[0] = new Imx2dsf(ims->Props());
      Imx2dsf* imd = (Imx2dsf*)objd[0];
      
      result = PConvolution(*ims, *imd, parv[0]);
      goto end;
   }
   if (objs[0]->Type() == Po_Imx2dsf) {
      Imx2dsf* const ims = (Imx2dsf*)objs[0];
      objd[0] = new Imx2dsf(ims->Props());
      Imx2dsf* imd = (Imx2dsf*)objd[0];
      
      result = PConvolution(*ims, *imd, parv[0]);
      goto end;
   }
   if (objs[0]->Type() == Po_Img3duc) {
      Img3duc* const ims = (Img3duc*)objs[0];
      objd[0] = new Img3dsf(ims->Props());
      Img3dsf* imd = (Img3dsf*)objd[0];
      
      result = PConvolution(*ims, *imd, parv[0]);
      goto end;
   }
   if (objs[0]->Type() == Po_Img3dsl) {
      Img3dsl* const ims = (Img3dsl*)objs[0];
      objd[0] = new Img3dsf(ims->Props());
      Img3dsf* imd = (Img3dsf*)objd[0];
      
      result = PConvolution(*ims, *imd, parv[0]);
      goto end;
   }
   if (objs[0]->Type() == Po_Img3dsf) {
      Img3dsf* const ims = (Img3dsf*)objs[0];
      objd[0] = new Img3dsf(ims->Props());
      Img3dsf* imd = (Img3dsf*)objd[0];
      
      result = PConvolution(*ims, *imd, parv[0]);
      goto end;
   }
   if (objs[0]->Type() == Po_Imc3duc) {
      Imc3duc* const ims = (Imc3duc*)objs[0];
      objd[0] = new Imc3dsf(ims->Props());
      Imc3dsf* imd = (Imc3dsf*)objd[0];
      
      result = PConvolution(*ims, *imd, parv[0]);
      goto end;
   }
   if (objs[0]->Type() == Po_Imc3dsl) {
      Imc3dsl* const ims = (Imc3dsl*)objs[0];
      objd[0] = new Imc3dsf(ims->Props());
      Imc3dsf* imd = (Imc3dsf*)objd[0];
      
      result = PConvolution(*ims, *imd, parv[0]);
      goto end;
   }
   if (objs[0]->Type() == Po_Imc3dsf) {
      Imc3dsf* const ims = (Imc3dsf*)objs[0];
      objd[0] = new Imc3dsf(ims->Props());
      Imc3dsf* imd = (Imc3dsf*)objd[0];
      
      result = PConvolution(*ims, *imd, parv[0]);
      goto end;
   }
   if (objs[0]->Type() == Po_Imx3duc) {
      Imx3duc* const ims = (Imx3duc*)objs[0];
      objd[0] = new Imx3dsf(ims->Props());
      Imx3dsf* imd = (Imx3dsf*)objd[0];
      
      result = PConvolution(*ims, *imd, parv[0]);
      goto end;
   }
   if (objs[0]->Type() == Po_Imx3dsl) {
      Imx3dsl* const ims = (Imx3dsl*)objs[0];
      objd[0] = new Imx3dsf(ims->Props());
      Imx3dsf* imd = (Imx3dsf*)objd[0];
      
      result = PConvolution(*ims, *imd, parv[0]);
      goto end;
   }
   if (objs[0]->Type() == Po_Imx3dsf) {
      Imx3dsf* const ims = (Imx3dsf*)objs[0];
      objd[0] = new Imx3dsf(ims->Props());
      Imx3dsf* imd = (Imx3dsf*)objd[0];
      
      result = PConvolution(*ims, *imd, parv[0]);
      goto end;
   }
  {
     PrintErrorFormat(objin, FINC, argv); 
     result = FAILURE; 
  }	

end:
  if (result) {
	WriteArgs(argc, argv, PARC, FINC, FOUTC, &mask, objin, objs, objout, objd, MASK); 
  }
  Exit(result); 
  return 0; 
}
#endif
