#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
using namespace std;

string getTargetValuesFrom(string input) {

    string numberFirstCar = "";
    string numberLastCar = "";

    int numberStart = 100000000;
    int numberEnd = -1;

    for(int i = 0; i < input.length(); i++) {
        char c = input[i];
        if(c > 47 && c < 58) { //ascii values of numbers
            if(numberFirstCar == "") {
                numberFirstCar = c;
                numberLastCar = c;
                numberStart = i;
                numberEnd = i;
            }
            else {
                numberLastCar = c;
                numberEnd = i;
            }
        }
    }

    string digitFirstCar = "";
    string digitLastCar = "";

    int digitStart = 1000000000;
    int digitEnd = -1;

    string asDigits[9]  = { "one", "two", "three", "four", "five", "six", "seven", "eight", "nine" };
    
    for(int i = 0; i < input.length(); i++) {
        string substring = input.substr(i);
        for(int j = 0; j < 9; j++) {
            string digit = asDigits[j];
            int pos = substring.find(digit);
            if(pos != std::string::npos) {
                if(pos + i < digitStart) {
                    digitStart = pos + i;
                    digitEnd = digitStart;
                    digitFirstCar = std::to_string(j+1);
                    digitLastCar = digitFirstCar;
                }
                else if(digitFirstCar != "" && pos + i > digitEnd) {
                    digitEnd = pos + i;
                    digitLastCar = std::to_string(j+1);
                }
            }
        }
    }

    string output = "";
    if(digitStart < numberStart) {
        output += digitFirstCar;
    }
    else {
        output += numberFirstCar;
    }

    if(digitEnd > numberEnd) {
        output += digitLastCar;
    }
    else {
        output += numberLastCar;
    }

    return output;
}


int main(char * args[]) {
    cout << "Hello World ! " << endl;

    std::ifstream infile("input.txt");
    std::string line;
    int result = 0;

    while (std::getline(infile, line))
    {
        std::istringstream iss(line);
        string input;
        if (!(iss >> input)) { break; } // error
        string output = getTargetValuesFrom(input);
        cout << input << " ==> " << output << endl;
        result += std::stoi( output );
    }


    cout << "result : " << result << endl;

    cout << "Bye World" << endl;
    return 0;
}