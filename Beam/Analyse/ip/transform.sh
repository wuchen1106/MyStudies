#!/bin/bash

#for configName in "g40cm10mm182gcm3" "g60cm6mm_170gcm3" "g60cm6mm_200gcm3" "t16cm6mm"
for configName in "g50cm10mm182gcm3_03T"
do
#	for runname in "Andy" "Hayashi" "QGSPBERT" "QGSPBERTHP" "original" "modified" "nomuec" "QGSPBERT49302" "QGSPBERT49201" "QGSPBERTg4sim"
	for runname in "QGSPBERTHPg4sim"
	do
#		for monitor in "blt0" "ptacs_shielding";
		for monitor in "blt0"
		do
#			for pid in -11 -13 211 2212 -2212 22 11 13 -211 2112;
			for pid in 13;
			do
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
				fi
				pbsfile=$PWD'/result/'$configName'_'$monitor'_'$pname'_'$runname'.boss'
				echo "#!/bin/bash" > $pbsfile
				echo "source $MYHOME/.setana.sh" >> $pbsfile
				echo $PWD'/ip -m '$monitor' -P '$pid' -r '$monitor'.'$pname'.txt -i '$PWD'/input -d '$PWD'/result -p 10000 -v 5 > '$PWD'/result/'$monitor'.'$pname'.txt.log 2> '$pbsfile'.err' >> $pbsfile
				chmod +x $pbsfile
				qsub -j oe -o /dev/null -q shortq $pbsfile
			done
		done
	done
done
