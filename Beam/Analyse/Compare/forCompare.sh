#!/bin/bash
num_line_temp=`gawk '{i++};END{print i}' temp1`
for (( iline=1; iline<=num_line_temp; iline++ ))
do 
	sed -n "$iline s/\(.*\)/\1/p" temp1 >> temp
	sed -n "$iline s/\(.*\)/\1/p" temp2 >> temp
done
