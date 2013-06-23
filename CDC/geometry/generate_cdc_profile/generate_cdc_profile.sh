#!/bin/bash
declare -a array_wireNo
declare -a array_radius
declare -a array_shiftNo
declare -a array_layerNo
declare -a array_superlayerSpace

PI=3.14159265358

#if [ $# = 1 ]; then
#	input_file=$1
#else
#	echo "Please indicate the initial file!"
#	exit -1
#fi

input_endplate="else.txt"
input_wire="layer.txt"

# read layerinfo
num_superlayer=0
num_line=`gawk '{i++};END{print i}' $input_wire`
for (( iline=1; iline<=num_line; iline++ ))
do 
	temp=`sed -n "$iline s/\(.*\)/\1/p" $input_wire`
	if echo $temp | grep -P -q '^[ \t]*$|^#|^[ \t]*//' ; then
		continue
	fi
	if echo $temp | grep -P -q '^length' ; then
		inner_length=`echo $temp | gawk '{print $2}'`
		inner_length=`echo "$inner_length*10"|bc` # cm -> mm
	elif echo $temp | grep -P -q '^layer_info' ; then
		array_layerNo[num_superlayer]=`echo $temp | gawk '{print $3}'`
		temp_radius=`echo $temp | gawk '{print $4}'`
		array_wireNo[num_superlayer]=`echo $temp | gawk '{print $5}'`
		array_shiftNo[num_superlayer]=`echo $temp | gawk '{print $6}'`
		temp_layerSpace=`echo $temp | gawk '{print $7}'`
		array_radius[num_superlayer]=`echo "$temp_radius*10"|bc` # cm -> mm
		array_layerSpace[num_superlayer]=`echo "$temp_layerSpace*10"|bc` # cm -> mm
		((num_superlayer++))
	fi
done

# read endplate info
num_line=`gawk '{i++};END{print i}' $input_endplate`
for (( iline=1; iline<=num_line; iline++ ))
do 
	temp=`sed -n "$iline s/\(.*\)/\1/p" $input_endplate`
	if echo $temp | grep -P -q '^[ \t]*$|^#|^[ \t]*//' ; then
		continue
	fi
	if echo $temp | grep -P -q '^TotalIndent' ; then
		TotalIndent=`echo $temp | gawk '{print $2}'`
	elif echo $temp | grep -P -q '^Thickness' ; then
		Thickness=`echo $temp | gawk '{print $2}'`
	elif echo $temp | grep -P -q '^InnerRadius' ; then
		InnerRadius=`echo $temp | gawk '{print $2}'`
	elif echo $temp | grep -P -q '^OuterRadius' ; then
		OuterRadius=`echo $temp | gawk '{print $2}'`
	elif echo $temp | grep -P -q '^EndPlateMat' ; then
		EndPlateMat=`echo $temp | gawk '{print $2}'`
	elif echo $temp | grep -P -q '^EndPlateColor' ; then
		EndPlR=`echo $temp | gawk '{print $2}'`
		EndPlG=`echo $temp | gawk '{print $3}'`
		EndPlB=`echo $temp | gawk '{print $4}'`
	elif echo $temp | grep -P -q '^ChamberMat' ; then
		ChamberMat=`echo $temp | gawk '{print $2}'`
	elif echo $temp | grep -P -q '^InnerThick' ; then
		InnerThick=`echo $temp | gawk '{print $2}'`
	elif echo $temp | grep -P -q '^OuterThick' ; then
		OuterThick=`echo $temp | gawk '{print $2}'`
	elif echo $temp | grep -P -q '^InnerCylinderMat' ; then
		InnerCylinderMat=`echo $temp | gawk '{print $2}'`
	elif echo $temp | grep -P -q '^OuterCylinderMat' ; then
		OuterCylinderMat=`echo $temp | gawk '{print $2}'`
	elif echo $temp | grep -P -q '^InnerCylinderColor' ; then
		InCylR=`echo $temp | gawk '{print $2}'`
		InCylG=`echo $temp | gawk '{print $3}'`
		InCylB=`echo $temp | gawk '{print $4}'`
	elif echo $temp | grep -P -q '^OuterCylinderColor' ; then
		OutCylR=`echo $temp | gawk '{print $2}'`
		OutCylG=`echo $temp | gawk '{print $3}'`
		OutCylB=`echo $temp | gawk '{print $4}'`
	fi
