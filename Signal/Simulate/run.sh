#!/bin/bash
export OUTPUTDIRROOT=/home/chen/MyWorkArea/Data/raw/g4sim/Signal_uni
export CONFIGUREROOT=/home/chen/MyWorkArea/Data/raw/g4sim/Signal_uni/configure
export CONFIGUREDATAROOT=/home/chen/MyWorkArea/Data/raw/g4sim/Signal_uni/configure/data
export LOGFILEROOT=/home/chen/MyWorkArea/g4sim/runlog/logfile
export MATERIALLISTROOT=material_list
export GEOCARDROOT=geometry_cdc_130629
export MAGFIELDCARDROOT=MagField_1T
export GENFILEROOT=gen_Signal
export OUTCARDROOT=output_Signal
nohup /home/chen/MyWorkArea/g4sim/g4sim /home/chen/MyWorkArea/Data/raw/g4sim/Signal_uni/macros/0_Signal.mac > /home/chen/MyWorkArea/Data/raw/g4sim/Signal_uni/0_simulateLog 2> /home/chen/MyWorkArea/Data/raw/g4sim/Signal_uni/0_simulateError &
nohup /home/chen/MyWorkArea/g4sim/g4sim /home/chen/MyWorkArea/Data/raw/g4sim/Signal_uni/macros/1_Signal.mac > /home/chen/MyWorkArea/Data/raw/g4sim/Signal_uni/1_simulateLog 2> /home/chen/MyWorkArea/Data/raw/g4sim/Signal_uni/1_simulateError &
nohup /home/chen/MyWorkArea/g4sim/g4sim /home/chen/MyWorkArea/Data/raw/g4sim/Signal_uni/macros/2_Signal.mac > /home/chen/MyWorkArea/Data/raw/g4sim/Signal_uni/2_simulateLog 2> /home/chen/MyWorkArea/Data/raw/g4sim/Signal_uni/2_simulateError &
nohup /home/chen/MyWorkArea/g4sim/g4sim /home/chen/MyWorkArea/Data/raw/g4sim/Signal_uni/macros/3_Signal.mac > /home/chen/MyWorkArea/Data/raw/g4sim/Signal_uni/3_simulateLog 2> /home/chen/MyWorkArea/Data/raw/g4sim/Signal_uni/3_simulateError &
nohup /home/chen/MyWorkArea/g4sim/g4sim /home/chen/MyWorkArea/Data/raw/g4sim/Signal_uni/macros/4_Signal.mac > /home/chen/MyWorkArea/Data/raw/g4sim/Signal_uni/4_simulateLog 2> /home/chen/MyWorkArea/Data/raw/g4sim/Signal_uni/4_simulateError &
nohup /home/chen/MyWorkArea/g4sim/g4sim /home/chen/MyWorkArea/Data/raw/g4sim/Signal_uni/macros/5_Signal.mac > /home/chen/MyWorkArea/Data/raw/g4sim/Signal_uni/5_simulateLog 2> /home/chen/MyWorkArea/Data/raw/g4sim/Signal_uni/5_simulateError &
nohup /home/chen/MyWorkArea/g4sim/g4sim /home/chen/MyWorkArea/Data/raw/g4sim/Signal_uni/macros/6_Signal.mac > /home/chen/MyWorkArea/Data/raw/g4sim/Signal_uni/6_simulateLog 2> /home/chen/MyWorkArea/Data/raw/g4sim/Signal_uni/6_simulateError &
nohup /home/chen/MyWorkArea/g4sim/g4sim /home/chen/MyWorkArea/Data/raw/g4sim/Signal_uni/macros/7_Signal.mac > /home/chen/MyWorkArea/Data/raw/g4sim/Signal_uni/7_simulateLog 2> /home/chen/MyWorkArea/Data/raw/g4sim/Signal_uni/7_simulateError &
