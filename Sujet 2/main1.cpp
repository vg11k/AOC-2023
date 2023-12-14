#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
using namespace std;

bool isGameValid(string input) {

    int nbMaxGreen = 13;
    int nbMaxRed = 12;
    int nbMaxBlue = 14;

    string inputCleaned = input.substr(input.find(":") + 1);
    std::istringstream iss(inputCleaned);
    string oneSetString;

    while(std::getline(iss, oneSetString, ';')) {
        std::istringstream iss2(oneSetString);
        string oneColoredDiceString;
        std::vector<string> colors;
        std::vector<int> numbers;
        while(std::getline(iss2, oneColoredDiceString, ' ')) {
            if(oneColoredDiceString != "") {
                if(numbers.size() == colors.size()) {
                    numbers.push_back(stoi(oneColoredDiceString));
                }
                else {
                    int offset = 0;
                    if(oneColoredDiceString.find(",") != std::string::npos) {
                        offset = 1;
                    }
                    colors.push_back(oneColoredDiceString.substr(0,oneColoredDiceString.length() - offset));
                }
            }
        }

        for(int i = 0; i < colors.size(); i++) {
            string color = colors.at(i);
            int qt = numbers.at(i);
            if(color == "red" && nbMaxRed < qt) return false;
            else if(color == "green" && nbMaxGreen < qt) return false;
            else if(color == "blue" && nbMaxBlue < qt) return false;
        }
    }
    
    return true;
}

int main(char * args[]) {
    cout<< "Hello World !" << endl;

    std::ifstream infile("input.txt");
    std::string line;
    int result = 0;
    int it = 1;
    while (std::getline(infile, line))
    {
        std::istringstream iss(line);
        string input;
        if (!(iss >> input)) { break; } // error
        bool isValid = isGameValid(line);
        if(isValid) {
            cout << line << " is valid !" << endl;
            result+= it;
        }
        else {
            cout << line << " is not valid !" << endl;
        }
        it++;
    }

    cout << "result : " << endl << endl;
    cout << result << endl;

    cout << "Bye World !" << endl;
    return 0;
}