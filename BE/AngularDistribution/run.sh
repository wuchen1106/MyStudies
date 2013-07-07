#!/bin/bash
nohup ./AngularDistribution 100000000 0.0 > log_00 2>1 &
nohup ./AngularDistribution 100000000 0.1 > log_01 2>1 &
nohup ./AngularDistribution 100000000 0.2 > log_02 2>1 &
nohup ./AngularDistribution 100000000 0.3 > log_03 2>1 &
nohup ./AngularDistribution 100000000 0.4 > log_04 2>1 &
nohup ./AngularDistribution 100000000 0.5 > log_05 2>1 &
