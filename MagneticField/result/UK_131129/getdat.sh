#!/bin/bash

gridparamtitle="maxline/I current/D gradient/D normB/D normE/D type/C nX/I nY/I nZ/I dX/D dY/D dZ/D X0/D Y0/D Z0/D extendY/O extendYbits/I"
griddatatitle="X/F Y/F Z/F Bx/F By/F Bz/F"

# cap
input="cap1-X_-1000to+3000.table"
output="TOSHIBA_20131129_CS"
echo "TTREE param" > $output
echo $gridparamtitle >> $output
echo "128 1 1 1 1 grid 401 61 121 10 10 10 4650 0 -600 1 3" >> $output
echo "TTREE data" >> $output
echo $griddatatitle >> $output
gawk '{if ($1!="x"&&$1!="#") print (7650-$1)" "($2)" "($3)" "($4)" "(-$5)" "($6);}' $input >> $output
txt2root $output $output.root

## ts1
#input="ts1_X+3000to+4650_Z-240to+240.table"
#output="TOSHIBA_20131129_TS1"
#echo "TTREE param" > $output
#echo $gridparamtitle >> $output
#echo "128 1 1 1 1 grid 166 25 49 10 10 10 3000 0 -240 1 3" >> $output
#echo "TTREE data" >> $output
#echo $griddatatitle >> $output
#gawk '{if ($1!="x"&&$1!="#") print (7650-$1)" "($2)" "($3)" "($4)" "(-$5)" "($6);}' $input >> $output
#txt2root $output $output.root

## ts2
#input="ts2_X+4650to+7890_Z-3000to+240.table"
#output="TOSHIBA_20131129_TS2"
#echo "TTREE param" > $output
#echo $gridparamtitle >> $output
#echo "128 1 1 1 1 grid 325 25 325 10 10 10 -240 0 -240 1 3" >> $output
#echo "TTREE data" >> $output
#echo $griddatatitle >> $output
#gawk '{if ($1!="x"&&$1!="#") print (7650-$1)" "($2)" "(-$3)" "($4)" "(-$5)" "($6);}' $input >> $output
#
## bs
#input="bs1_X+7410to+7890_Z-5000to-3000.table"
#output="TOSHIBA_20131129_BS"
#echo "TTREE param" > $output
#echo $gridparamtitle >> $output
#echo "128 1 1 1 1 grid 49 25 201 10 10 10 -240 0 3000 1 3" >> $output
#echo "TTREE data" >> $output
#echo $griddatatitle >> $output
#gawk '{if ($1!="x"&&$1!="#") print (7650-$1)" "($2)" "(-$3)" "($4)" "(-$5)" "($6);}' $input >> $output
#txt2root $output $output.root
#
## det
#input="det1_X+6650to+8650_Z-9000to-5000.table"
#output="TOSHIBA_20131129_DET"
#echo "TTREE param" > $output
#echo $gridparamtitle >> $output
#echo "128 1 1 1 1 grid 201 101 401 10 10 10 -1000 0 5000 1 3" >> $output
#echo "TTREE data" >> $output
#echo $griddatatitle >> $output
#gawk '{if ($1!="x"&&$1!="#") print (7650-$1)" "($2)" "(-$3)" "($4)" "(-$5)" "($6);}' $input >> $output
#txt2root $output $output.root
