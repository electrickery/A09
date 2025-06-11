#!/bin/sh

. ../config.sh

TESTFILE=fcchtest

FILES="$TESTFILE.lst $TESTFILE.s19"

rm -f out/*
mkdir -p out

$EXE $TESTFILE.asm -Sout/$TESTFILE.s19 -Lout/$TESTFILE.lst > out/a09out.log 2>&1

DIFFOUND=0
for FILE in $FILES
do
    diff -w out/$FILE ref/$FILE >> out/$LOG
    if [ $? != 0 ] 
    then
        DIFFOUND=-1
        echo "Difference in $FILE"
    fi
done

if [ $DIFFOUND = -1 ] 
then
    cat out/$LOG
fi
