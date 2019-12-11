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
# @(#)snrmeasures.sh
# 
# (C)Régis Clouard - 2006-09-06
#

TMPFILE="a.pan i1.pan i2.pan"

echo "- Computing the PSNR after a meanfilter smoothing on image "
echo "  with artificial gaussian noise (mean=0; std-dev=1.5):"
echo
paddnoise 1 0 1.5 tangram.pan a.pan
pmeanfiltering 2 a.pan i1.pan
pim2uc i1.pan i2.pan
ppsnr 255 tangram.pan i2.pan
a=`pstatus`
echo "   The PSNR (Peak Signal to Noise Ration) = $a"
pmse tangram.pan i2.pan
a=`pstatus`
echo "   The MSE (Mean Square Error) = $a"
psnr tangram.pan i2.pan
a=`pstatus`
echo "   The SNR (Signal-to-Noise Ratio) = $a"

rm $TMPFILE
