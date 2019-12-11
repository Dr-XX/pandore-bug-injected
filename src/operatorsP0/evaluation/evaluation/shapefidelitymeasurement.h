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
 * References: Average Hausdorff based on Dubuisson et Jain.
 */

#include <cmath>
#include <cstdlib>
#include "evaluation.h"

/** The predefined array for y-shift for 4-connexity neighbours. */
const int V4y[] = {  0, -1,  0,  1,  0};

/** The predefined array for x-shift for 4-connexity neighbours. */
const int V4x[] = { -1,  0,  1,  0, -1};

inline void
getRiBoundary( const MatchingGraph &graph,
	       const LabelImage<unsigned int> &reference,
	       std::vector<Point> *regionBoundaries ) {
   for (int y = 0; y < reference.Height(); y++) {
      int x = 0;
      int r = reference(y, x);
      regionBoundaries[r].push_back(Point(y, x));
      x = reference.Width() - 1;
      r = reference(y, x);
      regionBoundaries[r].push_back(Point(y, x));
   }
   for (int x = 1; x < reference.Width() - 1 ; x++) {
      int y = 0;
      int r = reference(y, x);
      regionBoundaries[r].push_back(Point(y, x));
      y = reference.Height() - 1;
      r = reference(y, x);
      regionBoundaries[r].push_back(Point(y, x));
   }
   
   for (int y = 1; y < reference.Height() - 1; y++) {
      for (int x = 1; x < reference.Width() - 1; x++) {
	 int r = reference(y, x);
	 if (r > 0 && graph.isRegionMatched(r)) {
	    for (int v = 0; v < 4; v++) {
	       bool isContourPoint = reference(y + V4y[v], x + V4x[v]) != reference(y, x);
	       if (isContourPoint) {
		  regionBoundaries[r].push_back(Point(y, x));
		  break;
	       }
	    }
	 }
      }
   }
}

inline void
getRiInterSjBoundary( const MatchingGraph &graph,
		      const LabelImage<unsigned int> &reference,
		      const LabelImage<unsigned int> &segmentation,
		      std::vector<Point> *regionInterSegmentsBoundaries ) {
   for (int y = 0; y < reference.Height(); y++) {
      for (int x = 0; x < reference.Width(); x++) {
	 int r = reference(y, x);
	 if (r > 0 && graph.isRegionMatched(r)) {
	    int s = segmentation(y, x);
	    if (graph.isRegionMatchedBy(r, s)) {
	       for (int v = 0; v < 4; v++) {
		  bool isContourPoint = (!reference.Hold(y + V4y[v], x + V4x[v])
					 || ((reference(y + V4y[v], x + V4x[v]) != (unsigned int)r)
					     || ((segmentation(y + V4y[v], x + V4x[v]) != (unsigned int)s)
						 && !graph.isRegionMatchedBy(r, segmentation(y + V4y[v], x + V4x[v])))));
		  if (isContourPoint) {
		     regionInterSegmentsBoundaries[r].push_back(Point(y, x));
		     break;
		  }
	       }
	    }
	 }
      }
   }
}

