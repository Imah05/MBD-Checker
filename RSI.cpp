#include "RSI.h"

using namespace std;

bool check_all_RSI_Lemmas_sur7() {
    // use nauty-genbg and nauty-geng on the institute computers!!!


    // checks, that D. wins (going second) on every graph with at least 
    // 4 and at most 6 vertices, minimum degree 1 and sum_{v != w} |deg(v)-3| <= 4, 
    // where w is a maxdeg vtx, except for two exceptions (one with 
    // 5 vertices and one with 6 vertices)
    if(!check_R("geng -d1 4", 'S', 4).empty())
        return false;
    if(check_R("geng -d1 5", 'S', 4).size() != 1)
        return false;
    if(check_R("geng -d1 6", 'S', 4).size() != 1)
        return false;

    // checks, whether D. wins (going first) on every bipartite graph with bipartition (S,I) 
    // such that |S| = 8, |I| = 9, deg(i) = 3 for i in I, 2 <= deg(s) <= 7 
    // for all s in S and at most 4 degree 2 vtx.
    if(!check_SI("genbg -d2:3 -D7:3 8 9", 'D', 4))
        return false;

    // checks, whether D. wins (going first) on every bipartite graph with bipartition (S,I) 
    // such that |S| = 7, |I| = 8, deg(i) = 3 for i in I, 2 <= deg(s) 
    // for all s in S and at most 4 degree 2 vtx.
    if (!check_SI("genbg -d2:3 -D8:3 7 8", 'D', 4))
        return false;

    // checks, whether D. wins (going first) on every bipartite graph with bipartition (S,I) 
    // such that |S| = 7, |I| = 9, 2 <= deg(i) <= 3 for all i in I, 2 <= deg(s) 
    // for all s in S and at most 3 degree 2 vtx.
    if (!check_SI("genbg -d2:2 -D9:3 7 9", 'D', 3))
        return false;

    // checks, whether D. wins (going second) on every bipartite graph with bipartition (S,I) 
    // such that 3 <= |S| <= 6 , |I| = |S|+1, deg(i) = 3 for all i in I, 2 <= deg(s) 
    // for all s in S and at most 4 degree 2 vtx.
    for (int s = 6; s > 2; --s) {
        string cmd = "genbg -d2:3 -D" + to_string(s+1) + ":3 " + to_string(s) + " " + to_string(s+1);
        if (!check_SI(cmd, 'S', 4))
            return false;
    }


    // checks, whether D. wins (going first) on every graph on at most 12 
    // vertices, minimum degree 1 and sum_{v != w} |deg(v)-3| <= 4, 
    // where w is a maxdeg vtx.
    if(!check_R("geng -d1D9 12 16:19", 'D', 4).empty())
        return false;
    if(!check_R("geng -d1D8 11 15:17", 'D', 4).empty())
        return false;
    if(!check_R("geng -d1D3 10 13:13", 'D', 4).empty())
        return false;
   
    cout << "ALL RSI-LEMMAS FOR SUR 7 CHECKED SUCCESSFULLY" << endl;
    return true;
}

bool check_all_RSI_Lemmas_sur8() {
    // use nauty-genbg and nauty-geng on the institute computers!!!

    // checks, whether D. wins (going second) on every graph on at most 7 
    // vertices, minimum degree 1 and sum_{v != w} |deg(v)-3| <= 2, 
    // where w is a maxdeg vtx.
    for (int r = 2; r <= 7; ++r) {
        if(!check_R("geng -d1 " + to_string(r), 'S', 2).empty())
            return false;
    }

    // checks, whether D. wins (going first) on every bipartite graph with bipartition (S,I) 
    // such that 6 <= |S| <= 7, 8 <= |I| <= 9, deg(i) = 3 for i in I, 2 <= deg(s)
    // for all s in S and at most 2 degree 2 vtx.
    if (!check_SI("genbg -d2:3 -D9:3 7 9", 'D', 2))
        return false;
    if (!check_SI("genbg -d2:3 -D8:3 6 8", 'D', 2))
        return false;

    // checks, whether D. wins (going second) on every bipartite graph with bipartition (S,I) 
    // such that 3 <= |S| <= 5, 5 <= |I| <= 7, deg(i) = 3 for i in I, 2 <= deg(s)
    // for all s in S and at most 2 degree 2 vtx.
    if (!check_SI("genbg -d2:3 -D7:3 5 7", 'S', 2))
        return false;
    if (!check_SI("genbg -d2:3 -D6:3 4 6", 'S', 2))
        return false;
    if (!check_SI("genbg -d2:3 -D5:3 3 5", 'S', 2))
        return false;

    // checks, whether D. wins (going second) on every bipartite graph with bipartition (S,I) 
    // such that |S| = 9, |I| = 10, deg(i) = 3 for i in I, 2 <= deg(s) <= 4
    // for all s in S and at most 2 degree 2 vtx.
    if (!check_SI("genbg -d2:3 -D4:3 9 10", 'D', 5))
        return false;

    // checks, whether D. wins (going first) on every bipartite graph with bipartition (S,I) 
    // such that |S| = 8, |I| = 9, deg(i) = 3 for i in I, 2 <= deg(s) <= 8 
    // for all s in S and at most 5 degree 2 vtx.
    if(!check_SI("genbg -d2:3 -D8:3 8 9", 'D', 5))
        return false;

    // checks, whether D. wins (going first) on every bipartite graph with bipartition (S,I) 
    // such that |S| = 7, |I| = 8, deg(i) = 3 for i in I, 2 <= deg(s) 
    // for all s in S and at most 5 degree 2 vtx.
    if (!check_SI("genbg -d2:3 -D8:3 7 8", 'D', 5))
        return false;

    // checks, whether D. wins (going first) on every bipartite graph with bipartition (S,I) 
    // such that |S| = 7, |I| = 9, 2 <= deg(i) <= 3 for all i in I with just one vertex of 
    // degree 2 in I, 2 <= deg(s) for all s in S and at most 3 degree 2 vtx.
    if (!check_SI("genbg -d2:2 -D9:3 7 9 26:26", 'D', 4))
        return false;

    // checks, whether D. wins (going second) on every bipartite graph with bipartition (S,I) 
    // such that 3 <= |S| <= 6 , |I| = |S|+1, deg(i) = 3 for all i in I, 2 <= deg(s) 
    // for all s in S.
    for (int s = 6; s > 2; --s) {
        string cmd = "genbg -d2:3 -D" + to_string(s+1) + ":3 " + to_string(s) + " " + to_string(s+1);
        if (!check_SI(cmd, 'S', 4))
            return false;
    }

    // checks, whether D. wins (going first) on every graph on at most 12 
    // vertices, minimum degree 1 and sum_{v != w} |deg(v)-3| <= 5, 
    // where w is a maxdeg vtx.
    if(!check_R("geng -d1D9 12 16:19", 'D', 5).empty())
        return false;
    if(!check_R("geng -d1D8 11 14:17", 'D', 5).empty())
        return false;
    if(!check_R("geng -d1D3 10 13:13", 'D', 5).empty())
        return false;

    cout << "ALL RSI-LEMMAS FOR SUR 8 CHECKED SUCCESSFULLY" << endl;
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