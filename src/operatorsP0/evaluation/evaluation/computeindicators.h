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

#ifndef _COMPUTEINDICATOIRS_H
#define _COMPUTEINDICATOIRS_H

#include <math.h>
#include "evaluation.h"

inline float
WeightedHarmonicMean( float e1, float e2, float alpha ) {
   if (e1 == 1.0f && e2 == 1.0f) {
      return 1.0f;
   } else {
      return (float)(1.0f - (1.0f / ( (alpha / (1.0f - e2)) +  ((1.0f - alpha) / (1.0f - e1)))));
   }
}

inline EvaluationPerformance
ComputeIndicators( const DiscrepancyResult & discrepancyResult, int * propositions ) {
   EvaluationPerformance evaluationPerformance;
   for (int i = 0; i < 5; i++) {
	 switch (propositions[i]) {
	    case 1:
	       evaluationPerformance.indicators[i] = 0;
	       break;
	    case 2:
	       evaluationPerformance.indicators[i] = WeightedHarmonicMean(discrepancyResult.indicators[i].errors[0].value, discrepancyResult.indicators[i].errors[1].value, 0.5f);
	       break;
	    case 3:
	       evaluationPerformance.indicators[i] = WeightedHarmonicMean(discrepancyResult.indicators[i].errors[0].value, discrepancyResult.indicators[i].errors[1].value, 0.8f);
	       break;
	    case 4:
	       evaluationPerformance.indicators[i] = WeightedHarmonicMean(discrepancyResult.indicators[i].errors[0].value, discrepancyResult.indicators[i].errors[1].value, 0.2f);
	       break;
	    case 5:
	       evaluationPerformance.indicators[i] = WeightedHarmonicMean(discrepancyResult.indicators[i].errors[0].value, discrepancyResult.indicators[i].errors[1].value, 1.0f);
	       break;
	    case 6:
	       evaluationPerformance.indicators[i] = WeightedHarmonicMean(discrepancyResult.indicators[i].errors[0].value, discrepancyResult.indicators[i].errors[1].value, 0.0f);
	       break;
	    case 7:
	       if (discrepancyResult.indicators[i].errors[0].value == 0) {
		  evaluationPerformance.indicators[i] = discrepancyResult.indicators[i].errors[1].value;
	       } else {
		  evaluationPerformance.indicators[i] = 1.0;
	       }
	       break;
	    case 8:
	       if (discrepancyResult.indicators[i].errors[1].value == 0) {
		  evaluationPerformance.indicators[i] = discrepancyResult.indicators[i].errors[0].value;
	       } else {
		  evaluationPerformance.indicators[i] = 1.0;
	       }
	       break;
	    default:
	       std::stringstream message;
	       message << "Invalid paramater #" << (i + 1) << "="
		       << propositions[i] << ". Should be in range [1-8].";
	       throw RuntimeException(message.str());
	 }
   }
   return evaluationPerformance;
}

#endif
