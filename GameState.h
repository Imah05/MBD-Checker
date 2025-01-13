#ifndef GAMESTATE_H
#define GAMESTATE_H

#include <set>
#include <vector>
#include <list>
#include <utility>
#include <algorithm>
#include <stdexcept>
#include <cmath>
# include "graph.h"

using namespace std;

class GameState {
public:
    GameState(Graph *g);
    void update();
    // void addDVtx(int dVtx);  // later, for optimization
    // void addSVtx(int sVtx);  // later, for optimization
    bool isSWin();  // returns whether if there exists a vertex,
                    // claimed by Staller, all of whose neighbors 
                    // are also claimed by Staller.
    char outcome(char firstPlayer); // returns 'D' iff Dominator wins 
                                    // and 'S' iff Staller wins.
                                    // firstPlayer is either 'D' or
                                    // 'S' depending on who plays first
            
private:
    Graph *graph;       // pointer avoids expensive copying
    vector<bool> DVtx;  // DVtx[i] is true iff i is occupied by Dominator.
    vector<bool> SVtx;  // SVtx[i] is true iff i is occupied by Staller.
    vector<int> gameStateDegSeq;        // Degree sequence; D's vtx and vtx adj. to D's vtx have 'deg' -1; 
                                        // for other vtx it counts the number of vtx not occupied by S. 
    vector<double> potSeq;  // For a vertex i, which is not claimed by Dominator nor Staller,
                            // the total potential drops by potSeq[i] if Dominator claimes vertex i.
    double totalPot;        // Total ES potential
    vector<int> remainingVtx;   // Vertices not claimed by Dominator nor Staller ordered by the corresponding
                                // value stored in potSeq.
};

#endif // GAMESTATE_H