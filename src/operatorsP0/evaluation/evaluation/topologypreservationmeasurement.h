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
 * Hole addition / deletion.
 * Recall + Precision
 */

#include "evaluation.h"
#include <map>

#define NOTHOLE -2
#define UNVISITED -1

const int V8y[] = {  0, -1, -1, -1,  0,  1,  1,  1,  0};
const int V8x[] = { -1, -1,  0,  1,  1,  1,  0, -1, -1};

unsigned long MergeLabels( unsigned long *color, unsigned long c1, unsigned long c2 ) {
   unsigned long e = c1;
   unsigned long b = c2;
   
   // Get the real label of color c1.
   while (color[e] != e) {
      e = color[e];
   }
   
   // Get the real label of color c2.
   while (color[b] != b) {
      b = color[b];
   }
   
   unsigned long min, max;
   if (e > b) {
      min = b;
      max = e;
   } else {
      min = e;
      max = b;
   }
   
   color[max] = min;   
   color[c1] = color[c2] = min;
   
   return min;
}

unsigned long
PLabeling( const LabelImage<unsigned int> &ims, LabelImage<unsigned int> &rgd ) {
   unsigned long etiquette;
   unsigned long* couleur;
   unsigned long* labels;
   register int	y, x;
   unsigned long nlabelsmax = ims.VectorSize() + 1; // No more label than pixel...
   unsigned long i;

   couleur = new unsigned long[nlabelsmax];

   // Initialization of the label table.
   for (i = 0; i < nlabelsmax; i++) {
      couleur[i] = i;
   }
   etiquette = 0;

   // 1th line.
   if (ims(0, 0) != 0) {
      rgd(0, 0) = ++etiquette;
   } else {
      rgd(0, 0) = 0;
   }
   for (y = 0, x = 1; x < ims.Width(); x++) {
      if (ims(y, x) != 0) {
	 rgd(y, x) = ((ims(y, x) == ims(y, x - 1)) ? etiquette : ++etiquette);
      } else {
	 rgd(y, x) = 0;
      }
   }
   
   for (y = 1; y < ims.Height(); y++) {
      x = 0;
      if (ims(y, x) != 0) {
	 if (ims(y, x) == ims(y - 1, x)) {
	    rgd(y, x) = rgd(y - 1, x);
	 } else {
	    rgd(y, x) = ++etiquette;
	    }
      } else {
	 rgd(y, x) = 0;
      }
      for (x = 1; x < ims.Width(); x++) {
	 rgd(y, x) = 0;
	 if (ims(y, x) == 0) {
	    continue;
	 }
	 if (ims(y, x) == ims(y - 1, x))
	    rgd(y, x) = couleur[rgd(y - 1, x)];
	 if ((x + 1 < ims.Width()) && (ims(y, x) == ims(y - 1, x + 1))) {
	    if (rgd(y, x) == 0) {
	       rgd(y, x) = couleur[rgd(y - 1, x + 1)];
	    } else {
	       rgd(y, x) = MergeLabels(couleur, rgd(y - 1, x + 1), rgd(y, x));
	    }
	 }
	 if (ims(y, x) == ims(y - 1, x - 1)) {
	    if (rgd(y, x) == 0) {
	       rgd(y, x) = couleur[rgd(y - 1, x - 1)];
	    } else {
	       rgd(y, x) = MergeLabels(couleur, rgd(y - 1, x - 1), rgd(y, x));
	    }
	 }
	 if (ims(y, x) == ims(y, x - 1)) {
	    if (rgd(y, x) == 0) {
	       rgd(y, x) = couleur[rgd(y, x - 1)];
	    } else {
	       rgd(y, x) = MergeLabels(couleur, rgd(y, x - 1), rgd(y, x));
	    }
	 }
	 if (rgd(y, x) == 0) {
	    rgd(y, x) = ++etiquette;
	 }
      } 
   }
  
   labels = new unsigned long[etiquette  +1];
   
   unsigned long nbreg = 0;

   // Minimize labels   
   for (i = 1; i <= etiquette; i++) {
      if (couleur[i] != i) {
	 labels[i] = labels[couleur[i]];
      } else {
	 labels[i] = ++nbreg;
      }
   }
   
   // Marking
   for (y = 0; y < ims.Height(); y++) {
      for (x = 0; x < ims.Width(); x++) {
	 if (ims(y, x)) {
	    rgd(y, x) = labels[couleur[rgd(y, x)]];
	 } else {
	    rgd(y, x) = 0;
	 }
      }
   }

   delete[] labels;
   delete[] couleur;

   rgd.Labels(nbreg);
   return nbreg;
}

