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


#include <iomanip>
#include <math.h>
#include "evaluation/log.h"

#include <pandore.h>
using namespace pandore;

#if (defined _MSC_VER) && (!defined snprintf)
#define snprintf _snprintf
#endif

inline Float
getSum( Float const * const * errors, int vectorSize, int criteria, int error ) {
   Float sum = 0;
   int errorNumber = (criteria - 1) * 2 + error + 1;
   for (int i = 0; i < vectorSize; ++i) {
      sum += errors[errorNumber-1][i];
   }
   return sum;
}

inline Errc
DisplayPerformanceValues( const Collection &cols1, const Collection &cols2, const bool indicatorFlags[] ) {
   Long numberOfVectors;
   std::string type1, type2;
   Long vectorSize;

   const std::string nameError = "Error";
   const std::string nameNormalization = "Normalization";

   if (!cols1.NbOf(nameError, type1, numberOfVectors, vectorSize)) {
      std::cerr << "Error displayperformancevalues: No evaluation result found in parameter 1." << std::endl;
      return FAILURE;
   }

   if (type1 != "Array:Float") {
      std::cerr << "Error displayperformancevalues: Invalid collection parameter 1" << std::endl;
      return FAILURE;
   }
   
   if (!cols1.NbOf(nameNormalization, type2, numberOfVectors, vectorSize)) {
      std::cerr << "Error displayperformancevalues: No evaluation result found in parameter 1." << std::endl;
      return FAILURE;
   }
   
   if (type2 != "Array:Float") {
      std::cerr << "Error displayperformancevalues: Invalid collection parameter 1" << std::endl;
      return FAILURE;
   }
   Float** errors = cols1.GETNARRAYS(nameError, Float, numberOfVectors, vectorSize);
   Float** normalizations = cols1.GETNARRAYS(nameNormalization, Float, numberOfVectors, vectorSize);

   int criteria;

   LOG(CRITICAL) << std::endl << "Total evaluated images:  "<< vectorSize << std::endl;

   char tmp1[256];
   char tmp2[256];

   // 1. detection accuracy
   if (indicatorFlags[0]) {
      criteria  = 1;
      Float numerator1 = getSum(errors, vectorSize, criteria, 0);
      Float denominator1 = getSum(normalizations, vectorSize, criteria, 0);
      Float numerator2 = getSum(errors, vectorSize, criteria, 1);
      Float denominator2 = getSum(normalizations, vectorSize, criteria, 1);

      snprintf(tmp1, sizeof(tmp1), "%s.%d", nameError.c_str(), 2 * criteria - 1);
      tmp1[sizeof(tmp1) - 1] = 0;
      std::string type1 = cols2.GetType(tmp1);
      snprintf(tmp2, sizeof(tmp2), "%s.%d", nameError.c_str(), 2 * criteria);
      tmp2[sizeof(tmp2) - 1] = 0;
      std::string type2 = cols2.GetType(tmp2);
      
      if (type1 != "Float" || type2 != "Float") {
	 std::cerr << "Error: Invalid collection parameter 2" << std::endl;
	 return FAILURE;
      }
      
      Float error1 = cols2.GETVALUE(tmp1, Float);
      Float error2 = cols2.GETVALUE(tmp2, Float);

      LOG(CRITICAL) << std::endl << "+------------------------------------+" << std::endl;
      LOG(CRITICAL) << "|        DETECTION ACCURACY          |" << std::endl;
      LOG(CRITICAL) << "+------------------------------------+" << std::endl;
      LOG(CRITICAL) << "| Recall Error              | "<< std::fixed << std::setprecision(3) << error1 << "  | " << std::endl;
      LOG(CRITICAL) << "| Precision Error           | "<< std::fixed << std::setprecision(3) << error2 << "  | " << std::endl;
      LOG(CRITICAL) << "+------------------------------------+" << std::endl;
      LOG(CRITICAL) << " " << std::setprecision(0) << std::fixed << error1 * 100.0f << "% of misdetected region(s)." << std::endl;
      LOG(CRITICAL) << " " << numerator1 << " missed region(s) / "<< denominator1 << " existing region(s)." << std::endl;      
      LOG(CRITICAL) << " " << std::setprecision(0) << std::fixed << error2 * 100.0f << "% of spurious segment(s)." << std::endl;
      LOG(CRITICAL) << " "<< numerator2 << " spurious segment(s) / "<< denominator2 << " detected segment(s)." << std::endl;      
   }
   
   // 2. fragmentation consistency
   if (indicatorFlags[1]) {
      criteria = 2;
      snprintf(tmp1, sizeof(tmp1), "%s.%d", nameError.c_str(), 2 * criteria - 1);
      tmp1[sizeof(tmp1) - 1] = 0;
      std::string type1 = cols2.GetType(tmp1);
      snprintf(tmp2, sizeof(tmp2), "%s.%d", nameError.c_str(), 2 * criteria);
      tmp2[sizeof(tmp2) - 1] = 0;
      std::string type2 = cols2.GetType(tmp2);
      
      if (type1 != "Float" || type2 != "Float") {
	 std::cerr << "Error: Invalid collection parameter 2" << std::endl;
	 return FAILURE;
      }
      
      Float error1 = cols2.GETVALUE(tmp1, Float);
      Float error2 = cols2.GETVALUE(tmp2, Float);

      LOG(CRITICAL) << std::endl << "+------------------------------------+" << std::endl;
      LOG(CRITICAL) << "|     FRAGMENTATION CONSISTENCY      |" << std::endl;
      LOG(CRITICAL) << "+------------------------------------+" << std::endl;
      Float fragmentation = pow(2, error1 / (1.0f - error1));
      LOG(CRITICAL) << "| Under-Segmentation Error  | "<< std::fixed << std::setprecision(3) << error1 << "  | " << std::endl;
      Float inclusion = pow(2, error2 / (1.0f - error2));
      LOG(CRITICAL) << "| Over-Segmentation Error   | "<< std::fixed << std::setprecision(3) << error2 << "  | " << std::endl;
      LOG(CRITICAL) << "+------------------------------------+" << std::endl;
      LOG(CRITICAL) << " " << std::setprecision(3) << std::fixed << fragmentation << " region(s) per correct segment." << std::endl;
      LOG(CRITICAL) << " " << std::setprecision(3) << std::fixed << inclusion << " segment(s) per correctly detected region." << std::endl;
   }

   // 3. boundary localization
   if (indicatorFlags[2]) {
   criteria = 3;
      snprintf(tmp1, sizeof(tmp1), "%s.%d", nameError.c_str(), 2 * criteria - 1);
      tmp1[sizeof(tmp1) - 1] = 0;
      std::string type1 = cols2.GetType(tmp1);
      snprintf(tmp2, sizeof(tmp2), "%s.%d", nameError.c_str(), 2 * criteria);
      tmp2[sizeof(tmp2) - 1] = 0;
      std::string type2 = cols2.GetType(tmp2);
      
      if (type1 != "Float" || type2 != "Float") {
	 std::cerr << "Error: Invalid collection parameter 2" << std::endl;
	 return FAILURE;
      }
      
      Float error1 = cols2.GETVALUE(tmp1, Float);
      Float error2 = cols2.GETVALUE(tmp2, Float);

      LOG(CRITICAL) << std::endl << "+------------------------------------+" << std::endl;
      LOG(CRITICAL) << "|       BOUNDARY LOCALIZATION        |" << std::endl;
      LOG(CRITICAL) << "+------------------------------------+" << std::endl;
      LOG(CRITICAL) << "| Deficit Error             | " << std::fixed<< std::setprecision(3) << error1 << "  | " << std::endl;
      LOG(CRITICAL) << "| Excess Error              | " << std::fixed << std::setprecision(3) << error2 << "  | " << std::endl;
      LOG(CRITICAL) << "+------------------------------------+" << std::endl;
      LOG(CRITICAL) << " " << std::setprecision(1) << std::fixed << error1 * 100.0f << "% of misdetected area per correctly detected region." << std::endl;
      LOG(CRITICAL) << " " << std::setprecision(1) << std::fixed << error2 * 100.0f << "% of overdetected area per correct segment." << std::endl;
   }

   // 4. shape fidelity
   if (indicatorFlags[3]) {
      criteria  = 4;
      snprintf(tmp1, sizeof(tmp1), "%s.%d", nameError.c_str(), 2 * criteria  - 1);
      tmp1[sizeof(tmp1) - 1] = 0;
      std::string type1 = cols2.GetType(tmp1);
      snprintf(tmp2, sizeof(tmp2), "%s.%d", nameError.c_str(), 2 * criteria);
      tmp2[sizeof(tmp2) - 1] = 0;
      std::string type2 = cols2.GetType(tmp2);
      
      if (type1 != "Float" || type2 != "Float") {
	 std::cerr << "Error: Invalid collection parameter 2" << std::endl;
	 return FAILURE;
      }
      
      Float error1 = cols2.GETVALUE(tmp1, Float);
      Float error2 = cols2.GETVALUE(tmp2, Float);

      LOG(CRITICAL) << std::endl << "+------------------------------------+" << std::endl;
      LOG(CRITICAL) << "|          SHAPE FIDELITY            |" << std::endl;
      LOG(CRITICAL) << "+------------------------------------+" << std::endl;
      float omission = (float)pow(10, error1 / (1.0f - error1)) - 1.0f;
      LOG(CRITICAL) << "| Shape Omission Error      | " << std::fixed << std::setprecision(3) << error1 << "  | " << std::endl;
      float commission = (float)pow(10, error2 / (1.0f - error2)) - 1.0f;
      LOG(CRITICAL) << "| Shape Commission Error    | " << std::fixed << std::setprecision(3) << error2  << "  | " << std::endl;
      LOG(CRITICAL) << "+------------------------------------+" << std::endl;
      LOG(CRITICAL) << " " << std::setprecision(1) << std::fixed << omission << " pixel(s) of average distance between the omission area boundary and the correct region boundary." << std::endl;
      LOG(CRITICAL) << " " << std::setprecision(1) << std::fixed << commission << " pixel(s) of average distance between the commission area boundary and the correct region boundary." << std::endl;
   }
   
    // 5. topology preservation
   if (indicatorFlags[4]) {
      criteria  = 5;
      Float numerator1 = getSum(errors, vectorSize, criteria, 0);
      Float denominator1 = getSum(normalizations, vectorSize, criteria, 0);
      Float numerator2 = getSum(errors, vectorSize, criteria, 1);
      Float denominator2 = getSum(normalizations, vectorSize, criteria, 1);

      snprintf(tmp1, sizeof(tmp1), "%s.%d", nameError.c_str(), 2 * criteria  - 1);
      tmp1[sizeof(tmp1) - 1] = 0;
      std::string type1 = cols2.GetType(tmp1);
      snprintf(tmp2, sizeof(tmp2), "%s.%d", nameError.c_str(), 2 * criteria);
      tmp2[sizeof(tmp2) - 1] = 0;
      std::string type2 = cols2.GetType(tmp2);

      if (type1 != "Float" || type2 != "Float") {
	 std::cerr << "Error: Invalid collection parameter 2" << std::endl;
	 return FAILURE;
      }
      
      Float error1 = cols2.GETVALUE(tmp1, Float);
      Float error2 = cols2.GETVALUE(tmp2, Float);

      LOG(CRITICAL) << std::endl << "+------------------------------------+" << std::endl;
      LOG(CRITICAL) << "|       TOPOLOGY PRESERVATION        |" << std::endl;
      LOG(CRITICAL) << "+------------------------------------+" << std::endl;
      LOG(CRITICAL) << "| Hole Addition Error       |  " << std::fixed << std::setprecision(3) << std::setw(4) << error1 << "  | " << std::endl;
      LOG(CRITICAL) << "| Hole Deletion Error       |  " << std::fixed << std::setprecision(3) << std::setw(4) << error2 << "  | " << std::endl;
      LOG(CRITICAL) << "+-----------------------------------+" << std::endl;
      LOG(CRITICAL) << " " << std::fixed << numerator1 << " hole(s) added / " << denominator1 << " detected holes." << std::endl;
      LOG(CRITICAL) << " " << std::fixed << numerator2 << " hole(s) missed / " << denominator2 << " existing holes." << std::endl;
   }
   
   return SUCCESS;
}

