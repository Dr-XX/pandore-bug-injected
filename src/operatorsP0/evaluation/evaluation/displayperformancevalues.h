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

#include <iomanip>
#include <math.h>
#include "log.h"

void
DisplayErrors( const DiscrepancyResult &evaluationResult, const bool *indicatorFlags = 0 ) {
   int criteria;
   // 1. detection accuracy 
   if (indicatorFlags == 0 || indicatorFlags[0]) {
      criteria = 0;
      float error1 = evaluationResult.indicators[criteria].errors[0].value;
      float error2 = evaluationResult.indicators[criteria].errors[1].value;

      LOG(CRITICAL) << std::endl << "+-------------------------------------+" << std::endl;
      LOG(CRITICAL) << "|        DETECTION ACCURACY           |" << std::endl;
      LOG(CRITICAL) << "+-------------------------------------+" << std::endl;
      LOG(CRITICAL) << "| Recall Error               | "<< std::fixed << std::setprecision(3) << error1 << "  | " << std::endl;
      LOG(CRITICAL) << "| Precision Error            | "<< std::fixed << std::setprecision(3) << error2 << "  | " << std::endl;
      LOG(CRITICAL) << "+-------------------------------------+" << std::endl;
      LOG(CRITICAL) << " " << std::setprecision(0) << std::fixed << error1 * 100.0f << "% of misdetected region(s)." << std::endl;
      LOG(CRITICAL) << " " << std::setprecision(0) << std::fixed << error2 * 100.0f << "% of spurious segment(s)." << std::endl;
   }
   
   // 2. fragmentation consistency
   if (indicatorFlags == 0 || indicatorFlags[1]) {
      criteria = 1;
      float error1 = evaluationResult.indicators[criteria].errors[0].value;
      float error2 = evaluationResult.indicators[criteria].errors[1].value;

      LOG(CRITICAL) << std::endl << "+-------------------------------------+" << std::endl;
      LOG(CRITICAL) << "|     FRAGMENTATION CONSISTENCY       |" << std::endl;
      LOG(CRITICAL) << "+-------------------------------------+" << std::endl;
      float fragmentation = pow(2, error1 / (1.0f - error1));
      LOG(CRITICAL) << "| Under-Segmentation Error   | "<< std::fixed << std::setprecision(3) << error1 << "  | " << std::endl;
      float inclusion = pow(2, error2 / (1.0f - error2));
      LOG(CRITICAL) << "| Over-Segmentation Error    | "<< std::fixed << std::setprecision(3) << error2 << "  | " << std::endl;
      LOG(CRITICAL) << "+-------------------------------------+" << std::endl;
      LOG(CRITICAL) << " " << std::setprecision(3) << std::fixed << fragmentation << " region(s) per correct segment." << std::endl;
      LOG(CRITICAL) << " " << std::setprecision(3) << std::fixed << inclusion << " segment(s) per correctly detected region." << std::endl;
   }

   // 3. boundary localization
   if (indicatorFlags == 0 || indicatorFlags[2]) {
      criteria = 2;
      float error1 = evaluationResult.indicators[criteria].errors[0].value;
      float error2 = evaluationResult.indicators[criteria].errors[1].value;

      LOG(CRITICAL) << std::endl << "+-------------------------------------+" << std::endl;
      LOG(CRITICAL) << "|       BOUNDARY LOCALIZATION         |" << std::endl;
      LOG(CRITICAL) << "+-------------------------------------+" << std::endl;
      LOG(CRITICAL) << "| Deficit Surface Area Error | " << std::fixed<< std::setprecision(3) << error1 << "  | " << std::endl;
      LOG(CRITICAL) << "| Excess Surface Area Error  | " << std::fixed << std::setprecision(3) << error2 << "  | " << std::endl;
      LOG(CRITICAL) << "+-------------------------------------+" << std::endl;
      LOG(CRITICAL) << " " << std::setprecision(1) << std::fixed << error1 * 100.0f << "% of misdetected area per correctly detected region." << std::endl;
      LOG(CRITICAL) << " " << std::setprecision(1) << std::fixed << error2 * 100.0f << "% of overdetected area per correct segment." << std::endl;
   }

   // 4. shape fidelity
   if (indicatorFlags == 0 || indicatorFlags[3]) {
      criteria = 3;
      float error1 = evaluationResult.indicators[criteria].errors[0].value;
      float error2 = evaluationResult.indicators[criteria].errors[1].value;

      LOG(CRITICAL) << std::endl << "+-------------------------------------+" << std::endl;
      LOG(CRITICAL) << "|          SHAPE FIDELITY             |" << std::endl;
      LOG(CRITICAL) << "+-------------------------------------+" << std::endl;
      float omission = (float)pow(10, error1 / (1.0f - error1)) - 1.0f;
      LOG(CRITICAL) << "| Deficit Surface Shape Error| " << std::fixed << std::setprecision(3) << error1 << "  | " << std::endl;
      float commission = (float)pow(10, error2 / (1.0f - error2)) - 1.0f;
      LOG(CRITICAL) << "| Excess Surface Shape Error | " << std::fixed << std::setprecision(3) << error2  << "  | " << std::endl;
      LOG(CRITICAL) << "+-------------------------------------+" << std::endl;
      LOG(CRITICAL) << " " << std::setprecision(1) << std::fixed << omission << " pixel(s) of average distance between the omission area boundary and the correct region boundary." << std::endl;
      LOG(CRITICAL) << " " << std::setprecision(1) << std::fixed << commission << " pixel(s) of average distance between the commission area boundary and the correct region boundary." << std::endl;
   }
   
    // 5. topology preservation
   if (indicatorFlags == 0 || indicatorFlags[4]) {
      criteria = 4;
      float error1 = evaluationResult.indicators[criteria].errors[0].value;
      float error2 = evaluationResult.indicators[criteria].errors[1].value;

      LOG(CRITICAL) << std::endl << "+-------------------------------------+" << std::endl;
      LOG(CRITICAL) << "|       TOPOLOGY PRESERVATION         |" << std::endl;
      LOG(CRITICAL) << "+-------------------------------------+" << std::endl;
      LOG(CRITICAL) << "| Hole Addition Error       |  " << std::fixed << std::setprecision(3) << std::setw(4) << error1 << "  | " << std::endl;
      LOG(CRITICAL) << "| Hole Deletion Error       |  " << std::fixed << std::setprecision(3) << std::setw(4) << error2 << "  | " << std::endl;
      LOG(CRITICAL) << "+-------------------------------------+" << std::endl;
      LOG(CRITICAL) << " " << std::setprecision(1) << std::fixed << error1 * 100.0f << "% of added holes." << std::endl;
      LOG(CRITICAL) << " " << std::setprecision(1) << std::fixed << error2 * 100.0f << "% of missed holes." << std::endl;
   }
}

