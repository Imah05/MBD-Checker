#include "game_state.h"
#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <iomanip>


using namespace std;

int main(int argc, char* argv[]) {
    if (argc != 2) {
        cerr << "Usage: " << argv[0] << " <firstPlayer>\n";
        return 1;
    }
    char firstPlayer = argv[1][0];
    if (firstPlayer != 'D' && firstPlayer != 'S') {
        cerr << "First player must be 'D' or 'S'\n";
        return 1;
    }
    string graph6;
    clock_t c_start = clock();
    int counter = 0;
    while(getline(cin, graph6)) {
        GameState gameState(graph6);
        if (gameState.outcome(firstPlayer) != 'D') {
            cerr << "Found graph, on which Staller wins: " << graph6 << "\n";
        }
        ++counter;
    }
    clock_t c_end = clock();
    double time = double(c_end - c_start) / CLOCKS_PER_SEC;
    cerr << fixed << setprecision(2);
    cerr << ">F " << counter << " graphs checked in " << time << " sec\n";
    return 0;
}

