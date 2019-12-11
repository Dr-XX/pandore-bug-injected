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

#ifndef _COMPUTEDISCREPANCYRESULTS_H
#define _COMPUTEDISCREPANCYRESULTS_H

#include <iomanip>
#include <algorithm>
#include <sstream>

#include "log.h"
#include "fileutils.h"
#include "matching.h"
#include "detectionaccuracymeasurement.h"
#include "fragmentationconsistencymeasurement.h"
#include "boundaryprecisionmeasurement.h"
#include "shapefidelitymeasurement.h"
#include "topologypreservationmeasurement.h"

#include "myinputoutput.h"

inline int
MedianIndex( float * errors, int size ) {
   float *tmp = new float[size];
   for (int i = 0; i < size; i++ ) {
      tmp[i] = errors[i];
   }
   // tri
   float* first(&tmp[0]);
   float* last(first + size);
   std::sort(first, last);

   float median = tmp[(size - 1) / 2];
   int index = 0;

   for (int i = 0; i < size; i++ ) {
      if (median == errors[i]) {
	 index = i;
	 break;
      }
   }
   delete[] tmp;
   return index;
}

inline DiscrepancyResult
ComputeMedianErrorsAgainstReferences( DiscrepancyResult *groundtruthIndicators, int size ) {
   DiscrepancyResult discrepancyResult;
   float *errors1 = new float[size];
   float *errors2 = new float[size];

   for (int i = 0; i < 5; i++) {
      for (int gt = 0; gt < size; gt++) {
	 float error1;
	 if (groundtruthIndicators[gt].indicators[i].errors[0].normalization == 0) {
	    error1 = 0;
	 } else {
	    error1 = groundtruthIndicators[gt].indicators[i].errors[0].value / groundtruthIndicators[gt].indicators[i].errors[0].normalization;
	 }
	 errors1[gt] = error1;
	 float error2; 
	 if (groundtruthIndicators[gt].indicators[i].errors[1].normalization == 0) {
	    error2 = 0;
	 } else {
	    error2 = groundtruthIndicators[gt].indicators[i].errors[1].value / groundtruthIndicators[gt].indicators[i].errors[1].normalization;
	 }
	 errors2[gt] = error2;
      }
      discrepancyResult.indicators[i].errors[0] = groundtruthIndicators[MedianIndex(errors1, size)].indicators[i].errors[0];
      discrepancyResult.indicators[i].errors[1] = groundtruthIndicators[MedianIndex(errors2, size)].indicators[i].errors[1];
   }
   delete[] errors1;
   delete[] errors2;

   return discrepancyResult;
}

inline bool
ComputePerformanceValues( int matching_algorithm_id,
			  float matching_threshold,
			  const std::string & segmentation_image_name,
			  const std::string & ground_truth_image_name,
			  DiscrepancyResult * evaluation) {
    LabelImage<unsigned int> * segmentationImage = LoadLabelImage(segmentation_image_name.c_str());
    if (segmentationImage->VectorSize() == 0) {
	std::cerr << "Error evaluatealgorithm: file " << segmentation_image_name.c_str() << " is not a regular label image" << std::endl;
	return false;
    }
    LabelImage<unsigned int> * groundtruthImage = LoadLabelImage(ground_truth_image_name.c_str());
    if (groundtruthImage->VectorSize() == 0) {
	std::cerr << "Error evaluatealgorithm: file " << ground_truth_image_name.c_str() << " is not a regular label image" << std::endl;
	return false;
    }
    if (segmentationImage->VectorSize() != groundtruthImage->VectorSize() ) {
	throw RuntimeException("Error evaluatealgorithm: Incompatible size between the two region maps.");
    }
    
    MatchingGraph *grs;
    try {
	grs = RegionGraphMatching(*segmentationImage, *groundtruthImage, matching_algorithm_id, matching_threshold);
    } catch (RuntimeException& caught) {
	std::cerr << "Error evaluatealgorithm: " << caught.what() << std::endl;
	return false;
    }

    evaluation->indicators[0] = DetectionAccuracyMeasurement(*segmentationImage, *groundtruthImage, *grs);

    evaluation->indicators[2] = BoundaryPrecisionMeasurement(*segmentationImage, *groundtruthImage, *grs);

    evaluation->indicators[3] = ShapeFidelityMeasurement(*segmentationImage, *groundtruthImage, *grs);

    evaluation->indicators[4] = TopologyPreservationMeasurement(*segmentationImage, *groundtruthImage, *grs);

    // Last one because it modifies the graph.
    evaluation->indicators[1] = FragmentationConsistencyMeasurement(*segmentationImage, *groundtruthImage, *grs);

    LOG(INFO) << "  " << std::fixed << std::setprecision(2);
    for (int i = 0; i < 5; ++i) {
	LOG(INFO) << "(" << evaluation->indicators[i].errors[0].value << " / "<< evaluation->indicators[i].errors[0].normalization
		  << ", " << evaluation->indicators[i].errors[1].value << " / "<< evaluation->indicators[i].errors[1].normalization << ") ";
    }
    LOG(INFO) << std::endl;
    
    delete grs;
    delete segmentationImage;
    delete groundtruthImage;
    
    return true;
}

