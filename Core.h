#ifndef CORE_H
#define CORE_H

#include <set>
#include <vector>
#include <list>
#include <utility>
#include <algorithm>
#include <stdexcept>
#include <cmath>
#include "graph.h"


using namespace std;

class Core {
public:
    Core(Graph *core);
    bool validCore();
    bool isSWin();

    // returns -1 if Dominator wins on every completion of this half completed core
    // returns the low deg vtx v if we do not a priori know that Dominator 
    // wins and where v is a 'dangerous' vertex we want to use first when 
    // completing this halfcompleted core
    // returns -2 if we do not a priori know that Dominator 
    // wins, but we don't have a hint vertex for the completion
    int out_lw_bnd(char firstPlayer); 
    char out_lw_bnd_after_lowDegMove(int vertex); 
    bool filter();
    bool completion_filter();
    void update();
            
private:
    Graph *core;        // pointer avoids expensive copying
    vector<bool> DVtx;  // DVtx[i] is true iff i is occupied by Dominator.
    vector<bool> SVtx;  // SVtx[i] is true iff i is occupied by Staller.
    vector<int> gameStateDegSeq;
    vector<int> lowDegVtx;
    double totalPot;  // upper bound on total ES potential in any completion of core. 
    vector<int> remainingVtx;   // Vertices not claimed by Dominator nor Staller of deg
                                // at least 3.
};

#endif // CORE_H
