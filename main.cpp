#include "GameState.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cmath>
#include <limits>
#include "core.h"
#include "RSI.h"


using namespace std;

int main() {
    GameState gs = GameState("EsP?");
    cout << gs.outcome('D') << endl;
    return 0;
}