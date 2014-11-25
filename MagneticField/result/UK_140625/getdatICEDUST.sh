#!/bin/bash

gridparamtitle="maxline/I current/D gradient/D normB/D normE/D type/C nX/I nY/I nZ/I dX/D dY/D dZ/D X0/D Y0/D Z0/D extendY/O extendYbits/I"
griddatatitle="X/F Y/F Z/F Bx/F By/F Bz/F"

# cap
input="cap1_X-1000to+3000_Z-600to+600.table"
output="TOSHIBA_20140625_CS"
echo "TTREE param" > $output
echo $gridparamtitle >> $output
echo "128 1 1 1 1 grid 121 61 401 10 10 10 -600 0 -1000 1 2" >> $output
echo "TTREE data" >> $output
echo $griddatatitle >> $output
gawk '{if ($1!="x"&&$1!="#") print (-$3)" "($2)" "($1)" "($6)" "($5)" "(-$4);}' $input >> $output
txt2root $output $output.root

# ts1
input="ts1_X+3000to+4650_Z-240to+240.table"
output="TOSHIBA_20140625_TS1"
echo "TTREE param" > $output
echo $gridparamtitle >> $output
echo "128 1 1 1 1 grid 49 25 166 10 10 10 -240 0 3000 1 2" >> $output
echo "TTREE data" >> $output
echo $griddatatitle >> $output
gawk '{if ($1!="x"&&$1!="#") print (-$3)" "($2)" "($1)" "($6)" "($5)" "(-$4);}' $input >> $output
txt2root $output $output.root

# ts2
input="ts2_X+4650to+7890_Z-3000to+240.table"
output="TOSHIBA_20140625_TS2"
echo "TTREE param" > $output
echo $gridparamtitle >> $output
echo "128 1 1 1 1 grid 325 25 325 10 10 10 -240 0 4650 1 2" >> $output
echo "TTREE data" >> $output
echo $griddatatitle >> $output
gawk '{delta=(sqrt((4650-$1)*(4650-$1)+(3000+$3)*(3000+$3))-3000);if ($1!="x"&&$1!="#"&&delta<300&&delta>-300) print (-$3)" "($2)" "($1)" "($6)" "($5)" "(-$4);}' $input >> $output
txt2root $output $output.root

# ts2DF
input="ts2_X+4650to+7890_Z-3000to+240.table"
output="TOSHIBA_20140625_TS2DF_1T"
echo "TTREE param" > $output
echo $gridparamtitle >> $output
echo "128 1 1 1 1 grid 325 25 325 10 10 10 -240 0 4650 1 0" >> $output
echo "TTREE data" >> $output
echo $griddatatitle >> $output
gawk '{delta=(sqrt((4650-$1)*(4650-$1)+(3000+$3)*(3000+$3))-3000);if ($1!="x"&&$1!="#"&&delta<300&&delta>-300) print (-$3)" "($2)" "($1)" "0" "1" "0;}' $input >> $output
txt2root $output $output.root

# bs
input="bs1_X+7410to+7890_Z-5000to-3000.table"
output="TOSHIBA_20140625_BS"
echo "TTREE param" > $output
echo $gridparamtitle >> $output
echo "128 1 1 1 1 grid 201 25 49 10 10 10 3000 0 7410 1 2" >> $output
echo "TTREE data" >> $output
echo $griddatatitle >> $output
gawk '{if ($1!="x"&&$1!="#") print (-$3)" "($2)" "($1)" "($6)" "($5)" "(-$4);}' $input >> $output
txt2root $output $output.root

# det
input="det1_X+6650to+8650_Z-9000to-5000.table"
output="TOSHIBA_20140625_DET"
echo "TTREE param" > $output
echo $gridparamtitle >> $output
echo "128 1 1 1 1 grid 401 101 201 10 10 10 5000 0 6650 1 2" >> $output
echo "TTREE data" >> $output
echo $griddatatitle >> $output
gawk '{if ($1!="x"&&$1!="#") print (-$3)" "($2)" "($1)" "($6)" "($5)" "(-$4);}' $input >> $output
txt2root $output $output.root
