#!/bin/bash

for file in CDC.*ig.*.sh
do
	for (( j=1; j>0; j++ ))
	do
		sleep 2
		n=`ps -ef | grep CDC | wc -l`
		echo "		$j: $n"
		if [ $n -lt 3 ]
		then
			echo "submit $file"
			nohup ./$file &
			break
		fi
	done
done
