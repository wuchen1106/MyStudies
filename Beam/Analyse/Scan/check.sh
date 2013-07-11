#!/bin/bash
file="../../result/g60cm6mm/Andy/ts2_0.electron.txt"
#file="../../result/g60cm6mm/Andy/ts2_0.muon.txt"
#file="../../result/g60cm6mm/Andy/ts2_0.neutron.txt"
gawk 'BEGIN{FS="*";RS="\n";prev=-1;cur=-1;perv1=-1;cur1=-1;};{cur1=$2;cur=$4;if (prev==cur&&prev1==cur1) print $0;prev = cur;prev1=cur1;}' $file
