#include "pccgs.h"
#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <iomanip>


using namespace std;

// Reads cores in graph6 format from stdin, applies `filter()`, and reports
// all cores for which `filter()` returns false to stderr. 
// Tracks total runtime and count and writes both to stderr as well.
int main() {
    loadDegSeqs();

    string graph6;
    clock_t c_start = clock();
    long counter = 0;

    while(getline(cin, graph6)) {
        if (!filter(graph6)) {
            cerr << "Found core with completion, on which Staller wins: " 
                << graph6 << "\n";
        }
        ++counter;
    }

    clock_t c_end = clock();
    double time = double(c_end - c_start) / CLOCKS_PER_SEC;

    cerr << fixed << setprecision(2);
    cerr << ">F " << counter << " cores checked in " << time << " sec\n";

    return 0;
}