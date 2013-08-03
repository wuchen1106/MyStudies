#!/bin/bash

#for configName in "g40cm10mm182gcm3" "g60cm6mm_170gcm3" "g60cm6mm_200gcm3" "t16cm6mm" "t16cm6mm_03T"
for configName in "g40cm10mm182gcm3"
do
#	for runname in "Andy" "Hayashi" "QGSPBERT" "QGSPBERTHP" "original" "modified" "nomuec" "QGSPBERT49302" "QGSPBERT49201" "QGSPBERTg4sim" "QGSPBERTg4sim_NK"
	for runname in "QGSPBERT"
	do
		if [ $runname == "Andy" ]; then
			nEvents=1000000
		elif [ $runname == "Hayashi" ]; then
			nEvents=100000
		elif [ $runname == "modified" ]; then
			if [ $configName = "g60cm6mm_200gcm3" -o $configName = "t16cm6mm" ]; then
				nEvents=15000000
			else
				nEvents=1000000
			fi
		elif [ $runname == "QGSPBERT" ]; then
			if [ $configName = "g60cm6mm_200gcm3" -o $configName = "t16cm6mm" ]; then
				nEvents=250000
			elif [ $configName = "g40cm10mm182gcm3" ]; then
				nEvents=15000000
			else
				nEvents=1000000
			fi
		elif [ $runname == "QGSPBERTHP" ]; then
			if [ $configName = "g60cm6mm_200gcm3" -o $configName = "t16cm6mm" ]; then
				nEvents=250000
			else
				nEvents=1000000
			fi
		elif [ $runname == "QGSPBERT49302" ]; then
			if [ $configName = "g60cm6mm_170gcm3" ]; then
				nEvents=990000
			else
				nEvents=0
			fi
		else
			nEvents=1000000
		fi
		for monitor in "ts2_0" "blt1" "blt0" "ptacs_beampipe" "ptacs_shielding";
		do
			for pname in "em" "mum" "pim" "n0"
			do
				if [ $pname = em ]; then pname_inTitle="e^{-}";
				elif [ $pname = mum ]; then pname_inTitle="#mu^{-}";
				elif [ $pname = pim ]; then pname_inTitle="#pi^{-}";
				elif [ $pname = n0 ]; then pname_inTitle="n_{0}";
				fi
				rootfile="../../result/$configName/$runname/$monitor.$pname.txt.root"
				prefix=$configName"_"$monitor"_"$pname
				suffix=$runname
				condensedConfigName=$configName
				if [ $configName = "g60cm6mm_170gcm3" -o $configName = "g60cm6mm_200gcm3" -o $configName = "g40cm10mm182gcm3" ]; then
					condensedConfigName="g60cm6mm"
				elif [ $configName = "t16cm6mm_03T" -o $configName = "t16cm6mm" ]; then
					condensedConfigName="t16cm6mm"
				fi
				if [ -e $rootfile ]; then
					echo "Processing $rootfile"
					cp "input_"$condensedConfigName"_"$monitor"_"$pname "input"
					./BeamProfile -t "$pname_inTitle on $monitor" -s $nEvents -m argu -x $prefix -y $suffix -l 1e-7 $rootfile
#					./BeamProfile -t "$pname_inTitle on $monitor" -s $nEvents -m argu -x $prefix -y $suffix -l 1e-7 -n 1 -v 25 $rootfile
				else
					echo $rootfile does not exit!
				fi
			done
		done
	done
done
