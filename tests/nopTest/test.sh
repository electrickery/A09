#!/bin/sh

TESTFILE=noptest

FILES="$TESTFILE.lst"

rm -f *.log

BASE=`echo $TESTFILE | sed -e 's/\.asm$//'`

./asm09.sh $BASE.asm -Sout/$BASE.s19 -Lout/$BASE.lst  -B -W > test.log 2>&1


for FILE in $FILES
do
    diff -w out/$FILE ref/$FILE >> test.log
    if [ $? != 0 ] 
    then
        echo "Difference in $FILE"
        cat test.log
    fi
done
