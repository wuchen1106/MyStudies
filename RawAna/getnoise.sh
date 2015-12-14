#!/bin/bash

source ~/.setg4sim.sh
cd /scratchfs/bes/wuc/MyWorkArea/MyStudies/RawAna
name="0.0.90.90.10.16.5.4.4.1.6.8.1.35.Aluminium.Aluminium.Lead"
#((time ./getNoise noise.$name dir 3934331 CyDet.beam.all.$name CyDet.dio.o50.$name CyDet.pmc.$name 4 1 1 4.63491694480 28.34 2815 1 2 3) > log) 2>err
((time ./getNoise noise.$name dir 3934331 CyDet.beam.all.$name CyDet.dio.o50.$name CyDet.pmc.$name 1 1 1 1 28.34 2815 1 2 3) > log) 2>err
