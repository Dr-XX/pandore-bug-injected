/* -*- c-basic-offset: 3; mode:c++ -*-
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
 * @author Régis Clouard - 1997-07-15
 * @author Régis Clouard - 2001-05-09 (version 3.00)
 * @author Sébastien Bougleux - 2001-05-09 (new algorithm)
 */

#include <cmath>
#include <pandore.h>
using namespace pandore;
#define DELETED 2

/**
 * @file pdelaunay.cpp
 * Computes Euclidean Delaunay Triangulation of a region seed image 
 * (each seed must have its proper label), by the Steve J. Fortune 
 * Sweepline Algorithm for Voronoi Diagrams (Algorithmica 2, 153-174)
 */

// ----------------------------------------------------------
// --- FreeList ---------------------------------
// -- quick fifo list for memory storage ------
// ------------------------------------------
struct FreeNode { FreeNode *nextfree; };

class FreeList {
private:
   FreeNode *head;
   size_t nodesize;
   Long nb;
public:
   FreeList(size_t, Long);
   ~FreeList();
   void push(FreeNode*);
   Char* pop();
};


FreeList::FreeList(size_t size, Long n) : head(0), nodesize(size), nb(n) { }

FreeList::~FreeList() { }

void FreeList::push( FreeNode *curr ) {
   curr->nextfree = head;
   head = curr;
}

Char* FreeList::pop() {
   FreeNode *t;
   if (!head) {
      t = (FreeNode*)malloc(nb * nodesize);
      for (int i = 0; i < nb; i++) {
	 push((FreeNode*)((Char*)t + i * nodesize));
      }
   }
   t = head;
   head = head->nextfree;
   return (Char*)t;
}

// ----------------------------------------------------------
// --- Pt2d, Site2d, Edge, HalfEdge ---------------------------
// ----------------------------------------------------------
struct Pt2d {
   Float x;
   Float y;
};

struct Site2d {
   Pt2d coord;
   Long seednbr;
   Long refcnt;
};

struct Edge {
   Float a, b, c;
   Site2d* ep[2];
   Site2d* reg[2];
   int edgenbr;
   bool endpoint( int, Site2d* );
};

bool Edge::endpoint( int lr, Site2d *s ) {
   ep[lr] = s;
   if (!ep[1-lr]) {
      return 0;
   }
   return 1;
}

class HalfEdge {
public:
   HalfEdge *ELleft;
   HalfEdge *ELright;
   Edge *ELedge;
   int ELrefcnt;
   Char ELpm;
   Site2d *vertex;
   Float ystar;
   HalfEdge *PQnext;
   HalfEdge() : PQnext(NULL) { }
   ~HalfEdge() { }
   bool right_of( Pt2d* );
   bool intersect( HalfEdge* );
};

bool HalfEdge::right_of( Pt2d *p ) {  
   bool above, fast;
   Float dxp, dyp, dxs, t1, t2, t3, yl;
   Site2d *topsite = ELedge->reg[1];
   bool right_of_site = (p->x > topsite->coord.x);
   if (right_of_site && (ELpm == 0)) {
      return 1;
   }
   if (!right_of_site && (ELpm == 1)) {
      return 0;
   }
   if (ELedge->a == 1.0) {
      dyp = p->y - topsite->coord.y;
      dxp = p->x - topsite->coord.x;
      fast = 0;
      if ((!right_of_site & (ELedge->b < 0.0)) || (right_of_site & (ELedge->b >= 0.0))) {
	 fast = above = (dyp >= ELedge->b*dxp);
      } else {
	 above = ((p->x + p->y * ELedge->b) > (ELedge->c));
	 if (ELedge->b < 0.0) {
	    above = !above;
	 }
	 if (!above) {
	    fast = 1;
	 }
      }
      if (!fast) {
	 dxs = topsite->coord.x - (ELedge->reg[0])->coord.x;
	 above = (ELedge->b * (dxp * dxp - dyp * dyp)) < (dxs * dyp * (1.0 + 2.0 * dxp / dxs + ELedge->b * ELedge->b));
	 if (ELedge->b < 0.0) {
	    above = !above;
	 }
      }
   } else {
      yl = ELedge->c - ELedge->a * p->x;
      t1 = p->y - yl;
      t2 = p->x - topsite->coord.x;
      t3 = yl - topsite->coord.y;
      above = ((t1*t1) > ((t2 * t2) + (t3 * t3)));
   }
   return (ELpm == 0 ? above : !above);
}

