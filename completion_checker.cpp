#include "completion_checker.h"
#include <cstring>
#include <iostream>
#include <algorithm>
#include <chrono>
#include <ctime>
#include <iomanip>
#include <vector>
#include <string>
#include <numeric>
#include <sstream>
#include <stdexcept>
#include <cstdio>
#include <thread>
#include <fstream>

using namespace std;
using namespace std::chrono;

bool completionCheck(int n, vector<int> seq) {
    int mindeg = *std::min_element(seq.begin(), seq.end());
    int maxdeg = *std::max_element(seq.begin(), seq.end());
    int k = seq.size();
    int sum = std::accumulate(seq.begin(), seq.end(), 0);
    int n_minus_k = n - k;

    std::ostringstream cmdS;
    cmdS << "genbg -d" << mindeg << ":0 -D" << maxdeg << ":" << k << " "
         << k << " " << n_minus_k << " " << sum << ":" << sum;

    string cmd = cmdS.str();
    FILE *pipe = popen(cmd.c_str(), "r");
    if (!pipe) {
        throw runtime_error("completionCheck: popen failed");
    }

    char buffer[128];
    string graph6;

    // Time tracking
    auto startTime = steady_clock::now();
    auto lastReport = startTime;
    const auto maxDuration = minutes(6000);
    const auto reportInterval = minutes(30);

    // Buckets
    int under100ms = 0, under2s = 0, under30s = 0, under2min = 0, under6min = 0, under30min = 0;
    int coreCounter = 0;

    while (fgets(buffer, sizeof(buffer), pipe)) {
        // Check for timeout
        auto now = steady_clock::now();
        if (now - startTime > maxDuration) {
            pclose(pipe);
            cout << "completionCheck terminated prematurely" << endl;
            return false;
        }

        // Print time bucket status every 10 minutes
        if (now - lastReport >= reportInterval) {
            cout << "[completionCheck progress at "
                 << duration_cast<minutes>(now - startTime).count() << " mins]: "
                 << "total: " << coreCounter 
                 << ", under 0.1s: " << under100ms
                 << ", 0.1s–2s: " << under2s
                 << ", 2s–30s: " << under30s
                 << ", 30s–2min: " << under2min
                 << ", 2min–6min: " << under6min
                 << ", 6min–30min: " << under30min
                 << endl;
            lastReport = now;
        }

        ++coreCounter;
        buffer[strcspn(buffer, "\n")] = 0;
        graph6 = buffer;

        auto t0 = steady_clock::now();

        PartComplCoreGameState pccgs(graph6);
        vector<int> degSeq;
        for (int i = 0; i < pccgs.getN(); ++i) {
            if (pccgs.deg(i) > 3) {
                degSeq.push_back(pccgs.deg(i));
            }
        }
        sort(degSeq.begin(), degSeq.end());

        if (degSeq == seq && !pccgs.completionFilter()) {
            pclose(pipe);
            return false;
        }

        auto t1 = steady_clock::now();
        auto durMs = duration_cast<milliseconds>(t1 - t0).count();

        // Bucket classification
        if (durMs < 100) ++under100ms;
        else if (durMs < 2000) ++under2s;
        else if (durMs < 30000) ++under30s;
        else if (durMs < 120000) ++under2min;
        else if (durMs < 360000) ++under6min;
        else ++under30min;
    }

    pclose(pipe);
    cout << "completionCheck finished" << endl; 
    return true;
}


bool checkAllCores() {
    std::ifstream inFile("input_sequences.txt");
    if (!inFile) {
        std::cerr << "Error: Could not open input_sequences.txt\n";
        return false;
    }

    std::string line;
    while (std::getline(inFile, line)) {
        std::istringstream iss(line);
        std::vector<int> seq, seqWithout3s;
        int val;

        while (iss >> val) {
            seq.push_back(val);
            if (val > 3) {
                seqWithout3s.push_back(val);
            }
        }

        int n = seq.size();  // full sequence length

        if (!completionCheck(n, seqWithout3s)) {
            return false; // as soon as one fails, we're done
        }
    }

    return true; // all passed
}