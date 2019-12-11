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

#ifndef _MULTIINDICATORRANKING_H
#define _MULTIINDICATORRANKING_H

#include "evaluation.h"

inline double
RankOrderCentroid( int priority, int numberOfIndicators ) {
   float weight = 0.0;
   for (int j = priority; j <= numberOfIndicators; j++) {
      weight += 1.0 / j;
   }
   return weight / numberOfIndicators;
}

inline void
RankAlgorithmsInEachIndicator( std::vector<EvaluationPerformance> &performanceMatrix, EvaluationPerformance *rankedPerformances, int indicator ) {
   float *values = new float[performanceMatrix.size()];
   int size = 0;
   
   for (unsigned int a = 0; a < performanceMatrix.size(); a++) {
      bool exist = false;
      for (int b = 0; b < size; b++) {
	 if (performanceMatrix[a].indicators[indicator] == values[b]) {
	    exist = true;
	    break;
	 }
      }
      if (!exist) {
	 values[size++] = performanceMatrix[a].indicators[indicator];
      }
   }
   
   for (unsigned int a = 0; a < performanceMatrix.size(); a++) {
      int rank = 1;
      for (int b = 0; b < size; b++) {
	 if (performanceMatrix[a].indicators[indicator] > values[b]) {
	    rank++;
	 }
      }
      rankedPerformances[a].indicators[indicator] = (float)rank;
   }
}

inline void
MultiIndicatorRanking( std::vector<EvaluationPerformance> &performanceMatrix, std::vector<long> &ranks,
		       int nbOfIndicators, int propositions[], int priority[] ) {
   EvaluationPerformance *rankedPerformances = new EvaluationPerformance[performanceMatrix.size()];
   for (int i = 0; i < nbOfIndicators; i++) {
      RankAlgorithmsInEachIndicator(performanceMatrix, rankedPerformances, i);
   }
 
   int numberOfRelevantIndicators = 0;
   for (int i = 0; i < nbOfIndicators; i++) {
      if (propositions[i] > 1) {
	 numberOfRelevantIndicators++;
      }
   }
   float *averageRanks = new float[performanceMatrix.size()];
   for (unsigned int a = 0; a < performanceMatrix.size(); a++) {
      double averageRank = 0;
      for (int i = 0; i < nbOfIndicators; i++) {
	 if (propositions[i] > 1) {
	    averageRank += (double)rankedPerformances[a].indicators[i] * RankOrderCentroid(priority[i], numberOfRelevantIndicators);
	 }
      } 
      averageRanks[a] = (float)averageRank;
   }

   for (unsigned int a = 0; a < performanceMatrix.size(); a++) {
      int rank = 1;
      for (unsigned int b = 0; b < performanceMatrix.size(); b++) {
	 if (averageRanks[a] > averageRanks[b]) {
	    rank++;
	 }
      }
      ranks.push_back(rank);
   }
}

#endif
