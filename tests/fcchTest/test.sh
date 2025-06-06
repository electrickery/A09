#!/bin/sh

TESTFILE=fcchtest
EXE=../../a09

FILES="$TESTFILE.lst $TESTFILE.s19"
LOG=test.log

rm -f *.log out/*

BASE=`echo $TESTFILE | sed -e 's/\.asm$//'`

$EXE -sout/$BASE.s19 -lout/$BASE.lst $BASE.asm > $LOG 2>&1


for FILE in $FILES
do
    diff -w out/$FILE ref/$FILE >> $LOG
    if [ $? != 0 ] 
    then
        echo "Difference in $FILE"
        cat $LOG
    fi
done
