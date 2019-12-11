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
 * The consistency measure consists of fragmentation and inclusion indices.
 * It is based on the conditional entropy measures.
 *
 * H(R|S) = Sum Sum (|Ri * Sj| / n) . log2(|Sj| / |Ri * Sj|)
 * where * is the intesection operator, and n is the total overlapping area.
 *
 * Reference : Lukashevich 2008, Gong 2011.
 */

#include <cmath>
#include "matching.h"
#include "evaluation.h"

#ifdef WIN32
// Calculates log2 of number.  
inline float
log2( float n ) {
    return (float)(log(n) / log(2.0));
}
#endif

inline DiscrepancyError
computeUnderSegmentationError( MatchingGraph & graph ) {
   // 1. Compute actual overlapped segment areas (Sj inter {Ri})
   for (int s = 1; s < graph.NumberOfSegments(); s++) {
      long total = 0;      
      if (graph.isSegmentMatched(s)) { // Overlapping
	 for (Edge *ptr = graph.getMatchedRegionsList(s); ptr != NULL; ptr = ptr->Next()) {
	    total += ptr->overlappingArea;
	 }
	 graph.setSegmentArea(s, total);
      }
   }

   int segmentCount = 0;
   float Hrs = 0;
   float totalArea = 0;
   long totalOverlap = 0;
   // 2. Compute conditional entropy
   for (int s = 1; s < graph.NumberOfSegments(); s++) {
      float entropy = 0;
      if (graph.getSegmentArea(s) > 0 && graph.isSegmentMatched(s)) {
	 segmentCount++;
	 for (Edge *ptr = graph.getMatchedRegionsList(s); ptr != NULL; ptr = ptr->Next()) {
	    float prs = (float)ptr->overlappingArea / graph.getSegmentArea(s);
	    entropy += prs * log2(prs);
	    totalOverlap += ptr->overlappingArea;
	 }
	 Hrs += graph.getSegmentArea(s) * entropy;
	 totalArea += (float)graph.getSegmentArea(s);
      }
   }
   
   if (segmentCount > 0 && Hrs != 0) {
      Hrs /= (float)-totalOverlap;
   } else {
      Hrs = 0;
   }
   
#ifndef NDEBUG
   float inclusion = Hrs / (1.0 + Hrs);
   printf("* FRAGMENTATION CONSISTENCY\n");
   printf(" error 3/  Under-segmentation..........: %.3f (%.1f region(s) per correct segment in average)\n", fabs(inclusion), exp(Hrs * log(2.0)));
#endif
   return DiscrepancyError(Hrs, 1);
}

inline DiscrepancyError
computeOverSegmentationError( MatchingGraph & graph ) {
   // Compute actual overlapped region areas (do not consider missed pixels) 
   for (int r = 1; r < graph.NumberOfRegions(); r++) {
      long total = 0;
      if (graph.isRegionMatched(r)) { // Overlapping.
	 for (Edge *ptr = graph.getMatchedSegmentsList(r); ptr != NULL; ptr = ptr->Next()) {
	    total += ptr->overlappingArea;
	 }
	 graph.setRegionArea(r, total);
      }
   }
   
   // Compute overlapped region areas
   int regionCount = 0;
   float Hsr = 0;
   float totalArea = 0;
   long totalOverlap = 0;
   
   for (int r = 1; r < graph.NumberOfRegions(); r++) {
      float entropy = 0;
      if (graph.getRegionArea(r) > 0 && graph.isRegionMatched(r)) {
	 regionCount++;
	 for (Edge *ptr = graph.getMatchedSegmentsList(r); ptr != NULL; ptr = ptr->Next()) {
	    float prs = (float)ptr->overlappingArea / graph.getRegionArea(r);
	    entropy += prs * log2(prs);
	    totalOverlap += ptr->overlappingArea;
	 }
	 Hsr += graph.getRegionArea(r) * entropy;
	 totalArea += (float)graph.getRegionArea(r);
      }
   }
   
   if (regionCount > 0 && Hsr != 0) {
      Hsr /= (float)-totalOverlap;
   } else {
      Hsr = 0;
   }
#ifndef NDEBUG
   float fragmentation = Hsr / (1.0 + Hsr);
   printf(" error 4/  Over-segmentation...........: %.3f (%.1f segment(s) per detected region in average)\n", fabs(fragmentation), exp(Hsr * log(2.0)));
#endif
   
   return DiscrepancyError(Hsr, 1);
}

inline DiscrepancyIndicator
FragmentationConsistencyMeasurement( const LabelImage<unsigned int> &, const LabelImage<unsigned int> &, MatchingGraph &grs ) {
   DiscrepancyIndicator indicator;
   indicator.errors[0] = computeUnderSegmentationError(grs);
   indicator.errors[1] = computeOverSegmentationError(grs);
   
   return indicator;
}
