#!/bin/bash

hostname

run_number=$1
split_no=$2
num_events_in_split=$3

#source /data/RB230064/common/spiritroot/Submit/SpiRITROOT/build/config.sh
source /home/ejungwoo/spiritroot_2024/build_submit/config.sh
analysis_directory="/data/RB230064/ejungwoo/analysis202406/"
cd $analysis_directory;

touch $analysis_directory/log/start_of_"$run_number"_"$split_no"_"$num_events_in_split"
root -l -q -b run_reco_2024_lowgain.C\($run_number,$split_no,$num_events_in_split\);
touch $analysis_directory/log/end_of_"$run_number"_"$split_no"_"$num_events_in_split"
