#!/bin/bash
declare -a array_LayerName
declare -a array_wireNo
declare -a array_radius
declare -a array_phi
declare -a array_shiftNo

PI=3.14159265358

if [ $# = 1 ]; then
	input_file=$1
else
	echo "Please indicate the initial file!"
	exit -1
fi

num_layer=0
num_line=`gawk '{i++};END{print i}' $input_file`
for (( iline=1; iline<=num_line; iline++ ))
do 
	temp=`sed -n "$iline s/\(.*\)/\1/p" $input_file`
	if echo $temp | grep -P -q '^[ \t]*$|^#|^[ \t]*//' ; then
		continue
	fi
	if ! echo $temp | grep -P -q '^S[0-9]' ; then
		if ! echo $temp | grep -P -q '^F[0-9]' ; then
			continue
		fi
	fi
	array_LayerName[num_layer]=`echo $temp | gawk '{print $1}'`
	array_wireNo[num_layer]=`echo $temp | gawk '{print $2}'`
	array_radius[num_layer]=`echo $temp | gawk '{print $4}'`
	array_phi[num_layer]=`echo $temp | gawk '{print $5}'`
	array_shiftNo[num_layer]=`echo $temp | gawk '{print $7}'`
	((num_layer++))
done

echo "num_layer: "$num_layer
for (( ilayer=0; ilayer<num_layer; ilayer++ ))
do
	echo ""
	echo "${array_LayerName[ilayer]}: wireNo=${array_wireNo[ilayer]} radius=${array_radius[ilayer]}mm startPhi=${array_phi[ilayer]}rad shiftNo=${array_shiftNo[ilayer]}"
	echo "LayerName   iWire   UpStreamEndPlate                       DownStreamEndPlate"
	echo "                    phi/rad      x/mm         y/mm         phi/rad      x/mm         y/mm"
	num_wire=${array_wireNo[ilayer]}
	radius=${array_radius[ilayer]}
	delta_phi=`echo "scale=5;2*$PI/$num_wire"|bc`
	shift_phi=`echo "scale=5;2*${array_shiftNo[ilayer]}*$delta_phi"|bc`
	for (( iwire=0; iwire<num_wire; iwire++ ))
	do
		i_phi=`echo "scale=5;$iwire*$delta_phi"|bc`
		x=`echo "scale=5;$radius*c($i_phi)"|bc -l`
		y=`echo "scale=5;$radius*s($i_phi)"|bc -l`
		i_phi2=`echo "$i_phi+$shift_phi"|bc`
		x2=`echo "scale=5;$radius*c($i_phi2)"|bc -l`
		y2=`echo "scale=5;$radius*s($i_phi2)"|bc -l`
		printf ' %-12s %-5s %-12s %-12s %-12s %-12s %-12s %-12s\n' "${array_LayerName[ilayer]}" "$iwire" "$i_phi" "$x" "$y" "$i_phi2" "$x2" "$y2"
	done
done
