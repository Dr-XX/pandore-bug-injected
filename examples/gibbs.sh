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
# @(#)gibbs.sh
# 
# (C)Ludovic Soltys -  02-02-2003
#

TMPFILE="initial.pan b.pan c.pan d1.pan d2.pan d3.pan d4.pan e2.pan e3.pan e4.pan f.pan gibbsout.pan"

echo "- Building a synthetic image to illustrate the Gibbs phenomenon in wavelets analysis"
pshapedesign 256 256 0 2 150 150 initial.pan
pvisu -nofork initial.pan
pqmf daubechies 4 b.pan
pdwt 1 initial.pan b.pan c.pan
psplitimage c.pan d1.pan d2.pan d3.pan d4.pan
pthresholding 20 400 d2.pan e2.pan
pthresholding 20 400 d3.pan e3.pan
pthresholding 20 400 d4.pan e4.pan
pmergeimages d1.pan e2.pan e3.pan e4.pan f.pan
pidwt 1 f.pan b.pan gibbsout.pan
pvisu -nofork gibbsout.pan

rm $TMPFILE
