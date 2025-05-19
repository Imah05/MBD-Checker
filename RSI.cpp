#include "RSI.h"

using namespace std;

bool check_all_RSI_Lemmas() {
    // use nauty-genbg and nauty-geng on the institute computers!!!

    // checks, whether D. wins on every bipartite graph with bipartition (S,I) 
    // such that |S| = 8, |I| = 9, deg(i) = 3 for i in I, 2 <= deg(s) <= 7 
    // for all s in S and at most 4 degree 2 vtx going first.
    if(!check_SI("genbg -d2:3 -D7:3 8 9", 'D', 4))
        return false;

    // checks, whether D. wins on every bipartite graph with bipartition (S,I) 
    // such that |S| = 7, |I| = 8, deg(i) = 3 for i in I, 2 <= deg(s) 
    // for all s in S and at most 4 degree 2 vtx going first.
    if (!check_SI("genbg -d2:3 -D8:3 7 8", 'D', 4))
        return false;

    // checks, whether D. wins on every bipartite graph with bipartition (S,I) 
    // such that |S| = 7, |I| = 9, 2 <= deg(i) <= 3 for all i in I, 2 <= deg(s) 
    // for all s in S and at most 3 degree 2 vtx going first.
    if (!check_SI("genbg -d2:2 -D9:3 7 9", 'D', 3))
        return false;

    // checks, whether D. wins on every bipartite graph with bipartition (S,I) 
    // such that 3 <= |S| <= 6 , |I| = |S|+1, deg(i) = 3 for all i in I, 2 <= deg(s) 
    // for all s in S and at most 4 degree 2 vtx going second.
    for (int s = 6; s > 2; --s) {
        string cmd = "genbg -d2:3 -D" + to_string(s+1) + ":3 " + to_string(s) + " " + to_string(s+1);
        if (!check_SI(cmd, 'S', 4))
            return false;
    }
    cout << "ALL RSI-LEMMAS CHECKED SUCCESSFULLY" << endl;
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

list<string> check_R(string cmd, int sur) {
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
            if (gs.outcome('S') == 'D')
                ++numOfDomWinGraphs;
            else {
                cout << g6 << endl;
                out.push_back(g6);
            }
        }
    }
    pclose(pipe);
    cout << out.size() << " bad graphs, " << numOfGraphsWithWrongDegSeq << " graphs with surplus greater than ";
    cout << sur << " and " << numOfDomWinGraphs << " graphs on which Dominator wins going second." << endl;
    
    return out;
}