done
nSteps=$num_superlayer
superlayerIndent=`echo "scale=5;$TotalIndent/$nSteps"|bc`

# **********************Dump info************************
echo "###endplate info###"
echo "TotalIndent = $TotalIndent mm"
echo "layerIndent = $superlayerIndent mm"
echo "nSteps      = $nSteps mm"
echo "Thickness = $Thickness mm"
echo "InnerRadius = $InnerRadius mm"
echo "OuterRadius = $OuterRadius mm"

echo ""
echo "###wire info###"
echo "inner_length   = $inner_length cm"
echo "num_superlayer = $num_superlayer"
printf "%-12s %-12s %-6s %-6s %-6s %-6s\n" "SuperLayerID" "SubLayerNo" "radius" "wireNo" "shift" "space"
printf "%-12s %-12s %-6s %-6s %-6s %-6s\n" "" "" "cm" "" "" "cm"
num_total_layer=0
for (( isuperlayer=0; isuperlayer<num_superlayer; isuperlayer++ ))
do
	num_total_layer=`echo "$num_total_layer+${array_layerNo[isuperlayer]}"|bc`
	printf "%-12s %-12s %-6s %-6s %-6s %-6s\n" "$isuperlayer" "${array_layerNo[isuperlayer]}" "${array_radius[isuperlayer]}" "${array_wireNo[isuperlayer]}" "${array_shiftNo[isuperlayer]}" "${array_layerSpace[isuperlayer]}"
done
echo "Totally $num_total_layer layers inside"

# ******************************output layerinfo***************************
output_wire="output_wire.txt"
echo "" > $output_wire
output_endplate="output_endplate.txt"
echo "" > $output_endplate
output_vis="output_vis.txt"
echo "" > $output_vis
echo "#layer info" >> $output_wire
printf "\tLayerNo:\t\t$num_total_layer\n" >> $output_wire
printf "#\t%-5s %-8s %-11s %-7s %-12s %-12s %-9s %-7s %-11s\n" "type" "layerNo" "sublayerNo" "WireNo" "Length" "R" "StartPhi" "First" "RotateCell" >> $output_wire
printf "#\t%-5s %-8s %-11s %-7s %-12s %-12s %-9s %-7s %-11s\n" "" "" "" "" "mm" "mm" "rad" "0:F/1:S" "" >> $output_wire
echo "#Tubs info" >> $output_endplate
printf "#\t%-4s %-3s %-10s %-10s %-10s %-8s %-7s %-4s %-4s %-4s %-15s %-15s %-15s %-6s %-5s %-10s %-6s %-6s %-6s %-6s %-6s\n" \
	   "Type" "No" "RMin" "RMax" "Length" "StartAng" "SpanAng" "posX" "posY" "posZ" "Name" "MotVolume" "Material" "SDName" \
	   "RepNo" "Space" "DirThe" "DirPhi" "Ephi" "Etheta" "Epsi" >> $output_endplate
printf "#\t%-4s %-3s %-10s %-10s %-10s %-8s %-7s %-4s %-4s %-4s %-15s %-15s %-15s %-6s %-5s %-10s %-6s %-6s %-6s %-6s %-6s\n" \
	   "" "" "mm" "mm" "mm" "deg" "deg" "mm" "mm" "mm" "" "" "" "" \
	   "" "mm" "deg" "deg" "deg" "deg" "deg" >> $output_endplate
ContRMin=`echo "$InnerRadius-$InnerThick"|bc`
ContRMax=`echo "$OuterRadius+$OuterThick"|bc`
ContLen=`echo "$inner_length+2*$TotalIndent"|bc`
printf "\t%-4s %-3s %-10s %-10s %-10s %-8s %-7s %-4s %-4s %-4s %-15s %-15s %-15s %-6s %-5s %-10s %-6s %-6s %-6s %-6s %-6s\n" \
	   "T" "-3" "$ContRMin" "$ContRMax" "$ContLen" "0" "360" "0" "0" "0" "CDCContainer" "World" "$ChamberMat" "none" \
	   "1" "0" "0" "0" "0" "0" "0" >> $output_endplate
