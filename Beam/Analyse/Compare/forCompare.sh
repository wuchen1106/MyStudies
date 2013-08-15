#!/bin/bash

do_id(){
	iFile=$1
	var=$2
	vartype=$3
	monitor=$4
	pname=$5
	app=$6
	phys=$7
	Target=$8
	DF=$9
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
	elif [ $iFile = 5 ]; then
		COL=616
	elif [ $iFile = 6 ]; then
		COL=432
	fi
	if [ $monitor = "MT1" ]; then
		rootfile="../../result/$Target/$app/$phys/$monitor/$DF/"$monitor"_"$pname"_"$Target"_"$DF"_"$app"_"$phys".root"
		histo=$monitor"_"$pname"_"$var$vartype$Target"_"$DF"_"$app"_"$phys
	elif [ $monitor = "PTACS" ]; then
		rootfile="../../result/$Target/$app/$phys/$monitor/"$monitor"_"$pname"_"$Target"_"$app"_"$phys".root"
		histo=$monitor"_"$pname"_"$var$vartype$Target"_"$app"_"$phys
	fi
	if [ $vartype = "_" ]; then
		LOG=0
	else
		LOG=1
	fi
	if [ -e $rootfile ]; then
#		legendName=$phys
#		legendName=$Target
#		legendName=$app
		legendName=$DF
		echo "" >> $input
		echo "#   TYPE    | Directory                                                            | histoName" >> $input
		echo "    refTH1D | "$rootfile" | "$histo >> $input
		echo "#   TYPE    | NAME            | TITLE              | xAxisName    | yAxisName     | BIN | LEFT  | RIGHT | minX | minY  |COL|COM|LOGX|LOGY|Marker|Norm   |DrawOption|UseLeg|legendName|legx1|legy1|legx2|legy2|npadx|npady" >> $input
		if [ $iFile = 0 ]; then
			echo "    TH1D    | "$histo"     |                    |              |               | 120 |   1   |   1   | 0    | 1e-6  | $COL | _NFILES | 0  | $LOG  | 3   | 0 | LP       |   1  |$legendName  |0.8 |0.8 |1 |1 |$nx |$ny " >> $input
		else
			echo "    TH1D    | "$histo"     |                    |              |               | 120 |   1   |   1   | 0    | 1e-6  | $COL | 0 | 0  | $LOG  | 3   | 0 | LP       |   1  |$legendName  |0.8 |0.8 |1 |1 " >> $input
		fi
		((iFile++))
	else
		echo "$rootfile does not exist!"
	fi
	return $iFile
}

var_process(){
	Target=$1
	monitor=$2
	pname=$3
	pname_inTitle=$4
	var=$5
	vartype=$6
	nx=$7
	ny=$8
	iFile=0
#	for app in "A" "H" "cg4" "g4s"
	for app in "g4s"
	do
#		for phys in "QB" "QBH" "original" "modified" "nomuec" "QB49302" "QB49201"
		for phys in "QBH"
		do
#			for Target in "g40cm10mm" "g50cm10mm" "g30cm10mm" "t16cm6mm" "g60cm6mm170gcm3"
			for Target in "g40cm10mm"
			do
				if [ $monitor = "MT1" ]; then
					for DF in "03T" "018T"
					do
						do_id $iFile $var $vartype $monitor $pname $app $phys $Target $DF
						iFile=$?
					done
				elif [ $monitor = "PTACS" ]; then
					do_id $iFile $var $vartype $monitor $pname $app $phys $Target
					iFile=$?
				fi
			done
		done
	done
	((iFile--))
	sed -i "s/\<_NFILES\>/$iFile/g" $input
}

input="input"
#for Target in "g40cm10mm" "g50cm10mm" "g30cm10mm" "t16cm6mm" "g60cm6mm170gcm3"
for Target in "g40cm10mm"
do
#	for monitor in "PTACS" "MT1"
	for monitor in "MT1"
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
				for var in "pa" "pt" "pz" "theta" "pa_tail" "pt_tail" "pz_tail" "gTime_tail" "gTime" "ox" "oy" "oz" "r"
				do
					nx=1
					ny=1
					if [ $var = "pa" -o $var = "pa_tail" -o $var = "gTime" ]; then
						nx=2
						ny=2
					fi
					var_process $Target $monitor $pname $pname_inTitle $var $vartype $nx $ny
				done
			done
		done
	done
done