int
PAnd( const LabelImage<unsigned int> &rgs1, const LabelImage<unsigned int> &rgs2, LabelImage<unsigned int> &rgd ) {
   std::map<double, unsigned long> colors;
   int nbreg = 0;
   const double offset = (double)rgs1.Labels();
   for (int y = 0; y < rgs1.Height(); y++) {
      for (int x = 0; x< rgs1.Width(); x++) { 
	 if (rgs1(y, x) * rgs2(y, x)) {
	    double val = rgs1(y, x) + rgs2(y, x) * offset; 
	    if (!colors[val]) {
	       colors[val] = ++nbreg;
	    }
	    rgd(y, x) = colors[val];
	 } else {
	    rgd(y, x) = 0;
	 }
      }
   }

   rgd.Labels(nbreg);
   return nbreg;
}

inline void
getHoles( const LabelImage<unsigned int> &holeImage, const LabelImage<unsigned int> &rgs, long * const neighbors, const int numberOfLabels ) {
   for (int r = 0; r < (int)numberOfLabels + 1; r++) {
      neighbors[r] = UNVISITED;
   }

   // A hole is a region that has only one neighbor.
   for (int y = 0; y < holeImage.Height(); y++) {
      for (int x = 0; x < holeImage.Width(); x++) {
	 unsigned long label = holeImage(y, x);
	 int segment = neighbors[label];
	 if (label > 0 && segment != NOTHOLE) {
	    for (int v = 0; v < 8; v++) {
	       int x1 = x + V8x[v];
	       int y1 = y + V8y[v];
	       if (holeImage.Hold(y1, x1)) {
		  if (holeImage(y1, x1) == 0) { // Hole boundary.
		     long region = neighbors[label];
		     if (region == UNVISITED) {
			neighbors[label] = rgs(y1, x1);
		     } else if (neighbors[label] != (long)rgs(y1, x1)) {
			neighbors[label] = NOTHOLE;
		     }
		  }
	       } else {
		  neighbors[label] = NOTHOLE;
	       }
	    }
	 }
      }
   }
}

inline void
HoleSelection( const LabelImage<unsigned int> &rgs, LabelImage<unsigned int>& rgd ) {
   LabelImage<unsigned int> mismatchedImage(rgs);
   
   // Build the mask image with the detected regions only.
   for (int y = 0; y < rgs.Height(); y++) {
      for (int x = 0; x < rgs.Width(); x++) {
	 int r = rgs(y, x);
	 if (r == 0 ) {
	    mismatchedImage(y, x) = 255;
	 } else {
	    mismatchedImage(y, x) = 0;
	 }
      }
   }

   LabelImage<unsigned int> holeImage(mismatchedImage);
   unsigned long numberOfLabels = PLabeling(mismatchedImage, holeImage);
   long *neighbors = new long[numberOfLabels + 1];
   getHoles(holeImage, rgs, neighbors, numberOfLabels);
   // Count the number of holes.
   for (int y = 0; y < holeImage.Height(); y++) {
      for (int x = 0; x < holeImage.Width(); x++) {
	 if (neighbors[holeImage(y, x)] > 0) {
	    rgd(y, x) = holeImage(y, x);
	 } else {
	    rgd(y, x) = 0;
	 }
      }
   }
   delete[] neighbors;
   rgd.Labels(holeImage.Labels());
}

inline int
getNumberOfDetectedHoles( const LabelImage<unsigned int> &reference, const LabelImage<unsigned int> &segmentation ) {
   LabelImage<unsigned int> referenceHoles(reference);
   LabelImage<unsigned int> segmentationHoles(segmentation);
   LabelImage<unsigned int> commonHoles(reference);

   HoleSelection(reference, referenceHoles);
   HoleSelection(segmentation, segmentationHoles);
   int numberOfRegions = PAnd(referenceHoles, segmentationHoles, commonHoles);

   return numberOfRegions;
}

/**
 * An added hole is a connected component composed of false negative that are completely inside
 * the related segment.
 */ 