#ifdef MAIN

/*
 * Modify only the following constants, and the function call.
 */
#define	USAGE	"usage: %s [col_in1|-] [col_in2|-]"
#define	PARC	0
#define	FINC	2
#define	FOUTC	0
#define	MASK	0

int main( int argc, char *argv[] ) {
   Errc  result;		// The result code of the execution.
   Pobject* mask;		// The region mask.
   Pobject* objin[FINC + 1];	// The input objects;
   Pobject* objs[FINC+  1];	// The source objects masked by the mask.
   Pobject* objout[FOUTC + 1];	// The output object.
   Pobject* objd[FOUTC + 1];	// The result object of the execution.
   char* parv[PARC + 1];	// The input parameters.
   
   ReadArgs(argc, argv, PARC, FINC, FOUTC, &mask, objin, objs, objout, objd, parv, USAGE, MASK);
   
   if (objs[0]->Type() == Po_Collection && objs[1]->Type() == Po_Collection) {
      Collection* cols1 = (Collection*)objs[0];
      Collection* cols2 = (Collection*)objs[1];
      const bool indicatorFlags[] = { true, true, true, true, true };   
      result = DisplayPerformanceValues(*cols1, *cols2, indicatorFlags);
   } else {
      PrintErrorFormat(objin, FINC);
      result = FAILURE;
   }
   
   if (result) {
      WriteArgs(argc, argv, PARC, FINC, FOUTC, &mask, objin, objs, objout, objd, MASK);
   }
   Exit(result);
   return 0;
}

#endif
