#!/bin/bash

queue=midq
nFiles=1
nSplit=1

do_the_job(){
	det=$1
	Target=$2
	monitor=$3
	beginNo=$4
	totalNo=$5
	pid=$6
	prepname=$7
	pname=$8
	DirName=$9
	OriginalFile=${10}
	DF=${11}
	A9=${12}

	prefix="Dummy"
	suffix="_Dummy"
	if [ $monitor = "MT1" ]; then
		name=$monitor.$prepname$det.$pname.$Target.$DF.$app.$phys
	elif [ $monitor  = "A9" -o $monitor = "McTruth" -o $monitor = "BLT" ]; then
		name=$monitor.$prepname$det.$pname.$Target.$DF.$A9.$app.$phys
		prefix=$monitor'_'$pname
		suffix=$det'_'$Target'_'$DF'_'$A9'_'$app'_'$phys
	elif [ $monitor  = "PTACS" ]; then
		name=$monitor.$prepname$det.$pname.$Target.$app.$phys
	fi

	if [ ! $nSplit = 1 ]; then
		name=$name.$iSplit
	fi

	pbsfile=$PWD'/'$name'.boss'
	echo "#!/bin/bash" > $pbsfile
	echo "source $MYHOME/.setana.sh" >> $pbsfile
	echo $PWD'/../beam -x '$prefix' -y '$suffix' -D '$DirName' -O '$OriginalFile' -b '$beginNo' -t '$totalNo' -M '$monitor' -P '$pid' -r '$name' -i '$PWD'/../input_'$monitor' -d '$PWD' -p 100000 -w 1000000 -v 0 > '$pbsfile'log 2> '$pbsfile'err' >> $pbsfile
	chmod +x $pbsfile
#	qsub -j oe -o /dev/null -q $queue $pbsfile
	nohup $pbsfile &
}

#for Target in "g40cm10mm" "g50cm10mm" "g30cm10mm" "t16cm6mm"
for Target in "g40cm10mm"
do
#   for app in "A" "H" "cg4" "g4s"
	for app in "g4s"
	do
#		for phys in "QB" "QBH" "original" "modified" "nomuec" "QB49302" "QB49201"
		for phys in "QBH"
		do
#			for monitor in "MT1" "PTACS" "McTruth" "A9";
			for monitor in "BLT"
			do
				for det in "_noStop"
				do
#					for prepname in "mum" "EP"
					for prepname in "mum"
					do
#						for pid in -11 -13 211 2212 -2212 22 11 13 -211 2112;
						for pid in "1"
						do
							for (( iSplit=0; iSplit<nSplit; iSplit++ ))
							do
								beginNo=`echo "$nFiles/$nSplit*$iSplit"|bc`
								totalNo=`echo "$nFiles/$nSplit"|bc`
								if [ $pid = 11 ]; then pname="em";
								elif [ $pid = 13 ]; then pname="mum";
								elif [ $pid = -211 ]; then pname="pim";
								elif [ $pid = 2112 ]; then pname="n0";
								elif [ $pid = -11 ]; then pname="ep";
								elif [ $pid = -13 ]; then pname="mup";
								elif [ $pid = 211 ]; then pname="pip";
								elif [ $pid = 2212 ]; then pname="pp";
								elif [ $pid = -2212 ]; then pname="pm";
								elif [ $pid = 22 ]; then pname="gamma";
								elif [ $pid = 0 ]; then pname="all";
								elif [ $pid = 1 ]; then pname="EP";
								elif [ $pid = -1 ]; then pname="NU";
								elif [ $pid = 2 ]; then pname="OT";
								fi
								if [ $monitor = "MT1" ]; then
#									for DF in "003T" "0018T"
									for DF in "003T"
									do
										DirName=$MYDATA/raw/g4sim/$monitor.$prepname$det.$Target.$DF.$app.$phys #FIXME need a convention. Now we have to change it in 'EP' and 'pim' etc
										OriginalFile=$MYG4SIMDATAROOT/PTACS.EP.$prepname$det.$Target.$app.$phys.root #FIXME need a convention. Now we have to change it in 'EP' and 'pim' etc
										if [ $det'.' = '.' ]; then
											do_the_job "" $Target $monitor $beginNo $totalNo $pid $prepname $pname $DirName $OriginalFile $DF 
										else
											do_the_job $det $Target $monitor $beginNo $totalNo $pid $prepname $pname $DirName $OriginalFile $DF 
										fi
									done
								elif [ $monitor = "A9" -o $monitor = "McTruth" -o $monitor = "BLT" ]; then
#									for DF in "003T" "0018T"
									for DF in "003T"
									do
#										for A9 in "He0731" "He0701" "Vac0731" "He0731Al" "Vac0731Al"
										for A9 in "Vac0731Al"
										do
											DirName=$MYDATA/raw/g4sim/$monitor.$prepname$det.$Target.$DF.$A9.$app.$phys #FIXME need a convention. Now we have to change it in 'EP' and 'pim' etc
											OriginalFile=$MYG4SIMDATAROOT/MT1.EP.$prepname.$Target.$DF.$app.$phys.root #FIXME need a convention. Now we have to change it in 'EP' and 'pim' etc
											if [ $det'.' = '.' ]; then
												do_the_job "" $Target $monitor $beginNo $totalNo $pid $prepname $pname $DirName $OriginalFile $DF $A9
											else
												do_the_job $det $Target $monitor $beginNo $totalNo $pid $prepname $pname $DirName $OriginalFile $DF $A9
											fi
										done
									done
								elif [ $monitor = "PTACS" ]; then
									DirName=$MYDATA/raw/g4sim/$monitor.$prepname$det.$Target.$app.$phys
									OriginalFile='NONE'
									if [ $det'.' = '.' ]; then
										do_the_job "" $Target $monitor $beginNo $totalNo $pid $prepname $pname $DirName $OriginalFile
									else
										do_the_job $det $Target $monitor $beginNo $totalNo $pid $prepname $pname $DirName $OriginalFile
									fi
								fi
							done
						done
					done
				done
			done
		done
	done
done
