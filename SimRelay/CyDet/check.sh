#!/bin/bash

source .setg4sim.sh
cd /scratchfs/bes/wuc/MyWorkArea/MyStudies/SimRelay/CyDet
((time ./getTracks A9.ALL.150919.W500um.1mmCFRP 1)>log)2>err

