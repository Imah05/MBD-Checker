#include "graph.h"
#include "GameState.h"
#include "filter2.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cmath>
#include <limits>
#include <cmath>


using namespace std;



int main() {
    string filename;
    cout << "Enter the name of the file containing graphs in graph6 format: ";
    cin >> filename;

    ifstream inputFile(filename);
    if (!inputFile) {
        cerr << "Error: Unable to open file " << filename << endl;
        return 1;
    }

    string graph6;
    vector<string> graphList;
    while (getline(inputFile, graph6)) {
        graphList.push_back(graph6);
    }
    inputFile.close();
	
	cout << "Loading complete." << endl;
    int counter = 0;
    int badcounter = 0;
    int* when = new int[5];  //counts when graphs get filtered
    for (int i=0; i<5; i++) {
    	when[i]=0;
    }
    int numberOfGraphs = graphList.size();
    bool schalter = true;
     ofstream myfile;
  	myfile.open (filename+"Filter2.g6");
  	
    for (const string& g6 : graphList) {
        ++counter;
        bool isbad=filter2(g6);
        if (isbad) {
        	isbad=filter1(g6);
        }

	if (isbad) {
		badcounter++;	
		myfile << g6 << endl;
	}
    	if (counter%10000==0) {
        	cout << "At the moment we handeled " << counter << "/" << numberOfGraphs << " graphs." << endl;
        	cout << badcounter << " of them behave badly." << endl;
        }
    }
	cout << "In total we handeled " << counter << "/" << numberOfGraphs << " graphs." << endl;
       	cout << badcounter << " of them behave badly." << endl;
	myfile.close();
    return 0;
}
