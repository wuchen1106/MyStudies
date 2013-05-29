#!/bin/bash
nbin=5
min=90
max=94
output=spectrum.txt
echo "x y z px py pz" > $output
step=`echo "scale=14;($max-$min)/($nbin-1)" | bc`
echo $step
for (( i=0; i<nbin; i++ ))
do
	val=`echo "scale=14;$min+$i*$step"|bc `
	echo "0 0 0 0 0 $val" >> $output
done
