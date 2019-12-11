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

/**
 * @author Régis Clouard - Dec 20, 2010
 */

#include <math.h>
#include <stdexcept>
#include <pandore.h>
using namespace pandore;

#if defined _MSC_VER
#pragma warning (disable: 4290)
#endif

/**
 * @file pmeanshiftsegmentation.cpp
 *
 * Performs mean shift based segmentation.
 * 3 differents algorithms are used depending on 
 * accuracy desired.
 */

/** Maximum number of iterations to find mode. */
const int MAX_ITERATIONS = 100;

/** Threshold value between between 2 LUV modes. */
const float LUV_THRESHOLD = 0.1f;

/** Approximate value of Mh at a peak or plateau. */
const double EPSILON = 0.01;

/** Cluster search windows near convergence that are a distance */
const double DIST_FACTOR = 0.5;

/**
 * List of Region Adjacency.
 */
class RegionAdjacencyList {
public:

   /**
    * Constructs a list of region adjacency.
    */
   RegionAdjacencyList( ): label(-1), edgeStrength(0), next(0) { }

   /**
    * Inserts a region node into the region adjacency list
    * if it does not already exist there.
    * The list is sorted by the label value.
    *
    * @param region  a node representing a connected region.
    * @return 1 if the entry already exists in the list,
    *         0 otherwise.
    */
   int insert( RegionAdjacencyList *region );

   /** The label of the region.  */
   int label;

   /** The region weight. */
   float edgeStrength;

   /** The next region. */
   RegionAdjacencyList *next;
};

/**
 * The class that perform the meanshift algorithm.
 */
class MeanShift {
public:
   MeanShift( );
   /** Operates on a gray image. */
   MeanShift( const Img2duc &ims );
   /** Operates on a color image. */
   MeanShift( const Imc2duc &ims );
   ~MeanShift( ); 

   /**
    * Segments the current image, and stores the result in
    * the specified region map.
    *
    * @param rgd the resulting region map.
    * @param sigmaS The spatial radius of the mean shift window.
    * @param sigmaR The range radius of the mean shift window.
    * @param minRegionArea The minimum density a region may have in the
    *     resulting segmented image. All regions have
    *     point density < minRegionArea are pruned from the image.
    * @param accuracyLevel the accuracy level.
    * @return the number of detected regions.
    */
   int segment( Reg2d &rgd, int sigmaS, float sigmaR, int minRegionArea, int accuracyLevel ) throw (std::runtime_error);

private:

   /**
    * Creates a custom kernel that can be used to perform
    * the mean shift. The number of subspaces is defined
    * by kp.
    *
    * @param P An array with the dimension of each subspace.
    * @param kp the number of subspaces.
    * @return 1 for success, 0 for failure.
    */
   void defineKernel( int *P, int kp ) throw (std::runtime_error);
   
   /**
    * Defines the lattice from the image data.
    *
    * @param height the height of the lattice.
    * @param width the width of the lattice.
    * @param dimension The dimension of the data points.
    */
   void defineLattice( int width, int height, int dimension ) throw (std::runtime_error);

   /**
    * Converts an RGB vector to LUV.
    *
    * @param rgbVal an unsigned char array containing the RGB vector.
    * @param luvVal a floating point array containing the LUV vector.
    */
   void convertRGBtoLUV( Uchar *rgbVal, float *luvVal );

   /**
    * Performs mean shift filter to the image.
    *                                                  
    * @param sigmaS the spatial radius of the mean shift window.
    * @param sigmaR the range radius of the mean shift window.
    * @param accuracyLevel determines the accuracy level.
    */
   void filter( int sigmaS, float sigmaR, int accuracyLevel ) throw (std::runtime_error);

   /*
    * Performs mean shift filtering on the specified input
    * image using a given kernel.  The result is a
    * filtered image stored in the private data members
    * of the MeanShift class. This an accurate but
    * time expensive algorithm.
    *
    * @param sigmaS spatial bandwidth.
    * @param sigmaR range bandwidth.
    */
   void accurateFilter( float sigmaS, float sigmaR ) throw (std::runtime_error);

   /**
    * Performs mean shift filtering on the specified input
    * image using a user defined kernel. Previous mode
    * information is used to avoid re-applying mean shift
    * on certain data points to improve performance.
    * Maintains high level of accuracy with large speed up.
    *
    * @param sigmaS spatial bandwidth.
    * @param sigmaR range bandwidth.
    */
   void optimizedFilter( float sigmaS, float sigmaR ) throw (std::runtime_error);

   /**
    * Filters the image using previous mode information
    * and window traversals to avoid re-applying mean shift to
    * some data points. Huge speed up but not as accurate
    * as previous filters.
    *
    * @param sigmaS spatial bandwidth
    * @param sigmaR range bandwidth.
    */
   void fastestFilter( float sigmaD, float sigmaR ) throw (std::runtime_error);

   /**
    * Classifies the regions of the mean shift filtered
    * image. Namely, each region uniquely identified by
    * its color and location has been labeled and its
    * area computed via an eight-connected fill.
    */
   void labelRegions( );
  
   /**
    * Performs an eight-connected fill for the specified region
    * using the the specicied region seed and a region label.
    * All pixels belonging to the region specified
    * by regionLoc that have the same integer LUV
    * value specified by LUV_data are classified
    * as one region by labeling each pixel in the
    * image clasification structure using label
    * via an eight-connected fill.
    *
    * @param regionLoc a location in the region map.
    * @param label a non zero label.
    */
   void fillRegions( int regionLoc, int label );

   /**
    * Builds a region adjacency matrix using the region list
    * object.
    */
   void buildRegionAdjacencyMatrix( ) throw (std::runtime_error);

   /**
    * Applies transitive closure to the RAM updating
    * labels, modes and modePointCounts to reflect the new
    * set of merged regions resulting from transitive clo-
    * sure.
    */
   void transitiveClosure( );
  
   /**
    * Prunes the image of spurious regions (regions
    * whose area is less than minRegionArea pixels).
    *
    * @param minRegionArea minimum pixel size of regions.
    */
   void pruneSpuriousRegions( int minRegionArea );

   /**
    * @return true if the range data of the specified data points
    * are within the defined search window (defined by kernel
    * bandwidth bandwidth[1]).
    */
   bool inWindow( int mode1, int mode2 );
  
   /**
    * @return the normalized square distance between two modes.
    */
   float squareDistance( int mode1, int mode2 );
  
   /*
    * The number of bands (typically1 for gray images or 3 for color images).
    */
   int _dimension;
   /** Height of the input image. */
   int	_height;
   /** Width of the input image. */
   int	_width;
   /** Length of the lattice = height* width */
   int _length;

   /**
    * Input data storage as linear storage
    * memory allocated for data points stored by tree nodes
    * when used by the lattice data structure data does not store
    * the lattice information; format of data:
    * data = <x11, x12, ..., x1N,...,xL1, xL2, ..., xLN>
    * in the case of the lattice the i in data(i,j) corresponds
    */
   float *_data;
   
   /** Subspace number. */
   int  _subspaceNumber;

   /** Subspace dimensions. */
   int *_subspaceDimensions;

   /** Bandwidth vector. */
   float *_bandwidths;

   /** The weight of the kernel. */
   float *_weightMap;

   /** The number of connected regions. */
   int	_regionCount;

   /** 8-connected neighbors. */
   int _neigh[8];

