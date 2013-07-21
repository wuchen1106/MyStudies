#!/bin/bash

for configName in "g60cm6mm" "t16cm6mm"
do
	for runname in "Andy" "Chen" "Hayashi" "QGSPBERT" "QGSPBERTHP"
	do
		rootfile="result/"$configName"*_"$runname"_output.root"
		pdffile="result/"$configName"*_"$runname".pdf"
		directory="../../result/$configName/$runname/result/"
		cp $rootfile $directory
		cp $pdffile $directory
	done
done
