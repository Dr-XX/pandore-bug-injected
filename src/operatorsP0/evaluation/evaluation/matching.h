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

/**
 * Some functions for computing the overlapping matrix.
 * Two algorithms are used:
 * 1/ best matching
 *  @article{Ozdemir2010,
 *   author    = {Bahadir \"{O}zdemir and Selim Aksoy and Sandra Eckert and Martino Pesaresi and Daniele Ehrlich},
 *   title     = {Performance measures for object detection evaluation},
 *   journal   = PRL,
 *   volume    = {31},
 *   number    = {10},
 *   month     = jul,
 *   year      = {2010},
 *   pages     = {1128-1137},
 *   publisher = {Elsevier Science Inc.},
 *   address   = {New York, NY, USA},
 *  }
 * 2/ bipartite matching (implemented with the Hungarian algorithm)
 *  @article{Jiang2006,
 *  author    = {Xiaoyi Jiang and Cyril Marti and Christophe Irniger and Horst Bunke},
 *   title     = {Distance measures for Image Segmentation Evaluation},
 *   journal   = {EURASIP Journal on Applied Signal Processing},
 *   publisher = {Hindawi Publishing Corporation},
 *   year      = {2006},
 *   volume    = {2006},
 *   number    = {15},
 *   month     = {jan},
 *   pages     = {1-10},
 *  }
 */

#ifndef _MATCHING_H
#define _MATCHING_H

#include <cstdio>
#include <limits>
#include <vector>

class Edge {
   private:
      Edge *_next; 
      long _label;
      
   public:
      long overlappingArea;
      
      Edge( long label, Edge *next, long area = 1 ): _next(next), _label(label), overlappingArea(area) {}
      ~Edge() {}
      
      long Label() const { return _label; };
      Edge* Next() const { return _next; };
      Edge* Next( Edge* next ) { return _next = next; }
};

class Node {
   private:
      Edge *_adjacents;
      /** Trash for removed edges */
      std::vector<Edge *> etrash;
      long _label;
      
   public :
      long area;
      
      Node( long label ): _adjacents(0), _label(label), area(1) { }
      ~Node() {
	 Edge *q, *p = _adjacents;

	 while ((q = p)) {
	    p = p->Next();
	    delete q;
	 }
	 
	 std::vector<Edge *>::iterator ite;
	 for (ite = etrash.begin(); ite != etrash.end(); ++ite) {
	    delete *ite;
	 }
      }

      Edge* Neighbours() const {
	 return _adjacents;
      }
      
      Edge* Search( long label ) const {
	 Edge *edge = 0;
	 for (edge = _adjacents; edge && (edge->Label() != label); edge = edge->Next()) ;
	 return edge;
      }
      
      Edge* Add( long label, long area ) {
	 return _adjacents = new Edge(label, _adjacents, area);
      }
      
      void Delete( long label ) {
	 Edge *p = _adjacents;
	 
	 if (p) {
	    if (p->Label() == label) {
	       etrash.push_back(p);
	       p = p->Next();
	       _adjacents = p;
	    } else {
	       for (Edge * q = p; q != NULL ; q = q->Next()) {
		  if (q->Label() == label) {
		     p->Next(q->Next());
		     etrash.push_back(q); // Problem when using Neighbours() list: cannot be deleted
		     break;
		  } else {
		     p = q;
		  }
	       }
	    }
	 }
      }
};

class MatchingGraph {
      friend class Edge;

   private:
      int _numberOfSegments;
      int _numberOfRegions;

      Node **_segments;
      Node **_regions;

   public:
      MatchingGraph(): _numberOfSegments(0), _numberOfRegions(0) { }
      MatchingGraph( int numberOfSegments, int numberOfRegions ): _numberOfSegments(numberOfSegments), _numberOfRegions(numberOfRegions) {
 	 _numberOfSegments = numberOfSegments;
 	 _numberOfRegions = numberOfRegions;

	 _segments = new Node*[_numberOfSegments];
	 for (int s = 0; s < numberOfSegments; _segments[s++] = 0) ;
	 _regions = new Node*[_numberOfRegions];
	 for (int r = 0; r < numberOfRegions; _regions[r++] = 0) ;
      }

