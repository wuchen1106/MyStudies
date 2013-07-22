#!/bin/bash

for configName in "g60cm6mm" "t16cm6mm"
do
	for runname in "Andy" "Chen" "QGSPBERT" "QGSPBERTHP" "Hayashi"
	do
		if [ $runname == "Andy" ]; then
			if [ $configName == "g60cm6mm" ]; then
				nEvents=1000000
			elif [ $configName == "t16cm6mm" ]; then
				nEvents=1000000
			fi
		elif [ $runname == "Chen" ]; then
			if [ $configName == "g60cm6mm" ]; then
				nEvents=15000000
			elif [ $configName == "t16cm6mm" ]; then
				nEvents=15000000
			fi
		elif [ $runname == "QGSPBERT" ]; then
			if [ $configName == "g60cm6mm" ]; then
				nEvents=1500000
			elif [ $configName == "t16cm6mm" ]; then
				nEvents=1500000
			fi
		elif [ $runname == "QGSPBERTHP" ]; then
			if [ $configName == "g60cm6mm" ]; then
				nEvents=60000
			elif [ $configName == "t16cm6mm" ]; then
				nEvents=60000
			fi
		elif [ $runname == "Hayashi" ]; then
			if [ $configName == "g60cm6mm" ]; then
				nEvents=50000
			elif [ $configName == "t16cm6mm" ]; then
				nEvents=100000
			fi
		fi
#		for monitor in "ts2_0" "blt0" "ptacs_shielding"
		for monitor in "ptacs_shielding"
		do
#			for pname in "em" "mum" "pim" "n0"
			for pname in "em"
			do
				if [ $pname = em ]; then pname_inTitle="e^{-}";
				elif [ $pname = mum ]; then pname_inTitle="#mu^{-}";
				elif [ $pname = pim ]; then pname_inTitle="#pi^{-}";
				elif [ $pname = n0 ]; then pname_inTitle="n_{0}";
				fi
				rootfile="../../result/$configName/$runname/$monitor.$pname.txt.root"
				prefix=$configName"_"$monitor"_"$pname
				suffix=$runname
				if [ -e $rootfile ]; then
					./BeamProfile -t "$pname_inTitle on $monitor" -s $nEvents -m argu -x $prefix -y $suffix -l 1e-7 $rootfile
				fi
			done
		done
	done
done