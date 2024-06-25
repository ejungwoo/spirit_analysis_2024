
submit_path = "/data/RB230064/ejungwoo/analysis202406/submit"
script = f"/data/RB230064/ejungwoo/analysis202406/submit_reco_2024_lowgain.sh"
runNo = 1046
num_events_in_split = 10

with open( "./submit_all.sh",'w') as file_submit_all:
    for split in range(2):
        job_output = f"{submit_path}/job_output_{split}.out"
        options = []
        options.append("--exclude=bwmpc053,bwmpc094")
        options.append("--partition=mpc")
        options.append("--account=RB230064")
        options.append("--cpus-per-task=4")
        options.append("--mem-per-cpu=4G")
        options.append(f"--output {job_output}")
        option = ' '.join(options)
        command = f"{script} {runNo} {split} {num_events_in_split}"
        print(f"sbatch {option} {command}")
        print(f"sbatch {option} {command}", file=file_submit_all)
