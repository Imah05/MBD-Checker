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

vector<vector<int>> inputDegSequences;

void loadInputSequences(const string& filename) {
    ifstream infile(filename);
    if (!infile) {
        cerr << "Error opening file: " << filename << endl;
        return;
    }

    string line;
    while (getline(infile, line)) {
        istringstream iss(line);
        vector<int> sequence;
        int num;
        while (iss >> num) {
            sequence.push_back(num);
        }
        inputDegSequences.push_back(sequence);
    }
}

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
            return lowDegVtx[0];
            // return out;
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
                return lowDegVtx[0];
                // return i;
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
    // unreachable return 
    return -3;
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
        execlp("nauty-labelg", "nauty-labelg", "-q", nullptr);
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
        throw runtime_error("nauty-labelg process failed");
    }
    return result;
}


bool PartComplCoreGameState::completionFilter() const {
    vector<string> g6Vec = {toGraph6()};
    while(!g6Vec.empty()) {
        unordered_set<string> g6Set = labelCanonicalBatch(g6Vec);
        g6Vec.clear();
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
            for (int b : coreGs.lowDegVtx) {
                if (!coreGs.hasEdge(a, b) && b != a) {
                    newCoreGS.addEdge(a, b);
                    g6Vec.push_back(newCoreGS.toGraph6());
                    newCoreGS.removeEdge(a, b);
                }
            }
        }
    }
    return true;
}

bool completionFilter(const string& graph6) {
    PartComplCoreGameState pcc(graph6);
    vector<int> degSeq;
    for (int i = 0; i < pcc.getN(); ++i) {
        int deg = pcc.deg(i);
        if (deg <= 3) {
            degSeq.push_back(3);
        }
        else {
            degSeq.push_back(deg);
        }
    }
    sort(degSeq.begin(), degSeq.end());

    for (vector<int> curDegSeq : inputDegSequences) {
        if (curDegSeq == degSeq) {
            return pcc.completionFilter();
        }
    }
    return true;
}