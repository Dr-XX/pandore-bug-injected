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
# @(#)regionmerging.sh
# 
# (C)Régis Clouard - 2006-09-06
#

TMPFILE="a.pan b.pan c.pan d.pan mean.pan var.pan"

echo "- Performing tangram.pan segmentation using quadtree division."
pcontrastquadtree 30 tangram.pan a.pan
prg2gr a.pan b.pan
pvisu -nofork a.pan

pmcmfiltering 10 tangram.pan c.pan
# On fusionne la carte de regions
echo "- Performing tangram.pan segmentation using region merge based on mean criterion homogeneity"
pmeanmerging -2 20 a.pan b.pan c.pan mean.pan d.pan
pvisu -nofork mean.pan

echo "- Performing tangram.pan segmentation using region merge based on variance criterion homogeneity"
pvariancemerging -2 120 a.pan b.pan tangram.pan var.pan d.pan
pvisu -nofork var.pan

rm $TMPFILE