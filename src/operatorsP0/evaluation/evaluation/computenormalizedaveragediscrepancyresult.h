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

#ifndef _COMPUTENORMALIZEDAVERAGEDISCREPANCYRESULT_H
#define _COMPUTENORMALIZEDAVERAGEDISCREPANCYRESULT_H

#include <cmath>

/**
 * Computes global discrepancy error values for individual result.
 */

inline void
NormalizeDetectionAccuracyEvaluationResult( DiscrepancyIndicator &indicator, DiscrepancyResult & result, int criteria ) {
   const int nbOfErrors = 2;
   for (int e = 0; e < nbOfErrors; e++) {
      if (indicator.errors[e].normalization == 0) {
	 result.indicators[criteria].errors[e].value = 0;
      } else {
	 result.indicators[criteria].errors[e].value = indicator.errors[e].value / indicator.errors[e].normalization;
      }
   }
}

inline void
NormalizeFragmentationConsistencyEvaluationResult( DiscrepancyIndicator &indicator, DiscrepancyResult & result, int criteria ) {
   const int nbOfErrors = 2;   
   for (int e = 0; e < nbOfErrors; e++) {
      if (indicator.errors[e].normalization == 0) {
	 result.indicators[criteria].errors[e].value = 0;
      } else {
	 float H = indicator.errors[e].value / indicator.errors[e].normalization;
	 result.indicators[criteria].errors[e].value =  H / (H + 1.0f);
      }   
   }
}

inline void
NormalizeBoundaryPrecisionEvaluationResult( DiscrepancyIndicator &indicator, DiscrepancyResult & result, int criteria ) {
   const int nbOfErrors = 2;
   for (int e = 0; e < nbOfErrors; e++) {
      if (indicator.errors[e].normalization == 0) {
	 result.indicators[criteria].errors[e].value = 0;
      } else {
	 result.indicators[criteria].errors[e].value = indicator.errors[e].value / indicator.errors[e].normalization;
      }
   }
}

inline void
NormalizeShapeFidelityEvaluationResult( DiscrepancyIndicator &indicator, DiscrepancyResult & result, int criteria ) {
   const int nbOfErrors = 2;
   for (int e = 0; e < nbOfErrors; e++) {
      if (indicator.errors[e].normalization == 0) {
	 result.indicators[criteria].errors[e].value = 0;
      } else {
	 float x = log10(indicator.errors[e].value / indicator.errors[e].normalization  + 1);
	 result.indicators[criteria].errors[e].value =  x / (x + 1.0F);
      }
   }
}

inline void
NormalizeTopologyPreservationResult( DiscrepancyIndicator &indicator, DiscrepancyResult & result, int criteria ) {
   const int nbOfErrors = 2;
   for (int e = 0; e < nbOfErrors; e++) {
      if (indicator.errors[e].normalization == 0) {
	 result.indicators[criteria].errors[e].value = 0;
      } else {
	 result.indicators[criteria].errors[e].value = indicator.errors[e].value / indicator.errors[e].normalization;
      }
   }
}

inline DiscrepancyResult
ComputeNormalizedAverageDiscrepancyResult( const std::vector<DiscrepancyResult> &discrepancyResultForAllSegmentations ) {
   DiscrepancyResult evaluationResult;

   const int vectorSize = discrepancyResultForAllSegmentations.size();
   
   // 1. detection accuracy
   int criteria = 0;
   {
      DiscrepancyIndicator indicator;
      
      for (int e = 0; e < 2; e++) {
	 for (int i = 0; i < vectorSize; ++i) {
	    indicator.errors[e].value += discrepancyResultForAllSegmentations[i].indicators[criteria].errors[e].value;
	    indicator.errors[e].normalization += discrepancyResultForAllSegmentations[i].indicators[criteria].errors[e].normalization;
	 }
      }
      NormalizeDetectionAccuracyEvaluationResult(indicator, evaluationResult, criteria);
   }

   // 2. fragmentation consistency
   criteria++;
   {
      DiscrepancyIndicator indicator;
      for (int e = 0; e < 2; e++) {
	 for (int i = 0; i < vectorSize; ++i) {
	    indicator.errors[e].value += discrepancyResultForAllSegmentations[i].indicators[criteria].errors[e].value;
	    indicator.errors[e].normalization += discrepancyResultForAllSegmentations[i].indicators[criteria].errors[e].normalization;
	 }
      }
      NormalizeFragmentationConsistencyEvaluationResult(indicator, evaluationResult, criteria);
   }

   // 3. boundary precision
   criteria++;
   {
      DiscrepancyIndicator indicator;
      for (int e = 0; e < 2; e++) {
	 for (int i = 0; i < vectorSize; ++i) {
	    indicator.errors[e].value += discrepancyResultForAllSegmentations[i].indicators[criteria].errors[e].value;
	    indicator.errors[e].normalization += discrepancyResultForAllSegmentations[i].indicators[criteria].errors[e].normalization;
	 }
      }
      NormalizeBoundaryPrecisionEvaluationResult(indicator, evaluationResult, criteria);
   }

   // 4. shape fidelity
   criteria++;
   {
      DiscrepancyIndicator indicator;
      for (int e = 0; e < 2; e++) {
	 for (int i = 0; i < vectorSize; ++i) {
	    indicator.errors[e].value += discrepancyResultForAllSegmentations[i].indicators[criteria].errors[e].value;
	    indicator.errors[e].normalization += discrepancyResultForAllSegmentations[i].indicators[criteria].errors[e].normalization;
	 }
      }
      NormalizeShapeFidelityEvaluationResult(indicator, evaluationResult, criteria);
   }

   // 5. topology preservation
   criteria++;
   {
      DiscrepancyIndicator indicator;
      for (int e = 0; e < 2; e++) {
	 for (int i = 0; i < vectorSize; ++i) {
	    indicator.errors[e].value += discrepancyResultForAllSegmentations[i].indicators[criteria].errors[e].value;
	    indicator.errors[e].normalization += discrepancyResultForAllSegmentations[i].indicators[criteria].errors[e].normalization;
	 }
      }
      NormalizeTopologyPreservationResult(indicator, evaluationResult, criteria);
   }

   return evaluationResult;
}

#endif
