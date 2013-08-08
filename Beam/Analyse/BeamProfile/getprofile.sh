#!/bin/bash
#for Target in "g40cm10mm" "g50cm10mm" "g30cm10mm" "t16cm6mm" "g60cm6mm170gcm3"
for Target in "g40cm10mm"
do
#	for app in "A" "H" "cg4" "g4s"
	for app in "cg4"
	do
#		for phys in "QB" "QBH" "original" "modified" "nomuec" "QB49302" "QB49201"
		for phys in "QB"
		do
			if [ $app == "A" ]; then
				nEvents=1000000
			elif [ $app == "H" ]; then
				nEvents=100000
			elif [ $app == "cg4" ]; then
				if [ $phys == "modified" ]; then
					if [ $Target = "g60cm6mm200gcm3" -o $Target = "t16cm6mm" ]; then
						nEvents=15000000
					fi
				elif [ $phys == "QB" ]; then
					if [ $Target = "g60cm6mm200gcm3" -o $Target = "t16cm6mm" ]; then
						nEvents=250000
					elif [ $Target = "g40cm10mm182gcm3" ]; then
						nEvents=15000000
					fi
				elif [ $phys == "QBH" ]; then
					if [ $Target = "g60cm6mm200gcm3" -o $Target = "t16cm6mm" ]; then
						nEvents=250000
					fi
				elif [ $phys == "QB49302" ]; then
					if [ $Target = "g60cm6mm170gcm3" ]; then
						nEvents=990000
					else
						nEvents=0
					fi
				fi
			elif [ $app == "g4s" ]; then
				if [ $phys == "QBH" -a $Target == "g40cm10mm" ]; then
					nEvents=10000000
				fi
			else
				nEvents=1000000
			fi
			for monitor in "PTACS" "MT1"
			do
				for pname in "em" "mum" "pim" "n0"
				do
					if [ $pname = em ]; then pname_inTitle="e^{-}";
					elif [ $pname = mum ]; then pname_inTitle="#mu^{-}";
					elif [ $pname = pim ]; then pname_inTitle="#pi^{-}";
					elif [ $pname = n0 ]; then pname_inTitle="n_{0}";
					fi
					prefix=$monitor"."$pname
					suffix="."$Target"."$app"."$phys
					pbsfile="$PWD/result/"$prefix$suffix".boss"
#					directory="$PWD/../../result/$Target/$app/$phys/$monitor/"
					directory="$PWD/../../result/$Target"
					rootfile=$directory$prefix$suffix".root"
					inputfile="input.$prefix$suffix"
					if [ -e $rootfile ]; then
						echo "Processing $rootfile"
						cp input.temp $inputfile
						R=350
						PMAX=300
						TMAX=2000
						if [ $monitor = "MT1" ]; then
							T=80
							if [ $pname = "em" ]; then
								P=2
								OXMIN=-250
								OXMAX=250
								OYMIN=-10
								OYMAX=10
								OZMIN=-250
								OZMAX=250
							elif [ $pname = "mum" ]; then
								P=150
								OXMIN=-200
								OXMAX=200
								OYMIN=-200
								OYMAX=200
								OZMIN=-800
								OZMAX=8000
							elif [ $pname = "pim" ]; then
								P=150
								OXMIN=-80 
								OXMAX=80 
								OYMIN=-10 
								OYMAX=10 
								OZMIN=-300
								OZMAX=350 
							elif [ $pname = "n0" ]; then
								T=1200
								P=60
								OXMIN=-800
								OXMAX=4000
								OYMIN=-800
								OYMAX=800
								OZMIN=-800
								OZMAX=9000
							fi
						elif [ $monitor = "PTACS" ]; then
							T=20
							if [ $pname = "em" ]; then
								P=2
								OXMIN=-200
								OXMAX=200
								OYMIN=-10
								OYMAX=10
								OZMIN=-450
								OZMAX=800
							elif [ $pname = "mum" ]; then
								P=250
								OXMIN=-400
								OXMAX=400
								OYMIN=-400
								OYMAX=400
								OZMIN=-800
								OZMAX=800
							elif [ $pname = "pim" ]; then
								P=350
								OXMIN=-80 
								OXMAX=80 
								OYMIN=-10 
								OYMAX=10 
								OZMIN=-300
								OZMAX=300 
							elif [ $pname = "n0" ]; then
								T=1000
								P=100
								OXMIN=-800
								OXMAX=800
								OYMIN=-800
								OYMAX=800
								OZMIN=-800
								OZMAX=800
							fi
						fi
						sed -i "s/\<_P\>/$P/g" $inputfile
						sed -i "s/\<_PMAX\>/$PMAX/g" $inputfile
						sed -i "s/\<_T\>/$T/g" $inputfile
						sed -i "s/\<_TMAX\>/$TMAX/g" $inputfile
						sed -i "s/\<_OXMIN\>/$OXMIN/g" $inputfile
						sed -i "s/\<_OYMIN\>/$OYMIN/g" $inputfile
						sed -i "s/\<_OZMIN\>/$OZMIN/g" $inputfile
						sed -i "s/\<_OXMAX\>/$OXMAX/g" $inputfile
						sed -i "s/\<_OYMAX\>/$OYMAX/g" $inputfile
						sed -i "s/\<_OZMAX\>/$OZMAX/g" $inputfile
						echo "#!/bin/bash" > $pbsfile
						echo "source $MYHOME/.setana.sh" >> $pbsfile
						echo "$PWD/BeamProfile -t \"$pname_inTitle on $monitor\" -s $nEvents -m argu -x $prefix -y $suffix -l 1e-7 $rootfile -i $PWD/$inputfile -d $PWD/result > $pbsfile""log 2> $pbsfile""err" >> $pbsfile
						chmod +x $pbsfile
#						qsub -j oe -o /dev/null -q midq $pbsfile
						$pbsfile
					else
						echo $rootfile does not exit!
					fi
				done
			done
		done
	done
done