inline DiscrepancyError
computeModifiedHausdorffDistanceBetweenRiAndRiInterSj( const MatchingGraph &graph,
						       const std::vector<Point> *regionBoundaries,
						       const std::vector<Point> *regionInterSegmentBoundaries,
						       const LabelImage<unsigned int> &segmentation ) { 
   const float maxDistance = (float)segmentation.VectorSize() * segmentation.VectorSize();
   float MHD = 0;
   int numberOfTruePositiveRegions = 0;
   
   for (int r = 1; r < graph.NumberOfRegions(); r++) {
      if (graph.isRegionMatched(r)) {
	 int pixelCount11 = 0;
	 float totalDistance11 = 0;
	 // Distance R -> {Si}
	 for (std::vector<Point>::const_iterator itr = regionBoundaries[r].begin();
	      itr != regionBoundaries[r].end();
	      itr++) {
	    Point p((*itr).y, (*itr).x);
 	    const int s = segmentation(p.y, p.x); 
 	    if (!graph.isRegionMatchedBy(r, s)) { // Only external boundary points.
	       float minDistance = maxDistance;
	       for (std::vector<Point>::const_iterator its = regionInterSegmentBoundaries[r].begin();
		    its != regionInterSegmentBoundaries[r].end();
		    its++) {
		  int dx = abs(p.x - (*its).x);
		  int dy = abs(p.y - (*its).y);
		  float distance = (float)dx * dx + (float)dy * dy;
		  if (distance < minDistance) {
		     minDistance = distance;
		  }
		  if (minDistance == 0.0f) {
		     break;
		  }
	       }
	       if (minDistance > 0) {
		  pixelCount11++;
		  totalDistance11 += sqrt(minDistance);
	       }
 	    }
 	 }
	 float MHD1 = (pixelCount11 == 0)? 0 : totalDistance11 / pixelCount11;

	 // Distance S -> {Ri}
	 int pixelCount12 = 0;
	 float totalDistance12 = 0;
	 for (std::vector<Point>::const_iterator its = regionInterSegmentBoundaries[r].begin();
	      its != regionInterSegmentBoundaries[r].end();
	      its++) {
	    Point p((*its).y, (*its).x);
	    const int s = segmentation(p.y, p.x);
 	    if (!graph.isRegionMatchedBy(r, s)) { // Only external boundary points.
	       float minDistance = maxDistance;
	       for (std::vector<Point>::const_iterator itr = regionBoundaries[r].begin();
		    itr != regionBoundaries[r].end();
		    itr++) {
		  int dx = abs(p.x - (*itr).x);
		  int dy = abs(p.y - (*itr).y);
		  float distance = ((float)dx * dx + (float)dy * dy);
		  if (distance < minDistance) {
		     minDistance = distance;
		  }
		  if (minDistance == 0.0f) {
		     break;
		  }
	       }
	       if (minDistance > 0) {
		  pixelCount12++;
		  totalDistance12 += sqrt(minDistance);
	       }
	    }
	 }
	 float MHD2 = (pixelCount12 == 0)? 0.0f : totalDistance12 / pixelCount12;

	 float mhd = std::max(MHD1, MHD2);
	 if (mhd > 0) {
	    numberOfTruePositiveRegions++;
	    MHD += mhd;
	 }
      }
   }

#ifndef NDEBUG
   printf("\n* SHAPE FIDELITY\n");
   float v = 0;
   if (numberOfTruePositiveRegions> 0) {
      v = (MHD / numberOfTruePositiveRegions);
   }
   float dmean1 = (float)(log10(v + 1) / (1.0 + log10(v + 1)));
   printf(" 7/  Shape omission error......: %.3f (%.2f pixels away from the segment boundary in average)\n", v, dmean1);
#endif

   return DiscrepancyError(MHD, (float)numberOfTruePositiveRegions);
}

inline void
getSiBoundary( const MatchingGraph &graph,
	       const LabelImage<unsigned int> &segmentation,
	       std::vector<Point> *segmentBoundaries ) {
   for (int y = 0; y < segmentation.Height(); y++) {
      int x = 0;
      int s = segmentation(y, x);
      segmentBoundaries[s].push_back(Point(y, x));
      x = segmentation.Width() - 1;
      s = segmentation(y, x);
      segmentBoundaries[s].push_back(Point(y, x));
   }
   for (int x = 1; x < segmentation.Width() - 1 ; x++) {
      int y = 0;
      int s = segmentation(y, x);
      segmentBoundaries[s].push_back(Point(y, x));
      y = segmentation.Height() - 1;
      s = segmentation(y, x);
      segmentBoundaries[s].push_back(Point(y, x));
   }

   for (int y = 1; y < segmentation.Height() - 1; y++) {
      for (int x = 1; x < segmentation.Width() - 1; x++) {
	 int s = segmentation(y, x);
	 if (s > 0 &&  graph.isSegmentMatched(s)) {
	    for (int v = 0; v < 4; v++) {
	       bool isContourPoint = segmentation(y + V4y[v], x + V4x[v]) != segmentation(y, x);
	       if (isContourPoint) {
		  segmentBoundaries[s].push_back(Point(y, x));
		  break;
	       }
	    }
	 }
      }
   }
}

inline void
getSiInterRjBoundary( const MatchingGraph &graph,
		      const LabelImage<unsigned int> &segmentation,
		      const LabelImage<unsigned int> &reference,
		      std::vector<Point> *segmentInterRegionBoundaries ) {
   for (int y = 0; y < segmentation.Height(); y++) {
      for (int x = 0; x < segmentation.Width(); x++) {
	 int s = segmentation(y, x);
	 if (s>0 && graph.isSegmentMatched(s)) {
	    int r = reference(y, x);
	    if (graph.isSegmentMatchedBy(s, r)) {
	       for (int v = 0; v < 4; v++) {
		  bool isContourPoint = (!segmentation.Hold(y + V4y[v], x + V4x[v])
					 || ((segmentation(y + V4y[v], x + V4x[v]) != (unsigned int)s)
					     || ((reference(y + V4y[v], x + V4x[v]) != (unsigned int)r)
						 && !graph.isSegmentMatchedBy(s, reference(y + V4y[v], x + V4x[v])))));
		  if (isContourPoint) {
		     segmentInterRegionBoundaries[s].push_back(Point(y,x));
		     break;
		  }
	       }
	    }
	 }
      }
   }
}

