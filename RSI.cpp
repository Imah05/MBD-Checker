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
pair<int, int> filter_R(int sur, int numVtxInR, char flag) {
    string inFilePath = "graphs/sur" + to_string(sur) + "/unfiltered_R/R" + to_string(numVtxInR) + "sur" + to_string(sur) + ".txt";
    string path = "graphs/sur" + to_string(sur) + "/filtered_R/R" + to_string(numVtxInR) + "sur" + to_string(sur);
    
    ifstream file(inFilePath);
    if (!file.is_open()) {
        throw runtime_error("Failed to open the file: " + inFilePath);
    }
  
    
    string line;
    int lineNumber = 0;
    int maxComplMaxDeg = 0;
    int remainingSur = -1;
    int lowDegSur, highDegSur, curDeg, maxDeg, curSur, complMaxDeg; 

    if (flag == 'S') {
        string outFilePathD = path + "flag" + flag + "outcomeD" + ".txt";
        ofstream outFileD(outFilePathD);
        if (!outFileD.is_open()) {
            throw runtime_error("Failed to open the output file: " + outFilePathD);
        }
        string outFilePathN = path + "flag" + flag + "outcomeN" + ".txt";
        ofstream outFileN(outFilePathN);
        if (!outFileN.is_open()) {
            throw runtime_error("Failed to open the output file: " + outFilePathN);
        }
        string outFilePathS = path + "flag" + flag + "outcomeS" + ".txt";
        ofstream outFileS(outFilePathS);
        if (!outFileS.is_open()) {
            throw runtime_error("Failed to open the output file: " + outFilePathS);
        }
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
                if (maxDeg <= 3) {
                    curSur = highDegSur + lowDegSur + 1;
                }
                else {
                    curSur = highDegSur + lowDegSur - (maxDeg - 3);
                }
                if (curSur <= (sur - 3)) {
                    if ((sur - 3) - curSur > remainingSur) {
                        remainingSur = (sur - 3) - curSur;
                    }
                    if (maxDeg <= 4) {
                        complMaxDeg = 4 + (sur - 3) - curSur;
                    }
                    else {
                        complMaxDeg = maxDeg + (sur - 3) - curSur;
                    }
                    if (complMaxDeg > maxComplMaxDeg) {
                        maxComplMaxDeg = complMaxDeg;
                    }
                    GameState gs = GameState(&graph);
                    if (gs.outcome('S') == 'D') {
                        outFileD << line << endl;
                    }
                    else if (gs.outcome('D') == 'D') {
                        outFileN << line << endl;
                    }
                    else {
                        outFileS << line << endl;
                    }
                }
            } catch (const exception& e) {
                cerr << "Error processing line " << lineNumber << ": " << e.what() << endl;
            }
        }
    outFileD.close();
    outFileN.close();
    outFileS.close();
    }
    else if (flag == 'N') {
        string outFilePathN = path + "flag" + flag + "outcomeN" + ".txt";
        ofstream outFileN(outFilePathN);
        if (!outFileN.is_open()) {
            throw runtime_error("Failed to open the output file: " + outFilePathN);
        }
        string outFilePathS = path + "flag" + flag + "outcomeS" + ".txt";
        ofstream outFileS(outFilePathS);
        if (!outFileS.is_open()) {
            throw runtime_error("Failed to open the output file: " + outFilePathS);
        }
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
                curSur = highDegSur + lowDegSur - (maxDeg - 3);
                if (maxDeg <= 3) {
                    ++curSur;
                }
                if (curSur <= (sur - 3)) {
                    if (maxDeg <= 4) {
                        complMaxDeg = 4 + (sur - 3) - curSur;
                    }
                    else {
                        complMaxDeg = maxDeg + (sur - 3) - curSur;
                    }
                    GameState gs = GameState(&graph);
                    if (gs.outcome('S') == 'D') {
                        // filter
                    }
                    else if (gs.outcome('D') == 'D') {
                        if ((sur - 3) - curSur > remainingSur) {
                            remainingSur = (sur - 3) - curSur;
                        }
                        if (complMaxDeg > maxComplMaxDeg) {
                            maxComplMaxDeg = complMaxDeg;
                        }
                        outFileN << line << endl;
                    }
                    else {
                        if ((sur - 3) - curSur > remainingSur) {
                            remainingSur = (sur - 3) - curSur;
                        }
                        if (complMaxDeg > maxComplMaxDeg) {
                            maxComplMaxDeg = complMaxDeg;
                        }
                        outFileS << line << endl;
                    }
                }
            } catch (const exception& e) {
                cerr << "Error processing line " << lineNumber << ": " << e.what() << endl;
            }
        }
    outFileN.close();
    outFileS.close();
    }
    else if (flag == 'D') {
        string outFilePathS = path + "flag" + flag + "outcomeS" + ".txt";
        ofstream outFileS(outFilePathS);
        if (!outFileS.is_open()) {
            throw runtime_error("Failed to open the output file: " + outFilePathS);
        }
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
                curSur = highDegSur + lowDegSur - (maxDeg - 3);
                if (maxDeg <= 3) {
                    ++curSur;
                }
                if (curSur <= (sur - 3)) {
                    if (maxDeg <= 4) {
                        complMaxDeg = 4 + (sur - 3) - curSur;
                    }
                    else {
                        complMaxDeg = maxDeg + (sur - 3) - curSur;
                    }
                    GameState gs = GameState(&graph);
                    if (gs.outcome('D') == 'D') {
                        // filter
                    }
                    else {
                        if ((sur - 3) - curSur > remainingSur) {
                            remainingSur = (sur - 3) - curSur;
                        }
                        if (complMaxDeg > maxComplMaxDeg) {
                            maxComplMaxDeg = complMaxDeg;
                        }
                        outFileS << line << endl;
                    }
                }
            } catch (const exception& e) {
                cerr << "Error processing line " << lineNumber << ": " << e.what() << endl;
            }
        }
    outFileS.close();
    }
    file.close();
    return {maxComplMaxDeg, remainingSur};
}

int filter_SI(int sur, int numVtxInS, int numVtxInI, char flag, int remainingSur)
{
    return 0;
}
