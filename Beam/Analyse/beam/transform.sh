#!/bin/bash

nFiles=100
nSplit=1

#for configName in "g40cm10mm" "g50cm10mm" "g30cm10mm" "t16cm6mm" "g60cm6mm_170gcm3"
for configName in "g40cm10mm"
do
#	for runname in "Andy" "Hayashi" "QGSPBERT" "QGSPBERTHP"  "QGSPBERTcometg4" "QGSPBERTHPcometg4" "originalcometg4" "modifiedcometg4" "nomueccometg4" "QGSPBERT49302cometg4" "QGSPBERT49201cometg4"
	for runname in "QB"
	do
#		for monitor in "MT1" "PTACS" "McTruth";
		for monitor in "McTruth"
		do
#			for pid in -11 -13 211 2212 -2212 22 11 13 -211 2112;
			for pid in 0;
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
					fi
					name=$configName'_'$monitor'_'$pname'_'$runname'_'$iSplit
					pbsfile=$PWD'/result/'$name'.boss'
					echo "#!/bin/bash" > $pbsfile
					echo "source $MYHOME/.setana.sh" >> $pbsfile
					echo $PWD'/ip -b '$beginNo' -t '$totalNo' -m monitor -M '$monitor' -P '$pid' -r '$name' -i '$PWD'/input_'$configName' -d '$PWD'/result -p 10000 -v 0 > '$pbsfile'log 2> '$pbsfile'err' >> $pbsfile
					chmod +x $pbsfile
#					qsub -j oe -o /dev/null -q midq $pbsfile
					$pbsfile
				done
			done
		done
	done
done
