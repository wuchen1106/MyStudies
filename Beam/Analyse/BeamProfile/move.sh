#!/bin/bash

#for Target in "g40cm10mm" "g50cm10mm" "g30cm10mm" "t16cm6mm" "g60cm6mm170gcm3"
for Target in "g40cm10mm" "g50cm10mm" "g30cm10mm" "t16cm6mm"
do
	for app in "A" "H" "cg4" "g4s"
	do
		for phys in "QB" "QBH" "original" "modified" "nomuec" "QB49302" "QB49201"
		do
			for monitor in "PTACS" "MT1"
			do
				if [ $monitor = "PTACS" ]; then
					rootfile="result/"$monitor"_*_"$Target"_"$app"_"$phys".root"
					profile="result/"$monitor"_*_"$Target"_"$app"_"$phys".*"
					directroy="../../result/$Target/$app/$phys/$monitor"
					mv $rootfile $directroy
					mv $profile $directroy
				elif [ $monitor = "MT1" ]; then
					for DF in "03T" "018T"
					do
						rootfile="result/"$monitor"_*_"$Target"_"$DF"_"$app"_"$phys".root"
						profile="result/"$monitor"_*_"$Target"_"$DF"_"$app"_"$phys".*"
						directroy="../../result/$Target/$app/$phys/$monitor/$DF"
						mv $rootfile $directroy
						mv $profile $directroy
					done
				fi
			done
		done
	done
done
