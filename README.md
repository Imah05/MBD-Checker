# MBD-Checker

This repository contains the code used for the computer-aided computations in [The minimum degree question for the Maker-Breaker Domination game](link_to_arxiv?).



---

## Quick summary

* `generate_and_check.sh`: master script that compiles .cpp files, launches [nauty](https://pallini.di.uniroma1.it/) graph generations, runs checks and collects logs.

* All checks needed in the thesis are covered by running `./generate_and_check.sh` (after making it executable via `chmod +x generate_and_check.sh`).

* Output: information on whether the checks were successful, how long they took and how many graphs were checked. After the computations finish, the output is written to log files which are stored in `logs/`.

* Precomputed logs are provided in `logs_precomputed/` (running all checks takes ~230 CPU-days).

* Requirements: Unix-like operating system (for nauty), `g++`, `bc`, [nauty](https://pallini.di.uniroma1.it/) (`geng`, `genbg` and `labelg` must be on `PATH`).

## The .cpp files

### `graph.cpp`/`graph.h`
- Rudimentary class for simple graphs via adjacency lists
- Conversion to and from [graph6 strings](https://users.cecs.anu.edu.au/~bdm/data/formats.html) is supported. 

### `game_state.cpp`/`game_state.h`
  - Class for game states (see [thesis](link_to_arxiv?))
  - `outcome` (see Algorithm 1 in the [thesis](link_to_arxiv?)) 
    * Input: a `char firstPlayer` (either `D` or `S`) and a `GameState gs`
    * Output: `gs.outcome(firstPlayer)` returns `D` or `S` depending on the outcome of the game state `gs` when `firstPlayer` starts the game.

### `pccgs.cpp`/`pccgs.h`
  - Class for partially completed core game states (see [thesis](link_to_arxiv?))
  - `completionOutcome` (see Algorithm 4 in the [thesis](link_to_arxiv?))
    * Input: a graph6 string `g6` corresponding to a pcc P and a `char firstPlayer` (either `D` or `S`)
    * Output: `completionOutcome(g6, firstPlayer)` returns `D`, if Dominator wins on every completion of P when `firstPlayer` starts the game. Otherwise it returns `S`. 
  - `filter`
    * Input: a graph6 string `g6` corresponding to a core C
    * Output: `filter(g6)` returns `true` if and only if the degree sequence of its completions is not among the sequences in `inSeqs` (which are loaded from `input_sequences.txt` via `loadInputSequences`) or Dominator wins on all completions of C going first.  

### `sequence_generator.cpp` (contains a `main`)
  - `generateSeqs` (see Algorithm 4 in the [thesis](link_to_arxiv?)) 
    * Input: integers `NStart`, `NEnd`, `d`, `surBound`
    * Output: a vector containing all degree sequences of reduced graphs on at least `NStart` and at most `NEnd` vertices of minimum degree `d` and surplus at least `surBound` to which Lemma 9 (Two-step Erdős-Selfridge for degree sequences of reduced graphs) does not apply.
  - `main`: calls `generateSeqs` with `NStart = 16`, `NEnd = 21`, `d = 3` and `surBound = 9` and writes the resulting degree sequences to the file `input_sequences.txt`. 

### Several `check_*.cpp` (all containing a `main`)
  * `main` in `check_graphs.cpp`: 
  * `main` in `check_R.cpp`:
  * `main` in `check_SI.cpp`:
  * `main` in `check_cores.cpp`: 
