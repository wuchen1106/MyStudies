#!/bin/bash

doloop(){
	for (( i=0; i<50; i++ ))
	do
		echo "#!/bin/bash" > "$name"."$i".sh
		echo "source /afs/ihep.ac.cn/users/w/wuc/.setg4sim.sh" >> "$name"."$i".sh
		echo "cd /scratchfs/bes/wuc/MyWorkArea/MyStudies/CDCHit" >> "$name"."$i".sh
		echo "((time ./transform $data/$name/$i"_job0.raw" $name.$i) > $name.$i.log) 2> $name.$i.err" >> "$name"."$i".sh
		chmod +x $name.$i.sh
		qsub -j oe -o /dev/null -q besq $name.$i.sh
	done
}

data="/scratchfs/bes/wuc/MyWorkArea/Data"
#name="CDC.140905M02.40um.102MeV.g41001QBH"
#doloop
#name="CDC.140905M02.40um.103MeV.g41001QBH"
#doloop
#name="CDC.140905M02.40um.104MeV.g41001QBH"
#doloop
#name="CDC.140905M02.63um.102MeV.g41001QBH"
#doloop
#name="CDC.140905M02.63um.103MeV.g41001QBH"
#doloop
#name="CDC.140905M02.63um.104MeV.g41001QBH"
#doloop
name="CDC.140905M02.40um.sig.g41001QBH"
doloop
name="CDC.140905M02.63um.sig.g41001QBH"
doloop
