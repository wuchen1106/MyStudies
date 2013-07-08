#!/bin/bash
export OUTPUTDIRROOT=/home/chen/MyWorkArea/Data/raw/g4sim/RMC_bm
export CONFIGUREROOT=/home/chen/MyWorkArea/Data/raw/g4sim/RMC_bm/configure
export CONFIGUREDATAROOT=/home/chen/MyWorkArea/Data/raw/g4sim/RMC_bm/configure/data
export LOGFILEROOT=/home/chen/MyWorkArea/g4sim/runlog/logfile
export MATERIALLISTROOT=material_list
export GEOCARDROOT=geometry_bm
export MAGFIELDCARDROOT=MagField_1T
export GENFILEROOT=gen_bm
export OUTCARDROOT=output_bm
nohup /home/chen/MyWorkArea/g4sim/g4sim /home/chen/MyWorkArea/Data/raw/g4sim/RMC_bm/macros/0_bm.mac > /home/chen/MyWorkArea/Data/raw/g4sim/RMC_bm/0_simulateLog 2> /home/chen/MyWorkArea/Data/raw/g4sim/RMC_bm/0_simulateError &
nohup /home/chen/MyWorkArea/g4sim/g4sim /home/chen/MyWorkArea/Data/raw/g4sim/RMC_bm/macros/1_bm.mac > /home/chen/MyWorkArea/Data/raw/g4sim/RMC_bm/1_simulateLog 2> /home/chen/MyWorkArea/Data/raw/g4sim/RMC_bm/1_simulateError &
nohup /home/chen/MyWorkArea/g4sim/g4sim /home/chen/MyWorkArea/Data/raw/g4sim/RMC_bm/macros/2_bm.mac > /home/chen/MyWorkArea/Data/raw/g4sim/RMC_bm/2_simulateLog 2> /home/chen/MyWorkArea/Data/raw/g4sim/RMC_bm/2_simulateError &
nohup /home/chen/MyWorkArea/g4sim/g4sim /home/chen/MyWorkArea/Data/raw/g4sim/RMC_bm/macros/3_bm.mac > /home/chen/MyWorkArea/Data/raw/g4sim/RMC_bm/3_simulateLog 2> /home/chen/MyWorkArea/Data/raw/g4sim/RMC_bm/3_simulateError &
nohup /home/chen/MyWorkArea/g4sim/g4sim /home/chen/MyWorkArea/Data/raw/g4sim/RMC_bm/macros/4_bm.mac > /home/chen/MyWorkArea/Data/raw/g4sim/RMC_bm/4_simulateLog 2> /home/chen/MyWorkArea/Data/raw/g4sim/RMC_bm/4_simulateError &
nohup /home/chen/MyWorkArea/g4sim/g4sim /home/chen/MyWorkArea/Data/raw/g4sim/RMC_bm/macros/5_bm.mac > /home/chen/MyWorkArea/Data/raw/g4sim/RMC_bm/5_simulateLog 2> /home/chen/MyWorkArea/Data/raw/g4sim/RMC_bm/5_simulateError &
nohup /home/chen/MyWorkArea/g4sim/g4sim /home/chen/MyWorkArea/Data/raw/g4sim/RMC_bm/macros/6_bm.mac > /home/chen/MyWorkArea/Data/raw/g4sim/RMC_bm/6_simulateLog 2> /home/chen/MyWorkArea/Data/raw/g4sim/RMC_bm/6_simulateError &
nohup /home/chen/MyWorkArea/g4sim/g4sim /home/chen/MyWorkArea/Data/raw/g4sim/RMC_bm/macros/7_bm.mac > /home/chen/MyWorkArea/Data/raw/g4sim/RMC_bm/7_simulateLog 2> /home/chen/MyWorkArea/Data/raw/g4sim/RMC_bm/7_simulateError &