      MatchingGraph & operator=( const MatchingGraph & graph ) {
	 reset();
	 for (int s = 0; s < _numberOfSegments; s++) {
	    if (graph.Segment(s)) {
	       AddSegment(s);
	       Segment(s)->area = graph.Segment(s)->area;
	    }
	 }
	 for (int r = 0; r < _numberOfRegions; r++) {
	    if (graph.Region(r)) {
	       AddRegion(r);
	       Region(r)->area = graph.Region(r)->area;
	    }
	 }
	 
	 for (int s = 0; s < _numberOfSegments; s++) {
	    if (graph.Segment(s)) {
	       for (Edge * ptr = graph.Segment(s)->Neighbours(); ptr != NULL; ptr = ptr->Next()) {
		  int r = ptr->Label();
		  _segments[s]->Add(r, ptr->overlappingArea);
		  _regions[r]->Add(s, ptr->overlappingArea);
	       }
	    }
	 }
	 return *this;
      }
      
      void reset() {
	 for (int i = 0; i< _numberOfSegments; i++) {
	    delete _segments[i];
	    _segments[i] = 0;
	 }
	 for (int i = 0; i< _numberOfRegions; i++) {
	    delete _regions[i];
	    _regions[i] = 0;
	 }
	 
      }
      ~MatchingGraph() {
	 reset();
	 delete[] _segments;
	 delete[] _regions;
      }

      const Node * Segment( int s ) const { return _segments[s]; }
      const Node * Region( int r ) const { return _regions[r]; }
      
      Node * Segment( int s ) { return _segments[s]; }
      Node * Region( int r ) { return _regions[r]; }
      
      int NumberOfSegments() const { return _numberOfSegments; }
      int NumberOfRegions() const { return _numberOfRegions; }

      void AddSegment( long segment ) {
	 _segments[segment] = new Node(segment);
      }

      void AddRegion( long region ) {
	 _regions[region] = new Node(region);
      }

      void Link( long segment, long region, long area, bool add = false ) {
	 Edge * edge = _segments[segment]->Search(region);
	 if (!edge) {
	    _segments[segment]->Add(region, area);
	 } else {
	    if (add) {
	       edge->overlappingArea += area;
	    } else {
	       edge->overlappingArea = area;
	    }
	 }
	 
	 edge = _regions[region]->Search(segment);
	 if (!edge) {
	    _regions[region]->Add(segment, area);
	 } else {
	    if (add) {
	       edge->overlappingArea += area;
	    } else {
	       edge->overlappingArea = area;
	    }
	 }
      }
      
      void Unlink( long segment, long region ) {
	 _segments[segment]->Delete(region);
	 _regions[region]->Delete(segment);
      }
      
      long getRegionArea( int region ) const {
	 if (!Region(region)) {
	    return 0;
	 }
	 return Region(region)->area;
      }
      
      long getSegmentArea( int segment ) const {
	 if (!Segment(segment)) {
	    return 0;
	 }
	 return Segment(segment)->area;
      }
      
      void setRegionArea( int region, long area ) {
	 Region(region)->area = area;
      }
      
      void setSegmentArea( int segment, long area ) {
	 Segment(segment)->area = area;
      }
      
      bool isRegionMatched( int region ) const {
	 return Region(region) && Region(region)->Neighbours() != 0;
      }
      
      bool isSegmentMatched( int segment ) const {
	 return Segment(segment) && Segment(segment)->Neighbours() !=0 ;
      }
            
      Edge * getMatchedSegmentsList( int region ) const {
	 return Region(region)->Neighbours();
      }
      
      Edge * getMatchedRegionsList( int segment ) const {
	 return Segment(segment)->Neighbours();
      }
      
      bool isRegionMatchedBy( int region, int segment ) const {
	 return Region(region)->Search(segment) != 0;
      }
      
      bool isSegmentMatchedBy( int segment, int region ) const {
	 return Segment(segment)->Search(region) != 0;
      }
};

const bool BEST_MATCHING_ID = 0;

class GraphMatching {
   private:
      MatchingGraph *_graph;
      int _numberOfSegments;
      int _numberOfRegions;
      
   public:
      GraphMatching( int numberOfSegments, int numberOfRegions ): _numberOfSegments(numberOfSegments), _numberOfRegions(numberOfRegions) { }
   
      MatchingGraph* computeMatching( const LabelImage<unsigned int> &segmentation, const LabelImage<unsigned int> &reference, int matchingAlgorithmId, float threshold ) {
	 _graph = new MatchingGraph(_numberOfSegments, _numberOfRegions);
	 if (reference.Labels() == 1) {
	    computeOverlappingAreaMatrix(segmentation, reference,* _graph);
	    thresholdOverlappingAreaMatrix(*_graph, threshold);
	 } else {
	    MatchingGraph overlappingAreaMatrix(_numberOfSegments, _numberOfRegions);
	    computeOverlappingAreaMatrix(segmentation, reference, overlappingAreaMatrix);
	    thresholdOverlappingAreaMatrix(overlappingAreaMatrix, threshold);
	    if (matchingAlgorithmId == BEST_MATCHING_ID) {
	       bestMatching(overlappingAreaMatrix);
	    } else {
	       bipartiteMatching(overlappingAreaMatrix);
	    }
	 }
   	 return _graph; 
      }

