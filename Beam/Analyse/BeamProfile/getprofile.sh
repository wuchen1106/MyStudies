#!/bin/bash

#for configName in "g60cm6mm" "t16cm6mm"
for configName in "g60cm6mm"
do
#	for runname in "Andy" "Chen" "QGSPBERT" "QGSPBERTHP" "Hayashi" "original" "nomuec" "QGSP_BERT" "real"
	for runname in "nomuec" "QGSP_BERT" "real"
	do
		if [ $runname == "Andy" ]; then
			if [ $configName == "g60cm6mm" ]; then
				nEvents=1000000
			elif [ $configName == "t16cm6mm" ]; then
				nEvents=1000000
			fi
		elif [ $runname == "Hayashi" ]; then
			if [ $configName == "g60cm6mm" ]; then
				nEvents=100000
			elif [ $configName == "t16cm6mm" ]; then
				nEvents=100000
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
				nEvents=250000
			elif [ $configName == "t16cm6mm" ]; then
				nEvents=250000
			fi
		else
			if [ $configName == "g60cm6mm" ]; then
				nEvents=1000000
			elif [ $configName == "t16cm6mm" ]; then
				nEvents=1000000
			fi
		fi
		for monitor in "blt0" "ptacs_shielding"
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
				if [ -e $rootfile ]; then
					cp "input_"$configName"_"$monitor"_"$pname "input"
					./BeamProfile -t "$pname_inTitle on $monitor" -s $nEvents -m argu -x $prefix -y $suffix -l 1e-7 $rootfile
				fi
			done
		done
	done
done
