#!/bin/bash
export OUTPUTDIRROOT=/home/chen/MyWorkArea/Data/raw/g4sim/Signal_wire
export CONFIGUREROOT=/home/chen/MyWorkArea/Data/raw/g4sim/Signal_wire/configure
export CONFIGUREDATAROOT=/home/chen/MyWorkArea/Data/raw/g4sim/Signal_wire/configure/data
export LOGFILEROOT=/home/chen/MyWorkArea/g4sim/runlog/logfile
export MATERIALLISTROOT=material_list
export GEOCARDROOT=geometry_Signal
export MAGFIELDCARDROOT=MagField_Signal
export GENFILEROOT=gen_Signal
export OUTCARDROOT=output_Signal
nohup /home/chen/MyWorkArea/g4sim/g4sim /home/chen/MyWorkArea/Data/raw/g4sim/Signal_wire/macros/0_Signal.mac > /home/chen/MyWorkArea/Data/raw/g4sim/Signal_wire/0_simulateLog 2> /home/chen/MyWorkArea/Data/raw/g4sim/Signal_wire/0_simulateError &
nohup /home/chen/MyWorkArea/g4sim/g4sim /home/chen/MyWorkArea/Data/raw/g4sim/Signal_wire/macros/1_Signal.mac > /home/chen/MyWorkArea/Data/raw/g4sim/Signal_wire/1_simulateLog 2> /home/chen/MyWorkArea/Data/raw/g4sim/Signal_wire/1_simulateError &
nohup /home/chen/MyWorkArea/g4sim/g4sim /home/chen/MyWorkArea/Data/raw/g4sim/Signal_wire/macros/2_Signal.mac > /home/chen/MyWorkArea/Data/raw/g4sim/Signal_wire/2_simulateLog 2> /home/chen/MyWorkArea/Data/raw/g4sim/Signal_wire/2_simulateError &
nohup /home/chen/MyWorkArea/g4sim/g4sim /home/chen/MyWorkArea/Data/raw/g4sim/Signal_wire/macros/3_Signal.mac > /home/chen/MyWorkArea/Data/raw/g4sim/Signal_wire/3_simulateLog 2> /home/chen/MyWorkArea/Data/raw/g4sim/Signal_wire/3_simulateError &
nohup /home/chen/MyWorkArea/g4sim/g4sim /home/chen/MyWorkArea/Data/raw/g4sim/Signal_wire/macros/4_Signal.mac > /home/chen/MyWorkArea/Data/raw/g4sim/Signal_wire/4_simulateLog 2> /home/chen/MyWorkArea/Data/raw/g4sim/Signal_wire/4_simulateError &
nohup /home/chen/MyWorkArea/g4sim/g4sim /home/chen/MyWorkArea/Data/raw/g4sim/Signal_wire/macros/5_Signal.mac > /home/chen/MyWorkArea/Data/raw/g4sim/Signal_wire/5_simulateLog 2> /home/chen/MyWorkArea/Data/raw/g4sim/Signal_wire/5_simulateError &
nohup /home/chen/MyWorkArea/g4sim/g4sim /home/chen/MyWorkArea/Data/raw/g4sim/Signal_wire/macros/6_Signal.mac > /home/chen/MyWorkArea/Data/raw/g4sim/Signal_wire/6_simulateLog 2> /home/chen/MyWorkArea/Data/raw/g4sim/Signal_wire/6_simulateError &
nohup /home/chen/MyWorkArea/g4sim/g4sim /home/chen/MyWorkArea/Data/raw/g4sim/Signal_wire/macros/7_Signal.mac > /home/chen/MyWorkArea/Data/raw/g4sim/Signal_wire/7_simulateLog 2> /home/chen/MyWorkArea/Data/raw/g4sim/Signal_wire/7_simulateError &