   private:
      template < typename T >
      void computeOverlappingAreaMatrix( const T &segmentation, const T &reference, MatchingGraph &overlappingAreaMatrix ) const {
	 unsigned int *psegmentation = segmentation.Vector();
	 const unsigned int *pend = psegmentation + segmentation.VectorSize();
	 unsigned int *preference = reference.Vector();

	 for (; psegmentation < pend; psegmentation++, preference++) {
	    int s = *psegmentation;
	    int r = *preference;

	    if (!_graph->Segment(s)) {
	       _graph->AddSegment(s);
	    } else {
	       _graph->Segment(s)->area++;
	    }
	    if (!_graph->Region(r)) {
	       _graph->AddRegion(r);
	    } else {
	       _graph->Region(r)->area++;
	    }

	    // Update the overlapping areas.
	    if (r > 0 && s > 0) {
	       if (!overlappingAreaMatrix.Segment(s)) {
		  overlappingAreaMatrix.AddSegment(s);
	       }
	       if (!overlappingAreaMatrix.Region(r)) {
		  overlappingAreaMatrix.AddRegion(r);
	       }
	       overlappingAreaMatrix.Link(s, r, 1, true);
	    }
	 }
      }
      
      void thresholdOverlappingAreaMatrix( MatchingGraph &overlappingAreaMatrix, float threshold ) {
	 for (int s = 1; s < _numberOfSegments ; s++) {
	    if (overlappingAreaMatrix.Segment(s) && overlappingAreaMatrix.Segment(s)->Neighbours()) {
	       for (Edge *ptr = overlappingAreaMatrix.Segment(s)->Neighbours(); ptr != NULL; ptr = ptr->Next()) {
		  int r = ptr->Label();
		  if ( ((float)ptr->overlappingArea / _graph->getRegionArea(r)) < threshold &&
		       ((float)ptr->overlappingArea / _graph->getSegmentArea(s)) < threshold) {
		     overlappingAreaMatrix.Unlink(s, r);
		  }
	       }
	    }
	 }
      }
      
      /**
       * Checks whether the edge between s and r is satisfied or not.
       * Assumption: le link between s and r exists.
       *
       * An edge is satisfied if it is :
       *    - 0
       *    - or 4-min(multiplicity(s), 2)-min(multiplicity(r), 2) >= 1,
       * where multiplicity(s) is the number of edges connected to s, and
       * where multiplicity(r) is the number of edges connected to r.
       * This means that r cannot be associated to s if either r or s is already
       * connected to several others r or s.
       */
      bool isSatisfied( const MatchingGraph &graph, const int s, const int r ) const {
	 int smultiplicity = 0;
	 for (Edge * ptr = graph.Segment(s)->Neighbours(); ptr != NULL; ptr = ptr->Next()) {
	    if (ptr->overlappingArea > 0) {
	       smultiplicity++;
	    }
	 }
	 int rmultiplicity = 0;
	 for (Edge * ptr = graph.Region(r)->Neighbours(); ptr != NULL; ptr = ptr->Next()) {
	    if (ptr->overlappingArea > 0) {
	       rmultiplicity++;
	    }
	 }
	 return smultiplicity <= 1 || rmultiplicity <= 1;
      }
      
      /**
       * Tests whether all the nodes are consistent.
       * @return 0 if at least one node is unconsistent, the total area otherwise.
       */
      long isGoalState( const MatchingGraph &graph ) const {
	 long max = 0;
	 for (int s = 1; s < graph.NumberOfSegments(); s++) {
	    if (graph.Segment(s)) {
	       for (Edge *ptr = graph.Segment(s)->Neighbours(); ptr != NULL; ptr = ptr->Next()) {
		  int r = ptr->Label();
		  if (!isSatisfied(graph, s, r)) {
		     return 0;
		  }
		  max += ptr->overlappingArea;
	       }
	    }
	 }
	 return max;
      }
      
      void satisfyEdgeFromSPerspective( MatchingGraph & graph, int *segments, const int s, const int r ) const {
	 for (Edge * ptr = graph.Region(r)->Neighbours(); ptr != NULL; ptr = ptr->Next()) {
	    int si = ptr->Label();
	    segments[si] = ptr->overlappingArea;
	 }
	 for (int si = 1; si < graph.NumberOfSegments(); si++) {
	    if (si != s && segments[si] > 0) {
	       graph.Unlink(si, r);
	    }
	 }
      }
   
