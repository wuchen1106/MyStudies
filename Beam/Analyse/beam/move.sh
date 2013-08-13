#!/bin/bash

DestDir="/scratchfs/bes/wuc/MyWorkArea/g4simData"
for name in result/*.root
do
	DestName=`echo $name | sed "s/result\/\(.*\)\.root/\1.ref.root/g"`
	mv $name $DestDir/$DestName
done
