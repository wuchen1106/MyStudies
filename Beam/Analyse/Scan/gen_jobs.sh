#!/bin/bash

for monitor in "blt0" "ptacs_shielding";
do
#	for configName in "g60cm6mm170gcm3" "g40cm10mm" "g30cm10mm" "g50cm10mm" "t16cm6mm"
	for configName in "g40cm10mm"
	do
#		for pid in 11 13 -211 2112;
		for pid in 0;
		do
			if [ $pid = 11 ]; then pname="em";
			elif [ $pid = 13 ]; then pname="mum";
			elif [ $pid = -211 ]; then pname="pim";
			elif [ $pid = 2112 ]; then pname="n0";
			elif [ $pid = 0 ]; then pname="all";
			fi
#			for runname in "Andy" "Hayashi" "QGSPBERTcometg4" "QGSPBERTHPcometg4" "originalcometg4" "modifiedcometg4" "nomueccometg4" "QGSPBERT49302cometg4" "QGSPBERT49201cometg4"
			for runname in "QGSPBERTcometg4"
			do
				pbsfile=$configName.$monitor.$pname.$runname.boss
				echo "#!/bin/bash" > $pbsfile
				echo "source $MYHOME/.setana.sh" >> $pbsfile
				fileexist=false
				if [ $runname == "Andy" ]; then
					if [ $configName == "g60cm6mm170gcm3" ]; then
						file="$MYDATA/other/Andy/graphite-proton-target_length-60cm.root"
					else
						file="DUMMY"
					fi
					if [ -e $file ]; then
						fileexist=true
					fi
					echo "((time $PWD/../Scan -f $file -m $monitor -i $pid) > $PWD/$pbsfile""log ) 2> $PWD/$pbsfile""err" >> $pbsfile
				elif [ $runname == "Hayashi" ]; then
					if [ $configName == "g60cm6mm_200gcm3" ]; then
						file="$MYDATA/other/Hayashi/test0722_2mmGra1.2cm_60cm.root"
					elif [ $configName == "t16cm6mm" ]; then
						file="$MYDATA/other/Hayashi/test0622_Tun16cm.root"
					else
						file="DUMMY"
					fi
					if [ -e $file ]; then
						fileexist=true
					fi
					echo "((time $PWD/../Scan -f $file -m $monitor -i $pid) > $PWD/$pbsfile""log ) 2> $PWD/$pbsfile""err" >> $pbsfile
				else
					directory="$MYDATA/raw/comet_g4/$configName""_""$runname"
					if [ -d $directory ]; then
						fileexist=true
					fi
					njobs=`ls $directory/*.raw | wc -l`
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
