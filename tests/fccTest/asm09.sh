#!/bin/sh
#

EXE=../../a09

BASE=`echo $1 | sed -e 's/\.asm$//'`

#$EXE -S out/$BASE.s19  -F out/$BASE.flx -L out/$BASE.lst $BASE.asm 

$EXE  $BASE.asm 

