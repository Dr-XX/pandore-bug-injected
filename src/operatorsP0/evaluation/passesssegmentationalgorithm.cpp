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

#define NOMINMAX
#include "./evaluation/evaluationfunction.h"
#include "./evaluation/myinputoutput.h"

Errc
PAssessSegmentationAlgorithm( int matching_algorithm_id, float matching_threshold, 
			      std::string segmentation_result_path, 
			      std::string ground_truth_path, 
			      Collection & cold1, 
			      Collection & cold2,
			      bool verbose ) {
   std::vector<DiscrepancyResult> *discrepancyResultForAllSegmentations;
   DiscrepancyResult totalDiscrepancyResult;
   
   try {
      AssessAlgorithm(matching_algorithm_id, matching_threshold,
		      segmentation_result_path, ground_truth_path,
		      discrepancyResultForAllSegmentations,
		      totalDiscrepancyResult, verbose);
      std::string names[] = { "Error", "Normalization" };
      SaveDiscrepancyResults(names, *discrepancyResultForAllSegmentations, cold1);
      const std::string errorName = "Error";
      SaveEvaluationResult(totalDiscrepancyResult, cold2, errorName); 
      return SUCCESS;
   } catch (RuntimeException& caught)  {
      std::cerr << "Error passesssegmentationalgorithm: " << caught.what() << std::endl;
      return FAILURE;
   }
}

#ifdef MAIN

/*
 * Modify only the following constants, and the function call.
 */
#define	USAGE	"usage: %s [-v] matching_algorithm_id matching_threshold segmentation_result_path reference_segmentation_path output_collection"

#define	PARC	4
#define	FINC	0
#define	FOUTC	2
#define	MASK	0

int
main( int argc, char *argv[] ) {
   Errc result;                 // The result code of the execution.
   Pobject* mask;               // The region mask.
   Pobject* objin[FINC + 1];    // The input objects;
   Pobject* objs[FINC + 1];     // The source objects masked by the mask.
   Pobject* objout[FOUTC + 1];  // The output object.
   Pobject* objd[FOUTC + 1];    // The result object of the execution.
   char* parv[PARC + 1];        // The input parameters.
   bool verbose = false;

   if (argc > 1 && !strcmp(argv[1], "-v")) {
      verbose = true;
      argc--;
      argv[1] = argv[0];
      argv++;
   }
   
   ReadArgs(argc, argv, PARC, FINC, FOUTC, &mask, objin, objs, objout, objd, parv, USAGE, MASK);
   
   objd[0] = new Collection;
   Collection* cold1 = (Collection*)objd[0];
   objd[1] = new Collection;
   Collection* cold2 = (Collection*)objd[1];

   result = PAssessSegmentationAlgorithm(atoi(parv[0]), (float)atof(parv[1]), std::string(parv[2]), std::string(parv[3]), *cold1, *cold2, verbose);
   
   if (result) {
      WriteArgs(argc, argv, PARC, FINC, FOUTC, &mask, objin, objs, objout, objd, MASK);
   }
   Exit(result);
   return 0;
}

#endif
