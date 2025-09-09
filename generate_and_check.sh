#!/bin/bash 

k=$(nproc)
[ "$k" -gt 1 ] && k=$((k - 1))
log_dir="logs"
tmp_dir="tmp"

mkdir -p "$log_dir" "$tmp_dir"

# starting small graph jobs ####################################################
g++ game_state.cpp graph.cpp check_graphs.cpp -o check_graphs

run_graphs_job() {
    cmd_name=$(echo "$1" | tr ' /' '__')
    mkdir -p "$tmp_dir/$3"
    tmpfile="$tmp_dir/$3/${cmd_name}_$(echo "$2").txt"

    {
        eval "$1" | ./check_graphs "$2"
    } &> "$tmpfile"
}

echo "=== Running checks for beta(1)"

run_graphs_job "geng -d1 4" "D" "beta(1)" &
run_graphs_job "geng -d1 5" "D" "beta(1)" &


echo "=== Running checks for beta'(2)"

run_graphs_job "geng -d2 4" "S" "beta'(2)" &
run_graphs_job "geng -d2 5" "S" "beta'(2)" &
run_graphs_job "geng -d2 6" "S" "beta'(2)" &

echo "=== Running checks for beta(2)"

run_graphs_job "geng -d2 8" "D" "beta(2)" &
run_graphs_job "geng -d2 9" "D" "beta(2)" &
run_graphs_job "geng -d2 10" "D" "beta(2)" &
run_graphs_job "geng -d2 11" "D" "beta(2)" &


echo "=== Running checks for beta'(3)"

run_graphs_job "geng -d3 8" "S" "beta'(3)" &
run_graphs_job "geng -d3 9" "S" "beta'(3)" &
run_graphs_job "geng -d3 10" "S" "beta'(3)" &
run_graphs_job "geng -d3 11" "S" "beta'(3)" &
run_graphs_job "geng -d3 12" "S" "beta'(3)" &

while [ "$(jobs -rp | wc -l)" -ge "$k" ]; do
    sleep 0.2
done

# starting R jobs ##############################################################
g++ game_state.cpp graph.cpp check_R.cpp -o check_R

run_R_job() {
    cmd_name=$(echo "$1" | tr ' /' '__')
    mkdir -p "$tmp_dir/$4"
    tmpfile="$tmp_dir/$4/${cmd_name}_$(echo "$2")_$(echo "$3").txt"

    {
        eval "$1" | ./check_R "$2" "$3" 
    } &> "$tmpfile"
}


echo "=== Running R-Lemma 2"

run_R_job "geng -d1 5" "S" "5" "R-Lemma_2" &
run_R_job "geng -d1 6" "S" "5" "R-Lemma_2" &


echo "=== Running R-Lemma 3"

run_R_job "geng -d1 4" "S" "2" "R-Lemma_3" &
run_R_job "geng -d1 5" "S" "2" "R-Lemma_3" &
run_R_job "geng -d1 6" "S" "2" "R-Lemma_3" &
run_R_job "geng -d1 7" "S" "2" "R-Lemma_3" &


echo "=== Running R-Lemma 4"

run_R_job "geng -d1D3 9 11:11" "D" "5" "R-Lemma_4" &
run_R_job "geng -d1D6 10 13:14" "D" "5" "R-Lemma_4" &
run_R_job "geng -d1D8 11 14:17" "D" "5" "R-Lemma_4" &
run_R_job "geng -d1D9 12 16:19" "D" "5" "R-Lemma_4" &


echo "=== Running R-Lemma 6"

run_R_job "geng -d2D9 13 17:20" "D" "5" "R-Lemma_6" &
run_R_job "geng -d2D9 14 19:22" "D" "5" "R-Lemma_6" &



while [ "$(jobs -rp | wc -l)" -ge "$k" ]; do
    sleep 0.2
done


# starting SI jobs #############################################################
g++ game_state.cpp graph.cpp check_SI.cpp -o check_SI

run_SI_job() {
    cmd_name=$(echo "$1" | tr ' /' '__')
    mkdir -p "$tmp_dir/$4"
    tmpfile="$tmp_dir/$4/${cmd_name}_$(echo "$2")_$(echo "$3").txt"

    {
        eval "$1" | ./check_SI "$2" "$3" 
    } &> "$tmpfile"
}

echo "=== Running SI-Lemma 1"

run_SI_job "genbg -d2:3 -D7:3 6 7" "S" "5" "SI-Lemma_1" &
run_SI_job "genbg -d2:3 -D6:3 5 6" "S" "5" "SI-Lemma_1" &
run_SI_job "genbg -d2:3 -D5:3 4 5" "S" "5" "SI-Lemma_1" &
run_SI_job "genbg -d2:3 -D4:3 3 4" "S" "5" "SI-Lemma_1" &


echo "=== Running SI-Lemma 2"

