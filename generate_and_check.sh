arr=(
"nauty-genbg -d8:0 -D8:3 3 16 24:24"
"nauty-genbg -d7:0 -D8:3 3 17 23:23"
"nauty-genbg -d7:0 -D9:3 3 17 25:25"
"nauty-genbg -d9:0 -D9:3 3 17 27:27"
"nauty-genbg -d6:0 -D6:3 4 16 24:24"
"nauty-genbg -d4:0 -D8:3 4 16 26:26"
# "nauty-genbg -d4:0 -D8:3 4 16 28:28"
# "nauty-genbg -d6:0 -D8:3 4 16 30:30"
# "nauty-genbg -d8:0 -D8:3 4 16 32:32"
# "nauty-genbg -d4:0 -D6:3 5 15 27:27"
# "nauty-genbg -d4:0 -D7:3 5 15 29:29"
# "nauty-genbg -d6:0 -D9:3 3 18 24:24"
# "nauty-genbg -d6:0 -D10:3 3 18 26:26"
# "nauty-genbg -d8:0 -D10:3 3 18 28:28"
# "nauty-genbg -d10:0 -D10:3 3 18 30:30"
# "nauty-genbg -d4:0 -D7:3 4 17 25:25"
# "nauty-genbg -d4:0 -D9:3 4 17 27:27"
# "nauty-genbg -d4:0 -D9:3 4 17 29:29"
# "nauty-genbg -d4:0 -D9:3 4 17 31:31"
# "nauty-genbg -d6:0 -D9:3 4 17 33:33"
# "nauty-genbg -d8:0 -D9:3 4 17 35:35"
# "nauty-genbg -d4:0 -D7:3 5 16 28:28"
# "nauty-genbg -d4:0 -D8:3 5 16 30:30"
# "nauty-genbg -d4:0 -D8:3 5 16 32:32"
# "nauty-genbg -d4:0 -D8:3 5 16 34:34"
# "nauty-genbg -d4:0 -D8:3 5 16 36:36"
# "nauty-genbg -d6:0 -D8:3 5 16 38:38"
# "nauty-genbg -d8:0 -D8:3 5 16 40:40"
# "nauty-genbg -d4:0 -D5:3 6 15 29:29"
# "nauty-genbg -d4:0 -D7:3 6 15 31:31"
# "nauty-genbg -d4:0 -D7:3 6 15 33:33"
# "nauty-genbg -d4:0 -D7:3 6 15 35:35"
# "nauty-genbg -d4:0 -D7:3 6 15 37:37"
# "nauty-genbg -d4:0 -D5:3 7 14 32:32"
# "nauty-genbg -d4:0 -D7:3 7 14 34:34"
# "nauty-genbg -d4:0 -D7:3 7 14 36:36"
# "nauty-genbg -d4:0 -D5:3 8 13 35:35" 
)


run_job() {
    eval "$1 $2" | ./check_cores
}

export -f run_job

k=$(nproc)
[ "$k" -gt 1 ] && k=$((k - 1))
parallel -j"$k" --group run_job ::: "${arr[@]}" ::: $(seq 0 999 | awk '{print $1"/1000"}')



