#!/bin/bash

for configName in "g60cm6mm_170gcm3" "g60cm6mm_200gcm3" "t16cm6mm"
do
	for runname in "Andy" "Hayashi" "QGSPBERT" "QGSPBERTHP" "original" "modified" "nomuec" "QGSPBERT49302"
	do
		rootfile="result/"$configName"*_"$runname"_output.root"
		pdffile="result/"$configName"*_"$runname".pdf"
		pngfile="result/"$configName"*_"$runname".png"
		directory="../../result/$configName/$runname/result/"
		mv $rootfile $directory
		mv $pdffile $directory
		mv $pngfile $directory
	done
done
