#include "Core.h"

Core::Core(Graph *g) : core(g), DVtx(vector<bool>(g->getNumVertices(), false)),
                                 SVtx(vector<bool>(g->getNumVertices(), false)) {
    update();
}

bool Core::validCore() {
    int total_deg_remaining = 0;
    for (auto& i : lowDegVtx) 
    total_deg_remaining += 3 - core->neighborhood(i).size();
    return total_deg_remaining%2 == 0;
}

bool Core::isSWin() {
    for (int i = 0; i < core->getNumVertices(); ++i) {
        if (gameStateDegSeq[i] == 0) {
            return true;
        }
    }
    return false;
}

int Core::out_lw_bnd(char firstPlayer) {
    if (isSWin()) {
        return -2;
    }
    int out = -2;

    if (firstPlayer == 'D') {
        if (totalPot < 1)
            return -1;
        int new_out = -1;
        Core nextCoreGS = *this;
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
        
        Core nextCoreGS = *this;
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

char Core::out_lw_bnd_after_lowDegMove(int vertex) {
    int num_miss_edg = 3 - core->neighborhood(vertex).size();
    int i = 0;
    int rem_edges = num_miss_edg;
    int poss_neigh;
    while(rem_edges > 0 && i < lowDegVtx.size()) {
        poss_neigh = lowDegVtx[i];
        if (!core->neighborhood(vertex).count(poss_neigh) && poss_neigh != vertex) {
            if (gameStateDegSeq[poss_neigh] != -1)
            totalPot += pow(2,-gameStateDegSeq[poss_neigh]);
            --rem_edges;
        }
        ++i;
    }
    if (rem_edges > 0 || totalPot < 1) 
        return 'D';
    return 'S';
}

bool Core::filter() {
    if (!validCore()) 
    return true;
    if (out_lw_bnd('D') == -1) 
    return true;
    return false;
}


// does not check if core is valid, this is assumed to be done 
// with filter() first
bool Core::completion_filter() {
    string test = core->toGraph6();
    int number_of_edges_to_add = 0;
    for (auto& i : lowDegVtx) {
        number_of_edges_to_add += 3 - core->neighborhood(i).size();
    }
    number_of_edges_to_add /= 2;

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
        if (!core->neighborhood(a).count(b) && b != a) {
            if (number_of_edges_to_add > 8) {
                cout << number_of_edges_to_add << " edges remaining, joining edge " << a << ", " << b << endl;
            }
            if(!Core(&core->addEdge(a, b)).completion_filter())
            return false;
        }
    }
    return true;
}

set<string> nextCompl(set<string> g6set) {
    set<string> out;
    string g6;

    for (string g6 : g6set) {
        Graph g = Graph(g6);
        Core core = Core(&g);
        int a = core.out_lw_bnd('D');

        if (a == -1)
            continue;
        else if (core.lowDegVtx.size() == 0) {
            cout << "Counterexample found!" << g6 << endl;
            return {"Counterexample:" + g6};
        }
        else if (a == -2)
            a = core.lowDegVtx[0];
    
        int b;
        for (int i = 0; i < core.lowDegVtx.size(); ++i) {
            b = core.lowDegVtx[i];
            if (!core.core->neighborhood(a).count(b) && b != a)
                out.insert((&core.core->addEdge(a, b))->toCanonicalGraph6());
        }
    }
    return out;
}

bool Core::completion_filter2() {
    set<string> g6set = {core->toCanonicalGraph6()};
    while(!g6set.empty()) {
        if ((*g6set.begin()).rfind("Counterexample:", 0) == 0)
            return false;
        g6set = nextCompl(g6set);
    }
    return true;
}


void Core::update() {
    int n = core->getNumVertices();
    gameStateDegSeq.assign(n, 0);
    totalPot = 0.;
    remainingVtx.clear();
    lowDegVtx.clear();

    // update gameStateDegSeq
    for (int i = 0; i < n; ++i) {
        if (DVtx[i]) {
            gameStateDegSeq[i] = -1;
            for (const auto& j : core->neighborhood(i)) 
            gameStateDegSeq[j] = -1;
        }
        else if (gameStateDegSeq[i] != -1) {
            if (!SVtx[i]) 
            ++gameStateDegSeq[i];

            for (const auto& j : core->neighborhood(i)) {
                if (!SVtx[j]) 
                ++gameStateDegSeq[i];
            }
            if (core->neighborhood(i).size() < 3) {
                gameStateDegSeq[i] += 3 - core->neighborhood(i).size();
            }
        }
    }

    for (int i = 0; i < n; ++i) {
        if (gameStateDegSeq[i] != -1)
        totalPot += pow(2,-gameStateDegSeq[i]);

        if (!DVtx[i] && !SVtx[i] && core->neighborhood(i).size() >= 3) 
            remainingVtx.push_back(i);

        if (core->neighborhood(i).size() < 3)
            lowDegVtx.push_back(i);
    }

    sort(remainingVtx.begin(), remainingVtx.end(), [&](auto left, auto right) {
        return core->neighborhood(left).size() > core->neighborhood(right).size();
    });
    sort(lowDegVtx.begin(), lowDegVtx.end(), [&](auto left, auto right) {
        if (gameStateDegSeq[right] == -1 && gameStateDegSeq[left] != -1) 
            return true;
        if (gameStateDegSeq[left] == -1 && gameStateDegSeq[right] != -1) 
            return false;
        if (gameStateDegSeq[left] != gameStateDegSeq[right])
            return gameStateDegSeq[left] < gameStateDegSeq[right];
        return core->neighborhood(left).size() < core->neighborhood(right).size();
    });
}
