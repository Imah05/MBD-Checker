#ifndef PART_COMPL_CORE_GAME_STATE_H
#define PART_COMPL_CORE_GAME_STATE_H

#include "graph.h"
#include <unordered_set>

// A vector containing all the degree sequences of the graphs we want to check.
extern vector<vector<int>> inputDegSequences;

// Loads the degree sequences from the input file filename to inputDegSequences. 
// Expects the degree sequences in filename to come in separate lines, where 
// each line contains an increasingly sorted sequence of non-negative integers 
// separated by whitespaces.
void loadInputSequences(const string& filename = "input_sequences.txt");

class PartComplCoreGameState : public Graph {
public:
    // Creates a game state on a partially completed core represented by a 
    // graph6 string, in which all the vertices are unclaimed. 
    PartComplCoreGameState(const string& graph6);

    // Updates the vectors gameStateDeg, pot, lowDegVtx and remVtx as well as 
    // the double totalPot so that they match their description from below. The 
    // graph itself as well as the vertices claimed by the two players, that is
    // the two vectors DVtx and SVtx, remain unchanged.
    void update();

    // Adds the edge uv to the underlying graph. 
    void addEdge(int u, int v);
    
    // Removes the edge uv from the underlying graph if it exists.
    void removeEdge(int u, int v);

    // If it returns -1 then Dominator wins on every completion of this 
    // partially completed core game state, where firstPlayer starts the game.
    // If it returns -2 then Staller wins on some completion of this partially 
    // completed core game state, where firstPlayer starts the game.
    // If it returns neither -1 nor -2 it returns a low degree vertex (which we
    // want to use first for completion).
    int outcome(char firstPlayer) const;

    // Returns true if and only if Dominator wins on every completion of this 
    // partially completed core game going first and false otherwise. The game 
    // state on this partially completed core is ignored and all vertices are 
    // assumed to be unclaimed. 
    bool completionFilter() const;
            
private:
    // A vector of length getN() such that DVtx[i] is true if and only if the 
    // vertex i is claimed by Dominator. DVtx[i] is always false for vertices i
    // of degree less than 3.
    vector<bool> DVtx;

    // A vector of length getN() such that SVtx[i] is true if and only if the 
    // vertex i is claimed by Staller. SVtx[i] is always false for vertices i
    // of degree less than 3.
    vector<bool> SVtx;

    // A vector of length getN() with the following property. If the vertex i is 
    // claimed by Dominator or adjacent to a vertex claimed by Dominator, then 
    // gameStateDeg[i] = -1. Otherwise gameStateDeg[i] is the number of 
    // unclaimed vertices in the closed neighborhood of i in any completion of 
    // this partially completed core game state. Here the closed neighborhood is
    // the set of vertices adjacent to i together with i itself.
    vector<int> gameStateDeg;

    // A vector containing all the vertices of the underlying graph, that have 
    // degree strictly less than three, ordered such that for two vertices i and
    // j of degree at most 2 it holds that
    // - if gameStateDeg[i] != -1 and gameStateDeg[j] = -1, then i preceeds j,
    // - if -1 < gameStateDeg[i] < gameStateDeg[j] then i preceeds j.
    vector<int> lowDegVtx;

    // totalPot is the total ES potential of any completion of this partially 
    // completed core game state.
    double totalPot;

    // For an unclaimed vertex i, pot[i] measures by how much totalPot drops if 
    // Dominator claims vertex i in the next move. This is the same as the 
    // quantity by which totalPot increases if Staller claims vertex i in the 
    // next move. For claimed vertices j, pot[j] is undefined and can be 
    // anything.
    vector<double> pot;


    
    // A vector containing all unclaimed vertices of degree at least 3.
    vector<int> remVtx;
};

// Returns an unordered_set containing the canonically labeled graph6 strings 
// of all the graph6 strings in the input vector by calling nauty's labelg
unordered_set<string> labelCanonicalBatch(const vector<string>& graph6Vec);

// Requires the graph G corresponding to the input graph6 string to be a core. 
// If the degree sequence of every completion of G occurs in inputDegSequences 
// and Dominator wins on every completion of G it returns true. Otherwise it 
// returns false.
bool completionFilter(const string& graph6);

#endif // PART_COMPL_CORE_GAME_STATE_H
