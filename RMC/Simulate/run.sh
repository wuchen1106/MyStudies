#!/bin/bash
export OUTPUTDIRROOT=/home/chen/MyWorkArea/Data/raw/g4sim/RMC_e
export CONFIGUREROOT=/home/chen/MyWorkArea/Data/raw/g4sim/RMC_e/configure
export CONFIGUREDATAROOT=/home/chen/MyWorkArea/Data/raw/g4sim/RMC_e/configure/data
export LOGFILEROOT=/home/chen/MyWorkArea/g4sim/runlog/logfile
export MATERIALLISTROOT=material_list
export GEOCARDROOT=geometry_cdc_130629
export MAGFIELDCARDROOT=MagField_1T
export GENFILEROOT=gen_rmce
export OUTCARDROOT=output_rmce
nohup /home/chen/MyWorkArea/g4sim/g4sim /home/chen/MyWorkArea/Data/raw/g4sim/RMC_e/macros/0_rmce.mac > /home/chen/MyWorkArea/Data/raw/g4sim/RMC_e/0_simulateLog 2> /home/chen/MyWorkArea/Data/raw/g4sim/RMC_e/0_simulateError &
nohup /home/chen/MyWorkArea/g4sim/g4sim /home/chen/MyWorkArea/Data/raw/g4sim/RMC_e/macros/1_rmce.mac > /home/chen/MyWorkArea/Data/raw/g4sim/RMC_e/1_simulateLog 2> /home/chen/MyWorkArea/Data/raw/g4sim/RMC_e/1_simulateError &
nohup /home/chen/MyWorkArea/g4sim/g4sim /home/chen/MyWorkArea/Data/raw/g4sim/RMC_e/macros/2_rmce.mac > /home/chen/MyWorkArea/Data/raw/g4sim/RMC_e/2_simulateLog 2> /home/chen/MyWorkArea/Data/raw/g4sim/RMC_e/2_simulateError &
nohup /home/chen/MyWorkArea/g4sim/g4sim /home/chen/MyWorkArea/Data/raw/g4sim/RMC_e/macros/3_rmce.mac > /home/chen/MyWorkArea/Data/raw/g4sim/RMC_e/3_simulateLog 2> /home/chen/MyWorkArea/Data/raw/g4sim/RMC_e/3_simulateError &
nohup /home/chen/MyWorkArea/g4sim/g4sim /home/chen/MyWorkArea/Data/raw/g4sim/RMC_e/macros/4_rmce.mac > /home/chen/MyWorkArea/Data/raw/g4sim/RMC_e/4_simulateLog 2> /home/chen/MyWorkArea/Data/raw/g4sim/RMC_e/4_simulateError &
nohup /home/chen/MyWorkArea/g4sim/g4sim /home/chen/MyWorkArea/Data/raw/g4sim/RMC_e/macros/5_rmce.mac > /home/chen/MyWorkArea/Data/raw/g4sim/RMC_e/5_simulateLog 2> /home/chen/MyWorkArea/Data/raw/g4sim/RMC_e/5_simulateError &
nohup /home/chen/MyWorkArea/g4sim/g4sim /home/chen/MyWorkArea/Data/raw/g4sim/RMC_e/macros/6_rmce.mac > /home/chen/MyWorkArea/Data/raw/g4sim/RMC_e/6_simulateLog 2> /home/chen/MyWorkArea/Data/raw/g4sim/RMC_e/6_simulateError &
nohup /home/chen/MyWorkArea/g4sim/g4sim /home/chen/MyWorkArea/Data/raw/g4sim/RMC_e/macros/7_rmce.mac > /home/chen/MyWorkArea/Data/raw/g4sim/RMC_e/7_simulateLog 2> /home/chen/MyWorkArea/Data/raw/g4sim/RMC_e/7_simulateError &
