#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <map>
using namespace std;

struct Race {
    int time;
    int distance;
    Race();
    Race(int t) : time(t) {}
};

int nbCombinaisonsToWin(Race race) {
    vector<int> winningHold;
    for(int holdingTime = 0; holdingTime < race.time; holdingTime++) {
        int distance = (race.time - holdingTime) * holdingTime;
        if(distance > race.distance) winningHold.push_back(holdingTime);
    }
    return winningHold.size();
}

int main(char * args[]) {

    cout<< "Hello World !" << endl;

    clock_t start, end;

    ifstream infile("input.txt");
    string line;
    int result = 0;
    vector<string> allLines;
    vector<Race> races;

    start = clock();

    while(getline(infile, line)) {
        allLines.push_back(line);
    }

    istringstream iss(allLines.at(0));
    istringstream iss2(allLines.at(1));
    string s;

    while(getline(iss, s, ' ')) {
        if(s != "" && s != "Time:") {
            Race r(stoi(s));
            races.push_back(r);
        }
    }

    int rindex = 0;
    while(getline(iss2, s, ' ')) {
        if(s != "" && s != "Distance:") {
            races.at(rindex).distance = stoi(s);
            rindex++;
        }
    }

    result = nbCombinaisonsToWin(races.at(0));
    result *= nbCombinaisonsToWin(races.at(1));
    result *= nbCombinaisonsToWin(races.at(2));
    result *= nbCombinaisonsToWin(races.at(3));


    end = clock();

    cout << "result : " << result << endl;
    cout << end - start << " ms to compute." << endl;

    cout << "Bye World !" << endl;
    return 0;
}