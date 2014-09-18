#!/bin/bash

#gawk 'BEGIN{pi=3.1415927;sin10=sin(10*pi/180);cos10=cos(10*pi/180);}{if ($1=="h") print "x y z px py pz"; else if ($1=="#") print "mm mm mm MeV MeV MeV"; else {ph=$5*1000*sin($2/1000);py=$5*1000*sin($4/1000);pl=($5*sqrt(1-sin($2/1000)*sin($2/1000)-sin($4/1000)*sin($4/1000))*1000);print 7650-8000*cos10+$1*10*sin10"  "($3*10)"  "(-8000*sin10-$1*10*cos10)"  "(ph*sin10+pl*cos10)"  "(py)"  "(-ph*cos10+pl*sin10);}}' $1 > $2
gawk 'BEGIN{pi=3.1415927;sin10=sin(10*pi/180);cos10=cos(10*pi/180);}{if ($1=="h") print "x y z px py pz"; else if ($1=="#") print "mm mm mm MeV MeV MeV"; else {ph=$5*1000*sin($2/1000);py=$5*1000*sin($4/1000);pl=($5*sqrt(1-sin($2/1000)*sin($2/1000)-sin($4/1000)*sin($4/1000))*1000);print 7650-310*cos10+$1*10*sin10"  "($3*10)"  "(-310*sin10-$1*10*cos10)"  "(ph*sin10+pl*cos10)"  "(py)"  "(-ph*cos10+pl*sin10);}}' $1 > $2
txt2root $2 $2.root
