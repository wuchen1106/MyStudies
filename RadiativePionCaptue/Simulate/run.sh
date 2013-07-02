#!/bin/bash
export OUTPUTDIRROOT=/home/chen/MyWorkArea/Data/raw/g4sim/RPC_wire
export CONFIGUREROOT=/home/chen/MyWorkArea/Data/raw/g4sim/RPC_wire/configure
export CONFIGUREDATAROOT=/home/chen/MyWorkArea/Data/raw/g4sim/RPC_wire/configure/data
export LOGFILEROOT=/home/chen/MyWorkArea/g4sim/runlog/logfile
export MATERIALLISTROOT=material_list
export GEOCARDROOT=geometry_RPC
export MAGFIELDCARDROOT=MagField_RPC
export GENFILEROOT=gen_RPC
export OUTCARDROOT=output_RPC
nohup /home/chen/MyWorkArea/g4sim/g4sim /home/chen/MyWorkArea/Data/raw/g4sim/RPC_wire/macros/0_RPC.mac > /home/chen/MyWorkArea/Data/raw/g4sim/RPC_wire/0_simulateLog 2> /home/chen/MyWorkArea/Data/raw/g4sim/RPC_wire/0_simulateError &
nohup /home/chen/MyWorkArea/g4sim/g4sim /home/chen/MyWorkArea/Data/raw/g4sim/RPC_wire/macros/1_RPC.mac > /home/chen/MyWorkArea/Data/raw/g4sim/RPC_wire/1_simulateLog 2> /home/chen/MyWorkArea/Data/raw/g4sim/RPC_wire/1_simulateError &
nohup /home/chen/MyWorkArea/g4sim/g4sim /home/chen/MyWorkArea/Data/raw/g4sim/RPC_wire/macros/2_RPC.mac > /home/chen/MyWorkArea/Data/raw/g4sim/RPC_wire/2_simulateLog 2> /home/chen/MyWorkArea/Data/raw/g4sim/RPC_wire/2_simulateError &
nohup /home/chen/MyWorkArea/g4sim/g4sim /home/chen/MyWorkArea/Data/raw/g4sim/RPC_wire/macros/3_RPC.mac > /home/chen/MyWorkArea/Data/raw/g4sim/RPC_wire/3_simulateLog 2> /home/chen/MyWorkArea/Data/raw/g4sim/RPC_wire/3_simulateError &
nohup /home/chen/MyWorkArea/g4sim/g4sim /home/chen/MyWorkArea/Data/raw/g4sim/RPC_wire/macros/4_RPC.mac > /home/chen/MyWorkArea/Data/raw/g4sim/RPC_wire/4_simulateLog 2> /home/chen/MyWorkArea/Data/raw/g4sim/RPC_wire/4_simulateError &
nohup /home/chen/MyWorkArea/g4sim/g4sim /home/chen/MyWorkArea/Data/raw/g4sim/RPC_wire/macros/5_RPC.mac > /home/chen/MyWorkArea/Data/raw/g4sim/RPC_wire/5_simulateLog 2> /home/chen/MyWorkArea/Data/raw/g4sim/RPC_wire/5_simulateError &
nohup /home/chen/MyWorkArea/g4sim/g4sim /home/chen/MyWorkArea/Data/raw/g4sim/RPC_wire/macros/6_RPC.mac > /home/chen/MyWorkArea/Data/raw/g4sim/RPC_wire/6_simulateLog 2> /home/chen/MyWorkArea/Data/raw/g4sim/RPC_wire/6_simulateError &
nohup /home/chen/MyWorkArea/g4sim/g4sim /home/chen/MyWorkArea/Data/raw/g4sim/RPC_wire/macros/7_RPC.mac > /home/chen/MyWorkArea/Data/raw/g4sim/RPC_wire/7_simulateLog 2> /home/chen/MyWorkArea/Data/raw/g4sim/RPC_wire/7_simulateError &
