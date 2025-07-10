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
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <stdexcept>
#include <string>
#include <vector>
#include <set>
#include <sstream>
#include <cstring>
#include <iostream>
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


// set<string> labelCanonicalBatch(const vector<string>& graph6Vec, string inFileName, int start, int end) {
//     stringstream ss;
//     ss << "temp//" << start << "_" << end << "_" << inFileName;
//     string fileName = ss.str();
//     string command = "nauty-labelg -q " + fileName;
//     ofstream outFile(fileName);
//     if (!outFile) {
//         throw runtime_error("labelCanonicalBatch: failed to open temp_input.g6 for writing");
//     }
//     for (string g6 : graph6Vec) {
//         outFile << g6 << '\n';
//     }
//     outFile.close();
    
//     FILE* fp = popen(command.c_str(), "r");
//     if (!fp) {
//         throw runtime_error("labelCanonicalBatch: popen failed");
//     }
//     set<string> result;
//     char buff[100];
//     while (fgets(buff, sizeof(buff), fp)) {
//         buff[strcspn(buff, "\n")] = 0;
//         result.insert(string(buff));
//     }
//     pclose(fp);
//     if (remove(fileName.c_str()) != 0) {
//         cerr << ("Error deleting temporary file");
//     }
//     return result;
// }


#include <thread>
// ... other includes

unordered_set<string> labelCanonicalBatch(const vector<string>& graph6Vec) {
    int inPipe[2];
    int outPipe[2];
    if (pipe(inPipe) == -1 || pipe(outPipe) == -1) {
        throw runtime_error("pipe failed");
    }

    pid_t pid = fork();
    if (pid < 0) throw runtime_error("fork failed");

    if (pid == 0) {
        // Child
        dup2(inPipe[0], STDIN_FILENO);
        dup2(outPipe[1], STDOUT_FILENO);
        close(inPipe[0]); close(inPipe[1]);
        close(outPipe[0]); close(outPipe[1]);
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
    while ((count = read(outPipe[0], buffer, sizeof(buffer))) > 0) {
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


bool PartComplCoreGameState::completionFilter(string inFileName, int start, int end) const {
    vector<string> g6Vec = {toGraph6()};
    while(!g6Vec.empty()) {
        // set<string> g6Set = labelCanonicalBatch(g6Vec, inFileName, start, end);
        unordered_set<string> g6Set = labelCanonicalBatch(g6Vec);
        g6Vec.clear();
        int numberOfPCC;
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
                    if (coreGs.deg(a) > 3 || coreGs.deg(b) > 3) {
                        cout << "Hilfe!!!" << endl;
                    }
                    newCoreGS.addEdge(a, b);
                    g6Vec.push_back(newCoreGS.toGraph6());
                    newCoreGS.removeEdge(a, b);
                }
            }
        }
        numberOfPCC = g6Set.size();
    }
    return true;
}


bool checkFile(const string& inFileName, int start, int end) {
    ifstream inFile("Cores//" + inFileName);
    if (!inFile) {
        cerr << "Error: Could not open " << "Cores//" << inFileName << endl;
        return false;
    }

    vector<vector<int>> seqs;
    string line;
    bool readingSeq = true;
    int n;

    while (getline(inFile, line)) {
        if (line.empty()) {
            readingSeq = false;
            break;
        }
        istringstream iss(line);
        vector<int> seq;
        int val;
        n = 0;
        while (iss >> val) {
            if (val > 3) {
                seq.push_back(val);
            }
            n++;
        }
        sort(seq.begin(), seq.end());
        seqs.push_back(move(seq));
    }

    if (readingSeq) {
        cerr << "Error: No blank line separating header from graph6 data\n";
        return false;
    }
    stringstream outFileStream;
    outFileStream << "Cores//output//";
    if (end == -1 && start == 1) {
        outFileStream << inFileName;
    }
    else {
        outFileStream << inFileName.substr(0, inFileName.size() - 4) << "_" << start << "-"; 
        if (end == -1)  {
            outFileStream << "end"; 
        }
        else {
            outFileStream << end;
        }
    }
    string outFileName = outFileStream.str();
    ofstream outFile(outFileName);
    if (!outFile) {
        cerr << "Could not open " << outFileName << endl;
        return false;
    }

    outFile << "Checking sequences: " << endl;
    for (vector<int> seq : seqs) {
        outFile << "n = " << n << ", high Vtx = ";
        for (int i : seq) {
            outFile << i << " ";
        }
        outFile << endl;
    }

    streampos graphsStartPos = inFile.tellg();
    int graphCount = 0;
    while (getline(inFile, line)) {
        if (!line.empty()) {
            ++graphCount;
        }
    }
    outFile << "Checking cores " << start << "-";
    if (end == -1) {
        outFile << graphCount;
    }
    else {
        outFile << end; 
    }
    outFile << " from a total of " << graphCount << endl;
    inFile.clear();
    inFile.seekg(graphsStartPos);
    
    auto startTime = steady_clock::now();
    auto lastReport = startTime;
    const auto maxDuration = minutes(6000);
    const auto reportInterval = minutes(30);
    int totalCounter = 0, under1s = 0, under1min = 0, under30min = 0,
            over30min = 0;

    while (getline(inFile, line)) {
        if (totalCounter < start - 1) {
            totalCounter++;
            continue;
        }
        if (end != -1 && totalCounter >= end) {
            break;
        }
        auto t0 = steady_clock::now();
        if (t0 - startTime > maxDuration) {
            outFile << "time out: completionCheckDegSeq terminated" << endl << endl;
            return false;
        }
        auto now = system_clock::now();
        time_t now_time = system_clock::to_time_t(now);
        tm local_tm = *localtime(&now_time);
        if (t0 - lastReport >= reportInterval) {
            outFile << put_time(&local_tm, "%H:%M:%S") << endl
                 << "total: " << totalCounter 
                 << ", under 1s: " << under1s
                 << ", 1s–1min: " << under1min
                 << ", 1min-30min: " << under30min
                 << ", 30min+: " << over30min
                 << endl;
            lastReport = t0;
        }

        if (line.empty()) {
            continue;
        }

        PartComplCoreGameState pccgs(line);
        vector<int> curSeq;
        for (int i = 0; i < pccgs.getN(); ++i) {
            int deg = pccgs.deg(i);
            if (deg > 3) {
                curSeq.push_back(deg);
            }
        }
        sort(curSeq.begin(), curSeq.end());

        bool matchesAny = false;
        for (vector<int> seq : seqs) {
            if (seq == curSeq) {
                matchesAny = true;
                break;
            }
        }
        if (matchesAny) {
            if (!pccgs.completionFilter(inFileName, start, end)) {
                outFile << "Counterexample found: " << line;
                return false; 
            }
        }
        auto t1 = steady_clock::now();
        auto durMs = duration_cast<seconds>(t1 - t0).count();

        ++totalCounter;
        if (durMs < 1) ++under1s;
        else if (durMs < 60) ++under1min;
        else if (durMs < 1800) ++under30min;
        else ++over30min;
    }
    inFile.close();
    outFile << "Finished checking" << endl << endl;
    //rename(outFileName.c_str(), ("Cores//output//FINISHED_" + inFileName).c_str());
    outFile.close();
    return true; 
}