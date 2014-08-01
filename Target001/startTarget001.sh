#!/bin/bash

cd $MYCOMETROOT
source setenv.sh
SRA_gen_sim -f -t sim -b 0 -e 49 -Y target001 20140729_Chen_Target001
tail -f $MYDATA/20140729_Chen_Target001/sim*log
