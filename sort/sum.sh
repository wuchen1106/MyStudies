#!/bin/bash
for dir in "140603" "140613"
do
	if [ $dir = "140603" ]; then
		names="AfterBending_BM2"
	elif [ $dir = "140613" ]; then
		names="BeamMonitor_EndPlate BeamMonitor_OuterCDC BeamMonitor_InnerCDC BeamMonitor_EndCDC BeamMonitor_BeginCDC BeamMonitor_AND"
	fi
	for run in `echo $names`
	do
		./getNumber -d $dir -r $run
		if [ $run == "AfterBending_BM2" ]; then
			./getNumber -d $dir -r $run -m 18
		fi
	done
done
