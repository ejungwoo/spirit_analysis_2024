import sys
import os

args = sys.argv
if len(args)<2:
    print("need run_number argument! ex) python make_submit.py 1047")
    exit()

##############################################################################################
run_mode = 2
run_number = int(sys.argv[1])
split_range = 30
num_events_in_split = 5000
cut_ntrack_for_LG_reco = 20
tag = f"recoLG"
name_root_macro = "run_reco_2024_lowgain.C"

use_metadata = 0
if len(sys.argv)>2:
    use_metadata = int(sys.argv[2])
if use_metadata:
    tag = tag + "_MD"

request_memory = "4G"
request_cpus = 4

name_submit = f"{tag}_run{run_number}"
path_analysis = f"/data/RB230064/ejungwoo/analysis202406";
path_metadata = f"/data/RB230064/ejungwoo/analysis202406/metadata";
source_spirit = "/data/RB230064/ejungwoo/spiritroot_2024/build_submit/config.sh"
##############################################################################################

path_submit = f"{path_analysis}/submit/{name_submit}";
path_data = f"/data/RB230064/ejungwoo/production/{name_submit}";
path_endof = f"{path_submit}";
path_log = f"{path_data}/log";
os.makedirs(path_submit, exist_ok=True)
os.makedirs(path_data, exist_ok=True)
os.makedirs(path_log, exist_ok=True)

name_submit_file = f"{path_submit}/submit_{name_submit}.sh"
name_toss_file = f"toss_submit_{name_submit}.sh"
name_watch_file = f"watch_submit_{name_submit}.sh"

submit_reco_content = f"""#!/bin/bash

split=$1
name_split=$2
name_output=$3
name_logger=$4

source {source_spirit}
cd {path_analysis}

rm -f {path_endof}/start_of_$name_split.stamp
rm -f {path_endof}/middle_of_$name_split.stamp
rm -f {path_endof}/end_of_$name_split.stamp
rm -f {path_endof}/list_of_$name_split.txt

hostname > {path_endof}/start_of_$name_split.stamp
echo $name_output > {path_endof}/middle_of_$name_split.stamp
####
root -l -q -b {name_root_macro}\\({run_mode},{run_number},$split,{num_events_in_split},\\"{tag}\\",{cut_ntrack_for_LG_reco}\\,\\"{path_data}\\",\\"$name_output\\",\\"$name_logger\\",{use_metadata},\\"{path_metadata}\\"\\);
####
echo $name_output > {path_endof}/end_of_$name_split.stamp
echo $name_output >> {path_endof}/list_of_{name_submit}.txt
rm -f {path_endof}/middle_of_$name_split.stamp"""

with open(name_submit_file,'w') as file_submit_single:
    print(submit_reco_content, file=file_submit_single)
    os.chmod(name_submit_file, 0o755)

with open(name_toss_file,'w') as file_submit_all:
    for split in range(split_range):
        name_job = f"S{run_number}_{split}"
        name_job_output = f"{path_submit}/job_output_{run_number}_{split}_{tag}.out"
        name_logger = f"{path_log}/spirit24_{run_number:04}.{split:02}.{tag}.log"
        name_output = f"{path_data}/spirit24_{run_number:04}.{split:02}.{tag}.root"
        name_split = f"{name_submit}_{split}"

        list_option = []
        #list_option.append(f"--output dev/null")

        list_option.append(f"--job-name={name_job}")
        list_option.append( "--exclude=bwmpc053,bwmpc094")
        list_option.append( "--partition=mpc")
        list_option.append( "--account=RB230064")
        list_option.append(f"--cpus-per-task={request_cpus}")
        list_option.append(f"--mem-per-cpu={request_memory}")
        list_option.append(f"--output {name_job_output}")
        options = ' \\\n'.join(list_option)

        list_argument = []
        ##############################################
        list_argument.append(f"{split}")
        list_argument.append(f"{name_split}")
        list_argument.append(f"{name_output}")
        list_argument.append(f"{name_logger}")
        ##############################################
        arguments = ' \\\n'.join(list_argument)

        #print(f"sbatch {options} \\\n{name_submit_file} \\\n{split} {name_split} \\\n{name_output} \\\n{name_logger}", file=file_submit_all)
        print(f"### {split}", file=file_submit_all)
        print(f"sbatch {options} \\\n{name_submit_file} \\\n{arguments}", file=file_submit_all)
        print("", file=file_submit_all)
    os.chmod(name_toss_file, 0o755)

with open(name_watch_file,'w') as file_watch:
    name_job_output = f"{path_submit}/job_output_{run_number}_'$split'_{tag}.out"
    #print(f"watch -n 2 'tail -n 20 {name_job_output}; ls {path_endof}/middle_of* {path_endof}/end_of* {path_endof}/list_*;'",file=file_watch)
    file_watch.write(f"split=$1\n")
    file_watch.write(f"watch -n 2 \\\n")
    file_watch.write(f'"tail -n 20 {name_job_output}; \\\n')
    file_watch.write(f"ls {path_endof}/middle_of*.stamp; \\\n")
    file_watch.write(f"ls {path_endof}/end_of*.stamp; \\\n")
    file_watch.write(f"ls {path_endof}/list_of*.txt; \\\n")
    file_watch.write(f'ls {path_data}/*;"')
    os.chmod(name_watch_file, 0o755)

print(name_submit_file)
print(name_toss_file)
print(name_watch_file)
