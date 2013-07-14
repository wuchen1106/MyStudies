#!/bin/bash

for monitor in "ts2_0" "blt0" "ptacs_shielding";
do
	for jobname in "g16cm6mm" "g60cm6mm";
	do
		for pid in 11 13 -211 2112;
		do
			if [ $pid = 11 ]; then pname="em";
			elif [ $pid = 13 ]; then pname="mum";
			elif [ $pid = -211 ]; then pname="pim";
			elif [ $pid = 2112 ]; then pname="n0";
			fi
			for runname in "Andy" "Chen";
			do
				pbsfile=$monitor.$jobname.$runname.$pname.boss
				echo "#!/bin/bash" > $pbsfile
				echo "source $MYHOME/.setana.sh" >> $pbsfile
				if [ $runname == "Andy" ]; then
					file="$MYDATA/other/Andy/graphite-proton-target_length-60cm.root"
					echo "((time $PWD/../Scan -f $file -m $monitor -i $pid) > $PWD/$pbsfile""log ) 2> $PWD/$pbsfile""err" >> $pbsfile
				elif [ $runname == "Chen" ]; then
					directory="$MYDATA/raw/comet_g4/$jobname"
					njobs=150
					echo "((time $PWD/../Scan -d $directory -j $njobs -m $monitor -i $pid) > $PWD/$pbsfile""log ) 2> $PWD/$pbsfile""err" >> $pbsfile
				fi
				chmod +x $pbsfile
			done
		done
	done
done
