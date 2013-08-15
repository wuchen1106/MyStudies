#!/bin/bash
#for Target in "g40cm10mm" "g50cm10mm" "g30cm10mm" "t16cm6mm" "g60cm6mm170gcm3"
for Target in "t16cm6mm"
do
#	for app in "A" "H" "cg4" "g4s"
	for app in "g4s"
	do
#		for phys in "QB" "QBH" "original" "modified" "nomuec" "QB49302" "QB49201"
		for phys in "QBH"
		do
#			for monitor in "MT1" "PTACS"
			for monitor in "MT1"
			do
				nEvents=1000000
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
						elif [ $Target = "g40cm10mm" ]; then
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
					if [ $monitor == "PTACS" ]; then
						if [ $phys == "QBH" -a $Target == "g40cm10mm" ]; then
							nEvents=10000000
						fi
					elif [ $monitor == "MT1" ]; then
						if [ $phys == "QBH" ]; then
							if [ $Target == "t16cm6mm" ]; then
								nEvents=2000000
							else
								nEvents=20000000
							fi
						fi
					fi
				fi
				for pname in "em" "mum" "pim" "n0"
				do
					pid=0
					if [ $pname = em ]; then pname_inTitle="e^{-}"; pid=11;
					elif [ $pname = mum ]; then pname_inTitle="#mu^{-}"; pid=13;
					elif [ $pname = pim ]; then pname_inTitle="#pi^{-}"; pid=-211;
					elif [ $pname = n0 ]; then pname_inTitle="n_{0}"; pid=2112;
					fi
					if [ $monitor = "MT1" ]; then
#						for DF in "03T" "018T"
						for DF in "018T" "03T"
						do
							prefix=$monitor"_"$pname
							suffix="_"$Target"_"$DF"_"$app"_"$phys
							pbsfile="$PWD/result/"$prefix$suffix".boss"
#							directory="$PWD/../../result/$Target/$app/$phys/$monitor/"
							directory="$PWD/../../result/$Target"
							rootfile=$directory/$monitor.EP.$Target.$DF.$app.$phys.root
							inputfile=input.$monitor.$pname
							cp input.temp $inputfile
							R=350
							PMAX=300
							TMAX=2000
							T=80
							if [ $pname = "em" ]; then
								P=2
								OZMIN=-6040.5
								OZMAX=-5540.5
								OYMIN=-10
								OYMAX=10
								OXMIN=7100
								OXMAX=7600
							elif [ $pname = "mum" ]; then
								P=150
								OZMIN=-5990.5
								OZMAX=-5590.5
								OYMIN=-200
								OYMAX=200
								OXMIN=6550
								OXMAX=8150
							elif [ $pname = "pim" ]; then
								P=150
								OZMIN=-5870.5
								OZMAX=5710.5
								OYMIN=-10 
								OYMAX=10 
								OXMIN=7000
								OXMAX=7700
							elif [ $pname = "n0" ]; then
								T=1200
								P=60
								OZMIN=-6590.5
								OZMAX=-1790.5
								OYMIN=-800
								OYMAX=800
								OXMIN=8150
								OXMAX=1650
							fi
							sed -i "s/\<_P\>/$P/g" $inputfile
							sed -i "s/\<_R\>/$R/g" $inputfile
							sed -i "s/\<_PMAX\>/$PMAX/g" $inputfile
							sed -i "s/\<_T\>/$T/g" $inputfile
							sed -i "s/\<_TMAX\>/$TMAX/g" $inputfile
							sed -i "s/\<_OXMIN\>/$OXMIN/g" $inputfile
							sed -i "s/\<_OYMIN\>/$OYMIN/g" $inputfile
							sed -i "s/\<_OZMIN\>/$OZMIN/g" $inputfile
							sed -i "s/\<_OXMAX\>/$OXMAX/g" $inputfile
							sed -i "s/\<_OYMAX\>/$OYMAX/g" $inputfile
							sed -i "s/\<_OZMAX\>/$OZMAX/g" $inputfile
							if [ -e $rootfile ]; then
								echo "Processing $prefix$suffix"
								echo "#!/bin/bash" > $pbsfile
								echo "source $MYHOME/.setana.sh" >> $pbsfile
								echo "$PWD/BeamProfile -t \"$pname_inTitle on $monitor\" -s $nEvents -m $monitor -x $prefix -y $suffix -l 1e-7 -i $PWD/$inputfile -P $pid -d $PWD/result -v 0 $rootfile > $pbsfile""log 2> $pbsfile""err" >> $pbsfile
								chmod +x $pbsfile