class HEheap {
private:
   HalfEdge *PQhash;
   int PQmin;
   int PQcount;
   int PQhashsize;
public:
   HEheap(int);
   ~HEheap() { }
   void insert( HalfEdge*, Site2d*, Float, Float, Float, Float);
   void del( HalfEdge*, Float, Float, Float );
   int bucket( HalfEdge*, Float, Float, Float );
   inline int empty();
   void min( Pt2d& );
   HalfEdge* extractmin();
};

HEheap::HEheap( int size ) : PQhash(0), PQmin(0), PQcount(0), PQhashsize(size) {
   if (!(PQhash = (HalfEdge*)malloc(PQhashsize * sizeof *PQhash))) {
      std::cerr << "Error pdelaunay: HEheap::HEheap(int) : Insufficient memory" << std::endl;
   } else {
      for (int i = 0; i < PQhashsize; i++) {
	 PQhash[i].PQnext = NULL;
      }
   }
}

void HEheap::insert( HalfEdge *he, Site2d *v, Float offset, Float ymin, Float ymax, Float deltay ) {
   HalfEdge *last, *next;
   he->vertex = v;
   ++(v->refcnt);
   he->ystar = v->coord.y + offset;
   last = &PQhash[bucket(he, ymin, ymax, deltay)];
   while (((next = last->PQnext) != NULL) &&
	  (he->ystar > next->ystar ||
	   (he->ystar == next->ystar &&
	    v->coord.x > next->vertex->coord.x))) {
      last = next;
   }
   he->PQnext = last->PQnext;
   last->PQnext = he;
   PQcount++;
}

void HEheap::del( HalfEdge *he, Float ymin, Float ymax, Float deltay ) {
   HalfEdge *last;
   if (he->vertex) {
      last = &PQhash[bucket(he, ymin, ymax, deltay)];
      while (last->PQnext != he) {
	 last = last->PQnext;
      }
      last->PQnext = he->PQnext;
      PQcount--;
   }
}

int HEheap::bucket( HalfEdge *he, Float ymin, Float ymax, Float deltay ) {
   int bucket;
   if (he->ystar < ymin) {
      bucket = 0;
   } else if (he->ystar >= ymax) {
      bucket = PQhashsize-1;
   } else {
      bucket = (int)((he->ystar - ymin)/deltay * PQhashsize);
   }
   if (bucket < 0) {
      bucket = 0;
   }
   if (bucket >= PQhashsize) {
      bucket = PQhashsize-1;
   }
   if (bucket < PQmin) {
      PQmin = bucket;
   }
   return bucket;
}

int HEheap::empty() {
   return PQcount == 0;
}

void HEheap::min( Pt2d& pt ) {
   while (PQhash[PQmin].PQnext == NULL) {
      ++PQmin;
   }
   pt.x = PQhash[PQmin].PQnext->vertex->coord.x;
   pt.y = PQhash[PQmin].PQnext->ystar;
}

HalfEdge* HEheap::extractmin() {
   HalfEdge *curr;
   curr = PQhash[PQmin].PQnext;
   PQhash[PQmin].PQnext = curr->PQnext;
   PQcount--;
   return curr;
}

