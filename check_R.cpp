#include "game_state.h"
#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <iomanip>


using namespace std;

bool checkR(string graph6, char firstPlayer, int surBound) {
    GameState gameState(graph6);
    int maxDeg = 0;
    int maxDegVtx;
    for(int i = 0; i < gameState.getN(); i++) {
        if (gameState.deg(i) > maxDeg) {
            maxDeg = gameState.deg(i);
            maxDegVtx = i;
        }
    }
    int sur = 0;
    for(int i = 0; i < gameState.getN(); i++) {
        if (i != maxDegVtx) 
            sur += (gameState.deg(i) > 3 ? gameState.deg(i) - 3 : 
                                                    3 - gameState.deg(i));
    }
    if (sur <= surBound && gameState.outcome(firstPlayer) == 'S') {
        return false;
    }
    return true;
}

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
    int counter = 0, exceptionalCounter = 0;
    while(getline(cin, graph6)) {
        if (!checkR(graph6, firstPlayer, surBound)) {
            cerr << "Found graph, on which Staller wins: " << graph6 << "\n";
            ++exceptionalCounter;
        }
        ++counter;
    }
    clock_t c_end = clock();
    double time = double(c_end - c_start) / CLOCKS_PER_SEC;
    cerr << fixed << setprecision(2);
    cerr << ">F " << counter << " R-graphs checked in " << time << " sec\n";
    return 0;
}

