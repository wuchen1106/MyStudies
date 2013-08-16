#!/bin/bash
#
#DestDir="/scratchfs/bes/wuc/MyWorkArea/g4simData"
#for name in result/*.root
#do
#	DestName=`echo $name | sed "s/result\/\(.*\)\.root/\1.ref.root/g"`
#	mv $name $DestDir/$DestName
#done

for Target in "g40cm10mm" "g50cm10mm" "g30cm10mm" "t16cm6mm"
do
#	for app in "A" "H" "cg4" "g4s"
	for app in "g4s"
	do
#		for phys in "QB" "QBH" "original" "modified" "nomuec" "QB49302" "QB49201"
		for phys in "QBH"
		do
#			for monitor in "MT1" "PTACS" "McTruth" "A9";
			for monitor in "A9";
			do
#				for pid in -11 -13 211 2212 -2212 22 11 13 -211 2112;
				for pid in 13 -211
				do
					if [ $pid = 11 ]; then pname="em";
					elif [ $pid = 13 ]; then pname="mum";
					elif [ $pid = -211 ]; then pname="pim";
					elif [ $pid = 2112 ]; then pname="n0";
					elif [ $pid = -11 ]; then pname="ep";
					elif [ $pid = -13 ]; then pname="mup";
					elif [ $pid = 211 ]; then pname="pip";
					elif [ $pid = 2212 ]; then pname="pp";
					elif [ $pid = -2212 ]; then pname="pm";
					elif [ $pid = 22 ]; then pname="gamma";
					elif [ $pid = 0 ]; then pname="all";
					elif [ $pid = 1 ]; then pname="EP";
					elif [ $pid = -1 ]; then pname="NU";
					fi
					if [ $monitor = "MT1" ]; then
						for DF in "003T" "0018T" "003TS" "0018TS"
						do
							name=result/$monitor.$pname.$Target.$DF.$app.$phys
							dir=../../result/$Target/$app/$phys/$monitor/$DF
							if [ -e $name.root ]; then
								mv $name.root $dir
							else
								echo $name.root does not exist
							fi
						done
					elif [ $monitor = "A9" ]; then
						for DF in "03T"
						do
							for A9 in "0731" "0701"
							do
								name=result/$monitor.$pname.$Target.$DF.$A9.$app.$phys
								prefix=result/$monitor'_'$pname
								suffix='_'$Target'_'$DF'_'$A9'_'$app'_'$phys
								dir=../../result/$Target/$app/$phys/$monitor/$DF/$A9
								if [ -e $name.root ]; then
									mv $name.root $dir
									mv $prefix'_'*$suffix.* $dir
								else
									echo $name.root does not exist
								fi
							done
						done
					elif [ $monitor = "PTACS" ]; then
						name=result/$monitor.$pname.$Target.$app.$phys
						dir=../../result/$Target/$app/$phys/$monitor
						if [ -e $name.root ]; then
							mv $name.root $dir
						else
							echo $name.root does not exist
						fi
					fi
				done
			done
		done
	done
done