run_SI_job "genbg -d2:2 -D9:3 7 9 26:26" "D" "4" "SI-Lemma_2" &
run_SI_job "genbg -d2:2 -D8:3 6 8 23:23" "D" "4" "SI-Lemma_2" &
run_SI_job "genbg -d2:2 -D7:3 5 7 20:20" "D" "4" "SI-Lemma_2" &
run_SI_job "genbg -d2:2 -D6:3 4 6 17:17" "D" "4" "SI-Lemma_2" &
run_SI_job "genbg -d2:2 -D5:3 3 5 14:14" "D" "4" "SI-Lemma_2" &


echo "=== Running SI-Lemma 3"

run_SI_job "genbg -d2:3 -D8:3 7 8" "D" "5" "SI-Lemma_3" &


echo "=== Running SI-Lemma 4"

run_SI_job "genbg -d2:3 -D8:3 8 9" "D" "5" "SI-Lemma_4" &


echo "=== Running SI-Lemma 5"

run_SI_job "genbg -d2:3 -D4:3 9 10" "D" "5" "SI-Lemma_5" &


echo "=== Running SI-Lemma 6"

run_SI_job "genbg -d2:3 -D7:3 5 7" "S" "2" "SI-Lemma_6" &
run_SI_job "genbg -d2:3 -D6:3 4 6" "D" "2" "SI-Lemma_6" &
run_SI_job "genbg -d2:3 -D5:3 3 5" "D" "2" "SI-Lemma_6" &


echo "=== Running SI-Lemma 7"

run_SI_job "genbg -d2:3 -D9:3 7 9" "D" "2" "SI-Lemma_7" &
run_SI_job "genbg -d2:3 -D8:3 6 8" "D" "2" "SI-Lemma_7" &

while [ "$(jobs -rp | wc -l)" -ge "$k" ]; do
    sleep 0.2
done

# generating degree sequences ##################################################

g++ sequence_generator.cpp -o sequence_generator

echo "=== Generating degree sequences" 
./sequence_generator &

# starting cores jobs ##########################################################
g++ pccgs.cpp graph.cpp check_cores.cpp -o check_cores

cores_cmd_arr=(
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
"genbg -d4:0 -D6:3 8 13 37:37"
)

n=150

# Takes a nauty command from cores_cmd_arr and a block number n.
run_cores_job() {
    cmd_name=$(echo "$1" | tr ' /' '__')
    mkdir -p "$tmp_dir/$1"
    tmpfile="$tmp_dir/$1/${cmd_name}_$2_$n.txt"

    {
        eval "$1 $2/$n" | ./check_cores
    } &> "$tmpfile"
}

for cmd in "${cores_cmd_arr[@]}"; do
    echo "=== Running: $cmd"
    cmd_name=$(echo "$cmd" | tr ' /' '__')
    for i in $(seq 0 $((n-1))); do
        run_cores_job "$cmd" "$i" &

        while [ "$(jobs -rp | wc -l)" -ge "$k" ]; do
            sleep 0.2
        done
    done
done

while [ "$(jobs -rp | wc -l)" -gt 0 ]; do
    sleep 0.2
done


# Collecting stats and tmp files in log ###################################

for dir in "$tmp_dir"/*/; do
    [ -d "$dir" ] || continue
    log_name=$(basename "${dir%/}")
    log_file="$log_dir/$log_name.log"
    echo "=== Log file for $log_name" >> "$log_file"
    echo -e "\n\n" >> "$log_file"
    for tmp_file in "$dir"*; do
        [ -f "$tmp_file" ] || continue
        cat "$tmp_file" >> "$log_file"
    done

    echo -e "\n\n" >> "$log_file"

    graph_nauty=$(awk '/^>Z/ {sum += $2} END {print sum}' "$log_file")
    graph_compl=$(awk '/^>F/ {sum += $2} END {print sum}' "$log_file")
    time_nauty=$(awk '/^>Z/ {print $(NF-1)}' "$log_file" | paste -sd+ - | bc)
    time_nauty=$(printf "%.2f" "$time_nauty")
    time_compl=$(awk '/^>F/ {print $(NF-1)}' "$log_file" | paste -sd+ - | bc)
    time_compl=$(printf "%.2f" "$time_compl")
    exc_graphs=$(grep -c '^Found graph' "$log_file")

    echo "=== Total number of graphs generated: $graph_nauty" >> "$log_file"
    echo "=== Total number of graphs checked: $graph_compl" >> "$log_file"
    echo "=== Total time for generation: $time_nauty seconds" >> "$log_file"
    echo "=== Total time for checking: $time_compl seconds" >> "$log_file"
    echo "=== Total number of exceptional graphs: $exc_graphs" >> "$log_file"
done

rm -rf "$tmp_dir"
echo "=== All checks completed. Logs are saved in the $log_dir directory."