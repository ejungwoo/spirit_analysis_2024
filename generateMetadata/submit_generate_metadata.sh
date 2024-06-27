#!/bin/bash
hostname
run_number=$1
source /data/RB230064/ejungwoo/spiritroot_2024/build_submit/config.sh
cd /data/RB230064/ejungwoo/analysis202406/generateMetadata/
srun root -l -q -b generateMetadata.C\($run_number\)
