#!/bin/sh
#
# Copyright (c) 2013, GREYC.
# All rights reserved
#
# You may use this file under the terms of the BSD license as follows:
#
# "Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions are
# met:
#   * Redistributions of source code must retain the above copyright
#     notice, this list of conditions and the following disclaimer.
#   * Redistributions in binary form must reproduce the above copyright
#     notice, this list of conditions and the following disclaimer in
#     the documentation and/or other materials provided with the
#     distribution.
#   * Neither the name of the GREYC, nor the name of its
#     contributors may be used to endorse or promote products
#     derived from this software without specific prior written
#     permission.
#
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
# "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
# LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
# A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
# OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
# SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
# LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
# DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
# THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
# (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
# OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
#
# For more information, refer to:
# https://clouard.users.greyc.fr/Pandore/
#

#
# @(#)frequency.sh
# 
# (C)Régis Clouard - 2006-09-06
#

TMPFILE="square.pan empty.pan real.pan imaginary.pan modulus.pan phase.pan square1.pan empty1.pan square2.pan outsquare.pan i1.pan i2.pan i3.pan i4.pan i5.pan i6.pan i7.pan i8.pan"

echo "- Converting trangram.pan from spatial domain to frequency domain and reciproquely"

   pshapedesign 256 256 0 2 20 0 square.pan
   pshapedesign 256 256 0 0  0 0 empty.pan
   pfft square.pan empty.pan real.pan imaginary.pan
   pmodulus real.pan imaginary.pan modulus.pan
   pphase real.pan imaginary.pan phase.pan
   pifft real.pan imaginary.pan square1.pan empty1.pan
   plineartransform 0 0 255 square1.pan square2.pan
   pim2uc square2.pan outsquare.pan
   pvisu -nofork outsquare.pan

echo "- Performing Butterworth lowpass filtering on tangram.pan"
   psetcst 0 tangram.pan i1.pan
   pfft tangram.pan i1.pan i2.pan i3.pan
   pbutterworthfilter 256 256 0 0 0 50 2 i4.pan
   pmult i2.pan i4.pan i5.pan
   pmult i3.pan i4.pan i6.pan
   pifft i5.pan i6.pan i7.pan i8.pan
   pvisu -nofork i8.pan

echo "- Performing Butterworth highpass filtering on tangram.pan"
   psetcst 0 tangram.pan i1.pan
   pfft tangram.pan i1.pan i2.pan i3.pan
   pbutterworthfilter 256 256 0 1 0 50 2 i4.pan
   pmult i2.pan i4.pan i5.pan
   pmult i3.pan i4.pan i6.pan
   pifft i5.pan i6.pan i7.pan i8.pan
   pvisu -nofork i8.pan

echo "- Performing Butterworth bandreject filtering on tangram.pan"
   psetcst 0 tangram.pan i1.pan
   pfft tangram.pan i1.pan i2.pan i3.pan
   pbutterworthfilter 256 256 0 0 25 50 2 i4.pan
   pmult i2.pan i4.pan i5.pan
   pmult i3.pan i4.pan i6.pan
   pifft i5.pan i6.pan i7.pan i8.pan
   pvisu -nofork i8.pan

echo "- Performing Butterworth bandpass filtering on tangram.pan"
   psetcst 0 tangram.pan i1.pan
   pfft tangram.pan i1.pan i2.pan i3.pan
   pbutterworthfilter 256 256 0 1 25 50 2 i4.pan
   pmult i2.pan i4.pan i5.pan
   pmult i3.pan i4.pan i6.pan
   pifft i5.pan i6.pan i7.pan i8.pan
   pvisu -nofork i8.pan

echo "- Performing Gaussian lowpass filtering on tangram.pan"
   psetcst 0 tangram.pan i1.pan
   pfft tangram.pan i1.pan i2.pan i3.pan
   pgaussianfilter 256 256 0 0 0 100 i4.pan
   pmult i2.pan i4.pan i5.pan
   pmult i3.pan i4.pan i6.pan
   pifft i5.pan i6.pan i7.pan i8.pan
   pvisu -nofork i8.pan

echo "- Performing Gaussian highpass filtering on tangram.pan"
   psetcst 0 tangram.pan i1.pan
   pfft tangram.pan i1.pan i2.pan i3.pan
   pgaussianfilter 256 256 0 1 0 50 i4.pan
   pmult i2.pan i4.pan i5.pan
   pmult i3.pan i4.pan i6.pan
   pifft i5.pan i6.pan i7.pan i8.pan
   pvisu -nofork i8.pan

echo "- Performing Gaussian bandreject filtering on tangram.pan"
   psetcst 0 tangram.pan i1.pan
   pfft tangram.pan i1.pan i2.pan i3.pan
   pgaussianfilter 256 256 0 0 25 50 i4.pan
   pmult i2.pan i4.pan i5.pan
   pmult i3.pan i4.pan i6.pan
   pifft i5.pan i6.pan i7.pan i8.pan
   pvisu -nofork i8.pan

echo "- Performing Gaussian bandpass filtering on tangram.pan"
   psetcst 0 tangram.pan i1.pan
   pfft tangram.pan i1.pan i2.pan i3.pan
   pgaussianfilter 256 256 0 1 25 50 i4.pan
   pmult i2.pan i4.pan i5.pan
   pmult i3.pan i4.pan i6.pan
   pifft i5.pan i6.pan i7.pan i8.pan
   pvisu -nofork i8.pan

rm -f $TMPFILE
