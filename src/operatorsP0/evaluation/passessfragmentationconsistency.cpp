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
 * https://clouard.users.greyc.fr/Pandore
 */

#define NOMINMAX
#ifdef MAIN
#undef MAIN
#include "./evaluation/evaluation.h"
#include "./evaluation/matching.h"
#include "./evaluation/computenormalizedaveragediscrepancyresult.h"
#include "./evaluation/fragmentationconsistencymeasurement.h"
#include "./evaluation/myinputoutput.h"
#define MAIN
#else
#include "./evaluation/evaluation.h"
#include "./evaluation/matching.h"
#include "./evaluation/computenormalizedaveragediscrepancyresult.h"
#include "./evaluation/fragmentationconsistencymeasurement.h"
#include "./evaluation/myinputoutput.h"
#endif

Errc
PAssessFragmentationConsistency( const Reg2d &segmentation, const Reg2d &reference, Collection &cold, 
				 const float matching_threshold ) {
   if (matching_threshold > 1.0f || matching_threshold < 0.0f) {
      throw RuntimeException("passessfragmentationconsistency: Bad threshold parameter value");
   }

   LabelImage<unsigned int> segmentationImage(segmentation.Width(), segmentation.Height(), segmentation.Labels(), segmentation.Vector());
   LabelImage<unsigned int> referenceImage(reference.Width(), reference.Height(), reference.Labels(), reference.Vector());
   MatchingGraph *grs = RegionGraphMatching(segmentationImage, referenceImage, 0, matching_threshold);

   DiscrepancyIndicator indicator = FragmentationConsistencyMeasurement(segmentationImage, referenceImage, *grs);
   
   std::string names[] = {
      "UndersegmentationError", 
      "OversegmentationError", 
   };
   
   DiscrepancyResult evaluationResult;
   NormalizeFragmentationConsistencyEvaluationResult(indicator, evaluationResult, 0);
   SaveEvaluationResult(evaluationResult, cold, names);
   
   return SUCCESS;
}

#ifdef MAIN
#define USAGE   "usage: %s matching_threshold [-m mask] [segmentation_result__in|-] [reference_segmentation_in|-] [col_out|-]"
#define PARC    1
#define FINC    2
#define FOUTC   1
#define MASK    1

int
main( int argc, char* argv[] ) {
   Errc result;			// The result code of the execution.
   Pobject *mask;		// The mask.
   Pobject *objin[FINC + 1];	// The input objects;
   Pobject *objs[FINC + 1];	// The source objects masked by mask.
   Pobject *objout[FOUTC + 1];	// The output object unmasked by mask.
   Pobject *objd[FOUTC + 1];	// The result object.
   char *parv[PARC + 1];	// The input parameters.

   ReadArgs(argc, argv, PARC, FINC, FOUTC, &mask, objin, objs, objout, objd, parv, USAGE, MASK);
   try {
      if (objs[0]->Type() == Po_Reg2d
	  && objs[1]->Type() == Po_Reg2d) {

	 Reg2d* const rgs1 = (Reg2d*)objs[0];
	 Reg2d* const rgs2 = (Reg2d*)objs[1];
	 
	 objd[0] = new Collection;
	 Collection* cold = (Collection*)objd[0];
	 
	 result = PAssessFragmentationConsistency(*rgs1, *rgs2, *cold, (float)atof(parv[0]));
      } else {
	 result = FAILURE;
	 PrintErrorFormat(objin, FINC, argv);
      }
   } catch (RuntimeException& e) {
      std::cerr << e.what() << std::endl;
      result = FAILURE;
   }
   
   if (result) {
      WriteArgs(argc, argv, PARC, FINC, FOUTC, &mask, objin, objs, objout, objd, MASK);
   }

   Exit(result);
   return 0;
}
#endif
