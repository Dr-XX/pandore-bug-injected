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
 * Data structures.
 */

#ifndef _EVALUATION_H
#define _EVALUATION_H

#include <stdexcept>

struct RuntimeException : public std::exception {
      RuntimeException( std::string message ) : _message(message) {}
      ~RuntimeException() throw () { }
      
      const char* what() const throw() { return _message.c_str(); }
   private:
      std::string _message;
};

struct DiscrepancyError {
      float value;
      float normalization;

      DiscrepancyError(): value(0.0F), normalization(0.0F) { }
      DiscrepancyError( const float v, const float n ) : value(v), normalization(n) { }
};

struct DiscrepancyIndicator {
      DiscrepancyError errors[2];
      
      DiscrepancyIndicator() { }
};

struct DiscrepancyResult {
   DiscrepancyIndicator indicators[5];
};

struct EvaluationPerformance {
      float indicators[5];
};

struct Point {
   Point( int y, int x ): x(x), y(y) { }
   int x;
   int y;
};

template< typename T >
class LabelImage {
   public:
      LabelImage(): _data(0) { }
      LabelImage( int width, int height, int labels, T * vector ): _width(width), _height(height), _labels(labels) {
	 _data = vector;
      }
      LabelImage( const LabelImage &image ): _width(image.Width()), _height(image.Height()), _labels(image.Labels()) {
	 _data = new T[_width * _height];
      }
      ~LabelImage() {
	 if (_data != 0) {
	    delete[] _data;
	 }
      }
      void New( int width, int height ) {
	 _width = width;
	 _height = height;
	 _labels = 0;
	 _data = new T[_width * _height];
      }
   
      void operator=( T value ) {
	 for (T* ptr = _data; ptr < _data + VectorSize(); *(ptr++) = value ) ;
      }
      unsigned int VectorSize() const {
	 return  _width * _height;
      }
      unsigned int Labels() const {
	 return _labels;
      }
      long Height() const {
	 return _height;
      } 
      long Width() const {
	 return _width;
      }
      bool Hold( long y, long x ) const {
	 return (y >= 0) && (y < _height) && (x >= 0) && (x < _width);
      }
      T operator() ( int y, int x ) const {
	 return _data[y * _width + x];
      }
      T & operator() ( int y, int x ) {
	 return _data[y * _width + x];
      }
      void Labels( int labels ) {
	 _labels = labels;
      }
      T * Vector() const {
	 return _data;
      }
      
   private:
      int _width;
      int _height;
      int _labels;
      T * _data;
};

#endif

