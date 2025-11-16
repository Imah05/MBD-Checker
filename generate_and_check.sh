#!/bin/bash 

# Set k, the number of cores among which the checking is distributed
k=$(nproc)
if (( k > 1 )); then
    ((k--))
fi

# Set n, the number of chunks to split the nauty command, in case it takes a
# long time to check the resulting graphs
n=200


# Waits until fewer than m=$1 background jobs are running
wait_for_jobs() {
    local m="$1"

    while (( $(jobs -rp | wc -l) >= m )); do
        sleep 0.2
    done
}

mkdir "logs" "tmps"

################################################################################
# Small graphs #################################################################
################################################################################
g++ game_state.cpp graph.cpp check_graphs.cpp -o check_graphs


# Runs the nauty command `cmd` to generate graphs and checks whether o_p(G) = D 
# for every generated graph G. Expects a first player `p` and `class` to be 
# defined globally. 
#
# Argument:
#   cmd = $1     - nauty command
#
# Results are stored in tmps/`class` and later collected in logs/`class`.log.
run_graphs_job() {
    local cmd="$1"

    mkdir -p "tmps/$class"

    local cmd_name=$(echo "$cmd" | tr ' /' '__')
    local tmpfile="tmps/$class/${cmd_name}_${p}.txt"

    {
        eval "$cmd" | ./check_graphs "$p"
    } &> "$tmpfile"
}

# beta(1) 
echo "=== Running checks for beta(1)"

class="beta(1)"
p="D"

run_graphs_job "geng -d1 4" &
run_graphs_job "geng -d1 5" &


# beta'(2) 
echo "=== Running checks for beta'(2)"

class="beta'(2)"
p="S"

run_graphs_job "geng -d2 4" &
run_graphs_job "geng -d2 5" &
run_graphs_job "geng -d2 6" &


# beta(2) 
echo "=== Running checks for beta(2)"

class="beta(2)"
p="D"

run_graphs_job "geng -d2 08" &
run_graphs_job "geng -d2 09" &
run_graphs_job "geng -d2 10" &
run_graphs_job "geng -d2 11" & 


# beta'(3)
echo "=== Running checks for beta'(3)"

class="beta'(3)"
p="S"

run_graphs_job "geng -d3 08" &
run_graphs_job "geng -d3 09" &
run_graphs_job "geng -d3 10" &
run_graphs_job "geng -d3 11" &
run_graphs_job "geng -d3 -D7 12 0/3" &
run_graphs_job "geng -d3 -D7 12 1/3" &
run_graphs_job "geng -d3 -D7 12 2/3" &


# Wait until fewer than k background jobs are running
wait_for_jobs "$k"



################################################################################
# R-Lemmas #####################################################################
################################################################################
g++ game_state.cpp graph.cpp check_R.cpp -o check_R

# Runs the nauty command `cmd` to generate graphs and checks whether o_p(G) = D 
# for every generated graph G with surplus at most `sur`. Expects a first player
# `p`, `class` and `sur` to be defined globally. 
#
# Argument:
#   cmd = $1     - nauty command
#
# Results are stored in tmps/`class` and later collected in logs/`class`.log.
run_R_job() {
    local cmd="$1"

    mkdir -p "tmps/$class"

    local cmd_name=$(echo "$cmd" | tr ' /' '__')
    local tmpfile="tmps/$class/${cmd_name}_${p}_${sur}.txt"

    {
        eval "$cmd" | ./check_R "$p" "$sur"
    } &> "$tmpfile"
}


# R-Lemma 2 
echo "=== Running R-Lemma 2"

class="R-Lemma_2"
sur="5"
p="S"

run_R_job "geng -d1 5" &
run_R_job "geng -d1 6" &


# R-Lemma 3
echo "=== Running R-Lemma 3"

class="R-Lemma_3"
sur="2"
p="S"

run_R_job "geng -d1 4" &
run_R_job "geng -d1 5" &
run_R_job "geng -d1 6" &
run_R_job "geng -d1 7" &


# R-Lemma 4
echo "=== Running R-Lemma 4"

class="R-Lemma_4"
sur="5"
p="D"

run_R_job "geng -d1D3 09 11:11" &
run_R_job "geng -d1D6 10 13:14" &
run_R_job "geng -d1D8 11 14:17" &
run_R_job "geng -d1D9 12 16:19" &


# R-Lemma 6
echo "=== Running R-Lemma 6"

class="R-Lemma_6"
sur="5"
p="D"

run_R_job "geng -d2D8 13 17:19" &
run_R_job "geng -d2D9 14 19:22" &




wait_for_jobs "$k"

################################################################################
# SI-Lemmas ####################################################################
################################################################################
g++ game_state.cpp graph.cpp check_SI.cpp -o check_SI


# Runs the nauty command `cmd` to generate graphs and checks whether o_p(G) = D 
# for every generated graph G with at most `sur` vertices of degree 2. Expects a
# first player `p`, `class` and `sur` to be defined globally. 
#
# Argument:
#   cmd = $1   - nauty command
#
# Results are stored in tmps/`class` and later collected in logs/`class`.log.
run_SI_job() {
    local cmd="$1"

    mkdir -p "tmps/$class"

    local cmd_name=$(echo "$cmd" | tr ' /' '__')
    local tmpfile="tmps/$class/${cmd_name}_${p}_${sur}.txt"

    {
        eval "$cmd" | ./check_SI "$p" "$sur"
    } &> "$tmpfile"
}


# SI-Lemma 3
echo "=== Running SI-Lemma 3"

class="SI-Lemma_3"
sur="6"
p="S"

