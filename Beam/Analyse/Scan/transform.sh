#!/bin/bash
echo Hei
do_trans="
	echo \"checking \"\$in\" ...\"
	echo -n \"                x\" > \$out
	echo -n \"                 y\" >> \$out
	echo -n \"                 z\" >> \$out
	echo -n \"                px\" >> \$out
	echo -n \"                py\" >> \$out
	echo -n \"                pz\" >> \$out
	echo -n \"                 t\" >> \$out
	echo -n \"                ox\" >> \$out
	echo -n \"                oy\" >> \$out
	echo -n \"                oz\" >> \$out
	echo    \"               pid\" >> \$out
	gawk 'BEGIN{FS=\"*\";RS=\"\n\";prev=-1;cur=-1;perv1=-1;cur1=-1;};{cur1=\$2;cur=\$4;if ((prev!=cur||prev1!=cur1)&&NF==16&&cur1!=\"    Row   \") print \$5 \$6 \$7 \$8 \$9 \$10 \$11 \$12 \$13 \$14 \$15;prev = cur;prev1=cur1;}' \$in >> \$out
	num_line=\`gawk '{i++};END{print i}' \$out\`
	if [ \$num_line -gt 1 ]; then
		txt2root -t \$out \$out.root
	fi
"

echo Hello
#for Target in "g40cm10mm" "g50cm10mm" "g30cm10mm" "t16cm6mm" "g60cm6mm170gcm3"
for Target in "g40cm10mm"
do
	for app in "A" "H" "cg4"
	do
		for phys in "QB" "QBH" "original" "modified" "nomuec" "QB49302" "QB49201"
		do
			for monitor in "MT1" "PTACS";
			do
#				for pname in "em" "mum" "n0" "pim"
				for pname in "all"
				do
					if [ $monitor = "PTACS" ]; then
						name=$monitor.$pname.$Target.$app.$phys
						logfile=$PWD/result/$name.bosslog
						output=$PWD/result/$name
						pbsfile=$PWD/result/$name.boss
						if [ -e $logfile ]; then
							echo "#!/bin/bash" > $pbsfile
							echo "source $MYHOME/.setana.sh" >> $pbsfile
							echo "in=$logfile" >> $pbsfile
							echo "out=$output" >> $pbsfile
							echo "$do_trans" >> $pbsfile
							chmod +x $pbsfile
							qsub -j oe -o /dev/null -q midq $pbsfile
						else
							echo $logfile does not exist
						fi
					elif [ $monitor = "MT1" ]; then
						for DF in "03T" "018T"
						do
							name=$monitor.$pname.$Target.$DF.$app.$phys
							logfile=$PWD/result/$name.bosslog
							output=$PWD/result/$name
							pbsfile=$PWD/result/$name.boss
							if [ -e $logfile ]; then
								echo "#!/bin/bash" > $pbsfile
								echo "source $MYHOME/.setana.sh" >> $pbsfile
								echo "in=$logfile" >> $pbsfile
								echo "out=$output" >> $pbsfile
								echo "$do_trans" >> $pbsfile
								chmod +x $pbsfile
								qsub -j oe -o /dev/null -q midq $pbsfile
							else
								echo $logfile does not exist
							fi
						done
					fi
				done
			done
		done
	done
done
