#include "part_compl_core_game_state.h"
#include <iostream>
#include <list>
#include <utility>
#include <algorithm>
#include <stdexcept>
#include <cmath>
#include <fstream>
#include <cstring>
#include <chrono>
#include <ctime>
#include <iomanip>
#include <numeric>
#include <sstream>
#include <cstdio>
#include <thread>

using namespace std;
using namespace std::chrono;

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
    graph[u].insert(v);
    graph[v].insert(u);
}

void PartComplCoreGameState::removeEdge(int u, int v) {
    if (hasEdge(u, v)) {
        graph[u].erase(v);
        graph[v].erase(u);
    }
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

    if (firstPlayer == 'D') {
        if (totalPot < 1) {
            return -1;
        }
        int out = -2;
        int newOut;
        PartComplCoreGameState nextCoreGS = *this;
        for (int i : remVtx) {
            nextCoreGS.DVtx[i] = true;
            nextCoreGS.update();
            newOut = nextCoreGS.outcome('S');
            if (newOut == -1) {
                return -1;
            }
            else if (out == -2) {
                out = newOut;
            }
            nextCoreGS.DVtx[i] = false;
        }
        if (out != -2) {
            return out;
        }
        else if (lowDegVtx.size() > 0) {
            return lowDegVtx[0];
        }
        else {
            return -2;
        }
    }
    else if (firstPlayer == 'S') {
        if (totalPot < 0.5) {
            return -1;
        }
        
        PartComplCoreGameState nextCoreGS = *this;
        for (int i : lowDegVtx) {
            double newTotalPot = totalPot;
            if (gameStateDeg[i] != -1) {
                newTotalPot += pow(2, -gameStateDeg[i]);
            }
            for (int j : neighborhood(i)) {
                if (gameStateDeg[j] != -1) {
                    newTotalPot += pow(2, -gameStateDeg[j]);
                }
            }
            int missingEdges = 3 - deg(i);
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
            if (out != -1) {
                return out;
            }
            nextCoreGS.SVtx[i] = false;
        }
        return -1;
    }
}


set<string> labelCanonicalBatch(const vector<string>& graph6Vec) {
    string inputFile = "temp_input.g6";
    string command = "nauty-labelg -q " + inputFile;
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

bool PartComplCoreGameState::completionFilter() const {
    set<string> g6Set = {toCanonicalGraph6()};
    while(!g6Set.empty()) {
        vector<string> newG6Vec;
        for (string g6 : g6Set) {
            PartComplCoreGameState coreGs = PartComplCoreGameState(g6);
            int a = coreGs.outcome('D');
            if (a == -1) {
                continue;
            }
            else if (a == -2) {
                return false;
            }
            PartComplCoreGameState newCoreGS(coreGs);
            for (int b : lowDegVtx) {
                if (!coreGs.hasEdge(a, b) && b != a) {
                    newCoreGS.addEdge(a, b);
                    newG6Vec.push_back(newCoreGS.toGraph6());
                    newCoreGS.removeEdge(a, b);
                }
            }
        }
        g6Set = labelCanonicalBatch(newG6Vec);
    }
    return true;
}

bool completionCheckDegSeq(int n, vector<int> seq) {
    sort(seq.begin(), seq.end());
    int numberOfEdges = 0;
    for (int i : seq) {
        numberOfEdges += i;
    }
    ostringstream cmdS;
    cmdS << "nauty-genbg -d" << seq[0] << ":0 -D" << seq.back() << ":3 " << seq.size()
    << " " << n - seq.size() << " " << numberOfEdges << ":" << numberOfEdges;
    string cmd = cmdS.str();

    FILE *pipe = popen(cmd.c_str(), "r");
    if (!pipe) {
        throw runtime_error("completionCheckDegSeq: popen failed");
    }
    
    auto startTime = steady_clock::now();
    auto lastReport = startTime;
    const auto maxDuration = minutes(6000);
    const auto reportInterval = minutes(30);
    int totalCounter = 0, under100ms = 0, under2s = 0, under30s = 0, 
        under2min = 0, under6min = 0, under30min = 0, over30min = 0;

    char buffer[128];
    while (fgets(buffer, sizeof(buffer), pipe)) {
        auto t0 = steady_clock::now();
        if (t0 - startTime > maxDuration) {
            pclose(pipe);
            cout << "time out: completionCheckDegSeq terminated for n = " << n
            << " and seq = ";
            for (int i : seq) {
                cout << i << " ";
            }
            cout << endl;
            return false;
        }

        if (t0 - lastReport >= reportInterval) {
            cout << "[completionCheck progress at "
                 << duration_cast<minutes>(t0 - startTime).count() << " mins]: "
                 << "total: " << totalCounter 
                 << ", under 0.1s: " << under100ms
                 << ", 0.1s–2s: " << under2s
                 << ", 2s–30s: " << under30s
                 << ", 30s–2min: " << under2min
                 << ", 2min–6min: " << under6min
                 << ", 6min–30min: " << under30min
                 << ", 30min+: " << over30min
                 << endl;
            lastReport = t0;
        }
        buffer[strcspn(buffer, "\n")] = 0;
        string graph6 = buffer;

        PartComplCoreGameState pccgs(graph6);
        vector<int> curSeq;
        for (int i = 0; i < pccgs.getN(); ++i) {
            if (pccgs.deg(i) > 3) {
                curSeq.push_back(pccgs.deg(i));
            }
        }
        sort(curSeq.begin(), curSeq.end());

        if (curSeq == seq &&!pccgs.completionFilter()) {
            pclose(pipe);
            return false;
        }

        auto t1 = steady_clock::now();
        auto durMs = duration_cast<milliseconds>(t1 - t0).count();

        ++totalCounter;
        if (durMs < 100) ++under100ms;
        else if (durMs < 2000) ++under2s;
        else if (durMs < 30000) ++under30s;
        else if (durMs < 120000) ++under2min;
        else if (durMs < 360000) ++under6min;
        else if (durMs < 1800000) ++under30min;
        else ++over30min;
    }

    pclose(pipe);
    cout << "completionCheckDegSeq finished for n = " << n << " and seq = ";
            for (int i : seq) {
                cout << i << " ";
            }
            cout << endl;
    return true;
}


bool checkAllSeq(string inFileName) {
    ifstream inFile(inFileName);
    if (!inFile) {
        cerr << "Error: Could not open " << inFileName << "\n";
        return false;
    }

    string line;
    while (getline(inFile, line)) {
        istringstream iss(line);
        vector<int> seq;
        int val, n = 0;

        while (iss >> val) {
            ++n;
            if (val > 3) {
                seq.push_back(val);
            }
        }
    // REPLACE BELOW CODE BY THIS ONECE WE DON'T HAVE A MAXIMUM TIME FOR A 
    // SINGLE DEGREE SEQUENCE ANYMORE
    //     if (!completionCheck(n, seq)) {
    //         return false;
    //     }
        completionCheckDegSeq(n, seq);
    }
    return true;
}