      void satisfyEdgeFromRPerspective( MatchingGraph & graph, int *regions, const int s, const int r ) const {
	 for (Edge * ptr = graph.Segment(s)->Neighbours(); ptr != NULL; ptr = ptr->Next()) {
	    int ri = ptr->Label();
	    regions[ri] = ptr->overlappingArea;
	 }
	 for (int ri = 1; ri < graph.NumberOfRegions(); ri++) {
	    if (ri != r && regions[ri] > 0) {
	       graph.Unlink(s, ri);
	    }
	 }
      }

      void restoreEdgeFromSPerspective( MatchingGraph & graph, int *segments, const int r ) const {
	 for (int s = 1; s < graph.NumberOfSegments(); s++) {
	    if (segments[s] > 0) {
	       graph.Link(s, r, segments[s]);
	    }
	 }
      }
 
      void restoreEdgeFromRPerspective( MatchingGraph & graph, const int *regions, const int s ) const {
	 for (int r = 1; r < graph.NumberOfRegions(); r++) {
	    if (regions[r] > 0) {
	       graph.Link(s, r, regions[r]);
	    }
	 }
      }

      long computeWeightRi( const MatchingGraph & graph, const int r ) const {
	 long w = 0;
	 for (Edge *ptr = graph.Region(r)->Neighbours(); ptr != NULL; ptr = ptr->Next()) {
	    w += ptr->overlappingArea;
	 } 
	 return w;
      }
   
      double computeWeightSi( const MatchingGraph & graph, const int s ) const {
	 long w = 0;
	 for (Edge *ptr = graph.Segment(s)->Neighbours(); ptr != NULL; ptr = ptr->Next()) {
	    w += ptr->overlappingArea;
	 } 
	 return w;
      }

      long computeArea( const MatchingGraph & graph ) const {
	 long area = 0;
	 for (int s = 1; s < graph.NumberOfSegments(); s++) {
	    if (graph.Segment(s)) {
	       for (Edge *ptr = graph.Segment(s)->Neighbours(); ptr != NULL; ptr = ptr->Next()) {
		  area += ptr->overlappingArea;
	       }
	    }
	 }
	 return area;
      }
      
      void display( MatchingGraph& g ) const {
	 puts("****  GRAPH ************");
	 for (int s = 0; s < g.NumberOfSegments(); s++) {
	    if (!g.Segment(s)) {
	       continue;
	    }
	    bool edge = false;
	    printf ("s=%d ->\t", s);
	    for (Edge *ptr = g.Segment(s)->Neighbours(); ptr != NULL; ptr = ptr->Next()) {
	       edge = true;
	       printf ("r=%ld (area = %ld)\t", ptr->Label(), ptr->overlappingArea);
	    }
	    if (edge) {
	       printf ("\n");
	    }
	 }
      }
   
      /**
       * Finds the edge with the maximum area.
       * The resulted edge is between smax and rmax.
       */
      long findMaximumEdge( MatchingGraph &graph, int &smax, int &rmax ) const {
	 long max = 0;

	 for (int s = 1; s < graph.NumberOfSegments(); s++) {
	    if (graph.Segment(s)) {
	       for (Edge *ptr = graph.Segment(s)->Neighbours(); ptr != NULL; ptr = ptr->Next()) {
		  int r = ptr->Label();
		  if (!isSatisfied(graph, s, r) && ptr->overlappingArea > max) {
		     max = ptr->overlappingArea;
		     smax = s;
		     rmax = r;
		  }
	       }
	    }
	 }
	 return max;
      }

