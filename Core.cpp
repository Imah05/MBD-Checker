#include "Core.h"

Core::Core(Graph *g) : core(g), DVtx(vector<bool>(g->getNumVertices(), false)),
                                 SVtx(vector<bool>(g->getNumVertices(), false)) {
    highDegVtx = vector<bool>(g->getNumVertices(), false);
    for (int i = 0; i < g->getNumVertices(); i++) {
        if(g->neighborhood(i).size() > mindeg) {
            highDegVtx[i] = true;
        }
    }
    sort(remainingVtx.begin(), remainingVtx.end(), [&](auto left, auto right) {
        return g->neighborhood(left).size() > g->neighborhood(right).size();
    });
    update();
}

void Core::update() {
    int n = core->getNumVertices();
    worstDegSeq.assign(n, 0);
    upperBoundTotalPot = 0.;
}

bool Core::couldBeSWin() {
    return false;
}

char Core::lowerBoundOutcome(char firstPlayer, int remainingGameTreeDepth) {
    return 0;
}
