#!/bin/bash

#source ~/.setg4sim.sh
#cd /scratchfs/bes/wuc/MyWorkArea/MyStudies/RawAna
#name="0.0.90.90.10.16.5.4.4.1.6.8.1.35.Aluminium.Aluminium.Lead"
#((time ./eventBuilder 1 dir 2309650 CyDet.beam.all.$name CyDet.dio.o50.$name CyDet.pmc.$name CyDet.signal.$name 100 1 1 1 46.3491694480 28.34 2815 1 1 2 3 0 ) > signal.log) 2> signal.err
./eventBuilder 1 file 1 ~/MyWorkArea/Simulate/comet/output/signal.160121.root 1 0
