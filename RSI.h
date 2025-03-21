#ifndef RSI_H
#define RSI_H

#include <iostream>
#include <fstream>
#include <cstdlib>
#include <sstream>
#include <string>
#include <deque>
#include <algorithm>
#include <functional>
#include <numeric>
#include "GameState.h"   
#include <stdexcept>
#include <utility>

using namespace std;

bool havel_hakimi(deque<int> degreeSequence);

// IN THE FOLLOWING:
// flag = 'D' means we are not interested in graphs on which 
// Dominator wins going first
// flag = 'N' --//-- second
// flag = 'S' means we are a priori interested in all graphs


// filters whole document of graphs
pair<int, int> filter_R(int sur, int numVtxInR, char flag);

// filters single graph (lazy)
// returns true if there are more than 3 vertices of degree 2 in S
// or if Dom wins on the graph (with firstPlayer going first)
// ASSUMES: g6 to represent a bipartite SI, where |I|=|S|+1, all vtx in 
// I having deg 3 and all vtx in S having degree at least 2. 
bool filter_SIsur6(string g6, char firstPlayer);

// int filter_SI(int sur, int numVtxInS, int numVtxInI, char flag, int remainingSur); 


#endif // RSI_H