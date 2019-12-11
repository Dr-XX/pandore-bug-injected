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
 * For more information, refer to:
 * https://clouard.users.greyc.fr/Pandore/
 */

/**
 * @author Matthieu Toutain - 2012-09-21
 */

#ifndef _PERFORM_REG_H
#define _PERFORM_REG_H

#include <pandore.h>
#include <limits>
#include <vector>
#include <algorithm>

using namespace pandore;

class IPerformReg {  
public:
    IPerformReg() : _p(0) {}
    IPerformReg(float p) : _p(p) {}
    virtual ~IPerformReg() {}
    virtual double performRegNgb( Imx2d<float> * imgRegularized01, Img2dsf & weightImg, Point2d & currentLocation, int currentC, const Point2d * ngbPts, int nbNgb ) = 0;
    
    void setP(float p) {
	_p = p;
    }
    
protected:
    float _p;   
};

class IRunThroughImage {
public:
    IRunThroughImage(int nbNgb = 0, int nbIter = 0) : _nbNgb(nbNgb), _nbIter(nbIter) {}
    virtual ~IRunThroughImage() { }
    virtual void performRunThrough(Imx2d<float> * imgRegularized, Img2dsf & weightImg, IPerformReg * regPerformer) = 0;
    
protected:
    int _nbNgb;
    int _nbIter;
};

class PerformRegP1 : public IPerformReg {
public:
    PerformRegP1() : IPerformReg() {}
    PerformRegP1(float p) : IPerformReg(p) {}
    double performRegNgb( Imx2d<float> * imgRegularized01, Img2dsf & weightImg, Point2d & currentLocation, int currentC, const Point2d * ngbPts, int nbNgb ) {
	unsigned long weightIdx = currentLocation.y * imgRegularized01->Width() + currentLocation.x;
	double sum1 = 0, sum2 = 0;
	int ngbCounter = 0;
	double currentVal = (double)(*imgRegularized01)(currentC, currentLocation.y, currentLocation.x);
      
	for (int ngbIt = 0; ngbIt < nbNgb; ++ngbIt) {
	    Point2d displacement = ngbPts[ngbIt] + currentLocation;
	    if (imgRegularized01->Hold(displacement)) {
		double ngbVal = (double)(*imgRegularized01)(currentC, displacement.y, displacement.x);
		double weightTimeDif = sqrt(weightImg(ngbCounter, weightIdx)) / (abs(ngbVal - currentVal) + 1e-6);
	    
		sum1 += weightTimeDif;
		sum2 += weightTimeDif * ngbVal;
	    
		++ngbCounter;
	    }
	}
	return sum2 / (sum1 + 1e-6);
    }
};

class PerformRegP2 : public IPerformReg {
public:
    PerformRegP2() : IPerformReg() {}
    PerformRegP2(float p) : IPerformReg(p) {}
   
    double performRegNgb(Imx2d<float> * imgRegularized01, Img2dsf & weightImg, Point2d & currentLocation, int currentC, const Point2d * ngbPts, int nbNgb) {
	unsigned long weightIdx = currentLocation.y * imgRegularized01->Width() + currentLocation.x;
	double sum1 = 0, sum2 = 0;
	int ngbCounter = 0;
      
	for(int ngbIt = 0; ngbIt < nbNgb; ++ngbIt) {
	    Point2d displacement = ngbPts[ngbIt] + currentLocation;
	    if (imgRegularized01->Hold(displacement)) {
		double ngbVal = (double)(*imgRegularized01)(currentC, displacement.y, displacement.x);
		double weightTimeDif = weightImg(ngbCounter, weightIdx);
	    
		sum1 += weightTimeDif;
		sum2 += weightTimeDif * ngbVal;
	    
		++ngbCounter;
	    }
	}
	return sum2 / (sum1 + 1e-6);
    }
};

class PerformRegAnyP : public IPerformReg {
public:
    PerformRegAnyP() : IPerformReg() {}
    PerformRegAnyP(float p) : IPerformReg(p) {}
   
