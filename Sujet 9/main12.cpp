#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>
using namespace std;

long findNext(vector<long> sequenceToComplete) {

    long sum = 0;
    for (int i = 0; i < sequenceToComplete.size(); i++) {
        sum += sequenceToComplete.at(i);
    }
    if (sum == 0) {
        return 0;
    }

    vector<long> subSequence;
    for (int i = 1; i < sequenceToComplete.size(); i++) {
        long subval = sequenceToComplete.at(i) - sequenceToComplete.at(i - 1);
        subSequence.push_back(subval);
    }

    long next = findNext(subSequence) + sequenceToComplete.at(sequenceToComplete.size() - 1);
    return next;
}


int main(char* args[]) {

    cout << "Hello World !" << endl;

    clock_t start, end;

    ifstream infile("input.txt");
    string line;
    long long resultPart1 = 0;
    long long resultPart2 = 0;
    vector<string> allLines;

    start = clock();

    while (getline(infile, line)) {
        allLines.push_back(line);
    }

    for (int lineIndex = 0; lineIndex < allLines.size(); lineIndex++) {
        vector<long> sequenceToComplete;
        string lineInput = allLines.at(lineIndex);

        istringstream iss(lineInput);
        string s;
        while (getline(iss, s, ' ')) {
            sequenceToComplete.push_back(stol(s));
        }


        resultPart1 += findNext(sequenceToComplete);

        reverse(sequenceToComplete.begin(), sequenceToComplete.end());
        resultPart2 += findNext(sequenceToComplete);


    }


    end = clock();

    cout << "result part 1 : " << resultPart1 << endl;
    cout << "result part 2 : " << resultPart2 << endl;
    cout << end - start << " ms to compute." << endl;

    cout << "Bye World !" << endl;
    return 0;
}