#ifndef PART_COMPL_CORE_GAME_STATE_H
#define PART_COMPL_CORE_GAME_STATE_H

#include "graph.h"

class PartComplCoreGameState : public Graph {
public:
    // Creates a game state on a partially completed core represented by a 
    // graph6 string, in which all the vertices are unclaimed. 
    PartComplCoreGameState(const string& graph6);

    void update();

    // Sets all the vertices to be unclaimed, adds the edge uv to the underlying
    // graph and calls update(). 
    void addEdge(int u, int v);

    // returns -1 if Dominator wins on every completion of this half completed core
    // returns the low deg vtx v if we do not a priori know that Dominator 
    // wins and where v is a 'dangerous' vertex we want to use first when 
    // completing this halfcompleted core
    // returns -2 if there are no lowDegVtx and Staller wins.
    int outcome(char firstPlayer) const;

    
    bool completionFilter() const;
            
private:
    // A vector of length getN() such that DVtx[i] is true if and only if the 
    // vertex i is claimed by Dominator. DVtx[i] is always false for vertices i
    // of degree less than 3.
    vector<bool> DVtx;

    // A vector of length getN() such that SVtx[i] is true if and only if the 
    // vertex i is claimed by Staller.
    vector<bool> SVtx;

    // A vector of length getN() with the following property. If the vertex i is 
    // claimed by Dominator or adjacent to a vertex claimed by Dominator, then 
    // gameStateDeg[i] = -1. Otherwise gameStateDeg[i] is calculated as follows.
    // Let x be the number of unclaimed vertices in the closed neighborhood of 
    // i (the neighborhood of i including i itself). If i has degree at least 3,
    // then gameStateDeg[i] = x, otherwise gameStateDeg[i] = x + 3 - deg(i).
    vector<int> gameStateDeg;

    // A vector containing all the vertices of the underlying graph, that have 
    // degree strictly less than three, ordered such that for two vertices i and
    // j of degree at most 2 it holds that
    // - if gameStateDeg[i] != -1 and gameStateDeg[j] = -1, then i preceeds j,
    // - if -1 < gameStateDeg[i] < gameStateDeg[j] then i preceeds j,
    // - if gameStateDeg[i] = gameStateDeg[j] and deg(i) < deg(j), then i
    //   preceeds j.
    vector<int> lowDegVtx;


    vector<double> pot;

    // totalPot is an upper bound on total ES potential in any completion of 
    // this.
    double totalPot;
    
    // A vector containing all unlcaimed vertices of degree at least 3. 
    vector<int> remVtx;
};

bool completionCheckDegSeq(int n, vector<int> seq);

bool checkAllSeq(string inFileName);

#endif // PART_COMPL_CORE_GAME_STATE_H
