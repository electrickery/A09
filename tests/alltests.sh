#!/bin/sh
#
for TEST in macroTest nopTest fccTest fcchTest
do
  pushd $TEST
  sh ./test.sh
  popd
done
