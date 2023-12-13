#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
using namespace std;

bool dichoExist(int winValToSeek, vector<int> hand, int profondeur, int pos) {

    int value = hand.at(pos);
    if(value == winValToSeek) {
        return true;
    }

    if(profondeur > 5 ) return false;

    int arrayProf[6] = {25, 13, 6, 3, 2, 1};
    int nextJump = arrayProf[profondeur];

    if(value > winValToSeek) {
        int newPos = pos - nextJump;
        if(newPos < 0 || newPos > hand.size() - 1) {
            return false;
        }
        return dichoExist(winValToSeek, hand, profondeur + 1, newPos);
    }
    int newPos = pos + nextJump;
    if(newPos < 0 || newPos > hand.size() - 1) {
        return false;
    }
    return dichoExist(winValToSeek, hand, profondeur + 1, newPos);
}

int computePoints(std::string line) {
    int result = 0;

    vector<int> winning;
    vector<int> hand;

    std::istringstream iss(line);
    string s;
    bool pipeFound = false;
    while(std::getline(iss, s, ' ')) {
        if(s == "|") pipeFound = true;
        else if(s != "Card" && s != " " && s != "" && s.find(':') == std::string::npos) {
            int value = stoi(s);
            if(!pipeFound) winning.push_back(value);
            else {
                int idx = 0;
                bool found = false;
                std::vector<int>::iterator it;
                it = hand.begin();
                for(int i = 0; i < hand.size() && !found; i++) {
                    if(value < hand.at(i)) {
                        idx = i;
                        found = true;
                    }
                }
                if(found) hand.insert(it + idx, value);
                else hand.push_back(value);
            }
        }
    }

    for(int wini = 0; wini < winning.size(); wini++) {
        int winValToSeek = winning.at(wini);
        if(dichoExist(winValToSeek, hand, 2, 12)) {
            if(result == 0) result = 1;
            else { result *= 2; }
        }
    }

    return result;
}



int main(char * args[]) {

    std::cout<< "Hello World !" << endl;

    std::ifstream infile("input.txt");
    std::string line;
    int result = 0;
    std::vector<std::string> allLines;
    while (std::getline(infile, line))
    {
        allLines.push_back(line);
    }

    for(int lineIdx = 0; lineIdx < allLines.size(); lineIdx++) {
        result += computePoints(allLines.at(lineIdx));
    }

    std::cout << "result : " << endl;
    std::cout << result << endl;

    std::cout << "Bye World !" << endl;
    return 0;
}