#include "game_state.h"
#include <algorithm>
#include <cmath>
#include <iostream>

GameState::GameState(string graph6) : Graph(graph6), 
                                DVtx(vector<bool>(getN(), false)),
                                SVtx(vector<bool>(getN(), false)) {
    update();
}

void GameState::update() {
    int n = getN();
    totalPot = 0.;
    remVtx.clear();
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
        }
    }

    // update totalPot and the vectors pot and remVtx
    for (int i = 0; i < n; ++i) {
        if (gameStateDeg[i] != -1)
            totalPot += pow(2,-gameStateDeg[i]);

        if (!DVtx[i] && !SVtx[i]) {
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

    // order the unclaimed vertices in remVtx
    sort(remVtx.begin(), remVtx.end(), [&](int left, int right) {
        return pot[left] > pot[right];
    });
}

bool GameState::isSWin() {
    for (int i = 0; i < getN(); ++i) {
        if (gameStateDeg[i] == 0) {
            return true;
        }
    }
    return false;
}

char GameState::outcome(char firstPlayer) {
    if (firstPlayer != 'D' && firstPlayer != 'S') {
        throw invalid_argument("outcome: firstPlayer must be one of "
            "\'D\' or \'S\'");
        }

    if (isSWin()) {
        return 'S';
    }
    
    if (firstPlayer == 'D') {
        if (totalPot < 1.) {
            return 'D';
        }

        GameState nextGs = *this;
        for (int i : remVtx) {
            nextGs.DVtx[i] = true;
            nextGs.update();
            if (nextGs.outcome('S') == 'D') {
                return 'D';
            }
            nextGs.DVtx[i] = false;
        }
        return 'S';
    }
    else if (firstPlayer == 'S') {
        if (totalPot < 0.5) {
            return 'D';
        }
        GameState nextGs = *this;
        for (int i : remVtx) {
            nextGs.SVtx[i] = true;
            nextGs.update();
            if (nextGs.outcome('D') == 'S') {
                return 'S';
            }
            nextGs.SVtx[i] = false;
        }
        return 'D';
    }
    throw logic_error("outcome: Reached unreachable code");
}