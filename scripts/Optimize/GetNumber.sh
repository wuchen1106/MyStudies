#!/bin/bash

numbers="Numbers"

#for Target in "g40cm10mm" "g50cm10mm" "g30cm10mm" "t16cm6mm"
#for Target in "g40cm10mm" "g60cm10mm"
for Target in "g60cm10mm"
do
	PrTgtMat=`echo $Target | sed 's/\<\(\w\).*/\1/g'`
	PrTgtLen=`echo $Target | sed 's/\<\w\([0-9]*\)cm.*/\1/g'`
	PrTgtRadi=`echo $Target | sed 's/\<\w*cm\([0-9]*\)mm.*/\1/g'`
	nProtonPT=`echo ''|gawk '{print 1e7}'`
#   for app in "A" "H" "cg4" "g4s"
	for app in "g4s"
	do
#		for phys in "QB" "QBH" "original" "modified" "nomuec" "QB49302" "QB49201"
		for phys in "QBH"
		do
#			for monitor in "MT1" "PTACS" "McTruth" "CDC" "BLT"
			for monitor in "CDC"
			do
#				for DF in "003T" "004T" "005T" "006T" "007T"
				for DF in "005T"
				do
					if [ $Target == "g60cm10mm" ]; then
						nProtonB9=`echo "$nProtonPT*10"|bc`
					else
						if [ $DF == "003T" ]; then
							nProtonB9=`echo "$nProtonPT*2"|bc`
						else
							nProtonB9=`echo "$nProtonPT*1"|bc`
						fi
					fi
					DiFd=`echo $DF | sed 's/\<\(\w*\)T/\1/g'`
					for A9 in\
						"Vac0927_130_130"
#						"Vac0927_130_130"\
#						"Vac0731_250_75"\
#						"Vac0731_250_125"\
#						"Vac0731_250_175"\
#						"Vac0731_200_75"\
#						"Vac0731_200_100"\
#						"Vac0731_200_125"\
#						"Vac0731_200_150"\
#						"Vac0731_200_175"\
#						"Vac0731_200_200"\
#						"Vac0731_250_200"\
#						"Vac0731_250_250"\
#						"Vac0731_300_200"\
#						"Vac0731_300_250"\
#						"Vac0731_300_300"
					do
					for CDC in "_39" "_17"
					do
						if [ $Target == "g60cm10mm" ]; then
							nProtonA9=`echo "$nProtonB9*10"|bc`
						else
							if [ $DF == "003T" ]; then
								nProtonA9=`echo "$nProtonB9*10"|bc`
#								nProtonA9=`echo "$nProtonB9*1"|bc`
							else
								nProtonA9=`echo "$nProtonB9*10"|bc`
							fi
						fi
						CoRadi=`echo $A9$CDC | sed 's/\<\w*_\([0-9]*\)_\([0-9]*\)_\([0-9]*\)/\1/g'`
						minY=`echo $A9$CDC | sed 's/\<\w*_\([0-9]*\)_\([0-9]*\)_\([0-9]*\)/\2/g'`
						nDisks=`echo $A9$CDC | sed 's/\<\w*_\([0-9]*\)_\([0-9]*\)_\([0-9]*\)/\3/g'`
						if [ $nDisks = 17 ]; then
							StTgtSpace=5
							TriLen=20
						else
							StTgtSpace=3
							TriLen=25
						fi
						StTgtRadi=10
#						for det in "" "_0p01"
#						for det in "_pim_EP"
						for det in "_mum" "_pim_EP"
						do
#							for prepname in "mum" "em" "OT"
							for prepname in "EP"
							do
#								for pid in  -11 -13 211 2212 -2212 22 11 13 -211 2112;
#								for pid in "-211"
								for pid in "13" "-211"
								do
									if [ $pid = 11 ]; then pname="em";
									elif [ $pid = 13 ]; then pname="mum";
									elif [ $pid = -211 ]; then pname="pim";
									elif [ $pid = 2112 ]; then pname="n0";
									elif [ $pid = -11 ]; then pname="ep";
									elif [ $pid = -13 ]; then pname="mup";
									elif [ $pid = 211 ]; then pname="pip";
									elif [ $pid = 2212 ]; then pname="pp";
									elif [ $pid = -2212 ]; then pname="pm";
									elif [ $pid = 22 ]; then pname="gamma";
									elif [ $pid = 0 ]; then pname="all";
									elif [ $pid = 1 ]; then pname="EP";
									elif [ $pid = -1 ]; then pname="NU";
									elif [ $pid = 2 ]; then pname="OT";
									elif [ $pid = -2 ]; then pname="ST";
									fi
									bosslog="$monitor.$prepname$det.$pname.$Target.$DF.$A9$CDC.$app.$phys.bosslog"
									if [ -e $bosslog ]; then
										Header="$PrTgtLen $PrTgtRadi $DiFd $CoRadi $minY $nDisks $StTgtRadi $TriLen"
										if ! grep "$Header" $numbers; then
											echo "$Header -1 -1 -1 -1 -1 -1 -1 -1 -1" >> $numbers
										fi
#										echo $bosslog
#										echo "    Exists!!!"
										if [ $pid = 13 ]; then
											mum_all=`grep "N_CDC = " $bosslog | sed 's/.*N_CDC = \([0-9.+e\-]*\),.*/\1/g'`
											mum_o75=`grep "N_CDC_75 = " $bosslog | sed 's/.*N_CDC_75 = \([0-9.+e\-]*\),.*/\1/g'`
											mum_stop=`grep "N_Stop = " $bosslog | sed 's/.*N_Stop = \([0-9.+e\-]*\),.*/\1/g'`
											sed -i "s/^\($Header\) \(.*\) \(.*\) \(.*\) \(.*\) \(.* .* .* .* .*\)/\1 $nProtonA9 $mum_all $mum_o75 $mum_stop \6/g" $numbers
										elif [ $pid = -211 ]; then
											pim_all=`grep "N_CDC = " $bosslog | sed 's/.*N_CDC = \([0-9.+e\-]*\),.*/\1/g'`
											pim_stop=`grep "N_Stop = " $bosslog | sed 's/.*N_Stop = \([0-9.+e\-]*\),.*/\1/g'`
											echo pim_all = $pim_all
											echo pim_stop = $pim_stop
											sed -i "s/^\($Header\) \(.*\) \(.* .* .*\) \(.*\) \(.*\) \(.* .* .*\)/\1 \2 \3 $pim_all $pim_stop \6/g" $numbers
										fi
#									else
#										echo $bosslog
#										echo "    NO!!!"
									fi
								done
							done
						done
						done
					done
				done
			done
		done
	done
done
