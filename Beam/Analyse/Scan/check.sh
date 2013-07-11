#!/bin/bash
file="../../result/g60cm6mm/Andy/ts2_0.electron.txt.bk"
echo "checking $file"
gawk 'BEGIN{FS="*";RS="\n";prev=-1;cur=-1;perv1=-1;cur1=-1;};{cur1=$2;cur=$4;if (prev!=cur||prev1!=cur1) print $0;prev = cur;prev1=cur1;}' $file > $file.tr 
file="../../result/g60cm6mm/Andy/ts2_0.muon.txt.bk"
echo "checking $file"
gawk 'BEGIN{FS="*";RS="\n";prev=-1;cur=-1;perv1=-1;cur1=-1;};{cur1=$2;cur=$4;if (prev!=cur||prev1!=cur1) print $0;prev = cur;prev1=cur1;}' $file > $file.tr
file="../../result/g60cm6mm/Andy/ts2_0.neutron.txt.bk"
echo "checking $file"
gawk 'BEGIN{FS="*";RS="\n";prev=-1;cur=-1;perv1=-1;cur1=-1;};{cur1=$2;cur=$4;if (prev!=cur||prev1!=cur1) print $0;prev = cur;prev1=cur1;}' $file > $file.tr
file="../../result/g60cm6mm/Andy/ts2_0.pim.txt.bk"
echo "checking $file"
gawk 'BEGIN{FS="*";RS="\n";prev=-1;cur=-1;perv1=-1;cur1=-1;};{cur1=$2;cur=$4;if (prev!=cur||prev1!=cur1) print $0;prev = cur;prev1=cur1;}' $file > $file.tr
file="../../result/g60cm6mm/Andy/blt0.electron.txt.bk"
echo "checking $file"
gawk 'BEGIN{FS="*";RS="\n";prev=-1;cur=-1;perv1=-1;cur1=-1;};{cur1=$2;cur=$4;if (prev!=cur||prev1!=cur1) print $0;prev = cur;prev1=cur1;}' $file > $file.tr
file="../../result/g60cm6mm/Andy/blt0.muon.txt.bk"
echo "checking $file"
gawk 'BEGIN{FS="*";RS="\n";prev=-1;cur=-1;perv1=-1;cur1=-1;};{cur1=$2;cur=$4;if (prev!=cur||prev1!=cur1) print $0;prev = cur;prev1=cur1;}' $file > $file.tr
file="../../result/g60cm6mm/Andy/blt0.neutron.txt.bk"
echo "checking $file"
gawk 'BEGIN{FS="*";RS="\n";prev=-1;cur=-1;perv1=-1;cur1=-1;};{cur1=$2;cur=$4;if (prev!=cur||prev1!=cur1) print $0;prev = cur;prev1=cur1;}' $file > $file.tr
file="../../result/g60cm6mm/Andy/blt0.pim.txt.bk"
echo "checking $file"
gawk 'BEGIN{FS="*";RS="\n";prev=-1;cur=-1;perv1=-1;cur1=-1;};{cur1=$2;cur=$4;if (prev!=cur||prev1!=cur1) print $0;prev = cur;prev1=cur1;}' $file > $file.tr
file="../../result/g60cm6mm/Chen/ts2_0.electron.txt.bk"
echo "checking $file"
gawk 'BEGIN{FS="*";RS="\n";prev=-1;cur=-1;perv1=-1;cur1=-1;};{cur1=$2;cur=$4;if (prev!=cur||prev1!=cur1) print $0;prev = cur;prev1=cur1;}' $file > $file.tr
file="../../result/g60cm6mm/Chen/ts2_0.muon.txt.bk"
echo "checking $file"
gawk 'BEGIN{FS="*";RS="\n";prev=-1;cur=-1;perv1=-1;cur1=-1;};{cur1=$2;cur=$4;if (prev!=cur||prev1!=cur1) print $0;prev = cur;prev1=cur1;}' $file > $file.tr
file="../../result/g60cm6mm/Chen/ts2_0.neutron.txt.bk"
echo "checking $file"
gawk 'BEGIN{FS="*";RS="\n";prev=-1;cur=-1;perv1=-1;cur1=-1;};{cur1=$2;cur=$4;if (prev!=cur||prev1!=cur1) print $0;prev = cur;prev1=cur1;}' $file > $file.tr
file="../../result/g60cm6mm/Chen/ts2_0.pim.txt.bk"
echo "checking $file"
gawk 'BEGIN{FS="*";RS="\n";prev=-1;cur=-1;perv1=-1;cur1=-1;};{cur1=$2;cur=$4;if (prev!=cur||prev1!=cur1) print $0;prev = cur;prev1=cur1;}' $file > $file.tr
file="../../result/g60cm6mm/Chen/blt0.electron.txt.bk"
echo "checking $file"
gawk 'BEGIN{FS="*";RS="\n";prev=-1;cur=-1;perv1=-1;cur1=-1;};{cur1=$2;cur=$4;if (prev!=cur||prev1!=cur1) print $0;prev = cur;prev1=cur1;}' $file > $file.tr
file="../../result/g60cm6mm/Chen/blt0.muon.txt.bk"
echo "checking $file"
gawk 'BEGIN{FS="*";RS="\n";prev=-1;cur=-1;perv1=-1;cur1=-1;};{cur1=$2;cur=$4;if (prev!=cur||prev1!=cur1) print $0;prev = cur;prev1=cur1;}' $file > $file.tr
file="../../result/g60cm6mm/Chen/blt0.neutron.txt.bk"
echo "checking $file"
gawk 'BEGIN{FS="*";RS="\n";prev=-1;cur=-1;perv1=-1;cur1=-1;};{cur1=$2;cur=$4;if (prev!=cur||prev1!=cur1) print $0;prev = cur;prev1=cur1;}' $file > $file.tr
file="../../result/g60cm6mm/Chen/blt0.pim.txt.bk"
echo "checking $file"
gawk 'BEGIN{FS="*";RS="\n";prev=-1;cur=-1;perv1=-1;cur1=-1;};{cur1=$2;cur=$4;if (prev!=cur||prev1!=cur1) print $0;prev = cur;prev1=cur1;}' $file > $file.tr
