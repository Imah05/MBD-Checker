#ifndef GAME_STATE_H
#define GAME_STATE_H

#include "graph.h"

class GameState : public Graph {
public:
    // Creates a game state on the graph represented by a graph6 string, in
    // which all the vertices are unclaimed. 
    GameState(string graph6);

    // Updates the vectors gameStateDeg, pot and remVtx as well as the double 
    // totalPot so that they satisfy their conditions as described below. The 
    // graph itself as well as the vertices claimed by the two players, that is
    // the two vectors DVtx and SVtx remain unchanged.
    void update();

    // Returns 'D' if Dominator wins and 'S' if Staller wins in the 
    // Maker-Breaker Domination game on this game state. firstPlayer has to be 
    // 'D' or 'S' depending on whether Dominator or Staller starts the game. 
    char outcome(char firstPlayer);
            
private:
    // A vector of length getN() such that DVtx[i] is true if and only if the 
    // vertex i is claimed by Dominator.
    vector<bool> DVtx;

    // A vector of length getN() such that SVtx[i] is true if and only if the 
    // vertex i is claimed by Staller.
    vector<bool> SVtx;  

    // A vector of length getN() with the following property. If the vertex i is 
    // claimed by Dominator or adjacent to a vertex claimed by Dominator, then 
    // gameStateDeg[i] = -1. Otherwise gameStateDeg[i] is the number of 
    // unclaimed vertices in the closed neighborhood of v (the neighborhood of 
    // v including v itself).
    vector<int> gameStateDeg;

    // For an unclaimed vertex i, pot[i] measures by how much the overall 
    // Erdős-Selfridge potential drops if Dominator claims vertex i in the next
    // move. This is the same as the quantity by which the overall potential 
    // increases if Staller claims vertex i in the next move. For claimed 
    // vertices j, pot[j] is undefined and can be anything.
    vector<double> pot;

    // The total Erdős-Selfridge potential of this game state.
    double totalPot;

    // A vector containing all the unclaimed vertices of this game state ordered
    // by their potential. If i and j are two unclaimed vertices with pot[i] > 
    // pot[j] then i appears before j in remVtx
    vector<int> remVtx;
};

#endif // GAMESTATE_H