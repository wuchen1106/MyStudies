#!/bin/bash
for Target in "g40cm10mm" "g50cm10mm" "g30cm10mm" "t16cm6mm" "g60cm6mm170gcm3"
do
	for app in "A" "H" "cg4" "g4s"
	do
		for phys in "QB" "QBH" "original" "modified" "nomuec" "QB49302" "QB49201"
		do
#			for pname in "em" "mum" "n0" "pim"
			for pname in "all"
			do
				for monitor in "PTACS" "MT1"
				do
					if [ $monitor = "PTACS" ]; then
						rootfile="result/"$monitor".$pname."$Target"."$app"."$phys".root"
						rootdir="../../result/$Target"
						if [ -e $rootfile ]; then
							mv $rootfile $rootdir
						fi
					elif [ $monitor = "MT1" ]; then
						for DF in "003T" "0018T"
						do
							rootfile="result/"$monitor".$pname."$Target"."$DF"."$app"."$phys".root"
							rootdir="../../result/$Target"
							if [ -e $rootfile ]; then
								mv $rootfile $rootdir
							fi
						done
					fi
				done
			done
		done
	done
done

