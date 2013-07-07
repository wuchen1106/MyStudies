#!/bin/bash
export OUTPUTDIRROOT=/home/chen/MyWorkArea/Data/raw/g4sim/RMC_ph2e
export CONFIGUREROOT=/home/chen/MyWorkArea/Data/raw/g4sim/RMC_ph2e/configure
export CONFIGUREDATAROOT=/home/chen/MyWorkArea/Data/raw/g4sim/RMC_ph2e/configure/data
export LOGFILEROOT=/home/chen/MyWorkArea/g4sim/runlog/logfile
export MATERIALLISTROOT=material_list
export GEOCARDROOT=geometry_ph2e
export MAGFIELDCARDROOT=MagField_ph2e
export GENFILEROOT=gen_ph2e
export OUTCARDROOT=output_ph2e
nohup /home/chen/MyWorkArea/g4sim/g4sim /home/chen/MyWorkArea/Data/raw/g4sim/RMC_ph2e/macros/0_ph2e.mac > /home/chen/MyWorkArea/Data/raw/g4sim/RMC_ph2e/0_simulateLog 2> /home/chen/MyWorkArea/Data/raw/g4sim/RMC_ph2e/0_simulateError &
nohup /home/chen/MyWorkArea/g4sim/g4sim /home/chen/MyWorkArea/Data/raw/g4sim/RMC_ph2e/macros/1_ph2e.mac > /home/chen/MyWorkArea/Data/raw/g4sim/RMC_ph2e/1_simulateLog 2> /home/chen/MyWorkArea/Data/raw/g4sim/RMC_ph2e/1_simulateError &
nohup /home/chen/MyWorkArea/g4sim/g4sim /home/chen/MyWorkArea/Data/raw/g4sim/RMC_ph2e/macros/2_ph2e.mac > /home/chen/MyWorkArea/Data/raw/g4sim/RMC_ph2e/2_simulateLog 2> /home/chen/MyWorkArea/Data/raw/g4sim/RMC_ph2e/2_simulateError &
nohup /home/chen/MyWorkArea/g4sim/g4sim /home/chen/MyWorkArea/Data/raw/g4sim/RMC_ph2e/macros/3_ph2e.mac > /home/chen/MyWorkArea/Data/raw/g4sim/RMC_ph2e/3_simulateLog 2> /home/chen/MyWorkArea/Data/raw/g4sim/RMC_ph2e/3_simulateError &
nohup /home/chen/MyWorkArea/g4sim/g4sim /home/chen/MyWorkArea/Data/raw/g4sim/RMC_ph2e/macros/4_ph2e.mac > /home/chen/MyWorkArea/Data/raw/g4sim/RMC_ph2e/4_simulateLog 2> /home/chen/MyWorkArea/Data/raw/g4sim/RMC_ph2e/4_simulateError &
nohup /home/chen/MyWorkArea/g4sim/g4sim /home/chen/MyWorkArea/Data/raw/g4sim/RMC_ph2e/macros/5_ph2e.mac > /home/chen/MyWorkArea/Data/raw/g4sim/RMC_ph2e/5_simulateLog 2> /home/chen/MyWorkArea/Data/raw/g4sim/RMC_ph2e/5_simulateError &
nohup /home/chen/MyWorkArea/g4sim/g4sim /home/chen/MyWorkArea/Data/raw/g4sim/RMC_ph2e/macros/6_ph2e.mac > /home/chen/MyWorkArea/Data/raw/g4sim/RMC_ph2e/6_simulateLog 2> /home/chen/MyWorkArea/Data/raw/g4sim/RMC_ph2e/6_simulateError &
nohup /home/chen/MyWorkArea/g4sim/g4sim /home/chen/MyWorkArea/Data/raw/g4sim/RMC_ph2e/macros/7_ph2e.mac > /home/chen/MyWorkArea/Data/raw/g4sim/RMC_ph2e/7_simulateLog 2> /home/chen/MyWorkArea/Data/raw/g4sim/RMC_ph2e/7_simulateError &
