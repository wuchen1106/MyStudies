#!/bin/bash
export OUTPUTDIRROOT=/home/chen/MyWorkArea/Data/raw/g4sim/BE_05
export CONFIGUREROOT=/home/chen/MyWorkArea/Data/raw/g4sim/BE_05/configure
export CONFIGUREDATAROOT=/home/chen/MyWorkArea/Data/raw/g4sim/BE_05/configure/data
export LOGFILEROOT=/home/chen/MyWorkArea/g4sim/runlog/logfile
export MATERIALLISTROOT=material_list
export GEOCARDROOT=geometry_BE
export MAGFIELDCARDROOT=MagField_BE
export GENFILEROOT=gen_BE
export OUTCARDROOT=output_BE
nohup /home/chen/MyWorkArea/g4sim/g4sim /home/chen/MyWorkArea/Data/raw/g4sim/BE_05/macros/0_BE.mac > /home/chen/MyWorkArea/Data/raw/g4sim/BE_05/0_simulateLog 2> /home/chen/MyWorkArea/Data/raw/g4sim/BE_05/0_simulateError &
#nohup /home/chen/MyWorkArea/g4sim/g4sim /home/chen/MyWorkArea/Data/raw/g4sim/BE_05/macros/1_BE.mac > /home/chen/MyWorkArea/Data/raw/g4sim/BE_05/1_simulateLog 2> /home/chen/MyWorkArea/Data/raw/g4sim/BE_05/1_simulateError &
#nohup /home/chen/MyWorkArea/g4sim/g4sim /home/chen/MyWorkArea/Data/raw/g4sim/BE_05/macros/2_BE.mac > /home/chen/MyWorkArea/Data/raw/g4sim/BE_05/2_simulateLog 2> /home/chen/MyWorkArea/Data/raw/g4sim/BE_05/2_simulateError &
#nohup /home/chen/MyWorkArea/g4sim/g4sim /home/chen/MyWorkArea/Data/raw/g4sim/BE_05/macros/3_BE.mac > /home/chen/MyWorkArea/Data/raw/g4sim/BE_05/3_simulateLog 2> /home/chen/MyWorkArea/Data/raw/g4sim/BE_05/3_simulateError &
#nohup /home/chen/MyWorkArea/g4sim/g4sim /home/chen/MyWorkArea/Data/raw/g4sim/BE_05/macros/4_BE.mac > /home/chen/MyWorkArea/Data/raw/g4sim/BE_05/4_simulateLog 2> /home/chen/MyWorkArea/Data/raw/g4sim/BE_05/4_simulateError &
#nohup /home/chen/MyWorkArea/g4sim/g4sim /home/chen/MyWorkArea/Data/raw/g4sim/BE_05/macros/5_BE.mac > /home/chen/MyWorkArea/Data/raw/g4sim/BE_05/5_simulateLog 2> /home/chen/MyWorkArea/Data/raw/g4sim/BE_05/5_simulateError &
#nohup /home/chen/MyWorkArea/g4sim/g4sim /home/chen/MyWorkArea/Data/raw/g4sim/BE_05/macros/6_BE.mac > /home/chen/MyWorkArea/Data/raw/g4sim/BE_05/6_simulateLog 2> /home/chen/MyWorkArea/Data/raw/g4sim/BE_05/6_simulateError &
#nohup /home/chen/MyWorkArea/g4sim/g4sim /home/chen/MyWorkArea/Data/raw/g4sim/BE_05/macros/7_BE.mac > /home/chen/MyWorkArea/Data/raw/g4sim/BE_05/7_simulateLog 2> /home/chen/MyWorkArea/Data/raw/g4sim/BE_05/7_simulateError &
