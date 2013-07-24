#!/bin/bash

for monitor in "ts2_0" "blt0" "ptacs_shielding";
do
	for configName in "t16cm6mm" "g60cm6mm";
	do
		for pid in 11 13 -211 2112;
		do
			if [ $pid = 11 ]; then pname="em";
			elif [ $pid = 13 ]; then pname="mum";
			elif [ $pid = -211 ]; then pname="pim";
			elif [ $pid = 2112 ]; then pname="n0";
			fi
			for runname in "Andy" "Chen" "Hayashi" "QGSPBERT" "QGSPBERTHP" "original" "nomuec" "QGSP_BERT" "real";
			do
				pbsfile=$configName.$monitor.$pname.$runname.boss
				echo "#!/bin/bash" > $pbsfile
				echo "source $MYHOME/.setana.sh" >> $pbsfile
				fileexist=false
				if [ $runname == "Andy" ]; then
					if [ $configName == "g60cm6mm" ]; then
						file="$MYDATA/other/Andy/graphite-proton-target_length-60cm.root"
					elif [ $configName == "t16cm6mm" ]; then
						file="DUMMY"
					fi
					if [ -e $file ]; then
						fileexist=true
					fi
					echo "((time $PWD/../Scan -f $file -m $monitor -i $pid) > $PWD/$pbsfile""log ) 2> $PWD/$pbsfile""err" >> $pbsfile
				elif [ $runname == "Hayashi" ]; then
					if [ $configName == "g60cm6mm" ]; then
						file="$MYDATA/other/Hayashi/test0722_2mmGra1.2cm_60cm.root"
					elif [ $configName == "t16cm6mm" ]; then
						file="$MYDATA/other/Hayashi/test0622_Tun16cm.root"
					fi
					if [ -e $file ]; then
						fileexist=true
					fi
					echo "((time $PWD/../Scan -f $file -m $monitor -i $pid) > $PWD/$pbsfile""log ) 2> $PWD/$pbsfile""err" >> $pbsfile
				elif [ $runname == "QGSPBERTHP" ]; then
					directory="$MYDATA/raw/comet_g4/$configName""_""$runname"
					if [ -d $directory ]; then
						fileexist=true
					fi
					njobs=6
					echo "((time $PWD/../Scan -d $directory -j $njobs -m $monitor -i $pid) > $PWD/$pbsfile""log ) 2> $PWD/$pbsfile""err" >> $pbsfile
				elif [ $runname == "Chen" -o $runname == "QGSPBERT" ]; then
					directory="$MYDATA/raw/comet_g4/$configName""_""$runname"
					njobs=150
					if [ -d $directory ]; then
						fileexist=true
					fi
					echo "((time $PWD/../Scan -d $directory -j $njobs -m $monitor -i $pid) > $PWD/$pbsfile""log ) 2> $PWD/$pbsfile""err" >> $pbsfile
				else
					directory="$MYDATA/raw/comet_g4/$configName""_""$runname"
					if [ -d $directory ]; then
						fileexist=true
					fi
					njobs=100
					echo "((time $PWD/../Scan -d $directory -j $njobs -m $monitor -i $pid) > $PWD/$pbsfile""log ) 2> $PWD/$pbsfile""err" >> $pbsfile
				fi
				if [ $fileexist = false ]; then
					rm $pbsfile
				else
					chmod +x $pbsfile
				fi
			done
		done
	done
done
