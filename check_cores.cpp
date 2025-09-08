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
        if (!filter(graph6)) {
            cerr << "Found graph, on which Staller wins: " << graph6 << "\n";
        }
        ++counter;
    }
    clock_t c_end = clock();
    double time = double(c_end - c_start) / CLOCKS_PER_SEC;
    cerr << fixed << setprecision(2);
    cerr << ">F " << counter << " cores checked in " << time << " sec\n";
    return 0;
}


int main() {
    vector<vector<int>> result = generateSeqs(16, 21, 3, 9);

    ofstream outFile("sequences1.txt");
    if (!outFile) {
        cerr << "Error opening file sequences.txt" << endl;
        return 1;
    }

    for (const auto& seq : result) {
        for (size_t i = 0; i < seq.size(); ++i) {
            outFile << seq[i];
            if (i + 1 < seq.size()) outFile << " ";
        }
        outFile << "\n";
    }

    outFile.close();
    return 0;
}