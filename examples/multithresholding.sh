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
# @(#)multithresholding.sh
# 
# (C)Régis Clouard - 2006-09-06
#

TMPFILE="weszka.pan deravi.pan chanda.pan fisher.pan contrast.pan histo.pan mass.pan a.pan b.pan c.pan d.pan"

echo "- Performing tangram.pan segmentation using Weska multithresholding"
   pweszka 10 tangram.pan weszka.pan
   pvisu -nofork weszka.pan

echo "- Performing tangram.pan segmentation using Deravi multithresholding"
   pderavi 15 tangram.pan deravi.pan
   pvisu -nofork deravi.pan

echo "- Performing tangram.pan segmentation using Chanda multithresholding"
   pchanda 20 tangram.pan chanda.pan
   pvisu -nofork chanda.pan

echo "- Performing trangram.pan segmentation using fisher multithresholding"
   pfisher 0 2 tangram.pan fisher.pan
   pvisu -nofork fisher.pan

echo "- Performing trangram.pan segmentation using multithresholding based on contrast"
   pgradient 1 tangram.pan a.pan b.pan
   pnonmaximasuppression a.pan b.pan c.pan
   pthresholding 10 1e20 c.pan d.pan
   pcontrastthresholding 2 tangram.pan d.pan contrast.pan
   pvisu -nofork contrast.pan


echo "- Performing trangram.pan segmentation using hsitogram thresholding"
   phistothresholding 10 tangram.pan histo.pan
   pvisu -nofork histo.pan

echo "- Performing trangram.pan segmentation using pixel mnass thresholding"
   pmassthresholding 86 tangram.pan mass.pan
   pvisu -nofork mass.pan

rm $TMPFILE
