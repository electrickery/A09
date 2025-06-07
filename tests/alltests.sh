#!/bin/sh
#

DIR=`pwd`
for TEST in macroTest nopTest fccTest fcchTest
do
#  pushd $TEST
  cd $TEST
  sh ./test.sh
#  popd
  cd $DIR
done
