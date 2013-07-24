#!/bin/bash

xmin=-1000
xmax=1000
ymin=-1000
ymax=1000
zmin=-1000
zmax=1000
for (( i=0; i<10; i++ ))
do
	x=`echo "scale=6;$xmin+($xmax- $xmin)/10*$i"|bc`
#	echo "x=$xmin+($xmax-$xmin)/10*$i=$x"
	for (( j=0; j<10; j++ ))
	do
		y=`echo "scale=6;$ymin+($ymax- $ymin)/10*$j"|bc`
#		echo "y=$ymin+($ymax-$ymin)/10*$i=$y"
		for (( k=0; k<10; k++ ))
		do
			z=`echo "scale=6;$zmin+($zmax- $zmin)/10*$k"|bc`
#			echo "z=$zmin+($zmax-$zmin)/10*$i=$z"
			bx=0
			by=0
			bz=1
			echo "$x $y $z $bx $by $bz"
		done
	done
done