echo "#visual settings" >> $output_vis
printf "\tVerboseLevel:    5\n" >> $output_vis
printf "\tVISSETTING  #Keep this line to call Reset\n" >> $output_vis
printf "#\t%-20s %-6s %-6s %-6s\n" \
	   "Name" "red" "green" "blue" >> $output_vis
printf "#\t%-20s %-6s %-6s %-6s\n" \
	   "vis_layer" "0" "1" "0" >> $output_vis
printf "#\t%-20s %-6s %-6s %-6s\n" \
	   "vis_cell" "0" "1" "0" >> $output_vis
printf "#\t%-20s %-6s %-6s %-6s\n" \
	   "vis_SignalWire" "1" "0" "0" >> $output_vis
printf "#\t%-20s %-6s %-6s %-6s\n" \
	   "vis_FieldWire" "0" "0" "1" >> $output_vis
printf "#\t%-20s %-6s %-6s %-6s\n" \
	   "vis_CDCContainer" "0" "1" "1" >> $output_vis
layer_count=0
num_superlayerm1=`echo "$num_superlayer-1"|bc`
num_superlayerp1=`echo "$num_superlayer+1"|bc`
for (( isuperlayer=0; isuperlayer<num_superlayer; isuperlayer++ ))
do
	Layer_thick=`echo "scale=5;${array_layerSpace[isuperlayer]}"|bc`
	halfLayer_thick=`echo "scale=5;$Layer_thick/2"|bc`
	num_wire=${array_wireNo[isuperlayer]}
	Length=`echo "$inner_length+($isuperlayer*$superlayerIndent)"|bc`
	num_subLayer=${array_layerNo[isuperlayer]}
	rotate_cell=${array_shiftNo[isuperlayer]}
	num_subLayerm1=`echo "$num_subLayer-1"|bc`
	RMin=`echo "${array_radius[isuperlayer]}-$halfLayer_thick"|bc`
	RMax=`echo "${array_radius[isuperlayer]}+$halfLayer_thick*2*$num_subLayer-$halfLayer_thick"|bc`
	Space=`echo "$Length+$Thickness"|bc`
	if [ $isuperlayer == 0 ]; then
		iRMax=$RMin
		iRMin=$InnerRadius
		CylLen=`echo "$Length+2*$Thickness"|bc`
		CylRMin=`echo "$iRMin-$InnerThick"|bc`
		CylRMax=$iRMin
		printf "\t%-4s %-3s %-10s %-10s %-10s %-8s %-7s %-4s %-4s %-4s %-15s %-15s %-15s %-6s %-5s %-10s %-6s %-6s %-6s %-6s %-6s\n" \
		   "T" "-2" "$CylRMin" "$CylRMax" "$CylLen" "0" "360" "0" "0" "0" "InnerCylinder" "CDCContainer" "$InnerCylinderMat" "none" \
		   "1" "0" "0" "0" "0" "0" "0" >> $output_endplate
		printf "\t%-20s %-6s %-6s %-6s\n" \
			   "vis_InnerCylinder" "$InCylR" "$InCylG" "$InCylB" >> $output_vis
		printf "\t%-4s %-3s %-10s %-10s %-10s %-8s %-7s %-4s %-4s %-4s %-15s %-15s %-15s %-6s %-5s %-10s %-6s %-6s %-6s %-6s %-6s\n" \
		   "T" "-1" "$iRMin" "$iRMax" "$Thickness" "0" "360" "0" "0" "0" "EndPlate_in" "CDCContainer" "Aluminium" "none" \
		   "2" "$Space" "0" "0" "0" "0" "0" >> $output_endplate
		printf "\t%-20s %-6s %-6s %-6s\n" \
			   "vis_EndPlate_in" "$EndPlR" "$EndPlG" "$EndPlB" >> $output_vis
	fi
	printf "\t%-4s %-3s %-10s %-10s %-10s %-8s %-7s %-4s %-4s %-4s %-15s %-15s %-15s %-6s %-5s %-10s %-6s %-6s %-6s %-6s %-6s\n" \
	   "T" "$isuperlayer" "$RMin" "$RMax" "$Thickness" "0" "360" "0" "0" "0" "EndPlate_$isuperlayer" "CDCContainer" "Aluminium" "none" \
	   "2" "$Space" "0" "0" "0" "0" "0" >> $output_endplate
	printf "\t%-20s %-6s %-6s %-6s\n" \
		   "vis_EndPlate_$isuperlayer" "$EndPlR" "$EndPlG" "$EndPlB" >> $output_vis
	for (( isubLayer=0; isubLayer<num_subLayer; isubLayer++ ))
	do
		mid_radius=`echo "${array_radius[isuperlayer]}+$halfLayer_thick*2*$isubLayer"|bc`
		inner_radius=`echo "$mid_radius-$halfLayer_thick"|bc`
		startPhi=0
		FirstWire=`echo "($layer_count+1)%2"|bc`
		printf "\t%-5s %-8s %-11s %-7s %-12s %-12s %-9s %-7s %-11s\n" "F" "" "" "" "" "$inner_radius" "" "" "" >> $output_wire
		printf "\t%-5s %-8d %-11d %-7d %-12s %-12s %-9d %-7d %-11d\n" "S" "$layer_count" "$isubLayer" "$num_wire" "$Length" "$mid_radius" "$startPhi" "$FirstWire" "$rotate_cell" >> $output_wire
		if [ $isubLayer == $num_subLayerm1 -a $isuperlayer == $num_superlayerm1 ]; then
			outer_radius=`echo "$mid_radius+$halfLayer_thick"|bc`
			printf "\t%-5s %-8s %-11s %-7s %-12s %-12s %-9s %-7s %-11s\n" "LF" "" "" "" "" "$outer_radius" "" "" "" >> $output_wire
		fi
		((layer_count++))
	done
	if [ $isuperlayer == $num_superlayerm1 ]; then
		oRMin=$RMax
		oRMax=$OuterRadius
		printf "\t%-4s %-3s %-10s %-10s %-10s %-8s %-7s %-4s %-4s %-4s %-15s %-15s %-15s %-6s %-5s %-10s %-6s %-6s %-6s %-6s %-6s\n" \
		   "T" "$num_superlayer" "$oRMin" "$oRMax" "$Thickness" "0" "360" "0" "0" "0" "EndPlate_out" "CDCContainer" "Aluminium" "none" \
		   "2" "$Space" "0" "0" "0" "0" "0" >> $output_endplate
		printf "\t%-20s %-6s %-6s %-6s\n" \
			   "vis_EndPlate_out" "$EndPlR" "$EndPlG" "$EndPlB" >> $output_vis
		CylLen=`echo "$Length+2*$Thickness"|bc`
		CylRMin=$oRMax
		CylRMax=`echo "$oRMax+$OuterThick"|bc`
		printf "\t%-4s %-3s %-10s %-10s %-10s %-8s %-7s %-4s %-4s %-4s %-15s %-15s %-15s %-6s %-5s %-10s %-6s %-6s %-6s %-6s %-6s\n" \
		   "T" "$num_superlayerp1" "$CylRMin" "$CylRMax" "$CylLen" "0" "360" "0" "0" "0" "OuterCylinder" "CDCContainer" "$OuterCylinderMat" "none" \
		   "1" "0" "0" "0" "0" "0" "0" >> $output_endplate
		printf "\t%-20s %-6s %-6s %-6s\n" \
			   "vis_OuterCylinder" "$OutCylR" "$OutCylG" "$OutCylB" >> $output_vis
	fi
done

# ******************************output***************************
cat wire.txt > output.txt
cat $output_wire >> output.txt
cat $output_endplate >> output.txt
cat $output_vis >> output.txt
rm $output_wire
rm $output_endplate
rm $output_vis
