#!/bin/bash 

k=$(nproc)
[ "$k" -gt 1 ] && k=$((k - 1))

mkdir -p logs logs/graphs logs/R logs/SI logs/cores 
mkdir -p tmp tmp/graphs tmp/R tmp/SI tmp/cores

# starting small graph jobs ####################################################
g++ game_state.cpp graph.cpp check_graphs.cpp -o check_graphs

run_graphs_job() {
    cmd_name=$(echo "$1" | tr ' /' '__')
    tmpfile="tmp/graphs/${cmd_name}_$(echo "$2").txt"

    {
        eval "$1" | ./check_graphs "$2"
    } &> "$tmpfile"
}


echo "=== Running checks for beta(2)"

run_graphs_job "nauty-geng -d2D6 11" "D" &
run_graphs_job "nauty-geng -d2D5 10" "D" &


echo "=== Running checks for beta'(3)"

run_graphs_job "nauty-geng -d3D5 12" "S" &
run_graphs_job "nauty-geng -d3D5 11" "S" &
run_graphs_job "nauty-geng -d3D4 10" "S" &
run_graphs_job "nauty-geng -d3D4 9" "S" &

while [ "$(jobs -rp | wc -l)" -ge "$k" ]; do
    sleep 0.2
done

# starting R jobs ##############################################################
g++ game_state.cpp graph.cpp check_R.cpp -o check_R

# Takes a nauty command, a first player and a surBound
run_R_job() {
    cmd_name=$(echo "$1" | tr ' /' '__')
    tmpfile="tmp/R/${cmd_name}_$(echo "$2")_$(echo "$3").txt"

    {
        eval "$1" | ./check_R "$2" "$3" 
    } &> "$tmpfile"
}


echo "=== Running R-Lemma 2"

run_R_job "nauty-geng -d1 5" "S" "5" &
run_R_job "nauty-geng -d1 6" "S" "5" & 


echo "=== Running R-Lemma 3"

run_R_job "nauty-geng -d1 4" "S" "2" &
run_R_job "nauty-geng -d1 5" "S" "2" &
run_R_job "nauty-geng -d1 6" "S" "2" &
run_R_job "nauty-geng -d1 7" "S" "2" &


echo "=== Running R-Lemma 4"

run_R_job "nauty-geng -d1D3 9 11:11" "D" "5" &
run_R_job "nauty-geng -d1D6 10 13:14" "D" "5" &
run_R_job "nauty-geng -d1D8 11 14:17" "D" "5" &
run_R_job "nauty-geng -d1D9 12 16:19" "D" "5" & 


echo "=== Running R-Lemma 6"

run_R_job "nauty-geng -d2D9 13 17:20" "D" "5" &
run_R_job "nauty-geng -d2D9 14 19:22" "D" "5" &



while [ "$(jobs -rp | wc -l)" -ge "$k" ]; do
    sleep 0.2
done


# starting SI jobs #############################################################
g++ game_state.cpp graph.cpp check_SI.cpp -o check_SI

run_SI_job() {
    cmd_name=$(echo "$1" | tr ' /' '__')
    tmpfile="tmp/SI/${cmd_name}_$(echo "$2")_$(echo "$3").txt"

    {
        eval "$1" | ./check_SI "$2" "$3" 
    } &> "$tmpfile"
}

echo "=== Running SI-Lemma 1"

run_SI_job "nauty-genbg -d2:3 -D7:3 6 7" "S" "5" &
run_SI_job "nauty-genbg -d2:3 -D6:3 5 6" "S" "5" &
run_SI_job "nauty-genbg -d2:3 -D5:3 4 5" "S" "5" &
run_SI_job "nauty-genbg -d2:3 -D4:3 3 4" "S" "5" &


echo "=== Running SI-Lemma 2"

run_SI_job "nauty-genbg -d2:2 -D9:3 7 9 26:26" "D" "4" &
run_SI_job "nauty-genbg -d2:2 -D8:3 6 8 23:23" "D" "4" &
run_SI_job "nauty-genbg -d2:2 -D7:3 5 7 20:20" "D" "4" &
run_SI_job "nauty-genbg -d2:2 -D6:3 4 6 17:17" "D" "4" &
run_SI_job "nauty-genbg -d2:2 -D5:3 3 5 14:14" "D" "4" &


echo "=== Running SI-Lemma 3"

run_SI_job "nauty-genbg -d2:3 -D8:3 7 8" "D" "5" &


echo "=== Running SI-Lemma 4"

run_SI_job "nauty-genbg -d2:3 -D8:3 8 9" "D" "5" &


echo "=== Running SI-Lemma 5"

run_SI_job "nauty-genbg -d2:3 -D4:3 9 10" "D" "5" &


echo "=== Running SI-Lemma 6"

run_SI_job "nauty-genbg -d2:3 -D7:3 5 7" "S" "2" &
run_SI_job "nauty-genbg -d2:3 -D6:3 4 6" "D" "2" &
run_SI_job "nauty-genbg -d2:3 -D5:3 3 5" "D" "2" &


