#!/bin/bash

input=$1



scaleb=1.1
scalex=1000.
deltax=0
deltay=0
deltaz=-12.5

num_line=`gawk '{i++};END{print i}' $input`
for (( iline=1; iline<=num_line; iline++ ))
do 
	temp=`sed -n "$iline s/\(.*\)/\1/p" $input`
	x=`echo $temp | gawk '{print $1}'`
	y=`echo $temp | gawk '{print $2}'`
	z=`echo $temp | gawk '{print $3}'`
	x=`echo "($x+$deltax)*$scalex"|bc`
	y=`echo "($y+$deltay)*$scalex"|bc`
	z=`echo "($z+$deltaz)*$scalex"|bc`
	bx=`echo $temp | gawk '{print $4}'`
	by=`echo $temp | gawk '{print $5}'`
	bz=`echo $temp | gawk '{print $6}'`
	echo "$x $y $z $bx $by $bz"
done
