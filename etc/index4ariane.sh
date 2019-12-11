#!/bin/sh
#
# PANTHEON Project
# 
# GREYC IMAGE
# 6 Boulevard Maréchal Juin
# F-14050 Caen Cedex France
#
# This file is free software. You can use it, distribute it
# and/or modify it. However, the entire risk to the quality
# and performance of this program is with you.
#
# For more information, refer to:
# https://clouard.users.greyc.fr/Pandore
#

#
# @author Régis Clouard - 2004-08-13
# @author Régis Clouard - 2005-09-27 (add boxes)
# 

#
# @file index4ariane.sh
#
# Build the configuration file `ariane.cfg' for Ariane
#

echo
echo "###########  Generate configuration file for Ariane ##############"
echo


# Get pandorehome directory.
PANDOREHOME=${PANDOREHOME:-"../"}

#
# List of operators to discard.
DISCARDLIST=statut

PANDORECFG=$PANDOREHOME/lib/ariane.cfg

version=`cat ../VERSION.txt|awk '{print $1}'`

# Find flavor for echoing a string without a NEWLINE
# (ew_c: echo with \c, ew_n: echo with -n, ew_t: tabulation)
if (echo "example\c"; echo 1,2,3) | grep c >/dev/null; then 
  if (echo -n example; echo 1,2,3) | sed s/-n/xn/ | grep xn >/dev/null; then 
    ew_n= ew_c=' 
' ew_t='        ' 
  else
    ew_n=-n ew_c= ew_t=
  fi 
else 
  ew_n= ew_c='\c' ew_t=
fi

##
# Prologue
cat > $PANDORECFG<<EOF
<?xml version="1.0" encoding="ISO-8859-1"?>
<!DOCTYPE  Library [
]>

<ARIANE ariane.version="1.0">
<Library>
EOF

echo "  <Version>Pandore $version</Version>" >> $PANDORECFG
echo "  <DocPath>/doc/operators</DocPath>" >> $PANDORECFG
echo "  <Index>operatorsP0.html</Index>" >> $PANDORECFG
echo "  <CommandStatus>pstatus</CommandStatus>" >> $PANDORECFG
echo "  <CommandVisu>pvisu</CommandVisu>" >> $PANDORECFG

#
# Parse boxes operators
#
BOXE=$PANDOREHOME/boxes
for d in $BOXE/*
do
if [ ! -d $d ]
then
continue;
fi
dir=`basename $d`

echo $ew_n "adding operators for boxe $dir "": $ew_c"

for s in ".cc" ".cpp"
do
for i in $BOXE/$dir/src/*$s
do
    name=`basename $i $s`
    if [ "$name" = '*' ]; then continue;fi
    proto=`$PANDOREHOME/bin/$name -p`
    if [ -z "$proto" ]
    then continue
    fi
    signature=`$PANDOREHOME/bin/$name -s`
    parc=`echo $proto | awk -F' ' '{print  $2}'`
    finc=`echo $proto | awk -F' ' '{print  $3}'`
    foutc=`echo $proto | awk -F' ' '{print  $4}'`
    echo $ew_n " $name""$ew_c"
    echo "  <Operator>" >> $PANDORECFG
    echo "    <Category>$dir</Category>">> $PANDORECFG
    echo "    <Nom>$name</Nom>">> $PANDORECFG
    echo "    <Documentation>/$dir/$name.html</Documentation>">> $PANDORECFG
    echo "    <Binary>$name</Binary>">> $PANDORECFG
    echo "    <Parameters>$parc</Parameters>" >> $PANDORECFG
    if test -n $signature
    then echo "    <Signature>$signature</Signature>">> $PANDORECFG
    fi
    echo "    <Inputs>$finc</Inputs>" >> $PANDORECFG
    echo "    <Outputs>$foutc</Outputs>">> $PANDORECFG
    echo "  </Operator>" >> $PANDORECFG
done
done
echo "."
done

#
# Parse each operators.
#
BOXE=$PANDOREHOME/src
for d in $BOXE/operatorsP*
do
dir=`basename $d`
echo $ew_n "adding operators for boxe $dir "": $ew_c"
for s in ".cc" ".cpp"
do
for i in $BOXE/$dir/*/*$s
do
    name=`basename $i $s`
    if [ "$name" = '*' ]; then continue;fi
    proto=`$PANDOREHOME/bin/$name -p`
    if [ -z "$proto" ]
    then continue
    fi

    signature=`$PANDOREHOME/bin/$name -s`
    parc=`echo $proto | awk -F' ' '{print  $2}'`
    finc=`echo $proto | awk -F' ' '{print  $3}'`
    foutc=`echo $proto | awk -F' ' '{print  $4}'`
    echo $ew_n " $name""$ew_c"
    a=`dirname $i`
    echo "  <Operator>" >> $PANDORECFG
    echo "    <Category>`basename $a`</Category>">> $PANDORECFG
    echo "    <Nom>$name</Nom>">> $PANDORECFG
    echo "    <Documentation>/$dir/$name.html</Documentation>">> $PANDORECFG
    echo "    <Binary>$name</Binary>">> $PANDORECFG
    echo "    <Parameters>$parc</Parameters>" >> $PANDORECFG
    if test $parc -gt 0
    then echo "    <Signature>$signature</Signature>">> $PANDORECFG
    fi
    echo "    <Inputs>$finc</Inputs>" >> $PANDORECFG
    echo "    <Outputs>$foutc</Outputs>">> $PANDORECFG
    echo "  </Operator>" >> $PANDORECFG
done
done
echo "."
done

##
# Epilogue
cat >> $PANDORECFG<<EOF
</Library>
</ARIANE>
EOF

#Kept for sake of backwards compatibility
cp $PANDORECFG $PANDOREHOME/lib/pandore.cfg