    double performRegNgb(Imx2d<float> * imgRegularized01, Img2dsf & weightImg, Point2d & currentLocation, int currentC, const Point2d * ngbPts, int nbNgb) {
	unsigned long weightIdx = currentLocation.y * imgRegularized01->Width() + currentLocation.x;
	double sum1 = 0, sum2 = 0;
	int ngbCounter = 0;
	double currentVal = (double)(*imgRegularized01)(currentC, currentLocation.y, currentLocation.x);
      
	for(int ngbIt = 0; ngbIt < nbNgb; ++ngbIt) {
	    Point2d displacement = ngbPts[ngbIt] + currentLocation;
	    if (imgRegularized01->Hold(displacement)) {
		double ngbVal = (double)(*imgRegularized01)(currentC, displacement.y, displacement.x);
	    
		double weightTimeDif = pow((double)weightImg(ngbCounter, weightIdx), _p / 2.0) * pow(abs(ngbVal - currentVal) + 1.0e-6, _p - 2.0);
	    
		sum1 += weightTimeDif;
		sum2 += weightTimeDif * ngbVal;
	    
		++ngbCounter;
	    }
	}
	return sum2 / (sum1 + 1e-6);
    }
};

class PerformRegNLE : public IPerformReg {
   
public:
    PerformRegNLE() : IPerformReg() {}
    PerformRegNLE(float p) : IPerformReg(p) {}

    double performRegNgb(Imx2d<float> * imgRegularized01, Img2dsf & weightImg, Point2d & currentLocation, int currentC, const Point2d * ngbPts, int nbNgb) {
	unsigned long weightIdx = currentLocation.y * imgRegularized01->Width() + currentLocation.x;
	int ngbCounter = 0;
	double currentVal = (double)(*imgRegularized01)(currentC, currentLocation.y, currentLocation.x);
	double min = std::numeric_limits<double>::max();
      
	for(int ngbIt = 0; ngbIt < nbNgb; ++ngbIt) {
	    Point2d displacement = ngbPts[ngbIt] + currentLocation;
	    if (imgRegularized01->Hold(displacement)) {
		double ngbVal = (double)(*imgRegularized01)(currentC, displacement.y, displacement.x);
		double weight = weightImg(ngbCounter, weightIdx);
		double dif = ngbVal - currentVal;
	    
		double tmpWMin = weight * MIN(dif, 0.0f);
	    
		if (tmpWMin < min) {
		    min = tmpWMin;
		}
		++ngbCounter;
	    }
	 
	}
      
	if (min > 0) {
	    min = 0;
	}
	return currentVal + min;
    }
};

class PerformRegNLD : public IPerformReg {
public:
    PerformRegNLD() : IPerformReg() {}
    PerformRegNLD(float p) : IPerformReg(p) {}
   
    double performRegNgb(Imx2d<float> * imgRegularized01, Img2dsf & weightImg, Point2d & currentLocation, int currentC, const Point2d * ngbPts, int nbNgb) {
	unsigned long weightIdx = currentLocation.y * imgRegularized01->Width() + currentLocation.x;
	int ngbCounter = 0;
	double currentVal = (double)(*imgRegularized01)(currentC, currentLocation.y, currentLocation.x);
	double max = 1 - std::numeric_limits<double>::max();
      
	for(int ngbIt = 0; ngbIt < nbNgb; ++ngbIt) {
	    Point2d displacement = ngbPts[ngbIt] + currentLocation;
	 
	    if (imgRegularized01->Hold(displacement)) {
		double ngbVal = (double)(*imgRegularized01)(currentC, displacement.y, displacement.x);
		double weight = weightImg(ngbCounter, weightIdx);
		double dif = ngbVal - currentVal;
	    
		double tmpWMax = weight * MAX(dif, 0.0f);
	    
		if  (tmpWMax > max) {
		    max = tmpWMax;
		}
		++ngbCounter;
	    }
	 
	}
      
	if (max < 0) {
	    max = 0;
	}
      
	return currentVal + max;
    }
};

class PerformRegNLMED : public IPerformReg {
   
public:
    PerformRegNLMED() : IPerformReg() {}
    PerformRegNLMED(float p) : IPerformReg(p) {}
   
