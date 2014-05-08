#!/bin/bash
RUNNAME="default"
if [ $# -gt 0 ]; then
	RUNNAME=$1
fi
export RUNNAME=$RUNNAME
source /home/chen/.setg4sim.sh
export OUTPUTDIRROOT=$PWD
export GENFILEROOT=gen_root
nohup /home/chen/MyWorkArea/g4sim/g4sim macros/opt.mac > result/log_$RUNNAME 2>&1 &
