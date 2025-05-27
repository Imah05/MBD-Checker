#include "GameState.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cmath>
#include <limits>
#include "core.h"
#include "RSI.h"


using namespace std;

// int main(int argc, const char *argv[]) {
//     string graph6_in;
//     int count=0;
//     clock_t c_start = clock();
//     while (getline(cin, graph6_in))
//     {
//         Graph graph(graph6_in);
//         Core core(&graph);
//         if (! core.completion_filter2())
//         {
//             cout << graph6_in;
//             cout << '\n';
//             count++;
//         }
//     }
//     clock_t c_end = clock();
//     double time = double(c_end - c_start) / CLOCKS_PER_SEC;
//     cerr.precision(2);
//     cerr << ">Z " << count << " graphs passed filter in " << time << " sec" << endl;
//     return 0;
// }

// check RSI Lemmas

int main() {
    // check_all_RSI_Lemmas();

    check_Lemma_R2();
    check_Lemma_R3();
    check_Lemma_R4();
    check_Lemma_R6();

    // checks, whether D. wins (going first) on every graph on at most 12 
    // vertices, minimum degree 1 and sum_{v != w} |deg(v)-3| <= 5, 
    // where w is a maxdeg vtx.
    // if(!check_R("geng -d1D9 12 16:19", 'D', 5).empty())
    //     return false;
    // if(!check_R("geng -d1D8 11 14:17", 'D', 5).empty())
    //     return false;
    // if(!check_R("geng -d1D3 10 13:13", 'D', 5).empty())
    //     return false;


    return 0;
}