    double performRegNgb(Imx2d<float> * imgRegularized01, Img2dsf & weightImg, Point2d & currentLocation, int currentC, const Point2d * ngbPts, int nbNgb) {
	unsigned long weightIdx = currentLocation.y * imgRegularized01->Width() + currentLocation.x;
	int ngbCounter = 0;
	double currentVal = (double)(*imgRegularized01)(currentC, currentLocation.y, currentLocation.x);
	std::vector<double> ngbValsVec;
      
	for(int ngbIt = 0; ngbIt < nbNgb; ++ngbIt) {
	    Point2d displacement = ngbPts[ngbIt] + currentLocation;
	 
	    if (imgRegularized01->Hold(displacement)) {
		double ngbVal = (double)(*imgRegularized01)(currentC, displacement.y, displacement.x);
		double weight = weightImg(ngbCounter, weightIdx);
		double dif = ngbVal - currentVal;
		double tmpVal = sqrt(weight) * dif;
	    
		ngbValsVec.push_back(tmpVal);
	    
		++ngbCounter;
	    }
	 
	}
      
	std::sort(ngbValsVec.begin(), ngbValsVec.end());
      
	return getMedianValue(ngbValsVec) + currentVal;
    }
   
private:
   
    double getMedianValue(std::vector<double> &vec) {
	int vecSize = vec.size();
      
	if (vecSize == 0) {
	    return 0;
	}
	if (vecSize % 2) {
	    return vec[(vecSize / 2)];
	} else {
	    int medIdx = vecSize / 2;
	 
	    if (medIdx > 0) {
		return (vec[medIdx] + vec[medIdx - 1]) / 2;
	    }
	 
	    return (vec[medIdx] + vec[medIdx + 1]) / 2;
	}  
    }
};

class PerformRegNLMInf : public IPerformReg {
public:
    PerformRegNLMInf() : IPerformReg() {}
    PerformRegNLMInf(float p) : IPerformReg(p) {}
   
    double performRegNgb(Imx2d<float> * imgRegularized01, Img2dsf & weightImg, Point2d & currentLocation, int currentC, const Point2d * ngbPts, int nbNgb) {
	unsigned long weightIdx = currentLocation.y * imgRegularized01->Width() + currentLocation.x;
	int ngbCounter = 0;
	double currentVal = (double)(*imgRegularized01)(currentC, currentLocation.y, currentLocation.x);
	double min = std::numeric_limits<double>::max();
	double max = 1 - std::numeric_limits<double>::max();
      
	for(int ngbIt = 0; ngbIt < nbNgb; ++ngbIt) {
	    Point2d displacement = ngbPts[ngbIt] + currentLocation;
	    if (imgRegularized01->Hold(displacement)) {
		double ngbVal = (double)(*imgRegularized01)(currentC, displacement.y, displacement.x);
		double weight = weightImg(ngbCounter, weightIdx);
		double dif = ngbVal - currentVal;
	    
		double tmpWMax = weight * MAX(dif, 0.0f);
	    
		if (tmpWMax > max) {
		    max = tmpWMax;
		}
		double tmpWMin = weight * MIN(dif, 0.0f);
	    
		if (tmpWMin < min) {
		    min = tmpWMin;
		}
		++ngbCounter;
	    }
	 
	}
      
	if (max < 0) {
	    max = 0;
	}
	if (min > 0) {
	    min = 0;
	}
	return currentVal + (max + min) / 2;  
    }
};

class RunThroughImageP : public IRunThroughImage {
public:
    RunThroughImageP(int nbNgb = 0, int nbIter = 0) : IRunThroughImage(nbNgb, nbIter) {}
   