      double optimize( MatchingGraph &graphin, MatchingGraph &graphout, double max ) const {
	 int smax = 0;
	 int rmax = 0;

	 long area = findMaximumEdge(graphin, smax, rmax);

	 int * segments = new int[graphin.NumberOfSegments()];
	 int * regions = new int[graphin.NumberOfRegions()];

	 if (smax > 0) {
	    double max1;
	    double w1 = computeWeightRi(graphin, rmax);
	    double w2 = computeWeightSi(graphin, smax);

	    if (w1 > w2) {
	       // 1. Consider rmax as the main region that has a multiplicity>1.
	       // Remove the edges of the si that are connected to rmax
	       // other than to the edge rmax-si.
	       for ( int *ptr = regions; ptr < regions + graphin.NumberOfRegions();  *ptr++ = 0);
	       satisfyEdgeFromRPerspective(graphin, regions, smax, rmax);
	       max1 = optimize(graphin, graphout, max);
	       if (max1 > max) {
		  max = max1;
	       }
	       restoreEdgeFromRPerspective(graphin, regions, smax);
	 
	       // 2. Consider si as the main region that has a multiplicity>1.
	       // Remove the edges of the ri that are connected to smax
	       // other than to the edge ri-smax.
	       for ( int *ptr = segments; ptr < segments + graphin.NumberOfSegments();  *ptr++ = 0);
	       satisfyEdgeFromSPerspective(graphin, segments, smax, rmax);
	       if (computeArea(graphin) > max) {
		  max1 = optimize(graphin, graphout, max);
		  if (max1 > max) {
		     max = max1;
		  }
	       }
	       restoreEdgeFromSPerspective(graphin, segments, rmax);
	    } else {
	       // 1. Consider rmax as the main region that has a multiplicity>1.
	       // Remove the edges of the ri that are connected to smax
	       // other than to the edge ri-smax.
	       for ( int *ptr = segments; ptr < segments + graphin.NumberOfSegments();  *ptr++ = 0);
	       satisfyEdgeFromSPerspective(graphin, segments, smax, rmax);
	       max1 = optimize(graphin, graphout, max);
	       if (max1 > max) {
		  max = max1;
	       }
	       restoreEdgeFromSPerspective(graphin, segments, rmax);
		 
	       // 2. Consider si as the main region that has a multiplicity>1.
	       // Remove the edges of the si that are connected to rmax
	       // other than to the edge si-rmax.
	       for ( int *ptr = regions; ptr < regions + graphin.NumberOfRegions();  *ptr++ = 0);
	       satisfyEdgeFromRPerspective(graphin, regions, smax, rmax);
	       if (computeArea(graphin) > max) {
		  max1 = optimize(graphin, graphout, max);
		  if (max1 > max) {
		     max = max1;
		  }
	       }
	       restoreEdgeFromRPerspective(graphin, regions, smax);
	    }

	    // 3. Remove the edge.
	    graphin.Unlink(smax, rmax);

	    // If the new state have an area < max then stop the optimization
	    if (computeArea(graphin) > max) {
	       max1 = optimize(graphin, graphout, max);
	       if (max1 > max) {
		  max = max1;
	       }
	    }
	    graphin.Link(smax, rmax, area);
	    delete[] segments;
	    delete[] regions;
	    return max;
	 } else {
	    // When all the edges are satisfied.
	    int max1 = isGoalState(graphin);
	    if (max1 > max) {
	       graphout = graphin;
	       max = max1;
	    }
	    delete[] segments;
	    delete[] regions;
	    return max1;
	 }
      }

      void getCliqueFromRegion( MatchingGraph &grs, MatchingGraph &grd, int region ) {
	 if (!grs.Region(region)) { 
	    return;
	 }

	 if (!grd.Region(region)) {
	    grd.AddRegion(region);
	 }

	 Edge *ptr = grs.Region(region)->Neighbours();
	 while (ptr != NULL) {
	    int segment = ptr->Label();
	    if (!grd.Segment(segment)) {
	       grd.AddSegment(segment);
	    }
	    grd.Link(segment, region, ptr->overlappingArea);
	    grs.Unlink(segment, region);
	    getCliqueFromSegment(grs, grd, segment); 
	    ptr = grs.Region(region)->Neighbours();
	 }
      }

      void getCliqueFromSegment( MatchingGraph &grs, MatchingGraph &grd, int segment ) {
	 if (!grs.Segment(segment)) { 
	    return;
	 }
	 
	 if (!grd.Segment(segment)) {
	    grd.AddSegment(segment);
	 }
	 
	 Edge *ptr = grs.Segment(segment)->Neighbours();
	 while (ptr != NULL) {
	    int region = ptr->Label();
	    if (!grd.Region(region)) {
	       grd.AddRegion(region);
	    }
	    grd.Link(segment, region, ptr->overlappingArea);
	    grs.Unlink(segment, region);
	    getCliqueFromRegion(grs, grd, region); 
	    ptr = grs.Segment(segment)->Neighbours();
	 }
      }

