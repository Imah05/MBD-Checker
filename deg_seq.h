#ifndef DEG_SEQ_H
#define DEG_SEQ_H

#include <vector>
#include <cmath>

int d = 3; 

bool potTooSmall(vector<int> degseq) {
    int n = degseq.size();
    double totalPot = 0.;
    double totalPotAfterD = 0.;
    for (int i = 0; i < n; ++i) {
        double inc = pow(2, -degseq[i]-1);
        totalPot += inc;
        if (degseq[n-1] <= i && i < n-1) {
            totalPotAfterD += inc;
        }
    }
    if (totalPot < 1 || totalPotAfterD < 0.5) {
        return true;
    }
    return false;
}

// Returns a vector of all degree sequences degseq satisfying the following 
// conditions.
//  (a) degseq has n elements and all entries are at least 3. 
//  (b) degseq is ordered from smallest to largest.
//  (c) sum_{i=0}^{n-1} degseq[i] is even.
//  (e) sum_{i=0}^{n-2} |degseq[i]-3| is at least surBound.
//  (d) The maximum entry of degseq is at most the number of 3's in degseq.
//  (f) The Erdős-Selfridge potential degseq is at least 1. 
//  (g) The Erdős-Selfridge potential of every graph with degree sequence degseq
//      and no edge between vertices of degree greater than 3 has potential at 
//      least 1/2 after Dominater chooses a maximal degree vertex.
// vector<vector<int>> generateDegSeq(int n, int surBound) { 
//     vector<int> degseq(n, 3);
//     bool flagPotTooSmall = false;

//     while (!flagPotTooSmall || degseq[n-1] > degseq[0] + 1) {

//         int lastIndex = -1;
//         for (int i = degseq.size() - 1; i >= 0; --i) {
//             if (degseq[i] == 3) {
//                 lastIndex = i;
//                 break;
//             }
//         }
//         while (!flagPotTooSmall && degseq[n-1] <= lastIndex + 1) {
//             degseq[n-1]++;
//         }
//     }
// }



#endif // DEG_SEQ_H