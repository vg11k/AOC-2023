#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <map>
using namespace std;

struct Tuple {
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

    for (int i = 2; i < allLines.size(); i++) {
        istringstream iss(allLines.at(i));

        string position;
        string s;
        Tuple t;

        getline(iss, position, ' ');
        getline(iss, s, ' ');
        getline(iss, s, ' ');
        t.L = s.substr(1, 3);
        getline(iss, s, ' ');
        t.R = s.substr(0, 3);

        positionToPaths[position] = t;
    }

    string currentPosition = "AAA";
    Tuple currentTuple = positionToPaths.find(currentPosition)->second;

    while (currentPosition != "ZZZ") {
        string side = instructs.substr(result % instructs.length(), 1);
        if (side == "L") currentPosition = currentTuple.L;
        else if (side == "R") currentPosition = currentTuple.R;
        else cout << "WTF ??" << endl;

        currentTuple = positionToPaths.find(currentPosition)->second;
        result++;
    }



    end = clock();

    cout << "result : " << result << endl;
    cout << end - start << " ms to compute." << endl;

    cout << "Bye World !" << endl;
    return 0;
}