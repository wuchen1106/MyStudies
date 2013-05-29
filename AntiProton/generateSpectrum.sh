#!/bin/bash
nbin=100
min=1
max=100
output=spectrum.txt
echo "pa" > $output
step=`echo "scale=14;($max-$min)/($nbin-1)" | bc`
echo $step
for (( i=0; i<nbin; i++ ))
do
	val=`echo "scale=14;$min+$i*$step"|bc `
	echo $val >> $output
done