class EdgeList {
   public:
      int sqrt_nsites;
      int ELhashsize;
      HalfEdge **ELhash;
      Site2d *bottomsite;
      FreeList *hfl;
      HalfEdge *ELleftend;
      HalfEdge *ELrightend;
      int ntry;
      int totalsearch;
   public:
      EdgeList( int, Site2d* );
      HalfEdge* HEcreate( Edge*, int );
      HalfEdge* gethash( int );
      HalfEdge* leftbnd( Pt2d*, Float, Float);
      void insert( HalfEdge*, HalfEdge*);
      void del( HalfEdge* );
      HalfEdge* right( HalfEdge* );
      HalfEdge* left( HalfEdge* );
      Site2d* leftreg( HalfEdge* );
      Site2d* rightreg( HalfEdge* );
      HalfEdge* bottomedge() {
	 return ELhash[0];
      }
};

EdgeList::EdgeList( int size, Site2d* bottoms ) : sqrt_nsites(size), ELhashsize(2*size), ELhash(NULL), bottomsite(bottoms) {
   hfl = new FreeList(sizeof(HalfEdge), sqrt_nsites);
   if (!(ELhash = (HalfEdge**)malloc(sizeof(*ELhash)*ELhashsize))) {
      return;
   }
   for (int i = 0; i < ELhashsize; i++) {
      ELhash[i] = NULL;
   }
   ELleftend = HEcreate(NULL, 0);
   ELrightend = HEcreate(NULL, 0);
   ELleftend->ELleft = NULL;
   ELleftend->ELright = ELrightend;
   ELrightend->ELleft = ELleftend;
   ELrightend->ELright = NULL;
   ELhash[0] = ELleftend;
   ELhash[ELhashsize-1] = ELrightend;
}

HalfEdge* EdgeList::HEcreate( Edge *e, int pm ) {
   HalfEdge *answer;
   answer = (HalfEdge*)hfl->pop();
   answer->ELedge = e;
   answer->ELpm = pm;
   answer->PQnext = NULL;
   answer->vertex = NULL;
   answer->ELrefcnt = 0;
   return answer;
}

void EdgeList::insert( HalfEdge *lb, HalfEdge *ne ) {
   ne->ELleft = lb;
   ne->ELright = lb->ELright;
   (lb->ELright)->ELleft = ne;
   lb->ELright = ne;
}

/* Get entry from hash table, pruning any deleted nodes */
HalfEdge* EdgeList::gethash( int b ) {
   HalfEdge *he;
   if ((b < 0) || (b >= ELhashsize)) {
      return NULL;
   }
   he = ELhash[b];
   if ((he == NULL) || (he->ELedge != (Edge*)DELETED)) {
      return he;
   }
   /* Hash table points to deleted half edge.  Patch as necessary. */
   ELhash[b] = NULL;
   if ((--(he->ELrefcnt)) == 0) {
      hfl->push((FreeNode*)he);
   }
   return NULL;
}

HalfEdge* EdgeList::leftbnd( Pt2d *p, Float xmin, Float deltax ) {
   int i, bucket;
   HalfEdge *he;
   // Use hash table to get close to desired halfedge
   bucket = (int)((p->x - xmin) / deltax * ELhashsize);
   if (bucket < 0) bucket = 0;
   if (bucket >= ELhashsize) bucket = ELhashsize - 1;
   he = gethash(bucket);
   if  (he == NULL) {
      for (i = 1; 1; i++) {
	 if ((he = gethash(bucket-i)) != NULL) break;
	 if ((he = gethash(bucket+i)) != NULL) break;
      }
      totalsearch += i;
   }
   ntry++;
   // Now search linear list of halfedges for the corect one
   if (he == ELleftend || (he != ELrightend && he->right_of(p))) {
      do {
	 he = he->ELright ;
      } while (he != ELrightend && he->right_of(p));
      he = he->ELleft;
   } else { 
      do {
	 he = he->ELleft ;
      } while (he != ELleftend && !he->right_of(p)) ;
   }
   // Update hash table and reference counts
   if ((bucket > 0) && (bucket < ELhashsize-1)) {
      if (ELhash[bucket]) {
	 (ELhash[bucket]->ELrefcnt)--;
      }
      ELhash[bucket] = he;
      (ELhash[bucket]->ELrefcnt)++;
   }
   return he;
}

