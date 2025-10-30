#include "game_state.h"
#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <iomanip>


using namespace std;

// Let G be the graph corresponding to `graph6`.
// Returns false if and only if there are at most `surBound` degree 2 vertices
// in G and Staller wins on G when `firstPlayer` starts the game. 
bool checkSI(string graph6, char firstPlayer, int surBound) {
    GameState gameState(graph6);
    int numOfDeg2Vtx = 0;

    for(int i = 0; i < gameState.getN(); i++) {
        if (gameState.deg(i) == 2) {
            ++numOfDeg2Vtx;
        }
    }

    if (numOfDeg2Vtx <= surBound && gameState.outcome(firstPlayer) == 'S') {
        return false;
    }
    return true;
}

// First argument:      `firstPlayer` - either 'D' or 'S'
// Second argument:     `surBound` - upper bound for number of degree 2 vertices
// Reads graphs in graph6 format from stdin, applies `checkSI()` and reports 
// all graphs for which `checkSI()` returns false to stderr. 
// Tracks total runtime and count, writing both to stderr as well.
int main(int argc, char* argv[]) {
    if (argc != 3) {
        cerr << "Usage: " << argv[0] << " <firstPlayer> <surBound>\n";
        return 1;
    } 

    char firstPlayer = argv[1][0];
    if (firstPlayer != 'D' && firstPlayer != 'S') {
        cerr << "First player must be 'D' or 'S'\n";
        return 1;
    }

    int surBound = atoi(argv[2]);

    string graph6;
    clock_t c_start = clock();
    int counter = 0;

    while(getline(cin, graph6)) {
        if (!checkSI(graph6, firstPlayer, surBound)) {
            cerr << "Found graph, on which Staller wins: " << graph6 << "\n";
        }
        ++counter;
    }

    clock_t c_end = clock();
    double time = double(c_end - c_start) / CLOCKS_PER_SEC;

    cerr << fixed << setprecision(2);
    cerr << ">F " << counter << " SI-graphs checked in " << time << " sec\n";
    
    return 0;
}

