/* -*- mode: c++; c-basic-offset: 4 -*-
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
 * @author Matthieu Toutain - 2012-09-21
 * @todo Add a user-defined weight function and neighborhood
 */

/**
 * @file pNLMED.cpp
 * @brief performs a p non local median filter on an image $f$.
 *
 * This algorithm is iterative, it performs :
 * $f(u)^{t+1} = Med_{v \sim u}{v \tilde u}(\sqrt{w(u, v)}(f^t(v) - f^t(u))) + f^t(u)$
 * with $u$ a pixel location, $v$ a neighbor of $u$, w(u, v) the weight between $u$ and $v$
 * (which can be a similarity measure, a distance, etc.). With $w(u, v) = 1$ you get the median filter.
 */

#include <stdio.h>
#include <math.h>
#include <cstring>
#include "perform_reg.h"

#include <pandore.h>
using namespace pandore;

/**
 * Compute weights between a pixel value and its neighbors
 * (here with an exponential similarity measure : $w(u, v) = \exp^{-||f(v) - f(u)||^2 / \sigma^2}
 * @param imgIn	the original image on which we want to compute the weights.
 * @param imgWeight An image which contain the weights values, its dimensions
 *            are : $ imgIn.Width*imgIn.Height \times nbNeighbours $.
 * @param sigma	sigma for the similarity measure.
 * @param ngbIdx The idx used in pandore to get the neighborhood
 *           (with vc, see include/neighbours.h), accepted values are $4$ and $8$.
 */

template <typename T>
void makeWeightImg( Imx2d<T> & imgIn, Img2dsf & imgWeight, float sigma, int ngbIdx ) {
    float sigma2 = sigma * sigma;
    const Point2d * ngbPts = reinterpret_cast<const Point2d *>(vc[ngbIdx]);
    int imgWidth = imgIn.Width(), imgHeight = imgIn.Height(), imgSpectrum = imgIn.Bands();
    
    for (int y = 0; y < imgHeight; ++y) {
	for (int x = 0; x < imgWidth; ++x) {
	    int ngbCounter = 0;
	    unsigned long weightIdx = y * imgWidth + x;
	    
	    Point2d currentLocation(y, x);
	    for (int i = 0; i < ngbIdx; ++i) {
		Point2d displacement = ngbPts[i] + currentLocation;
		
		if (imgIn.Hold(displacement)) {
		    double dif = 0;
		    
		    for (int c = 0; c < imgSpectrum; ++c) {
			double tmpSquaredif = (double)(imgIn(c, currentLocation.y, currentLocation.x) - imgIn(c, displacement.y, displacement.x));
			tmpSquaredif *= tmpSquaredif;
			dif += tmpSquaredif;
			
		    }
		    
		    imgWeight(ngbCounter, weightIdx) = (Float)exp(-dif / sigma2);
		    ++ngbCounter;
		}
	    }
	}
    }
}


/**
 * Performs the NLMED
 * @param imgIn	the original image.
 * @param imgOut the resulting image.
 * @param sigma	sigma for the similarity measure.
 * @param ngbIdx the idx used in pandore to get the neighborhood
 *         (with vc, see include/neighbours.h), accepted values are $4$ and $8$.
 * @param imageRunner	used to run through the image, which may change in
            case $p = +\infty$. see perform_reg.h for more details on implementation.
 * @param regPerformer	performs the regularization, which are special
 *         cases for $p = 1$, $p = 2$ and $p = +\infty$. see perform_reg.h for more details on implementation.
 */
template <typename T>
Errc lplRegularization( Imx2d<T> & imgIn, Imx2d<T> & imgOut, float sigma, int ngbIdx, IRunThroughImage * imageRunner,  IPerformReg * regPerformer ) {
    int imgWidth = imgIn.Width(), imgHeight = imgIn.Height(), imgSpectrum = imgIn.Bands();
    
    Img2dsf weightImg(ngbIdx, imgWidth * imgHeight);
    makeWeightImg(imgIn, weightImg, sigma, ngbIdx);
    Imx2d<float> * imgRegularized01 = new Imx2d<float>(imgSpectrum, imgHeight, imgWidth);
    *imgRegularized01 = imgIn;
    imageRunner->performRunThrough(imgRegularized01, weightImg, regPerformer);
    
    imgOut = *imgRegularized01;
    
    delete imgRegularized01;
    return SUCCESS;
}