void EdgeList::del( HalfEdge *he ) {
   (he->ELleft)->ELright = he->ELright;
   (he->ELright)->ELleft = he->ELleft;
   he->ELedge = (Edge*)DELETED;
}

HalfEdge* EdgeList::right( HalfEdge *he ) {
   return he->ELright;
}

HalfEdge* EdgeList::left( HalfEdge *he ) {
   return he->ELleft;
}

Site2d* EdgeList::leftreg( HalfEdge *he ) {
   if (he->ELedge == NULL) {
      return bottomsite;
   }
   return (he->ELpm == 0 ? he->ELedge->reg[0] : he->ELedge->reg[1]);
}

Site2d* EdgeList::rightreg( HalfEdge * he ) {
   if (he->ELedge == NULL) {
      return bottomsite;
   }
   return (he->ELpm == 0 ? he->ELedge->reg[1] : he->ELedge->reg[0]);
}

class SetOfPoints;
EdgeList* VoronoiDelaunay(SetOfPoints&, Graph2d&);
void deref( Site2d* v, FreeList &sfl ) {
   if (--(v->refcnt) == 0) {
      sfl.push((FreeNode*)v);
   }
}

// --------------------------------------------------------------
// class SetOfPoints
// --------------------------------------------------------------
class SetOfPoints {
private:
   FreeList *sfl;
   Site2d *seeds;
   Long nseeds;
   Long niseeds;
   Long seedidx;
   Long sqrt_nseeds;
   Float xmin;
   Float xmax;
   Float ymin;
   Float ymax;
   Float deltax;
   Float deltay;
public:
   SetOfPoints(Long);                       // creation given the maximum number of seeds
   ~SetOfPoints();                          // destruction
   inline Site2d* nextone();                // select the next seed
   inline void putone(Float, Float, Long);    // add a point
   void sort();                             // sort the seeds by their x-coordinates
   Long freeSize() { return (Long)sqrt(niseeds+4.0); }  
   friend EdgeList* VoronoiDelaunay(SetOfPoints&, Graph2d&);
};

SetOfPoints::SetOfPoints(Long nbmxpts) : seeds(NULL), nseeds(nbmxpts), niseeds(0), seedidx(0) {
   if (!(seeds = (Site2d*)malloc(nbmxpts*sizeof(Site2d)))) {
      std::cerr << "Error pdelaunay: Insufficient memory" << std::endl;
      exit(0);
   } else {
      sqrt_nseeds = (Long)sqrt(nbmxpts+4.0);
   }
}

SetOfPoints::~SetOfPoints() {
   if ((seeds)) {
      free(seeds);
   }
}

Site2d* SetOfPoints::nextone() {
   if (seedidx < niseeds) {
      return &seeds[seedidx++];
   }
   return NULL;
}

void SetOfPoints::putone( Float x, Float y, Long sbr ) {
   seeds[niseeds].coord.x = x;
   seeds[niseeds].coord.y = y;
   seeds[niseeds].seednbr = sbr;
   seeds[niseeds].refcnt = 0;
   niseeds++;
}

int scomp( const void *vs1, const void *vs2 ) {
   const Pt2d *s1 = (const Pt2d*)vs1;
   const Pt2d *s2 = (const Pt2d*)vs2;
   if (s1->y < s2->y) {
      return -1;
   }
   if (s1->y > s2->y) {
      return 1;
   }
   if (s1->x < s2->x) {
      return -1;
   }
   if (s1->x > s2->x) {
      return 1;
   }
   return 0;
}

void SetOfPoints::sort() {
   qsort((void*)seeds, niseeds, sizeof(Site2d), scomp);
   xmin = seeds[0].coord.x;
   xmax = seeds[0].coord.x;
   for (Long i = 0; i < niseeds; ++i) {
      if (seeds[i].coord.x < xmin) {
	 xmin = seeds[i].coord.x;
      }
      if (seeds[i].coord.x > xmax) {
	 xmax = seeds[i].coord.x;
      }
   }
   ymin = seeds[0].coord.y;
   ymax = seeds[niseeds-1].coord.y;
   deltay = ymax - ymin;
   deltax = xmax - xmin;
}

