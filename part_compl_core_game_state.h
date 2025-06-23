#ifndef PART_COMPL_CORE_GAME_STATE_H
#define PART_COMPL_CORE_GAME_STATE_H

#include <set>
#include <string>
#include <vector>
#include "graph.h"

class PartComplCoreGameState : public Graph {
public:
    // Creates a game state on a partially completed core represented by a 
    // graph6 string, in which all the vertices are unclaimed. 
    PartComplCoreGameState(const string& graph6);

    void update();

    // Sets all the vertices ot be unclaimed, adds the edge uv to the underlying
    // graph and calls update(). 
    void addEdge(int u, int v);


    bool isSWin() const;

    // returns -1 if Dominator wins on every completion of this half completed core
    // returns the low deg vtx v if we do n ot a priori know that Dominator 
    // wins and where v is a 'dangerous' vertex we want to use first when 
    // completing this halfcompleted core
    // returns -2 if we do not a priori know that Dominator 
    // wins, but we don't have a hint vertex for the completion
    int outcomeLowerBound(char firstPlayer) const; 
    char out_lw_bnd_after_lowDegMove(int vertex) const; 
    bool filter() const;
    bool completion_filter() const;
    bool completion_filter2() const;
    

    friend set<string> nextCompl(set<string> g6set);
            
private:
    vector<int> lowDegVtx;
    vector<bool> DVtx;  // DVtx[i] is true iff i is occupied by Dominator.
    vector<bool> SVtx;  // SVtx[i] is true iff i is occupied by Staller.
    vector<int> gameStateDeg;
    vector<double> pot;

    
    double totalPot;  // upper bound on total ES potential in any completion of core. 
    vector<int> remVtx;   // Vertices not claimed by Dominator nor Staller of deg
                                // at least 3.
};

#endif // PART_COMPL_CORE_GAME_STATE_H
