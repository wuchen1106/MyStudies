#!/bin/bash

input="input"
for configName in "g60cm6mm" "t16cm6mm"
do
	for monitor in "ts2_0" "blt0" "ptacs_shielding"
	do
		for pname in "em" "mum" "pim" "n0"
		do
			if [ $pname = em ]; then pname_inTitle="e^{-}";
			elif [ $pname = mum ]; then pname_inTitle="#mu^{-}";
			elif [ $pname = pim ]; then pname_inTitle="#pi^{-}";
			elif [ $pname = n0 ]; then pname_inTitle="n_{0}";
			fi
			for var in "pa" "pt" "pz" "gTime" "r" "theta" "ox" "oy" "oz"
			do
				if [ $var = "pa" -o $var = "pt" -o $var = "pz" ]; then
					left=0
					right=250
				elif [ $var = "r" ]; then
					left=0
					right=500
				elif [ $var = "theta" ]; then
					left=0
					right=3.15
				elif [ $var = "gTime" ]; then
					left=0
					right=2000
				elif [ $var = "ox" ]; then
					left=-810
					right=4500
				elif [ $var = "oy" ]; then
					left=-900
					right=900
				elif [ $var = "oz" ]; then
					left=-950
					right=8000
				fi
				for vartype in "_" "_log_"
				do
					if [ $vartype = "_" ]; then
						LOG=0
					else
						LOG=1
					fi
					nfiles=-1
					for runname in "Andy" "Chen" "QGSPBERT" "Hayashi"
					do
						rootfile="../../result/$configName/$runname/result/""$configName""_""$runname""_""$monitor""_""$pname""_output.root"
						histo="$configName""_""$runname""_""$monitor""_""$pname""$vartype""$var"
						if [ -e $rootfile ]; then
							((nfiles++))
						fi
					done
					if [ $nfiles -lt 1 ]; then
						continue
					fi
					iFile=0
					for runname in "Andy" "Chen" "QGSPBERT" "Hayashi"
					do
						if [ $iFile = 0 ]; then
							COL=1
						elif [ $iFile = 1 ]; then
							COL=632
						elif [ $iFile = 2 ]; then
							COL=600
						elif [ $iFile = 3 ]; then
							COL=416
						elif [ $iFile = 4 ]; then
							COL=800
						fi
						rootfile="../../result/$configName/$runname/result/""$configName""_""$runname""_""$monitor""_""$pname""_output.root"
						histo="$configName""_""$runname""_""$monitor""_""$pname""$vartype""$var"
						if [ -e $rootfile ]; then
							echo "" >> $input
							echo "#   TYPE    | Directory                                                            | histoName" >> $input
							echo "    refTH1D | "$rootfile" | "$histo >> $input
							echo "#   TYPE    | NAME            | TITLE              | xAxisName    | yAxisName     | BIN | LEFT  | RIGHT | minX | minY  |COL|COM|LOGX|LOGY|Marker|Norm   |DrawOption" >> $input
							if [ $iFile = 0 ]; then
								echo "    TH1D    | "$histo"     |                    |              |               | 120 | $left | $right| 0    | 1e-7  | $COL | $nfiles | 0  | $LOG  | 3   | 0 | LP" >> $input
							else
								echo "    TH1D    | "$histo"     |                    |              |               | 120 | $left | $right| 0    | 1e-7  | $COL | 0 | 0  | $LOG  | 3   | 0 | LP" >> $input
							fi
							((iFile++))
						fi
					done
				done
			done
		done
	done
done