   /** Location of the data point on the lattice. */
   float *_msRawData;

   /**
    * Assigns a label to each data point associating it to
    * a mode in modes (e.g. a data point having label l has
    * mode modes[l]).
    */
   int *_labels;

   /** Stores the mode data of the input data set, indexed by labels. */
   float *_modes;

   /**
    * Stores for each mode the number of point mapped to that mode,
    * indexed by labels.
    */
   int *_modePointCounts;
  
   /**
    * An array of RegionAdjacencyList objects.
    */
   RegionAdjacencyList *_raList;
  
   /**
    * A pool of RegionAdjacencyList objects.
    */
   RegionAdjacencyList *_raPool;
};

MeanShift::MeanShift( const Img2duc &ims ): _data(0), _subspaceDimensions(0), _weightMap(0), _regionCount(0), _msRawData(0), _labels(0), _modes(0), _modePointCounts(0), _raList(0), _raPool(0) {
   if (!(_data = new float [ims.VectorSize()*ims.Bands()])) {
      throw std::runtime_error("Not enough memory.");
   }

   Uchar *pixel = ims.Vector();
   for (int i=0; i<(int)ims.VectorSize(); i++) {
      _data[i] = (float)(pixel[i]);
   }
   defineLattice(ims.Width(), ims.Height(), ims.Bands());
}

MeanShift::MeanShift( const Imc2duc &ims ): _data(0), _subspaceDimensions(0), _weightMap(0), _regionCount(0), _msRawData(0), _labels(0), _modes(0), _modePointCounts(0), _raList(0), _raPool(0) {
   if (!(_data = new float [ims.VectorSize()*ims.Bands()])) {
      throw std::runtime_error("Not enough memory.");
   }

   Uchar *red = ims.Vector(0);
   Uchar *green = ims.Vector(1);
   Uchar *blue = ims.Vector(2);
   for (int i = 0; i < (int)ims.VectorSize(); i++) {
      Uchar rgb[3];
      rgb[0] = red[i];
      rgb[1] = green[i];
      rgb[2] = blue[i];
      convertRGBtoLUV(rgb, &_data[ims.Bands()*i]);
   }
   defineLattice(ims.Width(), ims.Height(), ims.Bands());
}

MeanShift::~MeanShift( ) {
   if (_weightMap) delete[] _weightMap;
   if (_bandwidths) delete[] _bandwidths;
   if (_subspaceDimensions) delete[] _subspaceDimensions;
   if (_data) delete[] _data;
   if (_msRawData) delete[] _msRawData;
   if (_modes) delete[] _modes;
   if (_labels) delete[] _labels;
   if (_modePointCounts) delete [] _modePointCounts;
   if (_raList) delete[] _raList;
   if (_raPool) delete[] _raPool;
}

int RegionAdjacencyList::insert(RegionAdjacencyList *region) {   
   if (!next) {
      next = region;
      region->next = 0;
      return 0;
   }

   // if the region's label already exists - do nothing
   if (next->label > region->label) {
      region->next = next;
      next = region;
      return 0;
   }

   // check the rest of the list...
   RegionAdjacencyList *cur = next;
   while (cur) {
      if (region->label == cur->label) {
	 return 1;
      } else if ((!(cur->next))||(cur->next->label > region->label)) {
	 region->next = cur->next;
	 cur->next = region;
	 return 0;
      }
      cur = cur->next;
   }
   return 0;
}

int MeanShift::segment( Reg2d &rgd, int sigmaS, float sigmaR, int minRegionArea, int accuracyLevel ) throw (std::runtime_error) {
   _subspaceNumber = 2;
   if ((! (_subspaceDimensions = new int[_subspaceNumber]))
       || (!(_bandwidths = new float[_subspaceNumber]))) {
      throw std::runtime_error("Not enough memory.");
   }
   _subspaceDimensions[0] = 2;		 // 1st subspace
   _subspaceDimensions[1] = _dimension;  // 2nd subspace
   filter(sigmaS, sigmaR, accuracyLevel);

   transitiveClosure();
   int oldRC = _regionCount;
   int deltaRC, counter = 10;
   do {
      transitiveClosure();
      deltaRC = oldRC-_regionCount;
      oldRC = _regionCount;
      counter--;
   } while ((deltaRC <= 0) && (counter >0));
   pruneSpuriousRegions(minRegionArea);

   Reg2d::ValueType *data = rgd.Vector();
   for (int i = 0; i < _length; i++) {
      data[i] = _labels[i]+1;
   }

   rgd.Labels(_regionCount);
   return _regionCount;
}

void MeanShift::defineLattice( int wt, int ht, int dimension ) throw (std::runtime_error) {
   _height=ht;
   _width=wt;
   _length = _height*_width;
   _dimension = dimension;
 
   if (!(_weightMap = new float [_length])) {
      throw std::runtime_error("Not enough memory.");
   }
   memset(_weightMap, 0, _length*(sizeof(float)));
}

void MeanShift::filter( int sigmaS, float sigmaR, int accuracyLevel ) throw (std::runtime_error) { 
  if ((!(_msRawData = new float [_length*_dimension]))
       || (!(_modes = new float [_length*(_dimension+2)]))
       ||(!(_labels = new int [_length]))
       ||(!(_modePointCounts = new int [_length]))) {
      throw std::runtime_error("Not enough memory.");
   }
   switch(accuracyLevel) {
   case 0: 
      fastestFilter((float)sigmaS, sigmaR);		
      break;
   case 1:	
      optimizedFilter((float)sigmaS, sigmaR);		
      break;
   case 2:
      accurateFilter((float)sigmaS, sigmaR);	
      break;
   }

   for (int i=0; i<_length*_dimension; i++) {
      _data[i] = _msRawData[i];
   }

   labelRegions();
}

/**
 * Converts an RGB vector to LUV.
 *
 * @param rgbVal the RGB vector.
 * @param luvVal the resulting LUV vector.
 */
void MeanShift::convertRGBtoLUV(Uchar *rgbVal, float *luvVal) {
   /** LUV transform constants. */
   const double Yn	= 1.00000;
   const double Un_prime = 0.19784977571475;
   const double Vn_prime = 0.46834507665248;
   const double Lt	= 0.008856;
   /** LUV transform constants. */
   const double RGB2XYZ[3][3] = {{ 0.4125, 0.3576, 0.1804 },
				 { 0.2125, 0.7154, 0.0721 },
				 { 0.0193, 0.1192, 0.9502 }};
   
   double x, y, z;
   // convert RGB to XYZ...
   x = RGB2XYZ[0][0]*rgbVal[0] + RGB2XYZ[0][1]*rgbVal[1] + RGB2XYZ[0][2]*rgbVal[2];
   y = RGB2XYZ[1][0]*rgbVal[0] + RGB2XYZ[1][1]*rgbVal[1] + RGB2XYZ[1][2]*rgbVal[2];
   z = RGB2XYZ[2][0]*rgbVal[0] + RGB2XYZ[2][1]*rgbVal[1] + RGB2XYZ[2][2]*rgbVal[2];
   
   // convert XYZ to LUV...
   double L0, u_prime, v_prime;
   L0 = y / (255.0 * Yn);
   if (L0 > Lt)
      luvVal[0]	= (float)(116.0 * (pow(L0, 1.0/3.0)) - 16.0);
   else
      luvVal[0]	= (float)(903.3 * L0);

   double cst = x + 15 * y + 3 * z;
   if (cst != 0) {
      u_prime = (4 * x) / cst;
      v_prime = (9 * y) / cst;
   } else {
      u_prime = 4.0;
      v_prime = 9.0/15.0;
   }
   
   luvVal[1] = (float) (13 * luvVal[0] * (u_prime - Un_prime));
   luvVal[2] = (float) (13 * luvVal[0] * (v_prime - Vn_prime));
}

