#!/bin/bash

for Target in "g40cm10mm" "g50cm10mm" "g30cm10mm" "t16cm6mm" "g60cm6mm170gcm3"
do
	for app in "A" "H" "cg4" "g4s"
	do
		for phys in "QB" "QBH" "original" "modified" "nomuec" "QB49302" "QB49201"
		do
			for monitor in "PTACS" "MT1"
			do
				if [ $monitor = "PTACS" ]; then
					rootfile="result/"$monitor".*."$Target"."$app"."$phys".root"
					rootdir="../../result"
					profile="result/"$monitor".*."$Target"."$app"."$phys".*"
					profdir="../../result/$Target/$app/$phys/$monitor"
					mv $rootfile $rootdir
					mv $profile $profdir
				elif [ $monitor = "MT1" ]; then
					for DF in "03T" "018T"
					do
						rootfile="result/"$monitor".*."$Target"."$DF"."$app"."$phys".root"
						rootdir="../../result"
						profile="result/"$monitor".*."$Target"."$DF"."$app"."$phys".*"
						profdir="../../result/$Target/$app/$phys/$monitor/$DF"
						mv $rootfile $rootdir
						mv $profile $profdir
					done
				fi
			done
		done
	done
done
