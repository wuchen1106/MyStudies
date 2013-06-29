#!/bin/bash
export OUTPUTDIRROOT=/home/chen/MyWorkArea/Data/raw/g4sim/Signal002
export CONFIGUREROOT=/home/chen/MyWorkArea/Data/raw/g4sim/Signal002/configure
export CONFIGUREDATAROOT=/home/chen/MyWorkArea/Data/raw/g4sim/Signal002/configure/data
export LOGFILEROOT=/home/chen/MyWorkArea/g4sim/runlog/logfile
export MATERIALLISTROOT=material_list
export GEOCARDROOT=geometry_Signal
export MAGFIELDCARDROOT=MagField_Signal
export GENFILEROOT=gen_Signal
export OUTCARDROOT=output_Signal
nohup /home/chen/MyWorkArea/g4sim/g4sim /home/chen/MyWorkArea/Data/raw/g4sim/Signal002/macros/0_Signal.mac > /home/chen/MyWorkArea/Data/raw/g4sim/Signal002/0_simulateLog 2> /home/chen/MyWorkArea/Data/raw/g4sim/Signal002/0_simulateError &
nohup /home/chen/MyWorkArea/g4sim/g4sim /home/chen/MyWorkArea/Data/raw/g4sim/Signal002/macros/1_Signal.mac > /home/chen/MyWorkArea/Data/raw/g4sim/Signal002/1_simulateLog 2> /home/chen/MyWorkArea/Data/raw/g4sim/Signal002/1_simulateError &
nohup /home/chen/MyWorkArea/g4sim/g4sim /home/chen/MyWorkArea/Data/raw/g4sim/Signal002/macros/2_Signal.mac > /home/chen/MyWorkArea/Data/raw/g4sim/Signal002/2_simulateLog 2> /home/chen/MyWorkArea/Data/raw/g4sim/Signal002/2_simulateError &
nohup /home/chen/MyWorkArea/g4sim/g4sim /home/chen/MyWorkArea/Data/raw/g4sim/Signal002/macros/3_Signal.mac > /home/chen/MyWorkArea/Data/raw/g4sim/Signal002/3_simulateLog 2> /home/chen/MyWorkArea/Data/raw/g4sim/Signal002/3_simulateError &
nohup /home/chen/MyWorkArea/g4sim/g4sim /home/chen/MyWorkArea/Data/raw/g4sim/Signal002/macros/4_Signal.mac > /home/chen/MyWorkArea/Data/raw/g4sim/Signal002/4_simulateLog 2> /home/chen/MyWorkArea/Data/raw/g4sim/Signal002/4_simulateError &
nohup /home/chen/MyWorkArea/g4sim/g4sim /home/chen/MyWorkArea/Data/raw/g4sim/Signal002/macros/5_Signal.mac > /home/chen/MyWorkArea/Data/raw/g4sim/Signal002/5_simulateLog 2> /home/chen/MyWorkArea/Data/raw/g4sim/Signal002/5_simulateError &
nohup /home/chen/MyWorkArea/g4sim/g4sim /home/chen/MyWorkArea/Data/raw/g4sim/Signal002/macros/6_Signal.mac > /home/chen/MyWorkArea/Data/raw/g4sim/Signal002/6_simulateLog 2> /home/chen/MyWorkArea/Data/raw/g4sim/Signal002/6_simulateError &
nohup /home/chen/MyWorkArea/g4sim/g4sim /home/chen/MyWorkArea/Data/raw/g4sim/Signal002/macros/7_Signal.mac > /home/chen/MyWorkArea/Data/raw/g4sim/Signal002/7_simulateLog 2> /home/chen/MyWorkArea/Data/raw/g4sim/Signal002/7_simulateError &
