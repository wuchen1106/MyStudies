#!/bin/bash

nFiles=100
nSplit=100

#for configName in "g40cm10mm182gcm3" "g60cm6mm_170gcm3" "g60cm6mm_200gcm3" "t16cm6mm"
for configName in "TPg40cm10mm182gcm3"
do
#	for runname in "Andy" "Hayashi" "QGSPBERT" "QGSPBERTHP" "original" "modified" "nomuec" "QGSPBERT49302" "QGSPBERT49201" "QGSPBERTg4sim"
	for runname in "QGSPBERTg4sim"
	do
		for monitor in "MT1";
		do
#			for pid in 11 13 -211 -11 -13 211 2112;
			for pid in -211;
			do
				for (( iSplit=0; iSplit<nSplit; iSplit++ ))
				do
					beginNo=`echo "$nFiles/$nSplit*$iSplit"|bc`
					totalNo=`echo "$nFiles/$nSplit"|bc`
					if [ $pid = 11 ]; then pname="em";
					elif [ $pid = -11 ]; then pname="ep";
					elif [ $pid = 13 ]; then pname="mum";
					elif [ $pid = -13 ]; then pname="mup";
					elif [ $pid = -211 ]; then pname="pim";
					elif [ $pid = 211 ]; then pname="pip";
					elif [ $pid = 22 ]; then pname="gamma";
					fi
#					./bp -m $monitor -P $pid -r $monitor"."$pname".txt" -n 1000 -p 1 -v 25 > result/$monitor"."$pname".txt.log"
#					./bp -m $monitor -P $pid -r $monitor"."$pname".txt" -p 10000 -v 5 > result/$monitor"."$pname".txt.log"
					pbsfile=$PWD'/result/'$configName'_'$monitor'_'$pname'_'$runname'_'$iSplit'.boss'
					echo "#!/bin/bash" > $pbsfile
					echo "source $MYHOME/.setana.sh" >> $pbsfile
					echo $PWD'/bp -b '$beginNo' -t '$totalNo' -m '$monitor' -P '$pid' -r '$monitor'.'$pname'.'$iSplit'.txt -i '$PWD'/input -d '$PWD'/result -p 100000 -v 5 > '$pbsfile'.log 2> '$pbsfile'.err' >> $pbsfile
					chmod +x $pbsfile
#					qsub -j oe -o /dev/null -q besq $pbsfile
					nohup $pbsfile &
				done
			done
		done
	done
done
