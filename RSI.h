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



pair<int, int> filter_R(int sur, int numVtxInR, char flag);

int filter_SI(int sur, int numVtxInS, int numVtxInI, char flag, int remainingSur); 


#endif // RSI_H