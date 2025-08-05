#include "sequence_generator.h"
#include <cmath>
#include <numeric>
using namespace std;

vector<vector<int>> generateSeqs(int NStart, int NEnd, int d, int surBound) {
    vector<vector<int>> result;
    for (int N = NStart; N <= NEnd; ++N) {
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
                    result.push_back(seq);
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
    return result;
}