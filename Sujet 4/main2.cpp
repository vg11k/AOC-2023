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
            result++;
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

    std::vector<int> generatedCardsByCardIndex;
    std::vector<int> totalCardsSumByIndex;

    for(int lineIdx = 0; lineIdx < allLines.size(); lineIdx++) {
        generatedCardsByCardIndex.push_back(computePoints(allLines.at(lineIdx)));
        totalCardsSumByIndex.push_back(1);
    }

    for(int cardIndex = 0; cardIndex < generatedCardsByCardIndex.size(); cardIndex++) {
        int cardAtCurrentIndexGenerateAsMuch = generatedCardsByCardIndex.at(cardIndex);
        int occurenceCurrentIndex = totalCardsSumByIndex.at(cardIndex);
        for(int doublonIndex = 0; doublonIndex < occurenceCurrentIndex; doublonIndex++) {
            for(int generatedIndex = 0; generatedIndex < cardAtCurrentIndexGenerateAsMuch; generatedIndex++) {
                int indexOfGeneratedCard = cardIndex + generatedIndex + 1;
                int oldTotalCardsAt = totalCardsSumByIndex.at(indexOfGeneratedCard);
                totalCardsSumByIndex[indexOfGeneratedCard] = oldTotalCardsAt + 1;
            }
        }
    }

     for(int cardIndex = 0; cardIndex < totalCardsSumByIndex.size(); cardIndex++) {
        result += totalCardsSumByIndex.at(cardIndex);
     }


    std::cout << "result : " << endl;
    std::cout << result << endl;

    std::cout << "Bye World !" << endl;
    return 0;
}