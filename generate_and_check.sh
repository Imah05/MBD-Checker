#!/bin/bash 
arr=(
"nauty-genbg -d8:0 -D8:3 3 16 24:24"
"nauty-genbg -d7:0 -D8:3 3 17 23:23"
"nauty-genbg -d7:0 -D9:3 3 17 25:25"
"nauty-genbg -d9:0 -D9:3 3 17 27:27"
"nauty-genbg -d6:0 -D6:3 4 16 24:24"
"nauty-genbg -d4:0 -D8:3 4 16 26:26"
"nauty-genbg -d4:0 -D8:3 4 16 28:28"
"nauty-genbg -d6:0 -D8:3 4 16 30:30"
"nauty-genbg -d8:0 -D8:3 4 16 32:32"
"nauty-genbg -d4:0 -D6:3 5 15 27:27"
"nauty-genbg -d4:0 -D7:3 5 15 29:29"
"nauty-genbg -d6:0 -D9:3 3 18 24:24"
"nauty-genbg -d6:0 -D10:3 3 18 26:26"
"nauty-genbg -d8:0 -D10:3 3 18 28:28"
"nauty-genbg -d10:0 -D10:3 3 18 30:30"
"nauty-genbg -d4:0 -D7:3 4 17 25:25"
"nauty-genbg -d4:0 -D9:3 4 17 27:27"
"nauty-genbg -d4:0 -D9:3 4 17 29:29"
"nauty-genbg -d4:0 -D9:3 4 17 31:31"
"nauty-genbg -d6:0 -D9:3 4 17 33:33"
"nauty-genbg -d8:0 -D9:3 4 17 35:35"
"nauty-genbg -d4:0 -D7:3 5 16 28:28"
"nauty-genbg -d4:0 -D8:3 5 16 30:30"
"nauty-genbg -d4:0 -D8:3 5 16 32:32"
"nauty-genbg -d4:0 -D8:3 5 16 34:34"
"nauty-genbg -d4:0 -D8:3 5 16 36:36"
"nauty-genbg -d6:0 -D8:3 5 16 38:38"
"nauty-genbg -d8:0 -D8:3 5 16 40:40"
"nauty-genbg -d4:0 -D5:3 6 15 29:29"
"nauty-genbg -d4:0 -D7:3 6 15 31:31"
"nauty-genbg -d4:0 -D7:3 6 15 33:33"
"nauty-genbg -d4:0 -D7:3 6 15 35:35"
"nauty-genbg -d4:0 -D7:3 6 15 37:37"
"nauty-genbg -d4:0 -D5:3 7 14 32:32"
"nauty-genbg -d4:0 -D7:3 7 14 34:34"
"nauty-genbg -d4:0 -D7:3 7 14 36:36"
"nauty-genbg -d4:0 -D5:3 8 13 35:35" 
)


k=$(nproc)
[ "$k" -gt 1 ] && k=$((k - 1))

n=1000

mkdir -p logs tmp

run_job() {
    cmd_name=$(echo "$1" | tr ' /' '__')
    tmpfile="tmp/tmp_${cmd_name}_$(echo "$2" | tr '/' '_').log"

    {
        eval "$1 $2/$n" | ./check_cores
    } &> "$tmpfile"
}


for cmd in "${arr[@]}"; do
    echo "=== Running: $cmd"
    cmd_name=$(echo "$cmd" | tr ' /' '__')
    for i in $(seq 0 $((n-1))); do
        run_job "$cmd" "$i" &

        while [ "$(jobs -rp | wc -l)" -ge "$k" ]; do
            sleep 0.2
        done
    done
done

while [ "$(jobs -rp | wc -l)" -gt 0 ]; do
    sleep 0.2
done

for cmd in "${arr[@]}"; do
    logfile="logs/log_${cmd_name}.txt"
    
    total_graphs_nauty=0
    total_graphs_compl=0
    total_time=0

    for j in $(seq 0 $((n-1))); do
        log_file="tmp/tmp_${cmd_name}_$j.log"

        graphs_nauty=$(grep '^>Z' "$log_file" | awk '{print $2}')
        graphs_compl=$(grep '^>F' "$log_file" | awk '{print $2}')
        time_nauty=$(grep '^>Z' "$log_file" | awk '{print $6}')
        time_compl=$(grep '^>F' "$log_file" | awk '{print $6}')

        total_graphs_nauty=$((total_graphs_nauty + graphs_nauty))
        total_graphs_compl=$((total_graphs_compl + graphs_compl))
        total_time=$(echo "$total_time + $time_nauty + $time_compl" | bc)

        cat "$log_file" >> "$logfile"
        rm "$log_file"
    done

    echo -e "\n\n" >> "$logfile"

    echo "===Total number of cores generated: $total_graphs_nauty" >> "$logfile"
    echo "===Total number of cores checked: $total_graphs_compl" >> "$logfile"
    echo "===Total time: $total_time seconds" >> "$logfile"
done