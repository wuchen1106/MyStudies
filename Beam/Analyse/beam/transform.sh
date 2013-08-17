#!/bin/bash

queue=besq
nFiles=100
nSplit=1

do_the_job(){
	Target=$1
	monitor=$2
	beginNo=$3
	totalNo=$4
	pid=$5
	pname=$6
	DirName=$7
	OriginalFile=$8
	DF=$9
	A9=${10}

	prefix="Dummy"
	suffix="_Dummy"
	if [ $monitor = "MT1" ]; then
		name=$monitor.$pname.$Target.$DF.$app.$phys
	elif [ $monitor  = "A9" -o $monitor = "McTruth" ]; then
		name=$monitor.$pname.$Target.$DF.$A9.$app.$phys
		prefix=$monitor'_'$pname
		suffix='_'$Target'_'$DF'_'$A9'_'$app'_'$phys
	elif [ $monitor  = "PTACS" ]; then
		name=$monitor.$pname.$Target.$app.$phys
	fi

	if [ ! $nSplit = 1 ]; then
		name=$name.$iSplit
	fi

	pbsfile=$PWD'/result/'$name'.boss'
	echo "#!/bin/bash" > $pbsfile
	echo "source $MYHOME/.setana.sh" >> $pbsfile
	echo $PWD'/beam -x '$prefix' -y '$suffix' -D '$DirName' -O '$OriginalFile' -b '$beginNo' -t '$totalNo' -M '$monitor' -P '$pid' -r '$name' -i '$PWD'/input_'$monitor' -d '$PWD'/result -p 10000 -v 0 > '$pbsfile'log 2> '$pbsfile'err' >> $pbsfile
#	echo $PWD'/beam -n 73346 -m McTruth -P '$pid' -r '$name' -i '$PWD'/input_'$monitor' -d '$PWD'/result -p 10000 -v 0 > '$pbsfile'log 2> '$pbsfile'err' >> $pbsfile
	chmod +x $pbsfile
#	qsub -j oe -o /dev/null -q $queue $pbsfile
	nohup $pbsfile &
}

#for Target in "g40cm10mm" "g50cm10mm" "g30cm10mm" "t16cm6mm" "g60cm6mm170gcm3"
for Target in  "g40cm10mm"
do
#   for app in "A" "H" "cg4" "g4s"
	for app in "g4s"
	do
#	for phys in "QB" "QBH" "original" "modified" "nomuec" "QB49302" "QB49201"
		for phys in "QBH"
		do
#		for monitor in "MT1" "PTACS" "McTruth" "A9";
			for monitor in "MT1"
			do
#			for pid in -11 -13 211 2212 -2212 22 11 13 -211 2112;
				for pid in "-211"
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
#							for DF in "003T" "0018T"
							for DF in "003TS"
							do
								DirName=$MYDATA/raw/g4sim/$monitor.pim.$Target.$DF.$app.$phys #FIXME need a convention. Now we have to change it in 'EP' and 'pim' etc
								OriginalFile=$MYG4SIMDATAROOT/PTACS.EP.$Target.$app.$phys.root #FIXME need a convention. Now we have to change it in 'EP' and 'pim' etc
								do_the_job $Target $monitor $beginNo $totalNo $pid $pname $DirName $OriginalFile $DF 
							done
						elif [ $monitor = "A9" -o $monitor = "McTruth" ]; then
#							for DF in "003T" "0018T"
							for DF in "003TS"
							do
#								for A9 in "0731" "0701"
								for A9 in "0701"
								do
									DirName=$MYDATA/raw/g4sim/$monitor.$pname.$Target.$DF.$A9.$app.$phys #FIXME need a convention. Now we have to change it in 'EP' and 'pim' etc
									OriginalFile=$MYG4SIMDATAROOT/MT1.$pname.$Target.$DF.$app.$phys.root #FIXME need a convention. Now we have to change it in 'EP' and 'pim' etc
									do_the_job $Target $monitor $beginNo $totalNo $pid $pname $DirName $OriginalFile $DF $A9
								done
							done
						elif [ $monitor = "PTACS" ]; then
							DirName=$MYDATA/raw/g4sim/$monitor.EP.$Target.$app.$phys
							OriginalFile='NONE'
							do_the_job $Target $monitor $beginNo $totalNo $pid $pname $DirName $OriginalFile
						fi
					done
				done
			done
		done
	done
done