void bisect( Edge *newedge, Site2d *s1, Site2d *s2 ) {
   Float dx, dy, adx, ady;
   newedge->reg[0] = s1;
   newedge->reg[1] = s2;
   ++(s1->refcnt);
   ++(s2->refcnt);
   newedge->ep[0] = newedge->ep[1] = NULL;
   dx = s2->coord.x - s1->coord.x;
   dy = s2->coord.y - s1->coord.y;
   adx = dx > 0 ? dx : -dx;
   ady = dy > 0 ? dy : -dy;
   newedge->c = s1->coord.x * dx + s1->coord.y * dy + (dx*dx + dy*dy) * 0.5F;
   if (adx > ady) {
      newedge->a = 1.0;
      newedge->b = dy/dx;
      newedge->c /= dx;
   } else {
      newedge->b = 1.0;
      newedge->a = dx/dy;
      newedge->c /= dy;
   }
}

Float distance(Site2d *s, Site2d *t) {
   register Float dx, dy;
   dx = s->coord.x - t->coord.x;
   dy = s->coord.y - t->coord.y;
   return sqrt(dx*dx + dy*dy);
}

void out_triple( Graph2d &gr, Site2d *s1, Site2d *s2, Site2d *s3 ) {
   GEdge* p;
   Long n1 = s1->seednbr, n2 = s2->seednbr, n3 = s3->seednbr;
   if (!(p = gr[n1]->Search(n2))) gr[n1]->Add(n2, 1.0);
   else p->weight += 1.0;
   if (!(p = gr[n1]->Search(n3))) gr[n1]->Add(n3, 1.0);
   else p->weight += 1.0;
   if (!(p = gr[n2]->Search(n1))) gr[n2]->Add(n1, 1.0);
   else p->weight += 1.0;
   if (!(p = gr[n2]->Search(n3))) gr[n2]->Add(n3, 1.0);
   else p->weight += 1.0;
   if (!(p = gr[n3]->Search(n2))) gr[n3]->Add(n2, 1.0);
   else p->weight += 1.0;
   if (!(p = gr[n3]->Search(n1))) gr[n3]->Add(n1, 1.0);
   else p->weight += 1.0;
}

Site2d* intersect(HalfEdge *el1, HalfEdge *el2, FreeList& fr) {
   Edge *e1, *e2, *e;
   HalfEdge *el;
   Float d, xint, yint;
   bool right_of_seed;
   Site2d *v;

   e1 = el1->ELedge;
   e2 = el2->ELedge;
   if ((e1 == (Edge*)NULL) || (e2 == (Edge*)NULL)) {
      return ((Site2d *)NULL);
   }
   if (e1->reg[1] == e2->reg[1]) {
      return ((Site2d *)NULL);
   }
   d = (e1->a * e2->b) - (e1->b * e2->a);
   if ((-1.0e-10 < d) && (d < 1.0e-10)) {
      return ((Site2d *)NULL);
   }
   xint = (e1->c * e2->b - e2->c * e1->b) / d;
   yint = (e2->c * e1->a - e1->c * e2->a) / d;
   if ((e1->reg[1]->coord.y < e2->reg[1]->coord.y) ||
       (e1->reg[1]->coord.y == e2->reg[1]->coord.y &&
	e1->reg[1]->coord.x < e2->reg[1]->coord.x)) {
      el = el1;
      e = e1;
   } else {
      el = el2;
      e = e2;
   }
   right_of_seed = (xint >= e->reg[1]->coord.x);
   if ((right_of_seed && (el->ELpm == 0)) || (!right_of_seed && (el->ELpm == 1))) {
      return ((Site2d*)NULL);
   }
   v = (Site2d*)fr.pop();
   v->refcnt = 0;
   v->coord.x = xint;
   v->coord.y = yint;
   return v;
}