void MeanShift::labelRegions( ) {
   _neigh[0] = 1;
   _neigh[1] = 1-_width;
   _neigh[2] = -_width;
   _neigh[3] = -(1+_width);
   _neigh[4] = -1;
   _neigh[5] = _width-1;
   _neigh[6] = _width;
   _neigh[7] = _width+1;

   int i;
   for (i = 0; i < _length; i++) {
      _labels[i] = -1;
      _modePointCounts[i] = 0;
   }
   int k, label = -1;
   for (i = 0; i < _length; i++) {
      if (_labels[i] < 0) {
	 _labels[i] = ++label;
	 for (k = 0; k < _dimension; k++)
	    _modes[(_dimension*label)+k] = _data[(_dimension*i)+k];
	 fillRegions(i, label);
      }
   }
   _regionCount	= label+1;
}

void MeanShift::fillRegions( int regionLoc, int label ) {
   int i, k, neighLoc, neighborsFound;
   int index = 0;
   int *indexTable;

   indexTable = new int[_length];
   indexTable[0] = regionLoc;
   _modePointCounts[label]++;

   while (true) {
      neighborsFound = 0;
      for (i=0; i<8; i++) {
	 neighLoc = regionLoc + _neigh[i];
	 if ((neighLoc >= 0)&&(neighLoc < _length)&&(_labels[neighLoc] < 0)) {
	    for (k = 0; k < _dimension; k++) {
	       if (fabs(_data[(regionLoc*_dimension)+k]
			-_data[(neighLoc*_dimension)+k])>=LUV_THRESHOLD)
		  break;
	    }

	    if (k == _dimension) {
	       _labels[neighLoc] = label;
	       _modePointCounts[label]++;
	       indexTable[++index] = neighLoc;
	       neighborsFound = 1;
	    }
	 }
      }
      if (neighborsFound)
	 regionLoc = indexTable[index];
      else if (index > 1)
	 regionLoc = indexTable[--index];
      else
	 break;
   }
   delete[] indexTable;
}

void MeanShift::buildRegionAdjacencyMatrix( ) throw (std::runtime_error) {
   const int node_multiple=10;

   // This method is called several times
   // -> use the same arrays.
   if ((!_raList)
       && (   !(_raList = new RegionAdjacencyList[_regionCount])
	   || !(_raPool = new RegionAdjacencyList[node_multiple*_regionCount]))) {
      throw std::runtime_error("Not enough memory.");
   }

   for (int r = 0; r < _regionCount; r++) {
      _raList[r].edgeStrength = 0;
      _raList[r].label = r;
      _raList[r].next = 0;
   }

   RegionAdjacencyList *freeRa= _raPool;
   for (int r = 0; r < node_multiple*_regionCount-1; r++) {
      _raPool[r].edgeStrength = 0;
      _raPool[r].next = &_raPool[r+1];
   }
   _raPool[node_multiple*_regionCount-1].next = 0;

   int exists;
   RegionAdjacencyList	*raNode1, *raNode2, *oldRAFreeList;
   int i,j;
   for (i = 0; i < _height - 1; i++) {
      for (j = 0; j < _width - 1; j++) {
	 int curLabel = _labels[i*_width+j];
	 int rightLabel = _labels[i*_width+j+1];
	 int bottomLabel = _labels[(i+1)*_width+j];

	 if (curLabel != rightLabel) {
	    raNode1 = freeRa;
	    raNode2 = freeRa->next;

	    oldRAFreeList = freeRa;

	    freeRa = freeRa->next->next;

	    raNode1->label = curLabel;
	    raNode2->label = rightLabel;

	    exists = 0;
	    _raList[curLabel].insert(raNode2);
	    exists = _raList[rightLabel].insert(raNode1);

	    if (exists)
	       freeRa = oldRAFreeList;
	 }
	 
	 if (curLabel != bottomLabel) {
	    raNode1 = freeRa;
	    raNode2 = freeRa->next;

	    oldRAFreeList = freeRa;

	    freeRa = freeRa->next->next;

	    raNode1->label = curLabel;
	    raNode2->label = bottomLabel;

	    exists = 0;
	    _raList[curLabel].insert(raNode2);
	    exists = _raList[bottomLabel].insert(raNode1);

	    if (exists)
	       freeRa = oldRAFreeList;
	 }
      }

      int curLabel = _labels[i*_width+j];
      int bottomLabel = _labels[(i+1)*_width+j];

      if (curLabel != bottomLabel) {
	 raNode1 = freeRa;
	 raNode2 = freeRa->next;

	 oldRAFreeList = freeRa;
			
	 freeRa = freeRa->next->next;
			
	 raNode1->label	= curLabel;
	 raNode2->label	= bottomLabel;
			
	 exists = 0;
	 _raList[curLabel].insert(raNode2);
	 exists	= _raList[bottomLabel].insert(raNode1);
			
	 if (exists) {
	    freeRa = oldRAFreeList;
	 }
      }
   }
   for (j = 0; j < _width - 1; j++) {
      int curLabel = _labels[i*_width+j];
      int rightLabel = _labels[i*_width+j+1];

      if (curLabel != rightLabel) {
	 raNode1 = freeRa;
	 raNode2 = freeRa->next;

	 oldRAFreeList = freeRa;
			
	 freeRa = freeRa->next->next;
			
	 raNode1->label	= curLabel;
	 raNode2->label	= rightLabel;
			
	 exists = 0;
	 _raList[curLabel].insert(raNode2);
	 exists	= _raList[rightLabel].insert(raNode1);
			
	 if (exists)
	    freeRa = oldRAFreeList;
      }
   }
}

