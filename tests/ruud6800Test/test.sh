#!/bin/sh

. ../config.sh

# not the .asm extension
TESTFILE=TEST6800N.a68

FILES="$TESTFILE.lst $TESTFILE.s19"

rm -f out/*
mkdir -p out

$EXE $TESTFILE.asm -Sout/$TESTFILE.s19 -Lout/$TESTFILE.lst > out/a09out.log 2>&1

DIFFOUND=0
for FILE in $FILES
do
#    diff -w out/$FILE ref/$FILE >> out/$LOG
    OUTTEMP=`mktemp /tmp/test_out_temp.XXXXXX`
    REFTEMP=`mktemp /tmp/test_ref_temp.XXXXXX`
    sed -f sedfile.txt out/$FILE > $OUTTEMP
    sed -f sedfile.txt ref/$FILE > $REFTEMP
    diff -w $OUTTEMP $REFTEMP >> out/$LOG
    if [ $? != 0 ] 
    then
        DIFFOUND=-1
        echo "Difference in $FILE"
    else
        rm -f $OUTTEMP $REFTEMP
    fi
done

if [ $DIFFOUND = -1 ] 
then
    head out/$LOG
    echo ....`wc -l out/$LOG` lines
fi
