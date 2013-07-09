#!/bin/bash
export OUTPUTDIRROOT=/home/chen/MyWorkArea/Data/raw/g4sim/RMC_ph2e2
export CONFIGUREROOT=/home/chen/MyWorkArea/Data/raw/g4sim/RMC_ph2e2/configure
export CONFIGUREDATAROOT=/home/chen/MyWorkArea/Data/raw/g4sim/RMC_ph2e2/configure/data
export LOGFILEROOT=/home/chen/MyWorkArea/g4sim/runlog/logfile
export MATERIALLISTROOT=material_list
export GEOCARDROOT=geometry_ph2e
export MAGFIELDCARDROOT=MagField_1T
export GENFILEROOT=gen_ph2e
export OUTCARDROOT=output_ph2e
nohup /home/chen/MyWorkArea/g4sim/g4sim /home/chen/MyWorkArea/Data/raw/g4sim/RMC_ph2e2/macros/0_ph2e.mac > /home/chen/MyWorkArea/Data/raw/g4sim/RMC_ph2e2/0_simulateLog 2> /home/chen/MyWorkArea/Data/raw/g4sim/RMC_ph2e2/0_simulateError &
nohup /home/chen/MyWorkArea/g4sim/g4sim /home/chen/MyWorkArea/Data/raw/g4sim/RMC_ph2e2/macros/1_ph2e.mac > /home/chen/MyWorkArea/Data/raw/g4sim/RMC_ph2e2/1_simulateLog 2> /home/chen/MyWorkArea/Data/raw/g4sim/RMC_ph2e2/1_simulateError &
nohup /home/chen/MyWorkArea/g4sim/g4sim /home/chen/MyWorkArea/Data/raw/g4sim/RMC_ph2e2/macros/2_ph2e.mac > /home/chen/MyWorkArea/Data/raw/g4sim/RMC_ph2e2/2_simulateLog 2> /home/chen/MyWorkArea/Data/raw/g4sim/RMC_ph2e2/2_simulateError &
nohup /home/chen/MyWorkArea/g4sim/g4sim /home/chen/MyWorkArea/Data/raw/g4sim/RMC_ph2e2/macros/3_ph2e.mac > /home/chen/MyWorkArea/Data/raw/g4sim/RMC_ph2e2/3_simulateLog 2> /home/chen/MyWorkArea/Data/raw/g4sim/RMC_ph2e2/3_simulateError &
nohup /home/chen/MyWorkArea/g4sim/g4sim /home/chen/MyWorkArea/Data/raw/g4sim/RMC_ph2e2/macros/4_ph2e.mac > /home/chen/MyWorkArea/Data/raw/g4sim/RMC_ph2e2/4_simulateLog 2> /home/chen/MyWorkArea/Data/raw/g4sim/RMC_ph2e2/4_simulateError &
nohup /home/chen/MyWorkArea/g4sim/g4sim /home/chen/MyWorkArea/Data/raw/g4sim/RMC_ph2e2/macros/5_ph2e.mac > /home/chen/MyWorkArea/Data/raw/g4sim/RMC_ph2e2/5_simulateLog 2> /home/chen/MyWorkArea/Data/raw/g4sim/RMC_ph2e2/5_simulateError &
nohup /home/chen/MyWorkArea/g4sim/g4sim /home/chen/MyWorkArea/Data/raw/g4sim/RMC_ph2e2/macros/6_ph2e.mac > /home/chen/MyWorkArea/Data/raw/g4sim/RMC_ph2e2/6_simulateLog 2> /home/chen/MyWorkArea/Data/raw/g4sim/RMC_ph2e2/6_simulateError &
nohup /home/chen/MyWorkArea/g4sim/g4sim /home/chen/MyWorkArea/Data/raw/g4sim/RMC_ph2e2/macros/7_ph2e.mac > /home/chen/MyWorkArea/Data/raw/g4sim/RMC_ph2e2/7_simulateLog 2> /home/chen/MyWorkArea/Data/raw/g4sim/RMC_ph2e2/7_simulateError &