      /**
       * Computes the best matching between the segmentation
       * and the reference. The algorithm is based on
       * constraint satisfaction.
       */
      void bestMatching( MatchingGraph &overlappingAreaMatrix ) {
	 MatchingGraph subGraph(overlappingAreaMatrix.NumberOfSegments(), overlappingAreaMatrix.NumberOfRegions());
	 MatchingGraph bestMatchingGraph(overlappingAreaMatrix.NumberOfSegments(), overlappingAreaMatrix.NumberOfRegions());
	 
	 for (int s = 1; s < _numberOfSegments ; s++) {
	    if (overlappingAreaMatrix.Segment(s) && overlappingAreaMatrix.Segment(s)->Neighbours()) {
	       subGraph.reset();
	       getCliqueFromSegment(overlappingAreaMatrix, subGraph, s);
	       bestMatchingGraph.reset();
	       double max = optimize(subGraph, bestMatchingGraph, 0.0);
	       if (max > 0) {
		  for (int si = 1; si < _numberOfSegments; si++) {
		     if (bestMatchingGraph.Segment(si)) {
			for (Edge *ptr = bestMatchingGraph.Segment(si)->Neighbours(); ptr != NULL; ptr = ptr->Next()) {
			   if (ptr->overlappingArea > 0) {
			      int ri = ptr->Label();

			      if (!_graph->Segment(si)) {
				 _graph->AddSegment(si);
			      }
			      if (!_graph->Region(ri)) {
				 _graph->AddRegion(ri);
			      }
			      _graph->Link(si, ri, ptr->overlappingArea);
			   }
			}
		     }
		  }
	       }
	    }
	 }
      }

      // BIPARTITE 
      LabelImage<int> _costMatrix;
      LabelImage<unsigned char> _maskMatrix;  // Used to star and prime zeros of the cost matrix.
      int ** _path;
      int *_rowCover;
      int *_colCover;
      int path_row_0;
      int path_col_0;
      int step;  // the current step in the processing (state machine).
      bool _invert; // true if the width < height;

      void computeBipartiteGraph( const MatchingGraph &overlappingAreaMatrix ) {
	 bool done = false;
	 step = 1;
	 while (!done) {
	    switch (step) {
	       case 1:
		  step = step1();
		  break;
	       case 2:
		  step = step2();
		  break;
	       case 3:
		  step = step3();
		  break;
	       case 4:
		  step = step4();
		  break;
	       case 5:
		  step = step5();
		  break;
	       case 6:
		  step = step6();
		  break;
	       case 7:
		  done = true;
		  break;
	    }
	 }
	 if (_invert) {
	    for (int s = 0; s < _numberOfSegments; s++) {
	       if (overlappingAreaMatrix.Segment(s)) {
		  for (Edge *ptr = overlappingAreaMatrix.Segment(s)->Neighbours(); ptr != NULL; ptr = ptr->Next()) {
		     int r = ptr->Label();

		     if (_maskMatrix(s, r) == 1) {
			if (!_graph->Segment(s)) { 
			   _graph->AddSegment(s);
			}
			if (!_graph->Region(r)) {
			   _graph->AddRegion(r);
			}
			_graph->Link(s, r, ptr->overlappingArea);
		     }
		  }
	       }
	    }
	 } else {
	    for (int s = 0; s < _numberOfSegments; s++) {
	       if (overlappingAreaMatrix.Segment(s)) {
		  for (Edge *ptr = overlappingAreaMatrix.Segment(s)->Neighbours(); ptr != NULL; ptr = ptr->Next()) {
		     int r = ptr->Label();
		     if (_maskMatrix(r, s) == 1) {
			if (!_graph->Segment(s)) {
			   _graph->AddSegment(s);
			}
			if (!_graph->Region(r)) {
			   _graph->AddRegion(r);
			}
			_graph->Link(s, r, ptr->overlappingArea);
		     }
		  }
	       }
	    }
	 }
      }

      /**
       * For each row of the cost matrix,
       * finds the smallest element and subtracts
       * it from every element in its row. 
       * When finished, Go to Step 2.
       */
      int step1( ) {
	 for (int r = 0; r < _costMatrix.Height(); r++) {
	    int min_in_row = _costMatrix(r, 0);
	    for (int c = 0; c < _costMatrix.Width(); c++) {
	       if (_costMatrix(r, c) < min_in_row) {
		  min_in_row = _costMatrix(r, c);
	       }
	    }
	    for (int c = 0; c < _costMatrix.Width(); c++) {
	       _costMatrix(r, c) -= min_in_row;
	    }
	 }

	 return 2;
      }

      /**
       * Finds a zero (Z) in the resulting matrix. If there is no starred 
       * zero in its row or column, stars Z. Repeats for each element in the 
       * matrix. Go to Step 3.
       */
      int step2( ) {
	 for (int r = 0; r < _costMatrix.Height(); r++) {
	    for (int c = 0; c < _costMatrix.Width(); c++) {
	       if (_costMatrix(r, c) == 0 && _rowCover[r] == 0 && _colCover[c] == 0) {
		  _maskMatrix(r, c) = 1;
		  _rowCover[r] = 1;
		  _colCover[c] = 1;
	       }
	    }
	 }
	 for (int r = 0; r < _costMatrix.Height(); r++) {
	    _rowCover[r] = 0;
	 }
	 for (int c = 0; c < _costMatrix.Width(); c++) {
	    _colCover[c] = 0;
	 }
	 return 3;
      }
   
