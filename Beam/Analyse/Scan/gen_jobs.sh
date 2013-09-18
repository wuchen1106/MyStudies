#!/bin/bash

for monitor in "blt0" "ptacs_shielding";
do
	if [ $monitor = "blt0" ]; then
		monitorname="MT1"
	elif [ $monitor = "ptacs_shielding" ]; then
		monitorname="PTACS"
	fi
#	for Target in "g40cm10mm" "g50cm10mm" "g30cm10mm" "t16cm6mm" "g60cm6mm170gcm3"
	for Target in "g40cm10mm"
	do
#		for pid in 11 13 -211 2112;
		for pid in 0;
		do
			if [ $pid = 11 ]; then pname="em";
			elif [ $pid = 13 ]; then pname="mum";
			elif [ $pid = -211 ]; then pname="pim";
			elif [ $pid = 2112 ]; then pname="n0";
			elif [ $pid = 0 ]; then pname="all";
			elif [ $pid = 1 ]; then pname="EP";
			fi
#			for app in "A" "H" "cg4"
			for app in "H"
			do
#				for phys in "QB" "QBH" "original" "modified" "nomuec" "QB49302" "QB49201"
				for phys in "original"
				do
					pbsfile=result/$monitorname.$pname.$Target.$app.$phys.boss
					echo "#!/bin/bash" > $pbsfile
					echo "source $MYHOME/.setana.sh" >> $pbsfile
					fileexist=false
					if [ $app == "A" ]; then
						if [ $Target == "g60cm6mm170gcm3" ]; then
							file="$MYDATA/other/Andy/graphite-proton-target_length-60cm.root"
						elif [ $Target == "g40cm10mm" ]; then
							file="$MYDATA/other/Andy/graphite-proton-target_length-40cm_radius-10mm.root"
						else
							file="DUMMY"
						fi
						if [ -e $file ]; then
							fileexist=true
						fi
						echo "((time $PWD/Scan -f $file -m $monitor -i $pid) > $PWD/$pbsfile""log ) 2> $PWD/$pbsfile""err" >> $pbsfile
					elif [ $app == "H" ]; then
						if [ $Target == "g60cm6mm_200gcm3" ]; then
#							file="$MYDATA/other/Hayashi/test0722_2mmGra1.2cm_60cm.root"
							file="DUMMY"
						elif [ $Target == "t16cm6mm" ]; then
#							file="$MYDATA/other/Hayashi/test0622_Tun16cm.root"
							file="DUMMY"
						elif [ $Target == "g40cm10mm" ]; then
							file="$MYDATA/other/Hayashi/test_gra2cm_40cm.root"
						else
							file="DUMMY"
						fi
						if [ -e $file ]; then
							fileexist=true
						fi
						echo "((time $PWD/Scan -f $file -m $monitor -i $pid) > $PWD/$pbsfile""log ) 2> $PWD/$pbsfile""err" >> $pbsfile
					else
						directory="$MYDATA/raw/comet_g4/$Target.$phys"
						if [ -d $directory ]; then
							fileexist=true
						fi
						njobs=`ls $directory/*.raw | wc -l`
						echo "((time $PWD/Scan -d $directory -j $njobs -m $monitor -i $pid) > $PWD/$pbsfile""log ) 2> $PWD/$pbsfile""err" >> $pbsfile
					fi
					if [ $fileexist = false ]; then
						rm $pbsfile
					else
						chmod +x $pbsfile
					fi
				done
			done
		done
	done
done
