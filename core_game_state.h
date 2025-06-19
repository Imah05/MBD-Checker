#ifndef CORE_GAME_STATE_H
#define CORE_GAME_STATE_H

#include <set>
#include <string>
#include <vector>
#include "graph.h"

class CoreGameState : public Graph {
public:
    CoreGameState(const string& graph6);

    void addEdge(int u, int v);
    bool validCore() const;
    bool isSWin() const;

    // returns -1 if Dominator wins on every completion of this half completed core
    // returns the low deg vtx v if we do n ot a priori know that Dominator 
    // wins and where v is a 'dangerous' vertex we want to use first when 
    // completing this halfcompleted core
    // returns -2 if we do not a priori know that Dominator 
    // wins, but we don't have a hint vertex for the completion
    int out_lw_bnd(char firstPlayer) const; 
    char out_lw_bnd_after_lowDegMove(int vertex) const; 
    bool filter() const;
    bool completion_filter() const;
    bool completion_filter2() const;
    void update();

    friend set<string> nextCompl(set<string> g6set);
            
private:
    vector<bool> DVtx;  // DVtx[i] is true iff i is occupied by Dominator.
    vector<bool> SVtx;  // SVtx[i] is true iff i is occupied by Staller.
    vector<int> gameStateDegSeq;
    vector<int> lowDegVtx;
    double totalPot;  // upper bound on total ES potential in any completion of core. 
    vector<int> remainingVtx;   // Vertices not claimed by Dominator nor Staller of deg
                                // at least 3.
};

#endif // CORE_GAME_STATE_H
