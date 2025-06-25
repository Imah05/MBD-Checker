#include "part_compl_core_game_state.h"
#include <iostream>
#include <list>
#include <utility>
#include <algorithm>
#include <stdexcept>
#include <cmath>

PartComplCoreGameState::PartComplCoreGameState(const string& graph6) : 
                            Graph(graph6), DVtx(vector<bool>(getN(), false)), 
                            SVtx(vector<bool>(getN(), false)) {
    update();
}

void PartComplCoreGameState::update() {
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
        if (gameStateDeg[left] != -1 && gameStateDeg[right] == -1) 
            return true;
        if (gameStateDeg[left] == -1 && gameStateDeg[right] != -1) 
            return false;
        if (gameStateDeg[left] != gameStateDeg[right])
            return gameStateDeg[left] < gameStateDeg[right];
        return deg(left) < deg(right);
    });
}

void PartComplCoreGameState::addEdge(int u, int v) {
    int n = getN();
    if (u < 0 || v < 0 || u >= n || v >= n) {
        throw out_of_range("addEdge: vertex index out of bounds");
    }
    if (u == v) {
        throw invalid_argument("addEdge: self-loops are not allowed");
    }
    DVtx = vector<bool>(n, false);
    SVtx = vector<bool>(n, false);
    graph[u].insert(v);
    graph[v].insert(u);
    update();
}

int PartComplCoreGameState::outcome(char firstPlayer) const {
    if (firstPlayer != 'D' && firstPlayer != 'S') {
        throw invalid_argument("outcome: firstPlayer must be one of "
            "\'D\' or \'S\'");
    }
    for (int i = 0; i < getN(); ++i) {
        if (gameStateDeg[i] == 0) {
            return -2;
        }
    }
    int out = -2;

    if (firstPlayer == 'D') {
        if (totalPot < 1)
            return -1;
        int new_out = -1;
        PartComplCoreGameState nextCoreGS = *this;
        for (const auto& i : remVtx) {
            nextCoreGS.DVtx[i] = true;
            nextCoreGS.update();
            new_out = nextCoreGS.outcome('S');
            if (new_out == -1)
                return -1;
            else if (out == -2) 
                out = new_out;
            nextCoreGS.DVtx[i] = false;
        }
        return out;
    }
    else if (firstPlayer == 'S') {
        if (totalPot < 0.5)
            return -1;
        
            PartComplCoreGameState nextCoreGS = *this;
        for (const auto& i : lowDegVtx) {
            nextCoreGS.SVtx[i] = true; 
            nextCoreGS.update(); 
            if(nextCoreGS.out_lw_bnd_after_lowDegMove(i) == 'S') 
                return i;
            nextCoreGS.SVtx[i] = false;
        }
        for (const auto& i : remVtx) {
            nextCoreGS.SVtx[i] = true; 
            nextCoreGS.update(); 
            out = nextCoreGS.outcome('D');
            if (out != -1)
                return out;
            nextCoreGS.SVtx[i] = false;
        }
        return -1;
    }
    cout << "firstPlayer has to be one of 'D' or 'S'!" << endl;
    return -2;
}

char PartComplCoreGameState::out_lw_bnd_after_lowDegMove(int vertex) const {
    int num_miss_edg = 3 - deg(vertex);
    double updated_total_pot = totalPot;
    int i = 0;
    int rem_edges = num_miss_edg;
    int poss_neigh;
    while(rem_edges > 0 && i < lowDegVtx.size()) {
        poss_neigh = lowDegVtx[i];
        if (!hasEdge(vertex, poss_neigh) && poss_neigh != vertex) {
            if (gameStateDeg[poss_neigh] != -1)
            updated_total_pot += pow(2,-gameStateDeg[poss_neigh]);
            --rem_edges;
        }
        ++i;
    }
    if (rem_edges > 0 || updated_total_pot < 1) 
        return 'D';
    return 'S';
}

bool PartComplCoreGameState::completionFilter() const {
    set<string> g6Set = {toCanonicalGraph6()};
    while(!g6Set.empty()) {
        set<string> newG6Set;
        string g6;

        for (string g6 : g6Set) {
            PartComplCoreGameState coreGs = PartComplCoreGameState(g6);
            int a = coreGs.outcome('D');
            if (a == -1) {
                continue;
            }
            else if (coreGs.lowDegVtx.size() == 0) {
                return false;
            }
            else if (a == -2) {
                a = coreGs.lowDegVtx[0];
            }
        
            int b;
            for (int i = 0; i < coreGs.lowDegVtx.size(); ++i) {
                b = coreGs.lowDegVtx[i];
                if (!coreGs.hasEdge(a, b) && b != a) {
                    PartComplCoreGameState newCoreGS(coreGs);
                    newCoreGS.addEdge(a, b);
                    newG6Set.insert(newCoreGS.toCanonicalGraph6());
                }
            }
        }
        g6Set = newG6Set;
    }
    return true;
}