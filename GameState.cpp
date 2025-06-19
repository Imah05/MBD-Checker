#include "GameState.h"
#include "graph.h"

GameState::GameState(string graph6) : Graph(graph6), DVtx(vector<bool>(getNumVertices(), false)),
                                 SVtx(vector<bool>(getNumVertices(), false)) {
    update();
}


// updates gameStatDegSeq, totalPot, potSeq and remainingVtx so that they 
// again match DVtx and SVtx.
void GameState::update() {
    int n = getNumVertices();
    gameStateDegSeq.assign(n, 0);
    potSeq.assign(n, 0.);
    totalPot = 0.;
    remainingVtx.clear();
    for (int i = 0; i < n; ++i) {
        if (DVtx[i]) {
            gameStateDegSeq[i] = -1;
            for (const auto& j : neighborhood(i)) {
                gameStateDegSeq[j] = -1;
            }
        }
        else if (gameStateDegSeq[i] != -1) {
            if (!SVtx[i]) {
                ++gameStateDegSeq[i];
            }
            for (const auto& j : neighborhood(i)) {
                if (!SVtx[j]) {
                    ++gameStateDegSeq[i];
                }
            }
        }
    }

    for (int i = 0; i < n; ++i) {
        if (gameStateDegSeq[i] != -1) {
            totalPot += pow(2,-gameStateDegSeq[i]);
        }
        if (!DVtx[i] && !SVtx[i]) {
            for (const auto& j : neighborhood(i)) {
                if (gameStateDegSeq[j] != -1) {
                    potSeq[i] += pow(2,-gameStateDegSeq[j]);
                }
            }
            if (gameStateDegSeq[i] != -1) {
                potSeq[i] += pow(2,-gameStateDegSeq[i]);
            }
            remainingVtx.push_back(i);
        }
        else {
            potSeq[i] = -1.;
        }
    }

    sort(remainingVtx.begin(), remainingVtx.end(), [&](auto left, auto right) {
        return potSeq[left] > potSeq[right];
    });
}


bool GameState::isSWin() {
    for (int i = 0; i < getNumVertices(); ++i) {
        if (gameStateDegSeq[i] == 0) {
            return true;
        }
    }
    return false;
}

char GameState::outcome(char firstPlayer) {
    if (isSWin()) {
        return 'S';
    }
    if (firstPlayer == 'D') {
        if (totalPot < 1) {
            return 'D';
        }
        GameState nextgs = *this;
        for (const auto& i : remainingVtx) {
            nextgs.DVtx[i] = true;
            nextgs.update();
            if (nextgs.outcome('S') == 'D') {
                return 'D';
            }
            nextgs.DVtx[i] = false;
        }
        return 'S';
    }
    else if (firstPlayer == 'S') {
        if (totalPot < 0.5) {
            return 'D';
        }
        GameState nextgs = *this;
        for (const auto& i : remainingVtx) {
            nextgs.SVtx[i] = true;
            nextgs.update();
            if (nextgs.outcome('D') == 'S') {
                return 'S';
            }
            nextgs.SVtx[i] = false;
        }
        return 'D';
    }
    return 0;
}