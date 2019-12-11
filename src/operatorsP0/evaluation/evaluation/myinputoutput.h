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

#ifndef _INPUTOUTPUT_H
#define _INPUTOUTPUT_H

#include <cmath>
#include <iomanip>
#include "log.h"
#include "fileutils.h"
#include "evaluation.h"

#if (defined _MSC_VER) && (!defined snprintf)
#define snprintf _snprintf
#endif

#include <pandore.h>
using namespace pandore;

LabelImage<unsigned int> *
LoadLabelImage( const char * filename ) {
   class RegionMap: public Reg2d {
   public:
      unsigned int * getVector() {
	 _isDataOwner = false;
	 return Vector();
      }
   };
   
   RegionMap regionmap;
   
   regionmap.LoadFile(filename);
   LabelImage<unsigned int> * image = new LabelImage<unsigned int>(regionmap.Width(), regionmap.Height(), regionmap.Labels(), regionmap.getVector());
   return image;
}

inline void
SaveDiscrepancyResults( std::string *featureNames, std::vector<DiscrepancyResult> &results, Collection &cold ) {
   const int nbIndicators = 5;
   Float** valueArray = new Float*[nbIndicators * 2];
   Float** normalizationArray = new Float*[nbIndicators * 2];
   
   for (int i = 0; i < nbIndicators; i++) {
      valueArray[i * 2] = new Float[results.size()];
      normalizationArray[i * 2] = new Float[results.size()];
      valueArray[i * 2 + 1] = new Float[results.size()];
      normalizationArray[i * 2 + 1] = new Float[results.size()];
      for (unsigned int j = 0; j < results.size(); j++) {
       	 valueArray[i * 2][j] = results[j].indicators[i].errors[0].value;
      	 normalizationArray[i * 2][j] = results[j].indicators[i].errors[0].normalization;
      	 valueArray[i * 2 + 1][j] = results[j].indicators[i].errors[1].value;
      	 normalizationArray[i * 2 + 1][j] = results[j].indicators[i].errors[1].normalization;
      }
   }
   
   char tmp[256];
   for (int i = 0; i < nbIndicators * 2 ; i++) {
      snprintf(tmp, sizeof(tmp), "%s.%d", featureNames[0].c_str(), i + 1);
      tmp[sizeof(tmp) - 1] = 0;
      cold.SETARRAY(tmp, Float, valueArray[i], results.size());
      snprintf(tmp, sizeof(tmp), "%s.%d", featureNames[1].c_str(), i + 1);
      tmp[sizeof(tmp) - 1] = 0;
      cold.SETARRAY(tmp, Float, normalizationArray[i], results.size());
   }
}

inline void
SaveEvaluationResult( const DiscrepancyResult & evaluationError, Collection & cold, const std::string errorName ) {
   char tmp[256];
   const int nbIndicators = 5;
   for (int i = 0; i < nbIndicators; i++ ) {
      for (int j = 0; j < 2; j++ ) {
	 snprintf(tmp, sizeof(tmp), "%s.%d", errorName.c_str(), i * 2 + j + 1);
	 tmp[sizeof(tmp) - 1] = 0;
	 cold.SETVALUE(tmp, Float, evaluationError.indicators[i].errors[j].value);   
      }
   }
}

inline void
SaveEvaluationResult( const DiscrepancyResult & evaluationError, Collection & cold, const std::string errorNames[]) {
   for (int j = 0; j < 2; j++ ) {
      cold.SETVALUE(errorNames[j], Float, (Float)evaluationError.indicators[0].errors[j].value);
   }
}

Errc
ReadCollection( const Collection & cols,  const std::string &errorName, int nbOfIndicators, DiscrepancyResult &evaluationResult ) {
   char tmp1[256];
   char tmp2[256];
   
   for (int i = 0; i < nbOfIndicators; i++) {
      snprintf(tmp1, sizeof(tmp1), "%s.%d", errorName.c_str(), 2 * i + 1);
      tmp1[sizeof(tmp1) - 1] = 0;
      std::string type1 = cols.GetType(tmp1);
      snprintf(tmp2, sizeof(tmp2), "%s.%d", errorName.c_str(), 2 * i + 2);
      tmp1[sizeof(tmp2) - 1] = 0;
      std::string type2 = cols.GetType(tmp2);
      
      if (type1 == "Float" && type2 == "Float") {
	 evaluationResult.indicators[i].errors[0].value = cols.GETVALUE(tmp1, Float);
	 evaluationResult.indicators[i].errors[1].value = cols.GETVALUE(tmp2, Float);
      } else {
	 std::cerr << "Error : Invalid collection contents" << std::endl;
	 return FAILURE;
      }
   }
   return SUCCESS;
}

void
BuildOutputResult1( const std::vector<EvaluationPerformance> &performances, Collection & cold1 ) {
   const std::string nameOut = "Rank";
   Float **indicators = new Float*[performances.size()];
   for (unsigned int i = 0; i < performances.size(); i++) {
      indicators[i] = new Float[performances.size()];
      for (unsigned int j = 0; j < performances.size(); j++) {
	 indicators[i][j] =  performances[j].indicators[i];
      }
   }
   char tmp1[256];
   for (unsigned int i = 0; i < performances.size(); i++) {
      snprintf(tmp1, sizeof(tmp1), "Indicator.%d", i + 1);
      tmp1[sizeof(tmp1) - 1] = 0;
      cold1.SETARRAY(tmp1, Float, indicators[i], performances.size());
   }
}

void
BuildOutputResult2( std::vector<long> &ranks, Collection & cold2 ) {
   Long * array = new Long[ranks.size()];
   std::copy(ranks.begin(), ranks.end(), array);
   cold2.SETARRAY("Rank", Long, array, ranks.size());
}

#endif
