#include "core_game_state.h"
#include <iostream>
#include <list>
#include <utility>
#include <algorithm>
#include <stdexcept>
#include <cmath>
#include <fstream>
#include <cstring>

CoreGSTest::CoreGSTest(const string& graph6) : 
                            Graph(graph6), DVtx(vector<bool>(getN(), false)), 
                            SVtx(vector<bool>(getN(), false)) {
    update();
}

void CoreGSTest::update() {
    int n = getN();
    totalPot = 0.;
    remVtx.clear();
    lowDegVtx.clear();
    gameStateDeg.assign(n, 0);
    pot.assign(n, 0.);

    // set gameStateDeg[i] = -1 for dominated vertices i
    for (int i = 0; i < n; ++i) {
        if (DVtx[i]) {
            gameStateDeg[i] = -1;
            for (int j : neighborhood(i)) {
                gameStateDeg[j] = -1;
            }
        }
    }

    // determine gameStateDeg[i] for undominated vertices i
    for (int i = 0; i < n; ++i) {
        if (gameStateDeg[i] != -1) {
            if (!SVtx[i]) {
                ++gameStateDeg[i];
            }
            for (int j : neighborhood(i)) {
                if (!SVtx[j]) {
                    ++gameStateDeg[i];
                }
            }
            if (deg(i) < 3) {
                gameStateDeg[i] += 3 - deg(i);
            }
        }
    }

    for (int i = 0; i < n; ++i) {
        if (gameStateDeg[i] != -1) {
            totalPot += pow(2,-gameStateDeg[i]);
        }
        if (deg(i) < 3) {
            lowDegVtx.push_back(i);
        }
        else if (!DVtx[i] && !SVtx[i]) {
            if (gameStateDeg[i] != -1) {
                pot[i] += pow(2, -gameStateDeg[i]);
            }
            for (int j : neighborhood(i)) {
                if (gameStateDeg[j] != -1) {
                    pot[i] += pow(2, -gameStateDeg[j]);
                }
            }
            remVtx.push_back(i);
        }
    }

    sort(remVtx.begin(), remVtx.end(), [&](auto left, auto right) {
        return pot[left] > pot[right];
    });
    sort(lowDegVtx.begin(), lowDegVtx.end(), [&](auto left, auto right) {
        if (deg(left) < deg(right)) {
            return true;
        }
        if (gameStateDeg[left] != -1 && gameStateDeg[right] == -1) 
            return true;
        if (gameStateDeg[left] == -1 && gameStateDeg[right] != -1) 
            return false;
        return gameStateDeg[left] < gameStateDeg[right];
    });
}

void CoreGSTest::addEdge(int u, int v) {
    int n = getN();
    if (u < 0 || v < 0 || u >= n || v >= n) {
        throw out_of_range("addEdge: vertex index out of bounds");
    }
    if (u == v) {
        throw invalid_argument("addEdge: self-loops are not allowed");
    }
    graph[u].insert(v);
    graph[v].insert(u);
    update();
}

void CoreGSTest::removeEdge(int u, int v) {
    if (hasEdge(u, v)) {
        graph[u].erase(v);
        graph[v].erase(u);
        update();
    }
}

int CoreGSTest::outcome(char firstPlayer) const {
    if (firstPlayer != 'D' && firstPlayer != 'S') {
        throw invalid_argument("outcome: firstPlayer must be one of "
            "\'D\' or \'S\'");
    }
    for (int i = 0; i < getN(); ++i) {
        if (gameStateDeg[i] == 0) {
            return 'S';
        }
    }

    if (firstPlayer == 'D') {
        if (totalPot < 1) {
            return -1;
        }
        CoreGSTest nextCoreGS = *this;
        for (int i : remVtx) {
            nextCoreGS.DVtx[i] = true;
            nextCoreGS.update();
            if (nextCoreGS.outcome('S') == 'D') {
                return 'D';
            }
            nextCoreGS.DVtx[i] = false;
        }
        for (int i : lowDegVtx) {
            nextCoreGS.DVtx[i] = true;
            vector<int> b;
            int remEdges = 3 - deg(i);
            for (int j = 0; j < remEdges; ++j) {
                b.push_back(j);
            }
            while (b[0] != lowDegVtx.size() - remEdges) {
                int j = remEdges - 1;
                while (j >= 0) {
                    if (b[j] < lowDegVtx.size() - remEdges + j - 1) {
                        b[j]++;
                        nextCoreGS.addEdge()
                    }
                    --j;
                }
            }
        }
    }
    else if (firstPlayer == 'S') {
        if (totalPot < 0.5) {
            return -1;
        }
        
        CoreGSTest nextCoreGS = *this;
        for (int i : lowDegVtx) {
            int missingEdges = 3 - deg(i);
            double newTotalPot = totalPot + pow(2, -gameStateDeg[i]);
            int j = 0;
            while (missingEdges > 0 && j < lowDegVtx.size()) {
                int possNeigh = lowDegVtx[j];
                if (!hasEdge(i, possNeigh) && i != possNeigh) {
                    if (gameStateDeg[possNeigh] != -1) {
                        newTotalPot += pow(2, -gameStateDeg[possNeigh]);
                    }
                    missingEdges--;
                }
                ++j;
            }
            if (missingEdges == 0 && newTotalPot >= 1) {
                return i;
            }
        }
        for (int i : remVtx) {
            nextCoreGS.SVtx[i] = true; 
            nextCoreGS.update(); 
            int out = nextCoreGS.outcome('D');
            if (out != -1)
                return out;
            nextCoreGS.SVtx[i] = false;
        }
        return -1;
    }
}

set<string> labelCanonicalBatch(const vector<string>& graph6Vec) {
    const string inputFile = "temp_input.g6";
    const string command = "labelg -q " + inputFile;
    ofstream tempIn(inputFile);
    if (!tempIn) {
        throw runtime_error("labelCanonicalBatch: failed to open temp_input.g6 for writing");
    }
    for (string g6 : graph6Vec) {
        tempIn << g6 << '\n';
    }
    tempIn.close();
    FILE* fp = popen(command.c_str(), "r");
    if (!fp) {
        throw runtime_error("labelCanonicalBatch: popen failed");
    }
    set<string> result;
    char buff[100];
    while (fgets(buff, sizeof(buff), fp)) {
        buff[strcspn(buff, "\n")] = 0;
        result.insert(string(buff));
    }
    pclose(fp);
    return result;
}

bool CoreGSTest::completionFilter() const {
    set<string> g6Set = {toCanonicalGraph6()};
    while(!g6Set.empty()) {
        vector<string> newG6Vec;
        for (string g6 : g6Set) {
            CoreGSTest coreGs = CoreGSTest(g6);
            int a = coreGs.outcome('D');
            if (a == -1) {
                continue;
            }
            else if (a == -2) {
                return false;
            }
        
            int b;
            for (int i = 0; i < coreGs.lowDegVtx.size(); ++i) {
                b = coreGs.lowDegVtx[i];
                if (!coreGs.hasEdge(a, b) && b != a) {
                    CoreGSTest newCoreGS(coreGs);
                    newCoreGS.addEdge(a, b);


                    for (int a = 0; a < getN(); ++a) {
                        if (newCoreGS.deg(a) > 3) {
                            for (int b : newCoreGS.neighborhood(a)) {
                                if (newCoreGS.deg(b) > 3) {
                                    cout << "debug error: core with adjacent highdegvtx";
                                }
                            }
                        }
                    }
                    newG6Vec.push_back(newCoreGS.toGraph6());
                }
            }
        }
        g6Set = labelCanonicalBatch(newG6Vec);
    }
    return true;
}