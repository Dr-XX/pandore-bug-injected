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

#include "./evaluation/rankingfunction.h"
#include "./evaluation/myinputoutput.h"

Errc
PRankSegmentationAlgorithms( Pobject ** colss,
			     int nbOfAlgorithms,
			     Collection & cold1, Collection & cold2,
			     int acceptable_error1, int priority1,
			     int acceptable_error2, int priority2,
			     int acceptable_error3, int priority3,
			     int acceptable_error4, int priority4,
			     int acceptable_error5, int priority5,
			     bool verbose ) {
   setVerbose(verbose);
   const Long nbOfIndicators = 5;
   int propositions[nbOfIndicators] = {
      acceptable_error1,
      acceptable_error2,
      acceptable_error3,
      acceptable_error4,
      acceptable_error5
   };

   int priorities[nbOfIndicators] = {
      priority1,
      priority2,
      priority3,
      priority4,
      priority5
   };

   const std::string errorName = "Error";

   std::vector<EvaluationPerformance> performances;
   for (int i = 0; i < nbOfAlgorithms; i++) {
      Collection *cols = (Collection *)colss[i];
      DiscrepancyResult evaluationResult;

      ReadCollection(*cols,  errorName, nbOfIndicators, evaluationResult);
      performances.push_back(ComputeIndicators(evaluationResult, propositions));
   }
   
   std::vector<long> ranks;
   MultiIndicatorRanking(performances, ranks, nbOfIndicators, propositions, priorities);
   
   LOG(INFO) << std::endl << "    FINAL RESULTS" << std::endl; 
   DisplayIndicators(performances, nbOfIndicators);
   DisplayRanks(ranks);

   BuildOutputResult1(performances, cold1);
   BuildOutputResult2(ranks, cold2);

   return SUCCESS;
}

#ifdef MAIN

/*
 * Modify only the following constants, and the function call.
 */
#define	USAGE	"usage: %s [-v] acceptable_error1 priority1 acceptable_error2 priority2 acceptable_error3 priority3 acceptable_error4 priority4 acceptable_error5 priority5 indicator_collection* rank_collection"

#define	PARC	10
#define	FOUTC	2
#define	MASK	0

int
main( int argc, char *argv[] ) {
   Errc result;			// The result code of the execution.
   Pobject* mask;		// The region mask.
   Pobject** objin = 0;		// The input objects.
   Pobject** objs = 0;		// The source objects masked by the mask.
   Pobject* objout[FOUTC + 1];	// The input objects.
   Pobject* objd[FOUTC + 1];	// The source objects masked by the mask.
   char* parv[PARC + 1];	// The input parameters.
   bool verbose = false;

   if (argc > 1 && !strcmp(argv[1], "-v")) {
      verbose  = true;
      argc --;
      argv[1] = argv[0];
      argv++;
   }

   const int finc = argc - PARC - FOUTC - 1;

   if (finc > 1) {
      objin = new Pobject*[finc + 1];	// The output object.
      objs = new Pobject*[finc + 1];	// The result object of the execution.
   }
   
   ReadArgs(argc, argv, PARC, finc, FOUTC, &mask, objin, objs, objout, objd, parv, USAGE, MASK);

   objd[0] = new Collection;
   Collection* cold1 = (Collection*)objd[0];
   objd[1] = new Collection;
   Collection* cold2 = (Collection*)objd[1];
   
   for (int i = 0; i < finc; i++) {
      if (objs[i]->Type() != Po_Collection) {
   	 goto end;
      }
   }

   result = PRankSegmentationAlgorithms(objs, finc, 
					*cold1, *cold2, 
					atoi(parv[0]), atoi(parv[1]),
					atoi(parv[2]), atoi(parv[3]),
					atoi(parv[4]), atoi(parv[5]),
					atoi(parv[6]), atoi(parv[7]),
					atoi(parv[8]), atoi(parv[9]),
					verbose);
   
   if (result) {
      WriteArgs(argc, argv, PARC, finc, FOUTC, &mask, objin, objs, objout, objd, MASK);
   }
  end:
   if (finc > 0) {
      delete[] objs;
      delete[] objin;      
   }
   Exit(result);
   return 0;
}

#endif