inline std::vector<DiscrepancyResult> *
ComputeDiscrepancyResults( int matching_algorithm_id, float matching_threshold, std::string segmentation_result_path, std::string ground_truth_path ) {
   std::vector<std::string> files = std::vector<std::string>();
   if (!getAllFilesWithExtension(segmentation_result_path, ".pan", files) || files.size() == 0) {
      std::stringstream message;
      message << "No segmentation result found in directory: " << segmentation_result_path;
      throw RuntimeException(message.str());
   }

   std::vector<DiscrepancyResult> * discrepancyResultForAllSegmentations = new std::vector<DiscrepancyResult>();

   std::vector<std::string> directories = std::vector<std::string>();
   getSubdirectories(ground_truth_path, directories);
   if (directories.size() == 0) {
      std::cerr << "Error: No ground truth found in directory: " << ground_truth_path << std::endl;
      return discrepancyResultForAllSegmentations;
   }

   std::sort(files.begin(), files.end());
   std::sort(directories.begin(), directories.end());
   DiscrepancyResult * discrepancyResultForOneSegmentation = new DiscrepancyResult[directories.size()];

   for (unsigned int segmentation = 0; segmentation < files.size(); segmentation++) {
      std::string directory = dirname(files[segmentation]);
      std::string subdirectory = getSubdirectoryName(directory.c_str(), segmentation_result_path.c_str());
      std::string imagename = basename(files[segmentation]); 
      std::string segmentationImageName = files[segmentation];

      int nbOfExperts = 0;
      for (unsigned int expert = 0; expert < directories.size(); expert++) {
	 std::string expertDirectory = getSubdirectoryName(directories[expert].c_str(), ground_truth_path.c_str());
	 std::string referenceImageName = ground_truth_path + expertDirectory + PATH_SEPARATOR + subdirectory + PATH_SEPARATOR + imagename;
	 if (isFile(segmentationImageName) && isFile(referenceImageName)) {
	    LOG(INFO) << std::endl << "Segmentation result: " << segmentationImageName << std::endl;
	    LOG(INFO) << "Ground truth image:  " << referenceImageName << std::endl;
	    if (ComputePerformanceValues(matching_algorithm_id, matching_threshold,
					 segmentationImageName, referenceImageName,
					 &discrepancyResultForOneSegmentation[nbOfExperts])) {
	       nbOfExperts++;
	    }
	 }
      }
      if (nbOfExperts > 0) {
	 discrepancyResultForAllSegmentations->push_back(ComputeMedianErrorsAgainstReferences(discrepancyResultForOneSegmentation, nbOfExperts));
      }
   }
   return discrepancyResultForAllSegmentations;
}

#endif