inline DiscrepancyError
computeModifiedHausdorffDistanceBetweenSiAndSiInterRj( const MatchingGraph &graph,
						       const std::vector<Point> *segmentBoundaries,
						       const std::vector<Point> *segmentInterRegionBoundaries,
						       const LabelImage<unsigned int> &reference ) {
   const float maxDistance = (float)reference.VectorSize() * reference.VectorSize();
   float MHD = 0;
   int numberOfTruePositiveSegments = 0;
   
   for (int s = 1; s < graph.NumberOfSegments(); s++) {
      if (graph.isSegmentMatched(s)) {
	 int pixelCount21 = 0;
	 float totalDistance21 = 0;
	 for (std::vector<Point>::const_iterator its = segmentBoundaries[s].begin();
	      its != segmentBoundaries[s].end();
	      its++) {
	    Point p((*its).y, (*its).x);
 	    const int r = reference(p.y, p.x);
 	    if (!graph.isSegmentMatchedBy(s, r)) { // Only external shape
	       float minDistance = maxDistance;
	       for (std::vector<Point>::const_iterator itr = segmentInterRegionBoundaries[s].begin();
		    itr != segmentInterRegionBoundaries[s].end();
		    itr++) {
		  int dx = abs(p.x - (*itr).x);
		  int dy = abs(p.y - (*itr).y);
		  float distance = ((float)dx * dx + (float)dy * dy);
		  if (distance < minDistance) {
		     minDistance = distance;
		  }
		  if (minDistance == 0.0f) {
		     break;
		  }
	       }
	       if (minDistance > 0) {
		  pixelCount21++;
		  totalDistance21 += sqrt(minDistance);
	       }
	    }
	 }
	 float MHD1 = (pixelCount21 == 0)? 0 : totalDistance21 / pixelCount21;
    
	 int pixelCount22 = 0;
	 float totalDistance22 = 0;
	 for (std::vector<Point>::const_iterator itr = segmentInterRegionBoundaries[s].begin();
	      itr != segmentInterRegionBoundaries[s].end();
	      itr++) {
	    Point p((*itr).y, (*itr).x);
 	    const int r = reference(p.y, p.x);
 	    if (!graph.isSegmentMatchedBy(s, r)) { // Only external shape
	       float minDistance = maxDistance;
	       for (std::vector<Point>::const_iterator its = segmentBoundaries[s].begin();
		    its != segmentBoundaries[s].end();
		    its++) {
		  int dx = abs(p.x - (*its).x);
		  int dy = abs(p.y - (*its).y);
		  float distance = ((float)dx * dx + (float)dy * dy);
		  if (distance < minDistance) {
		  minDistance = distance;
		  }
		  if (minDistance == 0.0f) {
		     break;
		  }
	       }
	       if (minDistance > 0) {
	       pixelCount22++;
	       totalDistance22 += sqrt(minDistance);
	       }
	    }
	 }
	 float MHD2 =  (pixelCount22 == 0)? 0.0f : totalDistance22 / pixelCount22;

	 float mhd = std::max(MHD1, MHD2);
	 if (mhd > 0) {
	    numberOfTruePositiveSegments++;
	    MHD += mhd;
	 }
      }
   }

#ifndef NDEBUG
   float v = 0;
   if (numberOfTruePositiveSegments > 0) {
      v = (MHD / numberOfTruePositiveSegments);
   }
   float dmean1 = (float)(log10(v + 1)/(1.0 + log10(v + 1)));
   printf(" 8/  Shape commission error....: %.3f (%.2f pixels away from the region boundary in average)\n", v, dmean1);
#endif
   return DiscrepancyError(MHD, (float)numberOfTruePositiveSegments);
}

inline DiscrepancyIndicator
ShapeFidelityMeasurement( const LabelImage<unsigned int> &segmentation, const LabelImage<unsigned int> &reference, MatchingGraph &graph ) {
   const int numberOfSegments = segmentation.Labels() + 1;
   const int numberOfRegions = reference.Labels() + 1;
   
   std::vector<Point> *regionBoundaries = new std::vector<Point>[numberOfRegions];
   getRiBoundary(graph, reference, regionBoundaries);

   std::vector<Point> *regionInterSegmentBoundaries = new std::vector<Point>[numberOfRegions];
   getRiInterSjBoundary(graph, reference, segmentation, regionInterSegmentBoundaries);

   DiscrepancyIndicator indicator;
   indicator.errors[0] = computeModifiedHausdorffDistanceBetweenRiAndRiInterSj(graph,
									       regionBoundaries,
									       regionInterSegmentBoundaries,
									       segmentation);
   delete[] regionBoundaries;
   delete[] regionInterSegmentBoundaries;

   std::vector<Point> *segmentBoundaries = new std::vector<Point>[numberOfSegments];
   getSiBoundary(graph, segmentation, segmentBoundaries); 

   std::vector<Point> *segmentInterRegionBoundaries = new std::vector<Point>[numberOfSegments];
   getSiInterRjBoundary(graph, segmentation, reference, segmentInterRegionBoundaries);

   indicator.errors[1] = computeModifiedHausdorffDistanceBetweenSiAndSiInterRj(graph,
									       segmentBoundaries,
									       segmentInterRegionBoundaries,
									       reference);   
   delete[] segmentBoundaries;
   delete[] segmentInterRegionBoundaries;

   return indicator;
}
