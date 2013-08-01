#!/bin/bash

mode=$1
runName=$2

./FieldProfile -m $mode -x $mode'_'$runName
mv result/* ../../result/$mode'_'$runName/result
