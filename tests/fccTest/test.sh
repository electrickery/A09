#!/bin/sh

TESTFILE=fcctest
EXE=../../a09

FILES="$TESTFILE.lst $TESTFILE.s19"
LOG=test.log

rm -f *.log out/*

BASE=`echo $TESTFILE | sed -e 's/\.asm$//'`

#./asm09.sh $BASE.asm -Sout/$BASE.s19 -Lout/$BASE.lst  -B -W > test.log 2>&1
$EXE -sout/$BASE.s19 -lout/$BASE.lst  $BASE.asm > test.log 2>&1


for FILE in $FILES
do
    diff -w out/$FILE ref/$FILE >> $LOG
    if [ $? != 0 ] 
    then
        echo "Difference in $FILE"
        cat $LOG
    fi
done
