/* -*- c-basic-offset: 3; mode: c++ -*-
 *
 * PANDORE (PANTHEON Project)
 *
 * GREYC IMAGE
 * 6 Boulevard Maréchal Juin
 * F-14050 Caen Cedex France
 *
 * This file is free software. You can use it, distribute it
 * and/or modify it. However, the entire risk to the quality
 * and performance of this program is with you.
 *
 * 
 * For more information, refer to:
 * https://clouard.users.greyc.fr/Pandore
 */

/**
 * @author Régis Clouard - Jul 08, 2013
 */

#include <math.h>
#include <pandore.h>
using namespace pandore;

/**
 * @file pcorrelationcoefficient.cpp
 */

float Correlation( const Char* X, const Char* Y, Long size ) {
   Double mean1 = 0;
   Double mean2 = 0;

   for (int i = 0; i < size; ++i) {
      mean1 += (Double)X[i];
      mean2 += (Double)Y[i];
   }
   mean1 /= (Double)size;
   mean2 /= (Double)size;
   Double covariance = 0;
   Double stddev1 = 0;
   Double stddev2 = 0;

   for (int i = 0; i < size; ++i) {
      covariance = (X[i] - mean1) * (Y[i] - mean2);
      stddev1 += (X[i] - mean1) * (X[i] - mean1);
      stddev2 += (Y[i] - mean2) * (Y[i] - mean2);
   }

   if (stddev1 == 0 || stddev1 == 0) {
      return 1.0f;
   } else {
      return (float)(covariance / (sqrt(stddev1) * sqrt(stddev2)));
   }
}
float Correlation( const Uchar* X, const Uchar* Y, Long size ) {
   Double mean1 = 0;
   Double mean2 = 0;

   for (int i = 0; i < size; ++i) {
      mean1 += (Double)X[i];
      mean2 += (Double)Y[i];
   }
   mean1 /= (Double)size;
   mean2 /= (Double)size;
   Double covariance = 0;
   Double stddev1 = 0;
   Double stddev2 = 0;

   for (int i = 0; i < size; ++i) {
      covariance = (X[i] - mean1) * (Y[i] - mean2);
      stddev1 += (X[i] - mean1) * (X[i] - mean1);
      stddev2 += (Y[i] - mean2) * (Y[i] - mean2);
   }

   if (stddev1 == 0 || stddev1 == 0) {
      return 1.0f;
   } else {
      return (float)(covariance / (sqrt(stddev1) * sqrt(stddev2)));
   }
}
float Correlation( const Short* X, const Short* Y, Long size ) {
   Double mean1 = 0;
   Double mean2 = 0;

   for (int i = 0; i < size; ++i) {
      mean1 += (Double)X[i];
      mean2 += (Double)Y[i];
   }
   mean1 /= (Double)size;
   mean2 /= (Double)size;
   Double covariance = 0;
   Double stddev1 = 0;
   Double stddev2 = 0;

   for (int i = 0; i < size; ++i) {
      covariance = (X[i] - mean1) * (Y[i] - mean2);
      stddev1 += (X[i] - mean1) * (X[i] - mean1);
      stddev2 += (Y[i] - mean2) * (Y[i] - mean2);
   }

   if (stddev1 == 0 || stddev1 == 0) {
      return 1.0f;
   } else {
      return (float)(covariance / (sqrt(stddev1) * sqrt(stddev2)));
   }
}
float Correlation( const Ushort* X, const Ushort* Y, Long size ) {
   Double mean1 = 0;
   Double mean2 = 0;

   for (int i = 0; i < size; ++i) {
      mean1 += (Double)X[i];
      mean2 += (Double)Y[i];
   }
   mean1 /= (Double)size;
   mean2 /= (Double)size;
   Double covariance = 0;
   Double stddev1 = 0;
   Double stddev2 = 0;

   for (int i = 0; i < size; ++i) {
      covariance = (X[i] - mean1) * (Y[i] - mean2);
      stddev1 += (X[i] - mean1) * (X[i] - mean1);
      stddev2 += (Y[i] - mean2) * (Y[i] - mean2);
   }

   if (stddev1 == 0 || stddev1 == 0) {
      return 1.0f;
   } else {
      return (float)(covariance / (sqrt(stddev1) * sqrt(stddev2)));
   }
}
float Correlation( const Long* X, const Long* Y, Long size ) {
   Double mean1 = 0;
   Double mean2 = 0;

   for (int i = 0; i < size; ++i) {
      mean1 += (Double)X[i];
      mean2 += (Double)Y[i];
   }
   mean1 /= (Double)size;
   mean2 /= (Double)size;
   Double covariance = 0;
   Double stddev1 = 0;
   Double stddev2 = 0;

   for (int i = 0; i < size; ++i) {
      covariance = (X[i] - mean1) * (Y[i] - mean2);
      stddev1 += (X[i] - mean1) * (X[i] - mean1);
      stddev2 += (Y[i] - mean2) * (Y[i] - mean2);
   }

   if (stddev1 == 0 || stddev1 == 0) {
      return 1.0f;
   } else {
      return (float)(covariance / (sqrt(stddev1) * sqrt(stddev2)));
   }
}
float Correlation( const Ulong* X, const Ulong* Y, Long size ) {
   Double mean1 = 0;
   Double mean2 = 0;

   for (int i = 0; i < size; ++i) {
      mean1 += (Double)X[i];
      mean2 += (Double)Y[i];
   }
   mean1 /= (Double)size;
   mean2 /= (Double)size;
   Double covariance = 0;
   Double stddev1 = 0;
   Double stddev2 = 0;

   for (int i = 0; i < size; ++i) {
      covariance = (X[i] - mean1) * (Y[i] - mean2);
      stddev1 += (X[i] - mean1) * (X[i] - mean1);
      stddev2 += (Y[i] - mean2) * (Y[i] - mean2);
   }

   if (stddev1 == 0 || stddev1 == 0) {
      return 1.0f;
   } else {
      return (float)(covariance / (sqrt(stddev1) * sqrt(stddev2)));
   }
}
float Correlation( const Llong* X, const Llong* Y, Long size ) {
   Double mean1 = 0;
   Double mean2 = 0;

   for (int i = 0; i < size; ++i) {
      mean1 += (Double)X[i];
      mean2 += (Double)Y[i];
   }
   mean1 /= (Double)size;
   mean2 /= (Double)size;
   Double covariance = 0;
   Double stddev1 = 0;
   Double stddev2 = 0;

   for (int i = 0; i < size; ++i) {
      covariance = (X[i] - mean1) * (Y[i] - mean2);
      stddev1 += (X[i] - mean1) * (X[i] - mean1);
      stddev2 += (Y[i] - mean2) * (Y[i] - mean2);
   }

   if (stddev1 == 0 || stddev1 == 0) {
      return 1.0f;
   } else {
      return (float)(covariance / (sqrt(stddev1) * sqrt(stddev2)));
   }
}
float Correlation( const Ullong* X, const Ullong* Y, Long size ) {
   Double mean1 = 0;
   Double mean2 = 0;

   for (int i = 0; i < size; ++i) {
      mean1 += (Double)X[i];
      mean2 += (Double)Y[i];
   }
   mean1 /= (Double)size;
   mean2 /= (Double)size;
   Double covariance = 0;
   Double stddev1 = 0;
   Double stddev2 = 0;

   for (int i = 0; i < size; ++i) {
      covariance = (X[i] - mean1) * (Y[i] - mean2);
      stddev1 += (X[i] - mean1) * (X[i] - mean1);
      stddev2 += (Y[i] - mean2) * (Y[i] - mean2);
   }

   if (stddev1 == 0 || stddev1 == 0) {
      return 1.0f;
   } else {
      return (float)(covariance / (sqrt(stddev1) * sqrt(stddev2)));
   }
}
float Correlation( const Float* X, const Float* Y, Long size ) {
   Double mean1 = 0;
   Double mean2 = 0;

   for (int i = 0; i < size; ++i) {
      mean1 += (Double)X[i];
      mean2 += (Double)Y[i];
   }
   mean1 /= (Double)size;
   mean2 /= (Double)size;
   Double covariance = 0;
   Double stddev1 = 0;
   Double stddev2 = 0;

   for (int i = 0; i < size; ++i) {
      covariance = (X[i] - mean1) * (Y[i] - mean2);
      stddev1 += (X[i] - mean1) * (X[i] - mean1);
      stddev2 += (Y[i] - mean2) * (Y[i] - mean2);
   }

   if (stddev1 == 0 || stddev1 == 0) {
      return 1.0f;
   } else {
      return (float)(covariance / (sqrt(stddev1) * sqrt(stddev2)));
   }
}
float Correlation( const Double* X, const Double* Y, Long size ) {
   Double mean1 = 0;
   Double mean2 = 0;

   for (int i = 0; i < size; ++i) {
      mean1 += (Double)X[i];
      mean2 += (Double)Y[i];
   }
   mean1 /= (Double)size;
   mean2 /= (Double)size;
   Double covariance = 0;
   Double stddev1 = 0;
   Double stddev2 = 0;

   for (int i = 0; i < size; ++i) {
      covariance = (X[i] - mean1) * (Y[i] - mean2);
      stddev1 += (X[i] - mean1) * (X[i] - mean1);
      stddev2 += (Y[i] - mean2) * (Y[i] - mean2);
   }

   if (stddev1 == 0 || stddev1 == 0) {
      return 1.0f;
   } else {
      return (float)(covariance / (sqrt(stddev1) * sqrt(stddev2)));
   }
}

