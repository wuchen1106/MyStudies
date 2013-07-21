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
	gawk 'BEGIN{FS="*";RS="\n";prev=-1;cur=-1;perv1=-1;cur1=-1;};{cur1=$2;cur=$4;if ((prev!=cur||prev1!=cur1)&&NF==15&&cur1!="    Row   ") print $5 $6 $7 $8 $9 $10 $11 $12 $13 $14;prev = cur;prev1=cur1;}' $in >> $out
	num_line=`gawk '{i++};END{print i}' $out`
	if [ $num_line -gt 1 ]; then
		txt2root -t -i $out -o $out.root
	fi
}

for configName in "g60cm6mm" "t16cm6mm"
do
	for runname in "Andy" "Chen" "Hayashi" "QGSPBERT" "QGSPBERTHP"
	do
		for monitor in "ts2_0" "blt0" "ptacs_shielding"
		do
			for pname in "em" "mum" "n0" "pim"
			do
				logfile="../../result/$configName/$runname/$configName.$monitor.$pname.$runname.bosslog"
				output="../../result/$configName/$runname/$monitor.$pname.txt"
				if [ -e $logfile ]; then
					do_trans $logfile $output
				fi
			done
		done
	done
done
