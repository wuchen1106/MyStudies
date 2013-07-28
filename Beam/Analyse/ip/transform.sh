#!/bin/bash

#for configName in "g40cm10mm182gcm3" "g60cm6mm_170gcm3" "g60cm6mm_200gcm3" "t16cm6mm"
for configName in "g60cm6mm_170gcm3"
do
#	for runname in "Andy" "Hayashi" "QGSPBERT" "QGSPBERTHP" "original" "modified" "nomuec" "QGSPBERT49302" "QGSPBERT49201" "QGSPBERTg4sim"
	for runname in "QGSPBERTg4sim"
	do
		for monitor in "blt0" "ptacs_shielding";
		do
			for pid in 11 13 -211 2112;
			do
				if [ $pid = 11 ]; then pname="em";
				elif [ $pid = 13 ]; then pname="mum";
				elif [ $pid = -211 ]; then pname="pim";
				elif [ $pid = 2112 ]; then pname="n0";
				fi
#				./ip -m $monitor -P $pid -r $monitor"."$pname".txt" -n 1000 -p 1 -v 25 > result/$monitor"."$pname".txt.log"
				./ip -m $monitor -P $pid -r $monitor"."$pname".txt" -p 10000 -v 5 > result/$monitor"."$pname".txt.log"
			done
		done
	done
done