    void performRunThrough(Imx2d<float> * imgRegularized01, Img2dsf & weightImg, IPerformReg * regPerformer) {
	int imgWidth = imgRegularized01->Width(), imgHeight = imgRegularized01->Height(), imgSpectrum = imgRegularized01->Bands();
	const Point2d * ngbPts = reinterpret_cast<const Point2d *>(vc[_nbNgb]);
      
	Imx2d<float> * imgRegularized02 = new Imx2d<float>(imgSpectrum, imgHeight, imgWidth);
	*imgRegularized02 = *imgRegularized01;
	Imx2d<float> * tmpSwap;
      
	for (int i = 0; i < _nbIter; ++i) {
	    for (int c = 0; c < imgSpectrum; ++c) {
		for (int y = 0; y < imgHeight; ++y) {
		    for (int x = 0; x < imgWidth; ++x) {
			Point2d currentLocation(y, x);
			(*imgRegularized02)(c, y, x) = (Float)regPerformer->performRegNgb(imgRegularized01, weightImg, currentLocation, c, ngbPts, _nbNgb);
		    }
		}
	    }
	 
	    tmpSwap = imgRegularized02;
	    imgRegularized02 = imgRegularized01;
	    imgRegularized01 = tmpSwap;
	}
      
	if (_nbIter % 2) {	 
	    *imgRegularized02 = *imgRegularized01;
	    delete imgRegularized01;
	} else {
	    delete imgRegularized02;
	}
    }
};

class RunThroughImagePInf : public IRunThroughImage {
   
public:
    RunThroughImagePInf(int nbNgb = 0, int nbIter = 0) : IRunThroughImage(nbNgb, nbIter) {}
   
    void performRunThrough(Imx2d<float> * imgRegularized01, Img2dsf & weightImg, IPerformReg * ) {
	PerformRegNLE * nlePerformer = new PerformRegNLE();
	PerformRegNLD * nldPerformer = new PerformRegNLD();
	int imgWidth = imgRegularized01->Width(), imgHeight = imgRegularized01->Height(), imgSpectrum = imgRegularized01->Bands();
	const Point2d * ngbPts = reinterpret_cast<const Point2d *>(vc[_nbNgb]);
      
	Imx2d<float> * imgRegularized02 = new Imx2d<float>(imgSpectrum, imgHeight, imgWidth);
	*imgRegularized02 = *imgRegularized01;
      
	Imx2d<float> * imgRegularized03 = new Imx2d<float>(imgSpectrum, imgHeight, imgWidth);
	*imgRegularized03 = *imgRegularized01;
      
	Imx2d<float> * imgRegularized04 = new Imx2d<float>(imgSpectrum, imgHeight, imgWidth);
	*imgRegularized04 = *imgRegularized01;
	Imx2d<float> * tmpSwap;
      
	for(int i = 0; i < _nbIter; ++i) {
	    for(int c = 0; c < imgSpectrum; ++c) {
		for(int y = 0; y < imgHeight; ++y) {
		    for(int x = 0; x < imgWidth; ++x) {
			Point2d currentLocation(y, x);
			(*imgRegularized02)(c, y, x) = (Float)nlePerformer->performRegNgb(imgRegularized01, weightImg, currentLocation, c, ngbPts, _nbNgb);
			(*imgRegularized04)(c, y, x) = (Float)nldPerformer->performRegNgb(imgRegularized03, weightImg, currentLocation, c, ngbPts, _nbNgb);
		    }
		}
	    }
	 
	    tmpSwap = imgRegularized02;
	    imgRegularized02 = imgRegularized01;
	    imgRegularized01 = tmpSwap;
	 
	    tmpSwap = imgRegularized04;
	    imgRegularized04 = imgRegularized03;
	    imgRegularized03 = tmpSwap;
	 
	}
      
	AddImages(*imgRegularized01, *imgRegularized03, *imgRegularized01);
      
	delete nlePerformer;
	delete nldPerformer;
      
	if (_nbIter % 2) {
	    *imgRegularized02 = *imgRegularized01;
	    delete imgRegularized01;
	} else {
	    delete imgRegularized02;
	}
	delete imgRegularized03;
	delete imgRegularized04;
    }
   
private:
      
    void AddImages(Imx2d<float> & imgIn1, Imx2d<float> & imgIn2, Imx2d<float> & imgOut) {
	for(int c = 0; c < imgOut.Bands(); ++c) {
	    for(int y = 0; y < imgOut.Height(); ++y) {
		for(int x = 0; x < imgOut.Width(); ++x) {
		    imgOut(c, y, x) = (imgIn1(c, y, x) + imgIn2(c, y, x)) / 2;
		}
	    }
	}  
    }   
};

#endif // _PERFORM_REG_H
