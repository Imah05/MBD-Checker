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
    check_all_RSI_Lemmas();
    return 0;
}