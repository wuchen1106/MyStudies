#!/bin/bash

source /afs/ihep.ac.cn/users/w/wuc/.setana.sh
WORKDIR="/scratchfs/bes/wuc/MyWorkArea/MyStudies/Beam/Analyse/beamnew"
cd $WORKDIR
((time $WORKDIR/commit.sh) > $WORKDIR/log) 2> $WORKDIR/err

