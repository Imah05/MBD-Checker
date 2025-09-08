#include "part_compl_core_game_state.h"
#include <iostream>
#include <algorithm>
#include <stdexcept>
#include <cmath>
#include <fstream>
#include <sstream>
#include <unistd.h>
#include <sys/wait.h>
#include <thread>

using namespace std;


vector<vector<int>> inSeqs; 

PartComplCoreGameState::PartComplCoreGameState(const string& graph6) : 
                            Graph(graph6), DVtx(vector<bool>(getN(), false)), 
                            SVtx(vector<bool>(getN(), false)) {
    update();
}

void PartComplCoreGameState::update() {
    int n = getN();
    gameStateDeg.assign(n, 0);
    lowDegVtx.clear();
    totalPot = 0.;
    pot.assign(n, 0.);
    remVtx.clear();

    // set gameStateDeg[i] = -1 for vertices i claimed by Dominator or adjacent
    // to a vertex claimed by Dominator
    for (int i = 0; i < n; ++i) {
        if (DVtx[i]) {
            gameStateDeg[i] = -1;
            for (int j : neighborhood(i)) {
                gameStateDeg[j] = -1;
            }
        }
    }

    // determine gameStateDeg[i] for all other vertices i
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
            if (deg(i) < d) {
                gameStateDeg[i] += d - deg(i);
            }
        }
    }

    for (int i = 0; i < n; ++i) {
        if (gameStateDeg[i] != -1) {
            totalPot += pow(2,-gameStateDeg[i]);
        }
        if (deg(i) < d) {
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
        if (gameStateDeg[left] != -1 && gameStateDeg[right] == -1) {
            return true;
        }
        if (gameStateDeg[left] == -1 && gameStateDeg[right] != -1) {
            return false;
        }
        return gameStateDeg[left] < gameStateDeg[right];
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

char PartComplCoreGameState::potentialOutcome(char firstPlayer) const {
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
            return 'D';
        }
        if (remVtx.empty() && !lowDegVtx.empty()) {
            return 'U';
        }
        PartComplCoreGameState nextCoreGS = *this;
        char uFlag = false;
        for (int i : remVtx) {
            nextCoreGS.DVtx[i] = true;
            nextCoreGS.update();
            char out = nextCoreGS.potentialOutcome('S');
            if (out == 'D') {
                return 'D';
            }
            if (out == 'U') {
                uFlag = true;
            }
            nextCoreGS.DVtx[i] = false;
        }
        return uFlag ? 'U' : 'S';
    }
    else if (firstPlayer == 'S') {
        if (totalPot < 0.5) {
            return 'D';
        }
        
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
            int missingEdges = d - deg(i);
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
            if (missingEdges <= 0 && newTotalPot >= 1) {
                return 'U';
            }
        }
        PartComplCoreGameState nextCoreGS = *this;
        char uFlag = false;
        for (int i : remVtx) {
            nextCoreGS.SVtx[i] = true; 
            nextCoreGS.update(); 
            char out = nextCoreGS.potentialOutcome('D');
            if (out == 'S') {
                return 'S';
            }
            if (out == 'U') {
                uFlag = true;
            }
            nextCoreGS.SVtx[i] = false;
        }
        return uFlag ? 'U' : 'D';
    }
    // unreachable return 
    return 'F';
}

