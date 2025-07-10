#include "part_compl_core_game_state.h"
#include <iostream>
#include <fstream>
#include <stdlib.h>


using namespace std;


int main(int argc, char** argv) {
    vector<string> input;
    string line;
    ifstream inFile("Cores//n21h7min4max7e34.txt");

    if (!inFile.is_open()) {
        cerr << "Failed to open file.\n";
        return 1;
    }

    for (int i = 0; i < 10; i++) {
        if (!getline(inFile, line)) {
            cerr << "File has fewer than 10 lines.\n";
            return 1;
        }
    }

    size_t lineCount = 0;
    int counter = 100000000;
    while (--counter && getline(inFile, line)) {
        input.push_back(line);
        lineCount++;
        if (lineCount % 1000000 == 0) {
            cout << "Read " << lineCount << " lines.\n";
        }
    }

    cout << "Finished reading. Total lines: " << lineCount << endl;

    unordered_set<string> out = labelCanonicalBatch(input);
    cout << *out.begin() << endl;
    // checkFile(argv[1], atoi(argv[2]), atoi(argv[3]));
}
