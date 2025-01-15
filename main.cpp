#include "graph.h"
#include "GameState.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cmath>
#include <limits>
#include "RSI.h"


using namespace std;

// int main() {
//     string filename;
//     cout << "Enter the name of the file containing graphs in graph6 format: ";
//     cin >> filename;
//     ifstream inputFile(filename);
//     if (!inputFile) {
//         cerr << "Error: Unable to open file " << filename << endl;
//         return 1;
//     }
//     string graph6;
//     vector<string> graphList;
//     while (getline(inputFile, graph6)) {
//         graphList.push_back(graph6);
//     }
//     inputFile.close();
//     int counter = 0;
//     int numberOfGraphs = graphList.size();
//     bool allOutcomesD = true;
//     for (const string& g6 : graphList) {
//         ++counter;
//         if ((counter % 100000) == 0) {
//             cout << counter << "/" << numberOfGraphs << endl;
//         }
//         Graph g = Graph(g6);
//         GameState gs = GameState(&g);
//         if (gs.outcome('D') != 'D') {
//             allOutcomesD = false;
//             break;
//         }
//     }
//     if (allOutcomesD) {
//         cout << "All graphs have outcome 'D'." << endl;
//     } else {
//         cout << "Not all graphs have outcome 'D'." << endl;
//     }
//     cin.ignore(numeric_limits<streamsize>::max(), '\n');
//     cout << "Press Enter to exit..." << endl;
//     cin.get();
//     return 0;
// }


int main() {
    cout << filter_R(8, 2, 'S').first << ", " << filter_R(8, 2, 'S').second << endl;
    cout << filter_R(8, 3, 'S').first << ", " << filter_R(8, 3, 'S').second << endl;
    cout << filter_R(8, 4, 'S').first << ", " << filter_R(8, 4, 'S').second << endl;
    cout << filter_R(8, 5, 'S').first << ", " << filter_R(8, 5, 'S').second << endl;
    cout << filter_R(8, 6, 'S').first << ", " << filter_R(8, 6, 'S').second << endl;
    cout << filter_R(8, 7, 'S').first << ", " << filter_R(8, 7, 'S').second << endl;
    cout << filter_R(8, 8, 'S').first << ", " << filter_R(8, 8, 'S').second << endl;
    cout << filter_R(8, 9, 'S').first << ", " << filter_R(8, 9, 'S').second << endl;
    return 0;
}