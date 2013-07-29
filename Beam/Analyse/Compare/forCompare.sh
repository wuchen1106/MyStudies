#!/bin/bash

var_process(){
	configNam=$1
	monitor=$2
	pname=$3
	pname_inTitle=$4
	var=$5
	vartype=$6
	nx=$7
	ny=$8
	if [ $vartype = "_" ]; then
		LOG=0
	else
		LOG=1
	fi
	nfiles=-1
#	for runname in "Andy" "Hayashi" "QGSPBERT" "QGSPBERTHP" "original" "modified" "nomuec" "QGSPBERT49302" "QGSPBERT49201" "QGSPBERTg4sim"
	for runname in "QGSPBERT" "QGSPBERTg4sim"
	do
		rootfile="../../result/$configName/$runname/result/""$configName""_""$monitor""_""$pname""_""$runname""_output.root"
		histo="$configName""_""$monitor""_""$pname""_""$var""$vartype""$runname"
		if [ -e $rootfile ]; then
			((nfiles++))
		else
			echo $rootfile does not exist!
		fi
	done
	if [ $nfiles -lt 1 ]; then
		echo "nfiles = $nfiles"
		continue
	fi
	iFile=0
#	for runname in "Andy" "Hayashi" "QGSPBERT" "QGSPBERTHP" "original" "modified" "nomuec" "QGSPBERT49302" "QGSPBERT49201" "QGSPBERTg4sim"
	for runname in "QGSPBERT" "QGSPBERTg4sim"
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
		rootfile="../../result/$configName/$runname/result/""$configName""_""$monitor""_""$pname""_""$runname""_output.root"
		histo="$configName""_""$monitor""_""$pname""_""$var""$vartype""$runname"
		if [ -e $rootfile ]; then
			echo "" >> $input
			echo "#   TYPE    | Directory                                                            | histoName" >> $input
			echo "    refTH1D | "$rootfile" | "$histo >> $input
			echo "#   TYPE    | NAME            | TITLE              | xAxisName    | yAxisName     | BIN | LEFT  | RIGHT | minX | minY  |COL|COM|LOGX|LOGY|Marker|Norm   |DrawOption|UseLeg|legendName|legx1|legy1|legx2|legy2|npadx|npady" >> $input
			if [ $iFile = 0 ]; then
				echo "    TH1D    | "$histo"     |                    |              |               | 120 |   1   |   1   | 0    | 1e-6  | $COL | $nfiles | 0  | $LOG  | 3   | 0 | LP       |   1  |$runname  |0.8 |0.8 |1 |1 |$nx |$ny " >> $input
			else
				echo "    TH1D    | "$histo"     |                    |              |               | 120 |   1   |   1   | 0    | 1e-6  | $COL | 0 | 0  | $LOG  | 3   | 0 | LP       |   1  |$runname  |0.8 |0.8 |1 |1 " >> $input
			fi
			((iFile++))
		fi
	done
}

input="input"
#for configName in "g40cm10mm182gcm3" "g60cm6mm_170gcm3" "g60cm6mm_200gcm3" "t16cm6mm"
for configName in "g60cm6mm_170gcm3"
do
#	for monitor in "ts2_0" "blt1" "blt0" "ptacs_beampipe" "ptacs_shielding";
	for monitor in "blt0" "ptacs_shielding"
	do
		for pname in "em" "mum" "pim" "n0"
		do
			if [ $pname = em ]; then pname_inTitle="e^{-}";
			elif [ $pname = mum ]; then pname_inTitle="#mu^{-}";
			elif [ $pname = pim ]; then pname_inTitle="#pi^{-}";
			elif [ $pname = n0 ]; then pname_inTitle="n_{0}";
			fi
			for vartype in "_" "_log_"
			do
				if [ $pname = em ]; then
					for var in "pa" "pt" "pz" "theta" "pa_tail" "pt_tail" "pz_tail" "gTime_tail" "gTime" "ox" "oy" "oz" "r"
					do
						nx=1
						ny=1
						if [ $var = "pa" -o $var = "pa_tail" -o $var = "gTime" ]; then
							nx=2
							ny=2
						fi
						var_process $configName $monitor $pname $pname_inTitle $var $vartype $nx $ny
					done
				else
					for var in "pa" "pt" "pz" "theta" "gTime" "ox" "oy" "oz" "gTime_tail" "r"
					do
						nx=1
						ny=1
						if [ $var = "pa" -o $var = "gTime" ]; then
							nx=2
							ny=2
						fi
						var_process $configName $monitor $pname $pname_inTitle $var $vartype $nx $ny
					done
				fi
			done
		done
	done
done