void
DisplayAlgorithmNames( const std::vector<std::string> algorithmNames ) {
   LOG(INFO) << std::endl << "  * Algorithm names" << std::endl; 
   LOG(INFO) << "+-------+------------------------------------------+" << std::endl;
   LOG(INFO) << "| Index |           Algorithm name                 |" << std::endl;
   LOG(INFO) << "+-------+------------------------------------------+" << std::endl;
   for (unsigned int i = 0; i < algorithmNames.size(); i++) {
      LOG(INFO) << "| " << std::setw(5) << (i + 1) << " | " << std::setw(40) << algorithmNames[i] << " | " << std::endl;
   }
   LOG(INFO) << "+-------+------------------------------------------+" << std::endl << std::endl;
}

void
DisplayIndicators( const std::vector<EvaluationPerformance> &performances, int nbOfIndicators ) {
   LOG(INFO) << std::endl << "  * Performance indicators" << std::endl; 
   LOG(INFO) << "+-----------+-------------+-------------+-------------+-------------+-------------+" << std::endl;
   LOG(INFO) << "| Algorithm | indicator 1 | indicator 2 | indicator 3 | indicator 4 | indicator 5 |" << std::endl;
   LOG(INFO) << "+-----------+-------------+-------------+-------------+-------------+-------------+" << std::endl;
   
   for (unsigned int element = 0; element < performances.size(); element++) {
      LOG(INFO) << "|" << std::setw(10) << element + 1 << " |" ;
      for (int indicator = 0; indicator < nbOfIndicators; indicator++) {
	 LOG(INFO) << std::fixed << std::setw(12) << std::setprecision(3) << performances[element].indicators[indicator] << " |";
      }
      LOG(INFO) <<std::endl;
   }
   LOG(INFO) << "+-----------+-------------+------------+-------------+--------------+-------------+" << std::endl << std::endl;
}

void
DisplayRanks( const std::vector<long> &ranks ) {
   LOG(INFO) << std::endl << "  * Algorithm ranking" << std::endl; 
   LOG(INFO) << "+-------+-----------+" << std::endl;
   LOG(INFO) << "| Ranks | Algorithm |" << std::endl;
   LOG(INFO) << "+-------+-----------+" << std::endl;
   for (unsigned int r = 0; r < ranks.size(); r++) {
      LOG(INFO) << "| " << std::setw(5) << (r + 1) << " | " << std::setw(9) << ranks[r] << " | " << std::endl;
   }
   LOG(INFO) << "+-------+-----------+" << std::endl;
}
