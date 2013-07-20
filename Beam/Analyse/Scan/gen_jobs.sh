#!/bin/bash

for monitor in "ts2_0" "blt0" "ptacs_shielding";
do
	for configName in "g60cm6mm";
#	for configName in "t16cm6mm" "g60cm6mm";
	do
		for pid in 11 13 -211 2112;
		do
			if [ $pid = 11 ]; then pname="em";
			elif [ $pid = 13 ]; then pname="mum";
			elif [ $pid = -211 ]; then pname="pim";
			elif [ $pid = 2112 ]; then pname="n0";
			fi
			for runname in "Andy" "Hayashi" "QGSPBERT";
#			for runname in "Andy" "Chen" "Hayashi" "QGSPBERT";
			do
				pbsfile=$configName.$runname.$monitor.$pname.boss
				echo "#!/bin/bash" > $pbsfile
				echo "source $MYHOME/.setana.sh" >> $pbsfile
				if [ $runname == "Andy" ]; then
					if [ $configName == "g60cm6mm" ]; then
						file="$MYDATA/other/Andy/graphite-proton-target_length-60cm.root"
					elif [ $configName == "t16cm6mm" ]; then
						file=""
					fi
					echo "((time $PWD/../Scan -f $file -m $monitor -i $pid) > $PWD/$pbsfile""log ) 2> $PWD/$pbsfile""err" >> $pbsfile
				elif [ $runname == "Chen" ]; then
					directory="$MYDATA/raw/comet_g4/$configName""_""$runname"
					njobs=150
					echo "((time $PWD/../Scan -d $directory -j $njobs -m $monitor -i $pid) > $PWD/$pbsfile""log ) 2> $PWD/$pbsfile""err" >> $pbsfile
				elif [ $runname == "QGSPBERT" ]; then
					directory="$MYDATA/raw/comet_g4/$configName""_""$runname"
					njobs=150
					echo "((time $PWD/../Scan -d $directory -j $njobs -m $monitor -i $pid) > $PWD/$pbsfile""log ) 2> $PWD/$pbsfile""err" >> $pbsfile
				elif [ $runname == "Hayashi" ]; then
					if [ $configName == "g60cm6mm" ]; then
						file="$MYDATA/other/Hayashi/test0618_Gra60cm_2.root"
					elif [ $configName == "t16cm6mm" ]; then
						file="$MYDATA/other/Hayashi/test0622_Tun16cm.root"
					fi
					echo "((time $PWD/../Scan -f $file -m $monitor -i $pid) > $PWD/$pbsfile""log ) 2> $PWD/$pbsfile""err" >> $pbsfile
				fi
				chmod +x $pbsfile
			done
		done
	done
done