Errc PCorrelationCoefficient( const Collection &col_in1, const Collection &col_in2, 
			      Collection &col_out, 
			      const std::string &name_in1, const std::string &name_in2, 
			      const std::string &name_out ) {
   std::string type1 = col_in1.GetType(name_in1);
   std::string type2 = col_in2.GetType(name_in2);
   if (type1 != type2) {
      std::cerr << "Error pcorrelationcoefficient: Uncompatible arrays" << std::endl;
      return FAILURE;
   }

   float r=0;
   if (type1 == "Array:Char") {
      Long size1 = col_in1.GETARRAYSIZE(name_in1, Char);
      Long size2 = col_in2.GETARRAYSIZE(name_in2, Char);
      if (size1 != size2) {
	 std::cerr << "Error pcorrelationcoefficient: Uncompatible array sizes" << std::endl;
	 return FAILURE;
      }
      r =  Correlation(col_in1.GETARRAY(name_in1, Char), 
			     col_in2.GETARRAY(name_in2, Char), 
			     size1);
      col_out.SETVALUE(name_out, Char, r);
   } else
   if (type1 == "Array:Uchar") {
      Long size1 = col_in1.GETARRAYSIZE(name_in1, Uchar);
      Long size2 = col_in2.GETARRAYSIZE(name_in2, Uchar);
      if (size1 != size2) {
	 std::cerr << "Error pcorrelationcoefficient: Uncompatible array sizes" << std::endl;
	 return FAILURE;
      }
      r =  Correlation(col_in1.GETARRAY(name_in1, Uchar), 
			     col_in2.GETARRAY(name_in2, Uchar), 
			     size1);
      col_out.SETVALUE(name_out, Uchar, r);
   } else
   if (type1 == "Array:Short") {
      Long size1 = col_in1.GETARRAYSIZE(name_in1, Short);
      Long size2 = col_in2.GETARRAYSIZE(name_in2, Short);
      if (size1 != size2) {
	 std::cerr << "Error pcorrelationcoefficient: Uncompatible array sizes" << std::endl;
	 return FAILURE;
      }
      r =  Correlation(col_in1.GETARRAY(name_in1, Short), 
			     col_in2.GETARRAY(name_in2, Short), 
			     size1);
      col_out.SETVALUE(name_out, Short, r);
   } else
   if (type1 == "Array:Ushort") {
      Long size1 = col_in1.GETARRAYSIZE(name_in1, Ushort);
      Long size2 = col_in2.GETARRAYSIZE(name_in2, Ushort);
      if (size1 != size2) {
	 std::cerr << "Error pcorrelationcoefficient: Uncompatible array sizes" << std::endl;
	 return FAILURE;
      }
      r =  Correlation(col_in1.GETARRAY(name_in1, Ushort), 
			     col_in2.GETARRAY(name_in2, Ushort), 
			     size1);
      col_out.SETVALUE(name_out, Ushort, r);
   } else
   if (type1 == "Array:Long") {
      Long size1 = col_in1.GETARRAYSIZE(name_in1, Long);
      Long size2 = col_in2.GETARRAYSIZE(name_in2, Long);
      if (size1 != size2) {
	 std::cerr << "Error pcorrelationcoefficient: Uncompatible array sizes" << std::endl;
	 return FAILURE;
      }
      r =  Correlation(col_in1.GETARRAY(name_in1, Long), 
			     col_in2.GETARRAY(name_in2, Long), 
			     size1);
      col_out.SETVALUE(name_out, Long, r);
   } else
   if (type1 == "Array:Ulong") {
      Long size1 = col_in1.GETARRAYSIZE(name_in1, Ulong);
      Long size2 = col_in2.GETARRAYSIZE(name_in2, Ulong);
      if (size1 != size2) {
	 std::cerr << "Error pcorrelationcoefficient: Uncompatible array sizes" << std::endl;
	 return FAILURE;
      }
      r =  Correlation(col_in1.GETARRAY(name_in1, Ulong), 
			     col_in2.GETARRAY(name_in2, Ulong), 
			     size1);
      col_out.SETVALUE(name_out, Ulong, r);
   } else
   if (type1 == "Array:Llong") {
      Long size1 = col_in1.GETARRAYSIZE(name_in1, Llong);
      Long size2 = col_in2.GETARRAYSIZE(name_in2, Llong);
      if (size1 != size2) {
	 std::cerr << "Error pcorrelationcoefficient: Uncompatible array sizes" << std::endl;
	 return FAILURE;
      }
      r =  Correlation(col_in1.GETARRAY(name_in1, Llong), 
			     col_in2.GETARRAY(name_in2, Llong), 
			     size1);
      col_out.SETVALUE(name_out, Llong, r);
   } else
   if (type1 == "Array:Ullong") {
      Long size1 = col_in1.GETARRAYSIZE(name_in1, Ullong);
      Long size2 = col_in2.GETARRAYSIZE(name_in2, Ullong);
      if (size1 != size2) {
	 std::cerr << "Error pcorrelationcoefficient: Uncompatible array sizes" << std::endl;
	 return FAILURE;
      }
      r =  Correlation(col_in1.GETARRAY(name_in1, Ullong), 
			     col_in2.GETARRAY(name_in2, Ullong), 
			     size1);
      col_out.SETVALUE(name_out, Ullong, r);
   } else
   if (type1 == "Array:Float") {
      Long size1 = col_in1.GETARRAYSIZE(name_in1, Float);
      Long size2 = col_in2.GETARRAYSIZE(name_in2, Float);
      if (size1 != size2) {
	 std::cerr << "Error pcorrelationcoefficient: Uncompatible array sizes" << std::endl;
	 return FAILURE;
      }
      r =  Correlation(col_in1.GETARRAY(name_in1, Float), 
			     col_in2.GETARRAY(name_in2, Float), 
			     size1);
      col_out.SETVALUE(name_out, Float, r);
   } else
   if (type1 == "Array:Double") {
      Long size1 = col_in1.GETARRAYSIZE(name_in1, Double);
      Long size2 = col_in2.GETARRAYSIZE(name_in2, Double);
      if (size1 != size2) {
	 std::cerr << "Error pcorrelationcoefficient: Uncompatible array sizes" << std::endl;
	 return FAILURE;
      }
      r =  Correlation(col_in1.GETARRAY(name_in1, Double), 
			     col_in2.GETARRAY(name_in2, Double), 
			     size1);
      col_out.SETVALUE(name_out, Double, r);
   } else
      {
	 std::cerr << "Error pcorrelationcoefficient: Invalid collection type" << std::endl;
	 return FAILURE;
      }
   return r;
}


