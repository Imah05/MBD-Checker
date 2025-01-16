#include "Core.h"

Core::Core(Graph *g) : core(g), DVtx(vector<bool>(g->getNumVertices(), false)),
                                 SVtx(vector<bool>(g->getNumVertices(), false)) {
    update();
}

bool Core::filter(char firstPlayer) {
    if (firstPlayer == 'D') {
        if (totalPot < 1)
            return true;
        Core nextCoreGS = *this;
        for (const auto& i : remainingVtx) {
            nextCoreGS.DVtx[i] = true;
            nextCoreGS.update();
            if (nextCoreGS.filter('S'))
                return true;
            nextCoreGS.DVtx[i] = false;
        }
        return false;
    }
    else if (firstPlayer == 'S') {
        if (totalPot < 0.5)
            return true;
        for (const auto& i : lowDegVtx) {
            if(!this->filterAfterLowDegMove(i)) 
                return false;
        }
        Core nextCoreGS = *this;
        for (const auto& i : remainingVtx) {
            nextCoreGS.SVtx[i] = true;
            nextCoreGS.update();
            if (!nextCoreGS.filter('D'))
                return false;
            nextCoreGS.SVtx[i] = false;
        }
        return true;
    }
    cout << "firstPlayer has to be one of 'D' or 'S'!" << endl;
    return false;
}

bool Core::filterAfterLowDegMove(int vertex) {
    int r = 3 - core->neighborhood(vertex).size();
    if (lowDegVtx.size() < r + 1)
        return true;
    
    double addPot = 0.;
    bool vertexDone = false;
    int i = 0;
    int v;
    for (; i<r; ++i) {
        v = lowDegVtx[i];
        if (gameStateDegSeq[v] != -1)
            addPot += pow(2,-gameStateDegSeq[lowDegVtx[i]]);
        if (v == vertex)
            vertexDone = true;
    }
    if (vertexDone && gameStateDegSeq[lowDegVtx[i]] != -1)
        addPot += pow(2,-gameStateDegSeq[lowDegVtx[i]]);
    else if (gameStateDegSeq[vertex] != -1)
        addPot += pow(2,-gameStateDegSeq[vertex]);
    
    for (auto j : core->neighborhood(vertex)) {
        if (gameStateDegSeq[j] != -1)
            addPot += pow(2,-gameStateDegSeq[j]);
    }
    if (totalPot + addPot < 1) 
        return true;
    return false;
}

void Core::update() {
    int n = core->getNumVertices();
    gameStateDegSeq.assign(n, 0);
    totalPot = 0.;
    remainingVtx.clear();
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
                lowDegVtx.push_back(i);
            }
        }
    }

    for (int i = 0; i < n; ++i) {
        if (gameStateDegSeq[i] != -1)
        totalPot += pow(2,-gameStateDegSeq[i]);

        if (!DVtx[i] && !SVtx[i] && core->neighborhood(i).size() >= 3) 
        remainingVtx.push_back(i);
    }

    sort(remainingVtx.begin(), remainingVtx.end(), [&](auto left, auto right) {
        return core->neighborhood(left).size() > core->neighborhood(right).size();
    });
     sort(lowDegVtx.begin(), lowDegVtx.end(), [&](auto left, auto right) {
        if (gameStateDegSeq[right] == -1 && gameStateDegSeq[left] != -1) 
            return true;
        if (gameStateDegSeq[left] == -1 && gameStateDegSeq[right] != -1) 
            return false;
        return gameStateDegSeq[left] < gameStateDegSeq[right];
    });
}