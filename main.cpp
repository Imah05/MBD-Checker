#include "GameState.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cmath>
#include <limits>
#include "Core.h"
#include <filesystem>
#include <chrono>
#include <thread>


namespace fs = std::filesystem;
using namespace std;


int main() {
    Graph graph = Graph("TXeSc@OSA_G?_?_?O????O?@??A???????Bm");
    Core c = Core(&graph);
    auto t1 = std::chrono::high_resolution_clock::now();
    bool filter = c.filter();
    c.completion_filter();
    auto t2 = std::chrono::high_resolution_clock::now();
    std::cout << "f() took "
              << std::chrono::duration_cast<std::chrono::nanoseconds>(t2-t1).count()
              << " nanoseconds\n";
}
