#include "core_game_state.h"
#include <iostream>
#include <list>
#include <utility>
#include <algorithm>
#include <stdexcept>
#include <cmath>

CoreGameState::CoreGameState(const string& graph6) : Graph(graph6), DVtx(vector<bool>(getN(), false)),
                                 SVtx(vector<bool>(getN(), false)) {
    update();
}

void CoreGameState::addEdge(int u, int v) {
    int n = getN();
    if (u < 0 || v < 0 || u >= n || v >= n)
        throw out_of_range("addEdge: vertex index out of bounds");
    if (u == v)
        throw invalid_argument("addEdge: self-loops are not allowed");
    graph[u].insert(v);
    graph[v].insert(u);
    update();
}

bool CoreGameState::validCore() const {
    int total_deg_remaining = 0;
    for (auto& i : lowDegVtx) 
    total_deg_remaining += 3 - deg(i);
    return total_deg_remaining%2 == 0;
}

bool CoreGameState::isSWin() const {
    for (int i = 0; i < getN(); ++i) {
        if (gameStateDegSeq[i] == 0) {
            return true;
        }
    }
    return false;
}


int CoreGameState::out_lw_bnd(char firstPlayer) const {
    if (isSWin()) {
        return -2;
    }
    int out = -2;

    if (firstPlayer == 'D') {
        if (totalPot < 1)
            return -1;
        int new_out = -1;
        CoreGameState nextCoreGS = *this;
        for (const auto& i : remainingVtx) {
            nextCoreGS.DVtx[i] = true;
            nextCoreGS.update();
            new_out = nextCoreGS.out_lw_bnd('S');
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
        
            CoreGameState nextCoreGS = *this;
        for (const auto& i : lowDegVtx) {
            nextCoreGS.SVtx[i] = true; 
            nextCoreGS.update(); 
            if(nextCoreGS.out_lw_bnd_after_lowDegMove(i) == 'S') 
                return i;
            nextCoreGS.SVtx[i] = false;
        }
        for (const auto& i : remainingVtx) {
            nextCoreGS.SVtx[i] = true; 
            nextCoreGS.update(); 
            out = nextCoreGS.out_lw_bnd('D');
            if (out != -1)
                return out;
            nextCoreGS.SVtx[i] = false;
        }
        return -1;
    }
    cout << "firstPlayer has to be one of 'D' or 'S'!" << endl;
    return -2;
}

char CoreGameState::out_lw_bnd_after_lowDegMove(int vertex) const {
    int num_miss_edg = 3 - deg(vertex);
    double updated_total_pot = totalPot;
    int i = 0;
    int rem_edges = num_miss_edg;
    int poss_neigh;
    while(rem_edges > 0 && i < lowDegVtx.size()) {
        poss_neigh = lowDegVtx[i];
        if (!hasEdge(vertex, poss_neigh) && poss_neigh != vertex) {
            if (gameStateDegSeq[poss_neigh] != -1)
            updated_total_pot += pow(2,-gameStateDegSeq[poss_neigh]);
            --rem_edges;
        }
        ++i;
    }
    if (rem_edges > 0 || updated_total_pot < 1) 
        return 'D';
    return 'S';
}

bool CoreGameState::filter() const {
    if (!validCore()) 
        return true;
    if (out_lw_bnd('D') == -1) 
        return true;
    return false;
}


// does not check if core is valid, this is assumed to be done 
// with filter() first
bool CoreGameState::completion_filter() const {

    // for testing purposes ---------------------
    string test = toGraph6();
    int number_of_edges_to_add = 0;
    for (auto& i : lowDegVtx) {
        number_of_edges_to_add += 3 - deg(i);
    }
    number_of_edges_to_add /= 2;
    // end of for testing purposes --------------

    int a = out_lw_bnd('D');

    if (a == -1)
        return true;
    else if (lowDegVtx.size() == 0)
        return false;
    else if (a == -2)
        a = lowDegVtx[0];

    int b;
    for (int i = 0; i < lowDegVtx.size(); ++i) {
        b = lowDegVtx[i];
        if (!hasEdge(a, b) && b != a) {
            if (number_of_edges_to_add > 8) {
                cout << number_of_edges_to_add << " edges remaining, joining edge " << a << ", " << b << endl;
            }
            CoreGameState newCoreGameState(*this);
            newCoreGameState.addEdge(a, b);
            if(!newCoreGameState.completion_filter())
                return false;
        }
    }
    return true;
}

set<string> nextCompl(set<string> g6set) {
    set<string> out;
    string g6;

    for (string g6 : g6set) {
        CoreGameState coregs = CoreGameState(g6);
        int a = coregs.out_lw_bnd('D');

        if (a == -1)
            continue;
        else if (coregs.lowDegVtx.size() == 0) {
            cout << "Counterexample found!" << g6 << endl;
            return {"Counterexample:" + g6};
        }
        else if (a == -2)
            a = coregs.lowDegVtx[0];
    
        int b;
        for (int i = 0; i < coregs.lowDegVtx.size(); ++i) {
            b = coregs.lowDegVtx[i];
            if (!coregs.hasEdge(a, b) && b != a) {
                CoreGameState newCoreGS(coregs);
                newCoreGS.addEdge(a, b);
                out.insert(newCoreGS.toCanonicalGraph6());
            }
        }
    }
    return out;
}

bool CoreGameState::completion_filter2() const {
    set<string> g6set = {toCanonicalGraph6()};
    while(!g6set.empty()) {
        if ((*g6set.begin()).rfind("Counterexample:", 0) == 0)
            return false;
        g6set = nextCompl(g6set);
    }
    return true;
}


void CoreGameState::update() {
    int n = getN();
    gameStateDegSeq.assign(n, 0);
    totalPot = 0.;
    remainingVtx.clear();
    lowDegVtx.clear();

    // update gameStateDegSeq
    for (int i = 0; i < n; ++i) {
        if (DVtx[i]) {
            gameStateDegSeq[i] = -1;
            for (const auto& j : neighborhood(i)) 
            gameStateDegSeq[j] = -1;
        }
        else if (gameStateDegSeq[i] != -1) {
            if (!SVtx[i]) 
            ++gameStateDegSeq[i];

            for (const auto& j : neighborhood(i)) {
                if (!SVtx[j]) 
                ++gameStateDegSeq[i];
            }
            if (deg(i) < 3) {
                gameStateDegSeq[i] += 3 - deg(i);
            }
        }
    }

    for (int i = 0; i < n; ++i) {
        if (gameStateDegSeq[i] != -1)
        totalPot += pow(2,-gameStateDegSeq[i]);

        if (!DVtx[i] && !SVtx[i] && deg(i) >= 3) 
            remainingVtx.push_back(i);

        if (deg(i) < 3)
            lowDegVtx.push_back(i);
    }

    sort(remainingVtx.begin(), remainingVtx.end(), [&](auto left, auto right) {
        return deg(left) > deg(right);
    });
    sort(lowDegVtx.begin(), lowDegVtx.end(), [&](auto left, auto right) {
        if (gameStateDegSeq[right] == -1 && gameStateDegSeq[left] != -1) 
            return true;
        if (gameStateDegSeq[left] == -1 && gameStateDegSeq[right] != -1) 
            return false;
        if (gameStateDegSeq[left] != gameStateDegSeq[right])
            return gameStateDegSeq[left] < gameStateDegSeq[right];
        return deg(left) < deg(right);
    });
}
