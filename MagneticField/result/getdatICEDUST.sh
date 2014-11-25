#!/bin/bash

cylparamtitle="maxline/I current/D gradient/D normB/D normE/D type/C nR/I nZ/I dR/D dZ/D R0/D Z0/D"
gridparamtitle="maxline/I current/D gradient/D normB/D normE/D type/C nX/I nY/I nZ/I dX/D dY/D dZ/D X0/D Y0/D Z0/D extendY/O extendYbits/I"
cyldatatitle="R/F Z/F Br/F Bz/F"
griddatatitle="X/F Y/F Z/F Bx/F By/F Bz/F"

## 140905
#input="IHEP_140905.dat"
#output="IHEP_A9_140905"
#echo "TTREE param" > $output
#echo $cylparamtitle >> $output
#echo "128 1 1 1 1 cylinder 200 500 10.02506265 10.02004008 5.0125313 3000" >> $output
#echo "TTREE data" >> $output
#echo $cyldatatitle >> $output
#gawk '{x=($1+3)*1000;y=(-$2+3)*1000;if ($1!="x"&&$1!="#"&&x>=0) print (x)" "(y)" "(-$4)" "($5);}' $input >> $output
#txt2root $output $output.root

## 140909s
#input="IHEP_140909s.dat"
#output="IHEP_A9_140909s"
#echo "TTREE param" > $output
#echo $gridparamtitle >> $output
#echo "128 1 1 1 1 grid 293 420 293 10 10 10 -1460 -1460 3000" >> $output
#echo "TTREE data" >> $output
#echo $griddatatitle >> $output
#gawk '{x=($1+3)*1000;y=($3)*1000;z=(-$2+3)*1000;if (NF>=6&&$1!="#") print (x)" "(y)" "(z)" "($4)" "($6)" "($5);}' $input >> $output
#txt2root $output $output.root

## 140909
#input="IHEP_140909.dat"
#output="IHEP_A9_140909"
#echo "TTREE param" > $output
#echo $gridparamtitle >> $output
#echo "128 1 1 1 1 grid 293 293 420 10 10 10 -1460 -1460 3000" >> $output
#echo "TTREE data" >> $output
#echo $griddatatitle >> $output
#gawk '{x=($1+300)*10;y=($3)*10;z=(-$2+300)*10;if (NF>=6&&$1!="#") print (x)" "(y)" "(z)" "($4/10000)" "($6/10000)" "($5/10000);}' $input >> $output
#txt2root $output $output.root

## 140924
#input="IHEP_140924.dat"
#output="IHEP_A9_140924"
#echo "TTREE param" > $output
#echo $cylparamtitle >> $output
#echo "128 1 1 1 1 cylinder 147 472 10 10 0 4650" >> $output
#echo "TTREE data" >> $output
#echo $cyldatatitle >> $output
#gawk '{x=($1+3)*1000;y=(-$2+3)*1000;if ($1!="x"&&$1!="#"&&x>=0) print (x)" "(y)" "(-$4)" "($5);}' $input >> $output
#txt2root $output $output.root

#input="IHEP_140924_TS3.dat"
#output="IHEP_A9_140924_TS3"
#echo "TTREE param" > $output
#echo $gridparamtitle >> $output
#echo "128 1 1 1 1 grid 147 75 66 10 10 10 -730 0 3000 1 2" >> $output
#echo "TTREE data" >> $output
#echo $griddatatitle >> $output
#gawk '{x=($1+300)*10;y=($3)*10;z=(-$2+300)*10;if (NF>=6&&$1!="#"&&y>=0) print (x)" "(y)" "(z)" "(-$4/10000)" "(-$6/10000)" "($5/10000);}' $input >> $output
#txt2root $output $output.root
#
#input="IHEP_140924_TS3a.dat"
#output="IHEP_A9_140924_TS3a"
#echo "TTREE param" > $output
#echo $gridparamtitle >> $output
#echo "128 1 1 1 1 grid 147 75 101 10 10 10 -730 0 3650 1 2" >> $output
#echo "TTREE data" >> $output
#echo $griddatatitle >> $output
#gawk '{x=($1+300)*10;y=($3)*10;z=(-$2+300)*10;if (NF>=6&&$1!="#"&&y>=0) print (x)" "(y)" "(z)" "(-$4/10000)" "(-$6/10000)" "($5/10000);}' $input >> $output
#txt2root $output $output.root

## 140929
#input="IHEP_140929.dat"
#output="IHEP_A9_140929"
#echo "TTREE param" > $output
#echo $cylparamtitle >> $output
#echo "128 1 1 1 1 cylinder 147 637 10 10 0 3000" >> $output
#echo "TTREE data" >> $output
#echo $cyldatatitle >> $output
#gawk '{x=($1+3)*1000;y=(-$2+3)*1000;if ($1!="x"&&$1!="#"&&x>=0) print (x)" "(y)" "(-$4)" "($5);}' $input >> $output
#txt2root $output $output.root

# 141022
input="IHEP_141022_TS3.dat"
output="IHEP_A9_141022_TS3"
echo "TTREE param" > $output
echo $cylparamtitle >> $output
echo "128 1 1 1 1 cylinder 74 166 10 10 0 3000" >> $output
echo "TTREE data" >> $output
echo $cyldatatitle >> $output
gawk '{x=($1+3)*1000;y=(-$2+3)*1000;if ($1!="X"&&$1!="#"&&x>=0) print (x)" "(y)" "(-$4)" "($5);}' $input >> $output
txt2root $output $output.root

input="IHEP_141022_BSDS.dat"
output="IHEP_A9_141022_BSDS"
echo "TTREE param" > $output
echo $cylparamtitle >> $output
echo "128 1 1 1 1 cylinder 147 472 10 10 0 4660" >> $output
echo "TTREE data" >> $output
echo $cyldatatitle >> $output
gawk '{x=($1+3)*1000;y=(-$2+3)*1000;if ($1!="X"&&$1!="#"&&x>=0) print (x)" "(y)" "(-$4)" "($5);}' $input >> $output
txt2root $output $output.root