run_SI_job "genbg -d2:3 -D7:3 6 7" &
run_SI_job "genbg -d2:3 -D6:3 5 6" &
run_SI_job "genbg -d2:3 -D5:3 4 5" &
run_SI_job "genbg -d2:3 -D4:3 3 4" &


# SI-Lemma 4
echo "=== Running SI-Lemma 4"

class="SI-Lemma_4"
sur="4"
p="D"

run_SI_job "genbg -d2:2 -D9:3 7 9 26:26" &
run_SI_job "genbg -d2:2 -D8:3 6 8 23:23" &
run_SI_job "genbg -d2:2 -D7:3 5 7 20:20" &
run_SI_job "genbg -d2:2 -D6:3 4 6 17:17" &
run_SI_job "genbg -d2:2 -D5:3 3 5 14:14" &


# SI-Lemma 5
echo "=== Running SI-Lemma 5"

class="SI-Lemma_5"
sur="8"
p="D"

run_SI_job "genbg -d2:3 -D9:3 8 9" &
run_SI_job "genbg -d2:3 -D8:3 7 8" &


# SI-Lemma 6
echo "=== Running SI-Lemma 6"

class="SI-Lemma_6"
sur="5"
p="D"

run_SI_job "genbg -d2:3 -D4:3 9 10" &


# SI-Lemma 7
echo "=== Running SI-Lemma 7"

class="SI-Lemma_7"
sur="2"
p="S"

run_SI_job "genbg -d2:3 -D7:3 5 7" &
run_SI_job "genbg -d2:3 -D6:3 4 6" &
run_SI_job "genbg -d2:3 -D5:3 3 5" &


# SI-Lemma 8
echo "=== Running SI-Lemma 8"

class="SI-Lemma_8"
sur="2"
p="D"

run_SI_job "genbg -d2:3 -D9:3 7 9" &
run_SI_job "genbg -d2:3 -D8:3 6 8" &



wait_for_jobs "$k"

################################################################################
# Cores ########################################################################
################################################################################


# Generating degree sequences
#
# Note:
# - The checking of the cores uses the degree sequences generated here only in 
#   `filter()` for checking whether the completions of the generated cores have 
#   the right degree sequence. 
# - Generation relies on `cores_cmds.txt`, which contains the full set of
#   nauty commands needed to generate all cores corresponding to reduced graphs
#   having degree sequences produced by the degree sequence generator.
# - The conversion from `deg_seqs.txt` to `cores_cmds.txt` was performed 
#   manually.

g++ sequence_generator.cpp -o sequence_generator
echo "=== Generating degree sequences" 
./sequence_generator &


g++ pccgs.cpp graph.cpp check_cores.cpp -o check_cores


# Array containing the nauty commands that generate all cores
cores_cmd_arr=()
while read line; do
    cores_cmd_arr+=("$line")
done < cores_cmds.txt


# Runs the nauty command `cmd ind/n` to generate cores and checks whether 
# Dominator wins on every completion of each generated core.
#
# Arguments:
#   cmd = $1   - nauty command 
#   ind = $2   - index of the current batch
#
# Results are stored in tmps/`cmd_name` and later collected in logs/`cmd_name`.log.
run_cores_job() {
    local cmd="$1"
    local ind="$2"

    local cmd_name=$(echo "$cmd" | tr ' /' '__')
    mkdir -p "tmps/$cmd_name"

    local tmpfile="tmps/$cmd_name/$(printf "%03d" "$ind")_${n}.txt"

    {
        eval "$cmd $ind/$n" | ./check_cores
    } &> "$tmpfile"
}

# Runs over all commands in cores_cmd_arr and, for each command, over all i from
# 0 to n-1. Calls run_cores_job for each pair (cmd, i).
for cmd in "${cores_cmd_arr[@]}"; do
    echo "=== Running: $cmd"

    cmd_name=$(echo "$cmd" | tr ' /' '__')

    for (( i = 0; i < n; i++ )); do
        run_cores_job "$cmd" "$i" &
        wait_for_jobs "$k"
    done
done


# Wait until no background job is running
wait_for_jobs 1


# Collecting tmp files in log ##################################################
for dir in tmps/*/; do
    # Skip if not a directory
    if [ ! -d "$dir" ]; then
        continue
    fi

    class=$(basename "${dir%/}")
    log_file="logs/$class.log"

    echo "=== Log file for $class" >> "$log_file"
    echo -e "\n\n" >> "$log_file"

    # Concatenate all temporary results into the log
    for tmp_file in "$dir"*; do
        if [ ! -f "$tmp_file" ]; then
            continue
        fi
        cat "$tmp_file" >> "$log_file"
    done

    echo -e "\n\n" >> "$log_file"

    # Compute summary statistics
    graph_nauty=$(awk '/^>Z/ {sum += $2} END {print sum}' "$log_file")
    graph_compl=$(awk '/^>F/ {sum += $2} END {print sum}' "$log_file")

    time_nauty=$(awk '/^>Z/ {print $(NF-1)}' "$log_file" | paste -sd+ - | bc)
    time_nauty=$(printf "%.2f" "$time_nauty")

    time_compl=$(awk '/^>F/ {print $(NF-1)}' "$log_file" | paste -sd+ - | bc)
    time_compl=$(printf "%.2f" "$time_compl")

    exc_graphs=$(grep -c '^Found graph' "$log_file")

    # Append summary to the log
    {
        echo "=== Total number of graphs generated: $graph_nauty"
        echo "=== Total number of graphs checked: $graph_compl"
        echo "=== Total time for generation: $time_nauty seconds"
        echo "=== Total time for checking: $time_compl seconds"
        echo "=== Total number of exceptional graphs: $exc_graphs"
        echo
    } >> "$log_file"
done


rm -rf "tmps"
echo "=== All checks completed. Logs are saved in the logs directory."