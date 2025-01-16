#include "GameState.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cmath>
#include <limits>
#include "Core.h"
#include <filesystem>

namespace fs = std::filesystem;
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
    const string inputDir = "graphs/18-vertices-deg3/unfiltered/";
    const string outputDir = "graphs/18-vertices-deg3/filtered/";
    if (!fs::exists(outputDir)) {
        fs::create_directories(outputDir);
    }
    for (const auto& entry : fs::directory_iterator(inputDir)) {
        if (!entry.is_regular_file()) {
            continue;
        }
        string inputFilePath = entry.path().string();
        string fileName = entry.path().filename().string();
        string outputFilePath = outputDir + fileName;

        cout << "Processing: " << inputFilePath << " -> " << outputFilePath << endl;
        ifstream inputFile(inputFilePath);
        if (!inputFile.is_open()) {
            cerr << "Error: Could not open input file " << inputFilePath << endl;
            continue;
        }

        ofstream outputFile(outputFilePath);
        if (!outputFile.is_open()) {
            cerr << "Error: Could not open output file " << outputFilePath << endl;
            continue;
        }
        string graphStr;
        while (getline(inputFile, graphStr)) {
            try {
                Graph g = Graph(graphStr);
                Core core = Core(&g);
                if (!core.filter('D')) {
                    outputFile << graphStr << endl;
                }
            } catch (const exception& e) {
                cerr << "Error processing graph: " << graphStr << endl;
                cerr << "Exception: " << e.what() << endl;
            }
        }

        inputFile.close();
        outputFile.close();
    }

    cout << "All files processed." << endl;
    return 0;
}