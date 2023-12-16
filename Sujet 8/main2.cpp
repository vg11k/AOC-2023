#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <map>
using namespace std;

struct Tuple {
    string position;
    string L;
    string R;
};

int main(char* args[]) {

    cout << "Hello World !" << endl;

    clock_t start, end;

    ifstream infile("input.txt");
    string line;
    long long result = 0;
    vector<string> allLines;

    start = clock();

    while (getline(infile, line)) {
        allLines.push_back(line);
    }

    string instructs = allLines.at(0);
    map<string, Tuple> positionToPaths;

    vector<Tuple> currentTuples;

    for (int i = 2; i < allLines.size(); i++) {
        istringstream iss(allLines.at(i));

        string position;
        string s;
        Tuple t;

        getline(iss, position, ' ');
        t.position = position;
        getline(iss, s, ' ');
        getline(iss, s, ' ');
        t.L = s.substr(1, 3);
        getline(iss, s, ' ');
        t.R = s.substr(0, 3);

        positionToPaths[position] = t;

        if (position.substr(2, 1) == "A") {
            currentTuples.push_back(t);
        }
    }

    bool bruteForce = false; 

    if (bruteForce) {
        //don't do that. It's 13 days of computing at least.
        int instructionCursor = 0;
        bool everyz = false;

        while (!everyz) {

            everyz = true;
            string side = instructs.substr(result % instructs.length(), 1);

            for (int i = 0; i < currentTuples.size(); i++) {

                string currentPosition = currentTuples.at(i).position;
                Tuple currentTuple = currentTuples.at(i);

                if (side == "L") currentPosition = currentTuple.L;
                else if (side == "R") currentPosition = currentTuple.R;
                else cout << "WTF ??" << endl;

                currentTuples.at(i) = positionToPaths.find(currentPosition)->second;

                if (currentPosition.substr(2, 1) != "Z") {
                    everyz = false;
                }
            }

            result++;
        }
    }
    else {

        long long biggestGhostLoopValue = 0;
        vector<long long> ghostLoopValues;

        cout << "Beware the " << currentTuples.size() << " ghosts " << endl;

        for (int i = 0; i < currentTuples.size(); i++) {
            Tuple ghost = currentTuples.at(i);
            long long ghostSteps = 0;
            while (ghost.position.substr(2, 1) != "Z") {
                string side = instructs.substr(ghostSteps % instructs.length(), 1);

                if (side == "L") ghost = positionToPaths.find(ghost.L)->second;
                else if (side == "R") ghost = positionToPaths.find(ghost.R)->second;
                else cout << "WTF ??" << endl;

                ghostSteps++;
            }
            ghostLoopValues.push_back(ghostSteps);
            if (biggestGhostLoopValue < ghostSteps) biggestGhostLoopValue = ghostSteps;
        }

        cout << "biggest ghost loop value : " << biggestGhostLoopValue << endl;

        result += biggestGhostLoopValue;
        bool multiples = false;
        while (!multiples) {
            multiples = true;
            for (int ghostId = 0; ghostId < ghostLoopValues.size(); ghostId++) {
                if (result % ghostLoopValues.at(ghostId) != 0) {
                    multiples = false;
                }
            }

            if(!multiples) result += biggestGhostLoopValue;
        }
    }


    end = clock();

    cout << "result : " << result << endl;
    cout << end - start << " ms to compute." << endl;

    cout << "Bye World !" << endl;
    return 0;
}