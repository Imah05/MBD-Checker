#include "RSI.h"

using namespace std;

bool havel_hakimi(deque<int> degreeSequence)  {
    //Sort the input into non-increasing order.
    sort(degreeSequence.begin(), degreeSequence.end(), greater<int>());

    //If there is a element with a value greater than the number of elements, return false.
    //Since the container is sorted, we only need to inspect the first element.
    if (degreeSequence.front() > degreeSequence.size())
        return false;
    
    //If there is a negative element, return false.
    //Since the container is sorted, we only need to inspect the last element.
    if (degreeSequence.back() < 0)
        return false;
    
    //If the container has all 0s, return true.
    //Since the container is sorted and there is no negative values, we only need to inspect the first element.
    if (!degreeSequence.front())
        return true;
    
    int sum = accumulate(degreeSequence.begin(), degreeSequence.end(), 0);
    
    //If the sum of the elements is odd, return false.
    if (sum % 2)
        return false;

    //Remove the element from the front, decrement the first "front" number of values.
    int front = degreeSequence.front();
    degreeSequence.pop_front();
    for (int i = 0; i < front; ++i)
        --degreeSequence[i];

    //Recursive call
    return havel_hakimi(move(degreeSequence));
}

// assumes that a file "R"+ (char)numVtxInR + "sur" + (char)sur
// exists in the graphs directory
char filter_R(int sur, int numVtxInR, char flag) {
    string inFilePath = "graphs/sur8/R" + to_string(numVtxInR) + "sur" + to_string(sur) + ".txt";
    string outFilePathD = inFilePath.substr(0, inFilePath.size() - 4) + "flag" + to_string(flag) + "outcomeD" + ".txt";
    string outFilePathN = inFilePath.substr(0, inFilePath.size() - 4) + "flag" + to_string(flag) + "outcomeN" + ".txt";
    string outFilePathS = inFilePath.substr(0, inFilePath.size() - 4) + "flag" + to_string(flag) + "outcomeS" + ".txt";

    char outFlag = 'D';
    ifstream file(inFilePath);
    if (!file.is_open()) {
        throw runtime_error("Failed to open the file: " + inFilePath);
    }
    ofstream outFileD(outFilePathD);
    if (!outFileD.is_open()) {
        throw runtime_error("Failed to open the output file: " + outFilePathD);
    }
    ofstream outFileN(outFilePathN);
    if (!outFileN.is_open()) {
        throw runtime_error("Failed to open the output file: " + outFilePathN);
    }
    ofstream outFileS(outFilePathS);
    if (!outFileS.is_open()) {
        throw runtime_error("Failed to open the output file: " + outFilePathS);
    }
    
    string line;
    int lineNumber = 0;

    int lowDegSur, highDegSur, curDeg, maxDeg; 
    while (getline(file, line)) {
        ++lineNumber;
        try {
            Graph graph = Graph(line);
            lowDegSur = 0;
            highDegSur = 0;
            maxDeg = 0;
            for (int i = 0; i < numVtxInR; i++) {
                curDeg = graph.neighborhood(i).size();
                if (curDeg > maxDeg) {
                    maxDeg = curDeg;
                }
                if (curDeg > 3) {
                    highDegSur += (curDeg - 3);
                }
                else if (curDeg < 3) {
                    lowDegSur += (3 - curDeg);
                }
            }
            if (maxDeg > 3) {
                highDegSur -= (maxDeg - 3);
            }
            // maybe stronger first filter?
            if ((highDegSur + lowDegSur) <= (sur - 3)) {
                GameState gs = GameState(&graph);
                if (flag == 'S') { 
                    if (gs.outcome('S') == 'D') {
                        outFileD << line << endl;
                    }
                    else if (gs.outcome('D') == 'D') {
                        outFileN << line << endl;
                        if (outFlag != 'S') {
                            outFlag = 'N';
                        }
                    }
                    else {
                        outFileS << line << endl;
                        outFlag = 'S';
                    }
                }
                if (flag == 'N') { 
                    if (gs.outcome('S') == 'D') {
                        // filter
                    }
                    else if (gs.outcome('D') == 'D') {
                        outFileN << line << endl;
                        if (outFlag != 'S') {
                            outFlag = 'N';
                        }
                    }
                    else {
                        outFileS << line << endl;
                        outFlag = 'S';
                    }
                }
                if (flag == 'D') { 
                    if (gs.outcome('D') == 'D') {
                        // filter
                        if (outFlag != 'S') {
                            outFlag = 'N';
                        }
                    }
                    else {
                        outFileS << line << endl;
                        outFlag = 'S';
                    }
                }
            }
        } catch (const exception& e) {
            cerr << "Error processing line " << lineNumber << ": " << e.what() << endl;
        }
    }
    file.close();
    outFileD.close();
    outFileN.close();
    outFileS.close();
    return outFlag;
}
