#include "small_graphs.h"
#include <iostream>
#include <cstring>

bool checkGraph(string cmd, char firstPlayer) {
    FILE *pipe = popen(cmd.c_str(), "r");
    if (!popen) {
        pclose(pipe);
        throw runtime_error("checkGraph: popen failed");
    }
    char buffer[128];
    string graph6;
    
    while(fgets(buffer, sizeof(buffer), pipe)) {
        buffer[strcspn(buffer, "\n")] = 0;
        graph6 = buffer;
        GameState gameState(graph6);
        if (gameState.outcome(firstPlayer) == 'S') {
            return false;
        }
    }
    pclose(pipe);
    return true;
}

list<string> checkR(string cmd, char firstPlayer, int surBound) {
    list<string> out;
    FILE *pipe = popen(cmd.c_str(), "r");
    if (!popen) {
        pclose(pipe);
        throw runtime_error("checkR: popen failed");
    }
    char buffer[128];
    string graph6;
    
    while(fgets(buffer, sizeof(buffer), pipe)) {
        buffer[strcspn(buffer, "\n")] = 0;
        graph6 = buffer;

        GameState gameState(graph6);
        int maxDeg = 0;
        int maxDegVtx;
        for(int i = 0; i < gameState.getN(); i++) {
            if (gameState.deg(i) > maxDeg) {
                maxDeg = gameState.deg(i);
                maxDegVtx = i;
            }
        }
        int sur = 0;
        for(int i = 0; i < gameState.getN(); i++) {
            if (i != maxDegVtx) 
                sur += (gameState.deg(i) > 3 ? gameState.deg(i) - 3 : 
                                                        3 - gameState.deg(i));
        }
        if (sur <= surBound && gameState.outcome(firstPlayer) == 'S') {
            cout << "Exceptional graph found in checkR: " << graph6 << endl;
            out.push_back(graph6);
        }
    }
    pclose(pipe);
    return out;
}

bool checkSI(string cmd, char firstPlayer, int surBound) {
    FILE *pipe = popen(cmd.c_str(), "r");
    if (!popen) {
        pclose(pipe);
        throw runtime_error("checkSI: popen failed");
    }
    char buffer[128];
    string graph6;

    while (fgets(buffer, sizeof(buffer), pipe)) {
        buffer[strcspn(buffer, "\n")] = 0;
        graph6 = buffer;

        GameState gameState(graph6);
        int numOfDeg2Vtx = 0;
        for(int i = 0; i < gameState.getN(); i++) {
            if (gameState.deg(i) == 2)
                ++numOfDeg2Vtx;
        }
        if (numOfDeg2Vtx <= surBound && gameState.outcome(firstPlayer) == 'S') {
            cout << "Exceptional graph found in checkSI: " << graph6 << endl;
            return false;
        }
    }
    pclose(pipe);
    return true;
}

bool checkLemmaR2() {
    if (checkR("geng -d1 5", 'S', 5).size() > 3)
        return false;
    if (checkR("geng -d1 6", 'S', 5).size() > 3)
        return false;
    cout << "Lemma #2 in the R-Lemma table checked successfuly" << endl;
    return true;
}

bool checkLemmaR3() {
    if(!checkR("geng -d1 4", 'S', 2).empty())
        return false;
    if(!checkR("geng -d1 5", 'S', 2).empty())
        return false;
    if(!checkR("geng -d1 6", 'S', 2).empty())
        return false;
    if(!checkR("geng -d1 7", 'S', 2).empty())
        return false;    
    cout << "Lemma #3 in the R-Lemma table checked successfuly" << endl;
    return true;
}

bool checkLemmaR4() {
    if(!checkR("geng -d1D3 9 11:11", 'D', 5).empty())
        return false;
    if(!checkR("geng -d1D6 10 13:14", 'D', 5).empty())
        return false;
    if(!checkR("geng -d1D8 11 14:17", 'D', 5).empty())
        return false;
    if(!checkR("geng -d1D9 12 16:19", 'D', 5).empty())
        return false;    
    cout << "Lemma #4 in the R-Lemma table checked successfuly" << endl;
    return true;
}

bool checkLemmaR6() {
    if (!checkR("geng -d2D9 13 17:20", 'D', 5).empty())
        return false;
    if (!checkR("geng -d2D9 14 19:22", 'D', 5).empty())
        return false;
    cout << "Lemma #6 in the R-Lemma table checked successfuly" << endl;
    return true;
}

bool checkLemmaSI1() {
    if (!checkSI("genbg -d2:3 -D7:3 6 7", 'S', 5))
        return false;
    if (!checkSI("genbg -d2:3 -D6:3 5 6", 'S', 5))
        return false;
    if (!checkSI("genbg -d2:3 -D5:3 4 5", 'S', 5))
        return false;
    if (!checkSI("genbg -d2:3 -D4:3 3 4", 'S', 5))
        return false;
    cout << "Lemma #1 in the SI-Lemma table checked successfuly" << endl;
    return true;
}

bool checkLemmaSI2() {
    if (!checkSI("genbg -d2:2 -D9:3 7 9 26:26", 'D', 4))
        return false;
    if (!checkSI("genbg -d2:2 -D8:3 6 8 23:23", 'D', 4))
        return false;
    if (!checkSI("genbg -d2:2 -D7:3 5 7 20:20", 'D', 4))
        return false;
    if (!checkSI("genbg -d2:2 -D6:3 4 6 17:17", 'D', 4))
        return false;
    if (!checkSI("genbg -d2:2 -D5:3 3 5 14:14", 'D', 4))
        return false;
    cout << "Lemma #2 in the SI-Lemma table checked successfuly" << endl;
    return true;
}

bool checkLemmaSI3() {
    if (!checkSI("genbg -d2:3 -D8:3 7 8", 'D', 5))
        return false;
    cout << "Lemma #3 in the SI-Lemma table checked successfuly" << endl;
    return true;
}

bool checkLemmaSI4() {
    if (!checkSI("genbg -d2:3 -D8:3 8 9", 'D', 5))
        return false;
    cout << "Lemma #4 in the SI-Lemma table checked successfuly" << endl;
    return true;
}

bool checkLemmaSI5() {
    if (!checkSI("genbg -d2:3 -D4:3 9 10", 'D', 5))
        return false;
    cout << "Lemma #5 in the SI-Lemma table checked successfuly" << endl;
    return true;
}

bool checkLemmaSI6() {
    if (!checkSI("genbg -d2:3 -D7:3 5 7", 'S', 2))
        return false;
    if (!checkSI("genbg -d2:3 -D6:3 4 6", 'D', 2))
        return false;
    if (!checkSI("genbg -d2:3 -D5:3 3 5", 'D', 2))
        return false;
    cout << "Lemma #6 in the SI-Lemma table checked successfuly" << endl;
    return true;
}

bool checkLemmaSI7() {
    if (checkSI("genbg -d2:3 -D9:3 7 9", 'D', 2))
        return false;
    if (checkSI("genbg -d2:3 -D8:3 6 8", 'D', 2))
        return false;
    cout << "Lemma #7 in the SI-Lemma table checked successfuly" << endl;
    return true;
}