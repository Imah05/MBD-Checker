#include "part_compl_core_game_state.h"
#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <iomanip>


using namespace std;

int main() {
    loadInputSequences();
    string graph6;
    clock_t c_start = clock();
    int counter = 0;
    while(getline(cin, graph6)) {
        if (!completionFilter(graph6)) {
            cerr << "Found graph, on which Staller wins: " << graph6 << endl;
        }
        ++counter;
    }
    clock_t c_end = clock();
    double time = double(c_end - c_start) / CLOCKS_PER_SEC;
    cerr << fixed << setprecision(2);
    cerr << ">F " << counter << " cores checked in " << time << " sec" << endl;
    return 0;
}