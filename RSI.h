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

using namespace std;

bool havel_hakimi(deque<int> degreeSequence);

// IN THE FOLLOWING:
// flag = 'D' means we are not interested in graphs on which 
// Dominator wins going first
// flag = 'N' --//-- second
// flag = 'S' means we are a priori interested in all graphs



char filter_R(int sur, int numVtxInR, char flag);


#endif // RSI_H