inline DiscrepancyError
computeHoleAdditionError( const LabelImage<unsigned int> &reference, const LabelImage<unsigned int> &segmentation, const MatchingGraph &graph ) {
   LabelImage<unsigned int> mismatchedImage(reference);
   
   // Build the mask image with the detected regions only.
   for (int y = 0; y < reference.Height(); y++) {
      for (int x = 0; x < reference.Width(); x++) {
	 int r = reference(y, x);
	 if (r == 0 || !graph.isRegionMatched(r)) {
	    mismatchedImage(y, x) = 0;
	 } else {
	    int s = segmentation(y, x);
	    if (s == 0) {
	       mismatchedImage(y, x) = 255;
	    } else {
	       mismatchedImage(y, x)= 0;
	    }
	 }
      }
   }
   
   LabelImage<unsigned int> holeImage(mismatchedImage);
   
   const int numberOfLabels = PLabeling(mismatchedImage, holeImage);
   long *neighbors = new long[numberOfLabels + 1];
   
   getHoles(holeImage, segmentation, neighbors, numberOfLabels);
   
   // Count the mean of hole ratio (|hole|/|segment|)
   int holeAdditionRatio = 0;
   for (int r = 0; r < (int)holeImage.Labels() + 1; r++) {
      if (neighbors[r] > 0) {
	 holeAdditionRatio++;
      }
   }
   
   delete[] neighbors;
   
   return DiscrepancyError((float)holeAdditionRatio, 1.0f);
}

/**
 * An deleted hole is a connected component composed of false positive that are completely inside
 * the related region.
 */ 
inline DiscrepancyError
computeHoleDeletionError( const LabelImage<unsigned int> &segmentation, const LabelImage<unsigned int> &reference, const MatchingGraph &graph ) {
   LabelImage<unsigned int> mismatchedImage(segmentation);
   
   // Build the mask image with the detected regions only.
   for (int y = 0; y < segmentation.Height(); y++) {
      for (int x = 0; x < segmentation.Width(); x++) {
	 int s = segmentation(y, x);
	 if (s == 0 || !graph.isSegmentMatched(s)) {
	    mismatchedImage(y, x) = 0;
	 } else {
	    int r = reference(y, x);
	    if (r == 0) {
	       mismatchedImage(y, x)= 255;
	    } else {
	       mismatchedImage(y, x) = 0;
	    }
	 }
      }
   }

   LabelImage<unsigned int> holeImage(mismatchedImage);
   const int numberOfLabels = PLabeling(mismatchedImage, holeImage);
   long *neighbors = new long[numberOfLabels + 1];

   getHoles(holeImage, reference, neighbors, numberOfLabels);

   // Count the mean of hole ratio (|hole|/|segment|)
   int holeAdditionRatio = 0;
   for (int r = 1; r < (int)holeImage.Labels() + 1; r++) {
      if (neighbors[r] > 0) {
	 holeAdditionRatio++;
      }
   }

   delete[] neighbors;

   return DiscrepancyError((float)holeAdditionRatio, 1.0f);
}

inline DiscrepancyIndicator
TopologyPreservationMeasurement( const LabelImage<unsigned int> &segmentation, const LabelImage<unsigned int> &reference, MatchingGraph &grs ) {
    DiscrepancyIndicator indicator;

    // 3a. Count the number of correctly detected holes.
    const int holeCount = getNumberOfDetectedHoles(reference, segmentation);

    // 3b. number of added holes.
    indicator.errors[0] = computeHoleAdditionError(reference, segmentation, grs);
    indicator.errors[0].normalization = holeCount + indicator.errors[0].value;

    // 3c. number of deleted holes.
    indicator.errors[1] = computeHoleDeletionError(segmentation, reference, grs);
    indicator.errors[1].normalization = holeCount + indicator.errors[1].value;

#ifndef NDEBUG
    printf("\n* TOPOLOGY PRESERVATION\n");
    printf(" 9/  Hole addition...............: %.0f hole(s) / %.0f detected holes\n", indicator.errors[0].value, indicator.errors[1].normalization);
    printf(" 10/ Hole deletion...............: %.0f hole(s) / %.0f existing holes\n", indicator.errors[1].value, indicator.errors[0].normalization);
#endif

    return indicator;
}