#								qsub -j oe -o /dev/null -q midq $pbsfile
								$pbsfile
							else
								echo $rootfile does not exit!
							fi
						done
					elif [ $monitor = "PTACS" ]; then
						prefix=$monitor"_"$pname
						suffix="_"$Target"_"$app"_"$phys
						pbsfile="$PWD/result/"$prefix$suffix".boss"
#						directory="$PWD/../../result/$Target/$app/$phys/$monitor/"
						directory="$PWD/../../result/$Target"
						rootfile=$directory/$monitor.EP.$Target.$app.$phys.root
						inputfile=input.$monitor.$pname
						cp input.temp $inputfile
						R=350
						PMAX=300
						TMAX=2000
						T=20
						if [ $pname = "em" ]; then
							P=2
							OZMIN=-5990.5
							OZMAX=-5590.5
							OYMIN=-10
							OYMAX=10
							OXMIN=7800
							OXMAX=6550
						elif [ $pname = "mum" ]; then
							P=250
							OZMIN=-6190.5
							OZMAX=-5390.5
							OYMIN=-400
							OYMAX=400
							OXMIN=8150
							OXMAX=6550
						elif [ $pname = "pim" ]; then
							P=350
							OZMIN=-5870.5
							OZMAX=-5710.5
							OYMIN=-10 
							OYMAX=10 
							OXMIN=7050
							OXMAX=7650
						elif [ $pname = "n0" ]; then
							T=1000
							P=100
							OZMIN=-6590.5
							OZMAX=-4990.5
							OYMIN=-800
							OYMAX=800
							OXMIN=6550
							OXMAX=8150
						fi
						sed -i "s/\<_P\>/$P/g" $inputfile
						sed -i "s/\<_R\>/$R/g" $inputfile
						sed -i "s/\<_PMAX\>/$PMAX/g" $inputfile
						sed -i "s/\<_T\>/$T/g" $inputfile
						sed -i "s/\<_TMAX\>/$TMAX/g" $inputfile
						sed -i "s/\<_OXMIN\>/$OXMIN/g" $inputfile
						sed -i "s/\<_OYMIN\>/$OYMIN/g" $inputfile
						sed -i "s/\<_OZMIN\>/$OZMIN/g" $inputfile
						sed -i "s/\<_OXMAX\>/$OXMAX/g" $inputfile
						sed -i "s/\<_OYMAX\>/$OYMAX/g" $inputfile
						sed -i "s/\<_OZMAX\>/$OZMAX/g" $inputfile
						if [ -e $rootfile ]; then
							echo "Processing $prefix$suffix"
							echo "#!/bin/bash" > $pbsfile
							echo "source $MYHOME/.setana.sh" >> $pbsfile
							echo "$PWD/BeamProfile -t \"$pname_inTitle on $monitor\" -s $nEvents -m $monitor -x $prefix -y $suffix -l 1e-7 -i $PWD/$inputfile -P $pid -d $PWD/result -v 0 $rootfile > $pbsfile""log 2> $pbsfile""err" >> $pbsfile
							chmod +x $pbsfile
#							qsub -j oe -o /dev/null -q midq $pbsfile
							$pbsfile
						else
							echo $rootfile does not exit!
						fi
					fi
				done
			done
		done
	done
done