void MeanShift::transitiveClosure( ) {
   int i;
   const float epsilon = 1.0;
   int candidEl, neighCanEl;

   buildRegionAdjacencyMatrix();

   for (i = 0; i < _regionCount; i++) {
      RegionAdjacencyList *neighbor = _raList[i].next;
      
//       float threshold;
//       if (epsilon > _raList[i].edgeStrength)
// 	 threshold = epsilon;
//       else
// 	 threshold = _raList[i].edgeStrength;

      while (neighbor) {
	 if ((inWindow(i, neighbor->label)) && (neighbor->edgeStrength < epsilon)) {
	    candidEl = i;
	    while (_raList[candidEl].label != candidEl)
	       candidEl = _raList[candidEl].label;

	    neighCanEl = neighbor->label;
	    while (_raList[neighCanEl].label != neighCanEl)
	       neighCanEl = _raList[neighCanEl].label;
	       
	    if (candidEl < neighCanEl) {
	       _raList[neighCanEl].label = candidEl;
	    } else {
	       _raList[_raList[candidEl].label].label = neighCanEl;
	       _raList[candidEl].label = neighCanEl;
	    }
	 }
	 neighbor = neighbor->next;
      }
   }
   
   for (i = 0; i < _regionCount; i++) {
      candidEl = i;
      while (_raList[candidEl].label != candidEl)
	 candidEl	= _raList[candidEl].label;
      _raList[i].label = candidEl;
   }

   float *modes_buffer = new float[_dimension*_regionCount];
   int *MPC_buffer = new int[_regionCount];

   for (i = 0; i < _regionCount; i++)
      MPC_buffer[i]	= 0;
   for (i = 0; i < _dimension*_regionCount; i++)
      modes_buffer[i]	= 0;

   int k, iMPC;
   for (i = 0; i < _regionCount; i++) {
      candidEl = _raList[i].label;
      iMPC = _modePointCounts[i];
      for (k = 0; k < _dimension; k++)
	 modes_buffer[(_dimension*candidEl)+k] += iMPC*_modes[(_dimension*i)+k];
      MPC_buffer[candidEl] += iMPC;
   }

   int *label_buffer = new int [_regionCount];
   for (i = 0; i < _regionCount; i++)
      label_buffer[i]	= -1;

   int label = -1;
   for (i = 0; i < _regionCount; i++) {
      candidEl	= _raList[i].label;
      if (label_buffer[candidEl] < 0) {
	 label_buffer[candidEl] = ++label;

	 iMPC	= MPC_buffer[candidEl];
	 for (k = 0; k < _dimension; k++)
	    _modes[(_dimension*label)+k] = (modes_buffer[(_dimension*candidEl)+k])/(iMPC);
	 _modePointCounts[label] = MPC_buffer[candidEl];
      }
   }
   _regionCount	= label+1;
   for (i = 0; i < _length; i++)
      _labels[i] = label_buffer[_raList[_labels[i]].label];

   delete [] modes_buffer;
   delete [] MPC_buffer;
   delete [] label_buffer;
}

void MeanShift::pruneSpuriousRegions( int minRegionArea ) {	
   float *modes_buffer = new float[_dimension*_regionCount];
   int *MPC_buffer = new int[_regionCount];
   int *label_buffer = new int[_regionCount];
   int minRegionCount,i;
   
   do {
      minRegionCount = 0;	     
      buildRegionAdjacencyMatrix();
      for (i = 0; i < _regionCount; i++) {
	 if (_modePointCounts[i] < minRegionArea) {
	    RegionAdjacencyList *neighbor;      
	    minRegionCount++;
	    neighbor = _raList[i].next;
	    int candidate = neighbor->label;
	    double minSqDistance = squareDistance(i, candidate);
	    neighbor = neighbor->next;
	    while (neighbor) {
	       double neighborDistance = squareDistance(i, neighbor->label);
	       if (neighborDistance < minSqDistance) {
		  minSqDistance	= neighborDistance;
		  candidate = neighbor->label;
	       }
	       neighbor	= neighbor->next;
	    }

	    int candidEl = i;
	    while (_raList[candidEl].label != candidEl)
	       candidEl = _raList[candidEl].label;
	    int neighCanEl = candidate;
	    while (_raList[neighCanEl].label != neighCanEl)
	       neighCanEl = _raList[neighCanEl].label;

	    if (candidEl < neighCanEl) {
	       _raList[neighCanEl].label = candidEl;
	    } else {
	       _raList[_raList[candidEl].label].label = neighCanEl;
	       _raList[candidEl].label = neighCanEl;
	    }
	 }
      }

      for (i = 0; i < _regionCount; i++) {
	 int candidEl	= i;
	 while (_raList[candidEl].label != candidEl)
	    candidEl = _raList[candidEl].label;
	 _raList[i].label = candidEl;
      }
		
      for (i = 0; i < _regionCount; i++)
	 MPC_buffer[i]	= 0;

      for (i = 0; i < _dimension*_regionCount; i++)
	 modes_buffer[i] = 0;
		
      for (i = 0; i < _regionCount; i++) {
	 int candidEl = _raList[i].label;
	 int iMPC = _modePointCounts[i];
	 for (int k = 0; k < _dimension; k++)
	    modes_buffer[(_dimension*candidEl)+k] += iMPC*_modes[(_dimension*i)+k];
	 MPC_buffer[candidEl] += iMPC;
      }
		
      for (i = 0; i < _regionCount; i++)
	 label_buffer[i] = -1;

      int label = -1;
      for (i = 0; i < _regionCount; i++) {
	 int candidEl = _raList[i].label;
	 if (label_buffer[candidEl] < 0) {
	    label_buffer[candidEl] = ++label;
	    int iMPC = MPC_buffer[candidEl];
	    for (int k = 0; k < _dimension; k++)
	       _modes[(_dimension*label)+k]=(modes_buffer[(_dimension*candidEl)+k])/(iMPC);
	    _modePointCounts[label] = MPC_buffer[candidEl];
	 }
      }
      _regionCount = label+1;
      for (i = 0; i < _length; i++)
	 _labels[i] = label_buffer[_raList[_labels[i]].label];
   } while (minRegionCount > 0);

   delete [] modes_buffer;
   delete [] MPC_buffer;
   delete [] label_buffer;
}

bool MeanShift::inWindow(int mode1, int mode2) {
   int k = 1, s	= 0;
   double diff = 0;
   
   while ((diff < 0.25) && (k != _subspaceNumber)) {
      diff = 0;
      for (int p=0; p < _subspaceDimensions[k]; p++) {
	 double el  = (_modes[mode1*_dimension+p+s]-_modes[mode2*_dimension+p+s])
	    /(_bandwidths[k]);
	 if ((!p)&&(k == 1)&&(_modes[mode1*_dimension] > 80))
	    diff += 4*el*el;
	 else
	    diff += el*el;
      }
      
      s += _subspaceDimensions[k];
      k++;
   }
   return (bool)(diff < 0.25);
}

float MeanShift::squareDistance(int mode1, int mode2) {
   int s = 0;
   float dist = 0;
   
   for (int k = 1; k < _subspaceNumber; k++) {
      for (int p = 0; p < _subspaceDimensions[k]; p++)	 {
	 float el   = (_modes[mode1*_dimension+p+s]-_modes[mode2*_dimension+p+s])
	    /(_bandwidths[k]);
	 dist += el*el;
      }
      s += _subspaceDimensions[k];
      k++;
   }
   
   return dist;
}