EdgeList* VoronoiDelaunay( SetOfPoints& st, Graph2d& gr ) {
   FreeList sfl(sizeof(Site2d), st.freeSize());
   FreeList efl(sizeof(Edge), st.freeSize());
   Pt2d newintstar = {0, 0};
   Site2d *newseed = NULL, *bot, *top, *p, *tmp, *v;
   HalfEdge *lbnd, *rbnd, *llbnd, *rrbnd, *bisector;
   Edge *e;
   int pm, nedges = 0, nvertices = 0;
   HEheap heap(4*st.freeSize());
   EdgeList* edlist = new EdgeList(st.freeSize(), st.nextone());
   
   newseed = st.nextone();
   
   while (1) {
      if (!heap.empty()) {
	 heap.min(newintstar);
      }
      if (newseed && (heap.empty() || 
		      newseed->coord.y < newintstar.y ||
		      (newseed->coord.y == newintstar.y
		       && newseed->coord.x < newintstar.x))) {
	 lbnd = edlist->leftbnd(&(newseed->coord), st.xmin, st.deltax);
	 rbnd = edlist->right(lbnd);
	 bot = edlist->rightreg(lbnd);
	 
	 e = (Edge*)efl.pop();
	 bisect(e, bot, newseed);
	 e->edgenbr = nedges;
	 nedges++;
	 
	 bisector = edlist->HEcreate(e, 0);
	 edlist->insert(lbnd, bisector);
	 
 	 if ((p = intersect(lbnd, bisector, sfl))) {
	    heap.del(lbnd, st.ymin, st.ymax, st.deltay);
	    if (lbnd->vertex) {
	       deref(lbnd->vertex, sfl);
	       lbnd->vertex = NULL;
	    }
	    heap.insert(lbnd, p, distance(p, newseed), st.ymin, st.ymax, st.deltay);
	 }
	 
	 lbnd = bisector;
	 bisector = edlist->HEcreate(e, 1);
	 edlist->insert(lbnd, bisector);
	 
	 if ((p = intersect(bisector, rbnd, sfl))) {
	    heap.insert(bisector, p, distance(p, newseed), st.ymin, st.ymax, st.deltay);
	 }
	 newseed = st.nextone();
      } else if (!heap.empty()) {
	 lbnd = heap.extractmin();
	 llbnd = edlist->left(lbnd);
	 rbnd = edlist->right(lbnd);
	 rrbnd = edlist->right(rbnd);
	 bot = edlist->leftreg(lbnd);
	 top = edlist->rightreg(rbnd);
	 out_triple(gr, bot, top, edlist->rightreg(lbnd));
	 v = lbnd->vertex;
	 v->seednbr = nvertices++;
	 
	 ++(v->refcnt);
	 if (lbnd->ELedge->endpoint(lbnd->ELpm, v)) {
	    deref(lbnd->ELedge->reg[0], sfl);
	    deref(lbnd->ELedge->reg[1], sfl);
	    efl.push((FreeNode*)lbnd->ELedge);
	 }
	 
	 ++(v->refcnt);
	 if (rbnd->ELedge->endpoint(rbnd->ELpm, v)) {
	    deref(rbnd->ELedge->reg[0], sfl);
	    deref(rbnd->ELedge->reg[1], sfl);
	    efl.push((FreeNode*)rbnd->ELedge);
	 }
	 
	 edlist->del(lbnd);
	 heap.del(rbnd, st.ymin, st.ymax, st.deltay);
	 edlist->del(rbnd);
	 pm = 0;
	 if (bot->coord.y > top->coord.y) {
	    tmp = bot;
	    bot = top;
	    top = tmp;
	    pm = 1;
	 }
	 
	 e = (Edge*)efl.pop();
	 bisect(e, bot, top);
	 e->edgenbr = nedges;
	 nedges++;
	 
	 bisector = edlist->HEcreate(e, pm);
	 edlist->insert(llbnd, bisector);
	 
	 ++(v->refcnt);
	 if (e->endpoint(1-pm, v)) {
	    deref(e->reg[0], sfl);
	    deref(e->reg[1], sfl);
	    efl.push((FreeNode*)e);
	 }
	 deref(v, sfl);
	 
	 if ((p = intersect(llbnd, bisector, sfl))) {
	    heap.del(llbnd, st.ymin, st.ymax, st.deltay);
	    heap.insert(llbnd, p, distance(p, bot), st.ymin, st.ymax, st.deltay);
	 }
	 
	 if ((p = intersect(bisector, rrbnd, sfl))) {
	    heap.insert(bisector, p, distance(p, bot), st.ymin, st.ymax, st.deltay);
	 }
      } else {
	 break;
      }
   }
   return edlist;
}

