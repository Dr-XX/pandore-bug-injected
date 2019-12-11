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

#ifndef _RANKINGFUNCTION_H
#define _RANKINGFUNCTION_H

#include "log.h"
#include "fileutils.h"
#include "evaluation.h"
#include "displayperformancevalues.h"
#include "computediscrepancyresults.h"
#include "computeindicators.h"
#include "computenormalizedaveragediscrepancyresult.h"
#include "multiindicatorranking.h"

inline void
RankAlgorithms( int matching_algorithm_id, float matching_threshold,
		std::string segmentation_result_path, std::string ground_truth_path,
		int *propositions, int *priorities, int nbOfIndicators,
		std::vector<long> &ranks,
		std::vector<EvaluationPerformance> &performances,
		bool verbose ) { 
   setVerbose(verbose);

   if (!endsWith(segmentation_result_path, PATH_SEPARATOR)) {
      segmentation_result_path += PATH_SEPARATOR;
   }
   if (!isRegularDirectory(segmentation_result_path)) {
      std::stringstream message;
      message << "[ERROR: " << errno << " ] Couldn't open folder " << segmentation_result_path << ".";
      throw RuntimeException(message.str());
   }
   
   if (!endsWith(ground_truth_path, PATH_SEPARATOR)) {
      ground_truth_path += PATH_SEPARATOR;
   }
   if (!isRegularDirectory(ground_truth_path)) {
      std::stringstream message;
      message << "[ERROR: " << errno << " ] Couldn't open folder " << ground_truth_path << ".";
      throw RuntimeException(message.str());
   }

   LOG(INFO) << "Segmentation result path: " << segmentation_result_path << std::endl;
   LOG(INFO) << "Reference segmentation path: " << ground_truth_path << std::endl;

   std::vector<std::string> segmentationDirectories = std::vector<std::string>();
   getSubdirectories(segmentation_result_path, segmentationDirectories);
   std::sort(segmentationDirectories.begin(), segmentationDirectories.end());

   std::vector<std::string> algorithmNames;
   for (unsigned int algorithm = 0; algorithm < segmentationDirectories.size(); algorithm++) {
      std::string algorithmName = basename(segmentationDirectories[algorithm]);
      algorithmNames.push_back(algorithmName);
      LOG(INFO) << std::endl << " Algorithm: " << algorithmName << std::endl;

      std::vector<DiscrepancyResult> * discrepancyResultForAllSegmentations = ComputeDiscrepancyResults(matching_algorithm_id, matching_threshold, segmentation_result_path + algorithmName + PATH_SEPARATOR, ground_truth_path);
      if (discrepancyResultForAllSegmentations->size() > 0) {
	 DiscrepancyResult evaluationResult = ComputeNormalizedAverageDiscrepancyResult(*discrepancyResultForAllSegmentations);
	 DisplayErrors(evaluationResult);
	 performances.push_back(ComputeIndicators(evaluationResult, propositions));
      }
      
      delete discrepancyResultForAllSegmentations;
   }
   
   MultiIndicatorRanking(performances, ranks, nbOfIndicators, propositions, priorities);
   
   LOG(INFO) << std::endl << "    FINAL RESULTS" << std::endl; 
   DisplayAlgorithmNames(algorithmNames);
   DisplayIndicators(performances, nbOfIndicators);
   DisplayRanks(ranks);
}

#endif