unordered_set<string> labelCanonicalBatch(const vector<string>& graph6Vec) {
    int inPipe[2];
    int outPipe[2];
    if (pipe(inPipe) == -1 || pipe(outPipe) == -1) {
        throw runtime_error("pipe failed");
    }

    pid_t pid = fork();
    if (pid < 0) {
        throw runtime_error("fork failed");
    }

    if (pid == 0) {
        // Child
        dup2(inPipe[0], STDIN_FILENO);
        dup2(outPipe[1], STDOUT_FILENO);
        close(inPipe[0]); 
        close(inPipe[1]);
        close(outPipe[0]); 
        close(outPipe[1]);
        execlp("labelg", "labelg", "-q", nullptr);
        _exit(1);
    }

    // Parent
    close(inPipe[0]);
    close(outPipe[1]);

    // Thread to write input
    thread writerThread([&]() {
        for (const string& g6 : graph6Vec) {
            write(inPipe[1], g6.c_str(), g6.size());
            write(inPipe[1], "\n", 1);
        }
        close(inPipe[1]); // signal EOF
    });

    // Main thread reads output
    unordered_set<string> result;
    char buffer[256];
    string partial;
    ssize_t count;
    int counter = 0;
    while ((count = read(outPipe[0], buffer, sizeof(buffer))) > 0) {
        counter++;
        partial.append(buffer, count);
        size_t pos;
        while ((pos = partial.find('\n')) != string::npos) {
            result.insert(partial.substr(0, pos));
            partial.erase(0, pos + 1);
        }
    }
    close(outPipe[0]);
    writerThread.join();

    int status;
    waitpid(pid, &status, 0);
    if (!WIFEXITED(status) || WEXITSTATUS(status) != 0) {
        throw runtime_error("labelg process failed");
    }
    return result;
}


char completionOutcome(const string& graph6, char firstPlayer) {
    vector<string> g6Vec = {graph6};
    while(!g6Vec.empty()) {
        unordered_set<string> g6Set = labelCanonicalBatch(g6Vec);
        g6Vec.clear();
        for (string g6 : g6Set) {
            PartComplCoreGameState coreGs = PartComplCoreGameState(g6);
            char out = coreGs.potentialOutcome(firstPlayer);
            if (out == 'D') {
                continue;
            }
            else if (out == 'S') {
                return 'S';
            }
            else if (out == 'U') {
                int v = coreGs.lowDegVtx[0];
                PartComplCoreGameState newCoreGS(coreGs);
                for (int w : coreGs.lowDegVtx) {
                    if (!coreGs.hasEdge(v, w) && w != v) {
                        newCoreGS.addEdge(v, w);
                        g6Vec.push_back(newCoreGS.toGraph6());
                        newCoreGS.removeEdge(v, w);
                    }
                }
            }
        }
    }
    return 'D';
}

vector<vector<int>> generateSeqs(int N1, int N2, int d, int surBound) {
    for (int N = N1; N <= N2; ++N) {
        vector<int> seq = vector<int>(N, d);
        seq[N - 1] = d + 1; 
        seq[N - 2] = d + 1; 
        while(seq[0] == d) {
            int n = 0;
            for (int i : seq) {
                if (i == d) {
                    ++n;
                }
            }
            int D1 = seq[N - 1];
            int D2 = seq[N - 2];
            double totalPot = pow(2, -D2);
            if (n - D1 > D2) {
                totalPot += (n - D1 + D2) * pow(2, -d - 1);
            }
            else {
                totalPot += (2 * n - 2 * D1) * pow(2, -d - 1);
            }
            for (int i = n; i < N - 2; ++i) {
                totalPot += pow(2, -seq[i] - 1);
            }
            if (totalPot >= 1) {
                int sur = 0, odds = 0, highSum = 0;
                for (int i = 0; i < N; ++i) {
                    if (i < N - 1) {
                        sur += seq[i] - d;
                    }
                    if (seq[i] % 2 == 1) {
                        ++odds;
                    }
                    else {
                        highSum += seq[i];
                    }
                }
                if (sur >= surBound && odds % 2 == 0 && n * d >= highSum) {
                    inSeqs.push_back(seq);
                }
                ++seq[N - 1];
            }
            else {
                for (int i = N - 2; i >= 0; --i) {
                    if (seq[i] < D1) {
                        seq[i]++;
                        for (int j = i + 1; j < N; ++j) {
                            seq[j] = seq[i];
                        }
                        break;
                    }
                }
            }
        }
    }
    return inSeqs;
}

bool filter(const string& graph6) {
    Graph pcc(graph6);
    vector<int> degSeq;
    for (int i = 0; i < pcc.getN(); ++i) {
        int deg = pcc.deg(i);
        if (deg <= d) {
            degSeq.push_back(d);
        }
        else {
            degSeq.push_back(deg);
        }
    }
    sort(degSeq.begin(), degSeq.end());

    for (vector<int> curDegSeq : inSeqs) {
        if (curDegSeq == degSeq) {
            return completionOutcome(graph6, 'D') == 'D';
        }
    }
    return true;
}