Errc PDelaunay( const Reg2d &reg, Graph2d &gr ) {
   int x, y, W = reg.Width(), H = reg.Height();
   Long test = reg.Labels();
   gr.New(reg.Labels()+1, reg.Height(), reg.Width());
   SetOfPoints st(reg.Labels());
   
   // add the seeds to the set of points st
   for (y = 0; y < H; y++) {
      for (x = 0; x < W; x++) {
	 if ((reg[y][x])) {
	    if (!gr[reg[y][x]] && (gr.Add(reg[y][x], reg[y][x], y, x) == SUCCESS)) {
	       st.putone((Float)x, (Float)y, reg[y][x]);
	       if (test-- == 0) { 
		  std::cerr << "Error pdelaunay: Some seeds are not pixel" << std::endl; 
		  return FAILURE;
	       }
	    } else {
	       std::cerr << "Error pdelaunay: Several seeds with the same label" << std::endl; 
	       return FAILURE;
	    }
	 }
      }
   }
   st.sort();
   EdgeList *voredges = VoronoiDelaunay(st, gr);
   delete voredges;
   return SUCCESS;
}

#ifdef MAIN
#undef MAIN
#define EXMAIN
#endif

#include "segmentation/pvoronoi.cpp"
#ifdef EXMAIN
#define MAIN
#endif

Errc PDelaunay( const Reg3d &rgs, Graph3d &grd ) {
   Reg3d rgi(rgs.Size());
   Img3dsl imi(rgs.Size());
   
   return ((PVoronoi(rgs, rgi, imi)==SUCCESS) && (grd.Init(rgi, rgs) == SUCCESS));
}

#ifdef MAIN
#define USAGE	"usage: %s [-m mask] [rg_in|-] [gr_out|-]"
#define PARC    0
#define FINC    1
#define FOUTC   1
#define MASK    1
// --------------------------------------------------------------
int main( int argc, char *argv[] ) {
   Errc result;              // The result code of the execution.
   Pobject *mask;            // The mask.
   Pobject *objin[FINC+1];   // The input objects.
   Pobject *objs[FINC+1];    // The source objects masked by mask.
   Pobject *objout[FOUTC+1]; // The output object unmasked by mask.
   Pobject *objd[FOUTC+1];   // The result object.
   char* parv[PARC+1];     // The input parameters.
  
   ReadArgs(argc, argv, PARC, FINC, FOUTC, &mask, objin, objs, objout, objd, parv, USAGE, MASK);
   if (objs[0]->Type() == Po_Reg2d) {
	 Reg2d* const reg = (Reg2d*)objs[0];
	 objd[0] = new Graph2d;
	 Graph2d* const grd = (Graph2d*)objd[0];
	 result = PDelaunay(*reg, *grd);
	 goto end;
      }
   else if (objs[0]->Type() == Po_Reg3d) {
      Reg3d* const rgs = (Reg3d*)objs[0];
      objd[0]=new Graph2d;
      Graph3d* const grd = (Graph3d*)objd[0];
      result = PDelaunay(*rgs, *grd);
      goto end;
   }
   PrintErrorFormat(objin, FINC);
  end:
   if (result) {
      WriteArgs(argc, argv, PARC, FINC, FOUTC, &mask, objin, objs, objout, objd, MASK);   
   }
   Exit(result);
   return 0;
}
#endif
