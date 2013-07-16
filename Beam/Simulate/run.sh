#!/bin/bash
export OUTPUTDIRROOT=/home/chen/MyWorkArea/Data/raw/g4sim/Beam_ip
export CONFIGUREROOT=/home/chen/MyWorkArea/Data/raw/g4sim/Beam_ip/configure
export CONFIGUREDATAROOT=/home/chen/MyWorkArea/Data/raw/g4sim/Beam_ip/configure/data
export LOGFILEROOT=/home/chen/MyWorkArea/g4sim/runlog/logfile
export MATERIALLISTROOT=material_list
export GEOCARDROOT=geometry_PrTgt
export MAGFIELDCARDROOT=MagField_0T
export GENFILEROOT=gen_ip
export OUTCARDROOT=output_ip
nohup /home/chen/MyWorkArea/g4sim/g4sim /home/chen/MyWorkArea/Data/raw/g4sim/Beam_ip/macros/0_ip.mac > /home/chen/MyWorkArea/Data/raw/g4sim/Beam_ip/0_simulateLog 2> /home/chen/MyWorkArea/Data/raw/g4sim/Beam_ip/0_simulateError &
nohup /home/chen/MyWorkArea/g4sim/g4sim /home/chen/MyWorkArea/Data/raw/g4sim/Beam_ip/macros/1_ip.mac > /home/chen/MyWorkArea/Data/raw/g4sim/Beam_ip/1_simulateLog 2> /home/chen/MyWorkArea/Data/raw/g4sim/Beam_ip/1_simulateError &
nohup /home/chen/MyWorkArea/g4sim/g4sim /home/chen/MyWorkArea/Data/raw/g4sim/Beam_ip/macros/2_ip.mac > /home/chen/MyWorkArea/Data/raw/g4sim/Beam_ip/2_simulateLog 2> /home/chen/MyWorkArea/Data/raw/g4sim/Beam_ip/2_simulateError &
nohup /home/chen/MyWorkArea/g4sim/g4sim /home/chen/MyWorkArea/Data/raw/g4sim/Beam_ip/macros/3_ip.mac > /home/chen/MyWorkArea/Data/raw/g4sim/Beam_ip/3_simulateLog 2> /home/chen/MyWorkArea/Data/raw/g4sim/Beam_ip/3_simulateError &
nohup /home/chen/MyWorkArea/g4sim/g4sim /home/chen/MyWorkArea/Data/raw/g4sim/Beam_ip/macros/4_ip.mac > /home/chen/MyWorkArea/Data/raw/g4sim/Beam_ip/4_simulateLog 2> /home/chen/MyWorkArea/Data/raw/g4sim/Beam_ip/4_simulateError &
nohup /home/chen/MyWorkArea/g4sim/g4sim /home/chen/MyWorkArea/Data/raw/g4sim/Beam_ip/macros/5_ip.mac > /home/chen/MyWorkArea/Data/raw/g4sim/Beam_ip/5_simulateLog 2> /home/chen/MyWorkArea/Data/raw/g4sim/Beam_ip/5_simulateError &
nohup /home/chen/MyWorkArea/g4sim/g4sim /home/chen/MyWorkArea/Data/raw/g4sim/Beam_ip/macros/6_ip.mac > /home/chen/MyWorkArea/Data/raw/g4sim/Beam_ip/6_simulateLog 2> /home/chen/MyWorkArea/Data/raw/g4sim/Beam_ip/6_simulateError &
nohup /home/chen/MyWorkArea/g4sim/g4sim /home/chen/MyWorkArea/Data/raw/g4sim/Beam_ip/macros/7_ip.mac > /home/chen/MyWorkArea/Data/raw/g4sim/Beam_ip/7_simulateLog 2> /home/chen/MyWorkArea/Data/raw/g4sim/Beam_ip/7_simulateError &
