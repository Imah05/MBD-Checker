#include <cmath>
#include <numeric>
#include <fstream>
#include <iostream>
#include <vector>

using namespace std;

vector<vector<int>> generateSeqs(int NStart, int NEnd, int d, int surBound) {
    vector<vector<int>> result;
    for (int N = NStart; N <= NEnd; ++N) {
        vector<int> seq = vector<int>(N, d);
        seq[N - 1] = d + 1; 
        seq[N - 2] = d + 1; 
        while(seq[0] == d) {
            int n = 1;
            for (int i : seq) {
                if (i == d) {
                    ++n;
                }
            }
            int dN = seq[N - 1];
            double maxPot = 0.;
            for (int i = n - 1; i < N - 1; ++i) {
                int di = seq[i];
                double totalPot = pow(2, -di);
                if (n - 1 - dN > di) {
                    totalPot += (n - 1 - dN + di) * pow(2, -d - 1);
                }
                else {
                    totalPot += (2 * n - 2 - 2 * dN) * pow(2, -d - 1);
                }
                for (int j = n - 1; j < N - 1; ++j) {
                    if (j != i) {
                        totalPot += pow(2, -seq[j] - 1);
                    }
                }
                if (totalPot > maxPot) {
                    maxPot = totalPot;
                }
            }
            
            if (maxPot >= 1) {
                int sur = 0, highSum = 0;
                for (int i = n - 1; i < N; ++i) {
                    if (i < N - 1) {
                        sur += seq[i] - d;
                    }
                    highSum += seq[i];
                }
                if (sur >= surBound && (highSum + d * (n - 1)) % 2 == 0 
                                        && (n - 1) * d >= highSum) {
                    result.push_back(seq);
                }
                ++seq[N - 1];
            }
            else {
                for (int i = N - 2; i >= 0; --i) {
                    if (seq[i] < dN) {
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
    return result;
}


int main() {
    vector<vector<int>> result = generateSeqs(16, 21, 3, 9);

    ofstream outFile("input_sequences.txt");
    if (!outFile) {
        cerr << "Error opening file sequences.txt" << endl;
        return 1;
    }

    for (const auto& seq : result) {
        for (size_t i = 0; i < seq.size(); ++i) {
            outFile << seq[i];
            if (i + 1 < seq.size()) outFile << " ";
        }
        outFile << "\n";
    }

    outFile.close();
    return 0;
}