#ifdef MAIN
#define USAGE   "usage: %s sigma nb_iter connexity [im_in|-] [im_out|-]"
#define	PARC	3
#define	FINC	1
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
    
    ReadArgs(argc, argv, PARC, FINC, FOUTC, &mask, objin, objs, objout, objd, parv, USAGE, MASK);
    
    int ngbIdx = atoi(parv[2]);
    int nbIter = atoi(parv[1]);
    
    if (ngbIdx != 4 && ngbIdx != 8) {
	fprintf(stderr,"ERROR nonlocalmedianfiltering: specified neighborhood not supported by this operator : can only be 4 or 8\n");
    	result = FAILURE;
	goto end;
    }
    
    if (objs[0]->Type() == Po_Img2duc) {
	IRunThroughImage * imageRunner = new RunThroughImageP(ngbIdx, nbIter);
	IPerformReg * regPerformer = new PerformRegNLMED();

	Img2duc* const ims=(Img2duc*)objs[0];
	objd[0] = new Img2duc(ims->Props());
	Img2duc* const imd = (Img2duc*)objd[0];
	result = lplRegularization(*ims, *imd, (float)atof(parv[0]), ngbIdx, imageRunner, regPerformer);
	delete imageRunner;
	delete regPerformer;
	delete imageRunner;
	delete regPerformer;
	goto end;
    }
    if (objs[0]->Type() == Po_Img2dsl) {
	IRunThroughImage * imageRunner = new RunThroughImageP(ngbIdx, nbIter);
	IPerformReg * regPerformer = new PerformRegNLMED();

	Img2dsl* const ims=(Img2dsl*)objs[0];
	objd[0] = new Img2dsl(ims->Props());
	Img2dsl* const imd = (Img2dsl*)objd[0];
	result = lplRegularization(*ims, *imd, (float)atof(parv[0]), ngbIdx, imageRunner, regPerformer);
	delete imageRunner;
	delete regPerformer;
	goto end;
    }
    if (objs[0]->Type() == Po_Img2dsf) {
	IRunThroughImage * imageRunner = new RunThroughImageP(ngbIdx, nbIter);
	IPerformReg * regPerformer = new PerformRegNLMED();

	Img2dsf* const ims=(Img2dsf*)objs[0];
	objd[0] = new Img2dsf(ims->Props());
	Img2dsf* const imd = (Img2dsf*)objd[0];
	result = lplRegularization(*ims, *imd, (float)atof(parv[0]), ngbIdx, imageRunner, regPerformer);
	delete imageRunner;
	delete regPerformer;
	goto end;
    }
    if (objs[0]->Type() == Po_Imc2duc) {
	IRunThroughImage * imageRunner = new RunThroughImageP(ngbIdx, nbIter);
	IPerformReg * regPerformer = new PerformRegNLMED();

	Imc2duc* const ims=(Imc2duc*)objs[0];
	objd[0] = new Imc2duc(ims->Props());
	Imc2duc* const imd = (Imc2duc*)objd[0];
	result = lplRegularization(*ims, *imd, (float)atof(parv[0]), ngbIdx, imageRunner, regPerformer);
	delete imageRunner;
	delete regPerformer;
	goto end;
    }
    if (objs[0]->Type() == Po_Imc2dsl) {
	IRunThroughImage * imageRunner = new RunThroughImageP(ngbIdx, nbIter);
	IPerformReg * regPerformer = new PerformRegNLMED();

	Imc2dsl* const ims=(Imc2dsl*)objs[0];
	objd[0] = new Imc2dsl(ims->Props());
	Imc2dsl* const imd = (Imc2dsl*)objd[0];
	result = lplRegularization(*ims, *imd, (float)atof(parv[0]), ngbIdx, imageRunner, regPerformer);
	delete imageRunner;
	delete regPerformer;
	goto end;
    }
    if (objs[0]->Type() == Po_Imc2dsf) {
	IRunThroughImage * imageRunner = new RunThroughImageP(ngbIdx, nbIter);
	IPerformReg * regPerformer = new PerformRegNLMED();

	Imc2dsf* const ims=(Imc2dsf*)objs[0];
	objd[0] = new Imc2dsf(ims->Props());
	Imc2dsf* const imd = (Imc2dsf*)objd[0];
	result = lplRegularization(*ims, *imd, (float)atof(parv[0]), ngbIdx, imageRunner, regPerformer);
	delete imageRunner;
	delete regPerformer;
	goto end;
    }
    {
    	fprintf(stderr,"ERROR nonlocalmedianfiltering: Pandore type not supported by this operator\n");
    	result = FAILURE;
    }
    
  end:
    
    WriteArgs(argc,argv,PARC,FINC,FOUTC,&mask,objin,objs,objout,objd,MASK);
    
    Exit(result);
    return 0;
}

#endif