void MeanShift::optimizedFilter( float sigmaS, float sigmaR ) throw (std::runtime_error) {
   unsigned char *modeTable;
   int *pointList;
   int lN = _dimension + 2;
   double *yk, *Mh;
   float* sdata;
   
   if ((! (modeTable = new unsigned char [_length]))
       || (!(pointList = new int [_length]))
       || (!(yk = new double[lN]))
       || (!(Mh = new double[lN]))
       || (!(sdata = new float[lN*_length]))) {
      throw std::runtime_error("Not enough memory.");
   }
   
   _bandwidths[0] = sigmaS;
   _bandwidths[1] = sigmaR;
   
   int idxs = 0, idxd = 0;
   int i,j;
   for (i=0; i<_length; i++) {
      sdata[idxs++] = (i%_width)/sigmaS;
      sdata[idxs++] = (i/_width)/sigmaS;
      for (j=0; j<_dimension; j++)
	 sdata[idxs++] = _data[idxd++]/sigmaR;
   }

   int* buckets;
   int* slist = new int[_length];
   int bucNeigh[27];

   float sMins;
   float sMaxs[3];
   sMaxs[0] = _width/sigmaS;
   sMaxs[1] = _height/sigmaS;
   sMaxs[2] = sMins = sdata[2];
   idxs = 2;
   for (i=0; i<_length; i++) {
      float cval = sdata[idxs];
      if (cval < sMins)
	 sMins = cval;
      else if (cval > sMaxs[2])
	 sMaxs[2] = cval;

      idxs += lN;
   }

   int cBuck1, cBuck2, cBuck3, cBuck;
   int nBuck1 = (int) (sMaxs[0] + 3);
   int nBuck2 = (int) (sMaxs[1] + 3);
   int nBuck3 = (int) (sMaxs[2] - sMins + 3);
   buckets = new int[nBuck1*nBuck2*nBuck3];
   for (i=0; i<(nBuck1*nBuck2*nBuck3); i++) {
      buckets[i] = -1;
   }
   idxs = 0;
   for (i=0; i<_length; i++) {
      cBuck1 = (int)sdata[idxs] + 1;
      cBuck2 = (int)sdata[idxs+1] + 1;
      cBuck3 = (int)(sdata[idxs+2] - sMins) + 1;
      cBuck = cBuck1 + nBuck1*(cBuck2 + nBuck2*cBuck3);
      slist[i] = buckets[cBuck];
      buckets[cBuck] = i;
      idxs += lN;
   }
   idxd = 0;
   for (cBuck1=-1; cBuck1<=1; cBuck1++) {
      for (cBuck2=-1; cBuck2<=1; cBuck2++) {
	 for (cBuck3=-1; cBuck3<=1; cBuck3++) {
	    bucNeigh[idxd++] = cBuck1 + nBuck1*(cBuck2 + nBuck2*cBuck3);
	 }
      }
   }
   double wsuml, weight;
   double hiLTr = 80.0/sigmaR;

   memset(modeTable, 0, _length);
   for (i = 0; i < _length; i++) {
      if (modeTable[i] == 1)
	 continue;

      int pointCount = 0;
      idxs = i*lN;
      for (j=0; j<lN; j++)
	 yk[j] = sdata[idxs+j];
		
      for (j = 0; j < lN; j++)
	 Mh[j] = 0;
      wsuml = 0;

      cBuck1 = (int) yk[0] + 1;
      cBuck2 = (int) yk[1] + 1;
      cBuck3 = (int) (yk[2] - sMins) + 1;
      cBuck = cBuck1 + nBuck1*(cBuck2 + nBuck2*cBuck3);
      for (j=0; j<27; j++) {
	 idxd = buckets[cBuck+bucNeigh[j]];
	 while (idxd>=0) {
	    idxs = lN*idxd;
	    double el = sdata[idxs+0]-yk[0];
	    double diff = el*el;
	    el = sdata[idxs+1]-yk[1];
	    diff += el*el;

	    if (diff < 1.0) {
	       el = sdata[idxs+2]-yk[2];
	       if (yk[2] > hiLTr)
		  diff = 4*el*el;
	       else
		  diff = el*el;

	       if (_dimension>1) {
		  el = sdata[idxs+3]-yk[3];
		  diff += el*el;
		  el = sdata[idxs+4]-yk[4];
		  diff += el*el;
	       }

	       if (diff < 1.0) {
		  weight = 1-_weightMap[idxd];
		  for (int k=0; k<lN; k++)
		     Mh[k] += weight*sdata[idxs+k];
		  wsuml += weight;
	       }
	    }
	    idxd = slist[idxd];
	 }
      }
      if (wsuml > 0) {
	 for (j = 0; j < lN; j++)
	    Mh[j] = Mh[j]/wsuml - yk[j];
      } else {
	 for (j = 0; j < lN; j++)
	    Mh[j] = 0;
      }

      double mvAbs = (Mh[0]*Mh[0]+Mh[1]*Mh[1])*sigmaS*sigmaS;
      if (_dimension==3)
	 mvAbs += (Mh[2]*Mh[2]+Mh[3]*Mh[3]+Mh[4]*Mh[4])*sigmaR*sigmaR;
      else
	 mvAbs += Mh[2]*Mh[2]*sigmaR*sigmaR;

      int iterationCount = 1;
      while ((mvAbs >= EPSILON) && (iterationCount < MAX_ITERATIONS)) {
	 for (j = 0; j < lN; j++)
	    yk[j] += Mh[j];
	 int modeCandidateX = (int)(sigmaS*yk[0]+0.5);
	 int modeCandidateY = (int)(sigmaS*yk[1]+0.5);
	 int modeCandidate_i = modeCandidateY*_width + modeCandidateX;
	 if ((modeTable[modeCandidate_i] != 2) && (modeCandidate_i != i))  {
	    double diff = 0;
	    idxs = lN*modeCandidate_i;
	    for (int k=2; k<lN; k++) {
	       double el = sdata[idxs+k] - yk[k];
	       diff += el*el;
	    }

	    if (diff < DIST_FACTOR) {
	       if (modeTable[modeCandidate_i] == 0) {
		  pointList[pointCount++] = modeCandidate_i;
		  modeTable[modeCandidate_i]	= 2;
	       } else {
		  for (j = 0; j < _dimension; j++)
		     yk[j+2] = _msRawData[modeCandidate_i*_dimension+j]/sigmaR;
		  modeTable[i] = 1;
		  mvAbs = -1;
		  break;
	       }
	    }
	 }

	 for (j = 0; j < lN; j++)
	    Mh[j] = 0;
	 wsuml = 0;
	 cBuck1 = (int)yk[0] + 1;
	 cBuck2 = (int)yk[1] + 1;
	 cBuck3 = (int)(yk[2] - sMins) + 1;
	 cBuck = cBuck1 + nBuck1*(cBuck2 + nBuck2*cBuck3);
	 for (j=0; j<27; j++) {
	    idxd = buckets[cBuck+bucNeigh[j]];
	    while (idxd>=0) {
	       idxs = lN*idxd;
	       double el = sdata[idxs+0]-yk[0];
	       double diff = el*el;
	       el = sdata[idxs+1]-yk[1];
	       diff += el*el;
               
	       if (diff < 1.0) {
		  el = sdata[idxs+2]-yk[2];
		  if (yk[2] > hiLTr)
		     diff = 4*el*el;
		  else
		     diff = el*el;
                  
		  if (_dimension>1) {
		     el = sdata[idxs+3]-yk[3];
		     diff += el*el;
		     el = sdata[idxs+4]-yk[4];
		     diff += el*el;
		  }
                  
		  if (diff < 1.0) {
		     weight = 1-_weightMap[idxd];
		     for (int k=0; k<lN; k++)
			Mh[k] += weight*sdata[idxs+k];
		     wsuml += weight;
		  }
	       }
	       idxd = slist[idxd];
	    }
	 }
	 if (wsuml > 0) {
	    for (j = 0; j < lN; j++)
	       Mh[j] = Mh[j]/wsuml - yk[j];
	 } else {
	    for (j = 0; j < lN; j++)
	       Mh[j] = 0;
	 }
	       
	 mvAbs = (Mh[0]*Mh[0]+Mh[1]*Mh[1])*sigmaS*sigmaS;
	 if (_dimension==3)
	    mvAbs += (Mh[2]*Mh[2]+Mh[3]*Mh[3]+Mh[4]*Mh[4])*sigmaR*sigmaR;
	 else
	    mvAbs += Mh[2]*Mh[2]*sigmaR*sigmaR;

	 iterationCount++;
      }
	 
      if (mvAbs >= 0) {
	 for (j = 0; j < lN; j++)
	    yk[j] += Mh[j];
	 modeTable[i] = 1;
      }
		
      for (int k=0; k<_dimension; k++)
	 yk[k+2] *= sigmaR;

      for (j = 0; j < pointCount; j++) {
	 int modeCandidate_i = pointList[j];

	 modeTable[modeCandidate_i] = 1;

	 for (int k = 0; k < _dimension; k++)
	    _msRawData[_dimension*modeCandidate_i+k] = (float)(yk[k+2]);
      }
      for (j = 0; j < _dimension; j++)
	 _msRawData[_dimension*i+j] = (float)(yk[j+2]);
   }

   delete [] modeTable;
   delete [] pointList;

   delete [] buckets;
   delete [] slist;
   delete [] sdata;

   delete [] yk;
   delete [] Mh;
}

