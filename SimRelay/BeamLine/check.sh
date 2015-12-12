#!/bin/bash
cd
source .setg4sim.sh
cd /scratchfs/bes/wuc/MyWorkArea/MyStudies/Beam/Analyse/beamnew

((time ./beam -M CSPTMonitor -r PTMonitor.pim.150916.g41001p02QBH -P -211) > log ) 2> err
