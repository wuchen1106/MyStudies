#!/bin/bash

for gas in GAS2-5.GASFILE GAS1-1.GASFILE
do
	if [ $gas = "GAS2-5.GASFILE" ]
	then
		hv=2300
	elif [ $gas = "GAS1-1.GASFILE" ]
	then
		hv=1850
	fi
	for zk in 0 0.128967
	do
#		for phi in 0 10 20 33.6900681 45 90 135 123.6900681
		for phi in 2 5 88 85 80 70
		do
			cos=`echo "c($phi/180*3.1415926)"| bc -l`
			sin=`echo "s($phi/180*3.1415926)"| bc -l`
			for t in 0 1
			do
				cp sample.4.5.temp.garf sample.4.5.garf
				sed -i "s/opGAS/$gas/" sample.4.5.garf
				sed -i "s/opSV/$hv/" sample.4.5.garf
				sed -i "s/opzk/$zk/" sample.4.5.garf
				sed -i "s/opSIN/$sin/" sample.4.5.garf
				sed -i "s/opCOS/$cos/" sample.4.5.garf
				sed -i "s/opT/$t/" sample.4.5.garf
				suffix=$gas.$hv.$zk.$phi.$t
				./garfield-9 < sample.4.5.garf > log 2>&1
				sed -i "s/\[cm\]       \[microsec\]             \[cm\]       \[microsec\]/THISISSTART/" log
				sed -i "s/====== INPGET INPUT   : \*===================================//g" log
				sed -i "s/  ====== INPGET INPUT   : &STOP  /THISISEND/" log
				keyword="THISISSTART"
				gawk -v gawk_keyword=$keyword 'BEGIN{FS=gawk_keyword;RS="You will not find me, will you?";ORS=""};{print $2}' log > template.1
				keyword="THISISEND"
				gawk -v gawk_keyword=$keyword 'BEGIN{FS=gawk_keyword;RS="You will not find me, will you?";ORS=""};{print $1}' template.1 > template.2
				echo "x dt" > xt.$suffix.txt
				gawk '{if(NF>7&&$2!="Not"){ print $1" "$2;}}' template.2 >> xt.$suffix.txt
				txt2root xt.$suffix.txt xt.$suffix.root
				ps2pdf garfield.metafile
				mv garfield.metafile.pdf garfield.$suffix.pdf
				mv log log.$suffix.txt
				rm template.1 template.2
			done
		done
	done
done
