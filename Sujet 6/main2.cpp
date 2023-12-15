#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <map>
using namespace std;

struct Race {
    long long time;
    long long distance;
    Race();
    Race(long long t) : time(t) {}
    Race(long long t, long long d) : time(t), distance(d) {}
};

long long nbCombinaisonsToWin(Race race) {
    vector<long long> winningHold;
    for(long long holdingTime = 0; holdingTime < race.time; holdingTime++) {
        long long distance = (race.time - holdingTime) * holdingTime;
        if(distance > race.distance) winningHold.push_back(holdingTime);
    }
    return winningHold.size();
}

long long crappyLineToLongLong(string line) {
    string sresult;
    istringstream iss(line);
    string s;
    while(getline(iss, s, ' ')) {
        if(s != "" && s != "Distance:" && s != "Time:") {
            sresult += s;
        }
    }
    return stoll(sresult);
}

int main(char * args[]) {

    cout<< "Hello World !" << endl;

    clock_t start, end;

    ifstream infile("input.txt");
    string line;
    long long result = 0;
    vector<string> allLines;

    start = clock();

    while(getline(infile, line)) {
        allLines.push_back(line);
    }

    

    /*istringstream iss(allLines.at(0));
    istringstream iss2(allLines.at(1));
    string s;

    string time = "";
    string distance = "";

    while(getline(iss, s, ' ')) {
        if(s != "" && s != "Time:") {
            time += s;
        }
    }

    while(getline(iss2, s, ' ')) {
        if(s != "" && s != "Distance:") {
            distance += s;
        }
    }

    Race r(stoll(time), stoll(distance));*/
     Race r(crappyLineToLongLong(allLines.at(0)),
     crappyLineToLongLong(allLines.at(1)));
     
    result = nbCombinaisonsToWin(r);

    end = clock();

    cout << "result : " << result << endl;
    cout << end - start << " ms to compute." << endl;

    cout << "Bye World !" << endl;
    return 0;
}