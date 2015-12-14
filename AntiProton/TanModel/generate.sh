#!/bin/bash
n=1500000
for (( i=0; i<100; i++ ))
do
	name="sim_PTMonitor.ap.150919.g41001p02QBH_${i}.txt.boss"
	s=`echo "$n*$i"| bc -l`
	echo "#!/bin/bash" > $name
	echo "sleep 3h" >> $name
	echo "source $MYHOME/.setg4sim.sh" >> $name
	echo "cd /scratchfs/bes/wuc/MyWorkArea/MyStudies/AntiProton/TanModel" >> $name
	echo "((time ./TanModel -n $n -s $s) > $i.log ) 2> $i.err" >> $name
	chmod +x $name
done
