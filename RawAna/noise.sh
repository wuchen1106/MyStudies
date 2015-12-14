#!/bin/bash

source ~/.setg4sim.sh
cd /scratchfs/bes/wuc/MyWorkArea/MyStudies/RawAna
((time ./eventBuilder 0 dir 2309650 CyDet.beam.all.0.0.90.90.10.16.4.4.4.1.8.8.1.35.Aluminium.Aluminium.Lead CyDet.dio.o50.0.0.90.90.10.16.5.4.4.1.6.8.1.35.Aluminium.Aluminium.Lead CyDet.pmc.0.0.90.90.10.16.5.4.4.1.6.8.1.35.Aluminium.Aluminium.Lead 100 1 1 46.3491694480 28.34 2815 1 2 3) > noise.log) 2>noise.err