      /**
       * Covers each column containing a starred zero. If K columns are covered, 
       * the starred zeros describe a complete set of unique assignments. In this 
       * case, Go to DONE, otherwise, Go to Step 4.
       */
      int step3( ) {
	 for (int r = 0; r < _costMatrix.Height(); r++) {
	    for (int c = 0; c < _costMatrix.Width(); c++) {
	       if (_maskMatrix(r, c) == 1) {
		  _colCover[c] = 1;
	       }
	    }
	 }
	 int colcount = 0;
	 for (int c = 0; c < _costMatrix.Width(); c++) {
	    if (_colCover[c] == 1) {
	       colcount += 1;
	    }
	 }
	 if (colcount >= _costMatrix.Width() || colcount >= _costMatrix.Height()) {
	    return 7;
	 }
	 return 4;
      }
   
      Point find_a_zero() {
	 Point p(-1, -1);

	 for (int r = 0; r < _costMatrix.Height(); r++) {
	    for (int c = 0; c < _costMatrix.Width(); c++) {
	       if (_costMatrix(r, c) == 0 && _rowCover[r] == 0 && _colCover[c] == 0) {
		  p.y = r;
		  p.x = c;
		  return p;
	       }
	    }
	 }
	 return p;
      }

      int find_star_in_row( int row ) {
	 for (int c = 0; c < _costMatrix.Width(); c++) {
	    if (_maskMatrix(row, c) == 1) {
	       return c;
	    }
	 }
	 return -1;
      }

      /**
       * Finds a noncovered zero and primes it. If there is no starred zero 
       * in the row containing this primed zero, Go to Step 5. Otherwise, 
       * cover this row and uncover the column containing the starred zero. 
       * Continue in this manner until there are no uncovered zeros left. 
       * Save the smallest uncovered value and Go to Step 6.
       */
      int step4( ) {	 
	 while (true) {
	    Point p = find_a_zero();
	    int row = p.y;
	    int col = p.x;
	    if (row == -1) {
	       return 6;
	    } else {
	       _maskMatrix(row, col) = 2;
	       int ncol = find_star_in_row(row);
	       if (ncol >= 0) {
		  _rowCover[row] = 1;
		  _colCover[ncol] = 0;
	       } else {
		  path_row_0 = row;
		  path_col_0 = col;
		  return 5;
	       }
	    }
	 }
      }
      
      int find_star_in_col( int c ) {
	 for (int i = 0; i < _costMatrix.Height(); i++) {
	    if (_maskMatrix(i, c) == 1) {
	       return i;
	    }
	 }
	 return -1;
      }
      
      int find_prime_in_row( int r ) {
	 for (int j = 0; j < _costMatrix.Width(); j++) {
	    if (_maskMatrix(r, j) == 2) {
	       return j;
	    }
	 }
	 return 0;
      }

      void augment_path( int path_count ) {
	 for (int p = 0; p < path_count; p++) {
	    if (_maskMatrix(_path[p][0], _path[p][1]) == 1) {
	       _maskMatrix(_path[p][0], _path[p][1]) = 0;
	    } else {
	       _maskMatrix(_path[p][0], _path[p][1]) = 1;
	    }
	 }
      }
      
      void clear_covers() {
	 for (int r = 0; r < _costMatrix.Height(); r++) {
	    _rowCover[r] = 0;
	 }
	 for (int c = 0; c < _costMatrix.Width(); c++) {
	    _colCover[c] = 0;
	 }
      }
      
      void erase_primes() {
	 for (int r = 0; r < _costMatrix.Height(); r++) {
	    for (int c = 0; c < _costMatrix.Width(); c++) {
	       if (_maskMatrix(r, c) == 2) {
		  _maskMatrix(r, c) = 0;
	       }
	    }
	 }
      }
      
