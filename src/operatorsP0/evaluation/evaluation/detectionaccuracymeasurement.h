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

/**
 * The detection accuracy measure is based
 * on precision and recall errors.
 */

#ifndef _DETECTIONACCURACYMEASUREMENT_H
#define _DETECTIONACCURACYMEASUREMENT_H

#include "evaluation.h"

inline void
debug1( int truePositiveRegions , int falseNegativeRegions ) {
   double recallError;   
   if ((truePositiveRegions + falseNegativeRegions) != 0) {
      recallError = falseNegativeRegions / (truePositiveRegions + falseNegativeRegions);
   } else {
      recallError = 0.0;
   }
   printf("* DETECTION ACCURACY\n");
   printf(" 1/  Recall error.............: %.3f (%.0f%% of misdetected regions)\n", recallError, recallError * 100.0f);
   printf("     FN=%d and TP+FN=%d\n", falseNegativeRegions, truePositiveRegions + falseNegativeRegions);
}

inline void
debug2( int truePositiveSegments , int falsePositiveSegments ) {
   double precisionError;
   if ((truePositiveSegments + falsePositiveSegments) != 0) {
      precisionError = falsePositiveSegments / (truePositiveSegments + falsePositiveSegments);
   } else {
      precisionError = 0.0;
   }
   printf(" 2/  Precision error..........: %.3f (%.0f%% of spurious segments)\n", precisionError, precisionError * 100.0f);
   printf("     FP=%d and TP+FP=%d\n", falsePositiveSegments, truePositiveSegments + falsePositiveSegments);
}

inline DiscrepancyError
computeRecallError( const MatchingGraph &graph ) {
   int truePositiveRegions = 0;
   int falseNegativeRegions = 0;

   for (int r = 1; r < graph.NumberOfRegions(); r++) {
      if (graph.getRegionArea(r) > 0) {
	 if (graph.isRegionMatched(r)) {
	    truePositiveRegions++;
	 } else {
	    falseNegativeRegions++;
	 }
      }
   }

#ifndef NDEBUG
   debug1(truePositiveRegions, falseNegativeRegions);
#endif

   return DiscrepancyError((float)falseNegativeRegions, (float)(truePositiveRegions + falseNegativeRegions));
}

inline DiscrepancyError
computePrecisionError( const MatchingGraph &graph ) {
   int truePositiveSegments = 0;
   int falsePositiveSegments = 0;

   for (int s = 1; s < graph.NumberOfSegments(); s++) {
      if (graph.getSegmentArea(s) > 0) {
	 if (graph.isSegmentMatched(s)) {
	    truePositiveSegments++;
	 } else {
	    falsePositiveSegments++;
	 }
      }
   }

#ifndef NDEBUG
   debug2(truePositiveSegments, falsePositiveSegments);
#endif

   return DiscrepancyError((float)falsePositiveSegments, (float)(truePositiveSegments + falsePositiveSegments));
}

inline DiscrepancyIndicator
DetectionAccuracyMeasurement( const LabelImage<unsigned int> &, const LabelImage<unsigned int> &, const MatchingGraph &graph ) {
   DiscrepancyIndicator indicator; 
   indicator.errors[0] = computeRecallError(graph);
   indicator.errors[1] = computePrecisionError(graph);
   return indicator;
}

#endif