void MeanShift::fastestFilter( float sigmaS, float sigmaR ) throw (std::runtime_error) {
   /** the percentile of window radius. */
   const float windowRadiusThreshold = 0.1f;
   unsigned char *modeTable;
   int *pointList;
   int lN = _dimension + 2;
   double *yk, *Mh;
   float* sdata;

   if ((! (modeTable = new unsigned char [_length]))
       || (!(pointList = new int [_length]))
       || (!(yk = new double[lN]))
       || (!(Mh = new double[lN]))
       || (!(sdata = new float[lN*_length]))) {
      throw std::runtime_error("Not enough memory.");
   }
   
   _bandwidths[0] = sigmaS;
   _bandwidths[1] = sigmaR;
   
   int idxs=0, idxd=0;
   int i, j;
   for (i=0; i<_length; i++) {
      sdata[idxs++] = (i%_width)/sigmaS;
      sdata[idxs++] = (i/_width)/sigmaS;
      for (j=0; j<_dimension; j++)
	 sdata[idxs++] = _data[idxd++]/sigmaR;
   }

   int* buckets;
   int* slist = new int[_length];
   int bucNeigh[27];
   
   float sMins;
   float sMaxs[3];
   sMaxs[0] = _width/sigmaS;
   sMaxs[1] = _height/sigmaS;
   sMaxs[2] = sMins = sdata[2];
   idxs = 2;
   for (i=0; i<_length; i++) {
      float cval = sdata[idxs];
      if (cval < sMins)
	 sMins = cval;
      else if (cval > sMaxs[2])
	 sMaxs[2] = cval;      
      idxs += lN;
   }
   
   int cBuck1, cBuck2, cBuck3, cBuck;
   int nBuck1 = (int)(sMaxs[0] + 3);
   int nBuck2 = (int)(sMaxs[1] + 3);
   int nBuck3 = (int)(sMaxs[2] - sMins + 3);
   buckets = new int[nBuck1*nBuck2*nBuck3];
   for (i=0; i<(nBuck1*nBuck2*nBuck3); i++)
      buckets[i] = -1;

   idxs = 0;
   for (i=0; i<_length; i++) {
      cBuck1 = (int)sdata[idxs] + 1;
      cBuck2 = (int)sdata[idxs+1] + 1;
      cBuck3 = (int)(sdata[idxs+2] - sMins) + 1;
      cBuck = cBuck1 + nBuck1*(cBuck2 + nBuck2*cBuck3);
      
      slist[i] = buckets[cBuck];
      buckets[cBuck] = i;
      
      idxs += lN;
   }
   idxd = 0;
   for (cBuck1=-1; cBuck1<=1; cBuck1++) {
      for (cBuck2=-1; cBuck2<=1; cBuck2++) {
	 for (cBuck3=-1; cBuck3<=1; cBuck3++) {
	    bucNeigh[idxd++] = cBuck1 + nBuck1*(cBuck2 + nBuck2*cBuck3);
	 }
      }
   }
   double wsuml, weight;
   double hiLTr = 80.0/sigmaR;

   memset(modeTable, 0, _length);

   for (i=0; i<_length; i++) {
      if (modeTable[i] == 1)
	 continue;

      int pointCount = 0;
      idxs = i*lN;
      for (j=0; j<lN; j++)
	 yk[j] = sdata[idxs+j];
		
      for (j = 0; j < lN; j++)
	 Mh[j] = 0;
      wsuml = 0;
      cBuck1 = (int) yk[0] + 1;
      cBuck2 = (int) yk[1] + 1;
      cBuck3 = (int) (yk[2] - sMins) + 1;
      cBuck = cBuck1 + nBuck1*(cBuck2 + nBuck2*cBuck3);
      for (j=0; j<27; j++) {
	 idxd = buckets[cBuck+bucNeigh[j]];
	 while (idxd>=0) {
	    idxs = lN*idxd;
	    double el = sdata[idxs+0]-yk[0];
	    double diff = el*el;
	    el = sdata[idxs+1]-yk[1];
	    diff += el*el;

	    if (diff < 1.0) {
	       el = sdata[idxs+2]-yk[2];
	       if (yk[2] > hiLTr)
		  diff = 4*el*el;
	       else
		  diff = el*el;

	       if (_dimension>1) {
		  el = sdata[idxs+3]-yk[3];
		  diff += el*el;
		  el = sdata[idxs+4]-yk[4];
		  diff += el*el;
	       }

	       if (diff < 1.0) {
		  weight = 1-_weightMap[idxd];
		  for (int k=0; k<lN; k++)
		     Mh[k] += weight*sdata[idxs+k];
		  wsuml += weight;

		  if (diff < windowRadiusThreshold) {
		     if (modeTable[idxd] == 0) {
			pointList[pointCount++] = idxd;
			modeTable[idxd]	= 2;
		     }
		  }
	       }
	    }
	    idxd = slist[idxd];
	 }
      }
      if (wsuml > 0) {
	 for (j = 0; j < lN; j++)
	    Mh[j] = Mh[j]/wsuml - yk[j];
      } else {
	 for (j = 0; j < lN; j++)
	    Mh[j] = 0;
      }

      double mvAbs = (Mh[0]*Mh[0]+Mh[1]*Mh[1])*sigmaS*sigmaS;
      if (_dimension==3)
	 mvAbs += (Mh[2]*Mh[2]+Mh[3]*Mh[3]+Mh[4]*Mh[4])*sigmaR*sigmaR;
      else
	 mvAbs += Mh[2]*Mh[2]*sigmaR*sigmaR;

      int iterationCount = 1;
      while ((mvAbs >= EPSILON) && (iterationCount < MAX_ITERATIONS)) {
	 for (j = 0; j < lN; j++)
	    yk[j] += Mh[j];
			
	 int modeCandidateX = (int)(sigmaS*yk[0]+0.5);
	 int modeCandidateY = (int)(sigmaS*yk[1]+0.5);
	 int modeCandidate_i= modeCandidateY*_width + modeCandidateX;
	 if ((modeTable[modeCandidate_i] != 2) && (modeCandidate_i != i)) {
	    double diff = 0;
	    idxs = lN*modeCandidate_i;
	    for (int k=2; k<lN; k++) {
	       double el = sdata[idxs+k] - yk[k];
	       diff += el*el;
	    }

	    if (diff < windowRadiusThreshold) {
	       if (modeTable[modeCandidate_i] == 0) {
		  pointList[pointCount++] = modeCandidate_i;
		  modeTable[modeCandidate_i]	= 2;
	       } else {
		  for (j = 0; j < _dimension; j++)
		     yk[j+2] = _msRawData[modeCandidate_i*_dimension+j]/sigmaR;
		  modeTable[i] = 1;
		  mvAbs = -1;
		  break;
	       }
	    }
	 }
	 for (j = 0; j < lN; j++)
	    Mh[j] = 0;
	 wsuml = 0;
	 cBuck1 = (int) yk[0] + 1;
	 cBuck2 = (int) yk[1] + 1;
	 cBuck3 = (int) (yk[2] - sMins) + 1;
	 cBuck = cBuck1 + nBuck1*(cBuck2 + nBuck2*cBuck3);
	 for (j=0; j<27; j++) {
	    idxd = buckets[cBuck+bucNeigh[j]];
	    while (idxd>=0) {
	       idxs = lN*idxd;
	       double el = sdata[idxs+0]-yk[0];
	       double diff = el*el;
	       el = sdata[idxs+1]-yk[1];
	       diff += el*el;
               
	       if (diff < 1.0) {
		  el = sdata[idxs+2]-yk[2];
		  if (yk[2] > hiLTr)
		     diff = 4*el*el;
		  else
		     diff = el*el;
                  
		  if (_dimension>1) {
		     el = sdata[idxs+3]-yk[3];
		     diff += el*el;
		     el = sdata[idxs+4]-yk[4];
		     diff += el*el;
		  }
                  
		  if (diff < 1.0) {
		     weight = 1-_weightMap[idxd];
		     for (int k=0; k<lN; k++)
			Mh[k] += weight*sdata[idxs+k];
		     wsuml += weight;
		     if (diff < windowRadiusThreshold) {
			if (modeTable[idxd] == 0) {
			   pointList[pointCount++] = idxd;
			   modeTable[idxd] = 2;
			}
		     }
		  }
	       }
	       idxd = slist[idxd];
	    }
	 }
	 if (wsuml > 0) {
	    for (j = 0; j < lN; j++)
	       Mh[j] = Mh[j]/wsuml - yk[j];
	 } else {
	    for (j = 0; j < lN; j++)
	       Mh[j] = 0;
	 }

	 mvAbs = (Mh[0]*Mh[0]+Mh[1]*Mh[1])*sigmaS*sigmaS;
	 if (_dimension==3)
	    mvAbs += (Mh[2]*Mh[2]+Mh[3]*Mh[3]+Mh[4]*Mh[4])*sigmaR*sigmaR;
	 else
	    mvAbs += Mh[2]*Mh[2]*sigmaR*sigmaR;
	 iterationCount++;
      }

      if (mvAbs >= 0) {
	 for (j = 0; j < lN; j++)
	    yk[j] += Mh[j];
	 modeTable[i] = 1;
      }
      
      for (int k=0; k<_dimension; k++)
	 yk[k+2] *= sigmaR;
      for (j = 0; j < pointCount; j++) {
	 int modeCandidate_i = pointList[j];
	 modeTable[modeCandidate_i] = 1;
	 for (int k = 0; k < _dimension; k++)
	    _msRawData[_dimension*modeCandidate_i+k] = (float)(yk[k+2]);
      }
      for (j = 0; j < _dimension; j++)
	 _msRawData[_dimension*i+j] = (float)(yk[j+2]);
   }
   
   delete [] modeTable;
   delete [] pointList;

   delete [] buckets;
   delete [] slist;
   delete [] sdata;

   delete [] yk;
   delete [] Mh;
}

