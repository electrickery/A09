#!/bin/sh
#

EXE=../../a09

BASE=`echo $1 | sed -e 's/\.asm$//'`

$EXE $BASE.asm -Sout/$BASE.s19 -Lout/$BASE.lst  -B -W