echo "=== Running SI-Lemma 7"

run_SI_job "nauty-genbg -d2:3 -D9:3 7 9" "D" "2" &
run_SI_job "nauty-genbg -d2:3 -D8:3 6 8" "D" "2" &

while [ "$(jobs -rp | wc -l)" -ge "$k" ]; do
    sleep 0.2
done

# starting cores jobs ##########################################################
g++ part_compl_core_game_state.cpp graph.cpp check_cores.cpp -o check_cores

cores_cmd_arr=(
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
"nauty-genbg -d4:0 -D6:3 8 13 37:37"
)

n=100

# Takes a nauty command from cores_cmd_arr and a block number n.
run_cores_job() {
    cmd_name=$(echo "$1" | tr ' /' '__')
    tmpfile="tmp/cores/${cmd_name}_$2_$n.txt"

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


# Functions for collecting stats in log file ###################################

collect_stats() {
    logfile="$1"
    graph_kind="$2"

    echo -e "\n\n" >> "$logfile"

    graph_nauty=$(awk '/^>Z/ {sum += $2} END {print sum}' "$logfile")
    graph_compl=$(awk '/^>F/ {sum += $2} END {print sum}' "$logfile")
    time_nauty=$(awk '/^>Z/ {print $(NF-1)}' "$logfile" | paste -sd+ - | bc)
    time_nauty=$(printf "%.2f" "$time_nauty")
    time_compl=$(awk '/^>F/ {print $(NF-1)}' "$logfile" | paste -sd+ - | bc)
    time_compl=$(printf "%.2f" "$time_compl")
    exc_graphs=$(grep -c '^Found graph' "$logfile")

    echo "===Total number of $graph_kind generated: $graph_nauty" >> "$logfile"
    echo "===Total number of $graph_kind checked: $graph_compl" >> "$logfile"
    echo "===Total time for generation: $time_nauty seconds" >> "$logfile"
    echo "===Total time for checking: $time_compl seconds" >> "$logfile"
    echo "===Total number of exceptional $graph_kind: $exc_graphs" >> "$logfile"
}

# Collecting graphs tmp files into log files ###################################

logfile="logs/graphs/beta(2).log"
echo "=== Log file for beta(2)" >> "$logfile"
echo -e "\n\n" >> "$logfile"
for cmd in "nauty-geng_-d2D6_11_D" "nauty-geng_-d2D5_10_D"; do
    tmp_file="tmp/graphs/${cmd}.txt"
    cat "$tmp_file" >> "$logfile"
done
collect_stats "$logfile" "graphs"


logfile="logs/graphs/beta'(3).log"
echo "=== Log file for beta'(3)" >> "$logfile"
echo -e "\n\n" >> "$logfile"
for cmd in "nauty-geng_-d3D5_12_S" "nauty-geng_-d3D5_11_S" \
            "nauty-geng_-d3D4_10_S" "nauty-geng_-d3D4_9_S"; do
    tmp_file="tmp/graphs/${cmd}.txt"
    cat "$tmp_file" >> "$logfile"
done
collect_stats "$logfile" "graphs"


# Collecting R tmp files into log files ########################################

logfile="logs/R/Lemma_2.log"
echo "=== Log file for R-Lemma 2" >> "$logfile"
echo -e "\n\n" >> "$logfile"
for cmd in "nauty-geng_-d1_5_S_5" "nauty-geng_-d1_6_S_5"; do
    tmp_file="tmp/R/${cmd}.txt"
    cat "$tmp_file" >> "$logfile"
done
collect_stats "$logfile" "R-graphs"


logfile="logs/R/Lemma_3.log"
echo "=== Log file for R-Lemma 3" >> "$logfile"
echo -e "\n\n" >> "$logfile"
for cmd in "nauty-geng_-d1_4_S_2" "nauty-geng_-d1_5_S_2" \
            "nauty-geng_-d1_6_S_2" "nauty-geng_-d1_7_S_2"; do
    tmp_file="tmp/R/${cmd}.txt"
    cat "$tmp_file" >> "$logfile"
done
collect_stats "$logfile" "R-graphs"


logfile="logs/R/Lemma_4.log"
echo "=== Log file for R-Lemma 4" >> "$logfile"
echo -e "\n\n" >> "$logfile"
for cmd in "nauty-geng_-d1D3_9_11:11_D_5" "nauty-geng_-d1D6_10_13:14_D_5" \
            "nauty-geng_-d1D8_11_14:17_D_5" "nauty-geng_-d1D9_12_16:19_D_5"; do
    tmp_file="tmp/R/${cmd}.txt"
    cat "$tmp_file" >> "$logfile"
done
collect_stats "$logfile" "R-graphs"


logfile="logs/R/Lemma_6.log"
echo "=== Log file for R-Lemma 6" >> "$logfile"
echo -e "\n\n" >> "$logfile"
for cmd in "nauty-geng_-d2D9_13_17:20_D_5" "nauty-geng_-d2D9_14_19:22_D_5"; do
    tmp_file="tmp/R/${cmd}.txt"
    cat "$tmp_file" >> "$logfile"
done
collect_stats "$logfile" "R-graphs"


# Collecting SI tmp files into log files #######################################

logfile="logs/SI/Lemma_1.log"
echo "=== Log file for SI-Lemma 1" >> "$logfile"
echo -e "\n\n" >> "$logfile"
for cmd in "nauty-genbg_-d2:3_-D7:3_6_7_S_5" "nauty-genbg_-d2:3_-D6:3_5_6_S_5" \
        "nauty-genbg_-d2:3_-D5:3_4_5_S_5" "nauty-genbg_-d2:3_-D4:3_3_4_S_5"; do
    tmp_file="tmp/SI/${cmd}.txt"
    cat "$tmp_file" >> "$logfile"
done
collect_stats "$logfile" "SI-graphs"


logfile="logs/SI/Lemma_2.log"
echo "=== Log file for SI-Lemma 2" >> "$logfile"
echo -e "\n\n" >> "$logfile"
for cmd in "nauty-genbg_-d2:2_-D9:3_7_9_26:26_D_4" \
            "nauty-genbg_-d2:2_-D8:3_6_8_23:23_D_4" \
            "nauty-genbg_-d2:2_-D7:3_5_7_20:20_D_4" \
            "nauty-genbg_-d2:2_-D6:3_4_6_17:17_D_4" \
            "nauty-genbg_-d2:2_-D5:3_3_5_14:14_D_4"; do
    tmp_file="tmp/SI/${cmd}.txt"
    cat "$tmp_file" >> "$logfile"
done
collect_stats "$logfile" "SI-graphs"


logfile="logs/SI/Lemma_3.log"
echo "=== Log file for SI-Lemma 3" >> "$logfile"
echo -e "\n\n" >> "$logfile"
for cmd in "nauty-genbg_-d2:3_-D8:3_7_8_D_5"; do
    tmp_file="tmp/SI/${cmd}.txt"
    cat "$tmp_file" >> "$logfile"
done
collect_stats "$logfile" "SI-graphs"


logfile="logs/SI/Lemma_4.log"
echo "=== Log file for SI-Lemma 4" >> "$logfile"
echo -e "\n\n" >> "$logfile"
for cmd in "nauty-genbg_-d2:3_-D8:3_8_9_D_5"; do
    tmp_file="tmp/SI/${cmd}.txt"
    cat "$tmp_file" >> "$logfile"
done
collect_stats "$logfile" "SI-graphs"


logfile="logs/SI/Lemma_5.log"
echo "=== Log file for SI-Lemma 5" >> "$logfile"
echo -e "\n\n" >> "$logfile"
for cmd in "nauty-genbg_-d2:3_-D4:3_9_10_D_5"; do
    tmp_file="tmp/SI/${cmd}.txt"
    cat "$tmp_file" >> "$logfile"
done
collect_stats "$logfile" "SI-graphs"


logfile="logs/SI/Lemma_6.log"
echo "=== Log file for SI-Lemma 6" >> "$logfile"
echo -e "\n\n" >> "$logfile"
for cmd in "nauty-genbg_-d2:3_-D7:3_5_7_S_2" "nauty-genbg_-d2:3_-D6:3_4_6_D_2" \
            "nauty-genbg_-d2:3_-D5:3_3_5_D_2"; do
    tmp_file="tmp/SI/${cmd}.txt"
    cat "$tmp_file" >> "$logfile"
done
collect_stats "$logfile" "SI-graphs"


logfile="logs/SI/Lemma_7.log"
echo "=== Log file for SI-Lemma 7" >> "$logfile"
echo -e "\n\n" >> "$logfile"
for cmd in "nauty-genbg_-d2:3_-D9:3_7_9_D_2" \
            "nauty-genbg_-d2:3_-D8:3_6_8_D_2"; do
    tmp_file="tmp/SI/${cmd}.txt"
    cat "$tmp_file" >> "$logfile"
done
collect_stats "$logfile" "SI-graphs"


# Collecting cores tmp files into log files ####################################

for cmd in "${cores_cmd_arr[@]}"; do
    cmd_name=$(echo "$cmd" | tr ' /' '__')
    logfile="logs/cores/${cmd_name}.log"
    echo "=== Log file for cores: $cmd_name" >> "$logfile"
    echo -e "\n\n" >> "$logfile"
    for j in $(seq 0 $((n-1))); do
        tmp_file="tmp/cores/${cmd_name}_${j}_${n}.txt"
        cat "$tmp_file" >> "$logfile"
    done
    collect_stats "$logfile" "cores"
done

rm -rf tmp
echo "=== All checks completed. Logs are saved in the 'logs' directory."