#ifdef MAIN

/*
 * Modify only the following constants, and the function call.
 */
#define	USAGE	"usage: %s name_in1 name_in2 name_out [col_in1|-] [col_in2|-] [im_out|-]"
#define	PARC	3
#define	FINC	2
#define	FOUTC	1
#define	MASK	0

int main( int argc, char *argv[] ) {
   Errc  result;               // The result code of the execution.
   Pobject* mask;              // The region mask.
   Pobject* objin[FINC + 1];   // The input objects;
   Pobject* objs[FINC + 1];    // The source objects masked by the mask.
   Pobject* objout[FOUTC + 1]; // The output object.
   Pobject* objd[FOUTC + 1];   // The result object of the execution.
   char* parv[PARC + 1];       // The input parameters.
   
   ReadArgs(argc, argv, PARC, FINC, FOUTC, &mask, objin, objs, objout, objd, parv, USAGE, MASK);
   
   if (objs[0]->Type() == Po_Collection && objs[1]->Type() == Po_Collection) {
      Collection* ims1 = (Collection*)objs[0];
      Collection* ims2 = (Collection*)objs[1];

      Collection* imd1 = new Collection;
      objd[0] = imd1;

      result = PCorrelationCoefficient(*ims1, *ims2, *imd1, parv[0], parv[1], parv[2]);
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
