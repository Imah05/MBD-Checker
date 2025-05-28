#include "RSI.h"

using namespace std;

// this checks Lemma with the number 2 in the table
bool check_Lemma_R2() {
    cout << endl << endl<< "Checking Lemma #2 in the R-Lemma table..." << endl;
    if (check_R("geng -d1 5", 'S', 5).size() > 3)
        return false;
    if (check_R("geng -d1 6", 'S', 5).size() > 3)
        return false;
    cout << "Lemma #2 in the R-Lemma table checked successfuly" << endl << endl;
    return true;
}

// this checks Lemma with the number 3 in the table
bool check_Lemma_R3() {
    cout << endl << endl<< "Checking Lemma #3 in the R-Lemma table..." << endl;
    if(!check_R("geng -d1 4", 'S', 2).empty())
        return false;
    if(!check_R("geng -d1 5", 'S', 2).empty())
        return false;
    if(!check_R("geng -d1 6", 'S', 2).empty())
        return false;
    if(!check_R("geng -d1 7", 'S', 2).empty())
        return false;    
    cout << "Lemma #3 in the R-Lemma table checked successfuly" << endl << endl;
    return true;
}

// this checks Lemma with the number 4 in the table
bool check_Lemma_R4() {
    cout << endl << endl<< "Checking Lemma #4 in the R-Lemma table..." << endl;
    if(!check_R("geng -d1D3 9 11:11", 'D', 5).empty())
        return false;
    if(!check_R("geng -d1D6 10 13:14", 'D', 5).empty())
        return false;
    if(!check_R("geng -d1D8 11 14:17", 'D', 5).empty())
        return false;
    if(!check_R("geng -d1D9 12 16:19", 'D', 5).empty())
        return false;    
    cout << "Lemma #4 in the R-Lemma table checked successfuly" << endl << endl;
    return true;
}

// this checks Lemma with the number 6 in the table
bool check_Lemma_R6() {
    cout << endl << endl<< "Checking Lemma #6 in the R-Lemma table..." << endl;
    if (!check_R("geng -d2D9 13 17:20", 'D', 5).empty())
        return false;
    if (!check_R("geng -d2D9 14 19:22", 'D', 5).empty())
        return false;
    cout << "Lemma #6 in the R-Lemma table checked successfuly" << endl << endl;
    return true;
}

bool check_SI(string cmd, char firstPlayer, int sur) {
    FILE *pipe = popen(cmd.c_str(), "r");
    if (!popen) {
        cerr << "Failed to execute nauty command." << endl;
        return false;
    }
    char buffer[128];
    string g6;

    int numOfGraphsWithWrongDegSeq = 0;
    int numOfDomWinGraphs = 0;
    while (fgets(buffer, sizeof(buffer), pipe)) {
        string g6 = buffer;
        if (g6.back() == '\n')
            g6.pop_back();
        Graph g(g6);
        int numOfDeg2Vtx = 0;
        for(int i = 0; i < g.getNumVertices(); i++) {
            if (g.neighborhood(i).size() == 2)
                ++numOfDeg2Vtx;
        }
        if (numOfDeg2Vtx > sur) {
            ++numOfGraphsWithWrongDegSeq;
        }
        else {
            GameState gs = GameState(&g);
            if (gs.outcome(firstPlayer) == 'D')
                ++numOfDomWinGraphs;
            else {
                cout << "Unsuccessful Check: Staller wins on " << g6 << " going";
                if (firstPlayer == 'D') 
                    cout << "second!" << endl; 
                else if (firstPlayer == 'S') 
                    cout << "first!" << endl;
                return false;
            }
        }
    }
    pclose(pipe);
    cout << "Successful Check: " << numOfGraphsWithWrongDegSeq << " graphs with more than ";
    cout << sur << " vertices of degree 2 and ";
    cout << numOfDomWinGraphs << " graphs on which Dominator wins going ";
    if (firstPlayer == 'D') 
        cout << "first." << endl; 
    else if (firstPlayer == 'S') 
        cout << "second." << endl;
    return true;
}

list<string> check_R(string cmd, char firstPlayer, int sur) {
    list<string> out;
    FILE *pipe = popen(cmd.c_str(), "r");
    if (!popen) {
        cerr << "Failed to execute nauty command." << endl;
        return out;
    }
    char buffer[128];
    string g6;
    
    int numOfGraphsWithWrongDegSeq = 0;
    int numOfDomWinGraphs = 0;
    while(fgets(buffer, sizeof(buffer), pipe)) {
        g6 = buffer;
        if (g6.back() == '\n')
            g6.pop_back();
        Graph g(g6);
        int maxDeg = 0;
        int maxDegVtx;
        for(int i = 0; i < g.getNumVertices(); i++) {
            if (g.neighborhood(i).size() > maxDeg) {
                maxDeg = g.neighborhood(i).size();
                maxDegVtx = i;
            }
        }
        int cursur = 0;
        for(int i = 0; i < g.getNumVertices(); i++) {
            if (i != maxDegVtx) 
                cursur += (g.neighborhood(i).size() > 3 ? g.neighborhood(i).size() - 3 : 3 - g.neighborhood(i).size());
        }
        if (cursur > sur) {
            ++numOfGraphsWithWrongDegSeq;
        }
        else {
            GameState gs = GameState(&g);
            if (gs.outcome(firstPlayer) == 'D')
                ++numOfDomWinGraphs;
            else {
                cout << g6 << endl;
                out.push_back(g6);
            }
        }
    }
    pclose(pipe);
    cout << out.size() << " bad graphs, " << numOfGraphsWithWrongDegSeq << " graphs with surplus greater than ";
    cout << sur << " and " << numOfDomWinGraphs << " graphs on which Dominator wins going ";
    if (firstPlayer == 'D') 
        cout << "first." << endl;
    else if (firstPlayer == 'S')
        cout << "second." << endl;
    
    return out;
}