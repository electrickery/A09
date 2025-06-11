#!/bin/sh
#

DIR=`pwd`
for TEST in macroTest nopTest fccTest fcchTest ruud6800Test ruud6809Test mc68hc11Test hm6309ExtraTest directiveTest
do
#  pushd $TEST
  cd $TEST
  sh ./test.sh
#  popd
  cd $DIR
done
