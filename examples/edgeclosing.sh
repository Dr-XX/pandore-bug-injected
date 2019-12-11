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
# @(#)edgeclosing.sh
# 
# (C)Régis Clouard - 2006-09-06
#

TMPFILE="b.pan c.pan d.pan e.pan f.pan g.pan i.pan j.pan h.pan extensionconic_out.pan extensionrect_out.pan out.pan"

echo "- Performing edge closing using iconic extension of the end points"
   psobel tangram.pan b.pan
   pbinarization 55 1e30 b.pan c.pan
   pskeletonization c.pan d.pan 
   ppostthinning d.pan e.pan
   pcontourextensionconic 4 e.pan extensionconic_out.pan
   pvisu -nofork extensionconic_out.pan

echo "- Performing edge closing using rectangular extension of the end points"
   pcontourextensionrect 2 4 e.pan extensionrect_out.pan
   pvisu -nofork  extensionrect_out.pan

echo "- Performing edge closing using distance to nearest contour"
   psobel tangram.pan b.pan
   pbinarization 50 1e30 b.pan c.pan
   pskeletonization c.pan d.pan 
   ppostthinning d.pan e.pan
   pdistance e.pan f.pan
   plocalmaxima 8 f.pan g.pan
   plabeling 8 g.pan h.pan
   pinverse f.pan i.pan
   pwatershed h.pan i.pan j.pan

   psobel tangram.pan b.pan
   pbinarization 34 1e30 b.pan c.pan
   pskeletonization c.pan d.pan 
   pboundarylabeling d.pan out.pan
   pvisu -nofork out.pan
rm -f $TMPFILE