void MeanShift::accurateFilter( float sigmaS, float sigmaR ) throw (std::runtime_error) {
   unsigned char *modeTable;
   int *pointList;
   int lN = _dimension + 2;
   double *yk, *Mh;
   float* sdata;

   if ((! (modeTable = new unsigned char [_length]))
       || (!(pointList = new int [_length]))
       || (!(yk = new double[lN]))
       || (!(Mh = new double[lN]))
       || (!(sdata = new float[lN*_length]))) {
      throw std::runtime_error("Not enough memory.");
   }
   
   _bandwidths[0] = sigmaS;
   _bandwidths[1] = sigmaR;

   int idxs, idxd;
   idxs = idxd = 0;
   int i,j;
   for (i=0; i<_length; i++) {
      sdata[idxs++] = (i%_width)/sigmaS;
      sdata[idxs++] = (i/_width)/sigmaS;
      for (j=0; j<_dimension; j++)
	 sdata[idxs++] = _data[idxd++]/sigmaR;
   }

   int* buckets;
   int* slist = new int[_length];
   int bucNeigh[27];

   double sMins;
   double sMaxs[3];
   sMaxs[0] = _width/sigmaS;
   sMaxs[1] = _height/sigmaS;
   sMaxs[2] = sMins = sdata[2];
   idxs = 2;
   for (i=0; i<_length; i++) {
      double cval = sdata[idxs];
      if (cval < sMins)
	 sMins = cval;
      else if (cval > sMaxs[2])
	 sMaxs[2] = cval;
      
      idxs += lN;
   }
   
   int nBuck1 = (int)(sMaxs[0] + 3);
   int nBuck2 = (int)(sMaxs[1] + 3);
   int nBuck3 = (int)(sMaxs[2] - sMins + 3);
   buckets = new int[nBuck1*nBuck2*nBuck3];
   for (i=0; i<(nBuck1*nBuck2*nBuck3); i++)
      buckets[i] = -1;
   
   idxs = 0;
   int cBuck1, cBuck2, cBuck3, cBuck;
   for (i=0; i<_length; i++) {
      cBuck1 = (int)sdata[idxs] + 1;
      cBuck2 = (int)sdata[idxs+1] + 1;
      cBuck3 = (int)(sdata[idxs+2] - sMins) + 1;
      cBuck = cBuck1 + nBuck1*(cBuck2 + nBuck2*cBuck3);
      
      slist[i] = buckets[cBuck];
      buckets[cBuck] = i;
      
      idxs += lN;
   }
   idxd = 0;
   for (cBuck1=-1; cBuck1<=1; cBuck1++) {
      for (cBuck2=-1; cBuck2<=1; cBuck2++) {
	 for (cBuck3=-1; cBuck3<=1; cBuck3++) {
	    bucNeigh[idxd++] = cBuck1 + nBuck1*(cBuck2 + nBuck2*cBuck3);
	 }
      }
   }

   double wsuml, weight;
   double hiLTr = 80.0/sigmaR;
   for (i=0; i<_length; i++) {      
      idxs = i*lN;
      for (j=0; j<lN; j++)
	 yk[j] = sdata[idxs+j];

      for (j = 0; j < lN; j++)
	 Mh[j] = 0;

      wsuml = 0;
      cBuck1 = (int)yk[0] + 1;
      cBuck2 = (int)yk[1] + 1;
      cBuck3 = (int)(yk[2] - sMins) + 1;
      cBuck = cBuck1 + nBuck1*(cBuck2 + nBuck2*cBuck3);
      for (j=0; j<27; j++) {
	 idxd = buckets[cBuck+bucNeigh[j]];
	 while (idxd>=0) {
	    idxs = lN*idxd;
	    double el = sdata[idxs+0]-yk[0];
	    double diff = el*el;
	    el = sdata[idxs+1]-yk[1];
	    diff += el*el;
	    
	    if (diff < 1.0) {
	       el = sdata[idxs+2]-yk[2];
	       if (yk[2] > hiLTr)
		  diff = 4*el*el;
	       else
		  diff = el*el;
	       
	       if (_dimension>1) {
		  el = sdata[idxs+3]-yk[3];
		  diff += el*el;
		  el = sdata[idxs+4]-yk[4];
		  diff += el*el;
	       }

	       if (diff < 1.0) {
		  weight = 1-_weightMap[idxd];
		  for (int k=0; k<lN; k++)
		     Mh[k] += weight*sdata[idxs+k];
		  wsuml += weight;
	       }
	    }
	    idxd = slist[idxd];
	 }
      }
      if (wsuml > 0) {
	 for (j = 0; j < lN; j++)
	    Mh[j] = Mh[j]/wsuml - yk[j];
      } else {
	 for (j = 0; j < lN; j++)
	    Mh[j] = 0;
      }

      double mvAbs = 0;
      for (j = 0; j < lN; j++)
	 mvAbs += Mh[j]*Mh[j];
      int iterationCount = 1;
      while ((mvAbs >= EPSILON) && (iterationCount < MAX_ITERATIONS)) {
	 for (j=0; j<lN; j++)
	    yk[j] += Mh[j];
	 
	 for (j=0; j<lN; j++)
	    Mh[j] = 0;
	 wsuml = 0;
	 cBuck1 = (int)yk[0] + 1;
	 cBuck2 = (int)yk[1] + 1;
	 cBuck3 = (int)(yk[2] - sMins) + 1;
	 cBuck = cBuck1 + nBuck1*(cBuck2 + nBuck2*cBuck3);
	 for (j=0; j<27; j++) {
	    idxd = buckets[cBuck+bucNeigh[j]];
	    while (idxd>=0) {
	       idxs = lN*idxd;
	       double el = sdata[idxs+0]-yk[0];
	       double diff = el*el;
	       el = sdata[idxs+1]-yk[1];
	       diff += el*el;
	       
	       if (diff < 1.0) {
		  el = sdata[idxs+2]-yk[2];
		  if (yk[2] > hiLTr)
		     diff = 4*el*el;
		  else
		     diff = el*el;
		  
		  if (_dimension>1) {
		     el = sdata[idxs+3]-yk[3];
		     diff += el*el;
		     el = sdata[idxs+4]-yk[4];
		     diff += el*el;
		  }
		  
		  if (diff < 1.0) {
		     weight = 1-_weightMap[idxd];
		     for (int k=0; k<lN; k++)
			Mh[k] += weight*sdata[idxs+k];
		     wsuml += weight;
		  }
	       }
	       idxd = slist[idxd];
	    }
	 }
	 if (wsuml > 0) {
	    for (j = 0; j < lN; j++)
	       Mh[j] = Mh[j]/wsuml - yk[j];
	 } else {
	    for (j = 0; j < lN; j++)
	       Mh[j] = 0;
	 }

	 mvAbs = (Mh[0]*Mh[0]+Mh[1]*Mh[1])*sigmaS*sigmaS;
	 if (_dimension==3)
	    mvAbs += (Mh[2]*Mh[2]+Mh[3]*Mh[3]+Mh[4]*Mh[4])*sigmaR*sigmaR;
	 else
	    mvAbs += Mh[2]*Mh[2]*sigmaR*sigmaR;
	 iterationCount++;
      }
      for (j = 0; j < lN; j++)
	 yk[j] += Mh[j];

      for (j=0; j<_dimension; j++)
	 _msRawData[_dimension*i+j] = (float)(yk[j+2]*sigmaR);
   }

   delete [] modeTable;
   delete [] pointList;

   delete [] buckets;
   delete [] slist;
   delete [] sdata;
   
   delete [] yk;
   delete [] Mh;
}

