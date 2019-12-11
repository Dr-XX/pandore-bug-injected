/* -*- c-basic-offset: 3; mode: c++ -*-
 *
 * Copyright (c), GREYC.
 * All rights reserved.
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
 * Based on (1-Recall) and  (1-Precision) with a pixel-wise approach.
 */

#include "evaluation.h"

inline DiscrepancyError
computeDeficitError( const MatchingGraph &graph ) {
   int numberOfTruePositiveRegions = 0;
   float sumOfRecallErrors = 0;

   for (int r = 1; r < graph.NumberOfRegions(); r++) {
      if (graph.isRegionMatched(r)) {
 	 int overlappingArea = 0;
	 for (Edge *ptr = graph.getMatchedSegmentsList(r); ptr != NULL; ptr = ptr->Next()) {
	    overlappingArea += ptr->overlappingArea;
	 }
	 sumOfRecallErrors += (float)(graph.getRegionArea(r) - overlappingArea)
	    / (float)graph.getRegionArea(r);
	 numberOfTruePositiveRegions++;
#ifndef NDEBUG
	 float falseNegatives = (float)graph.getRegionArea(r) - overlappingArea;
	 printf("\n    Region %d: area: %ld overlapping=%d FN=%f (%f)\n", r, graph.getRegionArea(r), overlappingArea, (float)falseNegatives, falseNegatives / (float)graph.getRegionArea(r));
#endif
      }
   }

#ifndef NDEBUG
   float deficitRate = 0;
   if (numberOfTruePositiveRegions > 0) {
      deficitRate = sumOfRecallErrors / numberOfTruePositiveRegions;
   }   
   printf(" Error 5/  Deficit.....................: %.3f (%.0f%% of misdetected pixels in detected regions)\n", deficitRate, deficitRate * 100.0f);
#endif
   return DiscrepancyError(sumOfRecallErrors, (float)numberOfTruePositiveRegions);
}

inline DiscrepancyError
computeExcessError( const MatchingGraph &graph ) {
   int numberOfTruePositiveSegments = 0;
   float sumOfPrecisionErrors = 0;

   for (int s = 1; s < graph.NumberOfSegments(); s++) {
      if (graph.isSegmentMatched(s)) {
	 int overlappingArea = 0;
	 for (Edge *ptr = graph.getMatchedRegionsList(s); ptr != NULL; ptr = ptr->Next()) {
	    overlappingArea += ptr->overlappingArea;
	 }
	 sumOfPrecisionErrors += (float)(graph.getSegmentArea(s) - overlappingArea)
	    / (float)graph.getSegmentArea(s);
	 numberOfTruePositiveSegments++;
#ifndef NDEBUG
	 float falsePositives = (float)graph.getSegmentArea(s) - (float)overlappingArea;
	 printf("\n    Segment %d: area: %ld overlapping=%d FP=%f (%f)\n", s, graph.getSegmentArea(s), overlappingArea, falsePositives, (float)falsePositives / (float)graph.getSegmentArea(s));
#endif
      }
   }
#ifndef NDEBUG
   float excessRate = 0;
   if (numberOfTruePositiveSegments > 0) {
      excessRate = sumOfPrecisionErrors / numberOfTruePositiveSegments;
   }
   printf(" Error 6/  Excess......................: %.3f (%.0f%% of overdetected pixels in correct segments)\n",  excessRate, excessRate * 100.0f);
#endif
   
   return DiscrepancyError(sumOfPrecisionErrors, (float)numberOfTruePositiveSegments);
}

inline DiscrepancyIndicator
BoundaryPrecisionMeasurement( const LabelImage<unsigned int> &, const LabelImage<unsigned int> &, MatchingGraph &grs ) {
   DiscrepancyIndicator indicator;
   indicator.errors[0] = computeDeficitError(grs);
   indicator.errors[1] = computeExcessError(grs);

   return indicator;
}
