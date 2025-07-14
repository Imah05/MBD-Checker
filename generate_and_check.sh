arr=(
"genbg -d8:0 -D8:3 3 16 24:24"
"genbg -d7:0 -D8:3 3 17 23:23"
"genbg -d7:0 -D9:3 3 17 25:25"
"genbg -d9:0 -D9:3 3 17 27:27"
"genbg -d6:0 -D6:3 4 16 24:24"
"genbg -d4:0 -D8:3 4 16 26:26"
"genbg -d4:0 -D8:3 4 16 28:28"
"genbg -d6:0 -D8:3 4 16 30:30"
"genbg -d8:0 -D8:3 4 16 32:32"
"genbg -d4:0 -D6:3 5 15 27:27"
"genbg -d4:0 -D7:3 5 15 29:29"
"genbg -d6:0 -D9:3 3 18 24:24"
"genbg -d6:0 -D10:3 3 18 26:26"
"genbg -d8:0 -D10:3 3 18 28:28"
"genbg -d10:0 -D10:3 3 18 30:30"
"genbg -d4:0 -D7:3 4 17 25:25"
"genbg -d4:0 -D9:3 4 17 27:27"
"genbg -d4:0 -D9:3 4 17 29:29"
"genbg -d4:0 -D9:3 4 17 31:31"
"genbg -d6:0 -D9:3 4 17 33:33"
"genbg -d8:0 -D9:3 4 17 35:35"
"genbg -d4:0 -D7:3 5 16 28:28"
"genbg -d4:0 -D8:3 5 16 30:30"
"genbg -d4:0 -D8:3 5 16 32:32"
"genbg -d4:0 -D8:3 5 16 34:34"
"genbg -d4:0 -D8:3 5 16 36:36"
"genbg -d6:0 -D8:3 5 16 38:38"
"genbg -d8:0 -D8:3 5 16 40:40"
"genbg -d4:0 -D5:3 6 15 29:29"
"genbg -d4:0 -D7:3 6 15 31:31"
"genbg -d4:0 -D7:3 6 15 33:33"
"genbg -d4:0 -D7:3 6 15 35:35"
"genbg -d4:0 -D7:3 6 15 37:37"
"genbg -d4:0 -D5:3 7 14 32:32"
"genbg -d4:0 -D7:3 7 14 34:34"
"genbg -d4:0 -D7:3 7 14 36:36"
"genbg -d4:0 -D5:3 8 13 35:35" 
)



# Number of parallel jobs (leave 1 core free)
k=$(nproc)
[ "$k" -gt 1 ] && k=$((k - 1))

n=100

mkdir -p logs tmp

run_job() {
    cmd_name=$(echo "$1" | tr ' /' '__')  # sanitize filename
    tmpfile="tmp/tmp_${cmd_name}_$(echo "$2" | tr '/' '_').log"

    {
        eval "$1 $2" | ./check_cores
    } &> "$tmpfile"
}

for cmd in "${arr[@]}"; do
    echo "=== Running: $cmd"
    cmd_name=$(echo "$cmd" | tr ' /' '__')


    for frac in $(seq 0 $((n-1)) | awk '{print $1"/$n"}'); do
        run_job "$cmd" "$frac" &
        
        # Limit concurrency
        while [ "$(jobs -rp | wc -l)" -ge "$k" ]; do
            sleep 0.1
        done
    done

    # Wait for all remaining background jobs to finish
    while [ "$(jobs -rp | wc -l)" -gt 0 ]; do
        sleep 0.2
    done

    # Merge tmp files after all frac jobs for this cmd
    logfile="logs/log_${cmd_name}.txt"
    cat tmp/tmp_${cmd_name}_*.log > "$logfile"
    rm tmp/tmp_${cmd_name}_*.log

    echo "=== Done: $cmd -> $logfile"
done