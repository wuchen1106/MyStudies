#!/bin/bash

for configName in "g60cm6mm" "t16cm6mm"
do
	for runname in "Andy" "Chen" "Hayashi" "QGSPBERT" "QGSPBERTHP" "original" "nomuec" "QGSP_BERT" "real"
	do
		for monitor in "ts2_0" "blt0" "ptacs_shielding"
		do
			for pname in "em" "mum" "n0" "pim"
			do
				logfile="result/$configName.$monitor.$pname.$runname.bosslog"
				directory="../../result/$configName/$runname/"
				if [ -e $logfile ]; then
					mv $logfile $directory
				fi
			done
		done
	done
done
