#!/bin/bash

for configName in "g60cm6mm" "t16cm6mm"
do
	for runname in "Andy" "Chen" "Hayashi" "QGSPBERT" "QGSPBERTHP" "original" "real" "QGSP_BERT" "nomuec"
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
