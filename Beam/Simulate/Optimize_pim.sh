#!/bin/bash

subgeo="configure/subgeo_BLTCDC"

#for CoRadi in 200 250 300
#for CoRadi in 250 300
for CoRadi in 250 300
do
#	for minY in 75 100 125 150 175 200 225 250 275 300
	for minY in 200 250 300
	do
		if [ $minY -gt $CoRadi ]; then
			continue
		fi
		ymin=`echo "$minY+600"|bc`
		sed -i "s/^\(.*\)\<\(\w*\)\(  *\<\w*\>  *\<\w*\>  *\<\w*\>  *BLTCHole0\)/\1$CoRadi\3/g" $subgeo
		sed -i "s/^\(.*\)\<\(\w*\)\(  *\<\w*\>  *BLTCHole\)\( .*\)/\1$ymin\3\4/g" $subgeo
		SRA_gen_sim -a fl.MT1.pim.g40cm10mm.05T -t sim -b 0 -e 9 -q midq CDC CDC.pim.g40cm10mm.005T.Vac0731_"$CoRadi"_"$minY"_39.g4s.QBH
	done
done
