#ifndef PCCGS_H
#define PCCGS_H

#include "graph.h"
#include <unordered_set>

// The minimum degree. 
inline int d = 3;

extern std::vector<std::vector<int>> inSeqs;

// Loads the degree sequences from the input file filename to inSeqs. 
// Expects the degree sequences in filename to come in separate lines, where 
// each line contains an increasingly sorted sequence of non-negative integers 
// separated by whitespaces.
void loadDegSeqs(const std::string& filename = "deg_seqs.txt");

class Pccgs : public Graph {
public:
    // Creates a game state on a PCC represented by a graph6 string, in which 
    // all the vertices are unclaimed. 
    Pccgs(const std::string& graph6);

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
    // PCCGS, where firstPlayer starts the game. If it returns -2 then Staller 
    // wins on some completion of this PCCGS, where firstPlayer starts the game.
    // If it returns neither -1 nor -2 it returns a low degree vertex (which we
    // want to use first for completion).
    char potentialOutcome(char firstPlayer) const;

    // Returns 'D' if Dominator wins on every completion of this PCC with 
    // firstPlayer starting the game. Otherwise it returns 'S'.
    friend char completionOutcome(const std::string& graph6, char firstPlayer);
            
private:
    // A vector of length getN() such that DVtx[i] is true if and only if the 
    // vertex i is claimed by Dominator. DVtx[i] is always false for vertices i
    // of degree less than d.
    std::vector<bool> DVtx;

    // A vector of length getN() such that SVtx[i] is true if and only if the 
    // vertex i is claimed by Staller. SVtx[i] is always false for vertices i
    // of degree less than d.
    std::vector<bool> SVtx;

    // A vector of length getN() with the following property. If the vertex i is 
    // claimed by Dominator or adjacent to a vertex claimed by Dominator, then 
    // gameStateDeg[i] = -1. Otherwise gameStateDeg[i] is the number of 
    // unclaimed vertices in the closed neighborhood of i in any completion of 
    // this PCCGS.
    std::vector<int> gameStateDeg;

    // A vector containing all the vertices of the underlying graph, that have 
    // degree strictly less than d, ordered such that for two vertices i and j
    // of degree less than d it holds that
    // - if gameStateDeg[i] != -1 and gameStateDeg[j] = -1, then i preceeds j,
    // - if -1 < gameStateDeg[i] < gameStateDeg[j] then i preceeds j.
    std::vector<int> lowDegVtx;

    // totalPot is the total ES potential of any completion of this PCCGS.
    double totalPot;

    // For an unclaimed vertex i, pot[i] measures by how much totalPot drops if 
    // Dominator claims vertex i in the next move. This is the same as the 
    // quantity by which totalPot increases if Staller claims vertex i in the 
    // next move. For claimed vertices j, pot[j] is undefined and can be 
    // anything.
    std::vector<double> pot;


    // A vector containing all unclaimed vertices of degree at least d.
    std::vector<int> remVtx;
};

// Returns an unordered_set containing the canonically labeled graph6 strings 
// of all the graph6 strings in the input vector by calling nauty's labelg
std::unordered_set<std::string> 
                labelCanonicalBatch(const std::vector<std::string>& graph6Vec);




// Returns a vector of all degree sequences with N1 <= N <= N2 entries, minimum 
// entry d and surplus at least surBound.
std::vector<std::vector<int>> generateSeqs(int N1, int N2, int d, int surBound);

// Requires the graph G corresponding to the input graph6 string to be a core. 
// If the degree sequence of every completion of G occurs in inSeqs and 
// Dominator wins on every completion of G it returns true. Otherwise it 
// returns false.
bool filter(const std::string& graph6);

#endif // PCCGS_H