      /**
       * Constructs a series of alternating primed and starred zeros as follows.
       * Let Z0 represents the uncovered primed zero found in Step 4.
       * Let Z1 denotes the starred zero in the column of Z0 (if any).
       * Let Z2 denotes the primed zero in the row of Z1 (there will always be one).
       * Continue until the series terminates at a primed zero that has no
       * starred zero in its column.
       * Unstars each starred zero of the series, star each primed zero of the
       * series, erases all primes and uncover every line in the matrix.
       * Returns to Step 3.
       */
      int step5( ) {
	 int path_count = 1;
	 _path[path_count - 1][0] = path_row_0;
	 _path[path_count - 1][1] = path_col_0;
	 while (true) {
	    int r = find_star_in_col(_path[path_count - 1][1]);
	    if (r<0) break;
	    path_count += 1;
	    _path[path_count - 1][0] = r;
	    _path[path_count - 1][1] = _path[path_count - 2][1];
	 
	    int c= find_prime_in_row(_path[path_count - 1][0]);
	    path_count += 1;
	    _path[path_count - 1][0] = _path[path_count - 2][0];
	    _path[path_count - 1][1] = c;
	 }
	 augment_path(path_count);
	 clear_covers();
	 erase_primes();
	 return 3;
      }

      int find_smallest() {
#undef max
	 int minval= std::numeric_limits<int>::max();
	 for (int r = 0; r < _costMatrix.Height(); r++) {
	    for (int c = 0; c < _costMatrix.Width(); c++) {
	       if (_rowCover[r] == 0 && _colCover[c] == 0) {
		  if (minval > _costMatrix(r, c)) {
		     minval = _costMatrix(r, c);
		  }
	       }
	    }
	 }
	 return minval;
      }

      /**
       * Adds the value found in Step 4 to every element of
       * each covered row, and subtracts it from every element of
       * each uncovered column.
       * Return to Step 4 without altering any stars, primes, or covered lines.
       */
      int step6(  ) {
	 int minval = find_smallest();
	 for (int r = 0; r < _costMatrix.Height(); r++) {
	    for (int c = 0; c < _costMatrix.Width(); c++) {
	       if (_rowCover[r] == 1) {
		  _costMatrix(r, c) += minval;
	       }
	       if (_colCover[c] == 0) {
		  _costMatrix(r, c) -= minval;
	       }
	    }
	 }
	 return 4;
      }

      void bipartiteMatching( const MatchingGraph &overlappingAreaMatrix ) { 
	 if (_numberOfSegments < _numberOfRegions) {
	    _costMatrix.New(_numberOfRegions, _numberOfSegments);
	    _costMatrix = 0;
	    _invert = true;

	    for (int s = 0; s < _numberOfSegments; s++) {
	       if (overlappingAreaMatrix.Segment(s)) {
		  for (Edge * ptr = overlappingAreaMatrix.Segment(s)->Neighbours(); ptr != NULL; ptr = ptr->Next()) {
		     int r = ptr->Label();
		     _costMatrix(s, r) = -1 * ptr->overlappingArea;
		  }
	       }
	    }
	 } else {
	    _costMatrix.New(_numberOfSegments, _numberOfRegions);
	    _costMatrix = 0;
	    _invert = false;

	    for (int s = 0; s < _numberOfSegments; s++) {
	       if (overlappingAreaMatrix.Segment(s)) {
		  for (Edge * ptr = overlappingAreaMatrix.Segment(s)->Neighbours(); ptr != NULL; ptr = ptr->Next()) {
		     int r = ptr->Label();
		     _costMatrix(r, s) = -1 * ptr->overlappingArea;
		  }
	       }
	    }
	 }
	 _maskMatrix.New(_costMatrix.Width(), _costMatrix.Height());
	 _maskMatrix = 0;
	 _rowCover = new int[_costMatrix.Height()];
	 _colCover = new int[_costMatrix.Width()];
	 
	 for (int r = 0; r < _costMatrix.Height(); r++) {
	    _rowCover[r] = 0;
	 }
	 for (int c = 0; c < _costMatrix.Width(); c++) {
	    _colCover[c] = 0;
	 }

	 _path = new int*[_costMatrix.Height() + _costMatrix.Width()]; 
	 for (int i = 0; i < _costMatrix.Height() + _costMatrix.Width(); i++) {
	    _path[i] = new int[2];
	 }
	 
	 computeBipartiteGraph(overlappingAreaMatrix);

	 delete[] _rowCover;
	 delete[] _colCover;

	 for (int i = 0; i < _costMatrix.Height() + _costMatrix.Width(); i++) {
	    delete[] _path[i];
	 }
	 delete[] _path;
      }
};

inline MatchingGraph *
RegionGraphMatching( const LabelImage<unsigned int> &segmentation, const LabelImage<unsigned int> &reference, int matchingAlgorithmId, float threshold ) {
   const int numberOfSegments = segmentation.Labels() + 1;
   const int numberOfRegions = reference.Labels() + 1;
   GraphMatching graph(numberOfSegments, numberOfRegions);
   
   return graph.computeMatching(segmentation, reference, matchingAlgorithmId, threshold);
}

#endif
