#!/bin/bash

for configName in "g40cm10mm182gcm3" "g60cm6mm_170gcm3" "g60cm6mm_200gcm3" "t16cm6mm"
do
	for runname in "Andy" "Hayashi" "QGSPBERT" "QGSPBERTHP" "original" "modified" "nomuec" "QGSPBERT49302"
	do
		for monitor in "ts2_0" "blt1" "blt0" "ptacs_beampipe" "ptacs_shielding";
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
