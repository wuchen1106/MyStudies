#!/bin/bash

do_trans(){
	in=$1
	out=$2
	echo "checking \"$in\" ..."
	echo -n "                x" > $out
	echo -n "                 y" >> $out
	echo -n "                 z" >> $out
	echo -n "                px" >> $out
	echo -n "                py" >> $out
	echo -n "                pz" >> $out
	echo -n "                 t" >> $out
	echo -n "                ox" >> $out
	echo -n "                oy" >> $out
	echo    "                oz" >> $out
	gawk 'BEGIN{FS="*";RS="\n";prev=-1;cur=-1;perv1=-1;cur1=-1;};{cur1=$2;cur=$4;if ((prev!=cur||prev1!=cur1)&&NF==15&&cur!="Row") print $5 $6 $7 $8 $9 $10 $11 $12 $13 $14;prev = cur;prev1=cur1;}' $in >> $out
}

#for jobname in "g60cm6mm" "g16cm6mm";
for jobname in "g16cm6mm"
do
	for monitor in "ts2_0" "blt0" "ptacs_shielding"
	do
		for pid in 11 13 -211 2112
		do
			if [ $pid = 11 ]; then pname="em";
			elif [ $pid = 13 ]; then pname="mum";
			elif [ $pid = -211 ]; then pname="pim";
			elif [ $pid = 2112 ]; then pname="n0";
			fi
			for runname in "Andy" "Chen"
			do
				if [ $runname == "Andy" ]; then
					logfile="../../result/$jobname/$runname/$monitor.$jobname.$runname.$pname.bosslog"
					output="../../result/$jobname/$runname/$monitor.$pname.txt"
					if [ $monitor = "ptacs_shielding" ]
					then
						output="../../result/$jobname/$runname/ptcs.$pname.txt"
					fi
				elif [ $runname == "Chen" ]; then
					logfile="../../result/$jobname/$runname/$monitor.$jobname.$runname.$pname.bosslog"
					output="../../result/$jobname/$runname/$monitor.$pname.txt"
					if [ $monitor = "ptacs_shielding" ]
					then
						output="../../result/$jobname/$runname/ptcs.$pname.txt"
					fi
				fi
				do_trans $logfile $output
			done
		done
	done
done
