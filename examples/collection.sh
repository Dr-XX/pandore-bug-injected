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
# @(#)collection.sh
# 
# (C)Régis Clouard - 2006-09-06
#


TMPFILE="col.pan c1.pan c2.pan image1.pan image2.pan"

echo "- Performing some tests on collection"

  pobject2col foo tangram.pan col.pan
echo "      1/ adding tangram.pan in the collection"
  pfile col.pan 

echo "      2/ renaming item"
  pcolrenameitem foo bar col.pan col.pan
  pfile col.pan 

echo "      3/ getting value 10.5"
  pcolsetvalue foo Float 10.5 col.pan col.pan
  pcolgetvalue foo col.pan 
  pstatus

echo "      4/ displaying collection contents"
  pcol2txt col.pan

echo "      5/ adding 2 images in the collection"
   pobject2col image1 tangram.pan c1.pan
   pobject2col image2 butterfly.pan c2.pan
   pcolcatenateitem c1.pan c2.pan col.pan
   pcolgetimages col.pan
   pfile col.pan

echo "      6/ checking resulting images"
   pfile image1.pan
   pfile image2.pan

rm $TMPFILE