Errc PMeanShift( const Img2duc &ims, Reg2d &rgd, int spatialBandwidth, float rangeBandwidth, int minimumRegionArea, int accuracyLevel ) {
   if ((spatialBandwidth <= 0) || (rangeBandwidth <= 0)) { 
      std::cerr<<"Error pmeanshift: bad parameter values. Out of bounds."<<std::endl;
      return FAILURE;
   }
   
   try {
       MeanShift msProcessor(ims);
       return msProcessor.segment(rgd, spatialBandwidth, rangeBandwidth, minimumRegionArea, accuracyLevel);
   } catch ( const std::exception &e ) {
      std::cerr<<"Error pmeanshift: "<<e.what()<<std::endl;
      return FAILURE;
   }
   
   return SUCCESS;
}


Errc PMeanShift( const Imc2duc &ims, Reg2d &rgd, int spatialBandwidth, float rangeBandwidth, int minimumRegionArea, int accuracyLevel ) {
   if ((spatialBandwidth <= 0) || (rangeBandwidth <= 0)) { 
      std::cerr<<"Error pmeanshift: bad parameter values."<<std::endl;
      return FAILURE;
   }
   try {
      MeanShift msProcessor(ims);
      return msProcessor.segment(rgd, spatialBandwidth, rangeBandwidth, minimumRegionArea, accuracyLevel);
   } catch ( const std::exception &e ) {
      std::cerr<<"Error pmeanshift: "<<e.what()<<std::endl;
      return FAILURE;
   }
   return SUCCESS;
}

#ifdef MAIN
#define	USAGE	"usage: %s spatial-bandwidth range-bandwidth minimum-region-area accuracy-level [-m mask] [im_in|-] [rg_out|-]"
#define	PARC	4
#define	FINC	1
#define	FOUTC	1
#define	MASK	3

int main( int argc, char *argv[] ) {
   Errc result;              // The result code of the execution.
   Pobject* mask;            // The region map.
   Pobject* objin[FINC+1];   // The input objects.
   Pobject* objs[FINC+1];    // The source objects masked.
   Pobject* objout[FOUTC+1]; // The output object.
   Pobject* objd[FOUTC+1];   // The result object of the execution.
   char* parv[PARC+1];       // The input parameters.
   
   ReadArgs(argc,argv,PARC,FINC,FOUTC,&mask,objin,objs,objout,objd,parv,USAGE,MASK);
   if (objs[0]->Type()==Po_Img2duc) {
      Img2duc* const ims=(Img2duc*)objs[0];
      objd[0]=new Reg2d(ims->Props());
      Reg2d* imd=(Reg2d*)objd[0];

      result=PMeanShift(*ims, *imd, atoi(parv[0]), (float)atof(parv[1]), atoi(parv[2]), atoi(parv[3]));
      goto end;
   } 
   if (objs[0]->Type()==Po_Imc2duc) {
      Imc2duc* const ims=(Imc2duc*)objs[0];
      objd[0]=new Reg2d(ims->Props());
      Reg2d* imd=(Reg2d*)objd[0];

      result=PMeanShift(*ims, *imd, atoi(parv[0]), (float)atof(parv[1]), atoi(parv[2]), atoi(parv[3]));
      goto end;
   } 
   {
      PrintErrorFormat(objin,FINC);
      result = FAILURE;
   }	

 end:
   if (result) {
      WriteArgs(argc,argv,PARC,FINC,FOUTC,&mask,objin,objs,objout,objd,MASK);
   }
   Exit(result);
   return 0;
}
#endif
