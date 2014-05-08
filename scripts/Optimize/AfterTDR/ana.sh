#!/bin/bash
echo "RUNNAME		opt/I		em/D		ep/D		mup/D			mum/D			gam/D			pim/D			tot/D" >> optimize
opt=0
for RUNNAME in 'BL' \
		       'BLK' \
		       '140331' \
		       '140331K' \
		       'a_p3_35' \
		       'a_p3_35_B_5' \
		       'b_p3_25' \
		       'c'
##		       'b_p3_35' \
##		       'b_p5_20' \
##		       'a2_3t10_30' \
##		       'a2_3t5_35' \
##		       'a_p5_30' \
##		       'a_1_20' \
##		       'a_p3_35_B' \
##		       'a_p3_35_B2' \
##		       'a_p3_35_B3' \
##		       'a_p3_35_B4' \
do
	echo -n "$RUNNAME			" >> optimize
	echo -n "$opt			" >> optimize
	Nt=0
	for PAR in 'em' \
		       'ep' \
		       'mup' \
		       'mum' \
		       'gam' \
		       'pim'
	do
		N=0
		if [ -e result/$RUNNAME.$PAR.root ]; then
			N=`./getNGamma -o result/$RUNNAME.$PAR.root -r $RUNNAME.$PAR.output`
			./getNGamma -o result/$RUNNAME.$PAR.root -r $RUNNAME.$PAR.hit -m GAM
#			echo "Found result/$RUNNAME.$PAR.root !!!"
		else
			echo "Cannot find result/$RUNNAME.$PAR.root !!!"
		fi
		if [ $PAR = 'mup' -o $PAR = 'mum' -o $PAR = 'gam' -o $PAR = 'pim' ]; then
#			echo "$PAR: This is a larger sample"
			N=`echo "scale=1;$N/10"|bc`
#		else
#			echo "$PAR: This is a smaller sample"
		fi
		echo -n "$N			" >> optimize
		Nt=`echo "$Nt+$N"|bc`
	done
	echo "$Nt" >> optimize
	((opt++))
done
