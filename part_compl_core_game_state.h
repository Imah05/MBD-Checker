#ifndef PART_COMPL_CORE_GAME_STATE_H
#define PART_COMPL_CORE_GAME_STATE_H

#include "graph.h"
#include <unordered_set>

// A vector containing all the degree sequences of the graphs we want to check.
extern vector<vector<int>> inputDegSequences;

// Loads the degree sequences from the input file filename to inputDegSequences. 
// Expects the degree sequences in filename to come in separate lines, where 
// each line contains an increasingly sorted sequence of integers separated by 
// whitespaces.
void loadInputSequences(const string& filename = "input_sequences.txt");

class PartComplCoreGameState : public Graph {
public:
    // Creates a game state on a partially completed core represented by a 
    // graph6 string, in which all the vertices are unclaimed. 
    PartComplCoreGameState(const string& graph6);

    // Updates the vectors gameStateDeg, pot, lowDegVtx and remVtx as well as the double 
    // totalPot so that they match their description from below. The 
    // graph itself as well as the vertices claimed by the two players, that is
    // the two vectors DVtx and SVtx remain unchanged.
    void update();

    // Adds the edge uv to the underlying graph.
    void addEdge(int u, int v);
    
    // Removes the edge uv to the underlying graph.
    void removeEdge(int u, int v);

    // returns -1 if Dominator wins on every completion of this half completed core
    // returns the low deg vtx v if we do not a priori know that Dominator 
    // wins and where v is a 'dangerous' vertex we want to use first when 
    // completing this halfcompleted core
    // returns -2 if Staller wins on a completion of this. 
    int outcome(char firstPlayer) const;

    // returns true if Dominator wins on every completion of this and false
    // otherwise.
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
    // gameStateDeg[i] = -1. Otherwise gameStateDeg[i] is the number of unclaimed vertices in the closed neighborhood of 
    // i (the neighborhood of i including i itself).
    vector<int> gameStateDeg;

    // A vector containing all the vertices of the underlying graph, that have 
    // degree strictly less than three, ordered such that for two vertices i and
    // j of degree at most 2 it holds that
    // - if gameStateDeg[i] != -1 and gameStateDeg[j] = -1, then i preceeds j,
    // - if -1 < gameStateDeg[i] < gameStateDeg[j] then i preceeds j,
    // - if gameStateDeg[i] = gameStateDeg[j] and deg(i) < deg(j), then i
    //   preceeds j.
    vector<int> lowDegVtx;

    // For an unclaimed vertex i, pot[i] measures by how much the overall 
    // Erdős-Selfridge potential drops if Dominator claims vertex i in the next
    // move. This is the same as the quantity by which the overall potential 
    // increases if Staller claims vertex i in the next move. For claimed 
    // vertices j, pot[j] is undefined and can be anything.
    vector<double> pot;

    // totalPot is the total ES potential in any completion of this.
    double totalPot;
    
    // A vector containing all unlcaimed vertices of degree at least 3. 
    vector<int> remVtx;
};

unordered_set<string> labelCanonicalBatch(const vector<string>& graph6Vec);

bool completionFilter(const string&);

// bool checkFile(const string& inFileName, int start = 1, int end = -1);

#endif // PART_COMPL_CORE_GAME_STATE_H
