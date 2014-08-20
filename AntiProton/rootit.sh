#!/bin/bash
source $MYHOME/.new662.sh
source $MYHOME/.myROOT.sh
#cd /besfs/groups/yuany/commondata/ProtoType/PTM
((time root -l -b  get_histos.C) > log ) 2> err
