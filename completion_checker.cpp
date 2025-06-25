#include "completion_checker.h"
#include <cstring>

// TODO

// bool completionCheck(string cmd, vector<set<int>>) {
//     FILE *pipe = popen(cmd.c_str(), "r");
//     if (!popen) {
//         pclose(pipe);
//         throw runtime_error("completionCheck: popen failed");
//     }
//     char buffer[128];
//     string graph6;

//     while (fgets(buffer, sizeof(buffer), pipe)) {
//         buffer[strcspn(buffer, "\n")] = 0;
//         graph6 = buffer;

//         PartComplCoreGameState pccgs(graph6);
        
//         for(int i = 0; i < pccgs.getN(); i++) {
//             if (pccgs.deg(i) == 2)
//                 ++numOfDeg2Vtx;
//         }
//         if (numOfDeg2Vtx <= surBound && pccgs.outcome(firstPlayer) == 'S') {
//             cout << "Exceptional graph found in checkSI: " << graph6 << endl;
//             return false;
//         }
//     }
//     pclose(pipe);
//     return true;
// }

