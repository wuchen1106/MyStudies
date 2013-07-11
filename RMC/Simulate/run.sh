#!/bin/bash
export OUTPUTDIRROOT=/home/chen/MyWorkArea/Data/raw/g4sim/RMC_bm1
export CONFIGUREROOT=/home/chen/MyWorkArea/Data/raw/g4sim/RMC_bm1/configure
export CONFIGUREDATAROOT=/home/chen/MyWorkArea/Data/raw/g4sim/RMC_bm1/configure/data
export LOGFILEROOT=/home/chen/MyWorkArea/g4sim/runlog/logfile
export MATERIALLISTROOT=material_list
export GEOCARDROOT=geometry_bm
export MAGFIELDCARDROOT=MagField_1T
export GENFILEROOT=gen_bm
export OUTCARDROOT=output_bm
nohup /home/chen/MyWorkArea/g4sim/g4sim /home/chen/MyWorkArea/Data/raw/g4sim/RMC_bm1/macros/0_bm.mac > /home/chen/MyWorkArea/Data/raw/g4sim/RMC_bm1/0_simulateLog 2> /home/chen/MyWorkArea/Data/raw/g4sim/RMC_bm1/0_simulateError &
nohup /home/chen/MyWorkArea/g4sim/g4sim /home/chen/MyWorkArea/Data/raw/g4sim/RMC_bm1/macros/1_bm.mac > /home/chen/MyWorkArea/Data/raw/g4sim/RMC_bm1/1_simulateLog 2> /home/chen/MyWorkArea/Data/raw/g4sim/RMC_bm1/1_simulateError &
nohup /home/chen/MyWorkArea/g4sim/g4sim /home/chen/MyWorkArea/Data/raw/g4sim/RMC_bm1/macros/2_bm.mac > /home/chen/MyWorkArea/Data/raw/g4sim/RMC_bm1/2_simulateLog 2> /home/chen/MyWorkArea/Data/raw/g4sim/RMC_bm1/2_simulateError &
nohup /home/chen/MyWorkArea/g4sim/g4sim /home/chen/MyWorkArea/Data/raw/g4sim/RMC_bm1/macros/3_bm.mac > /home/chen/MyWorkArea/Data/raw/g4sim/RMC_bm1/3_simulateLog 2> /home/chen/MyWorkArea/Data/raw/g4sim/RMC_bm1/3_simulateError &
nohup /home/chen/MyWorkArea/g4sim/g4sim /home/chen/MyWorkArea/Data/raw/g4sim/RMC_bm1/macros/4_bm.mac > /home/chen/MyWorkArea/Data/raw/g4sim/RMC_bm1/4_simulateLog 2> /home/chen/MyWorkArea/Data/raw/g4sim/RMC_bm1/4_simulateError &
nohup /home/chen/MyWorkArea/g4sim/g4sim /home/chen/MyWorkArea/Data/raw/g4sim/RMC_bm1/macros/5_bm.mac > /home/chen/MyWorkArea/Data/raw/g4sim/RMC_bm1/5_simulateLog 2> /home/chen/MyWorkArea/Data/raw/g4sim/RMC_bm1/5_simulateError &
nohup /home/chen/MyWorkArea/g4sim/g4sim /home/chen/MyWorkArea/Data/raw/g4sim/RMC_bm1/macros/6_bm.mac > /home/chen/MyWorkArea/Data/raw/g4sim/RMC_bm1/6_simulateLog 2> /home/chen/MyWorkArea/Data/raw/g4sim/RMC_bm1/6_simulateError &
nohup /home/chen/MyWorkArea/g4sim/g4sim /home/chen/MyWorkArea/Data/raw/g4sim/RMC_bm1/macros/7_bm.mac > /home/chen/MyWorkArea/Data/raw/g4sim/RMC_bm1/7_simulateLog 2> /home/chen/MyWorkArea/Data/raw/g4sim/RMC_bm1/7_simulateError &
